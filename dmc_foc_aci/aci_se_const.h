/* =================================================================================
File name:       ACI_SE_CONST.H                     
                    
Originator: Digital Control Systems Group
            Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the ACISE_CONST.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
------------------------------------------------------------------------------*/

#include "type.h"

typedef struct  { float  Rr;              // Input: Rotor resistance (ohm) 
                  float  Lr;              // Input: Rotor inductance (H) 
                  float  fb;              // Input: Base electrical frequency (Hz)
                  float  fc;              // Input: Cut-off frequency of lowpass filter (Hz)
                  float  Ts;              // Input: Sampling period in sec 
                  float  TcutSpd;         // Постоянная среза контура скорости из параметров      
                  float  K1;              // Output: constant using in rotor flux calculation 
                  float  K2;              // Output: constant using in rotor flux calculation 
                  float  K3;              // Output: constant using in rotor flux calculation 
                  float  K4;              // Output: constant using in stator current calculation 
                  float  K5;              // Output: constant using in rotor position calculation
                  float  K8;              // Output: Коэф. фильтра выходной скорости N2
                  float  K9;              // Output: Коэф. фильтра выходной скорости N2                  
                  float  Tc;              // постоянная полосы пропускания

                  void   (*calc)();         // Pointer to calculation function 
                } ACISE_CONST;
                                                                                                                                                                                                                                                                                                                                                                                                    
typedef ACISE_CONST *ACISE_CONST_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the ACISE_CONST object.
-----------------------------------------------------------------------------*/                     
#define ACISE_CONST_DEFAULTS {0,0,0, \
                             0,0,0,0,0, \
                             0,0,0,0,0,0, \
                            (void (*)(u32))aci_se_const_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_SE_CONST.C
------------------------------------------------------------------------------*/
void aci_se_const_calc(ACISE_CONST_handle);
