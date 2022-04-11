/* =================================================================================
File name:      ACI_SPEED_LIM_CONST.H  (float version)                   
                    
Originator:     Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the SPEED_LIM_CONST.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 28-02-2011 Version 1.00                                                   
 02-04-2011 Version 1.10: New file name (late "SPEED_LIM_CONST.H")      
 05-10-2020 Version 2.00: float (PAN)                                                   
------------------------------------------------------------------------------*/

#ifndef __SPEEDLIMCONST_H__
#define __SPEEDLIMCONST_H__

#include "type.h"

typedef struct  { float  Lr;          // Input: Rotor inductance (H)      
                  float  Lm;          // Input: Magnetizing inductance (H) 
                  float  WrL;         // Input: Low max rotor speed (Hz)  
                  float  WrH;         // Input: High max rotor speed (Hz)               
                  float  Zp;          // Input: число пар полюсов
                  float  Tb;          // Input: Base torgue (Nm)
                  float  Ib;          // Input: Base current (A)
                  float  Pb;          // Input: Base power (Вт)
                  float  fb;          // Input: Base frequance (Гц)

                  float  K1;          // Output: constant 
                  float  K2;          // Output: constant 
                  float  K3;          // Output: constant 
                  float  K4;          // Output: constant 
                  void   (*calc)(); // Pointer to calculation function
                } SPEEDLIMCONST;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
typedef SPEEDLIMCONST *SPEEDLIMCONST_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the SPEEDLIMCONST object.
-----------------------------------------------------------------------------*/                     
#define SPEEDLIMCONST_DEFAULTS { 0,     \
                                 0.181f,\
                                 0,     \
                                 0,     \
                                 1.0f,  \
                                 1.0f,  \
                                 1.0f,  \
                                 1.0f,  \
                                 1.0f,  \
                                 0,     \
                                 0,     \
                                 0,     \
                                 0,     \
                               (void (*)(u32))speed_lim_const_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_SPEED_LIM_CONST.C
------------------------------------------------------------------------------*/
void speed_lim_const_calc(SPEEDLIMCONST_handle);

#endif

