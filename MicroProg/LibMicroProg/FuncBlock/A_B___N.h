#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_A;                            //1  A
    u16  Pnt_B;                            //2  B
    u16  Pnt_N;                            //3  N
    u16  Pnt__A_B___N;                     //4  (A*B)>>N
    u16  Pnt_End;
}Func_A_B___N_type;
//********************************************************
u16 *Func_A_B___N_1(Func_A_B___N_type *progPnt, u32 ramPnt)
{
    s32     A, B, N;
    
    A = load_s16(ramPnt, progPnt->Pnt_A);
    B = load_s16(ramPnt, progPnt->Pnt_B);
    N = load_s16(ramPnt, progPnt->Pnt_N);
    A = A*B>>N;
    A = s16LimControl(A);
    save_s16(ramPnt, progPnt->Pnt__A_B___N, (s16)A);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFunc_A_B___N_1[4]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  N
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //4  (A*B)>>N
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("(A*B)>>N", "DA", TblFunc_A_B___N_1, Func_A_B___N_1, sizeof(TblFunc_A_B___N_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)Func_A_B___N_1,
#endif
//********************************************************
