/*============================================================================*/
//      Заголовочный файл для задачи работы с UART портами контроллера
/*============================================================================*/
#ifndef _DRVUART_H_
#define _DRVUART_H_

#include "type.h"

#ifdef _PROJECT_FOR_STM32_
#include "stm32f4xx_hal.h"

#endif //_PROJECT_FOR_STM32_

#ifndef _PROJECT_FOR_STM32_
#define inline __inline
#endif //_PROJECT_FOR_STM32_

#define WIFI_TX_MK_Pin       GPIO_PIN_8
#define WIFI_TX_MK_GPIO_Port GPIOD
#define WIFI_RX_MK_Pin       GPIO_PIN_9
#define WIFI_RX_MK_GPIO_Port GPIOD
#define WIFI_PROG_ON_Pin     GPIO_PIN_11
#define WIFI_PROG_ON_Port    GPIOD

#define RS485_DIR_Pin        GPIO_PIN_11
#define RS485_DIR_GPIO_Port  GPIOA
#define RS485_TX_ENABLE      GPIO_PIN_RESET  	// Указываем состояние ноги направления - на передачу
#define RS485_RX_ENABLE      GPIO_PIN_SET 	// Указываем состояние ноги направления - на прием

#define PULT_DIR_Pin         GPIO_PIN_15
#define PULT_DIR_GPIO_Port   GPIOD
#define PULT_TX_ENABLE       GPIO_PIN_SET	// Указываем состояние ноги направления - на передачу
#define PULT_RX_ENABLE       GPIO_PIN_RESET	// Указываем состояние ноги направления - на прием


#ifdef _PROJECT_FOR_STM32_
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef EXT2_PortStructure;
extern UART_HandleTypeDef EXT1_PortStructure;
#endif //_PROJECT_FOR_STM32_

/* USER CODE BEGIN Private defines */
#define NumUART1 0
#define NumUART2 1
#define NumUART3 2
#define DMA_CCR1_EN 1;

#define szUartBff 256

#define SendBackPingStatus    0x80    // Состояние статуса, послать обратный пинг к блоку - пока в Ethernetы
#define IdleStatus            0x00    // Статус ожидания

#ifdef _PROJECT_FOR_STM32_
typedef struct {
  u16  netAdr;                     // сетевой адрес ModBus
  u16  *lengthBff;                 // указатель на длинну буфера
  u32  baudrate;                   // скорость обмена
  u16   bdrIndex;                   // Индекс скорости обмена  
  u8   *bff;                        // указатель на буффер
  u32  wdt;                        // Таймер в тиках вызова от последней удачной транзакции по порту
  u32  WdtLink;                    // Таймер для проверки связи с ПЧ
  u16  enablePusk      :1  ;
  u16  enablePrmChange :1  ;
  u16  status;                      // Переменная статуса для WIFI модуля
  u16  needModbusSlave;            // Флаг, что UARTу требуется обращение в задачу Modbus Slave
  UART_HandleTypeDef *pntUart;          //указатель на физически юарт, нужно для виртуальных портов
}uart_type;
#else
typedef struct {
  u16  netAdr;                     // сетевой адрес ModBus
  u16  *lengthBff;                 // указатель на длинну буфера
  u32  baudrate;                   // скорость обмена
  u16   bdrIndex;                   // Индекс скорости обмена  
  u8   *bff;                        // указатель на буффер
  u32  wdt;                        // Таймер в тиках вызова от последней удачной транзакции по порту
  u32  WdtLink;                    // Таймер для проверки связи с ПЧ
  u16  enablePusk      :1  ;
  u16  enablePrmChange :1  ;
  u16  status;                      // Переменная статуса для WIFI модуля
  u16  needModbusSlave;            // Флаг, что UARTу требуется обращение в задачу Modbus Slave
  u32  *pntUart;          //указатель на физически юарт, нужно для виртуальных портов
}uart_type;
#endif
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);

/* USER CODE BEGIN Prototypes */
#ifdef _PROJECT_FOR_STM32_
void USER_UART_IT(UART_HandleTypeDef *huart);
void USER_INIT_UART(UART_HandleTypeDef *huart);
void USER_INIT_UARTs(void);
void USER_UART_Receive_DMA(UART_HandleTypeDef *huart);
void UserQueueSendToBackFromISR(UART_HandleTypeDef *huart, uart_type *pntUart);
void SetPreferences (UART_HandleTypeDef *huart);
void SendMessageRxReady(uart_type *uart);
void UserSetReceiveUart(UART_HandleTypeDef *huart);
#else

#endif //_PROJECT_FOR_STM32_

void uart_init(uart_type *uartLocl, u16 netAdr, u16 baudrate, u16 FirstCall);
void MX_DMA_Init(void);                                           // Настройка DMA для работы с портами (АСУ и WIFI) 
void RS485_GPIO_Init(void);                                       // Настройка ноги DIR

//extern u8 UartBuf0[szUartBff]; 
//extern u8 UartBuf1[szUartBff];

/*============================================================================*/
//                Режим работы канала Modbus
/*============================================================================*/
enum ModbusModeEnum
{
    ModbusASU  ,     // 0
    ModbusSlave  ,     // 1
    ModbusMaster,     // 2
    InternalUart     // 3 Внутреннее применение Uart
};

/*============================================================================*/
//                Код скорости обмена по UART
/*============================================================================*/
enum{
    BR1200  ,     // 0
    BR2400  ,     // 1
    BR4800  ,     // 2
    BR9600  ,     // 3
    BR14400 ,     // 4
    BR19200 ,     // 5
    BR38400 ,     // 6
    BR56000 ,     // 7
    BR57600 ,     // 8
    BR115200,     // 9
    BR230400,     // 10
    BR460800,     // 11
    BR921600,     // 12
    BR937500,     // 13
    BR1500000,    // 14
    BR4687500,    // 15
    BR9375000     // 16
};

/*============================================================================*/
//                  Флаги состояния порта UART
/*============================================================================*/
enum STATEuart
{
    STATEuart_WaiteTXend = 4, // Ожидание завершения передачи
    STATEuart_TX = 1,         // Ожидание завершения передачи
    STATEuart_RX = 2,         // Состояние приема
    STATEuart_ACK = 3,         // Состояние проверки запроса формирования и ответа
    STATEuart_None = 0,
    STATEuart_Err = 5
};

/*============================================================================*/
/*                    Объявления типов                                        */
/*============================================================================*/
/*============================================================================*/
/*                    Глобальные переменные                                   */
/*============================================================================*/

#ifdef _PROJECT_FOR_STM32_ 
    extern uart_type uart1;
    extern uart_type uart6;
    extern uart_type uart3;
    extern uart_type uartExt2;
    extern uart_type uartExt1;    
    extern uart_type uartVirtual1;
    extern uart_type uartVirtual2;    
#else
    uart_type uart0;
    uart_type uart1;
    uart_type uart2;
    uart_type uart3;
    uart_type uart4;
    uart_type uart5;
    uart_type uart6;
    uart_type uartVirtual1;
    uart_type uartVirtual2;    
#endif //_PROJECT_FOR_STM32_
/*============================================================================*/
/*                     Прототипы функций                                      */
/*============================================================================*/
#ifdef _PROJECT_FOR_STM32_
    extern void uart_init(uart_type *uart, u16 netAdr, u16 baudrate,u16 FirstCall);
#else
    #define uart_init( uart, netAdr, baudrate, FirstCall);
#endif

#define DisableUartWiFi()                               USART3->CR1 &=  ~USART_CR1_UE
#define EnableUartWiFi()                                USART3->CR1 |=  USART_CR1_UE
u16 DinControlWiFi(void);
    
typedef enum{
  none,
  WiFi_NormalWork, 
  WiFi_ModeProg 
}WiFi_State_type;

#endif // #ifndef _DRVUART_H_


