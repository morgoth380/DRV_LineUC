#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Value;
    u16  PrevC;
}CNTState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_C;                            //1  C
    u16  Pnt_Reset;                        //2  Reset
    u16  Pnt_Out;                          //3  Out
    u16  Pnt_End;
}FuncCNT_type;
//********************************************************
u16 *FuncCNT_1(FuncCNT_type *progPnt, u32 ramPnt)
{
    s16             C;
    CNTState_type   *sPnt;
    
    sPnt = (CNTState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    C = testBitMicro(ramPnt, progPnt->Pnt_C);
    if((!sPnt->PrevC)&&C){        //Rise
        sPnt->Value++;
    }
    sPnt->PrevC = C;
    if(testBitMicro(ramPnt, progPnt->Pnt_Reset))    sPnt->Value = 0;
    save_s16(ramPnt, progPnt->Pnt_Out, sPnt->Value);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCNT_1[3]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  C
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Reset
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CNT", "DD", TblFuncCNT_1, FuncCNT_1, sizeof(TblFuncCNT_1), sizeof(CNTState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCNT_1,
#endif
//********************************************************
