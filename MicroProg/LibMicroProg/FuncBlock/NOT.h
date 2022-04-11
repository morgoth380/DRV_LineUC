#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_Arg_2;                        //2  Arg_2
    u16  Pnt_End;
}FuncNOT_type;
//********************************************************
u16 *FuncNOT_1(FuncNOT_type *progPnt, u32 ramPnt)
{
    if(testBitMicro(ramPnt, progPnt->Pnt_Arg_1))    clrBitMicro(ramPnt, progPnt->Pnt_Arg_2);
    else                                            setBitMicro(ramPnt, progPnt->Pnt_Arg_2);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncNOT_1[2]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //2  Arg_2
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("NOT", "DD", TblFuncNOT_1, FuncNOT_1, sizeof(TblFuncNOT_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncNOT_1,
#endif
//********************************************************
