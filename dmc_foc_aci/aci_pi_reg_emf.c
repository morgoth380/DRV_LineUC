/*=====================================================================================
 File name:        PI_REG_EMF.C  (float version)                  
                    
 Originator: Triol

 Description:  The PI field weakening controller               

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 22-11-2011 Version 1.00
 06-12-2011 Version 1.10 : добалена защелка выходного напряжения (защита от возбуждения)
 06-12-2011 Version 1.10 : float (PAN)
-------------------------------------------------------------------------------------*/

#include "aci_pi_reg_emf.h"
#include "Filter.h"


void pi_reg_emf_calc(register PIREGEMF *v)
{
    v->Err = v->Ref - v->Fdb;

    // Compute the proportional output
    v->Up = v->Kp * v->Err;

    // Compute the integral output
    v->Ui += v->Ki * v->Err;

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

void pi_reg_emf_reset(PIREGEMF *v)
{
    v->Err = 0;
    v->ErrP = 0;
    v->ErrT = 0;
    v->Up = 0;
    v->Ui = 0;
    v->OutPreSat = 0;
    v->Out =  0;
}

