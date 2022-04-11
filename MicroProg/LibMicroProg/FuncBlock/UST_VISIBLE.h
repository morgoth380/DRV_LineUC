#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_Arg_2;                        //2  Arg_2
    u16  Pnt_End;
}FuncUST_VISIBLE_type;
//********************************************************
u16 *FuncUST_VISIBLE_1(FuncUST_FORMAT_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_VISIBLE_1[2]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,   //1  Arg_1
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG   //2  Arg_2
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_VISIBLE", "UST", TblFuncUST_VISIBLE_1, FuncUST_VISIBLE_1, sizeof(TblFuncUST_VISIBLE_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_VISIBLE_1,
#endif
//********************************************************
//********************************************************
//********************************************************
//2 Перезагрузка	2 Перезагрузка	2 Перезагрузка	
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncUST_VISIBLE_2(FuncUST_VISIBLE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_VISIBLE_2[2]={
        S32_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,   //1  Arg_1
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG   //2  Arg_2
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_VISIBLE", "UST", TblFuncUST_VISIBLE_2, FuncUST_VISIBLE_2, sizeof(TblFuncUST_VISIBLE_2), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_VISIBLE_2,
#endif
//********************************************************
