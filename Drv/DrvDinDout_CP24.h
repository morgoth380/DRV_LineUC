/*============================================================================*/
//      Заголовочный файл для обекта работы с дискретными входами/выходами
/*============================================================================*/
#ifndef _DRVDINDOUT_H_
#define _DRVDINDOUT_H_

#include "type.h"
#include "stm32f4xx_hal.h"

// Дискретные входы
#define DIN_1_PIN_V21  GPIO_PIN_2      // PE 2 +
#define DIN_1_PORT_V21 GPIOE           // PE 2
// Дополнительное объявления для блока V2.2
#define DIN_1_PIN_V22  GPIO_PIN_0      // PD 0 +
#define DIN_1_PORT_V22 GPIOD           // PD 0

#define DIN_2_PIN      GPIO_PIN_3      // PE 3 +
#define DIN_2_PORT     GPIOE           // PE 3

#define DIN_3_PIN_V21  GPIO_PIN_4      // PE 4 +
#define DIN_3_PORT_V21 GPIOE           // PE 4
// Дополнительное объявления для блока V2.2
#define DIN_3_PIN_V22  GPIO_PIN_1      // PD 1 +
#define DIN_3_PORT_V22 GPIOD           // PD 1

#define DIN_4_PIN      GPIO_PIN_0      // PE 0 +
#define DIN_4_PORT     GPIOE           // PE 0

#define DIN_5_PIN      GPIO_PIN_9      // PB 9 +
#define DIN_5_PORT     GPIOB           // PB 9

#define DIN_6_PIN      GPIO_PIN_8      // PB 8 +
#define DIN_6_PORT     GPIOB           // PB 8

#define DIN_HF_PIN     GPIO_PIN_3      // PD 3 +
#define DIN_HF_PORT    GPIOD           // PD 3

// Дискретные выходы
#define DOUT_1_PIN     GPIO_PIN_1      // PC 1 +
#define DOUT_1_PORT    GPIOC           // PC 1

#define DOUT_2_PIN     GPIO_PIN_0      // PC 0 +
#define DOUT_2_PORT    GPIOC           // PC 0

#define DOUT_HF_PIN    GPIO_PIN_7      // PB 7 +
#define DOUT_HF_PORT   GPIOB           // PB 7

// Состояние светодиода
typedef enum{
    PinOff  = 0,     // лог. 0
    PinOn,           // лог. 1
}PinStatus_type;

// Описание обекта драйвера дискретных входов/выходов
typedef struct {  
    u16 DinMask; 
    u16 DoutMask;
    
    void  (*init)();                   // Указатель на функцию настройки драйвера
    u8    (*getDin)();                 // Указатель на функцию Установить статус в светодиоды
    void  (*setDout)();                // Указатель на функцию Установить статус в светодиоды
}DINDOUT;            

typedef DINDOUT *DINDOUT_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the DINDOUT object.
-----------------------------------------------------------------------------*/                     
#define DIN_DOUT_DEFAULTS {  \
                  0,         \
                  0,         \
                  (void (*)(u32))DinDoutInit, \
                  (u8   (*)(u32))GetDinVal,   \
                  (void (*)(u32))SetDoutVal }
/*------------------------------------------------------------------------------
Prototypes for the functions 
------------------------------------------------------------------------------*/
void DinDoutInit(DINDOUT_handle);    // Настройка переферии DinDout
u8 GetDinVal(DINDOUT_handle);        // Прочитать состояние дискретных входов
void SetDoutVal(DINDOUT_handle);     // Установить дискретные выходы
#endif // #ifndef _DRVDINDOUT_H_


