#ifdef    GET_FUNC_CODE
typedef struct{
        s16         PrevState;
}RiseFallOutState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Fall;                         //2  Fall
    u16  Pnt_Rise;                         //3  Rise
    u16  Pnt_End;
}FuncRISE_FALL_OUT_type;
//********************************************************
u16 *FuncRISE_FALL_OUT_1(FuncRISE_FALL_OUT_type *progPnt, u32 ramPnt)
{
    s16                     PrevState, TekState;
    RiseFallOutState_type   *sPnt;
    
    sPnt = (RiseFallOutState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    PrevState = sPnt->PrevState;
    TekState = testBitMicro(ramPnt, progPnt->Pnt_In);
    defBitMicro(ramPnt, progPnt->Pnt_Rise, (!PrevState) && TekState);
    defBitMicro(ramPnt, progPnt->Pnt_Fall, (PrevState) && (!TekState));
    sPnt->PrevState = TekState;
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRISE_FALL_OUT_1[3]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Fall
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  Rise
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("RISE_FALL_OUT", "DD", TblFuncRISE_FALL_OUT_1, FuncRISE_FALL_OUT_1, sizeof(TblFuncRISE_FALL_OUT_1), sizeof(RiseFallOutState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRISE_FALL_OUT_1,
#endif
//********************************************************
