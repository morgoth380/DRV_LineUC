/*=====================================================================================
 File name:    ACI_FE.C  (float version)                  
                    
 Originator:   Digital Control Systems Group
               Texas Instruments

 Description:  Flux Estimator of Induction Motor                 

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 12-15-2005 Version 3.22 : Change the sign in current model equation
 21-09-2011 Version 3.30 : добавлен коэффициент ослабления и функция сброса регуляторов и интеграторов, отсортированы параметры по типу сигнала
 01-10-2020 Version 4.0  : переведен на float
-------------------------------------------------------------------------------------*/

#include "aci_fe.h"
#include "Global_Include.h"
#include "Filter.h"
#include "Const.h"
#include "ipark.h"          // Include header for the IPARK object
#include "GlobalVar.h"

extern IPARK  ipark2;

#define FluxThetaCorModeOFF     0
#define FluxThetaCorModeFIXED   1
#define FluxThetaCorModePID     2
#define  Kgain 5                   // усилительный коэффициент

void aci_fe_calc(register ACIFE *v)
{
    float Error, TmpKp, TmpKi;
    float PsiErr;
    
// The current model section (Classical Rotor Flux Vector Control Equation)
    v->FluxDrE = (v->K1 * v->FluxDrE) + (v->K2 * v->IDsE);

// Inverse park transformation on the rotor flux from the current model
    v->FluxDrS = v->FluxDrE * arm_cos_f32(v->ThetaFlux);            // Проверить размерность угла !!! 
    v->FluxQrS = v->FluxDrE * arm_sin_f32(v->ThetaFlux);            // Проверить размерность угла !!!

#ifdef COMPENSATE_DELAY_FE
    // Компенсируем задержки в наблюдателе    
    ipark2.Ds = v->FluxDrS;
    ipark2.Qs = v->FluxQrS;
    ipark2.Angle = v->PhaseCorr;
    ipark2.calc(&ipark2);    
    
// Compute the stator flux based on the rotor flux from current model
//    v->FluxDsS = _IQmpy(v->K3, ipark2.Alpha/*v->FluxDrS*/) + _IQmpy(v->K4,v->IDsS);
//    v->FluxQsS = _IQmpy(v->K3, ipark2.Beta/*v->FluxQrS*/) + _IQmpy(v->K4,v->IQsS); 
    v->FluxDsS = (v->K3 * ipark2.Alpha) + (v->K4 * v->IDsS);
    v->FluxQsS = (v->K3 * ipark2.Beta) + (v->K4 * v->IQsS);    
    
#else // Штатный обработчик без учета компенсации задержек    
    
// Compute the stator flux based on the rotor flux from current model
    v->FluxDsS = (v->K3 * v->FluxDrS) + (v->K4 * v->IDsS);
    v->FluxQsS = (v->K3 * v->FluxQrS) + (v->K4 * v->IQsS);
#endif
    
// Conventional PI controller section
    TmpKp = v->Kp;
    
    if(v->Ki == 0){                            // Если прилетает значение интеграла равное нулю -     
        v->UiDsS = v->UiQsS = TmpKi = 0;
    } else {
        TmpKi = (v->Kp / v->Ki) * v->Tpwm;     // Kp/Ki * Tpwm 
    }
    
    v->Filter_Kp = f32_Filtr(v->Filter_Kp, TmpKp, 0.25f); 
    TmpKp = v->Filter_Kp;
    v->Filter_Ki = f32_Filtr(v->Filter_Ki, TmpKi, 0.25f); 
    TmpKi = v->Filter_Ki;

#ifdef COMPENSATE_DELAY_FE
     // Компенсируем задержки в наблюдателе
    Error = v->FluxDsS_Z1 - v->PsiDsS;               // !
    v->UCompDsS = (TmpKp * Error) + v->UiDsS;     // !
    v->UiDsS = (Error * TmpKi) + v->UiDsS;        // ! Err*TmpKi + Ui (где Ki = Kp/Ki * Tpwm)

    Error = v->FluxQsS_Z1 - v->PsiQsS;               // !
    v->UCompQsS = (TmpKp * Error) + v->UiQsS;     // !
    v->UiQsS = (Error * TmpKi) + v->UiQsS;        // ! Err*TmpKi + Ui (где Ki = Kp/Ki * Tpwm)
#else // Штатный обработчик без учета компенсации задержек  
    Error = v->FluxDsS - v->PsiDsS;               // !
    v->UCompDsS = (TmpKp * Error) + v->UiDsS;     // !
    v->UiDsS = (Error * TmpKi) + v->UiDsS;        // ! Err*TmpKi + Ui (где Ki = Kp/Ki * Tpwm)

    Error = v->FluxQsS - v->PsiQsS;               // !
    v->UCompQsS = (TmpKp * Error) + v->UiQsS;     // !
    v->UiQsS = (Error * TmpKi) + v->UiQsS;        // ! Err*TmpKi + Ui (где Ki = Kp/Ki * Tpwm)    
    
    Ветка не работает - требует удаления
#endif
    
// Compute the estimated stator flux based on the integral of back emf
    v->OldEmfDsS = v->EmfDsS;                               // !
    v->EmfDsS = v->UDsS + v->UCompDsS - (v->K5 * v->IDsS);  // !
    v->PsiDsS += 0.5f * v->K6 * (v->EmfDsS + v->OldEmfDsS); // ! метод трапеции

    v->OldEmfQsS = v->EmfQsS;                               // !
    v->EmfQsS = v->UQsS + v->UCompQsS - (v->K5 * v->IQsS);  // !
    v->PsiQsS += 0.5f * v->K6 * (v->EmfQsS + v->OldEmfQsS); // ! метод трапеции

// Compute the estimated rotor flux based on the stator flux from the integral of back emf
    v->PsiDrS = (v->K7 * v->PsiDsS) - (v->K8 * v->IDsS);    // !
    v->PsiQrS = (v->K7 * v->PsiQsS) - (v->K8 * v->IQsS);    // !

// Compute the rotor flux angle
    if( v->PhirUP ){
        v->ThetaFlux = atan2f(v->PsiQrS,v->PsiDrS);
    } else {
        v->ThetaFlux = 0;
    }
          
    
//    // Активация регулятора фазы потока 
//    switch (v->FluxThetaCorMode) {
//        case FluxThetaCorModeOFF:
//        v->dTheta = 0.0f;
//        break;
//        
//        case FluxThetaCorModeFIXED:
//          // Фиксированная сдвижка, работает !!!
//          v->dTheta = v->FluxThetaOffset * _DEG_TO_RAD ;            
//        break;
//        
//        case FluxThetaCorModePID:
//          //Алгоритм подстройки фазы наблюдателя
//          // модуль потокосцепления ротора    
//          arm_sqrt_f32(((v->PsiDrS * v->PsiDrS) + (v->PsiQrS * v->PsiQrS)), &v->PsiR); // PsiR = _IQmag(v->PsiDrS, v->PsiQrS);
//          
//          // ошибка модуля потокосцепления ротора
//          PsiErr = v->FluxDrE - v->PsiR;
//          // PsiErr = v->PhiR_nom - PsiR;
//          
//          if (v->Fcur > (0.5f/80.0f) ) {
//            // пропорц часть
//            v->PsiErrProp = PsiErr * v->FluxThetaPidKp;
//            
//            // интег часть
//            if (v->PsiR <= v->FluxDrE && v->GenMode) {
//              v->PsiErrInteg = v->PsiErrInteg + (PsiErr * v->Tpwm) * v->FluxThetaPidKi;
//              v->dTheta = v->PsiErrProp + v->PsiErrInteg;
//            } else {
//              v->PsiErrProp = v->PsiErrInteg = 0.0f;        
//              v->dTheta = f32_Filtr(v->dTheta, 0.0f, 0.03125f); 
//            }            
//          }                      
//        break;            
//    }  
    
    // корректировка угла потокосцепления ротора
    v->ThetaFlux = v->ThetaFlux + v->dTheta;
    
    if (v->ThetaFlux > _PI) {
      v->ThetaFlux = v->ThetaFlux - _2PI;
    } else if (v->ThetaFlux < -_PI) {
      v->ThetaFlux = v->ThetaFlux + _2PI;
    }    
    // КОнец блока корекции фазы потокаы
    
    
#ifdef COMPENSATE_DELAY_FE    
    // Задержанные значения
    v->FluxDsS_Z1 = v->FluxDsS;
    v->FluxQsS_Z1 = v->FluxQsS;    
#endif
}

void aci_fe_reset(ACIFE *v)
{
    // reset integrator
    v->PsiDsS = 0;
    v->PsiQsS = 0;
    // reset PI regulator
    v->UiDsS  = 0;
    v->UiQsS  = 0;
    v->UCompDsS = 0;
    v->PsiErrInteg = 0;
}
