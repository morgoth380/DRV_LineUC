#ifdef    GET_FUNC_CODE

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
    u16  Pnt_OutRef;                       //2  OutRef
    u16  Pnt_FeedbackLev;                  //3  FeedbackLev
    u16  Pnt_FeedbDeviat;                  //4  FeedbDeviat
    u16  Pnt_Fmax;                         //5  Fmax
    u16  Pnt_Fmin;                         //6  Fmin
    u16  Pnt_F_Out;                        //7  F_Out
    u16  Pnt_PWM_On;                       //8  PWM_On
    u16  Pnt_Delay;                        //9  Delay
    u16  Pnt_In1;                          //10  In1
    u16  Pnt_In2;                          //11  In2
    u16  Pnt_Out5;                         //12  Out5
    u16  Pnt_Out4;                         //13  Out4
    u16  Pnt_Out3;                         //14  Out3
    u16  Pnt_Out2;                         //15  Out2
    u16  Pnt_Out1;                         //16  Out1
    u16  Pnt_Warn;                         //17  Warn
    u16  Pnt_Err;                          //18  Err
    u16  Pnt_End;
}FuncEND_OF_CURVE_type;
//********************************************************
u16 *FuncEND_OF_CURVE_1(FuncEND_OF_CURVE_type *progPnt, u32 ramPnt)
{
  endOfCurveCtrlType endOfCurveCtrl;
  u16 delay;
  f32 Fmax, Fout;
  f32 refLev, feedBackLev;
  f32 fBDeviation;
  END_OF_CURVE_State_type *sPnt;
  
  sPnt  = (END_OF_CURVE_State_type *)(ramPnt + progPnt->Pnt_State);
  endOfCurveCtrl = (endOfCurveCtrlType)load_s16(ramPnt, progPnt->Pnt_EndOfCurve);
  Fmax = load_float(ramPnt, progPnt->Pnt_Fmax);
  Fout = load_float(ramPnt, progPnt->Pnt_F_Out);
  delay = load_s16(ramPnt, progPnt->Pnt_Delay);
  refLev = load_float(ramPnt, progPnt->Pnt_OutRef);
  feedBackLev = load_float(ramPnt, progPnt->Pnt_FeedbackLev);
  fBDeviation = load_float(ramPnt, progPnt->Pnt_FeedbDeviat);
  
  if(endOfCurveCtrl == ACTION_OFF){
    sPnt->delayCnt = delay * K_TIME_CALL_MICRO_TAKT;
    clrBitMicro(ramPnt, progPnt->Pnt_Err);
    clrBitMicro(ramPnt, progPnt->Pnt_Warn);
    return &progPnt->Pnt_End;
  }
  
  //Condition of event is low pressure and max speed
  if((feedBackLev < refLev * fBDeviation) && (Fout == Fmax)){
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
    sPnt->delayCnt = delay * K_TIME_CALL_MICRO_TAKT;
    clrBitMicro(ramPnt, progPnt->Pnt_Err);
    clrBitMicro(ramPnt, progPnt->Pnt_Warn);
  }
 
  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncEND_OF_CURVE_1[18]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  EndOfCurve
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  OutRef
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  FeedbackLev
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  FeedbDeviat
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Fmax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Fmin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  F_Out
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  PWM_On
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Delay
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  In2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Out5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  Out4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14  Out3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //15  Out2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  Out1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //17  Warn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //18  Err
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
