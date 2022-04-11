#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_End;
}FuncFORMULA_STRING_type;
//********************************************************
u16 *FuncFORMULA_STRING_1(FuncFORMULA_STRING_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFORMULA_STRING_1[1]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FORMULA_STRING", "FRM", TblFuncFORMULA_STRING_1, FuncFORMULA_STRING_1, sizeof(TblFuncFORMULA_STRING_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFORMULA_STRING_1,
#endif
//********************************************************
