#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
#include "DrvCharge.h"
#include "DrvTypePow.h"

extern DRVTYPE DrvType;            // Обект для получения типа ПЧ и управления квитирования аварий ключей для мощных ПЧ 


// Настроить 
void ChargeInit(CHARGE_handle c){
   GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Включить тактирование портов
  __HAL_RCC_GPIOB_CLK_ENABLE();     // Включить тактирование порта B
  __HAL_RCC_GPIOE_CLK_ENABLE();     // Включить тактирование порта E
  
  // 2) Перевести выводы в безопасное состояние
  HAL_GPIO_WritePin(RELE_ON_PORT, RELE_ON_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ZAR_ON_PORT, ZAR_ON_PIN, GPIO_PIN_RESET);
  
  // 3) Настройка вывода для Rele  
  GPIO_InitStruct.Pin = RELE_ON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RELE_ON_PORT, &GPIO_InitStruct);
  
  // 4) Настроить пин для Zar
  GPIO_InitStruct.Pin = ZAR_ON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ZAR_ON_PORT, &GPIO_InitStruct);
}

// Установить режим работы компаратора
void ChargeSet(CHARGE_handle c){
   DrvType_Enum TypeDrv;
  
   // 1) Получить тип ПЧ
   TypeDrv = DrvType.GetType(&DrvType);
   
   if(TypeDrv == LowPow_PB24){                  // Для ПЧ низкой мощности
      // 1) Установить состояние реле
      HAL_GPIO_WritePin(RELE_ON_PORT, RELE_ON_PIN, c->ReleState);
     
      // 2) Состояние ZAR - всегда должен быть 0. ИНАЧЕ СПАЛИМ ПРОЦЕССОР. НА СИЛОВОМ БЛОКЕ ЭТОТ ВЫХОД ВКЛЮЧИЛИ НА ЗЕМЛЮ!!!
      HAL_GPIO_WritePin(ZAR_ON_PORT, ZAR_ON_PIN, GPIO_PIN_RESET);
   }
   else if(TypeDrv == HighPow_Combo || TypeDrv == HighPow_PB24){  // Для ПЧ большей мощности - COMBO  и мощные PB24
      // Алгоритм управления для COMBO - Если реле включено, ZAR также не выключаем.
      if (c->ReleState)  c->ZarState = c->ReleState;
     
      // 1) Установить вывод начала заряда
      HAL_GPIO_WritePin(RELE_ON_PORT, RELE_ON_PIN, c->ReleState);

      // 2) Установить вывод управления открытия на 100% тиристоров выпрямителя
      HAL_GPIO_WritePin(ZAR_ON_PORT, ZAR_ON_PIN, c->ZarState);
   }
   else{                                 // ПЧ не определен!!! Запретить управление выпрямителем!!!   
      HAL_GPIO_WritePin(RELE_ON_PORT, RELE_ON_PIN, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(ZAR_ON_PORT, ZAR_ON_PIN, GPIO_PIN_RESET);
      c->Fail = 1; // Блок не определен.
   }
}
#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)