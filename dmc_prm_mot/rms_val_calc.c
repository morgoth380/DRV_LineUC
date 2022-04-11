/*=====================================================================================
 File name:     RMSVALCALC.C  (float)                  
                    
 Originator:    Triol

 Description:   Calculation of RMS value

=====================================================================================
 History:
-------------------------------------------------------------------------------------
  13-01-2012 Version 1.00  
  04-11-2020 Version 2.00  PAN (float)   
-------------------------------------------------------------------------------------*/

#include "rms_val_calc.h"
#include "Global_Include.h"

void rms_value_calc(RMSVALCALC *v)
{
    v->SqrSum += v->Input * v->Input;
    v->cnt++;

    if(v->Period){
       arm_sqrt_f32((float)(v->SqrSum / v->cnt), &v->RMSVal);
       v->SqrSum = 0;
       v->cnt    = 0;
    }
}

void rms_value_reset(RMSVALCALC *v)
{
    v->SqrSum = 0;
    v->cnt = 0;
    v->RMSVal = 0 ;
}


