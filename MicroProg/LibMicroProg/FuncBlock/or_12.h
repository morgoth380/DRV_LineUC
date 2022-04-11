#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg[12];                        //1-12  Arg_1
    u16  Pnt_Arg_out;                        //13  Arg_9
    u16  Pnt_End;
}FuncOR_12_type;
//********************************************************
u16 *FuncOR_12_1(FuncOR_12_type *progPnt, u32 ramPnt)
{
    u16 i = 0;
    u16 out = 0;
    
    while (i < 12){
        out |= testBitMicro(ramPnt, progPnt->Pnt_Arg[i++]);
    }
    defBitMicro(ramPnt, progPnt->Pnt_Arg_out, out);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncOR_12_1[13]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Arg_2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Arg_3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Arg_4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Arg_5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Arg_6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Arg_7
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Arg_8
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Arg_9
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Arg_10
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  Arg_11
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  Arg_12
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //13  Arg_13
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("OR-12", "DD", TblFuncOR_12_1, FuncOR_12_1, sizeof(TblFuncOR_12_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncOR_12_1,
#endif
//********************************************************
