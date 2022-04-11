#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
#include "DrvErrSWandSTO.h"
#include "DrvTypePow.h"
#include "GlobalVar.h"

extern DRVTYPE DrvType;
extern SWANDSTO SwAndSto;
extern TIM_HandleTypeDef htim1;


// Настроить прерывание от компаратора
void Init(SWANDSTO_handle s){ 
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  DrvType_Enum     Type;
  u32              ModeIT;
  
  // 1) Получить тип ПЧ
  Type = DrvType.GetType(&DrvType);
  
  // 2) В зависимости от типа ПЧ выбрать нужный фронт сигнала который вызывает прерывание
#warning Надо внести изменения в обработку аварии ключей для блока PB24-M40  
  if(Type == LowPow_PB24 || HighPow_PB24){                       // Для CP24
     ModeIT = GPIO_MODE_IT_RISING;          // По переднему фронту
  }
  else if(Type == HighPow_Combo){                 // Для CP24B
     ModeIT = GPIO_MODE_IT_FALLING;         // По заднему фронту
  }
  else{
     ModeIT = GPIO_MODE_IT_RISING_FALLING;  // По заднему и переднему фронту
  }
  
  // 3)Настроить порт для прерывания по аварии ключа фазы U
  GPIO_InitStruct.Pin = SW_U_PIN;
  GPIO_InitStruct.Mode = ModeIT;               // 
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SW_U_PORT, &GPIO_InitStruct);
  
  // 4)Настроить порт для прерывания по аварии ключа фазы V
  GPIO_InitStruct.Pin = SW_V_PIN;
  GPIO_InitStruct.Mode = ModeIT;               // 
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SW_V_PORT, &GPIO_InitStruct);
  
  // 5)Настроить порт для прерывания по аварии ключа фазы W
  GPIO_InitStruct.Pin = SW_W_PIN;
  GPIO_InitStruct.Mode = ModeIT;               // 
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SW_W_PORT, &GPIO_InitStruct);
  
  // 6)Настроить порт для прерывания по аварии тормозного ключа
  GPIO_InitStruct.Pin = SW_T_PIN;
  GPIO_InitStruct.Mode = ModeIT;               // 
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SW_T_PORT, &GPIO_InitStruct);
    
  // 7)Настроить порт для прерывания по аварии STO 
  GPIO_InitStruct.Pin = STO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;                      // 
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(STO_PORT, &GPIO_InitStruct);
  
  // 8) Настроить прерывания  
  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);      // Приоритет как у МТЗ
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
  
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);    // Приоритет как у МТЗ
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);  // Приоритет как у МТЗ
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Разрешить обработку аварии ключей
void OnSW(SWANDSTO_handle s){
   s->ContrSW = ContrOn;
}

// Запретить обработку аварии ключей
void OffSW(SWANDSTO_handle s){
   s->ContrSW = ContrOff; 
}

// Включить обработку аварии STO
void OnSTO(SWANDSTO_handle s){
   s->ContrSTO = ContrOn;
}

// Запретить обработку аварии STO
void OffSTO(SWANDSTO_handle s){
   s->ContrSTO = ContrOff;
}

// Получить текущее состояние STO (не фронт а устоявшиеся значения)
// В прерывании отработаем только фронт, а здесь устоявшейся уровень
StoStatus_Enum getStoStatus(SWANDSTO_handle s){
   GPIO_PinState  PinState;
   StoStatus_Enum StatusSTO = StoOk;
  
   // 1) Проверить включен ли контроль STO
   if(SwAndSto.ContrSTO == ContrOn){
     // 2) Прочитать состояние порта
     PinState = HAL_GPIO_ReadPin(STO_PORT, STO_PIN);     
     
     // 3) Принять решение о наличии аварии STO
     if(PinState == GPIO_PIN_SET){
         StatusSTO = StoErr;     // Авария!!!  
     }
   }
   
   // 4) Проверить установлен ли флаг аварии по фронту в прерывани
   if(GlobalM4.FailData.FailsFlag.bit.ExtStop == 1){
      StatusSTO = StoErr;     // Авария!!!
   }
   
   return StatusSTO;
}

// Получить текущее состояние SwU (не фронт а устоявшиеся значения)
// В прерывании отработаем только фрон, а здесь устоявшейся уровень
SwStatus_Enum getSwUStatus(SWANDSTO_handle s){
   GPIO_PinState  PinState;
   SwStatus_Enum StatusSw = SwOk;
   DrvType_Enum     Type;
  
   // 1) Получить тип ПЧ
   Type = DrvType.GetType(&DrvType);
   // 2) Прочитать состояние порта
   PinState = HAL_GPIO_ReadPin(SW_U_PORT, SW_U_PIN);     
  
   // 3) Проверить включен ли контроль SwU
   if(SwAndSto.ContrSW == ContrOn) {
     if (Type == HighPow_Combo) {       
       //PinState = HAL_GPIO_ReadPin(SW_U_PORT, SW_U_PIN);          
       // 4) Принять решение о наличии аварии SwU
       if(PinState == GPIO_PIN_RESET){
         StatusSw = SwErr;     // Авария!!!  
       }
     }
//     else if (Type == LowPow) {       
//       //PinState = HAL_GPIO_ReadPin(SW_U_PORT, SW_U_PIN);          
//       // 4) Принять решение о наличии аварии SwU
//       if(PinState == GPIO_PIN_SET){
//         StatusSw = SwErr;     // Авария!!!  
//       }
//     }
   } 
   
   // 5) Проверить установлен ли флаг аварии по фронту в прерывани
   if(GlobalM4.FailData.FailsFlag.bit.SwU == 1){
      StatusSw = SwErr;     // Авария!!!
   }
   
   return StatusSw;
}

// Получить текущее состояние SwV (не фронт а устоявшиеся значения)
// В прерывании отработаем только фронт, а здесь устоявшейся уровень
SwStatus_Enum getSwVStatus(SWANDSTO_handle s){
   GPIO_PinState  PinState;
   SwStatus_Enum StatusSw = SwOk;
   DrvType_Enum     Type;
  
   // 1) Получить тип ПЧ
   Type = DrvType.GetType(&DrvType);
   // 2) Прочитать состояние порта
   PinState = HAL_GPIO_ReadPin(SW_V_PORT, SW_V_PIN);     
  
   // 3) Проверить включен ли контроль SwV
   if(SwAndSto.ContrSW == ContrOn) {
     if (Type == HighPow_Combo) {       
       //PinState = HAL_GPIO_ReadPin(SW_V_PORT, SW_V_PIN);          
       // 4) Принять решение о наличии аварии SwV
       if(PinState == GPIO_PIN_RESET){
         StatusSw = SwErr;     // Авария!!!  
       }
     }
//     else if (Type == LowPow) {       
//       //PinState = HAL_GPIO_ReadPin(SW_V_PORT, SW_V_PIN);          
//       // 4) Принять решение о наличии аварии SwV
//       if(PinState == GPIO_PIN_SET){
//         StatusSw = SwErr;     // Авария!!!  
//       }
//     }
   } 
   
   // 5) Проверить установлен ли флаг аварии по фронту в прерывани
   if(GlobalM4.FailData.FailsFlag.bit.SwV == 1){
      StatusSw = SwErr;     // Авария!!!
   }
   
   return StatusSw;
}

// Получить текущее состояние SwW (не фронт а устоявшиеся значения)
// В прерывании отработаем только фрон, а здесь устоявшейся уровень
SwStatus_Enum getSwWStatus(SWANDSTO_handle s){
   GPIO_PinState  PinState;
   SwStatus_Enum StatusSw = SwOk;
   DrvType_Enum     Type;
  
   // 1) Получить тип ПЧ
   Type = DrvType.GetType(&DrvType);
   // 2) Прочитать состояние порта
   PinState = HAL_GPIO_ReadPin(SW_W_PORT, SW_W_PIN);     
   
   // 3) Проверить включен ли контроль SwW
   if(SwAndSto.ContrSW == ContrOn) {
     if (Type == HighPow_Combo) {       
       //PinState = HAL_GPIO_ReadPin(SW_W_PORT, SW_W_PIN);          
       // 4) Принять решение о наличии аварии SwW
       if(PinState == GPIO_PIN_RESET){
         StatusSw = SwErr;     // Авария!!!  
       }
     }
//     else if (Type == LowPow) {       
//       //PinState = HAL_GPIO_ReadPin(SW_W_PORT, SW_W_PIN);          
//       // 4) Принять решение о наличии аварии SwW
//       if(PinState == GPIO_PIN_SET){
//         StatusSw = SwErr;     // Авария!!!  
//       }
//     }
   }
   
   // 4) Проверить установлен ли флаг аварии по фронту в прерывани
   if(GlobalM4.FailData.FailsFlag.bit.SwW == 1){
      StatusSw = SwErr;     // Авария!!!
   }
   
   return StatusSw;
}

// Получить текущее состояние SwT (не фронт а устоявшиеся значения)
// В прерывании отработаем только фрон, а здесь устоявшейся уровень
SwStatus_Enum getSwTStatus(SWANDSTO_handle s){
   GPIO_PinState  PinState;
   SwStatus_Enum StatusSw = SwOk;
   DrvType_Enum     Type;
  
   // 1) Получить тип ПЧ
   Type = DrvType.GetType(&DrvType);
   // 2) Прочитать состояние порта
   PinState = HAL_GPIO_ReadPin(SW_T_PORT, SW_T_PIN);
   
   // 3) Проверить включен ли контроль SwT
   if(SwAndSto.ContrSW == ContrOn) {
     if (Type == HighPow_Combo) {       
       //PinState = HAL_GPIO_ReadPin(SW_T_PORT, SW_T_PIN);          
       // 4) Принять решение о наличии аварии SwT
       if(PinState == GPIO_PIN_RESET){
         StatusSw = SwErr;     // Авария!!!  
       }
     }
//     else if (Type == LowPow) {       
//       //PinState = HAL_GPIO_ReadPin(SW_T_PORT, SW_T_PIN);          
//       // 4) Принять решение о наличии аварии SwT
//       if(PinState == GPIO_PIN_SET){
//         StatusSw = SwErr;     // Авария!!!  
//       }
//     }
   }
   
   // 4) Проверить установлен ли флаг аварии по фронту в прерывани
   if(GlobalM4.FailData.FailsFlag.bit.SwT == 1){
      StatusSw = SwErr;     // Авария!!!
   }
   
   return StatusSw;
}

// Квитирование аварии ключей
void AskErr(SWANDSTO_handle s){
   
  // Аварии ключей квитируем только для мощных ПЧ
  if(DrvType.GetType(&DrvType) == HighPow_Combo){
      DrvType.AskErr(&DrvType);                 // Квитирование     
  }
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Обработчики прерываний
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void HAL_GPIO_EXTI10_Callback(void){
  if(SwAndSto.ContrSW == ContrOn){
      // 1) Отключить ШИМ!!!
      htim1.Instance->CCER &= ~(0xfff);    
        
      // 2) Установить флаг наличие аварии ключей
      GlobalM4.FailData.FailsFlag.bit.SwU = 1;     // Установить флаг о том что авария была (в FailControl отработаем)
  }
}

void HAL_GPIO_EXTI7_Callback(void){
   if(SwAndSto.ContrSW == ContrOn){
      // 1) Отключить ШИМ!!!
      htim1.Instance->CCER &= ~(0xfff);    
        
      // 2) Установить флаг наличие аварии ключей
      GlobalM4.FailData.FailsFlag.bit.SwV = 1;     // Установить флаг о том что авария была (в FailControl отработаем)
   }
}

void HAL_GPIO_EXTI4_Callback(void){
   if(SwAndSto.ContrSW == ContrOn){
      // 1) Отключить ШИМ!!!
      htim1.Instance->CCER &= ~(0xfff);    
        
      // 2) Установить флаг наличие аварии ключей
      GlobalM4.FailData.FailsFlag.bit.SwW = 1;     // Установить флаг о том что авария была (в FailControl отработаем)
   }
}

void HAL_GPIO_EXTI5_Callback(void){
   if(SwAndSto.ContrSW == ContrOn){
      // 1) Отключить ШИМ!!!
      htim1.Instance->CCER &= ~(0xfff);    
        
      // 2) Установить флаг наличие аварии ключей
      GlobalM4.FailData.FailsFlag.bit.SwT = 1;     // Установить флаг о том что авария была (в FailControl отработаем)
   }
}

void HAL_GPIO_EXTI14_Callback(void){
  if(SwAndSto.ContrSTO == ContrOn){
      // 1) Отключить ШИМ!!!
      htim1.Instance->CCER &= ~(0xfff);    
        
      // 2) Установить флаг наличие аварии STO
      GlobalM4.FailData.FailsFlag.bit.ExtStop = 1;     // Установить флаг о том что авария была (в FailControl отработаем)
   }
}
#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)