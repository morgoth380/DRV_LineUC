/*=====================================================================================
 File name:        PMSM_PI_REG_SPD_CONST.C                   
                    
 Originator:    Triol

 Developer: Sikalov Y.Y. 

 Description:  The PI speed controller adjuction for PMSM

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-12-2011 Version 1.00
 25-02-2016 Version 1.01: manual/automatic + forced
-------------------------------------------------------------------------------------*/

#include "pmsm_pi_reg_spd_const.h"
#include "Pjk_const.h"
#include "type.h"
#include "math.h"

void pmsm_pi_reg_spd_const(PMSMPIREGSPDCONST *v)
{
    f32 Kb, tmp; 

    Kb = v->fb / v->Ib; // коэффициент пересчета баз

    if (v->ManualEnb) {
        v->Kp = v->Kp_manual;
        v->Ki = v->Tspd * v->Kp_manual / v->Ki_manual; //!!! Goncharov Debug (* v->Kp added)
    }
    else {
        v->Kp = v->Jr/(3.0f *v->Zp*v->Zp*v->Tsl*v->PsiR) * Kb;
        v->Ki = v->Jr/(12.0f*v->Zp*v->Zp*v->Tsl*v->Tsl*v->PsiR) * v->Tspd * Kb;// * 100;
        //v->Ki = v->Tspd * v->Kp / v->Ki;
    }


    // защита от переполнения
    if (v->Kp > 127.0f) v->Kp = 127.0f;
    if (v->Ki > 127.0f) v->Ki = 127.0f;

    v->Kp_manual = v->Kp;
    v->Ki_manual = v->Tspd * v->Kp_manual / v->Ki; //!!! Goncharov Debug (* v->Kp added)

    v->Kp_forced_flt = v->Kp_forced * v->Kp;
    v->Ki_forced_flt = v->Ki_forced * v->Ki;
    if (v->Kp_forced_flt > 127.0f)	v->Kp_forced_flt = 127.0f;
    if (v->Ki_forced_flt > 127.0f)	v->Ki_forced_flt = 127.0f;


    v->Kp_norm = v->Kp;
    v->Ki_norm = v->Ki;

    // Значение не должно превышать величину переменной в формате IQ иначе будет беда
    v->Kp_forced = v->Kp_forced_flt;
    v->Ki_forced = v->Ki_forced_flt;

    // расчет ограничения регулятора скорости
    v->OutMax = v->Imax * _SQRT_2;
    
    // вычисление коэффициентов входного фильтра
    v->K1 = exp(- v->Tspd/(4*v->Tsl));
    v->K2 = 1.0 - v->K1;

    // K3 умножается на ошибку регулятора K3,          
    // для уменьшения величин коэффициентов регулятора 
    v->K3 = 2.0f * _PI; 

    // расчет коэффициентов корректирующего звена
    tmp = 1.0f / (v->Tsl + v->Tspd);
    v->K4 = (2.0f*v->Tcl + v->Tspd) * tmp;
    v->K5 =  2.0f*v->Tcl * tmp;
    v->K6 =    v->Tsl * tmp;   
    v->K7 = 1.0f / (2*_PI*v->Fc_spd*v->Tspd);  
}

