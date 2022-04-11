#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "DrvFan.h"

TIM_HandleTypeDef htim3;

// Настроить вентиляторы
void InitFan(FAN_handle f){
  
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  //TIM3 GPIO Configuration    
  //PС3     ------> FAN control
  
  HAL_GPIO_WritePin(FAN_MK_GPIO_Port, FAN_MK_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = FAN_MK_Pin;                      
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FAN_MK_GPIO_Port, &GPIO_InitStruct);    
}

// Обновить задание для вентеляторов
void UpdateFan(FAN_handle f){
  // Если появилось хоть какое-то задание, значит вентилятор IGBT должен работать  
  if (f->IgbtFanTask > 0) 
    HAL_GPIO_WritePin(FAN_MK_GPIO_Port, FAN_MK_Pin, GPIO_PIN_SET);       
  else 
    HAL_GPIO_WritePin(FAN_MK_GPIO_Port, FAN_MK_Pin, GPIO_PIN_RESET);       
}
#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
