#ifndef _CHARGEDRV_H_
#define _CHARGEDRV_H_

#include "type.h"
#include "stm32f4xx_hal.h"

// Назначение ног для управления зарядным резистором и тиристорами выпрямителя
#define RELE_ON_PIN        GPIO_PIN_2
#define RELE_ON_PORT       GPIOB

#define ZAR_ON_PIN         GPIO_PIN_7
#define ZAR_ON_PORT        GPIOE


/*-----------------------------------------------------------------------------
Define the structure of the CHARGE Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   GPIO_PinState ReleState;   // Состояние зарядного реле  
   GPIO_PinState ZarState;    // Сосотояние тиристоров выпрямителя
   u16           Fail;
   void (*init)();            // Настройка драйвера заряда
   void (*set)();             // Установить текущее значение
} CHARGE;    

/*-----------------------------------------------------------------------------
Define a COMPZPTDRV_handle
-----------------------------------------------------------------------------*/
typedef CHARGE *CHARGE_handle;

/*------------------------------------------------------------------------------
Default Initializers for CHARGE Object 
------------------------------------------------------------------------------*/
#define CHARGE_DEFAULTS { GPIO_PIN_RESET,          \
                          GPIO_PIN_RESET,          \
                          0,			\
                          (void (*)(u32))ChargeInit,   \
                          (void (*)(u32))ChargeSet     \
                        }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void ChargeInit(CHARGE_handle);   // Настроить драйвер работы с зарядом
void ChargeSet(CHARGE_handle);    // Установить реле и тиристоры в необходимое состояние

#endif // _CHARGEDRV_H_