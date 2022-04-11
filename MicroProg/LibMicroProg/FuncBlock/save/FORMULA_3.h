#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Out;                         //1  Out
    u16  Pnt_formula;                     //2  formula
    u16  Pnt_Arg[3];                      //3
    u16  Pnt_End;
}FuncFORMULA_3_type;
//********************************************************
u16 *FuncFORMULA_3_1(FuncFORMULA_3_type *progPnt, u32 ramPnt)
{
    FuncFORMULA_X_s16((FuncFORMULA_X_type *)progPnt, ramPnt, 3);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFORMULA_3_1[5]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //1  Out
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  formula
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  A
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  B
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  C
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("FORMULA_3", "DA", TblFuncFORMULA_3_1, FuncFORMULA_3_1, sizeof(TblFuncFORMULA_3_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFORMULA_3_1,
#endif
//********************************************************
