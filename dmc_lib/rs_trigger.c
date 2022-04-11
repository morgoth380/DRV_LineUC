/*=====================================================================================
 File name:    RS_TRIGGER.C  (float version)
                   
 Originator:   Triol

 Description:  RS trigger

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 24-09-2019 Version 1.00
 02-10-2020 Version 2.00 float (PAN)
-------------------------------------------------------------------------------------*/

#include "rs_trigger.h"

void rs_trigger_calc(register RS_TRIGGER *v)
{   
    if(!v->OneTimeFix){
        // 1. Проверяем первую установку в 1 с заданием времени сброса
        if(v->Set && v->ResetTime && !v->Out) {
            v->ResetCnt = (u32)(v->ResetTime * v->Fpwm);
            v->Out = 1;
        }
        else if(v->Set){
            v->Out = 1;
        }
        else if(v->OneTimeSet && v->ResetTime && !v->Out){
            v->ResetCnt = (u32)(v->ResetTime * v->Fpwm);
            v->Out = 1;
            v->OneTimeFix = 1;
        } 
        else if(!v->Set && v->Reset){
            v->Out = 0;
        }
    }

    // Автосброс входных значений
    v->Set = v->Reset = v->OneTimeSet = 0;
    v->ResetTime = 0.0f;

    // Обработка счетчика автосброса триггера
    if (v->ResetCnt){
        v->ResetCnt--;
    }
    if(!v->ResetCnt){ 
        v->Out = 0;         // Если дошли до нуля, сбрасываем выход в ноль 
    } 

    v->NotOut = !v->Out;
}
//=====================================================================================
void rs_trigger_reset(register RS_TRIGGER *v)
{
    v->Out = v->Set = v->Reset = v->OneTimeSet = 0;
    v->ResetCnt = 0;
    v->ResetTime = 0.0f;
    v->NotOut = 1;
    v->OneTimeFix = 0;
}

