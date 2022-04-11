#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
//    u16  Pnt_State;
    u16  Pnt_S;                            //1  S
    u16  Pnt_R;                            //2  R
    u16  Pnt_nQ;                           //3  ~Q
    u16  Pnt_Q;                            //4  Q
    u16  Pnt_End;
}FuncRS_TRIGGER_type;
//********************************************************
u16 *FuncRS_TRIGGER_1(FuncRS_TRIGGER_type *progPnt, u32 ramPnt)
{
    if(flagIsFirstCall){            // Начальная установка триггера в сброс
        setBitMicro(ramPnt, progPnt->Pnt_nQ );
        clrBitMicro(ramPnt, progPnt->Pnt_Q  );
    }
    if(testBitMicro(ramPnt, progPnt->Pnt_R)){
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
const char TblFuncRS_TRIGGER_1[4]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  S
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  R
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  ~Q
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //4  Q
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("RS-TRIGGER", "DD", TblFuncRS_TRIGGER_1, FuncRS_TRIGGER_1, sizeof(TblFuncRS_TRIGGER_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRS_TRIGGER_1,
#endif
//********************************************************
