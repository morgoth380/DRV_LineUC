/* =================================================================================
File name:       PARK.H (float version)                    
                    
Originator: Digital Control Systems Group
            Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PARK.

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 23-09-2020 PAN                                                  
------------------------------------------------------------------------------*/
#ifndef __PARK_H__
#define __PARK_H__

typedef struct {  float  Alpha;     // Input: stationary d-axis stator variable 
                  float  Beta;      // Input: stationary q-axis stator variable 
                  float  Angle;     // Input: rotating angle (pu) 
                  float  Ds;        // Output: rotating d-axis stator variable 
                  float  Qs;        // Output: rotating q-axis stator variable 
                  void  (*calc)();  // Pointer to calculation function
                 } PARK;                

typedef PARK *PARK_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PARK object.
-----------------------------------------------------------------------------*/                     
#define PARK_DEFAULTS {  0.0f, \
                         0.0f, \
                         0.0f, \
                         0.0f, \
                         0.0f, \
                         (void (*)(u32))park_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in PARK.C
------------------------------------------------------------------------------*/
void park_calc(PARK_handle);

#endif // __PARK_H__
