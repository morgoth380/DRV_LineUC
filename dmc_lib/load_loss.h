/* =================================================================================
File name:      PMSM_BANDSTOP_FILTER.H   (IQ version)
                     
Originator:     Triol / Sosnovchik
URL: 			http://www.dsplib.ru/content/notch/notch.html

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the LOADLOSS.
====================================================================================
 History:
-------------------------------------------------------------------------------------
 22-12-2011 Version 1.00                                                
------------------------------------------------------------------------------*/

#ifndef __LOADLOSS_H__
#define __LOADLOSS_H__

typedef struct {  
                 u16  	timer;
                 f32    estimateTheta;
                 f32    estimateThetaTmp;
                 f32    prevTheta;
                 f32    Fout;
                 f32    dTheta;
                 f32    Tpwm;
                 u16    PwmOn;
                 f32    currentThetaI;
                 u16    cntFault;
                 u16    Fault;
                 void  	(*calc)();   // Pointer to calculation function
                 void  	(*init)();   // Pointer to reset function
                 } LOADLOSSmodule;

typedef LOADLOSSmodule *LOADLOSSmodule_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the LOADLOSS object.
-----------------------------------------------------------------------------*/                     
#define LOADLOSS_DEFAULTS {     0,0,0,0,0,\
				0,0,0,0,0,\
                                0,\
                                (void (*)(u32))LoadLoss_calc, \
                                (void (*)(u32))LoadLoss_init }

/*------------------------------------------------------------------------------
Prototypes for the functions in loadloss.C
------------------------------------------------------------------------------*/
void LoadLoss_calc(LOADLOSSmodule_handle);
void LoadLoss_init(LOADLOSSmodule_handle);

extern LOADLOSSmodule	loadloss1;
#endif

