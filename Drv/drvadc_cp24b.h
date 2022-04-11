#ifndef _DRVADC_H_
#define _DRVADC_H_

#include "type.h"

#ifdef _PROJECT_FOR_STM32_
	#include "stm32f4xx_hal.h"

	extern ADC_HandleTypeDef hadc1;
	extern ADC_HandleTypeDef hadc2;
	extern ADC_HandleTypeDef hadc3;
	extern DMA_HandleTypeDef hdma_adc3;
	extern DMA_HandleTypeDef hdma_adc1;

#endif

/*
АЦП1 запускается по таймеру, DMA складывает данные в глобальную структуру. Измеряет медленные сигналы не требующие усриднения в такте ШИМ
АЦП2 работает с максимальной скоростью и опрашивает только каналы измирения токов для контроля превышения тока (МТЗ)
с помощью встроенного цифрового компаратора.
АЦП3 запускается по таймеру, DMA складывает данные в массив для дальнейшего усреднения в периоде ШИМ. Измеряет только
токи и ЗПТ.
*/

// Назначение ног для подключение каналов измерения
#define MEAS_CHANNEL_IU              ADC_CHANNEL_10      // PC0  
#define MEAS_CHANNEL_IV              ADC_CHANNEL_11      // PC1 
#define MEAS_CHANNEL_IW              ADC_CHANNEL_12      // PC2   
#define MEAS_CHANNEL_UD              ADC_CHANNEL_13      // PC3
#define MEAS_CHANNEL_AB              ADC_CHANNEL_0       // PA0
#define MEAS_CHANNEL_BC              ADC_CHANNEL_1       // PA1

#define MEAS_CHANNEL_TEMP_U          ADC_CHANNEL_2       // PA2
#define MEAS_CHANNEL_TEMP_V          ADC_CHANNEL_3       // PA3
#define MEAS_CHANNEL_TEMP_W          ADC_CHANNEL_4       // PA4
#define MEAS_CHANNEL_TEMP_INTERN     ADC_CHANNEL_5       // PA5

// Порты
#define PORT_ADC_1                   GPIOC               // Порт 1                  
#define PORT_ADC_2                   GPIOA               // Порт 2
// Пины
#define IU_MK_Pin                    GPIO_PIN_0          // PC0
#define IV_MK_Pin                    GPIO_PIN_1          // PC1 
#define IW_MK_Pin                    GPIO_PIN_2          // PC2   
#define UD_MK_Pin                    GPIO_PIN_3          // PC3

#define AB_MK_Pin                    GPIO_PIN_0          // PA0
#define BC_MK_Pin                    GPIO_PIN_1          // PA1
#define TEMP_U_MK_Pin                GPIO_PIN_2          // PA2
#define TEMP_V_MK_Pin                GPIO_PIN_3          // PA3
#define TEMP_W_MK_Pin                GPIO_PIN_4          // PA4
#define TEMP_INTERN_MK_Pin           GPIO_PIN_5          // PA5


#define COUNT_SEMPL              16                               // Количество семплов одного канала измирения в буффере
#define COUNT_ADC_CHANNEL        6                                // I_U, I_V, I_W, Ud, Uab, Ubc
#define SIZE_BUF_UVWUD           COUNT_SEMPL * COUNT_ADC_CHANNEL  // Розмер буффера 

#define FREQ_TIM2                45000.0f                         // Частота синхронизации таймера T2 запуска ДМА АЦП измерения токов (в кГц)

#define ADC_MAX_MTZ              247                              // Максиальное значение АЦП для МТЗ (256 - 3%) 
#define ADC_MIN_MTZ              8                                // Минимальное значение АЦП для МТЗ (3%)   

extern volatile u16 Adc3Buff[SIZE_BUF_UVWUD];        // Буффер куда DMA будет класть данные с ADC3 (I_U, I_V, I_W, Ud) для усреднения

/*-----------------------------------------------------------------------------
Define the structure of the ADC Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   float Fpwm_kHz;           // Частота ШИМ
   s16   CompMax;            // Максимальное значение компаратора
   s16   CompMin;            // Минимальное значение компаратора
   u16   CompStatus;         // Текущий статус компаратора (вкл/откл)
   void (*Init)();           // Первичная настройка ADC
   void (*ReInit)();         // Перенастроить модуль ADC для синхронизации с ШИМ
   void (*CompInit)();       // Настроить компаратор (Установить новое значение)
   void (*CompStart)();      // Разрешить прерывание от компаратора
   void (*CompStop)();       // Запретить прерывание от компаратора
   void (*ReInitVarPwm)();   // Перенастроить модуль ADC для синхронизации с ШИМ при переменном ШИМе
} ADCDRV;    

/*-----------------------------------------------------------------------------
Define a ADCDRV_handle
-----------------------------------------------------------------------------*/
typedef ADCDRV *ADCDRV_handle;

/*------------------------------------------------------------------------------
Default Initializers for ADCDRV Object 
------------------------------------------------------------------------------*/
#define ADCDRV_DEFAULTS  { 2.0,    \
                           255,    \
                           0,      \
                           1,      \
                           (void (*)(u32))AdcInit,    \
                           (void (*)(u32))AdcReInit,  \
                           (void (*)(u32))CompReInit, \
                           (void (*)(u32))CompStart,  \
                           (void (*)(u32))CompStop,   \
                           (void (*)(u32))ReInitVarPwm,\
                         }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void AdcInit(ADCDRV_handle);       // Настроить ADC
void AdcReInit(ADCDRV_handle);     // Перенастроить модуль ADC (Синхронизировать с ШИМ)
void CompReInit(ADCDRV_handle);    // Перенастроить модуль компаратора (МТЗ)
void CompStart(ADCDRV_handle);     // Разрешить прерывание от компаратора
void CompStop(ADCDRV_handle);      // Запретить прерывание от компаратора
void ReInitVarPwm(ADCDRV_handle);  // Перенастроить модуль ADC для синхронизации с ШИМ при переменном ШИМе

#endif // _DRVADC_H_