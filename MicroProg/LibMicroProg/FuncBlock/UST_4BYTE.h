#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_End;
}FuncUST_4BYTE_type;
//********************************************************
u16 *FuncUST_4BYTE_1(FuncUST_4BYTE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_4BYTE_1[1]={
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_4BYTE", "UST2_", TblFuncUST_4BYTE_1, FuncUST_4BYTE_1, sizeof(TblFuncUST_4BYTE_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_4BYTE_1,
#endif
//********************************************************


//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncUST_4BYTE_2(FuncUST_4BYTE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_4BYTE_2[1]={
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE     //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_4BYTE", "UST2_", TblFuncUST_4BYTE_2, FuncUST_4BYTE_2, sizeof(TblFuncUST_4BYTE_2), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_4BYTE_2,
#endif
//********************************************************
