/* =================================================================================
File name:       SPEED_LIM.H  (float version)                   
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the SPEED_LIM.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 28-02-2011 Version 1.00
 02-10-2020 Version 2.00  float (PAN)                                                    
------------------------------------------------------------------------------*/

#include "type.h"

typedef struct  { float IRm;           // Input: ток намагничивания ротора (pu) 
                  float Mez;           // Input: задание электромагнитного момента (pu) 
                  float WrElect;       // Input: электрическая скорость вращения ротора (pu) 
                  float Isqz;          // Output: задание момента ввиде тока (pu)
                  float K1;            // Parameter: constant     
                  float K2;            // Parameter: constant      
                  float WrL;           // Parameter: максимальнаЯ частота вращениЯ вала ротора длЯ моментного контура   (pu)  
                  float WrH;           // Parameter: максимальнаЯ частота вращениЯ вала ротора длЯ моментного контура   (pu)
                  float Wrz;	       // Parameter: задание частоты вращениЯ вала ротора длЯ моментного контура   (pu)
                  float OutMax;        // Parameter: ограничение   
                  void   (*calc)(); // Pointer to calculation function
                } SPEEDLIM;

typedef SPEEDLIM *SPEEDLIM_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the SPEEDLIM object.
-----------------------------------------------------------------------------*/                     
#define SPEEDLIM_DEFAULTS { 0,0,0,0, \
                       0,0,0,0,0,0, \
                       (void (*)(u32))speed_lim_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in SPEED_LIM.C
------------------------------------------------------------------------------*/
void speed_lim_calc(SPEEDLIM_handle);
