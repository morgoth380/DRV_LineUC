#include "DrvDinDout.h"
#include "GlobalVar.h"

// Переобъявляю ноги для 1 и 3 входов, для их динамического выбора в зависимости от типа блока
GPIO_TypeDef    *DIN_1_PORT;
u16             DIN_1_PIN;
GPIO_TypeDef    *DIN_3_PORT;
u16             DIN_3_PIN;

// Настройка переферии Din / Dout
void DinDoutInit(DINDOUT_handle d){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  // 0) Предварительная настройка динмически изменяемых ного дискретных входов.
  #if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
  // Выбираем какие ноги мы будем использовать в блоке для входа 1 и 3
  switch (GlobalM4.versionCP24) {
    case CP24_v21:
        DIN_1_PORT = DIN_1_PORT_V21;
        DIN_1_PIN  = DIN_1_PIN_V21;
        DIN_3_PORT = DIN_3_PORT_V21;
        DIN_3_PIN  = DIN_3_PIN_V21; 
    break;
    case CP24_v22:
        DIN_1_PORT = DIN_1_PORT_V22;
        DIN_1_PIN  = DIN_1_PIN_V22;
        DIN_3_PORT = DIN_3_PORT_V22;
        DIN_3_PIN  = DIN_3_PIN_V22; 
    break;        
  }     
  #elif defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    // Замена определения для стыковки с обработчиком - т.к. он общий на оба блока СР24 и СР24В
    DIN_1_PORT = DIN_1_PORT_V11;
    DIN_1_PIN  = DIN_1_PIN_V11;
    DIN_3_PORT = DIN_3_PORT_V11;
    DIN_3_PIN  = DIN_3_PIN_V11; 
  #endif
  
  // 1) Включить тактирование портов
  __HAL_RCC_GPIOA_CLK_ENABLE();     // Включить тактирование порта A
  __HAL_RCC_GPIOB_CLK_ENABLE();     // Включить тактирование порта B
  __HAL_RCC_GPIOC_CLK_ENABLE();     // Включить тактирование порта C
  __HAL_RCC_GPIOD_CLK_ENABLE();     // Включить тактирование порта D
  __HAL_RCC_GPIOE_CLK_ENABLE();     // Включить тактирование порта E
  
  // 2) Настройка Din 
  // Настроить пин для Din1
  GPIO_InitStruct.Pin = DIN_1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIN_1_PORT, &GPIO_InitStruct);
  
  // Настроить пин для Din2
  GPIO_InitStruct.Pin = DIN_2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIN_2_PORT, &GPIO_InitStruct);
  
  // Настроить пин для Din3
  GPIO_InitStruct.Pin = DIN_3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIN_3_PORT, &GPIO_InitStruct);
 
  // Настроить пин для Din4
  GPIO_InitStruct.Pin = DIN_4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIN_4_PORT, &GPIO_InitStruct);
  //HAL_GPIO_LockPin (DIN_4_PORT, DIN_4_PIN); // !*!*!*!*!*!
  
  // Настроить пин для Din5
  GPIO_InitStruct.Pin = DIN_5_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIN_5_PORT, &GPIO_InitStruct);
  // ТЕСТ БЛОКИРОВКИ настроек
  //HAL_GPIO_LockPin (DIN_5_PORT, DIN_5_PIN); // !*!*!*!*!*!
  
  // Настроить пин для Din6
  GPIO_InitStruct.Pin = DIN_6_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIN_6_PORT, &GPIO_InitStruct);
  
  // Настроить пин для DinHF
  GPIO_InitStruct.Pin = DIN_HF_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIN_HF_PORT, &GPIO_InitStruct);
  
  // 3) Настройка Dout
  // Установить Dout в безопасное состояние
  HAL_GPIO_WritePin(DOUT_1_PORT, DOUT_1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DOUT_2_PORT, DOUT_2_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DOUT_HF_PORT, DOUT_HF_PIN, GPIO_PIN_RESET);
  
  // Настроить пин для Dout1
  GPIO_InitStruct.Pin = DOUT_1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DOUT_1_PORT, &GPIO_InitStruct);
  
  // Настроить пин для Dout2
  GPIO_InitStruct.Pin = DOUT_2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DOUT_2_PORT, &GPIO_InitStruct);
  
  // Настроить пин для DoutHF
  GPIO_InitStruct.Pin = DOUT_HF_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DOUT_HF_PORT, &GPIO_InitStruct);
}

// Прочитать состояние дискретных входов
u8 GetDinVal(DINDOUT_handle d){
  u16 PinState;
  u16 DinMask, PinCount;
    
  PinCount = 0;
  DinMask = 0;
  
  PinState = (u16)HAL_GPIO_ReadPin(DIN_1_PORT, DIN_1_PIN);
  DinMask = DinMask | (PinState << PinCount);
  PinCount++;
  
  PinState = (u16)HAL_GPIO_ReadPin(DIN_2_PORT, DIN_2_PIN);
  DinMask = DinMask | (PinState << PinCount);
  PinCount++;
  
  PinState = (u16)HAL_GPIO_ReadPin(DIN_3_PORT, DIN_3_PIN);
  DinMask = DinMask | (PinState << PinCount);
  PinCount++;
  
  PinState = (u16)HAL_GPIO_ReadPin(DIN_4_PORT, DIN_4_PIN);
  DinMask = DinMask | (PinState << PinCount);
  PinCount++;
  
  PinState = (u16)HAL_GPIO_ReadPin(DIN_5_PORT, DIN_5_PIN);
  DinMask = DinMask | (PinState << PinCount);
  PinCount++;
  
  PinState = (u16)HAL_GPIO_ReadPin(DIN_6_PORT, DIN_6_PIN);
  DinMask = DinMask | (PinState << PinCount);
  PinCount++;
  
  PinState = (u16)HAL_GPIO_ReadPin(DIN_HF_PORT, DIN_HF_PIN);
  DinMask = DinMask | (PinState << PinCount);
  PinCount++;
  
  // Обновить маску
  d->DinMask = DinMask;
    
  // Тестовый блок проверки состояни дискретного входа.
  // Читаем данные порта в дебаг переменные.
  //GlobalM4.Debug4 = (u16)HAL_GPIO_ReadPin(DIN_4_PORT, DIN_4_PIN);
  //GlobalM4.Debug5 = (u16)HAL_GPIO_ReadPin(DIN_5_PORT, DIN_5_PIN);
   
  return DinMask;
}

// Установить дискретные выходы
void SetDoutVal(DINDOUT_handle d){
  GPIO_PinState   PinState;
  u16             DoutMask;
  
  DoutMask = d->DoutMask;
  
  // Dout1
  PinState = (GPIO_PinState)(DoutMask & 0x01);
  HAL_GPIO_WritePin(DOUT_1_PORT, DOUT_1_PIN, PinState);
  DoutMask = DoutMask >> 1;
  
  // Dout2
  PinState = (GPIO_PinState)(DoutMask & 0x01);
  HAL_GPIO_WritePin(DOUT_2_PORT, DOUT_2_PIN, PinState);
  DoutMask = DoutMask >> 1;
  
  // Добавляем исключение - если не стоит флаг вывода МТЗ, то выход работает как обычно.
  //if (GlobalM4.FlagCommand.bit.Din7toMTZ == 0) {
      // DoutHF
      PinState = (GPIO_PinState)(DoutMask & 0x01);
      HAL_GPIO_WritePin(DOUT_HF_PORT, DOUT_HF_PIN, PinState);
      DoutMask = DoutMask >> 1;
  //}
}
