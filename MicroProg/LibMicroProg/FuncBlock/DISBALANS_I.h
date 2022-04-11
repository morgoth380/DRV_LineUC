#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_IU;                           //1  IU
    u16  Pnt_IV;                           //2  IV
    u16  Pnt_IW;                           //3  IW
    u16  Pnt_Inom;                         //4  Inom
    u16  Pnt_dImax;                        //5  dImax
    u16  Pnt_dI;                           //6  dI
    u16  Pnt_Err;                          //7  Err
    u16  Pnt_End;
}FuncDISBALANS_I_type;
//********************************************************
u16 *FuncDISBALANS_I_1(FuncDISBALANS_I_type *progPnt, u32 ramPnt)
{
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDISBALANS_I_1[7]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  IU
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  IV
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  IW
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Inom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  dImax
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //6  dI
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //7  Err
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew("DISBALANS_I", "DA", TblFuncDISBALANS_I_1, FuncDISBALANS_I_1, sizeof(TblFuncDISBALANS_I_1), 0, 0, 1, sizeof(FuncDISBALANS_I_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDISBALANS_I_1,
#endif
//********************************************************
