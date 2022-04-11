/*=====================================================================================
 File name:        ACI_FE_CONST.C                   
                    
 Originator:    Digital Control Systems Group
            Texas Instruments

 Description:  Flux Estimator of Induction Motor Constants                 

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
-------------------------------------------------------------------------------------*/

#include "aci_fe_const.h"

void aci_fe_const_calc(ACIFE_CONST *v)
{
   float Tr;
   
   Tr = v->Lr/v->Rr;                             // Rotor time constant (sec)

   v->K1 = Tr / (Tr + v->Ts);
   v->K2 = v->Ts / (Tr + v->Ts); 
   v->K3 = v->Lm / v->Lr;
   v->K4 = (v->Ls * v->Lr - v->Lm * v->Lm) / (v->Lr * v->Lm);   
   v->K5 = v->Ib * v->Rs / v->Vb; 
   v->K6 = v->Vb * v->Ts / (v->Lm * v->Ib); 
   v->K7 = v->Lr / v->Lm;   
   v->K8 = (v->Ls * v->Lr - v->Lm * v->Lm) / (v->Lm * v->Lm);
   v->Tr = Tr;
}


