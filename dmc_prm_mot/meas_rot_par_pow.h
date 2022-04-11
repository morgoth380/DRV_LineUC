/* =================================================================================
File name:      MEASROTPARPOW.H  (float)                   
                    
Originator:	Triol

Description: 
This file contains type definitions, constants and initializers for
the ramp generation functions contained in MEASROTPARPOW.C
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 11-06-2011 Version 1.00
 04-11-2020 Version 2.00  PAN (float)                                                
------------------------------------------------------------------------------*/

#ifndef __MEASROTPARPOW_H__
#define __MEASROTPARPOW_H__

#include "type.h"

typedef struct { float  Uline;      // Input: output line voltage (pu) 
                 float  Ifaz;       // Input: full current(pu)
                 float  UlineFlash; // Input: output line flash voltage (pu) 
                 float  IfazFlash;  // Input: Flash current (pu)
                 float  Wout;       // Input: output cyclic frequence (pu)
                 float  Rs;         // Input: Stator resistance (ohm)
                 u16    Period;     // Input: Period of Output voltage 
                 float  Rr;         // Output: Rotor resistance (ohm)
                 float  sLr;        // Output: Rotor inductance (H)
                 float  K1;         // Parameter: 
                 float  K2;         // Parameter: 
                 float  Pact;       // Variable:
                 float  Pact1;      // Variable:
                 float  PwrSum;     // Variable:  
                 float  Zeq;        // Variable:       
                 float  Req;        // Variable:
                 u32  Counter;      // Variable:
                 void  (*calc)(); // Pointer to calculation function
                 } MEASROTPARPOW;          

typedef MEASROTPARPOW *MEASROTPARPOW_handle;  
/*------------------------------------------------------------------------------
      Object Initializers
------------------------------------------------------------------------------*/                       
#define MEASROTPARPOW_DEFAULTS {0,0,0,0,0,0,0,0,0, \
                               (380.0f / (3.0f * 15.0f * 80.0f)), \
                               (380.0f * 2.0f/(3.0f * 15.0f)), \
                                0,0,0,0,0,0, \
                             (void (*)(u32))measrotparpow_calc }

/*------------------------------------------------------------------------------
      Funtion prototypes
------------------------------------------------------------------------------*/                                               
void measrotparpow_calc(MEASROTPARPOW_handle);

#endif // __MEASROTPARPOW_H__
