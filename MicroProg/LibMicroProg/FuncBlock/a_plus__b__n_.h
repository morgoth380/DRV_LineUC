#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_A;                            //1  A
    u16  Pnt_B;                            //2  B
    u16  Pnt_N;                            //3  N
    u16  Pnt_A__B__N_;                     //4  A+(B>>N)
    u16  Pnt_End;
}FuncA_PLUS__B__N__type;
//********************************************************
u16 *FuncA_PLUS__B__N__1(FuncA_PLUS__B__N__type *progPnt, u32 ramPnt)
{
    u16     A, B, N;
    
    A = load_s16(ramPnt, progPnt->Pnt_A);
    B = testBitMicro(ramPnt, progPnt->Pnt_B);
    N = load_s16(ramPnt, progPnt->Pnt_N);

    A = A +(B>>N);
    save_s16(ramPnt, progPnt->Pnt_A__B__N_, A);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncA_PLUS__B__N__1[4]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  N
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //4  A+(B>>N)
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("A_PLUS_(B>>N)", "DA", TblFuncA_PLUS__B__N__1, FuncA_PLUS__B__N__1, sizeof(TblFuncA_PLUS__B__N__1), 0, 0, 1), //, sizeof(FuncA_PLUS__B__N__type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncA_PLUS__B__N__1,
#endif
//********************************************************
