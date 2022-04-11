#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PwmOn;                        //1  PwmOn
    u16  Pnt_Fref;                         //2  Fref
    u16  Pnt_Ifull;                        //3  Ifull
    u16  Pnt_vPhirUp;                      //4  vPhirUp
    u16  Pnt_FbrkOn;                       //5  FbrkOn
    u16  Pnt_FbrkOff;                      //6  FbrkOff
    u16  Pnt_IfullBrkOn;                   //7  IfullBrkOn
    u16  Pnt_TdelayBrkOn;                  //8  TdelayBrkOn
    u16  Pnt_TdelayBrkOff;                 //9  TdelayBrkOff
    u16  Pnt_BrkFdbEnable;                 //10  BrkFdbEnable
    u16  Pnt_TimeFdb;                      //11  TimeFdb
    u16  Pnt_StatusDrv;                    //12  StatusDrv
    u16  Pnt_Reserved2;                    //13  Reserved2
    u16  Pnt_Reserved3;                    //14  Reserved3
    u16  Pnt_BrakeFault;                   //15  BrakeFault
    u16  Pnt_BrakeSignal;                  //16  BrakeSignal
    u16  Pnt_End;
}FuncBRAKE_CNTRL_type;
//********************************************************
typedef enum{
    BRAKE_OFF                   = 0,   // Brake is off
    BRAKE_OFF_CHECK_F_I         = 1,   // Brake is off, check Freq & Current
    BRAKE_OFF_TIME_DELAY        = 2,   // Brake still is off, wait delay time 
    BRAKE_ON                    = 3,   // Fine! Brake ON. GO_GO_GO
    BRAKE_ON_CHECK_F            = 4,   // Wait freq for brake off
    BRAKE_ON_TIME_DELAY         = 5,   // Delay brake off
    BRAKE_WAIT_STOP             = 6,   // Wait stop VFD
}BRAKE_State_type;
//********************************************************
typedef struct{
  BRAKE_State_type   BrakeState;  
  u16   timerOff;
  u16   timerOn;
}BRAKE_CNTRLState_type;

// Bit assigning from DRV_V5 AlgState
#define   STOP_CMD      1<<0
#define   START_CMD     1<<1
#define   PWM_ON        1<<3 

//********************************************************
u16 *FuncBRAKE_CNTRL_1(FuncBRAKE_CNTRL_type *progPnt, u32 ramPnt)
{
    float    Fref, Ifull, Inom;
    float    FbrkOn, FbrkOff, IfullBrkOn, TdelayBrkOn, TdelayBrkOff;
    u16      PwmOn, vPhirUp, DrvStatus, StopCmd;
    //u16      BrakeSignal;
    BRAKE_CNTRLState_type *sPnt;
    sPnt = (BRAKE_CNTRLState_type *)(ramPnt + progPnt->Pnt_State);
    
    if (flagIsFirstCall) {
        sPnt->BrakeState = BRAKE_OFF;
        clrBitMicro (ramPnt, progPnt->Pnt_BrakeSignal);        
        return &progPnt->Pnt_End;  
    }
        
    DrvStatus   = load_s16(ramPnt, progPnt->Pnt_StatusDrv);
    PwmOn       = testBitMicro (ramPnt, progPnt->Pnt_PwmOn);
    Inom        = GlobalM4.EnginParam.Isn;
    Fref        = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Fref), Herz_base); // Convert to the Herz unit
    Ifull       = load_float(ramPnt, progPnt->Pnt_Ifull);   
    Ifull       = IQ_to_FLOAT(Ifull, Current_base);
    vPhirUp     = testBitMicro (ramPnt, progPnt->Pnt_vPhirUp);
    FbrkOn      = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_FbrkOn), Herz_base); // Convert to the Herz unit
    FbrkOff     = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_FbrkOff), Herz_base); // Convert to the Herz unit 
    IfullBrkOn  = Inom * (f32)(load_s16(ramPnt, progPnt->Pnt_IfullBrkOn) * 0.1f * 0.01f);    
    TdelayBrkOn = (f32)(load_s16(ramPnt, progPnt->Pnt_TdelayBrkOn) * 0.01f);    
    TdelayBrkOff= (f32)(load_s16(ramPnt, progPnt->Pnt_TdelayBrkOff) * 0.01f);    

    StopCmd = (DrvStatus & STOP_CMD);
    if (sPnt->timerOn)          sPnt->timerOn--;
    if (sPnt->timerOff)         sPnt->timerOff--;

    switch (sPnt->BrakeState) {      
        case BRAKE_OFF:
          clrBitMicro (ramPnt, progPnt->Pnt_BrakeSignal);
          if (vPhirUp && PwmOn) { // If Pwm is on & vPhirUp is OK - go to the next state
              sPnt->BrakeState = BRAKE_OFF_CHECK_F_I;
          }
          break;
          
        case BRAKE_OFF_CHECK_F_I:
          if (Ifull > IfullBrkOn && fabsf(Fref) > FbrkOn) { // Is full current over IfullBrkOn & Fref drive over than FbrkOn - we are ready to BRAKE_ON
              sPnt->BrakeState = BRAKE_OFF_TIME_DELAY; 
              sPnt->timerOn = (u16)(TdelayBrkOn * K_TIME_CALL_MICRO_TAKT); // Reload timeout timer
          }
          break;
          
        case BRAKE_OFF_TIME_DELAY:
          if (sPnt->timerOn == 0) { // Wait timer count down
             setBitMicro (ramPnt, progPnt->Pnt_BrakeSignal); // Set Brake 
             sPnt->BrakeState = BRAKE_ON_CHECK_F; // Next step 
          }         
          break;
          
        case BRAKE_ON_CHECK_F: // Brake is on. Wait off state
          if (fabsf(Fref) <= FbrkOff && StopCmd) { // We need check the drive status and wait stop command
              sPnt->BrakeState = BRAKE_ON_TIME_DELAY; 
              sPnt->timerOff = (u16)(TdelayBrkOff * K_TIME_CALL_MICRO_TAKT); // Reload timeout timer            
          }          
          break; 
          
        case BRAKE_ON_TIME_DELAY:
          if (sPnt->timerOff == 0) { // Wait stop count timer and Pwm Off            
             clrBitMicro (ramPnt, progPnt->Pnt_BrakeSignal);
             sPnt->BrakeState = BRAKE_WAIT_STOP; 
          }          
          break;
          
        case BRAKE_WAIT_STOP:
            if (PwmOn == 0 ) sPnt->BrakeState = BRAKE_OFF; 
            break;
            
        default:          
          sPnt->BrakeState = BRAKE_OFF;        
          break;      
    }
    
    GlobalM4.BrakeState = testBitMicro(ramPnt, progPnt->Pnt_BrakeSignal);
    GlobalM4.BrakeAlgStatus = sPnt->BrakeState;
        
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBRAKE_CNTRL_1[16]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  PwmOn
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Fref
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Ifull
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  vPhirUp
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  FbrkOn
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  FbrkOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  IfullBrkOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  TdelayBrkOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  TdelayBrkOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  BrkFdbEnable
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  TimeFdb
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  StatusDrv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Reserved2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Reserved3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //15  BrakeFault
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //16  BrakeSignal
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("BRAKE_CNTRL", "DD", TblFuncBRAKE_CNTRL_1, FuncBRAKE_CNTRL_1, sizeof(TblFuncBRAKE_CNTRL_1), sizeof(BRAKE_CNTRLState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBRAKE_CNTRL_1,
#endif
//********************************************************
