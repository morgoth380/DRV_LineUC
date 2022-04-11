#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_BIT;                          //1  BIT
    u16  Pnt_S16;                          //2  S16
    u16  Pnt_End;
}FuncTYPE_BIT_TO_S16_type;
//********************************************************
u16 *FuncTYPE_BIT_TO_S16_1(FuncTYPE_BIT_TO_S16_type *progPnt, u32 ramPnt)
{
    s16     tmp;
    
    tmp = testBitMicro(ramPnt, progPnt->Pnt_BIT);
    save_s16(ramPnt, progPnt->Pnt_S16, tmp);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTYPE_BIT_TO_S16_1[2]={
        BIT_PIN_TYPE | INPUT_PIN_MODE ,   //1  BIT
        S16_PIN_TYPE | OUTPUT_PIN_MODE    //2  S16
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TYPE_BIT_TO_S16", "TYPE", TblFuncTYPE_BIT_TO_S16_1, FuncTYPE_BIT_TO_S16_1, sizeof(TblFuncTYPE_BIT_TO_S16_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTYPE_BIT_TO_S16_1,
#endif
//********************************************************
