#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
//    u16  Pnt_State;
    u16  Pnt_S;                            //1  S
    u16  Pnt_R1;                           //2  R
    u16  Pnt_R2;                           //3  R
    u16  Pnt_nQ;                           //4  ~Q
    u16  Pnt_Q;                            //5  Q
    u16  Pnt_End;
}FuncRS_TRIGGER_OR_type;
//********************************************************
u16 *FuncRS_TRIGGER_OR(FuncRS_TRIGGER_OR_type *progPnt, u32 ramPnt)
{
    if(flagIsFirstCall){         // Начальная установка триггера в сброс
        setBitMicro(ramPnt, progPnt->Pnt_nQ );
        clrBitMicro(ramPnt, progPnt->Pnt_Q  );
    }
    if(testBitMicro(ramPnt, progPnt->Pnt_R1) || testBitMicro(ramPnt, progPnt->Pnt_R2)){
        setBitMicro(ramPnt, progPnt->Pnt_nQ );
        clrBitMicro(ramPnt, progPnt->Pnt_Q  );
    }
    else if(testBitMicro(ramPnt, progPnt->Pnt_S)){
        clrBitMicro(ramPnt, progPnt->Pnt_nQ );
        setBitMicro(ramPnt, progPnt->Pnt_Q  );
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRS_TRIGGER_OR[5]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  S
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  R1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  R2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  ~Q
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //5  Q
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("RS-TRIGGER-OR", "DD", TblFuncRS_TRIGGER_OR, FuncRS_TRIGGER_OR, sizeof(TblFuncRS_TRIGGER_OR), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRS_TRIGGER_OR,
#endif
//********************************************************
