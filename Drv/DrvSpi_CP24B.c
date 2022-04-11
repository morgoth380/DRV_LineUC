#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
#include "DrvSpi_CP24B.h"


SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi4;
DMA_HandleTypeDef hdma_spi4_rx;
DMA_HandleTypeDef hdma_spi4_tx;

u8 FlashComBuf[4];                                 // Команды для Flash
u8 FRAMcommBuf[3];

// Прототипы функций
u32 FlashReadID(void);                                            // Чтение ID c микросхемы Flash памяти
void FlashReset(void);                                            // Сброс Flash памяти 


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


//============================================================================
//   Настройка порта для обмена с Flash W25Q16
//============================================================================
void setSpiForAt45(void)
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

  GPIO_InitStruct.Pin = FLASH_CLK_MK_Pin|FLASH_RX_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
  HAL_GPIO_Init(FLASH_CLK_MK_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FLASH_TX_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
  HAL_GPIO_Init(FLASH_TX_MK_GPIO_Port, &GPIO_InitStruct);
    
  // Ноги CS и Reset
  HAL_GPIO_WritePin(FLASH_CS_MK_GPIO_Port, FLASH_CS_MK_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(FLASH_RES_MK_GPIO_Port, FLASH_RES_MK_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = FLASH_CS_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(FLASH_CS_MK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin */
  GPIO_InitStruct.Pin = FLASH_RES_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_RES_MK_GPIO_Port, &GPIO_InitStruct);
  
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
  
  // для работы с Flash => FLASH_RES_MK_Pin = 1
  HAL_GPIO_WritePin(FLASH_RES_MK_GPIO_Port, FLASH_RES_MK_Pin, GPIO_PIN_SET);    

  // включить SPI
  SPI4->CR1 |= SPI_CR1_SPE;
  
  // Семофор для события окончания работы ДМА( чтение/запись)
  FlashSemDMA = osSemaphoreNew(1, 1, &FlashSemDMA_attributes);  
  
  FlashCsSet();
  DelayNop();
  FlashCsReset();
  
  FlashReset();                                            // Сброс Flash памяти 
}

// передача/прием одного байта по SPI
u8 W25qxx_Spi(u8 Data){
   u8 ret;
   HAL_SPI_TransmitReceive(&hspi4, &Data, &ret, 1, 100);
   return ret;	
}

// Ждать пока Flash освободится
void WaitBusy(u16 tick){  
  u8 Status;
  
  // 1) CS Low
  FlashCsSet();
  
  // 2) Передать команду на чтение статуса Flash
  W25qxx_Spi(FlashCmdStatusRead);
  
  // 3) Ждать пока Flash освободится
  do{
     Status = W25qxx_Spi(FLASH_DUMMY_BYTE);  // Прочитать статус
     osDelay(tick);
  }
  while ((Status & 0x01) == 0x01);
  
  // 4) CS High
  FlashCsReset();
}   

// Чтение ID c микросхемы Flash памяти
u32 FlashReadID(void){
  u8  Id0, Id1, Id2; 
  u32 Id;
  
  // 1) CS Low
  FlashCsSet();
  
  // 2) Передача команды в Flash на чтение ID
  W25qxx_Spi(FlashCmdRdID);
  
  // 3) Прочитать Id
  Id0 = W25qxx_Spi(FLASH_DUMMY_BYTE);
  Id1 = W25qxx_Spi(FLASH_DUMMY_BYTE);
  Id2 = W25qxx_Spi(FLASH_DUMMY_BYTE);
  
  // 4) CS Hight
  FlashCsReset();
  
  // 5) Собрать из 3 запросов один ответ
  Id = (Id0 << 16) | (Id1 << 8) | Id2;
  
  return Id;
}

// Сброс Flash памяти
void FlashReset(void){
  
  // 1) CS Low
  FlashCsSet();
  
  // 2) Передача команды в Flash на Разрешение сброса
  W25qxx_Spi(FlashCmdEnableReset);
  
  // 3) Передача команды на сброс
  W25qxx_Spi(FlashCmdReset);
  
  // 4) CS Hight
  FlashCsReset();
  
  // 5 Ждем готовности Flash
  WaitBusy(5);
}

// Разрешить запись во Flash
void FlashWriteEnable(void){
  // 1) CS Low
  FlashCsSet();
  
  // 2) Записать команду 
  W25qxx_Spi(FlashCmdWrEn);
  
  // 3) CS Hight
  FlashCsReset();
  
  // 4) Ждем готовности Flash
  WaitBusy(1);
}

// Стереть чип полностью
void FlashEraseChip(void){
  // 1) Разрешить запись
  FlashWriteEnable();
  
  // 2) CS Low
  FlashCsSet();
 	
  // 3) Передать команду на стирани
  W25qxx_Spi(FlashCmdEraseChip);
  
  // 4) CS Hight
  FlashCsReset();

  // 5) Ждем пока сотрется чип
  WaitBusy(10);
}

// Стереть сектор
void FlashEraseSector(u16 NumSector){
  u32 Sector;
  
  // 1) Ждать готовности
  WaitBusy(1);
  
  // 2) Вичислить адрес сектора для стирания по адресу записи
  Sector = NumSector * 4096;;
  
  // 3) Разрешить запись
  FlashWriteEnable();
  
  // 4) CS Low
  FlashCsSet();
 
  // 5) Записать команду на стирание сектора
  W25qxx_Spi(FlashCmdEraseSector);
  
  // 6) Передать адрес сектора
  W25qxx_Spi((Sector & 0xFF0000) >> 16);
  W25qxx_Spi((Sector & 0xFF00) >> 8);
  W25qxx_Spi(Sector & 0xFF);
  
  // 7) CS Hight
  FlashCsReset(); 
  
  // 8) Ждем пока сотрется сектор
  WaitBusy(1);
}

// Чтение статуса Flash
u8 FlashRdStatus(void){
  u8 Status; 

  // 1) CS Low
  FlashCsSet();
  
  // 2) Передача команды в Flash на чтение ID
  W25qxx_Spi(FlashCmdStatusRead);
  
  // 3) Прочитать Id
  Status = W25qxx_Spi(FLASH_DUMMY_BYTE);
    
  // 4) CS Hight
  FlashCsReset();
  
  return Status;
}

// Функция задания команды и адреса
void SetCommandAdr(u8 command, void *adr){
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
void FlashTxDMA(u32 SrcAddress, u32 DstAddress, u32 DataLength){
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
void FlashRxDMA(u32 SrcAddress, u32 DstAddress, u32 DataLength){
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);
  
  static u8 _filler = FLASH_DUMMY_BYTE;
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
u16 spflRd( void *AdrMk, void *AdrSpfl, u16 size){
  u16 *pAdrMk;
  
  pAdrMk   = (u16*)AdrMk;
  
  // 1) CS Low
  FlashCsSet();
  
  // 2) Передать команду и адрес во Flash 
  SetCommandAdr(FlashCmdRdAr, &AdrSpfl);
  IdleReading();
  
  // 3) Запустить чтение по DMA
  FlashRxDMA((u32)pAdrMk, (u32)&SPI4->DR, size);                                // Запустить чтение данных с DataFlash через DMA
    
  // 4) Ждать на симофоре пока прочитаются данные
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);                                // ожидание окончания передачи данных DMA
  
  // 5) СS Hight
  FlashCsReset();
  
  // 6) Отпустить симофор - данные прочитаны
  osSemaphoreRelease(FlashSemDMA);
  
  // 7) SC High
  FlashCsReset();
  
  return 0;
}

// Записать страницу
u16 spflWrSt(u32 AdrFlash, u32 AdrMk, u16 size){
  
  // 1) Ждать выполнение предыдущих операций
  WaitBusy(1);
  
  // 2) Разрешить запись во Flash
  FlashWriteEnable();
  
  // 3) CS Low
  FlashCsSet();
  
  // 4) Передать адресс и команду на запись
  SetCommandAdr(FlashCmdWrAr, &AdrFlash);                     // команда на заполнение буффера Flash
    
  // 5) Передать данные на запись через DMA
  FlashTxDMA(AdrMk, (u32)&SPI4->DR, size);                    // начать передачу данных с DMA

  // 6) Ждем завершения зписи данных
  osSemaphoreAcquire(FlashSemDMA, portMAX_DELAY);              // ожидание окончания передачи данных DMA
  
  // 7) CS Hight
  FlashCsReset();
  
  // 8) Сообщить что данные записаны
  osSemaphoreRelease(FlashSemDMA);
    
  return 0;
}

// запись данных  во Flash
u16 spflWrSektor(void *AdrSpfl, void *AdrMk, u16 size){
  u32      AdrInFlash, AdrInMk; 
  u16      SizeToEndPage, SazeWr, NumPage;
  
  AdrInFlash = (u32)AdrSpfl;
  AdrInMk = (u32)AdrMk;
  SazeWr = size;
  
  // 1) Вычислить количество байт до конца текущей страниции
  SizeToEndPage = FlashSizePage - (AdrInFlash % FlashSizePage);		// колличество байт до конца страницы
  
  // 2) Проверить поместятся ли данные на текущей странице
  if(SizeToEndPage < size){ 
     // 3) Определить сколько нужно дописать данных до конца текущей страници
     SazeWr -= SizeToEndPage;
    
     // 4) Записать данные во Flash
     spflWrSt(AdrInFlash, AdrInMk, SizeToEndPage);  // Пишем до конца страници
     
     // 5) Вычислить смищение в данных и Flash для записи оставшихся данных в следующей странице
     AdrInMk += SizeToEndPage;
     AdrInFlash += SizeToEndPage;
  }
  
  // 6) Проверить может уперлись в конец сектора
  if((AdrInFlash % FlashSizeSector) == 0){             // Запрос на запись в начало сектора
      // 7) Ждем пока Flash будет свободна
      WaitBusy(1);
    
      // 8) Опредилить номер следующего сектора в который необходимо дописать данные
      NumPage = (u32)(AdrInFlash) / FlashSizeSector;
    
      // 9) Стереть сектор
      FlashEraseSector(NumPage);
  }
  
  // 10) Дописать оставшееся данные
  spflWrSt(AdrInFlash, AdrInMk, SazeWr);
    
  return 0;
}

// "Холостое" считывание (первые 2 байта)
void IdleReading(void){
  READ_REG(SPI4->DR);
  SPI4->DR = FLASH_DUMMY_BYTE;                                              
  while(!(SPI4->SR & SPI_SR_RXNE));
  READ_REG(SPI4->DR);
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
#endif