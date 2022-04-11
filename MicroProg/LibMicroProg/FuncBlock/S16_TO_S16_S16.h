#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_OutLow;                       //1  OutLow
    u16  Pnt_OutHigh;                      //2  OutHigh
    u16  Pnt_In_U16;                       //3  In_U16
    u16  Pnt_End;
}FuncS16_TO_S16_S16_type;
//********************************************************
u16 *FuncS16_TO_S16_S16_1(FuncS16_TO_S16_S16_type *progPnt, u32 ramPnt)
{
    u16         tmp;
    
    tmp = load_s16(ramPnt, progPnt->Pnt_In_U16);
    save_s16(ramPnt, progPnt->Pnt_OutLow, tmp&0xFF);
    save_s16(ramPnt, progPnt->Pnt_OutHigh, (tmp>>8)&0xFF);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncS16_TO_S16_S16_1[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  OutLow
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //2  OutHigh
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //3  In_U16
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("S16_TO_S16_S16", "DD", TblFuncS16_TO_S16_S16_1, FuncS16_TO_S16_S16_1, sizeof(TblFuncS16_TO_S16_S16_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncS16_TO_S16_S16_1,
#endif
//********************************************************
