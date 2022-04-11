/* =================================================================================
File name:       MEAS_RS.H  (float version)                   
                    
Originator:	Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the MEASRS.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 26-04-2011	Version 1.00 
 19-10-2020	Version 2.00 PAN (float)                                                  
------------------------------------------------------------------------------*/
#ifndef __MEASRS_H__
#define __MEASRS_H__

#include "type.h"

typedef struct  { float  Vin;          // Input: liner voltage
                  float  Iin;          // Input: motor nominal current 
                  float  Rs;           // Output: calculated stator resistance 
                  float  K1;           // Parameter: VoltBASE/CurrBASE
                  float Vin_f ;        // Variable:
                  float Iin_f ;        // Variable:
                  float Rs_tmp;        // Variable:
                  void (*calc)();   // Pointer to calculation function
                  void (*reset)();   // Pointer to calculation function
                } MEASRS;

typedef MEASRS *MEASRS_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the MEASRS object.
-----------------------------------------------------------------------------*/               

#define MEASRS_DEFAULTS { 0,                               \
                          0,                               \
                          0,                               \
                          0,                               \
                          0,                               \
                          0,                               \
                          0,                               \
                          (void (*)(u32))meas_rs_calc,     \
                          (void (*)(u32))meas_rs_reset }

/*------------------------------------------------------------------------------
Prototypes for the functions in MEASRS.C
------------------------------------------------------------------------------*/
void meas_rs_calc(MEASRS_handle);
void meas_rs_reset(MEASRS_handle);

#endif
