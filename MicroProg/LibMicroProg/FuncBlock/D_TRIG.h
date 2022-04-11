#ifdef    GET_FUNC_CODE

typedef struct{
        s16         OutState;
        s16         ClockState;
}DtrigState_type;


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_C;                            //2  C
    u16  Pnt_Out;                          //3  Out
    u16  Pnt_End;
}FuncD_TRIG_type;
//********************************************************
u16 *FuncD_TRIG_1(FuncD_TRIG_type *progPnt, u32 ramPnt)
{
    s16                 Out, C;
    DtrigState_type     *sPnt;
    
    sPnt = (DtrigState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    C = testBitMicro(ramPnt, progPnt->Pnt_C);
    if((!sPnt->ClockState)&&C){        //Rise
        Out = load_s16(ramPnt, progPnt->Pnt_In);
        sPnt->OutState = Out;
    }
    else{
        Out = sPnt->OutState;
    }
    save_s16(ramPnt, progPnt->Pnt_Out, sPnt->OutState);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncD_TRIG_1[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  C
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("D-TRIG", "DD", TblFuncD_TRIG_1, FuncD_TRIG_1, sizeof(TblFuncD_TRIG_1), sizeof(DtrigState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncD_TRIG_1,
#endif
//********************************************************
