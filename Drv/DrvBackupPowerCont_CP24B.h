#ifndef _BACKUPPOWERDRV_H_
#define _BACKUPPOWERDRV_H_

#include "type.h"
#include "stm32f4xx_hal.h"

// Назначение ног 
#define VIN_CONTROL_PIN        GPIO_PIN_13
#define VIN_CONTROL_PORT       GPIOC

// Статус наличия резервного питания
typedef enum{
    BeckupPowOff = 0,        // Резервное питание отключено
    BeckupPowOn,             // Резервное питание подключено
}StatBeckupPow_Enum;


/*-----------------------------------------------------------------------------
Define the structure of the BACKUPPOW Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   void (*Init)();                     // Настройка 
   StatBeckupPow_Enum (*getStatus)();  // Получить статус
} BACKUPPOW;    

/*-----------------------------------------------------------------------------
Define a BACKUPPOW_handle
-----------------------------------------------------------------------------*/
typedef BACKUPPOW *BACKUPPOW_handle;

/*------------------------------------------------------------------------------
Default Initializers for BACKUPPOW Object 
------------------------------------------------------------------------------*/
#define BACKUPPOW_DEFAULTS {  (void               (*)(u32))BackupPowInit,         \
                              (StatBeckupPow_Enum (*)(u32))GetBackupPowStatus,    \
                           }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void BackupPowInit(BACKUPPOW_handle);                 // Настроить драйвер
StatBeckupPow_Enum GetBackupPowStatus(BACKUPPOW_handle);   // Получить статус

#endif // _BACKUPPOWERDRV_H_