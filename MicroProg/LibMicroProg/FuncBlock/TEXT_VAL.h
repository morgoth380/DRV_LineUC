#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_End;
}FuncTEXT_VAL_type;
//********************************************************
u16 *FuncTEXT_VAL_1(FuncTEXT_VAL_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEXT_VAL_1[1]={
        TXT_PIN_TYPE | PASSIVE_PIN_MODE    //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEXT_VAL", "TEXT", TblFuncTEXT_VAL_1, FuncTEXT_VAL_1, sizeof(TblFuncTEXT_VAL_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEXT_VAL_1,
#endif
//********************************************************
