#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_End;
}FuncCONST_type;
//********************************************************
u16 *FuncCONST_1(FuncCONST_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCONST_1[1]={
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG   //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONST", "DA", TblFuncCONST_1, FuncCONST_1, sizeof(TblFuncCONST_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONST_1,
#endif
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncCONST_2(FuncCONST_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCONST_2[1]={
        S32_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG   //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONST", "DA", TblFuncCONST_2, FuncCONST_2, sizeof(TblFuncCONST_2), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONST_2,
#endif
//********************************************************
//********************************************************
//3 Перезагрузка    3 Перезагрузка  3 Перезагрузка    
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncCONST_3(FuncCONST_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCONST_3[1]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG   //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONST", "DA", TblFuncCONST_3, FuncCONST_3, sizeof(TblFuncCONST_3), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONST_3,
#endif
//********************************************************
//********************************************************
//4 Перезагрузка    4 Перезагрузка  4 Перезагрузка    
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncCONST_4(FuncCONST_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCONST_4[1]={
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG   //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONST", "DA", TblFuncCONST_4, FuncCONST_4, sizeof(TblFuncCONST_4), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONST_4,
#endif
//********************************************************
//********************************************************
