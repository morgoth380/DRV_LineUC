#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_X0;                           //1  X0
    u16  Pnt_X1;                           //2  X1
    u16  Pnt_X2;                           //3  X2
    u16  Pnt_X3;                           //4  X3
    u16  Pnt_MIN;                       //5  MIN(X)
    u16  Pnt_MAX;                       //6  MAX(X)
    u16  Pnt_AVER;                      //7  AVER(X)
    u16  Pnt_End;
}FuncMIN_MAX_4_type;
//********************************************************
u16 *FuncMIN_MAX_4_1(FuncMIN_MAX_4_type *progPnt, u32 ramPnt)
{
    s16         x0, x1, x2, x3, min, max;
    
    x0 = load_s16(ramPnt, progPnt->Pnt_X0);
    x1 = load_s16(ramPnt, progPnt->Pnt_X1);
    x2 = load_s16(ramPnt, progPnt->Pnt_X2);
    x3 = load_s16(ramPnt, progPnt->Pnt_X3);
    min = minInt(x0, x1);
    min = minInt(min, x2);
    min = minInt(min, x3);
    max = maxInt(x0, x1);
    max = maxInt(max, x2);
    max = maxInt(max, x3);
    
    save_s16(ramPnt, progPnt->Pnt_MIN, min);
    save_s16(ramPnt, progPnt->Pnt_MAX, max);
    x0 = (s16)(((s32)x0+(s32)x1+(s32)x2+(s32)x3 + (s32)3)>>2);
    save_s16(ramPnt, progPnt->Pnt_AVER, x0);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMIN_MAX_4_1[7]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  X0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  X1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  X2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  X3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  MIN(X)
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  MAX(X)
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //7  AVER(X)
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MIN_MAX_4", "DA", TblFuncMIN_MAX_4_1, FuncMIN_MAX_4_1, sizeof(TblFuncMIN_MAX_4_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMIN_MAX_4_1,
#endif
//********************************************************
