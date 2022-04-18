#ifdef    GET_FUNC_CODE

typedef enum{
  PRESSURE_NORM = 0,
  PRESS_LOW = 1,
  PRESS_HIGH = 2
}AinPressureEventType;

typedef enum{
  ACTION_OFF = 0,
  WARN_ACTION = 1,
  ERR_ACTION = 2
}endOfCurveCtrlType;

typedef struct{
  u16 delayCnt;
}END_OF_CURVE_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_EndOfCurve;                   //1  EndOfCurve
    u16  Pnt_AinSel;                       //2  AinSel
    u16  Pnt_AinStates[4];                 //3...6  Ain1State...Ain4State
    u16  Pnt_Fmax;                         //7  Fmax
    u16  Pnt_Fmin;                         //8  Fmin
    u16  Pnt_F_Out;                        //9  F_Out
    u16  Pnt_PWM_On;                       //10  PWM_On
    u16  Pnt_Delay;                        //11  Delay
    u16  Pnt_In1;                          //12  In1
    u16  Pnt_In2;                          //13  In2
    u16  Pnt_In3;                          //14  In3
    u16  Pnt_In4;                          //15  In4
    u16  Pnt_Out5;                         //16  Out5
    u16  Pnt_Out4;                         //17  Out4
    u16  Pnt_Out3;                         //18  Out3
    u16  Pnt_Out2;                         //19  Out2
    u16  Pnt_Out1;                         //20  Out1
    u16  Pnt_Warn;                         //21  Warn
    u16  Pnt_Err;                          //22  Err
    u16  Pnt_End;
}FuncEND_OF_CURVE_type;
//********************************************************
u16 *FuncEND_OF_CURVE_1(FuncEND_OF_CURVE_type *progPnt, u32 ramPnt)
{
  endOfCurveCtrlType endOfCurveCtrl;
  u16 AinSel;
  AinPressureEventType AinEventState;
  u16 delay;
  f32 Fmax, Fout;
  END_OF_CURVE_State_type *sPnt;
  
  sPnt  = (END_OF_CURVE_State_type *)(ramPnt + progPnt->Pnt_State);
  endOfCurveCtrl = (endOfCurveCtrlType)load_s16(ramPnt, progPnt->Pnt_EndOfCurve);
  AinSel = load_s16(ramPnt, progPnt->Pnt_AinSel);
  AinEventState = (AinPressureEventType)load_s16(ramPnt, progPnt->Pnt_AinStates[AinSel]);
  Fmax = load_float(ramPnt, progPnt->Pnt_Fmax);
  Fout = load_float(ramPnt, progPnt->Pnt_F_Out);
  delay = load_s16(ramPnt, progPnt->Pnt_Delay);
  
  if(endOfCurveCtrl == ACTION_OFF){
    sPnt->delayCnt = delay * K_TIME_CALL_MICRO_TAKT;
    clrBitMicro(ramPnt, progPnt->Pnt_Err);
    clrBitMicro(ramPnt, progPnt->Pnt_Warn);
    return &progPnt->Pnt_End;
  }
  
  //Condition of event is low pressure and max speed
  if((AinEventState == PRESS_LOW) && (Fout == Fmax)){
    sPnt->delayCnt--;
    if(sPnt->delayCnt == 0){
      if(endOfCurveCtrl == WARN_ACTION){
        setBitMicro(ramPnt, progPnt->Pnt_Warn);
        clrBitMicro(ramPnt, progPnt->Pnt_Err);
      }else{
        setBitMicro(ramPnt, progPnt->Pnt_Err);
        clrBitMicro(ramPnt, progPnt->Pnt_Warn);
      }
    }
  }else{
    sPnt->delayCnt = delay;
    clrBitMicro(ramPnt, progPnt->Pnt_Err);
    clrBitMicro(ramPnt, progPnt->Pnt_Warn);
  }
  
  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncEND_OF_CURVE_1[22]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  EndOfCurve
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  AinSel
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Ain1State
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Ain2State
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Ain3State
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Ain4State
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Fmax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Fmin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  F_Out
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  PWM_On
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Delay
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  In2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  In3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  In4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  Out5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  Out4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  Out3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //19  Out2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //20  Out1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  Warn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //22  Err
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("END_OF_CURVE", "DD", TblFuncEND_OF_CURVE_1, FuncEND_OF_CURVE_1, sizeof(TblFuncEND_OF_CURVE_1), sizeof(END_OF_CURVE_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncEND_OF_CURVE_1,
#endif
//********************************************************
