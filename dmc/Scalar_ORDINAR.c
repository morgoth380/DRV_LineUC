/* =================================================================================
File Name:      SCALAR_CONTROL.H

Description:    Source for Scalar Control. 
 
Originator:     Triol

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 17-05-2012 Version 1.00
=================================================================================  */
#include "Global_include.h"
#include "Scalar_ORDINAR.h"
#include "LIB_DMC.h"

#include "DrvPWM.h"
#include "Pjk_const.h"

#define K_MOD_SPACEPWM  1.4142135623730950488016887242097f

extern PWMGEN Pwm;

void Scalar_ORDINAR(SCALARORDINAR *v)
{
  // расчет угла положения поля и выходного напряжения в скалярном управлении
    v->Ds = (v->Uscl * K_MOD_SPACEPWM) / v->Udc;
    if(v->Ds > 0.98f){
       v->Ds = 0.98f;             // Уперлись в ограничение по выходному напряжению
    }

    //Определим фактическое выходное напряжение
    v->Uout = (v->Udc * v->Ds) / K_MOD_SPACEPWM;
    
    v->Angle = v->Angle + (_2PI * v->StepAngle * IQ_to_FLOAT(v->Fscl, Herz_base)); // Fscl - в pu.
    
    if(v->Angle > _2PI) {
       v->Angle -= _2PI;
       v->Period = 1;
    } else if (v->Angle < -_2PI) {
       v->Angle += _2PI;
       v->Period = 1;
    }

// ------------------------------------------------------------------------------
//    Connect inputs of the INV_PARK module and call the inverse park transformation
//    calculation function.
// ------------------------------------------------------------------------------
    ipark1.Ds = v->Ds;
    ipark1.Qs = 0;
    ipark1.Angle = v->Angle;
    ipark1.calc(&ipark1);

// ------------------------------------------------------------------------------
//    Call the Space Vector calculation function.
// ------------------------------------------------------------------------------
    svgen_dqv.Ualpha = ipark1.Alpha;
    svgen_dqv.Ubeta = ipark1.Beta;
    svgen_dqv.calc(&svgen_dqv);

// ------------------------------------------------------------------------------
//    Connect inputs of the DT_COMP module and call
//    calculation function.
// ------------------------------------------------------------------------------
    dtc1.Iu = dPnt->IsU;
    dtc1.Iv = dPnt->IsV;
    dtc1.Iw = dPnt->IsW;
    dtc1.Ta = svgen_dqv.Ta;
    dtc1.Tb = svgen_dqv.Tb;
    dtc1.Tc = svgen_dqv.Tc;
    dtc1.Ud = UdcFiltr.UdcOut; // dPnt->Udc;
    dtc1.We = v->Fscl;//dPnt->Wrz;
    dtc1.Fpwm = dPnt->Fpwm;
    dtc1.calc(&dtc1);

    svgen_dqv.Ta = dtc1.Ta;
    svgen_dqv.Tb = dtc1.Tb;
    svgen_dqv.Tc = dtc1.Tc;
    
// ------------------------------------------------------------------------------
//    PWM References output._NEW
// ------------------------------------------------------------------------------
    Pwm.Ta = svgen_dqv.Ta;
    Pwm.Tb = svgen_dqv.Tb;
    Pwm.Tc = svgen_dqv.Tc;
    Pwm.Update(&Pwm);
}

