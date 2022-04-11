#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Num;                          //2  Num
    u16  Pnt_Out[4];                       //3-6  Out3
    u16  Pnt_End;
}FuncDC4_type;
//********************************************************
u16 *FuncDC4_1(FuncDC4_type *progPnt, u32 ramPnt)
{
    s16         Num, Val, i;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x3;
    Val = load_s16(ramPnt, progPnt->Pnt_In);
    for(i=0; i<4; i++){
        if(i!=Num)  save_s16(ramPnt, progPnt->Pnt_Out[i], 0);
        else        save_s16(ramPnt, progPnt->Pnt_Out[i], Val);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDC4_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Num
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Out0
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Out1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Out2
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //6  Out3
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("DC4", "DA", TblFuncDC4_1, FuncDC4_1, sizeof(TblFuncDC4_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDC4_1,
#endif
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncDC4_2(FuncDC4_type *progPnt, u32 ramPnt)
{
    s16         Num, Val, i;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x3;
    Val = testBitMicro(ramPnt, progPnt->Pnt_In);
    for(i=0; i<4; i++){
        if(i!=Num)  defBitMicro(ramPnt, progPnt->Pnt_Out[i], 0);
        else        defBitMicro(ramPnt, progPnt->Pnt_Out[i], Val);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDC4_2[6]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Num
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Out0
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Out1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Out2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //6  Out3
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("DC4", "DA", TblFuncDC4_2, FuncDC4_2, sizeof(TblFuncDC4_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDC4_2,
#endif
//********************************************************
