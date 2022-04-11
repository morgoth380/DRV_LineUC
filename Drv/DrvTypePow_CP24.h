#ifndef _DRVTYPE_H_
#define _DRVTYPE_H_

#include "type.h"
#include "GlobalDefines.h"

// Назначение ног 
#define TYPE_DRV_PIN        0
#define TYPE_DRV_PORT       0

#define DELAY_ASK_ERR       50               // Задержка для формирования сигнала квитирования (50 - 2.0мкс)

/*-----------------------------------------------------------------------------
Define the structure of the DRVTYPE Driver Object 
------------------------------------------------------------------------------*/
typedef struct {   
   DrvType_Enum DrvType;      // Тип ПЧ
   void (*Init)();            // Настройка
   DrvType_Enum (*GetType)(); // Получить тип ПЧ
   void (*AskErr)();          // Квитирование аварии ключей
} DRVTYPE;    
/*-----------------------------------------------------------------------------
Define a DRVTYPE_handle
------------------------------------------------------------------------------*/
typedef DRVTYPE *DRVTYPE_handle;
/*------------------------------------------------------------------------------
Default Initializers for DRVTYPE Object 
------------------------------------------------------------------------------*/
#define DRVTYPE_DEFAULTS { DrvTypeNoInit,                         \
                           (void         (*)(u32))TypeDrvInit,    \
                           (DrvType_Enum (*)(u32))GetTypeDrv,     \
                           (void         (*)(u32))AskErrKey       \
                         }
/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void TypeDrvInit(DRVTYPE_handle);         // Настроить 
DrvType_Enum GetTypeDrv(DRVTYPE_handle);  // Получить Тип ПЧ 
void AskErrKey(DRVTYPE_handle);           // Квитирование аварии ключей

#endif // _DRVTYPE_H_