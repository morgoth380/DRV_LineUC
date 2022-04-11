/* =================================================================================
File name:        MEASLMCUR.H  (float version)                   
                    
Originator:	Triol

Description: 
This file contains type definitions, constants and initializers for
the ramp generation functions contained in MEASLMCUR.C
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 23-06-2011 Version 1.00   
 19-10-2020 Version 2.00 PAN (float)                     
------------------------------------------------------------------------------*/

#ifndef __MEASLMCUR_H__
#define __MEASLMCUR_H__

#include "type.h"

typedef struct { float  Uline;        // Input: output voltage (pu)     
                 float  Iact;         // Input: active current (pu) 
                 float  Ireact;       // Input: reactive current(pu)
                 float  Ifull;        // Input: full current(pu)
                 float  Wout;         // Input: output frequence (pu)    
                 
                 float  sLr;          // Input: Rotor inductive  (pu)
                 float  UlineFlash;   // Input: output line flash voltage (pu) 
                 float  IfazFlash;    // Input: Flash current (pu)
                 float  Angle;        // Input: rotating angle (pu)
                 u16    Period;       // Input: Period of Output voltage 
                 
                 u16    PeriodAmt;    // Input: Period amount
                 u16    FirstStart;   // Input: First Start Flag
                 float  Rs;           // Output: Stator resistance (ohm)
                 float  Lm;           // Output: magnetizing inductance (H)
                 float  Cur_Phaze;    // Output
                 
                 u16    StopCalc;     // Output: Flag Stop Calculation
                 float  K1;           // Parameter: 
                 float  K2;           // Parameter: 
                 float ISumCos;       // Variable: 
                 float ISumSin;       // Variable: 
                 
                 float USumCos;       // Variable:           
                 float USumSin;       // Variable:           
                 u32    Counter;      // Variable:
                 float  IactSum ;     // Variable:  
                 float  IreactSum ;   // Variable:  
                 
                 float  IfullSum ;    // Variable:  
                 float  UlineSum;     // Variable:
                 float  PeriodCnt;    // Variable: Period Counter  
                 float  Uact;
                 float  Ureact;
                 
                 float  Ufull;
                 void  (*calc)();          // Pointer to calculation function
                } MEASLMCUR;           

typedef MEASLMCUR *MEASLMCUR_handle;  
/*------------------------------------------------------------------------------
      Object Initializers
------------------------------------------------------------------------------*/                       
#define MEASLMCUR_DEFAULTS {0,0,0,0,0,   \
                            0,0,0,0,0,   \
                            0,0,0,0,0,   \
                            0,           \
                            /*(380.0f * 2.0f) / (3.0f * 15.0f * 80.0f))*/ 0.2111, \
                            /*(380.0f * 2.0f) / (3.0f * 15.0f)*/ 16.8888,          \
                            0,0,         \
                            0,0,0,0,0,   \
                            0,0,0,0,0,   \
                            0, \
                         (void (*)(u32))measlmcur_calc }

/*------------------------------------------------------------------------------
      Funtion prototypes
------------------------------------------------------------------------------*/                                               
void measlmcur_calc(MEASLMCUR_handle);

#endif // __MEASLMCUR_H__
