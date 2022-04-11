#include "main.h"
#include "cmsis_os.h"


// Для микропрограммы
void FuncMicroProg(void *argument);   // Задача микропрограммы

// Переопредиление функций
void MX_TIM14_Init(void);                      // Настройка таймера для синхронизации вызова микропрограммы (4ms)
void MX_IWDG_Init(void);                       // Настройка вачдога для микропрограммы