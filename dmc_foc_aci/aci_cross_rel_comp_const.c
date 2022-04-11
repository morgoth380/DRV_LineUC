/*=====================================================================================
 File name:     ACI_CROSS_REL_COMP_CONST.C  (IQ version)                  
                    
 Originator:    Triol

 Description:   cross-relation compensation constant            

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 2.00
 02-04-2011 Version 2.10: New file name (late "CROSS_REL_COMP_CONST.C")
-------------------------------------------------------------------------------------*/

#include "aci_cross_rel_comp_const.h"
#include "Pjk_const.h"


void cross_rel_comp_const(CROSSRELCOMPCONST * v)
{
    float LrB,RB,LsB;

    LsB = v->Ub / (2.0f * _PI * v->fb * v->Ib);
    LrB = v->Lm;
    RB = v->Ub / v->Ib;
    v->K1 = v->Lm * v->Rr / (v->Lr * v->Lr) * LrB / RB;
    v->K2 = v->sigmaLs / LsB;
    v->K3 = v->Lm / v->Lr * LrB / LsB;
}

