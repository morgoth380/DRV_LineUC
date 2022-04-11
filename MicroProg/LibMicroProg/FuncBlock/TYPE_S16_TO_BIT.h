#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_S16;                          //1  S16
    u16  Pnt_BIT;                          //2  BIT
    u16  Pnt_End;
}FuncTYPE_S16_TO_BIT_type;
//********************************************************
u16 *FuncTYPE_S16_TO_BIT_1(FuncTYPE_S16_TO_BIT_type *progPnt, u32 ramPnt)
{
    s16     tmp;
    
    tmp = load_s16(ramPnt, progPnt->Pnt_S16);
    defBitMicro(ramPnt, progPnt->Pnt_BIT, tmp);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTYPE_S16_TO_BIT_1[2]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  S16
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //2  BIT
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TYPE_S16_TO_BIT", "TYPE", TblFuncTYPE_S16_TO_BIT_1, FuncTYPE_S16_TO_BIT_1, sizeof(TblFuncTYPE_S16_TO_BIT_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTYPE_S16_TO_BIT_1,
#endif
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncTYPE_S16_TO_BIT_2(FuncTYPE_S16_TO_BIT_type *progPnt, u32 ramPnt)
{
    s16     tmp;

    tmp = load_s32(ramPnt, progPnt->Pnt_S16) != 0 ? 1 : 0;

    defBitMicro(ramPnt, progPnt->Pnt_BIT, tmp);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTYPE_S16_TO_BIT_2[2]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  S16
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //2  BIT
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TYPE_S16_TO_BIT", "TYPE", TblFuncTYPE_S16_TO_BIT_2, FuncTYPE_S16_TO_BIT_2, sizeof(TblFuncTYPE_S16_TO_BIT_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTYPE_S16_TO_BIT_2,
#endif
//********************************************************
