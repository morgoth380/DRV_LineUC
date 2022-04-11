#include    "DRV_VECT_ALL_macro.h"
#include    "GT_DEFs.h"
#include    "MakeOpis.h"
#include    "Filter.h"
#include    "math.h"
#include    "Pjk_const.h"

#ifdef _PROJECT_FOR_VS2005_
		#include <stdlib.h>
		#include <time.h>
#endif

#ifdef    GET_FUNC_CODE
/******************************************************************************/
// Тип управления ПЧ
enum {
	ScalarWOdtc=0,
	ScalarWIdtc,
	ACIMVectWIfbk,
	ACIMVectWOfbk,
	PMSMVectWIfbk,
	PMSMVectWOfbk,
};
/******************************************************************************/
// Тип rалибровки каналов измерения
typedef enum{
    Calibr_Udc,    // Калибровка Udc
    Calibr_Iu,
    Calibr_Iv,
    Calibr_Iw,
    Calibr_Uab,
    Calibr_Ubc,
    Calibr_Uca  
}CalibrTec_type;
/******************************************************************************/
// Тип торможения
typedef enum {
    rampdown = 0,
    frequency,
    softcurrentdown,
}BRAKEtype_enum;
/******************************************************************************/
typedef enum {
    startUp = 0,
    motMsurmnt,  // Измерение параметров двигателя
    tormResChk,  // Проверка тормозного резистора
    contyChk,    // Прозвонка фаз до старта
    drying,      // Сушка
    running,     // Работа
}PUSKtype_enum;
/******************************************************************************/
#define KoefDiv_Ud      1
#define KoefDiv_Uin     1
#define KoefDiv_Curr    10

/******************************************************************************/
// Описание настроек калибровки одного канала
typedef struct{
        float     Val;          // Значение параметра
        float     Kcor;         // Калибровочный коеффициент
        float     Ustir;        // Юстировочное число
}CalibrVal_type;

/******************************************************************************/
// static
typedef struct{
	
    // Калибровки
    CalibrVal_type     CalibrUd;
    CalibrVal_type     CalibrIu;
    CalibrVal_type     CalibrIv;
    CalibrVal_type     CalibrIw;
    CalibrVal_type     CalibrUab;
    CalibrVal_type     CalibrUbc;
    CalibrVal_type     CalibrUca;    
        
    CalibrTec_type     CalibrTec;      // калибруемый в текущий момент канал измирения
    nvEngineParam_type EngineParam;    // Параметры двигателя
    
    BRAKEtype_enum     brakeType;
    PUSKtype_enum      puskState;
    u16	               combiBrakeTimer;
    u16		       softCurDwnTimer;
    u16		       rampDownTimer;
    u16		       contyChkTimer;
    u32		       dryingTimer;
    float              MeFiltr;
    //u16                prevS16Torque; // Предыдущее значение 
    
    s32                AdcFiltrIGBT;
    
    float              prevKpSpd;
    float              prevKiSpd;
    float              prevKpCur;
    float              prevKiCur;
    
    // Переменная
    union{ u16         value;
	 struct{       
            u16 stopCmd       :1;
	    u16 runCmd        :1;
	    u16 failCmd       :1;
	    u16 PwmOn         :1;    // Признак необходимости включить ШИМ
	    u16 dcInjectEna   :1;
	    u16 softCurDwnEna :1;
            u16 driveRunning  :1; // Привод перешел в состояние - Running. Все предварительные проверки завершены и мы поехали.            
            u16 countyChk     :1; // Активность режима прозвонки            
	    u16 reserve       :8;
	 }bit;
    }algState;
//--
    
}DRVV5_State_type;
/******************************************************************************/
// Расчет коеффициента для перевода кодов АЦП в реальное значение
float GainCalc(float Ust, float Kcor){
  return (Ust * Kcor) / 4096.0f;
}
/******************************************************************************/
// Функция калибровки
float CalibrFunk(u32 ramPnt, float Korrect, u16 PntK, float AdjustNum, float *Gain, float NewVal, CalibrVal_type *PrevVal, void *nvAdr, u16 koefDiv){

  float Kcor, KcorNew; 
  // 1) Получить текущее значение
  Kcor = load_float(ramPnt, PntK);                 // Коефф калибровки
  NewVal *= koefDiv;
  // 2) Проверить изменил ли пользователь значения калиброви 
  if(Kcor != PrevVal->Kcor){
      PrevVal->Kcor = Kcor;                               // Сохраняем новое значение в static  
      *Gain = GainCalc(AdjustNum, Kcor);                  // Расчитаем новое значение калибровочного коеффициента 
      
      WrFramMicro(nvAdr, &PrevVal->Kcor, sizeof(float));  // Сохранить новое значение Kcor в FRAM
  }
  else if(fabsf(Korrect - PrevVal->Val) > 0.01f){         // Если пользователь изменил текущее значение параметра

	  if (NewVal == 0)	NewVal = Korrect;				  // Защита от деления на ноль. Если новое значение ноль, приравниваем его Korrect. Тогда Ккор будет = 1.
      KcorNew = Kcor * (Korrect / NewVal);                // Расчитаем новое значение коеффициента калибровки Kcor 
      if((KcorNew < 0.5) || (KcorNew > 2.0))
      {
          NewVal += 0.5f;       // для округления
          return NewVal;
      }
      PrevVal->Kcor = KcorNew;                            // Сохраняем новое значение в static
      PrevVal->Val = Korrect;                             // Сохраняем новое значение в static 
      save_float(ramPnt, PntK, KcorNew);                  // Сохранить новое значение коеффициента калибровки Kcor в уставку
          
      *Gain = GainCalc(AdjustNum, KcorNew);               // Расчитаем новое значение калибровочного коеффициента 
      
      WrFramMicro(nvAdr, &PrevVal->Kcor, sizeof(float));  // Сохранить новое значение Kcor в FRAM
  }
  else if(AdjustNum != PrevVal->Ustir){
      PrevVal->Ustir = AdjustNum;                         // Сохраняем новое значение в static 
      
      *Gain = GainCalc(AdjustNum, Kcor);                  // Расчитаем новое значение калибровочного коеффициента
  }
    
  // 3) Записать обновленное значение параметра в уставку
  NewVal += 0.5f;       // для округления
  PrevVal->Val = (s16)(NewVal);
  return NewVal; 

}

/******************************************************************************/
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Pusk;                         //1  Pusk
    u16  Pnt_Stop;                         //2  Stop
    u16  Pnt_StopVibeg;                    //3  StopVibeg
    u16  Pnt_FailTek;                      //4  FailTek
    u16  Pnt_SelTormType;                  //5  SelTormType
    u16  Pnt_VibegTime;                    //6  VibegTime
    u16  Pnt_SoftCurDwnTime;               //7  SoftCurDwnTime
    u16  Pnt_Ftek;                         //8  Ftek		
    u16  Pnt_Fpusk;                        //9  Fpusk
    u16  Pnt_SoftCurDwnEna;                //10 SoftCurDown
    u16  Pnt_StateGT;                      //11 StateGT
    u16  Pnt_F;                            //12 F
    u16  Pnt_U;                            //13 U
    u16  Pnt_Napravlenie;                  //14 Napravlenie 
    u16  Pnt_Wr__Me_1;                     //15 Wr/~Me_1
    u16  Pnt_Pwm_Mode;                     //16 Pwm_Mode
    u16  Pnt_Udc_Korrect;                  //17 Ud_Korrect   
    u16  Pnt_K_Udc;                        //18 K_Ud		 
    u16  Pnt_Iu_Korrect;                   //19 Iu_Korrect
    u16  Pnt_K_Iu;                         //20 K_Iu
    u16  Pnt_Iv_Korrect;                   //21 Iv_Korrect
    u16  Pnt_K_Iv;                         //22 K_Iv
    u16  Pnt_Iw_Korrect;                   //23 Iw_Korrect
    u16  Pnt_K_Iw;                         //24 K_Iw
    u16  Pnt_FpwmDC;                       //25 FpwmDC
    u16  Pnt_FpwmDCenable;                 //26 FpwmDCenable
    u16  Pnt_ImaxVFD;                      //27 Imax VFD
    u16  Pnt_Ustir_Cur;                    //28 Ustir_Cur
    u16  Pnt_MTZref;                       //29 MTZref
    u16  Pnt_CurSensKolv;                  //30 CurSensKolv 
    u16  Pnt_InvCurSens;                   //31 InvCurSens  
    u16  Pnt_Ustir_Ud;                     //32 Ustir_Ud
    u16  Pnt_I_motor;                      //33 I_motor
    u16  Pnt_I_max;                        //34 I_max
    u16  Pnt_TypeDrv;                      //35 TypeDrv	
    u16  Pnt_Dtime;                        //36 Dtime
    u16  Pnt_Fpwm;                         //37 Fpwm
    u16  Pnt_PwmType;			   //38 PwmType	
    u16  Pnt_K_Il;                         //39 K_Ileak	
    u16  Pnt_Il_Korrect;                   //40 Ileak_Korect    
    u16  Pnt_Ileak_MTZ;                    //41 Ileak_MTZ   
    u16  Pnt_FeCorrectMode;                //42 FeCorrectMode 
    u16  Pnt_FeFixedPhase;                 //43 FeFixedPhase    
    u16  Pnt_FePidKp;                      //44 FePidKp
    u16  Pnt_FePidKi;                      //45 FePidKi
    u16  Pnt_FePidAutoset;                 //46 FePidAutoset
    u16  Pnt_EncoderFilter;                //47 EncoderFilter
    u16  Pnt_PsiRotor;                     //48 PsiRotor	
    u16  Pnt_Lsd;                          //49 Lsd		
    u16  Pnt_Lsq;                          //50 Lsq    	
    u16  Pnt_Reset_Default;                //51 Reset_Default	 
    u16  Pnt_Motor_CosFi;                  //52 Motor_CosFi
    u16  Pnt_Motor_Kpd;                    //53 Motor_Kpd
    u16  Pnt_Wr_nom;                       //54 Wr_nom
    u16  Pnt_Wr_max;                       //55 Wr_max
    u16  Pnt_Fs_nom;                       //56 Fs_nom
    u16  Pnt_Us_nom;                       //57 Us_nom
    u16  Pnt_CalcPrmMot;                   //58 CalcPrmMot	
    u16  Pnt_Rr;                           //59 Rr
    u16  Pnt_Rs;                           //60 Rs
    u16  Pnt_Lm;                           //61 Lm
    u16  Pnt_Lls;                      	   //62 SigmaLs	
    u16  Pnt_Zp;                           //63 Zp
    u16  Pnt_Adapt_Tr_Ls;                  //64 Adapt_Tr_Ls
    u16  Pnt_Calc_J;                       //65 Calc_J
    u16  Pnt_Jr;                           //66 Jr
    u16  Pnt_T0is;                         //67 T0is
    u16  Pnt_CalcRegI;                     //68 CalcRegI
    u16  Pnt_Regi_Kp;                      //69 Regi_Kp
    u16  Pnt_Regi_Ki;                      //70 Regi_Ki
    u16  Pnt_CalcRegILim;                  //71 CalcRegILim
    u16  Pnt_Reg_Id_Lim;                   //72 Reg_Id_Lim
    u16  Pnt_Reg_Iq_Lim;                   //73 Reg_Iq_Lim
    u16  Pnt_T0wr;                         //74 T0wr
    u16  Pnt_Tfwrz;                        //75 Tfwrz
    u16  Pnt_CalcRegWr;                    //76 CalcRegWr
    u16  Pnt_RegWr_Kp;                     //77 RegWr_Kp
    u16  Pnt_RegWr_Ki;                     //78 RegWr_Ki
    u16  Pnt_RegPos_Kp;			   //79 RegPos_Kp	
    u16  Pnt_BSFilter_Fr;                  //80 BSFilter_Fr 
    u16  Pnt_BSFilter_dF;                  //81 Pnt_BSFilter_dF 
    u16  Pnt_AutosetCurReg;                //82  Pnt_AutosetCurReg - Автонастройка регулятора тока
    u16  Pnt_CurRegKp;                     //83  Pnt_CurRegKp - Кп регулятора тока
    u16  Pnt_CurRegKi;                     //84  Pnt_CurRegKi - Ки регулятора тока
    u16  Pnt_T_ad_Lm;                      //85 T_ad_Lm
    u16  Pnt_T0e;                          //86 T0e
    u16  Pnt_Kez;                          //87 Kez
    u16  Pnt_CalcRegE;                     //88 CalcRegE
    u16  Pnt_Ustir_Uin;                    //89 Pnt_Ustir_Uin		
    u16  Pnt_Uab_Korrect;                  //90 Uab_Korrect
    u16  Pnt_K_Uab;                        //91 K_Uab
    u16  Pnt_Ubc_Korrect;                  //92 Ubc_Korrect
    u16  Pnt_K_Ubc;                        //93 K_Ubc
    u16  Pnt_Uca_Korrect;                  //94 Uca_Korrect
    u16  Pnt_K_Uca;                        //95 K_Uca
    u16  Pnt_Reserved96;                   //96 res
    u16  Pnt_Reserved97;                   //97 res
    u16  Pnt_Reserved98;                   //98 res
    u16  Pnt_DifFiltr;                     //99 DifFiltr
    u16  Pnt_Max;                          //100 Max
    u16  Pnt_Min;                          //101 Min
    u16  Pnt_TempMax;                      //102 TempMax
    u16  Pnt_ZerHzDc;                      //103 ZerHzDc
    u16  Pnt_F_DCSTP;                      //104 F_DCSTP
    u16  Pnt_DC_Brk_En;                    //105 DC_Brk_En
    u16  Pnt_DC_MaxCurrentOut;             //106 DC_MaxCurrentOut
    u16  Pnt_CurTormDc;                    //107 CurTormDc		
    u16  Pnt_Dry_On;                       //108 Dry_On
    u16  Pnt_Inom_Motor;                   //109 Pnt_Inom_Motor
    u16  Pnt_Ixx;	                   //110 Ixx SrcEnSel
    u16  Pnt_Ud_tek_iq;                    //111 Ud_tek_iq
    u16  Pnt_DF_Revers;                    //112 DF_Revers
    u16  Pnt_RUtime;                       //113 Iru
    u16  Pnt_RUena;                        //114 RUena		
    u16  Pnt_tormCheck;                    //115 tormCheck
    u16  Pnt_PwmOn;                        //116 PwmOn
    u16  Pnt_NotPwmOn;                     //117 NotPwmOn
    u16  Pnt_StopFreq;                     //118 StopFreq
    u16  Pnt_StopByDc;                     //119 StopByDc
    u16  Pnt_WorkRunMode;                  //120 WorkRunMode   
    u16  Pnt_VibegStat;                    //121 VibegStat  
    u16  Pnt_Uout;                         //122 Uout
    u16  Pnt_Pfull_out;                    //123 Pfull_out
    u16  Pnt_Pact_out;                     //124 Pact_out
    u16  Pnt_Preact_out;                   //125 Preact_out	
    u16  Pnt_cosFi_out;                    //126 cosFi_out
    u16  Pnt_Tu;                           //127 Tu
    u16  Pnt_Iact_out;                     //128 Iact_out
    u16  Pnt_Ifull_out;                    //129 Ifull_out
    u16  Pnt_Ireact_out;                   //130 Ireact_out
    u16  Pnt_We;                           //131 We
    u16  Pnt_Wr;                           //132 Wr
    u16  Pnt_Me;                           //133 Me
    u16  Pnt_Reg_Wpr;                      //134 Reg_Wpr  
    u16  Pnt_FoutSign;                     //135 FoutSign -- signum(Pnt_We)
    u16  Pnt_FoutAbs;                      //136 FoutAbs  -- abs(Pnt_We)
    u16  Pnt_StatusDRV;                    //137 StatusDRV 
    u16  Pnt_EncoderDetect;                //138 EncoderDetect
    u16  Pnt_EncoderAngle;                 //139 EncoderAngle
    u16  Pnt_Reg_Iq_Out;                   //140 Reg_Iq_Out
    u16  Pnt_Reg_Wr_Out;                   //141 Reg_Wr_Out
    u16  Pnt_RegE_Ref;                     //142 RegE_Ref
    u16  Pnt_MeasParamInterrupt;           //143 RegE_Out
    u16  Pnt_StatusDC;                     //144 StatusDC
    u16  Pnt_End;
}FuncDRV_V5_type;
//********************************************************
u16 *FuncDRV_V5_1(FuncDRV_V5_type *progPnt, u32 ramPnt)
{   
    f32                    UstirUd, UstirUin, UstirCur, TekVal, mtz_ref, temp, MaxDcCurrent; 
    //f32                    UinMin, UinMax;    
    DRVV5_State_type         *sPnt;
    FailsSetup_type          *pFailsSetup;
    f32                      TorqueTemp;  
    u16                      FlagNaN = 0, FlagLimit = 0; 
    s16			     temp_s16 = 0;
    u16 UstirCur_u16 = 0, mtz_ref_u16= 0;
    f32                    ValSave = 0, MaxFPwm = 0;
    ControlDrvType_type      typeDrv;
    nvAdjCoef_type           *nvAdrCoef;
    nvEngineParam_type       *nvEngineParam;
    u16                      UstRecoveryFlagValue;
    f32                    We_freq, Wr_freq, Me_temp;
    f32                    DC_Current;
    f32                    contyChkTime, ImaxVFD;
    f32                    Id_nom, Is_max, Iq_max;
    
    //f32     Mnmot, MkestNom, skestNom, snom, I2nomEst, InoLoadEst1, InoLoadEst2, InoLoadEst3, InoLoadEstMean;
      
    sPnt        = (DRVV5_State_type *)(ramPnt + progPnt->Pnt_State);


    // TEST
    
//    nvAdrAin            = (nvAinCalState_type* )nv.AInput.CalibrCode;
//    nvAdrAout           = (nvAoutCalState_type* )nv.AOutput.CalibrCode;
//    
//    
//    RdFramMicro(&FlagsFunctional, &nv.fileCurFunct, sizeof(FileFunctional_type));
//    crc = crcPacket((unsigned char *) &FlagsFunctional, sizeof(FlagsFunctional.curFunct));     
//    
    
//	nvAdrCoef   = &nv.MicroProg; // 0
//    nvAdrCoef   = &nv.MemSignatura;
//	nvAdrCoef   = &nv.MicroSignatura;
//	nvAdrCoef   = &nv.errImaxCnt;
//    nvAdrCoef   = &nv.UstRecoveryFlag;
//	nvAdrCoef   = &nv.stopTime;
//	nvAdrCoef   = &nv.mtzTime;
//	nvAdrCoef   = &nv.mtzTimeSum;
//	nvAdrCoef   = &nv.file;
//	nvAdrCoef   = &nv.DK_file;
//	nvAdrCoef   = &nv.UstModbusAdr;
//	nvAdrCoef   = &nv.trend_file;
//	nvAdrCoef   = &nv.Space;
//	nvAdrCoef   = &nv.mtzTimeBuff;
//	nvAdrCoef   = &nv.AOutput;
//	nvAdrCoef   = &nv.AInput;
//	nvAdrCoef   = &nv.adjCoefcnts;
//	nvAdrCoef   = &nv.nvEngineParam;
//	nvAdrCoef   = &nv.fileCurFunct;
//	nvAdrCoef   = &nv.fileCurFunctPrev;
//	nvAdrCoef   = &nv.EngineRunFlag;
//	nvAdrCoef   = &nv.PowerFailTime;
//

//	UstRecoveryFlagValue = 16384 * sizeof(u16);
//    UstRecoveryFlagValue = sizeof(u16) * SZ_MICROPROG_DATA;
//    UstRecoveryFlagValue = sizeof(u16) * 5;
//	UstRecoveryFlagValue = sizeof(u32)*5;
//	UstRecoveryFlagValue = sizeof(fram_file_type);
//	UstRecoveryFlagValue = sizeof(FileFunctional_type) * 2;
//	UstRecoveryFlagValue = sizeof(u32)*(MTZ_BUF_SIZE);
//	UstRecoveryFlagValue = sizeof(nvAoutCalState_type);
//	UstRecoveryFlagValue = sizeof(DK_file_type);
//	UstRecoveryFlagValue = sizeof(nvAinCalState_type );
//	UstRecoveryFlagValue = sizeof(nvAdjCoef_type);
//	UstRecoveryFlagValue = sizeof(Trend_file_type);
//	UstRecoveryFlagValue = sizeof(nvEngineParam_type);
//	UstRecoveryFlagValue = sizeof(Space_type);
//	UstRecoveryFlagValue = sizeof(nvParamAdr_type);
//	UstRecoveryFlagValue = sizeSpace;
//	UstRecoveryFlagValue = sizeof(u16);
       // u32     *adr, size_spfl;
//	adr =  (u32*)(&spfl.fileDescrPrm);
//        adr =  (u32*)(&spfl.filePultOP24B);
//        adr =  (u32*)(&spfl.fileHltPrmJrn);
//        adr =  (u32*)(&spfl.fileProg);
//        adr =  (u32*)(&spfl.fileMicroProg1);
//        adr =  (u32*)(&spfl.fileMicroProg2);
//        adr =  (u32*)(&spfl.fileMicroProg3);
//        //adr =  (u32*)(&spfl.fileMkarCP24B);
//        adr =  (u32*)(&spfl.filePultFontOP24B);
//        adr =  (u32*)(&spfl.filePultOP24S);
       // size_spfl = sizeof (spfl_type);
//          
	////////////////////////////////////////////////////
	nvAdrCoef   = (nvAdjCoef_type  *)(&nv.adjCoefcnts);
        // Коррекция адреса данных двигателя
        nvEngineParam = (nvEngineParam_type *)(&nv.nvEngineParam);
        pFailsSetup = &GlobalM4.FailsSetup;
    
    // Первичная инициализация
    if(flagIsFirstCall){            
       //***********************************************************************
       // Инициализация калибровок каналов измерения
       //***********************************************************************
       // 1) Прочитать с FRAM калибровочные коеффициенты
       RdFramMicro(&sPnt->CalibrIu.Kcor, &nvAdrCoef->K_Iu, sizeof(float));
       RdFramMicro(&sPnt->CalibrIv.Kcor, &nvAdrCoef->K_Iv, sizeof(float));
       RdFramMicro(&sPnt->CalibrIw.Kcor, &nvAdrCoef->K_Iw, sizeof(float));
       RdFramMicro(&sPnt->CalibrUd.Kcor, &nvAdrCoef->K_Udc, sizeof(float));
       RdFramMicro(&sPnt->CalibrUab.Kcor, &nvAdrCoef->K_Uab, sizeof(float));
       RdFramMicro(&sPnt->CalibrUbc.Kcor, &nvAdrCoef->K_Ubc, sizeof(float));
       RdFramMicro(&sPnt->CalibrUca.Kcor, &nvAdrCoef->K_Uca, sizeof(float));
       
       // 2) Проверим Kcor на валидность 
       #ifdef _PROJECT_FOR_STM32_
       if(isnan(sPnt->CalibrIu.Kcor)  || isnan(sPnt->CalibrIv.Kcor)  || isnan(sPnt->CalibrIw.Kcor) || isnan(sPnt->CalibrUd.Kcor) ||
          isnan(sPnt->CalibrUab.Kcor) || isnan(sPnt->CalibrUbc.Kcor) || isnan(sPnt->CalibrUca.Kcor)){
          FlagNaN = 1;   // Одно из значений Kcor не валидное
       }
       #endif //_PROJECT_FOR_STM32_

       // 3) Проверить значения Kcor на выход за пределы
       if(((sPnt->CalibrIu.Kcor < 0.5f) ||(sPnt->CalibrIu.Kcor > 2.0f))||
          ((sPnt->CalibrIv.Kcor < 0.5f) ||(sPnt->CalibrIv.Kcor > 2.0f))||
          ((sPnt->CalibrIw.Kcor < 0.5f) ||(sPnt->CalibrIw.Kcor > 2.0f))||
          ((sPnt->CalibrUd.Kcor < 0.5f) ||(sPnt->CalibrUd.Kcor > 2.0f))||
          ((sPnt->CalibrUab.Kcor < 0.5f)||(sPnt->CalibrUab.Kcor > 2.0f))||
          ((sPnt->CalibrUbc.Kcor < 0.5f)||(sPnt->CalibrUbc.Kcor > 2.0f))||
          ((sPnt->CalibrUca.Kcor < 0.5f)||(sPnt->CalibrUca.Kcor > 2.0f))  ){
          FlagLimit = 1;
       }
       
       // 4) Принимаем решение       
       if((FlagNaN)||(FlagLimit)){
            // Загрузить значения по умолчанию с уставкок
            sPnt->CalibrUd.Kcor = load_float(ramPnt, progPnt->Pnt_K_Udc);        
            sPnt->CalibrIu.Kcor = load_float(ramPnt, progPnt->Pnt_K_Iu);
            sPnt->CalibrIv.Kcor = load_float(ramPnt, progPnt->Pnt_K_Iv);
            sPnt->CalibrIw.Kcor = load_float(ramPnt, progPnt->Pnt_K_Iw);
            sPnt->CalibrUab.Kcor = load_float(ramPnt, progPnt->Pnt_K_Uab);
            sPnt->CalibrUbc.Kcor = load_float(ramPnt, progPnt->Pnt_K_Ubc);
            sPnt->CalibrUca.Kcor = load_float(ramPnt, progPnt->Pnt_K_Uca);
            
            WrFramMicro(&nvAdrCoef->K_Udc, &sPnt->CalibrUd.Kcor, sizeof(float));	// Прописать начальные значения в FRAM
            WrFramMicro(&nvAdrCoef->K_Iu, &sPnt->CalibrIu.Kcor, sizeof(float));
            WrFramMicro(&nvAdrCoef->K_Iv, &sPnt->CalibrIv.Kcor, sizeof(float));
            WrFramMicro(&nvAdrCoef->K_Iw, &sPnt->CalibrIw.Kcor, sizeof(float));
            WrFramMicro(&nvAdrCoef->K_Uab, &sPnt->CalibrUab.Kcor, sizeof(float));
            WrFramMicro(&nvAdrCoef->K_Ubc, &sPnt->CalibrUbc.Kcor, sizeof(float));
            WrFramMicro(&nvAdrCoef->K_Uca, &sPnt->CalibrUca.Kcor, sizeof(float));
       }
       else{   // В FRAM уставки валидные
           // Сохранить в уставки значение коефициентов калибровки из FRAM 
           save_float(ramPnt, progPnt->Pnt_K_Iu, sPnt->CalibrIu.Kcor);
           save_float(ramPnt, progPnt->Pnt_K_Iv, sPnt->CalibrIv.Kcor);
           save_float(ramPnt, progPnt->Pnt_K_Iw, sPnt->CalibrIw.Kcor);
           save_float(ramPnt, progPnt->Pnt_K_Udc, sPnt->CalibrUd.Kcor);        // Загрузить значения по умолчанию
           save_float(ramPnt, progPnt->Pnt_K_Uab, sPnt->CalibrUab.Kcor);
           save_float(ramPnt, progPnt->Pnt_K_Ubc, sPnt->CalibrUbc.Kcor);
           save_float(ramPnt, progPnt->Pnt_K_Uca, sPnt->CalibrUca.Kcor);
       }
       
       sPnt->CalibrTec = Calibr_Udc;               // Установить первый калибруемый канал измерения
       
       //***********************************************************************
       sPnt->algState.value = 0;                         // Сбросить апарат состояний ПЧ
       setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);       // PWM_OFF
       clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);          // PWM_ON  
       clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);       // Сбросить признак активного частотного торможения 
       
       
       RdFramMicro(&UstRecoveryFlagValue, &nv.UstRecoveryFlag,  sizeof(u16)); // Проверяем флаг восстановления уставок
       
       // Читаем данные параметров двигателя из FRAM в локальную sPnt
       RdFramMicro(&sPnt->EngineParam, nvEngineParam/*&nv.nvEngineParam*/, sizeof(nvEngineParam_type));
                    
       #ifdef _PROJECT_FOR_STM32_
          FlagNaN = 0;    
       
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.Rr);
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.Rs);	// Значения из микропрограммы по умолчанию
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.Lm);
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.Lls);
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.Lsd);
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.Lsq);	
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.Ixx);	
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.Jr);
          FlagNaN = FlagNaN || isnan(sPnt->EngineParam.PsiR);       
          //FlagNaN = FlagNaN || isnan(sPnt->EngineParam.RegWr_Kp);
          //FlagNaN = FlagNaN || isnan(sPnt->EngineParam.RegWr_Ki);
          //FlagNaN = FlagNaN || isnan(sPnt->EngineParam.KpCur);
          //FlagNaN = FlagNaN || isnan(sPnt->EngineParam.KiCur);
          
       #endif //_PROJECT_FOR_STM32_
       
       if(FlagNaN || sPnt->EngineParam.Rs == 0 || UstRecoveryFlagValue == 1) {
          // А если из системы параметров загружаем нули, то тогда принудительно поставить в 1 параметры двигателя.
          if (!load_s16(ramPnt, progPnt->Pnt_Rs) || !load_s16(ramPnt, progPnt->Pnt_Lm) || !load_s16(ramPnt, progPnt->Pnt_Rr)) {
            save_s16(ramPnt, progPnt->Pnt_Rr, 1000);
            save_s16(ramPnt, progPnt->Pnt_Rs, 1000);
            save_s16(ramPnt, progPnt->Pnt_Lm, 1000);
            save_float(ramPnt, progPnt->Pnt_Lls, 0.001);
            save_s16(ramPnt, progPnt->Pnt_Lsd, 1000);
            save_s16(ramPnt, progPnt->Pnt_Lsq, 1000);
            save_float(ramPnt, progPnt->Pnt_Jr, 0.03 );
          }         
         
          // Сохраняем данные в том виде, в котором они есть - т.е. Float
          sPnt->EngineParam.Rr	     = ((float)load_s16(ramPnt, progPnt->Pnt_Rr) * 0.001f);
          sPnt->EngineParam.Rs	     = ((float)load_s16(ramPnt, progPnt->Pnt_Rs) * 0.001f);	      // Значения из микропрограммы по умолчанию
          sPnt->EngineParam.Lm	     = ((float)load_s16(ramPnt, progPnt->Pnt_Lm) * 0.0001f);
          sPnt->EngineParam.Lls	     = load_float(ramPnt, progPnt->Pnt_Lls );
          sPnt->EngineParam.Lsd	     = ((float)load_s16(ramPnt, progPnt->Pnt_Lsd) * 0.0001f);
          sPnt->EngineParam.Lsq	     = ((float)load_s16(ramPnt, progPnt->Pnt_Lsq) * 0.0001f);	      // Значения из микропрограммы по умолчанию
          sPnt->EngineParam.Ixx	     = ((float)load_s16(ramPnt, progPnt->Pnt_Ixx) * 0.1f);	      // Значения из микропрограммы по умолчанию
          sPnt->EngineParam.Jr	     = load_float(ramPnt, progPnt->Pnt_Jr );
          sPnt->EngineParam.PsiR     = ((float)load_s16(ramPnt, progPnt->Pnt_PsiRotor) * 0.001f);     // Возможно он не нужен в реальности.
          sPnt->EngineParam.RegWr_Kp = ((float)load_s16(ramPnt, progPnt->Pnt_CalcRegILim) * 0.01f);
          sPnt->EngineParam.RegWr_Ki = ((float)load_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim) * 0.00001f);
          sPnt->EngineParam.KpCur = ((float)load_s16(ramPnt, progPnt->Pnt_CurRegKp) * 0.01f);
          sPnt->EngineParam.KiCur = ((float)load_s16(ramPnt, progPnt->Pnt_CurRegKi) * 0.00001f);
          
          if (UstRecoveryFlagValue == 1) { // Если был флаг восстановления уставок, то расчет момента переводим в ручной режим
            sPnt->EngineParam.TorqueAutoSet = 0; // 
          } else { // А если это глюк по параметрам, то ставим авторежим
            sPnt->EngineParam.TorqueAutoSet = 1; // 
          }
                    
          WrFramMicro(/*&nv.nvEngineParam*/ nvEngineParam, &sPnt->EngineParam, sizeof(nvEngineParam_type));          
       }
       
       // Заносим в текущией настройки ТехПО системы параметров
       // Переносим параметры двигателя в уставки системы параметров
       save_s16(ramPnt, progPnt->Pnt_Rr	, (s16)(sPnt->EngineParam.Rr * 1000.0f));
       save_s16(ramPnt, progPnt->Pnt_Rs	, (s16)(sPnt->EngineParam.Rs * 1000.0f));
       save_s16(ramPnt, progPnt->Pnt_Lm	, (s16)(sPnt->EngineParam.Lm * 10000.0f));
       save_float(ramPnt, progPnt->Pnt_Lls	, sPnt->EngineParam.Lls);
       save_s16(ramPnt, progPnt->Pnt_Lsd	, (s16)(sPnt->EngineParam.Lsd * 10000.0f));
       save_s16(ramPnt, progPnt->Pnt_Lsq	, (s16)(sPnt->EngineParam.Lsq * 10000.0f));
       save_s16(ramPnt, progPnt->Pnt_Ixx	, (s16)(sPnt->EngineParam.Ixx * 10.0f));
       save_float(ramPnt, progPnt->Pnt_Jr	, sPnt->EngineParam.Jr);
       save_s16(ramPnt, progPnt->Pnt_PsiRotor	, (s16)(sPnt->EngineParam.PsiR * 1000.0f));
       save_s16(ramPnt, progPnt->Pnt_CalcRegILim  , (s16)(sPnt->EngineParam.RegWr_Kp * 100.0f + 0.5f));
       save_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim	, (s16)(sPnt->EngineParam.RegWr_Ki * 10000.f + 0.5f));
       save_s16(ramPnt, progPnt->Pnt_CurRegKp  , (s16)(sPnt->EngineParam.KpCur * 100.0f + 0.5f));
       save_s16(ramPnt, progPnt->Pnt_CurRegKi	, (s16)(sPnt->EngineParam.KiCur * 10000.f + 0.5f));       
    }
    // Инициализация переменных ВУ - если пользователь изменил один из параметров
    if(flagIsFirstCall || GlobalM4.FlagCommand.bit.AciParamChange || GlobalM4.FlagCommand.bit.PwmParamChange){
       // Сброс флага AciParamChange перед выходом из условия
      
       // Необходимо до всех вычиток проверить на изменение три параметра: 
       // номинальный момент 
       // Номинальную скорость
       // Номинальную мощность
       // Если момент изменили руками, сбрасываем авторежим расчета
        if (!flagIsFirstCall) {
            TorqueTemp = (float)load_s16(ramPnt, progPnt->Pnt_RegPos_Kp); // Читаем момент из микропрограммы в целых числах, иначе глюки при сравнении
            if ((u16)(GlobalM4.EnginParam.Torque_n*10.0f) != (u16)TorqueTemp) {
              sPnt->EngineParam.TorqueAutoSet = 0;
              WrFramMicro(/*&nv.nvEngineParam*/ nvEngineParam, &sPnt->EngineParam, sizeof(nvEngineParam_type));
            } else if (GlobalM4.EnginParam.Vnom != (float)load_s16(ramPnt, progPnt->Pnt_Wr_nom) * 0.1f ||
                       GlobalM4.EnginParam.Pnom != (float)load_s16(ramPnt, progPnt->Pnt_T0e) * 0.1f){
                         // А если изменили номинальную мощность или скорость, ставим авторежим расчета момента
                         sPnt->EngineParam.TorqueAutoSet = 1;
                         WrFramMicro(/*&nv.nvEngineParam*/ nvEngineParam, &sPnt->EngineParam, sizeof(nvEngineParam_type));
            }
        }
       // Настройки режима измерения параметров, для мощности 7.5 кВт
       GlobalM4.Fpwm = (float)load_s16(ramPnt, progPnt->Pnt_Fpwm) * 10.0f;              // Частота ШИМ + перевод в кГц
       GlobalM4.EnginParam.Imax = (float)load_s16(ramPnt, progPnt->Pnt_I_max);          // Максимальный моменто образующий ток двигателя
       GlobalM4.TregSpd =  (float)load_s16(ramPnt, progPnt->Pnt_T0is) * 0.0001f;
       GlobalM4.FcutSpd =  (float)load_s16(ramPnt, progPnt->Pnt_CalcRegI);
       GlobalM4.CompDelaySpd = (float)load_s16(ramPnt, progPnt->Pnt_Regi_Kp) * 0.1f;
       
       GlobalM4.FlagCommand.bit.ForceEnb = load_s16(ramPnt, progPnt->Pnt_Tfwrz); // Активация антиотката
       GlobalM4.SpdForceTime = load_s16(ramPnt, progPnt->Pnt_CalcRegWr) * 0.1f; // Время антиотката
       GlobalM4.SpdForced = (float)load_s16(ramPnt, progPnt->Pnt_RegWr_Kp) * 0.001f;
//       GlobalM4.KpForced = (float)load_s16(ramPnt, progPnt->Pnt_RegWr_Kp) * 0.01f;
//       GlobalM4.KiForced = (float)load_s16(ramPnt, progPnt->Pnt_RegWr_Ki) * 0.0001f;
       
       GlobalM4.Isn = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_I_motor), Current_base);           // Номинальный ток двигателя
       GlobalM4.EnginParam.Usn = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Us_nom), Volt_base); //(float)load_s16(ramPnt, progPnt->Pnt_Us_nom) * 0.1f;                // Номинальное напряжение АД
       GlobalM4.EnginParam.Fnom = (float)load_s16(ramPnt, progPnt->Pnt_Fs_nom) * 0.1f;               // Номинальное частота АД
       GlobalM4.EnginParam.Vnom = (float)load_s16(ramPnt, progPnt->Pnt_Wr_nom) * 0.1f;               // Номинальная скорость двигателя об/мин       
       GlobalM4.EnginParam.Wnom = GlobalM4.EnginParam.Wmax = GlobalM4.EnginParam.Fnom;
       GlobalM4.EnginParam.Pnom = (float)load_s16(ramPnt, progPnt->Pnt_T0e) * 0.1f;            // Номинальная мощность АД
       GlobalM4.EnginParam.cosFi = (float)load_s16(ramPnt, progPnt->Pnt_Motor_CosFi) * 0.001f; // Номинальная косинус двигателя
       GlobalM4.EnginParam.Kpd = (float)load_s16(ramPnt, progPnt->Pnt_Motor_Kpd) * 0.001f; // Номинальная косинус двигателя                    
       
       // Делаем авторасчет Zp по паспортным данным скорости и частоты
       GlobalM4.EnginParam.Zp = (u16)(60.0f * GlobalM4.EnginParam.Fnom / GlobalM4.EnginParam.Vnom + 0.5f);
       save_s16(ramPnt, progPnt->Pnt_Zp, GlobalM4.EnginParam.Zp);		                     //– число пар полюсов Для вывода на параметр
       //GlobalM4.EnginParam.Zp = load_s16(ramPnt, progPnt->Pnt_Zp);		                     //– число пар полюсов;

       // Делаем расчет или вычитку номинального момента, в зависимости от GlobalM4.EnginParam.TorqueAutoSet
       TorqueTemp = (float)load_s16(ramPnt, progPnt->Pnt_RegPos_Kp) * 0.1f; // Читаем момент
       if (sPnt->EngineParam.TorqueAutoSet) {  // Проверяем режим расчета момента
         GlobalM4.EnginParam.Torque_n = 9550 * GlobalM4.EnginParam.Pnom / GlobalM4.EnginParam.Vnom; // Mном = 9550 * Рном(кВт) / Vном (об/мин)
		 save_s16(ramPnt, progPnt->Pnt_RegPos_Kp, (u16)(GlobalM4.EnginParam.Torque_n * 10.0f));
       } else {
         GlobalM4.EnginParam.Torque_n = TorqueTemp;
       }
              
       GlobalM4.Kez = (float)load_s16(ramPnt, progPnt->Pnt_Kez) * 0.01f;	                     // коэффициент задания по ЭДС - ОНИ В ОБЩЕМ-ТО НЕ НУЖНЫ
       //GlobalM4.Mez = FLOAT_to_IQ((float)load_s16(ramPnt, progPnt->Pnt_Me_z), Torq_base);            // Задание электромагнитного момента, Нм -  - ОНИ В ОБЩЕМ-ТО НЕ НУЖНЫ
       
//    if (GlobalM4.EnginParam.Rs) {
//      // номинальный момент
//      Mnmot = GlobalM4.EnginParam.Pnom*1000.0f / (2.0f*PI*GlobalM4.EnginParam.Vnom/60.0f);    
//      // номинальный рачетный критический момент
//      MkestNom = 3.0f*powf(GlobalM4.EnginParam.Usn / sqrtf(3.0f), 2) / (4.0f*PI*GlobalM4.EnginParam.Fnom/GlobalM4.EnginParam.Zp * (GlobalM4.EnginParam.Rs + sqrtf(powf(GlobalM4.EnginParam.Rs, 2) + powf(2.0f*PI*GlobalM4.EnginParam.Fnom*2.0f*GlobalM4.EnginParam.Lls, 2))));
//      // номинальное рачетное критическое скольжение
//      skestNom = GlobalM4.EnginParam.Rs / sqrtf(powf(GlobalM4.EnginParam.Rs,2) + powf(2.0f*PI*GlobalM4.EnginParam.Fnom*2.0f*GlobalM4.EnginParam.Lls,2));
//      // номинальное скольжение
//      snom = (GlobalM4.EnginParam.Fnom - GlobalM4.EnginParam.Vnom*GlobalM4.EnginParam.Zp/60) / GlobalM4.EnginParam.Fnom;
//      // номинальный расчетный ток ротора
//      I2nomEst = sqrtf(powf((GlobalM4.EnginParam.Usn/sqrt(3)*GlobalM4.EnginParam.cosFi - GlobalM4.EnginParam.Rs*GlobalM4.EnginParam.Isn), 2) + powf((GlobalM4.EnginParam.Usn/sqrtf(3)*sqrtf(1-powf(GlobalM4.EnginParam.cosFi,2)) - 2.0f*PI*GlobalM4.EnginParam.Fnom*GlobalM4.EnginParam.Lls*GlobalM4.EnginParam.Isn), 2)) / sqrtf(powf((GlobalM4.EnginParam.Rs + GlobalM4.EnginParam.Rr/snom),2) + powf((2.0f*PI*GlobalM4.EnginParam.Fnom*2.0f*GlobalM4.EnginParam.Lls),2));
//      
//      // рачет тока холостого хода
//      // 1 - через критический момент
//      InoLoadEst1 = GlobalM4.EnginParam.Isn*(sqrtf(1-powf(GlobalM4.EnginParam.cosFi,2)) - 1 / (2*(MkestNom/Mnmot))*GlobalM4.EnginParam.cosFi);
//      // 2-3 - через закон Кирхгофа и ток ротора
//      InoLoadEst2 = GlobalM4.EnginParam.Isn*sinf(atanf( ( (GlobalM4.EnginParam.Usn/sqrt(3)*sqrtf(1-powf(GlobalM4.EnginParam.cosFi,2)) - GlobalM4.EnginParam.Isn*(2.0f*PI*GlobalM4.EnginParam.Fnom*GlobalM4.EnginParam.Lls))/(GlobalM4.EnginParam.Usn/sqrt(3)*GlobalM4.EnginParam.cosFi - GlobalM4.EnginParam.Isn*GlobalM4.EnginParam.Rs)))) - I2nomEst * sinf(atanf(2.0f*PI*GlobalM4.EnginParam.Fnom*GlobalM4.EnginParam.Lls*snom/GlobalM4.EnginParam.Rr));
//      InoLoadEst3 = sqrt(powf(GlobalM4.EnginParam.Isn,2) + powf(I2nomEst,2) - 2*GlobalM4.EnginParam.Isn*I2nomEst*cosf(acosf(GlobalM4.EnginParam.cosFi) - atanf(2.0f*PI*GlobalM4.EnginParam.Fnom*2.0f*GlobalM4.EnginParam.Lls / (GlobalM4.EnginParam.Rs + GlobalM4.EnginParam.Rr/snom))));
//      // среднее значение тока ХХ метода 2,3,4
//      InoLoadEstMean = (InoLoadEst1 + InoLoadEst2 + InoLoadEst3)/3.0f; 
//    }


//       GlobalM4.Rsw = 0.12;// 0.03f; 			                                                // Сопротивление ключа инвертора для мощности 7.5 кВт (Ом)
//       GlobalM4.Udf = 0.95f;  			                                                // Напряжение включения обратного диода (В)
//       GlobalM4.dI = 0.00075; //0.07f; 			                                                // Минимальный ток переключения компесации МВ (А)

       // Параметры для блока компенсации DeadTime
       GlobalM4.DeadTimeInit.DeadTime = (float)load_s16(ramPnt, progPnt->Pnt_Dtime) * 0.1f;        // Dead Time (mks)
       //GlobalM4.DeadTime = (float)load_s16(ramPnt, progPnt->Pnt_Dtime) * 0.1f;          // DeadTime + перевод в мкс       
       GlobalM4.DeadTimeInit.Rsw = (float)AllPrmPnt->IGBT_KEY_RESISTANCE * 0.0001f; // Сопротивление ключа инвертора (Ом)
       GlobalM4.DeadTimeInit.Udf = (float)AllPrmPnt->RECT_U_OPEN * 0.01f;   // Напряжение включения обратного диода (В)
       GlobalM4.DeadTimeInit.Utf = (float)AllPrmPnt->VOLTAGE_U_IGBT * 0.01f;   // прямое падение напр. на IGBT, В
       GlobalM4.DeadTimeInit.dI  = (float)AllPrmPnt->I_MIN_COMPENSATE * 0.001f;   // Минимальный ток переключения компенсации МВ (А)
       GlobalM4.DeadTimeInit.nIGBT = AllPrmPnt->PARALEL_IGBT_COUNT;              // количество IGBT в параллель
       GlobalM4.DeadTimeInit.nCell = 1;            // количество ячеек инверторов
       GlobalM4.DeadTimeInit.Ton = (float)AllPrmPnt->TON_IGBT_DTC* 0.01f;   // время включения IGBT, мкс
       GlobalM4.DeadTimeInit.Toff = (float)AllPrmPnt->TOFF_IGBT_DTC * 0.01f; // время отключения IGBT, мкс
       GlobalM4.DeadTimeInit.ModeDtc = AllPrmPnt->DTC_COMP_MODE;          // Выбор алгоритма компенсации дедтайма
                     
       GlobalM4.FlagCommand.bit.EMFRegEnb = 0x03 & load_s16(ramPnt, progPnt->Pnt_T0wr);		// Выбор режима ослабления поля
       GlobalM4.FlagCommand.bit.FiltrRefSpdEnb = load_s16(ramPnt, progPnt->Pnt_Reg_Iq_Lim);	// вкл/выкл фильтра ослабления поля
       GlobalM4.FlagCommand.bit.ARollbackEnb = load_s16(ramPnt, progPnt->Pnt_Tfwrz);		// вкл/выкл Антиоткатной функции
       
       // Параметры двигателя
       GlobalM4.EnginParam.Rr		 = (f32)load_s16(ramPnt, progPnt->Pnt_Rr) * 0.001f;
       GlobalM4.EnginParam.Rs		 = (f32)load_s16(ramPnt, progPnt->Pnt_Rs) * 0.001f;			
       GlobalM4.EnginParam.Lm		 = (f32)load_s16(ramPnt, progPnt->Pnt_Lm) * 0.0001f;
       GlobalM4.EnginParam.Lls		 = load_float(ramPnt, progPnt->Pnt_Lls);
       GlobalM4.EnginParam.Lsd		 = (f32)load_s16(ramPnt, progPnt->Pnt_Lsd) * 0.0001f;
       GlobalM4.EnginParam.Lsq		 = (f32)load_s16(ramPnt, progPnt->Pnt_Lsq) * 0.0001f;			
       GlobalM4.EnginParam.Ixx		 = (f32)load_s16(ramPnt, progPnt->Pnt_Ixx) * 0.1f;			
       GlobalM4.EnginParam.Jr		 = load_float(ramPnt, progPnt->Pnt_Jr );
       GlobalM4.EnginParam.PsiR		 = (f32)load_s16(ramPnt, progPnt->Pnt_PsiRotor) * 0.001f;

       // Коеффициенты регулятора скорости
       if(load_s16(ramPnt, progPnt->Pnt_Regi_Ki)){			
         GlobalM4.FlagCommand.bit.ManualSpdReg = 0;    // Автоматический расчет
       } else {
         GlobalM4.FlagCommand.bit.ManualSpdReg = 1;    // Ручной расчет
       }
       
       // Коеффициенты регулятора тока
       if(load_s16(ramPnt, progPnt->Pnt_AutosetCurReg)){			
         GlobalM4.FlagCommand.bit.ManualCurReg = 0;    // Автоматический расчет
       } else {
         GlobalM4.FlagCommand.bit.ManualCurReg = 1;    // Ручной расчет
       }       
       
       if (GlobalM4.FlagCommand.bit.AciParamChange) { // Если вызывался пересчет параметров, значит параметр поменялся через ТехПО или после измерения параметров
          // И его надо сохранить в Fram
          // Сохраняем данные в том виде, в котором они есть - т.е. Float
          sPnt->EngineParam.Rr	     = ((float)load_s16(ramPnt, progPnt->Pnt_Rr) + 0.5f) * 0.001f;
          sPnt->EngineParam.Rs	     = ((float)load_s16(ramPnt, progPnt->Pnt_Rs) + 0.5f) * 0.001f;	    // Значения из микропрограммы по умолчанию
          sPnt->EngineParam.Lm	     = ((float)load_s16(ramPnt, progPnt->Pnt_Lm) + 0.5f) * 0.0001f;
          sPnt->EngineParam.Lls	     = load_float(ramPnt, progPnt->Pnt_Lls );
          sPnt->EngineParam.Lsd	     = ((float)load_s16(ramPnt, progPnt->Pnt_Lsd) + 0.5f) * 0.0001f;
          sPnt->EngineParam.Lsq	     = ((float)load_s16(ramPnt, progPnt->Pnt_Lsq) + 0.5f) * 0.0001f;	    // Значения из микропрограммы по умолчанию
          sPnt->EngineParam.Ixx	     = ((float)load_s16(ramPnt, progPnt->Pnt_Ixx) + 0.5f) * 0.1f;	    // Значения из микропрограммы по умолчанию
          sPnt->EngineParam.Jr	     = load_float(ramPnt, progPnt->Pnt_Jr );
          sPnt->EngineParam.PsiR     = ((float)load_s16(ramPnt, progPnt->Pnt_PsiRotor) + 0.5f) * 0.001f;    // Возможно он не нужен в реальности.
          
          // Параметры регулятора скорости
          if(GlobalM4.FlagCommand.bit.ManualSpdReg == 0){                       // Автоматический расчет       
              GlobalM4.KpSpd = sPnt->EngineParam.RegWr_Kp = GlobalM4.KpSpdCalc;     
              GlobalM4.KiSpd = sPnt->EngineParam.RegWr_Ki = GlobalM4.KiSpdCalc;
            
              save_s16(ramPnt, progPnt->Pnt_CalcRegILim, (s16)(GlobalM4.KpSpd * 100.0f + 0.5f));
              save_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim, (s16)(GlobalM4.KiSpd * 10000.0f + 0.5f));
          }
          else{                                                                 // Ручное задание
              GlobalM4.KpSpd = sPnt->EngineParam.RegWr_Kp = ((float)load_s16(ramPnt, progPnt->Pnt_CalcRegILim) + 0.5f) * 0.01f;     
              GlobalM4.KiSpd = sPnt->EngineParam.RegWr_Ki = ((float)load_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim) + 0.5f) * 0.0001f;
          }
          
          // Параметры регулятора тока
          if(GlobalM4.FlagCommand.bit.ManualCurReg == 0){                       // Автоматический расчет       
              GlobalM4.KpCur = sPnt->EngineParam.KpCur = GlobalM4.KpCurCalc;     
              GlobalM4.KiCur = sPnt->EngineParam.KiCur = GlobalM4.KiCurCalc;
            
              save_s16(ramPnt, progPnt->Pnt_CurRegKp, (s16)(GlobalM4.KpCur * 100.0f + 0.5f));
              save_s16(ramPnt, progPnt->Pnt_CurRegKi, (s16)(GlobalM4.KiCur * 10000.0f + 0.5f));
          }
          else{                                                                 // Ручное задание
              GlobalM4.KpCur = sPnt->EngineParam.KpCur = ((float)load_s16(ramPnt, progPnt->Pnt_CurRegKp) + 0.5f) * 0.01f;     
              GlobalM4.KiCur = sPnt->EngineParam.KiCur = ((float)load_s16(ramPnt, progPnt->Pnt_CurRegKi) + 0.5f) * 0.0001f;
          }          
          
          WrFramMicro(/*&nv.nvEngineParam*/ nvEngineParam, &sPnt->EngineParam, sizeof(nvEngineParam_type));	   // Обновить значения во FRAM
       }
              
       // Параметры ШИМ
       #ifdef _PROJECT_FOR_VS2005_      
       GlobalM4.PwmCalcTime_us = 100.0; // Защитная инициализация для симуляции.
       MaxFPwm = 1000000 / (GlobalM4.PwmCalcTime_us*1.5f); // Находим предельную частоту ШИМа, с учетом полуторного запаса по такту ШИМа
       #endif

       #ifdef _PROJECT_FOR_STM32_
       MaxFPwm = 1000000 / (GlobalM4.PwmCalcTime_us * _FPWM_SAFETY_FACTOR); // Находим предельную частоту ШИМа, с учетом полуторного запаса по такту ШИМа
       #endif
       GlobalM4.Fpwm = (float)load_s16(ramPnt, progPnt->Pnt_Fpwm) * 10.0f;      // Частота ШИМ + перевод в Гц
       GlobalM4.PwmDCmode.FpwmDC = (float)load_s16(ramPnt, progPnt->Pnt_FpwmDC) * 10.0f;    // Частота ШИМ на DC + перевод в Гц
       GlobalM4.PwmDCmode.PwmDCenable = load_s16(ramPnt, progPnt->Pnt_FpwmDCenable);    // Активация режиме смены частоты ШИМа на постоянном токе
       
       if (GlobalM4.Fpwm > MaxFPwm) {
           GlobalM4.Fpwm = MaxFPwm;
           save_s16(ramPnt, progPnt->Pnt_Fpwm, (u16)(MaxFPwm*0.1f));
       }
       //GlobalM4.DeadTime = (float)load_s16(ramPnt, progPnt->Pnt_Dtime) * 0.1f;  // DeadTime + перевод в мкс
       //GlobalM4.StepAngle = 1.0f / GlobalM4.Fpwm;                               // Шаг угла для скаляра
       if(flagIsFirstCall || GlobalM4.FlagCommand.bit.PwmParamChange){
          GlobalM4.FlagCommand.bit.PwmReInit = 1;                               // Запустить переинициализацию модуля ШИМ
       } 
              
       GlobalM4.FlagCommand.bit.AciParamChange = 0;                             // Запрос по изменению параметров отработан - сбросить флаг 
       GlobalM4.FlagCommand.bit.PwmParamChange = 0;                             // Запрос по изменению параметров ШИМ отработан - сбросить флаг
       GlobalM4.FlagCommand.bit.SysCalc = 1;
    } // End flag bit.SysCalc!!!
    
	//******************************************************************************    
	// Параметры блока симмуляции
	//******************************************************************************          
	#ifdef _PROJECT_FOR_STM32_     
	#warning Нужен отдельный блок для симуляции, а то не красиво так втягивать данные    
		GlobalM4.Simulation.Enable = AllPrmPnt->VFD_SIMULATION_MODE;
		GlobalM4.Simulation.Imot   = AllPrmPnt->VFD_SIMULATION_IMOT;
		GlobalM4.Simulation.Udc    = AllPrmPnt->VFD_SIMULATION_UD;
		GlobalM4.Simulation.Uin    = AllPrmPnt->VFD_SIMULATION_UIN;

		AllPrmPnt->SIMULATION_MODE_IN_ACTION = 0;
		if (GlobalM4.Simulation.Enable) {
			AllPrmPnt->SIMULATION_MODE_IN_ACTION = 1;
		}

	#endif

	#ifdef _PROJECT_FOR_VS2005_
	// Симуляция работы в VS2005 без реального железа
	{		
		double	Zs_sim, Rs_sim, Xs_sim, Fout_sim, encoSpd_sim, drvFref, deltaFenco, spdPhaseSign;
		u16		phaseParam;
		//const double	_2PI = 2*3.141592654;
		static double	encoPhase_sim = 0, phaseCur_sim = 0; 
		static u16		Rsygnal_sim=0, cntTestDropDown=0;
		
		if (flagIsFirstCall) {
			srand (time(NULL));	
			phaseCur_sim = encoPhase_sim = ((rand()%1000) * 0.001 * _2PI);	// Предустановка случайной фазы энкодера
			GlobalM4.Angle = 0.0;											// Обнуление фазы привода
		}

		AllPrmPnt->SIMULATION_MODE_IN_ACTION = 0;
		if (AllPrmPnt->VFD_SIMULATION_MODE) {
			// Задаем тип силового блока для СР24В
			GlobalM4.DrvType = LowPow_PB24;

			GlobalM4.Fpwm = 5000;
			GlobalM4.Simulation.Enable = AllPrmPnt->VFD_SIMULATION_MODE;
			GlobalM4.Simulation.Imot   = AllPrmPnt->VFD_SIMULATION_IMOT;
			GlobalM4.Simulation.Udc    = AllPrmPnt->VFD_SIMULATION_UD;
			GlobalM4.Simulation.Uin    = AllPrmPnt->VFD_SIMULATION_UIN;
		    	    
			GlobalM4.ValUd.Val  = GlobalM4.Simulation.Udc;
			GlobalM4.ValUab.Val = GlobalM4.Simulation.Uin;
			GlobalM4.ValUbc.Val = GlobalM4.Simulation.Uin;
			GlobalM4.ValUca.Val = GlobalM4.Simulation.Uin;
			
			// Обход алгоритма контроля тормозного резистора.
			if (GlobalM4.TormSwParam.TormCheckStart) GlobalM4.TormSwParam.TormCheckEnd=1;

			// В симуляции VS2005 заменяем выходную частоту на задание с генератора темпа. Пока временно.
			drvFref = load_float(ramPnt, progPnt->Pnt_F);
			GlobalM4.We = IQ_to_FLOAT(drvFref, Herz_base);
			GlobalM4.Uout = GlobalM4.Uref = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_U), Volt_base);

			Fout_sim = GlobalM4.We;
			Rs_sim = GlobalM4.EnginParam.Rs;
			Xs_sim = _2PI * Fout_sim * GlobalM4.EnginParam.Lm;
			Zs_sim = sqrt(Rs_sim * Rs_sim + Xs_sim * Xs_sim);

			GlobalM4.Ifull = GlobalM4.ValIu.Val = GlobalM4.ValIv.Val = GlobalM4.ValIw.Val = GlobalM4.Uout / Zs_sim;
			
			phaseCur_sim += (_2PI * GlobalM4.We * FLOAT_TIME_CALL_MICRO);
			if (phaseCur_sim > _2PI)	phaseCur_sim -= _2PI; 
			GlobalM4.ValIu.FlashVal = GlobalM4.ValIu.Val * sin(phaseCur_sim);
			GlobalM4.ValIv.FlashVal = GlobalM4.ValIv.Val * sin(phaseCur_sim - _2PI/3.0);
			GlobalM4.ValIw.FlashVal = GlobalM4.ValIw.Val * sin(phaseCur_sim + _2PI/3.0);


			// Симуляция работы флага возбуждения двигателя.
			if (GlobalM4.FlagCommand.bit.PwmOn) {
				GlobalM4.FlagState.bit.vPrUp = 1;
			} else {
				GlobalM4.FlagState.bit.vPrUp = 0;
			}

			// Симуляция фазы привода в скалярном режиме.
			GlobalM4.Angle += (_2PI * GlobalM4.We * FLOAT_TIME_CALL_MICRO); // Интегрируем скорость и находим суммарную фазу привода
			if (GlobalM4.Angle >= _2PI)		GlobalM4.Angle -= _2PI;
			if (GlobalM4.Angle <= -_2PI)	GlobalM4.Angle += _2PI;

			// В симуляции еще нужна возможность установки блока расширения, иначе часть возможность будет недоступна.
			GlobalM4.extConnectedBlocks.ext1.extType = SIN_COS_EXT_BLOCK;

			// Симуляция скорости энкодера
			deltaFenco = 0.0f;
			// Если задание с генератора темпа уже не ноль, то вводим поправку.
			if (drvFref != 0.0)		deltaFenco = 0.0f;//0.3f * (rand()%1000) * 0.001; // Шумоподобный сигнал скорости с энкодера
			
	/*		// Сдвижка скорости после снятия тормоза - иммитация падения кабины 
			if (!GlobalM4.FlagCommand.bit.PwmOn)		cntTestDropDown = 0;
			if (GlobalM4.FlagCommand.bit.BreakOff){
				
				if (cntTestDropDown < K_TIME_CALL_MICRO_TAKT) { // время падения кабины 1 сек 
					deltaFenco = 0.3f;
					cntTestDropDown ++;
				}
				
			}*/
			
			phaseParam = AllPrmPnt->ENCO_PHASING; // Текущее значение фазировки энкодера - прямая или инверсная.       
			spdPhaseSign = phaseParam ? -1.0 : 1.0;

	//		if (phaseParam){     
	//			encoSpd_sim = -(GlobalM4.We + deltaFenco); // Скорость задаем с учетом отклонения скорости при тестировании
	//		} else {
	//			encoSpd_sim = (GlobalM4.We + deltaFenco);
	//		}
			encoSpd_sim = spdPhaseSign * (GlobalM4.We + deltaFenco); // Корректируем знак скорости
			GlobalM4.EncoderCalcParam.electricSpd = FLOAT_to_IQ (encoSpd_sim, Herz_base); 

			// Симуляция фазы энкодера по абсолютной скорости, а учет знака фазировки для фазы будет ниже 
			encoPhase_sim = encoPhase_sim + (_2PI * encoSpd_sim * FLOAT_TIME_CALL_MICRO); // Интегрируем скорость и находим суммарную фазу
			if (encoPhase_sim > _2PI) {
				encoPhase_sim -= _2PI;
				Rsygnal_sim ++; // Если есть прокрутка фазы, увеличиваем счетчик R сигнала
			} else if (encoPhase_sim < 0) {
				encoPhase_sim += _2PI;
				Rsygnal_sim ++; // Если есть прокрутка фазы, увеличиваем счетчик R сигнала
			}

			// Находим электрическую фазу энкодера с учетом смещения фазы после калибровки
			// Фазу приводим в относительному виду от 0 до 1
			GlobalM4.EncoderCalcParam.electricTheta = spdPhaseSign * ((GlobalM4.EncoderParam.encoAngleShift / 3600.0) + encoPhase_sim / _2PI);
			// Делаем проверку на скрутки
			if (GlobalM4.EncoderCalcParam.electricTheta >= 1.0 ) GlobalM4.EncoderCalcParam.electricTheta -= 1.0;
			if (GlobalM4.EncoderCalcParam.electricTheta <= -1.0 ) GlobalM4.EncoderCalcParam.electricTheta += 1.0;

			// Иммитация сигнала R-метки
			GlobalM4.EncoderCalcParam.header.bits.Rsygnal = 0;
			
			// Прокрутили фазу на число пар полюсов - ставим глобальный R сигнал 
			if (Rsygnal_sim >= GlobalM4.EnginParam.Zp) {
				GlobalM4.EncoderCalcParam.header.bits.Rsygnal = 1;
				Rsygnal_sim = 0;
			}

			AllPrmPnt->SIMULATION_MODE_IN_ACTION = 1;
		}


	}
	#endif 

    //--------------------------------------------------------------------------
    // Тип настройки Контур скорости (ручное/авто)
    if(GlobalM4.FlagCommand.bit.ManualSpdReg == 0){			
      // Копируем расчетное значение коеффициентов
      GlobalM4.KpSpd = GlobalM4.KpSpdCalc;
      GlobalM4.KiSpd = GlobalM4.KiSpdCalc;
      
      // Если значение изменилось то пишем в FRAM
      if((sPnt->prevKpSpd != GlobalM4.KpSpd)||(sPnt->prevKiSpd != GlobalM4.KiSpd)){
        // Выводим расчетные значения в МП
        save_s16(ramPnt, progPnt->Pnt_CalcRegILim, (s16)(GlobalM4.KpSpd * 100.0f + 0.5f));
        save_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim, (s16)(GlobalM4.KiSpd * 10000.0f + 0.5f));
        
        // Сохранить в FRAM
        WrFramMicro(/*&nv.nvEngineParam.RegWr_Kp*/  &nvEngineParam->RegWr_Kp, &GlobalM4.KpSpd, sizeof(float));	   // 
        WrFramMicro(/*&nv.nvEngineParam.RegWr_Ki*/  &nvEngineParam->RegWr_Ki, &GlobalM4.KiSpd, sizeof(float));	   // 
      }
    }else{
      // Коеффициенты регулятора скорости
      GlobalM4.KpSpd = (float)load_s16(ramPnt, progPnt->Pnt_CalcRegILim) * 0.01f;
      GlobalM4.KiSpd = (float)load_s16(ramPnt, progPnt->Pnt_Reg_Id_Lim) * 0.0001f;
      
      // Если значение изменилось то пишем в FRAM
      if((sPnt->prevKpSpd != GlobalM4.KpSpd)||(sPnt->prevKiSpd != GlobalM4.KiSpd)){
        // Сохранить в FRAM
        WrFramMicro(/*&nv.nvEngineParam.RegWr_Kp*/  &nvEngineParam->RegWr_Kp, &GlobalM4.KpSpd, sizeof(float));	   // 
        WrFramMicro(/*&nv.nvEngineParam.RegWr_Ki*/  &nvEngineParam->RegWr_Ki, &GlobalM4.KiSpd, sizeof(float));	   // 
      }
    }
    
    sPnt->prevKpSpd = GlobalM4.KpSpd;
    sPnt->prevKiSpd = GlobalM4.KiSpd;
    
    //--------------------------------------------------------------------------
    // Читаем разрешенное значение тока перегруза по отношению к текущему МТЗ
    ImaxVFD = (f32)load_float(ramPnt, progPnt->Pnt_ImaxVFD); // Значение в процентах
    if (ImaxVFD > 0.80){
      ImaxVFD = 0.8; // не более 80%
      save_float(ramPnt, progPnt->Pnt_ImaxVFD, 0.8f);
    }
    // Текущая величина потока - она же величина текущего реактивного тока  
    Id_nom = IQ_to_FLOAT(GlobalM4.PhiRTorqIact, Current_base); // Ток возбуждения в А, амплитуда 
    Is_max = pFailsSetup->Imtz * ImaxVFD; // Допустимая величина тока = 80% от МТЗ    
    Iq_max = 0;
#ifdef _PROJECT_FOR_STM32_    
    arm_sqrt_f32(((Is_max * Is_max) - (Id_nom * Id_nom)), &Iq_max);
#endif
    ImaxVFD = FLOAT_to_IQ(Iq_max, Current_base);  
        
    if (GlobalM4.FlagCommand.bit.EvacOn) { // Если включен режим эвакуации, надо заменить предел активного тока на задание для эвакуации
      ImaxVFD = GlobalM4.ImaxEvac;
    }   
    GlobalM4.ImaxVFD = ImaxVFD;
            
    
    GlobalM4.FlagCommand.bit.EMFRegEnb = load_s16(ramPnt, progPnt->Pnt_T0wr);		// Выбор режима ослабления поля 
    GlobalM4.SoftCurrentTime = 0.001f * (f32)load_s16(ramPnt, progPnt->Pnt_SoftCurDwnTime);           // Читаем значение времени плавного сброса тока
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Тип настройки Контур тока (ручное/авто)
    if(GlobalM4.FlagCommand.bit.ManualCurReg == 0){			
      // Копируем расчетное значение коеффициентов
      GlobalM4.KpCur = GlobalM4.KpCurCalc;
      GlobalM4.KiCur = GlobalM4.KiCurCalc;
      
      // Если значение изменилось то пишем в FRAM
      if((sPnt->prevKpCur != GlobalM4.KpCur)||(sPnt->prevKiCur != GlobalM4.KiCur)){
        // Выводим расчетные значения в МП
        save_s16(ramPnt, progPnt->Pnt_CurRegKp, (s16)(GlobalM4.KpCur * 100.0f + 0.5f));
        save_s16(ramPnt, progPnt->Pnt_CurRegKi, (s16)(GlobalM4.KiCur * 10000.0f + 0.5f));
        
        // Сохранить в FRAM
        WrFramMicro(/*&nv.nvEngineParam.KpCur*/ &nvEngineParam->KpCur, &GlobalM4.KpCur, sizeof(float));	   // 
        WrFramMicro(/*&nv.nvEngineParam.KiCur*/ &nvEngineParam->KiCur, &GlobalM4.KiCur, sizeof(float));	   // 
      }
    }
    else{
      // Коеффициенты регулятора скорости
      GlobalM4.KpCur = (float)load_s16(ramPnt, progPnt->Pnt_CurRegKp) * 0.01f;
      GlobalM4.KiCur = (float)load_s16(ramPnt, progPnt->Pnt_CurRegKi) * 0.0001f;
      
      // Если значение изменилось то пишем в FRAM
      if((sPnt->prevKpCur != GlobalM4.KpCur)||(sPnt->prevKiCur != GlobalM4.KiCur)){
        // Сохранить в FRAM
        WrFramMicro(/*&nv.nvEngineParam.KpCur*/ &nvEngineParam->KpCur, &GlobalM4.KpCur, sizeof(float));	   // 
        WrFramMicro(/*&nv.nvEngineParam.KiCur*/ &nvEngineParam->KiCur, &GlobalM4.KiCur, sizeof(float));	   // 
      }
    }
    
    sPnt->prevKpCur = GlobalM4.KpCur;
    sPnt->prevKiCur = GlobalM4.KiCur;
    //--------------------------------------------------------------------------
    
    // Задание ПЧ по напряжению и частоте
    GlobalM4.DirectRotation = load_s16(ramPnt, progPnt->Pnt_Napravlenie);    // Прочитать уставку направление вращения    
    if(GlobalM4.DirectRotation){
        GlobalM4.Fref = (-1.0f) * IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_F), Herz_base);  // Обратное вращение
    }
    else{
        GlobalM4.Fref = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_F), Herz_base);            // Прямое вращение 
    }
    
    GlobalM4.Uref = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_U), Volt_base);                    // Задание по напряжению
    
    if(GlobalM4.Fref < 0){
        setBitMicro(ramPnt, progPnt->Pnt_FoutSign);
    }
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_FoutSign);
    }
    
    GlobalM4.Isn = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_I_motor), Current_base);  // Номинальный ток двигателя   
    // Поправочный коэф. для расчета момента
    GlobalM4.TorqueScale = (float)load_s16(ramPnt, progPnt->Pnt_K_Il) * 0.001f; // Находим поправочный коэф. для вычисляемого момента
    GlobalM4.FlagCommand.bit.CurrInv = load_s16(ramPnt, progPnt->Pnt_InvCurSens);            // инверсия токов
    GlobalM4.SvgenMode = load_s16 (ramPnt, progPnt->Pnt_PwmType); // Тип генератора ШИМа - Texas vs DPWM60
    // Коэффициенты наблюдателя потока - ручной ввод
    temp_s16 = load_s16(ramPnt, progPnt->Pnt_Ileak_MTZ);//1  K_INTEGR
    GlobalM4.KiFe = (float)temp_s16 * 0.001f;
    temp_s16 = load_s16(ramPnt, progPnt->Pnt_Il_Korrect);//2  K_PROPOR
    GlobalM4.KpFe = (float)temp_s16 * 0.001f;    
        
    GlobalM4.MultIdKoefGM = load_float(ramPnt, progPnt->Pnt_T_ad_Lm);        // Коэф. повышения тока возбуждения в генераторном режиме
//******************************************************************************    
// Командоапарат ПЧ    
//******************************************************************************    
    clrBitMicro(ramPnt, progPnt->Pnt_MeasParamInterrupt);
    // основное тело алгоритма DRV_CMD.h
  //--- Проверка наличия исключительных ситуаций
  if(!testBitMicro(ramPnt, progPnt->Pnt_FailTek)){		     // если в данный момент их нет, проверяем и отрабатываем поступающие команды
      sPnt->algState.bit.failCmd= 0;
      temp_s16 = testBitMicro(ramPnt, progPnt->Pnt_Stop);
      if((testBitMicro(ramPnt, progPnt->Pnt_Stop)) && (!sPnt->algState.bit.stopCmd) && (sPnt->algState.bit.runCmd)){
           //setBitMicro(ramPnt, progPnt->Pnt_NormStopMode);   // в POWER_LOSS_APV сигнализируем о команде останова
           sPnt->algState.bit.stopCmd = 1;                   // Переводим апарат состояний в состояние stopCmd
           
           if (GlobalM4.FlagCommand.bit.mWorkEnb)       setBitMicro(ramPnt, progPnt->Pnt_MeasParamInterrupt);
           
           if((testBitMicro(ramPnt, progPnt->Pnt_StopVibeg)) || (sPnt->puskState < running)){
               sPnt->brakeType = rampdown;                   // rampdown выбран по дискр. вх. или получили СТОП вначале ПУСКа, когда прозваниваем или сушим
           }  
           else{
               sPnt->brakeType = (BRAKEtype_enum)load_s16(ramPnt, progPnt->Pnt_SelTormType); 
           }
           
           clrBitMicro(ramPnt, progPnt->Pnt_WorkRunMode);    // больше не даем команду записи флага работающего двигателя
      }
      else if((testBitMicro(ramPnt, progPnt->Pnt_Pusk)&&(!sPnt->algState.bit.stopCmd))){
           sPnt->algState.bit.runCmd = 1;
      }      
      //nomIm = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_I_motor), Current_base);  // Номинальный ток двигателя
  }
  else if(!sPnt->algState.bit.failCmd){
      sPnt->algState.value = 0;  
      sPnt->algState.bit.failCmd = 1;
      sPnt->algState.bit.stopCmd = 1;
      sPnt->brakeType = rampdown;
            
      clrBitMicro(ramPnt, progPnt->Pnt_WorkRunMode);         // больше не даем команду записи флага работающего двигателя
  }		                                             // Иначе переводим командоаппарат в "Аварйный" режим

  // Выбор типа управления двигателем
  typeDrv = (ControlDrvType_type)load_s16(ramPnt, progPnt->Pnt_TypeDrv);   // Сохранить в глобальную структуру тип управления ПЧ
  GlobalM4.DriveType = typeDrv;                                            // 0=скаляр без компенсации МВ, 1=скалярный, 2=ВУ АД с ОС, и т.д.
  
  // В скаляре тоже можно измерять параметры АД - СОСНОВЧИК 12-03-2021
  //  if((load_s16(ramPnt, progPnt->Pnt_CalcPrmMot) == 1) && (TYPEDRV_ANYSCALAR & (1<<typeDrv)) ) 
  //      save_s16(ramPnt, progPnt->Pnt_CalcPrmMot, 0);
  
  // Читаем параметры торможения постоянным током 
  if (GlobalM4.EnginParam.Isn != 0) {
	  MaxDcCurrent = (pFailsSetup->Imtz * 0.55f / GlobalM4.EnginParam.Isn);
          if (MaxDcCurrent > 3.0f)       MaxDcCurrent = 3.0f;   // Предел не должен быть больше 300%
          
	  save_float(ramPnt, progPnt->Pnt_DC_MaxCurrentOut, MaxDcCurrent); // Верхний предел DC тока 80%/sqrt(2) от МТЗ, с учетом того что на постоянке МТЗ занижается
	  DC_Current = load_float(ramPnt, progPnt->Pnt_CurTormDc);
	  if (MaxDcCurrent < DC_Current) { // Если текущее значение тока ПТ больше предела, принудительное ограничение
		save_float(ramPnt, progPnt->Pnt_CurTormDc, MaxDcCurrent);
		DC_Current = MaxDcCurrent;
	  } 
	  DC_Current *= GlobalM4.EnginParam.Isn; 
	  GlobalM4.DCBrake.Current = FLOAT_to_IQ(DC_Current, Current_base);  // В систему управления передаем относительные значения
	  GlobalM4.DCBrake.Enable = load_s16(ramPnt, progPnt->Pnt_DC_Brk_En);                
	  GlobalM4.DCBrake.Frequency = load_s16(ramPnt, progPnt->Pnt_F_DCSTP) * 0.1f; // Один знак после запятой
  }
  //===========================================================================
  // Отработка стопов
  if(sPnt->algState.bit.stopCmd){  //если нужно остановить двигатель или привод не задействован
        sPnt->algState.bit.runCmd = 0;		         //Для начала, сбрасываем командоапарат пуска
	sPnt->puskState = startUp;
        switch(sPnt->brakeType){
                       
            case frequency:{         // Частотное торможение
                // Сравниваем частоту активации торможения постоянным током (текущую частоту домножаю на 10 и беру u16, для упрощения сравнения с параметром)
                // Переход в режим торможения постоянным током разрешаем только в режиме для асинхронного двигателя.
                if ( (((u16)(fabsf(GlobalM4.Fref))) <= GlobalM4.DCBrake.Frequency) && GlobalM4.DCBrake.Enable && (TYPEDRV_ANYACIM & (1 << GlobalM4.DriveType))) {
                    GlobalM4.DCBrake.ActionDC = 1; // Разрешаем торможение постоянным током
                }
              
                if(!testBitMicro(ramPnt, progPnt->Pnt_StopFreq)){
                    if(load_s16(ramPnt, progPnt->Pnt_StateGT) & (RefZero|EquRV|Freeze)){
                        sPnt->algState.bit.driveRunning = sPnt->algState.bit.PwmOn = 0;                       // Отключить ШИМ                         
                        sPnt->brakeType = rampdown;
                    } else { 
                        setBitMicro(ramPnt, progPnt->Pnt_StopFreq);         // установить признак частотного торможения
                    }
                } else if((load_s16(ramPnt, progPnt->Pnt_StateGT) & (RefZero|Freeze)) == RefZero){	 // Ждем пока GT перейдет в режим нулевой частоты или выходная частота будет ниже частоты пуска
                    if((load_s16(ramPnt, progPnt->Pnt_SoftCurDwnEna)) && (TYPEDRV_ANYFOC & (1 << typeDrv))){
                        sPnt->brakeType = softcurrentdown;                  // Перейти в режим плавного сброса тока
                    } else {
                        clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);         // сбросить признак частотного торможения
                        sPnt->algState.bit.driveRunning = sPnt->algState.bit.PwmOn = 0;                       // Отключить ШИМ
                        sPnt->brakeType = rampdown;
                    }
                }          
            } break;
            
            case rampdown:{         // Торможение выбегом (отработка времени выбега)
                if(PwmIsOn()){
                   sPnt->rampDownTimer = load_s16(ramPnt, progPnt->Pnt_VibegTime) * K_TIME_CALL_MICRO_TAKT;
                   sPnt->algState.bit.driveRunning = sPnt->algState.bit.PwmOn = 0;                                // Отключить ШИМ
                   setBitMicro(ramPnt, progPnt->Pnt_VibegStat);  
                   if(GlobalM4.FlagCommand.bit.mWorkEnb){
                      GlobalM4.FlagCommand.bit.mWorkEnb = 0;            // Сброс режима измерения параметров двигателя
                      save_s16(ramPnt, progPnt->Pnt_CalcPrmMot, 0);	// Сброс запуска определения параметров
                   }                   
                } else if(sPnt->rampDownTimer){ 
                      sPnt->rampDownTimer--;
                } else {
                    sPnt->algState.bit.stopCmd = 0;			   // Отработали останов, сбрасываем командоапарат
                    save_s16(ramPnt, progPnt->Pnt_StatusDC, 0);		   // Если перед этим отрабатывались drying или combi
                    clrBitMicro(ramPnt, progPnt->Pnt_VibegStat);
                    GlobalM4.DCBrake.ActionDC = 0;
                    GlobalM4.FlagCommand.bit.RingUp = 0;
                }
                if(load_s16(ramPnt, progPnt->Pnt_StateGT) & (RefZero)){	 // Ждем пока GT перейдет в режим нулевой частоты или выходная частота будет ниже частоты пуска
                    if((load_s16(ramPnt, progPnt->Pnt_SoftCurDwnEna)) && (TYPEDRV_ANYFOC & (1 << typeDrv))){
                        sPnt->brakeType = softcurrentdown;                  // Перейти в режим плавного сброса тока
                    } else {
                        clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);         // сбросить признак частотного торможения
                        sPnt->algState.bit.driveRunning = sPnt->algState.bit.PwmOn = 0;                       // Отключить ШИМ
                        sPnt->brakeType = rampdown;
                    }
                }
            } break;
			
            case softcurrentdown:{    // Плавный сброс тока при останове ПЧ
                if(!sPnt->softCurDwnTimer){
                    GlobalM4.FlagCommand.bit.SoftCurDwn = 1;
                    clrBitMicro(ramPnt, progPnt->Pnt_StopFreq);
                    sPnt->softCurDwnTimer = K_TIME_CALL_MICRO_TAKT;
                } else {
                     if(sPnt->softCurDwnTimer == 1){
                         GlobalM4.FlagCommand.bit.SoftCurDwn = 0;
                         sPnt->rampDownTimer = 0; // Обнулить счетчик выбега, и сразу переходим в стоп.
                         sPnt->brakeType = rampdown;
                     }
                     sPnt->softCurDwnTimer--;	
                }
            } break;    
            
            default: break;
        }
  }
  else if(sPnt->algState.bit.runCmd){  //--- ПУСК
	switch(sPnt->puskState){
            case startUp:{	//Начало пуска
              //sPnt->puskState = contyChk; 
              sPnt->contyChkTimer = 0;
              GlobalM4.DCBrake.ActionDC = 0; // Сбросить торможение постоянным током
              sPnt->puskState ++;
	    } break;
	    
            case motMsurmnt:{   // Измерение параметров двигателя
              if( load_s16(ramPnt, progPnt->Pnt_CalcPrmMot) != 0 ){ // После введения Подхвата, надо и в скаляре иметь влзможность измерять параметры двигателя
                 if(!GlobalM4.FlagCommand.bit.mWorkEnb) {
                      
                     // Определяем алгоритм определения параметров двигателя.
                      GlobalM4.MeasureLmRs = (MEASURE_LM_RS_type)load_s16(ramPnt, progPnt->Pnt_CalcPrmMot); // 0 - Отключено, 1-оценка Lm без вращения, 2-измерение Lm без вращения, 3-с вращением
                   
                      // Сброс для плохого завершения
                      GlobalM4.EnginParam.Rr = GlobalM4.EnginParam.MeasRr = 0.0f;
                      GlobalM4.EnginParam.Rs = GlobalM4.EnginParam.MeasRs = 0.0f;
                      GlobalM4.EnginParam.Lm = GlobalM4.EnginParam.MeasLm = 0.0f;
                      GlobalM4.EnginParam.Lls = GlobalM4.EnginParam.MeasLls = 0.0f;
                      GlobalM4.EnginParam.Lsd = GlobalM4.EnginParam.MeasLsd = 0.0f;
                      GlobalM4.EnginParam.Lsq = GlobalM4.EnginParam.MeasLsq = 0.0f;
                      GlobalM4.EnginParam.Ixx = GlobalM4.EnginParam.MeasIxx = 0.0f;
                      
                      // Тестовое значение Кп для оценки момента инерции
                      GlobalM4.EnginParam.Estim_Kp = 10.0f;
                      
                      //mWorkEnb_set(); 						// Инициировать измерение параметров
                      GlobalM4.FlagCommand.bit.mWorkEnb = 1;
                      GlobalM4.FlagState.bit.mWorkFin = 0;
                 }
                 else if(GlobalM4.FlagState.bit.mWorkFin && GlobalM4.FlagCommand.bit.mWorkEnb) { // Проверка статуса завершения измерения параметров
                      sPnt->EngineParam.Rs =  GlobalM4.EnginParam.Rs  = GlobalM4.EnginParam.MeasRs;
                      sPnt->EngineParam.Rr =  GlobalM4.EnginParam.Rr  = GlobalM4.EnginParam.MeasRr;
                      sPnt->EngineParam.Lm =  GlobalM4.EnginParam.Lm  = GlobalM4.EnginParam.MeasLm;
                      sPnt->EngineParam.Lls = GlobalM4.EnginParam.Lls = GlobalM4.EnginParam.MeasLls;
                      sPnt->EngineParam.Lsd = GlobalM4.EnginParam.Lsd = GlobalM4.EnginParam.MeasLsd;
                      sPnt->EngineParam.Lsq = GlobalM4.EnginParam.Lsq = GlobalM4.EnginParam.MeasLsq;
                      sPnt->EngineParam.Ixx = GlobalM4.EnginParam.Ixx = GlobalM4.EnginParam.MeasIxx;  // Измеренный ток перекидываем в нормальные переменные
                      sPnt->EngineParam.Jr = GlobalM4.EnginParam.Estim_Jr; // Оценка момента инерции
                      
                      WrFramMicro(nvEngineParam, &sPnt->EngineParam, sizeof(nvEngineParam_type));				// Сохранить вычисленные значения
                                            
                      save_s16  (ramPnt, progPnt->Pnt_Rs,  (s16)(GlobalM4.EnginParam.Rs*1000.0f)    );
                      save_s16  (ramPnt, progPnt->Pnt_Rr,  (s16)(GlobalM4.EnginParam.Rr*1000.0f)    );
                      save_s16  (ramPnt, progPnt->Pnt_Lm,  (s16)(GlobalM4.EnginParam.Lm*10000.0f)   );
                      save_float(ramPnt, progPnt->Pnt_Lls, GlobalM4.EnginParam.Lls                  );
                      save_s16  (ramPnt, progPnt->Pnt_Lsd, (s16)(GlobalM4.EnginParam.Lsd*10000.0f)  );
                      save_s16  (ramPnt, progPnt->Pnt_Lsq, (s16)(GlobalM4.EnginParam.Lsq*10000.0f)  );
                      save_s16  (ramPnt, progPnt->Pnt_Ixx, (s16)(GlobalM4.EnginParam.Ixx*10.0f)     );
                      save_float(ramPnt, progPnt->Pnt_Jr, GlobalM4.EnginParam.Estim_Jr);
                      save_s16(ramPnt, progPnt->Pnt_CalcPrmMot, 0);             // Сброс уставки выбора типа измирения параметров двигателя
                      
                      GlobalM4.FlagCommand.bit.AciParamChange = 1;
                      GlobalM4.FlagState.bit.mWorkFin = 0;
                      GlobalM4.FlagCommand.bit.mWorkEnb = 0;
                      sPnt->algState.bit.stopCmd = 1;		//Инициируем останов
                      sPnt->brakeType= rampdown;
                 }
              } else { // Если нет измерения параметров, идем на следующий этап
                sPnt->puskState ++;
              }
            } break;

            case tormResChk:{   // Проверка торомозного резистора. В режиме эвакуации проверку не проводим!!!!
              if (GlobalM4.TormSwParam.TormCheckEn && !GlobalM4.FlagCommand.bit.EvacOn) { // Если проверка резистора  разрешена, то проверяем
                if (!GlobalM4.TormSwParam.TormCheckStart){ // Если флаг запуска проверки тормозного резистора не установлен
                  GlobalM4.TormSwParam.TormCheckStart = 1; // Даем сигнал на начало проверки
                } else if (GlobalM4.TormSwParam.TormCheckEnd == 1) { // Если из такта ШИМ пришел ответ, что проверка окончена
                  sPnt->puskState ++; // переходим на следующий этап, если нас не остановят по ошибке обрыва
                  GlobalM4.TormSwParam.TormCheckStart = GlobalM4.TormSwParam.TormCheckEnd = 0; // Сброс флагов.
                } 
              }
              else {
                sPnt->puskState ++; // Если проверка не требуется - сразу переход на новый этап запуска.
              }			
	    } break;

            case contyChk:{	// Прозвонка
                // Работает только для ВУ
                if(load_s16(ramPnt, progPnt->Pnt_RUena) == 1 && (TYPEDRV_ANYFOC & (1<<typeDrv)) ){
                    if(!sPnt->contyChkTimer){
                        // Прозвонку делаем для всех двигателей, если она разрешена.
                        GlobalM4.FlagCommand.bit.RingUp = 1; // Ставим флаг разрешения прозвонки для такта ШИМа
                        GlobalM4.FlagCommand.bit.RingUpFreezeGT = 1; // Морозим генератор темпа на время прозвонки
                        // Автоматический расчет времени прозвонки в зависимости от постоянной времени двигателя
                        
                        if (load_s16 (ramPnt, progPnt->Pnt_RUtime) != 0) { // Ручное задание времени
                            contyChkTime =  load_s16 (ramPnt, progPnt->Pnt_RUtime) * 0.1f;
                        } else if (TYPEDRV_ACIMFOC & (1<<typeDrv)) {
                            contyChkTime = 6 * (GlobalM4.EnginParam.Lm+GlobalM4.EnginParam.Lls) / GlobalM4.EnginParam.Rr;
                            save_s16 (ramPnt, progPnt->Pnt_RUtime, (u16)(contyChkTime * 10.0f + 0.5));
                        } else if (TYPEDRV_PMSMFOC & (1<<typeDrv)) {
                            contyChkTime = 6 * (GlobalM4.EnginParam.Lsd / GlobalM4.EnginParam.Rs);
                            save_s16 (ramPnt, progPnt->Pnt_RUtime, (u16)(contyChkTime * 10.0f+0.5));                                                
                        }                       
                        GlobalM4.timeLoadContyCheck = contyChkTime;// * GlobalM4.Fpwm; // Время прозвонки
                        sPnt->contyChkTimer = (u16)(contyChkTime * K_TIME_CALL_MICRO_TAKT);
                        sPnt->algState.bit.PwmOn = 1; // Включаем ШИМ для прозвонки
                        sPnt->algState.bit.countyChk = 1;
                    }
                    else{
                        if(sPnt->contyChkTimer == 1) {
                            sPnt->puskState++;
                            sPnt->algState.bit.countyChk = 0;
                            GlobalM4.FlagCommand.bit.RingUp = 0;
                            GlobalM4.FlagCommand.bit.RingUpFreezeGT = 0; // Морозим генератор темпа на время прозвонки                            
                        }
                        sPnt->contyChkTimer--; // Обнуление счетчика без лишнего такта выполнения
                    }
                } else {
                  sPnt->puskState++;
                }
	    } break;
				
            case drying:{	// Сушка
                sPnt->puskState ++;
            } break;
            
            case running:{	// Работа (Нормальный режим работы)
                 sPnt->algState.bit.PwmOn = 1;           // Включить ШИМ !!!
                 sPnt->algState.bit.driveRunning = 1; // Выдаем сигнал, что привод уже поехал. Все проверки завершены.
                 // Для реализации АПВ при аварии по исчезновению питания
                 setBitMicro(ramPnt, progPnt->Pnt_WorkRunMode); // в блок POWER_LOSS_APV сигнализируем о нормальном режиме работы двигателя
                 // По данному сигналу , в модуле POWER_LOSS_APV.h в FRAM запишется флаг работающего двигателя и начнется  запись текущего времени в поле  энергонезависимой         
                 // структуры. Если двигатель остановится по аварии питания, об этом будет сигнализировать          
                 // установленный флаг 
                 
            } break;
        }
  }
  //--- Выдаем статус командоапарата
  GlobalM4.FlagCommand.bit.StartCmd = sPnt->algState.bit.runCmd; // Дублируем приход команды пуска в Глобальном флаге для Логераы
  save_s16(ramPnt, progPnt->Pnt_StatusDRV, sPnt->algState.value);
  GlobalM4.s16_PuskStateDRV = sPnt->puskState; 		// Состояние командоаппарата запуска DRV_V4
  //--- Отработаем запросы командоаппарата pwmOff/pwmOn
  if((GlobalM4.FlagCommand.bit.PwmOn == 1 ) && (!sPnt->algState.bit.PwmOn)){
      GlobalM4.FlagCommand.bit.PwmOn = 0;					  // Штатный запрос на отключение ШИМ
  }
  else if((GlobalM4.FlagCommand.bit.PwmOn == 0 ) && (sPnt->algState.bit.PwmOn)){  // Если есть попытка пуска
      if(load_s16(ramPnt, progPnt->Pnt_CalcPrmMot) == 0){			  // и не активен режим определения параметров, пускаемся. (Аварии и Ud проверяются в блоке PUSK_READY )
           GlobalM4.FlagCommand.bit.PwmOn = 1;                                    // Штатный запрос на включение ШИМ  
      }
  }  
  
  // Формируем статус ШИМа на выход блока
  if(GlobalM4.FlagCommand.bit.PwmOn){       // PWM on  
     setBitMicro(ramPnt, progPnt->Pnt_PwmOn);
     clrBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
//     MODBUS_PWM_ON = 1; // Глобальный флаг для передачи в Модбас
  }
  else{                                     // PWM off
     setBitMicro(ramPnt, progPnt->Pnt_NotPwmOn);
     clrBitMicro(ramPnt, progPnt->Pnt_PwmOn);
//     MODBUS_PWM_ON = 0; // Глобальный флаг для передачи в Модбас     
  }
            
//******************************************************************************    
// Калибровка каналов измерения тока и ЗПТ
//******************************************************************************    
    // Получить юстировочные числа тока и ЗПТ
    UstirUd  = (float)load_s16(ramPnt, progPnt->Pnt_Ustir_Ud);
    UstirUin = (float)load_s16(ramPnt, progPnt->Pnt_Ustir_Uin);
    GlobalM4.FailsSetup.Udustir = (f32)UstirUd; //FLOAT_to_IQ(UstirUd, Volt_base); // Рассчитываем юстировочное число ЗПТ приведенное к базе (для использования в DRV_UD)
      
    UstirCur_u16 = load_s16(ramPnt, progPnt->Pnt_Ustir_Cur);
    UstirCur = ((float)UstirCur_u16) * 0.1f; 

	// Проверка запроса из микропрограммы сбросить все на дефаулт значения
	if (GlobalM4.SetUstDefault == 1) {
          // Если да - то надо не дать изменить калибровки в 1.
          // Для этого из sPnt перезаписываем значения коэффициентов.
          save_float (ramPnt, progPnt->Pnt_K_Udc, sPnt->CalibrUd.Kcor);
          save_float (ramPnt, progPnt->Pnt_K_Iu, sPnt->CalibrIu.Kcor);
          save_float (ramPnt, progPnt->Pnt_K_Iv, sPnt->CalibrIv.Kcor);
          save_float (ramPnt, progPnt->Pnt_K_Iw, sPnt->CalibrIw.Kcor);
          save_float (ramPnt, progPnt->Pnt_K_Uab, sPnt->CalibrUab.Kcor);
          save_float (ramPnt, progPnt->Pnt_K_Ubc, sPnt->CalibrUbc.Kcor);
          save_float (ramPnt, progPnt->Pnt_K_Uca, sPnt->CalibrUca.Kcor);	
	}

    // Напряжение Ud
    TekVal = load_s16(ramPnt, progPnt->Pnt_Udc_Korrect);   // Текущее значение
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Udc, UstirUd, &GlobalM4.ValUd.Gain, GlobalM4.ValUd.Val, &sPnt->CalibrUd, &nvAdrCoef->K_Udc, KoefDiv_Ud);  // Калибровка
    save_s16(ramPnt, progPnt->Pnt_Udc_Korrect, (s16)ValSave);    // Сохранить новое значение Ud            
    temp = FLOAT_to_IQ(ValSave, Volt_base); 
    save_float(ramPnt, progPnt->Pnt_Ud_tek_iq, temp);                         
    
    // Ток фазы U
    TekVal = load_s16(ramPnt, progPnt->Pnt_Iu_Korrect);    // Текущее значение
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Iu, UstirCur, &GlobalM4.ValIu.Gain, GlobalM4.ValIu.Val, &sPnt->CalibrIu, &nvAdrCoef->K_Iu, KoefDiv_Curr);  // Калибровка
    save_s16(ramPnt, progPnt->Pnt_Iu_Korrect, (s16)ValSave);     // Сохранить новое значение Iu
    
    // Ток фазы V
    TekVal = load_s16(ramPnt, progPnt->Pnt_Iv_Korrect);    // Текущее значение
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Iv, UstirCur, &GlobalM4.ValIv.Gain, GlobalM4.ValIv.Val, &sPnt->CalibrIv, &nvAdrCoef->K_Iv, KoefDiv_Curr);  // Калибровка
    save_s16(ramPnt, progPnt->Pnt_Iv_Korrect, (s16)ValSave);     // Сохранить новое значение Iv

    // Ток фазы W    
    TekVal = load_s16(ramPnt, progPnt->Pnt_Iw_Korrect);    // Текущее значение
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Iw, UstirCur, &GlobalM4.ValIw.Gain, GlobalM4.ValIw.Val, &sPnt->CalibrIw, &nvAdrCoef->K_Iw, KoefDiv_Curr);  // Калибровка
    save_s16(ramPnt, progPnt->Pnt_Iw_Korrect, (s16)ValSave);     // Сохранить новое значение Iw
        
    // Напряжение Uab
    TekVal = load_s16(ramPnt, progPnt->Pnt_Uab_Korrect);    // Текущее значение
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Uab, UstirUin, &GlobalM4.ValUab.Gain, GlobalM4.ValUab.Val, &sPnt->CalibrUab, &nvAdrCoef->K_Uab, KoefDiv_Uin);  // Калибровка
    save_s16(ramPnt, progPnt->Pnt_Uab_Korrect, (s16)ValSave);     // Сохранить новое значение Uab

    // Напряжение Ubc
    TekVal = load_s16(ramPnt, progPnt->Pnt_Ubc_Korrect);    // Текущее значение
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Ubc, UstirUin, &GlobalM4.ValUbc.Gain, GlobalM4.ValUbc.Val, &sPnt->CalibrUbc, &nvAdrCoef->K_Ubc, KoefDiv_Uin);  // Калибровка
    save_s16(ramPnt, progPnt->Pnt_Ubc_Korrect, (s16)ValSave);     // Сохранить новое значение Ubc
    
    // Напряжение Uca
    TekVal = load_s16(ramPnt, progPnt->Pnt_Uca_Korrect);    // Текущее значение
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Uca, UstirUin, &GlobalM4.ValUca.Gain, GlobalM4.ValUca.Val, &sPnt->CalibrUca, &nvAdrCoef->K_Uca, KoefDiv_Uin);  // Калибровка
    save_s16(ramPnt, progPnt->Pnt_Uca_Korrect, (s16)ValSave);     // Сохранить новое значение Uca
    
//******************************************************************************
// Температура контроллера STM32
//******************************************************************************
    temp = (float)GlobalM4.LowSpeedAinAdc.Vbat / 4096.0f * 3.3f;   // Напряжение в вольтах на датчике.
    temp = (0.76f - temp)/0.0025f + 48.0f;                         // Температура в градусах.
    
    save_s16(ramPnt, progPnt->Pnt_StatusDC, (s16)((temp * 10.0f) + 0.5f));
   
//******************************************************************************    
// МТЗ
//******************************************************************************    
    mtz_ref_u16 = load_s16(ramPnt, progPnt->Pnt_MTZref);
    mtz_ref = ((float) mtz_ref_u16) * 0.1f;
    UstirCur *= 0.98f;
    if(mtz_ref > UstirCur){
      mtz_ref = UstirCur;
      save_s16(ramPnt, progPnt->Pnt_MTZref,(s16)(mtz_ref * 10));
    }
    pFailsSetup->Imtz = (float)load_s16(ramPnt, progPnt->Pnt_MTZref) * 0.1f;        // Максимальный ток (МТЗ)
       
//******************************************************************************    
// Настройки ВУ
//******************************************************************************    
      GlobalM4.FluxThetaCorMode = load_s16 (ramPnt, progPnt->Pnt_FeCorrectMode);       // Режим коррекции фазы потока
      GlobalM4.FluxThetaOffset  = (f32)((s16)load_s16 (ramPnt, progPnt->Pnt_FeFixedPhase)) * 0.10f;    // Фиксированное смещение фазы
      
      if (load_s16(ramPnt, progPnt->Pnt_FePidAutoset)) {
        temp = 2 * 6.28 * 0.7421;
        GlobalM4.FluxThetaPidKp   = 1.414 * temp;   // Кп регулятора фазы = 2*0.707*2pi*f, where f = 0.7421
        GlobalM4.FluxThetaPidKi   = temp * temp;  // Ки решулятора фазы = (2pi*f)^2 
        save_s16 (ramPnt, progPnt->Pnt_FePidKp, (u16)(GlobalM4.FluxThetaPidKp*100.0f));   // Кп регулятора фазы
        save_s16 (ramPnt, progPnt->Pnt_FePidKi, (u16)(GlobalM4.FluxThetaPidKi*1000.0f));  // Ки решулятора фазы            
      } else {
        GlobalM4.FluxThetaPidKp   = (f32)((s16)load_s16 (ramPnt, progPnt->Pnt_FePidKp)) * 0.01f;   // Кп регулятора фазы
        GlobalM4.FluxThetaPidKi   = (f32)((s16)load_s16 (ramPnt, progPnt->Pnt_FePidKi)) * 0.001f;  // Ки решулятора фазы    
      } 

//******************************************************************************    
// Параметры двигателя
//******************************************************************************    
    temp = load_float(ramPnt, progPnt->Pnt_I_motor);                                     // Номинальный ток двигателя
    GlobalM4.EnginParam.Isn = IQ_to_FLOAT(temp, Current_base);
    save_s16(ramPnt, progPnt->Pnt_Inom_Motor, (s16)((GlobalM4.EnginParam.Isn *10) + 0.5));
    
    GlobalM4.EnginParam.Imax = (float)load_s16(ramPnt, progPnt->Pnt_I_max);              // Максимальный моменто образующий ток двигателя    
    GlobalM4.EnginParam.Pnom = (float)load_s16(ramPnt, progPnt->Pnt_T0e) * 0.1f;            // Номинальная мощность АД
    //GlobalM4.EnginParam.Usn = (float)load_s16(ramPnt, progPnt->Pnt_Us_nom) * 0.1f;          // Номинальное напряжение АД
    GlobalM4.EnginParam.Usn = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Us_nom), Volt_base); //(float)load_s16(ramPnt, progPnt->Pnt_Us_nom) * 0.1f;                // Номинальное напряжение АД
    GlobalM4.EnginParam.Fnom = (float)load_s16(ramPnt, progPnt->Pnt_Fs_nom) * 0.1f;         // Номинальное частота АД
    GlobalM4.TorqueNomPu = FLOAT_to_IQ(GlobalM4.EnginParam.Torque_n, Torq_base) * 0.1f;     // Относительный номинальный момент
    GlobalM4.EnginParam.Vnom = (u16)load_s16(ramPnt, progPnt->Pnt_Wr_nom) * 0.1f;               // Номинальная скорость двигателя об/мин    
    GlobalM4.EnginParam.Wnom = GlobalM4.EnginParam.Wmax = GlobalM4.EnginParam.Fnom;
    // Делаем расчет Zp по паспортным данным
    GlobalM4.EnginParam.Zp = (u16)(60.0f * GlobalM4.EnginParam.Fnom / GlobalM4.EnginParam.Vnom + 0.5f);
    save_s16(ramPnt, progPnt->Pnt_Zp, GlobalM4.EnginParam.Zp);		                     //– число пар полюсов Для вывода на параметр
    //GlobalM4.EnginParam.Zp = load_s16(ramPnt, progPnt->Pnt_Zp);		                     //– число пар полюсов;

    // Делаем расчет или вычитку номинального момента, в зависимости от GlobalM4.EnginParam.TorqueAutoSet
    TorqueTemp = (float)load_s16(ramPnt, progPnt->Pnt_RegPos_Kp) * 0.1f; // Читаем момент
    if (sPnt->EngineParam.TorqueAutoSet) {  // Проверяем режим расчета момента
        GlobalM4.EnginParam.Torque_n = 9550 * GlobalM4.EnginParam.Pnom / GlobalM4.EnginParam.Vnom; // Mном = 9550 * Рном(кВт) / Vном (об/мин)
	    save_s16(ramPnt, progPnt->Pnt_RegPos_Kp, (u16)(GlobalM4.EnginParam.Torque_n * 10.0f));
    } else {
        GlobalM4.EnginParam.Torque_n = TorqueTemp;
    }
       
//******************************************************************************    
// Вывод текущих значений
//******************************************************************************    
    save_s16(ramPnt, progPnt->Pnt_EncoderAngle, (u16)(GlobalM4.EncoderCalcParam.electricTheta * 3600.0F + 0.5F)); //Позиция энкодера
    //AllPrmPnt->ENCODER_OUT_FREQ = GlobalM4.EncoderCalcParam.electricSpd;
       //= encoExtBlockDataPnt->electricSpd;//IQ_to_FLOAT(encoExtBlockDataPnt->electricSpd, Herz_base); // Переводим в Гц //encoExtBlockDataPnt->electricSpd * 80.0f;
        
    sPnt->MeFiltr = f32_Filtr(sPnt->MeFiltr, GlobalM4.Me, 0.025f);              // Подфильтруем момент
    if (GlobalM4.DirectRotation)
        Me_temp = (-sPnt->MeFiltr);
    else
        Me_temp = sPnt->MeFiltr;
    save_s16(ramPnt, progPnt->Pnt_Me, (s16)(Me_temp* 10.0f + 0.5f));                  // Момент текущий
    
    // Если работаем в ПЧ, то выдаем частоту поля
    /*
#ifdef _PROJECT_FOR_STM32_ 
    We_freq = FLOAT_to_IQ(GlobalM4.We, Herz_base);
#else // Иначе для симуляции просто транслируем задание генератора темпа.
    We_freq = load_float(ramPnt, progPnt->Pnt_F);
#endif    
    */
    We_freq = FLOAT_to_IQ(GlobalM4.We, Herz_base);
    if (GlobalM4.DirectRotation)
        We_freq *= (-1.0f);
    
    save_float(ramPnt, progPnt->Pnt_We, We_freq);               // Выходная частота ПЧ
    
    Wr_freq = FLOAT_to_IQ(GlobalM4.Wr, Herz_base);
    if (GlobalM4.DirectRotation)
        Wr_freq *=  (-1.0f);
    save_float(ramPnt, progPnt->Pnt_Wr, Wr_freq);               // Частота вращения ротора
    

    if( TYPEDRV_ANYSCALAR & (1<<typeDrv))                       // если скаляр, то пишем выходную частоту
        temp = We_freq;
    else
        temp = Wr_freq;                                         // если вектор, то частоту ротора
    
#ifdef _PROJECT_FOR_STM32_      
    save_float(ramPnt, progPnt->Pnt_FoutAbs, fabsf(temp));      // выдаем абсолютную частоту поля (для контроля аварии)
#endif
    temp = FLOAT_to_IQ(GlobalM4.Uout, Volt_base);
    save_float(ramPnt, progPnt->Pnt_Uout, temp);                // Выходное напряжение ПЧ
    
    temp = FLOAT_to_IQ((GlobalM4.Pfull * 10.0f), Power_base);
    save_float(ramPnt, progPnt->Pnt_Pfull_out, temp);    // Полная мощность
    
    temp = FLOAT_to_IQ((GlobalM4.Pact * 10.0f), Power_base);
    save_float(ramPnt, progPnt->Pnt_Pact_out, temp);     // Активная мощность
    
    temp = FLOAT_to_IQ((GlobalM4.Preact * 10.0f), Power_base);
    save_float(ramPnt, progPnt->Pnt_Preact_out, temp);   // Реактивная мощность
    
    temp = FLOAT_to_IQ(GlobalM4.Ifull, Current_base);
    save_float(ramPnt, progPnt->Pnt_Ifull_out, temp);    // Полный ток
    
    temp = FLOAT_to_IQ(GlobalM4.Iact, Current_base);
    save_float(ramPnt, progPnt->Pnt_Iact_out, temp);     // Активный ток
    
    temp = FLOAT_to_IQ(GlobalM4.Ireact, Current_base);
    save_float(ramPnt, progPnt->Pnt_Ireact_out, temp);   // Реактивный ток
        
    temp = FLOAT_to_IQ((GlobalM4.cosFi * 100.0f), Proc_base);    // *100.0f для перевода абсолютного значения в %
    save_float(ramPnt, progPnt->Pnt_cosFi_out, temp);    // Косинус фи
    
// Вывод в МК флагов управления
    defBitMicro(ramPnt, progPnt->Pnt_RegE_Ref, (u16)(GlobalM4.FlagCommand.bit.mWorkEnb));  // Флаг состояния измерения параметров двигателя
       
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDRV_V5_1[144]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Pusk
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Stop
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  StopVibeg
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  FailTek
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  SelTormType
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  VibegTime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  TypeDrv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Ftek
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  Fpusk
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  SoftCurDown
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  StateGT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  U
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Napravlenie
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  Wr/~Me_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Wr/~Me_2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Ud_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  K_Ud
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Iu_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  K_Iu
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Iv_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22  K_Iv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Iw_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //24  K_Iw
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  T_1_V
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  T_2_V
        S32_PIN_TYPE | INPUT_PIN_MODE,     //27  Tmax
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
        S16_PIN_TYPE | INPUT_PIN_MODE,     //39  K_Ileak
        S16_PIN_TYPE | INPUT_PIN_MODE,     //40  Ileak_Korect
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41  Ileak_MTZ		
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  FeCorrectMode		
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43  FeFixedPhase
        S16_PIN_TYPE | INPUT_PIN_MODE,     //44  FePidKp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //45  FePidKi
	S16_PIN_TYPE | INPUT_PIN_MODE,     //46  EncoShift
        S16_PIN_TYPE | INPUT_PIN_MODE,     //47  EncoderFilter
        S16_PIN_TYPE | INPUT_PIN_MODE,     //48  PsiRotor
        S16_PIN_TYPE | INPUT_PIN_MODE,     //49  Lsd
        S16_PIN_TYPE | INPUT_PIN_MODE,     //50  Lsq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //51  Reset_Default
        S16_PIN_TYPE | INPUT_PIN_MODE,     //52  Wr_z
        S16_PIN_TYPE | INPUT_PIN_MODE,     //53  Me_z
        S16_PIN_TYPE | INPUT_PIN_MODE,     //54  Wr_nom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //55  Wr_max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //56  Fs_nom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //57  Us_nom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //58  CalcPrmMot
        S16_PIN_TYPE | INPUT_PIN_MODE,     //59  Rr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //60  Rs
        S16_PIN_TYPE | INPUT_PIN_MODE,     //61  Lm
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //62  SigmaLs
        S16_PIN_TYPE | INPUT_PIN_MODE,     //63  Zp
        S32_PIN_TYPE | INPUT_PIN_MODE,     //64  Adapt_Tr_Ls
        S16_PIN_TYPE | INPUT_PIN_MODE,     //65  Calc_J
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //66  Jr
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
        S16_PIN_TYPE | INPUT_PIN_MODE,     //80  Pnt_BSFilter_Fr 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //81  Pnt_BSFilter_dF 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //82  Pnt_AutosetCurReg
        S16_PIN_TYPE | INPUT_PIN_MODE,     //83  Pnt_CurRegKp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //84  Pnt_CurRegKi
        S32_PIN_TYPE | INPUT_PIN_MODE,     //85  T_ad_Lm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //86  T0e
        S16_PIN_TYPE | INPUT_PIN_MODE,     //87  Kez
        S16_PIN_TYPE | INPUT_PIN_MODE,     //88  CalcRegE
        S16_PIN_TYPE | INPUT_PIN_MODE,     //89  Pnt_Ustir_Uin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //90  Uab_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //91  K_Uab
        S16_PIN_TYPE | INPUT_PIN_MODE,     //92  Ubc_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //93  K_Ubc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //94  Uca_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //95  K_Uca
        S16_PIN_TYPE | INPUT_PIN_MODE,     //96  Uin max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //97  Uin min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //98  Uin Mode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //99  DifFiltr
        S32_PIN_TYPE | INPUT_PIN_MODE,     //100  Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //101  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //102  TempMax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //103  ZerHzDc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //104  F_DCSTP
        S16_PIN_TYPE | INPUT_PIN_MODE,     //105  Time_STP
        S32_PIN_TYPE | INPUT_PIN_MODE,     //106  UinDC
        S32_PIN_TYPE | INPUT_PIN_MODE,     //107  CurTormDc
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //108  Dry_On
        S16_PIN_TYPE | INPUT_PIN_MODE,     //109  DryEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //110  SrcEnSel
        S32_PIN_TYPE | INPUT_PIN_MODE,     //111  Ud_tek_iq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //112  DF_REVERS
        S16_PIN_TYPE | INPUT_PIN_MODE,     //113  Iru
        S16_PIN_TYPE | INPUT_PIN_MODE,     //114  DinSrc		
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
	S16_PIN_TYPE | OUTPUT_PIN_MODE,    //137  StatusDRV		
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
OpisFuncBlock("DRV_V5", "U", TblFuncDRV_V5_1, FuncDRV_V5_1, sizeof(TblFuncDRV_V5_1), sizeof(DRVV5_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDRV_V5_1,
#endif
//********************************************************
