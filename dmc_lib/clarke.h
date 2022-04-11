/* =================================================================================
File name:       CLARKE.H  (float version)                  
                    
Originator: Digital Control Systems Group
            Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the CLARKE.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 23-09-2020 PAN                                                   
------------------------------------------------------------------------------*/
#ifndef __CLARKE_H__
#define __CLARKE_H__

typedef struct {  float  As;        // Input: phase-a stator variable
                  float  Bs;        // Input: phase-b stator variable 
                  float  Cs;        // Input: phase-b stator variable
                  float  Alpha;     // Output: stationary d-axis stator variable 
                  float  Beta;      // Output: stationary q-axis stator variable
                  float  AlphaFiltr;     // Output: stationary d-axis stator variable 
                  float  BetaFiltr;      // Output: stationary q-axis stator variable                  
                  void  (*calc)();  // Pointer to calculation function
                 } CLARKE;              

typedef CLARKE *CLARKE_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the CLARKE object.
-----------------------------------------------------------------------------*/                     
#define CLARKE_DEFAULTS { 0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          (void (*)(u32))clarke_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in CLARKE.C
------------------------------------------------------------------------------*/
void clarke_calc(CLARKE_handle);

#endif // __CLARKE_H__
