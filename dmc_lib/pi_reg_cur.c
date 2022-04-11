/*=====================================================================================
 File name:        PI_REG_CUR.C  (float version)                  
                    
 Originator: Triol

 Description:  The PI current controller               

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 1.00
 05-10-2020 Version 2.00 : float (PAN)
-------------------------------------------------------------------------------------*/

#include "pi_reg_cur.h"

void pi_reg_cur_calc(register PIREGCUR *v)
{

    // Compute the error
    v->Err = v->Ref - v->Fdb;

    // Compute the proportional output
    v->Up =  v->Kp *  v->Err;
    
    // Compute the integral output
    if (!v->FreezeUi) { // Проверяем флаг заморозки 
        v->Ui += v->Ki * v->Err;
    }

    // Saturate the integral output
    if(v->Ui > v->OutMax){
      v->Ui =  v->OutMax;
    }
    else if(v->Ui < v->OutMin){
      v->Ui =  v->OutMin;
    }

    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui;

    // Saturate the output
    if(v->OutPreSat > v->OutMax){
      v->Out =  v->OutMax;
    }
    else if(v->OutPreSat < v->OutMin){
      v->Out =  v->OutMin;
    }
    else{
      v->Out = v->OutPreSat;
    }
}

void pi_reg_cur_reset(PIREGCUR *v)
{
    v->Err = 0.0f;
    v->ErrP = 0.0f;
    v->ErrT = 0.0f;
    v->Up = 0.0f;
    v->Ui = 0.0f;
    v->OutPreSat = 0.0f;
    v->Out =  0.0f;
}

