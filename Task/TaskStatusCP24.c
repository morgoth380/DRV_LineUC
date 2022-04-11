#include "TaskStatusCP24.h"
#include "GlobalVar.h"
#include "Global_Include.h"
#include "stm32f446xx.h"

#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
    // Ноги MK_OK
    #define MK_OK_PORT    GPIOE    
    #define MK_OK_PIN     GPIO_PIN_1
    // Ноги MK_ERR
    #define MK_ERR_PORT   GPIOE
    #define MK_ERR_PIN    GPIO_PIN_6
    // Для блока версии V2.2
    #define MK_ERR_PORT_V22   GPIOC
    #define MK_ERR_PIN_V22    GPIO_PIN_13

#elif defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    // Ноги MK_OK
    #define MK_OK_PORT    GPIOE    
    #define MK_OK_PIN     GPIO_PIN_1
    // Ноги MK_ERR
    #define MK_ERR_PORT   GPIOB
    #define MK_ERR_PIN    GPIO_PIN_8
#endif


// Прототипы функций
void GpioInit(void);                        // Настроить порты для управления светодиодами
u16 GetPeriodOk(void);                      // Получить период мигания для светодиода "MK_OK"
u16 GetPeriodErr(RCC_StatusTypeDef Rcc);    // Получить период мигания для светодиода "MK_ERR"

// Глобальные переменные выхода управления MK_ERR
GPIO_TypeDef    *MK_ERR_PORT_CP24;
u16             MK_ERR_PIN_CP24;

// Задача отображающая статус CP24 при помощи двух светодиодов на блоке
void FuncStatusCP24(void *Rcc){
  u16 TimerOkLed, TimerErrLed;
  RCC_StatusTypeDef  StatusRcc;
  
  // 1) Получить статус модуля тактирования
  StatusRcc = *(RCC_StatusTypeDef*)Rcc;
  
  #if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)  
  // Задаем порт и ногу для управления
  switch (GlobalM4.versionCP24) {
      case CP24_v21:
        // "MK_ERR"
        MK_ERR_PORT_CP24 = MK_ERR_PORT;
        MK_ERR_PIN_CP24 = MK_ERR_PIN;
        break;
      case CP24_v22:
        // "MK_ERR"
        MK_ERR_PORT_CP24 = MK_ERR_PORT_V22;
        MK_ERR_PIN_CP24 = MK_ERR_PIN_V22;
        break;        
  }
  #elif defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
        MK_ERR_PORT_CP24 = MK_ERR_PORT;
        MK_ERR_PIN_CP24 = MK_ERR_PIN;    
  #endif
  
  // 1) Настроить порты
  GpioInit();
  
  // 2) Засветить светодиоды (для контроля целостности) 
  HAL_GPIO_WritePin(MK_OK_PORT, MK_OK_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(MK_ERR_PORT_CP24, MK_ERR_PIN_CP24, GPIO_PIN_SET);
  osDelay(500);
  HAL_GPIO_WritePin(MK_OK_PORT, MK_OK_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(MK_ERR_PORT_CP24, MK_ERR_PIN_CP24, GPIO_PIN_RESET);
  
  // 3) Инициализация счетчиков времени
  TimerOkLed = 0;
  TimerErrLed = 0;
  
  while(1){
     osDelay(10);
    
     // 4) Инкремент счетчиков
     TimerOkLed++;
     TimerErrLed++;

     // 5) Обработка светодиода "MK_OK"
     if(TimerOkLed > GetPeriodOk()){
        TimerOkLed = 0;                               // Обнулить счетчик
        
        HAL_GPIO_TogglePin(MK_OK_PORT, MK_OK_PIN);    // Изменить состояние порта
     }
  
     // 6) Обработка светодиода "MK_ERR"
     if(StatusRcc == RCC_OK){                                    // Все Ок!  
        TimerErrLed = 0;
        HAL_GPIO_WritePin(MK_ERR_PORT_CP24, MK_ERR_PIN_CP24, GPIO_PIN_RESET);       // Потушить 
     }
     else if((StatusRcc == RCC_ERROR)||(StatusRcc == RCC_TIMEOUT)||(StatusRcc == RCC_TIMEOUT_PLL)){
        TimerErrLed = 0;
        HAL_GPIO_WritePin(MK_ERR_PORT_CP24, MK_ERR_PIN_CP24, GPIO_PIN_SET);         // Засветить
     }
     else if(TimerErrLed > GetPeriodErr(StatusRcc)){
        TimerErrLed = 0; 
        HAL_GPIO_TogglePin(MK_ERR_PORT_CP24, MK_ERR_PIN_CP24);                      // мигаем
     }
   }
}

//******************************************************************************
// Настроить порты для управления светодиодами
void GpioInit(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // 1) Включить тактирование
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  // "MK_OK"
  HAL_GPIO_WritePin(MK_OK_PORT, MK_OK_PIN, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = MK_OK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MK_OK_PORT, &GPIO_InitStruct);

  
//  MK_ERR_PORT_CP24
//  MK_ERR_PIN_CP24

  // "MK_ERR"
  HAL_GPIO_WritePin(MK_ERR_PORT_CP24, MK_ERR_PIN_CP24, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = MK_ERR_PIN_CP24;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MK_ERR_PORT_CP24, &GPIO_InitStruct);      
}

// Получить период мигания для светодиода "MK_OK"
u16 GetPeriodOk(void){
  
  if(GlobalM4.FlagCommand.bit.PwmOn){
     return 5;       // *10ms               // В работе                        
  }
  else{
     return 50;      // *10ms               // В останове
  }
}

// Получить период мигания для светодиода "MK_ERR"
u16 GetPeriodErr(RCC_StatusTypeDef Rcc){
  
  if(Rcc == RCC_TIMEOUT_HSE){               // Отказал основной кварц
     return 5;                              
  }
  else if(Rcc == RCC_TIMEOUT_LSE){          // Отказал часовой кварц
     return 50;
  }
  else{
     return 0;
  }
}


 
  