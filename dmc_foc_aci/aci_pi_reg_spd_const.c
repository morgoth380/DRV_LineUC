/*=====================================================================================
 File name:     ACI_PI_REG_SPD_CONST.C                   
                    
 Originator:    Triol

 Developer:     Sikalov Y.Y. 

 Description:  The PI speed controller adjuction                 

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 11-11-2011 Version 2.00
 02-04-2011 Version 2.10: New file name (late "PI_REG_SPD_CONST.C") 
 02-10-2020 Version 3.0: float (PAN)                                                   
-------------------------------------------------------------------------------------*/
#include "Global_Include.h"
#include "aci_pi_reg_spd_const.h"
#include "Pjk_const.h"

void pi_reg_spd_const(PIREGSPDCONST *v)
{
    float Kb, tmp; 

    Kb = v->fb / (v->Ib * v->Lm * v->Ib); // коэффициент пересчета баз

    if (v->ManualEnb) {
        v->Kp = v->Kp_manual;
        v->Ki = v->Tspd * v->Kp_manual / v->Ki_manual;
    }
    else {
        v->Kp = v->Lr * v->Jr / (3.0f * v->Lm * v->Zp * v->Zp * v->Tsl * v->PrNom) * Kb;
        v->Ki = 4.0f * v->Tsl * 10.0f;
        v->Ki = v->Tspd * v->Kp / v->Ki;
    }

    // защита от переполнения
    if (v->Kp > 127.0f) v->Kp = 127.0f;
    if (v->Ki > 127.0f) v->Ki = 127.0f;
    //==================================
    v->Kp_manual = v->Kp;
    v->Ki_manual = v->Tspd * v->Kp / v->Ki;
    //---
    v->Kp_norm = v->Kp; 
    v->Ki_norm = v->Ki;

    // расчет ограничения регулятора скорости
    v->OutMax = v->Imax * _SQRT_2;
    
    // вычисление коэффициентов входного фильтра
    v->K1 = expf(- v->Tspd / (4.0f * v->Tsl));
    v->K2 = 1.0f - v->K1;

    // K3 умножается на ошибку регулятора K3,          
    // для уменьшения величин коэффициентов регулятора 
    v->K3 = _2PI; 

    // расчет коэффициентов корректирующего звена
    tmp = 1.0f / (v->Tsl + v->Tspd);
    v->K4 = (2.0f * v->Tcl + v->Tspd) * tmp;
    v->K5 =  2.0f * v->Tcl * tmp;
    v->K6 =  v->Tsl * tmp;   
    v->K7 = 1.0f / (2.0f * _PI * v->Fc_spd * v->Tspd);  
}

