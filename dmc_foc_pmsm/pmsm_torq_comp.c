/** =================================================================================
* \file		: pmsm_torq_comp.c
*
* \brief	: Реализация модуля компенсации реактивной составляющей ЭМ. момента
* \details	: включает функции инициализации и расчета модуля.
*
* \autor	: Константин Гончаров
*
* \date		: 31.08.2016 \version	: 1.0
*
* \todo		: 1. 
*
* \copyright: (C)TRIOLCORP, 2016
*
=====================================================================================*/

#include "pmsm_torq_comp.h"

void pmsm_torqcomp_calc(PMSMTORQCOMP_handle v)
{
    v->outId = v->inId;
    v->outIq = v->inIq;

    if(v->K1 != 0.0){ // (v->Lsd != v->Lsq)
        // вычисление компенсирующего значения
        v->Kc = v->K1 * v->currD + 1.0f;
        // компесанция
        v->outIq = (v->inIq / v->Kc);
    }
}

void pmsm_torqcomp_init(PMSMTORQCOMP_handle v)
{
    v->K1 = v->Ib * (v->Lsd - v->Lsq) / v->PsiR;
}
