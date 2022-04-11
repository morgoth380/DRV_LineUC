#ifdef    GET_FUNC_CODE

#define TimeOutCharge   K_TIME_CALL_MICRO_TAKT
#define TimeChangeLed   K_TIME_CALL_MICRO_TAKT / 2

#define DelayWarning_LedFailON          K_TIME_CALL_MICRO_TAKT / 8    // 125 ms
#define DelayWarning_LedFailOFF         K_TIME_CALL_MICRO_TAKT * 2    // 2 sec

#define  LedFailOFF     0
#define  LedFailON      1

typedef struct{
    u8  stateLED;               // состояния светодиодов( какой светодиод необходимо "зажигать" )
    u8  cntChangeLed;           // счетчик смены свечения светодиода 
    u16 delayTime;              // задержка на включения "Мигания" светодиодов
    
    u16 cntBlink_LedFail;       // счетчик смены состояния светодиода FAIL
    u16 state_LedFail;          // состояние светодиода FAIL
    u16 LedTestTime;            // Таймер проверки индикации
}StateLED_CONTROL_type;

typedef enum{
    LED_Fail_Work = 0,      
    LED_Ready_Work,      
    LED_Run_Work
}lampWork_enum;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Run_Status         ;       //1  Run_Status
    u16  Pnt_Ready_Status       ;       //2  Ready_Status
    u16  Pnt_Fail_Status        ;       //3  Fail_Status
    u16  Pnt_WiFi_Status        ;       //4  WiFi_Status
    u16  Pnt_Charge_Status      ;       //5  Charge_Status
    u16  Pnt_WarningBit         ;       //6  Warning
    u16  Pnt_Led_Test           ;       //7  LEd_test
    u16  Pnt_LED_Run            ;       //8  LED_Run
    u16  Pnt_LED_Ready          ;       //9  LED_Ready
    u16  Pnt_LED_Fail           ;       //10  LED_Fail
    u16  Pnt_LED_WiFi           ;       //11 LED_WiFi
    u16  Pnt_End;
}FuncLED_CONTROL_type;
//********************************************************
u16 *FuncLED_CONTROL_1(FuncLED_CONTROL_type *progPnt, u32 ramPnt)
{
    u8 led_Run, led_Ready, led_Fail, led_WiFi, warningBit, chargeStat, LedTest;
    StateLED_CONTROL_type *sPnt;
    led_Run = led_Ready = led_Fail = warningBit = chargeStat = 0;
    
    sPnt = (StateLED_CONTROL_type *)(ramPnt + progPnt->Pnt_State);
    
    warningBit  =  testBitMicro(ramPnt, progPnt->Pnt_WarningBit);
    led_WiFi    =  testBitMicro(ramPnt, progPnt->Pnt_WiFi_Status);
    chargeStat = testBitMicro(ramPnt, progPnt->Pnt_Charge_Status);
    LedTest = load_s16(ramPnt, progPnt->Pnt_Led_Test);
    
    if (sPnt->LedTestTime)      sPnt->LedTestTime --;
    
    if(chargeStat)                                                      // если статус заряд, то "бегущая строка"
    {   
        if(sPnt->delayTime < TimeOutCharge)
            sPnt->delayTime++;
        else
        {
            switch (sPnt->stateLED)
            {
                case LED_Run_Work:
                    led_Run = 1;
                    break;
                case LED_Ready_Work:
                    led_Ready = 1;
                    break;
                case LED_Fail_Work:
                    led_Fail = 1;
                    break;
            }
            sPnt->cntChangeLed++;
            if(sPnt->cntChangeLed >= TimeChangeLed)
            {
                if (sPnt->stateLED == LED_Run_Work)
                    sPnt->stateLED =  LED_Fail_Work;
                else
                    sPnt->stateLED++;
                
                sPnt->cntChangeLed = 0;
            }  
        }
    }
    else
    {
        led_Run         = testBitMicro(ramPnt, progPnt->Pnt_Run_Status);
        led_Ready       = testBitMicro(ramPnt, progPnt->Pnt_Ready_Status);
        led_Fail        = testBitMicro(ramPnt, progPnt->Pnt_Fail_Status);
        
        sPnt->delayTime = sPnt->cntChangeLed = sPnt->stateLED = 0;
    }
    if((!led_Fail) && warningBit && (!chargeStat))        // нет аварии, но есть предупр!
    {
        switch(sPnt->state_LedFail)
        {
            case LedFailOFF:
                led_Fail = LedFailOFF;
                if(sPnt->cntBlink_LedFail <= 0)
                {
                    sPnt->cntBlink_LedFail = DelayWarning_LedFailON;
                    sPnt->state_LedFail = LedFailON;
                }
            break;
            
            case LedFailON:
                led_Fail = LedFailON;
                if(sPnt->cntBlink_LedFail <= 0)
                {
                    sPnt->cntBlink_LedFail = DelayWarning_LedFailOFF;
                    sPnt->state_LedFail = LedFailOFF;
                }
            break;
        }
        sPnt->cntBlink_LedFail--;
    }
    
    if((!GlobalM4.FlagCommand.bit.PwmOn) && (led_Run))
      led_Run = 1;
    
    if (!sPnt->LedTestTime && LedTest) {
      sPnt->LedTestTime = 5*K_TIME_CALL_MICRO_TAKT; // Заряжаем таймер
    } else if (sPnt->LedTestTime && LedTest) {      
      if (sPnt->LedTestTime == 1)   save_s16(ramPnt, progPnt->Pnt_Led_Test, 0);
      
      if (sPnt->LedTestTime & (1<<7))
          led_Run = led_Ready = led_Fail = led_WiFi = 1;
      else
          led_Run = led_Ready = led_Fail = led_WiFi = 0;
    }
    
    defBitMicro(ramPnt, progPnt->Pnt_LED_Run    , led_Run       );
    defBitMicro(ramPnt, progPnt->Pnt_LED_Ready  , led_Ready     );
    defBitMicro(ramPnt, progPnt->Pnt_LED_Fail   , led_Fail      );
    defBitMicro(ramPnt, progPnt->Pnt_LED_WiFi   , led_WiFi      );

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLED_CONTROL_1[11]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Run_Status
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Ready_Status
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Fail_Status
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  WiFi_Status
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Charge_Status
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Warning
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  LedTest
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  LED_Run
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  LED_Ready
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  LED_Fail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //11 LED_WiFi
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LED_CONTROL", "DD", TblFuncLED_CONTROL_1, FuncLED_CONTROL_1, sizeof(TblFuncLED_CONTROL_1), sizeof(StateLED_CONTROL_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLED_CONTROL_1,
#endif
//********************************************************
