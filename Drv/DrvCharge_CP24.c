#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "DrvCharge.h"

// Настроить 
void ChargeInit(CHARGE_handle c){
   GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Включить тактирование портов
  __HAL_RCC_GPIOA_CLK_ENABLE();     // Включить тактирование порта A
  __HAL_RCC_GPIOB_CLK_ENABLE();     // Включить тактирование порта B
  __HAL_RCC_GPIOC_CLK_ENABLE();     // Включить тактирование порта C
  __HAL_RCC_GPIOD_CLK_ENABLE();     // Включить тактирование порта D
  __HAL_RCC_GPIOE_CLK_ENABLE();     // Включить тактирование порта E
  
  // 2) Перевести выводы в безопасное состояние
  HAL_GPIO_WritePin(RELE_ON_PORT, RELE_ON_PIN, GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(ZAR_ON_PORT, ZAR_ON_PIN, GPIO_PIN_RESET);
  
  // 3) Настройка вывода для Rele  
  GPIO_InitStruct.Pin = RELE_ON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RELE_ON_PORT, &GPIO_InitStruct);
  
//  // 4) Настроить пин для Zar
//  GPIO_InitStruct.Pin = ZAR_ON_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(ZAR_ON_PORT, &GPIO_InitStruct);
}

// Установить режим работы компаратора
void ChargeSet(CHARGE_handle c){
 
   // Управляем выходом ZAR
   HAL_GPIO_WritePin(RELE_ON_PORT, RELE_ON_PIN, c->ReleState);   
}

#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)