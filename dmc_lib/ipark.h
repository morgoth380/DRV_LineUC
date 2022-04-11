/* =================================================================================
File name:       IPARK.H   (float version)                  
                    
Originator: Digital Control Systems Group
            Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the IPARK.

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20         
 23-09-2020 PAN                                         
------------------------------------------------------------------------------*/
#ifndef __IPARK_H__
#define __IPARK_H__

typedef struct {  float  Alpha;     // Output: stationary d-axis stator variable
                  float  Beta;      // Output: stationary q-axis stator variable
                  float  Angle;     // Input: rotating angle (pu)
                  float  Ds;        // Input: rotating d-axis stator variable
                  float  Qs;        // Input: rotating q-axis stator variable
                  void  (*calc)();  // Pointer to calculation function 
                 } IPARK;               

typedef IPARK *IPARK_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the IPARK object.
-----------------------------------------------------------------------------*/                     
#define IPARK_DEFAULTS {  0.0f, \
                          0.0f, \
                          0.0f, \
                          0.0f, \
                          0.0f, \
                          (void (*)(u32))ipark_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in IPARK.C
------------------------------------------------------------------------------*/
void ipark_calc(IPARK_handle);

#endif // __IPARK_H__

