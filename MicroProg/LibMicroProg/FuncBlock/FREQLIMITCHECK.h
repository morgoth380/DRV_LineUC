#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Input;                        //1  Input
    u16  Pnt_Limit;                        //2  Limit
    u16  Pnt_Reaction;                     //3  Reaction
    u16  Pnt_Tdisable;                     //4  Tdisable
    u16  Pnt_Tcntrl;                       //5  Tcntrl
    u16  Pnt_ApvEnable;                    //6  ApvEnable
    u16  Pnt_PWM_ON;                       //7  PWM_ON
    u16  Pnt_StatusDrv;                    //8  Status Drv    
    u16  Pnt_Warning;                      //9  Warning
    u16  Pnt_Tcntdown;                     //10  Tcntdown
    u16  Pnt_Alarm;                        //11 Alarm
    u16  Pnt_Fault;                        //12 Fault
    u16  Pnt_End;
}FuncFREQLIMITCHECK_type;

typedef enum{
  DISABLE_STATE = 0,
  WARNING_STATE,
  FAULT_STATE,
}FREQLIMITCHECK_Reaction_type;


typedef struct{
    u16  tmrDelayOff;
    u16  tmrDelayCntr;
    float testFreq;
}FREQLIMITCHECK_State_type;

// Bit assigning from DRV_V5 AlgState
#define   STOP_CMD      1<<0
#define   START_CMD     1<<1
#define   PWM_ON        1<<3 

//********************************************************
u16 *FuncFREQLIMITCHECK_1(FuncFREQLIMITCHECK_type *progPnt, u32 ramPnt)
{
  u16         PwmEn, ApvEn, DelayCntr, DelayOff, ActiveFault, MaxDelayCntr, ActiveWarning, DrvStatus, StopCmd;
  float       Input, Limit;    
  FREQLIMITCHECK_State_type      *sPnt;
  FREQLIMITCHECK_Reaction_type        Reaction;
  
  sPnt = (FREQLIMITCHECK_State_type *)(ramPnt + progPnt->Pnt_State);    
  
  // Загружаем начальные настройки
  PwmEn       = testBitMicro (ramPnt, progPnt->Pnt_PWM_ON);
  ApvEn       = load_s16 (ramPnt, progPnt->Pnt_ApvEnable);
  Reaction    = (FREQLIMITCHECK_Reaction_type)load_s16 (ramPnt, progPnt->Pnt_Reaction);
  Input       = load_float (ramPnt, progPnt->Pnt_Input);
  Limit       = load_float (ramPnt, progPnt->Pnt_Limit);
  DelayOff    = load_s16 (ramPnt, progPnt->Pnt_Tcntrl); // Задержка
  DelayCntr   = load_s16 (ramPnt, progPnt->Pnt_Tdisable); // Разгон
  ActiveFault = ActiveWarning = 0;
  MaxDelayCntr = DelayOff * K_TIME_CALL_MICRO_TAKT;
  DrvStatus   = load_s16 (ramPnt, progPnt->Pnt_StatusDrv); // Статус ПЧ
  
  StopCmd = (DrvStatus & STOP_CMD);
  
  // ШИМ выключен - заряжаем таймеры
  if (!PwmEn)  { 
    sPnt->tmrDelayOff = DelayOff * K_TIME_CALL_MICRO_TAKT; // Задержка
    sPnt->tmrDelayCntr = DelayCntr * K_TIME_CALL_MICRO_TAKT; // Разгон
  }
  
  if (ApvEn) {
    save_s16 (ramPnt, progPnt->Pnt_Reaction, FAULT_STATE);
  }
  // Предтаймер задержки
  if (sPnt->tmrDelayCntr)     sPnt->tmrDelayCntr--; // Разгон
    
  // Проверка собственно аварии
  if (PwmEn && !StopCmd) {
    if (!sPnt->tmrDelayCntr) { // Ждем таймаут разгона при старте       
      if (Input > Limit) { // Есть превышение?
        ActiveWarning = 1;
        if (sPnt->tmrDelayOff)      sPnt->tmrDelayOff --; // Задержка
        else                        ActiveFault = 1;
      } else {
        if (sPnt->tmrDelayOff < MaxDelayCntr)      sPnt->tmrDelayOff ++;
      }
    }      
  }

  // Начальные настройки
  clrBitMicro(ramPnt, progPnt->Pnt_Alarm); 
  clrBitMicro(ramPnt, progPnt->Pnt_Warning); 
  clrBitMicro(ramPnt, progPnt->Pnt_Fault); 
  save_s16(ramPnt, progPnt->Pnt_Tcntdown, 0);
  
  // Определяем тип реакции на событие
  switch (Reaction){
  case DISABLE_STATE: 
    break;
    
  case WARNING_STATE:
    if (ActiveFault) { // Если есть авария - выдаем предупреждение
      setBitMicro(ramPnt, progPnt->Pnt_Alarm);  // даем предупреждения
    }
    break;
    
  case FAULT_STATE:
    // Если активное предупреждение
    if (ActiveWarning) {
      save_s16 (ramPnt, progPnt->Pnt_Tcntdown, (u16)(sPnt->tmrDelayOff/K_TIME_CALL_MICRO_TAKT)); // Время до аварии
      setBitMicro (ramPnt, progPnt->Pnt_Warning); // Предупреждение в блок отсчета до аварии
    }
    
    if (ActiveFault) { // Если авария активна - выдаем ее
      setBitMicro(ramPnt, progPnt->Pnt_Fault);   //выдаем сигнал аварии
    }
    break;
        
  default:
    break;
    
  }
    
  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFREQLIMITCHECK_1[12]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Input
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Limit
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Reaction
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Tdisable
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Tcntrl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  ApvEnable
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  PWM_ON
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Status Drv
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Warning
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Tcntdown
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Alarm
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Fault
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FREQLIMITCHECK", "U", TblFuncFREQLIMITCHECK_1, FuncFREQLIMITCHECK_1, sizeof(TblFuncFREQLIMITCHECK_1), sizeof (FREQLIMITCHECK_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFREQLIMITCHECK_1,
#endif
//********************************************************
