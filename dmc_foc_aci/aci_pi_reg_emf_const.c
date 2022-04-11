/*=====================================================================================
 File name:     ACI_PI_REG_EMF_CONST.C                   
                    
 Originator:    Triol
 
 Developer:      Sikalov Y.Y. 
 
 Description:  The PI field weakening controller adjuction                  

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 22-11-2011 Version 1.00
 02-04-2011 Version 2.10: New file name (late "PI_REG_EMF_CONST.C")                                                   
-------------------------------------------------------------------------------------*/

#include "aci_pi_reg_emf_const.h"

void pi_reg_emf_const(PIREGEMFCONST *v)
{
    float Tr,emfKp_PU,emfTi_PU,Temf, tmp;
    
    // расчет коэффициентов регулятора

    Tr = v->Lr / v->Rr;
    Temf = 2.0f * v->Tcl + 6.0f * v->Tpwm;

    // Новые Коэф. для регулятора противоЭДС
    emfKp_PU = Tr / (2.0f * Temf * v->Lm) * v->Ub / (v->Ib * v->Fb);
    emfTi_PU = Tr;
    v->Kp = emfKp_PU;
    v->Ki = emfKp_PU / emfTi_PU * v->Tpwm;

//    // защита от переполнения
    if(v->Kp > 127.0f){
        v->Kp = 127.0f;
    }
    if(v->Ki > 127.0f){
        v->Ki = 127.0f;
    }

    v->Kp_iq = v->Kp;
    v->Ki_iq = v->Ki;

    // расчет ограниченый регулятора
    tmp = v->Wnom / v->Wmax;
    if(tmp > 1.0f){
        tmp = 1.0f;
    }

    v->OutMax = v->Idn * (v->Wnom_Hz / v->Fb ); // Пределы для новых коэф. регулятора противоЭДС - номинальная частота вынесена за регулятор
    v->OutMin = -v->Idn * (v->Wnom_Hz / v->Fb ); // Пределы для новых коэф. регулятора противоЭДС

    v->InitInteg = (v->OutMax + v->OutMin) / 2.0f; // среднее значение
} 

