/** =================================================================================
* \file		: minCurrCtrl.h
*
* \brief	: Заголовочный файл модуля поддержания минимального полного тока
* \details	: включает типы данных, инициализационные константы, и функции
*
* \autor	: Константин Гончаров
*
* \date		: 22.10.2016 \version	: 1.0
*
* \copyright: (C)TRIOLCORP, 2016
*
=====================================================================================*/

#ifndef __MINCURRCTRL_H__
#define __MINCURRCTRL_H__

#include "type.h"

typedef struct {  f32   inId;           // Input:
                  f32   inIq;           // Input:
                  f32   minI;           // Input:
                  u16   bitStart;       // Parameter:
                  f32   outId;		// Output:
                  f32   outIq;		// Output:
                  void  (*init)();      // Pointer to initialization function
                  void  (*calc)();      // Pointer to calculation function
                } MINCURRCTRL_Obj;

extern  MINCURRCTRL_Obj minCurrCtrl;

typedef MINCURRCTRL_Obj *MINCURRCTRL_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the MINCURRCTRL object.
-----------------------------------------------------------------------------*/                     
#define MINCURRCTRL_DEFS {   0,0,0,0,0,0,\
                            (void (*)(u32))MINCURRCTRL_init,\
                            (void (*)(u32))MINCURRCTRL_calc}

/*------------------------------------------------------------------------------
Prototypes for the functions in minCurrCtrl.c
------------------------------------------------------------------------------*/
void MINCURRCTRL_init();
void MINCURRCTRL_calc(MINCURRCTRL_handle);

#endif


