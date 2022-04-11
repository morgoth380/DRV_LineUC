/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"
#include "GlobalVar.h"
#include "InputOutput.h"
#include "HeadFirmware.h"
#include "DrvRTC.h"

// Проверяем какой блок будем компилировать    
#ifdef _BOARD_CP24_V2_0
    #warning Выбрана настройка для блока СР24 версии 2.0
#elif  _BOARD_CP24_V2_1
    #warning Выбрана настройка для блока СР24 версии 2.1    
#elif  _BOARD_CP24B_V1_0
    #warning Выбрана настройка для блока СР24В версии 1.0    
#elif  _BOARD_CP24B_V1_1
    #warning Выбрана настройка для блока СР24В версии 1.1
#else
    #error В настройках не выбрана рабочая плата для проекта. Возможные варианты: _BOARD_CP24B_V1_1, _BOARD_CP24B_V1_0, _BOARD_CP24_V2_1, _BOARD_CP24_V2_0    
#endif
    
    
TIM_HandleTypeDef timMeasTime;     // TIM13 

GlobalVar GlobalM4;
uint32_t     HSE_VALUE;

// Задача для отображения статуса блока CP24
osThreadId_t StatusCP24Handle;
const osThreadAttr_t StatusCP24_attributes = {
  .name = "StatusCP24",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

// Definitions for Tsk MicroProg 
osThreadId_t MicroProgHandle;
const osThreadAttr_t MicroProg_attributes = {
  .name = "MicroProg",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 4096 * 4//512 * 4
};

// Definitions for Tsk ModbusSlave 
osThreadId_t ModbusSlaveHandle;
const osThreadAttr_t ModbusSlave_attributes = {
  .name = "ModbusSlave",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 256 * 4
};

// Definitions for Tsk DataFlash 
osThreadId_t DataFlashHandle;
const osThreadAttr_t DataFlash_attributes = {
  .name = "DataFlash",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

// Definitions for Tsk DataFlash
osThreadId_t Task20msHandle;
const osThreadAttr_t Task20ms_attributes = {
  .name = "Task20ms",
  .priority = (osPriority_t) osPriorityLow7,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
RCC_StatusTypeDef SystemClock_Config(void);
//static void MX_GPIO_Init(void);
void StartDefaultTask(void *argument); 
HAL_StatusTypeDef res;
extern u16 crcPacket(unsigned char *Bff, u32 szBff);
CP24version_type  getCP24Version (void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  RCC_StatusTypeDef    RccStatus;
  u32                  i;
  
  HAL_DeInit();
  
  __set_PRIMASK(1);
    SCB->VTOR = BEGIN_PROG;
  __set_MSP( *(uint32_t*)(BEGIN_PROG) );
  __set_PRIMASK(0);  
  __enable_irq();
    
  
  // Reset of all peripherals, Initializes the Flash interface and the Systick. 
  HAL_Init();
  
  // Таймаут при старте, чтобы опросить ноги типа блока с небольшой задержкой.
  for (i=0; i<600000; i++) {i++; i--;}
  
  HSE_VALUE = ((uint32_t)12000000U); // Начальная частота для блока CP24B v1.1    
  
#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)  
  // А если сборка для СР24 - то начинаем разбираться какая у нас плата.  
  GlobalM4.versionCP24 = getCP24Version(); // Get CP24 version
  
  switch (GlobalM4.versionCP24){
  case CP24_v21:
    HSE_VALUE = ((uint32_t)10000000U); // частота для блока CP24 v2.1      
    break;
  case CP24_v22:
  case CP24_v23:
  case CP24_v24:
    HSE_VALUE = ((uint32_t)12000000U); // частота для блока CP24 v2.2      
    break;
  }  
#endif
  
  // Configure the system clock
  RccStatus = SystemClock_Config();
  
  // Проверить заголовок прошивки
  CheckHeadFirmware();       

  // CRC программы
  GlobalM4.CrcProgFlash = crcPacket((u8*)BEGIN_PROG, SIZE_PROG);  

  // Настройка FRAM
  PortFRAM_Init();
  
  // Настрйока часов
  MX_RTC_Init();

  // Инициализировать подсистему измерения временных интервалов
  InitMeasTime();
   
  #if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
      // Инициализация входов для типов аналоговых входов/выходов
      AinAoutInit();
      // Инициализация таймера для аналоговых выходов МК
      PWM_Aout_Init();
  #endif


  // Init scheduler 
  osKernelInitialize();

  // Create the thread(s) 
  //defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);  // creation of defaultTask
  StatusCP24Handle = osThreadNew(FuncStatusCP24, &RccStatus, &StatusCP24_attributes);  // creation of StatusCP24Task  
  MicroProgHandle = osThreadNew(FuncMicroProg, NULL, &MicroProg_attributes);         // creation of MicroProg  
  ModbusSlaveHandle = osThreadNew(FuncModbusSlave, NULL, &ModbusSlave_attributes);   // creation of ModbusSlave
  DataFlashHandle = osThreadNew(FuncDataFlash, NULL, &DataFlash_attributes);         // creation of DataFlash
  Task20msHandle = osThreadNew(Func20ms, NULL, &Task20ms_attributes);                // creation of Task20ms
   
  // Start scheduler 
  osKernelStart();
 
  while (1){
    
  }
}

#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
    RCC_StatusTypeDef SystemClock_Config(void)
    {
      RCC_OscInitTypeDef RCC_OscInitStruct = {0};
      RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
      RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
      RCC_StatusTypeDef    RccStatus; 
      u16       _PLLN;

      // Выбираем величину множителя PLL для разных кварцев. ПредДелитель кварца общий и равен 10.
      switch (GlobalM4.versionCP24){
      case CP24_v21:        
        _PLLN = 360; // 10 MHz
        break;
      case CP24_v22:
      case CP24_v23:
      case CP24_v24:
        _PLLN = 300;  // 12 MHz
        break;
      }      
      
      /** Configure the main internal regulator output voltage 
      */
      __HAL_RCC_PWR_CLK_ENABLE();
      __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
      /** Initializes the CPU, AHB and APB busses clocks 
      */    
      RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
      RCC_OscInitStruct.HSEState = RCC_HSE_ON;
      RCC_OscInitStruct.LSEState = RCC_LSE_ON;
      RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
      RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
      RCC_OscInitStruct.PLL.PLLM = 10; // Делитель
      RCC_OscInitStruct.PLL.PLLN = _PLLN; // 360; // Множитель - !!!!! здесь надо для 12 Мгц 300, и для 10 Мгц - 360
      // Вот тут у меня должно быть 360 МГц
      RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
      // А тут после деления - 180 МГц - нам нужна стартовая частота 180 МГц
      RCC_OscInitStruct.PLL.PLLQ = 2;
      RCC_OscInitStruct.PLL.PLLR = 2;  
        
//      // ДО запуска LSE, выбираем его режим работы - нормальный или мощный
//      HAL_RCCEx_SelectLSEMode (RCC_LSE_HIGHDRIVE_MODE);
      
      //HAL_RCC_OscConfig(&RCC_OscInitStruct);
      RccStatus = RCC_Init(&RCC_OscInitStruct);   // Применить настройки и сохранить статус 
      /** Activate the Over-Drive mode 
      */
      HAL_PWREx_EnableOverDrive();

      /** Initializes the CPU, AHB and APB busses clocks 
      */
      RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
      RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
      RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
      RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
      RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

      HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
            
      PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
      PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
      HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
                  
      return RccStatus;
    }
 
#define CP24ver_Pin1            GPIO_PIN_7
#define CP24ver_Pin1_Port       GPIOE  
#define CP24ver_Pin2            GPIO_PIN_5
#define CP24ver_Pin2_Port       GPIOA  
  
CP24version_type  getCP24Version (void) {
      // Теперь надо инициализировать ноги которые мне покажут что за тип блока у меня сейчас тут сидит.
      // PA5 / PE7 - Два входа. В блоке СР24 2.1 никуда не подключены. Мы их сразу подтянем к 1 через PULLUP.
      // Если на входах 1 - значит это блок СР24 2.1
      // Первый ведущий вход РЕ7
  
      u16    PinState1, PinState2;
      CP24version_type result;
      GPIO_InitTypeDef GPIO_InitStruct = {0};
      
      __HAL_RCC_GPIOA_CLK_ENABLE();            // Включить тактирование порта
      __HAL_RCC_GPIOE_CLK_ENABLE();            // Включить тактирование портa
  
  
      GPIO_InitStruct.Pin = CP24ver_Pin1;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(CP24ver_Pin1_Port, &GPIO_InitStruct);  
      
      GPIO_InitStruct.Pin = CP24ver_Pin2;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(CP24ver_Pin2_Port, &GPIO_InitStruct);
      
      PinState1 = HAL_GPIO_ReadPin(CP24ver_Pin1_Port, CP24ver_Pin1);      
      PinState2 = HAL_GPIO_ReadPin(CP24ver_Pin2_Port, CP24ver_Pin2); 
      result = (CP24version_type)(PinState1 + (PinState2 << 1));
      
      return (result);
}  
#endif

#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    RCC_StatusTypeDef SystemClock_Config(void)
    {
      RCC_OscInitTypeDef RCC_OscInitStruct = {0};
      RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
      RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
      RCC_StatusTypeDef    RccStatus;

      /** Configure the main internal regulator output voltage 
      */
      __HAL_RCC_PWR_CLK_ENABLE();
      __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
      /** Initializes the CPU, AHB and APB busses clocks 
      */
      RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
      RCC_OscInitStruct.HSEState = RCC_HSE_ON;
      RCC_OscInitStruct.LSEState = RCC_LSE_ON;
      RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
      RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
      RCC_OscInitStruct.PLL.PLLM = 10;
      RCC_OscInitStruct.PLL.PLLN = 300;
      RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
      RCC_OscInitStruct.PLL.PLLQ = 2;
      RCC_OscInitStruct.PLL.PLLR = 2; 
      
      // ДО запуска LSE, выбираем его режим работы - нормальный или мощный
      //HAL_RCCEx_SelectLSEMode (RCC_LSE_HIGHDRIVE_MODE);
      
      //HAL_RCC_OscConfig(&RCC_OscInitStruct);
      RccStatus = RCC_Init(&RCC_OscInitStruct);   // Применить настройки и сохранить статус 
      
    /** Activate the Over-Drive mode 
      */
      HAL_PWREx_EnableOverDrive();

      /** Initializes the CPU, AHB and APB busses clocks 
      */
      RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
      RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
      RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
      RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
      RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

      HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
                  
      PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
      PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
      HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

      return RccStatus;
    }
#endif 

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
