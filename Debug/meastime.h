#ifndef     MEASTIME_H
#define     MEASTIME_H

#include "type.h"

#define COUT_TIMER_MAX      4000000000 // Максимальное количество тиков таймера   


// Типы измерителей времени
typedef enum{
    TskMicroProgTC = 0,          // Задача микропрограммы
    PeriodMicroProgTC,           // Период вызова микропрограммы
    TskModbusSlaveTC,            // Задача ModbusSlave
    TskModbusMasterTC,           // Задача ModbusMaster 
    TskFlashTC,                  // Задача Flash
    PeriodADC,                   // Период АЦП (время наполнения буффера)
    PwmPeriodProcessingTC,       // Время обработки периода ШИМ
    PwmPeriodTC,                 // Период ШИМ 


    TestTC,


    
    MaxTC,                       // Количество измерителей времени
}MeasTime_Enum;


typedef struct{
	u32	ValCount;         // Значение счетчика таймера на момент начала измирений   
	u32	ValMin;           // Минимальное значение времени выполнения
	u32	ValMax;           // Максимальное значение времени выполнения
	u32 	ValTekTime;       // Текущее время выполнения (последнее измирение)
}MeasTime_type;


//------------------------------------------------------------------------//
//                  Протопипы функций                                     //
//------------------------------------------------------------------------//
#ifdef _PROJECT_FOR_STM32_
void InitMeasTime(void);                     // Инициализировать подсистему измирения времени выполнения
void StartTimControl(MeasTime_Enum NewTC);   // Стартовая точка измирения временных интервалов
void EndTimControl(MeasTime_Enum NewTC);     // Конечная точка измирения времени 
#endif //_PROJECT_FOR_STM32_

	


#endif //   MEASTIME_H
