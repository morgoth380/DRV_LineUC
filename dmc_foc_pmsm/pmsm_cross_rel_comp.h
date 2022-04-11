/* =================================================================================
File name:       PMSM_cross_rel_comp.H  (IQ version)                   
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PMSM_cross_rel_comp.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-12-2011 Version 1.00                                                   
------------------------------------------------------------------------------*/

#ifndef __PMSMCROSSRELCOMP_H__
#define __PMSMCROSSRELCOMP_H__

#include "type.h"

typedef struct  { 
                  f32 Isd;           // Input: проекция тока на ось d вращ. сист. коорд.(pu)
                  f32 Isq;           // Input: проекция тока на ось q вращ. сист. коорд.(pu)
                  f32 Wr;            // Input: скорость вращения ротора (pu)
                  f32 Urd;           // Input: выход регулятора тока Id (pu)
                  f32 Urq;           // Input: выход регулятора тока Iq (pu)
                  f32 Usmax;         // Input: максимальное занчение вектора напряжения (pu)
                  f32 Ucd;           // Output: компенсирующие напряжение по оси d (pu)
                  f32 Ucq;           // Output: компенсирующие напряжение по оси q (pu)
                  f32 Usd;           // Output: компенсированное и ограниченное напряжение по оси d (pu)
                  f32 Usq;           // Output: компенсированное и ограниченное напряжение по оси q (pu)
                  f32 Us;            // Output: результирующий вектор напряжения (pu)
                  f32 K1;            // Parameter: Calculation parameter     
                  f32 K2;            // Parameter: Calculation parameter      
                  f32 K3;            // Parameter: Calculation parameter      
                  u16 UsmaxFlag;     // Cross overload flag
                  void   (*calc)();    // Pointer to calculation function
                } PMSMCROSSRELCOMP;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
typedef PMSMCROSSRELCOMP *PMSMCROSSRELCOMP_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the PMSMCROSSRELCOMP object.
-----------------------------------------------------------------------------*/                     
#define PMSMCROSSRELCOMP_DEFAULTS {  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, \
                                     (void (*)(u32))pmsm_cross_rel_comp }

/*------------------------------------------------------------------------------
Prototypes for the functions in PMSM_cross_rel_comp.C
------------------------------------------------------------------------------*/
void pmsm_cross_rel_comp(PMSMCROSSRELCOMP_handle);

#endif 
