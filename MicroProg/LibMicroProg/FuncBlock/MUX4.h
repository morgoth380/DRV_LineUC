#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In[4];                        //1  In0
    u16  Pnt_Num;                          //5  Num
    u16  Pnt_Out;                          //6  Out
    u16  Pnt_End;
}FuncMUX4_type;
//********************************************************
u16 *FuncMUX4_1(FuncMUX4_type *progPnt, u32 ramPnt)
{
    s16         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x3;
    Num = load_s16(ramPnt, progPnt->Pnt_In[Num]);
    save_s16(ramPnt, progPnt->Pnt_Out, Num);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX4_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Num
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //6  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MUX4", "DD", TblFuncMUX4_1, FuncMUX4_1, sizeof(TblFuncMUX4_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX4_1,
#endif
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncMUX4_2(FuncMUX4_type *progPnt, u32 ramPnt)
{
    s16         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x3;
    Num = testBitMicro(ramPnt, progPnt->Pnt_In[Num]);
    defBitMicro(ramPnt, progPnt->Pnt_Out, Num);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX4_2[6]={
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Num
         BIT_PIN_TYPE | OUTPUT_PIN_MODE     //6  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MUX4", "DD", TblFuncMUX4_2, FuncMUX4_2, sizeof(TblFuncMUX4_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX4_2,
#endif
//********************************************************
//********************************************************
//3 Перезагрузка    3 Перезагрузка  3 Перезагрузка    
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncMUX4_3(FuncMUX4_type *progPnt, u32 ramPnt)
{
    s32         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x3;
    Num = load_s32(ramPnt, progPnt->Pnt_In[Num]);
    save_s32(ramPnt, progPnt->Pnt_Out, Num);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX4_3[6]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Num
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //6  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MUX4", "DD", TblFuncMUX4_3, FuncMUX4_3, sizeof(TblFuncMUX4_3), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX4_3,
#endif
//********************************************************
