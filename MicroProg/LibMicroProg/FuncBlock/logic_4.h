#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Out;                          //1  Out
    u16  Pnt_Tabl;                         //2  Tabl
    u16  Pnt_bit0;                         //3  bit0
    u16  Pnt_bit1;                         //4  bit1
    u16  Pnt_bit2;                         //5  bit2
    u16  Pnt_bit3;                         //6  bit3
    u16  Pnt_End;
}FuncLOGIC_4_type;
//********************************************************
u16 *FuncLOGIC_4_1(FuncLOGIC_4_type *progPnt, u32 ramPnt)
{
    u16     bit0, bit1, bit2, bit3;
    u16     tmp, Tabl;
    
    bit0 = testBitMicro(ramPnt, progPnt->Pnt_bit0);
    bit1 = testBitMicro(ramPnt, progPnt->Pnt_bit1);
    bit2 = testBitMicro(ramPnt, progPnt->Pnt_bit2);
    bit3 = testBitMicro(ramPnt, progPnt->Pnt_bit3);
    tmp = (bit0<<0)|(bit1<<1)|(bit2<<2)|(bit3<<3);
    Tabl = load_s16(ramPnt, progPnt->Pnt_Tabl);
    tmp = (Tabl>>tmp) & 1;
    defBitMicro(ramPnt, progPnt->Pnt_Out, tmp);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLOGIC_4_1[6]={
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //1  Out 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Tabl
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  bit0  
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  bit1  
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  bit2  
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  bit3  
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LOGIC_4", "DD", TblFuncLOGIC_4_1, FuncLOGIC_4_1, sizeof(TblFuncLOGIC_4_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLOGIC_4_1,
#endif
//********************************************************
