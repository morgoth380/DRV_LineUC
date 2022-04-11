/* =================================================================================
File Name:      FILTER_DEFINES.H
Description:    Filter defines
Developer:      Sikalov Yuiry 
Ts Idea:        20-06-2012 
Originator:     Triol Corporation
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 20-06-2012 Version 1.00
===================================================================================== */

#ifndef __FILTERDEFINES_H__
#define __FILTERDEFINES_H__

#define	float_Filtr(valOld, valNew, Kfiltr)    (valOld + ((valNew - valOld) / Kfiltr))

#define	s16_Filtr(valOut, valNew, valFilter, Kfiltr)  \
valFilter = (valFilter + (((((s32)valNew << 16) - valFilter) + (((s32)1<<Kfiltr)-1)) >> Kfiltr)); \
valOut = (s16)(valFilter >> 16);

#define	u16_Filtr(valOut, valNew, valFilter, Kfiltr)  \
valFilter = (valFilter + (((((s32)valNew << 16) - valFilter) + (((s32)1<<Kfiltr)-1)) >> Kfiltr)); \
valOut = (u16)(valFilter >> 16);

#define f32_Filtr(valOld, valNew, Kfiltr)(valOld + ((valNew - valOld)*(Kfiltr)))
#endif
