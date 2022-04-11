/* ==============================================================================
System Name:    PMSM_Vector_Control (IQ version)

File Name:      PMSM_Vector_Control.C

Description:    Primary system header file for the Sensored Indirect and Sensorless
                Field Orientation Control for a Three Phase Permanent-Magnet Synchronous Motor. 
 
Developer:      Sikalov Y.
 
Originator:     Triol (based on Texas Instruments library)

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-12-2011 Version 1.00: Develop begin
=================================================================================  */

#include "Global_Include.h"
#include "type.h"
#include "LIB_DMC.h"
#include "PMSM_Vector_Control.h"
#include "FOC_params_copy.h"
#include "Pjk_const.h"
#include "DrvPWM.h"
#include "CommonMicro.h" 
#include "Filter.h"


#include "pmsm_cross_rel_comp.h"
//#include "pmsm_bemf_mod.h"
//#include "pmsm_smopos.h"
//
#include "pmsm_cross_rel_comp_const.h"
#include "pmsm_pi_reg_cur_const.h"
#include "pmsm_pi_reg_spd_const.h"
//#include "pmsm_bemf_mod_const.h"
//#include "pmsm_smopos_const.h"
#include "pmsm_antirollback.h"
//#include "pmsm_torq_iq.h"
#include "loadContyChk.h"
#include "minCurrCtrl.h"
#include "anti_rollback.h"          // Антиоткатная функция


typedef struct {
    u16 SpdCalc      : 1;
} PMSMFLAG;

PMSMFLAG pmsm_flag;

u16 pmsmIsrTicker = 0;

extern ANTIROLLBACK    arollback1;
extern PWMGEN Pwm;                                          // Обект модуля ШИМ 

// Обьявление вычислителя коэффициентов 
PMSMPIREGCURCONST pmsm_pi_is_const1  = PMSMPIREGCURCONST_DEFAULTS; //регулятора тока
PMSMPIREGSPDCONST pmsm_pi_spd_const1 = PMSMPIREGSPDCONST_DEFAULTS; // регулятора скорости

// Обьявление обьекта компесации перекрестных связей
PMSMCROSSRELCOMP pmsm_cross1 = PMSMCROSSRELCOMP_DEFAULTS;
// Обьявление вычислителя коэффициентов перекрестных связей
PMSMCROSSRELCOMPCONST pmsm_cross_const1 = PMSMCROSSRELCOMPCONST_DEFAULTS;
//
//// Обьявление обьекта модели противо ЭДС (наблюдатель БДВУ)
//PMSMBEMFMOD pmsm_bemf_mod1 = PMSMBEMFMOD_DEFAULTS;
//// Обьявление вычислителя коэффициентов для модели противо ЭДС (наблюдатель БДВУ)
//PMSMBEMFMOD_CONST pmsm_bemf_mod_const1 = PMSMBEMFMOD_CONST_DEFAULTS;
//
////SMOPOS pmsm_smopos1 = SMOPOS_DEFAULTS;
////SMOPOS_CONST pmsm_smopos_const1 = SMOPOS_CONST_DEFAULTS;
//
//// Обьявление вычислителя коэффициентов ограничителя скорости ротора
//PMSMSPEEDLIMCONST pmsm_speed_lim_const1 = PMSMSPEEDLIMCONST_DEFAULTS;
//
//PMSMTORQIQ pmsmTorqIq = PMSMTORQIQ_DEFAULTS;
//_iq	pmsmSpeedStrg[16] = {0};
//
#define SPEED_FCUT 100.0 // частота среза фильтра скорости


/*********************************************************************************************/
/************************************* PMSM_Vector_Init **************************************/
/*********************************************************************************************/

void PMSM_Vector_Init()
{
    f32 NumTdly = 5.0f; 
    f32 dtc1_STF_fc;
        
    if (!DrvWords.Ctrl.bit.SysCalc) return;
    
    if (!(DrvConst.DriveType == TYPE_PMSM_FOC_SENSORED ||
        DrvConst.DriveType == TYPE_PMSM_FOC_SENSORLESS )) return;
    
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
    dtc1.ThetaCorrCoeff = (_2PI * dPnt->FreqBASE * (1.0f + 1.0f + 1.0f - 0.5f) * dPnt->Tpwm); // корректор фазы тока
    dtc1.dtcMode = dPnt->DeadTimeInit.ModeDtc;   
    
    // Initialize the PMSM_cross_rel_comp_CONST module
    pmsm_cross_const1.Lsd  = dPnt->Lsd; 
    pmsm_cross_const1.Lsq  = dPnt->Lsq; 
    pmsm_cross_const1.PsiR = dPnt->PsiR;
    pmsm_cross_const1.fb   = dPnt->FreqBASE; 
    pmsm_cross_const1.Ib   = dPnt->CurrBASE; 
    pmsm_cross_const1.Ub   = dPnt->VoltBASE;
    pmsm_cross_const1.calc(&pmsm_cross_const1);

    // Initialize the Torque Compensation module
    pmsmTORQcomp.Lsd  = dPnt->Lsd;
    pmsmTORQcomp.Lsq  = dPnt->Lsq;
    pmsmTORQcomp.PsiR = dPnt->PsiR;
    pmsmTORQcomp.Ib   = dPnt->CurrBASE;
    pmsmTORQcomp.init(&pmsmTORQcomp);
   
    // Initialize the Current Regulator constant
    pmsm_pi_is_const1.Rs      = dPnt->Rs;
    pmsm_pi_is_const1.Lsd     = dPnt->Lsd;
    pmsm_pi_is_const1.Lsq     = dPnt->Lsq;
    pmsm_pi_is_const1.Kp_manual = DrvConst.KpCur;
    pmsm_pi_is_const1.Ki_manual = DrvConst.KiCur;

    // Корректировка влияния задержки, при изменении частоты ШИМ
    // (уменьшение влияния шума)
    pmsm_pi_is_const1.NumTdly = NumTdly;
    pmsm_pi_is_const1.Tdrv    = 0;
    pmsm_pi_is_const1.Tpwm    = dPnt->Tpwm;
    pmsm_pi_is_const1.Ub      = dPnt->VoltBASE;
    pmsm_pi_is_const1.Ib      = dPnt->CurrBASE;
    pmsm_pi_is_const1.ManualEnb = DrvWords.Ctrl.bit.ManualCurReg;
    pmsm_pi_is_const1.calc(&pmsm_pi_is_const1);

    dPnt->KpCur = pmsm_pi_is_const1.Kp_manual;
    dPnt->KiCur = pmsm_pi_is_const1.Ki_manual;

// Initialize the PI constant
    pmsm_pi_spd_const1.Jr     = dPnt->Jr;
    pmsm_pi_spd_const1.Zp     = dPnt->Zp;
    pmsm_pi_spd_const1.PsiR   = dPnt->PsiR;
    pmsm_pi_spd_const1.Tcl    = pmsm_pi_is_const1.Tcl;
    pmsm_pi_spd_const1.Tsl    = 2 * (pmsm_pi_is_const1.Tcl + dPnt->TcutSpd);
    pmsm_pi_spd_const1.Tspd   = dPnt->Tpwm * dPnt->DivRegSpd;
    pmsm_pi_spd_const1.Imax   = dPnt->Imax_f32;
    pmsm_pi_spd_const1.Fc_spd = SPEED_FCUT; // Гц (частота среза фильтра скорости)
    pmsm_pi_spd_const1.fb     = dPnt->FreqBASE;
    pmsm_pi_spd_const1.Ib     = dPnt->CurrBASE;
    pmsm_pi_spd_const1.ManualEnb = DrvWords.Ctrl.bit.ManualSpdReg;
    pmsm_pi_spd_const1.Kp_manual = dPnt->KpSpd;
    pmsm_pi_spd_const1.Ki_manual = dPnt->KiSpd;
    pmsm_pi_spd_const1.Kp_forced = dPnt->KpForced;
    pmsm_pi_spd_const1.Ki_forced = dPnt->KiForced;
    pmsm_pi_spd_const1.calc(&pmsm_pi_spd_const1);

    dPnt->KpSpd = pmsm_pi_spd_const1.Kp_manual;
    dPnt->KiSpd = pmsm_pi_spd_const1.Ki_manual;


    // Initialize the PMSM_cross_rel_comp module
    pmsm_cross1.K1 = (pmsm_cross_const1.K1); 
    pmsm_cross1.K2 = (pmsm_cross_const1.K2); 
    pmsm_cross1.K3 = (pmsm_cross_const1.K3);
    
    //// Initialize the PI module for Id
    dPnt->UmaxIqId = 1.0f; // !???!!!
    // Initialize the PI module for Id
    pi_isd.Kp = pmsm_pi_is_const1.Kp;
    pi_isd.Ki = pmsm_pi_is_const1.Ki;
    pi_isd.K1 = pmsm_pi_is_const1.K1;
    pi_isd.K2 = pmsm_pi_is_const1.K2;
    pi_isd.K3 = pmsm_pi_is_const1.K3;
    pi_isd.OutMax = dPnt->UmaxIqId;
    pi_isd.OutMin = -pi_isd.OutMax;
    
    // Initialize the PI module for Iq
    pi_isq.Kp = pmsm_pi_is_const1.Kp;
    pi_isq.Ki = pmsm_pi_is_const1.Ki;
    pi_isq.K1 = pmsm_pi_is_const1.K1;
    pi_isq.K2 = pmsm_pi_is_const1.K2;
    pi_isq.K3 = pmsm_pi_is_const1.K3;
    pi_isq.OutMax = dPnt->UmaxIqId;
    pi_isq.OutMin = -pi_isq.OutMax;            
    
    // Initialize the PI module for speed
    pi_spd.Kp = pi_spd.Kp_norm      = pmsm_pi_spd_const1.Kp;
    pi_spd.Ki = pi_spd.Ki_norm      = pmsm_pi_spd_const1.Ki;
    pi_spd.Kp_forced                = pmsm_pi_spd_const1.Kp_forced;
    pi_spd.Ki_forced                = pmsm_pi_spd_const1.Ki_forced;        
    pi_spd.K1 = pmsm_pi_spd_const1.K1;
    pi_spd.K2 = pmsm_pi_spd_const1.K2;
    pi_spd.K3 = pmsm_pi_spd_const1.K3;
    pi_spd.K4 = pmsm_pi_spd_const1.K4;
    pi_spd.K5 = pmsm_pi_spd_const1.K5;
    pi_spd.K6 = pmsm_pi_spd_const1.K6;
    pi_spd.K7 = pmsm_pi_spd_const1.K7;
    pi_spd.OutMax = pmsm_pi_spd_const1.OutMax;
    pi_spd.OutMin = -pi_spd.OutMax;
    //pi_spd.Imax_Ampl   = pi_spd_const1.OutMax;
    pi_spd.Tpwm      = dPnt->Tpwm;
    pi_spd.RefFiltrEnb = DrvWords.Ctrl.bit.FiltrRefSpdEnb;
    pi_spd.NewRegEnb   = 1;
    
    // Фильтр ЗПТ
    UdcFiltr.reset(&UdcFiltr);
    UdcFiltr.K1udcFilt = dPnt->TudcFilt / (dPnt->TudcFilt + dPnt->Tpwm);
    UdcFiltr.K2udcFilt = dPnt->Tpwm / (dPnt->TudcFilt + dPnt->Tpwm);         
}

/*********************************************************************************************/
/************************************* PMSM_Vector_Calc **************************************/
/*********************************************************************************************/
void PMSM_Vector_Calc()
{
    f32 Tmp1, TorqueMax;
    f32 TorqueMaxRefLocal;
    static f32 TorqueIqK = 0;
    static f32 Theta1 = 0, ThetaPeriod = 0;
//    static u32 Timer = 0;
    static f32 SpeedWr = 0;
    
    pmsm_flag.SpdCalc = 0;
    pmsmIsrTicker++;

    if (!(DrvConst.DriveType == TYPE_PMSM_FOC_SENSORED ||
        DrvConst.DriveType == TYPE_PMSM_FOC_SENSORLESS )) return;
    
    if (!DrvWords.Ctrl.bit.DriveEnb) {
        pi_spd.ResetUiPreviuos = GlobalM4.Pretorque.mPretorqueReset;    
        pi_isd.reset(&pi_isd);
        pi_isq.reset(&pi_isq);
        pi_spd.reset(&pi_spd);

        pmsmIsrTicker = dPnt->DivRegSpd;
        
        TorqueIqK = 1.5f * dPnt->Zp * dPnt->PhiR_nom;
//
//        // Если включен режим антиотката - загружаем в начале работы исходные значения коэф.
//        // в переменные фильтрации
//		if (arollback1.flag.ARollbackEnb) {
//			pi_spd.KpFiltr = pi_spd.koef->Kp_forced;
//			pi_spd.KiFiltr = pi_spd.koef->Ki_forced;
//		} else { // Если Антиотката нету - загружаем обычные коэф.
//			pi_spd.KpFiltr = pi_spd.koef->Kp;
//			pi_spd.KiFiltr = pi_spd.koef->Ki;
//		}
//		PiSpdReloadUi = 1; // Загрузить флаг предзагрузки интеграла КС
//		PiSpdReloadMaxCnt = (GlobalM3.timeUI_Up/1000.0) / dPnt->Tpwm; // Получаем время в тиках ШИМа
//		PiSpdReloadUiStep = _IQmpy(pi_spd.Ui_prev, _IQ(1.0/PiSpdReloadMaxCnt)); // Ступень приращения
////		pi_spd.Ui_prev = _IQ(1.0);
//		pi_spd.Ui2 = 0;
    }


    if(!DrvWords.Ctrl.bit.vCodeEnb) return; // выключение обработчика ВУ внешне
    
    if (pmsmIsrTicker == dPnt->DivRegSpd) {
        pmsm_flag.SpdCalc = 1;
        pmsmIsrTicker = 0;
    }

    
// ------------------------------------------------------------------------------
//    Фильтр (ФНЧ 1-го порядка) напряжения ЗПТ
// ------------------------------------------------------------------------------
        UdcFiltr.UdcIn = dPnt->Udc;
        UdcFiltr.calc(&UdcFiltr);        
        //UdcFiltr.UdcOut
        dPnt->UmaxIqId = UdcFiltr.UdcOut * 0.816496580927726; 

        dtc1.dtcMode = dPnt->DeadTimeInit.ModeDtc;           
        
        // Задаем в регулятор скорости его максимальный выходной ток который допустим в данный момент времени
        pi_spd.Imax = 1.414f*GlobalM4.ImaxVFD; //Приводим его к амплитудному значению
        
//	Udcveryslow = iq_Filtr(Udcveryslow, GlobalC28.UdcFast, 10);
//    dPnt->UmaxIqId = _IQmpy(Udcveryslow/*dPnt->Udc*/, _IQ(0.816496580927726)); //0.999969482421875 = (2^15-1)/2^15
//
//================================================================================================
    switch (DrvConst.DriveType) {
    case TYPE_PMSM_FOC_SENSORED: //вектор датчиковый

// ------------------------------------------------------------------------------
//    Call the ENCODER calculation function.
// ------------------------------------------------------------------------------
      if (GlobalM4.EncoderParam.EncoBlockPresent) {
        SpeedWr = GlobalM4.EncoderCalcParam.electricSpd; // Get speed from external Encoder module
        Theta1 = GlobalM4.EncoderCalcParam.electricTheta * _2PI; // Get theta from external Encoder module
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
        pi_spd.Kp = f32_Filtr(pi_spd.Kp, pi_spd.Kp_norm, GlobalM4.Filter_KprRS);
        pi_spd.Ki = f32_Filtr(pi_spd.Ki, pi_spd.Ki_norm, GlobalM4.Filter_KintRS);
      }                 
      
      if(pmsm_flag.SpdCalc){ // прореживание периодичности выполнения
        //Задание на момент
        TorqueMax = GlobalM4.ImaxVFD;
        TorqueMaxRefLocal = fabsf(GlobalM4.TorqueMax);        
        TorqueMax = fabsf(TorqueMaxRefLocal) / (TorqueIqK); // M = (3/2 * Zp * Psinom) * Iq
                        
        pi_spd.SavePrevUiFlag = GlobalM4.FlagCommand.bit.BreakOff; // Для Лифта - если тормоз отпущен, фиксируем текущее значение интеграла, для последующего восстановления (внутри регулятора за)
        pi_spd.RestorePrevUiFlag = GlobalM4.Pretorque.mPretorqueSet; // Флаг начала восстановления интеграла с последнего останова
        pi_spd.RestorePrevUiTime = GlobalM4.Pretorque.mPretorqueTime; // Время восстановления интеграла с последнего останова
        pi_spd.OutMax = f32_Filtr(pi_spd.OutMax, TorqueMax, 0.005f); // TorqueMax;  // ограничение момента в рег по скорости
        pi_spd.OutMin = -pi_spd.OutMax; // Нижний предел прийдет со знаком минус
        
        if(DrvWords.Ctrl.bit.FiltrRefSpdEnb) {
          pi_spd.Ref = dPnt->NewRefFiltrOut; // Задание скорости фильтрованное
        } else {
          pi_spd.Ref = dPnt->Wrz;
        } 
        // ФИЛЬТР ОТКЛЮЧЕН - ОН ПОКА НЕ РАБОТАЕТ НОРМАЛЬНО
        
        pi_spd.Ref = dPnt->Wrz;
        pi_spd.Fdb = SpeedWr;
        pi_spd.FreezeUi = 0;//pmsm_cross1.UsmaxFlag;
        pi_spd.calc(&pi_spd);        
      }
      
      // !!!! Вычитку регулятора скорости надо делать вне условия pmsm_flag.SpdCalc, иначе в прореживании будет глюк
      Tmp1 = GlobalM4.piSpdOut = pi_spd.Out; // Передаем в Глобал значение выхода регулятора - Используем для алгоритма Эвакуации
     
// ------------------------------------------------------------------------------
//    Connect inputs of the LOADCONTYCHK module (модуль проверки целостности нагрузки)
// ------------------------------------------------------------------------------
      loadContyChk.bitStart	= GlobalM4.FlagCommand.bit.RingUp;
      loadContyChk.IdRefIn	= 0.0f; // Для PMSM задаем реактивный ток нулевым
      loadContyChk.IqRefIn	= Tmp1; 
      loadContyChk.Iu 		= fabsf(dPnt->IsU);
      loadContyChk.Iv 		= fabsf(dPnt->IsV);
      loadContyChk.Iw 		= fabsf(dPnt->IsW);
      //loadContyChk.ThetaIn 	= Theta1;
      loadContyChk.Ichk		= (0.1 *dPnt->Istn);		// пока константой 10% от In
      loadContyChk.Imin		= (0.01*dPnt->Istn);		// пока константой  1% от In
      loadContyChk.Fpwm           = dPnt->Fpwm;
      loadContyChk.timerTestTime = (GlobalM4.timeLoadContyCheck * 0.5f); // Максимальное время провекри в тиках ШИМа
      loadContyChk.calc(&loadContyChk);
      dPnt->RingUpError = loadContyChk.Error;            
     
      // ------------------------------------------------------------------------------
      //    Формирование минимального полного тока, для обнаружения обрыва нагрузки
      // ------------------------------------------------------------------------------
      minCurrCtrl.bitStart = ((!GlobalM4.FlagCommand.bit.RingUp)&&(GlobalM4.FlagCommand.bit.PwmOn));
      minCurrCtrl.inId	 = loadContyChk.IdRefOut;
      minCurrCtrl.inIq	 = loadContyChk.IqRefOut;
      minCurrCtrl.minI	 = loadContyChk.Ichk;	//!!! пока константой 10% от In
      minCurrCtrl.calc(&minCurrCtrl);
      
      // ------------------------------------------------------------------------------
      //    Модуль антиотката для PMSM, с ограничением результирующего значения реактивного тока
      //	  для предотвращения срабатывания МТЗ.
      // ------------------------------------------------------------------------------
      pmsmAntirollBack.inId 	= minCurrCtrl.outId; // Передаем исходные значение токов управления
      pmsmAntirollBack.inIq 	= minCurrCtrl.outIq; // Передаем исходные значение токов управления
      pmsmAntirollBack.fixId 	= GlobalM4.pmsmAntirollback.UnbreakTorqPMSM * dPnt->Isn; // Задание тока на удержание по вектору Id
      pmsmAntirollBack.inSpeedWr= SpeedWr; // Передаем фактическую частоту вращения
      pmsmAntirollBack.inSpeedWrz = pi_spd.Ref; // Передаем частоту задания
      pmsmAntirollBack.inTheta 	= Theta1; // Передаем фактический угол - возможно нужен будет для фазировки энкодера
      pmsmAntirollBack.bitPhasing = GlobalM4.pmsmAntirollback.EnEncoPhasing; // Сигнал фазирования абсолютного энкодера
      pmsmAntirollBack.Fpwm 	= dPnt->Fpwm;	// Частота дискретизации
      pmsmAntirollBack.timeUI_Up= GlobalM4.pmsmAntirollback.timeUI_Up; // Время подъёма/спада тока возбуждения
      pmsmAntirollBack.pwmOn 	= GlobalM4.FlagCommand.bit.PwmOn; //ШИМ включен
      pmsmAntirollBack.BreakOff = GlobalM4.FlagCommand.bit.BreakOff; //ШИМ включен
      pmsmAntirollBack.Mode	= GlobalM4.pmsmAntirollback.pmsmAntirollBackMode; // Режим работы
      pmsmAntirollBack.Enable	= GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable; // Разрешить работу - сигнал должен прийти из командоаппарата Лифта
      pmsmAntirollBack.maxIfull	= (f32)GlobalM4.ImaxVFD; 
      pmsmAntirollBack.FixTimeId = (f32)GlobalM4.pmsmAntirollback.FixTimeId;
      pmsmAntirollBack.timeStepDown = (f32)GlobalM4.pmsmAntirollback.timeStepDown;
      pmsmAntirollBack.FrequencyDown = (f32)GlobalM4.pmsmAntirollback.FrequencyDown;
      
      pmsmAntirollBack.calc(&pmsmAntirollBack); // Находим необходимый ток Id
      Theta1 = pmsmAntirollBack.outTheta; // При фазировке энкодера
      SpeedWr = pmsmAntirollBack.outSpeedWr;
      
      
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
      park1.Beta =  clarke1.Beta;
      park1.Angle = Theta1;
      park1.calc(&park1);

      // ------------------------------------------------------------------------------
      //    REACTIVE TORQUE COMPENSATION module
      // ------------------------------------------------------------------------------
      pmsmTORQcomp.currD = park1.Ds;
      pmsmTORQcomp.inId	= pmsmAntirollBack.outId; //minCurrCtrl.outId; //loadContyChk.IdRefOut; 
      pmsmTORQcomp.inIq	= pmsmAntirollBack.outIq; //minCurrCtrl.outIq; //loadContyChk.IqRefOut; 
      pmsmTORQcomp.calc(&pmsmTORQcomp);
      
      // ------------------------------------------------------------------------------
      //    Connect inputs of the SOFTCURFAULT module and call the soft current fault
      //    calculation function.
      // ------------------------------------------------------------------------------
      soft_cur2.bit.Start_fl = DrvWords.Ctrl.bit.SoftCurDwn;
      soft_cur2.Fpwm         = dPnt->Fpwm;
      soft_cur2.ThetaPsi_in  = Theta1;
      soft_cur2.IdRef_in     = pmsmTORQcomp.outId;
      soft_cur2.IqRef_in     = pmsmTORQcomp.outIq;
      soft_cur2.TimeCurDown  = GlobalM4.SoftCurrentTime; 
      soft_cur2.calc(&soft_cur2);
      
      // ------------------------------------------------------------------------------
      //    Connect inputs of the PID module and call the PID IQ controller
      //    calculation function.
      // ------------------------------------------------------------------------------  
      pi_isq.Ref    = soft_cur2.IqRef_out;
      pi_isq.Fdb    = park1.Qs;
      pi_isq.OutMax =  dPnt->UmaxIqId;
      pi_isq.OutMin = -pi_isq.OutMax;
      pi_isq.FreezeUi = 0;//pmsm_cross1.UsmaxFlag;
      pi_isq.calc(&pi_isq);
      
      // ------------------------------------------------------------------------------
      //    Connect inputs of the PID module and call the PID ID controller
      //    calculation function.
      // ------------------------------------------------------------------------------  
      pi_isd.Ref    = soft_cur2.IdRef_out;
      pi_isd.Fdb    = park1.Ds;
      pi_isd.OutMax = dPnt->UmaxIqId;
      pi_isd.OutMin = -pi_isd.OutMax;
      pi_isd.FreezeUi = 0;//pmsm_cross1.UsmaxFlag;
      pi_isd.calc(&pi_isd);
      
      // ------------------------------------------------------------------------------
      //    Connect inputs of the CROSS RELATION COMPENSATION module and call the CCC 
      //    controller calculation function.
      // ------------------------------------------------------------------------------ 
      pmsm_cross1.Isd    = pi_isd.Ref;
      pmsm_cross1.Isq    = pi_isq.Ref;
      pmsm_cross1.Wr     = SpeedWr;
      pmsm_cross1.Urd    = pi_isd.Out;
      pmsm_cross1.Urq    = pi_isq.Out;
      pmsm_cross1.Usmax  = dPnt->UmaxIqId;
      pmsm_cross1.calc(&pmsm_cross1);
      
      // ------------------------------------------------------------------------------
      //    Connect inputs of the INV_PARK module and call the inverse park transformation
      //    calculation function.
      // ------------------------------------------------------------------------------
      ipark1.Ds     = pmsm_cross1.Usd;
      ipark1.Qs     = pmsm_cross1.Usq;
      ipark1.Angle  = soft_cur2.ThetaPsi_out;
      ipark1.calc(&ipark1);
      
      // ------------------------------------------------------------------------------
      //    Call the Space Vector calculation function.
      // ------------------------------------------------------------------------------
      svgen_dqv.Ualpha = (ipark1.Alpha * _SQRT_3) / dPnt->Udc;
      svgen_dqv.Ubeta  = (ipark1.Beta * _SQRT_3) / dPnt->Udc;
      svgen_dqv.calc(&svgen_dqv);            
      
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
      dtc1.We = SpeedWr;
      dtc1.calc(&dtc1);
      
      // ------------------------------------------------------------------------------
      //    PWM References output.
      // ------------------------------------------------------------------------------     
      Pwm.Ta = dtc1.Ta;
      Pwm.Tb = dtc1.Tb;
      Pwm.Tc = dtc1.Tc;
      Pwm.Update(&Pwm);      
   
      // Формирователь сигнала периода
      dPnt->Period = 0;
      if(fabsf(ThetaPeriod - Theta1) > _PI) {
        dPnt->Period = 1;
      }
      ThetaPeriod = Theta1;      

//
        dPnt->We    = dPnt->Wrz; // вывод скорости
        dPnt->Wr    = SpeedWr;   // вывод скорости
        dPnt->Angle = atan2f(ipark1.Beta, ipark1.Alpha);
        break;

    default: 
        break;      
    }

}

