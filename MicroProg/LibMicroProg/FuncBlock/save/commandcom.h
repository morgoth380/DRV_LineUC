#ifdef    GET_FUNC_CODE

typedef struct{
    u16     Ramp_down_time;
    u16     Start;
    u16     WorkFlag;
    u16     cntDelay;
    s16     cntSFTCRDN;
    s16     cntGotDel;
    u16     cntDrebezg;
    u16     cntStartDelay;
    u16     timeERR_ZAR;
	u16     modeOn :1;
    u16     failEn :1;
    u16     CHARGE_OK:1;
    u16     ERR_ZAR_T:1;
    u16     PULSE_OK:1;
    u16     CHARGE:1;
    u16     StartDRV:1;
    u16     StartDRV_OK:1;
    u16     CHARGEprev:1;
    u16     NPWM:1;
}COMMANDCOM_State_type;

#define TYPE_WR 0
#define TYPE_ME 1

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_INST[4];                         //1  NETW
//    u16  Pnt_DIN;                          //2  DIN
//    u16  Pnt_APV;                          //3  APV
//    u16  Pnt_DRY;                          //4  DRY
    u16  Pnt_In[5];                       //5  NETW
//    u16  Pnt_DIN_1;                        //6  DIN
//    u16  Pnt_SLEEP;                        //7  SLEEP
//    u16  Pnt_STBST;                        //8  STBST
//    u16  Pnt_DINVIBEG                    //9  DINVIBEG
    u16  Pnt_EN_FFS;                       //10  EN_FFS
    u16  Pnt_STP_DC;                       //11  STP_DC
    u16  Pnt_EXT_BR;                       //12  EXT_BR
    u16  Pnt_S_KEY;                        //13  S_KEY
    u16  Pnt_PWM_F;                        //14  PWM_F
    u16  Pnt_S_W_PU;                       //15  S_W_PU
    u16  Pnt_FNETW;                        //16  FNETW
    u16  Pnt_S_E_BR;                       //17  S_E_BR
    u16  Pnt_DC_OFF;                       //18  DC_OFF
    u16  Pnt_RS_OFF;                       //19  RS_OFF
    u16  Pnt_F_REF;                        //20  F_REF
    u16  Pnt_F_PUSK;                       //21  F_PUSK
    u16  Pnt_RD_TM;                        //22  RD_TM
    u16  Pnt_USTME;                        //23  USTME
    u16  Pnt_USTTYPE;                      //24  USTTYPE
    u16  Pnt_SEL;                          //25  SEL
    u16  Pnt_FAILTEK;                      //26  FAILTEK
    u16  Pnt_B_APV;                        //27  B_APV BLINK_APV
    u16  Pnt_Control;                      //28  Control
    u16  Pnt_DelayMotorOn;                 //29  DelayMotorOn
    u16  Pnt_CntDrebezg;                   //30  CntDrebezg
    u16  Pnt_LogicContr;                   //31  LogicContr
    u16  Pnt_LogicOn;                      //32  LogicOn
    u16  Pnt_Mode;                         //33  Mode
    u16  Pnt_UD_TEK;                       //34  UD_TEK
    u16  Pnt_UD_UST;                       //35  UD_UST
    u16  Pnt_OFF_ZAR;                      //36  OFF_ZAR
    u16  Pnt_T_ZAR;                        //37  T_ZAR
		u16  Pnt_Fire_mode_ON;                 //38 Fire_Mode_ON
		u16  Pnt_SOFT_C;                       //39 Soft_C
    u16  Pnt_ERR_ZAR;                      //40  ERR_ZAR
		u16  Pnt_START;                        //41  START
    u16  Pnt_CHARGE_OK;                    //42  CHARGE_OK
    u16  Pnt_CHARGE;                       //43  CHARGE
    u16  Pnt_ErrControl;                   //44  ErrControl
    u16  Pnt_ContactorOn;                  //45  ContactorOn
    u16  Pnt_STRTPLS;                      //46  STRTPLS
    u16  Pnt_NWKFLG;                       //47  NWKFLG
    u16  Pnt_WKFLG;                        //48  WKFLG
    u16  Pnt_ST_IMP;                       //49  ST_IMP
    u16  Pnt_COMBI;                        //50  COMBI
    u16  Pnt_FREQ;                         //51  FREQ
    u16  Pnt_RMPDN;                        //52  RMPDN
    u16  Pnt_PWM_D;                        //53  PWM_D
    u16  Pnt_NPWM;                         //54  NPWM
    u16  Pnt_PWM;                          //55  PWM
    u16  Pnt_GOTOV;                        //56  GOTOV


    u16  Pnt_End;
}FuncCOMMANDCOM_type;
//********************************************************
u16 *FuncCOMMANDCOM_1(FuncCOMMANDCOM_type *progPnt, u32 ramPnt)
{

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCOMMANDCOM_1[56]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  NETW
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  DIN
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  APV
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  DRY
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  NETW
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  DIN
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  SLEEP
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  STBST
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  DINVIBEG 
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  EN_FFS
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  STP_DC
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  EXT_BR
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  S_KEY
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  PWM_F
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  S_W_PU
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //16  FNETW
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //17  S_E_BR
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //18  DC_OFF
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //19  RS_OFF
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  F_REF
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  F_PUSK
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  RD_TM
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  USTME
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  USTTYPE
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  SEL
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //26  FAILTEK
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //27  B_APV BLINK_APV
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //28  Control
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  DelayMotorOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  CntDrebezg
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  LogicContr
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //32  LogicOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  Mode
        S32_PIN_TYPE | INPUT_PIN_MODE,     //34  UD_TEK
        S32_PIN_TYPE | INPUT_PIN_MODE,     //35  UD_UST
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //36  OFF_ZAR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  T_ZAR
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //38 Fire_mode_ON
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //39  SOFT_C
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //40  ERR_ZAR
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //41  START
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //42  CHARGE_OK
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //43  CHARGE
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //44  ErrControl
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //45  ContactorOn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //46  STRTPLS
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //47  NWKFLG
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //48  WKFLG
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //49  ST_IMP
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //50  COMBI
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //51  FREQ
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //52  RMPDN
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //53  PWM_D
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //54  NPWM
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //55  PWM
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //56  GOTOV
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("COMMANDCOM", "DA", TblFuncCOMMANDCOM_1, FuncCOMMANDCOM_1, sizeof(TblFuncCOMMANDCOM_1), sizeof(COMMANDCOM_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCOMMANDCOM_1,
#endif
//********************************************************
