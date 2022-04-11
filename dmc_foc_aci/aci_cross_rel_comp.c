/*=====================================================================================
 File name:     ACI_CROSS_REL_COMP.C  (float version)                  
                    
 Originator:    Triol

 Description:   cross-relation compansation            

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 2.00
 02-04-2011 Version 2.10: New file name (late "CROSS_REL_COMP.C")
 02-10-2020 Version 3.00: float (PAN)                                                   
 -------------------------------------------------------------------------------------*/

#include "aci_cross_rel_comp.h"
#include "Global_Include.h"

#define CROSS_OVERLOAD  1

void cross_rel_comp(register CROSSRELCOMP * v)
{
    float Ucd, Ucq; // компенсирующие напряжения - Umin

    Ucd = (v->K1 * v->IRm) + (v->We * v->Isq) * v->K2;  
    Ucq = - (v->We * v->Isd) * v->K2 - (v->Wr * v->IRm) * v->K3; 

    // вычисление скомпенсированного напряжения статора
    v->Usd = v->Urd - Ucd; 
    v->Usq = v->Urq - Ucq; 

    //Модуль вектора напряжения статора, мгновенное значение
    arm_sqrt_f32(((v->Usd * v->Usd) + (v->Usq * v->Usq)), &v->Us);
    
    //Ограничение напряжения
    v->UsmaxFlag = 0;
    if(v->Us > v->Usmax){
    	v->Us = v->Usmax;
        v->UsmaxFlag = CROSS_OVERLOAD;
        if(fabsf(v->Usd) >= v->Usmax) {
            if(v->Usd > 0.0f){
                v->Usd = v->Usmax;
            }
            else if(v->Usd < 0.0f){
                v->Usd = -v->Usmax;
            }

            v->Usq = 0.0f;
        }
        else{
            if (v->Usq >= 0.0f){ 
                arm_sqrt_f32(((v->Us * v->Us) - (v->Usd * v->Usd)), &v->Usq);
            }
            else{
                arm_sqrt_f32(((v->Us * v->Us) - (v->Usd * v->Usd)), &v->Usq);  
                v->Usq = v->Usq * -1.0f;
            }
        }
    }
}

