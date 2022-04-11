#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_Arg_2;                        //2  Arg_2
    u16  Pnt_Arg_3;                        //3  Arg_3
    u16  Pnt_Arg_4;                        //4  Arg_4
    u16  Pnt_Arg_5;                        //5  Arg_5
    u16  Pnt_End; 
}FuncAND_4_type;
//********************************************************
u16 *FuncAND_4_1(FuncAND_4_type *progPnt, u32 ramPnt)
{
    s16     Arg1, Arg2, Arg3, Arg4;
    
    Arg1 = testBitMicro(ramPnt, progPnt->Pnt_Arg_1);
    Arg2 = testBitMicro(ramPnt, progPnt->Pnt_Arg_2);
    Arg3 = testBitMicro(ramPnt, progPnt->Pnt_Arg_3);
    Arg4 = testBitMicro(ramPnt, progPnt->Pnt_Arg_4);
    if(Arg1&Arg2&Arg3&Arg4) setBitMicro(ramPnt, progPnt->Pnt_Arg_5);
    else                        clrBitMicro(ramPnt, progPnt->Pnt_Arg_5);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAND_4_1[5]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Arg_2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Arg_3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Arg_4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //5  Arg_5
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("AND-4", "DD", TblFuncAND_4_1, FuncAND_4_1, sizeof(TblFuncAND_4_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAND_4_1,
#endif
//********************************************************
