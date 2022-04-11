#ifdef    GET_FUNC_CODE

typedef struct{
    s16     FiltrDinValue;
    s16     Filtr;
}DinFiltrState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                          //1  In
    u16  Pnt_Tfiltr;                      //2  Tfiltr
    u16  Pnt_Out;                         //3  Out
    u16  Pnt_End;
}FuncFILTR_1PIN_type;
//********************************************************
u16 *FuncFILTR_1PIN_1(FuncFILTR_1PIN_type *progPnt, u32 ramPnt)
{
    DinFiltrState_type      *sPnt;
    s16                     TimeFiltr;
    
    sPnt = (DinFiltrState_type *)(ramPnt + progPnt->Pnt_State);
    if(testBitMicro(ramPnt, progPnt->Pnt_In)){
        TimeFiltr = load_s16(ramPnt, progPnt->Pnt_Tfiltr); 
        if(sPnt->Filtr < TimeFiltr) sPnt->Filtr++;
        else                        setBitMicro(ramPnt, progPnt->Pnt_State);
    }
    else{
        if(sPnt->Filtr > 0) sPnt->Filtr--;
        else                clrBitMicro(ramPnt, progPnt->Pnt_State/SZ_DIV);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFILTR_1PIN_1[3]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Tfiltr
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("FILTR_1PIN", "DD", TblFuncFILTR_1PIN_1, FuncFILTR_1PIN_1, sizeof(TblFuncFILTR_1PIN_1), sizeof(DinFiltrState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFILTR_1PIN_1,
#endif
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//      s32              s32              s32
//********************************************************
#ifdef    GET_FUNC_CODE

//********************************************************
//extern u16 PrintfVal;
u16 *FuncFILTR_1PIN_2(FuncFILTR_1PIN_type *progPnt, u32 ramPnt)
{
    s16             Tfiltr;
    LL_filtr_type   LL;
    _iq             iq_in;
    s32             *iPnt;
    
    iPnt   = (s32 *)(ramPnt + progPnt->Pnt_State);
    Tfiltr = load_s16(ramPnt, progPnt->Pnt_Tfiltr);
    
    LL.i[0] = *iPnt;
    LL.i[1] = load_s32(ramPnt, progPnt->Pnt_Out);
    iq_in  = load_s32(ramPnt, progPnt->Pnt_In);
    //LL.lli = s64_Filtr(LL.lli, iq_in, Tfiltr);
    
    *iPnt = LL.i[0];
    save_s32(ramPnt, progPnt->Pnt_Out, LL.i[1]);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFILTR_1PIN_2[3]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Tfiltr
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("FILTR_1PIN", "DD", TblFuncFILTR_1PIN_2, FuncFILTR_1PIN_2, sizeof(TblFuncFILTR_1PIN_2), sizeof(_iq), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFILTR_1PIN_2,
#endif
//********************************************************
//3 Перезагрузка    3 Перезагрузка  3 Перезагрузка    
//      s32              s32              s32
//********************************************************
#ifdef    GET_FUNC_CODE

//********************************************************
u16 *FuncFILTR_1PIN_3(FuncFILTR_1PIN_type *progPnt, u32 ramPnt)
{
    _iq         Ref, temp, iq_out, diff;
    
    temp = load_s32(ramPnt, progPnt->Pnt_Tfiltr);
    temp = _IQmpy(temp, _IQ(FLOAT_TIME_CALL_MICRO));
    Ref  = load_s32(ramPnt, progPnt->Pnt_In);
    iq_out = load_s32(ramPnt, progPnt->Pnt_Out);
    
    diff = Ref - iq_out;
    if(iq_out >= 0) temp = (diff < 0) ? -temp : temp;
    else            temp = (diff < 0) ? -temp : temp;
    
    if(_IQabs(diff) < _IQabs(temp)) iq_out = Ref;
    else                            iq_out += temp;

    save_s32(ramPnt, progPnt->Pnt_Out, iq_out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFILTR_1PIN_3[3]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Tfiltr
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("FILTR_1PIN", "DD", TblFuncFILTR_1PIN_3, FuncFILTR_1PIN_3, sizeof(TblFuncFILTR_1PIN_3), 1, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFILTR_1PIN_3,
#endif
//********************************************************
