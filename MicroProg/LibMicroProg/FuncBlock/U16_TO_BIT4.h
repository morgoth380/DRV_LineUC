#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Bit[4];                       //1  Bit_0
    u16  Pnt_In_u16;                       //5  In_u16
    u16  Pnt_End;
}FuncU16_TO_BIT4_type;
//********************************************************
u16 *FuncU16_TO_BIT4_1(FuncU16_TO_BIT4_type *progPnt, u32 ramPnt)
{
    s16     In, Nbit;
    u16     *BitPnt;
    
    In = load_s16(ramPnt, progPnt->Pnt_In_u16);
    for(BitPnt = progPnt->Pnt_Bit, Nbit=0; Nbit<4; Nbit++, BitPnt++){
        if(In & (1<<Nbit))  setBitMicro(ramPnt, *BitPnt);
        else                clrBitMicro(ramPnt, *BitPnt);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncU16_TO_BIT4_1[5]={
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //1  Bit_0
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Bit_1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Bit_2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Bit_3
        S16_PIN_TYPE | INPUT_PIN_MODE      //5  In_u16
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("U16_TO_BIT4", "DD", TblFuncU16_TO_BIT4_1, FuncU16_TO_BIT4_1, sizeof(TblFuncU16_TO_BIT4_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncU16_TO_BIT4_1,
#endif
//********************************************************
