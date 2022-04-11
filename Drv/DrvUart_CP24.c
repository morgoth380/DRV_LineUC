#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "DrvUart.h"
#include "Global_include.h"
#include "main.h"
#include "cmsis_os.h"
#include "GlobalVar.h"

//extern u16     MODBUS_FILE_RDWR;
//extern u16     MODBUS_PWM_ON;
extern GlobalVar        GlobalM4;
extern SPI_HandleTypeDef hspi3;
telegramModeType ext1ExchangeState; //состояние обмена: идентификация блока или обмен данными
telegramModeType ext2ExchangeState; //состояние обмена: идентификация блока или обмен данными

uint8_t UartBuf1[szUartBff]; 
uint8_t UartBuf3[szUartBff];
uint8_t UartBuf6[szUartBff];
uint8_t UartRxBufExt2[szUartBff]; // UartBuf2[szUartBff];
uint8_t UartRxBufExt1[szUartBff]; // UartBuf5[szUartBff]
uint8_t UartTxBufExt2[szUartBff]; // UartBuf2[szUartBff];
uint8_t UartTxBufExt1[szUartBff]; // UartBuf5[szUartBff]
uint8_t UartVirtualBuf1[szUartBff]; // Буфер для виртуального UART uartVirtual1
uint8_t UartVirtualBuf2[szUartBff]; // Буфер для виртуального UART uartVirtual2

uart_type uart1; // RS-485
uart_type uart3; // WIFI
uart_type uart6; // Local panel
uart_type uartExt2; // EXT2 UART XP6
uart_type uartExt1; // EXT1 UART XP7
uart_type uartVirtual1;
uart_type uartVirtual2;    
u16 LengthDataBuf1 = 0;
u16 LengthDataBuf2 = 0;


UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
UART_HandleTypeDef EXT1_PortStructure; // USART5
UART_HandleTypeDef EXT2_PortStructure; // USART2
GPIO_PinState RS485_RX_ENABLE;
GPIO_PinState RS485_TX_ENABLE;

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart5_rx;
DMA_HandleTypeDef hdma_usart5_tx;

u16 ext1TxSkipCnt;
u16 ext2TxSkipCnt;

const u32 uartBRate[11] = {
    9600UL,     // 0
    14400UL,    // 1
    19200UL,    // 2
    38400UL,    // 3
    56000UL,    // 4
    57600UL,    // 5
    115200UL,   // 6
    230400UL,   // 7
    460800UL,   // 8
    921600UL,   // 9
    1500000UL,  // 10
};

#define EXT_BLOCK_BAUDRATE  3000000 //115200
void UART5_DMA_Init(void); // EXT1
void USART2_DMA_Init(void); // EXT2

u16 extBlockDataBufferFill(extBlockModeType extBlockType, uint8_t *extDataPnt);
void extAnswerWaitTimers(timsCmdType ext1TimCmd, timsCmdType ext2TimCmd);
u16 extBlockAnswDataSizeDef(extBlockModeType extBlockType);
void extBlockDataParsing(extNumType extNum, extBlockModeType extBlockType, uint8_t *extDataPnt);
void extBlockActualDataDef(extNumType extNum, u16 crc, u16 softVersion, u16 softSubVersion);
UART_HandleTypeDef *Get_ExtUartHandle(extNumType extNum);
u8 Get_VirtualUartDataSize(extNumType extNum);
// Р СњР В°РЎРѓРЎвЂљРЎР‚Р С•Р в„–Р С”Р В° Р С—Р С•РЎР‚РЎвЂљР С•Р Р† Р Т‘Р В»РЎРЏ РЎР‚Р В°Р В±Р С•РЎвЂљРЎвЂ№ РЎРѓ Р С—Р С•РЎР‚РЎвЂљР В°Р СР С‘ (Р С’Р РЋР Р€ Р С‘ WIFI)
void RS485_GPIO_Init(void){

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  // Определяем уровни сигналов управления направлением передачи в соответствии с типом блока СР24
  switch (GlobalM4.versionCP24){
      case CP24_v21:
        RS485_RX_ENABLE = GPIO_PIN_RESET;
        RS485_TX_ENABLE = GPIO_PIN_SET;
        break;
      case CP24_v22:
      case CP24_v23:
      case CP24_v24:
        RS485_RX_ENABLE = GPIO_PIN_SET;
        RS485_TX_ENABLE = GPIO_PIN_RESET;         
        break;
  }  
  // Ноги управления направлением передачи
  HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, RS485_RX_ENABLE);
  HAL_GPIO_WritePin(PULT_DIR_GPIO_Port, PULT_DIR_Pin, PULT_RX_ENABLE);
  
  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = RS485_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS485_DIR_GPIO_Port, &GPIO_InitStruct);
  
    /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = PULT_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PULT_DIR_GPIO_Port, &GPIO_InitStruct);
  
  // 2) Настройка ноги WIFI_PROG_ON - блокирование канала обмена WIFI
  GPIO_InitStruct.Pin = WIFI_PROG_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WIFI_PROG_ON_Port, &GPIO_InitStruct);
}

// Р СњР В°РЎРѓРЎвЂљРЎР‚Р С•Р в„–Р С”Р В° DMA Р Т‘Р В»РЎРЏ РЎР‚Р В°Р В±Р С•РЎвЂљРЎвЂ№ РЎРѓ Р С—Р С•РЎР‚РЎвЂљР В°Р СР С‘ (Р С’Р РЋР Р€ Р С‘ WIFI)
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  // hdma_usart3_rx
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 8, 0); 
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);         
  // hdma_usart3_tx
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn); 
  
  // hdma_usart6_rx
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  // hdma_usart6_tx
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  
  // hdma_usart1_tx
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
  // hdma_usart1_rx
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
}

/* USART1 init function */

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = uart1.baudrate;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{
  EXT2_PortStructure.Instance = USART2;
  EXT2_PortStructure.Init.BaudRate = EXT_BLOCK_BAUDRATE;
  EXT2_PortStructure.Init.WordLength = UART_WORDLENGTH_8B;
  EXT2_PortStructure.Init.StopBits = UART_STOPBITS_1;
  EXT2_PortStructure.Init.Parity = UART_PARITY_NONE;
  EXT2_PortStructure.Init.Mode = UART_MODE_TX_RX;
  EXT2_PortStructure.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  EXT2_PortStructure.Init.OverSampling = UART_OVERSAMPLING_8; // Для 3 Мбит только OVER8
  if (HAL_UART_Init(&EXT2_PortStructure) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USART3 init function */
void MX_USART3_UART_Init(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = uart3.baudrate;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USART5 init function */
void MX_UART5_UART_Init(void)
{
  EXT1_PortStructure.Instance = UART5;
  EXT1_PortStructure.Init.BaudRate = EXT_BLOCK_BAUDRATE;
  EXT1_PortStructure.Init.WordLength = UART_WORDLENGTH_8B;
  EXT1_PortStructure.Init.StopBits = UART_STOPBITS_1;
  EXT1_PortStructure.Init.Parity = UART_PARITY_NONE;
  EXT1_PortStructure.Init.Mode = UART_MODE_TX_RX;
  EXT1_PortStructure.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  EXT1_PortStructure.Init.OverSampling = UART_OVERSAMPLING_8; // Для 3 Мбит только OVER8
  if (HAL_UART_Init(&EXT1_PortStructure) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USART6 init function */
void MX_USART6_UART_Init(void)
{
  huart6.Instance = USART6;
  huart6.Init.BaudRate = uart6.baudrate;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_DMA1_CLK_ENABLE(); //Тактирование модуля DMA
  __HAL_RCC_DMA2_CLK_ENABLE(); //Тактирование модуля DMA
  
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream5;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = WIFI_TX_MK_Pin|WIFI_RX_MK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Stream1;
    hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3_TX Init */
    hdma_usart3_tx.Instance = DMA1_Stream3;
    hdma_usart3_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART6 GPIO Configuration    
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART6 DMA Init */
    /* USART6_RX Init */
    hdma_usart6_rx.Instance = DMA2_Stream2;
    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_rx.Init.Mode = DMA_NORMAL;
    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart6_rx);

    /* USART6_TX Init */
    hdma_usart6_tx.Instance = DMA2_Stream6;
    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart6_tx);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
  
  if(uartHandle->Instance == USART2)
  {
    /* USART2 clock enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();  //Тактирование порта D
  __HAL_RCC_USART2_CLK_ENABLE(); //Тактирование модуля USART2
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Stream5;
    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(uartHandle, hdmarx, hdma_usart2_rx);

    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Stream6;
    hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart2_tx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
  
  if(uartHandle->Instance == UART5)
  {
    /* USART5 clock enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();  //Тактирование порта D
    __HAL_RCC_GPIOC_CLK_ENABLE();  //Тактирование порта C
    __HAL_RCC_UART5_CLK_ENABLE(); //Тактирование модуля UART5
    /**USART5 GPIO Configuration    
    PC12     ------> USART5_TX
    PD2     ------> USART5_RX 
    */
    //Настройка порта PD2. Вход чтения порта UART5
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    //Настройка порта PC12. Выход передачи порта UART5
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 
    
    /* UART5 DMA Init */
    /* UART5_RX Init */
    hdma_usart5_rx.Instance = DMA1_Stream0;
    hdma_usart5_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart5_rx.Init.Mode = DMA_NORMAL;
    hdma_usart5_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart5_rx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(uartHandle, hdmarx, hdma_usart5_rx);

    /* UART5_TX Init */
    hdma_usart5_tx.Instance = DMA1_Stream7;
    hdma_usart5_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart5_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart5_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart5_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart5_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart5_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart5_tx.Init.Mode = DMA_NORMAL;
    hdma_usart5_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart5_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart5_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart5_tx);

    /* USART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
  }  
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if (uartHandle->Instance == USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
  else if (uartHandle->Instance == USART3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();  
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOD, WIFI_TX_MK_Pin|WIFI_RX_MK_Pin);
    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  }
  else if(uartHandle->Instance == USART6)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();  
    /**USART6 GPIO Configuration    
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);
    /* USART6 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    /* USART6 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  }
  else if(uartHandle->Instance == USART2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();  
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);
    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    /* USART2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }
  else if(uartHandle->Instance == UART5)
  {
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();  
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2      ------> UART5_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);
    /* UART5 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    /* UART5 interrupt DeInit */
    HAL_NVIC_DisableIRQ(UART5_IRQn);
  }  
} 

/* USER CODE BEGIN 1 */
void USER_INIT_UARTs(void)
{
  USER_INIT_UART(&huart1);
  USER_INIT_UART(&huart3);
  USER_INIT_UART(&huart6);
}
// Р В РЎСџР В РЎвЂўР В Р’В»Р РЋР Р‰Р В Р’В·Р В РЎвЂўР В Р вЂ Р В Р’В°Р РЋРІР‚С™Р В Р’ВµР В Р’В»Р РЋР Р‰Р РЋР С“Р В РЎвЂќР В Р’В°Р РЋР РЏ Р В РЎвЂР В Р вЂ¦Р В РЎвЂР РЋРІР‚В Р В РЎвЂР В Р’В°Р В Р’В»Р В РЎвЂР В Р’В·Р В Р’В°Р РЋРІР‚В Р В РЎвЂР РЋР РЏ UART
void USER_INIT_UART(UART_HandleTypeDef *huart)
{
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);          // Р В РІР‚в„ўР В РЎвЂќР В Р’В»Р РЋР вЂ№Р РЋРІР‚РЋР В РЎвЂР РЋРІР‚С™Р РЋР Р‰ Р В РЎвЂ”Р РЋР вЂљР В Р’ВµР РЋР вЂљР РЋРІР‚в„–Р В Р вЂ Р В Р’В°Р В Р вЂ¦Р В РЎвЂР В Р’Вµ Р В РЎвЂ”Р В РЎвЂў Р В РЎвЂўР В РЎвЂќР В РЎвЂўР В Р вЂ¦Р РЋРІР‚РЋР В Р’В°Р В Р вЂ¦Р В РЎвЂР РЋР вЂ№ Р В РЎвЂ”Р РЋР вЂљР В РЎвЂР В Р’ВµР В РЎВР В Р’В° Р В РўвЂР В Р’В°Р В Р вЂ¦Р В Р вЂ¦Р РЋРІР‚в„–Р РЋРІР‚В¦   
    __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
    osDelay(1);                                         // Р В Р’В·Р В Р’В°Р В РўвЂР В Р’ВµР РЋР вЂљР В Р’В¶Р В РЎвЂќР В Р’В° Р В РўвЂР В Р’В»Р РЋР РЏ Р РЋР С“Р В Р’В±Р РЋР вЂљР В РЎвЂўР РЋР С“Р В Р’В° Р В Р’В±Р В РЎвЂР РЋРІР‚С™Р В РЎвЂўР В Р вЂ 
    SetPreferences(huart);                              // Р В Р’В·Р В Р’В°Р В РўвЂР В Р’В°Р В Р вЂ¦Р В РЎвЂР РЋР РЏ Р В РЎвЂ”Р В Р’В°Р РЋР вЂљР В Р’В°Р В РЎВР В Р’ВµР РЋРІР‚С™Р РЋР вЂљР В РЎвЂўР В Р вЂ  Р В РўвЂР В Р’В»Р РЋР РЏ UART
    USER_UART_Receive_DMA(huart);                       // Р РЋР С“Р РЋРІР‚С™Р В Р’В°Р В Р’ВµР В РЎВ Р В Р вЂ¦Р В Р’В° Р В РЎвЂ”Р РЋР вЂљР В РЎвЂР В Р’ВµР В РЎВ Р В РўвЂР В Р’В°Р В Р вЂ¦Р В Р вЂ¦Р РЋРІР‚в„–Р РЋРІР‚В¦
    CLEAR_BIT(huart->Instance->SR, USART_SR_TC);        // Р В РЎвЂєР РЋРІР‚РЋР В РЎвЂР РЋР С“Р РЋРІР‚С™Р В РЎвЂќР В Р’В° Р РЋРІР‚С›Р В Р’В»Р В Р’В°Р В РЎвЂ“Р В Р’В° Р В РЎвЂўР В РЎвЂќР В РЎвЂўР В Р вЂ¦Р РЋРІР‚РЋР В Р’В°Р В Р вЂ¦Р В РЎвЂР РЋР РЏ Р В РЎвЂ”Р В Р’ВµР РЋР вЂљР В Р’ВµР В РўвЂР В Р’В°Р РЋРІР‚РЋР В РЎвЂ  
    READ_REG(huart->Instance->DR);                      // Р В РЎвЂєР РЋРІР‚РЋР В РЎвЂР РЋР С“Р РЋРІР‚С™Р В РЎвЂќР В Р’В° Р РЋРІР‚С›Р В Р’В»Р В Р’В°Р В РЎвЂ“Р В Р’В° Р В РЎвЂўР В РЎвЂќР В РЎвЂўР В Р вЂ¦Р РЋРІР‚РЋР В Р’В°Р В Р вЂ¦Р В РЎвЂР РЋР РЏ Р В РЎвЂ”Р В Р’ВµР РЋР вЂљР В Р’ВµР В РўвЂР В Р’В°Р РЋРІР‚РЋР В РЎвЂ 
}

// Р В РЎвЂєР В Р’В±Р РЋР вЂљР В Р’В°Р В Р’В±Р В РЎвЂўР РЋРІР‚С™Р РЋРІР‚РЋР В РЎвЂР В РЎвЂќ Р В РЎвЂ”Р РЋР вЂљР В Р’ВµР РЋР вЂљР РЋРІР‚в„–Р В Р вЂ Р В Р’В°Р В Р вЂ¦Р В РЎвЂР В РІвЂћвЂ“ Р В РЎвЂ”Р В РЎвЂў UART
void USER_UART_IT(UART_HandleTypeDef *huart)
{
  uint16_t length = 0;
  if ((huart->Instance->SR & USART_SR_TC) != RESET)                     // Р В Р’В·Р В Р’В°Р В РЎвЂќР В РЎвЂўР В Р вЂ¦Р РЋРІР‚РЋР В РЎвЂР В Р’В»Р В РЎвЂ Р В РЎвЂ”Р В Р’ВµР РЋР вЂљР В Р’ВµР В РўвЂР В Р’В°Р РЋРІР‚РЋР РЋРЎвЂњ
  {
    CLEAR_BIT(huart->Instance->SR, USART_SR_TC);
    READ_REG(huart->Instance->DR);                                      // Р В РЎвЂєР РЋРІР‚РЋР В РЎвЂР РЋР С“Р РЋРІР‚С™Р В РЎвЂќР В Р’В° Р В Р’В±Р В РЎвЂР РЋРІР‚С™Р В Р’В° Р В РЎвЂўР В РЎвЂќР В РЎвЂўР В Р вЂ¦Р РЋРІР‚РЋР В Р’В°Р В Р вЂ¦Р В РЎвЂР РЋР РЏ Р В РЎвЂ”Р В Р’ВµР РЋР вЂљР В Р’ВµР В РўвЂР В Р’В°Р РЋРІР‚РЋР В РЎвЂ                                
    UserSetReceiveUart(huart);
  }
  else if ((huart->Instance->SR & USART_SR_IDLE) != RESET)              // Р В РЎвЂ”Р РЋР вЂљР В РЎвЂР В Р вЂ¦Р РЋР РЏР В Р’В»Р В РЎвЂ Р В РЎвЂ”Р В РЎвЂўР РЋР С“Р РЋРІР‚в„–Р В Р’В»Р В РЎвЂќР РЋРЎвЂњ
  {
    READ_REG(huart->Instance->DR); 
    huart->RxState = HAL_UART_STATE_READY; 
    // Проверяем на наличие ошибок на приеме
    if (huart->ErrorCode || huart->hdmarx->ErrorCode) {
      // Если есть ошибки - то надо перезапустить ДМА и USART
      HAL_DMA_Abort(huart->hdmarx); // Stop DMA
      UserSetReceiveUart(huart);  // Restart USART
    } else {
      length = szUartBff - huart->hdmarx->Instance->NDTR;                 // Р РЋР вЂљР В Р’В°Р В Р’В·Р В РЎВР В Р’ВµР РЋР вЂљ Р В РЎвЂ”Р В РЎвЂўР РЋР С“Р РЋРІР‚в„–Р В Р’В»Р В РЎвЂќР В РЎвЂ
      huart->RxXferSize = length;
      huart->hdmarx->Instance->CR &= ~DMA_CCR1_EN;                        // Р В РЎвЂєР РЋРІР‚С™Р В РЎвЂќР В Р’В»Р РЋР вЂ№Р РЋРІР‚РЋР В Р’В°Р В Р’ВµР В РЎВ Р В РЎвЂќР В Р’В°Р В Р вЂ¦Р В Р’В°Р В Р’В» Р РЋР вЂљР В Р’В°Р В Р’В±Р В РЎвЂўР РЋРІР‚С™Р РЋРІР‚в„– DMA Р В Р вЂ¦Р В Р’В° Р В РЎвЂ”Р РЋР вЂљР В РЎвЂР В Р’ВµР В РЎВ
      huart->hdmarx->Instance->NDTR = szUartBff;                          // Р В Р Р‹Р В Р’В±Р РЋР вЂљР В Р’В°Р РЋР С“Р РЋРІР‚в„–Р В Р вЂ Р В Р’В°Р В Р’ВµР В РЎВ Р В РўвЂР В Р’В»Р В РЎвЂР В Р вЂ¦Р В Р вЂ¦Р РЋРЎвЂњ Р В РЎвЂ”Р В РЎвЂўР В Р’В»Р РЋРІР‚в„–Р РЋР С“Р В РЎвЂќР В РЎвЂ Р В РўвЂР В Р’В»Р РЋР РЏ Р В РЎвЂ”Р РЋР вЂљР В РЎвЂР В Р’ВµР В РЎВР В Р’В° Р В РІР‚СњР В РЎС™Р В РЎвЂ™
      
      __HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);                         // Р В РЎвЂўР РЋРІР‚С™Р В РЎвЂќР В Р’В»Р РЋР вЂ№Р РЋРІР‚РЋР В Р’ВµР В Р вЂ¦Р В РЎвЂР РЋР РЏ Р В РЎвЂ”Р РЋР вЂљР В Р’ВµР РЋР вЂљР РЋРІР‚в„–Р В Р вЂ Р В Р’В°Р В Р вЂ¦Р В РЎвЂР РЋР РЏ IDLE ( Р В Р вЂ¦Р В Р’В° Р В РЎвЂ”Р РЋР вЂљР В РЎвЂР В Р’ВµР В РЎВ Р В РўвЂР В Р’В°Р В Р вЂ¦Р В Р вЂ¦Р РЋРІР‚в„–Р РЋРІР‚В¦)
      UserQueueSendToBackFromISR(huart, 0);
      __HAL_UART_ENABLE_IT(huart, UART_IT_TC);                            // Р В Р вЂ Р В РЎвЂќР В Р’В»Р РЋР вЂ№Р РЋРІР‚РЋР В Р’В°Р В Р’ВµР В РЎВ Р В РЎвЂ”Р РЋР вЂљР В Р’ВµР РЋР вЂљР РЋРІР‚в„–Р В Р вЂ Р В Р’В°Р В Р’ВµР В Р вЂ¦Р В РЎвЂР В Р’Вµ Р В Р вЂ¦Р В Р’В° Р В Р’В·Р В Р’В°Р В Р вЂ Р В Р’ВµР РЋР вЂљР РЋРІвЂљВ¬Р В Р’ВµР В Р вЂ¦Р В РЎвЂР РЋР РЏ Р В РЎвЂ”Р РЋР вЂљР В РЎвЂР В Р’ВµР В РЎВР В Р’В°       
    }
  }
  else if((huart->Instance->SR & USART_SR_FE) != RESET)                 // Р С›РЎв‚¬Р С‘Р В±Р С”Р В° Р С”Р В°Р Т‘РЎР‚Р В°
  {
    READ_REG(huart->Instance->DR);                                      // РЎРѓР В±РЎР‚Р С•РЎРѓ Р С—РЎР‚Р ВµРЎР‚РЎвЂ№Р Р†Р В°Р Р…Р С‘РЎРЏ USART_SR_FE
  }
}

void USER_UART_Receive_DMA (UART_HandleTypeDef *huart)
{
    uint8_t *bff;
    if(huart == &huart1)
      bff = UartBuf1;
    else if(huart == &huart3)
      bff = UartBuf3;
    else if(huart == &huart6)
      bff = UartBuf6;
    else if(huart == &EXT2_PortStructure)
      bff = UartRxBufExt2;
    else if(huart == &EXT1_PortStructure)
      bff = UartRxBufExt1;    
    
    HAL_UART_Receive_DMA(huart, bff, szUartBff);
}

void SetPreferences (UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
      uart1.lengthBff   = &huart->RxXferSize;
      uart1.bff         = UartBuf1;
      uart1.needModbusSlave = 1;      
    }
    else if(huart == &huart3)
    {
      uart3.lengthBff   = &huart->RxXferSize;
      uart3.bff         = UartBuf3;
      uart3.needModbusSlave = 1;      
    }
    else if(huart == &huart6)
    {
      uart6.lengthBff   = &huart->RxXferSize;
      uart6.bff         = UartBuf6;
      uart6.needModbusSlave = 1;      
    }
    else if(huart == &EXT2_PortStructure)
    {
      uartExt2.lengthBff   = &huart->RxXferSize;
      uartExt2.bff         = UartRxBufExt2;
      uartExt2.netAdr      = uart1.netAdr; // Адрес берем из настроек RS-485
      uartExt2.needModbusSlave = 0; // Первоначально запрос в MOdbusSlave не требуется
    }
    else if(huart == &EXT1_PortStructure)
    {
      uartExt1.lengthBff   = &huart->RxXferSize;
      uartExt1.bff         = UartRxBufExt1;
      uartExt1.netAdr      = uart1.netAdr; // Адрес берем из настроек RS-485
      uartExt1.needModbusSlave = 0; // Первоначально запрос в MOdbusSlave не требуется
    }
}

void UserSetReceiveUart(UART_HandleTypeDef *huart)
{
    huart->gState = HAL_UART_STATE_READY;                                       // Р РЋР С“Р В РЎвЂўР РЋР С“Р РЋРІР‚С™Р В РЎвЂўР РЋР РЏР В Р вЂ¦Р В РЎвЂР В Р’Вµ UART - Р В РЎвЂ“Р В РЎвЂўР РЋРІР‚С™Р В РЎвЂўР В Р вЂ 
    huart->RxState = HAL_UART_STATE_READY;
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE); 
    __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
    huart->hdmarx->Instance->CR |= DMA_CCR1_EN;                                 // Р В РІР‚в„ўР В РЎвЂќР В Р’В»Р РЋР вЂ№Р РЋРІР‚РЋР В Р’В°Р В Р’ВµР В РЎВ Р В РЎвЂќР В Р’В°Р В Р вЂ¦Р В Р’В°Р В Р’В» Р РЋР вЂљР В Р’В°Р В Р’В±Р В РЎвЂўР РЋРІР‚С™Р РЋРІР‚в„– DMA Р В Р вЂ¦Р В Р’В° Р В РЎвЂ”Р РЋР вЂљР В РЎвЂР В Р’ВµР В РЎВ
    USER_UART_Receive_DMA(huart);
    if(huart == &huart1) {                                                        // Р В Р’ВµР РЋР С“Р В Р’В»Р В РЎвЂ Р РЋР вЂљР В Р’В°Р В Р’В±Р В РЎвЂўР РЋРІР‚С™Р В Р’В°Р В Р’ВµР В РЎВ Р РЋР С“ RS-485
        HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, RS485_RX_ENABLE);
    } else if(huart == &huart6) {                                                       // Р В Р’ВµР РЋР С“Р В Р’В»Р В РЎвЂ Р РЋР вЂљР В Р’В°Р В Р’В±Р В РЎвЂўР РЋРІР‚С™Р В Р’В°Р В Р’ВµР В РЎВ Р РЋР С“ RS-485
        HAL_GPIO_WritePin(PULT_DIR_GPIO_Port, PULT_DIR_Pin, PULT_RX_ENABLE);
    } 
}

void uart_init(uart_type *uartLocl, u16 netAdr, u16 baudrate, u16 FirstCall)
{
  UART_HandleTypeDef *huart;

  uartLocl->baudrate          = uartBRate[baudrate];
  uartLocl->netAdr            = netAdr;
  uartLocl->wdt               = 0;                              // Р РЋР В±РЎР‚Р С•РЎРѓР С‘РЎвЂљРЎРЉ РЎРѓРЎвЂљР С•РЎР‚Р С•Р В¶Р ВµР Р†Р С•Р в„– РЎвЂљР В°Р в„–Р СР ВµРЎР‚
  uartLocl->WdtLink           = 10000;                          // Р РЋР В±РЎР‚Р С•РЎРѓР С‘РЎвЂљРЎРЉ РЎРѓРЎвЂљР С•РЎР‚Р С•Р В¶Р ВµР Р†Р С•Р в„– РЎвЂљР В°Р в„–Р СР ВµРЎР‚ Р С—РЎР‚Р С•Р Р†Р ВµРЎР‚Р С”Р С‘ Р Р…Р В°Р В»Р С‘РЎвЂЎР С‘РЎРЏ РЎРѓР Р†РЎРЏР В·Р С‘
  uartLocl->bdrIndex          = baudrate;                       // Код скорости
  
  if(!FirstCall)                                                // Р ВµРЎРѓР В»Р С‘ Р С‘Р В·Р СР ВµР Р…Р С‘Р В»Р С‘ РЎРѓР С”Р С•РЎР‚Р С•РЎРѓРЎвЂљРЎРЉ Р Р†Р С• Р Р†РЎР‚Р ВµР СРЎРЏ РЎР‚Р В°Р В±Р С•РЎвЂљРЎвЂ№
  {
    if(uartLocl == &uart1)
      huart = &huart1;
    else if(uartLocl == &uart3)
      huart = &huart3;
    else if(uartLocl == &uart6)
      huart = &huart6;
    else if(uartLocl == &uartExt2) // !*!*!*!
      huart = &EXT2_PortStructure;
    else if(uartLocl == &uartExt1) // !*!*!*!
      huart = &EXT1_PortStructure;    
     
    huart->Init.BaudRate      = uartBRate[baudrate];
    HAL_UART_Init(huart);                                       // Р Т‘Р В»РЎРЏ Р С‘Р В·Р СР ВµР Р…Р ВµР Р…Р С‘РЎРЏ РЎРѓР С”Р С•РЎР‚Р С•РЎРѓРЎвЂљР С‘ Р С•Р В±Р СР ВµР Р…Р В° ( Р С—Р ВµРЎР‚Р ВµР С‘Р Р…Р С‘РЎвЂ Р С‘Р В°Р В»Р С‘Р В·Р В°РЎвЂ Р С‘РЎРЏ UART)
  }
}

// Р СњР В°РЎРѓРЎвЂљРЎР‚Р С•Р в„–Р С”Р В° UART Р Т‘Р В»РЎРЏ Р С•Р В±Р СР ВµР Р…Р В° РЎРѓ Р СР С•Р Т‘РЎС“Р В»Р ВµР С WiFi
// Р СџРЎР‚Р С•Р Р†Р ВµРЎР‚РЎРЏР ВµР С Р Р…Р В°Р В»Р С‘РЎвЂЎР С‘Р Вµ Р Т‘Р В¶Р В°Р СР С—Р ВµРЎР‚Р В° Р Р…Р В° Р С—Р В»Р В°РЎвЂљР Вµ Р С”Р С•РЎвЂљР С•РЎР‚РЎвЂ№Р в„– Р С—Р ВµРЎР‚Р ВµР Р†Р С•Р Т‘Р С‘РЎвЂљ Р СР С•Р Т‘РЎС“Р В»РЎРЉ Р Р† РЎР‚Р ВµР В¶Р С‘Р С Р С—РЎР‚Р С•Р С–РЎР‚Р В°Р СР С‘РЎР‚Р С•Р Р†Р В°Р Р…Р С‘РЎРЏ
// Р вЂўРЎРѓР В»Р С‘ Р Т‘Р В¶Р В°Р СР С—Р ВµРЎР‚ РЎС“РЎРѓРЎвЂљР В°Р Р…Р С•Р Р†Р В»Р ВµР Р… РЎвЂљР С• Р С—Р ВµРЎР‚Р ВµР Р†Р С•Р Т‘Р С‘Р С Р Р…Р С•Р С–Р С‘ UART Р Р…Р В° Р Р†РЎвЂ¦Р С•Р Т‘ Р С‘ Р С•РЎвЂљР С”Р В»РЎР‹РЎвЂЎР В°Р ВµР С Р СР С•Р Т‘РЎС“Р В»РЎРЉ UART
u16 DinControlWiFi(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  static u8 stateWiFi = none;
  
  if(((WIFI_PROG_ON_Port->IDR & WIFI_PROG_ON_Pin) != GPIO_PIN_RESET) && (stateWiFi != WiFi_NormalWork)){        // Р РЃРЎвЂљР В°РЎвЂљР Р…РЎвЂ№Р в„– РЎР‚Р ВµР В¶Р С‘Р С (Р С•Р В±Р СР ВµР Р… РЎРѓ Р СР С•Р Т‘РЎС“Р В»Р ВµР С WiFi)
    stateWiFi = WiFi_NormalWork;
    
    GPIO_InitStruct.Pin = WIFI_TX_MK_Pin|WIFI_RX_MK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    EnableUartWiFi();
  }
  else if(((WIFI_PROG_ON_Port->IDR & WIFI_PROG_ON_Pin) == GPIO_PIN_RESET) && (stateWiFi != WiFi_ModeProg)){   // Р В Р ВµР В¶Р С‘Р С Р С—РЎР‚Р С•Р С–РЎР‚Р В°Р СР С‘РЎР‚Р С•Р Р†Р В°Р Р…Р С‘РЎРЏ Р СР С•Р Т‘РЎС“Р В»РЎРЏ WiFi 
    stateWiFi = WiFi_ModeProg;
    
    GPIO_InitStruct.Pin = WIFI_TX_MK_Pin|WIFI_RX_MK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    DisableUartWiFi();
  }
  return stateWiFi;
}

//Настройка порта EXT2 для обмена с блоками расширения
//Настройка DMA для приема
#define EXT2_RX_DMA_CR    (DMA_CHANNEL_4 + DMA_PERIPH_TO_MEMORY + DMA_PINC_DISABLE + DMA_MINC_ENABLE + DMA_PDATAALIGN_BYTE + DMA_MDATAALIGN_BYTE + DMA_NORMAL + DMA_PRIORITY_LOW + DMA_FIFOMODE_DISABLE)   
void USART2_Init(){
  
  MX_USART2_UART_Init();  
  USER_INIT_UART(&EXT2_PortStructure); // -> USER_UART_Receive_DMA -> HAL_UART_Receive_DMA

  // Сразу подключаем буферы для более удобного использования
  EXT2_PortStructure.pTxBuffPtr = &UartTxBufExt2[0];
  EXT2_PortStructure.pRxBuffPtr = &UartRxBufExt2[0];
  
  //Настройка прерываний
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);      
}

//Настройка порта EXT1 для обмена с блоками расширения
//Настройка DMA для приема
#define EXT1_RX_DMA_CR    (DMA_CHANNEL_4 + DMA_PERIPH_TO_MEMORY + DMA_PINC_DISABLE + DMA_MINC_ENABLE + DMA_PDATAALIGN_BYTE + DMA_MDATAALIGN_BYTE + DMA_NORMAL + DMA_PRIORITY_LOW + DMA_FIFOMODE_DISABLE)   
void UART5_Init(){
  
  MX_UART5_UART_Init();  
  USER_INIT_UART(&EXT1_PortStructure); // -> USER_UART_Receive_DMA -> HAL_UART_Receive_DMA

  // Сразу подключаем буферы для более удобного использования
  EXT1_PortStructure.pTxBuffPtr = &UartTxBufExt1[0];
  EXT1_PortStructure.pRxBuffPtr = &UartRxBufExt1[0];
  
  //Настройка прерываний
   HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 8, 0);
   HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
   
   HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 8, 0);
   HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);    
}

void sendExtBlocksProcessingData(void){
  DMA_HandleTypeDef *hdmaRx;  
  DMA_TypeDef *regs;
  extBlockModeType extType;
  identDataType *idDataPnt;
  uint16_t dataLength;
  uint32_t TxDataLength;
  static u16 TxMissCnt1 = 0;
  static u16 TxSkipCnt1 = 0;
  static u16 TxMissCnt2 = 0;
  static u16 TxSkipCnt2 = 0;
  static u16 ReInitUart = 0;
  u16 extDelayTimeTick;
  u16 extDataSendSkip1, extDataSendSkip2; //число пропусков отправки данных в блок расширения

  extDelayTimeTick = (u16)(SEND_DATA_DELAY_TYME * GlobalM4.Fpwm + 0.5F); //задержка перед отправкой данных при подаче питания
  // Ввел две переменные на таймаут опросов, чтобы можно было варьировать ими для каждого канала
  extDataSendSkip1 = extDataSendSkip2 = (u16)(EXT_PROCESSING_PERIOD * GlobalM4.Fpwm + 0.5F) - 1; //Прореживание отправки данных

  // Делаем кастомную настройку на пингование отедльных блоков
  extType = GlobalM4.extConnectedBlocks.ext1.extType;
  if (extType == RS485_EXT_BLOCK) {
    extDataSendSkip1 =  (u16)(COMM_BLOCK_PROCESSING_PERIOD * GlobalM4.Fpwm + 0.5F) - 1; //Прореживание отправки данных
  } else if (extType == INCREMENT_EXT_BLOCK || extType == ENDAT_EXT_BLOCK || extType == SIN_COS_EXT_BLOCK) {
    extDataSendSkip1 =  (u16)((GlobalM4.EncoderParam.processingPeriod*0.000001) * GlobalM4.Fpwm + 0.5F) - 1; //Прореживание отправки данных
  }

  extType = GlobalM4.extConnectedBlocks.ext2.extType;
  if (extType == RS485_EXT_BLOCK) {
    extDataSendSkip2 =  (u16)(COMM_BLOCK_PROCESSING_PERIOD * GlobalM4.Fpwm + 0.5F) - 1; //Прореживание отправки данных
  } else if (extType == INCREMENT_EXT_BLOCK || extType == ENDAT_EXT_BLOCK || extType == SIN_COS_EXT_BLOCK) {
    extDataSendSkip2 =  (u16)((GlobalM4.EncoderParam.processingPeriod*0.000001) * GlobalM4.Fpwm + 0.5F) - 1; //Прореживание отправки данных
  }
    
  // --- НЕТ - Если идет работа с файлами по Модбасу, то я не вызываю блоки расширения. Но таймер таймаута должен тикать  
  // Вариант 2 - Если есть запрос от Флэшки и при этом это блок СР24 v2.1 - то будет блокирование
  // Иначе если это блок v2.2 и выше - блокировать работу не требуется, там ДМА разнесены !!!  
  if (!GlobalM4.BLOCK_DMA.bits.FlashDMA_Req || GlobalM4.versionCP24 != CP24_v21 /*&& !MODBUS_FILE_RDWR*/) {
    
    // После того как FLASH забирала DMA надо переинициализировать под UART
    if (ReInitUart) {     
      // UART5
      HAL_DMA_DeInit(hspi3.hdmarx);
      HAL_DMA_Init(EXT1_PortStructure.hdmarx);
      HAL_DMA_Init(EXT2_PortStructure.hdmarx);
      //HAL_NVIC_EnableIRQ(UART5_IRQn);
     /* hdmaRx = EXT1_PortStructure.hdmarx;      
      // Тормозим ДМА и восстанавливаем настройку UARTa
      hdmaRx->State = HAL_DMA_STATE_BUSY;
      
      HAL_DMA_Abort (hdmaRx);
      
      USER_INIT_UART(&EXT1_PortStructure); // -> USER_UART_Receive_DMA -> HAL_UART_Receive_DMA      // А теперь делаем конфигурирование CR
      
      // Еще раз тормозим ДМА для перенастройки на нужный канал работы
      __HAL_DMA_DISABLE (hdmaRx); // hdmaRx->Instance->CR &=  ~(1 << DMA_SxCR_EN)
      while((hdmaRx->Instance->CR & DMA_SxCR_EN) == SET) { } // Ждем реального останова ДМА
      // Восстановить настройку ДМА
      hdmaRx->Instance->CR = EXT1_RX_DMA_CR;                                       // Базовая настройка RxDMA 
      // Задаем прерывания
      //hdmaRx->Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;                       // включить прерывание по ДМА      
      // Сбросить все фдаги прерываний до старта DMA
      regs = (DMA_TypeDef *)hdmaRx->StreamBaseAddress;  
      regs->LIFCR = (DMA_FLAG_FEIF0_4 | DMA_FLAG_DMEIF0_4 | DMA_FLAG_TEIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TCIF0_4) << hdmaRx->StreamIndex;           
      __HAL_DMA_ENABLE (hdmaRx); //   hdmaRx->Instance->CR |=  DMA_SxCR_EN; // Запускаем ДМА
      
      // USART2
      hdmaRx = EXT2_PortStructure.hdmarx;      
      // Тормозим ДМА и восстанавливаем настройку UARTa
      //hdmaRx->State = HAL_DMA_STATE_BUSY;
      
      HAL_DMA_Abort (hdmaRx);
      USER_INIT_UART(&EXT2_PortStructure); // -> USER_UART_Receive_DMA -> HAL_UART_Receive_DMA      // А теперь делаем конфигурирование CR
      // Еще раз тормозим ДМА для перенастройки на нужный канал работы
      __HAL_DMA_DISABLE (hdmaRx); // hdmaRx->Instance->CR &=  ~(1 << DMA_SxCR_EN)
      while((hdmaRx->Instance->CR & DMA_SxCR_EN) == SET) { } // Ждем реального останова ДМА
      // Восстановить настройку ДМА
      hdmaRx->Instance->CR = EXT2_RX_DMA_CR;                                       // Базовая настройка RxDMA 
      // Задаем прерывания  
      hdmaRx->Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;                       // включить прерывание по ДМА      
      // Сбросить все фдаги прерываний до старта DMA
      regs = (DMA_TypeDef *)hdmaRx->StreamBaseAddress;  
      regs->LIFCR = (DMA_FLAG_FEIF0_4 | DMA_FLAG_DMEIF0_4 | DMA_FLAG_TEIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TCIF0_4) << hdmaRx->StreamIndex;           
      __HAL_DMA_ENABLE (hdmaRx); //   hdmaRx->Instance->CR |=  DMA_SxCR_EN; // Запускаем ДМА
      */
      ReInitUart = 0;
      
//      if ((EXT1_PortStructure.hdmarx->Instance->CR & EXT1_RX_DMA_CR) != EXT1_RX_DMA_CR)
//        ReInitUart = 1;
//
//      if ((EXT2_PortStructure.hdmarx->Instance->CR & EXT2_RX_DMA_CR) != EXT2_RX_DMA_CR)
//        ReInitUart = 1;
      
    }
      
     switch(ext1ExchangeState){
    case EXT_DATA_SEND_DELAY: //Пауза перед началом обмена с блоком расширения
      extAnswerWaitTimers(EXT_TIM_SET, EXT_TIM_NOP); // перезапуск счетчика, чтобы не перейти в повторную инициализацию      
      if(TxMissCnt1 < extDelayTimeTick){
        TxMissCnt1++;
      }else{
        ext1ExchangeState = EXT_BLOCK_IDENT; //Переход в состояние отправок с ожиданием ответов
      }
      break;
    case EXT_BLOCK_IDENT:    //Состояние идентификации блока расширения                         //Тип запроса
      uartExt1.needModbusSlave = 0; // Blocking request to Modbus task          
      // Если DMA уже занят - счетчик не перезапускаем
      if (GlobalM4.BLOCK_DMA.bits.EXT1_DMA == 0) { 
        extAnswerWaitTimers(EXT_TIM_SET, EXT_TIM_NOP); // перезапуск счетчика, чтобы не перейти в повторную инициализацию      
      }
      if(TxSkipCnt1 < extDataSendSkip1){        //Прореживание отправки данных в блок расширения
        TxSkipCnt1++;
      }else{
        TxSkipCnt1 = 0;
        if(EXT1_PortStructure.gState == HAL_UART_STATE_READY){
          idDataPnt = (identDataType *)(EXT1_PortStructure.pTxBuffPtr);               //Указатель на буфер
          idDataPnt->header.word = 0;
          idDataPnt->reserv = 0;
          idDataPnt->header.bits.telegramType = IDENT_TELEGRAM;        //Задаем тип телеграммы
          dataLength = sizeof(identDataType) - sizeof(idDataPnt->crc); //область для расчета CRC
          idDataPnt->crc = Crc((unsigned char *)(idDataPnt), dataLength);
          TxDataLength = sizeof(identDataType);    //Длина данных передачи
          // Проверка доступности ресурсов DMA которые мы дели м с флэшкой
          if (GlobalM4.BLOCK_DMA.bits.EXT1_DMA == 0) {
            GlobalM4.BLOCK_DMA.bits.EXT1_DMA = 1;          
            // Иначе можем делать передачу
            HAL_UART_Transmit_DMA(&EXT1_PortStructure, EXT1_PortStructure.pTxBuffPtr, TxDataLength); //Обмен с устройством на EXT1
          }
        }        
      }
      break;
    case EXT_BLOCK_EXCHANGE: //Состояние обмена с блоком расширения 1    
      if(TxSkipCnt1 < extDataSendSkip1){ //Прореживание отправки данных в блок расширения
        TxSkipCnt1++;
      }else{
        TxSkipCnt1 = 0;
        if(EXT1_PortStructure.gState == HAL_UART_STATE_READY){
          TxDataLength =  extBlockDataBufferFill(GlobalM4.extConnectedBlocks.ext1.extType, EXT1_PortStructure.pTxBuffPtr);
          // Проверка доступности ресурсов DMA которые мы дели м с флэшкой
          if (GlobalM4.BLOCK_DMA.bits.EXT1_DMA == 0) {
            GlobalM4.BLOCK_DMA.bits.EXT1_DMA = 1;
            // Иначе можем делать передачу
            HAL_UART_Transmit_DMA(&EXT1_PortStructure, EXT1_PortStructure.pTxBuffPtr, TxDataLength);
          }
          
          // Проверяем, что за тип блока подключен к этому каналу - если блок связи, то переводим работу в Slave UART
          extType = GlobalM4.extConnectedBlocks.ext1.extType;
          if (/*extType == RS485_EXT_BLOCK || */ extType == CAN_EXT_BLOCK || extType == PROFIBUS_EXT_BLOCK ||
                          extType == ETHER_EXT_BLOCK || extType == PROFINET_EXT_BLOCK) {
            if (GlobalM4.extConnectedBlocks.ext1.softVersion !=0) {
                ext1ExchangeState =  EXT_WAIT_SOFT_INFO;           
            }
          }          
        }
      }
      break;
      
    case   EXT_WAIT_SOFT_INFO:
      if (GlobalM4.extConnectedBlocks.ext1.softVersion !=0)
        ext1ExchangeState =  EXT_SLAVE_UART;      
      break;      
      
    case EXT_BLOCK_SKIP: // Режим пропуска обмена с блоком расширения, если блока нету
    case EXT_SLAVE_UART: // Если режим Slave UART то пропускаем штатную работу.      
      uartExt1.needModbusSlave = 1; // Если работаем как Slave, разрешаем запросы в задачу Modbus      
      if (uartExt1.status == SendBackPingStatus && EXT1_PortStructure.gState == HAL_UART_STATE_READY) {
        // Делаем обратный пинг в блок
        TxDataLength =  extBlockDataBufferFill(GlobalM4.extConnectedBlocks.ext1.extType, EXT1_PortStructure.pTxBuffPtr);
        // Проверка доступности ресурсов DMA которые мы дели м с флэшкой
        if (GlobalM4.BLOCK_DMA.bits.EXT1_DMA == 0) {
          GlobalM4.BLOCK_DMA.bits.EXT1_DMA = 1;
          // Иначе можем делать передачу
          HAL_UART_Transmit_DMA(&EXT1_PortStructure, EXT1_PortStructure.pTxBuffPtr, TxDataLength);
          uartExt1.status = IdleStatus;          
        }               
      }
      break;
    }
    
    switch(ext2ExchangeState){
    case EXT_DATA_SEND_DELAY: //Пауза перед началом обмена с блоком расширения
      extAnswerWaitTimers(EXT_TIM_NOP, EXT_TIM_SET); // перезапуск счетчика, чтобы не перейти в повторную инициализацию      
      if(TxMissCnt2 < extDelayTimeTick){
        TxMissCnt2++;
      }else{
        ext2ExchangeState = EXT_BLOCK_IDENT; //Переход в состояние отправок с ожиданием ответов
      }
      break;
    case EXT_BLOCK_IDENT:    //Состояние идентификации блока расширения                         //Тип запроса
      uartExt2.needModbusSlave = 0; // Blocking request to Modbus task    
      extAnswerWaitTimers(EXT_TIM_NOP, EXT_TIM_SET); // перезапуск счетчика, чтобы не перейти в повторную инициализацию          
      if(TxSkipCnt2 < extDataSendSkip2){        //Прореживание отправки данных в блок расширения
        TxSkipCnt2++;     
      }else{
        TxSkipCnt2 = 0;
        if(EXT2_PortStructure.gState == HAL_UART_STATE_READY){
          idDataPnt = (identDataType *)(EXT2_PortStructure.pTxBuffPtr);                //Указатель на буфер
          idDataPnt->header.word = 0;
          idDataPnt->reserv = 0;
          idDataPnt->header.bits.telegramType = IDENT_TELEGRAM;        //Задаем тип телеграммы
          dataLength = sizeof(identDataType) - sizeof(idDataPnt->crc); //область для расчета CRC
          idDataPnt->crc = Crc((unsigned char *)(idDataPnt), dataLength);
          TxDataLength = sizeof(DoutStateRxType);    //Длина данных передачи
          
          // Проверка доступности ресурсов DMA которые мы дели м с флэшкой
          if (GlobalM4.BLOCK_DMA.bits.EXT2_DMA == 0) {
            GlobalM4.BLOCK_DMA.bits.EXT2_DMA = 1;          
            // Иначе можем делать передачу
            HAL_UART_Transmit_DMA(&EXT2_PortStructure, EXT2_PortStructure.pTxBuffPtr, TxDataLength); //Обмен с устройством на EXT2
          }        
        }
      }
      break;
    case EXT_BLOCK_EXCHANGE: //Состояние обмена с блоком расширения  
      if(TxSkipCnt2 < extDataSendSkip2){ //Прореживание отправки данных в блок расширения
        TxSkipCnt2++;
      }else{
        TxSkipCnt2 = 0;
        if(EXT2_PortStructure.gState == HAL_UART_STATE_READY){
          TxDataLength =  extBlockDataBufferFill(GlobalM4.extConnectedBlocks.ext2.extType, EXT2_PortStructure.pTxBuffPtr);
          // Проверка доступности ресурсов DMA которые мы дели м с флэшкой
          if (GlobalM4.BLOCK_DMA.bits.EXT2_DMA == 0) {
            GlobalM4.BLOCK_DMA.bits.EXT2_DMA = 1;
            // Иначе можем делать передачу
            HAL_UART_Transmit_DMA(&EXT2_PortStructure, EXT2_PortStructure.pTxBuffPtr, TxDataLength);
          }
          
          // Проверяем, что за тип блока подключен к этому каналу - если блок связи, то переводим работу в Slave UART
          extType = GlobalM4.extConnectedBlocks.ext2.extType;
          if (/*extType == RS485_EXT_BLOCK || */extType == CAN_EXT_BLOCK || extType == PROFIBUS_EXT_BLOCK ||
                          extType == ETHER_EXT_BLOCK || extType == PROFINET_EXT_BLOCK) {
            if (GlobalM4.extConnectedBlocks.ext2.softVersion !=0) {
                ext2ExchangeState =  EXT_WAIT_SOFT_INFO;           
            }
          }          
        }        
        
      }
      break;
      
    case   EXT_WAIT_SOFT_INFO:
      if (GlobalM4.extConnectedBlocks.ext2.softVersion !=0)
        ext2ExchangeState =  EXT_SLAVE_UART;      
      break;
      
    case EXT_BLOCK_SKIP: // Режим пропуска обмена с блоком расширения, если блока нету      
    case EXT_SLAVE_UART: // Если режим Slave UART то пропускаем штатную работу.      
       uartExt2.needModbusSlave = 1; // Если работаем как Slave, разрешаем запросы в задачу Modbus            
      if (uartExt2.status == SendBackPingStatus && EXT2_PortStructure.gState == HAL_UART_STATE_READY) {
        // Делаем обратный пинг в блок
        TxDataLength =  extBlockDataBufferFill(GlobalM4.extConnectedBlocks.ext2.extType, EXT2_PortStructure.pTxBuffPtr);
        // Проверка доступности ресурсов DMA которые мы дели м с флэшкой
        if (GlobalM4.BLOCK_DMA.bits.EXT2_DMA == 0) {
          GlobalM4.BLOCK_DMA.bits.EXT2_DMA = 1;
          // Иначе можем делать передачу
          HAL_UART_Transmit_DMA(&EXT2_PortStructure, EXT2_PortStructure.pTxBuffPtr, TxDataLength);
          uartExt2.status = IdleStatus;          
        }               
      }       
      break;      
    }
  } else {
    ReInitUart = 1; // По окончании работы FLASH перенастроить DMA
  }
  
  extAnswerWaitTimers(EXT_TIM_CNT, EXT_TIM_CNT);
}

//Заполнение буфера для отправки данных в блок расширения
//extBlockType тип блока расширения, в который отправляются данные
//extDataPnt - указатель на заполняемый буфер
// retvalue - длина данных в буфере
u16 extBlockDataBufferFill(extBlockModeType extBlockType, uint8_t *extDataPnt){
  u16 dataLength;
  u16 TxDataLength;
  encoDataType  *encoDataPnt;
  identDataType *idDataPnt;
  DoutStateRxType *DoutStatePnt;
  AinAoutStateTxType *AinAoutTxPnt;
  ExtRs2ExchangeType *Rs2TxPnt;
  
  switch(extBlockType){
  case INCREMENT_EXT_BLOCK: 
  case ENDAT_EXT_BLOCK: 
  case SIN_COS_EXT_BLOCK:
    encoDataPnt = (encoDataType *)extDataPnt;
    encoDataPnt->header.bits.telegramType = EXCHANGE_TELEGRAM;
    encoDataPnt->header.bits.serialMode = GlobalM4.EncoderParam.serialMode; 
    encoDataPnt->header.bits.fastSpdSign = GlobalM4.EncoderParam.fastSpeedPhasing;
    encoDataPnt->header.bits.PWM_On = GlobalM4.FlagCommand.bit.PwmOn;
    encoDataPnt->header.bits.drvType = (((GlobalM4.DriveType == 0) || (GlobalM4.DriveType == 1)) ? 0 : 1);
    encoDataPnt->header.bits.autoPhasing = GlobalM4.EncoderParam.autoPhasingFlg;
    encoDataPnt->header.bits.fastSpdUse = GlobalM4.EncoderParam.fastSpdUse;
    encoDataPnt->header.bits.encoResolMode = GlobalM4.EncoderParam.encoResolMode;
    encoDataPnt->header.bits.spdPhasingSign = GlobalM4.EncoderParam.speedPhasing;
    encoDataPnt->bitResolution = GlobalM4.EncoderParam.encoBitResol;
    encoDataPnt->pulseResolution = GlobalM4.EncoderParam.encoPulseResol;
    encoDataPnt->encoAngleShift = GlobalM4.EncoderParam.encoAngleShift;
    encoDataPnt->motorPolePairsNum = GlobalM4.EnginParam.Zp;
    encoDataPnt->processingPeriod = GlobalM4.EncoderParam.processingPeriod;
    encoDataPnt->angleFltBufNum = GlobalM4.EncoderParam.EncoFlt_N;
    encoDataPnt->spdFltTime = GlobalM4.EncoderParam.spdFltTime;
    encoDataPnt->ADC_Amplitude = GlobalM4.EncoderParam.ADC_Amplitude;
    encoDataPnt->header.bits.encoEmulMode = GlobalM4.EncoderParam.encoEmulMode;
    encoDataPnt->encoEmulResol = GlobalM4.EncoderParam.encoEmulResol;
    dataLength = sizeof(encoDataType) - sizeof(encoDataPnt->crc); //длина пакета для расчета
    encoDataPnt->crc = Crc((unsigned char *)(encoDataPnt), dataLength);
    TxDataLength = sizeof(encoDataType);
    break;
  case DIN_DOUT_EXT_BLOCK:
    DoutStatePnt = (DoutStateRxType *)extDataPnt;
    DoutStatePnt->header.word = 0;
    DoutStatePnt->header.bits.telegramType = EXCHANGE_TELEGRAM;
    DoutStatePnt->DoutState.bit.Dout1 = GlobalM4.Dout4;     //Состояние релейного выхода 1
    DoutStatePnt->DoutState.bit.Dout2 = GlobalM4.Dout5;     //Состояние релейного выхода 2
    DoutStatePnt->crc = Crc((unsigned char *)extDataPnt, sizeof(DoutStateRxType) - sizeof(DoutStatePnt->crc));
    TxDataLength = sizeof(DoutStateRxType);    //Длина данных передачи
    break;
  case AIN_AOUT_EXT_BLOCK:
    AinAoutTxPnt = (AinAoutStateTxType *)extDataPnt;
    AinAoutTxPnt->header.word = 0;
    AinAoutTxPnt->header.bits.telegramType = EXCHANGE_TELEGRAM;
    AinAoutTxPnt->AinAout.Ain1Type = GlobalM4.AinAoutExtData.Ain1Type;     //Состояние релейного выхода 1
    AinAoutTxPnt->AinAout.Ain2Type = GlobalM4.AinAoutExtData.Ain2Type;     //Состояние релейного выхода 1
    AinAoutTxPnt->AinAout.Aout1ExtType = GlobalM4.AinAoutExtData.Aout1ExtType;     //Состояние релейного выхода 1
    AinAoutTxPnt->AinAout.Aout2ExtType = GlobalM4.AinAoutExtData.Aout2ExtType;     //Состояние релейного выхода 1
    AinAoutTxPnt->AinAout.Status = GlobalM4.AinAoutExtData.Status;     //Состояние релейного выхода 1
    AinAoutTxPnt->AinAout.TaskAout1 = GlobalM4.AinAoutExtData.TaskAout1;     //Состояние релейного выхода 1
    AinAoutTxPnt->AinAout.TaskAout2 = GlobalM4.AinAoutExtData.TaskAout2;     //Состояние релейного выхода 1
    AinAoutTxPnt->crc = Crc((unsigned char *)extDataPnt, sizeof(AinAoutStateTxType) - sizeof(AinAoutTxPnt->crc));
    TxDataLength = sizeof(AinAoutStateTxType);    //Длина данных передачи
    break;
  // Блоки связи по другим протоколам
  case RS485_EXT_BLOCK:
    Rs2TxPnt = (ExtRs2ExchangeType *)extDataPnt;
    Rs2TxPnt->header.word = 0;
    Rs2TxPnt->header.bits.telegramType = EXCHANGE_TELEGRAM;        
    Rs2TxPnt->Baudrate1 = uartVirtual1.bdrIndex;
    Rs2TxPnt->NetAddr1 =  uartVirtual1.netAdr;     
    Rs2TxPnt->Baudrate2 = uartVirtual2.bdrIndex;
    Rs2TxPnt->NetAddr2 =  uartVirtual2.netAdr;           
    Rs2TxPnt->crc = Crc((unsigned char *)extDataPnt, sizeof(ExtRs2ExchangeType) - sizeof(Rs2TxPnt->crc));      
    TxDataLength = sizeof(ExtRs2ExchangeType);    //Длина данных передачи  
    break;
  case CAN_EXT_BLOCK:  // -
  case PROFIBUS_EXT_BLOCK: // -
  case ETHER_EXT_BLOCK: // +
  case PROFINET_EXT_BLOCK: // -
    // Отправляем в блок коммуникации сигнал о том, что переходим в режим Slave UART'a
    idDataPnt = (identDataType *)extDataPnt;    
    idDataPnt->header.word = 0;
    idDataPnt->reserv = 0;
    idDataPnt->header.bits.telegramType = EXCHANGE_TELEGRAM;     //Задаем тип телеграммы
    dataLength = sizeof(identDataType) - sizeof(idDataPnt->crc); //область для расчета CRC
    idDataPnt->crc = Crc((unsigned char *)(idDataPnt), dataLength);
    TxDataLength = sizeof(identDataType);    //Длина данных передачи    
    break;
  }
  
  return(TxDataLength);
}

//Программные таймеры ожидания ответов от блоков расширения
//ext1TimCmd - управление таймером блока расширения EXT1
//ext2TimCmd - управление таймером блока расширения EXT2
void extAnswerWaitTimers(timsCmdType ext1TimCmd, timsCmdType ext2TimCmd)
{
  static u16 ext1Tim = 0; //Счетчик тактов ШИМ
  static u16 ext2Tim = 0; //Счетчик тактов ШИМ
  static u16 ext1AnsTimeout = 0; //Счетчик тактов ШИМ для таймаута приема
  static u16 ext2AnsTimeout = 0; //Счетчик тактов ШИМ для таймаута приема

  
  switch(ext1TimCmd){
  case EXT_TIM_SET: //Установка счетчика
    ext1Tim = (u16)(EXT_BLOCK_TIMEOUT_CNT * GlobalM4.Fpwm + 0.5F);  // Таймаут сброса на идентифкацию
    ext1AnsTimeout = (u16)(EXT_BLOCK_ANWER_TIMEOUT * GlobalM4.Fpwm + 0.5F); // Таймаут приема ответа
  case EXT_TIM_CNT: 
    if(ext1AnsTimeout)  { ext1AnsTimeout--; }
    if(ext1Tim)         { ext1Tim--;        }
    
    if(!ext1AnsTimeout){
        GlobalM4.BLOCK_DMA.bits.EXT1_DMA = 0; // Сбросить занятость DMA
    }
   
    if(!ext1Tim){
      ext1ExchangeState = EXT_BLOCK_IDENT; //возврат к идентификации блока
      GlobalM4.extConnectedBlocks.ext1.extType = NOT_DEF;
      GlobalM4.extConnectedBlocks.ext1.softVersion = 0;
      GlobalM4.extConnectedBlocks.ext1.softSubVersion = 0;
      GlobalM4.extConnectedBlocks.ext1.crc = 0;
    }
    
    break;
  case EXT_TIM_NOP:
    break;
  }
  
  switch(ext2TimCmd){
  case EXT_TIM_SET: //Установка счетчика
    ext2Tim = (u16)(EXT_BLOCK_TIMEOUT_CNT * GlobalM4.Fpwm + 0.5F);
    ext2AnsTimeout = (u16)(EXT_BLOCK_ANWER_TIMEOUT * GlobalM4.Fpwm + 0.5F); // Таймаут приема ответа    
    break;
  case EXT_TIM_CNT: //Отсчет до перехода в состояние идентификации блока   
    if(ext2AnsTimeout)  { ext2AnsTimeout--; }
    if(ext2Tim)         { ext2Tim--;        }
    
    if(!ext2AnsTimeout){
        GlobalM4.BLOCK_DMA.bits.EXT2_DMA = 0; // Сбросить занятость DMA
    }
   
    if(!ext2Tim){
      ext2ExchangeState = EXT_BLOCK_IDENT; //возврат к идентификации блока
      GlobalM4.extConnectedBlocks.ext2.extType = NOT_DEF;
      GlobalM4.extConnectedBlocks.ext2.softVersion = 0;
      GlobalM4.extConnectedBlocks.ext2.softSubVersion = 0;
      GlobalM4.extConnectedBlocks.ext2.crc = 0;
    }       
    break;
  case EXT_TIM_NOP:
    break;
  }
}

//Обработка завершения передачи данных в обработчик энкодера
//После отправки пакета в обраотчик энкодера запускаем считывание ответа
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &EXT1_PortStructure){   //Данные переданы
      
      if(ext1TxSkipCnt < ENCO_TX_SYNCH_TYME){ //После инициализации МК шлем запросы без ожидания ответа
        ext1TxSkipCnt++;
      }else{
        //Включаем таймер ожидания ответа от блока обработки энкодеров
        switch(ext1ExchangeState){ //Проверяем тип отправленного пакета
        case EXT_BLOCK_IDENT:
          break;
        case EXT_BLOCK_EXCHANGE: //Состояние обмена
          break;
        }
      }
  }else if(huart == &EXT2_PortStructure){
    if(ext2TxSkipCnt < ENCO_TX_SYNCH_TYME){ //После инициализации МК шлем запросы без ожидания ответа
      ext2TxSkipCnt++;
    }else{
      //Включаем таймер ожидания ответа от блока обработки энкодеров 
      switch(ext2ExchangeState){ //Проверяем тип отправленного пакета
      case EXT_BLOCK_IDENT:        
        break;
      case EXT_BLOCK_EXCHANGE:
        break;
      }
    }
  }
}

////
//u16 extBlockAnswDataSizeDef(extBlockModeType extBlockType){
//  u16 RxDataLength;
//  
//  switch(extBlockType){
//  case INCREMENT_EXT_BLOCK: 
//  case ENDAT_EXT_BLOCK: 
//  case SIN_COS_EXT_BLOCK:
//    RxDataLength = sizeof(RxCalcEncoDataType);
//    break;
//  case RS485_EXT_BLOCK:
//    break;
//  case  DIN_DOUT_EXT_BLOCK:
//    RxDataLength = sizeof(DinStateTxType);
//    break;
//  case AIN_AOUT_EXT_BLOCK:
//	RxDataLength = sizeof(AinAoutRx_type);
//    break;
//  case CAN_EXT_BLOCK:
//    break;
//  case PROFIBUS_EXT_BLOCK:
//    break;
//  case ETHER_EXT_BLOCK:
//    break;
//  case PROFINET_EXT_BLOCK:
//    break; 
//  }
//  return(RxDataLength);
//}

//Парсинг считанных с блока расширения данных
//extBlockType тип блока расширения от которого приняты данные
//extDataPnt - указатель на буфер с принятыми данными
void extBlockDataParsing(extNumType extNum, extBlockModeType extBlockType, uint8_t *extDataPnt)
{
  u16 calcCRC;
  RxCalcEncoDataType *encoExtBlockDataPnt;
  DinStateTxType *DinDoutExtBlockDataPnt;
  AinAoutRx_type *AinAoutExtBlockDataPnt;
  UART_HandleTypeDef *huart;
  EthernetSoftType *EthernetInfo;
  
  u16 dataLength, crc, softVersion, softSubVersion;
  u16 rxCrc;
  
  static u16 RxErr = 0;
  static u16 RxCnt = 0;
  
  switch(extBlockType){
  case INCREMENT_EXT_BLOCK: 
  case ENDAT_EXT_BLOCK: 
  case SIN_COS_EXT_BLOCK:
    encoExtBlockDataPnt = (RxCalcEncoDataType *)extDataPnt;
    dataLength = sizeof(RxCalcEncoDataType) - sizeof(encoExtBlockDataPnt->dataCRC); //Длина данных для расчета CRC
    rxCrc = encoExtBlockDataPnt->dataCRC;                    //Полученное CRC
    GlobalM4.EncoderCalcParam.dataCRC = rxCrc;               //Полученное CRC
    calcCRC = Crc((unsigned char *)(encoExtBlockDataPnt), dataLength);
    RxCnt++;
    if(calcCRC == rxCrc){
      GlobalM4.EncoderCalcParam.header = encoExtBlockDataPnt->header;
      GlobalM4.EncoderCalcParam.electricTheta = encoExtBlockDataPnt->electricTheta;
      GlobalM4.EncoderCalcParam.electricSpd = encoExtBlockDataPnt->electricSpd;//IQ_to_FLOAT(encoExtBlockDataPnt->electricSpd, Herz_base); // Переводим в Гц //encoExtBlockDataPnt->electricSpd * 80.0f;
      GlobalM4.EncoderCalcParam.softVersion = encoExtBlockDataPnt->softVersion;
      GlobalM4.EncoderCalcParam.softCrc = encoExtBlockDataPnt->softCrc;
      
      //Заполнение информации о подключенном блоке расширения
      crc = encoExtBlockDataPnt->softCrc;
      softVersion = encoExtBlockDataPnt->softVersion.bits.version;
      softSubVersion = encoExtBlockDataPnt->softVersion.bits.subVersion;
      extBlockActualDataDef(extNum, crc, softVersion, softSubVersion);
    }else{
      RxErr++;
    }
    break;
  case RS485_EXT_BLOCK:
    // Проверка периодических запросов на изменение параметров RS2, а в ответ я получаю данные версии ПО, как и в блоке Ethernet
    // 1. Проверяем - может это ответ на пингование, то здесь должна быть версия ПО
    EthernetInfo = (EthernetSoftType*)extDataPnt;    
    calcCRC = Crc((unsigned char *)(EthernetInfo), sizeof(EthernetSoftType));
    if (calcCRC == 0 && extDataPnt[1] == 0x0) { // Проверяем CRC еа конкретную структуру
      //Заполнение информации о подключенном блоке расширения    
      crc = EthernetInfo->softCRC;
      softVersion = EthernetInfo->softVersion;
      softSubVersion = EthernetInfo->softSubVersion;
      extBlockActualDataDef(extNum, crc, softVersion, softSubVersion);
      huart = Get_ExtUartHandle(extNum);
       UserSetReceiveUart(huart);
    } else { // Если это не данные о версии ПО, то видимо запрос по Модбас
        
        if(extDataPnt[1] == 0x2B){
            uartVirtual1.needModbusSlave = 1;
            uartVirtual1.pntUart = Get_ExtUartHandle(extNum);
            
            uartVirtual1.bff = &UartVirtualBuf1[0];
            
            LengthDataBuf1 = *uartExt1.lengthBff;
            uartVirtual1.lengthBff = &LengthDataBuf1;
            
            memcpy(uartVirtual1.bff, extDataPnt, LengthDataBuf1);
            
            UserQueueSendToBackFromISR(0, &uartVirtual1);
        }
        if (extDataPnt[0] == uartVirtual1.netAdr) { // Проверяем адрес по первому каналу
     
            uartVirtual1.needModbusSlave = 1;
            uartVirtual1.pntUart = Get_ExtUartHandle(extNum);
            
            uartVirtual1.bff = &UartVirtualBuf1[0];
            
            LengthDataBuf1 = Get_VirtualUartDataSize(extNum);
            uartVirtual1.lengthBff = &LengthDataBuf1;
            
            memcpy(uartVirtual1.bff, extDataPnt, LengthDataBuf1);
            
            UserQueueSendToBackFromISR(0, &uartVirtual1);
        }
        if (extDataPnt[0] == uartVirtual2.netAdr) { // Проверяем адрес по второму каналу
         
            uartVirtual2.needModbusSlave = 1;
            uartVirtual2.pntUart = Get_ExtUartHandle(extNum);
            
            uartVirtual2.bff = &UartVirtualBuf2[0];
            
            LengthDataBuf2 = Get_VirtualUartDataSize(extNum);
            uartVirtual2.lengthBff = &LengthDataBuf2;
            
            memcpy(uartVirtual2.bff, extDataPnt, LengthDataBuf2);
            
            UserQueueSendToBackFromISR(0, &uartVirtual2);
        }
        huart = Get_ExtUartHandle(extNum);
        UserSetReceiveUart(huart);
    }
    break;
  case DIN_DOUT_EXT_BLOCK:
    DinDoutExtBlockDataPnt = (DinStateTxType *)extDataPnt;
    rxCrc = DinDoutExtBlockDataPnt->crc;
    dataLength = sizeof(DinStateTxType) - sizeof(DinDoutExtBlockDataPnt->crc);
    calcCRC = Crc((unsigned char *)(DinDoutExtBlockDataPnt), dataLength);
    if(rxCrc ==  calcCRC){
      GlobalM4.DinDoutExtData.DinState.bit.Din1 = DinDoutExtBlockDataPnt->DinState.bit.Din1;
      GlobalM4.DinDoutExtData.DinState.bit.Din2 = DinDoutExtBlockDataPnt->DinState.bit.Din2;
      GlobalM4.DinDoutExtData.DinState.bit.Din3 = DinDoutExtBlockDataPnt->DinState.bit.Din3;
      GlobalM4.DinDoutExtData.DinState.bit.Din4 = DinDoutExtBlockDataPnt->DinState.bit.Din4;
      GlobalM4.DinDoutExtData.DinState.bit.Din5 = DinDoutExtBlockDataPnt->DinState.bit.Din5;
      GlobalM4.DinDoutExtData.DinState.bit.Din6 = DinDoutExtBlockDataPnt->DinState.bit.Din6;
      
      GlobalM4.DinDoutExtData.softVersion = DinDoutExtBlockDataPnt->softVersion;
      GlobalM4.DinDoutExtData.softSubVersion = DinDoutExtBlockDataPnt->softSubVersion;
      GlobalM4.DinDoutExtData.softCRC = DinDoutExtBlockDataPnt->softCRC;
      
      //Заполнение информации о подключенном блоке расширения
      crc = DinDoutExtBlockDataPnt->softCRC;
      softVersion = DinDoutExtBlockDataPnt->softVersion;
      softSubVersion = DinDoutExtBlockDataPnt->softSubVersion;
      extBlockActualDataDef(extNum, crc, softVersion, softSubVersion);
    }
    break;
  case AIN_AOUT_EXT_BLOCK:
    AinAoutExtBlockDataPnt = (AinAoutRx_type *)extDataPnt;
    rxCrc = AinAoutExtBlockDataPnt->crc;
    dataLength = sizeof(AinAoutRx_type) - sizeof(AinAoutExtBlockDataPnt->crc);
    calcCRC = Crc((unsigned char *)(AinAoutExtBlockDataPnt) + 2, dataLength);
    if(rxCrc ==  calcCRC){
      GlobalM4.AinAoutExtData.Ain1Val   = AinAoutExtBlockDataPnt->Ain1Val;
      GlobalM4.AinAoutExtData.Ain2Val   = AinAoutExtBlockDataPnt->Ain2Val;
      GlobalM4.AinAoutExtData.Adc1Val   = AinAoutExtBlockDataPnt->Adc1Val;
      GlobalM4.AinAoutExtData.Adc2Val   = AinAoutExtBlockDataPnt->Adc2Val;
      GlobalM4.AinAoutExtData.Aout1Val   = AinAoutExtBlockDataPnt->Aout1Val;
      GlobalM4.AinAoutExtData.Aout2Val   = AinAoutExtBlockDataPnt->Aout2Val;
      GlobalM4.AinAoutExtData.AinState1 = AinAoutExtBlockDataPnt->AinState1;
      GlobalM4.AinAoutExtData.AinState2 = AinAoutExtBlockDataPnt->AinState2;
      crc = AinAoutExtBlockDataPnt->softCRC;
      softVersion = AinAoutExtBlockDataPnt->softVersion;
      softSubVersion = AinAoutExtBlockDataPnt->softSubVersion;
      extBlockActualDataDef(extNum, crc, softVersion, softSubVersion);
    }
    break;
  case CAN_EXT_BLOCK:
    break;
  case PROFIBUS_EXT_BLOCK:
    break;
  case ETHER_EXT_BLOCK:
    // На запрос перехода в режим обмена, вычитываем из блока версию ПО и CRC ПО блока Ethernet
    EthernetInfo = (EthernetSoftType*)extDataPnt;
    
    //Заполнение информации о подключенном блоке расширения
    crc = EthernetInfo->softCRC;
    softVersion = EthernetInfo->softVersion;
    softSubVersion = EthernetInfo->softSubVersion;
    extBlockActualDataDef(extNum, crc, softVersion, softSubVersion);    
    break;
  case PROFINET_EXT_BLOCK:
    break;
  }
}

// Проверка - есть такой блок в системе или нет?
u16 CheckExtConnectedBlocks (u16 extBlock)
{  
  // Если в данном слоте такой блок уже есть, значит возвращаем 1 - его применять нельзя.  
  if (GlobalM4.extConnectedBlocks.ext1.extType == extBlock)
    return 1;
  // Если в данном слоте такой блок уже есть, значит возвращаем 1 - его применять нельзя.  
  if (GlobalM4.extConnectedBlocks.ext2.extType == extBlock)
    return 1;
  // Если в данном слоте такой блок уже есть, значит возвращаем 1 - его применять нельзя.
  if (GlobalM4.extConnectedBlocks.ext3.extType == extBlock)
    return 1;
  
  // Такого блока в системе еще нету - разрешаем принять его
  return 0;
}

//Обработка завершения приема данных от блока расширения
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  u16 dataLength;
  u16 calcCRC;
  identAnswType *identAnswPnt;
  HeartBeatType *heartBeatPnt;
  
  if(huart == &EXT1_PortStructure){ //проверяем порт-источник прерывания
    //проверяем тип принятого пакета
    switch(ext1ExchangeState){
    case EXT_BLOCK_IDENT:
      identAnswPnt = (identAnswType *)(huart->pRxBuffPtr);
      dataLength = sizeof(identAnswType) - sizeof(identAnswPnt->crc);
      calcCRC = Crc((unsigned char *)(identAnswPnt), dataLength);
      if(calcCRC == identAnswPnt->crc){
        extAnswerWaitTimers(EXT_TIM_SET, EXT_TIM_NOP); // перезапуск счетчика, чтобы не перейти в повторную инициализацию
        // Делаем проверку блока - разрешено ли с ним работать. Если такой блок уже установлен, то мы не начинаем с ним работать
        if(!CheckExtConnectedBlocks(identAnswPnt->extBlock)) {
          GlobalM4.extConnectedBlocks.ext1.extType = identAnswPnt->extBlock; //текущий подключенный блок
          ext1ExchangeState = EXT_BLOCK_EXCHANGE;
        }
      }
      break;
    case EXT_BLOCK_EXCHANGE: //Состояние обмена
      extBlockDataParsing(EXT1, GlobalM4.extConnectedBlocks.ext1.extType, huart->pRxBuffPtr); //парсинг считанных от блока расширения данных
      extAnswerWaitTimers(EXT_TIM_SET, EXT_TIM_NOP); // перезапуск счетчика, чтобы не перейти в повторную инициализацию
      break;
    case EXT_SLAVE_UART:
      // Проверить что пришел запрос HeartBeat
      heartBeatPnt = (HeartBeatType *)(huart->pRxBuffPtr);
      dataLength = sizeof(HeartBeatType) - sizeof(heartBeatPnt->crc);
      calcCRC = Crc((unsigned char *)(heartBeatPnt), dataLength);
      if(calcCRC == heartBeatPnt->crc){
        extAnswerWaitTimers(EXT_TIM_SET, EXT_TIM_NOP); // перезапуск счетчика, чтобы не перейти в повторную инициализацию
        uartExt1.status = SendBackPingStatus;
        if (huart->RxState == HAL_UART_STATE_READY) // Если я не на приеме - перейти на прием!
          UserSetReceiveUart(huart);
        
        break; // Если это запрос HeartBeat - то сразу выходим.
      }
      
      // Запросы в задачу Модбас идут через общее прерывание, здесь только перезапуск таймера
      extAnswerWaitTimers(EXT_TIM_SET, EXT_TIM_NOP); // перезапуск счетчика, чтобы не перейти в повторную инициализацию      
      break; 
    case EXT_WAIT_SOFT_INFO:
      extBlockDataParsing(EXT1, GlobalM4.extConnectedBlocks.ext1.extType, huart->pRxBuffPtr); //парсинг считанных от блока расширения данных
      extAnswerWaitTimers(EXT_TIM_SET, EXT_TIM_NOP); // перезапуск счетчика, чтобы не перейти в повторную инициализацию
      break;      
    }
  }else if(huart == &EXT2_PortStructure){
    //проверяем тип принятого пакета
    switch(ext2ExchangeState){
    case EXT_BLOCK_IDENT:
      identAnswPnt = (identAnswType *)(huart->pRxBuffPtr);
      dataLength = sizeof(identAnswType) - sizeof(identAnswPnt->crc);
      calcCRC = Crc((unsigned char *)(identAnswPnt), dataLength);
      if(calcCRC == identAnswPnt->crc){
        extAnswerWaitTimers(EXT_TIM_NOP, EXT_TIM_SET); // перезапуск счетчика, чтобы не перейти в повторную инициализацию
        if(!CheckExtConnectedBlocks(identAnswPnt->extBlock)) {
          GlobalM4.extConnectedBlocks.ext2.extType = identAnswPnt->extBlock; //текущий подключенный блок
          ext2ExchangeState = EXT_BLOCK_EXCHANGE;
        }
      }
      break;
    case EXT_BLOCK_EXCHANGE: //Состояние обмена
      extBlockDataParsing(EXT2, GlobalM4.extConnectedBlocks.ext2.extType, huart->pRxBuffPtr); //парсинг считанных от блока расширения данных
      extAnswerWaitTimers(EXT_TIM_NOP, EXT_TIM_SET); // перезапуск счетчика, чтобы не перейти в повторную инициализацию
      break;
    case EXT_SLAVE_UART:
      // Проверить что пришел запрос не HeartBeat и отправить данные в задачу Modbus
      heartBeatPnt = (HeartBeatType *)(huart->pRxBuffPtr);
      dataLength = sizeof(HeartBeatType) - sizeof(heartBeatPnt->crc);
      calcCRC = Crc((unsigned char *)(heartBeatPnt), dataLength);
      if(calcCRC == heartBeatPnt->crc){
        extAnswerWaitTimers(EXT_TIM_NOP, EXT_TIM_SET); // перезапуск счетчика, чтобы не перейти в повторную инициализацию
        uartExt2.status = SendBackPingStatus;
        if (huart->RxState == HAL_UART_STATE_READY) // Если я не на приеме - перейти на прием!
          UserSetReceiveUart(huart);
        
        break; // Если это запрос HeartBeat - то сразу выходим.
      }
         
      // Запросы в задачу Модбас идут через общее прерывание, здесь только перезапуск таймера
      extAnswerWaitTimers(EXT_TIM_NOP, EXT_TIM_SET); // перезапуск счетчика, чтобы не перейти в повторную инициализацию       
      break;      
    case EXT_WAIT_SOFT_INFO:
      extBlockDataParsing(EXT2, GlobalM4.extConnectedBlocks.ext2.extType, huart->pRxBuffPtr); //парсинг считанных от блока расширения данных
      extAnswerWaitTimers(EXT_TIM_NOP, EXT_TIM_SET); // перезапуск счетчика, чтобы не перейти в повторную инициализацию
      break;      
    }
  }
}

//Заполнение текущей информации о подключенных блоках расширения
//extNum - номер порта
//CRC - контрольная сумма ПО блока, подключенного к порту extNum
//softVersion - версия ПО блока, подключенного к порту extNum
//softSubVersion - подверсия ПО блока, подключенного к порту extNum
void extBlockActualDataDef(extNumType extNum, u16 crc, u16 softVersion, u16 softSubVersion)
{
  switch(extNum){
  case EXT1:
    GlobalM4.extConnectedBlocks.ext1.crc = crc;
    GlobalM4.extConnectedBlocks.ext1.softVersion = softVersion;
    GlobalM4.extConnectedBlocks.ext1.softSubVersion = softSubVersion;
    break;
  case EXT2:
    GlobalM4.extConnectedBlocks.ext2.crc = crc;
    GlobalM4.extConnectedBlocks.ext2.softVersion = softVersion;
    GlobalM4.extConnectedBlocks.ext2.softSubVersion = softSubVersion;
    break;
  case EXT3:
    GlobalM4.extConnectedBlocks.ext3.crc = crc;
    GlobalM4.extConnectedBlocks.ext3.softVersion = softVersion;
    GlobalM4.extConnectedBlocks.ext3.softSubVersion = softSubVersion;
    break;
  }
}

UART_HandleTypeDef *Get_ExtUartHandle(extNumType extNum){
  
  switch(extNum){
    case EXT1:
        return &EXT1_PortStructure;
    break;
    case EXT2:
        return &EXT2_PortStructure;
    break;
    default:
        return &EXT1_PortStructure;
    break;
  }
}
u8 Get_VirtualUartDataSize(extNumType extNum){
  
  switch(extNum){
    case EXT1:
        return *uartExt1.lengthBff;
    break;
    case EXT2:
        return *uartExt2.lengthBff;
    break;
    default:
        return *uartExt1.lengthBff;
    break;
  }
}
#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)