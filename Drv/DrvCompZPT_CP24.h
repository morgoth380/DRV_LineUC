#ifndef _COMPZPTDRV_H_
#define _COMPZPTDRV_H_

#include "type.h"
#include "stm32f4xx_hal.h"

/*

*/

// Назначение ног для подключение каналов измирения

#define COMP_ZPT_GPIO_Pin        GPIO_PIN_6
#define COMP_ZPT_GPIO_Port       GPIOA





// Режим работы компаратора
typedef enum{
    CompNoInit = 0,        // Компаратор не настроен
    CompOff,               // Компаратор отключен 
    CompWaitErr,           // По прерыванию отключаем ШИМ
    CompCountFront,        // Считаем фронты
    CompErr,               // Была авария по компаратору МТЗ 
}ModeComp_Enum;

/*-----------------------------------------------------------------------------
Define the structure of the CompZPT Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   u32           CountFront;  // Счетчик прерываний от компаратора (фронтов)
   ModeComp_Enum ModeComp;    // Режим работы компаратора
   void (*Init)();            // Настройка компаратора
   void (*ReInit)();          // Изменить режим работы компаратора
} COMPZPTDRV;    

/*-----------------------------------------------------------------------------
Define a COMPZPTDRV_handle
-----------------------------------------------------------------------------*/
typedef COMPZPTDRV *COMPZPTDRV_handle;

/*------------------------------------------------------------------------------
Default Initializers for COMPZPTDRV Object 
------------------------------------------------------------------------------*/
#define COMPZPTDRV_DEFAULTS { 0,          \
                              CompNoInit, \
                              (void (*)(u32))CompZPTInit,    \
                              (void (*)(u32))SetModeComp,    \
                            }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void CompZPTInit(COMPZPTDRV_handle);   // Настроить компаратор
void SetModeComp(COMPZPTDRV_handle);   // Установить новый режим компаратора

#endif // _DRVADC_H_