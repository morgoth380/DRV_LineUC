#ifdef    GET_FUNC_CODE
//********************************************************
typedef struct{
        s32         Out;
}OBRATN_SVIAZState_type;
//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_Arg_2;                        //2  Arg_2
    u16  Pnt_Out;                           //3  Out
    u16  Pnt_End;
}FuncOBRATN_SVIAZ_type;
//********************************************************
u16 *FuncOBRATN_SVIAZ_1(FuncOBRATN_SVIAZ_type *progPnt, u32 ramPnt)
{
    OBRATN_SVIAZState_type    *sPnt;
    s32                        Vin;
    s32                        Vout;
    
    Vin = load_s32(ramPnt, progPnt->Pnt_Arg_1);
    Vout = load_s32(ramPnt, progPnt->Pnt_Arg_2);
    sPnt = (OBRATN_SVIAZState_type *)(ramPnt + progPnt->Pnt_State);
    if (flagIsFirstCall){
        sPnt->Out = 0;
    }
    sPnt->Out = Vin - Vout;
    save_s32(ramPnt, progPnt->Pnt_Out, sPnt->Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncOBRATN_SVIAZ_1[3]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Arg_2
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("OBRATN_SVIAZ", "U", TblFuncOBRATN_SVIAZ_1, FuncOBRATN_SVIAZ_1, sizeof(TblFuncOBRATN_SVIAZ_1), sizeof(OBRATN_SVIAZState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncOBRATN_SVIAZ_1,
#endif
//********************************************************
