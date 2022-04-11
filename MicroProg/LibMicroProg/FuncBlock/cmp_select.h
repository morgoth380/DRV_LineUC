#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_A;                            //1  A
    u16  Pnt_B;                            //2  B
    u16  Pnt_mode;                         //3  mode
    u16  Pnt_hister;                       //4  hister
    u16  Pnt_Out;                          //5  Out
    u16  Pnt_End;
}FuncCMP_SELECT_type;
//********************************************************
// 1 Перезагрузка   1 Перезагрузка  1 Перезагрузка    
//      s16              s16              s16
//********************************************************
u16 *FuncCMP_SELECT_1(FuncCMP_SELECT_type *progPnt, u32 ramPnt)
{
    s16         mode, hister, Out, A, B;
    
    mode    = load_s16(ramPnt, progPnt->Pnt_mode) & 0x3;
    hister  = load_s16(ramPnt, progPnt->Pnt_hister);
    Out     = testBitMicro(ramPnt, progPnt->Pnt_Out);

    if(mode == 3){
        B   = load_s16(ramPnt, progPnt->Pnt_A);
        A   = load_s16(ramPnt, progPnt->Pnt_B);
    }
    else{
        A   = load_s16(ramPnt, progPnt->Pnt_A);
        B   = load_s16(ramPnt, progPnt->Pnt_B);
    }
    switch(mode){
    case 0: Out = 0;    break;      // == 0
    case 1:case 3:                  // A>B A<B 
        if      (A > (B + hister))  Out = 1;
        else if (A < (B - hister))  Out = 0;
//        else                        Out = 0;
        break;
    case 2: Out = (abs(A - B) > hister) ? 0:1;      break;
    }

    defBitMicro(ramPnt, progPnt->Pnt_Out, Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCMP_SELECT_1[5]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  mode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  hister
         BIT_PIN_TYPE | OUTPUT_PIN_MODE     //5  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("CMP_SELECT", "DD", TblFuncCMP_SELECT_1, FuncCMP_SELECT_1, sizeof(TblFuncCMP_SELECT_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMP_SELECT_1,
#endif
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//      s32              s32              s32
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncCMP_SELECT_2(FuncCMP_SELECT_type *progPnt, u32 ramPnt)
{
    s32         hister,  A, B;
    s16         Out, mode;
    
    mode    = load_s16(ramPnt, progPnt->Pnt_mode) & 0x3;
    hister  = load_s32(ramPnt, progPnt->Pnt_hister);
    Out     = testBitMicro(ramPnt, progPnt->Pnt_Out);

    if(mode == 3){
        B   = load_s32(ramPnt, progPnt->Pnt_A);
        A   = load_s32(ramPnt, progPnt->Pnt_B);
    }
    else{
        A   = load_s32(ramPnt, progPnt->Pnt_A);
        B   = load_s32(ramPnt, progPnt->Pnt_B);
    }
    switch(mode){
    case 0: Out = 0;    break;      // == 0
    case 1:case 3:                  // A>B A<B 
        if      (A > (B + hister))  Out = 1;
//        else if (A < (B - hister))  Out = 0;
        else                        Out = 0;
        break;
    case 2: Out = (_IQabs(A - B) > hister) ? 0:1;      break;
    }

    defBitMicro(ramPnt, progPnt->Pnt_Out, Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCMP_SELECT_2[5]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  mode
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  hister
         BIT_PIN_TYPE | OUTPUT_PIN_MODE     //5  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("CMP_SELECT", "DD", TblFuncCMP_SELECT_2, FuncCMP_SELECT_2, sizeof(TblFuncCMP_SELECT_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMP_SELECT_2,
#endif
//********************************************************
