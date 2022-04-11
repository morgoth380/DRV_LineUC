
/*=====================================================================================
 File name:    DT_COMP.C  (float version)                  
                    
 Originator:   Triol

 Description:  Dead time compensation and compensation voltage drop                  

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 26-08-2011	Version 1.0
 29-09-2020     Version 2.0 PAN
-------------------------------------------------------------------------------------*/

#include "deadtime_comp.h"
#include "Pjk_const.h"
#include "GlobalVar.h"
#include "clarke.h"
#include "ipark.h"
#include "STFilter.h"
#include "iclarke.h"
#include "Filter.h"


CLARKE  DtcClarke  = CLARKE_DEFAULTS;
IPARK   DtcIpark   = IPARK_DEFAULTS;
ICLARKE DtcIclarke = ICLARKE_DEFAULTS;
extern STFILTR DtcSTFilter;               // Блок STFilter для блока dtc


#define F_PWM_MIN       4000.00f   // Минимальная частота коррекции 4 кГц
#define K_DTC           (2.3-1.0) / (8000.0f-F_PWM_MIN)

#define	 NO_COMPENSATION	0
#define  PREVIOUS_ALG		1
#define  NEW_ALG		2
#define  LIFT_CORR_ALG          3

//------------------------------------------------------------------------------
void deadtime_comp_calc_old(register DEADTIME_COMP *v)
{    
    float Tmp;
    // Dead time compensation 
    if (v->Fpwm < F_PWM_MIN) {
        Tmp = v->dT * 1;//0.5f;                           // Tmp = v->dT / 2.0f    
    } else {
        // Тестовый режим отладки Дедтайма  
        Tmp = (1.0 + (v->Fpwm - F_PWM_MIN) * K_DTC);
        Tmp = v->dT * (1.0 + (v->Fpwm - F_PWM_MIN) * K_DTC);//GlobalM4.Debug3;   1.0 -> 4 kHz, 2.3 -> 8 kHz
    }
      
    if (v->Iu >= v->dI)       v->Ta -=  Tmp;
    else if (v->Iu <= -v->dI) v->Ta +=  Tmp;

    if (v->Iv >= v->dI)       v->Tb -=  Tmp;
    else if (v->Iv <= -v->dI) v->Tb +=  Tmp;

    if (v->Iw >= v->dI)       v->Tc -=  Tmp;
    else if (v->Iw <= -v->dI) v->Tc +=  Tmp;

    // 
    if      (v->Ta > 0.999969482421875f)  v->Ta = 0.999969482421875f;
    else if (v->Ta < -1.0f) v->Ta = -1.0f;

    if      (v->Tb > 0.999969482421875f)  v->Tb = 0.999969482421875f;
    else if (v->Tb < -1.0f) v->Tb = -1.0f;

    if      (v->Tc > 0.999969482421875f)  v->Tc = 0.999969482421875f;
    else if (v->Tc < -1.0f) v->Tc = -1.0f;
}
//------------------------------------------------------------------------------
void deadtime_comp_calc(register DEADTIME_COMP *v)
{    
    f32      OneDivUd, UdfDivUdc, UtfDivUdc;
    f32      Tmp, Tmp1, Tmp2;
    f32      Ta, Tb, Tc; 
    // Dead time compensation 
    
    Ta = v->Ta;
    Tb = v->Tb;
    Tc = v->Tc;     
    
    switch (v->dtcMode) {      
    //========================================  
    case NO_COMPENSATION:
      // Ничего не делаем, только проверим на ограничения
      // Просто в конце входные данные проверим на ограничения
      break;
    
    //======================================== 
    case NEW_ALG:      
      OneDivUd = 1.0f / v->Ud;
      
      // приведение падения напряжения к Udc
      UdfDivUdc = (v->Udf * OneDivUd);
      UtfDivUdc = (v->Utf * OneDivUd);
      
      // вычисление компонент ошибки напряжения
      Tmp1 = (v->DtcRelative * (1.0f + UdfDivUdc - UtfDivUdc));
      Tmp2 = ((UdfDivUdc + UtfDivUdc) * 0.5f);
      Tmp  = ((Tmp1 + Tmp2) *2.0f);
      
      // Преобразование Clarke
      DtcClarke.As = v->Iu;
      DtcClarke.Bs = v->Iv;
      DtcClarke.Cs = v->Iw;
      DtcClarke.calc(&DtcClarke);
      
      // STF фильтр измеренного тока
      //----------------------------------------------------------
      DtcSTFilter.AlphaIn = DtcClarke.Alpha;  // Вход фильтра Alfa
      DtcSTFilter.BetaIn  = DtcClarke.Beta;   // Вход фильтра Beta
      DtcSTFilter.We      = v->We;            // Текущий угол
      DtcSTFilter.calc(&DtcSTFilter);
      
      // Обратное преобразование Park
      //----------------------------------------------------------
      DtcIpark.Ds = DtcSTFilter.AlphaOut;
      DtcIpark.Qs = DtcSTFilter.BetaOut;
      DtcIpark.Angle = (v->We * v->ThetaCorrCoeff);
      DtcIpark.calc(&DtcIpark);
      
      // Обратное преобразование Clarke
      //----------------------------------------------------------
      DtcIclarke.Alpha = DtcIpark.Alpha;
      DtcIclarke.Beta  = DtcIpark.Beta;
      DtcIclarke.calc(&DtcIclarke);      
      
      // компенсация дедтайма (знаки "-/+" связаны с тем, что Ta инверсен по отношения к Ualpha)
      if(DtcIclarke.As >= v->dI){
        Ta = Ta - Tmp;
      } else if(DtcIclarke.As <= -v->dI){
        Ta = Ta + Tmp;
      }
      
      if(DtcIclarke.Bs >= v->dI){
        Tb = Tb - Tmp;
      } else if(DtcIclarke.Bs <= -v->dI){
        Tb = Tb + Tmp;
      }
      
      if(DtcIclarke.Cs >= v->dI){
        Tc = Tc - Tmp;
      } else if(DtcIclarke.Cs <= -v->dI){
        Tc = Tc + Tmp;
      }
      
      // Compensation voltage drop (знак "-" связан с тем, что Ta инверсен по отношения к Ualpha)
      v->Ksw = (v->Rsw_2 * OneDivUd);
      Ta = Ta - (v->Ksw * DtcIclarke.As);     // + compensation voltage drop on phase A
      Tb = Tb - (v->Ksw * DtcIclarke.Bs);     // + compensation voltage drop on phase B
      Tc = Tc - (v->Ksw * DtcIclarke.Cs);     // + compensation voltage drop on phase C
        
//#warning Тест нового алгоритма. проверка работы, и реально замена данных на вариант без компенсации
//      Ta = v->Ta;
//      Tb = v->Tb;
//      Tc = v->Tc; 
      
        break;
             
    //========================================
    case PREVIOUS_ALG:
    default:
      // Dead time compensation
      Tmp = v->dT + (v->UdfConst * v->Ud);
      
      if (v->Iu >= v->dI)       Ta -=  Tmp;
      else if (v->Iu <= -v->dI) Ta +=  Tmp;
      
      if (v->Iv >= v->dI)       Tb -=  Tmp;
      else if (v->Iv <= -v->dI) Tb +=  Tmp;
      
      if (v->Iw >= v->dI)       Tc -=  Tmp;
      else if (v->Iw <= -v->dI) Tc +=  Tmp;
      
      // Compensation voltage drop
      v->Ksw = v->Rsw2Const / v->Ud;
      Ta += v->Ksw * v->Iu;        // + compensation voltage drop on phase A
      Tb += v->Ksw * v->Iv;        // + compensation voltage drop on phase B
      Tc += v->Ksw * v->Iw;        // + compensation voltage drop on phase C
      
      break;
      
    //========================================
    // Алгоритм исправленный под лифт. Подобранная компенсация
    case LIFT_CORR_ALG:       
      if (v->Fpwm < F_PWM_MIN) {
        Tmp = v->dT;
      } else {
        // Тестовый режим отладки Дедтайма  
        Tmp = (1.0 + (v->Fpwm - F_PWM_MIN) * K_DTC);
        Tmp = v->dT * (1.0 + (v->Fpwm - F_PWM_MIN) * K_DTC);//   1.0 -> 4 kHz, 2.3 -> 8 kHz
      }
      
      if (v->Iu >= v->dI)       Ta -=  Tmp;
      else if (v->Iu <= -v->dI) Ta +=  Tmp;
      
      if (v->Iv >= v->dI)       Tb -=  Tmp;
      else if (v->Iv <= -v->dI) Tb +=  Tmp;
      
      if (v->Iw >= v->dI)       Tc -=  Tmp;
      else if (v->Iw <= -v->dI) Tc +=  Tmp;
      
      break;      
    } // End of switch (v->dtcMode)
    
    // Проверяем ограничения для выходных значений
//    if      (Ta > 1.0f)  Ta = 1.0f;
//    else if (Ta < -1.0f) Ta = -1.0f;
//    
//    if      (Tb > 1.0f)  Tb = 1.0f;
//    else if (Tb < -1.0f) Tb = -1.0f;
//    
//    if      (Tc > 1.0f)  Tc = 1.0f;
//    else if (Tc < -1.0f) Tc = -1.0f;  

    if      (v->Ta >0.999969482421875f)  v->Ta = 0.999969482421875f;
    else if (v->Ta < -1.0f) v->Ta = -1.0f;
    
    if      (v->Tb > 0.999969482421875f)  v->Tb = 0.999969482421875f;
    else if (v->Tb < -1.0f) v->Tb = -1.0f;
    
    if      (v->Tc > 0.999969482421875f)  v->Tc = 0.999969482421875f;
    else if (v->Tc < -1.0f) v->Tc = -1.0f;    
    // Переносим данные на выход
    v->Ta = Ta;
    v->Tb = Tb;
    v->Tc = Tc;        
}
//------------------------------------------------------------------------------
void deadtime_comp_Lm_calc(DEADTIME_COMP *v)
{ 
    f32 Tmp, Ta, Tb, Tc;
    static f32 Iu_h = 0;
    static f32 Iv_h = 0;
    static f32 Iw_h = 0;
    static f32 Iu_f = 0;
    static f32 Iv_f = 0;
    static f32 Iw_f = 0;
    static s16 flag_u = 0;
    static s16 flag_v = 1;
    static s16 flag_w = 0;

    Ta = v->Ta;
    Tb = v->Tb;
    Tc = v->Tc;         
    
    // Dead time compensation
    Tmp = v->dT + (v->UdfConst * v->Ud);

    Iu_f = f32_Filtr(Iu_f, v->Iu, 0.0009765625f);
    Iv_f = f32_Filtr(Iv_f, v->Iv, 0.0009765625f);
    Iw_f = f32_Filtr(Iw_f, v->Iw, 0.0009765625f);

    /**************************** Phase U ***************************/
    if (flag_u == 0) {
        if ((v->Iu >= - v->dI) && ((Iu_f - Iu_h) > 0)) flag_u = 1;
        Ta +=  Tmp;
    }
    else {
        if ((v->Iu <=  v->dI) && ((Iu_f - Iu_h) < 0))  flag_u = 0;
        Ta -=  Tmp;
    }
    /**************************** Phase U ***************************/

    /**************************** Phase V ***************************/
    if (flag_v == 0) {
        if ((v->Iv >= - v->dI) && ((Iv_f - Iv_h) > 0)) flag_v = 1;
        Tb +=  Tmp;
    }
    else {
        if ((v->Iv <=  v->dI) && ((Iv_f - Iv_h) < 0))  flag_v = 0;
        Tb -=  Tmp;
    }
    /**************************** Phase V ***************************/

    /**************************** Phase W ***************************/
    if (flag_w == 0) {
        if ((v->Iw >= - v->dI) && ((Iw_f - Iw_h) > 0)) flag_w = 1;
        Tc +=  Tmp;
    }
    else {
        if ((v->Iw <=  v->dI) && ((Iw_f - Iw_h) < 0))  flag_w = 0;
        Tc -=  Tmp;
    }
    /**************************** Phase W ***************************/

    Iu_h = Iu_f;
    Iv_h = Iv_f;
    Iw_h = Iw_f;

    // Compensation voltage drop
    v->Ksw = v->Rsw2Const / v->Ud;
    Ta += v->Ksw * v->Iu;        // + compensation voltage drop on phase A
    Tb += v->Ksw * v->Iv;        // + compensation voltage drop on phase B
    Tc += v->Ksw * v->Iw;        // + compensation voltage drop on phase C
    
    // Проверяем ограничения для выходных значений
    if      (Ta > 1.0f)  Ta = 1.0f;
    else if (Ta < -1.0f) Ta = -1.0f;
    
    if      (Tb > 1.0f)  Tb = 1.0f;
    else if (Tb < -1.0f) Tb = -1.0f;
    
    if      (Tc > 1.0f)  Tc = 1.0f;
    else if (Tc < -1.0f) Tc = -1.0f;  
    
    // Переносим данные на выход
    v->Ta = Ta;
    v->Tb = Tb;
    v->Tc = Tc;  
}
//------------------------------------------------------------------------------
void deadtime_comp_Rs_calc(DEADTIME_COMP *v)
{
    f32      Ta, Tb, Tc, Tmp; 
    // Dead time compensation 
    
    Ta = v->Ta;
    Tb = v->Tb;
    Tc = v->Tc;  
  
    Tmp = (v->dT + (v->UdfConst * v->Ud)) * 2.0f;
    //_iq Tmp = _IQmpy(v->dT + _IQmpy(v->Udf,v->Ud) ,_IQ(2));
    ////_iq Tmp = _IQmpy(v->dT + _IQmpy(_IQ(0.7/380.0),v->Ud),_IQ(2));
    
    Ta -= Tmp;
    Tb += Tmp;
    Tc += Tmp;
    
    // Проверяем ограничения для выходных значений
    if      (Ta > 1.0f)  Ta = 1.0f;
    else if (Ta < -1.0f) Ta = -1.0f;
    
    if      (Tb > 1.0f)  Tb = 1.0f;
    else if (Tb < -1.0f) Tb = -1.0f;
    
    if      (Tc > 1.0f)  Tc = 1.0f;
    else if (Tc < -1.0f) Tc = -1.0f;  

    // Переносим данные на выход
    v->Ta = Ta;
    v->Tb = Tb;
    v->Tc = Tc;
}
//------------------------------------------------------------------------------
void deadtime_comp_Ls_PMSM_calc(DEADTIME_COMP *v)
{
}
//------------------------------------------------------------------------------
void deadtime_comp_reset(DEADTIME_COMP *v)
{
	v->Iu             = 0;  // Input: phase-u stator current
	v->Iv             = 0;  // Input: phase-v stator current
	v->Iw             = 0;  // Input: phase-w stator current
	v->Ta             = 0;  // Input: and Output: phaza A compensetion potential
	v->Tb             = 0;  // Input: and Output: phaza B compensetion potential
	v->Tc             = 0;  // Input: and Output: phaza C compensetion potential
	v->Ud             = 0;  // Input: DC bus voltage
	v->We             = 0;  // Input: to fight the sticky current

	v->Rsw_2          = 0;  // Init: двойное сопротивление ключа
	v->Udf            = 0;  // Init: прямое падение напр. на обр. диоде IGBT, В
	v->Utf            = 0;  // Init: прямое падение напр. на IGBT, В
	v->DtcRelative    = 0;  // Init:
	v->ThetaCorrCoeff = 0;  // Init:
	v->dtcMode        = 0;  // Init: Выбор алгоритма компенсации дедтайма
	v->dI             = 0;  // Init: to fight the sticky current
	v->dT             = 0;  // Init: compensation coefficient = 2*DeadTime*(sample frequence) +

	v->Ksw            = 0;  // Var: Coefficient of drop voltage = 2*(key resistance)/(DC bus voltage)
}
//------------------------------------------------------------------------------
void deadtime_comp_init(DEADTIME_COMP *v) {
//        // Initialize the Dead time compensation
//	// Блок STFilter для блока dtc
//	DtcSTFilter.reset(&DtcSTFilter);
//
//	dtc1_STF_fc    = 50.0f; // Гц
//	DtcSTFilter.K1 = (dPnt->FreqBASE * 0.5f * dPnt->Tpwm);           // Коеффициент фильтра 1
//	DtcSTFilter.K2 = (_2PI * dtc1_STF_fc * dPnt->Tpwm);            // Коеффициент фильтра 2
//	DtcSTFilter.K3 = (_2PI * dPnt->FreqBASE * dPnt->Tpwm);         // Коеффициент фильтра 3
//
//    // Initialize the Dead time compensation
//	dtc1.reset(&dtc1);
//
//	dtc1.dT = dPnt->DeadTimeInit.DeadTime * dPnt->Fpwm * 0.000001f;                // МВ в относительных единицах
//	dtc1.Rsw_2 = 2.0f * dPnt->DeadTimeInit.Rsw * dPnt->CurrBASE / dPnt->VoltBASE;  // двойное сопротивление ключа
//	dtc1.dI = FLOAT_to_IQ(dPnt->DeadTimeInit.dI, Current_base);                    // интервал неопределённости около 0 А
//	dtc1.Udf = FLOAT_to_IQ(dPnt->DeadTimeInit.Udf, Volt_base);
//        dtc1.Rsw2Const = 2.0f * 0.03f * dPnt->CurrBASE / dPnt->VoltBASE;        
//	dtc1.UdfConst = 1.0f/dPnt->VoltBASE;
//	dtc1.Utf   = dPnt->DeadTimeInit.Utf / dPnt->VoltBASE;                       // прямое падение напр. на IGBT, В
//	dtc1.DtcRelative = ((dPnt->DeadTimeInit.DeadTime + dPnt->DeadTimeInit.Ton - dPnt->DeadTimeInit.Toff) * dPnt->Fpwm * 1e-6f);
//	// а) 1.0 - задержка задания напр.
//	// б) 1.0 - задержка измерения тока
//	// в) 0.5 - опережение STF фильтра
//	dtc1.ThetaCorrCoeff = (2.0f * PI * dPnt->FreqBASE * (1.0f + 1.0f + 1.0f - 0.5f) * dPnt->Tpwm); // корректор фазы тока
//	dtc1.dtcMode = dPnt->DeadTimeInit.ModeDtc;  
}
//------------------------------------------------------------------------------