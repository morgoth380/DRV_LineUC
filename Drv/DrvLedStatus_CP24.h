/*============================================================================*/
//      Заголовочный файл для объекта работы с светодиодами индикации
/*============================================================================*/
#ifndef _DRVLEDSTATUS_H_
#define _DRVLEDSTATUS_H_

#include "type.h"

#include "stm32f4xx_hal.h"

// Светодиод WiFi
#define LED_READY_PIN      GPIO_PIN_15      // PE 15 +
#define LED_READY_PORT     GPIOE           // PE 15

#define LED_WiFi_PIN       GPIO_PIN_14      // PE 14  +
#define LED_WiFi_PORT      GPIOE           // Port E

#define LED_FAIL_PIN       GPIO_PIN_13     // PB 13 +
#define LED_FAIL_PORT      GPIOB           // PB 13

#define LED_WORK_PIN       GPIO_PIN_12     // PB 12 +
#define LED_WORK_PORT      GPIOB           // PB 12

// Состояние светодиода
typedef enum{
    LedOff  = 0,     // Светодиод погашен
    LedOn,           // Светодиод светится
}LedStatus_type;

// Описание обекта драйвера светодиодов
typedef struct {  
    LedStatus_type     LedReady;          // Статус светодиода "Готов"
    LedStatus_type     LedWiFi;           // Статус светодиода "WiFi" 
    LedStatus_type     LedFail;           // Статус светодиода "Авария"
    LedStatus_type     LedWork;           // Статус светодиода "Работа"
    void  (*init)();                   // Указатель на функцию настройки драйвера
    void  (*set)();                    // Указатель на функцию Установить статус в светодиоды
}LED;            

typedef LED *LED_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the LED object.
-----------------------------------------------------------------------------*/                     
#define LED_DEFAULTS {  \
             LedOff,    \
             LedOff,    \
             LedOff,    \
             LedOff,    \
             (void (*)(u32))LedInit, \
             (void (*)(u32))LedSet }
/*------------------------------------------------------------------------------
Prototypes for the functions 
------------------------------------------------------------------------------*/
void LedInit(LED_handle);    // Настройка переферии светодиодов
void LedSet(LED_handle);     // Установить статус светодиодов

#endif // #ifndef _DRVLEDSTATUS_H_


