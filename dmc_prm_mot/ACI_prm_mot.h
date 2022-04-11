/* ==============================================================================
System Name:    ACI_PRM_MOT (IQ version)

File Name:      ACI_PRM_MOT.H

Description:    Triol

Originator:     Triol

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 26-04-2011 Version 1.00
 21-03-2012 Version 2.00 
=================================================================================  */

#ifndef __ACIPRMMOT_H__
#define __ACIPRMMOT_H__

#include "LIB_GLOBAL_DEFINE.h"
#include "DRIVE_CONST.h"
#include "DRIVE_WORDS.h"

// Global Variables
extern DRIVECONST  DrvConst; // Структура обменная
extern DRIVEWORDS  DrvWords; // Структура управления и статуса


// Global Function prototype
void ACIM_PrmMot_Init();
void ACIM_PrmMot_Calc();

#endif

