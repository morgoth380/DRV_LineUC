/* ==============================================================================
System Name:    PMSM_PRM_MOT (floats version)

File Name:      PMSM_PRM_MOT.H

Description:    Triol

Originator:     Triol

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 26-04-2011 Version 1.00
 21-03-2012 Version 2.00 
 28-01-2022 Version 3.00 
=================================================================================  */

#ifndef __PMSMPRMMOT_H__
#define __PMSMPRMMOT_H__

#include "LIB_GLOBAL_DEFINE.h"
#include "DRIVE_CONST.h"
#include "DRIVE_WORDS.h"

// Global Variables
extern DRIVECONST  DrvConst; // Структура обменная 
extern DRIVEWORDS  DrvWords; // Структура управления и статуса


// Global Function prototype
void PMSM_PrmMot_Init();
void PMSM_PrmMot_Calc();

#endif

