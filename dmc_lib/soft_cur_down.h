/* =================================================================================
File name:      SOFT_CUR_DOWN.H  (float version)                        
Originator:     Triol
Author:         Sikalov Y. 
Description: 
Header file containing constants, data type definitions, and 
function prototypes for the SOFT_CUR_DOWN.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 24-07-2012 Version 1.00
 23-09-2020 Version 2.00 PAN (Перевел во float)
-----------------------------------------------------------------------------------*/

#ifndef __SOFTCURDOWN_H__
#define __SOFTCURDOWN_H__

#include "type.h"

typedef struct {
    u16 Start_fl     : 1; // Input: start of soft down
} DOWNBITS;

typedef struct {  DOWNBITS bit;        // Input: bits
                  float  Fpwm;         // Fpwm;
                  float  IdRef_in;     // Input: 
                  float  IqRef_in;     // Input:
                  float  ThetaPsi_in;  // Input:
                  float  TimeCurDown;  // Down time
                  float  StepD;        // Variable:
                  float  StepQ;        // Variable:
                  float  IdRef_out;    // Output:  
                  float  IqRef_out;    // Output:
                  float  ThetaPsi_out; // Output:
                  void  (*calc)();     // Pointer to calculation function
                 } SOFTCURDOWN;              

typedef SOFTCURDOWN *SOFTCURDOWN_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the SOFTCURDOWN object.
-----------------------------------------------------------------------------*/                     
#define SOFTCURDOWN_DEFAULTS {0.0f, \
                              0.0f, \
                              0.0f, \
                              0.0f, \
                              0.0f, \
                              0.0f, \
                              0.0f, \
                              0.0f, \
                              0.0f, \
                              0.0f, \
                              0.0f, \
                              (void (*)(u32))soft_cur_down_calc }
/*------------------------------------------------------------------------------
Prototypes for the functions in SOFTCURDOWN.C
------------------------------------------------------------------------------*/
void soft_cur_down_calc(SOFTCURDOWN_handle);

#endif
