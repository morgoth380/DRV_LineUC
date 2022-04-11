#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_End;
}FuncTEK_4BYTE_type;
//********************************************************
u16 *FuncTEK_4BYTE_1(FuncTEK_4BYTE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEK_4BYTE_1[1]={
        S32_PIN_TYPE | INPUT_PIN_MODE      //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEK_4BYTE", "TEK2_", TblFuncTEK_4BYTE_1, FuncTEK_4BYTE_1, sizeof(TblFuncTEK_4BYTE_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEK_4BYTE_1,
#endif
//********************************************************