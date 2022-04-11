#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_S32;                          //1  S32
    u16  Pnt_End;
}FuncTYPE_S32_type;
//********************************************************
u16 *FuncTYPE_S32_1(FuncTYPE_S32_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTYPE_S32_1[1]={
        S32_PIN_TYPE | INPUT_PIN_MODE      //1  S32
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TYPE_S32", "TYPE", TblFuncTYPE_S32_1, FuncTYPE_S32_1, sizeof(TblFuncTYPE_S32_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTYPE_S32_1,
#endif
//********************************************************
