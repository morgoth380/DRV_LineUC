#include "InputOutput.h"

#ifdef _BOARD_CP24_V2_0

//TIM_HandleTypeDef htim3;
//TIM_HandleTypeDef htim4;
//=============================================================================


//*** Назначения типов аналоговых входов/выходов ***//
void AinAoutInit(void)
{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  
//  __HAL_RCC_GPIOC_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  
//  HAL_GPIO_WritePin(GPIOC, AIN2_0_10V_MK_Pin|AIN1_4_20mA_MK_Pin, GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(GPIOD, AOUT_1_0_10V_MK_Pin|AOUT_2_4_20mA_MK_Pin|AOUT_2_0_10V_MK_Pin|AOUT_1_4_20mA_MK_Pin, GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(GPIOA, AIN1_0_10V_MK_Pin|AIN2_4_20mA_MK_Pin, GPIO_PIN_RESET);
//  
//    /*Configure GPIO pins : PCPin PCPin PCPin */
//  GPIO_InitStruct.Pin = AIN2_0_10V_MK_Pin |AIN1_4_20mA_MK_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//  
//    /*Configure GPIO pins : PDPin PDPin PDPin */
//  GPIO_InitStruct.Pin = AOUT_1_0_10V_MK_Pin|AOUT_2_4_20mA_MK_Pin|AOUT_2_0_10V_MK_Pin|AOUT_1_4_20mA_MK_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//
//    /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
//  GPIO_InitStruct.Pin = AIN1_0_10V_MK_Pin|AIN2_4_20mA_MK_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
//*** Инициализация таймера для формирования ШИМ сигнала для аналоговых выходов ***/
void PWM_Aout_Init(void)
{
//  TIM_MasterConfigTypeDef sMasterConfig = {0};
//  TIM_OC_InitTypeDef sConfigOC = {0};
//  
//  // частота ШИМ = 3 kHz
//  htim3.Instance = TIM3;
//  htim3.Init.Prescaler = 2;                                     // Пределитель 3(90 MHz / 3 = 30 ) 
//  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;                  // Режим считать "вверх", от 0 до переполнения
//  htim3.Init.Period = 9999;                                     // Переполнение 10000 ( 30 MHz/ 10000 = 3kHz ).
//  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Переносить значение в теневой регистр после переполнения
//  
//  __HAL_RCC_TIM3_CLK_ENABLE();                                  // Включить тактирования таймера
//  HAL_TIM_OC_Init(&htim3);                                      // инициализация таймера
//
//  htim4.Instance = TIM4;
//  htim4.Init.Prescaler = 2;                                     // Пределитель 3(90 MHz / 3 = 30 ) 
//  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;                  // Режим считать "вверх", от 0 до переполнения
//  htim4.Init.Period = 9999;                                     // Переполнение 10000 ( 30 MHz/ 10000 = 3kHz ).
//  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Переносить значение в теневой регистр после переполнения
//
//  __HAL_RCC_TIM4_CLK_ENABLE();                                  // Включить тактирования таймера
//  HAL_TIM_OC_Init(&htim4);                                      // инициализация таймера
//  
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;           // Тригерный сигнал будет послан при сбросе счётчика
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  
//  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
//  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);
//
//  sConfigOC.OCMode = TIM_OCMODE_PWM1;                           // режим работы таймера, ШИМ
//  sConfigOC.Pulse = 4999;                                       // начальный коэффициент сравнения 
//  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//  
//  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4); 
//  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1); // Конфигрузация канала таймера для формирования ШИМ
// 
//  Timer_Port_Init();
//  
//  HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_4);                      // Старт таймера
//  HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_1);
}
//*** Инициализация портов для вывода таймера ***//
void Timer_Port_Init(void)
{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  
//  __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  
//  //TIM3 GPIO Configuration    
//  //PB1     ------> TIM3_CH4
//  //PD12    ------> TIM4_CH1 
//   
//  GPIO_InitStruct.Pin = AOUT_2_PWM_MK_Pin;                      
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
//  HAL_GPIO_Init(AOUT_2_PWM_MK_GPIO_Port, &GPIO_InitStruct);
//
//  GPIO_InitStruct.Pin = AOUT_1_PWM_MK_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
//  HAL_GPIO_Init(AOUT_1_PWM_MK_GPIO_Port, &GPIO_InitStruct);
}
//***  Установка скважности ШИМ аналоговых выходов ***//
void SetValuePWM_Aout(s16 RefPWM, s16 numAout)
{
//  if(numAout != 0)
//      htim3.Instance->CCR4 = RefPWM; 
//  else
//      htim4.Instance->CCR1 = RefPWM;
}

#else   // _BOARD_CP24_V2_1
    
    TIM_HandleTypeDef htim4;

//*** Назначения типов аналоговых входов/выходов ***//
void AinAoutInit(void)
{
      GPIO_InitTypeDef GPIO_InitStruct = {0};
      
      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOD_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();
      
      HAL_GPIO_WritePin(GPIOC, AIN2_0_10V_MK_Pin|AIN1_4_20mA_MK_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOD, AOUT_1_0_10V_MK_Pin|AOUT_2_4_20mA_MK_Pin|AOUT_2_0_10V_MK_Pin|AOUT_1_4_20mA_MK_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, AIN1_0_10V_MK_Pin|AIN2_4_20mA_MK_Pin, GPIO_PIN_RESET);
      
        /*Configure GPIO pins : PCPin PCPin PCPin */
      GPIO_InitStruct.Pin = AIN2_0_10V_MK_Pin |AIN1_4_20mA_MK_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
      
        /*Configure GPIO pins : PDPin PDPin PDPin */
      GPIO_InitStruct.Pin = AOUT_1_0_10V_MK_Pin|AOUT_2_4_20mA_MK_Pin|AOUT_2_0_10V_MK_Pin|AOUT_1_4_20mA_MK_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
      GPIO_InitStruct.Pin = AIN1_0_10V_MK_Pin|AIN2_4_20mA_MK_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

//*** Инициализация таймера для формирования ШИМ сигнала для аналоговых выходов ***/
void PWM_Aout_Init(void)
{
      TIM_MasterConfigTypeDef sMasterConfig = {0};
      TIM_OC_InitTypeDef sConfigOC = {0};
      
      // частота ШИМ = 3 kHz
      htim4.Instance = TIM4;
      htim4.Init.Prescaler = 0;                                     // Пределитель 3(90 MHz / 3 = 30 ) 
      htim4.Init.CounterMode = TIM_COUNTERMODE_UP;                  // Режим считать "вверх", от 0 до переполнения
      htim4.Init.Period = 9999;                                     // Переполнение 10000 ( 30 MHz/ 10000 = 3kHz ).
      htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
      htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Переносить значение в теневой регистр после переполнения

      __HAL_RCC_TIM4_CLK_ENABLE();                                  // Включить тактирования таймера
      HAL_TIM_OC_Init(&htim4);                                      // инициализация таймера
      
      sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;           // Тригерный сигнал будет послан при сбросе счётчика
      sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
            
      HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

      sConfigOC.OCMode = TIM_OCMODE_PWM1;                           // режим работы таймера, ШИМ
      sConfigOC.Pulse = 4999;                                       // начальный коэффициент сравнения 
      sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
      sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
      
      HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1); // Конфигрузация канала таймера для формирования ШИМ
      HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2); // Конфигрузация канала таймера для формирования ШИМ
     
      Timer_Port_Init();
      
      HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_1);
      HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_2);
}

//*** Инициализация портов для вывода таймера ***//
void Timer_Port_Init(void)
{
      GPIO_InitTypeDef GPIO_InitStruct = {0};
      
      __HAL_RCC_GPIOD_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      
      //TIM3 GPIO Configuration    
      //PD12 ------> TIM4_CH1
      //PD13 ------> TIM4_CH2
       
      GPIO_InitStruct.Pin = AOUT_2_PWM_MK_Pin;                      
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
      HAL_GPIO_Init(AOUT_2_PWM_MK_GPIO_Port, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = AOUT_1_PWM_MK_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
      HAL_GPIO_Init(AOUT_1_PWM_MK_GPIO_Port, &GPIO_InitStruct);
    }

//***  Установка скважности ШИМ аналоговых выходов ***//
void SetValuePWM_Aout(s16 RefPWM, s16 numAout){
  if(numAout != 0){
      htim4.Instance->CCR1 = RefPWM; 
  }
  else{
      htim4.Instance->CCR2 = RefPWM;
  }
}
#endif  //_BOARD_CP24_V2_1