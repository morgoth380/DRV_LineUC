#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "DrvCompZPT.h"
#include "DrvPWM.h"
#include "GlobalVar.h"

// Прототипы privat функций

extern COMPZPTDRV CompZPT;
extern PWMGEN Pwm;           // Обект формирователя ШИМ (для отключения ШИМ)


// Настроить прерывание от компаратора
void CompZPTInit(COMPZPTDRV *p){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // Настроить порт для прерывания
  GPIO_InitStruct.Pin = COMP_ZPT_GPIO_Pin;
  GPIO_InitStruct.Mode = /*GPIO_MODE_IT_FALLING*/GPIO_MODE_IT_RISING_FALLING;   // Прерывание по переднему и заднему фронту
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(COMP_ZPT_GPIO_Port, &GPIO_InitStruct);
  
  // Настроим компаратор на счет событий
  CompZPT.ModeComp = CompCountFront;
  CompZPT.ReInit(&CompZPT);
}

// Установить режим работы компаратора
void SetModeComp(COMPZPTDRV *p){
  
  // 1) Отключить прерывание от компаратора
  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);  
  
  // 2) Установить приоритет прерывания в зависимости от режима
  if(p->ModeComp == CompWaitErr){                 // Отключение по переднему фронту
     HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);    // Приоритет как у МТЗ
  }
  else if(p->ModeComp == CompCountFront){         // Считаем количество импульсов
     HAL_NVIC_SetPriority(EXTI9_5_IRQn, 10, 0);   // Прерывания в фоне
  }
  else if((p->ModeComp == CompErr)||(p->ModeComp == CompOff)){
     HAL_NVIC_SetPriority(EXTI9_5_IRQn, 10, 0);   // 
     
     return;                                      // Выход без включения прерывания
  }
    
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

// Прерывание от компаратора
void HAL_GPIO_EXTI6_Callback(void){
    
    if(CompZPT.ModeComp == CompWaitErr){                 // МТЗ в цепи ЗПТ отключить ШИМ
        // 1) Отключить ШИМ!!!
        htim1.Instance->CCER &= ~(0xfff);   // 
        
        // 1) Закрыть нижние ключи
        HAL_GPIO_WritePin(PWM_PORT, BOT_W_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PWM_PORT, BOT_V_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PWM_PORT, BOT_U_PIN, GPIO_PIN_RESET);
    
        // 2) Установить флаг наличие аварии по МТЗ в цепи ЗПТ
        GlobalM4.FailData.FailsFlag.bit.MtzZPT = 1;     // Установить флаг о том что авария была (в FailControl отработаем)
        
        // 3) Установить статус компаратора авария
        CompZPT.ModeComp = CompErr;
        CompZPT.ReInit(&CompZPT);
        
        // 4) Обнулить счетчик прерываний
        CompZPT.CountFront = 0;
    }
    else{
        CompZPT.CountFront++;      
    }
}

#endif //defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)