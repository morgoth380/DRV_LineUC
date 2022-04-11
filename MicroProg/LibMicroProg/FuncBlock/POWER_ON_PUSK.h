#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Enable;                    //1  Enable
    u16  Pnt_Time_S;                    //2  Time_S
    u16  Pnt_Auto_Mode;                 //3  Auto_Mode
    u16  Pnt_StopCmd;                   //4  StopCmd
    u16  Pnt_Pusk;                      //5  Pusk
    u16  Pnt_Warn;                      //6  Warn
    u16  Pnt_End;
}FuncPOWER_ON_PUSK_type; 
//********************************************************
// ќбъ€вление переменных с глобальным временем жизни
typedef struct{
    u16         powerUpDelay;
    u8          AutoPusk_Command;
    u8          blockAutoPusk;
}POWERONPUSK_State_type;

//********************************************************
u16 *FuncPOWER_ON_PUSK_1(FuncPOWER_ON_PUSK_type *progPnt, u32 ramPnt)
{
    POWERONPUSK_State_type		*sPnt;

    sPnt	= (POWERONPUSK_State_type *)(ramPnt + progPnt->Pnt_State);
    
    clrBitMicro(ramPnt, progPnt->Pnt_Warn);
    
    if(flagIsFirstCall){
        sPnt->powerUpDelay = load_s16(ramPnt, progPnt->Pnt_Time_S) * K_TIME_CALL_MICRO_TAKT;
        sPnt->AutoPusk_Command = load_s16(ramPnt, progPnt->Pnt_Enable);
        return &progPnt->Pnt_End;
    }

    // If we have a StopCmd, forbidden start on Power
    if (testBitMicro (ramPnt, progPnt->Pnt_StopCmd)) {
      sPnt->blockAutoPusk = 1;
    }
    
    // ѕуск после включени€ питани€ только 1 раз
    if(!testBitMicro(ramPnt, progPnt->Pnt_Auto_Mode)){
        if(sPnt->blockAutoPusk == 0)  
            sPnt->blockAutoPusk = 1;
        clrBitMicro(ramPnt, progPnt->Pnt_Pusk);
        return &progPnt->Pnt_End;
    } 
    else if(sPnt->blockAutoPusk == 1){
        clrBitMicro(ramPnt, progPnt->Pnt_Pusk);
        return &progPnt->Pnt_End;
    }
    	
    if(sPnt->powerUpDelay){
        sPnt->powerUpDelay--;
        if (sPnt->AutoPusk_Command) {
          // Set AutoStartUp warning, if Mode enabled
          setBitMicro(ramPnt, progPnt->Pnt_Warn);        
        }
    }
    else if(sPnt->AutoPusk_Command){
        setBitMicro(ramPnt, progPnt->Pnt_Pusk);
        sPnt->blockAutoPusk = 1;                                   // выставл€ем флаг "јвтоматичестий пуск".
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPOWER_ON_PUSK_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Enable
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Time_S
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Auto_Mode
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  StopCmd
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Pusk
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Warn
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("POWER_ON_PUSK", "DD", TblFuncPOWER_ON_PUSK_1, FuncPOWER_ON_PUSK_1, sizeof(TblFuncPOWER_ON_PUSK_1), sizeof(POWERONPUSK_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPOWER_ON_PUSK_1,
#endif
//********************************************************
