/* =================================================================================
File name:       ACI_FE.H   (float version)                  
                     
Originator: Digital Control Systems Group
            Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the ACIFE.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 21-09-2011 Version 3.30 : добавлен коэффициент ослабления и функция сброса регуляторов и интеграторов, отсортированы параметры по типу сигнала
 01-10-2020 Version 4.0  : переведен на float
------------------------------------------------------------------------------*/
#ifndef __ACI_FE_H__
#define __ACI_FE_H__

#include "type.h"


typedef struct {  float  ThetaFlux;       // Output: Rotor flux angle 
		  float  OldThetaFlux;    // Output: Rotor flux angle
                  float  PsiDrS;          // Output: Stationary d-axis estimated rotor flux 
                  float  PsiQrS;          // Output: Stationary q-axis estimated rotor flux 
                  float  Kwn;             // Output: weakening of dinamic feature coefficient       
                  float  IDsE;            // Input: Rotating d-axis stator current 
                  float  IDsS;            // Input: Stationary d-axis stator current  
                  float  IQsS;            // Input: Stationary q-axis stator current  
                  float  UDsS;            // Input: Stationary d-axis stator voltage 
                  float  UQsS;            // Input: Stationary q-axis stator voltage 
                  u16    PhirUP;          // Input: Flag Rotor Flux is UP
                  u16    GenMode;         // Input: Flag Motor Generator mode
                  float  PhaseCorr;       // Input: Correction time delay 
                  float  Fcur;            // Input: Current frequence
                  float  K1;              // Parameter: Constant using in current model 
                  float  K2;              // Parameter: Constant using in current model 
                  float  K3;              // Parameter: Constant using in stator flux computation 
                  float  K4;              // Parameter: Constant using in stator flux computation 
                  float  K5;              // Parameter: Constant using in back emf computation 
                  float  K6;              // Parameter: Constant using in back emf computation 
                  float  K7;              // Parameter: Constant using in rotor flux computation 
                  float  K8;              // Parameter: Constant using in rotor flux computation 
                  float  Kp;              // Parameter: PI proportionnal gain 
                  float  Ki;              // Parameter: PI integral gain 
                  float  iKwn;            // Parameter: invert weakening of dinamic feature coefficient       
                  float  FluxDsS_Z1;      // Variable: Delayed FluxDsS
                  float  FluxQsS_Z1;      // Variable: Delayed FluxQsS
                  float  FluxDrE;         // Variable: Rotating d-axis rotor flux (current model)
                  float  FluxDrS;         // Variable: Stationary d-axis rotor flux (current model) 
                  float  FluxQrS;         // Variable: Stationary q-axis rotor flux (current model) 
                  float  FluxDsS;         // Variable: Stationary d-axis stator flux (current model) 
                  float  FluxQsS;         // Variable: Stationary q-axis stator flux (current model) 
                  float  PsiDsS;          // Variable: Stationary d-axis stator flux (voltage model) 
                  float  PsiQsS;          // Variable: Stationary q-axis stator flux (voltage model) 
                  float  UiDsS;           // Variable: Stationary d-axis integral term 
                  float  UiQsS;           // Variable: Stationary q-axis integral term  
                  float  UCompDsS;        // Variable: Stationary d-axis compensated voltage 
                  float  UCompQsS;        // Variable: Stationary q-axis compensated voltage 
                  float  EmfDsS;          // Variable: Stationary d-axis back emf 
                  float  EmfQsS;          // Variable: Stationary q-axis back emf  
                  float  KlowSpd;         // Variable: weakening of dinamic integral regulator coefficient       
                  float  OldEmfDsS;       // History: Stationary d-axis back emf 
                  float  OldEmfQsS;       // History: Stationary q-axis back emf
                  float  Filter_Kp;	  // Фильтрованное значение Кр
                  float  Filter_Ki;	  // Фильтрованное значение Кi
                  float  Tpwm;		  // период ШИМа                                    
                  float  PsiR;	      // Модуль потока по напряжению
                  float  PsiErrProp; 
                  float  PsiErrInteg;
                  float  dTheta;
                  u16    FluxThetaCorMode;
                  float  FluxThetaOffset;
                  float  FluxThetaPidKp;
                  float  FluxThetaPidKi;
                  void  (*calc)();    // Pointer to calculation function  
                  void  (*reset)();   // Pointer to reset function  
                 } ACIFE;            

typedef ACIFE *ACIFE_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the ACIFE object.
-----------------------------------------------------------------------------*/                     
#define ACIFE_DEFAULTS {  0,    /*  ThetaFlux    */ \
			  0,    /*  OldThetaFlux */ \
                          0,    /*  PsiDrS       */ \
                          0,    /*  PsiQrS       */ \
                          0,    /*  Kwn          */ \
                          0,    /*  IDsE         */ \
                          0,    /*  IDsS         */ \
                          0,    /*  IQsS         */ \
                          0,    /*  UDsS         */ \
                          0,    /*  UQsS         */ \
                          0,    /*  PhirUP       */ \
        		  0,    /*  GenMode      */ \
                          0,    /* PhaseCorr     */ \
                          0,    /*  Fcur         */ \
                          0,    /*  K1           */ \
                          0,    /*  K2           */ \
                          0,    /*  K3           */ \
                          0,    /*  K4           */ \
                          0,    /*  K5           */ \
                          0,    /*  K6           */ \
                          0,    /*  K7           */ \
                          0,    /*  K8           */ \
                          0,    /*  Kp           */ \
                          0,    /*  Ki           */ \
                          0,    /*  iKwn         */ \
                          0,    /* FluxDsS_Z1    */ \
                          0,    /* FluxQsS_Z1    */ \
                          0,    /*  FluxDrE      */ \
                          0,    /*  FluxDrS      */ \
                          0,    /*  FluxQrS      */ \
                          0,    /*  FluxDsS      */ \
                          0,    /*  FluxQsS      */ \
                          0,    /*  PsiDsS       */ \
                          0,    /*  PsiQsS       */ \
                          0,    /*  UiDsS        */ \
                          0,    /*  UiQsS        */ \
                          0,    /*  UCompDsS     */ \
                          0,    /*  UCompQsS     */ \
                          0,    /*  EmfDsS       */ \
                          0,    /*  EmfQsS       */ \
                          0,    /*  KlowSpd      */ \
                          0,    /*  OldEmfDsS    */ \
                          0,    /*  OldEmfQsS    */ \
			  0,    /*  Filter_Kp    */ \
			  0,    /*  Filter_Ki    */ \
			  0,    /*     Tpwm      */ \
		  	  0,    /*     Psir      */ \
                          0,    /*   PsiErrProp  */ \
                          0,    /*   PsiErrInteg */ \
                          0,    /*   dTheta      */ \
                          0,    /*FluxThetaCorMode*/\
                          0,    /*FluxThetaOffset*/ \
                          0,    /*FluxThetaPidKp*/  \
                          0,    /*FluxThetaPidKi*/  \
                         (void (*)(u32))aci_fe_calc, \
                         (void (*)(u32))aci_fe_reset }

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_FE.C
------------------------------------------------------------------------------*/
void aci_fe_calc(ACIFE_handle);
void aci_fe_reset(ACIFE_handle);

#endif // __ACI_FE_H__

