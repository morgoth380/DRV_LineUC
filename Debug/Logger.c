#define LOGER_GRAPH_IS_PRESENT 1
#if LOGER_GRAPH_IS_PRESENT != 0

#include "DrvCompZPT.h"
#include "Global_include.h"
#include "logger.h"
#include "type.h"
#include "GlobalVar.h"
#include "svgen_dq.h"
#include "ScalarControl.h"
#include "CurVoltMeas.h"
#include "DrvPWM.h"
#include "PwmControl.h"
#include "aci_fe.h"
#include "aci_se.h"
#include "deadtime_comp.h"
#include "svgen_dq.h"
#include "ipark.h"
#include "clarke.h"
#include "iclarke.h"
#include "park.h"
#include "pi_reg_spd.h"
#include "aci_cross_rel_comp.h"
#include "aci_pi_reg_emf.h"
#include "pi_reg_cur.h"
#include "DRIVE_CONST.h"
#include "cntr_voltage.h"
#include "linear_ramp.h"
#include "DrvCharge.h" 
#include "LIB_PRM_MOT.h"
#include "TormSW.h"
#include "anti_rollback.h"
#include "bandstop_filter.h"
#include "aci_pi_reg_psir.h"
#include "Scalar_ORDINAR.h"
#include "meas_lm_cur.h"
#include "load_loss.h"
#include "STFilter.h"
#include "pmsm_cross_rel_comp.h"
#include "pmsm_antirollback.h"


extern SCALARORDINAR ScalarDC_ORDINAR;    // Скаляр для измерителя параметров
extern SCALARCONTROL Scalar;              // Обект расчета скалярного управления
extern CURVOLTMEAS Meas;                  // Обект расчета значений токов и напряжений
extern PWMGEN Pwm;                        // Обект формирователя ШИМ 
extern ACIFE fe1;
extern ACISE se1;
extern DEADTIME_COMP dtc1;
extern SVGENDQ svgen_dqv;
extern IPARK ipark1; 
extern CLARKE clarke1;
extern ICLARKE iclarke1;
extern PARK park1;
extern PIREGCUR pi_isq;
extern PIREGCUR pi_isd;
extern PIREGEMF pi_emf;
extern CROSSRELCOMP cross1;
extern PIREGSPD pi_spd;
extern DRIVECONST DrvConst;  
extern CNTRVOLTAGE cntrvolt1;
extern TORMSW TormSw;                          // Объект управления тормозным ключем
extern PWMCONTROL PwmControl;
extern ANTIROLLBACK    arollback1;
extern MEASLMCUR  measLm2;
extern RAMPCNTLGEN rcg1;
extern FREQREF FreqRef1;
extern PARK park2;
extern COMPZPTDRV CompZPT;
extern CHARGE Charge;           // Объект управления зарядом 
extern ACIPIREGPSIR AciPidRegPsir;
extern PMSMCROSSRELCOMP pmsm_cross1;
extern  PMSMANTIROLLBACK_Obj pmsmAntirollBack;
extern CLARKE  DtcClarke;
extern IPARK   DtcIpark;
extern ICLARKE DtcIclarke;
extern STFILTR DtcSTFilter;               // Блок STFilter для блока dtc


// Прототипы функций
s16 debGetVal(u16 NumVar);      // Получить значение переменной

//****************************************************************************

//****************************************************************************
s16             PrescalLog1     = 10;
s16             ZERO = 0;

u16             LogInd;
u16             ExtRAMmem[szExtRAMmem];
logFile_type    *const logFile = (void*)ExtRAMmem;


//====================================================================
//  Таблица указателей на переменные         
//====================================================================
const LogVarTable_type debPntVar[] = {
//  Адрес переменной           				тип переменной | Масштаб  
    (void*)&ZERO,             				VarLog_s16,	1.0f,    // 0
    (void*)&GlobalM4.ValIu.Val,				VarLog_float,	10.0f,   // 1
    (void*)&GlobalM4.ValIv.Val,				VarLog_float,	10.0f,   // 2
    (void*)&GlobalM4.ValIw.Val,				VarLog_float,	10.0f,   // 3
    (void*)&GlobalM4.ValUd.Val,				VarLog_float,	1.0f,    // 4
    (void*)&GlobalM4.ValIu.FlashVal,			VarLog_float,	10.0f,   // 5
    (void*)&GlobalM4.ValIv.FlashVal,			VarLog_float,	10.0f,   // 6
    (void*)&GlobalM4.ValIw.FlashVal,			VarLog_float,	10.0f,   // 7
    (void*)&GlobalM4.We,				VarLog_float,	10.0f,   // 8
    (void*)&GlobalM4.Wr,				VarLog_float,	10.0f,   // 9
    (void*)&GlobalM4.TIGBT,				VarLog_float,	1.0f,    // 10
    (void*)&pi_spd.Ref,					VarLog_iq,	10.0f*Herz_base, // 11     
    (void*)&pi_spd.Fdb,					VarLog_iq,	10.0f*Herz_base, // 12     
    (void*)&pi_isq.Ref,					VarLog_iq,	10.0f*Current_base,  // 13     
    (void*)&pi_isq.Fdb,					VarLog_iq,	10.0f*Current_base,  // 14     
    (void*)&GlobalM4.Me,				VarLog_float,	10.0f,   // 15
    (void*)&GlobalM4.cosFi,				VarLog_float,	10.0f,   // 16
    (void*)&GlobalM4.Ifull,				VarLog_float,	10.0f,   // 17
    (void*)&GlobalM4.Iact,				VarLog_float,	10.0f,   // 18
    (void*)&GlobalM4.Ireact,				VarLog_float,	10.0f,   // 19
    (void*)&GlobalM4.Pfull,				VarLog_float,	10.0f,   // 20
    (void*)&GlobalM4.Iact,				VarLog_float,	10.0f,   // 21
    (void*)&GlobalM4.Pact,				VarLog_float,	10.0f,   // 22
    (void*)&GlobalM4.Uout,                     	        VarLog_float,	1.0f,    // 23     
    (void*)&GlobalM4.Din1,                             	VarLog_s16,	1.0f,    // 24 Дискретный вход 1
    (void*)&GlobalM4.Din2,                             	VarLog_s16,	1.0f,    // 25 Дискретный вход 2
    (void*)&GlobalM4.Din3,                             	VarLog_s16,	1.0f,    // 26 Дискретный вход 3
    (void*)&GlobalM4.Din4,                             	VarLog_s16,	1.0f,    // 27 Дискретный вход 4
    (void*)&GlobalM4.Din5,                             	VarLog_s16,	1.0f,    // 28 Дискретный вход 5
    (void*)&GlobalM4.Din6,                             	VarLog_s16,	1.0f,    // 29 Дискретный вход 6
    (void*)&GlobalM4.Din7,                             	VarLog_s16,	1.0f,    // 30 Дискретный вход 7
    (void*)&GlobalM4.Dout1,        	                VarLog_s16,	1.0f,    // 31 - Дискретный выход 1
    (void*)&GlobalM4.Dout2,		                VarLog_s16, 	1.0f,    // 32 - Дискретный выход 2
    (void*)&GlobalM4.Dout3,		                VarLog_s16, 	1.0f,    // 33 - Дискретный выход 3
    (void*)&GlobalM4.Ain1,              		VarLog_s16, 	1.0f,    // 34
    (void*)&GlobalM4.Ain2,              		VarLog_s16, 	1.0f,    // 35
    (void*)&GlobalM4.Aout1,              		VarLog_s16, 	1.0f,    // 36    
    (void*)&GlobalM4.Aout2,              		VarLog_s16, 	1.0f,    // 37        
    (void*)&GlobalM4.Din8,                             	VarLog_s16,	1.0f,    // 38 Дискретный вход 8
    (void*)&GlobalM4.Din9,                             	VarLog_s16,	1.0f,    // 39 Дискретный вход 9
    (void*)&GlobalM4.Din10,                             VarLog_s16,	1.0f,    // 40 Дискретный вход 10
    (void*)&GlobalM4.Din11,                             VarLog_s16,	1.0f,    // 41 Дискретный вход 11
    (void*)&GlobalM4.Din12,                             VarLog_s16,	1.0f,    // 42 Дискретный вход 12
    (void*)&GlobalM4.Din13,                             VarLog_s16,	1.0f,    // 43 Дискретный вход 13
    (void*)&GlobalM4.Dout4,        	                VarLog_s16,	1.0f,    // 44 - Дискретный выход 4
    (void*)&GlobalM4.Dout5,		                VarLog_s16, 	1.0f,    // 45 - Дискретный выход 5
    (void*)&GlobalM4.Ain3,              		VarLog_s16, 	1.0f,    // 46
    (void*)&GlobalM4.Ain4,              		VarLog_s16, 	1.0f,    // 47
    (void*)&GlobalM4.Aout3,              		VarLog_s16, 	1.0f,    // 48    
    (void*)&GlobalM4.Aout4,              		VarLog_s16, 	1.0f,    // 49        
    //********************************************************************************
    					
    // clarke				
    (void*)&clarke1.As,					VarLog_float,	100.0f,  // 50       // Input: phase-a stator variable
    (void*)&clarke1.Bs,	        			VarLog_float,	100.0f,  // 51       // Input: phase-b stator variable  
    (void*)&clarke1.Cs,	        			VarLog_float,	100.0f,  // 52       // Input: phase-b stator variable
    (void*)&clarke1.Alpha,				VarLog_float,	3000.0f, // 53       // Output: stationary d-axis stator variable  
    (void*)&clarke1.Beta,				VarLog_float,	3000.0f, // 54       // Output: stationary q-axis stator variable
    
    // park
    (void*)&park1.Alpha,				VarLog_float,	100.0f,  // 55       // Input: stationary d-axis stator variable
    (void*)&park1.Beta,	        			VarLog_float,	100.0f,  // 56       // Input: stationary q-axis stator variable  
    (void*)&park1.Angle,				VarLog_float,	100.0f,  // 57       // Input: rotating angle (pu)
    (void*)&park1.Ds,	        			VarLog_float,	1000.0f, // 58       // Output: rotating d-axis stator variable  
    (void*)&park1.Qs,	        			VarLog_float,	1000.0f, // 59       // Output: rotating q-axis stator variable
				                                                 
    // ipark				                                         
    (void*)&ipark1.Alpha,				VarLog_float,	3000.0f, // 60       // Output: stationary d-axis stator variable
    (void*)&ipark1.Beta,				VarLog_float,	3000.0f, // 61       // Output: stationary q-axis stator variable  
    (void*)&ipark1.Angle,				VarLog_float,	100.0f,  // 62       // Input: rotating angle (pu)
    (void*)&ipark1.Ds,	        			VarLog_float,	1000.0f,  // 63       // Input: rotating d-axis stator variable  
    (void*)&ipark1.Qs,	        			VarLog_float,	1000.0f,  // 64       // Input: rotating q-axis stator variable
				                                                 
    // fe - наблюдатель потока				                         
    (void*)&fe1.Fcur,					VarLog_float,	100.0f,  // 65       // Input: Current frequence
    (void*)&fe1.IDsE,					VarLog_float,	100.0f,  // 66       // Input: Rotating d-axis stator current 
    (void*)&fe1.UDsS,					VarLog_float,	100.0f,  // 67       // Input: Stationary d-axis stator voltage
    (void*)&fe1.UQsS,					VarLog_float,	100.0f,  // 68       // Input: Stationary q-axis stator voltage 
    (void*)&fe1.IDsS,					VarLog_float,	100.0f,  // 69       // Input: Stationary d-axis stator current  
    (void*)&fe1.IQsS,					VarLog_float,	100.0f,  // 70       // Input: Stationary q-axis stator current 
    (void*)&fe1.PhirUP,					VarLog_s16,  	100.0f,  // 71       // Input: Flag Rotor Flux is UP
    (void*)&fe1.GenMode,				VarLog_s16,  	100.0f,  // 72       // Input: Flag Motor Generator mode
    (void*)&fe1.FluxDrE,				VarLog_float,	10000.0f,  // 73       // Variable: Rotating d-axis rotor flux (current model)
    (void*)&fe1.PsiDrS,					VarLog_float,	100.0f,  // 74       // Output: Stationary d-axis estimated rotor flux 
    (void*)&fe1.PsiQrS,					VarLog_float,	100.0f,  // 75       // Output: Stationary q-axis estimated rotor flux 
    (void*)&fe1.ThetaFlux,				VarLog_float,	100.0f,  // 76       // Output: Rotor flux angle 
    
    (void*)&fe1.OldThetaFlux,				VarLog_float,	100.0f,  // 77       // Output: Rotor flux angle
    (void*)&fe1.Kwn,					VarLog_float,	100.0f,  // 78       // Output: weakening of dinamic feature coefficient       
    (void*)&fe1.K1,					VarLog_float,	100.0f,  // 79       // Parameter: Constant using in current model 
    (void*)&fe1.K2,					VarLog_float,	100.0f,  // 80       // Parameter: Constant using in current model 
    (void*)&fe1.K3,					VarLog_float,	100.0f,  // 81       // Parameter: Constant using in stator flux computation 
    (void*)&fe1.K4,					VarLog_float,	100.0f,  // 82       // Parameter: Constant using in stator flux computation 
    (void*)&fe1.K5,					VarLog_float,	100.0f,  // 83       // Parameter: Constant using in back emf computation 
    (void*)&fe1.K6,					VarLog_float,	100.0f,  // 84       // Parameter: Constant using in back emf computation 
    (void*)&fe1.K7,					VarLog_float,	100.0f,  // 85       // Parameter: Constant using in rotor flux computation 
    (void*)&fe1.K8,					VarLog_float,	100.0f,  // 86       // Parameter: Constant using in rotor flux computation 
    (void*)&fe1.Kp,					VarLog_float,	100.0f,  // 87       // Parameter: PI proportionnal gain 
    (void*)&fe1.Ki,					VarLog_float,	100.0f,  // 88       // Parameter: PI integral gain 
    (void*)&fe1.iKwn,					VarLog_float,	100.0f,  // 89       // Parameter: invert weakening of dinamic feature coefficient       
    (void*)&fe1.FluxDrS,				VarLog_float,	100.0f,  // 90       // Variable: Stationary d-axis rotor flux (current model) 
    (void*)&fe1.FluxQrS,				VarLog_float,	100.0f,  // 91       // Variable: Stationary q-axis rotor flux (current model) 
    (void*)&fe1.FluxDsS,				VarLog_float,	100.0f,  // 92       // Variable: Stationary d-axis stator flux (current model) 
    (void*)&fe1.FluxQsS,				VarLog_float,	100.0f,  // 93       // Variable: Stationary q-axis stator flux (current model) 
    (void*)&fe1.PsiDsS,					VarLog_float,	100.0f,  // 94       // Variable: Stationary d-axis stator flux (voltage model) 
    (void*)&fe1.PsiQsS,					VarLog_float,	100.0f,  // 95       // Variable: Stationary q-axis stator flux (voltage model) 
    (void*)&fe1.UiDsS,					VarLog_float,	100.0f,  // 96       // Variable: Stationary d-axis integral term 
    (void*)&fe1.UiQsS,					VarLog_float,	100.0f,  // 97       // Variable: Stationary q-axis integral term  
    (void*)&fe1.UCompDsS,				VarLog_float,	100.0f,  // 98       // Variable: Stationary d-axis compensated voltage 
    (void*)&fe1.UCompQsS,				VarLog_float,	100.0f,  // 99       // Variable: Stationary q-axis compensated voltage 
    (void*)&fe1.EmfDsS,					VarLog_float,	100.0f,  // 100      // Variable: Stationary d-axis back emf 
    (void*)&fe1.EmfQsS,					VarLog_float,	100.0f,  // 101      // Variable: Stationary q-axis back emf  
    (void*)&fe1.KlowSpd,				VarLog_float,	100.0f,  // 102      // Variable: weakening of dinamic integral regulator coefficient       
    (void*)&fe1.OldEmfDsS,				VarLog_float,	100.0f,  // 103      // History: Stationary d-axis back emf 
    (void*)&fe1.OldEmfQsS,				VarLog_float,	100.0f,  // 104      // History: Stationary q-axis back emf
    (void*)&fe1.Filter_Kp,				VarLog_float,	100.0f,  // 105	     // Фильтрованное значение Кр
    (void*)&fe1.Filter_Ki,				VarLog_float,	100.0f,  // 106	     // Фильтрованное значение Кi
    (void*)&fe1.Tpwm,					VarLog_float,	100.0f,  // 107	     // период ШИМа
    (void*)&fe1.PsiR,					VarLog_float,	100.0f,  // 108	     // период фазы
    
    
    (void*)&se1.WPsi,					VarLog_float,	100.0f,  // 109
    (void*)&se1.ModulePsi,      			VarLog_float,	100.0f,  // 110
    (void*)&se1.IQsS,					VarLog_float,	100.0f,  // 111       // Input: Stationary q-axis stator current  
    (void*)&se1.PsiDrS,					VarLog_float,	100.0f,  // 112       // Input: Stationary d-axis rotor flux  
    (void*)&se1.IDsS,					VarLog_float,	100.0f,  // 113       // Input: Stationary d-axis stator current  
    (void*)&se1.PsiQrS,					VarLog_float,	100.0f,  // 114       // Input: Stationary q-axis rotor flux 
    (void*)&se1.ThetaFlux,				VarLog_float,	100.0f,  // 115       // Input: Rotor flux angle  
    (void*)&se1.WPsi,					VarLog_float,	100.0f,  // 116       // Output: Synchronous rotor flux speed (Q21) - independently with global Q 
    (void*)&se1.WrHat,					VarLog_float,	100.0f,  // 117       // Output: Estimated speed in per unit  
    (void*)&se1.Period,					VarLog_float,	100.0f,  // 118       // Output: Period  
    (void*)&se1.ThetaRotor,				VarLog_float,	100.0f,  // 119       // Output: Rotor position angle  
    (void*)&se1.K1,					VarLog_float,	100.0f,  // 120       // Parameter: Constant using in speed computation  
    (void*)&se1.K2,					VarLog_float,	100.0f,  // 121       // Parameter: Constant using in differentiator (Q21) - independently with global Q 
    (void*)&se1.K3,					VarLog_float,	100.0f,  // 122       // Parameter: Constant using in low-pass filter   
    (void*)&se1.K4,					VarLog_float,	100.0f,  // 123       // Parameter: Constant using in low-pass filter  
    (void*)&se1.K5,					VarLog_float,	100.0f,  // 124       // Parameter: Constant using in rotor position calculation
    (void*)&se1.K6,					VarLog_float,	100.0f,  // 125       // константа для момента
    (void*)&se1.K7,					VarLog_float,	100.0f,  // 126       // se1.K7 = _IQ(2*GlobalM3.Zp)/_IQ(3)*_IQ(se1_const.Rr); // 2/3Zp*Rr  Суходольский 21.03.2018
    (void*)&se1.NewTorq,				VarLog_float,	100.0f,  // 127       // Вновь посчитанный момент
    (void*)&se1.WRotor,					VarLog_float,	100.0f,  // 128       // Вновь посчитанный момент
    (void*)&se1.MaxWrHat,				VarLog_float,	100.0f,  // 129       // Parameter: Max Speed (pu)
    (void*)&se1.MinWrHat,				VarLog_float,	100.0f,  // 130       // Parameter: Min Speed (pu)   
    (void*)&se1.OldThetaFlux,				VarLog_float,	100.0f,  // 131       // Variable: Previous rotor flux angle    
    (void*)&se1.SquaredPsi,				VarLog_float,	100.0f,  // 132       // Variable: Squared rotor flux
    (void*)&se1.ModulePsi,				VarLog_float,	100.0f,  // 133
    (void*)&se1.Torq_Alt,				VarLog_float,	100.0f,  // 134       // Variable:  альтернативный моммент
    (void*)&se1.WSlip,					VarLog_float,	100.0f,  // 135       // Рараметр
    (void*)&se1.WSyn,					VarLog_float,	100.0f,  // 136
    (void*)&ZERO,					VarLog_float,	100.0f,  // 137
    (void*)&Meas.Iu,					VarLog_float,	100.0f,  // 138
    (void*)&Meas.Iv,					VarLog_float,	100.0f,  // 139
    (void*)&Meas.Iw,					VarLog_float,	100.0f,  // 140
    
    // pi_spd
    (void*)&pi_spd.Ref,					VarLog_iq,	100.0f*Herz_base, // 141    // Input: Reference input 
    (void*)&pi_spd.Fdb,					VarLog_iq,	100.0f*Herz_base, // 142    // Input: Feedback input 
    (void*)&pi_spd.Ref_filtr,				VarLog_iq,	100.0f*Herz_base,  // 143    // Variable: Filtred Reference input 
    (void*)&pi_spd.Ui,					VarLog_iq,	10.0f*Current_base,  // 144    // Variable: Integral output 
    (void*)&pi_spd.Kp,					VarLog_float,	100.0f,  // 145    // Parameter: Proportional gain
    (void*)&pi_spd.Ki,					VarLog_float,	100.0f,  // 146    // Parameter: Integral gain
    (void*)&pi_spd.OutMax,				VarLog_iq,	10.0f*Current_base,  // 147    // Parameter: Maximum output 
    (void*)&pi_spd.Out,					VarLog_iq,	10.0f*Current_base,  // 148    // Output: PI output 
						                                 
    // pi_isq				                                         
    (void*)&pi_isq.Ref,					VarLog_iq,	10.0f*Current_base,  // 149    // Input: Reference input 
    (void*)&pi_isq.Fdb,					VarLog_iq,	10.0f*Current_base,  // 150    // Input: Feedback input 
    (void*)&pi_isq.Ui,					VarLog_float,	100.0f,  // 151    // Variable: Integral output 
    (void*)&pi_isq.OutMax,				VarLog_float,	100.0f,  // 152    // Parameter: Maximum output 
    (void*)&pi_isq.Out,					VarLog_float,	100.0f,  // 153    // Output: PI output 
				                                                 
    // pi_isd				                                         
    (void*)&pi_isd.Ref,					VarLog_iq,	10.0f*Current_base,  // 154   // Input: Reference input 
    (void*)&pi_isd.Fdb,					VarLog_iq,	10.0f*Current_base,  // 155   // Input: Feedback input 
    (void*)&pi_isd.Ui,					VarLog_float,	100.0f,  // 156   // Variable: Integral output 
    (void*)&pi_isd.OutMax,				VarLog_float,	100.0f,  // 157   // Parameter: Maximum output 
    (void*)&pi_isd.Out,					VarLog_float,	100.0f,  // 158   // Output: PI output 
    
    // pi_emf    
    (void*)&pi_emf.Ref,		        		VarLog_float, 	100.0f,  // 159   // Input: Reference input 
    (void*)&pi_emf.Fdb,		        		VarLog_float, 	100.0f,  // 160   // Input: Feedback input 
    (void*)&pi_emf.Ui,		        		VarLog_float, 	100.0f,  // 161   // Variable: Integral output 
    (void*)&pi_emf.OutPreSat,				VarLog_float, 	100.0f,  // 162   // Variable: Pre-saturated output
    (void*)&pi_emf.Kp,		        		VarLog_float, 	100.0f,  // 163   // Parameter: Proportional gain
    (void*)&pi_emf.Ki,		        		VarLog_float, 	100.0f,  // 164   // Parameter: Integral gain
    (void*)&pi_emf.OutMax,				VarLog_float, 	100.0f,  // 165   // Parameter: Maximum output 
    (void*)&pi_emf.Out,		        		VarLog_float, 	100.0f,  // 166   // Output: Regulator output 
					                                            
    // cross1			                                                    
    (void*)&cross1.Urd,		        		VarLog_float, 	100.0f,  // 167   // Input: выход регулятора тока Id (pu)
    (void*)&cross1.Urq,		        		VarLog_float, 	100.0f,  // 168   // Input: выход регулятора тока Iq (pu)
    (void*)&cross1.Usmax,				VarLog_float, 	100.0f,  // 169   // Input: максимальное занчение вектора напряжения (pu)
    (void*)&cross1.Usd,		        		VarLog_float, 	100.0f,  // 170   // Output: компенсированное и ограниченное напряжение по оси d (pu)
    (void*)&cross1.Usq,		        		VarLog_float, 	100.0f,  // 171   // Output: компенсированное и ограниченное напряжение по оси q (pu)
    (void*)&cross1.Us,		        		VarLog_float, 	100.0f,  // 172   // Output: результирующий вектор напряжения (pu)
    (void*)&cross1.UsmaxFlag,				VarLog_s16, 	1.0f,    // 173   

    (void*)&DrvConst.Isn,				VarLog_float, 	100.0f,  // 174   
    
// Meas params    
    (void*)&cntrvolt1.f32IsU,				VarLog_float,	100.0f,  // 175     //     
    (void*)&cntrvolt1.Ifaz,				VarLog_float,	100.0f,  // 176     // Input: full current(pu) 
    (void*)&cntrvolt1.s16IsU, 				VarLog_s16,  	1.0f,    // 177     //
    (void*)&cntrvolt1.Imax,  				VarLog_float,	100.0f,  // 178     //
    (void*)&cntrvolt1.UlineStep,  			VarLog_float,	100.0f,  // 179     // Input: output line voltage step (pu) 
    (void*)&cntrvolt1.Uline,  				VarLog_float,	100.0f,  // 180     // Output: output line voltage (pu)  
    (void*)&cntrvolt1.UmaxFlag,  			VarLog_float,	100.0f,  // 181     // Output: напряжение достигло максимума
    (void*)&cntrvolt1.Umax,  				VarLog_float,	100.0f,  // 182     // Parameter: 
    (void*)&cntrvolt1.Itst,  				VarLog_float,	100.0f,  // 183  
    (void*)&cntrvolt1.f32III,  				VarLog_float,	100.0f,  // 184  
    (void*)&cntrvolt1.Log0,  				VarLog_float,	10000.0f,  // 185  
    (void*)&cntrvolt1.Log1,  				VarLog_float,	10000.0f,  // 186  
    (void*)&cntrvolt1.Log2,  				VarLog_float,	10000.0f,  // 187  
    (void*)&cntrvolt1.Log3,  				VarLog_float,	10000.0f,  // 188  
    (void*)&cntrvolt1.Log4,  				VarLog_float,	10000.0f,  // 189  
    (void*)&cntrvolt1.LogN,  				VarLog_float,	10000.0f,  // 190  
    (void*)&cntrvolt1.Lm0,  				VarLog_float,	100.0f,  // 191  
    (void*)&cntrvolt1.Lm1,  				VarLog_float,	100.0f,  // 192   
    (void*)&cntrvolt1.Lm2,  				VarLog_float,	100.0f,  // 193  
    (void*)&cntrvolt1.Lm3,  				VarLog_float,	100.0f,  // 194  
    (void*)&cntrvolt1.s16LogN, 				VarLog_float,  	100.0f,    // 195
    (void*)&cntrvolt1.s16Lm0, 				VarLog_float,  	100.0f,    // 196
    (void*)&cntrvolt1.s16Lm1, 				VarLog_float,  	100.0f,    // 197
    (void*)&cntrvolt1.s16Lm2, 				VarLog_float,  	100.0f,    // 198
    (void*)&cntrvolt1.s16Lm3, 				VarLog_float,  	100.0f,    // 199
    (void*)&cntrvolt1.s16Lm, 				VarLog_float,  	100.0f,    // 200
    (void*)&cntrvolt1.s16III, 				VarLog_float,  	100.0f,    // 201
    (void*)&cntrvolt1.Period, 				VarLog_float,  	100.0f,    // 202     // Input: Period of Output voltage
    
    // rcg1
    (void*)&rcg1.Framp,  		         	VarLog_float, 	100.0f,  // 203   // Input: Ramp frequency (pu)   
    (void*)&rcg1.Fwork,  		         	VarLog_float, 	100.0f,  // 204   // Input: Work frequency (pu) 
    (void*)&rcg1.Fnom,  		         	VarLog_float, 	100.0f,  // 205   // Input: Nominal frequency (pu)
    (void*)&rcg1.StepAngleMax,  		 	VarLog_float, 	100.0f,  // 206   // Parameter: Maximum step angle (pu)   
    (void*)&rcg1.StepFreqMax,  		         	VarLog_float, 	100.0f,  // 207   // Parameter: Maximum step Frequence (pu)       
    (void*)&rcg1.Angle,  		         	VarLog_float, 	100.0f,  // 208   // Output: Step angle (pu)  
    (void*)&rcg1.Uout,  		         	VarLog_float, 	100.0f,  // 209   // Output:  (pu)    
    (void*)&rcg1.Period,  		         	VarLog_s16,   	100.0f,  // 210   // Output: period               
		
    // FreqRef1		
    (void*)&FreqRef1.LsLineFreq,  		 	VarLog_float, 	100.0f,  // 211
    (void*)&FreqRef1.LsCyclFreq,  		 	VarLog_float, 	100.0f,  // 212
    (void*)&FreqRef1.LmLineFreq,  		 	VarLog_float, 	100.0f,  // 213
    (void*)&FreqRef1.LmCyclFreq,  		 	VarLog_float, 	100.0f,  // 214
		
    // park2		
    (void*)&park2.Alpha,  		         	VarLog_float, 	100.0f,  // 215    // Input: stationary d-axis stator variable 
    (void*)&park2.Beta,  		         	VarLog_float, 	100.0f,  // 216    // Input: stationary q-axis stator variable 
    (void*)&park2.Angle,  		         	VarLog_float, 	100.0f,  // 217    // Input: rotating angle (pu) 
    (void*)&park2.Ds,  		                 	VarLog_float, 	100.0f,  // 218    // Output: rotating d-axis stator variable 
    (void*)&park2.Qs,  		                 	VarLog_float, 	100.0f,  // 219    // Output: rotating q-axis stator variable 
		
    // iclarke1		
    (void*)&iclarke1.Alpha,  		         	VarLog_float, 	100.0f,  // 220    // Output: stationary d-axis stator variable 
    (void*)&iclarke1.Beta,  		         	VarLog_float, 	100.0f,  // 221    // Output: stationary q-axis stator variable
    (void*)&iclarke1.As,  		         	VarLog_float, 	100.0f,  // 222    // Input: phase-a stator variable
    (void*)&iclarke1.Bs,  		         	VarLog_float, 	100.0f,  // 223    // Input: phase-b stator variable 
    (void*)&iclarke1.Cs,  		         	VarLog_float, 	100.0f,  // 224    // Input: phase-c stator variable 
    
    // measRP2
    (void*)&measRP2.Uline,  				VarLog_float,	100.0f,  // 225    // Input: output line voltage (pu) 
    (void*)&measRP2.Ifaz,  				VarLog_float,	100.0f,  // 226    // Input: full current(pu)
    (void*)&measRP2.UlineFlash,  			VarLog_float,	100.0f,  // 227    // Input: output line flash voltage (pu) 
    (void*)&measRP2.IfazFlash,  			VarLog_float,	100.0f,  // 228    // Input: Flash current (pu)
    (void*)&measRP2.Wout,  				VarLog_float,	100.0f,  // 229    // Input: output cyclic frequence (pu)
    (void*)&measRP2.Rs,  				VarLog_float,	100.0f,  // 230    // Input: Stator resistance (ohm)
    (void*)&measRP2.Rr,  				VarLog_float,	100.0f,  // 231    // Output: Rotor resistance (ohm)
    (void*)&measRP2.sLr,  				VarLog_float,	10000.0f,  // 232    // Output: Rotor inductance (H)
    (void*)&measRP2.K1,  				VarLog_float,	100.0f,  // 233    // Parameter: 
    (void*)&measRP2.K2,  				VarLog_float,	100.0f,  // 234    // Parameter: 
    (void*)&measRP2.Pact,  				VarLog_float,	100.0f,  // 235    // Variable:
    (void*)&measRP2.Pact1,  				VarLog_float,	100.0f,  // 236    // Variable:
    (void*)&measRP2.PwrSum,  				VarLog_float,	100.0f,  // 237    // Variable:  
    (void*)&measRP2.Zeq,  				VarLog_float,	100.0f,  // 238    // Variable:       
    (void*)&measRP2.Req,  				VarLog_float,	100.0f,  // 239    // Variable:				
    (void*)&measRP2.Counter, 				VarLog_s16,  	1.0f,    // 240    // Variable:
    (void*)&measRP2.Period, 				VarLog_s16,  	1.0f,    // 241    // Input: Period of Output voltage  		
    (void*)&PwmControl.StatusPwm,       		VarLog_s16,  	1.0f,    // 242       
    (void*)&Pwm.StatusPwm,              		VarLog_s16,  	1.0f,    // 243    // Текущий статус модуля ШИМ		
    (void*)&Meas.Iu,  		        		VarLog_float,	100.0f,  // 244    // Выход:     Значение тока Iu
    (void*)&Meas.Iv,  		        		VarLog_float,	100.0f,  // 245    // Выход:     Значение тока Iv
    (void*)&Meas.Iw,  		        		VarLog_float,	100.0f,  // 246    // Выход:     Значение тока Iw
    (void*)&Meas.Pact,  				VarLog_float,	1.0f,    // 247    // Выход:     Значение активного тока
    (void*)&Meas.Preact,  				VarLog_float,	1.0f,    // 248    // Выход:     Значение реактивного тока 
    (void*)&Meas.Pfull,  				VarLog_float,	1.0f,    // 249    // Выход:     Полный ток
    (void*)&GlobalM4.Fref,		   		VarLog_float,   10.0f,   // 250 - Гц, один знак после запятой
    (void*)&Meas.Angle,  		   		VarLog_float,   100.0f,  // 251    // Выход:     Полный ток
    (void*)&DrvConst.Period,  		   		VarLog_s16,     100.0f,  // 252    // Выход:     Полный ток
    (void*)&CompZPT.CountFront,            		VarLog_s16,     100.0f,  // 253    // Выход:     Полный ток  
    (void*)&pi_spd.OutMax,		   		VarLog_float,   100.0f,  // 254     // Parameter: Maximum output 
    (void*)&pi_spd.OutMin,		   		VarLog_float,   100.0f,  // 255     // Parameter: Maximum output 
    (void*)&GlobalM4.TorqueMax,		   		VarLog_float,   100.0f,  // 256     // Parameter: Maximum output 
    (void*)&GlobalM4.TorqueMin,		   		VarLog_float,   100.0f,  // 257     // Parameter: Maximum output 
    (void*)&GlobalM4.BrakeState,  	   		VarLog_s16,     1.0f,    // 258    // Выход:     Состояние механического тормоза
    (void*)&GlobalM4.BrakeAlgStatus,          		VarLog_s16,     1.0f,    // 259    // Выход:     Состояние командоаппарата механического тормоза
    
    (void*)&Pwm.PwmOn,          		        VarLog_s16,     1.0f,    // 260    // Состояние ШИМа - Вкл/Выкл
    (void*)&GlobalM4.PickupFreqFound,          		VarLog_s16,     1.0f,    // 261    // Сигнал от подхвата что частота найдена
    (void*)&GlobalM4.PickUpReadyToGo,          		VarLog_s16,     1.0f,    // 262    // Сигнал от подхвата, что можно переходить на нормальную работу для ВУ    
    
    (void*)&GlobalM4.Debug1,              		VarLog_float,   100.0f,    // 263
    (void*)&GlobalM4.Debug2,              		VarLog_float,   100.0f,    // 264
    (void*)&GlobalM4.Debug3,              		VarLog_float,   100.0f,    // 265
    (void*)&GlobalM4.Debug4,              		VarLog_float,   100.0f,    // 266
    (void*)&GlobalM4.Debug5,              		VarLog_float,   100.0f,    // 267
    (void*)&GlobalM4.Debug6,              		VarLog_float,   100.0f,    // 268
    (void*)&GlobalM4.Debug7,              		VarLog_float,   100.0f,    // 269
    (void*)&GlobalM4.Debug8,              		VarLog_float,   100.0f,    // 270
    (void*)&GlobalM4.Debug9,              		VarLog_float,   100.0f,    // 271
    (void*)&GlobalM4.Debug10,              		VarLog_float,   100.0f,    // 272
        
    (void*)&GlobalM4.Utab_uf,              		VarLog_float,   100.0f,     // 273 - Напряжение для Подхвата двигателя
    (void*)&GlobalM4.Iflash,              		VarLog_float,   10.0f,     // 274 - Полный мгновенный ток
    (void*)&GlobalM4.FinLsu,              		VarLog_float,   100.0f,    // 275 - Входное задание частоты генератора темпа iq    
    (void*)&GlobalM4.GTState,              		VarLog_s16,     1.0f,      // 276 
    (void*)&GlobalM4.GTStatus,              		VarLog_s16,     1.0f,      // 277    
    (void*)&GlobalM4.FlsuPickup,              		VarLog_float,   100.0f,    // 278
    (void*)&GlobalM4.PickupEnable,              	VarLog_s16,     1.0f,      // 279 - Статус подхвата в LSU          
    (void*)&DrvConst.DriveType,              	        VarLog_s16,     1.0f,      // 280 - Тип алгоритма работы ПЧ - скаляр, ВУ - посик частоты Подхвата
    (void*)&GlobalM4.vPrUP,              	        VarLog_s16,     1.0f,      // 281 Двигатель возбужден
    (void*)&DrvConst.Wrz,              	                VarLog_float,   100.0f,    // 282 Задание скорости
    
    (void*)&pi_spd.Up,					VarLog_float,	100.0f,  // 283   
    (void*)&pi_spd.Ui,					VarLog_float,	100.0f,  // 284   
    (void*)&pi_spd.Err,					VarLog_float,	100.0f,  // 285   
    (void*)&GlobalM4.PickupStateFOC,                    VarLog_s16,     1.0f,    // 286 Статус подхвата в ВУ блоке
    
    (void*)&GlobalM4.Uref,				VarLog_float,	100.0f,  // 287     
    (void*)&DrvConst.Uscl,				VarLog_float,	100.0f,  // 288   
          
    (void*)&GlobalM4.ValIu.AdcVal,               	VarLog_s16,	1.0f,    // 289        // Ток фазы U в кодах АЦП 
    (void*)&GlobalM4.ValIv.AdcVal,               	VarLog_s16,	1.0f,    // 290        // Ток фазы V в кодах АЦП 
    (void*)&GlobalM4.ValIw.AdcVal,               	VarLog_s16,	1.0f,    // 291        // Ток     // 6        // Аналоговый вход 2 в кодах АЦП
    (void*)&GlobalM4.ValUd.AdcVal,               	VarLog_s16,	1.0f,    // 292        // Напряжение в ЗПТ в кодах АЦП  
    (void*)&GlobalM4.LowSpeedAinAdc.TempIGBTU,    	VarLog_s16,	1.0f,    // 293        // Температура ключей в кодах АЦП
    (void*)&GlobalM4.LowSpeedAinAdc.Ain2,        	VarLog_s16,	1.0f,    // 294        // Аналоговый вход 2 в кодах АЦП
    (void*)&GlobalM4.LowSpeedAinAdc.Ain1,        	VarLog_s16,	1.0f,    // 295        // Аналоговый вход 1 в кодах АЦП
    (void*)&GlobalM4.LowSpeedAinAdc.AdcVoltRef,  	VarLog_s16,	1.0f,    // 296        // Опорное напряжение АЦП в кодах АЦП
    (void*)&GlobalM4.LowSpeedAinAdc.Vbat,        	VarLog_s16,	1.0f,    // 297        // Напряжение батарейки в кодах АЦП
    (void*)&ZERO,					VarLog_float,	100.0f,  // 298
    (void*)&ZERO,					VarLog_float,	100.0f,  // 299
    (void*)&ZERO,					VarLog_float,	100.0f,  // 300
    (void*)&ZERO,					VarLog_float,	100.0f,  // 301
    // Модуль ШИМ    
    (void*)&Pwm.Ta,					VarLog_float,	100.0f,  // 302       // Задание на ШИМ фаза U
    (void*)&Pwm.Tb,					VarLog_float,	100.0f,  // 303       // Задание на ШИМ фаза V
    (void*)&Pwm.Tc,					VarLog_float,	100.0f,  // 304       // Задание на ШИМ фаза W
    
    // Линейный ЗИ 1
    (void *)&linear_ramp1.StartFlag,                    VarLog_s16,     1.0f,   // 305 Флаг старта ЗИ
    (void *)&linear_ramp1.WorkFlag,                     VarLog_s16,     1.0f,   // 306 Флаг работы ЗИ
    (void *)&linear_ramp1.WorkDone,                     VarLog_s16,     1.0f,   // 307 Флаг завершения работы ЗИ
    (void *)&linear_ramp1.StartValue,                   VarLog_float,   100.0f,  // 308 Начальное значение ЗИ
    (void *)&linear_ramp1.EndValue,                     VarLog_float,   100.0f,  // 309 Конечное значение ЗИ    
    (void *)&linear_ramp1.Out,                          VarLog_float,   100.0f,  // 310 Выходное значение ЗИ    
        
    (void*)&GlobalM4.LowSpeedAinAdc.TempIGBTU,    	VarLog_s16,	1.0f,    // 311        // Температура ключей в кодах АЦП    
    (void*)&GlobalM4.LowSpeedAinAdc.TempIGBTV,    	VarLog_s16,	1.0f,    // 312        // Температура ключей в кодах АЦП    
    (void*)&GlobalM4.LowSpeedAinAdc.TempIGBTW,    	VarLog_s16,	1.0f,    // 313        // Температура ключей в кодах АЦП    
    (void*)&GlobalM4.LowSpeedAinAdc.TempBoard,    	VarLog_s16,	1.0f,    // 314        // Температура ключей в кодах АЦП    

    // Модуль ШИМ    
    (void*)&Pwm.MaxCountT1,			        VarLog_s16,	1.0f,    // 315       // Задание на ШИМ в тиках
    (void*)&Pwm.UvalTimer,				VarLog_s16,	1.0f,    // 316       // Задание на ШИМ в тиках
    (void*)&Pwm.VvalTimer,				VarLog_s16,	1.0f,    // 317       // Задание на ШИМ в тиках
    (void*)&Pwm.WvalTimer,				VarLog_s16,	1.0f,    // 318       // Задание на ШИМ в тиках
    
    (void*)&DrvConst.IsU,				VarLog_float, 	100.0f,  // 319
    (void*)&DrvConst.IsV,				VarLog_float, 	100.0f,  // 320
    (void*)&DrvConst.IsW,				VarLog_float, 	100.0f,  // 321
    (void*)&Meas.Iu_adcParam.Offset,			VarLog_s16,	1.0f,    // 322
    (void*)&Meas.Iv_adcParam.Offset,			VarLog_s16,	1.0f,    // 323
    (void*)&Meas.Iw_adcParam.Offset,			VarLog_s16,	1.0f,    // 324
    (void*)&GlobalM4.AdcOffsetIu,			VarLog_s16,	1.0f,    // 325
    (void*)&GlobalM4.AdcOffsetIv,			VarLog_s16,	1.0f,    // 326
    (void*)&GlobalM4.AdcOffsetIw,			VarLog_s16,	1.0f,    // 327 
    
    (void*)&GlobalM4.AdcVoltRef_Kkor,			VarLog_float,	1.0f,    // 328
    (void*)&DrvConst.Wr,              	                VarLog_iq,      10.0f*Herz_base,  // 329 Скорость ротора
    (void*)&DrvConst.We,				VarLog_iq,	10.0f*Herz_base,  // 330 Скорость поля
    (void*)&DrvConst.Us,				VarLog_float,	10.0f,   // 331 Напряжение на выходе ПЧ
    (void*)&DrvConst.Uscl,				VarLog_float,	10.0f,   // 332 Задание Напряжения в скалярном режиме
    (void*)&DrvConst.Angle,				VarLog_float,	10.0f,   // 333 Фаза в скаляре
        
    (void *)&linear_ramp1.RampStep,                     VarLog_float,   100.0f,  // 334 Шаг ЗИ
    (void *)&linear_ramp1.RampTime,                     VarLog_float,   10.0f,   // 335 Время ЗИ    
    // Линейный ЗИ 2 на РС
    (void *)&linear_ramp2.StartFlag,                    VarLog_s16,     1.0f,   // 336 Флаг старта ЗИ
    (void *)&linear_ramp2.WorkFlag,                     VarLog_s16,     1.0f,   // 337 Флаг работы ЗИ
    (void *)&linear_ramp2.WorkDone,                     VarLog_s16,     1.0f,   // 338 Флаг завершения работы ЗИ
    (void *)&linear_ramp2.StartValue,                   VarLog_float,   100.0f,  // 339 Начальное значение ЗИ
    (void *)&linear_ramp2.EndValue,                     VarLog_float,   100.0f,  // 340 Конечное значение ЗИ    
    (void *)&linear_ramp2.Out,                          VarLog_float,   100.0f,  // 341 Выходное значение ЗИ    
    (void *)&linear_ramp2.RampStep,                     VarLog_float,   100.0f,  // 342 Шаг ЗИ
    (void *)&linear_ramp2.RampTime,                     VarLog_float,   10.0f,   // 343 Время ЗИ    
    
    (void *)&DrvConst.StartWithPickup,			VarLog_s16,	10.0f,   // 344 ФЛаг перехода из Подхвата
    (void *)&Meas.Ud_adc,			        VarLog_s16,	1.0f,    // 345 АЦП в расчет напряжений
    
    (void*)&GlobalM4.s16_PwmOnM3,	   		VarLog_s16,     1.0f,    // 346             
    (void*)&GlobalM4.s16_Brake,	   		        VarLog_s16,     1.0f,    // 347     
    (void*)&GlobalM4.s16_Contactor,	   		VarLog_s16,     1.0f,    // 348     
    (void*)&GlobalM4.s16_AlgState,	   		VarLog_s16,     1.0f,    // 349     
    (void*)&GlobalM4.s16_StartStop,	   		VarLog_s16,     1.0f,    // 350     
    (void*)&GlobalM4.s16_LiftPuskOut,	   		VarLog_s16,     1.0f,    // 351         
    (void*)&GlobalM4.s16_VprUpState,	   		VarLog_s16,     1.0f,    // 352             
    (void*)&GlobalM4.s16_F_Ref,	   		        VarLog_s16,     1.0f,    // 353                 
    (void*)&GlobalM4.DF_NumSpeed,		        VarLog_s16,     1.0f,    // 354                     
    (void*)&GlobalM4.s16_PuskStateDRV,		        VarLog_s16,     1.0f,    // 355 
     // Входная сеть
    (void *)&GlobalM4.ValUab.Val,                       VarLog_float,   10.0f,   // 356 RMS
    (void *)&GlobalM4.ValUbc.Val,                       VarLog_float,   10.0f,   // 357 RMS
    (void *)&GlobalM4.ValUca.Val,                       VarLog_float,   10.0f,   // 358 RMS
    (void *)&GlobalM4.ValUab.FlashVal,                  VarLog_float,   10.0f,   // 359 Flash
    (void *)&GlobalM4.ValUbc.FlashVal,                  VarLog_float,   10.0f,   // 360 Flash 
    (void *)&GlobalM4.ValUca.FlashVal,                  VarLog_float,   10.0f,   // 361 Flash
    (void *)&GlobalM4.ValUab.AdcVal,                    VarLog_s16,     1.0f,    // 362 adc
    (void *)&GlobalM4.ValUbc.AdcVal,                    VarLog_s16,     1.0f,    // 363 adc
    (void *)&GlobalM4.ValUca.AdcVal,                    VarLog_s16,     1.0f,    // 364 adc

    (void*)&GlobalM4.MeasTime[0].ValTekTime,		VarLog_s16, 	1.0f,    // 365 TskMicroProgTC
    (void*)&GlobalM4.MeasTime[1].ValTekTime,		VarLog_s16, 	0.1f,    // 366 PeriodMicroProgTC
    (void*)&GlobalM4.MeasTime[2].ValTekTime,		VarLog_s16, 	1.0f,    // 367 TskModbusSlaveTC
    (void*)&GlobalM4.MeasTime[3].ValTekTime,		VarLog_s16, 	1.0f,    // 368 TskModbusMasterTC 
    (void*)&GlobalM4.MeasTime[4].ValTekTime,		VarLog_s16, 	1.0f,    // 369 TskFlashTC
    (void*)&GlobalM4.MeasTime[5].ValTekTime,		VarLog_s16, 	1.0f,    // 370 PeriodADC
    (void*)&GlobalM4.MeasTime[6].ValTekTime,		VarLog_s16, 	1.0f,    // 371 PwmPeriodProcessingTC
    (void*)&GlobalM4.MeasTime[7].ValTekTime,		VarLog_s16, 	1.0f,    // 372 PwmPeriodTC 

    // svgen				
    (void*)&svgen_dqv.Ta,				VarLog_float,	100.0f,  // 373       // Output: reference phase-a switching function
    (void*)&svgen_dqv.Tb,				VarLog_float,	100.0f,  // 374       // Output: reference phase-b switching function 
    (void*)&svgen_dqv.Tc,				VarLog_float,	100.0f,  // 375       // Output: reference phase-c switching function
    (void*)&svgen_dqv.Ta_60,				VarLog_float,	100.0f,  // 376       // Output: reference phase-a switching function
    (void*)&svgen_dqv.Tb_60,				VarLog_float,	100.0f,  // 377       // Output: reference phase-b switching function 
    (void*)&svgen_dqv.Tc_60,				VarLog_float,	100.0f,  // 378       // Output: reference phase-c switching function
    
    (void*)&Scalar.Ds,				        VarLog_float,	100.0f,  // 379       // Output: reference phase-c switching function    
            
    (void*)&loadContyChk.bitStart,		        VarLog_s16,     1.0f,    // 380
    (void*)&loadContyChk.Iu,    		        VarLog_float,   100.0f,  // 381
    (void*)&loadContyChk.Iv,    		        VarLog_float,   100.0f,  // 382
    (void*)&loadContyChk.Iw,    		        VarLog_float,   100.0f,  // 383
    (void*)&loadContyChk.Ichk,    		        VarLog_float,   100.0f,  // 384
    (void*)&loadContyChk.Imin,    		        VarLog_float,   100.0f,  // 385
    (void*)&loadContyChk.timerTestTime,		        VarLog_s16,     1.0f,    // 386
    (void*)&loadContyChk.Error,		                VarLog_s16,     1.0f,    // 387  
    (void*)&loadContyChk.filtrIu,    		        VarLog_float,   100.0f,  // 388
    (void*)&loadContyChk.filtrIv,    		        VarLog_float,   100.0f,  // 389
    (void*)&loadContyChk.filtrIw,    		        VarLog_float,   100.0f,  // 390
    
    (void*)&Charge.ReleState,		                VarLog_s16,     1.0f,    // 391            
    (void*)&Charge.ZarState,		                VarLog_s16,     1.0f,    // 392                
    // Тормозной резистор
    (void*)&GlobalM4.TormSwParam.TormCheckEn,	        VarLog_s16,     1.0f,    // 393                
    (void*)&GlobalM4.TormSwParam.TormCheckStart,        VarLog_s16,     1.0f,    // 394                
    (void*)&GlobalM4.TormSwParam.TormCheckEnd,          VarLog_s16,     1.0f,    // 395                    
    (void*)&TormSw.State,                               VarLog_s16,     1.0f,    // 396                    
        
    (void*)&pi_spd.Ui,					VarLog_float,	1000.0f,  // 397    // Variable: Integral output 
    (void*)&pi_spd.Ui_prev,				VarLog_float,	1000.0f,  // 398    // Variable: Integral output 
    (void*)&pi_spd.SavePrevUiFlag,			VarLog_s16,	1.0f,     // 399    // Variable: Integral output 
    (void*)&pi_spd.RestorePrevUiFlag,			VarLog_s16,	1.0f,     // 400    // Variable: Integral output 
    (void*)&pi_spd.Up,					VarLog_float,	1000.0f,  // 401    // Parameter: Proportional gain
    (void*)&pi_spd.Err,					VarLog_float,	1000.0f,  // 402    // Parameter: Integral gain
    (void*)&pi_spd.Out,					VarLog_float,	1000.0f,  // 403    // Parameter: Integral gain

    // Компенсация дедтайма				
    (void*)&dtc1.Ta,					VarLog_float,	100.0f,  // 404       // Задание на ШИМ фаза U
    (void*)&dtc1.Tb,					VarLog_float,	100.0f,  // 405       // Задание на ШИМ фаза V
    (void*)&dtc1.Tc,					VarLog_float,	100.0f,  // 406       // Задание на ШИМ фаза W
    (void*)&dtc1.Iu,					VarLog_float,	100.0f,  // 407       // Input: phase-u stator current
    (void*)&dtc1.Iv,					VarLog_float,	100.0f,  // 408       // Input: phase-v stator current
    (void*)&dtc1.Iw,					VarLog_float,	100.0f,  // 409       // ЗInput: phase-w stator current
				
    // svgen				
    (void*)&svgen_dqv.Ualpha,				VarLog_float,	100.0f,  // 410       // Input: reference alpha-axis phase voltage
    (void*)&svgen_dqv.Ubeta,				VarLog_float,	100.0f,  // 411       // Input: reference beta-axis phase voltage 
    (void*)&svgen_dqv.Ta,				VarLog_float,	100.0f,  // 412       // Output: reference phase-a switching function
    (void*)&svgen_dqv.Tb,				VarLog_float,	100.0f,  // 413       // Output: reference phase-b switching function 
    (void*)&svgen_dqv.Tc,				VarLog_float,	100.0f,  // 414       // Output: reference phase-c switching function
    
    (void*)&GlobalM4.GenMode,                           VarLog_s16,     1.0f,    // 415 - Генераторный режим ВУ                    
    
    
    (void*)&GlobalM4.ValUu.FlashVal,               	VarLog_float,	100.0f,    // 416        // Ток фазы U в кодах АЦП 
    (void*)&GlobalM4.ValUv.FlashVal,               	VarLog_float,	100.0f,    // 417        // Ток фазы V в кодах АЦП 
    (void*)&GlobalM4.ValUw.FlashVal,               	VarLog_float,	100.0f,    // 418        // Ток     // 6        // Аналоговый вход 2 в кодах АЦП
    
    (void*)&clarke1.AlphaFiltr,				VarLog_float,	3000.0f,   // 419       // Output: stationary d-axis stator variable  
    (void*)&clarke1.BetaFiltr,				VarLog_float,	3000.0f,   // 420      // Output: stationary q-axis stator variable
    
    (void*)&reject1.In,	        			VarLog_iq,	100.0f*Herz_base,    // 421      // 
    (void*)&reject1.Out,	       			VarLog_iq,	100.0f*Herz_base,    // 422      // 
    
    (void*)&GlobalM4.EncoderCalcParam.electricSpd,      VarLog_iq,	100.0f*Herz_base,    // 423      // Скорость с блока энкодераы
    
	// Регулятор потокосцепления
    ((void*)&AciPidRegPsir.pi_psir_Fdb),                VarLog_float,	100.0f,             // 424    // Input: Обратная связь регулятора потокосцепления
    ((void*)&AciPidRegPsir.WPsi),                       VarLog_float,	100.0f,             // 425    // Input: Частота поля
    ((void*)&AciPidRegPsir.Udcveryslow),                VarLog_float,	100.0f,             // 426    // Input: Текущее напряжение ЗПТ (фильтрованное)
    ((void*)&AciPidRegPsir.FWsecondZoneOnState),        VarLog_s16,	1.0f,               // 427    // Input: Флаг перехода во вторую зону
    ((void*)&AciPidRegPsir.FlagUsQmax),                 VarLog_s16,	1.0f,               // 428    // Input: Флаг наличия ограничения UsQ_MAXIM
    ((void*)&AciPidRegPsir.Idn),                        VarLog_float,	100.0f,             // 429    // Init: Номинальный ток возбуждения
    ((void*)&AciPidRegPsir.E_nom_new),                  VarLog_float,	100.0f,             // 430    // Init:
    ((void*)&AciPidRegPsir.Psir_nom_new),               VarLog_float,	100.0f,             // 431    // Init:
    ((void*)&AciPidRegPsir.fieldWeakening_Usmax),       VarLog_float,	100.0f,             // 432    // Init:
    ((void*)&AciPidRegPsir.fieldWeakening_invUsmax),    VarLog_float,	100.0f,             // 433    // Init:
    ((void*)&AciPidRegPsir.pi_psir_Kp),                 VarLog_float,	100.0f,             // 434    // Init:
    ((void*)&AciPidRegPsir.pi_psir_Ki),                 VarLog_float,	100.0f,             // 435    // Init:
    ((void*)&AciPidRegPsir.pi_psir_Ref),                VarLog_float,	100.0f,             // 436    // Var: Задание регулятора потокосцепления
    ((void*)&AciPidRegPsir.pi_psir_OutMax),             VarLog_float,	100.0f,             // 437    // Var: Максимальное ограничение выхода регулятора
    ((void*)&AciPidRegPsir.pi_psir_OutMin),             VarLog_float,	100.0f,             // 438    // Var: Минимальное ограничение выхода регулятора
    ((void*)&AciPidRegPsir.pi_psir_Ui),                 VarLog_float,	100.0f,             // 439    // Var: Интегральная составляющая регулятора
    ((void*)&AciPidRegPsir.pi_psir_Out),                VarLog_float,	100.0f,             // 440    
    
    (void*)&GlobalM4.EncoderCalcParam.electricTheta,    VarLog_float,	100.0f,             // 441      // Скорость с блока энкодераы    
    
    (void*)&ScalarDC_ORDINAR.Uscl,                      VarLog_float,	100.0f,             // 442      //    
    (void*)&ScalarDC_ORDINAR.Fscl,                      VarLog_float,	100.0f,             // 443      //
    (void*)&ScalarDC_ORDINAR.Uout,                      VarLog_float,	100.0f,             // 444      //
    
    (void*)&measLm2.Angle,                              VarLog_float,	100.0f,             // 445      //    
    (void*)&measLm2.Period,                             VarLog_s16,	100.0f,             // 446      //    
    (void*)&measLm2.UlineFlash,                         VarLog_float,	100.0f,             // 447      //    
    (void*)&measLm2.Lm,                                 VarLog_float,	100.0f,             // 448      //    
    (void*)&measLm2.Rs,                                 VarLog_float,	100.0f,             // 449      //        
    
    (void*)&Meas.Iu,					VarLog_float,	100.0f,  // 450
    (void*)&Meas.Iv,					VarLog_float,	100.0f,  // 451
    (void*)&Meas.Iw,					VarLog_float,	100.0f,  // 452
    (void*)&Meas.Iu_RMS,				VarLog_float,	100.0f,  // 453
    (void*)&Meas.Iv_RMS,	        		VarLog_float,	100.0f,  // 454
    (void*)&Meas.Iw_RMS,				VarLog_float,	100.0f,  // 455       
    
    (void*)&fe1.PsiR,				        VarLog_float,  	100.0f,  // 456       // Input: Flag Motor Generator mode
    (void*)&fe1.PsiErrProp,				VarLog_float,	100.0f,  // 457      // Variable: Rotating d-axis rotor flux (current model)
    (void*)&fe1.PsiErrInteg,			        VarLog_float,	100.0f,  // 458       // Output: Stationary d-axis estimated rotor flux 
    (void*)&fe1.dTheta,					VarLog_float,	100.0f,  // 459       // Output: Stationary q-axis estimated rotor flux 

    (void*)&Meas.ThetaI,        			VarLog_float,	10.0f,   // 460      
    
    (void*)&loadloss1.timer,        			VarLog_s16,	1.0f,    // 461
    (void*)&loadloss1.PwmOn,        			VarLog_s16,	1.0f,    // 462
    (void*)&loadloss1.cntFault,        			VarLog_s16,	1.0f,    // 463
    (void*)&loadloss1.Fault,        			VarLog_s16,	1.0f,    // 464
    (void*)&loadloss1.estimateTheta,        		VarLog_float,	100.0f,  // 465
    (void*)&loadloss1.prevTheta,        		VarLog_float,	100.0f,  // 466
    (void*)&loadloss1.Fout,        	        	VarLog_float,	100.0f,  // 467
    (void*)&loadloss1.dTheta,        	        	VarLog_float,	100.0f,  // 468
    (void*)&loadloss1.currentThetaI,   	        	VarLog_float,	100.0f,  // 469               
    
    (void*)&measRs1.Vin,   	        	        VarLog_float,	100.0f,  // 470               
    (void*)&measRs1.Iin,           	        	VarLog_float,	100.0f,  // 471               
    (void*)&measRs1.Rs,           	        	VarLog_float,	100.0f,  // 472                   
    (void*)&measRs1.K1,           	        	VarLog_float,	100.0f,  // 473                   
    (void*)&measRs1.Vin_f,           	        	VarLog_float,	100.0f,  // 474                   
    (void*)&measRs1.Iin_f,           	        	VarLog_float,	100.0f,  // 475                   
    (void*)&measRs1.Rs_tmp,           	        	VarLog_float,	100.0f,  // 476                       
        
    // DtcClarke				
    (void*)&DtcClarke.As,				VarLog_float,	100.0f,  // 477       // Input: phase-a stator variable
    (void*)&DtcClarke.Bs,	        		VarLog_float,	100.0f,  // 478       // Input: phase-b stator variable  
    (void*)&DtcClarke.Cs,	        		VarLog_float,	100.0f,  // 479       // Input: phase-b stator variable
    (void*)&DtcClarke.Alpha,				VarLog_float,	3000.0f, // 480       // Output: stationary d-axis stator variable  
    (void*)&DtcClarke.Beta,				VarLog_float,	3000.0f, // 481       // Output: stationary q-axis stator variable
    				                                                 
    // DtcIpark				                                         
    (void*)&DtcIpark.Alpha,				VarLog_float,	3000.0f, // 482       // Output: stationary d-axis stator variable
    (void*)&DtcIpark.Beta,				VarLog_float,	3000.0f, // 483       // Output: stationary q-axis stator variable  
    (void*)&DtcIpark.Angle,				VarLog_float,	100.0f,  // 484       // Input: rotating angle (pu)
    (void*)&DtcIpark.Ds,	        		VarLog_float,	1000.0f,  // 485       // Input: rotating d-axis stator variable  
    (void*)&DtcIpark.Qs,	        		VarLog_float,	1000.0f,  // 486       // Input: rotating q-axis stator variable
    
    // DtcIclarke		
    (void*)&DtcIclarke.Alpha,  		         	VarLog_float, 	100.0f,  // 487    // Output: stationary d-axis stator variable 
    (void*)&DtcIclarke.Beta,  		         	VarLog_float, 	100.0f,  // 488    // Output: stationary q-axis stator variable
    (void*)&DtcIclarke.As,  		         	VarLog_float, 	100.0f,  // 489    // Input: phase-a stator variable
    (void*)&DtcIclarke.Bs,  		         	VarLog_float, 	100.0f,  // 490    // Input: phase-b stator variable 
    (void*)&DtcIclarke.Cs,  		         	VarLog_float, 	100.0f,  // 491    // Input: phase-c stator variable 

    // DtcSTFilter		
    (void*)&DtcSTFilter.AlphaIn,  		        VarLog_float, 	100.0f,  // 492    // Output: stationary d-axis stator variable 
    (void*)&DtcSTFilter.BetaIn,  		        VarLog_float, 	100.0f,  // 493    // Output: stationary d-axis stator variable 
    (void*)&DtcSTFilter.We,  		                VarLog_float, 	100.0f,  // 494    // Output: stationary d-axis stator variable 
    (void*)&DtcSTFilter.AlphaOut,  		        VarLog_float, 	100.0f,  // 495    // Output: stationary d-axis stator variable 
    (void*)&DtcSTFilter.BetaOut,  		        VarLog_float, 	100.0f,  // 496    // Output: stationary d-axis stator variable 
    
     // minCurrCtrl
    (void*)&minCurrCtrl.bitStart,  		        VarLog_s16, 	1.0f,    // 497  
    (void*)&minCurrCtrl.inId,  		                VarLog_iq,	10.0f*Current_base,  // 498
    (void*)&minCurrCtrl.inIq,  		                VarLog_iq,	10.0f*Current_base,  // 499
    (void*)&minCurrCtrl.minI,  		                VarLog_iq,	10.0f*Current_base,  // 500
    (void*)&minCurrCtrl.outId,  		        VarLog_iq,	10.0f*Current_base,  // 501
    (void*)&minCurrCtrl.outIq,  		        VarLog_iq,	10.0f*Current_base,  // 502
    
    // pmsmTORQcomp
    (void*)&pmsmTORQcomp.inId,  		        VarLog_iq,	10.0f*Current_base,  // 503
    (void*)&pmsmTORQcomp.inIq,  		        VarLog_iq,	10.0f*Current_base,  // 504
    (void*)&pmsmTORQcomp.currD,  		        VarLog_iq,	10.0f*Current_base,  // 505    
    (void*)&pmsmTORQcomp.outId,  		        VarLog_iq,	10.0f*Current_base,  // 506
    (void*)&pmsmTORQcomp.outIq,  		        VarLog_iq,	10.0f*Current_base,  // 507
       
    // soft_cur2
    (void*)&soft_cur2.IdRef_in,  		        VarLog_iq,	10.0f*Current_base,  // 508
    (void*)&soft_cur2.IqRef_in,  		        VarLog_iq,	10.0f*Current_base,  // 509
    (void*)&soft_cur2.IdRef_out,  		        VarLog_iq,	10.0f*Current_base,  // 510    
    (void*)&soft_cur2.IqRef_out,  		        VarLog_iq,	10.0f*Current_base,  // 511    
    
    // pmsm_cross1
    (void*)&pmsm_cross1.Isd,  		                VarLog_iq,	10.0f*Current_base,  // 512
    (void*)&pmsm_cross1.Isq,  		                VarLog_iq,	10.0f*Current_base,  // 513
    (void*)&pmsm_cross1.Ucd,  		                VarLog_float,	100.0f,  // 514    
    (void*)&pmsm_cross1.Ucq,  		                VarLog_float,	100.0f,  // 515    
    (void*)&pmsm_cross1.Urd,  		                VarLog_float,	100.0f,  // 516    
    (void*)&pmsm_cross1.Urq,  		                VarLog_float,	100.0f,  // 517    
    (void*)&pmsm_cross1.Us,  		                VarLog_float,	100.0f,  // 518    
    (void*)&pmsm_cross1.Usd,  		                VarLog_float,	100.0f,  // 519    
    (void*)&pmsm_cross1.Usq,  		                VarLog_float,	100.0f,  // 520        
    (void*)&pmsm_cross1.Usmax,  		        VarLog_float,	100.0f,  // 521    
    (void*)&pmsm_cross1.UsmaxFlag,  		        VarLog_float,	100.0f,  // 522    
    (void*)&pmsm_cross1.Wr,  		                VarLog_iq,	10.0f*Herz_base,  // 523    
       
    (void*)&GlobalM4.Rsignal,                           VarLog_s16,	1.0f,  // 524    
    
    //  pmsmAntirollBack
    (void*)&pmsmAntirollBack.inId,  		        VarLog_iq,	10.0f*Current_base,  // 525    
    (void*)&pmsmAntirollBack.inIq,  		        VarLog_iq,	10.0f*Current_base,  // 526    
    (void*)&pmsmAntirollBack.fixId,  		        VarLog_iq,	10.0f*Current_base,  // 527    
    (void*)&pmsmAntirollBack.inSpeedWr,  		VarLog_iq,	10.0f*Herz_base,     // 528    
    (void*)&pmsmAntirollBack.inTheta,  		        VarLog_float,	100.0f,              // 529
    (void*)&pmsmAntirollBack.inSpeedWrz,  		VarLog_iq,	10.0f*Herz_base,     // 530            
    (void*)&pmsmAntirollBack.bitPhasing,  		VarLog_s16,	1.0f,                // 531
    (void*)&pmsmAntirollBack.outId,  		        VarLog_iq,	10.0f*Current_base,  // 532    
    (void*)&pmsmAntirollBack.outIq,  		        VarLog_iq,	10.0f*Current_base,  // 533    
    (void*)&pmsmAntirollBack.outSpeedWr,  		VarLog_iq,	10.0f*Herz_base,     // 534    
    (void*)&pmsmAntirollBack.outTheta,  		VarLog_float,	100.0f,              // 535
    (void*)&pmsmAntirollBack.onStartWork,  		VarLog_s16,	1.0f,                // 536
    (void*)&pmsmAntirollBack.pwmOn,  		        VarLog_s16,	1.0f,                // 537        
    (void*)&pmsmAntirollBack.BreakOff,  		VarLog_s16,	1.0f,                // 538        
    (void*)&pmsmAntirollBack.Mode,  		        VarLog_s16,	1.0f,                // 539        
    (void*)&pmsmAntirollBack.fixIdDone,  		VarLog_s16,	1.0f,                // 540        
    (void*)&pmsmAntirollBack.fixIdDoneCnt,  		VarLog_s16,	1.0f,                // 541        
    (void*)&pmsmAntirollBack.maxIfull,  		VarLog_iq,	10.0f*Current_base,  // 542        
    (void*)&pmsmAntirollBack.Imag,  		        VarLog_iq,	10.0f*Current_base,  // 543            
    (void*)&pmsmAntirollBack.timeKoef,  		VarLog_float,	1000.0f,             // 544             
    (void*)&pmsmAntirollBack.Enable,     		VarLog_s16,	1.0f,                // 545                 
    
    (void*)&GlobalM4.Pretorque.mPretorqueReset,     	VarLog_s16,	1.0f,                // 546
    (void*)&GlobalM4.Pretorque.mPretorqueSet,     	VarLog_s16,	1.0f,                // 547
    (void*)&GlobalM4.Pretorque.Enable,               	VarLog_s16,	1.0f,                // 548
    (void*)&GlobalM4.Pretorque.mPretorqueTime,     	VarLog_s16,	1.0f,                // 549    
    (void*)&GlobalM4.Pretorque.mPretorqueCatch,       	VarLog_s16,	1.0f,                // 550        

    (void*)&DrvConst.Fpwm,     		                VarLog_float,	1.0f,                // 551
       
};

//==========================================================
//==========================================================

// 
void LogN9(void)
{
    static  s16             *dataPnt    = &((logFile_type*)ExtRAMmem)->data[0];
    u16             	    *pntP       = &GlobalM4.logSetup.dataPnt.n[0];
    logSetup_type           *pntLogSetup;

    pntLogSetup = &GlobalM4.logSetup;

    // Маркер сброса текущего лога - обнулить индексы и начать с начала.
    if (pntLogSetup->ResetLogInd == 1) {
      LogInd = 0;
      dataPnt = &logFile->data[0];
      pntLogSetup->ResetLogInd = 0;
    }
    
    if(pntLogSetup->TypeLog == typeLog_PwmOn || pntLogSetup->TypeLog == typeLog_startPwmOn){
        if( /*!PwmIsOn() &&*/ /*!GlobalM4.Debug6*/ !Pwm.PwmOn){ // !*!*!*! Добавил контрольную проверку что ШИМ включен на нижнем уровне
            if( pntLogSetup->logEndTmr == 0 ){
                LogInd = 0;
                dataPnt = &logFile->data[0];
                PrescalLog1 = pntLogSetup->cmpPrescalLog1;
                return;
            }
        }
    }

    if(pntLogSetup->TypeLog == typeLog_startCmd){ 
      if( !GlobalM4.FlagCommand.bit.StartCmd ){ // Если команда старт не пришла, обнуляем указатель логера
            if( pntLogSetup->logEndTmr == 0 ){
                LogInd = 0;
                dataPnt = &logFile->data[0];
                PrescalLog1 = pntLogSetup->cmpPrescalLog1;
                return;
            }
        }
    }
    
    
    if( pntLogSetup->TypeLog == typeLog_NoFail ){
        if(GlobalM4.FailData.NumFail != 0){
            if( pntLogSetup->logEndTmr == 0 ){
                LogInd = 0;
                dataPnt = &logFile->data[0];
                PrescalLog1 = pntLogSetup->cmpPrescalLog1;
                return;
            }
        }
    }

    if(GlobalM4.debLockWriteLog == 0){
        if(PrescalLog1 > 0) PrescalLog1--;      // Прореживалка (тики логгера)
        else{
            PrescalLog1 = pntLogSetup->cmpPrescalLog1;
            if(LogInd >= GlobalM4.logSetup.LogLen ){
                if(pntLogSetup->TypeLog == typeLog_startPwmOn || pntLogSetup->TypeLog == typeLog_start || pntLogSetup->TypeLog == typeLog_startCmd){
                    return;
                }
                LogInd = 0;
                dataPnt = &logFile->data[0];
            }

            switch( pntLogSetup->NlogVar ){
                case 9:     *(dataPnt + 8)  = debGetVal(pntP[8]);
                case 8:     *(dataPnt + 7)  = debGetVal(pntP[7]);
                case 7:     *(dataPnt + 6)  = debGetVal(pntP[6]);
                case 6:     *(dataPnt + 5)  = debGetVal(pntP[5]);
                case 5:     *(dataPnt + 4)  = debGetVal(pntP[4]);
                case 4:     *(dataPnt + 3)  = debGetVal(pntP[3]);
                case 3:     *(dataPnt + 2)  = debGetVal(pntP[2]);
                case 2:     *(dataPnt + 1)  = debGetVal(pntP[1]);
                case 1:     *(dataPnt + 0)  = debGetVal(pntP[0]);
                default:    break;
            }

            dataPnt = dataPnt + pntLogSetup->NlogVar;
            if ((dataPnt - &logFile->data[0]) > szExtRAMmemData*2) {
              LogInd ++;
              LogInd --;              
            }
            LogInd++;
            logFile->hof.LogInd = LogInd;
        }
    }
}

//==========================================================
void log4msFunc(void){
    if(GlobalM4.debLockWriteLogReq == 1){
    	GlobalM4.debLockWriteLog     = debTimeLockWriteLog;
        GlobalM4.debLockWriteLogReq  = 0;
    }
    else{
        if( GlobalM4.debLockWriteLog != 0 ){
        	GlobalM4.debLockWriteLog--;
        }
    }
}

void setupLogFileHOF( void * dst)
{
    headOfLogFile_type *hof;

    hof = dst;
    hof->Nvar           = GlobalM4.logSetup.HOFlogFile.Nvar;
    hof->Sz             = GlobalM4.logSetup.HOFlogFile.Sz;
    hof->TimeOneTick    = GlobalM4.logSetup.HOFlogFile.TimeOneTick;
    hof->Ntic           = GlobalM4.logSetup.HOFlogFile.Ntic;
}

// Получить значение переменной
inline s16 debGetVal(u16 NumVar){
  const LogVarTable_type    *LogVar;
  s16                       Val, Base;
  float                     ValFloat, Scale, Temp;
  
  static s16 numberVariables = 0;          
  if (numberVariables == 0)
      numberVariables = sizeof(debPntVar) / sizeof(LogVarTable_type);     // разчет количества переменных
  
  if (numberVariables <= NumVar)                                           // если указан номер переменной больше возможно количе
      NumVar = 0;
  // 1) Получить указатель на описание переменной
  LogVar = &debPntVar[NumVar];
  
  // 2) Разберемся с типом переменной
  switch(LogVar->VarLogType){
    case VarLog_s16: {
        Val = *((s16*)LogVar->AdresVar);
    } break;
        
    case VarLog_s32: {
        Val = *((s32*)LogVar->AdresVar + 1);       // Откидываем младшую часть
    } break;
    
    case VarLog_float: {
        ValFloat = *((float*)LogVar->AdresVar);    // Текущее значение переменной
        Val = (s16)((ValFloat * LogVar->Scale) + 0.5f);     // Перевод в s16
    } break;

    case VarLog_iq: {
        Temp = LogVar->Scale;
        Scale = 1;
        while (1){
          if (Temp < 10) {
            Base = (u16)Temp;
            break;
          } else {
            Scale *= 10.0f; // Увеличиваем множитель результата
            Temp *= 0.1f;   // Уменьшаем входной параметр на 10.
          }          
        }      
        ValFloat = *((float*)LogVar->AdresVar);    // Текущее значение переменной        
        Val = (s16)((IQ_to_FLOAT(ValFloat, (NumBase_type)Base) * Scale) + 0.5f);     // Перевод в s16
    } break;
    
    
    
    default:{
      Val = 0;
    }break;
  }
  
  return Val; 
}



#endif // LOGER_GRAPH_IS_PRESENT
