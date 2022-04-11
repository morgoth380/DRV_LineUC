#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Kp;                           //1  Kp
    u16  Pnt_Ti;                           //2  Ti
    u16  Pnt_Td;                           //3  Td
    u16  Pnt_DifFiltr;                     //4  DifFiltr
    u16  Pnt_Max;                          //5  Max
    u16  Pnt_Min;                          //6  Min
    u16  Pnt_TempMax;                      //7  TempMax
    u16  Pnt_UinDC;                        //8  Напряжение входа в регулировку DC
    u16  Pnt_EN;                           //9  EN
    u16  Pnt_End;
}FuncPID_PWM_type;
//********************************************************
u16 *FuncPID_PWM_1(FuncPID_PWM_type *progPnt, u32 ramPnt)
{
    if (testBitMicro(ramPnt, progPnt->Pnt_EN)){
        COMMONPARAMM3 *mPnt3;
        mPnt3 = &GlobalM3;
        
        mPnt3->ppid_ust.Kp = load_s32(ramPnt, progPnt->Pnt_Kp);
        mPnt3->ppid_ust.Ti = load_s16(ramPnt, progPnt->Pnt_Ti);
        mPnt3->ppid_ust.Td = load_s16(ramPnt, progPnt->Pnt_Td);
        mPnt3->ppid_ust.DifFiltr = load_s16(ramPnt, progPnt->Pnt_DifFiltr);
        mPnt3->ppid_ust.Max = load_s32(ramPnt, progPnt->Pnt_Max);
        mPnt3->ppid_ust.Min = load_s32(ramPnt, progPnt->Pnt_Min);
        mPnt3->ppid_ust.TempMax = load_s16(ramPnt, progPnt->Pnt_TempMax);
        mPnt3->ppid_ust.UinDC = load_s32(ramPnt, progPnt->Pnt_UinDC);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPID_PWM_1[9]={
        S32_PIN_TYPE | INPUT_PIN_MODE,   //1  Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,   //2  Ti
        S16_PIN_TYPE | INPUT_PIN_MODE,   //3  Td
        S16_PIN_TYPE | INPUT_PIN_MODE,   //4  DifFiltr
        S32_PIN_TYPE | INPUT_PIN_MODE,   //5  Max
        S32_PIN_TYPE | INPUT_PIN_MODE,   //6  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,   //7  TempMax
        S32_PIN_TYPE | INPUT_PIN_MODE,   //8  UinDC
        BIT_PIN_TYPE | INPUT_PIN_MODE    //9  EN
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PID_PWM", "DA", TblFuncPID_PWM_1, FuncPID_PWM_1, sizeof(TblFuncPID_PWM_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPID_PWM_1,
#endif
//********************************************************
