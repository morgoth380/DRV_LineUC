#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_Arg_2;                        //2  Arg_2
    u16  Pnt_Arg_3;                        //3  Arg_3
    u16  Pnt_Arg_4;                        //4  Arg_4
    u16  Pnt_Arg_5;                        //5  Arg_5
    u16  Pnt_Arg_6;                        //6  Arg_6
    u16  Pnt_Arg_7;                        //7  Arg_7
    u16  Pnt_Arg_8;                        //8  Arg_8
    u16  Pnt_Arg_9;                        //9  Arg_9
    u16  Pnt_End;
}FuncOR_8_type;
//********************************************************
u16 *FuncOR_8_1(FuncOR_8_type *progPnt, u32 ramPnt)
{
    if( testBitMicro(ramPnt, progPnt->Pnt_Arg_1) ||
        testBitMicro(ramPnt, progPnt->Pnt_Arg_2) ||
        testBitMicro(ramPnt, progPnt->Pnt_Arg_3) ||
        testBitMicro(ramPnt, progPnt->Pnt_Arg_4) ||
        testBitMicro(ramPnt, progPnt->Pnt_Arg_5) ||
        testBitMicro(ramPnt, progPnt->Pnt_Arg_6) ||
        testBitMicro(ramPnt, progPnt->Pnt_Arg_7) ||
        testBitMicro(ramPnt, progPnt->Pnt_Arg_8) 
        )    
            setBitMicro(ramPnt, progPnt->Pnt_Arg_9);
    else    clrBitMicro(ramPnt, progPnt->Pnt_Arg_9);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncOR_8_1[9]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Arg_2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Arg_3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Arg_4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Arg_5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Arg_6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Arg_7
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Arg_8
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //9  Arg_9
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("OR-8", "DD", TblFuncOR_8_1, FuncOR_8_1, sizeof(TblFuncOR_8_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncOR_8_1,
#endif
//********************************************************
