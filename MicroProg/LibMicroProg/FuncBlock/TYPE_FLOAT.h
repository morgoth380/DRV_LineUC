#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Float;                        //1  Float
    u16  Pnt_End;
}FuncTYPE_FLOAT_type;
//********************************************************
u16 *FuncTYPE_FLOAT_1(FuncTYPE_FLOAT_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTYPE_FLOAT_1[1]={
        FLOAT_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG     //1  Float
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TYPE_FLOAT", "TYPE", TblFuncTYPE_FLOAT_1, FuncTYPE_FLOAT_1, sizeof(TblFuncTYPE_FLOAT_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTYPE_FLOAT_1,
#endif
//********************************************************
