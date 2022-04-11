/*=====================================================================================
 File name:    FOC_PARAMS_COPY.H  (float version)                                
 Originator:   Triol corporation
 Description:  Parameter copy macros               
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 26-08-2012 Version 1.00
 24-09-2020 
-------------------------------------------------------------------------------------*/
#include "GlobalVar.h"
#include "CommonMicro.h"


#define FOC_Params_Copy_from_M4() \
    DrvConst.DriveType     = GlobalM4.DriveType;        \
    DrvConst.Fpwm          = GlobalM4.Fpwm;             \
                                                        \
    DrvConst.Zp            = GlobalM4.EnginParam.Zp;    \
    DrvConst.Rs            = GlobalM4.EnginParam.Rs;    \
    DrvConst.Rr            = GlobalM4.EnginParam.Rr;    \
    DrvConst.Lm            = GlobalM4.EnginParam.Lm;    \
    DrvConst.Lls           = GlobalM4.EnginParam.Lls;   \
    DrvConst.Ixx           = GlobalM4.EnginParam.Ixx;   \
    DrvConst.Jr            = GlobalM4.EnginParam.Jr;    \
                                                        \
    DrvConst.Lsd           = GlobalM4.EnginParam.Lsd;   \
    DrvConst.Lsq           = GlobalM4.EnginParam.Lsq;   \
    DrvConst.PsiR          = GlobalM4.EnginParam.PsiR;  \
                                                        \
    DrvConst.Uscl          = FLOAT_to_IQ(GlobalM4.Uref, Volt_base);             \
    DrvConst.Wrz           = FLOAT_to_IQ(GlobalM4.Fref, FreqUser_base);         \
    DrvConst.Usn           = FLOAT_to_IQ(GlobalM4.EnginParam.Usn, Volt_base);   \
    DrvConst.Wnom          = FLOAT_to_IQ(GlobalM4.EnginParam.Wnom, Herz_base);  \
    DrvConst.Wmax          = FLOAT_to_IQ(GlobalM4.EnginParam.Wmax, Herz_base);  \
    DrvConst.Fnom          = FLOAT_to_IQ(GlobalM4.EnginParam.Fnom, Herz_base);  \
    DrvConst.Isn           = FLOAT_to_IQ(GlobalM4.EnginParam.Isn, Current_base);\
    DrvConst.Imax          = FLOAT_to_IQ(GlobalM4.EnginParam.Imax, Current_base);\
                                                                                \
    DrvConst.Rsw           = GlobalM4.Rsw;                                      \
    DrvConst.Udf           = GlobalM4.Udf;                                      \
    DrvConst.dI            = GlobalM4.dI;                                       \
                                                                                \
    DrvConst.KpSpd         = GlobalM4.KpSpd;                                    \
    DrvConst.KiSpd         = GlobalM4.KiSpd;                                    \
    DrvConst.KpCur         = GlobalM4.KpCur;                                    \
    DrvConst.KiCur         = GlobalM4.KiCur;                                    \
    DrvConst.RejectFreq    = GlobalM4.RejectFreq;                               \
    DrvConst.RejectBand    = GlobalM4.RejectBand;                               \
    DrvConst.SpdForced     = GlobalM4.SpdForced;                                \
                                                                                \
    DrvConst.Fpwm_MMP      = GlobalM4.Fpwm;                                     \
    DrvConst.TUsn_MMP      = 80.0f;                                             \
                                                                                \
    DrvConst.TregSpd       = GlobalM4.TregSpd;                                  \
    DrvConst.FcutSpd       = GlobalM4.FcutSpd;                                  \
    DrvConst.CompDelaySpd  = GlobalM4.CompDelaySpd;                             \
    DrvConst.Kez           = GlobalM4.Kez;                                      \
    DrvConst.Mez           = GlobalM4.Mez;                                      \
                                                                                \
    DrvConst.VoltBASE      = MPOpis.BaseFloat[Volt_base];                       \
    DrvConst.CurrBASE      = MPOpis.BaseFloat[Current_base];                    \
    DrvConst.PowerBASE     = MPOpis.BaseFloat[Power_base];                      \
    DrvConst.TorqBASE      = MPOpis.BaseFloat[Torq_base];                       \
    DrvConst.FreqBASE      = MPOpis.BaseFloat[Herz_base];                       \
    DrvWords.Ctrl.bit.ManualSpdReg = GlobalM4.FlagCommand.bit.ManualSpdReg;     \
    DrvWords.Ctrl.bit.ManualCurReg = GlobalM4.FlagCommand.bit.ManualCurReg;     \
                                                                                \
    DrvConst.DeadTimeInit  = GlobalM4.DeadTimeInit;                             \
    /*DrvConst.Rsw           = GlobalM4.DeadTimeInit.Rsw;                         \
    DrvConst.Udf           = GlobalM4.DeadTimeInit.Udf;                         \
    DrvConst.Utf           = GlobalM4.DeadTimeInit.Utf;                         \
    DrvConst.dI            = GlobalM4.DeadTimeInit.dI;                          \
    DrvConst.nIGBT         = GlobalM4.DeadTimeInit.nIGBT;                       \
    DrvConst.nCell         = GlobalM4.DeadTimeInit.nCell;                       \
    DrvConst.Ton           = GlobalM4.DeadTimeInit.Ton;                         \
    DrvConst.Toff          = GlobalM4.DeadTimeInit.Toff;                        \
    DrvConst.DeadTime      = GlobalM4.DeadTimeInit.DeadTime;                   \ */

    
// Копирование параметров из глобальной переменной в обменную структуру драйвера ВУ    
#define CopyParams_FromM4toDMC()                                                 \
    DrvConst.DriveType     = GlobalM4.DriveType;                                 \
                                                                                 \
    DrvConst.Udc           = FLOAT_to_IQ(GlobalM4.ValUd.Val, Volt_base);         \
    DrvConst.IsU           = FLOAT_to_IQ(GlobalM4.ValIu.FlashVal, Current_base); \
    DrvConst.IsV           = FLOAT_to_IQ(GlobalM4.ValIv.FlashVal, Current_base); \
    DrvConst.IsW           = FLOAT_to_IQ(GlobalM4.ValIw.FlashVal, Current_base); \
                                                                                 \
    DrvConst.Uscl          = FLOAT_to_IQ(GlobalM4.Uref, Volt_base);              \
    DrvConst.Wrz           = FLOAT_to_IQ(GlobalM4.Fref, Herz_base);              \
    /*DrvConst.StepAngle     = GlobalM4.StepAngle;*/                             \
    DrvConst.Mez           = GlobalM4.Mez;                                       \
    DrvConst.SvgenMode     = GlobalM4.SvgenMode;                                 \
    DrvConst.TudcFilt      = GlobalM4.TudcFilt;                                  \
    DrvConst.TfiltVolWeak  = GlobalM4.TfiltVolWeak;                              \
      

// Копирование параметров из обменной структуру драйвера ВУ в глобальную переменную 
#define CopyParams_FromDMCtoM4()                                             \
    GlobalM4.Uout                = IQ_to_FLOAT(DrvConst.Us, Volt_base);      \
    GlobalM4.Wr                  = IQ_to_FLOAT(DrvConst.Wr, Herz_base);      \
    GlobalM4.We                  = IQ_to_FLOAT(DrvConst.We, Herz_base);      \
    GlobalM4.Me                  = IQ_to_FLOAT(DrvConst.Me, Torq_base);      \
    GlobalM4.FluxBASE            = DrvConst.FluxBASE;                        \
    GlobalM4.FlagState.bit.vPrUp = DrvWords.Stat.bit.vPrUP;                  \
    GlobalM4.KpSpdCalc           = DrvConst.KpSpd;                           \
    GlobalM4.KiSpdCalc           = DrvConst.KiSpd;                           \
    GlobalM4.KpCurCalc           = DrvConst.KpCur;                           \
    GlobalM4.KiCurCalc           = DrvConst.KiCur;                           \
    GlobalM4.Angle               = DrvConst.Angle;                           \
              
// Копирование флагов управления в обменную структуру системы регулирования      
#define CopyFlags_FromM4toDMC()                                                 \
  /* Передача состояния модуля ШИМ */                                           \
  if((PwmControl.StatusPwm == PwmWork)||(PwmControl.StatusPwm == PwmStart)){    \
     DrvWords.Ctrl.bit.DriveEnb = 1;                                            \
  }                                                                             \
  else{                                                                         \
     DrvWords.Ctrl.bit.DriveEnb = 0;                                            \
  }                                                                             \
  /* Передача флага аварии */                                                   \
  if(GlobalM4.FailData.NumFail){                                                \
     DrvWords.Ctrl.bit.FailOn = 1;                                              \
  }                                                                             \
  else{                                                                         \
     DrvWords.Ctrl.bit.FailOn = 0;                                              \
  }                                                                             \
                                                                                \
  DrvWords.Ctrl.bit.EMFRegEnb = GlobalM4.FlagCommand.bit.EMFRegEnb;             \
  DrvWords.Ctrl.bit.FiltrRefSpdEnb = GlobalM4.FlagCommand.bit.FiltrRefSpdEnb;   \
  DrvWords.Ctrl.bit.SoftCurDwn = GlobalM4.FlagCommand.bit.SoftCurDwn;           \
  DrvWords.Ctrl.bit.mWorkEnb = GlobalM4.FlagCommand.bit.mWorkEnb;               \
  DrvWords.Ctrl.bit.ForceEnb = GlobalM4.FlagCommand.bit.ForceEnb;               \
      
// Копирование флагов управления с системы регулирования в глобальную структуру      
#define CopyFlags_FromDMCtoM4();                                                \
  GlobalM4.FlagState.bit.mWorkFin = DrvWords.Stat.bit.mWorkFin;                 \
      
      
      
      