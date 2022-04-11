/* =================================================================================
File name:       ACI_FE_CONST.H                     
                    
Originator: Digital Control Systems Group
            Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the ACIFE_CONST.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
------------------------------------------------------------------------------*/

#include "type.h"

typedef struct  { float  Rs;              // Input: Stator resistance (ohm) 
                  float  Rr;              // Input: Rotor resistance (ohm) 
                  float  Ls;              // Input: Stator inductance (H)     
                  float  Lr;              // Input: Rotor inductance (H) 
                  float  Lm;              // Input: Magnetizing inductance (H)
                  float  Ib;              // Input: Base phase current (amp) 
                  float  Vb;              // Input: Base phase voltage (volt) 
                  float  Ts;              // Input: Sampling period in sec 
                  float  K1;              // Output: constant using in rotor flux calculation  
                  float  K2;              // Output: constant using in rotor flux calculation  
                  float  K3;              // Output: constant using in rotor flux calculation 
                  float  K4;              // Output: constant using in stator current calculation  
                  float  K5;              // Output: constant using in stator current calculation  
                  float  K6;              // Output: constant using in stator current calculation  
                  float  K7;              // Output: constant using in stator current calculation  
                  float  K8;              // Output: constant using in torque calculation
                  float  Tr;              // Output: Rotor Time
                  void   (*calc)();  // Pointer to calculation function 
                } ACIFE_CONST;

typedef ACIFE_CONST *ACIFE_CONST_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the ACIFE_CONST object.
-----------------------------------------------------------------------------*/                     
#define ACIFE_CONST_DEFAULTS {0,0,0,0,0, \
                             0,0, \
                             0, \
                             0,0,0,0,0,0,0,0,0, \
                            (void (*)(u32))aci_fe_const_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_FE_CONST.C
------------------------------------------------------------------------------*/
void aci_fe_const_calc(ACIFE_CONST_handle);
