#ifdef    GET_FUNC_CODE
//-----------------------------------------------------------
typedef struct{
    s32        I_old;
    s32        I_contr;
}PROTECT_State_type;
//------------------------------------------------------------
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_I_OUT;                        //1  I_OUT
    u16  Pnt_F_OUT;                        //2  F_OUT
    u16  Pnt_I_UST;                        //3  I_UST
    u16  Pnt_EN_FSTP;                      //4  EN_FSTP
    u16  Pnt_EN_FSTRT;                     //5  EN_FSTRT
    u16  Pnt_PWM;                          //6  EN_FSTRT
    u16  Pnt_FAIL_STRT;                    //7  FAIL_STRT
    u16  Pnt_FAIL_STP;                     //8  FAIL_STP
    u16  Pnt_End;
}FuncPROTECT_type;
//********************************************************
u16 *FuncPROTECT_1(FuncPROTECT_type *progPnt, u32 ramPnt)
{
    PROTECT_State_type                *sPnt;
    _iq                               Itek, I_contr;

    sPnt = (PROTECT_State_type *)(ramPnt + progPnt->Pnt_State);

    if (flagIsFirstCall){
        sPnt->I_old = 0;
    }
    if (!testBitMicro(ramPnt, progPnt->Pnt_PWM) && (load_s16(ramPnt, progPnt->Pnt_EN_FSTP) == 1)){
        Itek = _IQabs(load_s32(ramPnt, progPnt->Pnt_I_OUT));
//        I_contr = _IQabs(_IQmpy(load_s32(ramPnt, progPnt->Pnt_I_UST), sPnt->I_old));
        I_contr = _IQabs(load_s32(ramPnt, progPnt->Pnt_I_UST));
//        if ((Itek > (sPnt->I_old + I_contr)) && (sPnt->I_old != 0)){
//            setBitMicro(ramPnt, progPnt->Pnt_FAIL_STP);
//        }else{
//            I_contr = _IQabs(_IQmpy(load_s32(ramPnt, progPnt->Pnt_I_UST), Itek));
//            if ((Itek < (sPnt->I_old - I_contr)) && (sPnt->I_old != 0)){
//                setBitMicro(ramPnt, progPnt->Pnt_FAIL_STP);
//            }else{
//                clrBitMicro(ramPnt, progPnt->Pnt_FAIL_STP);
//            }
//        }
//        sPnt->I_old = Itek;
        if (Itek > I_contr){
            setBitMicro(ramPnt, progPnt->Pnt_FAIL_STP);
        }else{
            clrBitMicro(ramPnt, progPnt->Pnt_FAIL_STP);
        }
    }else{
        clrBitMicro(ramPnt, progPnt->Pnt_FAIL_STP);
    }

    if (load_s16(ramPnt, progPnt->Pnt_EN_FSTRT) == 1){
        if (testBitMicro(ramPnt, progPnt->Pnt_PWM))
            if (_IQabs(load_s32(ramPnt, progPnt->Pnt_F_OUT)) > _IQ(0.025)){
                setBitMicro(ramPnt, progPnt->Pnt_FAIL_STRT);
            }
    }else{
        clrBitMicro(ramPnt, progPnt->Pnt_FAIL_STRT);
    }
//    logerForOscilloscope(measMem.Iact_out, measMem.Ireact_out, 0, 0, 2);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPROTECT_1[8]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  I_OUT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  F_OUT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  I_UST
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  EN_FSTP
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  EN_FSTRT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  PWM
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //7  FAIL_STRT
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //8  FAIL_STP
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PROTECT", "DA", TblFuncPROTECT_1, FuncPROTECT_1, sizeof(TblFuncPROTECT_1), sizeof(PROTECT_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPROTECT_1,
#endif
//********************************************************
