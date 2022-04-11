#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Out;                         //1  Out
    u16  Pnt_formula;                     //2  formula
    u16  Pnt_Arg[2];                      //3
    u16  Pnt_End;
}FuncFORMULA_2_type;
//********************************************************
u16 *FuncFORMULA_2_1(FuncFORMULA_2_type *progPnt, u32 ramPnt)
{
    FuncFORMULA_X_s16((FuncFORMULA_X_type *)progPnt, ramPnt, 2);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFORMULA_2_1[4]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //1  Out
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  formula
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  A
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  B
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("FORMULA_2", "DA", TblFuncFORMULA_2_1, FuncFORMULA_2_1, sizeof(TblFuncFORMULA_2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFORMULA_2_1,
#endif
//********************************************************
