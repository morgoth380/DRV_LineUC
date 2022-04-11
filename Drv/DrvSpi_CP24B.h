#ifndef _SPI_H_
#define _SPI_H_



#ifndef _PROJECT_FOR_STM32_
#define inline __inline
#endif //_PROJECT_FOR_STM32_


#include "type.h"
#ifdef _PROJECT_FOR_STM32_
#include "main.h"
#endif //_PROJECT_FOR_STM32_

#ifdef _PROJECT_FOR_STM32_
extern osSemaphoreId_t SpiFramSem;
#endif //_PROJECT_FOR_STM32_


// для работы с FM25V02
#define FRAM_CS_MK_Pin GPIO_PIN_7
#define FRAM_CS_MK_GPIO_Port GPIOD
#define FRAM_CLK_MK_Pin GPIO_PIN_3
#define FRAM_CLK_MK_GPIO_Port GPIOB
#define FRAM_RX_MK_Pin GPIO_PIN_4
#define FRAM_RX_MK_GPIO_Port GPIOB
#define FRAM_TX_MK_Pin GPIO_PIN_5
#define FRAM_TX_MK_GPIO_Port GPIOB

#define FramWrCmd       0x02
#define FramRdCmd       0x03
#define FramWrEnableCmd 0x06

// Странно. По документации Set & Reset работают с точностью до наоборот
// А все потому, что в коде set и reset стоят наоборот. Поэтому понятно почему так.
// Надо исправить это.
#define cs_set()  FRAM_CS_MK_GPIO_Port->BSRR = (uint32_t)FRAM_CS_MK_Pin << 16U // Это должен быть сброс бита  
#define cs_reset() FRAM_CS_MK_GPIO_Port->BSRR = FRAM_CS_MK_Pin                 // А это установка бита         


/*============================================================================*/
/*                     Прототипы функций                                      */
/*============================================================================*/
void PortFRAM_Init(void);
void framWr( void *adress, void *data, u16 size);
void framRd( void *adress, void *data, u16 size);
void FramSetCommand_Adr(u8 command , void *adr);
void FramEnableWrite(void);

//******************************************************************************
// для работы с W25Q16
//******************************************************************************

#ifndef _McBSP_H_
#define _McBSP_H_
/*============================================================================*/
#define FLASH_CS_MK_Pin GPIO_PIN_4
#define FLASH_CS_MK_GPIO_Port GPIOE

#define FLASH_CLK_MK_Pin GPIO_PIN_2
#define FLASH_CLK_MK_GPIO_Port GPIOE

#define FLASH_RX_MK_Pin GPIO_PIN_5
#define FLASH_RX_MK_GPIO_Port GPIOE

#define FLASH_TX_MK_Pin GPIO_PIN_6
#define FLASH_TX_MK_GPIO_Port GPIOE

#define FLASH_RES_MK_Pin GPIO_PIN_3
#define FLASH_RES_MK_GPIO_Port GPIOE

#define FlashCsSet()                    FLASH_CS_MK_GPIO_Port->BSRR = (uint32_t)FLASH_CS_MK_Pin << 16U  
#define FlashCsReset()                  FLASH_CS_MK_GPIO_Port->BSRR = FLASH_CS_MK_Pin

#define FlashStatusBusy                 1               // Бит занятности Flash  

// Команды для W25Q16
#define FlashCmdStatusRead              0x05            // Чтение статуса Flash
#define FlashCmdRdAr                    0x0b/*0x03*/            // Чтения страницы
#define FlashCmdWrAr                    0x02            // Запись страницы
#define FlashCmdRdID                    0x9F            // Чтение ID
#define FlashCmdWrEn                    0x06            // Разрешить запись во Flash
#define FlashCmdEraseChip               0xC7            // Стереть чип
#define FlashCmdEraseSector             0x20            // Стереть сектор 
#define FlashCmdEnableReset             0x66            // Разрешить сброс 
#define FlashCmdReset                   0x99            // Сброс



#define FLASH_DUMMY_BYTE                0xA5            // Холостая запись (при чтении)

#define FlashSizePage	                256             // размер страницы в байтах
#define FlashSizeSector	                4096            // размер сектора в байтах

#define AT45cmdWrThroughPage1WE         0x83UL          // Запись страницы со стиранием
#define AT45cmdWrBuf1                   0x84UL          // Запись данных в буфер Flash



#define AT45lengthBffAdr                10          // размер в битах длинны адресса буфера
#define AT45lengthPageAdr               12          // размер в битах длинны адресса страницы

#define FlashSizeCommand                4           // размер команды и адреса в Flash


#define AT45IndexStreamTxDMA            6
#define AT45IndexStreamRxDMA            0

#define AT45WrPageTimeWE                20          // время ( в мс ) записи 1 страницы со стиранием
#define AT45CompTime                    1           // время сравнения данных в Page and Bff
#define AT45TransTime                   1           // время на перенос странцы в буфер

/*============================================================================*/
/*                     Прототипы функций                                      */
/*============================================================================*/
#ifdef _PROJECT_FOR_STM32_

u8 FlashRdStatus(void);
//void At45SetCommand_Adr(u8 command , void *adr);

void Flash_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
#endif //_PROJECT_FOR_STM32_

void setSpiForAt45(void);
u16 spflWrSt(u32 AdrFlash, u32 AdrMk, u16 size);                // Записать страницу
u16 spflWrSektor( void *AdrSpfl, void *AdrMk, u16 size );
u16 spflRd( void *AdrMk, void *AdrSpfl, u16 size );
void IdleReading(void);
#endif // _MCBSP_H_



#endif // _SPI_H_