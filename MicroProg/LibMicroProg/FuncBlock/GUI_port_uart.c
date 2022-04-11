// Создание объектов портов
//#include "drvuart.h"
#include "DrvUart.h"

extern uart_type uart0;
extern uart_type uart1;
extern uart_type uart2;
extern uart_type uart3;
extern uart_type uart4;
extern uart_type uart5;
extern uart_type uart6;
uart_type * const uartPtr[8] = { NULL, &uart0, &uart1, &uart2, &uart3, &uart4, &uart5, &uart6}; // Массив указателей на порты
/*
Tuart uartB;
Tuart uartC;
Tuart * const uartPtr[3] = { NULL, &uartB, &uartC };          // Массив указателей на порты
*/