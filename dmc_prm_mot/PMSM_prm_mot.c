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
#include "PMSM_prm_mot.h"
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
    PMSMPrmMotStateOff,         				// 0 Алгоритм отключен
    PMSMPrmInitPwm,                                             // 1 Инициализация процесса измерения
    PMSMPrmMotStateCalcRs1,     				// 2 Состояние вычисления параметра Rs
    PMSMPrmMotStateCalcRs1SoftCurrentDown,     	                // 3 Состояние плавного снижения выходного тока двигателя
    PMSMPrmMotStateCalcLs1,     				// 4 Состояние вычисления параметра Ls Lr Rr
    PMSMPrmMotStateCalcLs2,     				// 5 Состояние вычисления параметра Lm
    PMSMPrmMotStateReady,        				// 6 Данные готовы, процесс завершен
    PMSMPrmDeinitPwm,                                           // 7 ДЕинициализация процесса измерения
}PMSMPrmMotState_type;

PMSMPrmMotState_type  PMSMPrmMotState = PMSMPrmMotStateOff;      // Флаги состояния процесса сбора и обработки данных

#define  Kp     0.005f
#define  Ki     0.0005f

#define TrOnOff      32.4f                    // 150 * 0.18f
#define TrOnOffLow   54.0f                    // 150 * 0.30f 	Малая мощность
#define TrOnOffSD    54.0f                    // 150 * 0.30f 	SD

#define  UonT     (1.0f + 0.8f)/2.0f          // 0.75v
#define  UonD     (1.2f + 0.9f)/2.0f          // 1.3v
#define  RdinT    (0.0094f + 0.014f)/2.0f     // 0.025ом
#define  RdinD    (0.0057f + 0.0094f)/2.0f    // 0.023ом


#define  DEAD_TIME_MEAS_PARAM   3.0f          // Мертвое время при измерениях параметров   
#define  F_PWM_MEAS_PARAM       4000.0f          // Частота ШИМ при измерениях параметров
#define  MEAS_PAUSE_TIMEOUT     1*F_PWM_MEAS_PARAM

// Прототипы функций
u8 GetStatusPWM(void);         // Получить сотояние ШИМ (Вкл/Выкл)

// Дополнительные объекты
//SCALARORDINAR        ScalarDC_ORDINAR = SCALARCONTROL_ORD;    // Скаляр для измерителя параметров
extern SCALARORDINAR ScalarDC_ORDINAR;
extern PWMCONTROL    PwmControl;    // Аппарат состояний ШИМ (Пуск/стоп только отсюда!!!)
extern PWMGEN        Pwm;           // Модуль ШИМ
extern CURVOLTMEAS   meas1;
extern CLARKE        clarke2;
extern PARK          park2;
extern ADCDRV        Adc;           // Модуль АЦП
f32     Fnom_pmsm;

void PMSM_PrmMot_Init()
{
    float Tmp1;
    
    if (!DrvWords.Ctrl.bit.SysCalc) return;
    if (!(DrvConst.DriveType == TYPE_PMSM_FOC_SENSORED ||
        DrvConst.DriveType == TYPE_PMSM_FOC_SENSORLESS )) return;

    Tmp1 = 10.0f / dPnt->FreqBASE;
    if(dPnt->Fnom >= Tmp1)
        Fnom_pmsm = dPnt->Fnom;
    else 
        Fnom_pmsm = Tmp1;
    
    //  Инициализация переменных для подстройки напряжения 
    cntrvolt1.Imax      = dPnt->Isn;   // Максимальный ток подаваемый на электродвигатель
    cntrvolt1.Umax      = 1.0f;        // Максимальная напруга
    // Шаг настройки напряжения
    // Скорость наростания напряжения Usn/TUsn_MMP = [В/с]
    UlineStep = (_2SQRT2_SQRT3 * dPnt->Usn) / (dPnt->TUsn_MMP * Fnom_pmsm* dPnt->FreqBASE);

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

//    //  Инициализация постоянных для определения Lm                         
//    measLm2.K1 = dPnt->VoltBASE*2.0/(3.0*dPnt->CurrBASE*dPnt->FreqBASE); 
//    measLm2.K2 = measRs1.K1;
        
    //  Инициализация генератора темпа
    rcg1.StepAngleMax = (dPnt->FreqBASE * _2PI) / F_PWM_MEAS_PARAM; // +
    rcg1.StepFreqMax  = 1.0f;
    
    rcg2.StepAngleMax = (dPnt->FreqBASE * _2PI) / F_PWM_MEAS_PARAM; //_IQ(dPnt->FreqBASE / dPnt->Fpwm_MMP);
    rcg2.StepFreqMax  = 1.0f;
        
    //  Инициализация частоты измерения
#warning А почему измерение на 5 Гц?? Причем и в оригинальном лифтовом проекте так же стоит 5 Гц. Должна быть номинальная частота.
    FreqRef1.LsLineFreq = Fnom_pmsm;// / dPnt->FreqBASE;
    FreqRef1.LsCyclFreq = FreqRef1.LsLineFreq * _2PI;
    
    //  Инициализация времени измерения опытов 
    //                              Интервал, Сек     Частота ШИМ, Гц
    prmTimes1.Init      = (u32)(     1.0        *  F_PWM_MEAS_PARAM);
    prmTimes1.RsCalc1   = (u32)(     10.0       *  F_PWM_MEAS_PARAM);
    prmTimes1.LsCalc1   = (u32)(     2.0        *  F_PWM_MEAS_PARAM); // Было 4 секунды - это по сути пауза между измерениями
    prmTimes1.LsCalc2   = (u32)(     10.0/*3.0*/        *  F_PWM_MEAS_PARAM); // Время на измерение индуктивностей
    prmTimes1.LsCalc3   = (u32)(     2.5        *  F_PWM_MEAS_PARAM);  // Время настройки смещения фильтра тока, перед измерением
}                        

void PMSM_PrmMot_Calc()
{ 
    static float Tmp1;
    static f32 TmpU = 0, Urs = 0;
    static f32 UrsFiltr = 0;
    //static u32 time = 0, LogTimer = 0, LogTimeOff=0;
    static s32 timer = 0;
    static u16 WorkEnbOld = 0;
    static f32 IqRr = 0, IdRr = 0;
    //static u32 Cnt = 0, UsclPidOffCnt = 0, RepeatCntLm11 = 0;
    //static u32 PauseTimer = 0;
    //static f32 Tlm0, Tlm1, Tlm2, Tlm3, Wout;
    //static f32 dUscl = 0, dFscl = 0;
    //static f32 RefFiltr_K11=0, RefFiltr_K21=0, dTsLmCnt = 0, LegalTs = 0;
    
    //f32         dTsLm;
    //f32         locTpwm;    
    //s16         Indx;
    //f32	        EstZ, EstPnom;
    f32         Iuv, Uuv, U1, U2;
    f32         Kb; 
    static f32  UlineOffset = 0;
    static f32  IfazOffset = 0;
    
    // 1) Проверить может у нас скаляр, тогда мерять параметры нет смысла
    if (!(DrvConst.DriveType == TYPE_PMSM_FOC_SENSORED ||
        DrvConst.DriveType == TYPE_PMSM_FOC_SENSORLESS )) return;
        
    // 2) Обработка вкл/выкл измерения параметров
    if((DrvWords.Ctrl.bit.mWorkEnb == 1)&&(WorkEnbOld == 0)){  // Запрос на включение измерения
        PMSMPrmMotState = PMSMPrmInitPwm;                        // Запустить измерение параметров двигателя
        DrvWords.Ctrl.bit.vCodeEnb = 0;                        // Заблокировать вызов обработчика системы регулирования 
        DrvWords.Stat.bit.mWorkFin = 0;                        // При начале измерений сбросить флаг завершения змирений  
    }
    else if(((DrvWords.Ctrl.bit.mWorkEnb == 0)&&(WorkEnbOld == 1)) || (DrvWords.Ctrl.bit.FailOn && (PMSMPrmMotState != PMSMPrmMotStateOff))){ // Запрос на выключение измерения
        PMSMPrmMotState = PMSMPrmDeinitPwm;                      // Переход на перенастройку ШИМ по уставкам
    }    
      
    WorkEnbOld = DrvWords.Ctrl.bit.mWorkEnb;
 

    DrvWords.Stat.bit.mExpNum = 0x1F & PMSMPrmMotState; // фильтрация первых 5 бит

    //GlobalM4.StatusPwm = PwmControl.StatusPwm;
    
// Аппарат состояний измерителя параметров двигателя    
    switch(PMSMPrmMotState) {

        case PMSMPrmInitPwm:{
           // Перенастроить модуль ШИМ (что бы всегда при измирениях была одна частота)
           Pwm.DeadTime_uS = DEAD_TIME_MEAS_PARAM; 
           Pwm.Fpwm_kHz = F_PWM_MEAS_PARAM*0.001f; // Перевод в кГц для перезапуска частоты ШИМа
           Pwm.Fpwm_Hz = F_PWM_MEAS_PARAM; // Перевод в Гц для перезапуска частоты ШИМа
           Pwm.ReInit(&Pwm);
             
           // Синхронизировать АЦП с периодом ШИМ
           Adc.Fpwm_kHz = Pwm.Fpwm_kHz;               // Получить частоту ШИМ модуля PWM (для синхронизации АЦП с ШИМ)
           Adc.ReInit(&Adc);
        
           // Переход на измерение параметров (Rs)
           PMSMPrmMotState = PMSMPrmMotStateCalcRs1;
           
           timer = 0;
           cntrvolt1.reset(&cntrvolt1);
           rcg1.reset(&rcg1);           
        } break;
        
//------------------------------------------------------------------------------        
        case PMSMPrmMotStateCalcRs1:{                           // Измерение Rs
          if(GetStatusPWM() == 0) {
    	      DriveInit(dPnt);
              cntrvolt1.reset(&cntrvolt1);
              rcg1.reset(&rcg1);
              measRs1.reset(&measRs1);
              cntrvolt1.Imax = dPnt->Isn;                 // номин ток              
              cntrvolt1.Umax      = 1.0f;        // Максимальная напруга
              UrsFiltr = 0;
              timer = prmTimes1.RsCalc1; // Заряжаем таймер
              
              Pwm.Ta = 0;
              Pwm.Tb = 0;
              Pwm.Tc = 0;
              Pwm.Update(&Pwm);            
    	      PwmControl.PwmOn(&PwmControl);     // Вкл ШИМ              
          } else {
            // ------------------------------------------------------------------------------
            //      The Ramp Generation 
            // ------------------------------------------------------------------------------
            rcg1.Fwork = Fnom_pmsm;
            rcg1.Fnom  = Fnom_pmsm;
            rcg1.calc(&rcg1);
            // Output
            dPnt->Angle     = rcg1.Angle;
            dPnt->Period    = rcg1.Period;
            
            // ------------------------------------------------------------------------------
            //      Voltage control and Current limiter 
            // ------------------------------------------------------------------------------
            cntrvolt1.Ifaz   = f32_Filtr(cntrvolt1.Ifaz, fabsf(dPnt->IsU), 0.0625f);	// Ток с фильтром
            cntrvolt1.f32IsU = cntrvolt1.Ifaz * dPnt->CurrBASE;                          // Абсолютное значение тока
            cntrvolt1.s16IsU = (s16) (cntrvolt1.f32IsU * 100.0f);            
            cntrvolt1.Period    = rcg1.Period;
            cntrvolt1.calc(&cntrvolt1);
            dPnt->Us    = cntrvolt1.Uline * _SQRT3_2; // вывод напряжения
            UlineOffset = cntrvolt1.Uline;
            
//            // Проверка исключительных ситуаций
//            if (!cntrvolt1.InomDoneFlag && !cntrvolt1.UmaxFlag) {
//              // Если ток не достигнут и напряжение не в потолке
//              timer = prmTimes1.RsCalc1; // переЗаряжаем таймер и ждем достижения необходимого уровня тока
//            } else if (cntrvolt1.UmaxFlag) { //Если достиг потолка по напряжению
//              timer = 1; // Таймер в минимум и готовимся к завершению 
//            } // Если ни одно из условий не выполняется, то значит ток достигнут, и ждем 10 секунд измерений
//            
            // Generate PWM output
            ScalarDC_ORDINAR.Angle     = _PI;
            ScalarDC_ORDINAR.Udc       = dPnt->Udc;
            ScalarDC_ORDINAR.Uscl      = cntrvolt1.Uline; 
            ScalarDC_ORDINAR.Fscl      = 0;
            ScalarDC_ORDINAR.StepAngle = 0;
            ScalarDC_ORDINAR.calc(&ScalarDC_ORDINAR);
            
            // ------------------------------------------------------------------------------
            //      Parameter Rs calculation
            // ------------------------------------------------------------------------------  
            Iuv = cntrvolt1.f32IsU;
            
            U1 = TrOnOff;
            if(Iuv < 8.0f) U1 = TrOnOffLow;		                    // Ток < 8A
            if(Iuv >= 50.0f) U1 = TrOnOffSD;	                            // Ток > 50A  SD
            
            Uuv = (GlobalM4.ValUd.FlashVal - 2 * UonT - 1.5f * Iuv * RdinT) * ((fabsf( (float)(Pwm.UvalTimer - Pwm.VvalTimer) ) - (Pwm.DeadTime_uS * SystemCoreClock / 1000000.0f ) + U1) / (float)Pwm.MaxCountT1);            // Пока так - Pwm.DeadTime_uS + U1) / (f32)EPwm2Regs.TBPRD;
            U1  = ((UonD + UonT) + Iuv * (RdinD + 0.5f * RdinT)) * ((((float)Pwm.MaxCountT1 - (float)(Pwm.UvalTimer) + (Pwm.DeadTime_uS * SystemCoreClock / 1000000.0f )) / (float)Pwm.MaxCountT1));                                            // Пока так - (f32)EPwm2Regs.TBPRD - (f32) EPwm2Regs.CMPA.half.CMPA + Pwm.DeadTime_uS) / (f32)EPwm2Regs.TBPRD;              
            U2  = ((UonD + UonT) + Iuv * (RdinT + 0.5f * RdinD)) * 0.5f;
            TmpU = (Uuv - U1 - U2);
            Urs = TmpU;
            cntrvolt1.s16Lm1 = (s16)((Urs) * 10.0f);
            cntrvolt1.s16Lm2 = (s16)((U2) * 10.0f);
            
            // Напряжение в вольтах, флотовое
            UrsFiltr = f32_Filtr(UrsFiltr, Urs, 0.125f);	//Напряжение фильтрованное            
            
            timer --;
            
            if(timer == 0 || timer < 0){
              // Контрольные проверки измерения.
              Tmp1 = cntrvolt1.Imax - fabsf(dPnt->IsU);
              if(fabsf(Tmp1) > 0.05)                        // ошибка измерения параметров
                GlobalM4.FailData.FailsFlag.bit.ErrMeasPrm = 1;
              
              // Проверяем, если ток менее 10% от номинала, то проблема. Вероятно есть обрыв фазы
              if (fabsf(dPnt->IsU) < 0.1f*cntrvolt1.Imax || fabsf(dPnt->IsV) < 0.1f*cntrvolt1.Imax || fabsf(dPnt->IsW) < 0.1f*cntrvolt1.Imax) {
                GlobalM4.FailData.FailsFlag.bit.MeasPrmBrkPhase = 1;
                GlobalM4.FailData.FailsFlag.bit.ErrMeasPrm = 0; // Если выявлен обрыв - то сбросить аварию ошибки измерения
              }
              
              // Определяем Rs по закону Ома, напряжение и ток в А и В, с учетом полуторафазной схемы включения
              dPnt->Rs = (f32)Urs / cntrvolt1.f32IsU * 0.66666666666f;
              GlobalM4.EnginParam.MeasRs = dPnt->Rs;              
              
              // Переход в режим плавного сброса тока, без отключения ШИМа
              PMSMPrmMotState = PMSMPrmMotStateCalcRs1SoftCurrentDown; // PMSMPrmMotStateCalcLs1;
            }            
          }
        } break;

        case PMSMPrmMotStateCalcRs1SoftCurrentDown:{ 
          // ------------------------------------------------------------------------------
          //      The Ramp Generation
          // ------------------------------------------------------------------------------
          rcg1.Fwork = Fnom_pmsm;
          rcg1.Fnom  = Fnom_pmsm;
          rcg1.calc(&rcg1);
          // Output
          dPnt->Angle     = rcg1.Angle;
          dPnt->Period    = rcg1.Period;
          
          // ------------------------------------------------------------------------------
          //      Voltage control and Current limiter
          // ------------------------------------------------------------------------------
          cntrvolt1.Ifaz   = f32_Filtr(cntrvolt1.Ifaz, fabsf(dPnt->IsU), 0.0625f);	// Ток с фильтром
          cntrvolt1.Period    = rcg1.Period;
          cntrvolt1.calcCurDown(&cntrvolt1);
          dPnt->Us    = cntrvolt1.Uline * _SQRT3_2; // вывод напряжения
          
          // Generate PWM output
          ScalarDC_ORDINAR.Angle     = _PI;
          ScalarDC_ORDINAR.Udc       = dPnt->Udc;
          ScalarDC_ORDINAR.Uscl      = cntrvolt1.Uline; 
          ScalarDC_ORDINAR.Fscl      = 0;
          ScalarDC_ORDINAR.StepAngle = 0;
          ScalarDC_ORDINAR.calc(&ScalarDC_ORDINAR);          
          
          
          if(cntrvolt1.CurDownFlag) { // Ждем флаг полного снижения напряжения
            PwmControl.PwmOff(&PwmControl);    // Выкл ШИМ
            
            timer = 0;
            cntrvolt1.reset(&cntrvolt1);
            rcg1.reset(&rcg1);
            PMSMPrmMotState = PMSMPrmMotStateCalcLs1;            
          }
        } break;
               
        
        case PMSMPrmMotStateCalcLs1: {
          if(timer == prmTimes1.LsCalc1) {
            timer = 0;
            cntrvolt1.Imax = 2.0f * dPnt->Isn;// Максимальный ток подаваемый на электродвигатель
            cntrvolt1.Umax = UlineOffset - (0.10f * UlineOffset); // напряжение смещения с опыта постоянного тока
            //cntrvolt1.Umax      = 1.0f;        // Максимальная напруга
            cntrvolt1.reset(&cntrvolt1);
            rcg1.reset(&rcg1);
            rmsval1.reset(&rmsval1);
            PMSMPrmMotState = PMSMPrmMotStateCalcLs2;
            IqRr = IdRr = 0;
            
            Pwm.Ta = 0;
            Pwm.Tb = 0;
            Pwm.Tc = 0;
            Pwm.Update(&Pwm);            
            PwmControl.PwmOn(&PwmControl);     // Вкл ШИМ              
            
          }
          timer++;
        } break;

        case PMSMPrmMotStateCalcLs2: {
          // ------------------------------------------------------------------------------
          //      The Ramp Generation 
          // ------------------------------------------------------------------------------
          rcg1.Fwork = FreqRef1.LsLineFreq;
          rcg1.Fnom  = FreqRef1.LsLineFreq;
          rcg1.calc(&rcg1);
          dPnt->Angle  = rcg1.Angle;
          dPnt->Period = rcg1.Period;
          
          // Connect inputs of the PARK module and call the park transformation calculation function.
          park2.Alpha = dPnt->IsU;
          park2.Beta =  0.0f; 
          park2.Angle = rcg1.Angle;
          park2.calc(&park2);
          
          IqRr  = f32_Filtr(IqRr, park2.Qs, 0.000244140625f); // 1/4096
          IdRr  = f32_Filtr(IdRr, park2.Ds, 0.000244140625f);
                    
          // ------------------------------------------------------------------------------
          //      Voltage control and Current limiter 
          // ------------------------------------------------------------------------------
          cntrvolt1.Ifaz    = rmsval1.RMSVal;
          cntrvolt1.Period  = rcg1.Period;
          cntrvolt1.calc(&cntrvolt1);          
          dPnt->Us    = cntrvolt1.Uline * _SQRT3_2SQRT2; // вывод напряжения
          
          if(timer < prmTimes1.LsCalc3) {
            cntrvolt1.reset(&cntrvolt1);
            IfazOffset = f32_Filtr(IfazOffset,dPnt->IsV, 0.03125f);
          }/* else { // Процесс калибровки нуля тока завершен, начинаем накачку и потом измерение         
            // Проверка исключительных ситуаций
            if (!cntrvolt1.InomDoneFlag && !cntrvolt1.UmaxFlag) {
              // Если ток не достигнут и напряжение не в потолке
              timer = prmTimes1.LsCalc3; // Держим таймер и ждем достижения необходимого уровня тока
            } else if (cntrvolt1.UmaxFlag) { //Если достиг потолка по напряжению
              timer = prmTimes1.LsCalc2; // Таймер в потолок и готовимся к завершению 
            } // Если ни одно из условий не выполняется, то значит ток достигнут, и ждем время измерения                  
          }*/
          
          // ------------------------------------------------------------------------------
          //      Space Vector Generator
          // ------------------------------------------------------------------------------
          Tmp1 = cntrvolt1.Uline * arm_cos_f32(rcg1.Angle);
          svgen_dqv.Ualpha = (Tmp1 + UlineOffset)  / dPnt->Udc; 
          svgen_dqv.Ubeta  = 0;                    
          svgen_dqv.calc(&svgen_dqv);
          
          // ------------------------------------------------------------------------------
          //      Dead Time Compesation (changed phaze)
          // ------------------------------------------------------------------------------
          dtc1.Iu = dPnt->IsW;
          dtc1.Iv = dPnt->IsV;
          dtc1.Iw = dPnt->IsW;
          dtc1.Ta = svgen_dqv.Tc;
          dtc1.Tb = svgen_dqv.Ta;
          dtc1.Tc = svgen_dqv.Tb;
          dtc1.Ud = UdcFiltr.UdcOut; //dPnt->Udc;
          dtc1.Fpwm = dPnt->Fpwm;
          dtc1.We = rcg1.Framp;  
          dtc1.dtcMode = 0;
          dtc1.calc(&dtc1); // Компенсация ДТ как для RS - полуторафазный режим // dtc1.calc(&dtc1);
          
          svgen_dqv.Ta = dtc1.Ta;
          svgen_dqv.Tb = dtc1.Tb;
          svgen_dqv.Tc = dtc1.Tc;
          
          // PWM References output
          Pwm.Ta = svgen_dqv.Ta;
          Pwm.Tb = svgen_dqv.Tb;
          Pwm.Tc = svgen_dqv.Tc;
          Pwm.Update(&Pwm);          
          
          // ------------------------------------------------------------------------------
          //      Current RMS calculation
          // ------------------------------------------------------------------------------
          rmsval1.Input  = dPnt->IsV - IfazOffset;
          rmsval1.Period = rcg1.Period;
          rmsval1.calc(&rmsval1);
          
          // ------------------------------------------------------------------------------
          //      Ls calculation
          // ------------------------------------------------------------------------------
          measRP2.Uline       = cntrvolt1.Uline * (1.0/_2SQRT2_SQRT3/_2_SQRT3); 
          measRP2.UlineFlash  = (Tmp1 * (2.0/3.0));
          measRP2.Ifaz        = rmsval1.RMSVal;
          measRP2.IfazFlash   = dPnt->IsV - IfazOffset;
          measRP2.Wout        = FreqRef1.LsCyclFreq;
          measRP2.Period      = rcg1.Period;
          measRP2.Pact1       = measRP2.Uline * IqRr;
          measRP2.calc(&measRP2);
          
          // Передаем в основную часть программы текущую частоту вращения и флаг периода
          DrvConst.We = rcg1.Framp;        // Текущая скорость поля (pu)
          // Флаг периода выходного напряжения
          if(rcg1.Period){
            dPnt->Period = 1;  
          }
          
          timer++;
          
          if(timer >= prmTimes1.LsCalc2) {
            PwmControl.PwmOff(&PwmControl);    // Выкл ШИМ
            
            timer = 0;
            cntrvolt1.reset(&cntrvolt1);
            rcg1.reset(&rcg1);
            //dPnt->Rs = measRs1.Rs;
            //dPnt->Rr = measRP2.Rr;
            //dPnt->Lsd = measRP2.sLr;
            //dPnt->Lsq = dPnt->Lsd; 
            
            GlobalM4.EnginParam.MeasLsd = measRP2.sLr;
            GlobalM4.EnginParam.MeasLsq = measRP2.sLr;
            // Контрольная подмена, иначе стираются параметры
            //GlobalM4.EnginParam.Lls = measRP2.sLr;
            //GlobalM4.EnginParam.Lm = measRP2.sLr;
            //GlobalM4.EnginParam.Rr = GlobalM4.EnginParam.Rs;

            PMSMPrmMotState = PMSMPrmMotStateReady;
          }
        } break;        
                
        //------------------------------------------------------------------------------        
        case PMSMPrmMotStateReady:{          // Параметры успешно измеряны
          DrvWords.Ctrl.bit.vCodeEnb = 1;
          DrvWords.Stat.bit.mWorkFin = 1;
          
          PwmControl.PwmOff(&PwmControl);   // Выкл ШИМ
          
          //-------------------------------------------
          // Оценочный расчет момента инерции          
          // v->Kp = v->Jr/(3 *v->Zp*v->Zp*v->Tsl*v->PsiR) * Kb;          
          Kb = dPnt->FreqBASE / dPnt->CurrBASE;
          GlobalM4.EnginParam.Estim_Jr = GlobalM4.EnginParam.Estim_Kp * (3.0f * dPnt->Zp * dPnt->Zp * dPnt->Tsl * dPnt->Idn)  / Kb;
          //-------------------------------------------
          
          // Вернем настройки ШИМ назад
          PMSMPrmMotState = PMSMPrmInitPwm;
        } break;
    
//------------------------------------------------------------------------------        
        case PMSMPrmDeinitPwm:{              // Перенастроить ШИМ на значение с уставок
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
              PMSMPrmMotState = PMSMPrmMotStateOff;
              
              DrvWords.Ctrl.bit.vCodeEnb = 1;            // Разблокировать вызов обработчика системы регулирования
            }
        } break;

//------------------------------------------------------------------------------        
        case PMSMPrmMotStateOff:{            // Состояние бездействия
      
        } break;
//----------------------------------------------------------------
    default: break;
    }
        
    rcg1.Period = 0; 
}

//// Получить сотояние ШИМ (Вкл/Выкл)
//u8 GetStatusPWM(void){
//  if((PwmControl.StatusPwm == PwmWork) || (PwmControl.StatusPwm == PwmStart)){
//     return 1;     // ШИМ работает!!!
//  }
//  else{
//     return 0;     // ШИМ отключен!!!
//  }  
//}


