#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_A;                            //1  A
    u16  Pnt_B;                            //2  B
    u16  Pnt_nA_B;                         //3  A>=B
    u16  Pnt_A_B;                          //4  A>B
    u16  Pnt_End;
}FuncCMP_5_type;
//********************************************************
// 1 Перезагрузка   1 Перезагрузка  1 Перезагрузка    
//      s16              s16              s16
//********************************************************
u16 *FuncCMP_5_2(FuncCMP_5_type *progPnt, u32 ramPnt)
{
    float     A, B;
    
    A = load_float(ramPnt, progPnt->Pnt_A);
    B = load_float(ramPnt, progPnt->Pnt_B);

    if(A > B){
        setBitMicro(ramPnt, progPnt->Pnt_A_B);
        clrBitMicro(ramPnt, progPnt->Pnt_nA_B);
    }
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_A_B);
        setBitMicro(ramPnt, progPnt->Pnt_nA_B);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCMP_5_2[4]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //3  A>=B
        BIT_PIN_TYPE | OUTPUT_PIN_MODE    //4  A>B
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("CMP_5", "DA", TblFuncCMP_5_2, FuncCMP_5_2, sizeof(TblFuncCMP_5_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMP_5_2,
#endif
//********************************************************
