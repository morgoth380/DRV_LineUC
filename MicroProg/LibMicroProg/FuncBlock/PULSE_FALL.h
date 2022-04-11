#ifdef    GET_FUNC_CODE
typedef struct{
        s16         PrevState;
}PULSE_FALLState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Out;                          //2  Out
    u16  Pnt_End;
}FuncPULSE_FALL_type;
//********************************************************
u16 *FuncPULSE_FALL_1(FuncPULSE_FALL_type *progPnt, u32 ramPnt)
{
    s16                     PrevState, TekState;
    PULSE_FALLState_type    *sPnt;
    
    sPnt = (PULSE_FALLState_type *)(ramPnt + progPnt->Pnt_State);
    PrevState = sPnt->PrevState;
    TekState = testBitMicro(ramPnt, progPnt->Pnt_In);
    defBitMicro(ramPnt, progPnt->Pnt_Out, (PrevState) && (!TekState));
    sPnt->PrevState = TekState;
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPULSE_FALL_1[2]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("PULSE_FALL", "DD", TblFuncPULSE_FALL_1, FuncPULSE_FALL_1, sizeof(TblFuncPULSE_FALL_1), sizeof(PULSE_FALLState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPULSE_FALL_1,
#endif
//********************************************************
