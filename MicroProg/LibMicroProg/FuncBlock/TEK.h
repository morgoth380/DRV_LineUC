#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_End;
}FuncTEK_type;
//********************************************************
u16 *FuncTEK_1(FuncTEK_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEK_1[1]={
        S16_PIN_TYPE | INPUT_PIN_MODE      //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEK", "TEK", TblFuncTEK_1, FuncTEK_1, sizeof(TblFuncTEK_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEK_1,
#endif
//********************************************************
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//********************************************************
//********************************************************

#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncTEK_2(FuncTEK_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEK_2[1]={
        S32_PIN_TYPE | INPUT_PIN_MODE      //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEK", "TEK", TblFuncTEK_2, FuncTEK_2, sizeof(TblFuncTEK_2), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEK_2,
#endif
//********************************************************
