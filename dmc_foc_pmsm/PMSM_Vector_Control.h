/* ==============================================================================
System Name:    PMSM_Vector_Control (IQ version)

File Name:      PMSM_Vector_Control.H

Description:    Primary system header file for the Sensored Indirect and Sensorless
                Field Orientation Control for a Three Phase Permanent-Magnet Synchronous Motor. 
 
Developer:      Sikalov Y.
 
Originator:     Triol (based on Texas Instruments library)

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-12-2011 Version 1.00: Develop begin
=================================================================================  */

#ifndef __PMSMVECTORCONTROL_H__
#define __PMSMVECTORCONTROL_H__

#include "LIB_GLOBAL_DEFINE.h"
#include "DRIVE_CONST.h"
#include "DRIVE_WORDS.h"

// Global Variables
extern DRIVECONST  DrvConst; // Структура обменная 
extern DRIVEWORDS  DrvWords; // Структура управления и статуса

// Global Function prototype
void PMSM_Vector_Init(void);
void PMSM_Vector_Calc(void);

#endif

