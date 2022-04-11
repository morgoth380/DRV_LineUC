/* =================================================================================
File name:  RMSVALCALC.H  (float)                   
                    
Originator: Triol

Description: 
This file contains type definitions, constants and initializers for
the ramp generation functions contained in RMSVALCALC.C
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-01-2012 Version 1.00 
 04-11-2020 Version 2.00  PAN (float)                                                   
------------------------------------------------------------------------------*/

#ifndef __RMSVALCALC_H__
#define __RMSVALCALC_H__

#include "type.h"

typedef struct { float  Input;      // Input: measured value
                 u16    Period;     // Input: Period of Output voltage
                 float  cnt;        // Variable: count sample by period
                 float  SqrSum;     // Variable: sum of squares
                 float  RMSVal;     // Output: Active value  
                 void  (*calc)(); // Pointer to calculation function
                 void  (*reset)(); // Pointer to reset function
                 } RMSVALCALC;          

typedef RMSVALCALC *RMSVALCALC_handle;  
/*------------------------------------------------------------------------------
      Object Initializers
------------------------------------------------------------------------------*/                       
#define RMSVALCALC_DEFAULTS { 0,0,0,0,0, \
                             (void (*)(u32))rms_value_calc, \
                             (void (*)(u32))rms_value_reset}

/*------------------------------------------------------------------------------
      Funtion prototypes
------------------------------------------------------------------------------*/                                               
void rms_value_calc(RMSVALCALC_handle);
void rms_value_reset(RMSVALCALC_handle);

#endif


