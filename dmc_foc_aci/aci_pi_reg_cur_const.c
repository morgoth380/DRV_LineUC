/*=====================================================================================
 File name:     ACI_PI_REG_CUR_CONST.C                   
                    
 Originator:    Triol

 Description:   The PI current controller adjuction                  

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 1.00
 02-04-2011 Version 1.10: New file name (late "PI_REG_CUR_CONST.C")
 05-10-2020 Version 2.0: float                                                   
-------------------------------------------------------------------------------------*/

#include "aci_pi_reg_cur_const.h"

void pi_reg_cur_const(PIREGCURCONST *v)
{
    float curKp_PU, curTi_PU, Ls; // Tdly, tmp

    v->Tcl = v->Tpwm * v->NumTdly;
    Ls = v->Lr;
    v->sigma = 1.0f - v->Lm * v->Lm / (Ls * v->Lr);
   
    if (v->ManualEnb) {
      v->Kp = v->Kp_manual;
      v->Ki = v->Tpwm * (v->Kp / v->Ki_manual);
    } else {
      curKp_PU = v->sigma * Ls / (2.0f * v->Tpwm * v->NumTdly) * v->Ib / v->Ub;
      curTi_PU = v->sigma * Ls / (v->Rs * (1 + (v->Rr * v->Lm * v->Lm) / (v->Rs * v->Lr * v->Lr)));
      
      v->Ki = curKp_PU / curTi_PU * v->Tpwm;
      v->Kp = curKp_PU;
    }    
    
    
  // защита от переполнения
    if (v->Kp > 127.0f) v->Kp = 127.0f;
    if (v->Ki > 127.0f) v->Ki = 127.0f;
    
    v->Kp_manual = v->Kp;
    v->Ki_manual = v->Kp * v->Tpwm / v->Ki; // Возврат Manual значения для отображения в параметрах        
} 

