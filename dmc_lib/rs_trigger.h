/* =================================================================================
File name:  RS_TRIGGER.H  (IQ version)
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the RS_TRIGGER.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 03-04-2012 Version 1.00                                                   
------------------------------------------------------------------------------*/

#ifndef __RS_TRIGGER_H__
#define __RS_TRIGGER_H__

#include "type.h"

typedef struct {  
                u16    Set;         // Input: set input
                u16    Reset;       // Input: Reset input
                float  ResetTime;   // Input: Time to Auto Reset
                float  Fpwm;        // Input: PWM frequency
                u16    Out;         // Input: Out
                u16    NotOut;      // Input: Invert Out
                u32    ResetCnt;    // Variable: internal counter
                u16    OneTimeSet;  // Вход одиночной установки
                u16    OneTimeFix;
                void  (*calc)();  // Pointer to calculation function
                void  (*reset)(); // Pointer to calculation function
               } RS_TRIGGER;

typedef RS_TRIGGER *RS_TRIGGER_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the RS_TRIGGER object.
-----------------------------------------------------------------------------*/                     
#define RS_TRIGGER_DEFAULTS { 0,0,0,0,0,\
			      0,0,0,0, \
                              (void (*)(u32))rs_trigger_calc,\
	                      (void (*)(u32))rs_trigger_reset}

/*------------------------------------------------------------------------------
Prototypes for the functions in RS_TRIGGER.C
------------------------------------------------------------------------------*/
void rs_trigger_calc (RS_TRIGGER_handle);
void rs_trigger_reset(RS_TRIGGER_handle);

#endif

