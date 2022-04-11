#include "measTime.h"
#include "TaskMicroProg.h"
#include "GlobalVar.h"
#include "MTZ_Time.h"
#include "Global_Include.h"
#include "stm32f446xx.h"

#define TIMIOUT_INIT_PERIF    100                // Таймаут настройки периферии

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

// Definitions for semWaitPerifInit 
osSemaphoreId_t semWaitPerifInitHandle;
const osSemaphoreAttr_t semWaitPerifInit_attributes = {
  .name = "semWaitPerifInit"
};


/* USER CODE BEGIN Header_FuncMicroProg */
/**
* @brief Function implementing the MicroProg thread.
* @param argument: Not used
* @retval None
*/

// п/п синхронизации DK файлов конфигурации
void SyncDKFile (void);

/* USER CODE END Header_FuncMicroProg */
void FuncMicroProg(void *argument)
{
    int    	ramPnt;
    u32         Serial;
    // 1) Создать симафор для синхронизации вызова задачи МП
    semStartMicroProgHandle = osSemaphoreNew(1, 1, &semStartMicroProg_attributes);     // Создать семафор semStartMicroProg
    
    // 2) Создать семафор для запуска МП только после инициализации периферии блока
    semWaitPerifInitHandle = osSemaphoreNew(1, 0, &semWaitPerifInit_attributes);      // Создать семафор semWaitPerifInit
    
    // 3) Инициализация Таймера синхронизации МП
    MX_TIM14_Init();                       // Настроить таймер синхронизации запуска микропрограммы
    HAL_TIM_Base_Start(&timMicroProg);
    HAL_TIM_Base_Start_IT(&timMicroProg);  // Разрешить прерывание от таймера и запустить таймер
   
    // 4) Залочить задачу пока не инициализирована переферия блока
    osSemaphoreAcquire(semWaitPerifInitHandle, TIMIOUT_INIT_PERIF);
    
    // 5) Настроить вачдог 
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
        
        // Выполнить синхронизацию DK_file нового и старого.
        SyncDKFile();
        
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
  timMicroProg.Init.Period = 39;                                             // Значение до какого считать
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


void DkSwapData (u8 *dst, u8 *srs, u16 size) {
  u16   i;
  u8    swap1, swap2;
  
  for (i=0; i<size/2; i++) {
    swap1 = *srs++;
    swap2 = *srs++;
    
    *dst++ = swap2;
    *dst++ = swap1;    
  }
    
}

void SyncDKFile (void) {
  DK_file_Old_type      dk_file_old, dk_file_old_swap;
  DK_file_type          dk_file, dk_file_swap;
  u16                   calcCRC, calcCRC_Old;
  //u16                   calcCRC_Old2, calcCRC2;
  u16                   size;
  //u8                    *pnt;
  
  RdFramMicro(&dk_file_old, &nv.DK_file_Old , sizeof(DK_file_Old_type));  
  RdFramMicro(&dk_file, &nv.DK_file , sizeof(DK_file_type));     
  
  // Делаем Свап данных из файла конфигурации
  DkSwapData((u8*)(&dk_file_swap.SerialNumber), (u8*)(&dk_file.SerialNumber), sizeof(DK_file_type));
  // Делаем Свап данных из файла конфигурации
  DkSwapData((u8*)(&dk_file_old_swap.SerialNumber), (u8*)(&dk_file_old.SerialNumber), sizeof(DK_file_Old_type));
  
  //RdFramMicro(&dk_file_old, &nv.DK_file_Old , sizeof(DK_file_Old_type));  
  //RdFramMicro(&dk_file, &nv.DK_file , sizeof(DK_file_type)); 
  // Находим CRC на старый файл конфигурации, для свапнутого файла
  calcCRC_Old = Crc((u8*)(&dk_file_old_swap.SerialNumber), sizeof(DK_file_Old_type)-sizeof(u32));
  
  // CRC нового файла конфигурации
  calcCRC = 0xFFFF;   
  size = sizeof(DK_file_Old_type)-sizeof(u32); //(u16)((u8*)&dk_file_swap.crc - (u8*)&dk_file_swap.SerialNumber);
  calcCRC = CrcPortion8((u8 *)(&dk_file_swap.SerialNumber), size, calcCRC);
  size = sizeof(DK_file_type) - size - sizeof(u16);
  calcCRC = CrcPortion8((u8 *)(&dk_file_swap.newFile), size, calcCRC);
    
  // !!! CRC находится в середине структуры. Это надо учесть при расчете CRC !!!
  //--------------------------------------------------------------------------//
    
  // Сверяем блок для старого формата файла конфигурации. Если там все Ок, значит первое вхождение
  // и файл надо восстановить в новый формат, иначе не делаем копирование
  if (dk_file_old.crc == calcCRC_Old) {
      // Переносим его данные в новый файл
      dk_file.SerialNumber = dk_file_old.SerialNumber;
      dk_file.timeUnix     = dk_file_old.timeUnix;      
      memcpy (&dk_file.Functional, &dk_file_old.Functional, sizeof(dk_file.Functional));
      memcpy (&dk_file.Ust1, &dk_file_old.Ust1, sizeof(dk_file.Ust1));
      memcpy (&dk_file.Ust2, &dk_file_old.Ust2, sizeof(dk_file.Ust2));      
      memcpy (&dk_file.Ust3, &dk_file_old.Ust3, sizeof(dk_file.Ust3));      
      memcpy (&dk_file.Tree, &dk_file_old.Tree, sizeof(dk_file.Tree));   
      memcpy (&dk_file.PultFirmware, &dk_file_old.PultFirmware, sizeof(dk_file.PultFirmware));                
    
      dk_file.newFile = 0x1111; // Ставим маркер нового формата файла.
      
      DkSwapData((u8*)(&dk_file_swap.SerialNumber), (u8*)(&dk_file.SerialNumber), sizeof(DK_file_type));      
      
      // Теперь находим CRC на новый файл кофигурации и сохраняем его
      calcCRC = 0xFFFF;   
      size = sizeof(DK_file_Old_type)-sizeof(u32); //(u16)((u8*)&dk_file.crc - (u8*)&dk_file.SerialNumber);
      calcCRC = CrcPortion8((u8 *)(&dk_file_swap.SerialNumber), size, calcCRC);
      size = sizeof(DK_file_type) - size - sizeof(u16);
      calcCRC = CrcPortion8((u8 *)(&dk_file_swap.newFile), size, calcCRC);
      
      dk_file.crc = calcCRC;
      WrFramMicro(&nv.DK_file, &dk_file, sizeof(DK_file_type));          
      
      // Обнулить старый DK_file
      dk_file_old.SerialNumber = 0x00000000;
      dk_file_old.timeUnix = 0x00000000;      
      memset (&dk_file_old.Functional, 0x00, sizeof(dk_file_old.Functional));
      memset (&dk_file_old.Ust1, 0x00, sizeof(dk_file_old.Ust1));
      memset (&dk_file_old.Ust2, 0x00, sizeof(dk_file_old.Ust2));      
      memset (&dk_file_old.Ust3, 0x00, sizeof(dk_file_old.Ust3));      
      memset (&dk_file_old.Tree, 0x00, sizeof(dk_file_old.Tree));   
      memset (&dk_file_old.PultFirmware, 0x00, sizeof(dk_file_old.PultFirmware));                      
      dk_file_old.crc = 0x0000;
      WrFramMicro(&nv.DK_file_Old, &dk_file_old, sizeof(DK_file_Old_type));             
  } 
  
  // Проверяем активирован ли новый формат конфигуратора
  if (dk_file.newFile != 0x1111) {
    
      dk_file.newFile = 0x1111; // Ставим маркер нового формата файла.
      
      DkSwapData((u8*)(&dk_file_swap.SerialNumber), (u8*)(&dk_file.SerialNumber), sizeof(DK_file_type));      
      
      // Теперь находим CRC на новый файл кофигурации и сохраняем его
      calcCRC = 0xFFFF;   
      size = sizeof(DK_file_Old_type)-sizeof(u32); 
      calcCRC = CrcPortion8((u8 *)(&dk_file_swap.SerialNumber), size, calcCRC);
      size = sizeof(DK_file_type) - size - sizeof(u16);
      calcCRC = CrcPortion8((u8 *)(&dk_file_swap.newFile), size, calcCRC);
      
      dk_file.crc = calcCRC;
      WrFramMicro(&nv.DK_file, &dk_file, sizeof(DK_file_type));          
  }
      
}

//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);