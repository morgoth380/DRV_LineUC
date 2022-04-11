#ifndef _TSK_MCBSP_H
#define _TSK_MCBSP_H

#include "main.h"
#include "cmsis_os.h"
#include "type.h"
#include "queue.h"

extern xQueueHandle xQueueFlash;
extern osSemaphoreId_t FlashSemRdWr;
/*============================================================================*/
//                    Коды запросов к АТ45
/*============================================================================*/
#define RD                  1
#define WR                  2
#define PAGEerase           3
#define BlockErase          4
#define RDUST               5
#define WRUST               6
#define RDONEUST            7
#define NOTerr              0

#define ERRat45notErr       0x80
#define ERRat45pageBond     0x81
#define ERRat45sign         0x100

#define STATEuartWaitRdFile 0x8000
#define MAXPARAMGRUP 100

/*============================================================================*/
//                  Тип описателя запроса к АТ45
/*============================================================================*/
typedef struct {
    u16         *opCode;        // Код операции и код возврата
    u16         size;           // Сколько
    void        *dest;          // Куда
    void        *srs;           // Откуда
    void        *ptrSem;        // Указатель на семафор, если нужно
    u16         codeFail;       // код ошибки 
}type_FlashMessage;

/*============================================================================*/
//                  Прототипы функций
/*============================================================================*/
void FuncDataFlash(void *argument);    // Задача флеш памяти

// AT45 function
void WrUstAT45(type_FlashMessage *msg);
void RdUstAT45(type_FlashMessage *msg);
void OneGrupAT45toFRAM(type_FlashMessage *msg);
// W25Q16 function
void WrUstW25(type_FlashMessage *msg);
void RdUstW25(type_FlashMessage *msg);
void OneGrupW25toFRAM(type_FlashMessage *msg);

#endif
