#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
#include "DrvBackupPowerCont.h"


// Настроить драйвер
void BackupPowInit(BACKUPPOW_handle b){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  
    // 1) Включить тактирование порта
    __HAL_RCC_GPIOC_CLK_ENABLE();     // Включить тактирование порта E
  
    // 2) Настроить порт
    GPIO_InitStruct.Pin = VIN_CONTROL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(VIN_CONTROL_PORT, &GPIO_InitStruct); 
}

// Получить статус
StatBeckupPow_Enum GetBackupPowStatus(BACKUPPOW_handle b){
  GPIO_PinState  PinState;
  
  // 1) Получить состояние пина
  PinState = HAL_GPIO_ReadPin(VIN_CONTROL_PORT, VIN_CONTROL_PIN);
  
  // 2) 
  if(PinState == GPIO_PIN_RESET){
     return BeckupPowOn;             // Присутствует
  }
  else{
     return BeckupPowOff;            // Отсутствует
  }
}
#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)