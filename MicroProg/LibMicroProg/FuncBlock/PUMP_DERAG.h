#ifdef    GET_FUNC_CODE

#define PULSE_DURATION 0.2F 

typedef enum{
  COMMAND_WAITING = 0,
  COMMAND_HOLD = 1
}commandStateType;

typedef enum{
  PWM_OFF = 0,
  ACCEL_DONE_WAITING = 1,
  DERAG_FWD_RUN = 2,
  STOP_WAITING = 3,
  DERAG_OFF_DELAY = 4,
  REVERSE_ACCEL_DONE_WAITING = 5,
  DERAG_REVERSE_RUN = 6,
  REV_STOP_WAITING = 7,
  DERAG_REV_OFF_DELAY = 8,
  DERAG_WAIT_FROM_WORK = 9,
  START_DELAY = 10, 
  DERAG_PWM_ON_WAIT = 11,
  WORK_STATE = 12,
  STOP_DONE_WAIT = 13
}deragStateType;

typedef enum{
  DERAG_BEFORE_WORK = 0,
  DERAG_AFTER_WORK = 1
}currentDeragModeType;

typedef enum{
  POWER_EVENT_WAITING = 0,
  POWER_EVENT_COUNT = 1,
  POWER_ERR_HOLD = 2
}powerDeragErrStateType;


typedef struct{
  u32 deragRunCnt;
  u32 deragOffCnt;
  commandStateType stopState;
  commandStateType startState;
  u16 stopHoldCnt;
  u16 startHoldCnt;
  u16 cyclesCnt;
  deragStateType deragState;
  u16 startAfterDeragDis;
  currentDeragModeType currentDeragMode;
  u32 powerIntervalCnt; //Счетчик интервала подсчета процедур очистки по мощности
  powerDeragErrStateType powerDeragErrState;
  u16 powerDeragCnt; //Счетчик процедур очистки по превышению мощности
  u16 powerDeragHoldCnt;
  u32 intervalStrg[10]; //хранилище интервалов между событиями очистки
  u16 i;
}PUMP_DERAG_StateType;

typedef enum{
  DERAG_OFF = 0,
  DERAG_ON_START = 1,
  DERAG_ON_STOP = 2,
  DERAG_ON_START_STOP = 3,
  DERAG_ON_DIN = 4,
  DERAG_ON_POWER = 5,
}deragModeType;

typedef enum{
  DONT_PERFORM_DERAG = 0,
  PERFORM_DERAG = 1
}stopSignalActionType;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_DeraggFunc;                   //1  DeraggFunc
    u16  Pnt_CyclesNum;                    //2  CyclesNum
    u16  Pnt_OffDelay;                     //3  OffDelay
    u16  Pnt_RunTime;                      //4  RunTime
    u16  Pnt_DeraggSpd;                    //5  DeraggSpd
    u16  Pnt_P1;                           //6  P1
    u16  Pnt_P2;                           //7  P2
    u16  Pnt_P3;                           //8  P3
    u16  Pnt_P4;                           //9  P4
    u16  Pnt_P5;                           //10  P5
    u16  Pnt_PowerOffset;                  //11  PowerOffset
    u16  Pnt_DeragNum;                     //12  DeragNum
    u16  Pnt_PowDeragDelay;                //13  PowDeragDelay
    u16  Pnt_ConsecTime;                   //14  ConsecTime
    u16  Pnt_CounterReset;                 //15  CounterReset
    u16  Pnt_ActualPower;                  //16  ActualPower
    u16  Pnt_In2;                          //17  In2
    u16  Pnt_In3;                          //18  In3
    u16  Pnt_In4;                          //19  In4
    u16  Pnt_Fmin;                         //20  Fmin
    u16  Pnt_Fmax;                         //21  Fmax
    u16  Pnt_PWM_On;                       //22  PWM_On
    u16  Pnt_GT_Up;                        //23  GT_Up
    u16  Pnt_GT_Down;                      //24  GT_Down
    u16  Pnt_F_Out;                        //25  F_Out
    u16  Pnt_F_Ref;                        //26  F_Ref
    u16  Pnt_ModeWhileStop;                //27  ModeWhileStop
    u16  Pnt_GlobalStop;                   //28  GlobalStop
    u16  Pnt_DrvFail;                      //29  DrvFail
    u16  Pnt_In5;                          //30  In5
    u16  Pnt_In6;                          //31  In6
    u16  Pnt_In7;                          //32  In7
    u16  Pnt_Functional;                   //33  Functional
    u16  Pnt_DeraggFlg;                    //34  DeraggFlg
    u16  Pnt_DeraggRef;                    //35  DeraggRef
    u16  Pnt_Err;                          //36  Err
    u16  Pnt_Warn;                         //37  Warn
    u16  Pnt_StartCmd;                     //38  StartCmd
    u16  Pnt_StopCmd;                      //39  StopCmd
    u16  Pnt_DeragPower;                   //40  DeragPower
    u16  Pnt_Out1;                         //41  Out1
    u16  Pnt_Out2;                         //42  Out2
    u16  Pnt_Out3;                         //43  Out3
    u16  Pnt_Out4;                         //44  Out4
    u16  Pnt_Out5;                         //45  Out5
    u16  Pnt_End;
}FuncPUMP_DERAG_type;
//********************************************************
void deragByStartStop(PUMP_DERAG_StateType *sPnt, FuncPUMP_DERAG_type *progPnt, u32 ramPnt);
void deragByDin(PUMP_DERAG_StateType *sPnt, FuncPUMP_DERAG_type *progPnt, u32 ramPnt);
void deragByPower(PUMP_DERAG_StateType *sPnt, FuncPUMP_DERAG_type *progPnt, u32 ramPnt);
u16 powerDeragErrCheck(u16 powerDeragNum, u16 powerInterval, PUMP_DERAG_StateType *sPnt);

u16 *FuncPUMP_DERAG_1(FuncPUMP_DERAG_type *progPnt, u32 ramPnt)
{
  deragModeType derugMode;
  u16 functional;
  PUMP_DERAG_StateType *sPnt;
  sPnt = (PUMP_DERAG_StateType *)(ramPnt + progPnt->Pnt_State);
  
  if(flagIsFirstCall){
    sPnt->deragRunCnt = 0;
    sPnt->deragOffCnt = 0;
    sPnt->startState = COMMAND_WAITING;
    sPnt->stopState = COMMAND_WAITING;
    sPnt->stopHoldCnt = 0;
    sPnt->startHoldCnt = 0;
    sPnt->cyclesCnt = 0;
    sPnt->deragState = PWM_OFF;
    sPnt->startAfterDeragDis = 0;
    sPnt->currentDeragMode = DERAG_BEFORE_WORK;
    sPnt->powerDeragErrState = POWER_EVENT_WAITING;
    return &progPnt->Pnt_End;
  }
  
  functional = load_s16(ramPnt, progPnt->Pnt_Functional);
  if(functional != UsePumpFunc){
    sPnt->deragRunCnt = 0;
    sPnt->deragOffCnt = 0;
    sPnt->startState = COMMAND_WAITING;
    sPnt->stopState = COMMAND_WAITING;
    sPnt->stopHoldCnt = 0;
    sPnt->startHoldCnt = 0;
    sPnt->cyclesCnt = 0;
    sPnt->deragState = PWM_OFF;
    sPnt->startAfterDeragDis = 0;
    sPnt->currentDeragMode = DERAG_BEFORE_WORK;
    clrBitMicro(ramPnt, progPnt->Pnt_StartCmd);
    clrBitMicro(ramPnt, progPnt->Pnt_StopCmd);
    clrBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);
    clrBitMicro(ramPnt, progPnt->Pnt_Err);
    save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
    save_float(ramPnt, progPnt->Pnt_DeragPower, 0.0F);
    return &progPnt->Pnt_End;
  }
  
  derugMode = (deragModeType)load_s16(ramPnt, progPnt->Pnt_DeraggFunc);  
     
  if(derugMode == DERAG_OFF){
    sPnt->deragRunCnt = 0;
    sPnt->deragOffCnt = 0;
    sPnt->startState = COMMAND_WAITING;
    sPnt->stopState = COMMAND_WAITING;
    sPnt->stopHoldCnt = 0;
    sPnt->startHoldCnt = 0;
    sPnt->cyclesCnt = 0;
    sPnt->deragState = PWM_OFF;
    sPnt->startAfterDeragDis = 0;
    sPnt->currentDeragMode = DERAG_BEFORE_WORK;
    clrBitMicro(ramPnt, progPnt->Pnt_StartCmd);
    clrBitMicro(ramPnt, progPnt->Pnt_StopCmd);
    clrBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);
    clrBitMicro(ramPnt, progPnt->Pnt_Err);
    save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
    save_float(ramPnt, progPnt->Pnt_DeragPower, 0.0F);
    return &progPnt->Pnt_End;
  }
   
  if((derugMode == DERAG_ON_START) || (derugMode == DERAG_ON_START_STOP) || (derugMode == DERAG_ON_STOP)){
    deragByStartStop(sPnt, progPnt, ramPnt);
  }else if(derugMode == DERAG_ON_DIN){
    deragByDin(sPnt, progPnt, ramPnt);
  }else{
    deragByPower(sPnt, progPnt, ramPnt);
  } 
  return &progPnt->Pnt_End;
};


//Управление очисткой при пуске и стопе
void deragByStartStop(PUMP_DERAG_StateType *sPnt, FuncPUMP_DERAG_type *progPnt, u32 ramPnt)
{
  u16 PWM_state;
  u16 stopCmd, startCmd;
  deragModeType derugMode;
  f32 deragRef;
  f32 actualSpd;
  u16 deragOffTime;
  u16 cyclesNum;
  u16 DinSignal;
  u16 stopSignal;
  stopSignalActionType stopSignalAction;
  u16 drvFail;
  u16 deragRunTime;
  
  derugMode = (deragModeType)load_s16(ramPnt, progPnt->Pnt_DeraggFunc);
  PWM_state = testBitMicro(ramPnt, progPnt->Pnt_PWM_On);
  deragRef = load_float(ramPnt, progPnt->Pnt_DeraggSpd);
  deragRunTime = load_s16(ramPnt, progPnt->Pnt_RunTime);
  actualSpd = load_float(ramPnt, progPnt->Pnt_F_Out);
  deragOffTime = load_s16(ramPnt, progPnt->Pnt_OffDelay);
  cyclesNum = load_s16(ramPnt, progPnt->Pnt_CyclesNum);
  drvFail = testBitMicro(ramPnt, progPnt->Pnt_DrvFail);
  stopSignal = testBitMicro(ramPnt, progPnt->Pnt_GlobalStop);
  stopSignalAction = (stopSignalActionType)load_s16(ramPnt, progPnt->Pnt_ModeWhileStop);
  DinSignal = load_s16(ramPnt, progPnt->Pnt_In5);  
  stopCmd = startCmd = 0;
  
  switch(sPnt->deragState){
  case PWM_OFF: //Исходное состояние. ШИМ выключен
    if(PWM_state == 1){
      if((derugMode == DERAG_ON_START) || (derugMode == DERAG_ON_START_STOP)){
        //Переключаемся на задание очистки 
        setBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);        
        save_float(ramPnt, progPnt->Pnt_DeraggRef, deragRef);
        //Заряжаем счетчик циклов очистки 
        sPnt->cyclesCnt = cyclesNum;  
        //Переходим в ожидание выхода на скорость очистки
        sPnt->deragState = ACCEL_DONE_WAITING;
        //Уточняем текущий процесс - очистка перед пуском
        sPnt->currentDeragMode = DERAG_BEFORE_WORK;
      }else if(derugMode == DERAG_ON_STOP){
        sPnt->deragState = WORK_STATE;
      }
    }
    break;
  case ACCEL_DONE_WAITING: //1 - Ожидание завершения разгона в прямом направлении
    if(actualSpd == deragRef){
      sPnt->deragRunCnt = deragRunTime * K_TIME_CALL_MICRO_TAKT; //Счетчик времени работы на скорости очистки
      sPnt->deragState = DERAG_FWD_RUN;
    }
    break;
  case DERAG_FWD_RUN: //2 - Состояние работы на скорости очистки
    sPnt->deragRunCnt--;
    if(sPnt->deragRunCnt == 0){ //Время работы выдержано
      save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
      stopCmd = 1; 
      sPnt->deragState = STOP_WAITING; 
    }
    break;
  case STOP_WAITING: //3 - Ожидание останова
    if(actualSpd != 0){ 
      break;
    }
    if(sPnt->currentDeragMode == DERAG_BEFORE_WORK){ 
      sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT;
      sPnt->deragState = DERAG_OFF_DELAY; //Переход к паузе перед реверсом
    }else{ 
      if(sPnt->cyclesCnt != 0){
        sPnt->cyclesCnt--;
        sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT;
        sPnt->deragState = DERAG_OFF_DELAY;
      }else{
        sPnt->deragState = PWM_OFF;
      }
    }
    break;
  case DERAG_OFF_DELAY: //4 - Пауза перед реверсом
    sPnt->deragOffCnt--;
    if(sPnt->deragOffCnt == 0){ //Пауза выдержана
      save_float(ramPnt, progPnt->Pnt_DeraggRef, -deragRef);
      startCmd = 1;
      //Переходим к ожиданию выхода на реверсивное задание
      sPnt->deragState = REVERSE_ACCEL_DONE_WAITING; 
    }
    break;
  case REVERSE_ACCEL_DONE_WAITING: //5 - Ожидание разгона в реверсивном направлении
    if(actualSpd == -deragRef){              
      sPnt->deragRunCnt = deragRunTime * K_TIME_CALL_MICRO_TAKT;
      sPnt->deragState = DERAG_REVERSE_RUN; 
    }
    break;
  case DERAG_REVERSE_RUN: //6 - Работа на реверсивном задании
    sPnt->deragRunCnt--;
    if(sPnt->deragRunCnt == 0){
      //Даем команду стоп и переходим к ожиданию останова
      stopCmd = 1;
      save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
      sPnt->deragState = REV_STOP_WAITING; 
    }
    break;
  case REV_STOP_WAITING: //7 - Ожидание останова после реверсивного движения
    if(actualSpd == 0.0F){ 
      sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT;
      sPnt->deragState = DERAG_REV_OFF_DELAY;
    }
    break;
  case DERAG_REV_OFF_DELAY: //8 -
    sPnt->deragOffCnt--;
    if(sPnt->deragOffCnt != 0){
      break;
    }
    if(sPnt->currentDeragMode == DERAG_BEFORE_WORK){
      sPnt->cyclesCnt--; //Учитываем пройденный цикл
      if(sPnt->cyclesCnt != 0){ 
        startCmd = 1;
        save_float(ramPnt, progPnt->Pnt_DeraggRef, deragRef);
        sPnt->deragState = ACCEL_DONE_WAITING;
      }else{ //Все циклы выполнены 
        clrBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);
        save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
        startCmd = 1;
        sPnt->deragState = DERAG_PWM_ON_WAIT; 
      }
    }else{
      startCmd = 1;
      save_float(ramPnt, progPnt->Pnt_DeraggRef, deragRef);
      sPnt->deragState = ACCEL_DONE_WAITING;
    }
    break;
  case WORK_STATE: 
    if(PWM_state == 1){
      break;
    }
    //ШИМ выключен
    if(derugMode == DERAG_ON_START){
      clrBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);
      sPnt->deragState = PWM_OFF;
    }else if((derugMode == DERAG_ON_STOP)||(derugMode == DERAG_ON_START_STOP)){ 
      sPnt->cyclesCnt = cyclesNum;
      //Уточняем текущий процесс: очистка при останове
      sPnt->currentDeragMode = DERAG_AFTER_WORK;
      sPnt->deragState = STOP_WAITING;
    }
    break;
  case DERAG_PWM_ON_WAIT:
    if(PWM_state == 1){
      sPnt->deragState = WORK_STATE;
    }
    if(drvFail == 1){
      sPnt->deragState = PWM_OFF;
    }
    break;
  }
  
  switch(sPnt->stopState){
  case COMMAND_WAITING:
    if(stopCmd == 1){
      setBitMicro(ramPnt, progPnt->Pnt_StopCmd);
      sPnt->stopHoldCnt = (u16)(PULSE_DURATION * K_TIME_CALL_MICRO_TAKT + 0.5F);
      sPnt->stopState = COMMAND_HOLD;
    }
    break;
  case COMMAND_HOLD:
    if(sPnt->stopHoldCnt == 0){
      clrBitMicro(ramPnt, progPnt->Pnt_StopCmd);
      sPnt->stopState = COMMAND_WAITING;
    }else{
      sPnt->stopHoldCnt--;
    }
    break;
  }
  
  
  switch(sPnt->startState){
  case COMMAND_WAITING:
    if(startCmd == 1){
      setBitMicro(ramPnt, progPnt->Pnt_StartCmd);
      sPnt->startHoldCnt = (u16)(PULSE_DURATION * K_TIME_CALL_MICRO_TAKT + 0.5F);
      sPnt->startState = COMMAND_HOLD;
    }
    break;
  case COMMAND_HOLD:
    if(sPnt->startHoldCnt == 0){
      clrBitMicro(ramPnt, progPnt->Pnt_StartCmd);
      sPnt->startState = COMMAND_WAITING;
    }else{
      sPnt->startHoldCnt--;
    }
    break;
  }

}

//Управление очисткой по команде от дискретного входа
void deragByDin(PUMP_DERAG_StateType *sPnt, FuncPUMP_DERAG_type *progPnt, u32 ramPnt)
{
  u16 PWM_state;
  u16 stopCmd, startCmd;
  deragModeType derugMode;
  f32 deragRef;
  f32 actualSpd;
  u16 accelFlg, decelFlg;
  u16 deragOffTime;
  u16 cyclesNum;
  u16 DinSignal;
  u16 stopSignal;
  stopSignalActionType stopSignalAction;
  u16 drvFail;
  u16 deragRunTime;
  
  derugMode = (deragModeType)load_s16(ramPnt, progPnt->Pnt_DeraggFunc);
  PWM_state = testBitMicro(ramPnt, progPnt->Pnt_PWM_On);
  deragRef = load_float(ramPnt, progPnt->Pnt_DeraggSpd);
  accelFlg = testBitMicro(ramPnt, progPnt->Pnt_GT_Up);
  decelFlg = testBitMicro(ramPnt, progPnt->Pnt_GT_Down);
  deragRunTime = load_s16(ramPnt, progPnt->Pnt_RunTime);
  actualSpd = load_float(ramPnt, progPnt->Pnt_F_Out);
  deragOffTime = load_s16(ramPnt, progPnt->Pnt_OffDelay);
  cyclesNum = load_s16(ramPnt, progPnt->Pnt_CyclesNum);
  drvFail = testBitMicro(ramPnt, progPnt->Pnt_DrvFail);
  stopSignal = testBitMicro(ramPnt, progPnt->Pnt_GlobalStop);
  stopSignalAction = (stopSignalActionType)load_s16(ramPnt, progPnt->Pnt_ModeWhileStop);
  DinSignal = load_s16(ramPnt, progPnt->Pnt_In5);  
  stopCmd = startCmd = 0;
  
  switch(sPnt->deragState){
  case PWM_OFF:
    if(PWM_state == 1){
      //По включению ШИМ переходим к ожиданию команды очистки
      sPnt->deragState = DERAG_WAIT_FROM_WORK;
    }
    break;
  case DERAG_WAIT_FROM_WORK: 
    if(PWM_state == 0){
      sPnt->startAfterDeragDis = 0;
      sPnt->deragState = PWM_OFF;
    }
    //Если во время ожидания команды очистки была команда останова
    //то возможно начался частотный останов. Если в данной ситуации нужно
    //игнорировать команду очистки, то переходим в состояние ижидания останова
    if((stopSignal == 1) && (stopSignalAction == DONT_PERFORM_DERAG)){ 
      sPnt->deragState = STOP_DONE_WAIT;
      break;
    }else if(stopSignal == 1){ //stopSignalAction == PERFORM_DERAG
      sPnt->startAfterDeragDis = 1; 
    }
    //Если поступила команда очистки, то даем команду останова и 
    //переходим в состояние ожидания выключения ШИМ
    if(DinSignal == 1){    
      stopCmd = 1;         
      sPnt->cyclesCnt = cyclesNum;     
      sPnt->deragState = STOP_WAITING; 
    }
    break;
  case STOP_WAITING: //Ожидание выключения ШИМ           
    if(stopSignal == 1){
      sPnt->deragState = STOP_DONE_WAIT;
      break;
    }
    if(actualSpd != 0.0F){
      break;
    }
    //Скорость обнулилась
    if(sPnt->cyclesCnt != 0){
      sPnt->cyclesCnt--;
      sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT; 
      sPnt->deragState = DERAG_OFF_DELAY;
    }else{
      //Если после очистки можно возвращаться в работу то переходим
      //к паузе перед возвратом в работу
      if(sPnt->startAfterDeragDis == 0){
        sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT;
        sPnt->deragState = START_DELAY;
      }else{
        sPnt->startAfterDeragDis = 0;
        sPnt->deragState = PWM_OFF;
      }
    }
    break;
  case DERAG_OFF_DELAY: //Выдержка перед реверсом 
    sPnt->deragOffCnt--;
    if(sPnt->deragOffCnt == 0){
      setBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);           
      save_float(ramPnt, progPnt->Pnt_DeraggRef, -deragRef); 
      startCmd = 1; 
      sPnt->deragState = REVERSE_ACCEL_DONE_WAITING; 
    }
    break;
  case REVERSE_ACCEL_DONE_WAITING: //Ожидание выхода на скорость очистки
    if(actualSpd == -deragRef){
      sPnt->deragRunCnt = deragRunTime * K_TIME_CALL_MICRO_TAKT;
      sPnt->deragState = DERAG_REVERSE_RUN; 
    }
    break;
  case DERAG_REVERSE_RUN: //Работа на реверсивной скорости
    sPnt->deragRunCnt--;
    if(sPnt->deragRunCnt == 0){
      stopCmd = 1; //Даем команду останова и переходим к ожиданию отключения ШИМ
      save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
      sPnt->deragState = REV_STOP_WAITING; 
    }
    break;
  case REV_STOP_WAITING: //Ожидание останова от реверсивного движения
    if(actualSpd == 0.0F){
      sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT;
      sPnt->deragState = DERAG_REV_OFF_DELAY;
    }
    break;
  case DERAG_REV_OFF_DELAY: //8 - Выдержка после реверсивного движения
    sPnt->deragOffCnt--;
    if(sPnt->deragOffCnt == 0){
       startCmd = 1;
       save_float(ramPnt, progPnt->Pnt_DeraggRef, deragRef);
       sPnt->deragState = ACCEL_DONE_WAITING; 
    }
    break;
  case ACCEL_DONE_WAITING: //Ожидание разгона в прямом направлении
    if(actualSpd == deragRef){
     sPnt->deragRunCnt = deragRunTime * K_TIME_CALL_MICRO_TAKT;
     sPnt->deragState = DERAG_FWD_RUN;
    }
    break;
  case DERAG_FWD_RUN: //2 - Работа в прямом направлении
    sPnt->deragRunCnt--;
    if(sPnt->deragRunCnt == 0){
      save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
      stopCmd = 1; 
      sPnt->deragState = STOP_WAITING; 
    }
    break; 
    case START_DELAY: 
      sPnt->deragOffCnt--;
      if(sPnt->deragOffCnt == 0){
        startCmd = 1;
        save_float(ramPnt, progPnt->Pnt_DeraggRef, 0);
        clrBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);
        sPnt->deragState = DERAG_WAIT_FROM_WORK;
      }
      break;
    case STOP_DONE_WAIT: 
      if(PWM_state == 0){
        sPnt->deragState = PWM_OFF;
      }
      break;
  }

  switch(sPnt->stopState){
  case COMMAND_WAITING:
    if(stopCmd == 1){
      setBitMicro(ramPnt, progPnt->Pnt_StopCmd);
      sPnt->stopHoldCnt = (u16)(PULSE_DURATION * K_TIME_CALL_MICRO_TAKT + 0.5F);
      sPnt->stopState = COMMAND_HOLD;
    }
    break;
  case COMMAND_HOLD:
    if(sPnt->stopHoldCnt == 0){
      clrBitMicro(ramPnt, progPnt->Pnt_StopCmd);
      sPnt->stopState = COMMAND_WAITING;
    }else{
      sPnt->stopHoldCnt--;
    }
    break;
  }
  
  
  switch(sPnt->startState){
  case COMMAND_WAITING:
    if(startCmd == 1){
      setBitMicro(ramPnt, progPnt->Pnt_StartCmd);
      sPnt->startHoldCnt = (u16)(PULSE_DURATION * K_TIME_CALL_MICRO_TAKT + 0.5F);
      sPnt->startState = COMMAND_HOLD;
    }
    break;
  case COMMAND_HOLD:
    if(sPnt->startHoldCnt == 0){
      clrBitMicro(ramPnt, progPnt->Pnt_StartCmd);
      sPnt->startState = COMMAND_WAITING;
    }else{
      sPnt->startHoldCnt--;
    }
    break;
  }
}

//Управление очисткой по превышению мощности
void deragByPower(PUMP_DERAG_StateType *sPnt, FuncPUMP_DERAG_type *progPnt, u32 ramPnt)
{
  u16 PWM_state;
  u16 stopCmd, startCmd;
  f32 deragRef;
  f32 actualSpd;
  u16 deragOffTime;
  u16 cyclesNum;
  u16 powerDeragCmd;
  u16 stopSignal;
  stopSignalActionType stopSignalAction;
  u16 drvFail;
  u16 deragRunTime;
  u16 powerInterval;
  u16 powerDeragCmdFlg;
  u16 powerDeragNum;
  u16 powerDeragErr;
  
  PWM_state = testBitMicro(ramPnt, progPnt->Pnt_PWM_On);
  deragRef = load_float(ramPnt, progPnt->Pnt_DeraggSpd);
  deragRunTime = load_s16(ramPnt, progPnt->Pnt_RunTime);
  actualSpd = load_float(ramPnt, progPnt->Pnt_F_Out);
  deragOffTime = load_s16(ramPnt, progPnt->Pnt_OffDelay);
  cyclesNum = load_s16(ramPnt, progPnt->Pnt_CyclesNum);
  drvFail = testBitMicro(ramPnt, progPnt->Pnt_DrvFail);
  stopSignal = testBitMicro(ramPnt, progPnt->Pnt_GlobalStop);
  stopSignalAction = (stopSignalActionType)load_s16(ramPnt, progPnt->Pnt_ModeWhileStop);
  powerInterval = load_s16(ramPnt, progPnt->Pnt_ConsecTime); //в минутах
  powerDeragCmd = load_s16(ramPnt, progPnt->Pnt_In5);
  powerDeragNum = load_s16(ramPnt, progPnt->Pnt_DeragNum); //Уставка количества процедур
  stopCmd = startCmd = 0;
  powerDeragCmdFlg = 0;
  
  switch(sPnt->deragState){
  case PWM_OFF:
    if(PWM_state == 1){
      //По включению ШИМ переходим к ожиданию команды очистки
      sPnt->deragState = DERAG_WAIT_FROM_WORK;
    }
    break;
  case DERAG_WAIT_FROM_WORK: //Состояние ожидания события превышения мощности
    if(PWM_state == 0){
      sPnt->startAfterDeragDis = 0;
      sPnt->deragState = PWM_OFF;
      break;
    }
    if(stopSignal == 1){ 
      sPnt->deragState = STOP_DONE_WAIT;
      break;
    }
    //Если поступила команда очистки, то даем команду останова и 
    //переходим в состояние ожидания выключения ШИМ
    if(powerDeragCmd == 1){
      powerDeragCmdFlg = 1;
      stopCmd = 1;         
      sPnt->cyclesCnt = cyclesNum;     
      sPnt->deragState = STOP_WAITING; 
    }
    break;
  case STOP_WAITING: //Ожидание выключения ШИМ           
    if(stopSignal == 1){
      sPnt->deragState = STOP_DONE_WAIT;
      break;
    }
    if(actualSpd != 0.0F){
      break;
    }
    //Скорость обнулилась
    if(sPnt->cyclesCnt != 0){
      sPnt->cyclesCnt--;
      sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT; 
      sPnt->deragState = DERAG_OFF_DELAY;
    }else{
      //Если после очистки можно возвращаться в работу то переходим
      //к паузе перед возвратом в работу
      if(sPnt->startAfterDeragDis == 0){
        sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT;
        sPnt->deragState = START_DELAY;
      }else{
        sPnt->startAfterDeragDis = 0;
        sPnt->deragState = PWM_OFF;
      }
    }
    break;
  case DERAG_OFF_DELAY: //Выдержка перед реверсом 
    sPnt->deragOffCnt--;
    if(sPnt->deragOffCnt == 0){
      setBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);           
      save_float(ramPnt, progPnt->Pnt_DeraggRef, -deragRef); 
      startCmd = 1; 
      sPnt->deragState = REVERSE_ACCEL_DONE_WAITING; 
    }
    break;
  case REVERSE_ACCEL_DONE_WAITING: //Ожидание выхода на скорость очистки
    if(actualSpd == -deragRef){
      sPnt->deragRunCnt = deragRunTime * K_TIME_CALL_MICRO_TAKT;
      sPnt->deragState = DERAG_REVERSE_RUN; 
    }
    break;
  case DERAG_REVERSE_RUN: //Работа на реверсивной скорости
    sPnt->deragRunCnt--;
    if(sPnt->deragRunCnt == 0){
      stopCmd = 1; //Даем команду останова и переходим к ожиданию отключения ШИМ
      save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
      sPnt->deragState = REV_STOP_WAITING; 
    }
    break;
  case REV_STOP_WAITING: //Ожидание останова от реверсивного движения
    if(actualSpd == 0.0F){
      sPnt->deragOffCnt = deragOffTime * K_TIME_CALL_MICRO_TAKT;
      sPnt->deragState = DERAG_REV_OFF_DELAY;
    }
    break;
  case DERAG_REV_OFF_DELAY: //8 - Выдержка после реверсивного движения
    sPnt->deragOffCnt--;
    if(sPnt->deragOffCnt == 0){
       startCmd = 1;
       save_float(ramPnt, progPnt->Pnt_DeraggRef, deragRef);
       sPnt->deragState = ACCEL_DONE_WAITING; 
    }
    break;
  case ACCEL_DONE_WAITING: //Ожидание разгона в прямом направлении
    if(actualSpd == deragRef){
     sPnt->deragRunCnt = deragRunTime * K_TIME_CALL_MICRO_TAKT;
     sPnt->deragState = DERAG_FWD_RUN;
    }
    break;
  case DERAG_FWD_RUN: //2 - Работа в прямом направлении
    sPnt->deragRunCnt--;
    if(sPnt->deragRunCnt == 0){
      save_float(ramPnt, progPnt->Pnt_DeraggRef, 0.0F);
      stopCmd = 1; 
      sPnt->deragState = STOP_WAITING; 
    }
    break; 
    case START_DELAY: 
      sPnt->deragOffCnt--;
      if(sPnt->deragOffCnt == 0){
        startCmd = 1;
        save_float(ramPnt, progPnt->Pnt_DeraggRef, 0);
        clrBitMicro(ramPnt, progPnt->Pnt_DeraggFlg);
        sPnt->deragState = DERAG_WAIT_FROM_WORK;
      }
      break;
    case STOP_DONE_WAIT: 
      if(PWM_state == 0){
        sPnt->deragState = PWM_OFF;
      }
      break;
  }

  switch(sPnt->stopState){
  case COMMAND_WAITING:
    if(stopCmd == 1){
      setBitMicro(ramPnt, progPnt->Pnt_StopCmd);
      sPnt->stopHoldCnt = (u16)(PULSE_DURATION * K_TIME_CALL_MICRO_TAKT + 0.5F);
      sPnt->stopState = COMMAND_HOLD;
    }
    break;
  case COMMAND_HOLD:
    if(sPnt->stopHoldCnt == 0){
      clrBitMicro(ramPnt, progPnt->Pnt_StopCmd);
      sPnt->stopState = COMMAND_WAITING;
    }else{
      sPnt->stopHoldCnt--;
    }
    break;
  }
  
  
  switch(sPnt->startState){
  case COMMAND_WAITING:
    if(startCmd == 1){
      setBitMicro(ramPnt, progPnt->Pnt_StartCmd);
      sPnt->startHoldCnt = (u16)(PULSE_DURATION * K_TIME_CALL_MICRO_TAKT + 0.5F);
      sPnt->startState = COMMAND_HOLD;
    }
    break;
  case COMMAND_HOLD:
    if(sPnt->startHoldCnt == 0){
      clrBitMicro(ramPnt, progPnt->Pnt_StartCmd);
      sPnt->startState = COMMAND_WAITING;
    }else{
      sPnt->startHoldCnt--;
    }
    break;
  }

  //Управление аварией, формируемой при превышении
  //количества последовательных процедур очистки по превышению мощности
  switch(sPnt->powerDeragErrState){
  case POWER_EVENT_WAITING: //Ожидаем первый импульс. Состояние после подачи питания
    if(PWM_state == 0){
      sPnt->powerIntervalCnt = 0;
      sPnt->i = 0;
      break;
    }
    if(powerDeragCmdFlg == 1){
      sPnt->powerDeragCnt = 1;    //Засчитываем первый импульс
      sPnt->powerIntervalCnt = 0; //Счетчик интервала времени между импульсами
      sPnt->powerDeragErrState = POWER_EVENT_COUNT;
    }
    break;
  case POWER_EVENT_COUNT:
    if(PWM_state == 0){
      sPnt->powerDeragErrState = POWER_EVENT_WAITING;
      break;
    }
    sPnt->powerIntervalCnt++;
    //Ждем сигнал о начале процедуры очистки
    if(powerDeragCmdFlg == 0){
      break;
    }
    //Сигнал начала процедуры очистки получен
    if(sPnt->powerDeragCnt < powerDeragNum + 1){
      sPnt->powerDeragCnt++;
      sPnt->intervalStrg[sPnt->i] = sPnt->powerIntervalCnt;
      sPnt->i = (sPnt->i < powerDeragNum - 1) ? sPnt->i + 1 : 0;
      sPnt->powerIntervalCnt = 0;
      if(sPnt->powerDeragCnt == powerDeragNum + 1){
        powerDeragErr = powerDeragErrCheck(powerDeragNum, powerInterval, sPnt);
        sPnt->powerDeragErrState = powerDeragErr ? POWER_ERR_HOLD:sPnt->powerDeragErrState; 
      }
      break;
    }
    //Сохраняем значение таймера в циклическом буфере
    //последовательных интервалов
    sPnt->intervalStrg[sPnt->i] = sPnt->powerIntervalCnt;
    sPnt->i = (sPnt->i < powerDeragNum - 1) ? sPnt->i + 1 : 0;
    //Рассчитваем общую длительность между очистками
    powerDeragErr = powerDeragErrCheck(powerDeragNum, powerInterval, sPnt);
    sPnt->powerDeragErrState = powerDeragErr ? POWER_ERR_HOLD:sPnt->powerDeragErrState; 
  
    break;
  case POWER_ERR_HOLD:
    sPnt->powerDeragHoldCnt--;
    if(sPnt->powerDeragHoldCnt == 0){
      sPnt->powerDeragErrState = POWER_EVENT_WAITING;
    }
    break;
  }
}

//Проверка аварии превышения процедур очистки за интервал времени
//powerDeragNum - максимальное количество процедур очистки
//powerInterval - время, за которое разрешено powerDeragNum процедур
//sPnt - Указатель на статичсекие данные функционального блока
//retval - 1 - авария, 0 - Ok
u16 powerDeragErrCheck(u16 powerDeragNum, u16 powerInterval, PUMP_DERAG_StateType *sPnt)
{
  u32 summ;
  u16 i;
  u16 err;
  
  for(summ = 0, i = 0; i < powerDeragNum; i++){
    summ += sPnt->intervalStrg[i];
  }
  if(summ < powerInterval * 60 * K_TIME_CALL_MICRO_TAKT){
    err = 1;
  }else{
    err = 0;
  }
  return err;
}

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPUMP_DERAG_1[45]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  DeraggFunc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  CyclesNum
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  OffDelay
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  RunTime
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  DeraggSpd
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  P1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  P2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  P3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  P4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  P5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  PowerOffset
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  DeragNum
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  PowDeragDelay
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  ConsecTime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  CounterReset
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  ActualPower
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  In2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  In3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  Fmin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  Fmax
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //22  PWM_On
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //23  GT_Up
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //24  GT_Down
        S32_PIN_TYPE | INPUT_PIN_MODE,     //25  F_Out
        S32_PIN_TYPE | INPUT_PIN_MODE,     //26  F_Ref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  ModeWhileStop
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //28  GlobalStop
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //29  DrvFail
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  In5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  In6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  In7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  Functional
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //34  DeraggFlg
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //35  DeraggRef
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //36  Err
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //37  Warn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //38  StartCmd
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //39  StopCmd
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //40  DeragPower
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //41  Out1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //42  Out2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //43  Out3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //44  Out4
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //45  Out5
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PUMP_DERAG", "DD", TblFuncPUMP_DERAG_1, FuncPUMP_DERAG_1, sizeof(TblFuncPUMP_DERAG_1), sizeof(PUMP_DERAG_StateType), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPUMP_DERAG_1,
#endif
//********************************************************
