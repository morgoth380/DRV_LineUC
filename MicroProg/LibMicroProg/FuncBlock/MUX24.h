#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In[24];                       // 1   In0..In23
    u16  Pnt_Num;                          // 25  Num
    u16  Pnt_Out;                          // 26  Out
    u16  Pnt_End;
}FuncMUX24_type;
//********************************************************
u16 *FuncMUX24_1(FuncMUX24_type *progPnt, u32 ramPnt)
{
    s16         Num;
    f32         value;
    Num = load_s16(ramPnt, progPnt->Pnt_Num);
    if(Num > 23) 
        Num = 23;
    value = load_float(ramPnt, progPnt->Pnt_In[Num]);
    save_float(ramPnt, progPnt->Pnt_Out, value);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX24_1[26]={
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
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  In16
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  In17
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19  In18
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  In19
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  In20
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22  In21
        S32_PIN_TYPE | INPUT_PIN_MODE,     //23  In22
        S32_PIN_TYPE | INPUT_PIN_MODE,     //24  In23
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Num
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //26  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MUX24", "DD", TblFuncMUX24_1, FuncMUX24_1, sizeof(TblFuncMUX24_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX24_1,
#endif
//********************************************************
