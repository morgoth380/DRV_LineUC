#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "DrvTypePow.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "measTime.h"


// Прототипы функций
void delay(u32 time_delay);                  // Функция задержки

// Настроить 
void TypeDrvInit(DRVTYPE_handle d){
  GPIO_PinState    PinState, PinStateOld;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Включить тактирование порта
  __HAL_RCC_GPIOA_CLK_ENABLE();     // Включить тактирование порта A
  
  // 2) Настроить порт как вход
  GPIO_InitStruct.Pin = TYPE_DRV_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN; //GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TYPE_DRV_PORT, &GPIO_InitStruct);
  
  // 3) Получить состояние порта
  osDelay(1);
  PinStateOld = HAL_GPIO_ReadPin(TYPE_DRV_PORT, TYPE_DRV_PIN);
  osDelay(1);
  PinState = HAL_GPIO_ReadPin(TYPE_DRV_PORT, TYPE_DRV_PIN);
  
  // 4) Принять решение о типе ПЧ
  if(PinStateOld != PinState){
     d->DrvType = DrvTypeFault;
  } else if(PinState == GPIO_PIN_SET){
     d->DrvType = HighPow_Combo;
  }
  else if(PinState == GPIO_PIN_RESET){
    switch (d->DrvTypePreset) {
        case DrvTypeAutoSet:
        case LowPow_PB24:
            d->DrvType = LowPow_PB24; // Если предустановка Авто или LowPow_PB24 - Разрешаем
          break;
        case HighPow_PB24:
            d->DrvType = HighPow_PB24; // Если предустановка HighPow_PB24 - Разрешаем     
          break;
        default:
          d->DrvType = DrvTypeFault; // Иначе ошибка - что-то не так встало
            break;          
    }      
  }
  
  // 5) Для блоков малой мощности все... 
  if(d->DrvType == LowPow_PB24){
     return;
  }
  
  // 6) Для блоков большей мощности перенастроить Пин на выход с открытым колектором
  HAL_GPIO_WritePin(TYPE_DRV_PORT, TYPE_DRV_PIN, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = TYPE_DRV_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TYPE_DRV_PORT, &GPIO_InitStruct);
}

// Получить Тип ПЧ
DrvType_Enum GetTypeDrv(DRVTYPE_handle d){
  return d->DrvType;
}

// Квитирование аварии ключей 
void AskErrKey(DRVTYPE_handle d){
  
  HAL_GPIO_WritePin(TYPE_DRV_PORT, TYPE_DRV_PIN, GPIO_PIN_RESET);
  
  delay(DELAY_ASK_ERR);
  
  HAL_GPIO_WritePin(TYPE_DRV_PORT, TYPE_DRV_PIN, GPIO_PIN_SET);
}

// Функция задержки
void delay(u32 time_delay){	
    u32 i;
    
    for(i = 0; i < time_delay; i++){
       __asm volatile("NOP");
    }
}

#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)