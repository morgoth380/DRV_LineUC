#include "DrvPWM.h"
#include "measTime.h"
#include "GlobalVar.h"
#include "DrvADC.h"
#include "DrvErrSWandSTO.h"

TIM_HandleTypeDef htim1;    // Указатель на таймер
extern ADCDRV Adc;          // Обект настройки модуля АЦП
extern SWANDSTO SwAndSto;   // Обект управления авариями ключей и STO

void GpioPwmInit(void);                         // Настройка портов
void TimerPWM_Init(u16 Tim1Max, u8 DeadTime);   // Настройка таймера T1 для формирования ШИМ
u16 FpwmCalc(float NewVal);                     // Пересчитать частоту ШИМ в максимальное значение таймера
u8 DeadTimеCalc(float NewVal);                  // Пересчитать DeadTimе в микросекундах в значение регистра таймера
void PwmOff(PWMGEN *p);                         // Выключить ШИМ

// Настроить ШИМ
void PwmInit(PWMGEN *p){
  u8 DeadTimeReg;
  
  // 1) Настроить Таймер 
  p->MaxCountT1 = FpwmCalc(p->Fpwm_kHz);        // Расчитать значение до которого считать таймеру для получения необходимой частоты ШИМ
  DeadTimeReg = DeadTimеCalc(p->DeadTime_uS);   // Расчитать значение регистра таймера для обеспичения необходимого DeadTime; 
  
  TimerPWM_Init(p->MaxCountT1, DeadTimeReg);
  
  // 2) Настроить порты
  GpioPwmInit();
  
  // 3) Запустить таймер
  PwmOff(p);                                    // СТОП - на всякий случай
  HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_4);   // Запуск 4-го канала для формирования прерывания
  
  // 4) Установить статус модуля ШИМ
  p->StatusPwm = PwmOff_Enum;
}


// Настройка таймера T1 для формирования ШИМ
void TimerPWM_Init(u16 Tim1Max, u8 DeadTime){
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  // 1) Включить тактирование таймера
  __HAL_RCC_TIM1_CLK_ENABLE();

  // 2) Настроить таймер  
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;                                         // Предделитель 1
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period = Tim1Max;                                      // Период таймера
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                // Внутренний предделитель
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;     // Включить буферизацию регистров таймера
  HAL_TIM_Base_Init(&htim1);
  
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;        // Тактирование таймера от внутренней шины
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
  
  HAL_TIM_PWM_Init(&htim1);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
  
  // Настройка 1-го канала ШИМ
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = Tim1Max/2;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
  
  // Настройка 2-го канала ШИМ
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = Tim1Max/2;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
  
  // Настройка 3-го канала ШИМ
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = Tim1Max/2;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3);
  
  // Настройка 4-го канала для формирования прерывания (такта ШИМ) при переходе счетчика через "0"
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);
  
  // Настроить режим Break и DeadTime 
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF; //TIM_LOCKLEVEL_1;
  sBreakDeadTimeConfig.DeadTime = DeadTime;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);
  
  // Разрешить прерывание в такте ШИМ
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
}

// Настройка портов для формирования ШИМ
void GpioPwmInit(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Включить тактирование порта 
  __HAL_RCC_GPIOE_CLK_ENABLE();
  /**TIM1 GPIO Configuration    
  BOT_U_PIN  ------> TIM1_CH1N
  TOP_U_PIN  ------> TIM1_CH1
  BOT_V_PIN ------> TIM1_CH2N
  TOP_V_PIN ------> TIM1_CH2
  BOT_W_PIN ------> TIM1_CH3N
  TOP_W_PIN ------> TIM1_CH3 
  */
  
  // 2) Настроить ноги на формирование ШИМ от таймера 1
  GPIO_InitStruct.Pin = BOT_U_PIN | TOP_U_PIN | BOT_V_PIN | TOP_V_PIN | BOT_W_PIN | TOP_W_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(PWM_PORT, &GPIO_InitStruct);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3) Настроить ногу для управления драйвером ШИМ (вкл/выкл), только для блока СР24 2.1
  #ifdef _BOARD_CP24_V2_1
    HAL_GPIO_WritePin(PWM_EN_PORT, PWM_EN_PIN, GPIO_PIN_SET);   // Сразу драйвер отключим (инверсный сигнал)
  
    GPIO_InitStruct.Pin = PWM_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PWM_EN_PORT, &GPIO_InitStruct);
  #endif  //_BOARD_CP24_V2_1
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
}

// Пересчитать частоту ШИМ в максимальное значение таймера
u16 FpwmCalc(float NewVal){
  u32 TimerMax;
      
#warning  НАДО ЗАМЕНИТЬ FREQ_APB2 НА SystemCoreClock, ЧТОБЫ ВСЕ БЫЛО КРАСИВО
  // Расчитаем максимальное значение таймера
  TimerMax = (u32)(SystemCoreClock / (NewVal * 1000.0 * 2.0f)); // SystemCoreClock - FREQ_APB2
  
  // Проверим прeделы
  if(TimerMax > TIMER1_MAX_COUNT){
    TimerMax = TIMER1_MAX_COUNT;
  }
  
  return (u16)TimerMax;
}

// Пересчитать DeadTimе в микросекундах в значение регистра таймера
u8 DeadTimеCalc(float NewVal){
  u32 DeadTimеReg = 0;
  float OneTik;
  
  // 1) Расчитать длительность одного такта таймера в мкс
  //OneTik = 1000.0f / FREQ_APB2;
  OneTik = 1000000.0f / SystemCoreClock;
  
  // 2) Расчитать значение с учетом изломов характеристики
  if(NewVal < DEADTIME_DOT1){
      DeadTimеReg = (u32)((NewVal/OneTik) + 0.5f);
  }
  else if((NewVal < (DEADTIME_DOT1 * 2.0f))&&(NewVal >= DEADTIME_DOT1)){
      DeadTimеReg = (u32)(((NewVal / (2.0f * OneTik)) - 64.0f) + 0.5f);
      DeadTimеReg = (DeadTimеReg & 255) | 128;
  }
  else if((NewVal < (DEADTIME_DOT1 * 4.0f))&&(NewVal >= (DEADTIME_DOT1 * 2.0f))){
      DeadTimеReg = (u32)(((NewVal / (8.0f * OneTik)) - 32.0f) + 0.5f);
      DeadTimеReg = (DeadTimеReg & 255) | 192;
  }
  else if((NewVal < (DEADTIME_DOT1 * 8.0f))&&(NewVal >= (DEADTIME_DOT1 * 4.0f))){
      DeadTimеReg = (u32)(((NewVal / (16.0f * OneTik)) - 32.0f) + 0.5f);
      DeadTimеReg = (DeadTimеReg & 255) | 224;
  }
  else{
      DeadTimеReg = 255;   // Максимально большой DeadTime
  }
  
  // 3) Проверим пределы
  if(DeadTimеReg > TIMER1_MAX_COUNT){
    DeadTimеReg = TIMER1_MAX_COUNT;
  }
  
  return (u16)DeadTimеReg;
}

//*****************************************************************************
#define MIN_PULSE   3.0f   // Минимальная длительность импульса в мкс

// Обновить задание ШИМ
void PwmUpdate(PWMGEN *p){
  float A, B, C;
  u16         MinImp, Val;// DeadTimeImp, MinImp2;
  
  // Расчитать задание для таймера которое соответствует требуемой минимальной длительности
  MinImp = 2 * (u16)((p->DeadTime_uS * (float)p->MaxCountT1 * p->Fpwm_kHz * 0.001f) + 0.5f); //  Дедтайм в тиках//(u16)((MIN_PULSE * (float)p->MaxCountT1 * p->Fpwm_kHz * 0.001f) + 0.5f);          // 
  //DeadTimeImp = (u16)((p->DeadTime_uS * (float)p->MaxCountT1 * p->Fpwm_kHz * 0.001f) + 0.5f); //  Дедтайм в тиках
  
  //MinImp = MinImp + 2*DeadTimeImp;
  // Получить задание для Фазы U
  if (p->Ta > 1.0f) p->Ta = 1.0f;
  else if (p->Ta < -1.0f) p->Ta = -1.0f;
  
  A = 1.0f - p->Ta;
  Val = (u16) ((A * (float)(p->MaxCountT1 - 1)) / 2.0f);   // Задание в тиках таймера
  
  // Защита от коротких импульсов
  if(Val < (MinImp/2) ){
     Val = 0;
  } else if ( (Val > (MinImp/2)) && (Val < MinImp) ) {
    Val = MinImp;
  }else if(Val > (p->MaxCountT1 - MinImp/2)){
     Val = p->MaxCountT1;
  } else if ( (Val < (p->MaxCountT1 - MinImp/2)) && (Val > (p->MaxCountT1 - MinImp))) {
    Val = p->MaxCountT1 - MinImp;
  }
  
  htim1.Instance->CCR1 = Val;
  
  // Получить жадание для Фазы V
  if (p->Tb > 1.0f) p->Tb = 1.0f;
  else if (p->Tb < -1.0f) p->Tb = -1.0f;

  B = 1.0f - p->Tb;
  Val = (u16) ((B * (float)(p->MaxCountT1 - 1)) / 2.0f);   // Задание в тиках таймера
 
  // Защита от коротких импульсов
  if(Val < (MinImp/2) ){
     Val = 0;
  } else if ( (Val > (MinImp/2)) && (Val < MinImp) ) {
    Val = MinImp;
  }else if(Val > (p->MaxCountT1 - MinImp/2)){
     Val = p->MaxCountT1;
  } else if ( (Val < (p->MaxCountT1 - MinImp/2)) && (Val > (p->MaxCountT1 - MinImp))) {
    Val = p->MaxCountT1 - MinImp;
  }
  
  htim1.Instance->CCR2 = Val;
  
  // Получить жадание для Фазы W
  if (p->Tc > 1.0f) p->Tc = 1.0f;
  else if (p->Tc < -1.0f) p->Tc = -1.0f;
  
  C = 1.0f - p->Tc;
  Val = (u16) ((C * (float)(p->MaxCountT1 - 1)) / 2.0f);   // Задание в тиках таймера
    
  // Защита от коротких импульсов
  if(Val < (MinImp/2) ){
     Val = 0;
  } else if ( (Val > (MinImp/2)) && (Val < MinImp) ) {
    Val = MinImp;
  }else if(Val > (p->MaxCountT1 - MinImp/2)){
     Val = p->MaxCountT1;
  } else if ( (Val < (p->MaxCountT1 - MinImp/2)) && (Val > (p->MaxCountT1 - MinImp))) {
    Val = p->MaxCountT1 - MinImp;
  }
  
  htim1.Instance->CCR3 = Val;
  
  // Сохраним в public переменные значение задание таймеров
  p->UvalTimer = htim1.Instance->CCR1;            // Значение задание для таймера по фазе U (Read only)
  p->VvalTimer = htim1.Instance->CCR2;            // Значение задание для таймера по фазе V (Read only)
  p->WvalTimer = htim1.Instance->CCR3;            // Значение задание для таймера по фазе W (Read only)  
}

// Перенастроить модуль ШИМ в работе
void InitVarPwm(PWMGEN *p) {  
  u8 DeadTimeReg;
  uint32_t tmpbdtr = 0U;
  
  p->MaxCountT1 = FpwmCalc(p->Fpwm_kHz);              // Расчитать значение до которого считать таймеру для получения необходимой частоты ШИМ
  DeadTimeReg = DeadTimеCalc(p->DeadTime_uS);         // Расчитать значение регистра таймера для обеспeчения необходимого DeadTime; 
             
  /* Set the Autoreload value */
  TIM1->ARR = (uint32_t)p->MaxCountT1;
  MODIFY_REG(tmpbdtr, TIM_BDTR_DTG, DeadTimeReg);  
}

// Перенастроить модуль ШИМ
void PwmReInit(PWMGEN *p){
  u8 DeadTimeReg;
    
  p->MaxCountT1 = FpwmCalc(p->Fpwm_kHz);              // Расчитать значение до которого считать таймеру для получения необходимой частоты ШИМ
  DeadTimeReg = DeadTimеCalc(p->DeadTime_uS);         // Расчитать значение регистра таймера для обеспeчения необходимого DeadTime; 
  
  
  PwmOff(p);                                          // Остановить ШИМ если он был включен
  HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_4);          // Остановить 4-й канал для формирования прерывания
  
  HAL_TIM_Base_DeInit(&htim1);
  
  TimerPWM_Init(p->MaxCountT1, DeadTimeReg);          // Установить новые настройки ШИМ 
 
  HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_4);         // Запуск 4-го канала для формирования прерывания
  
#warning 05-12-2021 Добавил включение обработки ключей сразу. И снова пришлось убрать - т.к. аппаратно нет на схеме подтяжки к нулю.
  // Включить обработку аварии ключей и STO
  //SwAndSto.onSW(&SwAndSto);
  
}

// Включить ШИМ
void PwmOn(PWMGEN *p){
    // 1) Проверим перед пуском может модуль ШИМ не настроен
    if(p->StatusPwm != PwmOff_Enum) return;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // При работе с СР24В здесь включается контроль МТЗ и обработка аварии ключей и STO
    #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    // 2) Включить компаратор (обработка МТЗ комп.)
    Adc.CompStart(&Adc);
    
    // 3) Включить обработку аварии ключей и STO
    SwAndSto.onSW(&SwAndSto);
    SwAndSto.onSTO(&SwAndSto);    
    #endif
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // 4) Включить ШИМ
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_1);
    
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_2);
    
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_3);
   
    // 5) Включить драйвер ШИМ
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    #ifdef _BOARD_CP24_V2_1
      HAL_GPIO_WritePin(PWM_EN_PORT, PWM_EN_PIN, GPIO_PIN_RESET);   // Включить драйвер ШИМ
    #endif  //_BOARD_CP24_V2_1
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // 6) Установить статус модуля ШИМ
    p->StatusPwm = PwmOn_Enum;
    p->PwmOn = 1;
}

// Выключить ШИМ
void PwmOff(PWMGEN *p){

  // 1) Отключить ШИМ
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_OCN_Stop(&htim1, TIM_CHANNEL_1);
  
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_OCN_Stop(&htim1, TIM_CHANNEL_2);
  
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
  HAL_TIMEx_OCN_Stop(&htim1, TIM_CHANNEL_3);
  
  // 2) Выключить драйвер ШИМ
  #ifdef _BOARD_CP24_V2_1
    HAL_GPIO_WritePin(PWM_EN_PORT, PWM_EN_PIN, GPIO_PIN_SET);   // Выключить драйвер ШИМ
  #endif  //_BOARD_CP24_V2_1
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // При работе с СР24В здесь выключается контроль МТЗ и обработка аварии ключей и STO
  #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)    
  // 3) Отключить компаратор (МТЗ комп)
  Adc.CompStop(&Adc);
    
  // 4) Выключить обработку аварии ключей и STO
#warning 05-12-2021 Заблокировано отключение обработки ключей. Контроль даже в стопе.
  SwAndSto.offSW(&SwAndSto);
  SwAndSto.offSTO(&SwAndSto);
  #endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
    
  // 5) Установить статус модуля ШИМ
  p->StatusPwm = PwmOff_Enum;  
  p->PwmOn = 0;
}

// Накачка питания драйверов верхних ключей
void FeedTopKey(void){
  static StatusBotKey_Enum NextStateKey;
  
  switch(NextStateKey){
     case OnBotKeyU_Enum:{           
       NextStateKey = OnBotKeyV_Enum;
       
       HAL_GPIO_WritePin(PWM_PORT, BOT_W_PIN, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(PWM_PORT, BOT_V_PIN, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(PWM_PORT, BOT_U_PIN, GPIO_PIN_SET);
     } break;                 
     case OnBotKeyV_Enum:{          
       NextStateKey = OnBotKeyW_Enum;
       
       HAL_GPIO_WritePin(PWM_PORT, BOT_W_PIN, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(PWM_PORT, BOT_U_PIN, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(PWM_PORT, BOT_V_PIN, GPIO_PIN_SET);
     } break;    
     case OnBotKeyW_Enum:{          
       NextStateKey = OnBotKeyU_Enum;
       
       HAL_GPIO_WritePin(PWM_PORT, BOT_V_PIN, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(PWM_PORT, BOT_U_PIN, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(PWM_PORT, BOT_W_PIN, GPIO_PIN_SET);
     } break; 
  }
}

// Настроить порт для прямого управления нижними ключами (для накачки)
void InitPortBotIGBT(void){
   GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Включить тактирование порта 
  __HAL_RCC_GPIOE_CLK_ENABLE();
    
  // 2) Настроить ноги на формирование ШИМ от таймера 1
  GPIO_InitStruct.Pin = BOT_U_PIN | BOT_V_PIN | BOT_W_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(PWM_PORT, &GPIO_InitStruct);
}

// Закрыть нижние ключи
void CloseBotKey(void){
   HAL_GPIO_WritePin(PWM_PORT, BOT_W_PIN, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(PWM_PORT, BOT_V_PIN, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(PWM_PORT, BOT_U_PIN, GPIO_PIN_RESET);
}

// Настроить модуль ШИМ для накачки
void InitFeed(PWMGEN *p){
  CloseBotKey();       // Закрыть нижние ключи
  InitPortBotIGBT();   // Настроить порт для прямого управления нижними ключами (для накачки)
  
  #ifdef _BOARD_CP24_V2_1
    HAL_GPIO_WritePin(PWM_EN_PORT, PWM_EN_PIN, GPIO_PIN_RESET);   // Выключить драйвер ШИМ
  #endif  //_BOARD_CP24_V2_1
  
  // Установить текущий статус модуля ШИМ
  p->StatusPwm = PwmPumping_Enum;  
}

// Остановить накачку и настроить модуль ШИМ для штатной работы
void DeInitFeed(PWMGEN *p){
  // 1) Настроить порты для формирования ШИМ
  GpioPwmInit();
  
  // 2) Выключить драйвер ШИМ
  #ifdef _BOARD_CP24_V2_1
    HAL_GPIO_WritePin(PWM_EN_PORT, PWM_EN_PIN, GPIO_PIN_SET);   // Выключить драйвер ШИМ
  #endif  //_BOARD_CP24_V2_1
    
  // 3) Установить текущий статус модуля ШИМ
  p->StatusPwm = PwmOff_Enum;
}

