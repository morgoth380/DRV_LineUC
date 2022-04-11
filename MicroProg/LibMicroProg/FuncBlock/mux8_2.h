#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In[8];                        // 1  In0..In7
    u16  Pnt_Num1;                         // 9  Num1
    u16  Pnt_Num2;                         //10  Num2
    u16  Pnt_Out2;                         //11  Out2
    u16  Pnt_Out1;                         //12  Out1
    u16  Pnt_End;
}FuncMUX8_2_type;
//********************************************************
u16 *FuncMUX8_2_1(FuncMUX8_2_type *progPnt, u32 ramPnt)
{
    s16         Num;
    float       Temp;

    Num = load_s16(ramPnt, progPnt->Pnt_Num1)&0x7;         // Cчитываем тип источника задания
    Temp = load_float(ramPnt, progPnt->Pnt_In[Num]);
    save_float(ramPnt, progPnt->Pnt_Out1, Temp);           // Выдаем в микропрограмму задание, соотв. выбранному типу

    Num = load_s16(ramPnt, progPnt->Pnt_Num2)&0x7;
    Temp = load_float(ramPnt, progPnt->Pnt_In[Num]);
    save_float(ramPnt, progPnt->Pnt_Out2, Temp);           // выдаем в микропрограмму задание, соотв. выбранному типу для аналогового выхода????

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX8_2_1[12]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Num1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Num2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Out2
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //12  Out1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MUX8_2", "DA", TblFuncMUX8_2_1, FuncMUX8_2_1, sizeof(TblFuncMUX8_2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX8_2_1,
#endif
//********************************************************
