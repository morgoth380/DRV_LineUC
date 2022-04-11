/*=====================================================================================
 File name:        CLARKE.C  (float version)                  
                    
 Originator:    Digital Control Systems Group
            Texas Instruments

 Description:  Clarke Transformation                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 23-09-2020 PAN
-------------------------------------------------------------------------------------*/
#include "clarke.h"

void clarke_calc(register CLARKE *v){   
   
    //Трехфазный Кларк  
    register CLARKE *rv = v;
    rv->Alpha = 0.66666666666667f * rv->As - (0.33333333333333f * (rv->Bs + rv->Cs));  // 2/3* Iu - 1/3 * (Iv + Iw)
    rv->Beta = 0.57735026918963f * (rv->Bs - rv->Cs);                                  // 1/sqrt(3) * (Iv - Iw)              
}


