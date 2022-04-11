#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_EnLog;                        //1  EnLog
    u16  Pnt_Fast0;                        //2  Fast0
    u16  Pnt_Fast1;                        //3  Fast1
    u16  Pnt_Fast2;                        //4  Fast2
    u16  Pnt_Fast3;                        //5  Fast3
    u16  Pnt_Slow_0;                       //6  Slow_0
    u16  Pnt_Slow_1;                       //7  Slow_1
    u16  Pnt_Slow_2;                       //8  Slow_2
    u16  Pnt_Slow_3;                       //9  Slow_3
    u16  Pnt_Slow_4;                       //10  Slow_4
    u16  Pnt_Slow_5;                       //11  Slow_5
    u16  Pnt_Slow_6;                       //12  Slow_6
    u16  Pnt_Slow_7;                       //13  Slow_7
    u16  Pnt_End;
}FuncFAST_JRN_type;
//********************************************************
u16 *FuncFAST_JRN_1(FuncFAST_JRN_type *progPnt, u32 ramPnt)
{
    load_s32(ramPnt, progPnt->Pnt_U);
    
    
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFAST_JRN_1[13]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  EnLog
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Fast0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Fast1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Fast2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Fast3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Slow_0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Slow_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Slow_2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Slow_3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Slow_4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Slow_5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Slow_6
        S16_PIN_TYPE | INPUT_PIN_MODE      //13  Slow_7
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FAST_JRN", "DA", TblFuncFAST_JRN_1, FuncFAST_JRN_1, sizeof(TblFuncFAST_JRN_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFAST_JRN_1,
#endif
//********************************************************
