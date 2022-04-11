#ifndef _TORMSW_H_
#define _TORMSW_H_

#include "type.h"
#ifdef _PROJECT_FOR_STM32_     
#include "stm32f4xx_hal.h"
#endif

// Пин для управления тормозным ключем
#ifdef _BOARD_CP24_V2_1
   #define TORM_SW_MK_Pin               GPIO_PIN_1
   #define TORM_SW_MK_GPIO_Port         GPIOB
#elif  _BOARD_CP24_V2_0
   #define TORM_SW_MK_Pin               GPIO_PIN_2
   #define TORM_SW_MK_GPIO_Port         GPIOB
#elif _BOARD_CP24B_V1_0
   #define TORM_SW_MK_Pin               GPIO_PIN_1
   #define TORM_SW_MK_GPIO_Port         GPIOB
#elif _BOARD_CP24B_V1_1
   #define TORM_SW_MK_Pin               GPIO_PIN_1
   #define TORM_SW_MK_GPIO_Port         GPIOB
#else
    #warning Для тормозного ключа выбран не правильный блок. Проверьте define блока
#endif  //_BOARD_CP24_V2_1


// Текущее состояние ключа
typedef enum{
    TormSwNoInit = 0,      // Тормозной ключ не настроен
    TormSwOff,             // Тормозной ключ закрыт 
    TormSwOn,              // Тормозной ключ открыт
    TormResBreak,          // Авария обрыва тормозного резистора
    TormSwNotEnable,       // Авария - тормозной ключ не разрешен, но включена проверка тормозного резистора
}StateTormSw_Enum;

// =================================================================
// Перечисление состояния работы модуля
typedef enum {
	None				= 0,
	InitSendSequence,
	SendSequence,
	TestSequence,
        TestDone,
}CheckTormRes_Enum; 
// =================================================================
// Перечисление возвращаемых событий
typedef enum {
	Stopped					= 0, // Останов
	Processing				= 2, // Идеть проверка
	Breakdown				= -1, // Обрыв резистора или резистор отсутствует
	Torm_OK					= 1,  // Резистор в норме
}CheckTormResStatus_Enum;

/*-----------------------------------------------------------------------------
Define the structure of the ADC Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
   float                        UdFast;             // Текущее напряжение в ЗПТ
   float                        UopenSw;            // Напряжение открытия ключа
   float                        UcloseSw;           // Напряжение закрытия ключа
   u16                          EnSw;               // Разрешить/Запретить работу ключа (0-запретить, 1-разрешить)
   StateTormSw_Enum             State;              // Текущее состояние тормозного ключа 
   u16                          PwmOn;              // Статус ШИМа
   f32                          Fpwm;               // Частота ШИМа   
   // Данные для теста сопротивленияы
   CheckTormRes_Enum            TestResState;       // Текущее состояние проверки тормозного резистора
   CheckTormResStatus_Enum      TestResResult;      // Результат проверки тормозного резистора   
   u16                          TormCheck;          // Сигнал запуска проверки тормозного резистора
   f32				Udslow;	  	    // Текущее значение Ud
   u16				BitTimeTick;  	    // Длительность бита сигнала в тиках ШИМа
   u16				seqSize;	    // Длина кодовой посылки
   u16				cntTick;	    // Счетчик длительности посылки одного бита в тиках ШИМа. Счет идет вниз к нулю.
   u16				sampleBit;	    // Текущий индекс формируемого бита в массиве битов
   u16				SampleTick;	    // Значение сравнения для взятия отсчета из текущего входного Ud
   f32				BitTime_Sec;        // Длительность одного бита сигнала в секундах
   s16				CorelationResult;   // Результат вычисления корреляционной функции
   f32				SampleArray[11];    // Массив отсчетов Ud в IQ формате
   
   void (*Init)();           // Настройка тормозного ключа
   void (*Control)();        // Управление ключом
   CheckTormResStatus_Enum (*CheckTorm)();      // Проверка тормозного резистора
} TORMSW;    

/*-----------------------------------------------------------------------------
Define a TORMSW_handle
-----------------------------------------------------------------------------*/
typedef TORMSW *TORMSW_handle;

/*------------------------------------------------------------------------------
Default Initializers for TORMSW Object 
------------------------------------------------------------------------------*/
#define TORMSW_DEFAULTS  { 0.0f,                  \
                           9999.0f,               \
                           0.0f,                  \
                           0,                     \
                           TormSwNoInit,          \
                           0,                     \
                           0,                     \
                           None,                  \
                           Stopped,               \
                           0,0,0,0,0,0,0,0,0,     \
                           0,0,0,0,0,0,0,0,0,0,0, \
                           (void (*)(u32))SwInit,    \
                           (void (*)(u32))SwControl, \
                           (CheckTormResStatus_Enum (*)(u32))CheckTorm, \
                          }
/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void SwInit(TORMSW_handle);         // Настроить тормозной ключ
void SwControl(TORMSW_handle);      // Управление ключом
CheckTormResStatus_Enum CheckTorm(TORMSW_handle); // Проверка тормозного резистора

#endif // _TORMSW_H_