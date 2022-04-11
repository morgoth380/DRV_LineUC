/** =================================================================================
* \file		: minCurrCtrl.c
*
* \brief	: Модуль поддержания минимального полного тока
* \details	: включает функции инициализации и  расчета
*
* \autor	: Константин Гончаров
*
* \date		: 22.10.2016 \version	: 1.0
*
* \todo		:
*
* \copyright: (C)TRIOLCORP, 2016
*
=====================================================================================*/

#include "Global_Include.h"
#include "minCurrCtrl.h"

void MINCURRCTRL_init()
{
}

void MINCURRCTRL_calc(MINCURRCTRL_handle v)
{
    f32         Ampl;
    
    v->outId = v->inId;
    v->outIq = v->inIq;

    if(v->bitStart){      
      // Находим вектор полного тока
      arm_sqrt_f32(((v->inId * v->inId) + (v->inIq * v->inIq)), &Ampl);          
      
      if(Ampl < v->minI){  // Если модуль вектора тока меньше минимума
        arm_sqrt_f32(((v->minI * v->minI) - (v->inIq * v->inIq)), &v->outId); // Добавляем реактивного тока
      }
    }
}
