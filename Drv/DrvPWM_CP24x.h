#ifndef _DRVPWM_
#define _DRVPWM_

#include "type.h"
#include "stm32f4xx_hal.h"

// Назначение ног для PWM
#define PWM_PORT     GPIOE  

#define BOT_U_PIN    GPIO_PIN_8
#define TOP_U_PIN    GPIO_PIN_9

#define BOT_V_PIN    GPIO_PIN_10
#define TOP_V_PIN    GPIO_PIN_11

#define BOT_W_PIN    GPIO_PIN_12
#define TOP_W_PIN    GPIO_PIN_13

#define PWM_EN_PORT  GPIOB  
#define PWM_EN_PIN   GPIO_PIN_6


//#define FREQ_APB2                  180000.0f   // Частота шины синхронизации таймера в кГц
#define TIMER1_MAX_COUNT           65500       // Максимальное значение до которого может считать таймер 

// Точки излома DeadTime
#define DEADTIME_DOT1              0.704f      // Первая точка излома в мкс

extern TIM_HandleTypeDef htim1;                // Указатель на таймер генерирующий ШИМ

// Текущее состояния нижних ключей в момент накачки
typedef enum{
    OnBotKeyU_Enum = 0,          // Открыт нижний ключ фазы U
    OnBotKeyV_Enum,              // Открыт нижний ключ фазы V
    OnBotKeyW_Enum,              // Открыт нижний ключ фазы W
}StatusBotKey_Enum;

// Состояние драйвера ШИМ
typedef enum{
    PwmNoInit_Enum = 0,           // Модуль ШИМ не настроен
    PwmPumping_Enum,              // Модуль ШИМ в режиме накачки
    PwmOff_Enum,                  // Модуль ШИМ настроен но ШИМ отключен
    PwmOn_Enum,                   // Модуль ШИМ настроен, ШИМ включен
}StatusPwm_Enum;

/*-----------------------------------------------------------------------------
Define the structure of the PWM Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   float Fpwm_kHz;             // Частота ШИМ, кГц
   float Fpwm_Hz;              // Частота ШИМ, Гц
   float DeadTime_uS;          // Мертвое время
   u16   MaxCountT1;           // Значение до которого считает таймер (зависит от частоты ШИМ)
   float Ta;                   // Задание для фазы A
   float Tb;                   // Задание для фазы B
   float Tc;                   // Задание для фазы C
   u16   UvalTimer;            // Значение задание для таймера по фазе U (Read only)
   u16   VvalTimer;            // Значение задание для таймера по фазе V (Read only)
   u16   WvalTimer;            // Значение задание для таймера по фазе W (Read only)
   StatusPwm_Enum StatusPwm;   // Текущий статус модуля ШИМ
   u16   PwmOn;                // Текущее состояние ШИМа - Вкл или выкл
   void (*Init)();             // Первичная настройка ШИМ
   void (*ReInit)();           // Перенастроить модуль ШИМ
   void (*Update)();           // Обновить выходное напряжение 
   void (*On)();               // Включить ШИМ (ПУСК!)
   void (*Off)();              // Выключить ШИМ (СТОП!)
   
   void (*InitPumping)();      // Настроить модуль ШИМ для накачки
   void (*Pumping)();          // Накачка напряжения для питания драйверов верхних ключей
   void (*DeInitPumping)();    // Остановить накачку и настроить модуль ШИМ для штатной работы  
   void (*InitVarPwm)();       // Перенастроить модуль ШИМ по переменной частоте ШИМа
} PWMGEN;    

/*-----------------------------------------------------------------------------
Define a PWMGEN_handle
-----------------------------------------------------------------------------*/
typedef PWMGEN *PWMGEN_handle;

/*------------------------------------------------------------------------------
Default Initializers for PWMGEN Object 
------------------------------------------------------------------------------*/
#define PWMGEN_DEFAULTS  { 2.0,    \
                           2000.0, \
                           3.0,    \
                           0,      \
                           0.0f,   \
                           0.0f,   \
                           0.0f,   \
                           0,      \
                           0,      \
                           0,      \
                           PwmNoInit_Enum,            \
                           0,      \
                           (void (*)(u32))PwmInit,    \
                           (void (*)(u32))PwmReInit,  \
                           (void (*)(u32))PwmUpdate,  \
                           (void (*)(u32))PwmOn,      \
                           (void (*)(u32))PwmOff,     \
                           (void (*)(u32))InitFeed,   \
                           (void (*)(u32))FeedTopKey, \
                           (void (*)(u32))DeInitFeed, \
                           (void (*)(u32))InitVarPwm, \
                         }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void PwmInit(PWMGEN_handle);          // Настроить ШИМ
void PwmUpdate(PWMGEN_handle);        // Обновить задание ШИМ
void PwmReInit(PWMGEN_handle);        // Перенастроить модуль ШИМ
void PwmOn(PWMGEN_handle);            // Включить ШИМ       
void PwmOff(PWMGEN_handle);           // Отключить ШИМ
void InitVarPwm(PWMGEN_handle);       // Перенастроить модуль ШИМ по переменной частоте ШИМа

void InitFeed(PWMGEN_handle);         // Настроить модуль ШИМ для накачки
void FeedTopKey(void);                // Накачка питания драйверов верхних ключей    
void DeInitFeed(PWMGEN_handle);       // Остановить накачку и настроить модуль ШИМ для штатной работы 

#endif //_DRVPWM_
