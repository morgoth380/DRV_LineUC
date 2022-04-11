#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In[16];                       //1  In0
    u16  Pnt_Num;                          //17  Num
    u16  Pnt_Out;                          //18  Out
    u16  Pnt_End;
}FuncMUX16_type;
//********************************************************
u16 *FuncMUX16_1(FuncMUX16_type *progPnt, u32 ramPnt)
{
    s16         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0xF;
    Num = load_s16(ramPnt, progPnt->Pnt_In[Num]);
    save_s16(ramPnt, progPnt->Pnt_Out, Num);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX16_1[18]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  In8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  In9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  In10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  In11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  In12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  In13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  In14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  In15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Num
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //18  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MUX16", "DD", TblFuncMUX16_1, FuncMUX16_1, sizeof(TblFuncMUX16_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX16_1,
#endif
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncMUX16_2(FuncMUX16_type *progPnt, u32 ramPnt)
{
    s16         Num;
    s32         data;
    
    Num  = load_s16(ramPnt, progPnt->Pnt_Num)&0xF;
    data = load_s32(ramPnt, progPnt->Pnt_In[Num]);
    save_s32(ramPnt, progPnt->Pnt_Out, data);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX16_2[18]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In10
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In11
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In12
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Num
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //18  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MUX16", "DD", TblFuncMUX16_2, FuncMUX16_2, sizeof(TblFuncMUX16_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX16_2,
#endif
