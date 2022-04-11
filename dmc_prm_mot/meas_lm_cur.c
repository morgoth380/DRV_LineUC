/*=====================================================================================
 File name:        MEASLMCUR.C  (float version)                  
                    
 Originator:    Triol

 Description:  magnetizing inductance measurement                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 23-06-2011 Version 1.00
 19-10-2020 Version 2.00 PAN (float)
-------------------------------------------------------------------------------------*/

#include "meas_lm_cur.h"
#include "Global_Include.h"
#include "Pjk_const.h"


void measlmcur_calc(MEASLMCUR *v)
{
    float Tmp1;
    static u16 PeriodPr = 0;
    static float IfazFlashDelay[2] = {0,0};

    if (v->Period > PeriodPr) {
        if(!v->FirstStart) {
            ////////////////////////////////////////////////////////
            Tmp1 = (float)(v->ISumCos / v->Counter);
            v->Iact = Tmp1 * _SQRT_2;       // умножение на 2 (связано математекой вычислений) 
                                            // и деление на sqrt(2) (перевод в действ. величину)
                                            // 2/sqrt(2) = sqrt(2)
            
            Tmp1 = (float)(v->ISumSin / v->Counter);
            v->Ireact = Tmp1 * _SQRT_2;     // умножение на 2 (связано математекой вычислений) 
                                            // и деление на sqrt(2) (перевод в действ. величину)
                                            // 2/sqrt(2) = sqrt(2)
            
            arm_sqrt_f32(((v->Iact * v->Iact) + (v->Ireact * v->Ireact)), &v->Ifull);   
           
            if(v->PeriodCnt < 2) v->Cur_Phaze = atan2f(v->Ireact, v->Iact);
    
            ////////////////////////////////////////////////////////
            Tmp1 = (float)(v->USumCos / v->Counter);
            v->Uact = Tmp1 * _SQRT_2;       // умножение на 2 (связано математекой вычислений) 
                                            // и деление на sqrt(2) (перевод в действ. величину)
                                            // 2/sqrt(2) = sqrt(2)
    
            Tmp1 = (float)(v->USumSin / v->Counter);
            v->Ureact = Tmp1 * _SQRT_2;     // умножение на 2 (связано математекой вычислений) 
                                            // и деление на sqrt(2) (перевод в действ. величину)
                                            // 2/sqrt(2) = sqrt(2)
    
            arm_sqrt_f32((((v->Uact * v->Uact)) + (v->Ureact * v->Ureact)), &v->Uline);
                                    
            v->IactSum   += v->Iact;
            v->IreactSum += v->Ireact;
            v->IfullSum  += v->Ifull;
            v->UlineSum  += v->Uline;

            v->PeriodCnt++;
        }
        
        v->FirstStart = 0;
        v->Counter = 0;
        v->ISumCos = 0;
        v->ISumSin = 0;
        v->USumCos = 0;
        v->USumSin = 0;
    }

    v->ISumCos += IfazFlashDelay[1] * arm_cos_f32(v->Angle);  
    v->ISumSin += IfazFlashDelay[1] * arm_sin_f32(v->Angle);   
    
    v->USumCos += v->UlineFlash *  arm_cos_f32(v->Angle);  
    v->USumSin += v->UlineFlash *  arm_sin_f32(v->Angle); 

    // задержка тока на 2 периода ШИМ
    IfazFlashDelay[1] = IfazFlashDelay[0]; 
    IfazFlashDelay[0] = v->IfazFlash;
      
    v->Counter++ ;

    if(v->PeriodCnt == v->PeriodAmt) {
        v->Iact   = v->IactSum   / v->PeriodAmt;
        v->Ireact = v->IreactSum / v->PeriodAmt;
        v->Ifull  = v->IfullSum  / v->PeriodAmt;
        v->Uline  = v->UlineSum  / v->PeriodAmt;

        v->Lm = (v->Uline * v->Ireact * v->K1) / (v->Ifull * v->Ifull * v->Wout);
        v->Rs = (v->Uline * v->Iact * v->K2) / (v->Ifull * v->Ifull);

        v->StopCalc  = 1;
        v->IactSum   = 0; 
        v->IreactSum = 0;
        v->IfullSum  = 0;
        v->UlineSum  = 0; 
        v->Counter   = 0;
        v->ISumCos = 0;
        v->ISumSin = 0;
        v->USumCos = 0;
        v->USumSin = 0;
        v->PeriodCnt = 0;
    }

    PeriodPr = v->Period;

}

