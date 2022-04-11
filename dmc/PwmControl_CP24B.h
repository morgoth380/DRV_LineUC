#ifndef _PWMCONTROL_
#define _PWMCONTROL_

#include "type.h"


//#define ADC_MIN                    1500        // Минимальное значение АЦП при котором считаем что датчик тока запитан
//#define ADC_MAM                    2500        // Максимальное значение АЦП при котором считаем что датчик тока запитан
//
//#define TIMOUT_PUMPING             100        // Таймаут накачки питания верхних ключей (в тактах ШИМ)
#define TIME_OFFSET_CALIBR         200         // Время калибровки Offset каналов измирения тока (в тактах ШИМ) 

// Состояния аппарата состояний драйвера ШИМ
typedef enum{
    PwmStop = 0,                 // Шим отключен
    PwmErr,                      // Ошибка модуля ШИМ
    PwmInitFeedTopKey,           // Настройка модуля Шим к запитке драйвера верхних ключей  
    PwmFeedTopKey,               // Запитка драйвера верхних ключей и датчиков тока
    PwmCalibrOffset,             // Калибровка каналов измерения (определение Оффсет)
    PwmInitCompMtz,              // Настройка компаратора ацп для контроля МТЗ 
    prePwmStart,                 // Второй такт калибровки нулей АЦП ДТ
    PwmStart,                    // Запуск ШИМ 
    PwmWork,                     // Штатная работа модуля ШИМ (ШИМ включен!!!) 
}StatusPwmControl_Enum;

/*-----------------------------------------------------------------------------
Define the structure of the PWM Control Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   StatusPwmControl_Enum StatusPwm; // Состояние аппарата состояний ШИМ
   u8 FlagStart;                    // Флаг запроса на запуск ШИМ
   u8 FlagStop;                     // Флаг запроса на останов ШИМ
   u32 Timer;                       // Таймер для отсчета времени 
   void (*PwmOn)();                 // Запрос на запуск ШИМ
   void (*PwmOff)();                // Запрос на останов ШИМ
   void (*PwmStateMachine)();       // Аппарат состояний драйвера PWM
} PWMCONTROL;    

/*-----------------------------------------------------------------------------
Define a PWMCONTROL_handle
-----------------------------------------------------------------------------*/
typedef PWMCONTROL *PWMCONTROL_handle;

/*------------------------------------------------------------------------------
Default Initializers for PWMCONTROL Object 
------------------------------------------------------------------------------*/
#define PWMCONTROL_DEFAULTS { PwmStop, \
                              0,       \
                              0,       \
                              0,       \
                              (void (*)(u32))RequestOn,    \
                              (void (*)(u32))RequestOff,   \
                              (void (*)(u32))StateMachine, \
                            }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void RequestOn(PWMCONTROL_handle);         // Включить ШИМ       
void RequestOff(PWMCONTROL_handle);        // Отключить ШИМ
void StateMachine(PWMCONTROL_handle);      // Аппарат состояний

#endif //_PWMCONTROL_
