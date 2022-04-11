#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "Task20ms.h"
#include "Global_Include.h"
#include "DrvPWM.h"
#include "DrvADC.h"
#include "CurVoltMeas.h"
#include "DrvCompZPT.h"
#include "GlobalVar.h"
#include "InputOutput.h"
#include "TormSW.h"
#include "DrvDinDout.h"
#include "DrvLedStatus.h"
#include "DrvCharge.h"
#include "DrvTypePow.h"
#include "DrvFan.h"
#include "DrvDiscrT.h"
//#include "DrvErrSWandSTO.h"


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

PWMGEN Pwm = PWMGEN_DEFAULTS;                  // Обект настройки генератора PWM
ADCDRV Adc = ADCDRV_DEFAULTS;                  // Обект настройки модуля АЦП
COMPZPTDRV CompZPT = COMPZPTDRV_DEFAULTS;      // Обект компаратора в ЗПТ
TORMSW TormSw = TORMSW_DEFAULTS;               // Обект управления тормозным ключем
DINDOUT DinDout = DIN_DOUT_DEFAULTS;           // Обект управления DinDout
LED LedStatus = LED_DEFAULTS;                  // Обект управления светодиодами на передней панели ПЧ
CHARGE Charge = CHARGE_DEFAULTS;               // Обект управления зарядом 
DRVTYPE DrvType = DRVTYPE_DEFAULTS;            // Обект для получения типа ПЧ и управления квитирования аварий ключей для мощных ПЧ 
FAN FanControl = FAN_DEFAULTS;                 // Обект управления вентеляторами
TDISCRDRV TDiscr = TDISCRDRV_DEFAULTS;         // Обект управления дискретным датчиком температуры
//SWANDSTO SwAndSto = SWANDSTO_DEFAULTS;         // Обект управления авариями ключей и STO

extern CURVOLTMEAS Meas;                       // Обект расчета значений токов и напряжений 
extern osSemaphoreId_t semWaitPerifInitHandle; // Симафор для разблокирование задачи МП
//void sendExtBlocksProcessingData(void); //Отправка данных в процессор обработки энкодеров

// Фоновая задача 20ms
void Func20ms(void *argument){
  
  //Получить тип ПЧ - фиктивная инициализация для СР24
  DrvType.Init(&DrvType); 
  GlobalM4.DrvType = DrvType.GetType(&DrvType);
  
  // 1) Настроить драйвер управления вентиляторами охлаждения
  FanControl.init(&FanControl); 
  
  // 2) Настройка компаратора в ЗПТ
  CompZPT.Init(&CompZPT);
  
  // 3) Настроить драйвер управления зарядом
  Charge.init(&Charge);  
  
  // 4) Настроить тормозной ключ
  TormSw.Init(&TormSw);
   
  // 5) Настроить драйвер для управления DinDout
  DinDout.init(&DinDout);
  
  // 6) Настроить драйвер управления светодиодами статуса
  LedStatus.init(&LedStatus);
      
  // 6а) Настроить обработчик Tdiscr
  TDiscr.Init(&TDiscr);
  
  // Разлочить задачу МП
  osSemaphoreRelease(semWaitPerifInitHandle);   // Установить семафор;  
  // Ждем запуска МП
  osDelay(20);        // Для инициализации значений в глобальной структуре значениями из системы параметров  
  
  // 7) Запустить таймер формирования ШИМ (T1)
  Pwm.DeadTime_uS = GlobalM4.DeadTimeInit.DeadTime;
  Pwm.Fpwm_kHz = GlobalM4.Fpwm * 0.001f;
  Pwm.Init(&Pwm);                      
  
  // 8) Запустить АЦП
  Adc.Fpwm_kHz = Pwm.Fpwm_kHz;  // Получить частоту ШИМ модуля PWM (для синхронизации АЦП с ШИМ)
  Adc.Init(&Adc);
    
  // 9) Ждем пока запустится АЦП и прерывания в такте ШИМ
  osDelay(80);    // osDelay(4) было 4 для СР24
  GlobalM4.FlagCommand.bit.PerifInitComplete = 1;
   
  // 10) Создать семафор semStartTask20ms
  semStartTask20msHandle = osSemaphoreNew(1, 1, &semStartTask20ms_attributes);
  
  // 11) Создать таймер  
  TimerTask20msHandle = osTimerNew(CallbackTimerTask20ms, osTimerPeriodic, NULL, &TimerTask20ms_attributes);
  osTimerStart(TimerTask20msHandle, 20);   // Запустить таймер с интервалом 20ms                            
    
  UART5_Init();       // Настроить порт UART5
  USART2_Init();  
  
  while(1){
    // Залочить задачу до получения симафора с таймера синхронизации
    osSemaphoreAcquire(semStartTask20msHandle, portMAX_DELAY);          // Симофор для синхронизации выполнения задачи с таймером
    
    // 1) Проверим флаг необходимости переинициализировать модуль ШИМ
    if(GlobalM4.FlagCommand.bit.PwmReInit == 1){
      osDelay(5);
      // Пока с задержкой так как флаг устанавливается быстрей чем обновляются данные в глобальной структуре
      
      // Переинициализация ШИМ
      Pwm.DeadTime_uS = DrvConst.DeadTimeInit.DeadTime; // GlobalM4.DeadTime;
      Pwm.Fpwm_kHz = GlobalM4.Fpwm * 0.001f;
      Pwm.ReInit(&Pwm);
    
      // Синхронизировать АЦП с периодом ШИМ
      Adc.Fpwm_kHz = Pwm.Fpwm_kHz;               // Получить частоту ШИМ модуля PWM (для синхронизации АЦП с ШИМ)
      Adc.ReInit(&Adc);
    
      GlobalM4.FlagCommand.bit.PwmReInit = 0;    // перенастройка отработана сбросить флаг
    }
    
    // 2) Перенастроить МТЗ компаратора  
    //ReInitMTZcomp();
    
    // 3) Расчет мощности на выходе ПЧ
    Meas.meas20ms_calc(&Meas);
    
    //sendExtBlocksProcessingData(); //Отправка данных в процессор обработки энкодеров    
   
  }
}

// CallbackTimerTask20ms function 
void CallbackTimerTask20ms(void *argument){
  osSemaphoreRelease(semStartTask20msHandle);   // Установить симафор;
}

#endif