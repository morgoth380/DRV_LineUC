/* =================================================================================
File name:        RAMPCNTLGEN.H  (float)                   
                    
Originator: Triol

Description: 
This file contains type definitions, constants and initializers for
the ramp generation functions contained in RAMPCNTRGEN.C
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 04-11-2020 Version 4.00  PAN (float)                                                 
------------------------------------------------------------------------------*/

#ifndef __RAMPCNTLGEN_H__
#define __RAMPCNTLGEN_H__

#include "type.h"


typedef struct { float  Framp;        // Input: Ramp frequency (pu)   
                 float  Fwork;        // Input: Work frequency (pu) 
                 float  Fnom;         // Input: Nominal frequency (pu)
                 float  StepAngleMax; // Parameter: Maximum step angle (pu)   
                 float  StepFreqMax;  // Parameter: Maximum step Frequence (pu)       
                 float  Angle;        // Output: Step angle (pu)  
                 float  Uout;         // Output:  (pu)    
                 u16 Period;          // Output: period               
                 void  (*calc)();   // Pointer to calculation function
                 void  (*reset)();
               } RAMPCNTLGEN;               

typedef RAMPCNTLGEN *RAMPCNTLGEN_handle;  
/*------------------------------------------------------------------------------
      Object Initializers
------------------------------------------------------------------------------*/                       
#define RAMPCNTLGEN_DEFAULTS {0,0,0,0,0,0,0,0, \
                         (void (*)(u32))rampcntlgen_calc, \
                         (void (*)(u32))rampcntlgen_reset }

/*------------------------------------------------------------------------------
      Funtion prototypes
------------------------------------------------------------------------------*/                                               
void rampcntlgen_calc(RAMPCNTLGEN_handle);
void rampcntlgen_reset(RAMPCNTLGEN_handle);

#endif // __RAMPCNTLGEN_H__
