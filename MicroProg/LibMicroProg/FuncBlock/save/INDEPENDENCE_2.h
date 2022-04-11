#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_End;
}FuncINDEPENDENCE_2_type;
//********************************************************
u16 *FuncINDEPENDENCE_2_1(FuncINDEPENDENCE_2_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncINDEPENDENCE_2_1[1]={
        S16_PIN_TYPE | INPUT_PIN_MODE |INDEPEND_2_FLAG     //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("INDEPENDENCE_2", "DA", TblFuncINDEPENDENCE_2_1, FuncINDEPENDENCE_2_1, sizeof(TblFuncINDEPENDENCE_2_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncINDEPENDENCE_2_1,
#endif
//********************************************************
