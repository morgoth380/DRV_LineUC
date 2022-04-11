/* ==============================================================================
 System Name:    ACI_VECTOR_CONTROL (IQ version)

 File Name:      ACI_VECTOR_CONTROL.C

 Description:    Primary system header file for Sensored Indirect and Sensorless
 Field Orientation Control for a Three Phase AC Induction Motor.
 
 Developer:      TRIOL
 
 Originator:     Triol (based on Texas Instruments library)

 =====================================================================================
 History:
 -------------------------------------------------------------------------------------
 22-12-2011 Version 1.00: Complete Sensored and Sensorless FOC for AC Induction Motor 
 with autotuning regulator
 =================================================================================  */


#include "Global_Include.h"
#include "Filter.h"
#include "LIB_DMC.h"
#include "ACI_Vector_Control.h"
#include "PMSM_Vector_Control.h"
#include "ACI_prm_mot.h"
#include "PMSM_prm_mot.h"
#include "FOC_params_copy.h"
#include "ScalarControl.h"
#include "Pjk_const.h"
#include "DrvPWM.h"
#include "CommonMicro.h"   
#include "aci_fe.h"           // Include header for the ACIFE object
#include "aci_fe_const.h"     // Include header for the ACIFE_CONST object
#include "aci_se.h"           // Include header for the ACISE object
#include "aci_se_const.h"     // Include header for the ACISE_CONST object
#include "aci_pi_reg_spd_const.h"
#include "aci_pi_reg_emf.h" 
#include "aci_pi_reg_cur_const.h"
#include "aci_pi_reg_emf_const.h"
#include "aci_cross_rel_comp.h"
#include "aci_cross_rel_comp_const.h"
#include "aci_speed_lim_const.h"
#include "anti_rollback.h"          // Антиоткатная функция
#include "bandstop_filter.h"

   
typedef struct {
	u16 SpdCalc :1;
} ACIFLAG;

ACIFLAG aci_flag;
u16 IsrTicker = 0;

// Instance rotor flux and speed estimations
ACIFE fe1 = ACIFE_DEFAULTS;
ACISE se1 = ACISE_DEFAULTS;

// Instance the constant calculations for rotor flux and speed estimations
ACIFE_CONST fe1_const = ACIFE_CONST_DEFAULTS;
ACISE_CONST se1_const = ACISE_CONST_DEFAULTS;

PIREGSPDCONST pi_spd_const1 = PIREGSPDCONST_DEFAULTS; // регулятора скорости  aci_pi_reg_spd_const

// Instance PID regulators to regulate the d and q synchronous axis currents,
// and speed
PIREGEMF pi_emf = PIREGEMF_DEFAULTS;

// Обьявление вычислителей коэффициентов 
PIREGCURCONST pi_is_const1 = PIREGCURCONST_DEFAULTS; //регулятора тока
PIREGEMFCONST pi_emf_const1 = PIREGEMFCONST_DEFAULTS; // регулятора ослабления поля

// Обьявление вычислителя коэффициентов ограничителя скорости
SPEEDLIMCONST spdlimconst1 = SPEEDLIMCONST_DEFAULTS;

// Обьявление обьекта компесации перекрестных связей
CROSSRELCOMP cross1 = CROSSRELCOMP_DEFAULTS;

// Обьявление вычислителя констант, компесации перекрестных связей
CROSSRELCOMPCONST cross_const1 = CROSSRELCOMPCONST_DEFAULTS;

// Обект расчета скалярного управления
SCALARCONTROL Scalar = SCALARCONTROL_DEFAULTS;              



#define UsQ_MAXIM  1
#define UsD_MAXIM  2

//float  UiStorage[32] ={0};
//float  AccStorage[64] ={0}; // Буфер для расчета ускорения

//float  UiStorage2[32] ={0};
//float  AccStorage2[64] ={0}; // Буфер для расчета ускорения


#define SPEED_FCUT 100.0f     // частота среза фильтра скорости


extern PWMGEN Pwm;                                          // Обект модуля ШИМ 
extern ANTIROLLBACK    arollback1;

//void ACIM_Vector_Init(void);

//------------------------------------------------------------------------//
//              ОБРАБОТЧИК ПЕРЕСЧЕТА ПАРАМЕТРОВ ВУ и ИП                   //
//------------------------------------------------------------------------//
void FOC_ReCalc_func()
{
    // копирование параметров ВУ из глобальной обменной структуры
    FOC_Params_Copy_from_M4(); 
    
    DrvWords.Ctrl.bit.SysCalc = 1;
    
    ACIM_Vector_Init();               // Функции инициализации ВУ и ИП, для асинхронного двигателя
    ACIM_PrmMot_Init();               // Функция инициализации подсистемы измеsрения параметров
    
    // Функции инициализации ВУ и ИП, для вентильного двигателя
    PMSM_Vector_Init();
    PMSM_PrmMot_Init();

    //ENCOEMU_init();
    DrvWords.Ctrl.bit.SysCalc = 0;
 
}

void ACIM_Vector_Init() {

        f32 fL, dtc1_STF_fc; 
	f32 NumTdly = 5.0f; 
	
	if(!DrvWords.Ctrl.bit.SysCalc){
		return;
        }
	if(!(DrvConst.DriveType == TYPE_SCALAR_NO_DEADBAND || DrvConst.DriveType == TYPE_SCALAR || DrvConst.DriveType == TYPE_ACIM_FOC_SENSORED || DrvConst.DriveType == TYPE_ACIM_FOC_SENSORLESS))
		return;

        // Проверка задержек
        if (dPnt->Fpwm <= 10000.0f) NumTdly = 8;
        if (dPnt->Fpwm <= 7500.0f)  NumTdly = 7;
        if (dPnt->Fpwm <= 6500.0f)  NumTdly = 6;
        if (dPnt->Fpwm <= 5500.0f)  NumTdly = 5;
        if (dPnt->Fpwm <= 3500.0f)  NumTdly = 5;        

        //-----------------------------------------  
        if (dPnt->FcutSpd == 0)
          dPnt->FcutSpd = 100;
          
        dPnt->TcutSpd = 1.0f/(2*PI*dPnt->FcutSpd);
        //-----------------------------------------
//  Initialize
	DrvWords.Ctrl.bit.vCodeEnb = 1;
	DriveInit(dPnt);

// Initialize the ACI constant module
	fe1_const.Rs = dPnt->Rs;
	fe1_const.Rr = dPnt->Rr;
	fe1_const.Ls = dPnt->Ls;
	fe1_const.Lr = dPnt->Lr;
	fe1_const.Lm = dPnt->Lm;
	fe1_const.Ib = dPnt->CurrBASE;
	fe1_const.Vb = dPnt->VoltBASE;
	fe1_const.Ts = dPnt->Tpwm;
	fe1_const.calc(&fe1_const);

// Initialize the ACI module
	fe1.K1 = fe1_const.K1;
	fe1.K2 = fe1_const.K2;
	fe1.K3 = fe1_const.K3;
	fe1.K4 = fe1_const.K4;
	fe1.K5 = fe1_const.K5;
	fe1.K6 = fe1_const.K6;
	fe1.K7 = fe1_const.K7;
	fe1.K8 = fe1_const.K8;
	fe1.Ki = 0.25f * fe1_const.Tr; // Фиксированное значение интеграла

// Initialize the ACI constant module
	se1_const.Rr = dPnt->Rr;
	se1_const.Lr = dPnt->Lr;
	se1_const.fb = dPnt->FreqBASE;
	se1_const.fc = SPEED_FCUT;           //частота среза фильтра 100 Hz, Гц
        se1_const.TcutSpd = dPnt->TcutSpd;   // Постоянная среза контура скорости из системы параметров - переменное значение
	se1_const.Ts = dPnt->Tpwm;
	se1_const.calc(&se1_const);

// Initialize the ACI module 
	se1.K1 = se1_const.K1;
	se1.K2 = se1_const.K2;
	se1.K3 = se1_const.K3;
	se1.K4 = se1_const.K4;
	se1.K5 = se1_const.K5;
	se1.K8 = se1_const.K8;
	se1.K9 = se1_const.K9;
        
	fL = dPnt->Lm / dPnt->Lr; 
	fL = fL * (float)GlobalM4.EnginParam.Zp * 1.5f; 
	se1.K6 = fL;                                // 3/2ZpLm/Lr  Суходольский 21.03.2018
	fL = 0.666f * dPnt->Rr * dPnt->CurrBASE;    // Voltage base
	fL = fL /dPnt->VoltBASE;
	fL = fL / (float)GlobalM4.EnginParam.Zp;
	se1.K7 = fL;                                // 2/3Zp*Rr  Суходольский 21.03.2018

	se1.MaxWrHat = 400.0f / dPnt->FreqBASE;
	se1.MinWrHat = -400.0f / dPnt->FreqBASE;
	se1.BaseRpm = (u32)(60.0f * dPnt->FreqBASE / (float)dPnt->Zp);

        // Initialize the Dead time compensation
	// Блок STFilter для блока dtc
	DtcSTFilter.reset(&DtcSTFilter);

	dtc1_STF_fc    = 50.0f; // Гц
	DtcSTFilter.K1 = (dPnt->FreqBASE * 0.5f * dPnt->Tpwm);           // Коеффициент фильтра 1
	DtcSTFilter.K2 = (_2PI * dtc1_STF_fc * dPnt->Tpwm);            // Коеффициент фильтра 2
	DtcSTFilter.K3 = (_2PI * dPnt->FreqBASE * dPnt->Tpwm);         // Коеффициент фильтра 3

    // Initialize the Dead time compensation
	dtc1.reset(&dtc1);

	dtc1.dT = dPnt->DeadTimeInit.DeadTime * dPnt->Fpwm * 0.000001f;                // МВ в относительных единицах
	dtc1.Rsw_2 = 2.0f * dPnt->DeadTimeInit.Rsw * dPnt->CurrBASE / dPnt->VoltBASE;  // двойное сопротивление ключа
	dtc1.dI = FLOAT_to_IQ(dPnt->DeadTimeInit.dI, Current_base);                    // интервал неопределённости около 0 А
	dtc1.Udf = FLOAT_to_IQ(dPnt->DeadTimeInit.Udf, Volt_base);
        dtc1.Rsw2Const = 2.0f * 0.03f * dPnt->CurrBASE / dPnt->VoltBASE;        
	dtc1.UdfConst = 1.0f/dPnt->VoltBASE;
	dtc1.Utf   = dPnt->DeadTimeInit.Utf / dPnt->VoltBASE;                       // прямое падение напр. на IGBT, В
	dtc1.DtcRelative = ((dPnt->DeadTimeInit.DeadTime + dPnt->DeadTimeInit.Ton - dPnt->DeadTimeInit.Toff) * dPnt->Fpwm * 1e-6f);
	// а) 1.0 - задержка задания напр.
	// б) 1.0 - задержка измерения тока
	// в) 0.5 - опережение STF фильтра
        // в скобках добавить +1 и понаблюдать. Изменить dI (0.07-0.5)
	dtc1.ThetaCorrCoeff = (_2PI * dPnt->FreqBASE * (1.0f + 1.0f + 1.0f - 0.5f) * dPnt->Tpwm); // корректор фазы тока
	dtc1.dtcMode = dPnt->DeadTimeInit.ModeDtc;
        
        
// Initialize the CROSSRELCOMP constant module 
	cross_const1.Lm = dPnt->Lm;
	cross_const1.Lr = dPnt->Lr;
	cross_const1.sigmaLs = dPnt->sigma * dPnt->Ls;
	cross_const1.Rr = dPnt->Rr;
	cross_const1.Rs = dPnt->Rs;
	cross_const1.fb = dPnt->FreqBASE;
	cross_const1.Ib = dPnt->CurrBASE;
	cross_const1.Ub = dPnt->VoltBASE;
	cross_const1.calc(&cross_const1);

// Initialize the PI constant
	pi_is_const1.Rs = dPnt->Rs;
	pi_is_const1.Rr = dPnt->Rr;
	pi_is_const1.Lls = dPnt->Lls;
	pi_is_const1.Lr = dPnt->Lr;
	pi_is_const1.Lm = dPnt->Lm;
	pi_is_const1.Tdrv = 0;
	pi_is_const1.Tpwm = dPnt->Tpwm;
	pi_is_const1.Ub = dPnt->VoltBASE;
	pi_is_const1.Ib = dPnt->CurrBASE;
        pi_is_const1.NumTdly = NumTdly;
	pi_is_const1.ManualEnb = DrvWords.Ctrl.bit.ManualCurReg;
	pi_is_const1.Kp_manual = dPnt->KpCur;
	pi_is_const1.Ki_manual = dPnt->KiCur;
  	pi_is_const1.calc(&pi_is_const1);

	dPnt->KBase = 4.1887902047864f * dPnt->Lm * dPnt->CurrBASE * dPnt->FreqBASE / (dPnt->VoltBASE * dPnt->Zp);      //K          //4.1887902047864=4pi/3

       	// Initialize the CROSSRELCOMPCONST module
	cross1.K1 = cross_const1.K1;
	cross1.K2 = cross_const1.K2;
	cross1.K3 = cross_const1.K3;
	cross1.K4 = cross_const1.K4;

// Initialize the PI constant
	pi_spd_const1.Jr = dPnt->Jr;
	pi_spd_const1.Lr = dPnt->Lr;
	pi_spd_const1.Lm = dPnt->Lm;
	pi_spd_const1.Zp = dPnt->Zp;
	pi_spd_const1.PrNom = dPnt->PhiR_nom;

	pi_spd_const1.Tcl = pi_is_const1.Tcl;
	pi_spd_const1.Tsl = 2.0f * (pi_is_const1.Tcl + se1_const.Tc + dPnt->TcutSpd); // 6.0 * pi_is_const1.Tcl; //
	pi_spd_const1.Tspd = dPnt->Tpwm * dPnt->DivRegSpd;
	pi_spd_const1.Imax = dPnt->Imax_f32; // Действующее значение максимального тока
	pi_spd_const1.Fc_spd = SPEED_FCUT; // Гц (частота среза фильтра скорости)
	pi_spd_const1.fb = dPnt->FreqBASE;
	pi_spd_const1.Ib = dPnt->CurrBASE;
	pi_spd_const1.ManualEnb = DrvWords.Ctrl.bit.ManualSpdReg;
	pi_spd_const1.Kp_manual = dPnt->KpSpd;
	pi_spd_const1.Ki_manual = dPnt->KiSpd;
	pi_spd_const1.Kp_forced = dPnt->KpForced;
	pi_spd_const1.Ki_forced = dPnt->KpForced * pi_spd_const1.Tspd / dPnt->KiForced;
	pi_spd_const1.calc(&pi_spd_const1);

	dPnt->KpSpd = pi_spd_const1.Kp_manual;
	dPnt->KiSpd = pi_spd_const1.Ki_manual;
	dPnt->KpCur = pi_is_const1.Kp_manual;
	dPnt->KiCur = pi_is_const1.Ki_manual;        
	GlobalM4.Tspd = pi_spd_const1.Tspd;
        dPnt->Tsl = pi_spd_const1.Tsl;
          
// Initialize the PI constant
	pi_emf_const1.Rr = dPnt->Rr;
	pi_emf_const1.Lr = dPnt->Lr;
	pi_emf_const1.Lm = dPnt->Lm;
	pi_emf_const1.Wnom = dPnt->Wnom_Hz;
	pi_emf_const1.Wmax = dPnt->Wmax_Hz;
	pi_emf_const1.Tcl = pi_is_const1.Tcl;
	pi_emf_const1.Temfl = 2.0f * pi_is_const1.Tcl + 6.0f * dPnt->Tpwm; // 2.0*v->NumTdly*v->Tpwm - реальное выражение
	pi_emf_const1.Tpwm = dPnt->Tpwm;
	pi_emf_const1.Idn = dPnt->Idn;
	pi_emf_const1.Ub = dPnt->VoltBASE;
	pi_emf_const1.Ib = dPnt->CurrBASE;
	pi_emf_const1.Fb = dPnt->FreqBASE;
	pi_emf_const1.NumTdly = NumTdly;          
	pi_emf_const1.Wnom_Hz = dPnt->Wnom_Hz;
	pi_emf_const1.calc(&pi_emf_const1);


// Initialize the SPEEDLIM constant
	spdlimconst1.Lm = dPnt->Lm;
	spdlimconst1.Lr = dPnt->Lr;
	spdlimconst1.Zp = dPnt->Zp;
	spdlimconst1.WrL = dPnt->WrL;
	spdlimconst1.WrH = dPnt->WrH;
	spdlimconst1.Tb = dPnt->TorqBASE;
	spdlimconst1.Ib = dPnt->CurrBASE;
	spdlimconst1.Pb = dPnt->PowerBASE;
	spdlimconst1.fb = dPnt->FreqBASE;
	spdlimconst1.calc(&spdlimconst1);

// Initialize the SPEEDLIM
	spdlim1.WrL = dPnt->WrL;
	spdlim1.WrH = dPnt->WrH;
	spdlim1.K1 = spdlimconst1.K1;
	spdlim1.K2 = spdlimconst1.K2;
	spdlim1.OutMax = pi_spd_const1.OutMax;

	dPnt->UmaxIqId = 1.0f;  

// Initialize the PI module for Id
  	pi_isd.Kp = pi_is_const1.Kp;
 	pi_isd.Ki = pi_is_const1.Ki;
	pi_isd.K1 = pi_is_const1.K1;
	pi_isd.K2 = pi_is_const1.K2;
	pi_isd.K3 = pi_is_const1.K3;
	pi_isd.OutMax = dPnt->UmaxIqId;
	pi_isd.OutMin = -pi_isd.OutMax;
 // Initialize the PI module for Iq
  	pi_isq.Kp = pi_is_const1.Kp;
 	pi_isq.Ki = pi_is_const1.Ki;
	pi_isq.K1 = pi_is_const1.K1;
	pi_isq.K2 = pi_is_const1.K2;
	pi_isq.K3 = pi_is_const1.K3;
	pi_isq.OutMax = dPnt->UmaxIqId;
	pi_isq.OutMin = -pi_isq.OutMax;

	pi_spd.Kp = pi_spd.Kp_norm      = pi_spd_const1.Kp;
	pi_spd.Ki = pi_spd.Ki_norm      = pi_spd_const1.Ki;
	pi_spd.Kp_forced                = pi_spd_const1.Kp_forced;
	pi_spd.Ki_forced                = pi_spd_const1.Ki_forced;        
	pi_spd.K1 = pi_spd_const1.K1;
	pi_spd.K2 = pi_spd_const1.K2;
	pi_spd.K3 = pi_spd_const1.K3;
	pi_spd.K4 = pi_spd_const1.K4;
	pi_spd.K5 = pi_spd_const1.K5;
	pi_spd.K6 = pi_spd_const1.K6;
	pi_spd.K7 = pi_spd_const1.K7;
 	pi_spd.OutMax = pi_spd_const1.OutMax;
	pi_spd.OutMin = -pi_spd.OutMax;
	//pi_spd.Imax_Ampl   = pi_spd_const1.OutMax;
	pi_spd.Tpwm      = dPnt->Tpwm;
	pi_spd.RefFiltrEnb = DrvWords.Ctrl.bit.FiltrRefSpdEnb;
	pi_spd.NewRegEnb   = 1; 
//	pi_spd.UiStorage   = UiStorage;
//	pi_spd.storageLen  = 32;
//	pi_spd.storagePos  = 0;
//
//	pi_spd.AccStorage 	= AccStorage;
//	pi_spd.AccStorageLen = 2;/*GlobalM4.EncoderParam.EncoFlt_N;*/
//	pi_spd.AccStoragePos = 0;
//	pi_spd.K_Acc = (float)pi_spd.AccStorageLen * dPnt->Tpwm;

// Initialize the PI module for field weakening
	pi_emf.Kp = pi_emf_const1.Kp;
	pi_emf.Ki = pi_emf_const1.Ki;
	pi_emf.K1 = pi_emf_const1.K1;
	pi_emf.K2 = pi_emf_const1.K2;
	pi_emf.K3 = pi_emf_const1.K3;
	pi_emf.OutMax = pi_emf_const1.OutMax;       // Увеличиваем в два раза предел ограничения
	pi_emf.OutMin = pi_emf_const1.OutMin;
	pi_emf.InitInteg = pi_emf_const1.InitInteg;
        
	// Регулятор потокосцепления
        f32     E_nom_new;
        
	AciPidRegPsir.reset(&AciPidRegPsir);   // Сбросить блок
	AciPidRegPsir.Idn = (dPnt->Idn);
	AciPidRegPsir.pi_psir_OutMax = (dPnt->Idn);
	E_nom_new = (dPnt->Usn * dPnt->VoltBASE * sqrt(2.0f) / sqrt(3.0f)  - sqrt(2.0f) * dPnt->Rs * dPnt->Isn * dPnt->CurrBASE)  * dPnt->Lm /  (dPnt->Lm  + dPnt->Lls) * 1.0f / (dPnt->CurrBASE * dPnt->Lm * 2.0f * PI * dPnt->FreqBASE);
	AciPidRegPsir.E_nom_new = E_nom_new;
	AciPidRegPsir.Psir_nom_new = E_nom_new / dPnt->Fnom; //E_nom_new / (2.0f * PI * dPnt->Fnom * dPnt->FreqBASE / (2.0f * PI * dPnt->FreqBASE));      // Psir_nom_new = E_nom_new / (2*pi*dPnt_Fnom*dPnt_FreqBASE/(2*pi*dPnt_FreqBASE));        
	// порог активации ослабления поля при понижении напряженяи ЗПТ
	if(dPnt->Usn < 1.0f) {
          // Если номинальное напряение двигателя меньше чем номинальное напряжение ПЧ
          AciPidRegPsir.fieldWeakening_Usmax = dPnt->Usn * sqrt(2.0f);
	} else {
          AciPidRegPsir.fieldWeakening_Usmax = sqrt(2.0f);
	}

	AciPidRegPsir.K1filtVolWeak = dPnt->TfiltVolWeak/ (dPnt->TfiltVolWeak + dPnt->Tpwm);
	AciPidRegPsir.K2filtVolWeak = dPnt->Tpwm / (dPnt->TfiltVolWeak + dPnt->Tpwm);
	AciPidRegPsir.pi_psir_Kp = pi_emf_const1.Kp * (dPnt->CurrBASE * dPnt->FreqBASE) / dPnt->VoltBASE * dPnt->Lm;
	AciPidRegPsir.pi_psir_Ki = pi_emf_const1.Ki * (dPnt->CurrBASE * dPnt->FreqBASE) / dPnt->VoltBASE * dPnt->Lm;
        
	// Фильтр ЗПТ
	UdcFiltr.reset(&UdcFiltr);
	UdcFiltr.K1udcFilt = dPnt->TudcFilt / (dPnt->TudcFilt + dPnt->Tpwm);
	UdcFiltr.K2udcFilt = dPnt->Tpwm / (dPnt->TudcFilt + dPnt->Tpwm);        
}

#define K_MOD_SPACEPWM       1.4142135623730950488016887242097

typedef enum {
    InitStartPU	= 0,	
    WaitPhirUp,
    StabilizedPhir,
    SearchFreqPU,		
    SetNormalPhirPU,		
} PickUpState_enum;


///*********************************************************************************************///
///                       Торможение постоянным током                                           ///
///*********************************************************************************************///
void DC_Brake_Calc(u16 DCTormSync, f32 dAngle, f32 DC_Current) {
            float       Ds;
            
            if (!DCTormSync) {
                pi_isq.Ui = ipark1.Ds;
            }
          
// ------------------------------------------------------------------------------
//    Connect inputs of the CLARKE module and call the clarke transformation
//    calculation function.
// ------------------------------------------------------------------------------
            clarke1.As = dPnt->IsU;
            clarke1.Bs = dPnt->IsV;
            clarke1.Cs = dPnt->IsW;
            clarke1.calc(&clarke1);

// ------------------------------------------------------------------------------
//    Connect inputs of the PARK module and call the park transformation
//    calculation function.
// ------------------------------------------------------------------------------
            park1.Alpha = clarke1.Alpha;
            park1.Beta  = clarke1.Beta;
            park1.Angle = dPnt->Angle;
            park1.calc(&park1);
          
// ------------------------------------------------------------------------------
//    Добавляем регулятор тока для активной компоненты
//    для удержания двигателя на постоянном токе - путем фиксированного тока
//    GlobalM3.DCTorm - акивирует торможение постоянным током, но только на торможении включится фиксированный ток
// ------------------------------------------------------------------------------
            arm_sqrt_f32(((park1.Ds * park1.Ds) + (park1.Qs * park1.Qs)), &pi_isq.Fdb); // Находим модуль тока            
            pi_isq.Ref  =  DC_Current;//f32_Filtr(pi_isq.Ref, GlobalM4.DCBrakeCurrent, 0.03125f); // Ток задаем без амплитуды, т.к. это постоянка
            pi_isq.OutMax = dPnt->UmaxIqId;
            pi_isq.OutMin = 0.0f;
            pi_isq.calc(&pi_isq);
            Ds = pi_isq.Out;
            
            // ------------------------------------------------------------------------------
            //    Connect inputs of the INV_PARK module and call the inverse park transformation
            //    calculation function.
            // ------------------------------------------------------------------------------
            ipark1.Ds = Ds;
            ipark1.Qs = 0;
            ipark1.Angle = dPnt->Angle;
            ipark1.calc(&ipark1);

            // ------------------------------------------------------------------------------
            //    Call the Space Vector calculation function.
            // ------------------------------------------------------------------------------
            svgen_dqv.Ualpha = ipark1.Alpha;
            svgen_dqv.Ubeta = ipark1.Beta;
            svgen_dqv.calc(&svgen_dqv);

            iclarke1.Alpha = ipark1.Alpha;
            iclarke1.Beta = ipark1.Beta;
            iclarke1.calc(&iclarke1); 
            // ------------------------------------------------------------------------------
            //    Connect inputs of the DT_COMP module and call
            //    calculation function.
            // ------------------------------------------------------------------------------
            dtc1.Iu = dPnt->IsU;
            dtc1.Iv = dPnt->IsV;
            dtc1.Iw = dPnt->IsW;
            dtc1.Ta = svgen_dqv.Ta;
            dtc1.Tb = svgen_dqv.Tb;
            dtc1.Tc = svgen_dqv.Tc;
            dtc1.Ud = UdcFiltr.UdcOut; //dPnt->Udc;
            dtc1.Fpwm = 0;
            dtc1.calc(&dtc1);

            svgen_dqv.Ta = dtc1.Ta;
            svgen_dqv.Tb = dtc1.Tb;
            svgen_dqv.Tc = dtc1.Tc;
            
// ------------------------------------------------------------------------------
//    PWM References output._NEW
// ------------------------------------------------------------------------------
            Pwm.Ta = svgen_dqv.Ta;
            Pwm.Tb = svgen_dqv.Tb;
            Pwm.Tc = svgen_dqv.Tc;
            Pwm.Update(&Pwm);

// ------------------------------------------------------------------------------
//    Вывод текущих значений
// ------------------------------------------------------------------------------
            dPnt->Wr = 0.0f;        // Текущая скорость ротора (в скаляре всегда = 0)
            dPnt->We = dPnt->Wrz;   // Текущая скорость поля (pu)          
}
///*********************************************************************************************///

void ACIM_Vector_Calc(void) {
  
    float Ds, Tmp1;  // IQsE
    f32          WnomRot;
    static float IMDs1 = 0, Iqslow = 0;
    static float SpeedWr = 0;
    static float Udcveryslow = 0, GeneratorUdc = 0;
    static float We1 = 0;
    static float RefFiltr_T = 0, RefFiltr_Ts = 0, RefFiltr_K11 = 0, RefFiltr_K21 = 0, RefFiltr_K41 = 0;
    static float output = 0, outputn_1 = 0;
    static float PickupIsd = 0, PickupFluxMaxErr = 0; // dIsdPU = 0;
    float        PickupFluxErr, TorqueMaxRefLocal;
    static u32   PickupSearchTimeCnt = 0; // PickupPhirUpTime = 0
    f32          Fref_Hz;

    float TorqueNomPu_KBASE;
    float TorqueMax;// TorqueMin;    
    static PickUpState_enum PickupState = InitStartPU;
    float       Pickup_Idn_iq;
    static f32  LinearTorqueMax = 0;
    static u32 PickupStabilizedTimeCnt = 0;
    
    aci_flag.SpdCalc = 0;
    IsrTicker++;

	if(!DrvWords.Ctrl.bit.DriveEnb) {
  
            DrvWords.Stat.bit.vPrUP = 0;
            pi_spd.ResetUiPreviuos = GlobalM4.Pretorque.mPretorqueReset;    
            pi_isd.reset(&pi_isd);
            pi_isq.reset(&pi_isq);
            pi_spd.reset(&pi_spd);
            fe1.reset(&fe1); // сброс потокосцепления
            linear_ramp1.reset(&linear_ramp1); // Сброс линейного ЗИ
            linear_ramp2.reset(&linear_ramp2); // Сброс линейного ЗИ
            AciPidRegPsir.reset(&AciPidRegPsir);
            
            IsrTicker = dPnt->DivRegSpd;
            GeneratorUdc = Udcveryslow * 1.1f;
          
            // Дополнительный фильтр на скорость
            RefFiltr_T = GlobalM4.NewRefFiltrTime;                                   // постоянная времени фильтра (задержка АСУ)
            RefFiltr_Ts = 1.0f/dPnt->Fpwm;

            RefFiltr_K11 = RefFiltr_Ts 	/ (2.0f * RefFiltr_T + RefFiltr_Ts);
            RefFiltr_K21 = (2.0f * RefFiltr_T)/ (2.0f * RefFiltr_T + RefFiltr_Ts);

            RefFiltr_K41 = RefFiltr_Ts 	/ (RefFiltr_T);
            
            PickupState = InitStartPU; // Инициалализация этапа Подхват
            DrvConst.StartWithPickup = 0; // Дополнительный флаг для информирования ВУ, что переход выполнен из Подхвата.
            
            // Для скалярных режимов старт идет с поля 180 градусов, для безопасной выдачи выходного напряжения.
//            if (DrvConst.DriveType == TYPE_SCALAR_NO_DEADBAND || DrvConst.DriveType == TYPE_SCALAR) {
//              dPnt->Angle = Scalar.Angle =0; //_5PI_DIV6; //_1PI_DIV6;//_PI;
//            }
	}

	if(!DrvWords.Ctrl.bit.vCodeEnb){
	    return; // выключение обработчика ВУ внешне
	}

	if(IsrTicker == dPnt->DivRegSpd) {
	    aci_flag.SpdCalc = 1;
	    IsrTicker = 0;
	}
        if (!DrvWords.Ctrl.bit.DriveEnb) {
            GlobalM4.PhiRTorqIact = dPnt->Idn_iq; // Если я в останове - берем задание тока возбуждения
        } else {
            GlobalM4.PhiRTorqIact = IMDs1; // Если в работе - берем текущее значение потока.
        }

// ------------------------------------------------------------------------------
//    Фильтр (ФНЧ 1-го порядка) напряжения ЗПТ
// ------------------------------------------------------------------------------
        UdcFiltr.UdcIn = dPnt->Udc;
        UdcFiltr.calc(&UdcFiltr);        
        
	// Фильтруем Udc и определяем верхнием пределы ограничений
	Udcveryslow = UdcFiltr.UdcOut; //f32_Filtr(Udcveryslow, dPnt->Udc, 0.0009765f);
	dPnt->Ustmax = dPnt->Udc * dPnt->Kez;

	dPnt->UmaxIqId = Udcveryslow / _SQRT_3; 
	
	// Передаем текущий поток для оценки достижения возбуждения
	dPnt->PhiR = IMDs1;
	DrvWords.calc(&DrvWords, dPnt);

	// Ввод коэффициентов наблюдателя потока
  	if(GlobalM4.FlagCommand.bit.BitAutoKest == 0){
	   fe1.Kp = GlobalM4.KpFe;                  //ручное
	   fe1.Ki = GlobalM4.KiFe * fe1_const.Tr;
	}else{
	   fe1.Kp =  GlobalM4.KpFe;                 //автомат
	   fe1.Ki = 0.25f * fe1_const.Tr;           // Фиксированное значение интеграла
	}      
        
        // Задаем в регулятор скорости его максимальный выходной ток который допустим в данный момент времени
        pi_spd.Imax = 1.414f*GlobalM4.ImaxVFD; //Приводим его к амплитудному значению
        
	fe1.Tpwm = dPnt->Tpwm;
        GlobalM4.vPrUP = DrvWords.Stat.bit.vPrUP;
        if (GlobalM4.FlagState.bit.PickUpFOC) {
          DrvConst.DriveType = TYPE_ACIM_PICKUP_MASK + DrvConst.DriveType;
        }
        
	switch (DrvConst.DriveType){

///*********************************************************************************************///
///                    СКАЛЯРНОЕ УПРАВЛЕНИЕ БЕЗ КОМПЕНСАЦИИ МВ                                  ///
///*********************************************************************************************///
        case TYPE_SCALAR_NO_DEADBAND:{ // скаляр без компенсации мертвого времени
             Scalar.Udc = IQ_to_FLOAT(DrvConst.Udc, Volt_base);      // Текущее значение напряжения в ЗПТ 
             Scalar.Uscl = IQ_to_FLOAT(DrvConst.Uscl, Volt_base);    // Задание на выходное напряжения
             //Scalar.Fscl = DrvConst.Wrz; // Задание частоты - Здесь частота задания относительная. Перевод ее в абсолютное значение учтен в StepAngle = FreqBASE * Tpwm
             Scalar.Fscl = IQ_to_FLOAT(DrvConst.Wrz, Herz_base); // Задание частоты
             Scalar.StepAngle = DrvConst.StepAngle;                  // Шаг угла (за период ШИМ)
             Scalar.SvgenMode = DrvConst.SvgenMode;                  // тип ШИМа
             Scalar.calc(&Scalar);                                   // Расчет и формирование ШИМ 
  
             dPnt->Us = FLOAT_to_IQ(Scalar.Uout, Volt_base);         // Сохранить выходное напряжение ПЧ (расчетное значение)
             dPnt->Wr = 0;//dPnt->Wrz;                               // Текущая скорость ротора
             dPnt->We = dPnt->Wrz;                                   // Текущая скорость поля (pu)
             dPnt->Angle = Scalar.Angle;                             // Текущий угол поля
             dPnt->Mout = 0.0f;                                      // Текущий момент
             // Флаг периода выходного напряжения
             if(Scalar.Period){
               Scalar.Period = 0;
               dPnt->Period = 1;  
             }              
	} break;

///*********************************************************************************************///
///                             СКАЛЯРНОЕ УПРАВЛЕНИЕ                                            ///
///*********************************************************************************************///
	case TYPE_SCALAR: //скаляр
            // расчет угла положения поля и выходного напряжения в скалярном управлении
            Ds = (dPnt->Uscl * K_MOD_SPACEPWM) / dPnt->Udc;
            if(Ds > 0.98f){
               Ds = 0.98f;             // Уперлись в ограничение по выходному напяжению
            }

            if(DrvWords.Ctrl.bit.DriveEnb){
               dPnt->Us = (dPnt->Udc * Ds) / K_MOD_SPACEPWM;
            }
            else{
               dPnt->Us = 0;
            }
            
            Fref_Hz = IQ_to_FLOAT(DrvConst.Wrz, Herz_base);
            dPnt->Angle = dPnt->Angle + (dPnt->StepAngle * IQ_to_FLOAT(DrvConst.Wrz, Herz_base) * _2PI);
            //dPnt->Angle = dPnt->Angle + (dPnt->StepAngle * dPnt->Wrz * _2PI); // Здесь частота задания относительная. Перевод ее в абсолютное значение учтен в StepAngle = FreqBASE * Tpwm
            if(dPnt->Angle > _2PI) {
               dPnt->Angle -= _2PI;
               dPnt->Period = 1;
            } else if (dPnt->Angle < -_2PI) {
               dPnt->Angle += _2PI;
               dPnt->Period = 1;
            }
            
            // ------------------------------------------------------------------------------
            //    Connect inputs of the INV_PARK module and call the inverse park transformation
            //    calculation function.
            // ------------------------------------------------------------------------------
            ipark1.Ds = Ds;
            ipark1.Qs = 0;
            ipark1.Angle = dPnt->Angle;
            ipark1.calc(&ipark1);

            // ------------------------------------------------------------------------------
            //    Call the Space Vector calculation function.
            // ------------------------------------------------------------------------------
            svgen_dqv.Ualpha = ipark1.Alpha;
            svgen_dqv.Ubeta = ipark1.Beta;
//            svgen_dqv.calc(&svgen_dqv);
//            svgen_dqv.dpwm60(&svgen_dqv);
                                                 
            switch (dPnt->SvgenMode) {
                case 0: svgen_dqv.calc(&svgen_dqv); break;
                case 1: svgen_dqv.dpwm60(&svgen_dqv); break;
                case 2:
                  if (Fref_Hz > 10.0) {
                    svgen_dqv.dpwm60(&svgen_dqv);
                  } else {
                    svgen_dqv.calc(&svgen_dqv);
                  }                  
                  break;
                default: svgen_dqv.calc(&svgen_dqv); break;
            }            
            
            iclarke1.Alpha = ipark1.Alpha;
            iclarke1.Beta = ipark1.Beta;
            iclarke1.calc(&iclarke1); // Добавиь в скаляр Ua Ub - UaUbUc   и вывести на логгер
            // ------------------------------------------------------------------------------
            //    Connect inputs of the DT_COMP module and call
            //    calculation function.
            // ------------------------------------------------------------------------------
            dtc1.Iu = dPnt->IsU;
            dtc1.Iv = dPnt->IsV;
            dtc1.Iw = dPnt->IsW;
            dtc1.Ta = svgen_dqv.Ta;
            dtc1.Tb = svgen_dqv.Tb;
            dtc1.Tc = svgen_dqv.Tc;
            dtc1.Ud = UdcFiltr.UdcOut; //dPnt->Udc;
            dtc1.We = dPnt->Wrz;
            dtc1.Fpwm = 0; //dPnt->Fpwm;
            dtc1.calc(&dtc1);

            svgen_dqv.Ta = dtc1.Ta;
            svgen_dqv.Tb = dtc1.Tb;
            svgen_dqv.Tc = dtc1.Tc;
            
// ------------------------------------------------------------------------------
//    PWM References output._NEW
// ------------------------------------------------------------------------------
            Pwm.Ta = svgen_dqv.Ta;
            Pwm.Tb = svgen_dqv.Tb;
            Pwm.Tc = svgen_dqv.Tc;
            Pwm.Update(&Pwm);

// ------------------------------------------------------------------------------
//    Вывод текущих значений
// ------------------------------------------------------------------------------
            //dPnt->Wr = 0.0f;        // Текущая скорость ротора (в скаляре всегда = 0) - ЗАБЛОКИРОВАТЬ ЕСЛИ ВКЛЮЧЕН НАБЛЮДАТЕЛЬ
            dPnt->We = dPnt->Wrz;   // Текущая скорость поля (pu)
            dPnt->Mout = 0.0f;
                          
            if (GlobalM4.EncoderParam.EncoBlockPresent) {
                SpeedWr = GlobalM4.EncoderCalcParam.electricSpd; //f32_Filtr(dPnt->Wr, GlobalM4.EncoderCalcParam.electricSpd, 0.0078125f);   // вывод скорости ротора
            }           
              
            dPnt->Wr = f32_Filtr(dPnt->Wr, SpeedWr, 0.0078125f);   // вывод скорости ротора            
                                  
	break;
                
///*********************************************************************************************///
///                       ДАТЧИКОВОЕ ВЕКТОРНОЕ УПРАВЛЕНИЕ                                       ///
///*********************************************************************************************///
	case TYPE_ACIM_FOC_SENSORED: //вектор датчиковый

//		break;

///*********************************************************************************************///
///                       БЕЗДАТЧИКОВОЕ ВЕКТОРНОЕ УПРАВЛЕНИЕ                                    ///
///*********************************************************************************************///
	case TYPE_ACIM_FOC_SENSORLESS: //вектор бездатчиковый
// ------------------------------------------------------------------------------
//    Connect inputs of the CLARKE module and call the clarke transformation
//    calculation function.
// ------------------------------------------------------------------------------
		clarke1.As = dPnt->IsU;
		clarke1.Bs = dPnt->IsV;
                clarke1.Cs = dPnt->IsW;
		clarke1.calc(&clarke1);
             
// ------------------------------------------------------------------------------
//    Connect inputs of the PARK module and call the park transformation
//    calculation function.
// ------------------------------------------------------------------------------               
                park1.Alpha = clarke1.Alpha;
                park1.Beta  = clarke1.Beta;                
		park1.Angle = soft_cur2.ThetaPsi_out;
		park1.calc(&park1);
                                                           
		// ------------------------------------------------------------------------------
		// Проверяем режим работы двигателя: генераторный или двигательный
		// Если знак активного тока и частоты ротора совпадают - это двигательный режим
		// ------------------------------------------------------------------------------
		if(DrvWords.Stat.bit.vPrUP){
                    if( fabsf(dPnt->Wrz) < _01_Hz){
                       Iqslow = 0.0f;
                    } else {
                       Iqslow = f32_Filtr(Iqslow, park1.Qs, 0.0009765f); 
                    }
	             // Помимо активного тока и частоты, проверяем факт заливки Ud. Превышение 105% от нормального значения Ud
                    //if( ((dPnt->Wrz < -_2_HZ) && (Iqslow > GlobalM4.EnginParam.Isn * 0.05f) && (dPnt->Udc > GeneratorUdc)) || ((dPnt->Wr > _2_HZ) && (Iqslow < -GlobalM4.EnginParam.Isn * 0.05f)) && (dPnt->Udc > GeneratorUdc)) {
                    if( dPnt->Udc > GeneratorUdc ) {
                        DrvWords.Stat.bit.GeneratorMode = 1;
                    } else {
                        DrvWords.Stat.bit.GeneratorMode = 0;
                    }
		} else {
		    Iqslow = 0.0f;
		}
                
                GlobalM4.GenMode = DrvWords.Stat.bit.GeneratorMode;
// ------------------------------------------------------------------------------
//    Connect inputs of the ACI module and call the flux estimation
//    calculation function. We
// ------------------------------------------------------------------------------
//                if (GlobalM4.Debug3 == 111) {
//                  fe1.IDsS = clarke1.AlphaFiltr;
//                  fe1.IQsS = clarke1.BetaFiltr;
//                } else {
//                  fe1.IDsS = clarke1.Alpha;
//                  fe1.IQsS = clarke1.Beta;
//                } 
                
#ifdef COMPENSATE_DELAY_FE         
                // Работа наблюдателя с учетом компенсации задержек
                f32     MeasPhaseCor;
		MeasPhaseCor = (1.0f*dPnt->Tpwm*dPnt->FreqBASE);
                
		ipark2.Ds = clarke1.Alpha;
		ipark2.Qs = clarke1.Beta;
		ipark2.Angle = We1 * MeasPhaseCor; // We1 берем по сути за предыдущий такт расчета
		ipark2.calc(&ipark2);                
                
                fe1.PhaseCorr = We1 * MeasPhaseCor; // 
                fe1.FluxThetaCorMode = GlobalM4.FluxThetaCorMode;
                fe1.FluxThetaOffset  = GlobalM4.FluxThetaOffset;
                fe1.FluxThetaPidKp   = GlobalM4.FluxThetaPidKp;
                fe1.FluxThetaPidKi   = GlobalM4.FluxThetaPidKi;
                fe1.Fcur = SpeedWr;
	 	fe1.IDsE = park1.Ds;
		fe1.UDsS = ipark1.Alpha;
		fe1.UQsS = ipark1.Beta;
		fe1.IDsS = ipark2.Alpha; //clarke1.Alpha;
		fe1.IQsS = ipark2.Beta; //clarke1.Beta;
		fe1.PhirUP = DrvWords.Stat.bit.vPrUP || DrvConst.StartWithPickup; // Разрешаем работу по возбуждению или по переходу из Подхвата, иначе не будет фазы
		fe1.GenMode = DrvWords.Stat.bit.GeneratorMode;
		fe1.calc(&fe1);
		IMDs1 = fe1.FluxDrE;
#else // Штатная работа наблюдателя
	 	fe1.IDsE = park1.Ds;
		fe1.UDsS = ipark1.Alpha;
		fe1.UQsS = ipark1.Beta;
		fe1.IDsS = clarke1.Alpha;
		fe1.IQsS = clarke1.Beta;
		fe1.PhirUP = DrvWords.Stat.bit.vPrUP || DrvConst.StartWithPickup; // Разрешаем работу по возбуждению или по переходу из Подхвата, иначе не будет фазы
		fe1.GenMode = DrvWords.Stat.bit.GeneratorMode;
		fe1.calc(&fe1);
		IMDs1 = fe1.FluxDrE; 
                
                Ветка не работает ее надо удалить
#endif
// ------------------------------------------------------------------------------
//    Connect inputs of the ACI module and call the speed estimation
//    calculation function.
// ------------------------------------------------------------------------------
//                if (GlobalM4.Debug3 == 111) {
//                  se1.IDsS = clarke1.AlphaFiltr;
//                  se1.IQsS = clarke1.BetaFiltr;                  
//                } else {
//                  se1.IDsS = clarke1.Alpha;
//                  se1.IQsS = clarke1.Beta;
//                }		
                
                se1.IDsS = clarke1.Alpha;
		se1.IQsS = clarke1.Beta;
		se1.PsiDrS = fe1.PsiDrS;
		se1.PsiQrS = fe1.PsiQrS;
		se1.ThetaFlux = soft_cur2.ThetaPsi_out;
                se1.PhirUP = fe1.PhirUP;
		se1.calc(&se1);

		We1 = se1.WPsi; // 
                SpeedWr = se1.WrHat;
                
//                if (GlobalM4.Debug5 == 111 /*&& GlobalM4.EncoderParam.EncoBlockPresent*/) {
//                  SpeedWr = GlobalM4.EncoderCalcParam.electricSpd; //f32_Filtr(dPnt->Wr, GlobalM4.EncoderCalcParam.electricSpd, 0.0078125f);   // вывод скорости ротора
//                } else {
//                    SpeedWr = se1.WrHat;
//                }
                              
	 	
		// Обнулялка скорости, когда ШИМ выключен
	 	if(!DrvWords.Ctrl.bit.DriveEnb){
                   SpeedWr = 0.0f;		
                }

 		//Выбор формулы расчет момента. Для двигательного режима хорошо работает через активный ток, для генераторного режима - через dq компоненты.
		if(DrvWords.Stat.bit.vPrUP){
                    if(fabsf(SpeedWr) >= _3_HZ) {
                        dPnt->Me = park1.Qs * se1.K6 * IMDs1 * dPnt->KBase;               // Через активный ток
                        dPnt->Mout1= se1.NewTorq * dPnt->KBase;                       // Через квадратурные компоненты
                    }
                    else{
                        dPnt->Me = 1.25f * park1.Qs * se1.K6 * IMDs1 * dPnt->KBase;       // Через активный ток
                        dPnt->Mout1 = 1.25f * se1.NewTorq * dPnt->KBase;
                    }
                    dPnt->Me = dPnt->Me * GlobalM4.TorqueScale;                       // Поправка момента с учетом коэф. масштабирования
		}
                else{
                  dPnt->Me      = 0;
                  dPnt->Mout1   = 0;
                }

		//============================================================
		// Дополнительный фильтр на скорость
		if(DrvWords.Stat.bit.vPrUP) {
                    output = outputn_1 + (dPnt->Wrz * RefFiltr_K41);
                    dPnt->NewRefFiltrOut = output;

                    // Если фильтр включен - берем задание прямо из АСУ канала
                    output = (dPnt->Wrz * RefFiltr_K11) + (outputn_1 * RefFiltr_K21);
                    outputn_1 = output;
		} else {
		    dPnt->NewRefFiltrOut = output = outputn_1 = 0;
		}
		//============================================================
		// Фильтрация коэф. контура скорости при динамическом изменении
		// Обязательно вынести за проверку возбуждения                               
                // Если включен режим антиотката - загружаем в начале работы исходные значения коэф.
                // в переменные фильтрации
                if (arollback1.flag.ForceEnb) { // До старта, при разрешенном Антиоткате,этот флаг уже стоит, и сбросится только после отведенного времени
                  pi_spd.Kp = pi_spd.Kp_norm * DrvConst.SpdForced; // Одинаково пропорциональное ускорение регулятора скорости
                  pi_spd.Ki = pi_spd.Ki_norm * DrvConst.SpdForced;
                } else { // Если Антиотката нету - загружаем обычные коэф.
                  pi_spd.Kp = f32_Filtr(pi_spd.Kp, pi_spd.Kp_norm/*GlobalM4.KpSpd2*/, GlobalM4.Filter_KprRS);
                  pi_spd.Ki = f32_Filtr(pi_spd.Ki, pi_spd.Ki_norm/*GlobalM4.KiSpd2*/, GlobalM4.Filter_KintRS);
                }                 
		//============================================================
		// Расчет номинального момента приведенного к Iq
		TorqueNomPu_KBASE = fabsf(GlobalM4.TorqueNomPu) / (se1.K6 * IMDs1);
		TorqueNomPu_KBASE = TorqueNomPu_KBASE / dPnt->KBase;
		//============================================================

                // Линейное повышение момента задания, при переходе их Подхвата
                if (DrvConst.StartWithPickup && DrvWords.Stat.bit.vPrUP) {
                  LinearTorqueMax = f32_Filtr(LinearTorqueMax, fabsf(GlobalM4.TorqueMax), 0.005f);                  
                  linear_ramp2.EndValue = LinearTorqueMax; //fabsf(GlobalM4.TorqueMax); // Конечное значение момента
                  linear_ramp2.StartValue = 0; // Начальное значение момента
                  linear_ramp2.Fpwm = dPnt->Fpwm;
                  linear_ramp2.RampTime = GlobalM4.RiseTorqueTime * fe1_const.Tr; // Разгон момента           
                  linear_ramp2.StartFlag = 1;
                  linear_ramp2.calc(&linear_ramp2); // Запускаем в работу ЗИ
                  if (linear_ramp2.WorkDone) DrvConst.StartWithPickup = 0;
                  
                  TorqueMaxRefLocal = linear_ramp2.Out;
                } else {
                  TorqueMaxRefLocal = fabsf(GlobalM4.TorqueMax);
                  LinearTorqueMax = f32_Filtr(LinearTorqueMax, fabsf(GlobalM4.TorqueMax), 0.005f);
                }
                                
        	if(DrvWords.Stat.bit.vPrUP){
		    if(aci_flag.SpdCalc) {                   // прореживание периодичности выполнения
                        //Задание на момент
                        TorqueMax = fabsf(TorqueMaxRefLocal) / (se1.K6 * IMDs1);
                        TorqueMax = TorqueMax / dPnt->KBase;
                        pi_spd.SavePrevUiFlag = GlobalM4.FlagCommand.bit.BreakOff; // Для Лифта - если тормоз отпущен, фиксируем текущее значение интеграла, для последующего восстановления (внутри регулятора за)
                        pi_spd.RestorePrevUiFlag = GlobalM4.Pretorque.mPretorqueSet; // Флаг начала восстановления интеграла с последнего останова
                        pi_spd.RestorePrevUiTime = GlobalM4.Pretorque.mPretorqueTime; // Время восстановления интеграла с последнего останова
                        pi_spd.OutMax = f32_Filtr(pi_spd.OutMax, TorqueMax, 0.005f); // TorqueMax;  // ограничение момента в рег по скорости
                        pi_spd.OutMin = -pi_spd.OutMax; // Нижний предел прийдет со знаком минус
                        
                        if(DrvWords.Ctrl.bit.FiltrRefSpdEnb) {
                            pi_spd.Ref = dPnt->NewRefFiltrOut; // Задание скорости фильтрованное
                        }else {
                            pi_spd.Ref = dPnt->Wrz;
                        } 
                        // ФИЛЬТР ОТКЛЮЧЕН - ОН ПОКА НЕ РАБОТАЕТ НОРМАЛЬНО
                        pi_spd.Ref = dPnt->Wrz;
                        pi_spd.Fdb = SpeedWr; //GlobalM4.EncoderCalcParam.electricSpd; //SpeedWr;
                        pi_spd.FreezeUi = 0; //cross1.UsmaxFlag;
                        // Вызываем расчет регулятора скорости
                        pi_spd.calc(&pi_spd);
                    } // if (aci_flag.SpdCalc)
                    
                    // !!!! Вычитку регулятора скорости надо делать вне условия aci_flag.SpdCalc, иначе в прореживании будет глюк
                    Tmp1 = GlobalM4.piSpdOut = pi_spd.Out;	// Фиксируем выходное значение регулятора скорости                     
                }
                else{
                    pi_spd.Ref = 0.0f;
                    pi_spd.Fdb = 0.0f;
                    pi_spd.calc(&pi_spd);
                    Tmp1 = 0.0f;
                    pi_spd.Out = 0.0f;
                    spdlim1.WrH = spdlim1.WrL; // Пока мы в останове, максимальная скорость в контуре момента равна минимальной скорости
                }

// ------------------------------------------------------------------------------
//    Connect inputs of the PID module and call the PID field weakening controller
//    calculation function.
// ------------------------------------------------------------------------------
            // (МОДЕЛЬ) Если генераторный режим - расширяем пределы для регуляторов ЭДС и потокосцепеления
	    if(DrvWords.Stat.bit.GeneratorMode) {
              WnomRot = dPnt->Wnom * 0.9f;
            } else {
              WnomRot = dPnt->Wnom;
            }
            
            // (МОДЕЛЬ) гистерезис включениz регуляторов ЭДС и потокосцепеления, ширина 1 Гц
            #define HIST_1_HZ (1.0f/80.0f) 
            if(fabsf(pi_spd.Fdb) > WnomRot) {
              DrvWords.Stat.bit.FWsecondZoneOnState = 1;         // Переход в ослабление поля
            } else if(fabsf(pi_spd.Fdb) < (WnomRot - HIST_1_HZ)) {
              DrvWords.Stat.bit.FWsecondZoneOnState = 0;         // Работа без ослабления поля
            }
            
            // Изменение пределов ограничения для регулятора противоЭДС, в зависимости от режимов работы
            if(!DrvWords.Stat.bit.GeneratorMode) { // Если двигательный режим
               if(DrvWords.Stat.bit.FWsecondZoneOnState){
                    pi_emf.OutMax = pi_emf_const1.OutMax * 0.8f; // В одиночном режиме Предел занижаем до 0.9, и тогда работает нормально.
                    pi_emf.OutMin = -pi_emf.OutMax;
               } else {                 
                 #define SLOW_LIFT_SPD   (10.0f/80.0f)                 
                 #define NORMAL_LIFT_SPD  (12.0f/80.0f) 
                 if (GlobalM4.PresentFunctional == UseLiftFunc && DrvWords.Stat.bit.vPrUP) {
                   // Если у нас Лифт, то на низких частотах в обычном режиме поднимаем возбуждение на величину  GlobalM4.MultIdKoefGM                     
                   if (pi_spd.Ref < SLOW_LIFT_SPD) {
                     pi_emf.OutMax = pi_emf_const1.OutMax * GlobalM4.MultIdKoefGM;
                     pi_emf.OutMin = -pi_emf.OutMax;                 
                   } else if (pi_spd.Ref > NORMAL_LIFT_SPD) {
                      pi_emf.OutMax = pi_emf_const1.OutMax;
                      pi_emf.OutMin = -pi_emf.OutMax;                     
                   }
                 } else {
                      pi_emf.OutMax = pi_emf_const1.OutMax;
                      pi_emf.OutMin = -pi_emf.OutMax;
                 }
               }
            } else {  // Если генераторный режим - расширяем пределы в 1,3 раза для подъема возбуждения в двигателе
              pi_emf.OutMax = pi_emf_const1.OutMax * GlobalM4.MultIdKoefGM;
              pi_emf.OutMin = -pi_emf.OutMax;              
            }
            //------------------------------------------------------------
            // Ограничиваем задание противоЭДС, чтоб было не выше вычисленного значения при забросах Ud.
            if(UdcFiltr.UdcOut > _SQRT_2){
                pi_emf.Ref = dPnt->EmfRef_iq * _SQRT_2; // Если Ud выше нормы - ставим нормальное значение Ud (sqrt(2))
            }
            else{
                pi_emf.Ref = dPnt->EmfRef_iq * UdcFiltr.UdcOut; // Если ниже нормы - учитываем его в расчете
            }

            pi_emf.Fdb = fabsf(IMDs1 * se1.WPsi * dPnt->EmfFdb_K); //EmfFdb;
            // Тестовый фильтр на ОС противоЭДС
            //pi_emf.Fdb = f32_Filtr(pi_emf.Fdb, fabsf(IMDs1 * se1.WPsi * dPnt->EmfFdb_K), 0.0002f);  //EmfFdb; 
            pi_emf.calc(&pi_emf);

            // Приведение регулятора противоЭДС к новой форме
            if( fabsf(se1.WPsi) >= dPnt->Wnom ) {
                pi_emf.Out = pi_emf.Out / fabsf(se1.WPsi); // Выход делим на текущую частоту вращения
            }
            else{
                pi_emf.Out = pi_emf.Out / dPnt->Wnom;     // Приводим задание возбуждения через номинальную частоту двигателя
            }

            
            // Регулятор потокосцепления
            arm_sqrt_f32((fe1.PsiDrS * fe1.PsiDrS) + (fe1.PsiQrS * fe1.PsiQrS), &AciPidRegPsir.pi_psir_Fdb);
            //AciPidRegPsir.pi_psir_Fdb         = sqrt((fe1.PsiDrS * fe1.PsiDrS) + (fe1.PsiQrS * fe1.PsiQrS));   // ОС регулятора
            AciPidRegPsir.WPsi                = se1.WPsi;
            AciPidRegPsir.Udcveryslow         = UdcFiltr.UdcOut;                                                            // Текущее напряжение ЗПТ (фильтрованное)
            AciPidRegPsir.FWsecondZoneOnState = DrvWords.Stat.bit.FWsecondZoneOnState;                                      // Флаг перехода в ослабление поля
            
            if(cross1.UsmaxFlag){
              AciPidRegPsir.FlagUsQmax = 1;
            }
            else{
              AciPidRegPsir.FlagUsQmax = 0;
            }
            AciPidRegPsir.calc(&AciPidRegPsir);            
            
// ------------------------------------------------------------------------------
//    Connect inputs of the SOFTCURFAULT module and call the soft current fault
//    calculation function.
// ------------------------------------------------------------------------------
            soft_cur2.ThetaPsi_in = fe1.ThetaFlux;
            soft_cur2.bit.Start_fl = DrvWords.Ctrl.bit.SoftCurDwn;  
            soft_cur2.Fpwm = dPnt->Fpwm;

            // Старый алгоритм выбора ослабления поля
//            if(DrvWords.Ctrl.bit.EMFRegEnb) {       // Вкл/Откл ослабление поля
//                soft_cur2.IdRef_in = pi_emf.Out;
//            }
//            else{
//              
//                if(!DrvWords.Stat.bit.GeneratorMode){ 
//                  if (GlobalM4.PresentFunctional == UseLiftFunc && DrvWords.Stat.bit.vPrUP) {
//                    // Если у нас Лифт, то на низких частотах в обычном режиме поднимаем возбуждение на величину  GlobalM4.MultIdKoefGM                     
//                    if (pi_spd.Ref < SLOW_LIFT_SPD) {
//                      soft_cur2.IdRef_in = dPnt->Idn_iq * GlobalM4.MultIdKoefGM;                 
//                    } else if (pi_spd.Ref > NORMAL_LIFT_SPD) {
//                      soft_cur2.IdRef_in = dPnt->Idn_iq;                     
//                    }
//                  } else {
//                    soft_cur2.IdRef_in = dPnt->Idn_iq;
//                  }
//                } else { 
//                    soft_cur2.IdRef_in = dPnt->Idn_iq * GlobalM4.MultIdKoefGM;
//                }
//            }

            
            // выбор управления током возбуждения
            switch((SourceIdRef_type)DrvWords.Ctrl.bit.EMFRegEnb){
                case RefIxx:{                                // Ixx - No regulation
//                      soft_cur2.IdRef_in = dPnt->Idn_iq;                      
                      if(!DrvWords.Stat.bit.GeneratorMode){ 
                        if (GlobalM4.PresentFunctional == UseLiftFunc && DrvWords.Stat.bit.vPrUP) {
                          // Если у нас Лифт, то на низких частотах в обычном режиме поднимаем возбуждение на величину  GlobalM4.MultIdKoefGM                     
                          if (pi_spd.Ref < SLOW_LIFT_SPD) {
                            soft_cur2.IdRef_in = dPnt->Idn_iq * GlobalM4.MultIdKoefGM;                 
                          } else if (pi_spd.Ref > NORMAL_LIFT_SPD) {
                            soft_cur2.IdRef_in = dPnt->Idn_iq;                     
                          }
                        } else {
                          soft_cur2.IdRef_in = dPnt->Idn_iq;
                        }
                      } else { 
                        soft_cur2.IdRef_in = dPnt->Idn_iq * GlobalM4.MultIdKoefGM;
                      }
                    } break;
                
                case RefRegEMF:{                             // emf
                    soft_cur2.IdRef_in = pi_emf.Out;
                    } break;
                
                case RefRegPsir:{                            // psir
                        //soft_cur2.IdRef_in = AciPidRegPsir.pi_psir_Out;

                          // Заблокировано - Сосновчик. После ввода коррекции фазы потока это лишниее, плюс дает скачок в потоке
//                        if (GlobalM4.PresentFunctional == UseLiftFunc && DrvWords.Stat.bit.vPrUP) {
//                          // Если у нас Лифт, то на низких частотах включаем фиксированный поток и поднимаем на величину  GlobalM4.MultIdKoefGM                     
//                          if (pi_spd.Ref < SLOW_LIFT_SPD) {
//                            soft_cur2.IdRef_in = dPnt->Idn_iq * GlobalM4.MultIdKoefGM;                 
//                          } else if (pi_spd.Ref > NORMAL_LIFT_SPD) {
//                            soft_cur2.IdRef_in = AciPidRegPsir.pi_psir_Out;                     
//                          }
//                        } else { // Если не Лифт - то работает регулятор
//                          soft_cur2.IdRef_in = AciPidRegPsir.pi_psir_Out;
//                        }                    
                        
                        soft_cur2.IdRef_in = AciPidRegPsir.pi_psir_Out; // Работает только регулятор.
                    
                    } break;
                
                default:{ soft_cur2.IdRef_in = 0;
                
                    } break;
            }            
            
            soft_cur2.TimeCurDown  = GlobalM4.SoftCurrentTime;
            soft_cur2.IqRef_in = Tmp1;            
    	    soft_cur2.calc(&soft_cur2);

            
// ------------------------------------------------------------------------------
//    Connect inputs of the LOADCONTYCHK module (модуль проверки целостности нагрузки)
// ------------------------------------------------------------------------------
            loadContyChk.bitStart	= GlobalM4.FlagCommand.bit.RingUp;
            loadContyChk.IdRefIn	= soft_cur2.IdRef_out;
            loadContyChk.IqRefIn	= soft_cur2.IqRef_out;
            loadContyChk.Iu 		= fabsf(dPnt->IsU);
            loadContyChk.Iv 		= fabsf(dPnt->IsV);
            loadContyChk.Iw 		= fabsf(dPnt->IsW);
//            loadContyChk.ThetaIn 	= Theta1;
            loadContyChk.Ichk		= (0.20 *dPnt->Istn);		// пока константой 20% от In
            loadContyChk.Imin		= (0.05*dPnt->Istn);		// пока константой  5% от In
            loadContyChk.Fpwm           = dPnt->Fpwm;
            loadContyChk.timerTestTime = (GlobalM4.timeLoadContyCheck * 0.5f); // Максимальное время провекри в тиках ШИМа
            loadContyChk.calc(&loadContyChk);
            dPnt->RingUpError = loadContyChk.Error;            
            
// ------------------------------------------------------------------------------
//    Connect inputs of the PID module and call the PID IQ controller
//    calculation function.  РЕГУЛЯТОР  IQ
// ------------------------------------------------------------------------------
            pi_isq.Ref = loadContyChk.IqRefOut; //soft_cur2.IqRef_out;
            pi_isq.Fdb = park1.Qs;
            pi_isq.OutMax = dPnt->UmaxIqId;
            pi_isq.OutMin = -pi_isq.OutMax;
            pi_isq.FreezeUi = 0; //cross1.UsmaxFlag;
            pi_isq.calc(&pi_isq);

// ------------------------------------------------------------------------------
//    Подаем сигналы на линейный ЗИ для поднятия возбуждения после Подхвата
// ------------------------------------------------------------------------------           
            // Основная часть - рабочая            
            if (DrvConst.StartWithPickup) {
            // заряжаем линейный ЗИ на возбуждение, если был Подхват, и даем номинальный ток возбуждения без pi_emf
                linear_ramp1.StartFlag = 1; 
                linear_ramp1.EndValue = dPnt->Idn; // Конечное возбуждение равно номинальному                
            } else { // После выхода в норму, даем возбуждение с pi_emf
                linear_ramp1.EndValue = loadContyChk.IdRefOut; //soft_cur2.IdRef_out;// Конечное возбуждение равно номинальному                              
            }
            linear_ramp1.StartValue = PickupIsd; // Начальное возбуждение, которое есть сейчас
            linear_ramp1.Fpwm = dPnt->Fpwm;
            linear_ramp1.RampTime = GlobalM4.SecondRiseIdTime * fe1_const.Tr; //1.0f; // Разгон за 3 секунды            
            linear_ramp1.calc(&linear_ramp1); // Запускаем в работу ЗИ                       
            // сбросить флаг Подхвата после разгона регулятора активного тока
// ------------------------------------------------------------------------------
//    Connect inputs of the PID module and call the PID ID controller
//    calculation function.
// ------------------------------------------------------------------------------
            pi_isd.Ref = linear_ramp1.Out; //soft_cur2.IdRef_out; // linear_ramp1.Out;
            pi_isd.Fdb = park1.Ds;
            pi_isd.OutMax = dPnt->UmaxIqId;
            pi_isd.OutMin = -pi_isd.OutMax;
            pi_isd.Kp = pi_is_const1.Kp; // Восстановление после Подхвата
            pi_isd.Ki = pi_is_const1.Ki; 
            pi_isd.FreezeUi = 0; //cross1.UsmaxFlag;
            pi_isd.calc(&pi_isd);
// ------------------------------------------------------------------------------
//    Connect inputs of the CROSS RELATION COMPENSATION module and call the CCC
//    controller calculation function.
// ------------------------------------------------------------------------------
            cross1.Isd = pi_isd.Ref; // park1.Ds
            cross1.Isq = pi_isq.Ref; // park1.Qs
            cross1.We = We1;
            cross1.Wr = SpeedWr;
            cross1.IRm = IMDs1;
            cross1.Urd = pi_isd.Out;
            cross1.Urq = pi_isq.Out;
            cross1.Usmax = dPnt->UmaxIqId;
            cross1.calc(&cross1);
                
// ------------------------------------------------------------------------------
//    Connect inputs of the INV_PARK module and call the inverse park transformation
//    calculation function.
// ------------------------------------------------------------------------------
            ipark1.Ds = cross1.Usd;
            ipark1.Qs = cross1.Usq;
            ipark1.Angle = soft_cur2.ThetaPsi_out;
            ipark1.calc(&ipark1);

// ------------------------------------------------------------------------------
//    Call the Space Vector calculation function.
// ------------------------------------------------------------------------------
            svgen_dqv.Ualpha = (ipark1.Alpha * _SQRT_3) / dPnt->Udc;
            svgen_dqv.Ubeta  = (ipark1.Beta * _SQRT_3) / dPnt->Udc;
            svgen_dqv.calc(&svgen_dqv);            

            // В режиме ВУ пока новый ШИМ не проверен и не понятно как работает.
//            switch (dPnt->SvgenMode) {
//            case 0: svgen_dqv.calc(&svgen_dqv); break;
//            case 1: svgen_dqv.dpwm60(&svgen_dqv); break;
//            default: svgen_dqv.calc(&svgen_dqv); break;
//            }
// ------------------------------------------------------------------------------
//    Connect inputs of the DT_COMP module and call
//    calculation function.
// ------------------------------------------------------------------------------
            dtc1.Iu = dPnt->IsU;
            dtc1.Iv = dPnt->IsV;
            dtc1.Iw = dPnt->IsW;
            dtc1.Ta = svgen_dqv.Ta;
            dtc1.Tb = svgen_dqv.Tb;
            dtc1.Tc = svgen_dqv.Tc;
            dtc1.Ud = UdcFiltr.UdcOut; //dPnt->Udc;
            dtc1.Fpwm = dPnt->Fpwm;
            dtc1.We = se1.WPsi;
            dtc1.calc(&dtc1);

// ------------------------------------------------------------------------------
//    PWM References output.
// ------------------------------------------------------------------------------
            Pwm.Ta = dtc1.Ta;
            Pwm.Tb = dtc1.Tb;
            Pwm.Tc = dtc1.Tc;
            Pwm.Update(&Pwm);

// ------------------------------------------------------------------------------
//    Connect inputs of CALC_VAR module and call the
//    calculation variable function.
// ------------------------------------------------------------------------------
            if(DrvWords.Ctrl.bit.DriveEnb){
                dPnt->Wr = f32_Filtr(dPnt->Wr, SpeedWr, 0.0078125f);   // вывод скорости ротора
                dPnt->We = f32_Filtr(dPnt->We, We1, 0.00390625);       // вывод скорости поля
            }
            else{
                dPnt->Wr = 0;
                dPnt->We = 0;
                SpeedWr = 0;
            }

            dPnt->Angle = /*soft_cur2.ThetaPsi_out;*/  atan2f(ipark1.Beta, ipark1.Alpha);

            if(DrvWords.Ctrl.bit.DriveEnb){
                dPnt->Us = f32_Filtr(dPnt->Us, (cross1.Us * _SQRT_3_DIV_2), 0.03125f);
            }
            else{
                dPnt->Us = 0.0f;
            }

            // вывод флага периода
            if(se1.Period) {
                dPnt->Period = 1;
                se1.Period = 0;
            }

            break;
            
        // *********************************************************************************   
	case TYPE_ACIM_PICKUP_SENSORLESS: // Механизм определения выходной частоты вращения по БДВУ
        case TYPE_ACIM_PICKUP_SENSORED:
        case TYPE_ACIM_PICKUP_SCALAR:
        case TYPE_ACIM_PICKUP_SCALAR_DB:         
          
          Pickup_Idn_iq = dPnt->Idn_iq;// * 0.5f; // Подхват выполняем по задания тока возбуждения, равного половине нормального тока
          
          switch (PickupState) {
          case InitStartPU:
            //PickupIsd = Pickup_Idn_iq * GlobalM4.IsaturationPU; // Задание тока возбуждения           
            GlobalM4.FlagState.bit.PickUpReadyToGo = 0;
            GlobalM4.FlagState.bit.PickupFreqFound = 0;            
            //PickupPhirUpTime = (u32)(GlobalM4.FirstRiseIdTime * fe1_const.Tr/fe1_const.Ts); // Блокировка стартовой работы Подхвата на постоянную времени Ротора
            linear_ramp1.reset(&linear_ramp1); // Сброс линейного ЗИ            
            PickupState++; // На следующий уровень
            break;
            
          case WaitPhirUp:
            // заряжаем линейный ЗИ на подъем возбуждения для поиска
            linear_ramp1.StartFlag = 1;              
            linear_ramp1.StartValue = 0; // Начальное возбуждение = 0
            linear_ramp1.EndValue = Pickup_Idn_iq * GlobalM4.IsaturationPU; //Конечное возбуждение 
            linear_ramp1.Fpwm = dPnt->Fpwm;
            linear_ramp1.RampTime = GlobalM4.FirstRiseIdTime * fe1_const.Tr; // Время первичного Разгона возбуждения
            linear_ramp1.calc(&linear_ramp1); // Запускаем в работу ЗИ  
            
            // Если подъем завершен - переход далее
            if (linear_ramp1.WorkDone){ 
              PickupState++; // На следующий уровень 
              PickupStabilizedTimeCnt = (u32)(GlobalM4.FirstFixedIdTime * fe1_const.Tr/fe1_const.Ts); // Заряжаем первичный таймер поиска максимальных отклонений в потоке
//              PickupFluxMaxErr = 0; // Начальное отклонение потока - ноль.
            }
            
            PickupIsd = linear_ramp1.Out; //
            break;
              
          case StabilizedPhir: // Стадия стабилизации возбуждения после выхода на рабочее значение.
              if (PickupStabilizedTimeCnt)      PickupStabilizedTimeCnt --;
              else {
                  PickupState++; // На следующий уровень 
                  PickupSearchTimeCnt = (u32)(3.0f * fe1_const.Tr/fe1_const.Ts); // Заряжаем первичный таймер поиска максимальных отклонений в потоке
                  PickupFluxMaxErr = 0; // Начальное отклонение потока - ноль.
              }
            break;
            
          case SearchFreqPU:
            // Ждем стабилизации токовой модели, и после этого фиксируем частоту на выходе
            //GlobalM4.Debug3 = PickupSearchTimeCnt; // Logger data            
            if (PickupSearchTimeCnt) {
              PickupSearchTimeCnt --;
              PickupFluxErr = fabsf(PickupIsd - /*park1.Ds*/fe1.FluxDrE); // Находим отклонение, текущее - тока возбуждения
              if (PickupFluxErr > PickupFluxMaxErr)  PickupFluxMaxErr = PickupFluxErr; // Запоминаем новое максимальное значение
            } else { // Время наблюдения завершено
              PickupFluxErr = PickupFluxMaxErr / PickupIsd * 100.0f; // Расчет отклонения в процентах
              //GlobalM4.Debug2 = PickupFluxMaxErr * 10.0f; // Logger data
              // Если отклонение менее 1%, то мы можем запоминать текущую частоту и переходить на следующий уровень
              if (PickupFluxErr < 6.0f) {
                GlobalM4.FlagState.bit.PickupFreqFound = 1;
                //dIsdPU = ((Pickup_Idn_iq - PickupIsd) / (50.0f * fe1_const.Tr/fe1_const.Ts)); // Шаг подьема тока возбуждения для следующего такта                
                PickupState++; // На следующий уровень
                // Сброс линейного ЗИ
                linear_ramp1.reset(&linear_ramp1);                 
              } else { // Иначе повторяем оценку отклонения
                // Перезарядка поиска
                PickupSearchTimeCnt = (u32)(3.0f * fe1_const.Tr/fe1_const.Ts);  // Заряжаем первичный таймер поиска максимальных отклонений в потоке
                PickupFluxMaxErr = 0; // Начальное отклонение потока - ноль.              
              }
            }          
            break;
            
          case SetNormalPhirPU:
            // Если это ВУ - идем по укороченной программе выхода
            if (DrvConst.DriveType == TYPE_ACIM_PICKUP_SENSORLESS || DrvConst.DriveType == TYPE_ACIM_PICKUP_SENSORED) {
                // Ставим флаг, что можно передавать управление дальше
                GlobalM4.FlagState.bit.PickUpReadyToGo = 1;
                // Ставим флаг, что переход идет из Подхвата
                DrvConst.StartWithPickup = 1;
            } else {
              // TYPE_ACIM_PICKUP_SCALAR || TYPE_ACIM_PICKUP_SCALAR_DB:         
              // Если же это скаляр - то надо опустить ток возбуждения в ноль и выйти
              // заряжаем линейный ЗИ на подъем возбуждения для поиска
              linear_ramp1.StartFlag = 1;              
              linear_ramp1.StartValue = Pickup_Idn_iq * GlobalM4.IsaturationPU; //Начальное возбуждение 
              linear_ramp1.EndValue = 0; // Конечное возбуждение = 0
              linear_ramp1.Fpwm = dPnt->Fpwm;
              linear_ramp1.RampTime = 0.5f * GlobalM4.FirstRiseIdTime * fe1_const.Tr; // Обратный сброс в два раза быстрее
              linear_ramp1.calc(&linear_ramp1); // Запускаем в работу ЗИ  
              
              // Если работа завершена - завершаем работу
              if (linear_ramp1.WorkDone){ 
                // Ставим флаг, что можно передавать управление дальше
                GlobalM4.FlagState.bit.PickUpReadyToGo = 1;
                // Ставим флаг, что переход идет из Подхвата
                DrvConst.StartWithPickup = 1;
              }              
              PickupIsd = linear_ramp1.Out;             
            }            
            break;
            
          default:
            break;
          }
          
          linear_ramp2.reset(&linear_ramp2); // Сброс линейного ЗИ            
          GlobalM4.PickupStateFOC = PickupState;          
          GlobalM4.PickUpReadyToGo = GlobalM4.FlagState.bit.PickUpReadyToGo; // Logger data
          GlobalM4.PickupFreqFound = GlobalM4.FlagState.bit.PickupFreqFound; // Logger data
          
          clarke1.As = dPnt->IsU;
          clarke1.Bs = dPnt->IsV;
          clarke1.Cs = dPnt->IsW;
          clarke1.calc(&clarke1);  
          
          park1.Alpha = clarke1.Alpha;
          park1.Beta = clarke1.Beta;
          park1.Angle = fe1.ThetaFlux;
          park1.calc(&park1);
                
          fe1.Kp = GlobalM4.FluxExtimMultiple * GlobalM4.KpFe; // Ускоряем работу наблюдателя в режиме поиска.
          fe1.IDsE = park1.Ds;
          fe1.UDsS = ipark1.Alpha;
          fe1.UQsS = ipark1.Beta;
          fe1.IDsS = clarke1.Alpha;
          fe1.IQsS = clarke1.Beta;
          fe1.PhirUP = DrvWords.Ctrl.bit.DriveEnb && (DrvWords.Stat.bit.vPrUP || GlobalM4.FlagState.bit.PickUpFOC || GlobalM4.FlagState.bit.PickupFreqFound); // Либо возбуждение, либо Подхват
          fe1.GenMode = DrvWords.Stat.bit.GeneratorMode;
          fe1.calc(&fe1);
          IMDs1 = fe1.FluxDrE;
          
          se1.IDsS = clarke1.Alpha;
          se1.IQsS = clarke1.Beta;
          se1.PsiDrS = fe1.PsiDrS;
          se1.PsiQrS = fe1.PsiQrS;
          se1.ThetaFlux = fe1.ThetaFlux;
          se1.calc(&se1);
          
          We1 = se1.WPsi;          
          SpeedWr = se1.WrHat;
          
          // Регулятор скорости отключен - он в Подхвате не используется
          // Дополнительный фильтр на скорость - его также надо запустить до старта РС
          if (PickupState != InitStartPU && DrvWords.Ctrl.bit.FiltrRefSpdEnb) {
            output = outputn_1 + (dPnt->Wrz * RefFiltr_K41);
            dPnt->NewRefFiltrOut = output;
            
            // Если фильтр включен - берем задание прямо из АСУ канала
            output = (dPnt->Wrz * RefFiltr_K11) + (outputn_1 * RefFiltr_K21);
            outputn_1 = output;
          } else {
            dPnt->NewRefFiltrOut = output = outputn_1 = 0;
          }          
          //pi_spd.Ref_filtr = dPnt->NewRefFiltrOut;
          // Провести фильтрацию коэф РС, чтобы РС был готов принять управление на лету
          pi_spd.Kp = f32_Filtr(pi_spd.Kp, GlobalM4.KpSpd2, GlobalM4.Filter_KprRS);
          pi_spd.Ki = f32_Filtr(pi_spd.Ki, GlobalM4.KiSpd2, GlobalM4.Filter_KintRS);
                
          pi_spd.Ref_filtr = pi_spd.Ref = pi_spd.Fdb = SpeedWr;          
          pi_spd.Ui = 0;

          // Регулятор противоЭДС
          pi_emf.OutMax = pi_emf_const1.OutMax;
          pi_emf.OutMin = -pi_emf.OutMax;         
          if(Udcveryslow > _SQRT_2)     pi_emf.Ref = dPnt->EmfRef_iq * _SQRT_2; // Если Ud выше нормы - ставим нормальное значение Ud (sqrt(2))
          else                          pi_emf.Ref = dPnt->EmfRef_iq * Udcveryslow; // Если ниже нормы - учитываем его в расчете          
          pi_emf.Fdb = fabsf(IMDs1 * se1.WPsi * dPnt->EmfFdb_K); //EmfFdb;
          pi_emf.calc(&pi_emf);
          pi_emf.Out = pi_emf.Out / dPnt->Wnom;     // Приводим задание возбуждения через номинальную частоту двигателя
                    
          // Регулятор активного тока имеет заданием 0
          pi_isq.Ref = 0;
          pi_isq.Fdb = park1.Qs;
          pi_isq.OutMax = dPnt->UmaxIqId;
          pi_isq.OutMin = -pi_isq.OutMax;
          pi_isq.calc(&pi_isq);
          // Регулятор реактивного тока работает с задаваемым значением возбуждения по алгоритму
          pi_isd.Ref = PickupIsd;
          pi_isd.Fdb = park1.Ds;
          pi_isd.OutMax = dPnt->UmaxIqId;
          pi_isd.OutMin = -pi_isd.OutMax;
          pi_isd.Kp = GlobalM4.CurRegMultiple * pi_is_const1.Kp; // Ускорение РТ
          pi_isd.Ki = /*GlobalM4.CurRegMultiple **/ pi_is_const1.Ki;
          pi_isd.calc(&pi_isd);

          cross1.Isd = pi_isd.Ref; // park1.Ds
          cross1.Isq = pi_isq.Ref; // park1.Qs
          cross1.We = We1;
          cross1.Wr = SpeedWr;
          cross1.IRm = IMDs1;
          cross1.Urd = pi_isd.Out;
          cross1.Urq = pi_isq.Out;
          cross1.Usmax = dPnt->UmaxIqId;
          cross1.calc(&cross1);
            
          ipark1.Ds = cross1.Usd;
          ipark1.Qs = cross1.Usq;
          ipark1.Angle = fe1.ThetaFlux;
          ipark1.calc(&ipark1);
          
          svgen_dqv.Ualpha = (ipark1.Alpha * _SQRT_3) / dPnt->Udc;
          svgen_dqv.Ubeta  = (ipark1.Beta * _SQRT_3) / dPnt->Udc;
          svgen_dqv.calc(&svgen_dqv);
          
          dtc1.Iu = dPnt->IsU;
          dtc1.Iv = dPnt->IsV;
          dtc1.Iw = dPnt->IsW;
          dtc1.Ta = svgen_dqv.Ta;
          dtc1.Tb = svgen_dqv.Tb;
          dtc1.Tc = svgen_dqv.Tc;
          dtc1.Ud = UdcFiltr.UdcOut; //dPnt->Udc;
          dtc1.Fpwm = dPnt->Fpwm;
          dtc1.We = se1.WPsi;
          dtc1.calc(&dtc1);            
          
          Pwm.Ta = dtc1.Ta;
          Pwm.Tb = dtc1.Tb;
          Pwm.Tc = dtc1.Tc;
          Pwm.Update(&Pwm);          
          
          // ------------------------------------------------------------------------------
          //    Connect inputs of CALC_VAR module and call the
          //    calculation variable function.
          // ------------------------------------------------------------------------------
          if(DrvWords.Ctrl.bit.DriveEnb){
            dPnt->Wr = f32_Filtr(dPnt->Wr, SpeedWr, 0.0078125f);   // вывод скорости ротора
            dPnt->We = f32_Filtr(dPnt->We, We1, 0.00390625);       // вывод скорости поля
          } else {
            dPnt->Wr = 0;
            dPnt->We = 0;
            SpeedWr = 0;
          }
          
          dPnt->Angle =  atan2f(ipark1.Beta, ipark1.Alpha);
          
          if(DrvWords.Ctrl.bit.DriveEnb){
            dPnt->Us = f32_Filtr(dPnt->Us, (cross1.Us * _SQRT_3_DIV_2), 0.03125f);
          }
          else{
            dPnt->Us = 0.0f;
          }
          
          // вывод флага периода
          if(se1.Period) {
            dPnt->Period = 1;
            se1.Period = 0;
          }                              
          break;
            
	
        default:
        break;
		
	}
	//===конец свича выбора режимов

 }
