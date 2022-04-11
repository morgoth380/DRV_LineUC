#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Base0;                        //1  Base0
    u16  Pnt_End;
}FuncNAMEFUNC16_type;
//********************************************************
u16 *FuncNAMEFUNC16_1(FuncNAMEFUNC16_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncNAMEFUNC16_1[1]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE  | INDEPEND_FLAG     //1  Base0
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("NAMEFUNC16", "DA", TblFuncNAMEFUNC16_1, FuncNAMEFUNC16_1, sizeof(TblFuncNAMEFUNC16_1), 0, 0, 1), //, sizeof(FuncNAMEFUNC16_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncNAMEFUNC16_1,
#endif
//********************************************************
