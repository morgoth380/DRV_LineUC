#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In_u16;    //1  In_u16
    u16  Pnt_Bit_15;    // 2 
    u16  Pnt_Bit_14;    // 3 
    u16  Pnt_Bit_13;    // 4 
    u16  Pnt_Bit_12;    // 5 
    u16  Pnt_Bit_11;    // 6 
    u16  Pnt_Bit_10;    // 7 
    u16  Pnt_Bit_9 ;    // 8 
    u16  Pnt_Bit_8 ;    // 9 
    u16  Pnt_Bit_7 ;    // 10
    u16  Pnt_Bit_6 ;    // 11
    u16  Pnt_Bit_5 ;    // 12
    u16  Pnt_Bit_4 ;    // 13
    u16  Pnt_Bit_3 ;    // 14
    u16  Pnt_Bit_2 ;    // 15
    u16  Pnt_Bit_1 ;    // 16
    u16  Pnt_Bit_0 ;    // 17
    u16  Pnt_End;
}FuncU16_TO_BIT16_type;
//********************************************************
u16 *FuncU16_TO_BIT16_1(FuncU16_TO_BIT16_type *progPnt, u32 ramPnt)
{
    u16     In, *BitPnt;

    In = load_s16(ramPnt, progPnt->Pnt_In_u16);
    BitPnt = &progPnt->Pnt_Bit_0;
    do{
        if(In & 0x1)        setBitMicro(ramPnt, *BitPnt);
        else                clrBitMicro(ramPnt, *BitPnt);
        BitPnt--;        In >>=1;
        }while( BitPnt != &progPnt->Pnt_In_u16 );
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncU16_TO_BIT16_1[17]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //0  In_u16
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //1  Bit_15
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Bit_14
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Bit_13
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Bit_12
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Bit_11
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Bit_10
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Bit_9 
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Bit_8 
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
OpisFuncBlock("U16_TO_BIT16", "DD", TblFuncU16_TO_BIT16_1, FuncU16_TO_BIT16_1, sizeof(TblFuncU16_TO_BIT16_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncU16_TO_BIT16_1,
#endif
//********************************************************
