#include    "DRV_VECT_ALL_macro.h"
#include    "FAIL_CONTROL.h"
#include    "GT_DEFs.h"
#include    "MakeOpis.h"
#include 	"Tsk_MasterUart.h"
#define NUM_GRP_MTZ_UST 8 //номер группы, в кот. находится параметр  "Уставка МТЗ"
#define NUM_PRM         0 //номер параметра "Уставка МТЗ" в группе

#define MODE_OFF  0
#define MODE_FAIL 1
#define MODE_WARN 2
#define MODE_GO_TO_NO_ENCODER 3
//!!!! ПОХОЖЕ ЭТО ЛИШНЕЕ !!!!
//#define ENCO_TYPE_INCREMENTAL      0 // Инкрементальный энкодер
//#define ENCO_TYPE_ABS_ENDAT_2_0    1 // Абсолютный энкодер с протоколом EnDAT 2.0
//#define ENCO_TYPE_ABS_ENDAT_2_1    2 // Абсолютный энкодер с протоколом EnDAT 2.1
//#define ENCO_COS_SIN               3 // sin/cos энкодер

#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Pusk;                         //1  Pusk
    u16  Pnt_Stop;                         //2  Stop
    u16  Pnt_StopVibeg;                    //3  StopVibeg
    u16  Pnt_FailTek;                      //4  FailTek
    u16  Pnt_SelTormType;                  //5  SelTormType
    u16  Pnt_VibegTime;                    //6  VibegTime
    u16  Pnt_TypeDrv;                      //7  TypeDrv
    u16  Pnt_Ftek;                         //8  Ftek		
    u16  Pnt_Fpusk;                        //9  Fpusk
    u16  Pnt_SoftCurDwnEna;                //10 SoftCurDown  //u16  Pnt_SoftCurDwnEna;				   //28  SoftCurDwnEna	//Заменили вход
    u16  Pnt_StateGT;                      //11 StateGT		 //u16  Pnt_gtEqZero;			           //28  gtEqZero		//Добавили вход
    u16  Pnt_F;                            //12 F
    u16  Pnt_U;                            //13 U
    u16  Pnt_Napravlenie;                  //14  Napravlenie //u16  Pnt_Revers;                       //8  Revers
    u16  Pnt_Wr__Me_1;                     //15  Wr/~Me_1
    u16  Pnt_Wr__Me_2;                     //16  Wr/~Me_2	 //u16  Pnt_Wr__Me;                       //9  Wr/~Me
    u16  Pnt_Udc_Korrect;                   //17  Ud_Korrect   //Pnt_Udc_Korrect;
    u16  Pnt_K_Udc;                         //18  K_Ud		 //Pnt_K_Udc;
    u16  Pnt_Iu_Korrect;                   //19  Iu_Korrect
    u16  Pnt_K_Iu;                         //20  K_Iu
    u16  Pnt_Iv_Korrect;                   //21  Iv_Korrect
    u16  Pnt_K_Iv;                         //22  K_Iv
    u16  Pnt_Iw_Korrect;                   //23  Iw_Korrect
    u16  Pnt_K_Iw;                         //24  K_Iw
    u16  Pnt_T_1_V;                        //25  T_1_V
    u16  Pnt_T_2_V;                        //26  T_2_V
    u16  Pnt_Tmax;                         //27  Tmax
    u16  Pnt_Ustir_Cur;                    //28  Ustir_Cur
    u16  Pnt_MTZref;                       //29  MTZref
    u16  Pnt_CurSensKolv;                  //30  CurSensKolv //u16  Pnt_WrzAccel1Dn;                  //105  WrzAccel1Dn
    u16  Pnt_InvCurSens;                   //31  InvCurSens  //u16  Pnt_In_rez1;                      //143  In_rez1
    u16  Pnt_Ustir_Ud;                     //32  Ustir_Ud
	u16  Pnt_I_motor;                      //33  I_motor
	u16  Pnt_I_max;                        //34  I_max
    u16  Pnt_TypeDrv_1;                    //35  TypeDrv	//???? Повторяется
    u16  Pnt_Dtime;                        //36  Dtime
    u16  Pnt_Fpwm;                         //37  Fpwm
    u16  Pnt_PwmType;					   //38  PwmType	//Pnt_WrzAccel1Up;                  //104  WrzAccel1Up
	u16  Pnt_K_Il;                         //39  K_Ileak	//u16  Pnt_K_Il;                    //115  R_br
	u16  Pnt_Il_Korrect;                   //40  Ileak_Korect    u16  Pnt_Il_Korrect;                   //40  WrzAccel2Dn
	u16  Pnt_Ileak_MTZ;                    //41  Ileak_MTZ   //u16  Pnt_Phase_D_Wr;                   //41  Phase_D_Wr
	u16  Pnt_Ileak_Ustir;                  //42  Ileak_Ustir //u16  Pnt_WrzAccel2Up;                  //106  WrzAccel2Up
	u16  Pnt_EncoType;                     //43  EncoType    //u16  Pnt_In_rez2;                      //144  In_rez2
	u16  Pnt_Encoder_N;                    //44  Encoder_N
	u16  Pnt_Encoder_D;                    //45  Encoder_D
    u16  Pnt_EncoShift;                    //46  EncoShift
    u16  Pnt_EncoderFilter;                //47  EncoderFilter
	u16  Pnt_PsiRotor;                     //48  PsiRotor	???? //u16 Pnt_T0_Tr;                        //47  T0_Tr
	u16  Pnt_Lsd;                          //49  Lsd		???? //u16 Pnt_Tr_est_Max;                   //48  Tr_est_Max
	u16  Pnt_Lsq;                          //50  Lsq    	???? //u16  Pnt_Tr_est_Min;                  //49  Tr_est_Min
	u16  Pnt_Reset_Default;                //51  Reset_Default	 //u16  Pnt_Reset_Default_1;              //50  Reset_Default
    u16  Pnt_Wr_z;                         //52  Wr_z
    u16  Pnt_Me_z;                         //53  Me_z
    u16  Pnt_Wr_nom;                       //54  Wr_nom
    u16  Pnt_Wr_max;                       //55  Wr_max
    u16  Pnt_Fs_nom;                       //56  Fs_nom
    u16  Pnt_Us_nom;                       //57  Us_nom
	u16  Pnt_CalcPrmMot;                   //58  CalcPrmMot		//u16  Pnt_Calc_Rs_sigmaLs_Lm;           //59  Calc_Rs_sigmaLs_Lm
    u16  Pnt_Rr;                           //59 Rr
    u16  Pnt_Rs;                           //60  Rs
    u16  Pnt_Lm;                           //61  Lm
    u16  Pnt_Lls;                      	   //62  SigmaLs		//u16  Pnt_Lls;                          //63  Lls
    u16  Pnt_Zp;                           //63  Zp
    u16  Pnt_Adapt_Tr_Ls;                  //64  Adapt_Tr_Ls
    u16  Pnt_Calc_J;                       //65  Calc_J
    u16  Pnt_Jr;                           //66  Jr
    u16  Pnt_T0is;                         //67  T0is
    u16  Pnt_CalcRegI;                     //68  CalcRegI
    u16  Pnt_Regi_Kp;                      //69  Regi_Kp
    u16  Pnt_Regi_Ki;                      //70  Regi_Ki
    u16  Pnt_CalcRegILim;                  //71  CalcRegILim
    u16  Pnt_Reg_Id_Lim;                   //72  Reg_Id_Lim
    u16  Pnt_Reg_Iq_Lim;                   //73  Reg_Iq_Lim
    u16  Pnt_T0wr;                         //74  T0wr
    u16  Pnt_Tfwrz;                        //75  Tfwrz
    u16  Pnt_CalcRegWr;                    //76  CalcRegWr
    u16  Pnt_RegWr_Kp;                     //77  RegWr_Kp
    u16  Pnt_RegWr_Ki;                     //78  RegWr_Ki
    u16  Pnt_RegPos_Kp;					   //79  RegPos_Kp	//u16  Pnt_CalcRegWrLim;                 //79  CalcRegWrLim
    u16  Pnt_BSFilter_Fr;                  //80  BSFilter_Fr // переименован параметр RegWr_Lim
    u16  Pnt_BSFilter_dF;                  //81  Pnt_BSFilter_dF // переименован параметр Wr_od
    u16  Pnt_K_od;                         //82  K_od
    u16  Pnt_Tiwe;                         //83  Tiwe
    u16  Pnt_OgrRegWpr;                    //84  OgrRegWpr
    u16  Pnt_T_ad_Lm;                      //85  T_ad_Lm
    u16  Pnt_T0e;                          //86  T0e
    u16  Pnt_Kez;                          //87  Kez
    u16  Pnt_CalcRegE;                     //88  CalcRegE
    u16  Pnt_RegE_Kp;                      //89  RegE_Kp
    u16  Pnt_RegE_Ki;                      //90  RegE_Ki
    u16  Pnt_RegE_KRef;                    //91  RegE_KRef
    u16  Pnt_CalcRegELim;                  //92  CalcRegELim
    u16  Pnt_RegE_UpLim;                   //93  RegE_UpLim
    u16  Pnt_RegE_LowLim;                  //94  RegE_LowLim
    u16  Pnt_Reset_Default_1;              //95  Reset_Default		//u16  Pnt_Reset_Default;                //94  Reset_Default
    u16  Pnt_Kp;                           //96  Kp
    u16  Pnt_Ti;                           //97  Ti
    u16  Pnt_Td;                           //98  Td
    u16  Pnt_DifFiltr;                     //99  DifFiltr
    u16  Pnt_Max;                          //100  Max
    u16  Pnt_Min;                          //101  Min
    u16  Pnt_TempMax;                      //102  TempMax
    u16  Pnt_ZerHzDc;                      //103  ZerHzDc
    u16  Pnt_F_DCSTP;                      //104  F_DCSTP
    u16  Pnt_Time_STP;                     //105  Time_STP
    u16  Pnt_UinDC;                        //106  UinDC
    u16  Pnt_CurTormDc;                    //107 CurTormDc			//u16  Pnt_Isz;                          //21  Isz
    u16  Pnt_Dry_On;                       //108  Dry_On
    u16  Pnt_DryEn;                        //109  DryEn
    u16  Pnt_Ixx;	                       //110  Ixx SrcEnSel
    u16  Pnt_TimeDRY;                      //111  TimeDRY
    u16  Pnt_Idry;                         //112  Idry
    u16  Pnt_Iru;                          //113  Iru
    u16  Pnt_RUena;                        //114  RUena				//u16  Pnt_DinSrc;                     //27  DinSrc				//Заменили вход на RUena
    u16  Pnt_tormCheck;                    //115  tormCheck
    u16  Pnt_PwmOn;                        //116  PwmOn
    u16  Pnt_NotPwmOn;                     //117  NotPwmOn
    u16  Pnt_StopFreq;                     //118  StopFreq
	u16  Pnt_StopByDc;                     //119  StopByDc
	u16  Pnt_WorkRunMode;                  //120  WorkRunMode   выход сигнализации нормального режима двигателя
	u16  Pnt_NormStopMode;                 //121  NormStopMode  выход останова двигателя
    u16  Pnt_Uout;                         //122  Uout
    u16  Pnt_Pfull_out;                    //123  Pfull_out
    u16  Pnt_Pact_out;                     //124  Pact_out
	u16  Pnt_Preact_out;                   //125  Preact_out		//u16  Pnt_Reg_Id_Out;                   //98  Reg_Id_Out
	u16  Pnt_cosFi_out;                    //126  cosFi_out
	u16  Pnt_Tu;                           //127  Tu
	u16  Pnt_Iact_out;                     //128  Iact_out
	u16  Pnt_Ifull_out;                    //129  Ifull_out
	u16  Pnt_Ireact_out;                   //130  Ireact_out
    u16  Pnt_We;                           //131  We
    u16  Pnt_Wr;                           //132  Wr
    u16  Pnt_Me;                           //133  Me
	u16  Pnt_Reg_Wpr;                      //134  Reg_Wpr    		//u16  Pnt_Me_Base;                      //148  Reg_Wpr
    u16  Pnt_FoutSign;                     //135  FoutSign -- signum(Pnt_We)
    u16  Pnt_FoutAbs;                      //136  FoutAbs  -- abs(Pnt_We)
    u16  Pnt_StatusDRV;                    //137  Заменил на StatusDRV //132  ScalarDrv
    u16  Pnt_EncoderDetect;                //138  EncoderDetect
    u16  Pnt_EncoderAngle;                 //139  EncoderAngle
    u16  Pnt_Reg_Iq_Out;                   //140 Reg_Iq_Out
    u16  Pnt_Reg_Wr_Out;                   //141 Reg_Wr_Out
    u16  Pnt_RegE_Ref;                     //142 RegE_Ref
    u16  Pnt_RegE_Out;                     //143 RegE_Out
	u16  Pnt_StatusDC;                     //144 StatusDC
    u16  Pnt_End;
}FuncDRV_V5_type;
//********************************************************
#ifdef _PROJECT_FOR_CCS_
extern TGlobalMasterSlave GlobalMstSlv;
#else
      TGlobalMasterSlave GlobalMstSlv;
#endif //-- _PROJECT_FOR_CCS_
//	DRV_VECT_ALL.h
//!!!! ПОХОЖЕ ЭТО ЛИШНЕЕ !!!!
extern u16          PwmEn;
#define ModbusSlave   1
#define  ModbusMaster 2

	#define  REGIM_ALONE   0 /*0 - одиночный*/
    #define  REGIM_MASTER  1 /*1 - master*/
    #define  REGIM_SLAVE   2 /*2 - slave*/
    #define  REGIM_NEUPR   3 /*3 - неуправляемый*/
f32                 Fbase_rad_s;// = 167.0f;
////====================================================================
//#define Wr_IQtoF(Wr)  (Fbase_rad_s *_IQtoF(Wr))         // Приведение скорости из IQ в рад/сек  Wr - в IQ формате привязано к числу 80.0 Гц
//#define Wr_FtoIQ(Wr)  _IQ( Wr / Fbase_rad_s)               // Приведение скорости из рад/сек в IQ   Wr - в IQ формате привязано к числу 80.0 Гц
////====================================================================
//******************************************************************************
/*===========================================================================*/
// Преобразование int <-> float
/*===========================================================================*/
#define m1          ((f32)1.	)
#define m10         ((f32)10.	)
#define m100        ((f32)100.	)
#define m1000       ((f32)1000.	)
#define m10000      ((f32)10000.)

#define d1          ((f32)1.    )
#define d10         ((f32).1    )
#define d100        ((f32).01   )
#define d1000       ((f32).001  )
#define d10000      ((f32).0001 )

/*===========================================================================*/
// Преобразование из float32 в s16 с заданной точностью
// передается коэфициент преобразования переменной
/*===========================================================================*/
s16 f2int(f32 data, f32 mf)
{
    return (s16)((data) * mf);
}
/*===========================================================================*/
// Преобразование из s16 в float32 с заданной точностью
// передается коэфициент преобразования переменной
/*===========================================================================*/
f32 int2f(s16 data, f32 mf)
{
    f32 tmp = data;
    return tmp * mf;
}
//****************************************************************************
typedef struct{
// -1-
    _iq     K_Iu;
    _iq     K_Iv;
    _iq     K_Iw;
    _iq     K_Udc;
    _iq     K_Il;
// -2-
    s32     Rr         ;
    s32     Rs         ;
    s32     Lm         ; 
    f32     Lls    	   ;
    s32  	Lsd		   ; // Input: Индуктивность статора PNSM по оси D
    s32  	Lsq		   ; // Input: Индуктивность статора PNSM по оси Q
    s32		Ixx		   ; // Ток холостого хода двигателя, вычисленный в режиме измерения с вращением или введенный руками.
    f32     Jr         ;
    s32  	PsiR	   ; // Input: Магнитный поток ротора PNSM
    f32     RegWr_Kp   ;
    f32     RegWr_Ki   ;
    s16     Encoder_D  ;
    u16     MTZCurrent ; //Максимальный ток ПЧ

// -3-
}nvDRVV5_State_type;

#define     nvSfDRV     (*(nvDRVV5_State_type*)0)
//#define addrNvVect  &nvSfDRV.Rr	  //!!!! ПОХОЖЕ ЭТО ЛИШНЕЕ !!!!
//#define sizeNvSkal  (5*sizeof(_iq)) //!!!! ПОХОЖЕ ЭТО ЛИШНЕЕ !!!!
#define sizeNvVect  (11*sizeof(s32)+2*sizeof(s16))	//nvDRVV5_State_type -2-

//====================================================================
typedef struct{
    u16     RsDef       :1;
    u16     JDef        :1;
    u16     AutoFase    :1;
}vectAutoFlags_type;
//====================================================================

//********************************************************
// Объявление состояний DRV_CMD.h
typedef enum {
	rampdown=0,
	frequency,
	combi,
	softcurrentdown,
}BRAKEtype_enum;

typedef enum {
	startUp	=0,
	motMsurmnt,
	tormResChk,  // Проверка тормозного резистора
	contyChk  ,
	drying    ,
	running   ,
}PUSKtype_enum;

typedef enum {
	continuityCheck	=(1<<0),
	dryingMode		=(1<<1),
	dcInjektBrake	=(1<<2),
}DCSTATEtype_enum;

typedef enum {
	TorqueRef=0,
	SpeedRef,
}FOCREFtype_enum;

enum {
	ScalarWOdtc=0,
	ScalarWIdtc,
	ACIMVectWIfbk,
	ACIMVectWOfbk,
	PMSMVectWIfbk,
	PMSMVectWOfbk,
};
/*
typedef enum GTSTATEtype {
    Up 			=(1<< 0),                             // Задатчик темпа работает на увеличение частоты
	Dwn			=(1<< 1),                             // Задатчик темпа работает на уменьшение частоты
	RevCur		=(1<< 2),                             // Задатчик темпа работает на реверс
	RevRef		=(1<< 3),                             // Задание для задатчика темпа на реверс
	Reset		=(1<< 4),
	Freeze		=(1<< 5),
	RefZero		=(1<< 6),                             // Выход нулевой
	EqResVal	=(1<< 7),
	Great		=(1<< 8),
	ReversHold	=(1<< 9),
	flazhok		=(1<<10),
	EqRefVal	=(1<<11),
	EquRV		=(1<<12)							 // в отличии от EqResVal действительно отражает состояние сигнала - Pnt_EquRV (sPnt->Fout == +/- tempResVal)
}GTSTATEtype_enum;
*/
// Объявление переменных с глобальным временем жизни
typedef struct{
//--- DRV_CMD.h
	BRAKEtype_enum		brakeType;
	PUSKtype_enum		puskState;
	u16					combiBrakeTimer;
    u16					softCurDwnTimer;
	u16					rampDownTimer;
	u16					contyChkTimer;
    u32					dryingTimer;
	u16                 count_ERROR;
	f32     Filter;
	f32     Filter1;
	f32     Filter2;
	f32     FilterTorrent;
	f32     KoefFiltr ;
	union{				u16 value;
	 struct{			 u16 stopCmd      :1;
						 u16 runCmd       :1;
						 u16 failCmd 	  :1;
						//DRVcmd2DRVм2 command
						 u16 freqBrake	  :1;		// Frequency Brake Started
						 u16 pwmOffC28Req :1;
						 u16 dcInjectEna  :1;
						 u16 softCurDwnEna:1;
						 u16 reserve      :9;
						}bit;
						}algState;
//--- DRV_VECT_ALL.h
//!!!    nvDRVV5_State_type     nvCopy;
	struct{
		nvAdjCoef_type		adjCoefcnts;
		nvDRVV5_State_type  nvParams;
	}nvCopy;

    struct{
// -1-
        _iq     Iu_Korrect;
        _iq     Iv_Korrect;
        _iq     Iw_Korrect;
        _iq     Udc_Korrect;
        _iq     Il_Korrect;
        _iq     Uu_Korrect;
        _iq     Uv_Korrect;
        _iq     Uw_Korrect;
        union{
            struct{
                u16     Pusk        : 1;
            }bit;
            u16     all;
        }Flag;

        s16     Dtime;
        s16     Fpwm;
        s16     Fbase;
        u16     tmrOffZar;
// -2-
        s32		Rr			;
        s32		Rs			;
        s32		Lm			;
        f32		Lls			;
        s32  	Lsd		    ; // Input: Индуктивность статора PNSM по оси D
        s32  	Lsq		    ; // Input: Индуктивность статора PNSM по оси Q
        s32		Ixx		   ; // Ток холостого хода двигателя, вычисленный в режиме измерения с вращением или введенный руками.
        f32		Jr			;
        s32  	PsiR	    ; // Input: Магнитный поток ротора PMSM
        f32     RegWr_Kp    ;
        f32     RegWr_Ki    ;
        s16		Encoder_D	;
        u16     MTZCurrent  ;
        vectAutoFlags_type  vectAutoFlags   ;
        u16     StateDC;

    }prev;
}DRVV5_State_type;

//******************************************************************************
void PwmCheckDrv(DRVV5_State_type *sPnt, u16 Fpwm, u16 Dtime, u16 typeDrv)
{
	COMMONPARAMM3		*mPnt3;
	
	mPnt3	= &GlobalM3;

	if(TYPEDRV_ANYFOC&(1<<typeDrv)){
			if((Fpwm < 200)||(Fpwm > 1000))  Fpwm = 1000;	//2.00 - 10.00 kHz для вектора
	}else{
	        if((Fpwm < 200)||(Fpwm > 1400))  Fpwm = 500;	//2.00 - 16.00 kHz
	 }
	    
	if      (Dtime < 10)        Dtime   = 10;   			//1.0 uS
	else if (Dtime > 65)        Dtime   = 65;   			//6.5 uS    Ограничение в модуле DTime
	
	mPnt3->Fpwm 	= int2f(Fpwm , m10);		// частота ШИМ, Гц //(f32)Fpwm * 10.0f;
	mPnt3->DeadTime = int2f(Dtime, d10);		//(f32)Dtime / 10.0f;

	DrvReInit_Start(); 										// Переинициализация ШИМ
	
	 sPnt->prev.Fpwm    = Fpwm;                 			// Запомнить для истории
	 sPnt->prev.Dtime   = Dtime;
	mPnt3->Tpwm 		= _IQ(100/(f32)Fpwm);				// инициализация периода ШИМ Partola

}
//******************************************************************************



//*************************Функция начальной инициализации параметров при переинициализации************************************//
void UstirPrevSet(u32 ramPnt, _iq addKustir, u16 saveKorrect, u16 savePnt_K, s32 adjCoefcntsK, _iq *prevKorrect)
{
	_iq name = addKustir ? addKustir : _IQ(1.0); //name инициализируется текущим измеренным действующим значением тока или значением _IQ(1.0)

	save_s32(ramPnt, saveKorrect, name);         //сохраняем это значение в параметр для юстировочного значения выходного тока по фазе
	*prevKorrect = name;
	save_s32(ramPnt, savePnt_K, adjCoefcntsK);   //считываем из энергонезависмой памяти в микропрограмму начальное калибровочное значение
}
//*************************************************************************************************************************//
void UstirFunc(u32 ramPnt, u16 PntKorrect, u16 Pnt_K, _iq *adjCoefcntsK, _iq *prevKorrect, _iq *nvAdrCoefK, _iq mPnt28Name, s16 addKustir, s16 *mPnt3Gain)
{
    _iq     K, NewVal_Kor;
    u16     flg = 0;

    NewVal_Kor = (_iq)load_s32(ramPnt, PntKorrect);     //17  Ud_Korrect
    K = (_iq)load_s32(ramPnt, Pnt_K);

    if(*adjCoefcntsK != K) flg = 1;
    else if(NewVal_Kor != *prevKorrect){
    	 K = _IQmpy(K, _IQdiv(NewVal_Kor, (_iq)mPnt28Name)); //mPnt28Name - текущее значение К;
    	 flg = 1;
    }

    if(flg == 1){

		if((K >= _IQ(0.5))&&(K <= _IQ(2.0)))
		{
			WrFramMicro(nvAdrCoefK, &K, sizeof(s32));
		}
    }
    save_s32(ramPnt, Pnt_K, K);                          //18  K_Ud
	*mPnt3Gain = (s16)_IQrmpyI32int((_iq)K, addKustir);
	*adjCoefcntsK = K;                                   //сохраняем текущее значение К

	save_s32(ramPnt, PntKorrect, mPnt28Name);
	*prevKorrect = mPnt28Name;
}
//********************************************************
u16 *FuncDRV_V5_1(FuncDRV_V5_type *progPnt, u32 ramPnt)
{
//--- Описательная часть DRV_CMD.h
	u16					typeDrv;
	u32					refIdc;//, Time;
	f32					nomIm,tempF;
     _iq                 temp_iq;
    COMMONPARAMM3	   *mPnt;
//    COMMONPARAMC28	   *cPnt;
	DRVV5_State_type   *sPnt;
//--- Описательная часть DRV_VECT_ALL.р
    s16								tmp_s16;
	u16                            time_error;
    //s16    temp1,temp2;
    //static s16 						tmrPwmOnOff;
 //   static u16						tmp161Old=0;
  //  static u16						FirstStartMMP;
	 
    s16								tmp161, t_1_v, t_2_v;
    s16								Fpwm, Dtime;
    u16								tmp, PWM_On;//, OFF_PWM; //, OFF_ZAR
	u16								tmpUstirCur;
	u16								tmpMTZ,Pn;
    //u16								NumFailDRVLocal;//,MTZmax; //mtz_, ustirCur
    f32                 			tmpF,invCurBase,invVoltBase,invHertzBase,fL,K_BASE;//,tmpfL,tmpfL1;
	f32 KoefIact = 0;
    _iq								tmpIQ,tmpIQ1;
	u16                              mode;
    //DRVV5_State_type				*sPnt;
    nvDRVV5_State_type				*nvAdr;
    nvAdjCoef_type					*nvAdrCoef;
    MPOpis_type                     *moPnt;
    COMMONPARAMC28					*mPnt28;
    COMMONPARAMM3					*mPnt3;
    wm_type *pWM; //для записи значения максимальной токовой защиты
    wm_type   WM;
//--- Инициализация переменных DRV_CMD.h
	sPnt   = (DRVV5_State_type *)(ramPnt + progPnt->Pnt_State);
//    cPnt   = &GlobalC28;
    mPnt   = &GlobalM3;
	refIdc = 0;
	pWM = &WM;
//--- Инициализация переменных DRV_VECT_ALL.h
    mPnt28	  = &GlobalC28;
    mPnt3	  = &GlobalM3;
    moPnt     = (MPOpis_type *)&MPOpis;
	//sPnt	  = (DRVV5_State_type *)(ramPnt + progPnt->Pnt_State);
	nvAdr 	  = GetNvAdr(progPnt->Pnt_nvState, &nvSfDRV);
	nvAdrCoef = (nvAdjCoef_type  *)&nv.adjCoefcnts;

	Fpwm  = load_s16(ramPnt, progPnt->Pnt_Fpwm);
    Dtime = load_s16(ramPnt, progPnt->Pnt_Dtime);
//	NumFailDRVLocal = mPnt28->NumFailDRV;

	invCurBase   = (f32)1/((f32)MPOpis.Base[NUM_CUR_BASE ]);		// reciproctal(MPOpis.Base[NUM_CUR_BASE ])=1/MPOpis.Base[NUM_CUR_BASE ]
	invVoltBase  = (f32)1/((f32)MPOpis.Base[NUM_VOLT_BASE]);		// reciproctal(MPOpis.Base[NUM_VOLT_BASE])=1/MPOpis.Base[NUM_VOLT_BASE]
	invHertzBase = (f32)1/((f32)MPOpis.Base[NUM_HZ_BASE  ]);		// reciproctal(MPOpis.Base[NUM_HZ_BASE  ])=1/MPOpis.Base[NUM_HZ_BASE  ]
	//invFreqBase  = (f32)1/((f32)MPOpis.Base[NUM_FREQ_BASE]);		// reciproctal(MPOpis.Base[NUM_FREQ_BASE])=1/MPOpis.Base[NUM_FREQ_BASE]
//#warning  проверка МТЗ утечки
//	GlobalM3.FlagCommand.bit.EnLeakageCurrent = 1;//
//********************************************************
// Инициализация алгоритма при первом запуске
//--- Инициализация DRV_CMD.h
	if(flagIsFirstCall){
		sPnt->algState.value =0;
		setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
		clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);
		//clrBitMicro(ramPnt, progPnt->Pnt_DC_STOP);
		clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);
		save_s16(ramPnt, progPnt->Pnt_StatusDC, 0);
		GlobalM3.FlagCommand.bit.BitJumpToNoEncod=0;
		sPnt->combiBrakeTimer=0;
		sPnt->softCurDwnTimer=0;
		sPnt->rampDownTimer  =0;
		sPnt->contyChkTimer  =0;
		sPnt->dryingTimer	 =0;
		sPnt->Filter=0.0;
		sPnt->Filter1=0.0;
		sPnt->Filter2=0.0;
		sPnt->FilterTorrent=0.0;
        //return &progPnt->Pnt_End;
    }
//--- Инициализация DRV_VECT_ALL.h
	if(flagIsFirstCall || mPnt3->FlagCommand.bit.DrvReInit){				//начальная инициализация до начала любых действий
		RdFramMicro(&sPnt->nvCopy.adjCoefcnts, nvAdrCoef, sizeof(nvAdjCoef_type));
		if(sPnt->nvCopy.adjCoefcnts.K_Iu == 0){
			sPnt->nvCopy.adjCoefcnts.K_Iu	= load_s32(ramPnt, progPnt->Pnt_K_Iu);		// Загрузить значения по умолчанию
			sPnt->nvCopy.adjCoefcnts.K_Iv	= load_s32(ramPnt, progPnt->Pnt_K_Iv);
			sPnt->nvCopy.adjCoefcnts.K_Iw	= load_s32(ramPnt, progPnt->Pnt_K_Iw);
			sPnt->nvCopy.adjCoefcnts.K_Udc	= load_s32(ramPnt, progPnt->Pnt_K_Udc);
			sPnt->nvCopy.adjCoefcnts.K_Il	= load_s32(ramPnt, progPnt->Pnt_K_Il);
			WrFramMicro(nvAdrCoef, &sPnt->nvCopy.adjCoefcnts, sizeof(nvAdjCoef_type));	// Прописать начальные значения
		}
		RdFramMicro(&sPnt->nvCopy.nvParams, nvAdr, sizeof(nvDRVV5_State_type));
//		if(sPnt->nvCopy.nvParams.Rs == 0){
//            sPnt->nvCopy.nvParams.Rr		 = load_s16(ramPnt, progPnt->Pnt_Rr );
//            sPnt->nvCopy.nvParams.Rs		 = load_s16(ramPnt, progPnt->Pnt_Rs );				// Значения из микропрограммы по умолчанию
//            sPnt->nvCopy.nvParams.Lm		 = load_s16(ramPnt, progPnt->Pnt_Lm );
//            sPnt->nvCopy.nvParams.Lls		 = load_float(ramPnt, progPnt->Pnt_Lls );
//            sPnt->nvCopy.nvParams.Lsd		 = load_s16(ramPnt, progPnt->Pnt_Lsd);
//            sPnt->nvCopy.nvParams.Lsq		 = load_s16(ramPnt, progPnt->Pnt_Lsq);				// Значения из микропрограммы по умолчанию
//            sPnt->nvCopy.nvParams.Ixx		 = load_s16(ramPnt, progPnt->Pnt_Ixx);			// Значения из микропрограммы по умолчанию
//            sPnt->nvCopy.nvParams.Jr		 = load_float(ramPnt, progPnt->Pnt_Jr);
//            sPnt->nvCopy.nvParams.PsiR		 = load_s16(ramPnt, progPnt->Pnt_PsiRotor);		//!!! ПРОВЕРИТЬ
//            sPnt->nvCopy.nvParams.RegWr_Kp	 = int2f(load_s16(ramPnt, progPnt->Pnt_CalcRegILim), d100);
//#ifdef _PROJECT_FOR_CCS_
//#warning Поменять кол-во знаков параметра в ТехПО
//#endif
//            sPnt->nvCopy.nvParams.RegWr_Ki	= int2f(load_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim), d10000);
//			WrFramMicro(nvAdr, &sPnt->nvCopy.nvParams, sizeof(nvDRVV5_State_type));	// Прописать начальные значения
//		}

		UstirPrevSet(ramPnt, mPnt28->Iu, progPnt->Pnt_Iu_Korrect, progPnt->Pnt_K_Iu, sPnt->nvCopy.adjCoefcnts.K_Iu, &sPnt->prev.Iu_Korrect);
		UstirPrevSet(ramPnt, mPnt28->Iv, progPnt->Pnt_Iv_Korrect, progPnt->Pnt_K_Iv, sPnt->nvCopy.adjCoefcnts.K_Iv, &sPnt->prev.Iv_Korrect);
		UstirPrevSet(ramPnt, mPnt28->Iw, progPnt->Pnt_Iw_Korrect, progPnt->Pnt_K_Iw, sPnt->nvCopy.adjCoefcnts.K_Iw, &sPnt->prev.Iw_Korrect);
		UstirPrevSet(ramPnt, mPnt28->Il, progPnt->Pnt_Il_Korrect, progPnt->Pnt_K_Il, sPnt->nvCopy.adjCoefcnts.K_Il, &sPnt->prev.Il_Korrect);
		UstirPrevSet(ramPnt, mPnt28->Udc, progPnt->Pnt_Udc_Korrect, progPnt->Pnt_K_Udc, sPnt->nvCopy.adjCoefcnts.K_Udc, &sPnt->prev.Udc_Korrect);

//		save_s16  (ramPnt, progPnt->Pnt_Rr			, (s16)sPnt->nvCopy.nvParams.Rr 			);
//        save_s16  (ramPnt, progPnt->Pnt_Rs			, (s16)sPnt->nvCopy.nvParams.Rs 			);
//        save_s16  (ramPnt, progPnt->Pnt_Lm			, (s16)sPnt->nvCopy.nvParams.Lm 			);
//        save_float(ramPnt, progPnt->Pnt_Lls			, sPnt->nvCopy.nvParams.Lls 				);
//		save_s16  (ramPnt, progPnt->Pnt_Lsd			, (s16)sPnt->nvCopy.nvParams.Lsd			);
//        save_s16  (ramPnt, progPnt->Pnt_Lsq			, (s16)sPnt->nvCopy.nvParams.Lsq			);
//        save_s16  (ramPnt, progPnt->Pnt_Ixx			, (s16)sPnt->nvCopy.nvParams.Ixx			);
//        save_float(ramPnt, progPnt->Pnt_Jr			, sPnt->nvCopy.nvParams.Jr    			);
//        save_s16  (ramPnt, progPnt->Pnt_PsiRotor	, (s16)sPnt->nvCopy.nvParams.PsiR		);
//		save_s16(ramPnt, progPnt->Pnt_CalcRegILim	, f2int(sPnt->nvCopy.nvParams.RegWr_Kp, m100) 	);
//		save_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim	, f2int(sPnt->nvCopy.nvParams.RegWr_Ki, m10000) 	);
//		//save_s16(ramPnt, progPnt->Pnt_Encoder_D		,(s16)sPnt->nvCopy.nvParams.Encoder_D	);

//        mPnt3->Rr		= int2f((s16)sPnt->nvCopy.nvParams.Rr , d1000);
//        mPnt3->Rs		= int2f((s16)sPnt->nvCopy.nvParams.Rs , d1000);
//        mPnt3->Lm		= int2f((s16)sPnt->nvCopy.nvParams.Lm , d10000);
//        mPnt3->Lls		= sPnt->nvCopy.nvParams.Lls;
//        mPnt3->Lsd		= int2f((s16)sPnt->nvCopy.nvParams.Lsd, d10000);
//        mPnt3->Lsq		= int2f((s16)sPnt->nvCopy.nvParams.Lsq, d10000);
//        mPnt3->Ixx		= int2f((s16)sPnt->nvCopy.nvParams.Ixx, d10);
//        mPnt3->Jr		= sPnt->nvCopy.nvParams.Jr;
//        mPnt3->PsiR		= int2f((s16)sPnt->nvCopy.nvParams.PsiR, d1000);


        mPnt3->Rr		= int2f((s16)load_s16(ramPnt, progPnt->Pnt_Rr), d1000);
        mPnt3->Rs		= int2f((s16)load_s16(ramPnt, progPnt->Pnt_Rs), d1000);
        mPnt3->Lm		= int2f((s16)load_s16(ramPnt, progPnt->Pnt_Lm), d10000);
        mPnt3->Lls		= load_float(ramPnt, progPnt->Pnt_Lls);
        mPnt3->Lsd		= int2f((s16)load_s16(ramPnt, progPnt->Pnt_Lsd), d10000);
        mPnt3->Lsq		= int2f((s16)load_s16(ramPnt, progPnt->Pnt_Lsq), d10000);
        mPnt3->Ixx		= int2f((s16)load_s16(ramPnt, progPnt->Pnt_Ixx), d10);
        mPnt3->Jr		= load_float(ramPnt, progPnt->Pnt_Jr);
        mPnt3->PsiR		= int2f((s16)load_s16(ramPnt, progPnt->Pnt_PsiRotor), d1000);


        //mPnt3->Angle = _IQ(0.0/360.0); //!!! Пока на этапе отладки

		if (load_s16(ramPnt, progPnt->Pnt_Regi_Ki)) {//Проверяем автонастройку регулятора скорости
			mPnt3->FlagCommand.bit.ManualSpdReg = 0;
		}else{
		    mPnt3->FlagCommand.bit.ManualSpdReg = 1;
		    //mPnt3->KpSpd = sPnt->nvCopy.nvParams.RegWr_Kp;
		    //mPnt3->KiSpd = sPnt->nvCopy.nvParams.RegWr_Ki;
		    mPnt3->KpSpd = int2f(load_s16(ramPnt, progPnt->Pnt_CalcRegILim), d100);
		    mPnt3->KiSpd = int2f(load_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim), d10000);
		}
		//возможно убрать это mPnt3->Encoder_D= (s16)sPnt->nvCopy.nvParams.Encoder_D;
		mPnt3->Encoder_D= load_s16(ramPnt, progPnt->Pnt_Encoder_D );
//--
		mPnt3->Encoder_N 	=  load_s32(ramPnt, progPnt->Pnt_Encoder_N);							// число меток датчика скорости;
		mPnt3->Enco_Shift 	=  load_s16(ramPnt, progPnt->Pnt_EncoShift);							// смещение нулевой позиции энкодера;
        mPnt3->Imax		 	= _IQ(int2f(load_s16(ramPnt, progPnt->Pnt_I_max), m10)*invCurBase   );  // максимальный ток двигателя, А;

		mPnt3->Fpwm			= int2f(load_s16(ramPnt, progPnt->Pnt_Fpwm), m10);		// частота ШИМ, Гц
		mPnt3->TregSpd		=  (f32)load_s16(ramPnt, progPnt->Pnt_T0is)		* 0.0001f;
		mPnt3->FcutSpd		=  (f32)load_s16(ramPnt, progPnt->Pnt_CalcRegI);
		mPnt3->CompDelaySpd =  (f32)load_s16(ramPnt, progPnt->Pnt_Regi_Kp)	* 0.1f;
		//mPnt3->AntiForceSpd =  (f32)load_s16(ramPnt, progPnt->Pnt_RegWr_Kp)	* 0.1f;
		mPnt3->KpForced     = int2f(load_s16(ramPnt, progPnt->Pnt_RegWr_Kp), d100);
		mPnt3->KiForced	    = int2f(load_s16(ramPnt, progPnt->Pnt_RegWr_Ki), d10000);
		mPnt3->AntiTime     = (f32)load_s16(ramPnt, progPnt->Pnt_CalcRegWr)* 0.1f;

		// Настройки режима измерения параметров, для мощности 7.5 кВт
////		mPnt3->Fpwm_MMP = int2f(400, m10);    									// частота ШИМ, Гц
		mPnt3->Fpwm_MMP = mPnt3->Fpwm;
		mPnt3->TUsn_MMP = 80.0; 												// Время выхода напряжения на ном. напряжение двигателя, сек
		mPnt3->DeadTime = (f32)Dtime*0.1f; 										// мертвое время, мкс
		mPnt3->Rsw		= 0.03f; 												// Сопротивление ключа инвертора для мощности 7.5 кВт (Ом)
		mPnt3->Udf 		= 1.0f;  												// Напряжение включения обратного диода (В)
		mPnt3->dI		= 0.07f; 												// Минимальный ток переключения компесации МВ (А)

        mPnt3->Isn		= load_s32(ramPnt, progPnt->Pnt_I_motor);				// номинальный ток двигателя, А;
        mPnt3->Usn		= _IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Us_nom  ), d10)*invVoltBase); //_IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Us_nom  ), d10)/moPnt->Base[NUM_VOLT_BASE]); //номинальное напряжение двигателя
        mPnt3->Fnom		= _IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Fs_nom  ), invHertzBase   )); //_IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Fs_nom  ), d1)/moPnt->Base[NUM_HZ_BASE]);
        mPnt3->Wnom		= load_s32(ramPnt, progPnt->Pnt_Wr_nom );				// Частота вращения вала ротора номинальная    рад/с
        mPnt3->Wmax		= load_s32(ramPnt, progPnt->Pnt_Wr_max );				// Частота вращения вала ротора максимальная   рад/c
        mPnt3->Zp		= load_s16(ramPnt, progPnt->Pnt_Zp);					//– число пар полюсов;
		mPnt3->Kez		= _IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Kez), d100));	// коэффициент задания по ЭДС
		
		mPnt3->CurrBASE	   = int2f(moPnt->Base[NUM_CUR_BASE], d10);//((f32)(moPnt->Base[NUM_CUR_BASE]))*0.1f;
		mPnt3->VoltBASE	   = 	   moPnt->Base[NUM_VOLT_BASE];
		mPnt3->FreqBASE	   = int2f(moPnt->Base[NUM_HZ_BASE] , d10);//((f32) moPnt->Base[NUM_HZ_BASE])*0.1f;
		mPnt3->invFreqBASE = _IQ((f32)1/mPnt3->FreqBASE);
		mPnt3->PowerBASE   = mPnt3->VoltBASE  * _IQtoF(mPnt3->Isn) * mPnt3->CurrBASE;
		// Расчет базы по моменту
		mPnt3->TorqBASE = mPnt3->VoltBASE*mPnt3->CurrBASE*(f32)mPnt3->Zp/(mPnt3->FreqBASE*4.18879020478639);// 4.18879020478639=4/3*pi
		mPnt3->Mez		   = _IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Me_z), d1)/mPnt3->TorqBASE); //задание электромагнитного момента, Нм;
		
		mPnt3->FlagCommand.bit.CurrInv  		= load_s16(ramPnt, progPnt->Pnt_InvCurSens);	// инверсия токов
		mPnt3->FlagCommand.bit.EMFRegEnb		= load_s16(ramPnt, progPnt->Pnt_T0wr);			// вкл/выкл ослабления поля
		mPnt3->FlagCommand.bit.FiltrRefSpdEnb 	= load_s16(ramPnt, progPnt->Pnt_Reg_Iq_Lim);	// вкл/выкл фильтра ослабления поля
		mPnt3->FlagCommand.bit.ARollbackEnb		= load_s16(ramPnt, progPnt->Pnt_Tfwrz);		// вкл/выкл Антиоткатной функции
		mPnt3->Encoder_Type						= load_s16(ramPnt, progPnt->Pnt_EncoType);		// Выбор типа энкодера

	  //if(flagIsFirstCall) SysCalc_Start(); -- Выполняется в конце
	}
	mPnt3->EncoFlt_N	=   32; //load_s16(ramPnt, progPnt->Pnt_EncoderFilter);//47 фильтр энкодера

	//коэфф для коррекции входного напряжения
	mPnt3->Koeff_V	= (f32)load_s16(ramPnt, progPnt->Pnt_RegE_Ki)*0.001 ;//90
	mPnt3->Koeff_U=(f32)load_s16(ramPnt, progPnt->Pnt_CalcRegELim)*0.001;//92
	mPnt3->Koeff_W	=(f32)load_s16(ramPnt, progPnt->Pnt_RegE_LowLim)*0.001 ;//94

	tmpF =(f32)GlobalC28.Faza_V.RMS;
	tmpF =tmpF*0.001;
	save_s16(ramPnt, progPnt->Pnt_RegE_KRef,(s16)tmpF);

	tmpF =(f32)GlobalC28.Faza_U.RMS;
	tmpF =tmpF*0.001;
	save_s16(ramPnt, progPnt->Pnt_RegE_UpLim,(s16)tmpF);

	tmpF =(f32)GlobalC28.Faza_W.RMS;
	tmpF =tmpF*0.001;
	save_s16(ramPnt, progPnt->Pnt_Reset_Default_1,(s16)tmpF);

//UstirPrevSet(ramPnt, mPnt28->Faza_U.VoltageLog,progPnt->Pnt_RegE_KRef,progPnt->Pnt_CalcRegELim, sPnt->nvCopy.adjCoefcnts.K_Uu, &sPnt->prev.Uu_Korrect);
//UstirPrevSet(ramPnt, mPnt28->Faza_V.VoltageLog,progPnt->Pnt_RegE_UpLim,progPnt->Pnt_RegE_Ki, sPnt->nvCopy.adjCoefcnts.K_Uv, &sPnt->prev.Uv_Korrect);
//UstirPrevSet(ramPnt, mPnt28->Faza_W.VoltageLog,progPnt->Pnt_Reset_Default_1,progPnt->Pnt_RegE_LowLim, sPnt->nvCopy.adjCoefcnts.K_Uw, &sPnt->prev.Uw_Korrect);

  //********************************************************
	if(GlobalM3.Regim_Privod == REGIM_NEUPR){
		sPnt->algState.bit.runCmd=0;
		sPnt->algState.bit.stopCmd=1;
	}
 	if(GlobalM3.REGIM_COUNT_J==5){  //добавил для останова после измерения момента инерции
		sPnt->algState.bit.runCmd=0;
		sPnt->algState.bit.stopCmd=1;
		save_float(ramPnt, progPnt->Pnt_Jr	, GlobalM3.Jr);
	}
  // основное тело алгоритма DRV_CMD.h
  //--- Проверка наличия исключительных ситуаций
	if(!load_s16(ramPnt, progPnt->Pnt_FailTek)){			//если в данный момент их нет, проверяем и отрабатываем поступающие команды
		sPnt->algState.bit.failCmd= 0;
		if((testBitMicro(ramPnt, progPnt->Pnt_Stop)) && (!sPnt->algState.bit.stopCmd) && (sPnt->algState.bit.runCmd))
		{
		    setBitMicro(ramPnt, progPnt->Pnt_NormStopMode); //в POWER_LOSS_APV сигнализируем о команде останова
			sPnt->algState.bit.stopCmd=1;
			if((testBitMicro(ramPnt, progPnt->Pnt_StopVibeg)) || (sPnt->puskState < running)){ sPnt->brakeType = rampdown; }	// rampdown выбран по дискр. вх. или получили СТОП вначале ПУСКа, когда прозваниваем или сушим
			else{ sPnt->brakeType = (BRAKEtype_enum)load_s16(ramPnt, progPnt->Pnt_SelTormType); }
			clrBitMicro(ramPnt, progPnt->Pnt_WorkRunMode); //больше не даем команду записи флага работающего двигателя
		}
		else if((testBitMicro(ramPnt, progPnt->Pnt_Pusk)&&(!sPnt->algState.bit.stopCmd))) {
			sPnt->algState.bit.runCmd=1;
		}

		typeDrv			 = load_s16(ramPnt, progPnt->Pnt_TypeDrv);
		mPnt3->DriveType = typeDrv; // 0=скаляр без компенсации МВ, 1=скалярный, 2=ВУ АД с ОС, и т.д.

		#ifdef _PROJECT_FOR_CCS_
		#warning Не красивый переход на бездатчиковый режиме при обрыве энкодера
		#endif
		if(GlobalM3.FlagCommand.bit.BitJumpToNoEncod==1){//ВРЕМЕННО!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			mPnt3->DriveType = TYPE_ACIM_FOC_SENSORLESS; // Асинхронное Без Датчиковое Векторное управлениеTYPE_ACIM_FOC_SENSORLESS; //вектор бездатчиковый
		}
		nomIm			 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_I_motor));
		refIdc			 = _IQ(nomIm * _IQtoF(load_s32(ramPnt, progPnt->Pnt_CurTormDc))); // Чтобы вначале в переменой на ходилось осознанное значение
	}
	else if(!sPnt->algState.bit.failCmd)
	{
	    sPnt->algState.bit.failCmd= 1;
	    sPnt->algState.bit.stopCmd=1;
	    sPnt->brakeType = rampdown;
	    clrBitMicro(ramPnt, progPnt->Pnt_WorkRunMode); //больше не даем команду записи флага работающего двигателя
	}		//иначе переводим командоаппарат в "Аварйный" режим

	// Определяем режим торможения ПЧ для особых случаев его режима работы.
	// Код должен находится именно здесь, так как выше тдет выбор режима торможения для стандартных режимов работы
	if( mPnt3->Regim_Privod == REGIM_SLAVE || mPnt3->Regim_Privod == REGIM_NEUPR ) {
		sPnt->brakeType = rampdown; // Принудительно ставим останов Выбегом - если есть хоть один ведомый.
	}

	// Отработка стопов
    if (sPnt->algState.bit.stopCmd)  //если нужно остановить двигатель или привод не задействован
    {
			sPnt->algState.bit.runCmd=0;		         //Для начала, сбрасываем командоапарат пуска
			sPnt->puskState=startUp;
			switch (sPnt->brakeType){
                case combi:
                	if((TYPEDRV_ANYSCALAR&(1<<typeDrv))||(load_s16(ramPnt, progPnt->Pnt_Wr__Me_2) == SpeedRef)){	//бывший Pnt_USTME (Задание по скорости или моменту)
						if(!sPnt->combiBrakeTimer){
							//setBitMicro(ramPnt, progPnt->Pnt_COMBI);
							sPnt->combiBrakeTimer= load_s16(ramPnt, progPnt->Pnt_Time_STP) * K_TIME_CALL_MICRO_TAKT;
						}
						else{
							if (_IQabs(load_s32(ramPnt, progPnt->Pnt_F)) <= load_s32(ramPnt, progPnt->Pnt_F_DCSTP)){
								if(sPnt->combiBrakeTimer==1){
									if((testBitMicro(ramPnt, progPnt->Pnt_SoftCurDwnEna))&&(TYPEDRV_ANYFOC&(1<<typeDrv))) sPnt->brakeType= softcurrentdown;
									else{
										//sPnt->algState.bit.dcInjectEna=0;  // это progPnt->Pnt_DC_STOP
										clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);
										setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
										clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);
										sPnt->brakeType= rampdown;		   //Там и обнулим Pnt_StatusDC и algState.bit.dcInjectEna
									}
								}
								else{
									sPnt->algState.bit.freqBrake  =0;
									sPnt->algState.bit.dcInjectEna=1;
									refIdc= _IQ(nomIm * _IQtoF(load_s32(ramPnt, progPnt->Pnt_CurTormDc)));
									save_s16(ramPnt, progPnt->Pnt_StatusDC, dcInjektBrake);
								}
								sPnt->combiBrakeTimer--;
								break;
							}
						}
					}
					 break;
                case frequency:
                	if((TYPEDRV_ANYSCALAR&(1<<typeDrv))||(load_s16(ramPnt, progPnt->Pnt_Wr__Me_2) == SpeedRef)){	//бывший Pnt_USTME (Задание по скорости или моменту)
						if(!testBitMicro(ramPnt, progPnt->Pnt_StopFreq)){
							if(load_s16(ramPnt, progPnt->Pnt_StateGT) & (RefZero|EquRV|Freeze))
							{
								setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
								clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);
								sPnt->brakeType= rampdown;
							}
							else{ setBitMicro(ramPnt, progPnt->Pnt_StopFreq); }
						}
                        else if(sPnt->algState.bit.freqBrake == 0){
                        	 if(load_s16(ramPnt, progPnt->Pnt_StateGT)  & (Dwn|EquRV)) sPnt->algState.bit.freqBrake = 1; // !!! Этот флаг уже лишний, ПРИЧЕСАТЬ !!!
                        }
                         //else if(load_s16(ramPnt, progPnt->Pnt_StateGT) & (RefZero/*|EqRefVal|Freeze*/)){				//testBitMicro(ramPnt, progPnt->Pnt_gtEqZero)
						else if((load_s16(ramPnt, progPnt->Pnt_StateGT) & (RefZero|Freeze)) == RefZero){				//testBitMicro(ramPnt, progPnt->Pnt_gtEqZero)
                       	 	sPnt->algState.bit.freqBrake = 0;
							if((testBitMicro(ramPnt, progPnt->Pnt_SoftCurDwnEna)) && (TYPEDRV_ANYFOC & (1<<typeDrv))) sPnt->brakeType= softcurrentdown;
							else{
								clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);
								setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
								clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);
								sPnt->brakeType= rampdown;
							}
						}
                        
                    }
					break;
                case rampdown:
					if(PwmIsOn()){
						 sPnt->rampDownTimer = load_s16(ramPnt, progPnt->Pnt_VibegTime) * K_TIME_CALL_MICRO_TAKT;
						 setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
						 clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);
						 if(mPnt3->FlagCommand.bit.mWorkEnb){
		           	            mWorkDsb_set(); 								// выключить измерение параметров
		           	            mPnt3->FlagCommand.bit.mWorkEnb = 0;
		           	            PwmCheckDrv(sPnt, Fpwm, Dtime, typeDrv);
		        	       	    mPnt3->FlagCommand.bit.DrvReInit = 1; 			// Переинициализация ШИМ назад
		        	           	save_s16(ramPnt, progPnt->Pnt_CalcPrmMot, 0);	// Сброс запуска определения параметров
						 }
					}
					else if(sPnt->rampDownTimer){ sPnt->rampDownTimer--;}
					else{
						sPnt->algState.bit.stopCmd=0;								//Отработали останов, сбрасываем командоапарат
						save_s16(ramPnt, progPnt->Pnt_StatusDC, 0);					//Если перед этим отрабатывались drying или combi
						sPnt->algState.bit.dcInjectEna=0;
						sPnt->algState.bit.freqBrake  =0;	// !!! Этот флаг уже лишний, ПРИЧЕСАТЬ !!!
					}
					break;
				case softcurrentdown:
						if(!sPnt->softCurDwnTimer){
							sPnt->algState.bit.dcInjectEna=0;   // это progPnt->Pnt_DC_STOP
							GlobalM3.FlagCommand.bit.SoftCurDwn=1;
							sPnt->algState.bit.softCurDwnEna=1; // это progPnt->Pnt_SoftCDflag); // !!! Этот флаг уже лишний, ПРИЧЕСАТЬ !!!
							clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);
							sPnt->softCurDwnTimer= K_TIME_CALL_MICRO_TAKT;
							}
						else{
							 if(sPnt->softCurDwnTimer==1){
								 setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
								 clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);
								 GlobalM3.FlagCommand.bit.SoftCurDwn=0;
								 sPnt->algState.bit.softCurDwnEna=0; // это progPnt->Pnt_SoftCDflag); // !!! Этот флаг уже лишний, ПРИЧЕСАТЬ !!!
								 sPnt->brakeType= rampdown;
							 }
							 sPnt->softCurDwnTimer--;
						}
                    break;    
                default: break;
			}
    }
	else if (sPnt->algState.bit.runCmd) //--- ПУСК
	    {
			switch(sPnt->puskState){
				case startUp:	//Начало пуска
#ifdef _PROJECT_FOR_CCS_
#warning !!!! Разобраться с входом блока или параметр или DIN_SRC
#endif
					mPnt3->Angle = _IQ(0.0/360.0); //!!! Пока на этапе отладки
			        mPnt->RingUp	   = 0;
			        sPnt->dryingTimer  = 0;
			        sPnt->contyChkTimer= 0;

			        if(load_s16(ramPnt, progPnt->Pnt_CalcPrmMot) == 0){
						if(load_s16(ramPnt, progPnt->Pnt_tormCheck)){
							sPnt->puskState = tormResChk;
							break;
						}
						else{
							sPnt->puskState = contyChk;
							break;
						}
					}
					else{
						sPnt->puskState = motMsurmnt;}

				case motMsurmnt://Измерение параметров двигателя

					if(TYPEDRV_ANYFOC&(1<<typeDrv)){
						if(!mPnt3->FlagCommand.bit.mWorkEnb) {
////							mPnt3->Fpwm 	= 4000.0f;
////							mPnt3->Fpwm_MMP = 4000.0f;
							// Определяем алгоритм определения параметров двигателя.
							if(load_s16(ramPnt, progPnt->Pnt_CalcPrmMot) == 1)	GlobalM3.FlagCommand.bit.MeasureLmRs=0;  //выбор алгоритма измерения  LmRs
							else												GlobalM3.FlagCommand.bit.MeasureLmRs=1;

							mPnt3->Fpwm			= int2f(load_s16(ramPnt, progPnt->Pnt_Fpwm), m10);		// частота ШИМ, Гц
							mPnt3->Fpwm_MMP = mPnt3->Fpwm;
							DrvReInit_Start(); 						// Запуститть переинициализацию
							SysCalc_Start();

					        mPnt3->Rr		= 0.0f;  // Сброс для плохого завершения
					        mPnt3->Rs		= 0.0f;  // Сброс для плохого завершения
					        mPnt3->Lm		= 0.0f;  // Сброс для плохого завершения
					        mPnt3->Lls		= 0.0f;  // Сброс для плохого завершения
					        mPnt3->Lsd		= 0.0f;  // Сброс для плохого завершения
					        mPnt3->Lsq		= 0.0f;  // Сброс для плохого завершения
					        mPnt3->Ixx		= 0.0f;  // Сброс для плохого завершения

							//FirstStartMMP 	= 4;
							mWorkEnb_set(); 						// Инициировать измерение параметров
							mPnt3->FlagCommand.bit.mWorkEnb = 1;
						}
						else if (mPnt28->FlagState.bit.mWorkFin){
							sPnt->nvCopy.nvParams.Rr  = (s16)(mPnt28->Rr * 1000.0 );
							sPnt->nvCopy.nvParams.Rs  = (s16)(mPnt28->Rs * 1000.0 );
							sPnt->nvCopy.nvParams.Lm  = (s16)(mPnt28->Lm * 10000.0);
							//mPnt3->LM1=sPnt->nvCopy.nvParams.Lm;
							sPnt->nvCopy.nvParams.Lls =       mPnt28->Lls;
							sPnt->nvCopy.nvParams.Lsd = (s16)(mPnt28->Lsd* 10000.0);
							sPnt->nvCopy.nvParams.Lsq = (s16)(mPnt28->Lsq* 10000.0);
							sPnt->nvCopy.nvParams.Ixx = (s16)(mPnt28->Ixx_meas * 10.0); // Измеренный ток перекидываем в нормальные переменные
							mPnt3->Rs   	 = 		 mPnt28->Rs;
							mPnt3->Rr   	 = 		 mPnt28->Rr;
							mPnt3->Lm   	 = 		 mPnt28->Lm;
							mPnt3->Lls  	 = 		 mPnt28->Lls;
							mPnt3->Lsd  	 = 		 mPnt28->Lsd;
							mPnt3->Lsq  	 = 		 mPnt28->Lsq;
							mPnt3->Ixx  	 = 		 mPnt28->Ixx_meas;  // Измеренный ток перекидываем в нормальные переменные

							WrFramMicro(&nvAdr->Rr, &sPnt->nvCopy.nvParams.Rr, sizeof(f32) * 7 ); 				// Сохранить вычисленные значения
							memcpy((char *)&sPnt->prev.Rr, (char *)&sPnt->nvCopy.nvParams.Rr, sizeNvVect);	// Скопировать начальные значения

							save_s16  (ramPnt, progPnt->Pnt_Rs       , (s16)sPnt->nvCopy.nvParams.Rs );
							save_s16  (ramPnt, progPnt->Pnt_Rr       , (s16)sPnt->nvCopy.nvParams.Rr );
							save_s16  (ramPnt, progPnt->Pnt_Lm       , (s16)sPnt->nvCopy.nvParams.Lm );
							save_float(ramPnt, progPnt->Pnt_Lls      ,     sPnt->nvCopy.nvParams.Lls );
							save_s16  (ramPnt, progPnt->Pnt_Lsd      , (s16)sPnt->nvCopy.nvParams.Lsd);
							save_s16  (ramPnt, progPnt->Pnt_Lsq      , (s16)sPnt->nvCopy.nvParams.Lsq);
							save_s16  (ramPnt, progPnt->Pnt_Ixx		 , (s16)sPnt->nvCopy.nvParams.Ixx);

							mWorkFinClr_set(); 					// запрос на сброс флага окончания измерения mWorkFin
							sPnt->algState.bit.stopCmd=1;		//Инициируем останов
							sPnt->brakeType= rampdown;
						}
					}
					else{
						save_s16(ramPnt, progPnt->Pnt_CalcPrmMot, 0);
						sPnt->algState.bit.stopCmd=1;		//Инициируем останов
						sPnt->brakeType= rampdown;
					}
					break;

				case tormResChk: // Проверка торомозного резистора
					if (load_s16(ramPnt, progPnt->Pnt_tormCheck)) { // Если проверка резистора  разрешена, то проверяем
						if (!GlobalM3.mTormCheck){ // Если флаг запуска проверки тормозного резистора не установлен
							GlobalM3.mTormCheck = 1; // Даем сигнал на проверку в С28 ядро
						} else if (GlobalC28.cTormCheck == 1) { // Если из С28 ядра пришел ответ 1
							sPnt->puskState ++; // переходим на следующий этап, если нас не остановят по ошибке обрыва
							GlobalM3.mTormCheck = 0; // Сброс флага запуска в ноль.
						}
					}
					else {
						sPnt->puskState ++; // Если проверка не требуется - сразу переход на новый этап запуска.
					}
					break;

				case contyChk:	//Прозвонка
					if(!sPnt->contyChkTimer){  //!!! Реализация без параметров после отладки ПЕРЕДЕЛАТЬ !!!
						if(TYPEDRV_PMSMFOC&(1<<typeDrv)){
							mPnt3->FlagCommand.bit.WrotCtrl = TorqueRef;
							mPnt3->Imax	= _IQmpy(_IQ(0.2), load_s32(ramPnt, progPnt->Pnt_I_motor));// максимальный ток двигателя, А на время проверки;
							mPnt3->Wmax	= _IQmpy(_IQ(0.1), load_s32(ramPnt, progPnt->Pnt_Wr_nom ));// максимальная частота вращения вала ротора, рад/c на время проверки;
							SysCalc_Start(); //!!! ПЕРЕДЕЛАТЬ, чтобы небыло полного пересчета параметров - SysCalc_Start()
						}
						else{
							sPnt->puskState++;
						}
						//!!! Здесь лучше засинзронизировать не по времени а по сигналу от Lift_CMD о начале снятия тормоза
						sPnt->contyChkTimer = 4 * (K_TIME_CALL_MICRO_TAKT / 10);
						clrBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
						setBitMicro(ramPnt, progPnt->Pnt_PwmOn);
					}
					else{
						if(sPnt->contyChkTimer==1){
							mPnt3->FlagCommand.bit.WrotCtrl = load_s16(ramPnt, progPnt->Pnt_Wr__Me_2);
							mPnt3->Imax	=_IQ(int2f(load_s16(ramPnt, progPnt->Pnt_I_max ), m10 )*invCurBase);
							mPnt3->Wmax	= load_s32(ramPnt, progPnt->Pnt_Wr_max);
							SysCalc_Start();
							sPnt->puskState++;
							sPnt->contyChkTimer = 0;
						}
						sPnt->contyChkTimer--;
					}
					break;
				case drying:	//Сушка
					 if(!sPnt->dryingTimer){
						if((testBitMicro(ramPnt, progPnt->Pnt_Dry_On) == 0) && (load_s16(ramPnt, progPnt->Pnt_DryEn) == 0))  sPnt->puskState++;
						else{
							sPnt->dryingTimer= (u32)load_s16(ramPnt, progPnt->Pnt_TimeDRY) * K_TIME_CALL_MICRO_TAKT;
							sPnt->algState.bit.dcInjectEna=1;
							refIdc= _IQ(nomIm * _IQtoF(load_s32(ramPnt, progPnt->Pnt_Idry)));
							save_s16(ramPnt, progPnt->Pnt_StatusDC, dryingMode);
							clrBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
							setBitMicro(ramPnt, progPnt->Pnt_PwmOn);
						}
					 }
					 else{
							if(sPnt->dryingTimer==1){
								//sPnt->algState.bit.dcInjectEna=0;  // это Pnt_DC_STOP
								setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
								clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);
								//sPnt->algState.bit.runCmd=0;		//Просушили, сбрасываем командоапарат пуска
								//sPnt->puskState=startUp;
								sPnt->algState.bit.stopCmd=1;		//Инициируем останов
								sPnt->brakeType= rampdown;			//Там и обнулим Pnt_StatusDC и algState.bit.dcInjectEna
							}
							sPnt->dryingTimer--;
					 }
					 break;
				case running:	//Работа (Нормальный режим работы)
					save_s16(ramPnt, progPnt->Pnt_StatusDC, 0);
					sPnt->algState.bit.dcInjectEna = 0;
					clrBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
					setBitMicro(ramPnt, progPnt->Pnt_PwmOn);

					/**********Для реализации АПВ при аварии по исчезновению питания********/

                    #ifdef _PROJECT_FOR_CCS_
                    #warning  Добавлено для реализации АПВ по питанию
                    #endif
					setBitMicro(ramPnt, progPnt->Pnt_WorkRunMode); //в блок POWER_LOSS_APV сигнализируем о нормальном режиме работы двигателя
					/*По данному сигналу , в модуле POWER_LOSS_APV.h в FRAM запишется флаг работающего двигателя и начнется  запись текущего времени в поле  энергонезависимой         */
					/*структуры. Если двигатель остановится по аварии питания, об этом будет сигнализировать          */
					/*установленный флаг */
					/***********************************************************************/
					break;
			}
    	}
//--- Выдаем статус командоапарата
    save_s16(ramPnt, progPnt->Pnt_StatusDRV, sPnt->algState.value);

//--- Отработаем запросы командоаппарата pwmOff/pwmOn
    if((mPnt3->FlagCommand.bit.PwmOn == 1 ) && (testBitMicro(ramPnt, progPnt->Pnt_NotPwmOn) == 1)){
    	mPnt3->FlagCommand.bit.PwmOn = 0;																	// Штатный запрос на отключение ШИМ
    }
    else if((mPnt3->FlagCommand.bit.PwmOn == 0 ) && (testBitMicro(ramPnt, progPnt->Pnt_NotPwmOn) == 0)){	//Если есть попытка пуска
#ifdef _PROJECT_FOR_CCS_
#warning !Контроль CalcPrmMot нужно разруливать на уровне командоапарата(доделать)!
#endif
		if(load_s16(ramPnt, progPnt->Pnt_CalcPrmMot) == 0){													//и не активен режим определения параметров, пускаемся. (Аварии и Ud проверяются в блоке PUSK_READY )
			mPnt3->FlagCommand.bit.PwmOn = 1;
		}
	}
	if( OFF_PWM_CHK() ){																					//Сразу после пуска, проверяем состояния запроса на отключение от С28 ядра
		OFF_PWM_ACK();																						//Квитируем запрос
		sPnt->algState.bit.pwmOffC28Req=1;
	}

//--- Отработаем исключительные ситуации
	if((sPnt->algState.bit.pwmOffC28Req)||(sPnt->algState.bit.failCmd)||(load_s16(ramPnt, progPnt->Pnt_FailTek))){
		mPnt3->FlagCommand.bit.PwmOn  = 0;								// Снять глобальный флаг ШИМ в M3
		setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
		clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);  //СТОП на выход

			if(mPnt3->FlagCommand.bit.mWorkEnb) {		     // Если работали в реж. опред. параметров
            mWorkDsb_set();
            mPnt3->FlagCommand.bit.mWorkEnb = 0;

            mPnt3->Rs  = int2f(load_s16(ramPnt, progPnt->Pnt_Rs), d10000);
            mPnt3->Rr  = int2f(load_s16(ramPnt, progPnt->Pnt_Rr), d1000);
            mPnt3->Lm  = int2f(load_s16(ramPnt, progPnt->Pnt_Lm), d10000);
            mPnt3->Lls = load_float(ramPnt, progPnt->Pnt_Lls);
            mPnt3->Lsd = int2f(load_s16(ramPnt, progPnt->Pnt_Lsd), d10000);
            mPnt3->Lsq = int2f(load_s16(ramPnt, progPnt->Pnt_Lsq), d10000);
            mPnt3->Ixx = int2f(load_s16(ramPnt, progPnt->Pnt_Ixx), d10);

            PwmCheckDrv(sPnt, Fpwm, Dtime, typeDrv);
       	    mPnt3->FlagCommand.bit.DrvReInit = 1; 			// Переинициализация ШИМ назад
            save_s16(ramPnt, progPnt->Pnt_CalcPrmMot, 0);
        }

		//clrBitMicro(ramPnt, progPnt->Pnt_DC_STOP);
		clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);
		save_s16(ramPnt, progPnt->Pnt_StatusDC, 0);

		mPnt->RingUp  = 0;
		/*!!!mPnt->RUState = 0;*/

		sPnt->brakeType					=rampdown;
		sPnt->puskState					=startUp;
		sPnt->algState.bit.runCmd		=0;
		sPnt->algState.bit.pwmOffC28Req =0;

		sPnt->combiBrakeTimer = 0;
		sPnt->softCurDwnTimer = 0;
		sPnt->contyChkTimer   = 0;
		sPnt->dryingTimer	  = 0;
	}

	mPnt3->ppid_ust.Kp       = load_s32(ramPnt, progPnt->Pnt_Kp		 );
	mPnt3->ppid_ust.Ti       = load_s16(ramPnt, progPnt->Pnt_Ti		 );
	mPnt3->ppid_ust.Td       = load_s16(ramPnt, progPnt->Pnt_Td		 );
	mPnt3->ppid_ust.DifFiltr = load_s16(ramPnt, progPnt->Pnt_DifFiltr);
	mPnt3->ppid_ust.Max      = load_s32(ramPnt, progPnt->Pnt_Max	 );
	mPnt3->ppid_ust.Min      = load_s32(ramPnt, progPnt->Pnt_Min	 );
	mPnt3->ppid_ust.TempMax  = load_s16(ramPnt, progPnt->Pnt_TempMax );
	mPnt3->ppid_ust.UinDC    = load_s32(ramPnt, progPnt->Pnt_UinDC	 );


  //********************************************************
  // основное тело алгоритма DRV_VECT_ALL.h
  PWM_On=!testBitMicro(ramPnt, progPnt->Pnt_NotPwmOn); // Для совместимости DRV_CMD.h и DRV_VECT_ALL.h
  mPnt3->Revers = (load_s16(ramPnt, progPnt->Pnt_Napravlenie) & 1);

// подача на мотор постоянного тока
	if((!sPnt->algState.bit.dcInjectEna) /*&& ((load_s32(ramPnt, progPnt->Pnt_F) != 0) !!!! смысл этого параметра искажен???? ||(load_s16(ramPnt, progPnt->Pnt_ZerHzDc) == 0))*/){
		mPnt3->Uout = load_s32(ramPnt, progPnt->Pnt_U);						//индекс модуляции
		if(mPnt28->FlagState.bit.PwmEn == 0){
			mPnt3->FlagCommand.bit.TormConstCurrOn = 0;
		}
		if(sPnt->prev.StateDC == continuityCheck){
			mPnt3->FlagCommand.bit.TormConstCurrOn = 0;
		}
		tmpIQ = load_s32(ramPnt, progPnt->Pnt_F);							//частота
		if (mPnt3->Revers == 1){
			mPnt3->Fref = -tmpIQ;
		}
		else{
			mPnt3->Fref = tmpIQ;
		}
	}
	else{
		sPnt->prev.StateDC = load_s16(ramPnt, progPnt->Pnt_StatusDC);
		defBitMicro(ramPnt, progPnt->Pnt_StopByDc, sPnt->algState.bit.dcInjectEna );	//!!! Временное решение, нужно решать на уровне командоапрата
		mPnt3->I_dc_ust = refIdc;
		if((mPnt28->FlagState.bit.PwmEn == 1) && (sPnt->algState.bit.dcInjectEna == 1)){ //!!! Временное решение, нужно решать на уровне командоапрата if (mPnt28->FlagState.bit.PwmEn == 1){
			mPnt3->FlagCommand.bit.TormConstCurrOn = 1;
		}else{
			mPnt3->Fref = 0;
			mPnt3->Uout = 0;
			mPnt3->FlagCommand.bit.TormConstCurrOn = 0;
		}
	}

// ====================== Разбираемся с пусками и остановами =========
#ifdef COMMENT 	//Реализовано в алгоритме DRV_CMD
	OFF_PWM = 0;
	if((mPnt3->FlagCommand.bit.PwmOn == 0 ) && PWM_On !=0){				//попытка пуска
		if(mPnt3->FlagCommand.bit.ChargeOk && (mPnt28->NumFail == 0)){  //нет текущих аварий и есть готовность по заряду
			if (load_s16(ramPnt, progPnt->Pnt_CalcPrmMot) == 0){		//не активен режим определения параметров
				mPnt3->FlagCommand.bit.PwmOn = 1;
			}
		}
	}

	//Проверка состояния запроса на отключение от С28 ядра
	if( OFF_PWM_CHK() ){												// OFF_PWM - УСТАНОВИЛ ОБРАБОТЧИК АВАРИИ
		OFF_PWM_ACK();													// Команда на выключение прошла можно снять этот сигнал.
		PWM_On  = 0;
		OFF_PWM = 1;													// Для выключения в микропрограмме
// Управление OFF_ZAR перенесено в блок FAIL_COMMON.h
//		if((OFF_ZAR = offZarFail[NumFailDRVLocal])) {					// По номеру аварии определить надо ли выключать зарядный модуль
//			tmrPwmOnOff = load_s16(ramPnt, progPnt->Pnt_TimeOffZar);	// Время для выключения зарядного реле после выключения ШИМ
//		}
	}
// Управление OFF_ZAR перенесено в блок FAIL_COMMON.h
//	if( testBitMicro(ramPnt, progPnt->Pnt_START) && PWM_On ==0 ){
//		if(offZarFailStop[NumFailDRVLocal] == 1)            			// Для выключения заряда в останове по авариям Ud и питающей сети
//		if(tmrPwmOnOff == -1){                       					// Таймер не заряжен
//			tmrPwmOnOff = load_s16(ramPnt, progPnt->Pnt_TimeOffZar);	// Время для выключения зарядного реле
//		}
//	}
//
//	OFF_ZAR = 0;
//	if( tmrPwmOnOff <0 )	;
//	else if ( tmrPwmOnOff >0 )    tmrPwmOnOff--;
//	else                        { tmrPwmOnOff--;  OFF_ZAR = 1; }		// Установить сигнал для выключения заряда, tmrPwmOnOff = -1

	defBitMicro(ramPnt, progPnt->Pnt_NotPwmOn, !OFF_PWM );				//заменили !!!! defBitMicro(ramPnt, progPnt->Pnt_Off_PWM, OFF_PWM );
#endif //-- COMMENT

	if(PWM_On ==0){
#ifdef COMMENT
		mPnt3->FlagCommand.bit.PwmOn  = 0;								// Снять флаг ШИМ
		if( mPnt28->FlagState.bit.PwmEn == 1 ){
			mPnt3->NumFail_fval = 0;									// Тут только штатный останов !!!
		}
#endif
	// ===================================================================
		// Проверка на изменение параметров
		//Fpwm  = load_s16(ramPnt, progPnt->Pnt_Fpwm);
		//Dtime = load_s16(ramPnt, progPnt->Pnt_Dtime);
		if( (sPnt->prev.Fpwm  != Fpwm )||(sPnt->prev.Dtime != Dtime)||(sPnt->prev.Fbase != MPOpis.Base[NUM_FREQ_BASE])){	// Если изменены эти параметры, то переинициализировать параметры ШИМ
			sPnt->prev.Fbase = MPOpis.Base[NUM_FREQ_BASE];
			PwmCheckDrv(sPnt, Fpwm, Dtime, typeDrv);
		}
	}
	save_s16(ramPnt, progPnt->Pnt_Fpwm,     sPnt->prev.Fpwm);
	save_s16(ramPnt, progPnt->Pnt_Dtime,    sPnt->prev.Dtime);

	//===========================================================================
	// !!!!!! Исправление от Сосновчика

#ifdef _PROJECT_FOR_CCS_
#warning Изменен алгоритм отработки взаимосвязи между МТЗ и Юстир.знач.тока (Исходник от Колесника ниже). Вернул назад энергонезависимость в общей куче, а маскирование выполняется через RAM. ЭТО НЕ САМЫЙ ЛУЧШИЙ ВАРИАНТ РЕШЕНИЯ, НО НЕ БЫЛО ВРЕМЕНИ НА ДЕТАЛЬНУЮ ПРОРАБОТКУ.
#endif

	// Проверка корректности введенных значений МТЗ и Юстировочного значения тока
	tmpMTZ 		= (u16)load_s16(ramPnt, progPnt->Pnt_MTZref); //счит. значение уставки МТЗ
	tmpUstirCur = (u16)load_s16(ramPnt, progPnt->Pnt_Ustir_Cur); //считыв. юстировочное число тока

	if (tmpMTZ > tmpUstirCur) {                             //если введенное значение МТЗ больше юстировочного числа тока..
		save_s16(ramPnt, progPnt->Pnt_MTZref, tmpUstirCur); //ограничиваем значение МТЗ на значении, равном юстирвочному числу
		pWM->AdrPrm = (NUM_GRP_MTZ_UST << 7) | NUM_PRM;     //формируем адрес параметра
		pWM->s32.Value = tmpUstirCur;                       //передаем в структуру новое значение уставки МТЗ
		MakeOpis(pWM, WRITE_UST);                           //записываем ограниченное значение уставки МТЗ в FRAM
	}

	mPnt3->UstirCur			= int2f(load_s16(ramPnt, progPnt->Pnt_Ustir_Cur  ), d10);
	mPnt3->UstirCurLeakage	= int2f(load_s16(ramPnt, progPnt->Pnt_Ileak_Ustir), d10);
	mPnt3->ImtzLeakage		= int2f(load_s16(ramPnt, progPnt->Pnt_Ileak_MTZ  ), d10);
	mPnt3->UstirUdc			= load_s16(ramPnt, progPnt->Pnt_Ustir_Ud);
	mPnt3->Imtz 			= _IQ((f32)load_s16(ramPnt, progPnt->Pnt_MTZref   )*invCurBase); //_IQ(((f32)load_s16(ramPnt, progPnt->Pnt_MTZref   )) / ((f32)MPOpis.Base[NUM_CUR_BASE]));		// Значение уставки МТЗ
	// !!!!!!
	//===========================================================================

	/*
	//===========================================================================
	// !!!!!! Оригинальная вставка работы до всех изменений
	mPnt3->UstirCur			= int2f(load_s16(ramPnt, progPnt->Pnt_Ustir_Cur  ), d10); //mPnt3->UstirCur		= ((f32)((u16)load_s16(ramPnt, progPnt->Pnt_Ustir_Cur)))/10.0f;
	mPnt3->UstirCurLeakage	= int2f(load_s16(ramPnt, progPnt->Pnt_Ileak_Ustir), d10); //((f32)load_s16(ramPnt, progPnt->Pnt_Ileak_Ustir))/10.0f;
	mPnt3->ImtzLeakage		= int2f(load_s16(ramPnt, progPnt->Pnt_Ileak_MTZ  ), d10); //(f32)load_s16(ramPnt, progPnt->Pnt_Ileak_MTZ) / 10.0f;
	mPnt3->UstirUdc			= load_s16(ramPnt, progPnt->Pnt_Ustir_Ud);
	// !!!!!!
	//===========================================================================
 	*/
	mPnt3->FlagCommand.bit.EnLeakageCurrent = load_s16(ramPnt, progPnt->Pnt_CalcRegE);//разрешение бита тока утечки

	mPnt3->ImtzLeakage_IQ	= _IQ((f32)load_s16(ramPnt, progPnt->Pnt_Ileak_MTZ)*invCurBase); //_IQ(((f32)load_s16(ramPnt, progPnt->Pnt_Ileak_MTZ)) / ((f32)MPOpis.Base[NUM_CUR_BASE]));

	tmp161 = (s16)(int2f(mPnt3->UstirCur	   , (m10*4096.0f)) * invCurBase); //tmp161 =(s16)((mPnt3->UstirCur * 4096.0f * 10.0f) / ((f32)MPOpis.Base[NUM_CUR_BASE]));
	mPnt3->KustirCurOut  = tmp161; // Отношение ном тока привода к юстировочному числу

	tmp161 = (s16)(int2f(mPnt3->UstirCurLeakage, (m10*4096.0f)) * invCurBase); //tmp161 = (s16)((mPnt3->UstirCurLeakage * 4096.0f * 10.0f) / ((f32)MPOpis.Base[NUM_CUR_BASE]));
	mPnt3->KustirLeakage  = tmp161; // Отношение ном тока привода к юстировочному числу
	
	tmp161 = (s16)(int2f(mPnt3->UstirUdc, 4096.0f)*invVoltBase); //(s16)((mPnt3->UstirUdc * 4096.0f) / ((f32)MPOpis.Base[NUM_VOLT_BASE]));
	mPnt3->KustirUdc     = (s16)((tmp161 < 4*4096) ? tmp161 : (4*4096));// s16 WARNING

	UstirFunc(ramPnt, progPnt->Pnt_Iu_Korrect, progPnt->Pnt_K_Iu, &sPnt->nvCopy.adjCoefcnts.K_Iu, &sPnt->prev.Iu_Korrect, &nvAdrCoef->K_Iu, mPnt28->Iu, mPnt3->KustirCurOut, &mPnt3->IuGain);
	UstirFunc(ramPnt, progPnt->Pnt_Iv_Korrect, progPnt->Pnt_K_Iv, &sPnt->nvCopy.adjCoefcnts.K_Iv, &sPnt->prev.Iv_Korrect, &nvAdrCoef->K_Iv, mPnt28->Iv, mPnt3->KustirCurOut, &mPnt3->IvGain);
	UstirFunc(ramPnt, progPnt->Pnt_Iw_Korrect, progPnt->Pnt_K_Iw, &sPnt->nvCopy.adjCoefcnts.K_Iw, &sPnt->prev.Iw_Korrect, &nvAdrCoef->K_Iw, mPnt28->Iw, mPnt3->KustirCurOut, &mPnt3->IwGain);
	UstirFunc(ramPnt, progPnt->Pnt_Il_Korrect, progPnt->Pnt_K_Il, &sPnt->nvCopy.adjCoefcnts.K_Il, &sPnt->prev.Il_Korrect, &nvAdrCoef->K_Il, mPnt28->Il, mPnt3->KustirLeakage, &mPnt3->IlGain);
	UstirFunc(ramPnt, progPnt->Pnt_Udc_Korrect, progPnt->Pnt_K_Udc, &sPnt->nvCopy.adjCoefcnts.K_Udc, &sPnt->prev.Udc_Korrect, &nvAdrCoef->K_Udc, mPnt28->Udc, mPnt3->KustirUdc, &mPnt3->UdcGain);

	mPnt3->IuUstirCur = _IQtoF((_iq)load_s32(ramPnt, progPnt->Pnt_K_Iu)) * mPnt3->UstirCur;
	mPnt3->IvUstirCur = _IQtoF((_iq)load_s32(ramPnt, progPnt->Pnt_K_Iv)) * mPnt3->UstirCur;
	mPnt3->IwUstirCur = _IQtoF((_iq)load_s32(ramPnt, progPnt->Pnt_K_Iw)) * mPnt3->UstirCur;
	mPnt3->IlUstirCur = _IQtoF((_iq)load_s32(ramPnt, progPnt->Pnt_K_Il)) * mPnt3->UstirCurLeakage;

//-- Получение уставок
	t_1_v				= load_s16(ramPnt, progPnt->Pnt_T_1_V);
	t_2_v				= load_s16(ramPnt, progPnt->Pnt_T_2_V);
	mPnt3->Tmax_deg		= load_s16(ramPnt, progPnt->Pnt_Tmax);
	tmp161				= (s16)Fy_x1x2y1y2x(t_1_v, t_2_v, 1365, 3413, mPnt3->Tmax_deg);
	mPnt3->Tmax			= tmp161;

	if(sPnt->puskState != contyChk){
	    // !*!*!*!*
	 	mPnt3->Wmax	= load_s32(ramPnt, progPnt->Pnt_Wr_max	);								  	// Частота вращения вала ротора максимальная   рад/c
		mPnt3->Imax	=_IQ(int2f(load_s16(ramPnt, progPnt->Pnt_I_max ), m10 )*invCurBase    );    //_IQ(int2f(load_s16(ramPnt, progPnt->Pnt_I_max), d1)/moPnt->Base[NUM_CUR_BASE]*10);     // максимальный ток двигателя, А;
		mPnt3->FlagCommand.bit.WrotCtrl = load_s16(ramPnt, progPnt->Pnt_Wr__Me_2);				// Тип задания =0 момент =1 скорость
	}
	mPnt3->Isn		  = load_s32(ramPnt, progPnt->Pnt_I_motor	);								  // номинальный ток двигателя, А;
	mPnt3->Isn10	  = _IQmpy(mPnt3->Isn, _IQ(0.1));	// 0,1*Inom - для оценки генераторного режима
	mPnt3->Wnom		  = load_s32(ramPnt, progPnt->Pnt_Wr_nom	);								  // Частота вращения вала ротора номинальная    рад/с
	mPnt3->Encoder_N  = load_s32(ramPnt, progPnt->Pnt_Encoder_N	);								  // число меток датчика скорости;
	mPnt3->Enco_Shift = load_s16(ramPnt, progPnt->Pnt_EncoShift );								  // смещение нулевой позиции энкодера;
	mPnt3->Zp		  = load_s16(ramPnt, progPnt->Pnt_Zp		);								  //– число пар полюсов;
	mPnt3->Usn		  = _IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Us_nom), d10 )*invVoltBase   );   //_IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Us_nom), d10)/moPnt->Base[NUM_VOLT_BASE]); //номинальное напряжение двигателя
	mPnt3->Fnom		  = _IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Fs_nom), invHertzBase       ));   //_IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Fs_nom  ), d1)/moPnt->Base[NUM_HZ_BASE]);
	mPnt3->Kez		  = _IQ(int2f(load_s16(ramPnt, progPnt->Pnt_Kez   ), d100			    ));	  // коэффициент задания по ЭДС
	//mPnt3->KpPos	  = _IQ(d100)*load_s16(ramPnt, progPnt->Pnt_RegPos_Kp);
                                
 //суходольский расчет момента номинальн ого ;
//	 Pn= load_s16(ramPnt, progPnt->Pnt_T0e);//мощность в квт
////Pn*1000*zp/(2*fn*pi)=Pn(квт)*zp*3.18309886183791  1000/100pi=3.18309886183791
//	 Pn= Pn*mPnt3->Zp;
//	 mPnt3->Torque_n= (f32)Pn*3.18309886183791; //номинальный момент
	mPnt3->Torque_n = int2f(load_s16(ramPnt, progPnt->Pnt_RegPos_Kp), d10);  //ввод номинального момента - 03.25

	if (load_s16(ramPnt, progPnt->Pnt_Regi_Ki)){
	    mPnt3->FlagCommand.bit.ManualSpdReg = 0;
	    mPnt3->KpSpd = mPnt28->KpSpd;
	    mPnt3->KiSpd = mPnt28->KiSpd;
	    if((mPnt3->KpSpd != sPnt->prev.RegWr_Kp) || (mPnt3->KiSpd != sPnt->prev.RegWr_Ki)){
	        sPnt->prev.RegWr_Kp 	= mPnt3->KpSpd;
	        sPnt->prev.RegWr_Ki 	= mPnt3->KiSpd;
	        sPnt->nvCopy.nvParams.RegWr_Kp	= mPnt28->KpSpd;
	        sPnt->nvCopy.nvParams.RegWr_Ki	= mPnt28->KiSpd;
	        WrFramMicro(&nvAdr->RegWr_Kp, &sPnt->prev.RegWr_Kp, 2* sizeof(f32));
	    }
	    save_s16(ramPnt, progPnt->Pnt_CalcRegILim, f2int(sPnt->prev.RegWr_Kp, m100  ));
	    save_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim , f2int(sPnt->prev.RegWr_Ki, m10000));
	}
	else{
	    mPnt3->FlagCommand.bit.ManualSpdReg = 1;
	    mPnt3->KpSpd = int2f(load_s16(ramPnt, progPnt->Pnt_CalcRegILim), d100); 
	    mPnt3->KiSpd = int2f(load_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim), d10000);
	#ifdef _PROJECT_FOR_CCS_
	    if((mPnt3->KpSpd != sPnt->prev.RegWr_Kp) || (mPnt3->KiSpd != sPnt->prev.RegWr_Ki)){
	        sPnt->prev.RegWr_Kp = mPnt3->KpSpd;
	        sPnt->prev.RegWr_Ki = mPnt3->KiSpd;
	        nvAdr->RegWr_Kp      = mPnt28->KpSpd;
	        nvAdr->RegWr_Ki      = mPnt28->KiSpd;
	        WrFramMicro(&nvAdr->RegWr_Kp, &sPnt->prev.RegWr_Kp, 2* sizeof(f32));
	    }
	#endif
	}
// Пока разместил здесь, по хорошему нужно просто сделать эти параметры энергонезависимыми
// такая реализация нужна только если будет автоопределение. Или засунуть это в checkUst !!!

	mPnt3->Encoder_D = load_s16(ramPnt, progPnt->Pnt_Encoder_D);		// направление вращения датчика скорости;

	tmpF = load_float(ramPnt, progPnt->Pnt_Jr);
	if( sPnt->prev.Jr != tmpF ){
	    mPnt3->Jr     = tmpF;
	    sPnt->prev.Jr = tmpF;
	    WrFramMicro(&nvAdr->Jr, &tmpF, sizeof(s32));
	}
	tmp = load_s16(ramPnt, progPnt->Pnt_PsiRotor);		// Магнитный поток ротора
	if( (s16)sPnt->prev.PsiR != tmp ){
		mPnt3->PsiR		= int2f(tmp, d1000);
	    sPnt->prev.PsiR = tmp;
	    WrFramMicro(&nvAdr->PsiR, &tmp, sizeof(s32));
	}
//-- Выдача текущих значений
	save_s32(ramPnt, progPnt->Pnt_Uout,			mPnt28->Us			);
	save_s32(ramPnt, progPnt->Pnt_Pfull_out,	mPnt28->Pfull_out	);
	save_s32(ramPnt, progPnt->Pnt_Pact_out,		mPnt28->Pact_new);//Pact_out	);
	save_s32(ramPnt, progPnt->Pnt_Preact_out,	mPnt28->Preact_out	);
	save_s32(ramPnt, progPnt->Pnt_cosFi_out,	mPnt28->cosFi_out	);

	tmp_s16 = max3(mPnt28->TswU, mPnt28->TswV, mPnt28->TswW);
	tmp_s16 = (s16)Fy_x1x2y1y2x(1365, 3413, t_1_v, t_2_v, tmp_s16);
	save_s16(ramPnt, progPnt->Pnt_Tu		, tmp_s16		);

 	save_s32(ramPnt, progPnt->Pnt_Iact_out	, mPnt28->Iact	);
	save_s32(ramPnt, progPnt->Pnt_Ifull_out	, mPnt28->Ifull	);
	save_s32(ramPnt, progPnt->Pnt_Ireact_out, mPnt28->Ireact);
 	// Энкодер детект
	tmp_s16 = (s16)((_IQtoIQ16(mPnt28->Enco_Angle*36)*100 + _IQ16(0.5))>>16);
	save_s16(ramPnt, progPnt->Pnt_EncoderAngle, tmp_s16);
 	tmp_s16 = load_s16(ramPnt, progPnt->Pnt_F);




#ifdef _PROJECT_FOR_CCS_

 	if(GlobalM3.FlagCommand.bit.PwmOn==0){
		fL=0.0;
 	}else{
		// момент ос
 		fL=_IQtoF(GlobalC28.Mout)*GlobalM3.TorqBASE;
 	}
		sPnt->Filter1=sPnt->Filter1*0.99+fL* 0.01;//
 		save_s16(ramPnt, progPnt->Pnt_Me, (s16)sPnt->Filter1);   // Текущий электромагнитный момент Нм
		// момент задания
	if(GlobalM3.FlagCommand.bit.PwmOn==0){
		fL=0.0;
	}else{
		fL=_IQtoF(GlobalC28.Mout1)*GlobalM3.TorqBASE;//Tzad
	}
		sPnt->Filter2=sPnt->Filter2*0.99+ fL* 0.01;//  GlobalC28.MBASE
		mPnt3->Debug7=sPnt->Filter2;


#endif


//-------------------------------

 	save_s16(ramPnt, progPnt->Pnt_Reg_Wpr, (s16)(mPnt3->TorqBASE * 10.0));  // База момента
#ifdef _PROJECT_FOR_CCS_
	if (mPnt3->Revers == 1){

	     save_s32(ramPnt, progPnt->Pnt_Wr, -(s32)mPnt28->Wr);					// Текущая скорость ротора
	    save_s32(ramPnt, progPnt->Pnt_We, -(s32)mPnt28->We);					// Текущая скорость поля статора
	}else{
	    save_s32(ramPnt, progPnt->Pnt_Wr, (s32)mPnt28->Wr);					// Текущая скорость ротора
	    save_s32(ramPnt, progPnt->Pnt_We, (s32)mPnt28->We);					// Текущая скорость поля статора
	 }
	save_s32(ramPnt, progPnt->Pnt_FoutAbs, (s32)mPnt28->We); //выдаем абсолютную частоту поля (для контроля аварии)

#else //-- _PROJECT_FOR_CCS_
//#warning !Отладка не забыть! Эта часть отладочная длWRITE_TO_FRAMя Visual Studio, верхняя - рабочая
	if (mPnt3->Revers == 1){
	    save_s32(ramPnt, progPnt->Pnt_Wr, -load_s32(ramPnt, progPnt->Pnt_F));					// Текущая скорость ротора
	    save_s32(ramPnt, progPnt->Pnt_We, -load_s32(ramPnt, progPnt->Pnt_F));					// Текущая скорость поля статора
	}else{
	    save_s32(ramPnt, progPnt->Pnt_Wr, load_s32(ramPnt, progPnt->Pnt_F));					// Текущая скорость ротора
	    save_s32(ramPnt, progPnt->Pnt_We, load_s32(ramPnt, progPnt->Pnt_F));					// Текущая скорость поля статора
	}
#endif //-- _PROJECT_FOR_CCS_

	if(mPnt3->Fref < 0){
		 setBitMicro(ramPnt, progPnt->Pnt_FoutSign);}
	else{
		 clrBitMicro(ramPnt, progPnt->Pnt_FoutSign);}
	
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////Вкл/Выкл Измерения параметров////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	
	tmp161 = load_s16(ramPnt, progPnt->Pnt_CalcPrmMot );
#ifdef _PROJECT_FOR_CCS_
#warning !!!Не забыть, реализовано на уровне командоапарата!!!
#endif

//================авария предупреждение обмена -энкодера Суходольский
//===================================================================
	mode = load_s16(ramPnt, progPnt->Pnt_T_ad_Lm);//85 авария энкодера

	if(GlobalM3.FlagCommand.bit.BitEnable_J!=0){
		mode= MODE_OFF;
	}
	switch(mode)
	{
	   case  MODE_OFF:
			clrBitMicro(ramPnt, progPnt->Pnt_Reg_Wr_Out); // 141 avaria encoder
			clrBitMicro(ramPnt, progPnt->Pnt_RegE_Out);  //143 warning encoder
			GlobalM3.FlagCommand.bit.BitJumpToNoEncod=0;
	   break;
	   case  MODE_FAIL:
		if((GlobalC28.ErrorEncoder !=0)||(GlobalC28.AvariaEncoder!=0)){
		 	setBitMicro(ramPnt, progPnt->Pnt_Reg_Wr_Out);// } // 141 avaria encoder
		} else {
			clrBitMicro(ramPnt, progPnt->Pnt_Reg_Wr_Out); // 141 avaria encoder
		}
		clrBitMicro(ramPnt, progPnt->Pnt_RegE_Out);  //143 warning encoder
		GlobalM3.FlagCommand.bit.BitJumpToNoEncod=0;
	   break;
	   case MODE_WARN:
		   if((GlobalC28.ErrorEncoder !=0)||(GlobalC28.AvariaEncoder!=0)){
			 	setBitMicro(ramPnt, progPnt->Pnt_RegE_Out);  //143 warning encoder
		   } else {
				clrBitMicro(ramPnt, progPnt->Pnt_RegE_Out);  //143 warning encoder
		   }
		   clrBitMicro(ramPnt, progPnt->Pnt_Reg_Wr_Out); // 141 avaria encoder
		   GlobalM3.FlagCommand.bit.BitJumpToNoEncod=0;
	   break;
	   case MODE_GO_TO_NO_ENCODER:
		   //============================Сделать по аварии датчика скорости bit.BitJumpToNoEncod=1 при дачиковом режиме и аварии датчика - переход в бездатчиковый

 	 	   if(mPnt3->FlagCommand.bit.PwmOn){
 		         if(GlobalC28.AvariaEncoder!=0){
 	 		    		GlobalM3.FlagCommand.bit.BitJumpToNoEncod=1; //признак перехода в бд режим
 		   			    setBitMicro(ramPnt, progPnt->Pnt_RegE_Out);  //143 warning encoder
 		   			    clrBitMicro(ramPnt, progPnt->Pnt_Reg_Wr_Out);
 		   			//}
  		   		} else {
 		   			clrBitMicro(ramPnt, progPnt->Pnt_Reg_Wr_Out); // 141 avaria encoder
 		   	    	clrBitMicro(ramPnt, progPnt->Pnt_RegE_Out);
 		   			sPnt->count_ERROR = K_TIME_CALL_MICRO_TAKT * 5; //time_error;//5sec
 		   			GlobalM3.FlagCommand.bit.BitJumpToNoEncod=0;
 		   		}
 	 	   }else{
 	 		  if(GlobalM3.FlagCommand.bit.BitJumpToNoEncod){
 	 		     	setBitMicro(ramPnt, progPnt->Pnt_Reg_Wr_Out);
      		      	GlobalM3.FlagCommand.bit.BitJumpToNoEncod=0;
      	      }else{
      	    	    clrBitMicro(ramPnt, progPnt->Pnt_Reg_Wr_Out);
      	       }
 	 		        clrBitMicro(ramPnt, progPnt->Pnt_RegE_Out);
      	   }



	   break;

	}

	checkUstS16(Rr, d1000);		// Текущее значение активного сопротивления ротора Rr (Ом)
	checkUstS16(Rs, d1000);		// Текущее значение активного сопротивления статора Rs (Ом)
	checkUstS16(Lm, d10000);	// Индуктивность намагничивания Lm (Гн)
	checkUstF32(Lls);   		// Индуктивность рассеяния Lls (Гн)
	checkUstS16(Lsd, d10000);	// Индуктивность статора по оси D
	checkUstS16(Lsq, d10000);	// Индуктивность статора по оси Q
	checkUstS16(Ixx, d10);	// Холостой ход двигателя
	
	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	
	mPnt3->FlagCommand.bit.CurrInv  		= load_s16(ramPnt, progPnt->Pnt_InvCurSens); 	// инверсия токов
	mPnt3->FlagCommand.bit.EMFRegEnb 		= load_s16(ramPnt, progPnt->Pnt_T0wr); 			// вкл/выкл ослабления поля
	mPnt3->FlagCommand.bit.FiltrRefSpdEnb	= load_s16(ramPnt, progPnt->Pnt_Reg_Iq_Lim);	// вкл/выкл фильтра задания частоты
	mPnt3->FlagCommand.bit.ARollbackEnb		= load_s16(ramPnt, progPnt->Pnt_Tfwrz);		// вкл/выкл Антиоткатной функции
	mPnt3->Encoder_Type  					= load_s16(ramPnt, progPnt->Pnt_EncoType);		// Выбор типа энкодера
	
	mPnt3->TregSpd 							= (f32)load_s16(ramPnt, progPnt->Pnt_T0is) / 10000.0f;
	mPnt3->FcutSpd							= (f32)load_s16(ramPnt, progPnt->Pnt_CalcRegI);
	mPnt3->CompDelaySpd						= (f32)load_s16(ramPnt, progPnt->Pnt_Regi_Kp) / 10.0f;
	//mPnt3->AntiForceSpd						= (f32)load_s16(ramPnt, progPnt->Pnt_RegWr_Kp) / 10.0f;
	mPnt3->KpForced                         = int2f(load_s16(ramPnt, progPnt->Pnt_RegWr_Kp), d100);
	mPnt3->KiForced	                        = int2f(load_s16(ramPnt, progPnt->Pnt_RegWr_Ki), d10000);

	mPnt3->RejectFreq                   	= int2f(load_s16(ramPnt, progPnt->Pnt_BSFilter_Fr), d10); // XXX,X
	mPnt3->RejectBand                   	= int2f(load_s16(ramPnt, progPnt->Pnt_BSFilter_dF), d10); // XXX,X

	mPnt3->AntiTime     					= (f32)load_s16(ramPnt, progPnt->Pnt_CalcRegWr)/ 10.0f;
	mPnt3->PWMType      					= load_s16(ramPnt, progPnt->Pnt_PwmType);
	mPnt3->FlagCommand.bit.TwoCurrent		= load_s16(ramPnt, progPnt->Pnt_CurSensKolv);
	

	if(moPnt->cntCall >= 4*K_TIME_CALL_MICRO_TAKT) mPnt3->FlagState.bit.DRVInitComplete = 1;	//4s на старт АЦП	// cntCall==0 при flagIsFirstCall ==1
	if(flagIsFirstCall || mPnt3->FlagCommand.bit.DrvReInit){ SysCalc_Start(); mPnt3->FlagCommand.bit.DrvReInit = 0;}
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDRV_V5_1[144]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Pusk
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Stop
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  StopVibeg
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  FailTek
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  SelTormType
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  VibegTime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  TypeDrv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Ftek
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  Fpusk
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  SoftCurDown
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  StateGT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  U
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Napravlenie
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  Wr/~Me_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Wr/~Me_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Ud_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  K_Ud
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19  Iu_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  K_Iu
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  Iv_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22  K_Iv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //23  Iw_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //24  K_Iw
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  T_1_V
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  T_2_V
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  Tmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Ustir_Cur
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  MTZref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  CurSensKolv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  InvCurSens
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  Ustir_Ud
        S32_PIN_TYPE | INPUT_PIN_MODE,     //33  I_motor
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  I_max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  TypeDrv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  Dtime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  Fpwm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38  PwmType
        S32_PIN_TYPE | INPUT_PIN_MODE,     //39  K_Ileak
        S32_PIN_TYPE | INPUT_PIN_MODE,     //40  Ileak_Korect
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41  Ileak_MTZ			//Заменил на S16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  Ileak_Ustir		//Заменил на S16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43  EncoType
        S32_PIN_TYPE | INPUT_PIN_MODE,     //44  Encoder_N
        S16_PIN_TYPE | INPUT_PIN_MODE,     //45  Encoder_D
		S16_PIN_TYPE | INPUT_PIN_MODE,     //46  EncoShift
        S16_PIN_TYPE | INPUT_PIN_MODE,     //47  EncoderFilter
        S16_PIN_TYPE | INPUT_PIN_MODE,     //48  PsiRotor
        S16_PIN_TYPE | INPUT_PIN_MODE,     //49  Lsd
        S16_PIN_TYPE | INPUT_PIN_MODE,     //50  Lsq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //51  Reset_Default
        S32_PIN_TYPE | INPUT_PIN_MODE,     //52  Wr_z
        S16_PIN_TYPE | INPUT_PIN_MODE,     //53  Me_z
        S32_PIN_TYPE | INPUT_PIN_MODE,     //54  Wr_nom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //55  Wr_max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //56  Fs_nom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //57  Us_nom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //58  CalcPrmMot
        S16_PIN_TYPE | INPUT_PIN_MODE,     //59  Rr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //60  Rs
        S16_PIN_TYPE | INPUT_PIN_MODE,     //61  Lm
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //62  SigmaLs
        S16_PIN_TYPE | INPUT_PIN_MODE,     //63  Zp
        S32_PIN_TYPE | INPUT_PIN_MODE,     //64  Adapt_Tr_Ls
        S16_PIN_TYPE | INPUT_PIN_MODE,     //65  Calc_J
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //66  Jr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //67  T0is
        S16_PIN_TYPE | INPUT_PIN_MODE,     //68  CalcRegI
        S16_PIN_TYPE | INPUT_PIN_MODE,     //69  Regi_Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //70  Regi_Ki
        S16_PIN_TYPE | INPUT_PIN_MODE,     //71  CalcRegILim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //72  Reg_Id_Lim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //73  Reg_Iq_Lim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //74  T0wr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //75  Tfwrz
        S16_PIN_TYPE | INPUT_PIN_MODE,     //76  CalcRegWr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //77  RegWr_Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //78  RegWr_Ki
        S16_PIN_TYPE | INPUT_PIN_MODE,     //79  CalcRegWrLim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //80  Pnt_BSFilter_Fr // Замена RegWr_Lim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //81  Pnt_BSFilter_dF // Замена Wr_od
        S16_PIN_TYPE | INPUT_PIN_MODE,     //82  K_od
        S16_PIN_TYPE | INPUT_PIN_MODE,     //83  Tiwe
        S32_PIN_TYPE | INPUT_PIN_MODE,     //84  OgrRegWpr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //85  T_ad_Lm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //86  T0e
        S16_PIN_TYPE | INPUT_PIN_MODE,     //87  Kez
        S16_PIN_TYPE | INPUT_PIN_MODE,     //88  CalcRegE
        S16_PIN_TYPE | INPUT_PIN_MODE,     //89  RegE_Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //90  RegE_Ki
        S16_PIN_TYPE | INPUT_PIN_MODE,     //91  RegE_KRef
        S16_PIN_TYPE | INPUT_PIN_MODE,     //92  CalcRegELim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //93  RegE_UpLim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //94  RegE_LowLim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //95  Reset_Default
        S16_PIN_TYPE | INPUT_PIN_MODE,     //96  Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //97  Ti
        S16_PIN_TYPE | INPUT_PIN_MODE,     //98  Td
        S16_PIN_TYPE | INPUT_PIN_MODE,     //99  DifFiltr
        S32_PIN_TYPE | INPUT_PIN_MODE,     //100  Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //101  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //102  TempMax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //103  ZerHzDc
        S32_PIN_TYPE | INPUT_PIN_MODE,     //104  F_DCSTP
        S16_PIN_TYPE | INPUT_PIN_MODE,     //105  Time_STP
        S32_PIN_TYPE | INPUT_PIN_MODE,     //106  UinDC
        S32_PIN_TYPE | INPUT_PIN_MODE,     //107  CurTormDc
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //108  Dry_On
        S16_PIN_TYPE | INPUT_PIN_MODE,     //109  DryEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //110  SrcEnSel
        S16_PIN_TYPE | INPUT_PIN_MODE,     //111  TimeDRY
        S32_PIN_TYPE | INPUT_PIN_MODE,     //112  Idry
        S32_PIN_TYPE | INPUT_PIN_MODE,     //113  Iru
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //114  DinSrc		//Заменили вход на RUena
        S16_PIN_TYPE | INPUT_PIN_MODE,     //115  tormCheck
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //116  PwmOn
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //117  NotPwmOn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //118  StopFreq
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //119  StopByDc
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //120  WorkRunMode
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //121  NormStopMode
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //122  Uout
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //123  Pfull_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //124  Pact_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //125  Preact_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //126  cosFi_out
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //127  Tu
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //128  Iact_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //129  Ifull_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //130  Ireact_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //131  We
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //132  Wr
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //133  Me
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //134  Reg_Wpr
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //135  FoutSign
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //136  FoutAbs
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //137  Заменили на StatusDRV		//BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //132  ScalarDrv
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //138  EncoderDetect
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //139  EncoderAngle
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //140  Reg_Iq_Out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //141  Reg_Wr_Out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //142  RegE_Ref
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //143  RegE_Out
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //144  StatusDC
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DRV_V5", "U", TblFuncDRV_V5_1, FuncDRV_V5_1, sizeof(TblFuncDRV_V5_1), sizeof(DRVV5_State_type), sizeof(nvDRVV5_State_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDRV_V5_1,
#endif
//********************************************************
