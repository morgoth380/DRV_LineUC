#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Bit[16];                      //1  Bit_0
    u16  Pnt_Out_u16;                      //17  Out_u16
    u16  Pnt_End;
}FuncBIT16_TO_U16_type;
//********************************************************
u16 *FuncBIT16_TO_U16_1(FuncBIT16_TO_U16_type *progPnt, u32 ramPnt)
{
    s16     Nbit, Out;
    
    for(Out=0, Nbit=0; Nbit<16; Nbit++){
        if(testBitMicro(ramPnt, progPnt->Pnt_Bit[Nbit]))    Out |= (1<<Nbit);
    }
    save_s16(ramPnt, progPnt->Pnt_Out_u16, Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBIT16_TO_U16_1[17]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Bit_0
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Bit_1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Bit_2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Bit_3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Bit_4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Bit_5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Bit_6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Bit_7
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Bit_8
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Bit_9
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  Bit_10
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  Bit_11
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  Bit_12
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  Bit_13
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  Bit_14
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //16  Bit_15
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //17  Out_u16
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("BIT16_TO_U16", "DD", TblFuncBIT16_TO_U16_1, FuncBIT16_TO_U16_1, sizeof(TblFuncBIT16_TO_U16_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBIT16_TO_U16_1,
#endif
//********************************************************
