/* =================================================================================
File name:  STFilter.h  (IQ version)
                    
Originator: TRIOL
Описание: Фильтр Коваленкова/Шипулина (ФНЧ не вносящий задержек)
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 18-12-2021 Version 1.00
------------------------------------------------------------------------------*/
#ifndef _STFILTR_H_
#define _STFILTR_H_
#include "type.h"

typedef struct {  f32  AlphaIn;       // Input: Вход фильтра Alfa
                  f32  BetaIn;        // Input: Вход фильтра Beta
                  f32  We;            // Input: Текущий угол

                  f32  K1;            // Init: Коеффициент фильтра 1
   		  f32  K2;            // Init: Коеффициент фильтра 2
   		  f32  K3;            // Init: Коеффициент фильтра 3

                  f32  AlphaOut;       // Output: Выход фильтра Alpha
                  f32  BetaOut;        // Output: Выход фильтра Beta

                  void  (*calc)();     // Pointer to calculation function
                  void  (*reset)();    // Pointer to reset function
                 } STFILTR;

typedef STFILTR *STFILTR_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the STFILTR object.
-----------------------------------------------------------------------------*/                     
#define STFILTR_DEFAULTS { 0,        \
                           0,        \
                           0,        \
                           0,        \
                           0,        \
                           0,        \
                           0,        \
                           0,        \
                           (void (*)(u32))STFilterCalc, \
                           (void (*)(u32))STFilterReset }

/*------------------------------------------------------------------------------
Prototypes for the functions in STFilter.c
------------------------------------------------------------------------------*/
void STFilterCalc(STFILTR_handle);
void STFilterReset(STFILTR_handle);

#endif // _STFILTR_H_
