#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Hi;                           //1  Hi
    u16  Pnt_Lo;                           //2  Lo
    u16  Pnt_In;                           //3  In
    u16  Pnt_Out;                          //4  Out
    u16  Pnt_End;
}FuncCMP_4_type;
//********************************************************
// 1 Перезагрузка   1 Перезагрузка  1 Перезагрузка    
//      s16              s16              s16
//********************************************************
u16 *FuncCMP_4(FuncCMP_4_type *progPnt, u32 ramPnt)
{
    s16         Hi, Lo, In ;

    Hi   = load_s16(ramPnt, progPnt->Pnt_Hi);
    Lo   = load_s16(ramPnt, progPnt->Pnt_Lo);
    In   = load_s16(ramPnt, progPnt->Pnt_In);

    if     (In >= Hi)    
        setBitMicro(ramPnt, progPnt->Pnt_Out);
    else if(In <= Lo)    
        clrBitMicro(ramPnt, progPnt->Pnt_Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCMP_4[4]={
        S16_PIN_TYPE | INPUT_PIN_MODE,      //1  Hi
        S16_PIN_TYPE | INPUT_PIN_MODE,      //2  Lo
        S16_PIN_TYPE | INPUT_PIN_MODE,      //3  In
         BIT_PIN_TYPE | OUTPUT_PIN_MODE     //4  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("CMP_4", "DD", TblFuncCMP_4, FuncCMP_4, sizeof(TblFuncCMP_4), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMP_4,
#endif
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//      s32              s32              s32
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncCMP_4_2(FuncCMP_4_type *progPnt, u32 ramPnt)
{
    s32         Hi, Lo, In ;

    Hi   = load_s32(ramPnt, progPnt->Pnt_Hi);
    Lo   = load_s32(ramPnt, progPnt->Pnt_Lo);
    In   = load_s32(ramPnt, progPnt->Pnt_In);

    if     (In >= Hi)    
        setBitMicro(ramPnt, progPnt->Pnt_Out);
    else if(In <= Lo)    
        clrBitMicro(ramPnt, progPnt->Pnt_Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCMP_4_2[4]={
        S32_PIN_TYPE | INPUT_PIN_MODE,      //1  Hi
        S32_PIN_TYPE | INPUT_PIN_MODE,      //2  Lo
        S32_PIN_TYPE | INPUT_PIN_MODE,      //3  In
         BIT_PIN_TYPE | OUTPUT_PIN_MODE     //4  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("CMP_4", "DD", TblFuncCMP_4_2, FuncCMP_4_2, sizeof(TblFuncCMP_4_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMP_4_2,
#endif
//********************************************************
