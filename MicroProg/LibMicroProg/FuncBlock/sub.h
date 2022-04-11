#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_Arg_2;                        //2  Arg_2
    u16  Pnt_Arg_3;                        //3  Arg_3
    u16  Pnt_End;
}FuncSUB_type;
//********************************************************
u16 *FuncSUB_1(FuncSUB_type *progPnt, u32 ramPnt)
{
    s32     Arg1, Arg2;
    
    Arg1 = load_s16(ramPnt, progPnt->Pnt_Arg_1);
    Arg2 = load_s16(ramPnt, progPnt->Pnt_Arg_2);
    Arg1 -= Arg2;
    Arg1 = s16LimControl(Arg1);
    save_s16(ramPnt, progPnt->Pnt_Arg_3, (s16)Arg1);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSUB_1[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Arg_2
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //3  Arg_3
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("SUB", "SUB", TblFuncSUB_1, FuncSUB_1, sizeof(TblFuncSUB_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSUB_1,
#endif
//********************************************************
//********************************************************
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncSUB_2(FuncSUB_type *progPnt, u32 ramPnt)
{
    float     Arg1, Arg2;
    
    Arg1 = load_float(ramPnt, progPnt->Pnt_Arg_1);
    Arg2 = load_float(ramPnt, progPnt->Pnt_Arg_2);
    Arg1 -= Arg2;
    save_float(ramPnt, progPnt->Pnt_Arg_3, Arg1);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSUB_2[3]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Arg_2
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //3  Arg_3
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("SUB", "SUB", TblFuncSUB_2, FuncSUB_2, sizeof(TblFuncSUB_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSUB_2,
#endif
//********************************************************
