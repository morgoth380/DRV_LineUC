/*=====================================================================================
 File name:        ACI_SE.C  (float version)                  
                    
 Originator:    Digital Control Systems Group Texas Instruments

 Description:  Open-loop Speed Estimator of Induction Motor                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 21-09-2011 Version 3.30 : добавлена обработка перехода через единицу и нуль, взвод флага period
                         отсортированы параметры по типу сигнала
 02-12-2011 Version 3.31 : сделана полная фильтрация рассчитаной скорости
 01-10-2020 Version 3.0  PAN float версия
-------------------------------------------------------------------------------------*/

#include "Global_Include.h"
#include "aci_se.h"
#include "Pjk_const.h"


void aci_se_calc(register ACISE *v)
{
    float WSlip, Tmp1;
    float Te,wck;
    //static float WSlip_filtr = 0;
    //static float WrHat_filtr = 0;
    WSlip = 0;

    if (!v->PhirUP) { // Нет возбуждения, сбросить фильтры
      v->WrHat_filtr = v->WSlip_filtr = 0;
    }
    
    //-------------------------------------------------------------------
    //              Slip computation
    //-------------------------------------------------------------------
    v->SquaredPsi = (v->PsiDrS * v->PsiDrS) + (v->PsiQrS * v->PsiQrS);
    arm_sqrt_f32(((v->PsiDrS * v->PsiDrS) + (v->PsiQrS * v->PsiQrS)), &v->ModulePsi);
    
    if( v->SquaredPsi != 0.0f){
	v->WSlip = v->K1 * ((v->PsiDrS * v->IQsS) - (v->PsiQrS * v->IDsS));
	v->WSlip = v->WSlip / v->SquaredPsi;
    }
    Te = v->K6 * ((v->PsiDrS * v->IQsS) - (v->PsiQrS * v->IDsS));     //измеренный момент

    wck = (v->K7 * Te) / v->SquaredPsi;
    //-------------------------------------------------------------------
    //             Synchronous speed computation
    //-------------------------------------------------------------------
  
    Tmp1 = v->ThetaFlux - v->OldThetaFlux;
    
    // пресчет перехода через единицу
    if(Tmp1 < (-0.8f * _PI)){
        Tmp1 += _2PI;
        v->Period = 1;
    }
    if(Tmp1 > (0.8f * _PI)){
        Tmp1 -= _2PI;   //при реверсе
        v->Period = 1;
    }

    v->WSyn = v->K2 * Tmp1;
        
    v->OldThetaFlux = v->ThetaFlux;
   
    //-------------------------------------------------------------------
    //             Low-pass filteration of Synchronous speed and Slip
    //-------------------------------------------------------------------
    v->WPsi = (v->K3 * v->WPsi) + (v->K4 * v->WSyn);
    v->WSlip_filtr = (v->K3 * v->WSlip_filtr) + (v->K4 * WSlip);

    //-------------------------------------------------------------------
    //             Rotor Speed calcucation
    //-------------------------------------------------------------------
    v->WrHat = v->WPsi - v->WSlip_filtr;     
    v->WrHat_filtr = (v->K8 * v->WrHat_filtr) + (v->K9 * v->WrHat);
    //WrHat_filtr = WrHat_filtr + ((v->WrHat - WrHat_filtr) * 0.03125f);    // Фильтр
    v->WrHat = v->WrHat_filtr;
    //-------------------------------------------------------------------
    //             Rotor position calculation
    //-------------------------------------------------------------------
    v->ThetaRotor += v->K5 * (v->WSyn - WSlip);

    // ограничение угла положения ротора от 0 до 1
    if (v->ThetaRotor < -_PI) v->ThetaRotor += _2PI;
    if (v->ThetaRotor > _PI) v->ThetaRotor -= _2PI;
    
    //-------------------------------------------------------------------
    //             Limit the estimated speed between -1 and 1 per-unit
    //-------------------------------------------------------------------
    if(v->WrHat > v->MaxWrHat){
        v->WrHat = v->MaxWrHat;
    }
    else if(v->WrHat < v->MinWrHat){
        v->WrHat = v->MinWrHat;     
    }
    else{
        v->WrHat = v->WrHat;
    }

    //-------------------------------------------------------------------
    //             Rotor speed calculation in the Rpm
    //-------------------------------------------------------------------
    v->WrHatRpm = (u32)((float)v->BaseRpm * v->WrHat);     // !!!
    v->NewTorq = Te;
    v->WRotor = v->WSyn - wck;
}

