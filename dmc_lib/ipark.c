/*=====================================================================================
 File name:        IPARK.C  (float version)                  
                    
 Originator:    Digital Control Systems Group
            Texas Instruments

 Dsscription:  Inverse Park Transformation                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 23-09-2020 PAN
-------------------------------------------------------------------------------------*/

#include "ipark.h"
#include "Global_Include.h"

void ipark_calc(register IPARK *v){   
   
   float Cosine, Sine;
   
   Sine = arm_sin_f32(v->Angle);
   Cosine = arm_cos_f32(v->Angle);
    
   v->Alpha = (v->Ds * Cosine) - (v->Qs * Sine);
   v->Beta = (v->Qs * Cosine) + (v->Ds * Sine);  
}


