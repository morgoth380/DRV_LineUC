/************************** (C) Copyright 2012 Triol ***************************
* File Name          : MTZ_Time.h
* Author             : Alexey Partola
* TS Idea            : 2012.12.06
* TS Version         : 2012.12.06
* Description        : Файл содержит функцию, которая реализует алгоритм 
* блокировки пуска ПЧ на 1 минуту после возникновения аварии аппаратной МТЗ , 
* и на 10 минут, если в течении пяти минут МТЗ сработала 3 раза..
*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef MTZ_TIME_H
#define MTZ_TIME_H



/* Includes ------------------------------------------------------------------*/
#include "type.h"



/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
extern "C"{
#endif

#define NUMavarIMax     2   // Допустимое количество аварий по МТЗ

void MTZ_Time();

#ifdef __cplusplus
}
#endif



#endif // MTZ_TIME_H

/************************** (C) Copyright 2012 Triol **************************/
