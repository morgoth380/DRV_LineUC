#ifdef    GET_FUNC_CODE

#define DelayBlinkRun   K_TIME_CALL_MICRO_TAKT     // 500 = 0.5 sec 
#define LedOFF  0
#define LedON   1

typedef struct{
  u16 cntBlink;                 //
  u16 stateLed;                 //         
}FuncLED_RUN_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PWM_On;                    //1  PWM_On
    u16  Pnt_Warning;                   //2  Warning   
    u16  Pnt_TimeToStop;                //3  TimeToStop
    u16  Pnt_Led_RUN;                   //4  Led_RUN
    u16  Pnt_End;
}FuncLED_RUN_type;
//********************************************************
u16 *FuncLED_RUN_1(FuncLED_RUN_type *progPnt, u32 ramPnt)
{
    FuncLED_RUN_State_type *sPnt;
    u8 stateLedRun = 0;
    sPnt = (FuncLED_RUN_State_type *)(ramPnt + progPnt->Pnt_State);

    stateLedRun = testBitMicro(ramPnt, progPnt->Pnt_PWM_On);
    
    if( load_s16(ramPnt, progPnt->Pnt_Warning))
    {
        if(sPnt->cntBlink == 0)
        {
          if (sPnt->stateLed == LedOFF)
              sPnt->stateLed = LedON;
          else 
              sPnt->stateLed = LedOFF;
          
          sPnt->cntBlink = DelayBlinkRun;
        }
        else
            sPnt->cntBlink--;
        stateLedRun = sPnt->stateLed;
    }
    
    defBitMicro(ramPnt, progPnt->Pnt_Led_RUN, stateLedRun);  
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLED_RUN_1[4]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1 PWM_On
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2 Warning   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3 TimeToStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //4 Led_RUN
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LED_RUN", "DD", TblFuncLED_RUN_1, FuncLED_RUN_1, sizeof(TblFuncLED_RUN_1), sizeof(FuncLED_RUN_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLED_RUN_1,
#endif
//********************************************************
