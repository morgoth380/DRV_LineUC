/* =================================================================================
File name:       ACI_CROSS_REL_COMP.H  (float version)                   
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the CROSSRELCOMP.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 2.00
 02-04-2011 Version 2.10: New file name (late "CROSS_REL_COMP.H") 
 02-10-2020 Version 3.00: float (PAN)                                                  
 ------------------------------------------------------------------------------*/

#ifndef __CROSSRELCOMP_H__
#define __CROSSRELCOMP_H__

#include "type.h"

typedef struct  { float IRm;           // Input: потокосцепление ротора (pu)
                  float Isd;           // Input: проекция тока на ось d вращ. сист. коорд.(pu)
                  float Isq;           // Input: проекция тока на ось q вращ. сист. коорд.(pu)
                  float We;            // Input: скорость вращения потокосцепления ротора (pu)
                  float Wr;            // Input: скорость вращения ротора (pu)
                  float Urd;           // Input: выход регулятора тока Id (pu)
                  float Urq;           // Input: выход регулятора тока Iq (pu)
                  float Usmax;         // Input: максимальное занчение вектора напряжения (pu)
                  float Usd;           // Output: компенсированное и ограниченное напряжение по оси d (pu)
                  float Usq;           // Output: компенсированное и ограниченное напряжение по оси q (pu)
                  float Us;            // Output: результирующий вектор напряжения (pu)
                  float K1;            // Parameter: constant using in cross-connection calculation     
                  float K2;            // Parameter: constant using in cross-connection calculation      
                  float K3;            // Parameter: constant using in cross-connection calculation     
                  float K4;            // Parameter: constant using in cross-connection calculation
                  u16   UsmaxFlag;
                  void (*calc)();    // Pointer to calculation function
                } CROSSRELCOMP;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
typedef CROSSRELCOMP *CROSSRELCOMP_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the CROSSRELCOMP object.
-----------------------------------------------------------------------------*/                     
#define CROSSRELCOMP_DEFAULTS { 0,0,0, \
                       0,0,0,0,0,0,0, \
                       0,0,0,0,0,0, \
                       (void (*)(u32))cross_rel_comp }

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_CROSS_REL_COMP.C
------------------------------------------------------------------------------*/
void cross_rel_comp(CROSSRELCOMP_handle);

#endif // __CROSSRELCOMP_H__


