/*=====================================================================================
 File name:        MEASRS.C  (float version)                  
                    
 Originator:    Triol

 Description:  Calculation of stator resistance (Rs)             

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 26-04-2011 Version 1.00
 19-10-2020 Version 2.00 PAN (float)
-------------------------------------------------------------------------------------*/

#include "meas_rs.h"
#include "Filter.h"

void meas_rs_calc(MEASRS *v){

    v->Vin_f = f32_Filtr(v->Vin_f, v->Vin, 0.0009765625f);
    v->Iin_f = f32_Filtr(v->Iin_f, v->Iin, 0.0009765625f);

    v->Rs_tmp = v->Vin_f / v->Iin_f; 
    v->Rs_tmp = v->Rs_tmp * v->K1;

    v->Rs = f32_Filtr(v->Rs, v->Rs_tmp, 0.00390625f);
}


void meas_rs_reset(MEASRS *v){
    v->Vin_f = v->Vin = 0;
    v->Iin_f = v->Iin = 0;
    v->Rs = v->Rs_tmp = 0;
}
