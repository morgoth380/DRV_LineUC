#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_A;                            //1  A
    u16  Pnt_B;                            //2  B
    u16  Pnt_A_B;                          //3  A>B
    u16  Pnt_End;
}FuncCMP_3_type;
//********************************************************
// 1 Перезагрузка   1 Перезагрузка  1 Перезагрузка    
//      s16              s16              s16
//********************************************************
u16 *FuncCMP_3_1(FuncCMP_3_type *progPnt, u32 ramPnt)
{
    s16     A, B;
    
    A = load_s16(ramPnt, progPnt->Pnt_A);
    B = load_s16(ramPnt, progPnt->Pnt_B);
    if(A > B)   setBitMicro(ramPnt, progPnt->Pnt_A_B);
    else        clrBitMicro(ramPnt, progPnt->Pnt_A_B);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCMP_3_1[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  B
         BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  A>B
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("CMP_3", "DA", TblFuncCMP_3_1, FuncCMP_3_1, sizeof(TblFuncCMP_3_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMP_3_1,
#endif
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//      s32              s32              s32
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncCMP_3_2(FuncCMP_3_type *progPnt, u32 ramPnt)
{
    s32     A, B;
    
    A = load_s32(ramPnt, progPnt->Pnt_A);
    B = load_s32(ramPnt, progPnt->Pnt_B);
    if(A > B)   setBitMicro(ramPnt, progPnt->Pnt_A_B);
    else        clrBitMicro(ramPnt, progPnt->Pnt_A_B);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCMP_3_2[3]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  B
         BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  A>B
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("CMP_3", "DA", TblFuncCMP_3_2, FuncCMP_3_2, sizeof(TblFuncCMP_3_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMP_3_2,
#endif
//********************************************************
