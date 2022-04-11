#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Bit;                          //1  Bit
    u16  Pnt_End;
}FuncTYPE_BIT_type;
//********************************************************
u16 *FuncTYPE_BIT_1(FuncTYPE_BIT_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTYPE_BIT_1[1]={
        BIT_PIN_TYPE | INPUT_PIN_MODE      //1  Bit
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TYPE_BIT", "TYPE", TblFuncTYPE_BIT_1, FuncTYPE_BIT_1, sizeof(TblFuncTYPE_BIT_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTYPE_BIT_1,
#endif
//********************************************************
