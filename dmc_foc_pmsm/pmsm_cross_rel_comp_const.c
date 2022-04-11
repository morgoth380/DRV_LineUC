/*=====================================================================================
 File name:        PMSM_cross_rel_comp_CONST.C  (IQ version)                  
                    
 Originator:    Triol

 Description:  cross-relation compansation constant for PMSM

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 15-12-2011 Version 1.00
-------------------------------------------------------------------------------------*/

#include "type.h"
#include "Pjk_const.h"
#include "pmsm_cross_rel_comp_const.h"

void pmsm_cross_rel_comp_const(PMSMCROSSRELCOMPCONST * v)
{
    f32 tmp; 

    tmp = 2 * _PI * v->fb / v->Ub;

    v->K1 = tmp * v->Ib * v->Lsq;
    v->K2 = tmp * v->Ib * v->Lsd;
    v->K3 = tmp * v->PsiR;

    // умножение на SQRT3, для приведения к линейному напряжению 
    v->K1 = v->K1;// * SQRT3;
    v->K2 = v->K2;// * SQRT3;
    v->K3 = v->K3;// * SQRT3;
}

