#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Bit_0;                        //1  Bit_0
    u16  Pnt_Bit_1;                        //2  Bit_1
    u16  Pnt_Bit_2;                        //3  Bit_2
    u16  Pnt_Bit_3;                        //4  Bit_3
    u16  Pnt_Out_u16;                      //5  Out_u16
    u16  Pnt_End;
}FuncBIT4_TO_U16_type;
//********************************************************
u16 *FuncBIT4_TO_U16_1(FuncBIT4_TO_U16_type *progPnt, u32 ramPnt)
{
    s16     Out;
    
    Out = 0;
    if(testBitMicro(ramPnt, progPnt->Pnt_Bit_0))    Out |= (1<<0);
    if(testBitMicro(ramPnt, progPnt->Pnt_Bit_1))    Out |= (1<<1);
    if(testBitMicro(ramPnt, progPnt->Pnt_Bit_2))    Out |= (1<<2);
    if(testBitMicro(ramPnt, progPnt->Pnt_Bit_3))    Out |= (1<<3);
    
    save_s16(ramPnt, progPnt->Pnt_Out_u16, Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBIT4_TO_U16_1[5]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Bit_0
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Bit_1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Bit_2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Bit_3
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //5  Out_u16
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("BIT4_TO_U16", "DD", TblFuncBIT4_TO_U16_1, FuncBIT4_TO_U16_1, sizeof(TblFuncBIT4_TO_U16_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBIT4_TO_U16_1,
#endif
//********************************************************
