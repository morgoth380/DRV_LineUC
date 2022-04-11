#ifdef    GET_FUNC_CODE

typedef struct{
    s16     SoftCurDwnDelay;
    u16     Fr_stop_flag:1;
    u16     IN_flag:1;
    u16     FailFlag:1;
}SOFT_CUR_DOWN_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_IN;                            //1  IN
    u16  Pnt_Fr_stop;                       //2  Fr_stop
    u16  Pnt_EpicFail;                      //3  EpicFail
    u16  Pnt_SoftCDflag;                    //4  SoftCDflag
    u16  Pnt_OUT;                           //5  Out
    u16  Pnt_End;
}FuncSOFT_CUR_DOWN_type;
//********************************************************
u16 *FuncSOFT_CUR_DOWN_1(FuncSOFT_CUR_DOWN_type *progPnt, u32 ramPnt)
{
    SOFT_CUR_DOWN_State_type   *sPnt;
    sPnt = (SOFT_CUR_DOWN_State_type *)(ramPnt + progPnt->Pnt_State);

    if (testBitMicro(ramPnt, progPnt->Pnt_IN) == 1) sPnt->FailFlag = 0;

    if((testBitMicro(ramPnt, progPnt->Pnt_EpicFail)==0)&&(sPnt->FailFlag == 0)){
        if ((testBitMicro(ramPnt, progPnt->Pnt_Fr_stop))&&(testBitMicro(ramPnt, progPnt->Pnt_IN)))  sPnt->Fr_stop_flag  = 1;
        if (sPnt->Fr_stop_flag == 1){
            if (testBitMicro(ramPnt, progPnt->Pnt_IN)==0) sPnt->IN_flag = 1;
        }
        if ((sPnt->Fr_stop_flag) && (sPnt->IN_flag==1)){
            if (sPnt->SoftCurDwnDelay >0){
                setBitMicro(ramPnt, progPnt->Pnt_SoftCDflag);
                setBitMicro(ramPnt, progPnt->Pnt_OUT);
                sPnt->SoftCurDwnDelay--;
            }
            else{
                clrBitMicro(ramPnt, progPnt->Pnt_SoftCDflag);
                clrBitMicro(ramPnt, progPnt->Pnt_OUT);
                sPnt->Fr_stop_flag = 0;
                sPnt->IN_flag = 0;
            }
        }
        else{
            sPnt->SoftCurDwnDelay = K_TIME_CALL_MICRO_TAKT;
            clrBitMicro(ramPnt, progPnt->Pnt_SoftCDflag);
            defBitMicro(ramPnt, progPnt->Pnt_OUT, testBitMicro(ramPnt, progPnt->Pnt_IN));
        }
    }
    else{
        sPnt->FailFlag = 1;
        sPnt->Fr_stop_flag = 0;
        sPnt->IN_flag = 0;
        sPnt->SoftCurDwnDelay = K_TIME_CALL_MICRO_TAKT;
        clrBitMicro(ramPnt, progPnt->Pnt_SoftCDflag);
//      defBitMicro(ramPnt, progPnt->Pnt_OUT, testBitMicro(ramPnt, progPnt->Pnt_IN));
        clrBitMicro(ramPnt, progPnt->Pnt_OUT);
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSOFT_CUR_DOWN_1[5]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  IN
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Fr_stop
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  EpicFail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  SoftCDflag
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //5  OUT
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SOFT_CUR_DOWN", "DA", TblFuncSOFT_CUR_DOWN_1, FuncSOFT_CUR_DOWN_1, sizeof(TblFuncSOFT_CUR_DOWN_1), sizeof(SOFT_CUR_DOWN_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSOFT_CUR_DOWN_1,
#endif
//********************************************************
