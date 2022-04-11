/* ==============================================================================
System Name:    ACI_PRM_MOT (float version)

File Name:      ACI_PRM_MOT.C

Description:    Измерение параметров асинхронного движка

Originator:     Triol

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 26-04-2011 Version 1.00
 21-03-2012 Version 2.00 
 16-10-2020 Version 3.00  PAN (float) 
=================================================================================  */

#include "LIB_PRM_MOT.h"
#include "Global_include.h"
#include "GlobalVar.h"
#include "ACI_prm_mot.h"
#include "PwmControl.h" 
#include "meas_lm_cur.h" 
#include "CurVoltMeas.h"
#include "Scalar_ORDINAR.h"
#include "DrvPWM.h"
#include "DrvADC.h"
#include "LIB_DMC.h"
#include "Pjk_const.h"
#include "Filter.h"
#include "math.h"


typedef enum{
    ACIPrmMotStateOff,         // Алгоритм отключен
    ACIPrmInitPwm,             // Настроить ШИМ
    ACIPrmLmNew0,              // Старт - измерение Rs
    ACIPrmLmNew1,              // Разряд Lm
    ACIPrmLmNew11,             // Заряд тока до номинала, для оценки Lms
    ACIPrmMotStateCalcLsLrRr2, // Измерение Rr & Lls
    ACIPrmLmNew2,              // Если есть измерение с вращением - разгон двигателя
    ACIPrmLmNew3,              // Измерение Lm  с вращением
    ACIPrmDeinitPwm,           // Вернуть частоту ШИМ назад      
    ACIPrmMotStateCalcLm1,     // Lm на низкой частоте - начало 
    ACIPrmMotStateCalcLm2,     // Lm на низкой частоте - измерение
    ACIPrmMotStateReady,       // Данные готовы, процесс завершен
}ACIPrmMotState_type;


#define  Kp     0.005f
#define  Ki     0.0005f

/*
#define TrOnOff      27.0f                    // 150 * 0.18f
#define TrOnOffLow   45.0f                    // 150 * 0.30f 	Малая мощность
#define TrOnOffSD    45.0f                    // 150 * 0.30f 	SD
*/
#define TrOnOff      32.4f                    // 150 * 0.18f
#define TrOnOffLow   54.0f                    // 150 * 0.30f 	Малая мощность
#define TrOnOffSD    54.0f                    // 150 * 0.30f 	SD

/*
#define  UonT     (1.0f + 0.8f)/2.0f          // 0.75v
#define  UonD     (1.2f + 0.9f)/2.0f          // 1.3v
#define  RdinT    (0.0094f + 0.014f)/2.0f     // 0.025ом
#define  RdinD    (0.0057f + 0.0094f)/2.0f    // 0.023ом
*/
#define  UonT     (1.0f + 0.8f)/2.0f          // 0.75v
#define  UonD     (1.2f + 0.9f)/2.0f          // 1.3v
#define  RdinT    (0.0094f + 0.014f)/2.0f     // 0.025ом
#define  RdinD    (0.0057f + 0.0094f)/2.0f    // 0.023ом


#define  DEAD_TIME_MEAS_PARAM   3.0f          // Мертвое время при измерениях параметров   
#define  F_PWM_MEAS_PARAM       4000.0f          // Частота ШИМ при измерениях параметров
#define  MEAS_PAUSE_TIMEOUT     1*F_PWM_MEAS_PARAM
#define  Tdiskr 0.020f // 0.050f                        // 50 ms - Время дискретизации при оценке Lm по экспоненте
#define  LOG_FILTER_TIME  0.04f  // Постоянная времени фильтра данных логарифма 40 мс

// Прототипы функций
u8 GetStatusPWM(void);         // Получить сотояние ШИМ (Вкл/Выкл)

// Дополнительные объекты
ACIPrmMotState_type  ACIPrmMotState   = ACIPrmMotStateOff;    // Флаги состояния процесса сбора и обработки данных
SCALARORDINAR        ScalarDC_ORDINAR = SCALARCONTROL_ORD;    // Скаляр для измерителя параметров

extern PWMCONTROL    PwmControl;    // Аппарат состояний ШИМ (Пуск/стоп только отсюда!!!)
extern PWMGEN        Pwm;           // Модуль ШИМ
extern CURVOLTMEAS   meas1;
extern CLARKE        clarke2;
extern PARK          park2;
extern ADCDRV        Adc;           // Модуль АЦП


void ACIM_PrmMot_Init()
{
    static float Fnom, Tmp1;
    if (!DrvWords.Ctrl.bit.SysCalc) return;
    if (!(DrvConst.DriveType == TYPE_ACIM_FOC_SENSORED ||
        DrvConst.DriveType == TYPE_ACIM_FOC_SENSORLESS || 
        DrvConst.DriveType == TYPE_SCALAR ||
        DrvConst.DriveType == TYPE_SCALAR_NO_DEADBAND )) return;

    Tmp1 = 10.0f / dPnt->FreqBASE;
    if(dPnt->Fnom >= Tmp1)
        Fnom = dPnt->Fnom;
    else 
        Fnom = Tmp1;
    
    //  Инициализация переменных для подстройки напряжения 
    cntrvolt1.Imax      = dPnt->Isn;   // Максимальный ток подаваемый на электродвигатель
    cntrvolt1.Umax      = 1.0f;        // Максимальная напруга
    // Шаг настройки напряжения
    // Скорость наростания напряжения Usn/TUsn_MMP = [В/с]
    UlineStep = (_2SQRT2_SQRT3 * dPnt->Usn) / (dPnt->TUsn_MMP * Fnom* dPnt->FreqBASE);
     
#warning Надо блок дедтайма инициализировать в зависимости от частоты ШИМа через общий метод. Иначе будут мешать друг другу
//    //  Инициализация переменных для компенсации эффекта мёртвого времени
//    dtc1.dT = DEAD_TIME_MEAS_PARAM * F_PWM_MEAS_PARAM * 0.000001f;  // МВ в относительных единицах +
//    dtc1.Rsw_2 = 2.0f * dPnt->Rsw * dPnt->CurrBASE / dPnt->VoltBASE;            // двойное сопротивление ключа
//    dtc1.dI = FLOAT_to_IQ(dPnt->dI, Current_base);                              // интервал неопределённости около 0 А
//    dtc1.Udf = FLOAT_to_IQ(dPnt->Udf, Volt_base);
    
    //  Инициализация постоянных для определения Rs
    measRs1.K1 = dPnt->VoltBASE * 2.0f / (dPnt->CurrBASE * 3.0f);
   
    //  Инициализация постоянных для определения sigmaLs и Rs
    measRP2.K1 = dPnt->VoltBASE / (3.0f * dPnt->CurrBASE * dPnt->FreqBASE);
    measRP2.K2 = dPnt->VoltBASE * 2.0f / (dPnt->CurrBASE * 3.0f)/_SQRT_2; // 3/2??  dPnt->VoltBASE * 2.0f / (dPnt->CurrBASE * 3.0f);

    //  Инициализация постоянных для определения Lm                         
    measLm2.K1 = dPnt->VoltBASE*2.0/(3.0*dPnt->CurrBASE*dPnt->FreqBASE); 
    measLm2.K2 = measRs1.K1;
        
    //  Инициализация генератора темпа
    rcg1.StepAngleMax = (dPnt->FreqBASE * _2PI) / F_PWM_MEAS_PARAM; // +
    rcg1.StepFreqMax  = 1.0f;
    
    rcg2.StepAngleMax = (dPnt->FreqBASE * _2PI) / F_PWM_MEAS_PARAM; //_IQ(dPnt->FreqBASE / dPnt->Fpwm_MMP);
    rcg2.StepFreqMax  = 1.0f;
    
    
    //  Инициализация частоты измерения
    FreqRef1.LsLineFreq = Fnom;
    FreqRef1.LsCyclFreq = Fnom * _2PI;
    FreqRef1.LmLineFreq = 0.1f / dPnt->FreqBASE;
    FreqRef1.LmCyclFreq = FreqRef1.LmLineFreq * _2PI;

    //  Инициализация времени измерения опытов 
    //                      Интервал, Сек   Частота ШИМ, Гц
    prmTimes1.Init      = (u32)(1.0f  * F_PWM_MEAS_PARAM);//+
    prmTimes1.Start     = (u32)(8.0f  * F_PWM_MEAS_PARAM);//+
    prmTimes1.RsCalc1   = (u32)(4.0f  * F_PWM_MEAS_PARAM);//+
    prmTimes1.RsCalc2   = (u32)(15.0f * F_PWM_MEAS_PARAM);//+
    prmTimes1.LsCalc1   = (u32)(15.0f * F_PWM_MEAS_PARAM);//+
    prmTimes1.LmCalc1   = (u32)(1.0f  * F_PWM_MEAS_PARAM);//+
    prmTimes1.LmCalc2   = (u32)(15.0f * F_PWM_MEAS_PARAM);//+ 
}                        

float IqRr, IdRr;
float SqrSumF64;
float CntF32;

	// Таблица токов ХХ для разных типов двигателейб сдвижка по таблице 7
f32 NoLoadCurrTab[8*7]={ // Мощность, p=1, p=2,  p=3,  p=4,  p=5,  p=6
  0.55,   0.50, 0.65, 0.75, 0.80, 0.85, 0.90,
  1.50,   0.40, 0.60, 0.65, 0.70, 0.75, 0.80,
  5.50,   0.35, 0.55, 0.60, 0.65, 0.70, 0.75,
  11,     0.30, 0.50, 0.55, 0.60, 0.65, 0.70,
  22.5,   0.25, 0.45, 0.50, 0.55, 0.60, 0.65,
  55,     0.20, 0.40, 0.45, 0.50, 0.55, 0.60,
  110,    0.15, 0.30, 0.35, 0.40, 0.45, 0.50,
  500,    0.10, 0.25, 0.30, 0.35, 0.40, 0.45, // Прикидка на мои соображения-в таблице данных нету.
};
// Работа в звезде, требует понижения числа пар полюсов на 1 - проверить.
// Треугольники стыкуются хорошо.

//f32     IfazSampleBuf[100] = {0};

void ACIM_PrmMot_Calc()
{
    static float Tmp1;
    static s16 TmpU = 0, Urs = 0;
    static s32 TempFiltr = 0;
    static u32 time = 0, LogTimer = 0, LogTimeOff=0;
    static s32 timer = 0;
    static u16 WorkEnbOld = 0;
    static u32 Cnt = 0, UsclPidOffCnt = 0, RepeatCntLm11 = 0;
    static u32 PauseTimer = 0;
    static f32 Tlm0, Tlm1, Tlm2, Tlm3, Wout;
    static f32 dUscl = 0, dFscl = 0, LastUsclRs = 0, LastIsRs = 0, IsMeanTdiskr = 0, IsMeanCnt = 0;
    static f32 RefFiltr_K11=0, RefFiltr_K21=0, dTsLmCnt = 0, LegalTs = 0;
    static f32 ResultTs[3]={0};
    f32         dTsLm;
    f32         locTpwm;
    
    //f32         Mnmot, MkestNom, snom, I2nomEst, InoLoadEst1, InoLoadEst2, InoLoadEst3, InoLoadEstMean, MeanLogCnt; // skestNom
    s16         Indx;
    f32	        IxxEst, EstZ, EstPnom;
    f32         Iuv, Uuv, U1, U2;
    f32         Kb, LocalPhirNom;    
    u16		tabIndex, findIxxNumber;
    f32         tAcsl;
    
    // 1) Проверить может у нас скаляр, тогда мерять параметры нет смысла
    if (!(DrvConst.DriveType == TYPE_ACIM_FOC_SENSORED ||
        DrvConst.DriveType == TYPE_ACIM_FOC_SENSORLESS || 
        DrvConst.DriveType == TYPE_SCALAR ||
        DrvConst.DriveType == TYPE_SCALAR_NO_DEADBAND )) return;
        
    // 2) Обработка вкл/выкл измерения параметров
    if((DrvWords.Ctrl.bit.mWorkEnb == 1)&&(WorkEnbOld == 0)){  // Запрос на включение измерения
        ACIPrmMotState = ACIPrmInitPwm;                        // Запустить измерение параметров двигателя
        DrvWords.Ctrl.bit.vCodeEnb = 0;                        // Заблокировать вызов обработчика системы регулирования 
        DrvWords.Stat.bit.mWorkFin = 0;                        // При начале измерений сбросить флаг завершения змирений  
    }
    else if(((DrvWords.Ctrl.bit.mWorkEnb == 0)&&(WorkEnbOld == 1)) || (DrvWords.Ctrl.bit.FailOn && (ACIPrmMotState != ACIPrmMotStateOff))){ // Запрос на выключение измерения
        ACIPrmMotState = ACIPrmDeinitPwm;                      // Переход на перенастройку ШИМ по уставкам
    }    
      
    WorkEnbOld = DrvWords.Ctrl.bit.mWorkEnb;
 
//    GlobalM4.ACIPrmMotState = ACIPrmMotState;
//    GlobalM4.PrmTimer = timer;
//    GlobalM4.StatusPwm = PwmControl.StatusPwm;
    
//    if (!DrvWords.Ctrl.bit.mWorkEnb){
//      timer = 0;
//      ACIPrmMotState = ACIPrmMotStateOff;
//      return;
//    }

    DrvWords.Stat.bit.mExpNum = 0x1F & ACIPrmMotState; // фильтрация первых 5 бит

    //GlobalM4.StatusPwm = PwmControl.StatusPwm;
    
// Аппарат состояний измерителя параметров двигателя    
    switch(ACIPrmMotState) {

        case ACIPrmInitPwm:{
           // Перенастроить модуль ШИМ (что бы всегда при измирениях была одна частота)
           Pwm.DeadTime_uS = DEAD_TIME_MEAS_PARAM; 
           Pwm.Fpwm_kHz = F_PWM_MEAS_PARAM*0.001f; // Перевод в кГц для перезапуска частоты ШИМа
           Pwm.Fpwm_Hz = F_PWM_MEAS_PARAM;         // Перевод в Гц для перезапуска частоты ШИМа
           Pwm.ReInit(&Pwm);
             
           // Синхронизировать АЦП с периодом ШИМ
           Adc.Fpwm_kHz = Pwm.Fpwm_kHz;               // Получить частоту ШИМ модуля PWM (для синхронизации АЦП с ШИМ)
           Adc.ReInit(&Adc);
        
           // Переход на измерение параметров (Rs + накачка для Lm)           
           ACIPrmMotState = ACIPrmLmNew0;
           //ACIPrmMotState = ACIPrmMotStateCalcLsLrRr2;
           
           timer = 0;
           cntrvolt1.reset(&cntrvolt1);
           rcg1.reset(&rcg1);           
        } break;
        
//------------------------------------------------------------------------------        
        case ACIPrmLmNew0:{                           // Измерение Rs + накачка для Lm
    	   cntrvolt1.Ifaz   = f32_Filtr(cntrvolt1.Ifaz, fabsf(dPnt->IsU), 0.0625f);	// Ток с фильтром
    	   cntrvolt1.f32IsU = cntrvolt1.Ifaz * dPnt->CurrBASE;                          // Абсолютное значение тока
    	   cntrvolt1.s16IsU = (s16) (cntrvolt1.f32IsU * 100.0f);
    	   
           if(GetStatusPWM() == 0) {
    	      DriveInit(dPnt);

    	      dPnt->Us = 0.0f;
              cntrvolt1.Imax = dPnt->Isn;                 // номин ток
              timer = 10 * (u32)F_PWM_MEAS_PARAM;  // 10 sec; + 
              UsclPidOffCnt = 2 * (u32)F_PWM_MEAS_PARAM; 
            
              Pwm.Ta = 0;
              Pwm.Tb = 0;
              Pwm.Tc = 0;
              Pwm.Update(&Pwm);
            
    	      PwmControl.PwmOn(&PwmControl);     // Вкл ШИМ

              // Иниц фильтра
              Iuv = cntrvolt1.f32IsU;

              U1 = TrOnOff;
              if(Iuv < 8.0f) U1 = TrOnOffLow;	// Ток < 8A
              if(Iuv >= 50.0f) U1 = TrOnOffSD;	// Ток > 50A

              Uuv = (GlobalM4.ValUd.FlashVal - 2 * UonT - 1.5f * Iuv * RdinT) * ((fabsf( (float)(Pwm.UvalTimer - Pwm.VvalTimer) ) - (Pwm.DeadTime_uS * SystemCoreClock / 1000000.0f ) + U1) / (float)Pwm.MaxCountT1);            // Пока так - Pwm.DeadTime_uS + U1) / (f32)EPwm2Regs.TBPRD;
              U1  = ((UonD + UonT) + Iuv * (RdinD + 0.5f * RdinT)) * ((((float)Pwm.MaxCountT1 - (float)(Pwm.UvalTimer) + (Pwm.DeadTime_uS * SystemCoreClock / 1000000.0f )) / (float)Pwm.MaxCountT1));                                            // Пока так - (f32)EPwm2Regs.TBPRD - (f32) EPwm2Regs.CMPA.half.CMPA + Pwm.DeadTime_uS) / (f32)EPwm2Regs.TBPRD;             
//              Uuv = (GlobalM4.ValUd.FlashVal - 2 * UonT - 1.5f * Iuv * RdinT) * ((fabsf( (float)(Pwm.UvalTimer - Pwm.VvalTimer) ) - (Pwm.DeadTime_uS * FREQ_APB2 / 1000.0f ) + U1) / (float)Pwm.MaxCountT1);            // Пока так - Pwm.DeadTime_uS + U1) / (f32)EPwm2Regs.TBPRD;
//              U1  = ((UonD + UonT) + Iuv * (RdinD + 0.5f * RdinT)) * ((((float)Pwm.MaxCountT1 - (float)(Pwm.UvalTimer) + (Pwm.DeadTime_uS * FREQ_APB2 / 1000.0f )) / (float)Pwm.MaxCountT1));                                            // Пока так - (f32)EPwm2Regs.TBPRD - (f32) EPwm2Regs.CMPA.half.CMPA + Pwm.DeadTime_uS) / (f32)EPwm2Regs.TBPRD;
              U2  = ((UonD + UonT) + Iuv * (RdinT + 0.5f * RdinD)) * 0.5f;
              TempFiltr = (s32)((s16)((Uuv - U1 -U2) * 10.0f)) << 16;
    	   }
    	   else{
              Tmp1 = cntrvolt1.Imax - fabsf(dPnt->IsU);
              if (Tmp1*100.0 / cntrvolt1.Imax < 0.03f && !UsclPidOffCnt) {                
                Tmp1 = LastUsclRs; // Если достигли тока - отключаем регулятор
              } else {                
                  if (Tmp1*100.0 / cntrvolt1.Imax < 0.03f) UsclPidOffCnt --;
                  
                  dPnt->Us += Tmp1 * Ki;
                  if(dPnt->Us > 1.0f) dPnt->Us = 1.0f;
                  if(dPnt->Us < 0.0f) dPnt->Us = 0.0f;
                  Tmp1 = (Tmp1 * Kp) + dPnt->Us;
                  if(Tmp1 > 1.0f) Tmp1 = 1.0f;
                  if(Tmp1 < 0.0f) Tmp1 = 0.0f;
              }
              ScalarDC_ORDINAR.Angle     = _PI;
              ScalarDC_ORDINAR.Udc       = dPnt->Udc;
              ScalarDC_ORDINAR.Uscl      = Tmp1; 
              ScalarDC_ORDINAR.Fscl      = 0;
              ScalarDC_ORDINAR.StepAngle = 0;
              ScalarDC_ORDINAR.calc(&ScalarDC_ORDINAR);
              LastUsclRs = Tmp1; // !!! Текущее значение выходного напряжения, запоминаем в LastUsclRs для последующей подачи на двигатель
              
              //--------------------------------------------
              // Расчет напряжения на выходе
              //--------------------------------------------
              // UonT  - падение напряжения при открытии транзистора  [0.7; 0.8v]			[1.1v]	SD
              // UonD  - падение напряжения при открытии диода        [0.9; 1.5v]			[1.1v]	SD
              // RdinT - Rдин.транзистора                             [0.021; 0.032ом]	[0.0025 / 2 ом]	SD
              // RdinD - Rдин.диода                                   [0.018; 0.028ом]	[0.0021 / 2 ом]	SD
              // Uuv = (Ud - 2 * UonT - I * 1.5 RdinT) * (Tu - Tv - Tdead + TrOnOff) / Tшим -
              //     - [(UonD + UonT) + I * (RdinD + RdinT / 2)] * (Tшим / 2 - Tu + Tdead) / Tшим -
              //     - [(UonD + UonT) + I * (RdinT + RdinD / 2)] * (Tшим / 2  + Tdead) / Tшим;
              Iuv = cntrvolt1.f32IsU;
              LastIsRs  = f32_Filtr(LastIsRs, cntrvolt1.Ifaz, (1.0f/128.0f)); //cntrvolt1.Ifaz;

              U1 = TrOnOff;
              if(Iuv < 8.0f) U1 = TrOnOffLow;		                    // Ток < 8A
              if(Iuv >= 50.0f) U1 = TrOnOffSD;	                            // Ток > 50A  SD
              if(GlobalM4.ValUd.FlashVal > 900.0f) U1 = TrOnOffSD * 1.35f;  // SD 690 V

              Uuv = (GlobalM4.ValUd.FlashVal - 2 * UonT - 1.5f * Iuv * RdinT) * ((fabsf( (float)(Pwm.UvalTimer - Pwm.VvalTimer) ) - (Pwm.DeadTime_uS * SystemCoreClock / 1000000.0f ) + U1) / (float)Pwm.MaxCountT1);            // Пока так - Pwm.DeadTime_uS + U1) / (f32)EPwm2Regs.TBPRD;
              U1  = ((UonD + UonT) + Iuv * (RdinD + 0.5f * RdinT)) * ((((float)Pwm.MaxCountT1 - (float)(Pwm.UvalTimer) + (Pwm.DeadTime_uS * SystemCoreClock / 1000000.0f )) / (float)Pwm.MaxCountT1));                                            // Пока так - (f32)EPwm2Regs.TBPRD - (f32) EPwm2Regs.CMPA.half.CMPA + Pwm.DeadTime_uS) / (f32)EPwm2Regs.TBPRD;              
//              Uuv = (GlobalM4.ValUd.FlashVal - 2 * UonT - 1.5f * Iuv * RdinT) * ((fabsf( (float)(Pwm.UvalTimer - Pwm.VvalTimer) ) - (Pwm.DeadTime_uS * FREQ_APB2 / 1000.0f ) + U1) / (float)Pwm.MaxCountT1);            // Пока так - Pwm.DeadTime_uS + U1) / (f32)EPwm2Regs.TBPRD;
//              U1  = ((UonD + UonT) + Iuv * (RdinD + 0.5f * RdinT)) * ((((float)Pwm.MaxCountT1 - (float)(Pwm.UvalTimer) + (Pwm.DeadTime_uS * FREQ_APB2 / 1000.0f )) / (float)Pwm.MaxCountT1));                                            // Пока так - (f32)EPwm2Regs.TBPRD - (f32) EPwm2Regs.CMPA.half.CMPA + Pwm.DeadTime_uS) / (f32)EPwm2Regs.TBPRD;
              U2  = ((UonD + UonT) + Iuv * (RdinT + 0.5f * RdinD)) * 0.5f;
              TmpU = (s16)((Uuv - U1 - U2) * 10.0f);
              Urs = TmpU;
              cntrvolt1.s16Lm1 = (s16)((U1) * 10.0f);
              cntrvolt1.s16Lm2 = (s16)((U2) * 10.0f);
              
              s16_Filtr(Urs, TmpU, TempFiltr, 4);	//Ток с фильтром
   
              timer--;
              if(timer == 0 || timer < 0){		// 5 sec
                  Tmp1 = cntrvolt1.Imax - fabsf(dPnt->IsU);
                  if(fabsf(Tmp1) > 0.05)                        // ошибка измерения параметров
                      GlobalM4.FailData.FailsFlag.bit.ErrMeasPrm = 1;
                     
                  // Проверяем, если ток менее 10% от номинала, то проблема. Вероятно есть обрыв фазы
                  if (fabsf(dPnt->IsU) < 0.1f*cntrvolt1.Imax || fabsf(dPnt->IsV) < 0.1f*cntrvolt1.Imax || fabsf(dPnt->IsW) < 0.1f*cntrvolt1.Imax) {
                    GlobalM4.FailData.FailsFlag.bit.MeasPrmBrkPhase = 1;
                    GlobalM4.FailData.FailsFlag.bit.ErrMeasPrm = 0; // Если выявлен обрыв - то сбросить аварию ошибки измерения
                  }
                 // Определяем Rs
	         dPnt->Rs = (f32)Urs / cntrvolt1.f32IsU * 0.066666666666f;
                 GlobalM4.EnginParam.MeasRs = dPnt->Rs;
                
                 // Закрыть нижние ключи
                 Pwm.Ta = 1.0f;
                 Pwm.Tb = 1.0f;
                 Pwm.Tc = 1.0f;
                 Pwm.Update(&Pwm);
                 
		 cntrvolt1.Imax = 0.07f * dPnt->Isn;		                // Токи до 5.0% Iном
		 Cnt = (u32) (Tdiskr * (u32)F_PWM_MEAS_PARAM);   	        // Шаг дискретизации +
		 timer = Cnt;							// Чтобы при запуске вычислить
		 time = (u32)F_PWM_MEAS_PARAM;				        // Защита по времени 1 сек в тактах ШИМа
		 
                 TempFiltr = 0;			                                // Фильтр
		
                 cntrvolt1.Log1 = cntrvolt1.Log0 = cntrvolt1.LogN = Tlm0 = Tlm1 = Tlm2 = Tlm3 = cntrvolt1.Lm0 = 0;
                 ACIPrmMotState = ACIPrmLmNew1;
    	      }
    	   }
    	}break;
         
//--------------------------------------------------------------
// Обработка экспоненты до спада токов до  A
//------------------------------------------------------------------------
        case ACIPrmLmNew1:{
           time--;			//Время разряда
           timer++;
           cntrvolt1.Ifaz   = f32_Filtr(cntrvolt1.Ifaz, fabsf(dPnt->IsU), 0.0625f/*0.0078125f*//*0.015625f*/);	//Ток с фильтром
           cntrvolt1.s16III = (s16) fabsf(dPnt->IsU * dPnt->CurrBASE * 1000.0f);

           cntrvolt1.f32IsU = cntrvolt1.Ifaz * dPnt->CurrBASE;
           cntrvolt1.s16IsU = (s16) (cntrvolt1.f32IsU * 100.0f);
           Indx = (s16) (dPnt->Isn * dPnt->CurrBASE);                // номин ток;
           if(Indx > 24) Indx = 24;

           cntrvolt1.LogN = logf(cntrvolt1.f32IsU);// * CurrTab[Indx]; // Берем логарифм от тока с коррекцией по таблице токов
           cntrvolt1.s16LogN = (s16) (cntrvolt1.LogN * 1000.0f);
            
           // Дискретизация (прореживание)
           if(timer >= Cnt){		// Для Fpwm
               cntrvolt1.Log1 = cntrvolt1.Log0;
               cntrvolt1.Log0 = cntrvolt1.LogN;
               
//               if ((cntrvolt1.Log1 - cntrvolt1.Log0) > 0) {
               Tlm3 = Tlm2;
               Tlm2 = Tlm1;
               Tlm1 = Tlm0;
               Tlm0 = cntrvolt1.Lm0;
               cntrvolt1.Lm0 = Tdiskr / (cntrvolt1.Log1 - cntrvolt1.Log0);
               cntrvolt1.s16Lm0 = (s16) (cntrvolt1.Lm0 * 1000.0f);               
//               }
                              
               timer= 0;
           }
 
           if(time > 0){ 					           // 1sec  !!! Защита от недостижимого 5% Inom !!!
               if(cntrvolt1.Ifaz > cntrvolt1.Imax){ 
                   break;	                                           // Анализ завершения
               }
           }
        
           // Ух, все!
           dPnt->Lsq = (Tlm3 + Tlm2 + Tlm1 + Tlm0 + cntrvolt1.Lm0) / 5.0f; // Пока сохраняем пост времени !!!           
           GlobalM4.EnginParam.MeasLsq = dPnt->Lsq;

//           dPnt->Lsd = (Tlm3 + Tlm2 + Tlm1 + Tlm0 + cntrvolt1.Lm0) / 5.0f; // Пока сохраняем пост времени !!!           
//           GlobalM4.EnginParam.MeasLsd = dPnt->Lsd;
                      
           PwmControl.PwmOff(&PwmControl);                                 // Выкл ШИМ

           timer = 0;
           dPnt->Us = 0;
           cntrvolt1.reset(&cntrvolt1);
           rcg1.reset(&rcg1);
           RepeatCntLm11 = 3;// Повторяем три раза
           ResultTs[0]=ResultTs[1]=ResultTs[2]=0;
           PauseTimer = MEAS_PAUSE_TIMEOUT; // Заряжаем задержку перед следующим измерением

           // Определяем по какому алгоритму идем дальше
           switch (GlobalM4.MeasureLmRs) {
           case MEASURE_NO_ROTAT_LM_MEAS: // Измерение Lm без вращения       
             ACIPrmMotState = ACIPrmMotStateCalcLm1; // ACIPrmLmNew11; 
             break;
            
           case MEASURE_NO_ROTAT_LM_ESTIM: // Для всех остальных режимов переходим на
           case MEASURE_ROTATION_LM_MEAS:  // измерение Rr
           default:
             ACIPrmMotState = ACIPrmMotStateCalcLsLrRr2;             
             break;
           }           
           
           
//           GlobalM4.Debug6 = 0; // !*!*!*!*                           
        } break;

//--------------------------------------------------------------
// Обработка экспоненты по возрастанию токов номинального уровня
//------------------------------------------------------------------------
        case ACIPrmLmNew11:{
            if (PauseTimer){ // Задержка перед началом измерения, чтобы АЦП смог захватить новый ноль 
                PauseTimer--;
                //GlobalM4.Debug6 = 0; // !*!*!*!*                
                break;
            } 
              
           //GlobalM4.Debug6 = 1; // !*!*!*!*!*
           
           if(GetStatusPWM() == 0) {                         
             // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    	      DriveInit(dPnt);  // !!!!! Может здесь борода при смене частоты ШИМа !!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
             // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!              
              
    	      dPnt->Us = 0.0f;
              cntrvolt1.Imax = LastIsRs; //dPnt->Isn;                 // номин ток              
              timer = 5 * (u32)(F_PWM_MEAS_PARAM);         // 10 sec; Максимальное время анализа
              LogTimeOff = (u32)(5.0f*F_PWM_MEAS_PARAM);
//		 cntrvolt1.Imax = 0.07f * dPnt->Isn;		                // Токи до 5.0% Iном
              Cnt = (u32) (Tdiskr/*GlobalM4.Debug4*/ * (u32)F_PWM_MEAS_PARAM);   	        // Шаг дискретизации
              LogTimer = Cnt;                                               // Зарядить таймер дискретизации, чтобы сразу начать расчеты	 		
              //MeanLogCnt = 0;
              cntrvolt1.Log1 = cntrvolt1.Log0 = cntrvolt1.Log4 = cntrvolt1.LogN = Tlm0 = Tlm1 = Tlm2 = Tlm3 = cntrvolt1.Lm0 = cntrvolt1.Lm1 = cntrvolt1.Lm2 = 0; // Обнулить переменные оценки.
              cntrvolt1.s16Lm = 0;
              dTsLmCnt = 0;
              
              //memset(&IfazSampleBuf, 0.0f, sizeof(IfazSampleBuf));
              IsMeanTdiskr = 0; IsMeanCnt = 0; // Для расчета среднего тока за время квантования расчета производной логарифма
              
              Pwm.Ta = 0;
              Pwm.Tb = 0;
              Pwm.Tc = 0;
              Pwm.Update(&Pwm);
            
              // Текущий период ШИМа в соответствии с заданной частотой при измерении
              locTpwm = (f32)(1.0f/F_PWM_MEAS_PARAM);              
              RefFiltr_K11 = locTpwm / (LOG_FILTER_TIME + locTpwm);
              RefFiltr_K21 = LOG_FILTER_TIME/ (LOG_FILTER_TIME + locTpwm);
                            
    	      PwmControl.PwmOn(&PwmControl);     // Вкл ШИМ
    	   }
    	   else{
              ScalarDC_ORDINAR.Angle     = _PI;
              ScalarDC_ORDINAR.Udc       = dPnt->Udc;
              ScalarDC_ORDINAR.Uscl      = dPnt->Us = LastUsclRs; // Задаем напряжение из прошлого такта подъема напряжения для Rs
              ScalarDC_ORDINAR.Fscl      = 0;
              ScalarDC_ORDINAR.StepAngle = 0;
              ScalarDC_ORDINAR.calc(&ScalarDC_ORDINAR);

              // Текущее значение тока, только перевернутое относительно своего конечного значения
              IsMeanTdiskr += fabsf(dPnt->IsU); // Накапливаем ток для усредения за время расчета производной логарифма
              IsMeanCnt ++; // Счетчик увеличить
              
              // Расчет логарифма по мгновенному току
              cntrvolt1.Ifaz   = f32_Filtr(cntrvolt1.Ifaz, fabsf(dPnt->IsU), 1);	// Ток с фильтром
              cntrvolt1.Ifaz = cntrvolt1.Imax - cntrvolt1.Ifaz; // Переворачиваем ток, чтобы он стремился к нулю (виртуально)
              if (cntrvolt1.Ifaz < 0) cntrvolt1.Ifaz = 0; // Отрезаем отрицательный ток, чтоб не глючило логарифм
              cntrvolt1.f32IsU = cntrvolt1.Ifaz * dPnt->CurrBASE; // Абсолютное значение тока
              
              cntrvolt1.LogN = logf(cntrvolt1.f32IsU); // Берем логарифм от абсолютного значения тока
              cntrvolt1.Log4 = (cntrvolt1.LogN * RefFiltr_K11) + (cntrvolt1.Log4 * RefFiltr_K21); // Фильтруем логарифм через ФНЧ
              
              cntrvolt1.s16Lm = f32_Filtr(cntrvolt1.s16Lm, cntrvolt1.Ifaz, (1.0f/128.0f)); // Долгий тренд тока
              
              if (LogTimeOff) LogTimeOff--;  // Таймер остановки расчета Ts, через заданное время. Оно меньше чем общее время наблюдения. ВОзможно убрать потом.
                
              if ((++LogTimer) >= Cnt){  // Таймер квантования расчета Tdiskr (10...50 ms)
                IsMeanTdiskr = IsMeanTdiskr / IsMeanCnt; // Среднее значение тока за время усреднения Tdiskr
                cntrvolt1.s16III = IsMeanTdiskr; // Ток усреденный за Tdiskr
                
                cntrvolt1.s16LogN = (cntrvolt1.Imax - cntrvolt1.s16III) * dPnt->CurrBASE; // Логарифм от усредненного тока с Tdiskr
                
//                // Если ток совсем маленький - прерываем измерение
//                if (((cntrvolt1.Imax - cntrvolt1.s16III)/cntrvolt1.Imax) < 0.02) {
//                  timer = 1; // Анализ прерываем досрочно
//                }
                  
                cntrvolt1.s16III = IsMeanTdiskr * dPnt->CurrBASE; 
                cntrvolt1.s16Lm0 = logf(cntrvolt1.s16LogN); // Логарифм от усредненного тока с Tdiskr
                
                cntrvolt1.Log1 = cntrvolt1.Log0;
                cntrvolt1.Log0 = cntrvolt1.Log4; // Берем логарифм из ФНЧ
                
                Tlm3 = Tlm2;
                Tlm2 = Tlm1;
                Tlm1 = Tlm0;
                Tlm0 = cntrvolt1.Lm0;
                cntrvolt1.Lm0 = Tdiskr/*GlobalM4.Debug4*/ / (cntrvolt1.Log1 - cntrvolt1.Log0);

                LogTimer = 0;  

                if (LogTimeOff) {
                    dTsLm = cntrvolt1.Lm2; // Запомнить старое значение Ts(i-1)
                    dPnt->Lsd = (Tlm3 + Tlm2 + Tlm1 + Tlm0 + cntrvolt1.Lm0) / 5.0f; // Находим среднее Ts за 5 тактов                              
                    //dTsLm = dTsLm - cntrvolt1.Lm2; // Приращение Ts                    
                    GlobalM4.EnginParam.MeasLsd = dPnt->Lsd;
                    cntrvolt1.Lm1 = dPnt->Lsd;  
                    cntrvolt1.Lm2   = f32_Filtr(cntrvolt1.Lm2, dPnt->Lsd, 0.125f);
                    dTsLm = dTsLm - cntrvolt1.Lm2; // Приращение Ts
                    cntrvolt1.s16Lm1 = fabsf(dTsLm / cntrvolt1.Lm2 * 100);
                    
                    if (cntrvolt1.s16Lm1 < 2.0f) {  // Оценка отклонения для фиксации результата                    
                       dTsLmCnt ++;
                       if (dTsLmCnt > 4) {
                         LegalTs = cntrvolt1.Lm2;
                         dTsLmCnt = 100;    
                       }
                    } else if (dTsLmCnt != 100){
                      dTsLmCnt = 0;
                    }                                       
                }                
                cntrvolt1.s16Lm2 = dTsLmCnt;
                IsMeanTdiskr = IsMeanCnt = 0;
              }
                            
              timer--;
              if(timer == 0 || timer < 0){		// 5 sec
                  Tmp1 = cntrvolt1.Imax - fabsf(dPnt->IsU);
                  if(fabsf(Tmp1) > 0.05)                        // ошибка измерения параметров
                      GlobalM4.FailData.FailsFlag.bit.ErrMeasPrm = 1;
                     
                  // Проверяем, если ток менее 10% от номинала, то проблема. Вероятно есть обрыв фазы
                  if (fabsf(dPnt->IsU) < 0.1f*cntrvolt1.Imax || fabsf(dPnt->IsV) < 0.1f*cntrvolt1.Imax || fabsf(dPnt->IsW) < 0.1f*cntrvolt1.Imax) {
                    GlobalM4.FailData.FailsFlag.bit.MeasPrmBrkPhase = 1;
                    GlobalM4.FailData.FailsFlag.bit.ErrMeasPrm = 0; // Если выявлен обрыв - то сбросить аварию ошибки измерения
                  }
                
                  PwmControl.PwmOff(&PwmControl);                                 // Выкл ШИМ            
                  timer = 0;
                  dPnt->Us = 0;
                  LastIsRs = cntrvolt1.s16III/dPnt->CurrBASE; 
                  cntrvolt1.reset(&cntrvolt1);
                  rcg1.reset(&rcg1);                  
                  PauseTimer = MEAS_PAUSE_TIMEOUT; // Заряжаем задержку перед следующим измерением                  
                 
                  // Нормальное измерение
                  if (dTsLmCnt != 100) { // Надо ли повторять
                    PauseTimer = MEAS_PAUSE_TIMEOUT; // Заряжаем задержку перед следующим измерением
                    break;
                  } else {                  
                    RepeatCntLm11 --; // Уменьшаем счетчик
                    // Нормальный замер
                    if (dTsLmCnt == 100) { // Заносим в массив
                      ResultTs[RepeatCntLm11] = LegalTs;
                    }
                    if (RepeatCntLm11) {
                      PauseTimer = MEAS_PAUSE_TIMEOUT; // Заряжаем задержку перед следующим измерением
                      break;
                    }                    
                  }
                 
                 // Усредняем массив
                 LegalTs = (ResultTs[0] + ResultTs[1] + ResultTs[2]) / 3;
                 GlobalM4.EnginParam.MeasLsd = dPnt->Lsd = LegalTs;
                 ACIPrmMotState = ACIPrmMotStateCalcLsLrRr2;// ACIPrmMotStateReady;
    	      }
    	   }
                    
        } break;        
        
//------------------------------------------------------------------------------        
        case ACIPrmMotStateCalcLsLrRr2:{
            if (PauseTimer){ // Задержка перед началом измерения, чтобы АЦП смог захватить новый ноль 
                PauseTimer--;
                break;
            }
            
            // The Ramp Generation 
            rcg1.Fwork = FreqRef1.LsLineFreq;
            rcg1.Fnom  = FreqRef1.LsLineFreq;
            rcg1.calc(&rcg1);
            dPnt->Angle   = rcg1.Angle;            

            // Connect inputs of the PARK module and call the park transformation calculation function.
            park2.Alpha = dPnt->IsU;
            park2.Beta =  0.0f; 
            park2.Angle = rcg1.Angle;
            park2.calc(&park2);

            IqRr  = f32_Filtr(IqRr, park2.Qs, 0.000244140625f); // 1/4096
            IdRr  = f32_Filtr(IdRr, park2.Ds, 0.000244140625f);

            // Current RMS calculation
            Iuv = dPnt->IsU;
            
            SqrSumF64 += Iuv * Iuv;
            CntF32 += 1.0f;

            if(rcg1.Period){
                arm_sqrt_f32((SqrSumF64 / CntF32), &rmsval1.RMSVal);
                SqrSumF64 = 0.0f;
                CntF32    = 0.0f;
            }

            // Voltage control and Current limiter 
            cntrvolt1.Imax    = dPnt->Isn;              // Максимальный ток подаваемый на электродвигатель
            cntrvolt1.Umax    = 1.0f;                   // Максимальная напруга

            cntrvolt1.Ifaz    = rmsval1.RMSVal;
            cntrvolt1.Period  = rcg1.Period;
            cntrvolt1.calc(&cntrvolt1);
            dPnt->Us = cntrvolt1.Uline * _SQRT3_2SQRT2; // вывод напряжения
                                
            // Space Vector Generator
            Tmp1 = cntrvolt1.Uline * arm_cos_f32(rcg1.Angle);
            svgen_dqv.Ualpha = Tmp1 / dPnt->Udc;
            svgen_dqv.Ubeta  = 0; 
            svgen_dqv.calc(&svgen_dqv);

            iclarke1.Alpha = svgen_dqv.Ualpha * dPnt->Udc;
            iclarke1.Beta  = svgen_dqv.Ubeta;
            iclarke1.calc(&iclarke1);

            GlobalM4.ValUu.FlashVal = (iclarke1.As - iclarke1.Bs) * 0.57735026918962576450914878050196f;
            GlobalM4.ValUv.FlashVal = (iclarke1.Bs - iclarke1.Cs) * 0.57735026918962576450914878050196f;
            GlobalM4.ValUw.FlashVal = (iclarke1.Cs - iclarke1.As) * 0.57735026918962576450914878050196f;

            // Dead Time Compesation
            if( fabsf(Wout) < (10.0f/80.0f)){
                dtc1.Iu = dPnt->IsU;
                dtc1.Iv = dPnt->IsV;
                dtc1.Iw = dPnt->IsW;
                dtc1.Ta = svgen_dqv.Ta;
                dtc1.Tb = svgen_dqv.Tb;
                dtc1.Tc = svgen_dqv.Tc;
                dtc1.Ud = UdcFiltr.UdcOut; //dPnt->Udc;
                dtc1.Fpwm = dPnt->Fpwm;
                dtc1.We = rcg1.Framp; //se1.WPsi;
                dtc1.calcRs(&dtc1); // Компенсация ДТ как для RS - полуторафазный режим

                svgen_dqv.Ta = dtc1.Ta;
                svgen_dqv.Tb = dtc1.Tb;
                svgen_dqv.Tc = dtc1.Tc;
            }

            // PWM References output
            Pwm.Ta = svgen_dqv.Ta;
            Pwm.Tb = svgen_dqv.Tb;
            Pwm.Tc = svgen_dqv.Tc;
            Pwm.Update(&Pwm);
            
            // Parameters Rr and Lls Calculation
            measRP2.Uline = cntrvolt1.Uline * _SQRT3_2SQRT2; // sqrt(3)/(2*sqrt(2))
            measRP2.Ifaz  = rmsval1.RMSVal;
            measRP2.Wout  = FreqRef1.LsCyclFreq;
            Wout  = measRP2.Wout;

            measRP2.Period = rcg1.Period;
            measRP2.Pact1  = measRP2.Uline * IqRr;
            measRP2.calc(&measRP2);

            // Передаем в основную часть программы текущую частоту вращения и флаг периода
            DrvConst.We = rcg1.Framp;        // Текущая скорость поля (pu)
            // Флаг периода выходного напряжения
            if(rcg1.Period){
              dPnt->Period = 1;  
            }             
                        
            // Time Limiter
            if(timer == prmTimes1.LsCalc1) {
                PwmControl.PwmOff(&PwmControl);    // Выкл ШИМ         
                if(GetStatusPWM() == 0) {          // ожидание подтверждения выкл. ШИМ
                    if (cntrvolt1.UmaxFlag == 0)        // Ошибка измерения параметров
                        GlobalM4.FailData.FailsFlag.bit.ErrMeasPrm = 1;
                    timer = 0;
                    dPnt->Us = 0;
                    cntrvolt1.reset(&cntrvolt1);
                    rcg1.reset(&rcg1);
                    
                    // Пока не произвели корректировку активной мощности при определении Rr, принимаем Rr = 1.05 * Rs
                    //dPnt->Rr = dPnt->Rs * 1.05f;
                    dPnt->Rr  = measRP2.Rr;
                    dPnt->Lls = measRP2.sLr;                                      
                    
                    switch (GlobalM4.MeasureLmRs) {
                        case MEASURE_NO_ROTAT_LM_ESTIM: // Оценка Lm
                            //dPnt->Lm = ((dPnt->Usn  * dPnt->VoltBASE) * _1DIV_SQRT_3) / ((dPnt->Isn * dPnt->CurrBASE) * _1DIV_SQRT_2 * 2.0f * _PI * (dPnt->Wnom * dPnt->FreqBASE)); 
                            
                            //************************************************************
                            // Расчет индуктивности по оценке тока ХХ !! ВЕРНУТЬ НОРМАЛЬНОЕ ЗНАЧЕНИЕ Rr, а не dPnt->Rr = dPnt->Rs * 1.05;
                            // Находим оценочную мощность двигателя
                            EstPnom = 0.001f*sqrt(3)*(dPnt->Isn*dPnt->CurrBASE)*(dPnt->Usn*dPnt->VoltBASE)*0.8; // 0.8 - Примерный cos Fi, переводим в кВт
                            for (tabIndex = 0; tabIndex < (sizeof(NoLoadCurrTab)/(7*sizeof(f32))); tabIndex++ ){
                              // Ищем строку с необходимой мощностью
                              if (EstPnom < NoLoadCurrTab[tabIndex*7]){
                                findIxxNumber = 1;
                                break; // Как только нашли, сразу на выход
                              }
                            }
                            // Если не нашли нужную мощность, то видимо она еще больше. Берем последний индекс
                            if (findIxxNumber == 0)	tabIndex = sizeof(NoLoadCurrTab)/(7*sizeof(f32)) - 1;
                            // Находим величину тока ХХ
                            IxxEst = NoLoadCurrTab[tabIndex*7 + dPnt->Zp]; // Находим значение в соответствии с числом пар полюсов
                            EstZ = (dPnt->Usn*0.5773502691896f*dPnt->VoltBASE) / (IxxEst * dPnt->Isn * dPnt->CurrBASE); // Z = Uf/If
                            dPnt->Lm = (f32) sqrt((f64)(EstZ * EstZ - dPnt->Rs * dPnt->Rs)) / (_2PI*dPnt->Wnom_Hz) - dPnt->Lls; // Lm = sqrt(Z^2-Rs^2)/314 - Lls
                            //GlobalM4.Ixx_meas = IxxEst * dPnt->Isn * dPnt->CurrBASE; // Перекидываем значение тока в переменную
                            ACIPrmMotState = ACIPrmMotStateReady;
                            break;
                        case MEASURE_NO_ROTAT_LM_MEAS: // Измерение Lm без вращения       
                            // Для расчета по экспонентам 
                            //dPnt->Lm  = ((dPnt->Rs * dPnt->Rr) / (dPnt->Rs + dPnt->Rr)) * dPnt->Lsd - dPnt->Lls ;		// Lm
                            // Расчет по малой частоте
                            dPnt->Lm = measLm2.Lm;
                            ACIPrmMotState = ACIPrmMotStateReady;
                            break;
                        case MEASURE_ROTATION_LM_MEAS:     
                            ACIPrmMotState = ACIPrmLmNew2; // Идем на измерение с вращением
                            break;
                        default:
                            dPnt->Lm  = ((dPnt->Rs * dPnt->Rr) / (dPnt->Rs + dPnt->Rr)) * dPnt->Lsd - dPnt->Lls ;		// Lm
                            break;
                    }
                    PauseTimer = MEAS_PAUSE_TIMEOUT; // Заряжаем задержку перед следующим измерением
                    GlobalM4.EnginParam.MeasRr = dPnt->Rr;
                    GlobalM4.EnginParam.MeasLm = dPnt->Lm;
                    GlobalM4.EnginParam.MeasLls = dPnt->Lls;                    
                }
            }
            else{
                PwmControl.PwmOn(&PwmControl);                      // Вкл ШИМ
                if(GetStatusPWM() == 0) {                           // ожидание подтверждения вкл. ШИМ
                    timer = 0;
                    cntrvolt1.Imax = 0.55f * dPnt->Isn;             // Максимальный ток 55% от номинального тока
                    cntrvolt1.UlineStep = 2.0f * UlineStep;
                    cntrvolt1.reset(&cntrvolt1);
                    rcg1.reset(&rcg1);
                }
                timer++;
            }
        } break; // ACIPrmMotStateCalcLsLrRr2
        
        //--------------------------------------------------------------
        // Определение Lm с вращением двигателя
        //--------------------------------------------------------------
        // New2 - разгон до номинальной частоты за 15 сек по линейной характеристике до номинального напряжения 
        //--------------------------------------------------------------
        
        #define tAcslFixed  35.0f          		  // Время разгона сек        
        case ACIPrmLmNew2:
          if (PauseTimer){ // Задержка перед началом измерения, чтобы АЦП смог захватить новый ноль 
            PauseTimer--;
            break;
          }          

          tAcsl = GlobalM4.OriginAccelTime;
          // Проверяем время разгона, если оно меньше tAcslFixed, заменяем. Если больше - не трогаем          
          if(tAcsl < tAcslFixed)  tAcsl = tAcslFixed;
             
          if(GetStatusPWM() == 0) {
            // расчет прироста угла выходного поля
            ScalarDC_ORDINAR.StepAngle = 1 / F_PWM_MEAS_PARAM; // Шаг по фазе численно равен Tpwm
            ScalarDC_ORDINAR.Angle = 0.0f;
            ScalarDC_ORDINAR.Uscl  = 0.0f;  // Напряжение отдаем в вольтах  
            ScalarDC_ORDINAR.Fscl  = 0.0f;  // Частоту отдаем в Гц
            CntF32 = 0.0f;	// Задание частоты
            
            // dPnt->Usn // Номинальное напряжение АД pu
            // dPnt->Fnom  // Номинальная частота АД pu
            dUscl  = dPnt->Usn / F_PWM_MEAS_PARAM / tAcsl; // Находим шаг приращения по напряжению в соответствии с номинальным напряжением двигателя, pu
            dFscl  = dPnt->Fnom / F_PWM_MEAS_PARAM / tAcsl; // Находим шаг приращения по частоте в соответствии с номинальной частотой двигателя, pu
            
            Pwm.Ta = 0.0f;
            Pwm.Tb = 0.0f;
            Pwm.Tc = 0.0f;
            Pwm.Update(&Pwm);                        
            PwmControl.PwmOn(&PwmControl);   // Вкл ШИМ
          } 
          else{
            // Приращение частоты и напряжения                       
            CntF32 += dFscl;
            if(CntF32 > dPnt->Fnom ) CntF32 = dPnt->Fnom; // Разгон выполняем до номинальной частоты двигателя, pu
            
            ScalarDC_ORDINAR.Uscl += dUscl; // pu
            if(ScalarDC_ORDINAR.Uscl > dPnt->Usn) ScalarDC_ORDINAR.Uscl = dPnt->Usn; // pu
            
            ScalarDC_ORDINAR.Udc = dPnt->Udc; // pu
            ScalarDC_ORDINAR.Fscl = CntF32;             
            //=========================
            ScalarDC_ORDINAR.calc(&ScalarDC_ORDINAR);   //скаляр обычный
            // Передаем в основную часть программы текущую частоту вращения и флаг периода
            DrvConst.We = ScalarDC_ORDINAR.Fscl;        // Текущая скорость поля (pu)
             // Флаг периода выходного напряжения
            if(ScalarDC_ORDINAR.Period){
              ScalarDC_ORDINAR.Period = 0;
              DrvConst.Period = 1;  
            }            
            
            if(CntF32 >= dPnt->Fnom){
              timer = 0;
              measRP2.Zeq = ScalarDC_ORDINAR.Uout * 0.5773502691896f; // Фиксируем величину напряжения на двигателе, pu
              ACIPrmMotState = ACIPrmLmNew3;
              CntF32 = 0;
            }
          }
          
          break;
        //--------------------------------------------------------------
        // New3 - определение Lm
        //--------------------------------------------------------------
        case ACIPrmLmNew3:          
          timer++;
          ScalarDC_ORDINAR.Udc = dPnt->Udc; //  pu 
          ScalarDC_ORDINAR.calc(&ScalarDC_ORDINAR);   
          
          // Передаем в основную часть программы текущую частоту вращения и флаг периода
          DrvConst.We = ScalarDC_ORDINAR.Fscl;        // Текущая скорость поля (pu)           
                   
          Tmp1 = ScalarDC_ORDINAR.Uout * 0.5773502691896f; // pu
          measRP2.Zeq = f32_Filtr(measRP2.Zeq, Tmp1, 0.125f); // фильтруем выходное напряжение на двигателе - pu
          
          Iuv = dPnt->IsU;
          SqrSumF64 += Iuv * Iuv;
          Iuv = dPnt->IsV;
          SqrSumF64 += Iuv * Iuv;
          Iuv = dPnt->IsW;
          SqrSumF64 += Iuv * Iuv;
          
          CntF32 += 3.0f;
          
          if(ScalarDC_ORDINAR.Period){
            arm_sqrt_f32((SqrSumF64 / CntF32) , &Tmp1); // Извлекаем корень из суммы квадратов токов, pu
            measRP2.Req = f32_Filtr(measRP2.Req, Tmp1, 0.125f); // Нафильтрорвываем ток, pu           
            SqrSumF64 = 0.0f;
            CntF32    = 0.0f;
            ScalarDC_ORDINAR.Period = 0;   
          }
          // Флаг периода выходного напряжения
          if(ScalarDC_ORDINAR.Period){
            ScalarDC_ORDINAR.Period = 0;
            DrvConst.Period = 1;  
          }          
          
          if(timer >= (F_PWM_MEAS_PARAM * 5)){	     // 5 sec
            PwmControl.PwmOff(&PwmControl);    // Выкл ШИМ
            
            dPnt->Ixx = IQ_to_FLOAT(measRP2.Req, Current_base); // Ixx измеренный, A                   
            CntF32 = IQ_to_FLOAT(measRP2.Zeq, Volt_base) / IQ_to_FLOAT(measRP2.Req, Current_base); // Находим полное сопротивление
            arm_sqrt_f32 ((CntF32 * CntF32 - dPnt->Rs * dPnt->Rs), &Tmp1); // Находим индуктивное сопротивление              
            // Находим значение индуктивности
            dPnt->Lm =  Tmp1 / (_2PI * IQ_to_FLOAT(dPnt->Fnom, Herz_base)) - dPnt->Lls; // Lm = sqrt(Z^2-Rs^2)/314 - Lls
            
            GlobalM4.EnginParam.MeasLm = dPnt->Lm;
            GlobalM4.EnginParam.MeasIxx = dPnt->Ixx;
                        
            ACIPrmMotState = ACIPrmMotStateReady;
          }
          break;
          //------------------------------------------------------------------------------                  
        case ACIPrmMotStateCalcLm1:
          if(timer == prmTimes1.LmCalc1) {
            timer = 0;
            cntrvolt1.reset(&cntrvolt1);
            rcg1.reset(&rcg1);
            rcg2.reset(&rcg2);
            ////dtc1.dT    = (2 * dPnt->DeadTime * (1e-6*dPnt->Fpwm)); // МВ в относительных единицах
            //dtc1.dI    = (0.11 / dPnt->CurrBASE);
            //measLm1.FirstStart = 1;
            measLm2.FirstStart = 1;
            ACIPrmMotState = ACIPrmMotStateCalcLm2;
          }
          else{
            // Сброс флагов,для обнуления сумм 
            dPnt->Angle     = 0;
            dPnt->Period    = 1;
          }
          timer++;
          break; // ACIPrmMotStateCalcLm1
          
        case ACIPrmMotStateCalcLm2:
          // ------------------------------------------------------------------------------
          //      The Ramp Generation 
          // ------------------------------------------------------------------------------
          rcg1.Fwork = FreqRef1.LmLineFreq;
          rcg1.Fnom  = FreqRef1.LmLineFreq;
          rcg1.calc(&rcg1);
          dPnt->Angle     = rcg1.Angle;
          dPnt->Period    = rcg1.Period;
          
          // генерация периода, для плавного увеличения напряжения
          rcg2.Fwork = FreqRef1.LsLineFreq;
          rcg2.Fnom  = FreqRef1.LsLineFreq;
          rcg2.calc(&rcg2);
          
          // ------------------------------------------------------------------------------
          //      Voltage control and Current limiter 
          // ------------------------------------------------------------------------------
          cntrvolt1.Ifaz    = f32_Filtr(cntrvolt1.Ifaz, dPnt->IsV, 0.125);
          cntrvolt1.Period  = rcg2.Period;
          cntrvolt1.calcLm(&cntrvolt1);
          dPnt->Us = cntrvolt1.Uline *(_SQRT3_2SQRT2); // вывод напряжения
          
          if(!cntrvolt1.UmaxFlag) rcg1.reset(&rcg1);
          
          // ------------------------------------------------------------------------------
          //      Space Vector Generator
          // ------------------------------------------------------------------------------
          // Space Vector Generator
          Tmp1 = cntrvolt1.Uline * arm_cos_f32(rcg1.Angle);
          svgen_dqv.Ualpha = Tmp1 / dPnt->Udc;
          svgen_dqv.Ubeta  = 0; 
          svgen_dqv.calc(&svgen_dqv);
          
          // ------------------------------------------------------------------------------
          //      Dead Time Compesation
          // ------------------------------------------------------------------------------
          dtc1.Iu = dPnt->IsW;
          dtc1.Iv = dPnt->IsV;
          dtc1.Iw = dPnt->IsW;
          dtc1.Ta = svgen_dqv.Tc; // фаза перевернута на 120 градусов
          dtc1.Tb = svgen_dqv.Ta;
          dtc1.Tc = svgen_dqv.Tb;
          dtc1.Ud = UdcFiltr.UdcOut; //dPnt->Udc;
          dtc1.Fpwm = dPnt->Fpwm;
          dtc1.We = rcg1.Framp; //se1.WPsi;
          dtc1.dtcMode = 0;
          dtc1.calc(&dtc1);
          
          svgen_dqv.Ta = dtc1.Ta;
          svgen_dqv.Tb = dtc1.Tb;
          svgen_dqv.Tc = dtc1.Tc;
          
          // PWM References output
          Pwm.Ta = svgen_dqv.Ta;
          Pwm.Tb = svgen_dqv.Tb;
          Pwm.Tc = svgen_dqv.Tc;
          Pwm.Update(&Pwm);
          
          // ------------------------------------------------------------------------------
          //    Parameter Lm Calculation
          // ------------------------------------------------------------------------------
          measLm2.UlineFlash  = Tmp1 * (1.0/_2_SQRT3/_2_SQRT3 * 1.06); // ??????????????? 
          measLm2.IfazFlash   = dPnt->IsV;
          measLm2.Wout        = FreqRef1.LmCyclFreq;
          measLm2.Angle       = rcg1.Angle;
          measLm2.Period      = rcg1.Period;
          measLm2.PeriodAmt   = 4; // делаем расчет 4 периода
          measLm2.sLr         = measRP2.sLr;
          measLm2.calc(&measLm2);
          
          // ------------------------------------------------------------------------------
          //    Time Limiter
          // ------------------------------------------------------------------------------
          // если не вышел на заданный ток, то выход из данного опыта
          if(measLm2.StopCalc || (timer == prmTimes1.LmCalc2 && !cntrvolt1.UmaxFlag)) { 
            PwmControl.PwmOff(&PwmControl);    // Выкл ШИМ         
            if(GetStatusPWM() == 0) {  // ожидание подтверждения выкл. ШИМ
              timer = 0;
              dPnt->Us = 0;
              cntrvolt1.reset(&cntrvolt1);
              rcg1.reset(&rcg1);
              rcg2.reset(&rcg2);
              measLm2.StopCalc = 0;
              // передача параметра в обменную структуру
              dPnt->Lm = measLm2.Lm;
              
              ACIPrmMotState = ACIPrmMotStateCalcLsLrRr2; // ACIPrmMotStateReady;
            }
          }
          else{           
            PwmControl.PwmOn(&PwmControl);                      // Вкл ШИМ
            if(GetStatusPWM() == 0) {                           // ожидание подтверждения вкл. ШИМ              
              timer = 0;
              cntrvolt1.Imax = 0.70 * dPnt->Isn;   // Максимальный ток 60% от номинального тока
              cntrvolt1.UlineStep = UlineStep;
              cntrvolt1.reset(&cntrvolt1);
              rcg1.reset(&rcg1);
              rcg2.reset(&rcg2);              
            }
            timer++;                        
          }          
          break; // ACIPrmMotStateCalcLm2
          
          //------------------------------------------------------------------------------        
        case ACIPrmMotStateReady:{          // Параметры успешно измерены
          DrvWords.Ctrl.bit.vCodeEnb = 1;
          DrvWords.Stat.bit.mWorkFin = 1;
          
          PwmControl.PwmOff(&PwmControl);   // Выкл ШИМ
          
          //-------------------------------------------
          // Оценочный расчет момента инерции
          // Находим поток номинальный / ток ХХ по сути
          if (GlobalM4.EnginParam.MeasIxx == 0.0f){
    	    // Если тока ХХ нету - то вычисляем возбуждение через формулу
    	    LocalPhirNom = 0.9f * _SQRT_2_DIV_3 * dPnt->Usn * dPnt->VoltBASE / (2.0f * _PI * dPnt->Fnom * dPnt->FreqBASE * GlobalM4.EnginParam.MeasLm * dPnt->CurrBASE); //0.9
          } else {
    	    LocalPhirNom = GlobalM4.EnginParam.MeasIxx / dPnt->CurrBASE; //0.9
          }          
          
          // v->Kp = v->Lr * v->Jr / (3.0f * v->Lm * v->Zp * v->Zp * v->Tsl * v->PrNom) * Kb;          
          Kb = dPnt->FreqBASE / (dPnt->CurrBASE * GlobalM4.EnginParam.MeasLm * dPnt->CurrBASE);
          GlobalM4.EnginParam.Estim_Jr = GlobalM4.EnginParam.Estim_Kp * (3.0f * GlobalM4.EnginParam.MeasLm * dPnt->Zp * dPnt->Zp * dPnt->Tsl * LocalPhirNom) / ((GlobalM4.EnginParam.MeasLm + GlobalM4.EnginParam.MeasLls) * Kb);
          //-------------------------------------------
          
          // Вернем настройки ШИМ назад
          ACIPrmMotState = ACIPrmDeinitPwm;
        } break;
    
//------------------------------------------------------------------------------        
        case ACIPrmDeinitPwm:{              // Перенастроить ШИМ на значение с уставок
           PwmControl.PwmOff(&PwmControl);  // Выкл ШИМ
           
           if((PwmControl.StatusPwm == PwmStop)||(PwmControl.StatusPwm == PwmErr)){
              // Перенастроить ШИМ
              Pwm.DeadTime_uS = dPnt->DeadTimeInit.DeadTime; //GlobalM4.DeadTime;
              Pwm.Fpwm_kHz = GlobalM4.Fpwm * 0.001f;
              Pwm.Fpwm_Hz = GlobalM4.Fpwm;
              Pwm.ReInit(&Pwm);                      
  
              // Перенастроить АЦП
              Adc.Fpwm_kHz = Pwm.Fpwm_kHz;    // Получить частоту ШИМ модуля PWM (для синхронизации АЦП с ШИМ)
              Adc.ReInit(&Adc);
            
              // Переход в режим бездействия
              ACIPrmMotState = ACIPrmMotStateOff;
              
              DrvWords.Ctrl.bit.vCodeEnb = 1;            // Разблокировать вызов обработчика системы регулирования
            }
        } break;

//------------------------------------------------------------------------------        
        case ACIPrmMotStateOff:{            // Состояние бездействия
      
        } break;
//----------------------------------------------------------------
    default: break;
    }
        
    rcg1.Period = 0; 
}

// Получить сотояние ШИМ (Вкл/Выкл)
u8 GetStatusPWM(void){
  if((PwmControl.StatusPwm == PwmWork) || (PwmControl.StatusPwm == PwmStart)){
     return 1;     // ШИМ работает!!!
  }
  else{
     return 0;     // ШИМ отключен!!!
  }
  
}


