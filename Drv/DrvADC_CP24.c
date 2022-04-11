#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "DrvADC.h"
#include "measTime.h"
#include "GlobalVar.h"

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc3;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim8;

volatile u16 Adc3Buff[SIZE_BUF_UVWUD];        // Буффер для накопления измерений токов и Ud
   

// Прототипы privat функций
void GPIOInit(void);                       // Настроить порт
void DMAInit(void);                        // Настроить DMA
void ADC3_Init(void);                      // Настройка АЦП3 - для измирения токов и ЗПТ

void TIM2_Init(float Fpwm);                // Настройка таймера для запуска АЦП3

void ADC2_Init(void);                      // Настройка АЦП2 (аппаратная защита МТЗ)
void CompAdc2Init(s16 MaxVal, s16 MinVal); // Настройка компаратора ADC2

void ADC1_Init(void);                      // Настройка ADC1
void Adc1DMA_Init(void);                   // Настройка DMA для ADC1 

void TIM8_Init(void);                      // Настройка таймера для запуска АЦП1


// Настроить АЦП + запуск
void AdcInit(ADCDRV *p){
  // 1) Настроить АЦП3 для измирения токов и ЗПТ и складывания данных в буффер (для усреднения)  
  // TIMER->ADC->DMA->буффер
  GPIOInit();                 // Настроить порт
  DMAInit();                  // Настроить DMA  
  ADC3_Init();                // Настройка АЦП3
  
  TIM2_Init(p->Fpwm_kHz);     // Настройка таймера для запуска АЦП3
  
  HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&Adc3Buff, SIZE_BUF_UVWUD); // Запустить АЦП с DMA 
   
  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_2);                         // Запустить таймер (для запусков АЦП) 

  // 2) Настроить АЦП2 для аппаратной защиты МТЗ
  // Режим непрерывного преобразование токовых каналов с максимальной скоростью
  ADC2_Init();                           // Настройка АЦП2 (аппаратная защита МТЗ)
  CompAdc2Init(p->CompMax, p->CompMin);  // Настройка компаратора ADC2
  HAL_ADC_Start(&hadc2); // !!! Запуск АЦП для отработки МТЗ, но старт без прерываний, чтобы просто начал молотить
  //HAL_ADC_Start_IT(&hadc2);  // Запустить АЦП2
  
  // 3) Настроить АЦП1 для измирения медленно меняющихся сигналов
  
  Adc1DMA_Init();            // Настроить DMA для ADC1
  ADC1_Init();               // Настройка ADC1
  TIM8_Init();               // Настройка таймера для запуска АЦП1
  
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&GlobalM4.LowSpeedAinAdc, sizeof(GlobalM4.LowSpeedAinAdc)/2); // Запустить АЦП1  - size (LowSpeedAinAdc_type)
  
  HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_1);                           // Запустить таймер (для запусков АЦП)  
}

// Настроить порт
void GPIOInit(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Включить тактирование порта с ножками Ain  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();  // Ain пользователя
  
  // 2) Настроить ноги для каналов измирения токов и ЗПТ
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  // 3) Настроить ноги для аналоговых входов 1 и 2
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}   

// Настроить DMA
void DMAInit(void){
  
  // 1) DMA для токов и ЗПТ 
  __HAL_RCC_DMA2_CLK_ENABLE();                             // Включить тактирование DMA 

  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 14, 0);           // Установить приоритет прерывания
  //HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);                   // Включить прерывание по заполнению буффера
  
  hdma_adc3.Instance = DMA2_Stream0;
  hdma_adc3.Init.Channel = DMA_CHANNEL_2;
  hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc3.Init.Mode = DMA_CIRCULAR;
  hdma_adc3.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_adc3);
  
  __HAL_LINKDMA(&hadc3,DMA_Handle,hdma_adc3);

  /* ADC3 interrupt Init */
  HAL_NVIC_SetPriority(ADC_IRQn, 1, 0);                   // У АЦП наивысший приоритет
  HAL_NVIC_EnableIRQ(ADC_IRQn);
}

// Настройка АЦП3 - для измирения токов и ЗПТ
void ADC3_Init(void){
  // 1) Включить тактирование
  __HAL_RCC_ADC3_CLK_ENABLE();
 
  ADC_ChannelConfTypeDef sConfig = {0};
  
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ENABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 4;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  HAL_ADC_Init(&hadc3); // Применить настройки
  
  // Настройка каналов АЦП
  sConfig.Channel = MEAS_CHANNEL_IU;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc3, &sConfig);         // Применить настройки для 0-го канала АЦП
  
  sConfig.Channel = MEAS_CHANNEL_IV;
  sConfig.Rank = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc3, &sConfig);        // Применить настройки для 1-го канала АЦП
  
  sConfig.Channel = MEAS_CHANNEL_IW;
  sConfig.Rank = 3;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc3, &sConfig);        // Применить настройки для 2-го канала АЦП
  
  sConfig.Channel = MEAS_CHANNEL_UD;
  sConfig.Rank = 4;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc3, &sConfig);        // Применить настройки для 3-го канала АЦП
}

// Расчитать значение прескаллера таймера для заполнения буффера данными строго за период ШИМ
  
int CalcPrescalerT2(float Fpwm){
  return (int)((FREQ_TIM2/((float)COUNT_SEMPL * Fpwm)) + 0.5f);  
}

void ReInitVarPwm(ADCDRV *p) {
  int Tim2Prescaler;
   
  // Расчитать значение верхнего уровня счета
  Tim2Prescaler = CalcPrescalerT2(p->Fpwm_kHz);  
  TIM2->ARR = (uint32_t)Tim2Prescaler;
}

// Настройка таймера для запуска АЦП1 и  АЦП3
void TIM2_Init(float Fpwm){
  int Tim2Prescaler;
  
  // 1) Включить тактирование

  __HAL_RCC_TIM2_CLK_ENABLE();
  
  // 2) Расчитать значение прескаллера таймера

  Tim2Prescaler = CalcPrescalerT2(Fpwm);
  
  // 3) Настройка прерывания от таймера
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};


  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;                            // 1
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = (u16)(2.0f*Tim2Prescaler);
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim2);
  
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
  

  HAL_TIM_OC_Init(&htim2);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
  
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
}

// Настройка АЦП2 (аппаратная защита МТЗ)
void ADC2_Init(void){
  ADC_ChannelConfTypeDef sConfig = {0};

  // Включить тактирование АЦП2
  __HAL_RCC_ADC2_CLK_ENABLE();
  
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_8B;
  hadc2.Init.ScanConvMode = ENABLE;                                   // Сканировать все каналы по очереди
  hadc2.Init.ContinuousConvMode = ENABLE;                             // Циклический режим работы АЦП
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 3;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  HAL_ADC_Init(&hadc2);
  
  // Канал измирения I_U
  sConfig.Channel = MEAS_CHANNEL_IU;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);
  
  // Канал измирения I_V
  sConfig.Channel = MEAS_CHANNEL_IV;
  sConfig.Rank = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);
  
  // Канал измирения I_W
  sConfig.Channel = MEAS_CHANNEL_IW;
  sConfig.Rank = 3;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);
}

// Настройка компаратора ADC2
void CompAdc2Init(s16 MaxVal, s16 MinVal){
  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_ALL_REG;
  AnalogWDGConfig.HighThreshold = MaxVal;                       // Верхний предел
  AnalogWDGConfig.LowThreshold = MinVal;                        // Нижний предел 
  AnalogWDGConfig.ITMode = ENABLE;                              // Разрешить прерывания
  HAL_ADC_AnalogWDGConfig(&hadc2, &AnalogWDGConfig);
} 

// Настройка ADC1
void ADC1_Init(void){
  
  // Включить тактирование АЦП1
  __HAL_RCC_ADC1_CLK_ENABLE();

  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_CC1;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = sizeof(GlobalM4.LowSpeedAinAdc)/2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  HAL_ADC_Init(&hadc1);
  
  // Температура ключей - Канал U (виртуальный), реально это один сигнал температуры
  sConfig.Channel = MEAS_CHANNEL_TEMP;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  // Температура ключей - Канал V (виртуальный), реально это один сигнал температуры
  sConfig.Channel = MEAS_CHANNEL_TEMP;
  sConfig.Rank = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  // Температура ключей - Канал W (виртуальный), реально это один сигнал температуры
  sConfig.Channel = MEAS_CHANNEL_TEMP;
  sConfig.Rank = 3;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    // Температура Блока - (сюда бы подключить внутренний термдатчик)
  sConfig.Channel = MEAS_CHANNEL_TEMP;
  sConfig.Rank = 4;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  // Аналоговый вход 2
  sConfig.Channel = MEAS_CHANNEL_AIN2;
  sConfig.Rank = 5;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  // Аналоговый вход 1
  sConfig.Channel = MEAS_CHANNEL_AIN1;
  sConfig.Rank = 6;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  // Опорное напряжение АЦП
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = 7;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  // Напряжение батарейки
  sConfig.Channel = /*ADC_CHANNEL_VBAT*/ ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 8;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

// Настроить DMA для ADC1
void Adc1DMA_Init(void){

  // 1) Включить тактирование DMA
  __HAL_RCC_DMA2_CLK_ENABLE();

  // 2) Настроить прерывание от DMA
  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 13, 0);
  //HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

  // 3) Настроить DMA
  hdma_adc1.Instance = DMA2_Stream4;
  hdma_adc1.Init.Channel = DMA_CHANNEL_0;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1.Init.Mode = DMA_CIRCULAR;
  hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
  hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_adc1);                                       // Применить настройки
  
  __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);
}

// Настройка таймера для запуска АЦП1

void TIM8_Init(){
  // 1) Включить тактирование

  __HAL_RCC_TIM8_CLK_ENABLE();
  
  // 2) Настройка таймера
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};


  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 9;                                        // 1
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 16200;                                       // Период 3.6мс
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim8);
  
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

  HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig);
  

  HAL_TIM_OC_Init(&htim8);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig);
  
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  
  HAL_TIM_OC_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1);
}

// Перенастроить АЦП (синхронизировать с ШИМ)
void AdcReInit(ADCDRV *p){

  int Tim2Prescaler;
  
  // 1) Расчитать значение прескаллера таймера

  Tim2Prescaler = CalcPrescalerT2(p->Fpwm_kHz);
  
  // 2) Обновить значение в таймера 

  htim2.Init.Period = Tim2Prescaler;
  HAL_TIM_Base_Init(&htim2);
  
}

// Перенастроить компаратор (установить новые пределы МТЗ)
void CompReInit(ADCDRV *p){
  
  // 1) Проверить полученные значения на выход за приделы
  if(p->CompMax > ADC_MAX_MTZ){
    p->CompMax = ADC_MAX_MTZ;
  }
  if(p->CompMin < ADC_MIN_MTZ){
    p->CompMin = ADC_MIN_MTZ;
  }
  
  // Настроить компаратор ADC2
  CompAdc2Init(p->CompMax, p->CompMin);  
}

// Разрешить прерывание от компаратора
void CompStart(ADCDRV *p){
    
   if(!p->CompStatus){ 
     //HAL_ADC_Start_IT(&hadc2);  // Запустить АЦП2
     /* Enable end of conversion interrupt for regular group */              
     __HAL_ADC_CLEAR_FLAG(&hadc2, ADC_FLAG_AWD);
     __HAL_ADC_ENABLE_IT(&hadc2, ADC_IT_AWD);
      p->CompStatus = 1;
   }
}

// Запретить прерывание от компаратора
void CompStop(ADCDRV *p){
  if(p->CompStatus){
//    HAL_ADC_Stop_IT(&hadc2);    // Остановить АЦП2    
    /* Disable ADC end of conversion interrupt for regular group */
    __HAL_ADC_DISABLE_IT(&hadc2, ADC_IT_AWD); // /*ADC_IT_EOC | ADC_IT_OVR*/    
    CompAdc2Init(255, 0); // В останове, сделать максимальный размах для компаратора МТЗ, иначе при первом такте он сработает
     p->CompStatus = 0;
  } 
}

//// Разрешить прерывание от компаратора
//void CompStart(ADCDRV *p){
//   u16 tmp1;
//   if(!p->CompStatus){ 
//     
//     tmp1 = __HAL_ADC_GET_FLAG(&hadc2, ADC_FLAG_AWD);
//     //HAL_ADC_Start_IT(&hadc2);  // Запустить АЦП2
//     //__HAL_ADC_CLEAR_FLAG(&hadc2, ADC_FLAG_EOC | ADC_FLAG_OVR);
//     __HAL_ADC_CLEAR_FLAG(&hadc2, ADC_FLAG_AWD);
//     /* Enable end of conversion interrupt for regular group */
//     __HAL_ADC_ENABLE_IT(&hadc2, (ADC_IT_AWD /*ADC_IT_EOC | ADC_IT_OVR*/));     
//     p->CompStatus = 1;
//   }
//}
//
//// Запретить прерывание от компаратора
//void CompStop(ADCDRV *p){
//  if(p->CompStatus){
//    //HAL_ADC_Stop_IT(&hadc2);    // Остановить АЦП2
//    /* Disable ADC end of conversion interrupt for regular group */
//    __HAL_ADC_DISABLE_IT(&hadc2, (ADC_IT_AWD | ADC_IT_EOC | ADC_IT_OVR));     
//    __HAL_ADC_CLEAR_FLAG(&hadc2, ADC_FLAG_AWD);
//    p->CompStatus = 0;
//  }
//}
#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)