#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "DrvFan.h"

TIM_HandleTypeDef htim3;

// Настроить вентиляторы
void InitFan(FAN_handle f){
     GPIO_InitTypeDef GPIO_InitStruct = {0};
     TIM_MasterConfigTypeDef sMasterConfig = {0};
     TIM_OC_InitTypeDef sConfigOC = {0};
     
     // 1) Включить тактирование портов
     __HAL_RCC_GPIOA_CLK_ENABLE();
     __HAL_RCC_GPIOB_CLK_ENABLE();
       
    // 2) Настроить порты для выхода ШИМ управления вентелятора 1
    GPIO_InitStruct.Pin = FAN_1_PIN;                      
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(FAN_1_PORT, &GPIO_InitStruct);

    // 3) Настроить порты для выхода ШИМ управления вентелятора 2
    GPIO_InitStruct.Pin = FAN_2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(FAN_2_PORT, &GPIO_InitStruct);

    // 4) Включить тактирование таймера
    __HAL_RCC_TIM3_CLK_ENABLE();                                  // Включить тактирования таймера
    
    // 5) Настроить таймер для формирования ШИМ
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = (u16)(RCC_APB1/FREQ_TIM1) - 1;         // Пределитель таймера 
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;                  // Режим считать "вверх", от 0 до переполнения
    htim3.Init.Period = (u16)(FREQ_TIM1/f->Fpwm) - 1;             // Переполнение таймера
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Переносить значение в теневой регистр после переполнения
    HAL_TIM_OC_Init(&htim3);                                      // инициализация таймера
    
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;           // Тригерный сигнал будет послан при сбросе счётчика
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;                           // режим работы таймера, ШИМ
    sConfigOC.Pulse = 0;                                          // начальный коэффициент сравнения 
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1); // Конфигрузация канала таймера для формирования ШИМ
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3); // Конфигрузация канала таймера для формирования ШИМ
     
    HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_3);
}

// Обновить задание для вентиляторов
void UpdateFan(FAN_handle f){
   float k, FanTask;
  
   // 1) Расчитать коефициент для уравнения прямой y = k*x + b 
   k = ((FREQ_TIM1/f->Fpwm) - 1.0f) * 0.01f;
  
   // 2) Проверить пределы задания
   if(f->IgbtFanTask > MAX_TASK_FAN){
      f->IgbtFanTask = MAX_TASK_FAN;
   }
   
   // 3) Расчитать задание для вентелятора 1
   FanTask = f->IgbtFanTask * k; 
   
   // 4) Применить новое задание
   htim3.Instance->CCR3 = (u16)(FanTask + 0.5f);
   
   // 5) Проверить приделы задания
   if(f->BoardFanTask > MAX_TASK_FAN){
      f->BoardFanTask = MAX_TASK_FAN;
   }
   
   // 3) Расчитать задание для вентелятора 2
   FanTask = f->BoardFanTask * k; 
   
   // 4) Применить новое задание
   htim3.Instance->CCR1 = (u16)(FanTask + 0.5f);
}
#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)