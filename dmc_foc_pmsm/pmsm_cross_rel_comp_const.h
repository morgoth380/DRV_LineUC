/* =================================================================================
File name:       PMSM_cross_rel_comp_CONST.H  (FLOAT version)                   
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PMSM_cross_rel_comp_CONST.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 15-12-2011 Version 1.00                                                   
------------------------------------------------------------------------------*/

#ifndef __PMSMCROSSRELCOMPCONST_H__
#define __PMSMCROSSRELCOMPCONST_H__

#include "type.h"

typedef struct  {     
                  f32  Lsd;         // Input: Magnetizing inductance (H) 
                  f32  Lsq;         // Input: Stator inductance (H)
                  f32  PsiR;        // Input: Flux rotor constant (Wb)
                  f32  fb;          // Input: Base electrical frequency (Hz)
                  f32  Ib;          // Input: Base Current (A)
                  f32  Ub;          // Input: Base voltage (V)
                                    //
                  f32  K1;          // Output: constant 
                  f32  K2;          // Output: constant 
                  f32  K3;          // Output: constant 
                  void   (*calc)();     // Pointer to calculation function
                } PMSMCROSSRELCOMPCONST;
                                                                                                                                                                                                                                                                                                                                                                                                        
typedef PMSMCROSSRELCOMPCONST *PMSMCROSSRELCOMPCONST_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PMSM_cross_rel_comp object.
-----------------------------------------------------------------------------*/                     
#define PMSMCROSSRELCOMPCONST_DEFAULTS { 0,0,0,0,0,0,0,0,0, \
                               (void (*)(u32))pmsm_cross_rel_comp_const }

/*------------------------------------------------------------------------------
Prototypes for the functions in PMSM_cross_rel_comp.C
------------------------------------------------------------------------------*/
void pmsm_cross_rel_comp_const(PMSMCROSSRELCOMPCONST_handle);

#endif

