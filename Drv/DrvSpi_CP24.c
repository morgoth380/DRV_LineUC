#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "DrvSpi_CP24.h"
#include "DrvUart.h"
// FRAM memory
SPI_HandleTypeDef hspi1;
// Структуры для обмена с памятью AT45
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi3_rx;
DMA_HandleTypeDef hdma_spi3_tx;
uint8_t at45commBuf[4];                                 // Команды для АТ45

// Структуры для обмена с памятью W25Q16
SPI_HandleTypeDef hspi4;
DMA_HandleTypeDef hdma_spi4_rx;
DMA_HandleTypeDef hdma_spi4_tx;
u8 W25ComBuf[4];                                 // Команды для Flash W25
// Прототипы функций
u32 W25ReadID(void);                                            // Чтение ID c микросхемы Flash памяти
void W25Reset(void);                                            // Сброс Flash памяти 



uint8_t FRAMcommBuf[3];
// Зажержка для работы с Flash
#define WaitBusyAT45(tick, timeOutCnt)      do{                                     \
                                            if(timeOutCnt >= 80)                \
                                                return 1;                       \
                                            osDelay(tick);                      \
                                            timeOutCnt++;                       \
                                        }while (!(at45RdStatus() &  (1 << at45_flagStatusBusy)))\


#define DelayNop()              asm(" NOP");asm(" NOP");   // Задержка для CS
// Семафор для работы с Fram
osSemaphoreId_t SpiFramSem;
const osSemaphoreAttr_t SpiFramSem_attributes = {
  .name = "SpiFramSem"
};
// Семафор для работы с Flash для чтения и записи по ДМА
osSemaphoreId_t FlashSemDMA;
const osSemaphoreAttr_t FlashSemDMA_attributes = {
  .name = "FlashSemDMA"
};
/*============================================================================*/
//   Настройка порта для обмена с FRAM FM25V02
/*============================================================================*/
// Настройка интерфейса МК<->FRAM
void PortFRAM_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Настроить порты
  // Порт SPI
  __HAL_RCC_GPIOB_CLK_ENABLE();            // Включить тактирование порта SPI
  /**SPI1 GPIO Configuration    
  PB3     ------> SPI1_SCK
  PB4     ------> SPI1_MISO
  PB5     ------> SPI1_MOSI 
  */
  GPIO_InitStruct.Pin = FRAM_CLK_MK_Pin|FRAM_RX_MK_Pin|FRAM_TX_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  // Нога CS
  __HAL_RCC_GPIOD_CLK_ENABLE();            // Включить тактирование 
  
  HAL_GPIO_WritePin(FRAM_CS_MK_GPIO_Port, FRAM_CS_MK_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = FRAM_CS_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FRAM_CS_MK_GPIO_Port, &GPIO_InitStruct);  
    
  // 2) Настроить SPI
  __HAL_RCC_SPI1_CLK_ENABLE();             // Включить тактирование модуля SPI
  
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi1);                   // Применить настройки 
  
  // 3) Создать семофор для исключения использования порта SPI из других потоков 
  SpiFramSem = osSemaphoreNew(1, 1, &SpiFramSem_attributes);    // семофор для работы с чтениеми записью
    
  // 4) Включить SPI
  SPI1->CR1 |= SPI_CR1_SPE;                                     
}

/*============================================================================*/
//              Функция записи данных в FM25V02
/*============================================================================*/
void framWr( void *adress, void *data, uint16_t size)
{
  uint8_t  *ptr;
  
  osSemaphoreAcquire(SpiFramSem, portMAX_DELAY);
  
  ptr = (uint8_t*)data;                         // указатель на данные
  FramEnableWrite();                            // разрешить запись
  cs_set();                                     // открыть порт
  FramSetCommand_Adr(FramWrCmd,adress);         // задание команды и адреса
  do{
      SPI1->DR = *ptr;                          // передача данных
      while(!(SPI1->SR & SPI_SR_TXE));          // ожидание завершиние передачи данных 
      ptr++;                                    // след. байт
  }while(--size);

  while(SPI1->SR & SPI_SR_BSY);                 // ожидание завершения передачи
  cs_reset();                                   // закрыть порт
  
  osSemaphoreRelease(SpiFramSem);               // установка семафора
}

/*============================================================================*/
//              Функция чтения данных из FM25V02
/*============================================================================*/
void framRd( void *adress, void *data, uint16_t size)
{
  uint8_t  *ptr;
  osSemaphoreAcquire(SpiFramSem, portMAX_DELAY);
  
  ptr = (uint8_t*)data;                         // указатель на данные
  cs_set();                                     // открыть порт
  FramSetCommand_Adr(FramRdCmd,adress);         // задание команды и адреса   
  READ_REG(SPI1->DR);                           // холостое чтение регистра данных
  
  do{
      SPI1->DR = 0x01;                          // для чтения параметров
      while(!(SPI1->SR & SPI_SR_RXNE));         // ожидание приема данных
      *ptr++ = SPI1->DR;                        // запись байта в буфер
  }while(--size);
  
  while(SPI1->SR & SPI_SR_BSY);                 // ожидание завершения передачи
  cs_reset();                                   // закрыть порт

  osSemaphoreRelease(SpiFramSem);               // установка семафора
}
/*============================================================================*/
//              Функция разрешения записи в FM25V02
/*============================================================================*/
void FramEnableWrite(void)                      // команда на разрешения записи
{
  cs_set();                                     // открыть порт
  SPI1->DR = FramWrEnableCmd;                   // разрешить запись
  while(!(SPI1->SR & SPI_SR_TXE));
  while(SPI1->SR & SPI_SR_BSY) ;                // ожидание завершения передачи
  cs_reset();                                   // закрыть порт
  DelayNop();                                  // Задержка для CS
}
/*============================================================================*/
//              Функция для задания команды и адреса для FM25V02
/*============================================================================*/
void FramSetCommand_Adr(uint8_t command , void *adr)
{
  adr = (void*)(((uint32_t)adr));
  u8 cnt = 0;
  
  FRAMcommBuf[0] = command;
  FRAMcommBuf[1] = (u8)((u32)adr>>8);
  FRAMcommBuf[2] = (u8)((u32)adr);
  
  do{
      SPI1->DR = FRAMcommBuf[cnt++];                                            // передача данных
      while(!(SPI1->SR & SPI_SR_TXE));                                          // ожидание завершиние передачи данных                                              // след. байт
  }while(cnt < 3);
  
  while(SPI1->SR & SPI_SR_BSY);                 // ожидание завершения передачи
}

//****************************************************************
// для работы с АТ45
//****************************************************************
/*============================================================================*/
//   Настройка порта для обмена с Flash AT45 (161)
/*============================================================================*/
void setSpiForAt45(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  // 1) Настроить порты
  // Порт SPI
  __HAL_RCC_GPIOC_CLK_ENABLE();                         // Включить тактирование порта SPI
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /**SPI3 GPIO Configuration    
  PC10     ------> SPI3_SCK
  PC11     ------> SPI3_MISO
  PD0     ------> SPI3_MOSI 
  */

  GPIO_InitStruct.Pin = AT45_CLK_MK_Pin | AT45_RX_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(AT45_CLK_MK_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AT45_TX_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(AT45_TX_MK_GPIO_Port, &GPIO_InitStruct);
    
    // Ноги CS и Reset
  HAL_GPIO_WritePin(AT45_CS_MK_GPIO_Port, AT45_CS_MK_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(AT45_RES_MK_GPIO_Port, AT45_RES_MK_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = AT45_CS_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AT45_CS_MK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin */
  GPIO_InitStruct.Pin = AT45_RES_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AT45_RES_MK_GPIO_Port, &GPIO_InitStruct);
  
   __HAL_RCC_SPI3_CLK_ENABLE(); // Включить тактирование модуля SPI 
  
  // 2) Настроить SPI
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;

  HAL_SPI_Init(&hspi3);                   // Применить настройки 

  // настойка ДМА RX
  #define SPI3_RX_DMA_CR  (DMA_CHANNEL_0 + DMA_PERIPH_TO_MEMORY + DMA_PINC_DISABLE + DMA_MINC_ENABLE + DMA_PDATAALIGN_BYTE + DMA_MDATAALIGN_BYTE + DMA_NORMAL + DMA_PRIORITY_LOW + DMA_FIFOMODE_DISABLE)
 
  hdma_spi3_rx.Instance = DMA1_Stream0;
  hdma_spi3_rx.Init.Channel = DMA_CHANNEL_0;
  hdma_spi3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_spi3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi3_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi3_rx.Init.Mode = DMA_NORMAL;
  hdma_spi3_rx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_spi3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  
  HAL_DMA_Init(&hdma_spi3_rx);

  __HAL_LINKDMA(&hspi3,hdmarx,hdma_spi3_rx);                                    // Привязать инициализированный DMA к SPI

  // настойка ДМА TX
  #define SPI3_TX_DMA_CR    (DMA_CHANNEL_0 + DMA_MEMORY_TO_PERIPH + DMA_PINC_DISABLE + DMA_MINC_ENABLE + DMA_PDATAALIGN_BYTE + DMA_MDATAALIGN_BYTE + DMA_NORMAL + DMA_PRIORITY_LOW + DMA_FIFOMODE_DISABLE) 
      
  hdma_spi3_tx.Instance = DMA1_Stream5;
  hdma_spi3_tx.Init.Channel = DMA_CHANNEL_0;
  hdma_spi3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_spi3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi3_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi3_tx.Init.Mode = DMA_NORMAL;
  hdma_spi3_tx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_spi3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_spi3_tx);

  // Привязать инициализированный DMA к SPI
  __HAL_LINKDMA(&hspi3,hdmatx,hdma_spi3_tx);                                    

   // Включить прерывания DMA  
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  
  // для работы с Flash => FLASH_RES_MK_Pin = 1
  HAL_GPIO_WritePin(AT45_RES_MK_GPIO_Port, AT45_RES_MK_Pin, GPIO_PIN_SET);    

  // включить SPI
  SPI3->CR1 |= SPI_CR1_SPE;
  
  // Семофор для события окончания работы ДМА( чтение/запись)
  FlashSemDMA = osSemaphoreNew(1, 1, &FlashSemDMA_attributes);  
  
  at45_cs_set();
  DelayNop();
  at45_cs_reset();
}
// Чтение статуса АТ45
uint8_t at45RdStatus(void)
{
  uint8_t status; 
  
  at45_cs_set();
  SPI3->DR = AT45cmdStatusRead;                         // чтение статуса
  while(!(SPI3->SR & SPI_SR_TXE));
  while(SPI3->SR & SPI_SR_BSY);                         // ожидание завершения передачи        

  READ_REG(SPI3->DR);                                   // холостое чтение
  
  SPI3->DR = 0x01;                                      // для чтения параметров
  while(!(SPI3->SR & SPI_SR_RXNE));                     // ожидание приема данных
  status = SPI3->DR;                                    // запись байта в буфер
  while(SPI3->SR & SPI_SR_BSY);                         // ожидание завершения передачи
  
  at45_cs_reset();                                      // закрыть порт
  return status;
}
// Функция задания команды и адреса
void At45SetCommand_Adr(uint8_t command , void *adr)
{
  uint16_t numPage,byte, cnt;
  cnt = 0;
  adr = (void*)(((uint32_t)adr));
  
  numPage = (uint32_t)adr / AT45sizePage;
  byte = (uint32_t)adr % AT45sizePage;
  
  at45commBuf[0] = command;
  at45commBuf[1] = numPage >> 6;
  at45commBuf[2] = (numPage << 2)  | (byte >> 8);
  at45commBuf[3] = byte;
  at45_cs_set();
  while(SPI3->SR & SPI_SR_BSY);                                                 // ожидание завершения передачи
  do{
      SPI3->DR = at45commBuf[cnt++];                                            // передача данных
      while(!(SPI3->SR & SPI_SR_TXE));                                          // ожидание завершиние передачи данных                                              // след. байт
  }while(cnt < (AT45sizeCommand));
 
  while(SPI3->SR & SPI_SR_BSY);                                                 // ожидание завершения передачи
  
}
// Настойка ДМА на передачу данных
void AT45_TxDMA(u32 SrcAddress, u32 DstAddress, u32 DataLength, SPI_HandleTypeDef *hspi)
{
  DMA_HandleTypeDef *hdmaTx;
  DMA_TypeDef *regs;
  
  hdmaTx = hspi->hdmatx;
  
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);
  
  //__HAL_UNLOCK(hdmaTx);


  hdmaTx->State = HAL_DMA_STATE_BUSY;
  
  HAL_DMA_Abort(hdmaTx);
   
  
  // Тормозим ДМА
  __HAL_DMA_DISABLE (hdmaTx); // hdmaRx->Instance->CR &=  ~(1 << DMA_SxCR_EN)
  while((hdmaTx->Instance->CR & DMA_SxCR_EN) == SET) { } // Ждем реального останова ДМА
  // Делаем конфигурирование CR
  hdmaTx->Instance->CR = SPI3_TX_DMA_CR;
  
  hdmaTx->Instance->NDTR = DataLength;                                              // запись длинны передаваемых данных
  hdmaTx->Instance->PAR  = DstAddress;                                              // куда будем перелавать данные ( регистр данных SPI)
  hdmaTx->Instance->M0AR = SrcAddress;                                              // запись данных которые нужно передат
  hdmaTx->Instance->CR |=  (1 << DMA_SxCR_MINC_Pos);                                // включить инкремент памяти 
  hdmaTx->Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;                       // включить прерывание по ДМА

  // Сбросить все фдаги прерываний до старта DMA
  regs = (DMA_TypeDef *)hdmaTx->StreamBaseAddress;  
  regs->LIFCR = (DMA_FLAG_FEIF0_4 | DMA_FLAG_DMEIF0_4 | DMA_FLAG_TEIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TCIF0_4) << hdmaTx->StreamIndex;     
  
__HAL_DMA_ENABLE (hdmaTx); //   hdmaTx->Instance->CR |=  DMA_SxCR_EN; // Запускаем ДМА
  //hdmaTx->State = HAL_DMA_STATE_READY;
  hspi->Instance->CR2 |= (SPI_CR2_TXDMAEN);                                               // разрешить запрос ДМА по окончанию приема                                  
}
// Настройка ДМА на приема данных 
void AT45_RxDMA(u32 SrcAddress, u32 DstAddress, u32 DataLength, SPI_HandleTypeDef *hspi)
{
  DMA_HandleTypeDef *hdmaTx, *hdmaRx;  
  DMA_TypeDef *regs;
  static uint8_t _filler = 0x0A;
    
  hdmaTx = hspi->hdmatx;
  hdmaRx = hspi->hdmarx;
    
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);
  
   /* Allocate lock resource */
  __HAL_UNLOCK(hdmaRx);
  //__HAL_UNLOCK(hdmaTx);

  /* Change DMA peripheral state */
  //hdmaRx->State = HAL_DMA_STATE_BUSY;
  hdmaTx->State = HAL_DMA_STATE_BUSY;
  
  HAL_DMA_DeInit(EXT1_PortStructure.hdmarx);
  HAL_DMA_DeInit(EXT2_PortStructure.hdmarx);
  //HAL_NVIC_DisableIRQ(UART5_IRQn);
  
  HAL_DMA_Abort (hdmaRx);
   
  HAL_DMA_Abort(hdmaTx);
  
  // Тормозим ДМА
  __HAL_DMA_DISABLE (hdmaRx); // hdmaRx->Instance->CR &=  ~(1 << DMA_SxCR_EN)
  while((hdmaRx->Instance->CR & DMA_SxCR_EN) == SET) { } // Ждем реального останова ДМА
  // Делаем конфигурирование CR
  hdmaRx->Instance->CR = SPI3_RX_DMA_CR;                                        // Базовая настройка RxDMA 
  hdmaRx->Instance->NDTR = DataLength;                                          // запись длинны принимаемых данных
  hdmaRx->Instance->PAR  = DstAddress;                                          // куда будем перелавать данные ( регистр данных SPI)
  hdmaRx->Instance->M0AR = SrcAddress;     
  // чтение данных которые нужно передать  
  hdmaRx->Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;                       // включить прерывание по ДМА
  
  // Сбросить все фдаги прерываний до старта DMA
  regs = (DMA_TypeDef *)hdmaRx->StreamBaseAddress;  
  regs->LIFCR = (DMA_FLAG_FEIF0_4 | DMA_FLAG_DMEIF0_4 | DMA_FLAG_TEIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TCIF0_4) << hdmaRx->StreamIndex;     
  __HAL_DMA_ENABLE (hdmaRx); //   hdmaRx->Instance->CR |=  DMA_SxCR_EN; // Запускаем ДМА
  
  //--------настройка ДМА на передачу-----------//
  // Тормозим ДМА
  __HAL_DMA_DISABLE (hdmaTx); // hdmaRx->Instance->CR &=  ~(1 << DMA_SxCR_EN)
  while((hdmaTx->Instance->CR & DMA_SxCR_EN) == SET) { } // Ждем реального останова ДМА
  // делаем конфигурирование CR
  hdmaTx->Instance->CR = SPI3_TX_DMA_CR;  
  hdmaTx->Instance->NDTR = DataLength;                                          // запись длинны передаваемых данных
  hdmaTx->Instance->PAR  = DstAddress;                                          // куда будем перелавать данные ( регистр данных SPI)
  hdmaTx->Instance->M0AR = (uint32_t)(&_filler);                                // запись данных которые нужно передат    
  hdmaTx->Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;                   // включить прерывание по ДМА
  hdmaTx->Instance->CR &=  ~(1 << DMA_SxCR_MINC_Pos);                           // отключить инкремент памяти  
  
  // Сбросить все фдаги прерываний до старта DMA
  regs = (DMA_TypeDef *)hdmaTx->StreamBaseAddress;  
  regs->LIFCR = (DMA_FLAG_FEIF0_4 | DMA_FLAG_DMEIF0_4 | DMA_FLAG_TEIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TCIF0_4) << hdmaTx->StreamIndex;       
  
__HAL_DMA_ENABLE (hdmaTx); //   hdmaTx->Instance->CR |=  DMA_SxCR_EN; // Запускаем ДМА
    
  hspi->Instance->CR2 |= (SPI_CR2_RXDMAEN);                                     // разрешить запрос ДМА по окончанию приема                                    
  hspi->Instance->CR2 |= (SPI_CR2_TXDMAEN);                                     // разрешить запрос ДМА по окончанию приема                                  
  
  //hdmaTx->State = HAL_DMA_STATE_READY;
  //hdmaRx->State = HAL_DMA_STATE_READY;
}

// Чтение данные с Flash
uint16_t spflRdAT45( void *AdrMk, void *AdrSpfl, uint16_t size )
{
  uint16_t *pAdrSpfl, *pAdrMk;
  
  pAdrSpfl = (u16*)AdrSpfl;
  pAdrMk    = (u16*)AdrMk;
  
  At45SetCommand_Adr(AT45cmdRdAr,pAdrSpfl);
  IdleReadingAT45();
  AT45_RxDMA((uint32_t)pAdrMk,(uint32_t)&SPI3->DR, size, &hspi3);                       // заполнить буффер данными с DMA
  
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);                                // ожидание окончания передачи данных DMA
  at45_cs_reset();
  osSemaphoreRelease(FlashSemDMA);
  
  return 0;
}

// запись данных  во Flash
uint16_t spflWrPgAT45( void *AdrSpfl, void *AdrMk, uint16_t size)
{
  uint16_t      *pAdrSpfl, *pAdrMk;
  u16           sizeToEndpage = 0;			                        // колличество байт до конца страницы
  u8            timeOutCnt = 0;
  pAdrSpfl  = (u16*)AdrSpfl;
  pAdrMk    = (u16*)AdrMk;
  
  sizeToEndpage = AT45sizePage - ((u32)pAdrSpfl % AT45sizePage);		// колличество байт до конца страницы
  
  // дописываем до конца страницы
  if(sizeToEndpage < size)                                                      // если разрем данных больше чем количество байт до конца страницы                                                     
  {
    size -= sizeToEndpage;
             
    At45SetCommand_Adr(AT45cmdP1toBuff,pAdrSpfl);                               // Перенос страницы в буфер Flash
    at45_cs_reset();
    WaitBusyAT45(AT45TransTime,timeOutCnt);
    
    At45SetCommand_Adr(AT45cmdWrBuf1,pAdrSpfl);                                 // команда на заполнение буффера Flash
    AT45_TxDMA((uint32_t)pAdrMk,(uint32_t)&SPI3->DR, sizeToEndpage, &hspi3);            // заполнить буффер данными с DMA

    osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);                              // ожидание окончания передачи данных DMA
    at45_cs_reset();
    osSemaphoreRelease(FlashSemDMA);

    At45SetCommand_Adr(AT45cmdWrThroughPage1WE,pAdrSpfl);                       // команда на запись страницы
    at45_cs_reset();
    
    WaitBusyAT45(AT45WrPageTimeWE,timeOutCnt);

    At45SetCommand_Adr(AT45cmdCmpBuf1,pAdrSpfl);                                // Сравнение записанных данных с данными в буффере
    at45_cs_reset();
    
    WaitBusyAT45(AT45CompTime,timeOutCnt);

    if(at45RdStatus() &  (1 << at45_flagStatusComp))                              // Если данные не в буффера не совпадают с данными в старанице
      return 1;
            
    pAdrMk += sizeToEndpage/2;
    pAdrSpfl += sizeToEndpage/2;
  }
  
  if(size < AT45sizePage)                                                       // если записать нужно размер меньше 1 страницы
  {
    At45SetCommand_Adr(AT45cmdP1toBuff,pAdrSpfl);                               // Перенос страницы в буфер Flash
    at45_cs_reset();
    WaitBusyAT45(AT45TransTime, timeOutCnt);
    //if(WaitBusyAT45(AT45TransTime) != 0);
      //return 1;
  }
  
  At45SetCommand_Adr(AT45cmdP1toBuff,pAdrSpfl);                                 // Перенос страницы в буфер Flash
  at45_cs_reset();
  WaitBusyAT45(AT45TransTime,timeOutCnt);
  
  At45SetCommand_Adr(AT45cmdWrBuf1,pAdrSpfl);                                   // команда на заполнение буффера Flash
  AT45_TxDMA((uint32_t)pAdrMk,(uint32_t)&SPI3->DR, size, &hspi3);                       // заполнить буффер данными с DMA

  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);                                // ожидание окончания передачи данных DMA
  at45_cs_reset();
  osSemaphoreRelease(FlashSemDMA);                                               // отдаем семафор для работы функции AT45_TxDMA

  At45SetCommand_Adr(AT45cmdWrThroughPage1WE,pAdrSpfl);                         // команда на запись страницы
  at45_cs_reset();
  
  WaitBusyAT45(AT45WrPageTimeWE,timeOutCnt);

  At45SetCommand_Adr(AT45cmdCmpBuf1,pAdrSpfl);                                  // Сравнение записанных данных с данными в буффере
  at45_cs_reset();
  
  WaitBusyAT45(AT45CompTime,timeOutCnt);

  if(at45RdStatus() &  (1 << at45_flagStatusComp))                              // Если данные не в буффера не совпадают с данными в старанице
    return 1;

    
  return 0;
}
// "Холостое" считывание (первые 2 байта)
void IdleReadingAT45(void)
{
  READ_REG(SPI3->DR);
  SPI3->DR = 0x0A;                                              
  while(!(SPI3->SR & SPI_SR_RXNE));
  READ_REG(SPI3->DR);
}

// обработчик прерывания по окончанию передачи/приема данных ДМА
void Flash_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
  uint32_t tmpisr;
  DMA_TypeDef *regs = (DMA_TypeDef *)hdma->StreamBaseAddress;
  tmpisr = regs->LISR;
  
  if ((tmpisr & (DMA_FLAG_TCIF0_4 << hdma->StreamIndex)) != RESET)
  {
    if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC) != RESET)
    {
        regs->LIFCR = DMA_FLAG_TCIF0_4 << hdma->StreamIndex;                    // Очистить флаг прерывания
      
        if((hdma->Instance->CR & DMA_SxCR_CIRC) == RESET)
        {
          hdma->Instance->CR  &= ~(DMA_IT_TC);                                  // Выключить прерывание по завершению приема/передачи
        }
    }
  }
  osSemaphoreRelease(FlashSemDMA);
}

//*****************************************************************************//
//     Модули для W25Q16
//*****************************************************************************//

//============================================================================
//   Настройка порта для обмена с Flash W25Q16
//============================================================================
void setSpiForW25(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();                         // Включить тактирование порта SPI
  
  // 1) Настроить порты
  // Порт SPI
    
  /**SPI4 GPIO Configuration    
  PE4     ------> SPI4_SCK
  PE5     ------> SPI4_MISO
  PE6     ------> SPI4_MOSI 
  */

  GPIO_InitStruct.Pin = W25_CLK_MK_Pin | W25_RX_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
  HAL_GPIO_Init(W25_CLK_MK_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = W25_TX_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
  HAL_GPIO_Init(W25_TX_MK_GPIO_Port, &GPIO_InitStruct);
    
  // Ноги CS // и без Reset
  HAL_GPIO_WritePin(W25_CS_MK_GPIO_Port, W25_CS_MK_Pin, GPIO_PIN_RESET);
  //HAL_GPIO_WritePin(W25_RES_MK_GPIO_Port, W25_RES_MK_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = W25_CS_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(W25_CS_MK_GPIO_Port, &GPIO_InitStruct);

//  /*Configure GPIO pins : PDPin PDPin */
//  GPIO_InitStruct.Pin = FLASH_RES_MK_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(W25_RES_MK_GPIO_Port, &GPIO_InitStruct);
  
  __HAL_RCC_SPI4_CLK_ENABLE(); // Включить тактирование модуля SPI 
  
  // 2) Настроить SPI
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;

  HAL_SPI_Init(&hspi4);                   // Применить настройки 

  // настойка ДМА RX
  hdma_spi4_rx.Instance = DMA2_Stream3;
  hdma_spi4_rx.Init.Channel = DMA_CHANNEL_5;
  hdma_spi4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_spi4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi4_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi4_rx.Init.Mode = DMA_NORMAL;
  hdma_spi4_rx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_spi4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_spi4_rx);

  __HAL_LINKDMA(&hspi4, hdmarx,hdma_spi4_rx);                                    // Привязать инициализированный DMA к SPI

  // настойка ДМА TX
  hdma_spi4_tx.Instance = DMA2_Stream1;
  hdma_spi4_tx.Init.Channel = DMA_CHANNEL_4;
  hdma_spi4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_spi4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi4_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi4_tx.Init.Mode = DMA_NORMAL;
  hdma_spi4_tx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_spi4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_spi4_tx);

  // Привязать инициализированный DMA к SPI
  __HAL_LINKDMA(&hspi4, hdmatx,hdma_spi4_tx);                                    

  // Включить прерывания DMA
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  
//  // для работы с Flash => FLASH_RES_MK_Pin = 1
//  HAL_GPIO_WritePin(FLASH_RES_MK_GPIO_Port, FLASH_RES_MK_Pin, GPIO_PIN_SET);    

  // включить SPI
  SPI4->CR1 |= SPI_CR1_SPE;
  
  // Семофор для события окончания работы ДМА( чтение/запись)
  FlashSemDMA = osSemaphoreNew(1, 1, &FlashSemDMA_attributes);  
  
  W25_cs_Set();
  DelayNop();
  W25_cs_Reset();
  
  W25_cs_Reset();                                            // Сброс Flash памяти 
}

// передача/прием одного байта по SPI
u8 W25qxx_Spi(u8 Data){
   u8 ret;
   HAL_SPI_TransmitReceive(&hspi4, &Data, &ret, 1, 100);
   return ret;	
}

// Ждать пока Flash освободится
void WaitBusyW25(u16 tick){  
  u8 Status;
  
  // 1) CS Low
  W25_cs_Set();
  
  // 2) Передать команду на чтение статуса Flash
  W25qxx_Spi(W25CmdStatusRead);
  
  // 3) Ждать пока Flash освободится
  do{
     Status = W25qxx_Spi(W25_DUMMY_BYTE);  // Прочитать статус
     osDelay(tick);
  }
  while ((Status & 0x01) == 0x01);
  
  // 4) CS High
  W25_cs_Reset();
}   

// Чтение ID c микросхемы Flash памяти
u32 W25ReadID(void){
  u8  Id0, Id1, Id2; 
  u32 Id;
  
  // 1) CS Low
  W25_cs_Set();
  
  // 2) Передача команды в Flash на чтение ID
  W25qxx_Spi(W25CmdRdID);
  
  // 3) Прочитать Id
  Id0 = W25qxx_Spi(W25_DUMMY_BYTE);
  Id1 = W25qxx_Spi(W25_DUMMY_BYTE);
  Id2 = W25qxx_Spi(W25_DUMMY_BYTE);
  
  // 4) CS Hight
  W25_cs_Reset();
  
  // 5) Собрать из 3 запросов один ответ
  Id = (Id0 << 16) | (Id1 << 8) | Id2;
  
  return Id;
}

// Сброс Flash памяти
void W25Reset(void){
  
  // 1) CS Low
  W25_cs_Set();
  
  // 2) Передача команды в Flash на Разрешение сброса
  W25qxx_Spi(W25CmdEnableReset);
  
  // 3) Передача команды на сброс
  W25qxx_Spi(W25CmdReset);
  
  // 4) CS Hight
  W25_cs_Reset();
  
  // 5 Ждем готовности Flash
  WaitBusyW25(5);
}

// Разрешить запись во Flash
void W25WriteEnable(void){
  // 1) CS Low
  W25_cs_Set();
  
  // 2) Записать команду 
  W25qxx_Spi(W25CmdWrEn);
  
  // 3) CS Hight
  W25_cs_Reset();
  
  // 4) Ждем готовности Flash
  WaitBusyW25(1);
}

// Стереть чип полностью
void W25EraseChip(void){
  // 1) Разрешить запись
  W25WriteEnable();
  
  // 2) CS Low
  W25_cs_Set();
 	
  // 3) Передать команду на стирани
  W25qxx_Spi(W25CmdEraseChip);
  
  // 4) CS Hight
  W25_cs_Reset();

  // 5) Ждем пока сотрется чип
  WaitBusyW25(10);
}

// Стереть сектор
void W25EraseSector(u16 NumSector){
  u32 Sector;
  
  // 1) Ждать готовности
  WaitBusyW25(1);
  
  // 2) Вичислить адрес сектора для стирания по адресу записи
  Sector = NumSector * 4096;;
  
  // 3) Разрешить запись
  W25WriteEnable();
  
  // 4) CS Low
  W25_cs_Set();
 
  // 5) Записать команду на стирание сектора
  W25qxx_Spi(W25CmdEraseSector);
  
  // 6) Передать адрес сектора
  W25qxx_Spi((Sector & 0xFF0000) >> 16);
  W25qxx_Spi((Sector & 0xFF00) >> 8);
  W25qxx_Spi(Sector & 0xFF);
  
  // 7) CS Hight
  W25_cs_Reset(); 
  
  // 8) Ждем пока сотрется сектор
  WaitBusyW25(1);
}

// Чтение статуса Flash
u8 W25RdStatus(void){
  u8 Status; 

  // 1) CS Low
  W25_cs_Set();
  
  // 2) Передача команды в Flash на чтение ID
  W25qxx_Spi(W25CmdStatusRead);
  
  // 3) Прочитать Id
  Status = W25qxx_Spi(W25_DUMMY_BYTE);
    
  // 4) CS Hight
  W25_cs_Reset();
  
  return Status;
}

// Функция задания команды и адреса
void SetCommandAdrW25(u8 command, void *adr){
  u32 Adr;
    
  // 1) Получить адрес
  Adr = *((u32*)adr);
  
  // 2) Записать команду на стирание сектора
  W25qxx_Spi(command);
  
  // 3) Передать адрес сектора
  W25qxx_Spi((Adr & 0xFF0000) >> 16);
  W25qxx_Spi((Adr & 0xFF00) >> 8);
  W25qxx_Spi(Adr & 0xFF);
}

// Настойка ДМА на передачу данных
void W25TxDMA(u32 SrcAddress, u32 DstAddress, u32 DataLength){
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);
  DMA2_Stream1->CR &=  ~(1 << DMA_SxCR_EN);
  
  DMA2_Stream1->NDTR = DataLength;                                              // запись длинны передаваемых данных
  DMA2_Stream1->PAR  = DstAddress;                                              // куда будем перелавать данные ( регистр данных SPI)
  DMA2_Stream1->M0AR = SrcAddress;                                              // запись данных которые нужно передат
  
  DMA2_Stream1->CR |=  (1 << DMA_SxCR_MINC_Pos);                                // включить инкремент памяти 
  DMA2_Stream1->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;                       // включить прерывание по ДМА
  DMA2_Stream1->CR |=  DMA_SxCR_EN;                                             // включить ДМА
  
  SPI4->CR2 |= (SPI_CR2_TXDMAEN);                                               // разрешить запрос ДМА по окончанию передачи                                
}

// Настойка ДМА на приема данных 
void W25RxDMA(u32 SrcAddress, u32 DstAddress, u32 DataLength){
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);
  
  static u8 _filler = W25_DUMMY_BYTE;
  DMA2_Stream3->CR &=  ~(1 << DMA_SxCR_EN);
  
  DMA2_Stream3->NDTR = DataLength;                                              // запись длинны передаваемых данных
  DMA2_Stream3->PAR  = DstAddress;                                              // куда будем перелавать данные ( регистр данных SPI)
  DMA2_Stream3->M0AR = SrcAddress;                                              // запись данных которые нужно передат
  
  DMA2_Stream3->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;                       // включить прерывание по ДМА
  DMA2_Stream3->CR |=  DMA_SxCR_EN;                                             // включить ДМА
  
  //--------настойка ДМА на передачу-----------//
  DMA2_Stream1->CR &=  ~(1 << DMA_SxCR_EN);
  
  DMA2_Stream1->NDTR = DataLength;                                              // запись длинны передаваемых данных
  DMA2_Stream1->PAR  = DstAddress;                                              // куда будем перелавать данные ( регистр данных SPI)
  DMA2_Stream1->M0AR = (u32)(&_filler);                                         // запись данных которые нужно передат
  
  DMA2_Stream1->CR  |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;                      // включить прерывание по ДМА
  DMA2_Stream1->CR  &=  ~(1 << DMA_SxCR_MINC_Pos);                              // отключить инкремент памяти
  DMA2_Stream1->CR  |=  DMA_SxCR_EN;                                            // включить ДМА
  
  SPI4->CR2 |= (SPI_CR2_RXDMAEN);
  SPI4->CR2 |= (SPI_CR2_TXDMAEN);                                               // разрешить запрос ДМА по окончанию приема
}

// Чтение данные с Flash
u16 spflRdW25( void *AdrMk, void *AdrSpfl, u16 size){
  u16 *pAdrMk;
  
  pAdrMk   = (u16*)AdrMk;
  
  // 1) CS Low
  W25_cs_Set();
  
  // 2) Передать команду и адрес во Flash 
  SetCommandAdrW25(W25CmdRdAr, &AdrSpfl);
  IdleReadingW25();
  
  // 3) Запустить чтение по DMA
  W25RxDMA((u32)pAdrMk, (u32)&SPI4->DR, size);                                // Запустить чтение данных с DataFlash через DMA
    
  // 4) Ждать на симофоре пока прочитаются данные
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);                                // ожидание окончания передачи данных DMA
  
  // 5) СS Hight
  W25_cs_Reset();
  
  // 6) Отпустить симофор - данные прочитаны
  osSemaphoreRelease(FlashSemDMA);
  
  // 7) SC High
  W25_cs_Reset();
  
  return 0;
}

// Записать страницу
u16 spflWrStW25(u32 AdrFlash, u32 AdrMk, u16 size){
  
  // 1) Ждать выполнение предыдущих операций
  WaitBusyW25(1);
  
  // 2) Разрешить запись во Flash
  W25WriteEnable();
  
  // 3) CS Low
  W25_cs_Set();
  
  // 4) Передать адресс и команду на запись
  SetCommandAdrW25(W25CmdWrAr, &AdrFlash);                     // команда на заполнение буффера Flash
    
  // 5) Передать данные на запись через DMA
  W25TxDMA(AdrMk, (u32)&SPI4->DR, size);                    // начать передачу данных с DMA

  // 6) Ждем завершения зписи данных
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);              // ожидание окончания передачи данных DMA
  
  // 7) CS Hight
  W25_cs_Reset();
  
  // 8) Сообщить что данные записаны
  osSemaphoreRelease(FlashSemDMA);
    
  return 0;
}

// запись данных  во Flash
u16 spflWrSektorW25(void *AdrSpfl, void *AdrMk, u16 size){
  u32      AdrInFlash, AdrInMk; 
  u16      SizeToEndPage, SazeWr, NumPage;
  
  AdrInFlash = (u32)AdrSpfl;
  AdrInMk = (u32)AdrMk;
  SazeWr = size;
  
  // 1) Вычислить количество байт до конца текущей страниции
  SizeToEndPage = W25SizePage - (AdrInFlash % W25SizePage);		// колличество байт до конца страницы
  
  // 2) Проверить поместятся ли данные на текущей странице
  if(SizeToEndPage < size){ 
     // 3) Определить сколько нужно дописать данных до конца текущей страници
     SazeWr -= SizeToEndPage;
    
     // 4) Записать данные во Flash
     spflWrStW25(AdrInFlash, AdrInMk, SizeToEndPage);  // Пишем до конца страници
     
     // 5) Вычислить смищение в данных и Flash для записи оставшихся данных в следующей странице
     AdrInMk += SizeToEndPage;
     AdrInFlash += SizeToEndPage;
  }
  
  // 6) Проверить может уперлись в конец сектора
  if((AdrInFlash % W25SizeSector) == 0){             // Запрос на запись в начало сектора
      // 7) Ждем пока Flash будет свободна
      WaitBusyW25(1);
    
      // 8) Опредилить номер следующего сектора в который необходимо дописать данные
      NumPage = (u32)(AdrInFlash) / W25SizeSector;
    
      // 9) Стереть сектор
      W25EraseSector(NumPage);
  }
  
  // 10) Дописать оставшееся данные
  spflWrStW25(AdrInFlash, AdrInMk, SazeWr);
    
  return 0;
}

// "Холостое" считывание (первые 2 байта)
void IdleReadingW25(void){
  READ_REG(SPI4->DR);
  SPI4->DR = W25_DUMMY_BYTE;                                              
  while(!(SPI4->SR & SPI_SR_RXNE));
  READ_REG(SPI4->DR);
}

// обработчик прерывания по окончанию передачи/приема данных ДМА
void W25_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
  uint32_t tmpisr;
  DMA_TypeDef *regs = (DMA_TypeDef *)hdma->StreamBaseAddress;
  tmpisr = regs->LISR;
  
  if ((tmpisr & (DMA_FLAG_TCIF0_4 << hdma->StreamIndex)) != RESET)
  {
    if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC) != RESET)
    {
        regs->LIFCR = DMA_FLAG_TCIF0_4 << hdma->StreamIndex;                    // Очистить флаг прерывания
      
        if((hdma->Instance->CR & DMA_SxCR_CIRC) == RESET)
        {
          hdma->Instance->CR  &= ~(DMA_IT_TC);                                  // Выключить прерывание по завершению приема/передачи
        }
    }
  }
  
  osSemaphoreRelease(FlashSemDMA);
}


#endif