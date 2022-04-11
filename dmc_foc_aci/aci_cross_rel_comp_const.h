/* =================================================================================
File name:       ACI_CROSS_REL_COMP_CONST.H  (FLOAT version)                   
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the CROSSRELCOMPCONST.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 2.00
 02-04-2011 Version 2.10: New file name (late "CROSS_REL_COMP_CONST.H")
------------------------------------------------------------------------------*/

#ifndef __CROSSRELCOMPCONST_H__
#define __CROSSRELCOMPCONST_H__

#include "type.h"

typedef struct  { float  Rr;          // Input: Rotor resistance (ohm)    
                  float  Rs;          // Input: Stator resistance (ohm)  
                  float  Lr;          // Input: Rotor inductance (H)
                  float  Lm;          // Input: Magnetizing inductance (H) 
                  float  sigmaLs;     // Input: Stator inductance (H)
                  float  fb;          // Input: Base electrical frequency (Hz)
                  float  Ib;          // Input: Base Current (A)
                  float  Ub;          // Input: Base voltage (V)
                                        //
                  float  K1;          // Output: constant 
                  float  K2;          // Output: constant 
                  float  K3;          // Output: constant 
                  float  K4;          // Output: constant 
                  void   (*calc)();     // Pointer to calculation function
                } CROSSRELCOMPCONST;
                                                                                                                                                                                                                                                                                                                                                                                                        
typedef CROSSRELCOMPCONST *CROSSRELCOMPCONST_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the CROSSRELCOMPCONST object.
-----------------------------------------------------------------------------*/                     
#define CROSSRELCOMPCONST_DEFAULTS { 0,0,0,0,0,0,0,0,0,0,0,0, \
                               (void (*)(u32))cross_rel_comp_const }

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_CROSS_REL_COMP_CONST.C
------------------------------------------------------------------------------*/
void cross_rel_comp_const(CROSSRELCOMPCONST_handle);

#endif // __CROSSRELCOMPCONST_H__


