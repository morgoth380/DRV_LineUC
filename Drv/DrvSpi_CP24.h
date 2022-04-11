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

#define cs_set()  FRAM_CS_MK_GPIO_Port->BSRR = (uint32_t)FRAM_CS_MK_Pin << 16U  
#define cs_reset() FRAM_CS_MK_GPIO_Port->BSRR = FRAM_CS_MK_Pin


/*============================================================================*/
/*                     Прототипы функций                                      */
/*============================================================================*/
void PortFRAM_Init(void);
void framWr( void *adress, void *data, u16 size);
void framRd( void *adress, void *data, u16 size);
void FramSetCommand_Adr(u8 command , void *adr);
void FramEnableWrite(void);

//******************************************************************************
// для работы с АТ45
//******************************************************************************

#ifndef _McBSP_H_
#define _McBSP_H_
/*============================================================================*/
// Объявления для блока СР24 v2.2 с памятью W25Q16
#define W25_CS_MK_Pin             GPIO_PIN_4
#define W25_CS_MK_GPIO_Port       GPIOE
#define W25_CLK_MK_Pin            GPIO_PIN_2
#define W25_CLK_MK_GPIO_Port      GPIOE
#define W25_RX_MK_Pin             GPIO_PIN_5
#define W25_RX_MK_GPIO_Port       GPIOE
#define W25_TX_MK_Pin             GPIO_PIN_6
#define W25_TX_MK_GPIO_Port       GPIOE
// В блоке V22 - этой ноги нету!!
//#define W25_RES_MK_Pin_V22            GPIO_PIN_3
//#define W25_RES_MK_GPIO_Port_V22      GPIOE

#define W25_cs_Set()                    W25_CS_MK_GPIO_Port->BSRR = (uint32_t)W25_CS_MK_Pin << 16U  
#define W25_cs_Reset()                  W25_CS_MK_GPIO_Port->BSRR = W25_CS_MK_Pin

#define W25StatusBusy                 1               // Бит занятности Flash  
// Команды для W25Q16
#define W25CmdStatusRead              0x05            // Чтение статуса Flash
#define W25CmdRdAr                    0x0b            // Чтения страницы
#define W25CmdWrAr                    0x02            // Запись страницы
#define W25CmdRdID                    0x9F            // Чтение ID
#define W25CmdWrEn                    0x06            // Разрешить запись во Flash
#define W25CmdEraseChip               0xC7            // Стереть чип
#define W25CmdEraseSector             0x20            // Стереть сектор 
#define W25CmdEnableReset             0x66            // Разрешить сброс 
#define W25CmdReset                   0x99            // Сброс
#define W25_DUMMY_BYTE                0xA5            // Холостая запись (при чтении)
#define W25SizePage	                256             // размер страницы в байтах
#define W25SizeSector	                4096            // размер сектора в байтах
#define W25cmdWrThroughPage1WE         0x83UL          // Запись страницы со стиранием
#define W25cmdWrBuf1                   0x84UL          // Запись данных в буфер Flash
#define W25lengthBffAdr                10          // размер в битах длинны адресса буфера
#define W25lengthPageAdr               12          // размер в битах длинны адресса страницы
#define W25SizeCommand                4           // размер команды и адреса в Flash
//#define W25IndexStreamTxDMA            6        
//#define W25IndexStreamRxDMA            0
#define W25WrPageTimeWE                20          // время ( в мс ) записи 1 страницы со стиранием
#define W25CompTime                    1           // время сравнения данных в Page and Bff
#define W25TransTime                   1           // время на перенос странцы в буфер

/*============================================================================*/
/*                     Прототипы функций W25Q16                                     */
/*============================================================================*/
#ifdef _PROJECT_FOR_STM32_
u8 W25RdStatus(void);
void W25SetCommand_Adr(u8 command , void *adr);
//void Flash_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
#endif //_PROJECT_FOR_STM32_

void setSpiForW25(void);
u16 spflWrStW25(u32 AdrFlash, u32 AdrMk, u16 size);                // Записать страницу
u16 spflWrSektorW25( void *AdrSpfl, void *AdrMk, u16 size );
u16 spflRdW25( void *AdrMk, void *AdrSpfl, u16 size );
void IdleReadingW25(void);
/*============================================================================*/
// Объявления для блока СР24 v2.1 с памятью AT45
#define AT45_CS_MK_Pin             GPIO_PIN_15
#define AT45_CS_MK_GPIO_Port       GPIOA
#define AT45_CLK_MK_Pin            GPIO_PIN_10
#define AT45_CLK_MK_GPIO_Port      GPIOC
#define AT45_RX_MK_Pin             GPIO_PIN_11
#define AT45_RX_MK_GPIO_Port       GPIOC
#define AT45_TX_MK_Pin             GPIO_PIN_0
#define AT45_TX_MK_GPIO_Port       GPIOD
#define AT45_RES_MK_Pin            GPIO_PIN_1
#define AT45_RES_MK_GPIO_Port      GPIOD

#define at45_cs_set()                   AT45_CS_MK_GPIO_Port->BSRR = (uint32_t)AT45_CS_MK_Pin << 16U  
#define at45_cs_reset()                 AT45_CS_MK_GPIO_Port->BSRR = AT45_CS_MK_Pin
#define at45_flagStatusBusy             7               // Бит занятности Flash  
#define at45_flagStatusComp             6               // Бит несоответствия данных
#define AT45cmdRdAr                     0x0BUL//0xE8UL  // Чтения страницы
#define AT45cmdWrThroughPage1WE         0x83UL          // Запись страницы со стиранием
#define AT45cmdWrBuf1                   0x84UL          // Запись данных в буфер Flash
#define AT45cmdStatusRead               0xD7UL          // Чтение статуса Flash
#define AT45cmdCmpBuf1                  0x60UL          // Сравние данных в странице и буфере
#define AT45cmdP1toBuff                 0x53UL          // Перенос страницы в буфер Flash
#define AT45sizePage	                528         // размер страницы в байтах
#define AT45lengthBffAdr                10          // размер в битах длинны адресса буфера
#define AT45lengthPageAdr               12          // размер в битах длинны адресса страницы
#define AT45sizeCommand                 4           // размер команды и адреса в АТ45
//#define AT45IndexStreamTxDMA            6
//#define AT45IndexStreamRxDMA            0
#define AT45WrPageTimeWE                20          // время ( в мс ) записи 1 страницы со стиранием
#define AT45CompTime                    1           // время сравнения данных в Page and Bff
#define AT45TransTime                   1           // время на перенос странцы в буфер
/*============================================================================*/
/*                     Прототипы функций                                      */
/*============================================================================*/
#ifdef _PROJECT_FOR_STM32_
u8 at45RdStatus(void);
void At45SetCommand_Adr(u8 command , void *adr);
void AT45_TxDMA(u32 SrcAddress, u32 DstAddress, u32 DataLength, SPI_HandleTypeDef *hspi);
void AT45_RxDMA(u32 SrcAddress, u32 DstAddress, u32 DataLength, SPI_HandleTypeDef *hspi);
//void Flash_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
#endif //_PROJECT_FOR_STM32_

void setSpiForAt45(void);
u16 spflWrPgAT45( void *AdrSpfl, void *AdrMk, u16 size );
u16 spflRdAT45( void *AdrMk, void *AdrSpfl, u16 size );
void IdleReadingAT45(void);
u16 WaitBusyAT45(u16 delay);

#ifdef _PROJECT_FOR_STM32_
// Обработчик прерывания для ДМА SPI каналов Flash памяти.
void Flash_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
#endif

#endif // _MCBSP_H_



#endif // _SPI_H_