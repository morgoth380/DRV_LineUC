/* =================================================================================
 File name:     ANTI_ROLLBACK.H                                
 Originator:    Triol Corporation
 Autor:         Sikalov Yuriy
 Description:   Anti rollback function (20ms)
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 14-03-2012 Version 1.00
------------------------------------------------------------------------------*/
#ifndef __ANTIROLLBACK_H__
#define __ANTIROLLBACK_H__

#include "type.h"

typedef struct {  
                  struct {
                      u16 ARollbackEnb  :1;     // Input: Активация антиоткатной функции
                      u16 Start         :1;     // Input: Signal for start algorithm
                      u16 ForceEnb      :1;     // Output: Управление усилением регулятора скорости
                      u16 PwmOn         :1;     // Input: PwmOn signal. Restart all counter and flag
                  } flag;
                  f32 Time;                     // Input: Время работы антиоткатной функции
                  f32 TimeCount;                // Variable: Счетчик времени
                  f32 TimeStep;					// Input: Time Step
                  void  (*calc)();      // Pointer to calculation function
                 } ANTIROLLBACK;          

typedef ANTIROLLBACK *ANTIROLLBACK_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the ANTIROLLBACK object.
-----------------------------------------------------------------------------*/                     
#define ANTIROLLBACK_DEFAULTS { {0,0,0},0,0,0, \
                          (void (*)(u32))anti_rollback_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in ANTI_ROLLBACK.C
------------------------------------------------------------------------------*/
void anti_rollback_calc(ANTIROLLBACK_handle);

#endif
