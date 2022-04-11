/* =================================================================================
File name:      CNTRVOLTAGE.H  (float)                   
                    
Originator:	Triol

Description: 
This file contains type definitions, constants and initializers for
the ramp generation functions contained in CNTRVOLTAGE.C
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 15-06-2011	Version 1.00 
 04-11-2020	Version 2.00  PAN (float)                                                   
------------------------------------------------------------------------------*/

#ifndef __CNTRVOLTAGE_H__
#define __CNTRVOLTAGE_H__

#include "type.h"

typedef struct { f32  UlineStep;    // Input: output line voltage step (pu) 
                 f32  Ifaz;         // Input: full current(pu)
                 u16  Period;       // Input: Period of Output voltage
                 f32  Uline;        // Output: output line voltage (pu)  
                 u16  InomDoneFlag; // Output: Ток достиг необходимого уровня
                 
                 u16  UmaxFlag;     // Output: напряжение достигло максимума
                 u16  CurDownFlag;  // Output: Soft current down flag
                 f32  Imax;         // Parameter: 
                 f32  Umax;         // Parameter: 
                 f32  Itst;
                 
                 f32  f32IsU;
                 f32  f32III;
                 f32  Log0;
                 f32  Log1;
                 f32  Log2;
                 
                 f32  Log3;
                 f32  Log4;
                 f32  LogN;
                 f32  Lm0;
                 f32  Lm1;
                 
                 f32  Lm2;
                 f32  Lm3;
                 f32  s16IsU;
                 f32  s16LogN;
                 f32  s16Lm0;
                 
                 f32  s16Lm1;
                 f32  s16Lm2;
                 f32  s16Lm3;
                 f32  s16Lm;
                 f32  s16III;
                 
                 void  (*calc)();   // Pointer to calculation function
                 void  (*calcLm)(); // Pointer to calculation function
                 void  (*calcCurDown)();   // Pointer to calculation function
                 void  (*reset)();  // Pointer to reset function
                 } CNTRVOLTAGE;          

typedef CNTRVOLTAGE *CNTRVOLTAGE_handle;  
/*------------------------------------------------------------------------------
      Object Initializers
------------------------------------------------------------------------------*/                       
#define CNTRVOLTAGE_DEFAULTS {(1.63299316185545f * 57.0f / 380.0f / 50.0f),                \
                              0,0,0,0,0,0,                                                  \
                              (19.0f / 15.0f),                                             \
                              (1.63299316185545f * 57.0f / 380.0f),                        \
			      0.0f,                                                        \
			      0.0f,0.0f,0.0f,0.0f,0.0f,                         \
                              0.0f,0.0f,0.0f,0.0f,0.0f,                         \
                              0.0f,0.0f, \
			      0,0,0,0,0,0,0,0,                                             \
                             (void (*)(u32))cnrtvoltage_calc,  \
                             (void (*)(u32))cnrtvoltageLm_calc,\
                             (void (*)(u32))cnrtvoltageCurDown_calc,\
                             (void (*)(u32))cnrtvoltage_reset }

/*------------------------------------------------------------------------------
      Funtion prototypes
------------------------------------------------------------------------------*/                                               
void cnrtvoltage_calc(CNTRVOLTAGE_handle);
void cnrtvoltageLm_calc(CNTRVOLTAGE_handle);
void cnrtvoltageCurDown_calc(CNTRVOLTAGE_handle);
void cnrtvoltage_reset(CNTRVOLTAGE_handle);

#endif // __CNTRVOLTAGE_H__
