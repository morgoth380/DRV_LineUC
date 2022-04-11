#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "Global_include.h"
#include "measTime.h"
#include "GlobalVar.h"
#include "ScalarControl.h"
#include "MainControl.h"
#include "DrvPWM.h"
#include "DrvADC.h"
#include "PwmControl.h"
#include "CurVoltMeas.h"
#include "FailsControl.h"
#include "ACI_Vector_Control.h"
#include "FOC_params_copy.h"
#include "ACI_prm_mot.h"
#include "TormSW.h"
#include "anti_rollback.h"


// Прототипы функций
void LogN9(void);           // Обработчик логгера в такте ШИМ
void sendExtBlocksProcessingData(void); //Отправка данных в процессор обработки энкодеров
//void AveragingMeasAdcI();   // Усреднение значений от АЦП

CURVOLTMEAS Meas = CURVOLTMEAS_DEFAULTS;                    // Обект расчета значений токов и напряжений
PWMCONTROL PwmControl = PWMCONTROL_DEFAULTS;                // Обект аппарата состояний модуля ШИМ
ANTIROLLBACK    arollback1	= ANTIROLLBACK_DEFAULTS;

extern SCALARCONTROL Scalar;                                // Обект расчета скалярного управления (без МВ)
extern PWMGEN Pwm;                                          // Обект формирователя ШИМ 
extern ADCDRV Adc;
extern TORMSW TormSw;                                       // Обект управления тормозным ключем
//extern u16     MODBUS_FILE_RDWR;
//extern u16     MODBUS_PWM_ON;

static int PwmOnOld;



// Обработчик периода ШИМ
void PwmPeriodHandler(void){
  u32 IuSumm, IvSumm, IwSumm, UdSumm;
  int i;
  u16 static DCTormSync = 0;
  f32 static dAngle = 0;
  //u16 static  extCnt=0;
  u16   MaxAdcIu_index, MaxAdcIv_index, MaxAdcIw_index, MaxInd;
  f32   MaxAdcIu, MaxAdcIv, MaxAdcIw, AdcIu2, AdcIv2, AdcIw2;

    
  // 1) Проверить флаг необходимости переинициализации системы регулирования
  if(GlobalM4.FlagCommand.bit.SysCalc){
    
    FOC_ReCalc_func();                      // Перенастроить систему регулирования
    
    GlobalM4.FlagCommand.bit.SysCalc = 0;
     return;
  }
  
  // 2) Усреднение значений от АЦП
  IuSumm = IvSumm = IwSumm = UdSumm = 0;   // инициализация интеграторов
  
  for(i=0; i<SIZE_BUF_UVWUD; ){
    IuSumm += Adc3Buff[i++];
    IvSumm += Adc3Buff[i++];
    IwSumm += Adc3Buff[i++];
    UdSumm += Adc3Buff[i++];
  }
  
  if (GlobalM4.FailData.FailsFlag.bit.MtzComp == 1) { // Если поймали МТЗ компаратора, делаем поиск максимального тока
    MaxAdcIu = MaxAdcIv = MaxAdcIw = MaxAdcIu_index = MaxAdcIv_index = MaxAdcIw_index = 0; // Обнулить значения
    // COUNT_SEMPL              16                               // Количество семплов одного канала измерения в буфере
    // COUNT_ADC_CHANNEL        4                                // I_U, I_V, I_W, Ud, Uab, Ubc
    // SIZE_BUF_UVWUD           COUNT_SEMPL * COUNT_ADC_CHANNEL    
    
    for( i=0; i<COUNT_SEMPL; i++){ // Сканируем по одинаковым выборкам времени
        MaxInd = i*COUNT_ADC_CHANNEL;
        // Фаза U
        AdcIu2  = (f32)(Adc3Buff[MaxInd+0] - Meas.Iu_adcParam.Offset); // Находим квадрат значения АЦП относительно нуля АЦП
        AdcIu2  = AdcIu2 * AdcIu2; // Модуль АЦП тока
        if (AdcIu2 > MaxAdcIu) { // Фиксируем индекс максимума
          MaxAdcIu = AdcIu2; 
          MaxAdcIu_index = i;
        } 
        // Фаза V
        AdcIv2  = (f32)(Adc3Buff[MaxInd+1] - Meas.Iv_adcParam.Offset); // Находим квадрат значения АЦП относительно нуля АЦП
        AdcIv2  = AdcIv2 * AdcIv2; // Модуль АЦП тока
        if (AdcIv2 > MaxAdcIv) { // Фиксируем индекс максимума
          MaxAdcIv = AdcIv2; 
          MaxAdcIv_index = i;
        }         
        // Фаза W
        AdcIw2  = (f32)(Adc3Buff[MaxInd+2] - Meas.Iw_adcParam.Offset); // Находим квадрат значения АЦП относительно нуля АЦП
        AdcIw2  = AdcIw2 * AdcIw2; // Модуль АЦП тока
        if (AdcIw2 > MaxAdcIw) { // Фиксируем индекс максимума
          MaxAdcIw = AdcIw2; 
          MaxAdcIw_index = i;
        }                 
    } 
    // Поиск закончили, теперь давай найдем какой ток выше
    if (MaxAdcIu > MaxAdcIv && MaxAdcIu > MaxAdcIw) {
        MaxInd = MaxAdcIu_index; 
    } else if (MaxAdcIv > MaxAdcIu && MaxAdcIv > MaxAdcIw) {
        MaxInd = MaxAdcIv_index; 
    } else {
      MaxInd = MaxAdcIw_index; 
    }
    // Находим начальный индекс для выборки
    MaxInd = MaxInd*COUNT_ADC_CHANNEL;
    // Делаем иммитацию усреденного значения - учитывать величину COUNT_SEMPL!!!
    IuSumm =  ((u32)Adc3Buff[MaxInd+0]) << 4;
    IvSumm =  ((u32)Adc3Buff[MaxInd+1]) << 4;
    IwSumm =  ((u32)Adc3Buff[MaxInd+2]) << 4;
  }  
  
  // Передать усредненные значения АЦП для расчета мгновенных значений токов и Ud
  Meas.Iu_adc = GlobalM4.ValIu.AdcVal = IuSumm >> 4;
  Meas.Iv_adc = GlobalM4.ValIv.AdcVal = IvSumm >> 4;
  Meas.Iw_adc = GlobalM4.ValIw.AdcVal = IwSumm >> 4;
  Meas.Ud_adc = GlobalM4.ValUd.AdcVal = UdSumm >> 4;
  
  // Передать в обект дополнительные значения
  Meas.Flags.Period = DrvConst.Period;
  Meas.Flags.CurInv = GlobalM4.FlagCommand.bit.CurrInv;    // Флаг необходимости инвертирования датчиков тока  
  Meas.Fout  = GlobalM4.We;                            // указать знак выходной частоты (для расчета реактиного тока)
  Meas.Angle = DrvConst.Angle;
  Meas.Us = GlobalM4.Uout;                                 // выходное напряжение ПЧ, для расчета мощности  
  Meas.Tsample = GlobalM4.StepAngle;                     // Фактически шаг угла равен периоду ШИМ 
  Meas.Iu_adcParam.Gain = GlobalM4.ValIu.Gain;                    
  Meas.Iv_adcParam.Gain = GlobalM4.ValIv.Gain;                    
  Meas.Iw_adcParam.Gain = GlobalM4.ValIw.Gain;                    
  Meas.UdGain = GlobalM4.ValUd.Gain;
  
  if(PwmControl.StatusPwm >= PwmStart){
     Meas.Flags.DrvStart = 1;  
  }
  else{
     Meas.Flags.DrvStart = 0;
  }  
  //Meas.Ud_adc  *= GlobalM4.AdcKcor;
  Meas.fast_calc(&Meas);
  
  // Расчет RMS
  if(DrvConst.Period){        // вывод флага периода
     DrvConst.Period = 0;
     Meas.period_calc(&Meas);
  }
 
  // Сохранить результаты расчетов                  
  GlobalM4.ValUd.Val = Meas.Udc;  
  GlobalM4.ValIu.FlashVal = Meas.Iu;              
  GlobalM4.ValIv.FlashVal = Meas.Iv;              
  GlobalM4.ValIw.FlashVal = Meas.Iw;
  
//  Для вывода постоянки, надо изменить формат уставок  
  if (DrvConst.We == 0) {
    GlobalM4.ValIu.Val = Meas.Iu;                    
    GlobalM4.ValIv.Val = Meas.Iv;                    
    GlobalM4.ValIw.Val = Meas.Iw;                    
  } else {    
    GlobalM4.ValIu.Val = Meas.Iu_RMS;                    
    GlobalM4.ValIv.Val = Meas.Iv_RMS;                    
    GlobalM4.ValIw.Val = Meas.Iw_RMS;                    
  }  
  
  GlobalM4.ValUd.FlashVal = Meas.UdcFast;
  GlobalM4.Iflash = Meas.Iflash;
  GlobalM4.Iact = Meas.Iact;
  GlobalM4.Ireact = Meas.Ireact;
  GlobalM4.Ifull = Meas.Ifull;
  GlobalM4.cosFi = Meas.cosFi;
  
  GlobalM4.Pfull = Meas.Pfull;
  GlobalM4.Pact = Meas.Pact;
  GlobalM4.Preact = Meas.Preact;
  
  GlobalM4.ValIu.OffSet = Meas.Iu_adcParam.Offset;                    
  GlobalM4.ValIv.OffSet = Meas.Iv_adcParam.Offset;                    
  GlobalM4.ValIw.OffSet = Meas.Iw_adcParam.Offset;                    
    
  // Обработчик быстрых аварий
  FailsControl();
  
  sendExtBlocksProcessingData(); //Отправка данных в процессор обработки энкодеров
  
//------------------------------------------------------------------------
//                  VECTOR CONTROL
//------------------------------------------------------------------------
  //Обработка антиотката
  arollback1.flag.ARollbackEnb 	= DrvWords.Ctrl.bit.ForceEnb;
  arollback1.Time              	= GlobalM4.SpdForceTime;		
  arollback1.TimeStep           = DrvConst.Tpwm;
  arollback1.flag.PwmOn 	= GlobalM4.FlagCommand.bit.PwmOn;
  // Выбираем алгоритм запуска Антиотката
  if (GlobalM4.PresentFunctional == UseLiftFunc) { // Если у меня работает как лифт
    arollback1.flag.Start = GlobalM4.FlagCommand.bit.BreakOff; // То старт по снятию тормоза
  } else { // Если другие режимы работы - то зависит от типа двигателя
    if ( TYPEDRV_ACIMFOC & 1<<DrvConst.DriveType ) { // Если это Асинхронник
      arollback1.flag.Start         = DrvWords.Stat.bit.vPrUP; // Как только возбудился двигатель, разрешаем Включить антиоткат
    } else if ( TYPEDRV_PMSMFOC & 1<<DrvConst.DriveType ) { // Если синхронник - то сразу включаем
      arollback1.flag.Start         = GlobalM4.FlagCommand.bit.PwmOn;    
    }
  }
  // Вызываем обработчик Антиотката
  arollback1.calc(&arollback1);        
     
  // передача быстрых параметров
  
  // Система регулирования
  CopyParams_FromM4toDMC();        // Копирование данных в обменную структуру
  CopyFlags_FromM4toDMC();         // Копирование флагов управления в обменную структуру системы регулирования
    
  dAngle = (DrvConst.Angle - dAngle);          // Определили приращение угла
  if (GlobalM4.DCBrake.ActionDC && (TYPEDRV_ANYACIM & (1 << DrvConst.DriveType))){ // Торможение ПТ только для асинхронников
      DC_Brake_Calc(DCTormSync, dAngle, GlobalM4.DCBrake.Current); // Фаза у меня уже в dPnt->Angle          
      DCTormSync = 1;
  } else {
      switch (DrvConst.DriveType){
          case TYPE_SCALAR_NO_DEADBAND:
          case TYPE_SCALAR:
            if (GlobalM4.FlagCommand.bit.PwmOn)             
              DrvWords.Stat.bit.vPrUP = 1; // Для синхронника надо сразу поставить наличие возбуждения, после подачи ШИМа, иначе часть алгоритмов будет работать с ошибкой, особенно в лифте              
            else 
              DrvWords.Stat.bit.vPrUP = 0;
            
          case TYPE_ACIM_FOC_SENSORED:
          case TYPE_ACIM_FOC_SENSORLESS:
              dAngle = DrvConst.Angle;          // Запомнили последний угол              
              ACIM_PrmMot_Calc();              // Вызов системы измирения параметров двигателя
              ACIM_Vector_Calc();              // Вызов системы регулирования   
              DCTormSync = 0;
              break;

          case TYPE_PMSM_FOC_SENSORED:
          case TYPE_PMSM_FOC_SENSORLESS:
//              PMSM_Vector_Calc();
//              PMSM_PrmMot_Calc();
            
            if (GlobalM4.FlagCommand.bit.PwmOn)             
              DrvWords.Stat.bit.vPrUP = 1; // Для синхронника надо сразу поставить наличие возбуждения, после подачи ШИМа, иначе часть алгоритмов будет работать с ошибкой, особенно в лифте              
            else 
              DrvWords.Stat.bit.vPrUP = 0;
            
              break;
    }
  }                    
    
  CopyParams_FromDMCtoM4();
  CopyFlags_FromDMCtoM4();         // Копирование флагов управления с системы регулирования в глобальную структуру
      
  // Отработка команд Пуск/Стоп
  if(GlobalM4.FlagCommand.bit.PwmOn != PwmOnOld) {
     if (GlobalM4.FlagCommand.bit.PwmOn) {
        PwmControl.PwmOn(&PwmControl);  
     }else{
        PwmControl.PwmOff(&PwmControl); 
     }
  } else if (!GlobalM4.FlagCommand.bit.PwmOn && !DrvWords.Ctrl.bit.mWorkEnb){ // Дополнительное отключение ШИМа в останове
    PwmControl.PwmOff(&PwmControl);     // Выкл ШИМ
    Pwm.Off(&Pwm);
  }
  PwmOnOld = GlobalM4.FlagCommand.bit.PwmOn; 
 
  PwmControl.PwmStateMachine(&PwmControl);    // Вызов аппарата состояний ШИМ
  
// // Обработка тормозного ключа
//  TormSw.UdFast = GlobalM4.ValUd.FlashVal;
//  TormSw.UopenSw = GlobalM4.TormSwParam.UOn; 
//  TormSw.UcloseSw = GlobalM4.TormSwParam.UOff;
//  TormSw.EnSw = GlobalM4.FlagCommand.bit.TormSwEnable;
//  TormSw.Control(&TormSw);

  // Обработка тормозного ключа
  TormSw.UdFast         = GlobalM4.ValUd.FlashVal;
  TormSw.UopenSw        = GlobalM4.TormSwParam.UOn; 
  TormSw.UcloseSw       = GlobalM4.TormSwParam.UOff;
  TormSw.PwmOn          = Pwm.PwmOn;
  TormSw.Fpwm           = DrvConst.Fpwm;
  TormSw.EnSw           = GlobalM4.FlagCommand.bit.TormSwEnable; // Разрешение работы тормозного ключа
  TormSw.TormCheck      = GlobalM4.TormSwParam.TormCheckStart; // Команда на запуск тестирования тормозного резистора
  TormSw.Control(&TormSw);  
  GlobalM4.TormSwParam.TormCheckEnd = (TormSw.TestResResult != Processing) ? 1 : 0; // Если тест завершен, ставим флаг для командоаппарата.
  
 
  // Логгер
  LogN9();
}

// Прерывание в такте ШИМ
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
  
    if(htim->Instance == TIM1){
      // Измирение периода ШИМ
      EndTimControl(PwmPeriodTC); 
      StartTimControl(PwmPeriodTC);
      
      StartTimControl(PwmPeriodProcessingTC);       // Начать измерять время выполнения обработчика ШИМ

      PwmPeriodHandler();                           // Обработчик периода ШИМ        
          
      EndTimControl(PwmPeriodProcessingTC);         // Остановить измирение времени выполнения
    }
}
#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)