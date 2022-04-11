#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In_u16;                       //1  In_u16
    u16  Pnt_Bit_7;
    u16  Pnt_Bit_6;
    u16  Pnt_Bit_5;
    u16  Pnt_Bit_4;
    u16  Pnt_Bit_3;
    u16  Pnt_Bit_2;
    u16  Pnt_Bit_1;
    u16  Pnt_Bit_0;
    u16  Pnt_End;
}FuncU16_TO_BIT8_type;
//********************************************************
u16 *FuncU16_TO_BIT8_1(FuncU16_TO_BIT8_type *progPnt, u32 ramPnt)
{
    u16     In, Nbit, *BitPnt;

    In = load_s16(ramPnt, progPnt->Pnt_In_u16);
    BitPnt = &progPnt->Pnt_Bit_7;
    for( Nbit=0x80; Nbit>0; Nbit >>= 1){
        if(In & Nbit)        setBitMicro(ramPnt, *BitPnt);
        else                clrBitMicro(ramPnt, *BitPnt);
        BitPnt++;
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncU16_TO_BIT8_1[9]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //0  In_u16
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //1  Bit_7
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Bit_6
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Bit_5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Bit_4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Bit_3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Bit_2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Bit_1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Bit_0
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("U16_TO_BIT8", "DD", TblFuncU16_TO_BIT8_1, FuncU16_TO_BIT8_1, sizeof(TblFuncU16_TO_BIT8_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncU16_TO_BIT8_1,
#endif
//********************************************************
