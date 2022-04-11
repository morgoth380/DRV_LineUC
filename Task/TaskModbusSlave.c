#include "TaskModbusSlave.h"
#include "Global_Include.h"
#include "DrvUart.h"
#include "queue.h"

/* USER CODE BEGIN Header_FuncModbusSlave */
/**
* @brief Function implementing the ModbusSlave thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FuncModbusSlave */

// Объявить две переменных для управления передаче в RS-485 в блоке СР24 
#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
extern GPIO_PinState RS485_RX_ENABLE;
extern GPIO_PinState RS485_TX_ENABLE;
#endif

xQueueHandle xQueueUart;
void FuncModbusSlave(void *argument)
{
  uart_type *uart;
  uint16_t length = 0;
  
  osDelay(80);
  
  RS485_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  
  xQueueUart = xQueueCreate(20, sizeof(uart_type*));                      // создаем очередь на 20 элемента для 3 UART
  USER_INIT_UARTs();                                                    // настойка перывания, обнуления флага статусного регистра, стаем на прием данных

  for(;;)
  {
    xQueueReceive(xQueueUart, &uart, portMAX_DELAY);                    // ждем получения данных от UART
    
    // Начало замера времени выполнения
    //StartTimControl(TskModbusSlaveTC);
    
    length = *uart->lengthBff;
    //length = ModbusFunc((uint8_t*)uart->bff, length, uart);
    if(uart == &uart1) // RS-485 на основных клеммах
    {
      length = ModbusFunc((uint8_t*)uart->bff, length, uart);      
      if (length == 0)
        UserSetReceiveUart(&huart1);
      else
      {
        HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, RS485_TX_ENABLE);        
        HAL_UART_Transmit_DMA(&huart1,uart->bff,length);
        uart->wdt = 0;                          // Сбросить сторожевой таймер
        uart->WdtLink = 0;
      }
    }
    else if(uart == &uart3) // UART модуля WIFI
    {
      length = ModbusFunc((uint8_t*)uart->bff, length, uart);
      if (length == 0)
        UserSetReceiveUart(&huart3);
      else
      {        
        HAL_UART_Transmit_DMA(&huart3,uart->bff,length);
        if (uart->status != 0) { // Если статус WIFI не равен нулю, значит есть подключение.
          uart->wdt = 0;                          // Сбросить сторожевой таймер
          uart->WdtLink = 0;
        }
      }
    }
    else if(uart == &uart6) // RS-485 на панели светодиодов
    {
      length = ModbusFunc((uint8_t*)uart->bff, length, uart);
      if (length == 0)
        UserSetReceiveUart(&huart6);
      else
      {
        HAL_GPIO_WritePin(PULT_DIR_GPIO_Port, PULT_DIR_Pin, PULT_TX_ENABLE);
        HAL_UART_Transmit_DMA(&huart6,uart->bff,length);
        uart->wdt = 0;                          // Сбросить сторожевой таймер
        uart->WdtLink = 0;
      }
    }
    else if(uart == &uartExt2)
    {
      length = ModbusFunc((uint8_t*)uart->bff, length, uart);
      if (length == 0)
        UserSetReceiveUart(&EXT2_PortStructure);
      else
      {
        HAL_UART_Transmit_DMA(&EXT2_PortStructure, uart->bff, length);
        uart->wdt = 0;                          // Сбросить сторожевой таймер
        uart->WdtLink = 0;
      }
    }    
    else if(uart == &uartExt1)
    {
      length = ModbusFunc((uint8_t*)uart->bff, length, uart);
      if (length == 0)
        UserSetReceiveUart(&EXT1_PortStructure);
      else
      {
        HAL_UART_Transmit_DMA(&EXT1_PortStructure, uart->bff, length);
        uart->wdt = 0;                          // Сбросить сторожевой таймер
        uart->WdtLink = 0;
      }
    }else if(uart == &uartVirtual1)
    {
        length = ModbusFunc((uint8_t*)uart->bff, length, uart);
        if (length == 0){
          UserSetReceiveUart(uartVirtual1.pntUart);
        }else
        {
          if(uartVirtual1.pntUart == &EXT1_PortStructure){
            memcpy(uartExt1.bff, uart->bff, length);
            HAL_UART_Transmit_DMA(&EXT1_PortStructure, uartExt1.bff, length);
          }else if(uartVirtual1.pntUart == &EXT2_PortStructure){
            memcpy(uartExt2.bff, uart->bff, length);
            HAL_UART_Transmit_DMA(&EXT2_PortStructure, uartExt2.bff, length);
          }
          uart->wdt = 0;                          // Сбросить сторожевой таймер
          uart->WdtLink = 0;
        }    
    }else if(uart == &uartVirtual2)
    {
        length = ModbusFunc((uint8_t*)uart->bff, length, uart);
        if (length == 0)
          UserSetReceiveUart(&EXT1_PortStructure);
        else
        {
          if(uartVirtual2.pntUart == &EXT1_PortStructure){
            memcpy(uartExt1.bff, uart->bff, length);
            HAL_UART_Transmit_DMA(&EXT1_PortStructure, uartExt1.bff, length);
          }else if(uartVirtual2.pntUart == &EXT2_PortStructure){
            memcpy(uartExt2.bff, uart->bff, length);
            HAL_UART_Transmit_DMA(&EXT2_PortStructure, uartExt2.bff, length);
          }
          uart->wdt = 0;                          // Сбросить сторожевой таймер
          uart->WdtLink = 0;
        }
    }                    
    
    // Конец замера времени выполнения
    //EndTimControl(TskModbusSlaveTC);
  }
}
/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void UserQueueSendToBackFromISR(UART_HandleTypeDef *huart, uart_type *pntUart)
{
  uart_type *uart;
  
  if(huart == &huart1)
    uart = &uart1;
  else if(huart == &huart3)
    uart = &uart3;
  else if(huart == &huart6)
    uart = &uart6;
  else if(huart == &EXT2_PortStructure)
    uart = &uartExt2;
  else if(huart == &EXT1_PortStructure)
    uart = &uartExt1;
  
  if(pntUart != 0)
    uart = pntUart;
     
  if (uart->needModbusSlave /*&& uart->bff[0] == uart->netAdr*/) {
    SendMessageRxReady(uart);
  }
}

void SendMessageRxReady(uart_type *uart)
{
  xQueueSendToBackFromISR(xQueueUart, &uart, 0);
}
/* USER CODE END Application */
