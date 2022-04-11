//********************************************************
//              Функция инвертора
//********************************************************
#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Inv;                          //2  Inv
    u16  Pnt_Out;                          //3  Out
    u16  Pnt_End;
}FuncINVERTOR_type;
//********************************************************
//********************************************************
//1 Перезагрузка    1 Перезагрузка  1 Перезагрузка    
//                       s16
//********************************************************
//********************************************************
u16 *FuncINVERTOR_1(FuncINVERTOR_type *progPnt, u32 ramPnt)
{
    s16         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_In);
    if(testBitMicro(ramPnt, progPnt->Pnt_Inv))  Num *= -1;
    save_s16(ramPnt, progPnt->Pnt_Out, Num );
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncINVERTOR_1[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
          BIT_PIN_TYPE | INPUT_PIN_MODE,   //2  Inv
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("INVERTOR", "DA", TblFuncINVERTOR_1, FuncINVERTOR_1, sizeof(TblFuncINVERTOR_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncINVERTOR_1,
#endif
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//                       IQ !!!!
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncINVERTOR_2(FuncINVERTOR_type *progPnt, u32 ramPnt)
{
    s32         Num;
    
    Num = load_s32(ramPnt, progPnt->Pnt_In);
    if(testBitMicro(ramPnt, progPnt->Pnt_Inv))  
        Num = _IQrmpy(Num, _IQ(-1.0));
    save_s32(ramPnt, progPnt->Pnt_Out, Num );
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncINVERTOR_2[3]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
          BIT_PIN_TYPE | INPUT_PIN_MODE,   //2  Inv
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("INVERTOR", "DA", TblFuncINVERTOR_2, FuncINVERTOR_2, sizeof(TblFuncINVERTOR_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncINVERTOR_2,
#endif
//********************************************************
//********************************************************
//3 Перезагрузка    3 Перезагрузка  3 Перезагрузка    
//                       
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncINVERTOR_3(FuncINVERTOR_type *progPnt, u32 ramPnt)
{
    s16         Num;

    Num = load_s16(ramPnt, progPnt->Pnt_In);
    if(testBitMicro(ramPnt, progPnt->Pnt_Inv)) Num = !Num;
    defBitMicro(ramPnt, progPnt->Pnt_Out, Num);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncINVERTOR_3[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Inv
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("INVERTOR", "DA", TblFuncINVERTOR_3, FuncINVERTOR_3, sizeof(TblFuncINVERTOR_3), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncINVERTOR_3,
#endif
//********************************************************
