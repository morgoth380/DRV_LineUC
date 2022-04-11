/* ==============================================================================
System Name:    ACI_VECTOR_CONTROL (float version)

File Name:      ACI_VECTOR_CONTROL.H

Description:    Primary system header file for Sensored Indirect and Sensorless
                Field Orientation Control for a Three Phase AC Induction Motor. 
 
Developer:      Sikalov Y.
 
Originator:     Triol (based on Texas Instruments library)

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 22-12-2011 Version 1.00: Complete Sensored and Sensorless FOC for AC Induction Motor 
                          with autotuning regulator
 24-09-2020 PAN
=================================================================================  */

#ifndef __ACIVECTORCONTROL_H__
#define __ACIVECTORCONTROL_H__

#include "LIB_GLOBAL_DEFINE.h"
#include "DRIVE_CONST.h"
#include "DRIVE_WORDS.h"

// Global Variables
extern DRIVECONST  DrvConst;          // Структура обменная 
extern DRIVEWORDS  DrvWords;          // Структура управления и статуса

// Global Function prototype
void FOC_ReCalc_func(void);
void ACIM_Vector_Calc(void);
void ACIM_Vector_Init(void);
void DC_Brake_Calc(u16 DCTormSync, f32 dAngle, f32 DC_Current);



#endif 

