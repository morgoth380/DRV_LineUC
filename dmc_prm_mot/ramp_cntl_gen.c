/*=====================================================================================
 File name:    RAMPCNTRGEN.C  (float)                  
                    
 Originator:   Triol

 Description:  The Ramp Generation                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 04-11-2020 Version 4.00  PAN (float)  
-------------------------------------------------------------------------------------*/

#include "ramp_cntl_gen.h"
#include "Pjk_const.h"

void rampcntlgen_calc(RAMPCNTLGEN *v)
{   
    // расчет задания по частоте
    v->Framp  += v->StepFreqMax * v->Fwork; 

    if (v->Framp >= v->Fwork) {
       v->Framp  = v->Fwork;
    } else if (v->Framp <= -v->Fwork) {
       v->Framp  = -v->Fwork;
    }
     
    v->Angle +=  v->StepAngleMax * v->Framp; 
    
    if (v->Angle > _PI){
        v->Angle -= _2PI;
        v->Period = 1;
    } 
    else if (v->Angle < -_PI){
        v->Angle += _2PI;
        v->Period = 1;
    }

    // расчет задания по напряжению
    v->Uout = v->Framp / v->Fnom;
    if(v->Uout > 0.98f) v->Uout = 0.98f;
}

void rampcntlgen_reset(RAMPCNTLGEN *v)
{
    v->Framp = 0;
    v->Angle = 0;
    v->Uout  = 0;
    v->Period = 0;
}
