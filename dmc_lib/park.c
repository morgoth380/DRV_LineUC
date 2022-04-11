/*=====================================================================================
 File name:        PARK.C  (float version)                  
                    
 Originator:	Digital Control Systems Group
        	Texas Instruments

 Alphascription:  Park Transformation                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
 23-09-2020     PAN
-------------------------------------------------------------------------------------*/

#include "park.h"
#include "Global_Include.h"


void park_calc(register PARK *v){    
  
    float Cosine, Sine;

    Sine = arm_sin_f32(v->Angle);
    Cosine = arm_cos_f32(v->Angle); 

    v->Ds = (v->Alpha * Cosine) + (v->Beta * Sine);
    v->Qs = (v->Beta * Cosine) - (v->Alpha * Sine);
}


