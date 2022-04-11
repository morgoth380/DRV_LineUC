/*=====================================================================================
 File name:        cross_rel_comp.C  (IQ version)                  
                    
 Originator:    Triol

 Description:  cross-relation compansation for Permanent-Magnet Synchronous Motor           

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-12-2011 Version 1.00
-------------------------------------------------------------------------------------*/

#include "type.h"
#include "Global_Include.h"
#include "pmsm_cross_rel_comp.h"

#define CROSS_OVERLOAD         1

void pmsm_cross_rel_comp(PMSMCROSSRELCOMP * v)
{
    // вычисление скомпенсированного напряжения статора
    v->Ucd =   v->K1 * v->Wr * v->Isq;
    v->Ucq = - v->K2 * v->Wr * v->Isd - v->K3 * v->Wr;

    // компесанция
    v->Usd = v->Urd - v->Ucd; 
    v->Usq = v->Urq - v->Ucq; 

    //Модуль вектора напрЯжениЯ статора, мгновенное значение
    arm_sqrt_f32((v->Usd * v->Usd) + (v->Usq * v->Usq), &v->Us); // arm_sqrt_f32(((v->minI * v->minI) - (v->inIq * v->inIq)), v->outId);

    //Ограничение напрЯжениЯ
    if (v->Us > v->Usmax)
    {
        v->UsmaxFlag = CROSS_OVERLOAD;
        v->Us = v->Usmax;
        if (fabsf(v->Usd) >= v->Usmax){
          if (v->Usd > 0 ) {
                v->Usd = v->Usmax;
          } else if (v->Usd < 0 ) {
                v->Usd = -v->Usmax;
          }

          v->Usq = 0;
        } else {
          if (v->Usq >= 0){ 
            arm_sqrt_f32(((v->Us*v->Us) - (v->Usd*v->Usd)), &v->Usq);
          } else {
            arm_sqrt_f32(((v->Us*v->Us) - (v->Usd*v->Usd)), &v->Usq);
            v->Usq = -v->Usq;
          }
        }
    }   
}

