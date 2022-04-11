#ifndef _ERRSW_AND_STO_H_
#define _ERRSW_AND_STO_H_

#include "type.h"
#include "stm32f4xx_hal.h"


// Назначение ног
#define SW_U_PIN      GPIO_PIN_10     // PB 10
#define SW_U_PORT     GPIOB           // PB 10

#define SW_V_PIN      GPIO_PIN_7      // PA 7
#define SW_V_PORT     GPIOA           // PA 7

#define SW_W_PIN      GPIO_PIN_4      // PC 4
#define SW_W_PORT     GPIOC           // PC 4

#define SW_T_PIN      GPIO_PIN_5      // PC 5
#define SW_T_PORT     GPIOC           // PC 5

#define STO_PIN       GPIO_PIN_14     // PE 14
#define STO_PORT      GPIOE           // PE 14

// Режим работы 
typedef enum{
    ContrOff = 0,        // Выключен контроль аварий
    ContrOn,             // Включен контроль аварий
}ModeContr_Enum;

// Статус STO 
typedef enum{
    StoOk = 0,          // Авария STO отсутствует
    StoErr,             // Авария STO присутствует
}StoStatus_Enum;

// Статус аварии ключей
typedef enum{
    SwOk = 0,          // Авария SW отсутствует
    SwErr,             // Авария SW присутствует
}SwStatus_Enum;

/*-----------------------------------------------------------------------------
Define the structure of the SWANDSTO Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   ModeContr_Enum  ContrSW;
   ModeContr_Enum  ContrSTO;
   
   void (*init)();                    // Настройка обработчика аварий
   void (*onSW)();                    // Включить обработчик аварий силовых ключей 
   void (*offSW)();                   // Выключить обработчик аварий силовых ключей
   void (*onSTO)();                   // Включить обработчик аварий STO
   void (*offSTO)();                  // Выключить обработчик аварий STO
   void (*AskErr)();                  // Квитирование аварии ключей 
   StoStatus_Enum (*getStoStatus)();  // Получить текущее состояние STO (не фронт а устоявшиеся значения)
   SwStatus_Enum (*getSwUStatus)();   // Получить текущее состояние SwU (не фронт а устоявшиеся значения)
   SwStatus_Enum (*getSwVStatus)();   // Получить текущее состояние SwV (не фронт а устоявшиеся значения)
   SwStatus_Enum (*getSwWStatus)();   // Получить текущее состояние SwW (не фронт а устоявшиеся значения)
   SwStatus_Enum (*getSwTStatus)();   // Получить текущее состояние SwT (не фронт а устоявшиеся значения)
} SWANDSTO;    

/*-----------------------------------------------------------------------------
Define a SWANDSTO_handle
-----------------------------------------------------------------------------*/
typedef SWANDSTO *SWANDSTO_handle;

/*------------------------------------------------------------------------------
Default Initializers for SWANDSTO Object 
------------------------------------------------------------------------------*/
#define SWANDSTO_DEFAULTS { ContrOff,                \
                            ContrOff,                \
                            (void (*)(u32))Init,     \
                            (void (*)(u32))OnSW,     \
                            (void (*)(u32))OffSW,    \
                            (void (*)(u32))OnSTO,    \
                            (void (*)(u32))OffSTO,   \
                            (void (*)(u32))AskErr,   \
                            (StoStatus_Enum (*)(u32))getStoStatus, \
                            (SwStatus_Enum (*)(u32))getSwUStatus, \
                            (SwStatus_Enum (*)(u32))getSwVStatus, \
                            (SwStatus_Enum (*)(u32))getSwWStatus, \
                            (SwStatus_Enum (*)(u32))getSwTStatus, \
                          }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void Init(SWANDSTO_handle);
void OnSW(SWANDSTO_handle);
void OffSW(SWANDSTO_handle);
void OnSTO(SWANDSTO_handle);
void OffSTO(SWANDSTO_handle);
void AskErr(SWANDSTO_handle);                    // Квитирование аварии ключей 
StoStatus_Enum getStoStatus(SWANDSTO_handle);    // Получить текущее состояние STO (не фронт а устоявшиеся значения)
SwStatus_Enum getSwUStatus(SWANDSTO_handle);     // Получить текущее состояние STO (не фронт а устоявшиеся значения)
SwStatus_Enum getSwVStatus(SWANDSTO_handle);     // Получить текущее состояние STO (не фронт а устоявшиеся значения)
SwStatus_Enum getSwWStatus(SWANDSTO_handle);     // Получить текущее состояние STO (не фронт а устоявшиеся значения)
SwStatus_Enum getSwTStatus(SWANDSTO_handle);     // Получить текущее состояние STO (не фронт а устоявшиеся значения)



#endif // _ERRSW_AND_STO_H_