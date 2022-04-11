#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "Task20ms.h"
#include "Global_Include.h"
#include "DrvPWM.h"
#include "DrvADC.h"
#include "CurVoltMeas.h"
#include "GlobalVar.h"
#include "InputOutput.h"
#include "TormSW.h"
#include "DrvLedStatus.h"
#include "DrvDinDout.h"
#include "DrvErrSWandSTO.h"
#include "DrvFan.h"
#include "DrvDiscrT.h"
#include "DrvTypePow.h"
#include "DrvCharge.h"
#include "DrvBackupPowerCont.h"
#include "DrvExtAnalogMK.h"
#include "TaskMicroProg.h"
#include "DrvCompZPT.h"
#include "DRIVE_CONST.h"
#include "DRIVE_WORDS.h"

// Definitions for TimerTask20ms 
osTimerId_t TimerTask20msHandle;
const osTimerAttr_t TimerTask20ms_attributes = {
  .name = "TimerTask20ms"
};

// Definitions for semStartTask20ms 
osSemaphoreId_t semStartTask20msHandle;
const osSemaphoreAttr_t semStartTask20ms_attributes = {
  .name = "semStartTask20ms"
};


void CallbackTimerTask20ms(void *argument);    // Програмный таймер с периодом 20мс
void UART5_Init(void);
void USART2_Init(void);
void ReInitMTZcomp(void);                      // Перенастроить МТЗ компаратора  

COMPZPTDRV CompZPT = COMPZPTDRV_DEFAULTS;      // Обект компаратора в ЗПТ
PWMGEN Pwm = PWMGEN_DEFAULTS;                  // Обект настройки генератора PWM
ADCDRV Adc = ADCDRV_DEFAULTS;                  // Обект настройки модуля АЦП
TORMSW TormSw = TORMSW_DEFAULTS;               // Обект управления тормозным ключем
LED LedStatus = LED_DEFAULTS;                  // Обект управления светодиодами на передней панели ПЧ
DINDOUT DinDout = DIN_DOUT_DEFAULTS;           // Обект управления DinDout
SWANDSTO SwAndSto = SWANDSTO_DEFAULTS;         // Обект управления авариями ключей и STO
FAN FanControl = FAN_DEFAULTS;                 // Обект управления вентеляторами
TDISCRDRV TDiscr = TDISCRDRV_DEFAULTS;         // Обект управления дискретным датчиком температуры
DRVTYPE DrvType = DRVTYPE_DEFAULTS;            // Обект для получения типа ПЧ и управления квитирования аварий ключей для мощных ПЧ 
CHARGE Charge = CHARGE_DEFAULTS;               // Обект управления зарядом 
BACKUPPOW BackupPowStat = BACKUPPOW_DEFAULTS;  // Обект контроля наличия резервного питания
EXTANALOG Mkar = EXTANALOG_DEFAULTS;           // Обект для обмена с MKAR

extern CURVOLTMEAS Meas;                       // Обект расчета значений токов и напряжений 
// Global Variables
extern DRIVECONST  DrvConst;          // Структура обменная 

extern osSemaphoreId_t semWaitPerifInitHandle; // Симафор для разблокирование задачи МП

// Фоновая задача 20ms
void Func20ms(void *argument){
  DrvType_Enum  TypeDrv; 
    
//  // 1) Получить тип ПЧ
//  DrvType.Init(&DrvType);
//  // Получить тип ПЧ
//  TypeDrv = DrvType.GetType(&DrvType);
//    
//  GlobalM4.DrvType = TypeDrv;
//  // Сквитировать аварию ключей (на всякий случай)
//  DrvType.AskErr(&DrvType);       
  
  // 2) Настроить драйвер управления зарядом
  Charge.init(&Charge);
  
  // 3) Настроить драйвер контроля резервного питания блока 
  BackupPowStat.Init(&BackupPowStat);
  
  // 4) Настроить драйвер управления светодиодами статуса
  LedStatus.init(&LedStatus);
    
  // 5) Настроить драйвер для управления DinDout
  DinDout.init(&DinDout);
    
  // 7) Настроить обработчик Tdiscr
  TDiscr.Init(&TDiscr);
    
  // 8) Объект обмена с MKAR
  Mkar.init(&Mkar);
  
  // 9) Настроить тормозной ключ
  TormSw.Init(&TormSw);
  
  // Разлочить задачу МП
  osSemaphoreRelease(semWaitPerifInitHandle);   // Установить симафор;
  
  // 11) Ждем запуска МП
  osDelay(20);        // Для инициализации значений в глобальной структуре значениями из системы параметров  
  //----------------------------------------
  // После получения данных из микропрограммы, определяем с каким типом силового блока будем работать
  // 1) Получить тип ПЧ
  DrvType.DrvTypePreset = GlobalM4.DrvTypePreset;
  DrvType.Init(&DrvType);
  // Получить тип ПЧ
  TypeDrv = DrvType.GetType(&DrvType);
    
  GlobalM4.DrvType = TypeDrv;
  // Сквитировать аварию ключей (на всякий случай)
  DrvType.AskErr(&DrvType);  
  
  // 6) Настроить обработчик аварии ключей и STO
  SwAndSto.init(&SwAndSto);
  
  // 10) Настроить драйвер управления вентеляторами охлаждения
#warning Настройка вентиляторов для силовых блоков РВ24
  if(TypeDrv == LowPow_PB24){
     FanControl.Fpwm = 10.0f;      // в кГц
  }
  else{
     FanControl.Fpwm = 2.5f;       // в кГц
  } 
  FanControl.init(&FanControl);
  //----------------------------------------
  
  // 12) Запустить таймер формирования ШИМ (T1)
  Pwm.DeadTime_uS = GlobalM4.DeadTimeInit.DeadTime; // GlobalM4.DeadTime;
  Pwm.Fpwm_kHz = GlobalM4.Fpwm * 0.001f;
  Pwm.Fpwm_Hz = GlobalM4.Fpwm;
  Pwm.Init(&Pwm);                      
    
  // 13) Запустить АЦП
  Adc.Fpwm_kHz = Pwm.Fpwm_kHz;  // Получить частоту ШИМ модуля PWM (для синхронизации АЦП с ШИМ)
  Adc.Init(&Adc);
  
  // 14) Ждем пока запустится АЦП, прерывания в такте ШИМ и появятся измеренные значения ЗПТ и входной сети
  osDelay(80);   // 
  GlobalM4.FlagCommand.bit.PerifInitComplete = 1;
    
  // 15) Создать семафор semStartTask20ms
  semStartTask20msHandle = osSemaphoreNew(1, 1, &semStartTask20ms_attributes);
  
  // 16) Создать таймер  
  TimerTask20msHandle = osTimerNew(CallbackTimerTask20ms, osTimerPeriodic, NULL, &TimerTask20ms_attributes);
  osTimerStart(TimerTask20msHandle, 22);   // Запустить таймер с интервалом 20ms                            
      
  // Инициализация портов UART
  UART5_Init();       // Настроить порт UART5
  USART2_Init();
      
  while(1){
      // Залочить задачу до получения симафора с таймера синхронизации
      osSemaphoreAcquire(semStartTask20msHandle, portMAX_DELAY);          // Симофор для синхронизации выполнения задачи с таймером
    
      // 0) Проверим флаг необходимости переинициализировать модуль ШИМ
      if(GlobalM4.FlagCommand.bit.PwmReInit == 1){
        osDelay(5);
        // Пока с задержкой так как флаг устанавливается быстрей чем обновляются данные в глобальной структуре
        
        // Переинициализация ШИМ
        Pwm.DeadTime_uS = DrvConst.DeadTimeInit.DeadTime; //GlobalM4.DeadTime;
        Pwm.Fpwm_kHz = GlobalM4.Fpwm * 0.001f;
        Pwm.Fpwm_Hz = GlobalM4.Fpwm;
        Pwm.ReInit(&Pwm);
        
        // Синхронизировать АЦП с периодом ШИМ
        Adc.Fpwm_kHz = Pwm.Fpwm_kHz;               // Получить частоту ШИМ модуля PWM (для синхронизации АЦП с ШИМ)
        Adc.ReInit(&Adc);
        
        GlobalM4.FlagCommand.bit.PwmReInit = 0;    // перенастройка отработана сбросить флаг
      }      
            
      // 1) Контроль расчета напряжения RMS входной сети, Аварийный режим. Возможно, что на входе сети и нету.  
      if(Meas.Flags.PeriodUinCopy == 0){
          // Расчет RMS подвис
          Meas.Flags.PeriodUin = 1;                     // Инициировать расчет RMS в ручном режиме                  
          Meas.UinPhasingFlag = (u16)UndefinedSequence;
      }
      else{
          Meas.Flags.PeriodUinCopy = 0;                 // Сбрасываем этот флаг только здесь    
      }
      
      // 2) Перенастроить МТЗ компаратора  
      ReInitMTZcomp();
      
      // 3) Расчет мощности на выходе ПЧ
      Meas.PowerType = GlobalM4.DrvType;
      Meas.meas20ms_calc(&Meas);    
  }
}

// CallbackTimerTask20ms function 
void CallbackTimerTask20ms(void *argument){
  osSemaphoreRelease(semStartTask20msHandle);   // Установить семафор;
}

// Перенастроить МТЗ компаратора
void ReInitMTZcomp(void){
  s16 CompMax, CompMin;
  float Gain, Offset, Freq_K;
  
  Freq_K = 1;
  
  // 1) Получить среднее на три канала значение Gain и Ofset
  Gain = ((GlobalM4.ValIu.Gain + GlobalM4.ValIv.Gain + GlobalM4.ValIw.Gain)/3.0f) * 16.0f;                  // *16 - это перевод Gain c 12 бит в 8 бит 
  Offset = ((float)((GlobalM4.ValIu.OffSet + GlobalM4.ValIv.OffSet + GlobalM4.ValIw.OffSet) >> 4))/3.0f;    // >> 4 - Это перевод OffSet с 12 бит в 8 бит
  
  //1-2) Определяем  частоту поля на выходе и корректируем уставку МТЗ, если на выходе нулевая частота.
  if (GlobalM4.We == 0) {
    Freq_K = 0.7071068f;
  }
  
  // 2) Расчитать значения для компаратора с учетом частоты поля на выходе
  CompMax = (s16)(Offset + (GlobalM4.FailsSetup.Imtz*Freq_K / (Gain * 2.0f)));
  CompMin = (s16)(Offset - (GlobalM4.FailsSetup.Imtz*Freq_K / (Gain * 2.0f)));
      
  // 4) Применить полученные настройки в компаратор ADC
  Adc.CompMax = CompMax;
  Adc.CompMin = CompMin;
  Adc.CompInit(&Adc);
}
#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)