#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_TempIGBT;                     //1  TempIGBT
    u16  Pnt_TempBoard;                    //2  TempBoard
    u16  Pnt_Ifull;                        //3  Ifull
    u16  Pnt_AlarmOVL;                     //4  AlarmOVL
    u16  Pnt_Fpwm;                         //5  Fpwm
    u16  Pnt_FpwmDC;                       //6  FpwmDC
    u16  Pnt_EnFpwmDC;                     //7  EnFpwmDC
    u16  Pnt_TstartDR;                     //8  TstartDR
    u16  Pnt_TendDR;                       //9  TendDR
    u16  Pnt_FpwmMinDR;                    //10  FpwmMinDR
    u16  Pnt_FpwmMinDR_1;                  //11  FpwmMinDR
    u16  Pnt_FpwmStepDR;                   //12  FpwmStepDR
    u16  Pnt_ModeDR;                       //13  ModeDR
    u16  Pnt_TekFpwm;                      //14  TekFpwm
    u16  Pnt_WarnWorkDR;                   //15  WarnWorkDR
    u16  Pnt_End;
}FuncDERATING_CONTROL_type;
//********************************************************
u16 *FuncDERATING_CONTROL_1(FuncDERATING_CONTROL_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDERATING_CONTROL_1[15]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  TempIGBT
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  TempBoard
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Ifull
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  AlarmOVL
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Fpwm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  FpwmDC
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  EnFpwmDC
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  TstartDR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  TendDR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  FpwmMinDR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  FpwmMinDR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  FpwmStepDR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  ModeDR
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14  TekFpwm
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //15  WarnWorkDR
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DERATING_CONTROL", "DA", TblFuncDERATING_CONTROL_1, FuncDERATING_CONTROL_1, sizeof(TblFuncDERATING_CONTROL_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDERATING_CONTROL_1,
#endif
//********************************************************
