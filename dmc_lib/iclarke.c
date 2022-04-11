/*=====================================================================================
 File name:    ICLARKE.C  (float version)                  
                   
 Originator:   Triol

 Description:  Inverse Clarke Transformation                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 03-04-2012 Version 1.00
 23-09-2020 PAN                                                   
-------------------------------------------------------------------------------------*/

#include "iclarke.h"

void iclarke_calc(ICLARKE *v){   
  
    float Tmp = 1.73205080756888f * v->Beta;    //  sqrt(3) = 1.73205080756888

    v->As = v->Alpha;
    v->Bs = (Tmp - v->Alpha) * 0.5f;
    v->Cs = (-Tmp - v->Alpha) * 0.5f;
}

