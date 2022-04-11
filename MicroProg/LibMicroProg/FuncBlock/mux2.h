#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In[2];                        //1  In0
    u16  Pnt_Num;                          //3  Num
    u16  Pnt_Out;                          //4  Out
    u16  Pnt_End;
}FuncMUX2_type;
//********************************************************
u16 *FuncMUX2_1(FuncMUX2_type *progPnt, u32 ramPnt)
{
    s16         Num;
    
    Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x1;
    Num = load_s16(ramPnt, progPnt->Pnt_In[Num]);
    save_s16(ramPnt, progPnt->Pnt_Out, Num);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX2_1[4]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Num
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //4  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MUX2", "DA", TblFuncMUX2_1, FuncMUX2_1, sizeof(TblFuncMUX2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX2_1,
#endif