/* =================================================================================
File name:       LINEAR_RAMP.H  (float version)                    
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the LINEAR_RAMP.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 05-04-2021 Version 1.00
------------------------------------------------------------------------------*/
#ifndef __LINEARRAMP_H__
#define __LINEARRAMP_H__

#include "type.h"

typedef struct {  float  StartValue;      // Input - Начальное значение ЗИ
                  float  EndValue;        // Input - Конечное значение ЗИ
                  float  RampTime;        // Input - Время работы ЗИ
                  float  Fpwm;            // Input - Частота ШИМа
                  u16    StartFlag;       // Input - Флаг запуска работы
                  
                  u16    WorkFlag;        // Internal variable - статус работы - да/нет. Работает как защелка
                  float  RampStep;        // Internal variable - шаг приращения параметра
                  u16    WorkDone;        // Output - Выходной флаг, что работа выполнена
                  float  Out;             // Output - Текущее выходное значение
                  
                  void  (*calc)();      // Pointer to calculation function
                  void  (*reset)();     // Pointer to reset function
                 } LINEARRAMP;          

typedef LINEARRAMP *LINEARRAMP_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the LINEARRAMP object.
-----------------------------------------------------------------------------*/                     
#define LINEARRAMP_DEFAULTS {0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           0,     \
                           0,     \
                           0.0f,  \
                           0,     \
                           0.0f,  \
                          (void (*)(u32))linear_ramp_calc,\
                          (void (*)(u32))linear_ramp_reset }

/*------------------------------------------------------------------------------
Prototypes for the functions in linear_ramp.C
------------------------------------------------------------------------------*/
void linear_ramp_calc(LINEARRAMP_handle);
void linear_ramp_reset(LINEARRAMP_handle);

#endif // __LINEARRAMP_H__

