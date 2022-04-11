#include "DrvLedStatus.h"

// Настройка переферии светодиодов
void LedInit(LED_handle led){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Включить тактирование портов
  __HAL_RCC_GPIOA_CLK_ENABLE();     // Включить тактирование порта A
  __HAL_RCC_GPIOB_CLK_ENABLE();     // Включить тактирование порта B
  __HAL_RCC_GPIOC_CLK_ENABLE();     // Включить тактирование порта C
  __HAL_RCC_GPIOD_CLK_ENABLE();     // Включить тактирование порта D
  __HAL_RCC_GPIOE_CLK_ENABLE();     // Включить тактирование порта E

  // 2) Предварительно выключить светодиоды
  HAL_GPIO_WritePin(LED_READY_PORT, LED_READY_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_WiFi_PORT, LED_WiFi_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_FAIL_PORT, LED_FAIL_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_WORK_PORT, LED_WORK_PIN, GPIO_PIN_RESET);
  
  // 3) Настроить пин для управления светодиодом "Готов"
  GPIO_InitStruct.Pin = LED_READY_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_READY_PORT, &GPIO_InitStruct);
  
  // 4) Настроить пин для управления светодиодом "WiFi"
  GPIO_InitStruct.Pin = LED_WiFi_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_WiFi_PORT, &GPIO_InitStruct);
  
  // 5) Настроить пин для управления светодиодом "Авария"
  GPIO_InitStruct.Pin = LED_FAIL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_FAIL_PORT, &GPIO_InitStruct);
  
  // 6) Настроить пин для управления светодиодом "Работа"
  GPIO_InitStruct.Pin = LED_WORK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_WORK_PORT, &GPIO_InitStruct);
}

// Установить статус светодиодов
void LedSet(LED_handle led){

  // 1) "Готов"
  HAL_GPIO_WritePin(LED_READY_PORT, LED_READY_PIN, (GPIO_PinState)led->LedReady);      //    
    
  // 2) "WiFi"
  HAL_GPIO_WritePin(LED_WiFi_PORT, LED_WiFi_PIN, (GPIO_PinState)led->LedWiFi);         //    
    
  // 3) "Авария"
  HAL_GPIO_WritePin(LED_FAIL_PORT, LED_FAIL_PIN, (GPIO_PinState)led->LedFail);         //    
    
  // 4) "Работа"
  HAL_GPIO_WritePin(LED_WORK_PORT, LED_WORK_PIN, (GPIO_PinState)led->LedWork);         //    
}











