#ifdef    GET_FUNC_CODE

typedef struct{
    _iq     temp;
}FuncTEMP_TIME_ONEState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_On;                           //1  On
    u16  Pnt_Time;                         //2  Time
    u16  Pnt_Ok;                           //3  Ok
    u16  Pnt_Out;                          //4  Out
    u16  Pnt_End;
}FuncTEMP_TIME_ONE_type;
//********************************************************
u16 *FuncTEMP_TIME_ONE_1(FuncTEMP_TIME_ONE_type *progPnt, u32 ramPnt)
{
    _iq                             iq_out;
    s16                             Ok, tim;
    FuncTEMP_TIME_ONEState_type     *sPnt;
    
    Ok = 1;
    if(!testBitMicro(ramPnt, progPnt->Pnt_On)){
        iq_out = 0;
        Ok = 0;
    }
    else{
        sPnt = (FuncTEMP_TIME_ONEState_type *)(ramPnt + progPnt->Pnt_State);
        iq_out = load_s32(ramPnt, progPnt->Pnt_Out);
        if(iq_out == 0){
            tim = load_s16(ramPnt, progPnt->Pnt_Time);
            sPnt->temp = (tim == 0)     ? _IQ(1.0): (_IQ(INT_TIME_CALL_MICRO_mS)/ tim);
        }
        iq_out += sPnt->temp;
        if(iq_out >= _IQ(1.0)){
            iq_out = _IQ(1.0);
            Ok = 0;
        }
    }
    
    defBitMicro(ramPnt, progPnt->Pnt_Ok, Ok);
    save_s32(ramPnt, progPnt->Pnt_Out, iq_out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_TIME_ONE_1[4]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  On
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Time
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Ok
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //4  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEMP_TIME_ONE", "DA", TblFuncTEMP_TIME_ONE_1, FuncTEMP_TIME_ONE_1, sizeof(TblFuncTEMP_TIME_ONE_1), sizeof(FuncTEMP_TIME_ONEState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_TIME_ONE_1,
#endif
//********************************************************
