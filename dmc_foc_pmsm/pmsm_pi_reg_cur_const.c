/*=====================================================================================
 File name:        PMSM_PI_REG_CONST.C                   
                    
 Originator:    Triol

 Description:  The PI current controller adjuction for PMSM              

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-12-2011 Version 1.00
-------------------------------------------------------------------------------------*/

#include "pmsm_pi_reg_cur_const.h"
#include "Pjk_const.h"

void pmsm_pi_reg_cur_const(PMSMPIREGCURCONST *v)
{
    f32 Kb, Tdly, tmp;

    Tdly = v->Tpwm + v->Tdrv;
    v->Tcl = Tdly * v->NumTdly;

    Kb  = v->Ib / v->Ub;

    if (v->ManualEnb) {
        v->Kp  = v->Kp_manual;
        v->Ki  = v->Tpwm * (v->Kp / v->Ki_manual); //!!! Goncharov Debug (* v->Kp added)
        v->Kpd = v->Kp;
        v->Kpq = v->Kp;
    }
    else {
        v->Kpd = v->Lsd / (2*v->Tcl) * Kb;
        v->Kpq = v->Lsq / (2*v->Tcl) * Kb;
        //v->Ki  = v->Rs  / (2*v->Tcl) * v->Tpwm * Kb;
        //Goncharov, ниже более корректная запись, здесь v->Lsd/v->Rs это коэффициент при Р в регуляторе т.е. Ki в сек.
        v->Ki  = v->Tpwm * ( v->Kpd / (v->Lsd/v->Rs) );
        v->Kp  = v->Kpd;
    }

    // защита от переполнения
    if (v->Kpd > 127.0) v->Kpd = 127.0f;
    if (v->Kpq > 127.0) v->Kpq = 127.0f;
    if (v->Ki  > 127.0) v->Ki  = 127.0f;

    v->Kp_manual = v->Kp;
    v->Ki_manual = v->Kp * v->Tpwm / v->Ki; // Возврат Manual значения для отображения в параметрах

    // расчет коэффициентов корректирующего звена
    tmp = 1.0 / (v->Tcl + v->Tpwm);
    v->K1 = (Tdly + v->Tpwm) * tmp;
    v->K2 =   Tdly  * tmp;
    v->K3 = v->Tcl  * tmp; 
} 


