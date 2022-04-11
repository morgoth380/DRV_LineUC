#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#ifdef _PROJECT_FOR_STM32_     

#include "Filter.h"
#include "Global_include.h"
#include "measTime.h"
#include "GlobalVar.h"
#include "ScalarControl.h"
#include "MainControl.h"
#include "DrvPWM.h" // Нужна симуляция объекта 
#include "DrvADC.h" // Нужна симуляция объекта
#include "CurVoltMeas.h"
#include "PwmControl.h"
#include "FailsControl.h"
#include "ACI_Vector_Control.h"
#include "PMSM_Vector_Control.h"
#include "FOC_params_copy.h"
#include "ACI_prm_mot.h"
#include "PMSM_prm_mot.h"
#include "TormSW.h"
#include "anti_rollback.h"
#include "Pjk_const.h"
#include "load_loss.h"

// Прототипы функций
void LogN9(void);           // Обработчик логгера в такте ШИМ
void sendExtBlocksProcessingData(void); //Отправка данных в процессор обработки энкодеров
//void AveragingMeasAdcI();   // Усреднение значений от АЦП

// Прототипы обектов
CURVOLTMEAS     Meas            = CURVOLTMEAS_DEFAULTS;       // Обект расчета значений токов и напряжений
PWMCONTROL      PwmControl      = PWMCONTROL_DEFAULTS;                // Обект аппарата состояний модуля ШИМ
ANTIROLLBACK    arollback1	= ANTIROLLBACK_DEFAULTS;
extern SCALARCONTROL Scalar;                   // Обект расчета скалярного управления (без МВ)
extern PWMGEN Pwm;                             // Обект формирователя ШИМ 
extern ADCDRV Adc;
extern TORMSW TormSw;                          // Обект управления тормозным ключем
//extern ANTIROLLBACK    arollback1;

// Define для разрешения расчета такта ШИМа
#define ENABLE_PWM_CALCULATION  0
#define SKIP_PWM_CALCULATION    1

static int      PwmOnOld;

// Обработчик периода ШИМ
void PwmPeriodHandler(void){
  u32           IuSumm, IvSumm, IwSumm, UdSumm, UabSumm, UbcSumm;
  int           i;
  u16           MaxAdcIu_index, MaxAdcIv_index, MaxAdcIw_index, MaxInd;
  f32           MaxAdcIu, MaxAdcIv, MaxAdcIw, AdcIu2, AdcIv2, AdcIw2; 
  u16 static    DCTormSync = 0;
  f32 static    dAngle = 0;  
  u16 static	cntFpwmLock = 0; // Счетчик защелка для блокировки работы по усредненным значениям АЦП при смене частоты ШИМа
  f32           localFpwmC28, MaxFpwmFrequency, PresentPwmTime;
  u32           UdSuperfast;
  f32           testUab, testUbc;
  f32 static    testFi_current=0, testFi_voltage=0, Iutest=0, Ivtest=0, Iwtest=0;
  u16           SkipPwmCalculation;
  
    
  // 1) Проверить флаг необходимости переинициализации системы регулирования
  if(GlobalM4.FlagCommand.bit.SysCalc){
    // Обновить настройки модуля ШИМ   
    // Перенастроить систему регулирования
    FOC_ReCalc_func();
    GlobalM4.FlagCommand.bit.SysCalc = 0;
    return;
  }
       
  //===================================================
  // Выполнить смену частоты ШИМА
  //===================================================
  // Нужна локальная частота ШИМа
  // Нужна перестройки частоты выборки АЦП !!!  
  
  SkipPwmCalculation = ENABLE_PWM_CALCULATION; 
 
  if ( !DrvWords.Ctrl.bit.mWorkEnb ) {  // Если у нас не измерение параметров, то мы всегда корректируем частоту ШИМа в зависимости от текущей частоты. И не важно какой режим управления
    if (cntFpwmLock) cntFpwmLock--; // Уменьшаем защелку
    localFpwmC28 = GlobalM4.VariableFpwm;
    
    // Тестовый режим изменения частоты ШИМа на частотах ниже 1 Гц
    if (GlobalM4.PwmDCmode.PwmDCenable == 1) {
      if (fabsf(DrvConst.We) < _1_Hz) 
        localFpwmC28 = GlobalM4.PwmDCmode.FpwmDC;
      else 
        localFpwmC28 = GlobalM4.Fpwm;
    } else {
      localFpwmC28 = GlobalM4.Fpwm;
    }
        
    // Переинициализация ШИМа, если она необходима, и изменение частоты выборки для АЦП
    if ( Pwm.Fpwm_Hz != localFpwmC28 ) { // Проверяем предыдущую частоту ШИМа и текущую
      
      // Делаем логическое ограничение ШИМа здесь !!! В драйвере переинициализации также его добавим, но надо убедиться что все будут брать новую частоту ШИМ оттуда
      // Расчет защитной частоты ШИМа
      PresentPwmTime = GlobalM4.MeasTime[PwmPeriodProcessingTC].ValTekTime * 0.1f; // Получаем значение такта выполнения ШИМа за текущий такт
      MaxFpwmFrequency = 1000000 / (PresentPwmTime * _FPWM_SAFETY_FACTOR); // Находим максимальную частоту, которая допустима в данный момент. С запасом на вычисления      
      // Сравниваем - если новая частота ШИМа больше предельной, то ставим предельное значение частоты ШИМа.
      if (localFpwmC28 > MaxFpwmFrequency)      localFpwmC28 = MaxFpwmFrequency;      
      if (localFpwmC28 < _FPWM_SAFETY_FACTOR)   localFpwmC28 = _FPWM_SAFETY_FACTOR;
      
      // Применяем новую частоту ШИМа
      GlobalM4.VariableFpwm = localFpwmC28; // Запоминаем новую частоту ШИМа
      cntFpwmLock = 10; // Запрет получения усредненных значений АЦП на 3 такта ШИМа
      SkipPwmCalculation = SKIP_PWM_CALCULATION; // Блокируем расчет такта ШИМа на 1 такт, для выполнения пересчета данных в DriveInit
      // Выполняем переинициализацию
      DrvConst.Fpwm = GlobalM4.VariableFpwm;
      DrvConst.Tpwm = 1.0f / DrvConst.Fpwm;
      // расчет прироста угла выходного поля скаляра
      DrvConst.StepAngle = DrvConst.Tpwm; // Частоты для скаляра идут в Гц, так что база не нужна          
      
      Pwm.DeadTime_uS = DrvConst.DeadTimeInit.DeadTime;
      Pwm.Fpwm_kHz = DrvConst.Fpwm * 0.001f;
      Pwm.Fpwm_Hz = DrvConst.Fpwm;
      Pwm.InitVarPwm(&Pwm);                                
      Adc.Fpwm_kHz = Pwm.Fpwm_kHz;  // Получить частоту ШИМ модуля PWM (для синхронизации АЦП с ШИМ)
      Adc.ReInitVarPwm(&Adc);                    
    }              
  }  
  //===================================================   
  
  // 2) Усреднение значений от АЦП
  IuSumm = IvSumm = IwSumm = UdSumm = UabSumm = UbcSumm = 0;   // инициализация интеграторов

  for( i=0; i<SIZE_BUF_UVWUD; ){
    IuSumm  += Adc3Buff[i++];
    IvSumm  += Adc3Buff[i++];
    IwSumm  += Adc3Buff[i++];
    UdSuperfast = Adc3Buff[i];
    UdSumm  += Adc3Buff[i++];    
    UabSumm += Adc3Buff[i++];
    UbcSumm += Adc3Buff[i++];
  }
     
  UdSuperfast = (u32)((Adc3Buff[SIZE_BUF_UVWUD-3] + Adc3Buff[SIZE_BUF_UVWUD-COUNT_ADC_CHANNEL-3]) / 2.0f); // Udc идет третим с конца, поэтому такая сдвижка
  
  if (GlobalM4.FailData.FailsFlag.bit.MtzComp == 1) { // Если поймали МТЗ компаратора, делаем поиск максимального тока
    MaxAdcIu = MaxAdcIv = MaxAdcIw = MaxAdcIu_index = MaxAdcIv_index = MaxAdcIw_index = 0; // Обнулить значения
    // COUNT_SEMPL              16                               // Количество семплов одного канала измерения в буфере
    // COUNT_ADC_CHANNEL        6                                // I_U, I_V, I_W, Ud, Uab, Ubc
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
     
  //===========================================================================
  // Режим симуляции работы привода
  //===========================================================================  
  
  if (GlobalM4.Simulation.Enable) {
    if (!GlobalM4.FlagCommand.bit.PwmOn && !DrvWords.Ctrl.bit.mWorkEnb){
      Iutest = Ivtest = Iwtest=0;
      IuSumm = ((u32)(GlobalM4.ValIu.OffSet) << 4);
      IvSumm = ((u32)(GlobalM4.ValIv.OffSet) << 4);
      IwSumm = ((u32)(GlobalM4.ValIw.OffSet) << 4);
    } else {
      testFi_current = testFi_current + _2PI * DrvConst.Wrz * DrvConst.FreqBASE * DrvConst.Tpwm;
      if (testFi_current > _2PI) {
        testFi_current -= _2PI;
      }  
      Iutest = Pwm.Ta;
      Ivtest = Pwm.Tb;
      Iwtest = Pwm.Tc;
                 
      IuSumm = ((u32)(Iutest*(GlobalM4.Simulation.Imot/ DrvConst.CurrBASE / GlobalM4.ValIu.Gain) + GlobalM4.ValIu.OffSet) << 4); // 500.0f
      IvSumm = ((u32)(Ivtest*(GlobalM4.Simulation.Imot/ DrvConst.CurrBASE / GlobalM4.ValIv.Gain) + GlobalM4.ValIv.OffSet) << 4);
      IwSumm = ((u32)(Iwtest*(GlobalM4.Simulation.Imot/ DrvConst.CurrBASE / GlobalM4.ValIw.Gain) + GlobalM4.ValIw.OffSet) << 4);
    }
    
    // Иммитация Напряжение в ЗПТ                  
    UdSuperfast = (u16)(GlobalM4.Simulation.Udc / GlobalM4.ValUd.Gain);  

    // Иммитация напряжения сети
    testFi_voltage = testFi_voltage + 314.15f * DrvConst.Tpwm;
    if (testFi_voltage > _2PI) {
      testFi_voltage -= _2PI;
    }  
    testUab = arm_sin_f32(testFi_voltage);
    testUbc = arm_sin_f32(testFi_voltage-_DEG_TO_RAD*60);    
    UabSumm = ((u32)(testUab*(GlobalM4.Simulation.Uin/_SQRT_2 / GlobalM4.ValUab.Gain) + 2048) << 4);
    UbcSumm = ((u32)(testUbc*(GlobalM4.Simulation.Uin/_SQRT_2 / GlobalM4.ValUbc.Gain) + 2048) << 4);        
  }
  //=========================================================================== 
  // Конец режим симуляции работы привода
  //===========================================================================  
  
  // Передать усредненные значения АЦП для расчета мгновенных значений токов и Ud
  Meas.Iu_adc = GlobalM4.ValIu.AdcVal = IuSumm >> 4;
  Meas.Iv_adc = GlobalM4.ValIv.AdcVal = IvSumm >> 4;
  Meas.Iw_adc = GlobalM4.ValIw.AdcVal = IwSumm >> 4;
  Meas.Ud_adc = GlobalM4.ValUd.AdcVal = UdSuperfast; //UdSumm >> 4; // UdSuperfast;
  #ifdef _PROJECT_FOR_STM32_     
  #warning А какой выбор делать при измерении входных напряжений для РВ24_М40??
  #endif
  if (Meas.PowerType == HighPow_Combo) { // Combo Uab, Uac
      Meas.Uab_adc = GlobalM4.ValUab.AdcVal = UabSumm >> 4;
      Meas.Uca_adc = GlobalM4.ValUca.AdcVal = UbcSumm >> 4; 
  } else { // PB24 - Uab, Ucb
      Meas.Uab_adc = GlobalM4.ValUab.AdcVal = UabSumm >> 4;
      Meas.Ubc_adc = GlobalM4.ValUbc.AdcVal = UbcSumm >> 4;    
  }
   
  // Передать в объект дополнительные значения
  Meas.Flags.Period = DrvConst.Period;
  Meas.Flags.CurInv = GlobalM4.FlagCommand.bit.CurrInv;    // Флаг необходимости инвертирования датчиков тока
  Meas.Fout  = GlobalM4.We;                                // указать знак выходной частоты (для расчета реактивного тока)
  Meas.Angle = DrvConst.Angle;
  Meas.Us = GlobalM4.Uout;                                 // выходное напряжение ПЧ, для расчета мощности  
  Meas.Tsample = DrvConst.Tpwm; //DrvConst.StepAngle;                       // Фактически шаг угла равен периоду ШИМ 
  Meas.Iu_adcParam.Gain = GlobalM4.ValIu.Gain;                    
  Meas.Iv_adcParam.Gain = GlobalM4.ValIv.Gain;                    
  Meas.Iw_adcParam.Gain = GlobalM4.ValIw.Gain;
  Meas.Uab_adcParam.Gain = GlobalM4.ValUab.Gain;                    
  Meas.Ubc_adcParam.Gain = GlobalM4.ValUbc.Gain;
  Meas.Uca_adcParam.Gain = GlobalM4.ValUca.Gain;
  Meas.UdGain = GlobalM4.ValUd.Gain;
  
  //Meas.Flags.DrvStart = GlobalM4.FlagCommand.bit.PwmOn | DrvWords.Ctrl.bit.mWorkEnb;
    
  if(PwmControl.StatusPwm >= PwmStart){
     Meas.Flags.DrvStart = 1;
  }else{
     Meas.Flags.DrvStart = 0;
  }   
  Meas.fast_calc(&Meas);
      
  // Расчет RMS
  if(DrvConst.Period){        // вывод флага периода
     DrvConst.Period = 0;
     Meas.period_calc(&Meas);
  }
 
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
  
  // Сохранить результаты расчетов
//  GlobalM4.ValIu.Val = Meas.Iu_RMS;           
//  GlobalM4.ValIv.Val = Meas.Iv_RMS;                    
//  GlobalM4.ValIw.Val = Meas.Iw_RMS;                    
  GlobalM4.ValUab.Val = Meas.Uab_RMS;                    
  GlobalM4.ValUbc.Val = Meas.Ubc_RMS;                    
  GlobalM4.ValUca.Val = Meas.Uca_RMS;                    
  GlobalM4.ValUd.Val = Meas.Udc; //Meas.UdcFast; //Meas.Udc;
  
  GlobalM4.ValIu.FlashVal = Meas.Iu;              
  GlobalM4.ValIv.FlashVal = Meas.Iv;              
  GlobalM4.ValIw.FlashVal = Meas.Iw;
  GlobalM4.ValUab.FlashVal = Meas.Uab;              
  GlobalM4.ValUbc.FlashVal = Meas.Ubc;              
  GlobalM4.ValUca.FlashVal = Meas.Uca;
  GlobalM4.ValUd.FlashVal = DrvConst.Udc = Meas.UdcFast;

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
  GlobalM4.ValUab.OffSet = Meas.Uab_adcParam.Offset;                    
  GlobalM4.ValUbc.OffSet = Meas.Ubc_adcParam.Offset;                    
  GlobalM4.ValUca.OffSet = Meas.Uca_adcParam.Offset;                    
    
  GlobalM4.UinPhasingFlag = (UinPhasing_type)Meas.UinPhasingFlag;
    
  //----------------------------------------------  
  loadloss1.Tpwm = DrvConst.Tpwm;
  loadloss1.Fout = GlobalM4.We;
  loadloss1.currentThetaI = Meas.ThetaI;
  loadloss1.PwmOn = Pwm.PwmOn;
  loadloss1.calc(&loadloss1);
  GlobalM4.LoadLossFault = loadloss1.Fault;
  
  // Обработчик быстрых аварий
  FailsControl();
  
  // Обработка блоков расширения
  sendExtBlocksProcessingData(); //Отправка данных в блоки расширения  
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
  CopyParams_FromM4toDMC();        // Копирование данных в обменную структуру с переводом в о.е.
  CopyFlags_FromM4toDMC();         // Копирование флагов управления в обменную структуру системы регулирования
    
  dAngle = (DrvConst.Angle - dAngle);          // Определили приращение угла
  if (GlobalM4.DCBrake.ActionDC){ // Торможение ПТ только для асинхронников, активируется в DRV_V5 /*&& (TYPEDRV_ANYACIM & (1 << DrvConst.DriveType))*/
      DC_Brake_Calc(DCTormSync, dAngle, GlobalM4.DCBrake.Current); // Фаза у меня уже в dPnt->Angle          
      DCTormSync = 1;
  } else {
      switch (DrvConst.DriveType){
          case TYPE_SCALAR_NO_DEADBAND:
          case TYPE_SCALAR:
            if (GlobalM4.FlagCommand.bit.PwmOn) {             
              DrvWords.Stat.bit.vPrUP = 1; // Для синхронника надо сразу поставить наличие возбуждения, после подачи ШИМа, иначе часть алгоритмов будет работать с ошибкой, особенно в лифте              
            } else {
              DrvWords.Stat.bit.vPrUP = 0;
              DrvConst.Angle = 0;
            }
            // и сразу переходим на общий вариант Асинхронника
          case TYPE_ACIM_FOC_SENSORED:
          case TYPE_ACIM_FOC_SENSORLESS:
              dAngle = DrvConst.Angle;          // Запомнили последний угол                            
              if (SkipPwmCalculation == SKIP_PWM_CALCULATION) {              
                DrvWords.Ctrl.bit.SysCalc = 1;
                ACIM_Vector_Init();
                DrvWords.Ctrl.bit.SysCalc = 0;
              } else {              
                ACIM_PrmMot_Calc();              // Вызов системы измирения параметров двигателя
                ACIM_Vector_Calc();              // Вызов системы регулирования   
              }
              DCTormSync = 0;
          break;

          case TYPE_PMSM_FOC_SENSORED:  
          case TYPE_PMSM_FOC_SENSORLESS:            
              if (SkipPwmCalculation == SKIP_PWM_CALCULATION) {              
                DrvWords.Ctrl.bit.SysCalc = 1;
                PMSM_Vector_Init();
                DrvWords.Ctrl.bit.SysCalc = 0;
              } else {           
                PMSM_Vector_Calc();
                PMSM_PrmMot_Calc();
              }
              
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
       PwmControl.PwmOn(&PwmControl);     // Вкл ШИМ       
     }else{
       PwmControl.PwmOff(&PwmControl);     // Выкл ШИМ
     }
  } else if (!GlobalM4.FlagCommand.bit.PwmOn && !DrvWords.Ctrl.bit.mWorkEnb){ // Дополнительное отключение ШИМа в останове
    PwmControl.PwmOff(&PwmControl);     // Выкл ШИМ
    Pwm.Off(&Pwm);
  }
  PwmOnOld = GlobalM4.FlagCommand.bit.PwmOn; 
  
  PwmControl.PwmStateMachine(&PwmControl);    // Вызов аппарата состояний ШИМ
  
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
  //TormSw.State
  // Логгер
  LogN9();
}

// Прерывание в такте ШИМ
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
  
    if(htim->Instance == TIM1){
      // Измерение периода ШИМ
      EndTimControl(PwmPeriodTC); 
      StartTimControl(PwmPeriodTC);
      
      StartTimControl(PwmPeriodProcessingTC);       // Начать измерять время выполнения обработчика ШИМ

      PwmPeriodHandler();                           // Обработчик периода ШИМ        
          
      EndTimControl(PwmPeriodProcessingTC);         // Остановить измерение времени выполнения      
    }
}
#endif

#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)