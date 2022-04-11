//********************************************************
//        Функция вычисления знака числа
//********************************************************
#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Out;                          //2  Out
    u16  Pnt_End;
}FuncSIGN_type;
//********************************************************
//********************************************************
//1 Перезагрузка    1 Перезагрузка  1 Перезагрузка    
//                       s16
//********************************************************
//********************************************************
u16 *FuncSIGN_1(FuncSIGN_type *progPnt, u32 ramPnt)
{
    if(load_s16(ramPnt, progPnt->Pnt_In) >= 0 )   
        clrBitMicro(ramPnt, progPnt->Pnt_Out);
    else            
        setBitMicro(ramPnt, progPnt->Pnt_Out);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSIGN_1[2]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //2  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("SIGN", "DA", TblFuncSIGN_1, FuncSIGN_1, sizeof(TblFuncSIGN_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSIGN_1,
#endif
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//                       s32
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncSIGN_2(FuncSIGN_type *progPnt, u32 ramPnt)
{
    if(load_s32(ramPnt, progPnt->Pnt_In) >= 0 )
        clrBitMicro(ramPnt, progPnt->Pnt_Out);
    else            
        setBitMicro(ramPnt, progPnt->Pnt_Out);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSIGN_2[2]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //2  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("SIGN", "DA", TblFuncSIGN_2, FuncSIGN_2, sizeof(TblFuncSIGN_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSIGN_2,
#endif
//********************************************************
