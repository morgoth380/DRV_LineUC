#ifdef    GET_FUNC_CODE

#define minInt(x0,x1)   ((x0>x1) ? x1 : x0)
#define maxInt(x0,x1)   ((x0>x1) ? x0 : x1)

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_X0;                        //1  X0
    u16  Pnt_X1;                        //2  X1
    u16  Pnt_MIN;                       //3  MIN(X)
    u16  Pnt_MAX;                       //4  MAX(X)
    u16  Pnt_AVER;                      //5  AVER(X)
    u16  Pnt_End;
}FuncMIN_MAX_2_type;
//********************************************************
// 1 перегрузка   1 перегрузка  1 перегрузка    
//      s16           s16            s16
//********************************************************
u16 *FuncMIN_MAX_2_1(FuncMIN_MAX_2_type *progPnt, u32 ramPnt)
{
    s16         x0, x1, min, max;
    
    x0 = load_s16(ramPnt, progPnt->Pnt_X0);
    x1 = load_s16(ramPnt, progPnt->Pnt_X1);
    min = minInt(x0, x1);
    max = maxInt(x0, x1);
    x0 = (s16)(((s32)x0+(s32)x1 + (s32)1)>>1);
    save_s16(ramPnt, progPnt->Pnt_MIN, min);
    save_s16(ramPnt, progPnt->Pnt_MAX, max);
    save_s16(ramPnt, progPnt->Pnt_AVER, x0);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMIN_MAX_2_1[5]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  X0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  X1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //3  MIN(X)
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  MAX(X)
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //5  AVER(X)
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MIN_MAX_2", "DA", TblFuncMIN_MAX_2_1, FuncMIN_MAX_2_1, sizeof(TblFuncMIN_MAX_2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMIN_MAX_2_1,
#endif
//********************************************************
//2 перегрузка    2 перегрузка  2 перегрузка    
//                   float              
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncMIN_MAX_2_2(FuncMIN_MAX_2_type *progPnt, u32 ramPnt)
{
    float         x0, x1, min, max, aver;
    
    x0 = load_float(ramPnt, progPnt->Pnt_X0);
    x1 = load_float(ramPnt, progPnt->Pnt_X1);
    
    min = minInt(x0, x1);
    max = maxInt(x0, x1);
    aver = (x0 + x1) * 0.5f;
    
    save_float(ramPnt, progPnt->Pnt_MIN, min);
    save_float(ramPnt, progPnt->Pnt_MAX, max);
    save_float(ramPnt, progPnt->Pnt_AVER, aver);   // Среднее значение
  
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMIN_MAX_2_2[5]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  X0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  X1
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //3  MIN(X)
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //4  MAX(X)
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //5  AVER(X)
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MIN_MAX_2", "DA", TblFuncMIN_MAX_2_2, FuncMIN_MAX_2_2, sizeof(TblFuncMIN_MAX_2_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMIN_MAX_2_2,
#endif
