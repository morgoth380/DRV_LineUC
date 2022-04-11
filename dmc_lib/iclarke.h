/* =================================================================================
File name:  ICLARKE.H  (float version)                  
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the ICLARKE.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 03-04-2012 Version 1.00
 23-09-2020 PAN                                                   
------------------------------------------------------------------------------*/

#ifndef __ICLARKE_H__
#define __ICLARKE_H__

typedef struct {  
                float  Alpha;     // Output: stationary d-axis stator variable 
                float  Beta;      // Output: stationary q-axis stator variable
                float  As;        // Input: phase-a stator variable
                float  Bs;        // Input: phase-b stator variable 
                float  Cs;        // Input: phase-c stator variable 
                void  (*calc)();  // Pointer to calculation function
               } ICLARKE;              

typedef ICLARKE *ICLARKE_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the ICLARKE object.
-----------------------------------------------------------------------------*/                     
#define ICLARKE_DEFAULTS { 0.0f,0.0f,0.0f,0.0f,0.0f, \
                          (void (*)(u32))iclarke_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in ICLARKE.C
------------------------------------------------------------------------------*/
void iclarke_calc(ICLARKE_handle);

#endif

