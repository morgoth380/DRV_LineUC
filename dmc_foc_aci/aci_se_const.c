/*=====================================================================================
 File name:        ACI_SE_CONST.C                   
                    
 Originator:    Digital Control Systems Group
            Texas Instruments

 Description:  Open-loop Speed Estimator of Induction Motor Constants                 

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 01-10-2020 Version 4.0 PAN
-------------------------------------------------------------------------------------*/

#include "aci_se_const.h"
#include "Pjk_const.h"

void aci_se_const_calc(ACISE_CONST *v)
{
   float Wb, Tr, tc;
   
   Tr = v->Lr / v->Rr;          // Rotor time constant (sec)
   tc = 1.0f / (2.0f * _PI * v->fc);   // Lowpass filter time constant (sec)
   Wb = 2.0f * _PI * v->fb;
   
   v->K1 = 1.0f / (Wb * Tr); 
   v->K2 = 1.0f /(v->fb * v->Ts * _2PI);  
   v->K3 = tc / (tc + v->Ts);
   v->K4 = v->Ts / (tc + v->Ts);
   v->K5 = v->fb * v->Ts;  
   v->K8 = v->TcutSpd / (v->TcutSpd + v->Ts);
   v->K9 = v->Ts / (v->TcutSpd + v->Ts);      
   v->Tc = tc; 
}


