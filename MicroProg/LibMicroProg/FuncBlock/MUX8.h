#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In[8];                        //1  In0
    u16  Pnt_Num;                          //9  Num
    u16  Pnt_Out;                          //10 Out
    u16  Pnt_End;
}FuncMUX8_type;
//********************************************************
u16 *FuncMUX8_1(FuncMUX8_type *progPnt, u32 ramPnt)
{
    s16         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x7;
    Num = load_s16(ramPnt, progPnt->Pnt_In[Num]);
    save_s16(ramPnt, progPnt->Pnt_Out, Num);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX8_1[10]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Num
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //10  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MUX8", "DD", TblFuncMUX8_1, FuncMUX8_1, sizeof(TblFuncMUX8_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX8_1,
#endif
//********************************************************
//********************************************************
//2 Перезагрузка    3 Перезагрузка  3 Перезагрузка    
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncMUX8_2(FuncMUX8_type *progPnt, u32 ramPnt)
{
    s32         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x7;
    Num = load_s32(ramPnt, progPnt->Pnt_In[Num]);
    save_s32(ramPnt, progPnt->Pnt_Out, Num);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX8_2[10]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Num
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //10  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MUX8", "DD", TblFuncMUX8_2, FuncMUX8_2, sizeof(TblFuncMUX8_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX8_2,
#endif
//********************************************************
