#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_Arg_2;                        //2  Arg_2
    u16  Pnt_End;
}FuncMGRP_type;
//********************************************************
u16 *FuncMGRP_1(FuncMGRP_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMGRP_1[2]={
        TXT_PIN_TYPE | PASSIVE_PIN_MODE,   //1  Arg_1
        TXT_PIN_TYPE | PASSIVE_PIN_MODE    //2  Arg_2
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MGRP", "MGRP", TblFuncMGRP_1, FuncMGRP_1, sizeof(TblFuncMGRP_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMGRP_1,
#endif
//********************************************************
