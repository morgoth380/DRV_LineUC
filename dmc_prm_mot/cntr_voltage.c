/*=====================================================================================
 File name:   CNTRVOLTAGE.C  (float)                  
                    
 Originator:  Triol

 Description: Control voltage

=====================================================================================
 History:
-------------------------------------------------------------------------------------
  15-06-2011	Version 1.00
  04-11-2020	Version 2.00  PAN (float)   
-------------------------------------------------------------------------------------*/

#include "cntr_voltage.h"

void cnrtvoltage_calc(CNTRVOLTAGE *v)
{
    if(v->Period ){   // Прошел период регулируем напряжение
       
       v->InomDoneFlag = 1; // Ставим флаг достижения тока, в остальных условиях будет сброс.
       
       if((v->Ifaz <= v->Imax) && (v->Uline <= v->Umax)){
           v->Uline += v->UlineStep;
           v->InomDoneFlag = 0; // Сброс, так как еще правим напряжение
       }
       else{
           v->UmaxFlag = 1;           
           v->InomDoneFlag = 0; // Сброс, так как достигли потолка
       }
    }
}

void cnrtvoltageLm_calc(CNTRVOLTAGE *v)
{
    if(v->Period ){   // Прошел период регулируем напряжение
        if(!v->UmaxFlag){
           if((v->Ifaz <= v->Imax) && (v->Uline <= v->Umax)){
               v->Uline += v->UlineStep;
           }
           else{
               v->UmaxFlag = 1;
           }
        }
    }
}

void cnrtvoltageCurDown_calc(CNTRVOLTAGE *v)
{
    if(v->Period ){   // Прошел период регулируем напряжение
    	if (v->Uline > v->UlineStep){ // Если задание выходного напряжения, больше чем шаг ступени - уменьшаем выходное напряжение
    		v->Uline -= v->UlineStep;
    		v->CurDownFlag = 0;
    	}
        else
            v->CurDownFlag = 1;
    }
}

void cnrtvoltage_reset(CNTRVOLTAGE *v)
{
    v->Uline = 0;  
    v->UmaxFlag = 0; 
    v->InomDoneFlag = 0;
}
