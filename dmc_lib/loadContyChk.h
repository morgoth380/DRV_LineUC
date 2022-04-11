/** =================================================================================
* \file		: loadContyChk.h
*
* \brief	: Заголовочный файл модуля проверки целостности нагрузки
* \details	: включает типы данных, инициализационные константы, и функции
*
* \autor	: Константин Гончаров
*
* \date		: 02.10.2016 \version	: 1.0
*
* \copyright: (C)TRIOLCORP, 2016
*
=====================================================================================*/
#include "type.h"

#ifndef __LOADCONTYCHK_H__
#define __LOADCONTYCHK_H__

typedef struct {        f32  IdRefIn;           // Input:
			f32  IqRefIn;           // Input:
			f32  Iu;                // Input:
			f32  Iv;                // Input:
			f32  Iw;         	// Input:                        
			f32  filtrIu;           // Input:
			f32  filtrIv;           // Input:
			f32  filtrIw;         	// Input: 
			f32  ThetaIn;		// Input:
			f32  Ichk;		// Input:
			f32  Imin;		// Input:
                        f32  Fpwm;              // Input:
			u16  bitStart;          // Parameter:
			u16  chkState;          // Parameter:
			u16  timer;        	// Parameter:
                        f32  K1;                // Parametr - filter coefficient
                        f32  K2;                // Parametr - filter coefficient                        
			f32  timerTestTime;     // Время тестирования составляеи половину времени тестирования нагрузки из системы параметров
                        f32  IdRefOut;		// Output:
                        f32  IqRefOut;		// Output:
                        f32  ThetaOut;		// Output:
                        u16  bitForceSpd;       // Parameter:	///!!! тестируем форсироку рег. скорости (ваще это нужно сделать на уровне комамндоапарата)
                        u16  Error;             // Error - one of phase is brake
                        void  (*init)();        // Pointer to initialization function
                        void  (*calc)();        // Pointer to calculation function
                } LOADCONTYCHK_Obj;

//extern  LOADCONTYCHK_Obj loadContyChk;

typedef LOADCONTYCHK_Obj *LOADCONTYCHK_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the LOADCONTYCHK object.
-----------------------------------------------------------------------------*/                     
#define CONTYCHK_DEFS {         0,0,0,0,0,\
                                0,0,0,0,0,\
                                0,0,0,0,0,\
                                0,0,0,0,0,\
                                0,0,0,\
				(void (*)(u32))CONTYCHK_init,\
				(void (*)(u32))CONTYCHK_calc}

/*------------------------------------------------------------------------------
Prototypes for the functions in loadContyChk.c
------------------------------------------------------------------------------*/
void CONTYCHK_init();
void CONTYCHK_calc(LOADCONTYCHK_handle);

#endif
