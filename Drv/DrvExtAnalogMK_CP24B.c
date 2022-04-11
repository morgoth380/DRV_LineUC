#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "DrvExtAnalogMK_CP24B.h"
#include "string.h"
#include "Global_Include.h"


UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;


DataTx_type UartTx;                    // Буффер порта на передачу
DataRx_type UartRx;                    // Данные полученные от слейва
u8 UartBufRx[sizeof(DataRx_type) + 5]; // Буффер порта на прием

// Прототипы Функций
void MkarInitGPIO(void);                                  // Настройка GPIO UART
void DmaMkarInit(void);                                   // Настройка DMA для работы с MKAR 
void UART4Init(u32 Speed);                                // Настроить UART №4 


// Настроить UART
void DrvMkarInit(EXTANALOG_handle e){

  // 1) Настроить GPIO
  MkarInitGPIO();          // Настроить GPIO
 
  // 2) Настроить порт АСУ
  DmaMkarInit();           // Настроить DMA
  UART4Init(921600);      // Настроить порт  
  
  // 3) Определить указатели на массивы
  e->DataRx = &UartRx;
  e->DataTx = &UartTx;
  
  // 4) Прочистить приемный буффер
  memset((void*)&UartRx, 0, sizeof(DataRx_type));
}

// Настройка GPIO UART
void MkarInitGPIO(void){
   GPIO_InitTypeDef GPIO_InitStruct = {0};
  
   // 1) Включить тактирование портов
   __HAL_RCC_GPIOC_CLK_ENABLE();
   
   // 2) Настроить GPIO TX UART
   GPIO_InitStruct.Pin = EXTANALOG_TX_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
   HAL_GPIO_Init(EXTANALOG_TX_PORT, &GPIO_InitStruct);

   // 3) Настроить GPIO RX UART
   GPIO_InitStruct.Pin = EXTANALOG_RX_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
   HAL_GPIO_Init(EXTANALOG_RX_PORT, &GPIO_InitStruct);

}

// Настройка DMA для работы с портом АСУ
void DmaMkarInit(void){                                     
   // DMA для UART4 
   // DMA controller clock enable 
   __HAL_RCC_DMA1_CLK_ENABLE();

   // DMA interrupt init 
   // DMA1_Stream4_IRQn interrupt configuration 
   HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 8, 0);
   HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  
   // DMA1_Stream2_IRQn interrupt configuration 
   HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 8, 0);
   HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
   
   // USART4 clock enable 
   __HAL_RCC_UART4_CLK_ENABLE();
  
   hdma_uart4_rx.Instance = DMA1_Stream2;
   hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
   hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
   hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
   hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
   hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
   hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
   hdma_uart4_rx.Init.Mode = DMA_NORMAL;
   hdma_uart4_rx.Init.Priority = DMA_PRIORITY_LOW;
   hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
   HAL_DMA_Init(&hdma_uart4_rx);
   
   __HAL_LINKDMA(&huart4, hdmarx, hdma_uart4_rx);

   // USART4_TX Init 
   hdma_uart4_tx.Instance = DMA1_Stream4;
   hdma_uart4_tx.Init.Channel = DMA_CHANNEL_4;
   hdma_uart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
   hdma_uart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
   hdma_uart4_tx.Init.MemInc = DMA_MINC_ENABLE;
   hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
   hdma_uart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
   hdma_uart4_tx.Init.Mode = DMA_NORMAL;
   hdma_uart4_tx.Init.Priority = DMA_PRIORITY_LOW;
   hdma_uart4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
   HAL_DMA_Init(&hdma_uart4_tx);
   
   __HAL_LINKDMA(&huart4 ,hdmatx, hdma_uart4_tx);

   // USART4 interrupt Init 
  // HAL_NVIC_SetPriority(UART4_IRQn, 8, 0);
  // HAL_NVIC_EnableIRQ(UART4_IRQn);
}

// Настроить UART №4
void UART4Init(u32 Speed){
  
  huart4.Instance = UART4;
  huart4.Init.BaudRate = Speed;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart4);
}

// Получить количество принятых данных
u16 GetLenDataRx(EXTANALOG_handle e){
  u16 length, maxLength;
  
  if (e->versionMKAR == Mkar_16891) {
    maxLength = sizeof(DataRx_16891_type);
  } else if (e->versionMKAR == Mkar_16892) {
    maxLength = sizeof(DataRx_type);
  }
    
  // 1) Получить количество принятых данных
  length = sizeof(UartBufRx) - huart4.hdmarx->Instance->NDTR;        
  huart4.RxXferSize = length;
  
  // 2) Проверить корректность полученной длинны
  if(length > maxLength){ 
     length = 0;           // length - не корректно
  }
  
  return length;
}

// Очистить приемный буффер
void ClearRxBff(void){
   memset((void*)UartBufRx, 0, sizeof(UartBufRx));  
}

//******************************************************************************
// вызов обработчика
//******************************************************************************
#define         MAX_LINK_COUNT  100
void DrvHandlerMkar(EXTANALOG_handle e){
  u16 lenRx, crc, dataLen;//, crc2;
  static u16 LinkCount = MAX_LINK_COUNT;
  DataRx_type  *RxData;
  DataRx_16891_type  *RxData_16891;
  
  // 1) Остановить DMA
  HAL_UART_DMAStop(&huart4);  // Остановить работу DMA
  
  // 2) Получить количество данных от аналогового контроллера
  lenRx = GetLenDataRx(e);
    
  // Если связи нету, то меняем протокол обмена по кругу
//  if ( e->MkarLinkStat == MkarNoLink ) {   
//    if (e->versionMKAR < MkarTopVersion){
//      e->versionMKAR++;
//      LinkCount = MAX_LINK_COUNT;
//      e->MkarLinkStat = MkarOkLink;
//    } else {
//      e->versionMKAR = MkarLowVersion;
//    }
//  }
    
//  e->versionMKAR = Mkar_16891;  
  
  if (e->versionMKAR == Mkar_16891) {
    // 3) Проверить crc на принятый пакет
    // Дичь какая-то при расчете CRC 
    dataLen = sizeof(DataRx_16891_type)-sizeof(u16);
    crc = Crc((u8*)((u32)UartBufRx+2), sizeof(DataRx_16891_type));
    //обмен идет а ЦРЦ не правильный
    RxData_16891 = (DataRx_16891_type*)UartBufRx; 
    
    // 3) Проверить пакет на корректность
    if((lenRx != 0)&&(crc == RxData_16891->crc)){
      // Пакет получен корректный  
      // Инкриментируем счетчик пакетов
      LinkCount++;
      if(LinkCount > MAX_LINK_COUNT){
        LinkCount = MAX_LINK_COUNT;
      }
      
      // Копируем полученный пакет
      memcpy((void*)&UartRx, (void*)UartBufRx, sizeof(DataRx_16891_type));    
    }
    else{
      // Пакет получен некорректный
      if(LinkCount > 0){
        LinkCount--;
      }
    }
    
    // 4) Анализ наличия связи со слейвом
    if(LinkCount == 0){    // нет связи с устройством
      e->MkarLinkStat = MkarNoLink;
      
      // Если связь потеряли, то переключаемся на другой протокол обмена
      e->versionMKAR = Mkar_16892;        
      LinkCount = MAX_LINK_COUNT;
      
      // Прочистить буффер с принятыми данными
      memset((void*)&UartRx, 0, sizeof(DataRx_16891_type));
    }
    else{                  // связь есть
      e->MkarLinkStat = MkarOkLink;
    }
    
    // 5) Прочистить приемный буффер
    ClearRxBff();
    // 6) Посчитать КС на передаваемый пакет
    e->DataTx->crc = Crc((u8*)((u32)e->DataTx + sizeof(u16)), sizeof(DataTx_16891_type));
    
    // 7) Зарядить передачу запроса слейву 
    huart4.gState = HAL_UART_STATE_READY;
    HAL_UART_Transmit_DMA(&huart4, (u8*)e->DataTx, sizeof(DataTx_16891_type));   // послать запрос    
    
  } else if (e->versionMKAR == Mkar_16892) {
  
    // 3) Проверить crc на принятый пакет
    // Дичь какая-то при расчете CRC 
    dataLen = sizeof(DataRx_type)-sizeof(u16);
    crc = Crc((u8*)((u32)UartBufRx+2), dataLen); // ТАК НЕ ДОЛЖНО РАБОТАТЬ, но работает - ЭТО ОШРБКА В МКАР. НЕОБХОДИМА СОВМЕСТИМОСТЬ
    //crc2 = Crc((u8*)((u32)(UartBufRx)), dataLen+2);
    
    RxData = (DataRx_type*)UartBufRx; 
    
    // 3) Проверить пакет на корректность
    if((lenRx != 0)&&(crc == RxData->crc)){
      // Пакет получен корректный  
      // Инкриментируем счетчик пакетов
      LinkCount++;
      if(LinkCount > MAX_LINK_COUNT){
        LinkCount = MAX_LINK_COUNT;
      }
      
      // Копируем полученный пакет
      memcpy((void*)&UartRx, (void*)UartBufRx, sizeof(DataRx_type));    
      SetDataExtBlock(RxData);
    }
    else{
      // Пакет получен некорректный
      if(LinkCount > 0){
        LinkCount--;
      }
    }
    
    // 4) Анализ наличия связи со слейвом
    if(LinkCount == 0){    // нет связи с устройством
      e->MkarLinkStat = MkarNoLink;
      
      // Если связь потеряли, то переключаемся на другой протокол обмена
      e->versionMKAR = Mkar_16891;        
      LinkCount = MAX_LINK_COUNT;
      
      // Прочистить буффер с принятыми данными
      memset((void*)&UartRx, 0, sizeof(DataRx_type));
    }
    else{                  // связь есть
      e->MkarLinkStat = MkarOkLink;
    }
    
    // 5) Прочистить приемный буффер
    ClearRxBff();
    //заполнить буфер заданием для блока расширения
    FillDataExtBlock();
    // 6) Посчитать КС на передаваемый пакет
    e->DataTx->crc = Crc((u8*)((u32)e->DataTx + sizeof(u16)), sizeof(DataTx_type));
    
    // 7) Зарядить передачу запроса слейву 
    huart4.gState = HAL_UART_STATE_READY;
    HAL_UART_Transmit_DMA(&huart4, (u8*)e->DataTx, sizeof(DataTx_type));   // послать запрос
    
  }
  
  // 8) Настроить прием ответа от слейва
  HAL_UART_Receive_DMA(&huart4, UartBufRx, sizeof(UartBufRx));  // Начать прием данных
}

void SetDataExtBlock(DataRx_type* ExtBlockData)
{  
  AinAoutTx_type *AinAoutTx; 
  AinAoutRx_type *AinAoutRx;
  u16 calcCrc, crc;
  
//#warning ДЛя стыковки МКАРа еще отрубил здесь блок иначе рагуется на отсутствие структур.  
  
  switch(ExtBlockData->extBlockType.extType){
    case DIN_DOUT_EXT_BLOCK:
        GlobalM4.extConnectedBlocks.ext3.extType          = DIN_DOUT_EXT_BLOCK;
        GlobalM4.extConnectedBlocks.ext3.crc              = ExtBlockData->extBlockType.crc;
        GlobalM4.extConnectedBlocks.ext3.softSubVersion   = ExtBlockData->extBlockType.softSubVersion;
        GlobalM4.extConnectedBlocks.ext3.softVersion      = ExtBlockData->extBlockType.softVersion;
        GlobalM4.DinDoutExtData.DinState.dinStateWord     = ExtBlockData->extBlockData[0];
        GlobalM4.DinDoutExtData.softVersion               = ExtBlockData->extBlockType.softVersion;
        GlobalM4.DinDoutExtData.softSubVersion            = ExtBlockData->extBlockType.softSubVersion;
        GlobalM4.DinDoutExtData.softCRC                   = ExtBlockData->extBlockType.crc;
    break;
    case AIN_AOUT_EXT_BLOCK:
        GlobalM4.extConnectedBlocks.ext3.extType          = AIN_AOUT_EXT_BLOCK;
        GlobalM4.extConnectedBlocks.ext3.crc              = ExtBlockData->extBlockType.crc;
        GlobalM4.extConnectedBlocks.ext3.softSubVersion   = ExtBlockData->extBlockType.softSubVersion;
        GlobalM4.extConnectedBlocks.ext3.softVersion      = ExtBlockData->extBlockType.softVersion;
                
        AinAoutRx = (AinAoutRx_type*)&ExtBlockData->extBlockData[0];
        calcCrc = Crc((u8*)AinAoutRx + 2, sizeof(AinAoutRx_type) - sizeof(AinAoutRx->crc));
        crc = AinAoutRx->crc;
        if(calcCrc == crc){
          GlobalM4.AinAoutExtData.Ain1Val = AinAoutRx->Ain1Val;
          GlobalM4.AinAoutExtData.Ain2Val = AinAoutRx->Ain2Val;
          GlobalM4.AinAoutExtData.Adc1Val = AinAoutRx->Adc1Val;
          GlobalM4.AinAoutExtData.Adc2Val = AinAoutRx->Adc2Val;
          GlobalM4.AinAoutExtData.AinState1 = AinAoutRx->AinState1;
          GlobalM4.AinAoutExtData.AinState2 = AinAoutRx->AinState2;
          GlobalM4.AinAoutExtData.Aout1Val = AinAoutRx->Aout1Val;
          GlobalM4.AinAoutExtData.Aout2Val = AinAoutRx->Aout2Val;    
        }
        
    break; 
    case NOT_DEF:
        GlobalM4.extConnectedBlocks.ext3.extType          = NOT_DEF;
        GlobalM4.extConnectedBlocks.ext3.crc              = 0;
        GlobalM4.extConnectedBlocks.ext3.softSubVersion   = 0;
        GlobalM4.extConnectedBlocks.ext3.softVersion      = 0;
    break;
  }
}
void FillDataExtBlock()
{
  AinAoutTx_type *AinAoutTx;
//#warning ДЛя стыковки МКАРа еще отрубил здесь блок иначе рагуется на отсутствие структур.    
  switch(GlobalM4.extConnectedBlocks.ext3.extType){
    case DIN_DOUT_EXT_BLOCK:
        UartTx.ExtBlockTask[0] = GlobalM4.Dout4;    
        UartTx.ExtBlockTask[1] = GlobalM4.Dout5;  
        UartTx.extTaskCRC = Crc((u8*)&UartTx.ExtBlockTask[0] , 4);
    break;
    
    case AIN_AOUT_EXT_BLOCK:                            
        AinAoutTx = (AinAoutTx_type*)&UartTx.ExtBlockTask[0];
        
        AinAoutTx->Ain1Type = GlobalM4.AinAoutExtData.Ain1Type;
        AinAoutTx->Ain2Type = GlobalM4.AinAoutExtData.Ain2Type;
        AinAoutTx->Aout1ExtType = GlobalM4.AinAoutExtData.Aout1ExtType;
        AinAoutTx->Aout2ExtType = GlobalM4.AinAoutExtData.Aout2ExtType;
        AinAoutTx->Status = GlobalM4.AinAoutExtData.Status;
        AinAoutTx->TaskAout1 = GlobalM4.AinAoutExtData.TaskAout1;
        AinAoutTx->TaskAout2 = GlobalM4.AinAoutExtData.TaskAout2;
       
        AinAoutTx->crc = Crc((u8*)AinAoutTx + 2, sizeof(AinAoutTx_type) - sizeof(u16));                
        //AinAoutTx->crc = Crc((char *)AinAoutTx + 2, sizeof(AinAoutTx_type) - sizeof(AinAoutTx->crc));                
    break; 
  }
}
#endif