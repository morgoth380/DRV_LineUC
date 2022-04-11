//#include "journal.h"
//#include    "vector.h"
#include    "DRV_VECT_ALL_macro.h"
#include    "FAIL_CONTROL.h"


#define ENCO_TYPE_INCREMENTAL      0 // Инкрементальный энкодер
#define ENCO_TYPE_ABS_ENDAT_2_0    1 // Абсолютный энкодер с протоколом EnDAT 2.0
#define ENCO_TYPE_ABS_ENDAT_2_1    2 // Абсолютный энкодер с протоколом EnDAT 2.1
#define ENCO_COS_SIN               3 // sin/cos энкодер
//====================================================================
#ifdef    GET_FUNC_CODE
//====================================================================
extern u16          PwmEn;
extern const  u16   offZarFail[80];
extern const  u16   offZarFailStop[80];
s16                 TimeOffZar;
f32                 Fbase_rad_s;// = 167.0f;
//====================================================================
#define Wr_IQtoF(Wr)  (Fbase_rad_s *_IQtoF(Wr))         // Приведение скорости из IQ в рад/сек  Wr - в IQ формате привязано к числу 80.0 Гц
#define Wr_FtoIQ(Wr)  _IQ( Wr / Fbase_rad_s)               // Приведение скорости из рад/сек в IQ   Wr - в IQ формате привязано к числу 80.0 Гц
//====================================================================
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
    f32     Lls    ;
    f32     Jr         ;
    f32     RegWr_Kp    ;
    f32     RegWr_Ki    ;
    s16     Encoder_D  ;

// -3-
}nvStateFuncDRV_VECT_ALL_type;

#define     nvSfDRV     (*(nvStateFuncDRV_VECT_ALL_type*)0)

#define sizeNvSkal  (5*sizeof(_iq))
#define addrNvVect  &nvSfDRV.Rr
#define sizeNvVect  (8*sizeof(s32))
//====================================================================
typedef struct{
    u16     RsDef       :1;
    u16     JDef        :1;
    u16     AutoFase    :1;
}vectAutoFlags_type;
//====================================================================
typedef struct{
    struct{
// -1-
        nvStateFuncDRV_VECT_ALL_type     nvCopy;
        _iq     Iu_Korrect;
        _iq     Iv_Korrect;
        _iq     Iw_Korrect;
        _iq     Udc_Korrect;
        _iq     Il_Korrect;

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
        s32                 Rr              ;
        s32                 Rs              ;
        s32                 Lm              ;
        f32                 Lls         ;
        f32                 Jr              ;
        f32     RegWr_Kp    ;
        f32     RegWr_Ki    ;
        s16                 Encoder_D       ;
        vectAutoFlags_type  vectAutoFlags   ;
        u16     StateDC;
    }prev;
}StateFuncDRV_VECT_ALL_type;
//****************************************************************************
/*===========================================================================*/
// Преобразование int <-> float
/*===========================================================================*/
#define m1          1.
#define m10         10.
#define m100        100.
#define m1000       1000.
#define m10000      10000.

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
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_F;                            //1  F
    u16  Pnt_U;                            //2  U
    u16  Pnt_Dtime;                        //3  Dtime
    u16  Pnt_Fpwm;                         //4  Fpwm
    u16  Pnt_ResetAvar;                    //5  ResetAvar
    u16  Pnt_PWM_On;                       //6  PWM_On
    u16  Pnt_ChargeOk;                     //7  ChargeOk
    u16  Pnt_Revers;                       //8  Revers
    u16  Pnt_Wr__Me;                       //9  Wr/~Me
    u16  Pnt_Torm_const_I;                 //10  Torm_const_I
    u16  Pnt_Udc_Korrect;                   //11  Ud_Korrect
    u16  Pnt_K_Udc;                         //12  K_Ud
    u16  Pnt_Iu_Korrect;                   //13  Iu_Korrect
    u16  Pnt_K_Iu;                         //14  K_Iu
    u16  Pnt_Iv_Korrect;                   //15  Iv_Korrect
    u16  Pnt_K_Iv;                         //16  K_Iv
    u16  Pnt_Iw_Korrect;                   //17  Iw_Korrect
    u16  Pnt_K_Iw;                         //18  K_Iw
    u16  Pnt_MTZref;                       //19  MTZref
    u16  Pnt_Tmax;                         //20  Tmax
    u16  Pnt_Udmax;                        //21  Udmax
    u16  Pnt_Udmin;                        //22  Udmin
    u16  Pnt_Ustir_Cur;                   //23  Ustir_Cur
    u16  Pnt_Ustir_Ud;                     //24  Ustir_Ud
    u16  Pnt_ExternFail;                   //25  ExternFail
    u16  Pnt_START;                        //26  START
    u16  Pnt_TypeDrv;                      //27  TypeDrv
    u16  Pnt_Ifull_fval;                   //28  Ifull_fval
    u16  Pnt_Iact_fval;                    //29  Iact_fval
    u16  Pnt_Iw_fval;                      //30  Iw_fval
    u16  Pnt_Iv_fval;                      //31  Iv_fval
    u16  Pnt_Iu_fval;                      //32  Iu_fval
    u16  Pnt_Ud_fval;                      //33  Ud_fval
    u16  Pnt_Status_fval;                  //34  Status_fval
    u16  Pnt_Fout_fval;                    //35  Fout_fval
    u16  Pnt_Status;                       //36  Status
    u16  Pnt_NumAct;                       //37  NumAct
    u16  Pnt_FailMask_1;                   //38  FailMask_1
    u16  Pnt_FailMask_0;                   //39  FailMask_0
    u16  Pnt_Ireact_out;                   //40  Ireact_out
    u16  Pnt_Ifull_out;                    //41  Ifull_out
    u16  Pnt_Iact_out;                     //42  Iact_out
    u16  Pnt_Tu;                           //43  Tu
    u16  Pnt_cosFi_out;                    //44  cosFi_out
    u16  Pnt_Pact_out;                     //45  Pact_out
    u16  Pnt_Pfull_out;                    //46  Pfull_out
    u16  Pnt_Uout;                         //47  Uout
    u16  Pnt_OFF_ZAR;                      //48  OFF_ZAR
    u16  Pnt_I_motor;                      //49  I_motor
    u16  Pnt_Pr_Up;                        //50  Pr_Up
    u16  Pnt_T_1_V;                        //51  T_1_V
    u16  Pnt_T_2_V;                        //52  T_2_V
    u16  Pnt_Wr_z;                         //53  Wr_z
    u16  Pnt_Me_z;                         //54  Me_z
    u16  Pnt_Wr_nom;                       //55  Wr_nom
    u16  Pnt_Wr_max;                       //56  Wr_max
    u16  Pnt_Fs_nom;                       //57  Fs_nom
    u16  Pnt_Us_nom;                       //58  Us_nom
    u16  Pnt_Calc_Rs_sigmaLs_Lm;           //59  Calc_Rs_sigmaLs_Lm
    u16  Pnt_Rr;                           //60  Rr
    u16  Pnt_Rs;                           //61  Rs
    u16  Pnt_Lm;                           //62  Lm
    u16  Pnt_Lls;                          //63  Lls
    u16  Pnt_Zp;                           //64  Zp
    u16  Pnt_Adapt_Tr_Ls;                  //65  Adapt_Tr_Ls
    u16  Pnt_Calc_J;                       //66  Calc_J
    u16  Pnt_Jr;                           //67  Jr
    u16  Pnt_T0is;                         //68  T0is
    u16  Pnt_CalcRegI;                     //69  CalcRegI
    u16  Pnt_Regi_Kp;                      //70  Regi_Kp
    u16  Pnt_Regi_Ki;                      //71  Regi_Ki
    u16  Pnt_CalcRegILim;                  //72  CalcRegILim
    u16  Pnt_Reg_Id_Lim;                   //73  Reg_Id_Lim
    u16  Pnt_Reg_Iq_Lim;                   //74  Reg_Iq_Lim
    u16  Pnt_T0wr;                         //75  T0wr
    u16  Pnt_Tfwrz;                        //76  Tfwrz
    u16  Pnt_CalcRegWr;                    //77  CalcRegWr
    u16  Pnt_RegWr_Kp;                     //78  RegWr_Kp
    u16  Pnt_RegWr_Ki;                     //79  RegWr_Ki
    u16  Pnt_CalcRegWrLim;                 //80  CalcRegWrLim
    u16  Pnt_RegWr_Lim;                    //81  RegWr_Lim
    u16  Pnt_Wr_od;                        //82  Wr_od
    u16  Pnt_K_od;                         //83  K_od
    u16  Pnt_T0e;                          //84  T0e
    u16  Pnt_Kez;                          //85  Kez
    u16  Pnt_CalcRegE;                     //86  CalcRegE
    u16  Pnt_RegE_Kp;                      //87  RegE_Kp
    u16  Pnt_RegE_Ki;                      //88  RegE_Ki
    u16  Pnt_RegE_KRef;                    //89  RegE_KRef
    u16  Pnt_CalcRegELim;                  //90  CalcRegELim
    u16  Pnt_RegE_UpLim;                   //91  RegE_UpLim
    u16  Pnt_RegE_LowLim;                  //92  RegE_LowLim
    u16  Pnt_Reset_Default;                //93  Reset_Default
    u16  Pnt_RegE_Ref;                     //94  RegE_Ref
    u16  Pnt_RegE_Out;                     //95  RegE_Out
    u16  Pnt_Reg_Wr_Out;                   //96  Reg_Wr_Out
    u16  Pnt_Reg_Iq_Out;                   //97  Reg_Iq_Out
    u16  Pnt_Reg_Id_Out;                   //98  Reg_Id_Out
    u16  Pnt_We;                           //99  We
    u16  Pnt_Ar;                           //100  Ar
    u16  Pnt_Me;                           //101  Me
    u16  Pnt_Wr;                           //102  Wr
    u16  Pnt_START_def;                    //103  START_def
    u16  Pnt_WrzAccel1Up;                  //104  WrzAccel1Up
    u16  Pnt_WrzAccel1Dn;                  //105  WrzAccel1Dn
    u16  Pnt_WrzAccel2Up;                  //106  WrzAccel2Up
    u16  Pnt_Il_Korrect;                   //107  WrzAccel2Dn
    u16  Pnt_Encoder_N;                    //108  Encoder_N
    u16  Pnt_Encoder_D;                    //109  Encoder_D
    u16  Pnt_Phase_D_Wr;                   //110  Phase_D_Wr
    u16  Pnt_T0_Tr;                        //111  T0_Tr
    u16  Pnt_Tr_est_Max;                   //112  Tr_est_Max
    u16  Pnt_Tr_est_Min;                   //113  Tr_est_Min
    u16  Pnt_I_max;                        //114  I_max
    u16  Pnt_K_Il;                         //115  R_br
    u16  Pnt_P_br;                         //116  P_br
    u16  Pnt_K_br;                         //117  K_br
    u16  Pnt_T_br;                         //118  T_br
    u16  Pnt_I2T;                          //119  I2T
    u16  Pnt_K_I2T;                        //120  K_I2T
    u16  Pnt_T_I2T;                        //121  T_I2T
    u16  Pnt_Nr_Nr_max;                    //122  Nr_Nr_max
    u16  Pnt_N_ErrReset;                   //123  N_ErrReset
    u16  Pnt_T_ErrReset;                   //124  T_ErrReset
    u16  Pnt_N_ErrDetect;                  //125  N_ErrDetect
    u16  Pnt_Reset_Default_1;              //126  Reset_Default
    u16  Pnt_Udc_br_off;                   //127  Udc_br_off
    u16  Pnt_Udc_br_on;                    //128  Udc_br_on
    u16  Pnt_Udc_min;                      //129  Udc_min
    u16  Pnt_Udc_max;                      //130  Udc_max
    u16  Pnt_Tr_est;                       //131  Tr_est
    u16  Pnt_Tr0;                          //132  Tr0
    u16  Pnt_Fz_RegLvl;                    //133  Fz_RegLvl
    u16  Pnt_Fz_FixLvl;                    //134  Fz_FixLvl
    u16  Pnt_Isz;                          //135  Isz
    u16  Pnt_Idry;                         //136  Idry
    u16  Pnt_Iru;                          //137  Iru
    u16  Pnt_StatusDC;                     //138  StatusDC
    u16  Pnt_TimeOffZar;                   //139  TimeOffZar
    u16  Pnt_Tiwe;                         //140  Tiwe
    u16  Pnt_OgrRegWpr;                    //141  OgrRegWpr
    u16  Pnt_T_ad_Lm;                      //142  T_ad_Lm
    u16  Pnt_In_rez1;                      //143  In_rez1
    u16  Pnt_In_rez2;                      //144  In_rez2
    u16  Pnt_In_rez3;                      //145  In_rez3
    u16  Pnt_Out_rez1;                     //146  Out_rez1
    u16  Pnt_Out_rez2;                     //147  Out_rez2
    u16  Pnt_Me_Base;                      //148  Reg_Wpr
    u16  Pnt_Ud_fast;                      //149  Ud_fast
    u16  Pnt_Off_PWM;                      //150  OFF_PWM
    u16  Pnt_End;
}FuncDRV_VECT_ALL_type;
//******************************************************************************
// Функция  Функция  Функция  Функция  Функция  Функция  Функция  Функция
//******************************************************************************

extern  f32 Wr_z_test;
s16 TypeDrv;

u16 *FuncDRV_VECT_ALL_1(FuncDRV_VECT_ALL_type *progPnt, u32 ramPnt)
{

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDRV_VECT_ALL_1[150]={
// -1- ======================================================================================
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  U
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Dtime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Fpwm
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  ResetAvar
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  PWM_On
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  ChargeOk
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  StartUd
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Wr/~Me
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Torm_const_I
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  Ud_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  K_Ud
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  Iu_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  K_Iu
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  Iv_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  K_Iv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Iw_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  K_Iw
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //19  MTZref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Tmax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  Udmax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22  Udmin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Ustir_Cur
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  Ustir_Ud
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  ExternFail
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //26  START
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  TypeDrv
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  Ifull_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  Iact_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  Iw_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  Iv_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  Iu_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //33  Ud_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //34  Status_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //35  Fout_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //36  Status
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //37  NumAct
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //38  FailMask_1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //39  FailMask_0
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //40  Ireact_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //41  Ifull_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //42  Iact_out
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //43  Tu
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //44  cosFi_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //45  Pact_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //46  Pfull_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //47  Uout
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //48  OFF_ZAR
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //49  I_motor
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //50  Pr_Up
// -2- ======================================================================================
        S16_PIN_TYPE | INPUT_PIN_MODE,     //51  T_1_V
        S16_PIN_TYPE | INPUT_PIN_MODE,     //52  T_2_V
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //53  Wr_z РЕЗЕРВ см ногу 1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //54  Me_z
        S32_PIN_TYPE | INPUT_PIN_MODE,     //55  Wr_nom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //56  Wr_max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //57  Fs_nom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //58  Us_nom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //59  Calc_Rs_SigmaLs_Lm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //60   Rr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //61   Rs
        S16_PIN_TYPE | INPUT_PIN_MODE,     //62   Lm
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //63   Lls
        S16_PIN_TYPE | INPUT_PIN_MODE,     //64   Zp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //65   Adapt_Tr_Ls
        S16_PIN_TYPE | INPUT_PIN_MODE,     //66   Calc_J
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //67   Jr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //68   T0is
        S16_PIN_TYPE | INPUT_PIN_MODE,     //69   CalcRegI
        S16_PIN_TYPE | INPUT_PIN_MODE,     //70   Regi_Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //71   Regi_Ki
        S16_PIN_TYPE | INPUT_PIN_MODE,     //72   CalcRegILim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //73   Reg_Id_Lim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //74   Reg_Iq_Lim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //75   T0wr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //76   Tfwrz
        S16_PIN_TYPE | INPUT_PIN_MODE,     //77   CalcRegWr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //78   RegWr_Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //79   RegWr_Ki
        S16_PIN_TYPE | INPUT_PIN_MODE,     //80   CalcRegWrLim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //81   RegWr_Lim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //82   Wr_od
        S16_PIN_TYPE | INPUT_PIN_MODE,     //83   K_od
        S16_PIN_TYPE | INPUT_PIN_MODE,     //84   T0e
        S16_PIN_TYPE | INPUT_PIN_MODE,     //85   Kez
        S16_PIN_TYPE | INPUT_PIN_MODE,     //86   CalcRegE
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //87   RegE_Kp
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //88   RegE_Ki
        S16_PIN_TYPE | INPUT_PIN_MODE,     //89   RegE_KRef
        S16_PIN_TYPE | INPUT_PIN_MODE,     //90   CalcRegELim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //91   RegE_UpLim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //92   RegE_LowLim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //93   Reset_Default
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //94   RegE_Ref
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //95   RegE_Out
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //96   Reg_Wr_Out
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //97   Reg_Iq_Out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //98   Reg_Id_Out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //99   We
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //100  Ar
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //101  Me
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //102  Wr
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //103  START_def
// -3- ======================================================================================
        S16_PIN_TYPE | INPUT_PIN_MODE,     //104  WrzAccel1Up
        S16_PIN_TYPE | INPUT_PIN_MODE,     //105  WrzAccel1Dn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //106  WrzAccel2Up
        S32_PIN_TYPE | INPUT_PIN_MODE,     //107  WrzAccel2Dn
         S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Encoder_N
//         S16_PIN_TYPE | INPUT_PIN_MODE,    //108 Encoder_N
        S16_PIN_TYPE | INPUT_PIN_MODE,     //109 Encoder_D
        S16_PIN_TYPE | INPUT_PIN_MODE,     //110 Phase_D_Wr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //111 T0_Tr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //112 Tr_est_Max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //113  Tr_est_Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //114  I_max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //115  R_br
        S16_PIN_TYPE | INPUT_PIN_MODE,     //116  P_br
        S16_PIN_TYPE | INPUT_PIN_MODE,     //117  K_br
        S16_PIN_TYPE | INPUT_PIN_MODE,     //118  T_br
        S16_PIN_TYPE | INPUT_PIN_MODE,     //119  I2T
        S16_PIN_TYPE | INPUT_PIN_MODE,     //120  K_I2T
        S16_PIN_TYPE | INPUT_PIN_MODE,     //121  T_I2T
        S16_PIN_TYPE | INPUT_PIN_MODE,     //122  Nr_Nr_max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //123  N_ErrReset
////         FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //21  T_ErrReset
         S16_PIN_TYPE | INPUT_PIN_MODE,    //124  T_ErrReset
        S16_PIN_TYPE | INPUT_PIN_MODE,     //125  N_ErrDetect
        S16_PIN_TYPE | INPUT_PIN_MODE,     //126  Reset_Default
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //127  Udc_br_off
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //128  Udc_br_on
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //129  Udc_min
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //130  Udc_max
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //131  Tr_est
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //132  Tr0
// -4- ======================================================================================
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //133  Fz_RegLvl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //134  Fz_FixLvl
        S32_PIN_TYPE | INPUT_PIN_MODE,     //135  Isz
        S32_PIN_TYPE | INPUT_PIN_MODE,     //136  Idry
        S32_PIN_TYPE | INPUT_PIN_MODE,     //137  Iru
        S16_PIN_TYPE | INPUT_PIN_MODE,     //138  StatusDC
        S16_PIN_TYPE | INPUT_PIN_MODE,     //139  TimeOffZar
        S16_PIN_TYPE | INPUT_PIN_MODE,     //140  Tiwe
        S32_PIN_TYPE | INPUT_PIN_MODE,     //141  OgrRegWpr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //142  T_ad_Lm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //143  In_rez1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //144  In_rez2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //145  In_rez3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //146  Out_rez2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //147  Out_rez1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //148  Reg_Wpr
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //149  Ud_fast
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //150  OFF_PWM
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DRV_VECT_ALL", "DA", TblFuncDRV_VECT_ALL_1, FuncDRV_VECT_ALL_1, sizeof(TblFuncDRV_VECT_ALL_1),
sizeof(StateFuncDRV_VECT_ALL_type), sizeof(nvStateFuncDRV_VECT_ALL_type), 1 ),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDRV_VECT_ALL_1,
#endif
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
// Массив что делать с зарядом при различных авариях
// 1= Выключять зарядное реле
// 0= Не выключать зарядное реле
const  u16 offZarFail[80]={
1,      //0
1,      //1  Ошибка по сигналу от силового ключа U
1,      //2  Ошибка по сигналу от силового ключа V
1,      //3  Ошибка по сигналу от силового ключа W
1,      //4  Ошибка по сигналу от силового тормозного ключа
1,      //5  Аналоговая МТЗ по фазе U
1,      //6  Аналоговая МТЗ по фазе V
1,      //7  Аналоговая МТЗ по фазе W
1,      //8  Программная МТЗ по фазе U
1,      //9  Программная МТЗ по фазе V
1,      //10 Программная МТЗ по фазе W
1,      //11 Низкое Ud
1,      //12 Высокое Ud
1,      //13 Зарядное время Ud
1,      //14 Температура ключей
1,      //15 резерв

1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // 16-31 РЕЗЕРВ

1,      //32 STOP_KEY
1,      //33 FAILUREA
0,      //34 Авар. контактор
0,      //35 Авар. торм сопр
0,      //36 FAZIROVKA_ERR
1,      //37 ERR_ZAR_TIME
0,      //38 IMAX_ERR
0,      //39 AIN_1_ERR
0,      //40 AIN_2_ERR
0,      //41 F_MAX_ERR
0,      //42 F_MIN_ERR
0,      //43 FAIL_U
0,      //44 FAIL_V
0,      //45 FAIL_W
0,      //46 резерв
0,      //47 ERR_MB_CONNECT

1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,    // 48-63 РЕЗЕРВ
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1     // 64-79 РЕЗЕРВ

};
// для выключения заряда в останове
const  u16 offZarFailStop[80]={
        0,      //0
        0,      //1  Ошибка по сигналу от силового ключа U
        0,      //2  Ошибка по сигналу от силового ключа V
        0,      //3  Ошибка по сигналу от силового ключа W
        0,      //4  Ошибка по сигналу от силового тормозного ключа
        0,      //5  Аналоговая МТЗ по фазе U
        0,      //6  Аналоговая МТЗ по фазе V
        0,      //7  Аналоговая МТЗ по фазе W
        0,      //8  Программная МТЗ по фазе U
        0,      //9  Программная МТЗ по фазе V
        0,      //10 Программная МТЗ по фазе W
        1,      //11 Низкое Ud
        1,      //12 Высокое Ud
        1,      //13 Зарядное время Ud
        0,      //14 Температура ключей
        1,      //15 резерв

1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // 16-31 РЕЗЕРВ

1,      //32 STOP_KEY
1,      //33 FAILUREA
0,      //34 FAILUREB
0,      //35 FAILUREC
0,      //36 FAZIROVKA_ERR
1,      //37 ERR_ZAR_TIME
0,      //38 IMAX_ERR
0,      //39 AIN_1_ERR
0,      //40 AIN_2_ERR
0,      //41 F_MAX_ERR
0,      //42 F_MIN_ERR
0,      //43 FAIL_U
0,      //44 FAIL_V
0,      //45 FAIL_W
0,      //46 резерв
0,      //47 ERR_MB_CONNECT

1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,    // 48-63 РЕЗЕРВ
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1     // 64-79 РЕЗЕРВ

};
#endif
