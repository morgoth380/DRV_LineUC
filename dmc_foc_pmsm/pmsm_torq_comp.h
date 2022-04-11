/** =================================================================================
* \file		: pmsm_torq_comp.h
*
* \brief	: Заголовочный файл модуля компенсации реактивной составляющей ЭМ. момента
* \details	: включает типы данных, инициализационные константы, и функции
* 			  объекта модуля компенсации
*
* \autor	: Константин Гончаров
*
* \date		: 31.08.2016 \version	: 1.0
*
* \copyright: (C)TRIOLCORP, 2016
*
=====================================================================================*/

#ifndef __PMSMTORQCOMP_H__
#define __PMSMTORQCOMP_H__

#include "type.h"

typedef struct {
                f32     currD;     // Input: мгновенное значение тока по оси d вращ. сист. коорд.(pu)
                f32     inId;      // Input: задание тока по оси d вращ. сист. коорд.(pu)
                f32     inIq;      // Input: задание тока по оси q вращ. сист. коорд.(pu)
                f32     Lsd;       // Input: Magnetizing inductance (H)
                f32     Lsq;       // Input: Stator inductance (H)
                f32     PsiR;      // Input: Flux rotor constant (Wb)
                f32     Ib;        // Input: Base Current (A)
                f32     Kc;        // Parameter: 	корректирующее значение
                f32     K1;        // Parameter: 	корректирующий коэффициент
                f32     outId;     // Output:
                f32     outIq;     // Output: 	compensated output
                void  (*calc)();   // Pointer to calculation function
                void  (*init)();   // Pointer to reset function
               } PMSMTORQCOMP_Obj;

extern  PMSMTORQCOMP_Obj pmsmTORQcomp;

typedef PMSMTORQCOMP_Obj * PMSMTORQCOMP_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PMSMTORQCOMP object.
-----------------------------------------------------------------------------*/                     
#define PMSMTORQCOMP_DEFAULTS {    0,0,0,0,0,0,0,0,0,0,0,               \
                         	   (void (*)(u32))pmsm_torqcomp_calc,\
                                   (void (*)(u32))pmsm_torqcomp_init }
/*------------------------------------------------------------------------------
Prototypes for the functions
------------------------------------------------------------------------------*/
void pmsm_torqcomp_calc(PMSMTORQCOMP_handle);
void pmsm_torqcomp_init(PMSMTORQCOMP_handle);

#endif /* __PMSMTORQCOMP_H__ */


