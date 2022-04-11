//********************************************************
//        Функция вычисления абсолютной величины
//********************************************************
#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Out;                          //2  Out
    u16  Pnt_End;
}FuncABS_type;
//********************************************************
//********************************************************
//1 Перезагрузка    1 Перезагрузка  1 Перезагрузка    
//                       s16
//********************************************************
//********************************************************
u16 *FuncABS_1(FuncABS_type *progPnt, u32 ramPnt)
{
    s16         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_In);
    save_s16(ramPnt, progPnt->Pnt_Out, abs(Num) );

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncABS_1[2]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //2  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("ABS", "DA", TblFuncABS_1, FuncABS_1, sizeof(TblFuncABS_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncABS_1,
#endif
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//                       s32
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncABS_2(FuncABS_type *progPnt, u32 ramPnt)
{
    s32         Num;
    
    Num = load_s32(ramPnt, progPnt->Pnt_In);
    save_s32(ramPnt, progPnt->Pnt_Out, _IQabs(Num));

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncABS_2[2]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //2  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("ABS", "DA", TblFuncABS_2, FuncABS_2, sizeof(TblFuncABS_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncABS_2,
#endif
//********************************************************
