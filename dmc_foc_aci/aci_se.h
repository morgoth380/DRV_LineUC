/* =================================================================================
File name:       ACI_SE.H   (float version)                  
                    
Originator: Digital Control Systems Group
            Texas Instruments


Description: 
Header file containing constants, data type definitions, and 
function prototypes for the ACISE.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
 21-09-2011 Version 3.30 : добавлена обработка перехода через единицу и нуль, взвод флага period
                         отсортированы параметры по типу сигнала
01-10-2020 Version 3.0  PAN float версия
------------------------------------------------------------------------------ */
#ifndef __ACI_SE_H__
#define __ACI_SE_H__

#include "type.h"

typedef struct {  float  IQsS;             // Input: Stationary q-axis stator current  
                  float  PsiDrS;           // Input: Stationary d-axis rotor flux  
                  float  IDsS;             // Input: Stationary d-axis stator current  
                  float  PsiQrS;           // Input: Stationary q-axis rotor flux 
                  float  ThetaFlux;        // Input: Rotor flux angle  
                  u16    PhirUP;           // Input: PhirUP
                  float  WPsi;             // Output: Synchronous rotor flux speed (Q21) - independently with global Q 
                  
                  float WSyn;
                                    
                  float  WrHat;            // Output: Estimated speed in per unit  
                  u32    WrHatRpm;         // Output: Estimated speed in rpm (Q0) - independently with global Q   
                  float  Period;           // Output: Period  
                  float  ThetaRotor;       // Output: Rotor position angle  
                  float  K1;               // Parameter: Constant using in speed computation  
                  float  K2;               // Parameter: Constant using in differentiator (Q21) - independently with global Q 
                  float  K3;               // Parameter: Constant using in low-pass filter   
                  float  K4;               // Parameter: Constant using in low-pass filter  
                  float  K5;               // Parameter: Constant using in rotor position calculation
                  float  K6;               // константа для момента
                  float  K7;               // se1.K7 = _IQ(2*GlobalM3.Zp)/_IQ(3)*_IQ(se1_const.Rr); // 2/3Zp*Rr  Суходольский 21.03.2018
                  float  K8;               // Фильтр скорости
                  float  K9;               // Фильтр скорости
                  float  NewTorq;          // Вновь посчитанный момент
                  float  WRotor;           // Вновь посчитанный момент
                  u32    BaseRpm;          // Parameter: Base rpm speed (Q0) - independently with global Q 
                  float  MaxWrHat;         // Parameter: Max Speed (pu)
                  float  MinWrHat;         // Parameter: Min Speed (pu)   
                  float  OldThetaFlux;     // Variable: Previous rotor flux angle    
                  float  SquaredPsi;       // Variable: Squared rotor flux
                  float  ModulePsi;
                  float  Torq_Alt;         // Variable:  альтернативный моммент
                  float  WSlip;            // Рараметр
                  float  WSlip_filtr;           
                  float  WrHat_filtr;
                  void  (*calc)();         // Pointer to calculation function  
                 } ACISE;            
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
typedef ACISE *ACISE_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the ACISE object.
----------------------------------------------------------------------------- */                    
#define ACISE_DEFAULTS {  0,           /* IQsS           */     \
                          0,           /* PsiDrS         */     \
                          0,           /* IDsS           */     \
                          0,           /* PsiQrS         */     \
                          0,           /* ThetaFlux      */     \
                          0,           /* PhirUP         */     \
                          0,           /* WPsi           */     \
                          0,                                    \
                          0,           /* WrHat          */     \
                          0,           /* WrHatRpm       */     \
                          0,           /* Period         */     \
                          0,           /* ThetaRotor     */     \
                          0.1f,        /* K1             */     \
                          0.1f,        /* K2             */     \
                          0.1f,        /* K3             */     \
                          0.1f,        /* K4             */     \
                          0.1f,        /* K5             */     \
		          0,           /* K6             */     \
		          0,           /* K7             */     \
		          0,           /* K8             */     \
		          0,           /* K9             */     \
		          0.1f,        /* NewTorq        */     \
		          0.1f,        /* WRotor         */     \
                          3600,        /* BaseRpm        */     \
                          0,           /* MaxWrHat       */     \
                          0,           /* MinWrHat       */     \
                          0,           /* OldThetaFlux   */     \
                          0,           /* SquaredPsi     */     \
        		  0,           /* ModulePsi      */     \
                          0,           /* Torq_Alt;      */     \
                          0,           /* WSlip          */     \
                          0,           /* WSlip_filtr    */     \
                          0,           /* WrHat_filtr    */     \
		          (void (*)(u32))aci_se_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_SE.C
------------------------------------------------------------------------------ */
void aci_se_calc(ACISE_handle);

#endif // __ACI_SE_H__
