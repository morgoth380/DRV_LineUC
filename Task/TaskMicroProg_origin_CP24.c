#include "measTime.h"
#include "TaskMicroProg.h"
#include "GlobalVar.h"
#include "MTZ_Time.h"
#include "Global_Include.h"
#include "stm32f446xx.h"

extern u32 RamArea[2048];

#define setFlagIsFirstCall(a) { flagIsFirstCall = a; }
#define FlagIsFirstCall()     ( flagIsFirstCall      )


TIM_HandleTypeDef timMicroProg;
IWDG_HandleTypeDef hiwdg;

// Definitions for semStartMicroProg 
osSemaphoreId_t semStartMicroProgHandle;
const osSemaphoreAttr_t semStartMicroProg_attributes = {
  .name = "semStartMicroProg"
};

/* USER CODE BEGIN Header_FuncMicroProg */
/**
* @brief Function implementing the MicroProg thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FuncMicroProg */
void FuncMicroProg(void *argument)
{
    int    	ramPnt;
    u32         Serial;
   
    // Создать семафор semStartMicroProg
    semStartMicroProgHandle = osSemaphoreNew(1, 1, &semStartMicroProg_attributes);
    
    // Инициализация периферии
    MX_TIM14_Init();                       // Настроить таймер синхронизации запуска микропрограммы
    HAL_TIM_Base_Start(&timMicroProg);
    HAL_TIM_Base_Start_IT(&timMicroProg);  // Разрешить прерывание от таймера и запустить таймер
   
    //MX_IWDG_Init();                        // Настройка вачдога
        
    //------------------------------------------------------------------------
    //                  Обработчик 4мс такта
    // Два вложеных цикла необходимо, для переинициализации уставок, 
    // после восстановления уставок (запись из АТ45 в FRAM)
    //------------------------------------------------------------------------
    while(1){
    
        //------------------------------------------------------------------------
        // ИНИЦИАЛИЗАЦИЯ МИКРОПРОГРАММЫ
        //------------------------------------------------------------------------
        setFlagIsFirstCall(1);
        ramPnt = MicroProg_Init((u32)RamArea, (const MicroProg_type *)MicroProgTabl);

        //__HAL_IWDG_START(&hiwdg);            // Запустить вачдог
        
        if(ramPnt != 0){
           CoreMicroProg(ramPnt, &MPOpis);   // Первый запуск микропрограммы для инициализации
        }
        MTZ_Time();
        setFlagIsFirstCall(0);
        
        HAL_IWDG_Refresh(&hiwdg);            // Сбросить вачдог после первого вызова микропрограммы
        osDelay(4);                          // Имитация периода вызова микропрограммы
        HAL_IWDG_Refresh(&hiwdg);            // Сбросить вачдог
        
        // Вытаскиваем из ПЧ его фактический номер, как это прописано в файле конфигурации
        RdFramMicro(&Serial, &nv.DK_file , sizeof(u32));
        Serial = (Serial >> 8 & 0x00FF00FF) | (Serial << 8 & 0xFF00FF00); // SWAP байтов внутри, иначе значение неправильное будет
        GlobalM4.VfdSerialNumber = Serial;        
        
	while(1){

        //------------------------------------------------------------------------
        // ВЫПОЛНЕНИЕ МИКРОПРОГРАММЫ
        //------------------------------------------------------------------------
            // Залочить задачу до получения симофора с прерывания таймера синхронизации
            osSemaphoreAcquire(semStartMicroProgHandle, portMAX_DELAY);          // Симофор для синхронизации выполнения микропрограммы с таймером
          
            // Измирение периода вызова микропрограммы
            EndTimControl(PeriodMicroProgTC);
            StartTimControl(PeriodMicroProgTC);
            
            StartTimControl(TskMicroProgTC);         // Начать измерять время выполнения микропрограммы
            if(ramPnt != 0){
            	CoreMicroProg(ramPnt, &MPOpis);      // Выполнение микропрограммы
            }
            
            // Запрос на перезагрузку МК
            if (GlobalM4.CPU_restart == 1){
                HAL_NVIC_SystemReset();              //RESTART CPU
            }

            MTZ_Time();
            
            EndTimControl(TskMicroProgTC);           // Закончить измерять время выполнения микропрограммы
            
            HAL_IWDG_Refresh(&hiwdg);                // Сбросить вачдог
            

      }
   }
}

// Настройка таймера для синхронизации вызова микропрограммы (4ms)
void MX_TIM14_Init(void){
  // Тактирование таймера осуществляется от шины ABP1 с частотой 90Мгц
  // 1) Включить тактирование
  __HAL_RCC_TIM14_CLK_ENABLE();
  
  // 2) Настройка прерывания от таймера
  HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  
  timMicroProg.Instance = TIM14;
  timMicroProg.Init.Prescaler = 8999;                                        // Предделитель таймера (+1) - при тактовой частоте 90Мгц период 100мкс
  timMicroProg.Init.CounterMode = TIM_COUNTERMODE_UP;                        // Направление счета - только вперед
  timMicroProg.Init.Period = 40;                                             // Значение до какого считать
  timMicroProg.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  timMicroProg.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&timMicroProg) != HAL_OK)                            // Применить настройки
  {
    Error_Handler();
  }
}

// Обработчик прирывания по таймеру синхронизации
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
   if( htim->Instance == TIM14 ){
      osSemaphoreRelease(semStartMicroProgHandle);   // Установить симафор;
   }
}

void MX_IWDG_Init(void){
  // Тактируется IWDG от внутреннего RC генератора с частотой 32кГц
  // Переполнение на 10мс 
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;    // Предделитель на 4
  hiwdg.Init.Reload = 80;                     // Считать до... 
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
}


//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);