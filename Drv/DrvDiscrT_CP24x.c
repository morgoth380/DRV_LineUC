/*
    Модуль общий на два проекта. 
    h - файл на каждый проект свой. Если данная функция не используется, то порт и ногу надо объявить как ноль
*/

#include "DrvDiscrT.h"
#include "GlobalVar.h"

// Настроить 
void TDiscrInit(TDISCRDRV *t){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  u32 tmp;  
  
  tmp = (u32)DISCR_T_PORT;
  
  // Если порт  назначен, то проводим инициализацию порта  
  if (tmp != 0) {     
      // 1) Включить тактирование порта
      __HAL_RCC_GPIOE_CLK_ENABLE();     // Включить тактирование порта E
  
      // 2) Настроить порт
      GPIO_InitStruct.Pin = DISCR_T_PIN;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(DISCR_T_PORT, &GPIO_InitStruct); 
  }
}

// Получить статус
StatTdiscr_Enum GetTDiscr(TDISCRDRV *t){
  u32 tmp;
  tmp = (u32)DISCR_T_PORT;
  
  // Если разрешен режим симуляции - блокируем обработку данной аварии
  if (GlobalM4.Simulation.Enable)  return (TdiscrOk);
  
  if (tmp == 0) {
    return (TdiscrOk);  
  } else {
    return (StatTdiscr_Enum)HAL_GPIO_ReadPin(DISCR_T_PORT, DISCR_T_PIN);
  }
  
}