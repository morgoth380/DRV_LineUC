#include "Global_include.h"
//#include "measTime.h"
#include "main.h"
#include "GlobalVar.h"

extern TIM_HandleTypeDef timMeasTime;     // TIM2 Таймер для измирения времени выполнения 

// Инициализировать подсистему измерения времени выполнения
#define TIM5_clk        10000000 // Частота работы таймера должна быть 10 Мгц, для получения 0,1 мкс
void InitMeasTime(void){
  int i=0;
  u32   pclk1, apb1;
  
  // 1) Включить тактирование таймера
  __HAL_RCC_TIM5_CLK_ENABLE();
  //__HAL_RCC_TIM2_CLK_ENABLE();
  
  // 2) Настроить таймер на дикретность 0,1 мкс
  // Получить таковую частоту для таймера
  pclk1 = HAL_RCC_GetPCLK1Freq();
  apb1 = pclk1 << 1; // Шина APB1 работает на частоте в 2 раза выше, чем PCLK1
  
  // Тактирование таймера осуществляется от шины ABP1 с частотой 90Мгц  
  timMeasTime.Instance = TIM5;//TIM2;
  timMeasTime.Init.Prescaler = (u16)(apb1/TIM5_clk) - 1;//8;                                     // Предделитель на 9
  timMeasTime.Init.CounterMode = TIM_COUNTERMODE_UP;
  timMeasTime.Init.Period = COUT_TIMER_MAX;                                   
  timMeasTime.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  timMeasTime.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&timMeasTime) != HAL_OK)
  {
    Error_Handler();
  }
  
  // 3) Включить таймер
  HAL_TIM_Base_Start(&timMeasTime);
  
  // 4) Инициализировать структуру с измирениями
  for(i=0; i<MaxTC; i++){
    GlobalM4.MeasTime[i].ValCount = 0;         // Значение счетчика таймера на момент начала измирений         
    GlobalM4.MeasTime[i].ValMin = 0xffff;      // Минимальное значение времени выполнения
    GlobalM4.MeasTime[i].ValMax = 0;           // Максимальное значение времени выполнения
    GlobalM4.MeasTime[i].ValTekTime = 0;       // Текущее время выполнения (последнее измирение)
  }
}

// Получить текущее значение счетчика таймера
u32 getTimeUs(){
  return timMeasTime.Instance->CNT;
}

// Стартовая точка измерения временных интервалов
void StartTimControl(MeasTime_Enum NewTC){
  GlobalM4.MeasTime[NewTC].ValCount = getTimeUs();
}
  
// Конечная точка измирения времени
void EndTimControl(MeasTime_Enum NewTC){
      int tekVal, Time;
      
      // 1) Получить текущее значение таймера
      tekVal = getTimeUs();
      
      // 2) Расчитать интервал времени с учетом возможного переполнения таймера
      if(tekVal < GlobalM4.MeasTime[NewTC].ValCount){
        Time = COUT_TIMER_MAX - GlobalM4.MeasTime[NewTC].ValCount + tekVal;   // Было переполнение таймера
      }
      else{
        Time = tekVal - GlobalM4.MeasTime[NewTC].ValCount;                    // Переполнение таймера не было
      }
            
      // 3) Определим максимальное значение интервала
      if(Time > GlobalM4.MeasTime[NewTC].ValMax){
        GlobalM4.MeasTime[NewTC].ValMax = Time;             // Обновить максимальное значение               
      }
      
      // 4) Определим минимальное значение интервала
      if(Time < GlobalM4.MeasTime[NewTC].ValMin){
        GlobalM4.MeasTime[NewTC].ValMin = Time;             // Обновить минимальное значение
      }
      
      // 5) Сохранить теущее время выполнения
      GlobalM4.MeasTime[NewTC].ValTekTime = Time;
}