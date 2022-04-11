#ifndef measure_H
#define measure_H

#include "type.h"
#include "Global_include.h"
//#include "DSP2833x_Device.h"
//********************************************************
#define ZERO_Imax 2077
//********************************************************
//отладочная распечатка
#define	CodeAdc_Ia_Ib_Ic		0
#define	Iu_mgn_Iv_mgn_Iw_mgn	1
#define	Iact_Ireact_Ifull		3
#define	Iarf_vis             	4
#define	Iuvw_kor             	5
#define	Iuvw_vis             	6
#define	Iuvw_Fvis	        	7
#define	Uabc_Fzer	        	8
#define	Uabc_zer	        	9
#define	Uabc_sign	        	10
#define	Uabbcca_sign	    	11
#define	Uabbcca_deystv	    	12
#define	IfullIactIact_in		13
#define	PactPfullCosFi_In		14
#define	Iabc_sign	        	15
#define	Iabc_deystv	        	16

#define	Uabc_Kv	            	17
#define	Code_Angle              18
#define	As_Bs_Angle             19

#define	UaIa_sign               20
#define	UbIb_sign               21
#define	UcIc_sign               22
#define	Angle_iq30_Angle		23

#define	M1_cosU	                24
#define	M2_cosV	                25
#define	M3_cosW	                26

#define	U1_cosW	                27
#define	U2_cosW	                28
#define	U3_cosW	                29

#define	FiltrUF	            	30

//********************************************************
///////////константы синтетической синусоиды
/////////#define	PERIOD	    	0x6000000	            //Период 20mS представляется как число
///////////минимальная частота  Fmin = 48Hz; T=20,833mS;	375 тиков по 55,5uS
///////////максимальная частота Fmax = 52Hz; T=19,231mS;	346 тиков по 55,5uS
/////////#define	FREQ_MIN	    ((s64)(PERIOD/375))        //1048
/////////#define	FREQ_MAX	    ((s64)(PERIOD/346))        //1136
/////////
/////////#define	FREQ_MIN_X         ((s64)(FREQ_MIN<<32))
/////////#define	FREQ_MAX_X         ((s64)(FREQ_MAX<<32))
//********************************************************
typedef struct {
            	s32	            	Sum	            ;
            	s16	            	tek	            ;
            	u16	            	cnt	            ;
            }filtrSum_type;
//********************************************************
typedef struct {
            	f32             	Sum	            ;
            	s16	            	tek	            ;
            	u16	            	cnt	            ;
            }f32_filtrSum_type;
//********************************************************
typedef struct {
            	s16	            	tek	            ;
            	s16	            	minv	        ;
            	s16	            	maxv	        ;
            }filtrMinMax_type;
//********************************************************
typedef struct {
            	s16	            	Offset	        ;
            	s32	            	Gain	        ;
            }ustir_type;
//********************************************************
 typedef struct{                                        //Структура ФАПЧ для одного сигнала
             	f32	            	ErrTeta	        ;    //ошибка по фазе текущая
             	f32	            	ErrTetaCopy	    ;    //ошибка по фазе за период
             	f32	            	Pi_Sum	        ;    //интеграл ПИ регулятора
             	f32                 Pi2Prd          ;   // 2Pi/период таймера
//                f32                 SifuConst       ;   // Константа для вычисления сектора в SIFU
	volatile struct ECAP_REGS       *PntTmr         ;   //Указатель на структуру таймера
                 }sint_type;
//**************************************************************************
typedef struct {
            	s16	            	ZerMin	        ;
            	s16	            	ZerMax	        ;
            	filtr_type	    	Zer_aper	    ;
            	s64             	sqrSum	        ;
            	s64             	sqrSumCopy	    ;
            	_iq             	ActiveVal	    ;
                 }Var_type;
//**************************************************************************
#define FastFailMask_NONE           0
#define FastFailMask_SwU            1       // Ошибка по сигналу от силового ключа U
#define FastFailMask_SwV            2       // Ошибка по сигналу от силового ключа V
#define FastFailMask_SwW            3       // Ошибка по сигналу от силового ключа W
#define FastFailMask_SwT            4       // Ошибка по сигналу от силового тормозного ключа
#define FastFailMask_MtzD           5       // МТЗ по дискретному сигналу от компаратора
#define FastFailMask_MtzA           6       // МТЗ программное по уставке МТЗ
#define FastFailMask_R7             7       // резерв
#define FastFailMask_UdMin          8       // Низкое Ud
#define FastFailMask_UdMax          9       // Высокое Ud
#define FastFailMask_UdCharge       10      // 
#define FastFailMask_Ud_KZ          11      // 
#define FastFailMask_Tu             12      // Температура ключей
#define FastFailMask_r13            13      // резерв
#define FastFailMask_r14            14      // резерв
#define FastFailMask_r15            15      // резерв

#define SlowFailMask_Contactor      34      // ошибка контактора
/*              // Заглушено ПЕРЕПИСАТЬ под новый алгоритм формирования номера аварии !!!

#define SlowFailMask_Tu             16      // перегрев силовых ключей
#define SlowFailMask_r18            18
#define SlowFailMask_r19            19


#define SlowFailMask_ObrFazaA       20      // Обрыв фазы A
#define SlowFailMask_ObrFazaB       21      // Обрыв фазы B
#define SlowFailMask_ObrFazaC       22      // Обрыв фазы C
#define SlowFailMask_AVR_STOP       23      // Кнопка аварийный стоп
#define SlowFailMask_ZarTime        24      // Отсутствие заряда за 20 сек после подачи команды на заряд моста Ud
#define SlowFailMask_MBConnect      25      // авария "нет связи с пультом"

#define SlowFailMask_Imax           27      // авария "Перегруз"

                ///  Аварии только в векторном режиме управления ----------vvvvvvvvvvvvvvvvvvvv

#define SlowFailMask_TmLvl1         26      // "флаг 1-го уровнЯ срабатываниЯ времЯ-токовой защиты двигателЯ"
#define SlowFailMask_TmLvl2         27      // "флаг 2-го уровнЯ срабатываниЯ времЯ-токовой защиты двигателЯ"
#define SlowFailMask_RbErr          28      // "флаг времЯ токовой защиты балластного резистора"
#define SlowFailMask_MaxSpErr       29      // Авария "Максимальная скорость по датчику скорости"
#define SlowFailMask_CSErr          30      // Авария датчиков тока статора
*/

#define NUMFirstFailMicro           32      // 2 слова для номеров быстрых аварий


typedef union{
	struct MASK_FAST_FAIL_DRV{
    	u16		None	    : 1;    // 0 
    	u16		SwU	    : 1;    // 1  Авария силовой ключ фаза U
    	u16		SwV	    : 1;    // 2  Авария силовой ключ фаза V
    	u16		SwW	    : 1;    // 3  Авария силовой ключ фаза W
    	u16		SwT	    : 1;    // 4  Авария силовой ключ тормрзной
    	u16		MtzD	    : 1;    // 5  Авария по МТЗ по дискретным входам
    	u16		MtzA         : 1;    // 6  Авария по МТЗ по аналоговому измерителю
    	u16		r7          : 1;    // 7  
    	u16		UdMin	    : 1;    // 8  Авария по Ud минимум
    	u16		UdMax	    : 1;    // 9  Авария по Ud максимум
    	u16		UdCharge	: 1;    // 10 Авария зарядное время Ud
    	u16		Ud_KZ	    : 1;    // 11 Авария по КЗ в звене Ud
    	u16		Tu	    : 1;    // 12 Авария по температуре ключей
    	u16		r13	    : 1;    // 13
    	u16		r14	    : 1;    // 14
    	u16		r15	    : 1;    // 15
    };
	u16		all;
}FastFailMask_type;
/*              // Заглушено ПЕРЕПИСАТЬ под новый алгоритм формирования номера аварии !!!
typedef union{
	struct MASK_SLOW_FAIL_DRV{
    	u16		Tu	        : 1;    // 16 Авария Tu фаза U
    	u16		Tv	        : 1;    // 17 Авария Tv фаза V
    	u16		Tw	        : 1;    // 18 Авария Tw фаза W
    	u16             Td                      : 1;    // 19
    	u16		ObrFazaA                : 1;    // 20
    	u16		ObrFazaB                : 1;    // 21
    	u16		ObrFazaC                : 1;    // 22
    	u16		keyStop                 : 1;    // 23
    	u16             ZarTime                 : 1;    // 24
    	u16             MBConnect               : 1;    // 25
    	u16             r1                      : 1;    // 26
    	u16             Imax                    : 1;    // 27 авария перегруз
    }bit;
	u16		all;
}SlowFailMask_type;
*/
typedef union{                      //ставится из микропрограммы
	struct FLAG_CMD_DRV{
    	u16		PwmOn             : 1;
    	u16		ChargeOk	    : 1;
    	u16		ExternFail	    : 1;
    	u16		Sync_Pwr	    : 1;
    	u16             TormSwEnable        : 1;
    	u16             TormConstCurrOn     : 1;
                u16             Pick_Up             : 1;
    };
	u16		all;
}FlagComand_type;

typedef union{                                  // ставится из обработчика прерывания ШИМ
	struct FLAG_STATE_DRV{
    	u16		OFF_PWM	                : 1;    // Выключение ШИМ
    	u16		OFF_ZAR	                : 1;    // Выключение зарядного реле
    	u16		LockOk_Pwr	            : 1;    
    	u16     flag_FastFailMaskMP     : 1;    
    };
	u16		all;
}FlagState_type;

typedef union{
    u16         all;
    struct{
        u16     n_1      :1;
        u16     n_2      :1;
        u16     n_3      :1;
        u16     n_4      :1;
        u16     n_5      :1;
        u16     n_6      :1;
        u16     n_7      :1;
        u16     n_8      :1;
    }bit;
}Din_type;

typedef struct{
    u16     SysCalk     :1;         // Запрос на пересчет системы параметров для векторного управления
}VectCnrlWord;
//****************************************************************************
typedef struct {
            	f32	            	SumCos	        ;
            	f32	            	SumSin	        ;

            	f32	            	SumCosCopy	    ;
            	f32	            	SumSinCopy	    ;
            	f32	            	Iact	        ;
            	f32	            	Ireact	        ;
            	f32	            	Ifull	        ;
            }CapCur_type;
//********************************************************
typedef struct {    //выходные
            	s16	            	Iv_adcVal	    ;
            	s16	            	Iw_adcVal	    ;

            	s16	            	Iv_adcVal_sum	;
            	s16	            	Iw_adcVal_sum	;

            	s16                                     iu              ;
            	s16                                     iv              ;
            	s16                                     iw              ;

                                s16                                     Ain3            ;
            	s16	            	Tu_adcVal	    ;
            	s16	            	Imax_adcVal	    ;

            	s16	            	Ain1_adcVal	    ;
            	s16	            	Ain2_adcVal	    ;
            	s16	            	Ain3_adcVal	    ;
            	s16	            	Ain4_adcVal	    ;
            	s16	            	Ain5_adcVal	    ;
            	s16	            	Ain6_adcVal	    ;

            	filtr_type                              UdFiltr	        ;

            	filtr_type                              IvFiltr	        ;
            	filtr_type                              IwFiltr	        ;

            	ustir_type                              IuUstir	        ;
            	ustir_type                              IvUstir	        ;
            	ustir_type                              IwUstir	        ;

            	ustir_type                              UdUstir	        ;

                                ustir_type                              Ain3Ustir	    ;
                                Var_type                                IuCalc	        ;
            	Var_type                                IvCalc	        ;
            	Var_type                                IwCalc	        ;
                                Var_type                                Ain3Calc                ;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            	CapCur_type                             IuCC	        ;
            	CapCur_type                             IvCC	        ;
            	CapCur_type                             IwCC	        ;
                                CapCur_type                             Ain3CC                  ;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            	u16	            	cntCosAdd	    ;
            	u16	            	cntCosAddCopy	;
            struct{
            	u16	            	periodOut	    :1 ;
            	u16	            	CopyOk	        :1 ;
            	u16                                     flagCalcOffset  :1 ;
                                u16                                     EnableCalcOffset:1 ;
                                u16                                     PrdCalcOk       :1 ;
                };
            	s16                                     cntCalcOffset   ;

            	_iq	            	Ireact_out	    ;
            	_iq	            	Ifull_out	    ;
             	_iq	            	Iact_out	    ;

            	LL_filtr_type                           filtr_Ireact_out;
            	LL_filtr_type                           filtr_Ifull_out	;
             	LL_filtr_type                           filtr_Iact_out	;

                                _iq                                     iu_flash        ;
            	_iq                                     iv_flash        ;
            	_iq                                     iw_flash        ;

                                _iq                                     Ain3_volt       ;
            	_iq	            	Iu	            ;
            	_iq	            	Iv	            ;
            	_iq	            	Iw	            ;

            	_iq	            	UdFast	        ;
            	_iq	            	Ud              ;

            	_iq	            	StepAngleMax	;    // Parameter: Maximum step angle (pu)

            	_iq	            	Angle	        ;
            	_iq	            	cosT	        ;
            	_iq	            	sinT	        ;

            	_iq	            	Uout	        ;    //выходное напряжение инвертора
            	LL_filtr_type                           UoutReal	    ;
            	_iq	            	Fout	        ;    //выходная частота инвертора

            	_iq	            	Pact_out	    ;    //Активная мощность на выходе станции
            	_iq	            	Pfull_out	    ;    //Полная мощность на выходе станции
            	_iq	            	cosFi_out	    ;    //cosFi на выходе станции
//входные
                _iq                 Ud0             ;
                _iq                 Ud100	        ;
//пользовательские
//---------------
            	_iq                 MTZref          ;   // Уставка МТЗ

            	s32	            	KustirCurOut	;
            	s16	            	KustirUd	    ;

            	u16	            	maxValCur_out	;
            	s16	            	UstirUdc	    ;

            	u16	            	CherFaz	        ;

            	_iq	            	Udmin	        ;
            	_iq	            	Udmax	        ;

            	filtrSum_type		TuV	            ;   // Температура ключей
            	filtrSum_type		Imax_Val        ;   // Измерение порога компаратора Imax
            	filtrSum_type		Ain1_Val        ;   // Значение аналогового входа 1
            	filtrSum_type		Ain2_Val        ;   // Значение аналогового входа 2
                s16                 ImaxOffset      ;   // Смещение порога компаратора Imax

            	s16	            	Tmax	        ;    //уставка максимальной температуры в градусах
            	s16	            	NumFail	        ;
            	s16	            	NumSlowFail	    ;   // Аварии из микропрограммы
///значения параметров на момент аварии
            	FastFailMask_type	FastFailMask	;   //Маска быстрых аварий привода
            	FastFailMask_type	FastFailMaskMP	;   //Маска быстрых аварий привода для микропрограммы
//            	SlowFailMask_type	SlowFailMask	;   //Маска медленных аварий привода
//volatile    	SlowFailMask_type	ExtFailMask	    ;   //Маска внешних медленных аварий из микропрограммы

            	FlagComand_type		FlagComand	    ;
            	FlagState_type		FlagState	    ;
            	s16                 FailLockFlag    ;   //выставляется при аварии, сбрасывается при пуске
            	s16	            	cntOffZarClear	;

            	s16	            	Ifull_out_fval     ;
            	s16	            	Iact_out_fval      ;

            	s16	            	Iw_fval            ;
            	s16	            	Iv_fval            ;
            	s16	            	Iu_fval            ;
            	s16	            	Ud_fval            ;
            	s16	            	Fout_fval        ;
            	s16	            	NumFail_fval	;

            	s16	            	Iu_adcVal_fail	;
            	s16	            	Iv_adcVal_fail	;
            	s16	            	Iw_adcVal_fail	;
                f32                 R_cab           ;
            	Din_type            Din             ;
            }measMem_type;
//****************************************************************************
//TypeHard_thrd	    комплектация железа
#define Bit_UV_thrd	    	0	//наличие управляемого выпрямителя
#define Bit_NX_VC_thrd	            1	//измерение напряжения платформой NX
#define Bit_Cur_thrd	            2	//наличие датчиков тока
#define Bit_TORM_IS_PRESENT	        3	//наличие тормозного ключа


#define	Config_0_thrd	((0<<Bit_Cur_thrd)|(0<<Bit_NX_VC_thrd)|(0<<Bit_UV_thrd))    //транс тока нет, УВ нет, изм напряж нет.
#define	Config_1_thrd	((0<<Bit_Cur_thrd)|(0<<Bit_NX_VC_thrd)|(1<<Bit_UV_thrd))    //транс тока нет, УВ нет, изм напряж есть.
#define	Config_2_thrd	((0<<Bit_Cur_thrd)|(1<<Bit_NX_VC_thrd)|(0<<Bit_UV_thrd))    //транс тока нет, УВ есть, изм напряж нет.
#define	Config_3_thrd	((0<<Bit_Cur_thrd)|(1<<Bit_NX_VC_thrd)|(1<<Bit_UV_thrd))    //
#define	Config_4_thrd	((1<<Bit_Cur_thrd)|(0<<Bit_NX_VC_thrd)|(0<<Bit_UV_thrd))    //
#define	Config_5_thrd	((1<<Bit_Cur_thrd)|(0<<Bit_NX_VC_thrd)|(1<<Bit_UV_thrd))    //
#define	Config_6_thrd	((1<<Bit_Cur_thrd)|(1<<Bit_NX_VC_thrd)|(0<<Bit_UV_thrd))    //
#define	Config_7_thrd	((1<<Bit_Cur_thrd)|(1<<Bit_NX_VC_thrd)|(1<<Bit_UV_thrd))    //
//****************************************************************************
extern u16     slowADCin[4];
//****************************************************************************


void MeasAnyTimeCall(void);
void MeasInit(void);
void MeasCur(measMem_type	*mPnt);
//void PowerPrmControl(measMem_type	*mPnt);
void Meas_20mS(void);
u16 SintCalc(sint_type *Pnt, s16 Val);
void SintReg(sint_type	*Pnt);
void FailValueCopy(measMem_type   *mPnt);
void FailValueCopyVector(measMem_type   *mPnt);
void PwmReinit(s16 Fpwm_X_XXkHz, s16 Dtime_X_XuS, s16 TypeDrv);
void IfullCalc(measMem_type   *mPnt);

void CapCurCalc(CapCur_type	*ccPnt, _iq Kreact, u16 cntCosAddCopy);
void Cap_MeasCur(measMem_type   *mPnt);
void Cap_MeasCur_Vector(measMem_type   *mPnt);
void MeasCur_4ms(void);

void calcOffset(void);
void ConstCurr(void);
void switchAngle();

in_line s16 f32_filtrSumFlag(f32_filtrSum_type *Pnt, s16 Val, s16 flag);

in_line s16	filtrSumFlag(filtrSum_type *Pnt, s16 Val, s16 flag);
in_line s16	filtrMinMax(filtrMinMax_type *Pnt, s16 Val, s16 ResFlag);
in_line s16 filtrSum(filtrSum_type *Pnt, s16 Val, s16 kolv);


s32 Filtr_s64(LL_filtr_type *fPnt, s32 val, s16 Kfiltr);
void sintCalcInit(measMem_type  *mPnt);
//**************************************************************************
#define	s16_Filtr_MACRO_val(valOld, valNew, Kfiltr, out_s16)                \
{                                                                           \
    (valOld.i)    = ((valOld.i) + (((((s32)valNew<<16) - (valOld.i)) + ((1<<Kfiltr)-1)) >> Kfiltr)); \
	out_s16 = valOld.s[1];                                                  \
}
//****************************************************************************
#define	s16_Filtr_MACRO(valOld, valNew, Kfiltr)    	valOld.i	= ((valOld.i) + (((((s32)valNew<<16) - (valOld.i)) + ((1<<Kfiltr)-1)) >> Kfiltr))
//****************************************************************************
#define s16_Filtr(S32_oldVal, val, Kfiltr)    (S32_oldVal + ((((s32)val<<16) - S32_oldVal +((1<<Kfiltr)-1)) >> Kfiltr))    /* s32 s16 s16 */
#define	iq_Filtr(valOld, valNew, Kfiltr)    (valOld + (((valNew - valOld) + ((1<<Kfiltr)-1)) >> Kfiltr))        /*(_iq valOld, _iq ValNew, s16 Kfiltr)*/
#define	s64_Filtr(valOld, valNew, Kfiltr)    (valOld + (((((s64)valNew<<32) - valOld) + ((1<<Kfiltr)-1)) >> Kfiltr))    /*s64 s64 s16*/
//**************************************************************************
//****************************************************************************
//**************************************************************************

//****************************************************************************
#define	MAX_ADC_MACRO()
#define	sMAX_ADC_MACRO()                \
{                                    \
	if(cntMax > 0){                 \
    	cntMax--;                   \
    	if(iu > MaxU)	MaxU = iu;  \
    	if(iv > MaxV)	MaxV = iv;  \
    	if(iw > MaxW)	MaxW = iw;  \
    }                               \
	else{                           \
    	cntMax = 740L*50L*5L;       \
    	MaxU = 0;                   \
    	MaxV = 0;                   \
    	MaxW = 0;                   \
    }                               \
}
//**************************************************************************
void GPIO12_set(void);
void GPIO12_clear(void);
void GPIO12_toggle(void);
void GPIO13_set(void);
void GPIO13_clear(void);
void GPIO13_toggle(void);
s16 f2int(f32 data, f32 mf);
f32 int2f(s16 data, f32 mf);
/*
#if(CHAR_BIT == 16)
    #define	ObrFazFromBVAK()    (GpioDataRegs.GPBDAT.bit.GPIO61)
#else
    #define	ObrFazFromBVAK()	0
#endif
*/
extern _iq		MaxU, MaxV, MaxW;
extern s32		cntMax;

extern const  u16 offZarFail[80];       // Что делать с выключением моста по аварии 0= не выключать 1= выключить
//****************************************************************************
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/
#ifdef	DEFINE_MEASURE_GLOBAL
    #define	MEASURE_GLOBAL
#else
    #define	MEASURE_GLOBAL	extern
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/
//Для торможения DC. Partola
typedef struct {
    f32     Ki          ;
    f32     Kd          ;
    f32     FiltrIn_pred;
    f32     Sum         ;
    s16     Ti_pred     ;
    s16     Td_pred     ;
    f32     Result_pred ;
    s16     Sum_start   ;
} PID_pwm_struct;


typedef struct {
    s16     Ti          ;
    s16     Td          ;
    s16     DifFiltr    ;
    s32     Kp          ;
    s32     In          ;
    s32     Min         ;
    s32     Max         ;
    s16     TempMax     ;
    f32     UinDC       ;
//    s32     Out         ;
} PID_pwm_UST_struct;
// ХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХХ
    	MEASURE_GLOBAL		measMem_type		measMem;

    	MEASURE_GLOBAL		s16	            	GlobalFlag_20mS;
                
// Для торможения DC. Partola
                MEASURE_GLOBAL          f32                                     Tpwm;      // Период ШИМ в мс;
                MEASURE_GLOBAL          _iq                                     I_dc_ust;  // Заданый ток для торможения постоянным током;
                MEASURE_GLOBAL          PID_pwm_UST_struct                      ppid_ust;
                MEASURE_GLOBAL          PID_pwm_struct                          ppid;
                MEASURE_GLOBAL          s16                                     timeDC;    //время удержания текущего тока в режиме DC в мс

                MEASURE_GLOBAL          s32                                     DebIu;
                MEASURE_GLOBAL          s32                                     DebIv;
                MEASURE_GLOBAL          s32                                     DebIw;
                MEASURE_GLOBAL          s16                                     RUState;
                MEASURE_GLOBAL          s16                                     deb;
                MEASURE_GLOBAL          s16                                     deb1;
                MEASURE_GLOBAL          s16                                     AngCount;
                MEASURE_GLOBAL          s16                                     RingUp;
                MEASURE_GLOBAL          _iq                                     signRef;

void findRs();
//********************************************************
#endif	//measure_H
