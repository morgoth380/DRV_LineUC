#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_End;
}FuncEnd_type;
//********************************************************
u16 *FuncEnd_1(FuncEnd_type *progPnt, u32 ramPnt)
{
    return NULL;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncEnd_1[1]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("End",     "End",     TblFuncEnd_1,   NULL,   sizeof(TblFuncEnd_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    NULL,
#endif
//********************************************************
