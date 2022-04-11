#ifndef _DISCRT_H_
#define _DISCRT_H_

#include "type.h"
#include "stm32f4xx_hal.h"


// Назначение ног для подключение дискретного датчика температуры
#define DISCR_T_PIN        GPIO_PIN_15
#define DISCR_T_PORT       GPIOE

// Статус дискретного датчика температуры
typedef enum{
    TdiscrOk = 0,        // Дискретный датчик исправен
    TdiscrErr,           // Перегрев!!!
}StatTdiscr_Enum;

/*-----------------------------------------------------------------------------
Define the structure of the CompZPT Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   void            (*Init)();               // Настройка дискретного датчика температуры
   StatTdiscr_Enum (*GetStatus)();          // Получить состояние датчика температуры
} TDISCRDRV;    

/*-----------------------------------------------------------------------------
Define a TDISCRDRV_handle
-----------------------------------------------------------------------------*/
typedef TDISCRDRV *TDISCRDRV_handle;

/*------------------------------------------------------------------------------
Default Initializers for TDISCRDRV Object 
------------------------------------------------------------------------------*/
#define TDISCRDRV_DEFAULTS { (void            (*)(u32))TDiscrInit,   \
                             (StatTdiscr_Enum (*)(u32))GetTDiscr,    \
                           }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void TDiscrInit(TDISCRDRV_handle);    // Настроить дискретный термодатчик
StatTdiscr_Enum GetTDiscr(TDISCRDRV_handle);     // Получить статус дискретного термодатчика

#endif // _DISCRT_H_