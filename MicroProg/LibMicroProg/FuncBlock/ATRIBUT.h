#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_in;                           //1  in
    u16  Pnt_End;
}FuncATRIBUT_type;
//********************************************************
u16 *FuncATRIBUT_1(FuncATRIBUT_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncATRIBUT_1[1]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE   //1  in
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("ATRIBUT", "DD", TblFuncATRIBUT_1, FuncATRIBUT_1, sizeof(TblFuncATRIBUT_1), 0, 0, 0),// sizeof(FuncATRIBUT_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncATRIBUT_1,
#endif
//********************************************************
