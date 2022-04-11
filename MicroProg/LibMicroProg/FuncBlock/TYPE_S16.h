#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_S16;                          //1  S16
    u16  Pnt_End;
}FuncTYPE_S16_type;
//********************************************************
u16 *FuncTYPE_S16_1(FuncTYPE_S16_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTYPE_S16_1[1]={
        S16_PIN_TYPE | INPUT_PIN_MODE      //1  S16
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TYPE_S16", "TYPE", TblFuncTYPE_S16_1, FuncTYPE_S16_1, sizeof(TblFuncTYPE_S16_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTYPE_S16_1,
#endif
//********************************************************
