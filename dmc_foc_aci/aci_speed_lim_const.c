/*=====================================================================================
 File name:     ACI_SPEED_LIM_CONST.C  (float version)                  
                    
 Originator:    Triol

 Description:   Speed limiter constant            

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 03-02-2011 Version 1.00
 02-04-2011 Version 1.10: New file name (late "SPEED_LIM_CONST.C")
 05-10-2020 Version 2.00: float (PAN)                                                   
-------------------------------------------------------------------------------------*/


#include "aci_speed_lim_const.h"

void speed_lim_const_calc(SPEEDLIMCONST * v)
{
    v->K1 = 1.0f / (v->WrH - v->WrL);
    v->K2 = 2.0f * v->Lr * v->Tb / (3.0f * v->Zp * v->Lm * v->Lm * v->Ib * v->Ib);
    v->K3 = 1.0f / v->K2;
    v->K4 = v->fb * v->Tb / v->Pb;
}

