#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Max;                          //1  Max
    u16  Pnt_In;                           //2  In
    u16  Pnt_Min;                          //3  Min
    u16  Pnt_Out;                          //4  Out
    u16  Pnt_End;
}FuncMIN_MAX_LIMITER_type;
//********************************************************
u16 *FuncMIN_MAX_LIMITER_2(FuncMIN_MAX_LIMITER_type *progPnt, u32 ramPnt)
{
    float   Max, In, Min;
    
    Max = load_float(ramPnt, progPnt->Pnt_Max);
    In  = load_float(ramPnt, progPnt->Pnt_In );
    Min = load_float(ramPnt, progPnt->Pnt_Min);
    
    // Проверим приделы
    if(In > Max){ 
      In = Max; 
      save_float(ramPnt, progPnt->Pnt_In, Max);
    }
    
    if(In < Min){ 
      In = Min; 
      save_float(ramPnt, progPnt->Pnt_In, Min);
    }
    
    save_float(ramPnt, progPnt->Pnt_Out, In);
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMIN_MAX_LIMITER_2[4]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Min
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //4  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MIN_MAX_LIMITER", "DA", TblFuncMIN_MAX_LIMITER_2, FuncMIN_MAX_LIMITER_2, sizeof(TblFuncMIN_MAX_LIMITER_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMIN_MAX_LIMITER_2,
#endif
