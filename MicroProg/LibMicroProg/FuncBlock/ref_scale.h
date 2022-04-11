#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Ref_max;                      //1  Ref_max
    u16  Pnt_Ref_min;                      //2  Ref_min
    u16  Pnt_Ref_base;                     //3  Ref_base
    u16  Pnt_End;
}FuncREF_SCALE_type;
//********************************************************
u16 *FuncREF_SCALE_1(FuncREF_SCALE_type *progPnt, u32 ramPnt)
{   
    s16         max, min, base;
    
    max = load_s16(ramPnt, progPnt->Pnt_Ref_max);
    min = load_s16(ramPnt, progPnt->Pnt_Ref_min);
    base = max - min;
    save_s16(ramPnt, progPnt->Pnt_Ref_base,  base );
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncREF_SCALE_1[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Ref_max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Ref_min
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //3  Ref_base
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew("REF_SCALE", "DA", TblFuncREF_SCALE_1, FuncREF_SCALE_1, sizeof(TblFuncREF_SCALE_1), 0, 0, 1, sizeof(FuncREF_SCALE_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncREF_SCALE_1,
#endif
//********************************************************
