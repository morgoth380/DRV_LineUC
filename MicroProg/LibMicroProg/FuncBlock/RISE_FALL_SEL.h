#ifdef    GET_FUNC_CODE

typedef struct{
        s16         PrevState;
}RiseFallSelState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Sel;                          //2  Sel
    u16  Pnt_Out;                          //3  Out
    u16  Pnt_End;
}FuncRISE_FALL_SEL_type;
//********************************************************
u16 *FuncRISE_FALL_SEL_1(FuncRISE_FALL_SEL_type *progPnt, u32 ramPnt)
{
    s16                     PrevState, TekState, Sel;
    RiseFallSelState_type   *sPnt;
    
    sPnt = (RiseFallSelState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    PrevState = sPnt->PrevState;
    TekState = testBitMicro(ramPnt, progPnt->Pnt_In);
    Sel = load_s16(ramPnt, progPnt->Pnt_Sel);
    
    if(Sel==0)  defBitMicro(ramPnt, progPnt->Pnt_Out, (!PrevState) && TekState);
    else        defBitMicro(ramPnt, progPnt->Pnt_Out, (PrevState) && (!TekState));
    
    sPnt->PrevState = TekState;
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRISE_FALL_SEL_1[3]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Sel
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("RISE_FALL_SEL", "DD", TblFuncRISE_FALL_SEL_1, FuncRISE_FALL_SEL_1, sizeof(TblFuncRISE_FALL_SEL_1), sizeof(RiseFallSelState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRISE_FALL_SEL_1,
#endif
//********************************************************
