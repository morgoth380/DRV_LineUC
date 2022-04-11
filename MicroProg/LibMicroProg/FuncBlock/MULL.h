#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_A;                            //1  A
    u16  Pnt_B;                            //2  B
    u16  Pnt_A_B;                          //3  A*B
    u16  Pnt_End;
}FuncMULL_type;
//********************************************************
// 1 Перезагрузка   1 Перезагрузка  1 Перезагрузка    
//      s16              s16              s16
//********************************************************
u16 *FuncMULL_1(FuncMULL_type *progPnt, u32 ramPnt)
{
    s32     Arg1, Arg2;
    
    Arg1 = load_s16(ramPnt, progPnt->Pnt_A);
    Arg2 = load_s16(ramPnt, progPnt->Pnt_B);
    Arg1 *= Arg2;
    if(Arg1 > 32767)    Arg1 = 32767;
    if(Arg1 < -32768)   Arg1 = -32768;
    save_s16(ramPnt, progPnt->Pnt_A_B, (s16)Arg1);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMULL_1[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //3  A*B
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MULL", "DA", TblFuncMULL_1, FuncMULL_1, sizeof(TblFuncMULL_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMULL_1,
#endif
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//      s32              s32              s32
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncMULL_2(FuncMULL_type *progPnt, u32 ramPnt)
{
    /*
    s32     Arg1, Arg2;
    
    Arg1 = load_s32(ramPnt, progPnt->Pnt_A);
    Arg2 = load_s32(ramPnt, progPnt->Pnt_B);
    Arg1 = _IQrmpy(Arg1, Arg2);

    save_s32(ramPnt, progPnt->Pnt_A_B, Arg1);
    */
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMULL_2[3]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //3  A*B
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MULL", "DA", TblFuncMULL_2, FuncMULL_2, sizeof(TblFuncMULL_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMULL_2,
#endif

//********************************************************
//3 Перезагрузка    3 Перезагрузка  3 Перезагрузка
//      s16              s16              s32
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncMULL_3(FuncMULL_type *progPnt, u32 ramPnt)
{
  /*
    s16     Arg1, Arg2, sign;
        s32             Out;

    Arg1 = load_s16(ramPnt, progPnt->Pnt_A);
    Arg2 = load_s16(ramPnt, progPnt->Pnt_B);
        sign = 1;
        if (Arg1 < 0){
            sign = -1;
            Arg1 *= sign;
        }
        if (Arg2 * sign > 0){
            Arg2 *= sign;
            sign = 1;
        }else{
            if (sign == 1){
                sign = -1;
                Arg2 *= sign;
            }
        }
        Out = (s32)Arg1 * (s32)Arg2 * (s32)sign;
    save_s32(ramPnt, progPnt->Pnt_A_B, Out);
  */
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMULL_3[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //3  A*B
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MULL", "DA", TblFuncMULL_3, FuncMULL_3, sizeof(TblFuncMULL_3), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMULL_3,
#endif

