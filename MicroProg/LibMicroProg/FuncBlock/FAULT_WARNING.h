#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Warning_In[10];                    //1   Warning
    u16  Pnt_TimeToStop_In[10];                 //11  TimeToStop
    u16  Pnt_PWM_ON;                            //21
    u16  Pnt_Warning;                           //22  Warning
    u16  Pnt_TimeToStop;                        //23  TimeToStop
    u16  Pnt_End;
}FuncFAULT_WARNING_type;
//********************************************************
u16 *FuncFAULT_WARNING_1(FuncFAULT_WARNING_type *progPnt, u32 ramPnt)
{
    u8 cnt, tekWarning;
    u16 tmpTimeToStop, timeToStop;
    tmpTimeToStop = timeToStop = tekWarning = 0;
    if(testBitMicro(ramPnt, progPnt->Pnt_PWM_ON))
    {
        for(cnt  = 0; cnt < 10; cnt ++)
        {
            if(testBitMicro(ramPnt, progPnt->Pnt_Warning_In[cnt]))
            {
                tmpTimeToStop = load_s16(ramPnt, progPnt->Pnt_TimeToStop_In[cnt]);
                if((tmpTimeToStop != 0)&&((tmpTimeToStop < timeToStop) || (timeToStop == 0)))
                {
                    timeToStop = tmpTimeToStop;
                    tekWarning = cnt+1;
                }
            }
        }
    }
    save_s16(ramPnt, progPnt->Pnt_TimeToStop, timeToStop);
    save_s16(ramPnt, progPnt->Pnt_Warning, tekWarning);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFAULT_WARNING_1[23]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Warning_Ain1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Warning_Ain2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Warning_Nedogr
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Warning_Peregr
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Warning_Fmin
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Warning_Fmax
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Warning_NoWater
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Warning_Add1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Warning_Add2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Warning_Add3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  TimeToStop_Ain1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  TimeToStop_Ain2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  TimeToStop_Nedogr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  TimeToStop_Peregr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  TimeToStop_Fmin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  TimeToStop_Fmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  TimeToStop_NoWater
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  TimeToStop_Add1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  TimeToStop_Add2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  TimeToStop_Add3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //21  PWM_ON
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Warning
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //23  TimeToStop
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FAULT_WARNING", "DD", TblFuncFAULT_WARNING_1, FuncFAULT_WARNING_1, sizeof(TblFuncFAULT_WARNING_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFAULT_WARNING_1,
#endif
//********************************************************
