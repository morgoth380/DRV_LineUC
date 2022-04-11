#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
//    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Out;                          //2  Out
    u16  Pnt_End;
}FuncPULSE_RISE_PRM_type;
//********************************************************
// Перезакрузка для импульсных параметров двоичного типа Off/On
//********************************************************
u16 *FuncPULSE_RISE_PRM_1(FuncPULSE_RISE_PRM_type *progPnt, u32 ramPnt)
{
    if(testBitMicro(ramPnt, progPnt->Pnt_Out)==0 && load_s16(ramPnt, progPnt->Pnt_In)!= 0){
        setBitMicro(ramPnt, progPnt->Pnt_Out);
    }
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_Out);
        save_s16(ramPnt, progPnt->Pnt_In, 0);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPULSE_RISE_PRM_1[2]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("PULSE_RISE_PRM", "DD", TblFuncPULSE_RISE_PRM_1, FuncPULSE_RISE_PRM_1, sizeof(TblFuncPULSE_RISE_PRM_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPULSE_RISE_PRM_1,
#endif
