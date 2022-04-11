#ifdef    GET_FUNC_CODE

typedef struct{
    u32  tmrDelayOff;
    u32  tmrDelayCntr;
}Underload_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Iakt;                         //1  Iakt
    u16  Pnt_Ifull;                        //2  Ifull
    u16  Pnt_Mode;                         //3  Mode
    u16  Pnt_Itreshold;                    //4  Pnt_Imin
    u16  Pnt_Tdis;                         //5  Tdis
    u16  Pnt_Tcntrl;                       //6  Tcntrl
    u16  Pnt_Reaction;                     //7  Reaction
    u16  Pnt_ApvEnable;                    //8  ApvEnable
    u16  Pnt_PWM_ON;                       //9  PWM_ON
    u16  Pnt_Enable;                       //10  Enable
    u16  Pnt_Warning;                      //11  Warning
    u16  Pnt_Tcntdown;                     //12  Tcntdown
    u16  Pnt_Fault;                        //13  Fault
    u16  Pnt_Alarm;                        //14  Alarm
    u16  Pnt_End;
}FuncUNDERLOAD_type;

//********************************************************
typedef enum {
    UNDERLOAD_OFF = 0,
    UNDERLOAD_WARN = 1,
    UNDERLOAD_ERR = 2,
} Underload_Enum;
//********************************************************
u16 *FuncUNDERLOAD_1(FuncUNDERLOAD_type *progPnt, u32 ramPnt)
{
   Underload_State_type  *sPnt;
//   u32          TimeWait;
   float        Current_I, Ilevel;
   u16          ApvEnable, ActiveFault, ActiveWarning, PwmEn; 
   u32          DelayCntr, DelayOff, MaxDelayOff;
   Underload_Enum Reaction;

   sPnt = (Underload_State_type *)(ramPnt + progPnt->Pnt_State);

    if (load_s16(ramPnt, progPnt->Pnt_Mode)){               // если 1 - полный ток
        Current_I = load_float(ramPnt, progPnt->Pnt_Ifull);      // Текущее значение cosFi
    } else {
        Current_I = load_float(ramPnt, progPnt->Pnt_Iakt);      // Текущее значение cosFi
    }
    
   PwmEn = testBitMicro (ramPnt, progPnt->Pnt_PWM_ON);
   Ilevel = load_float(ramPnt, progPnt->Pnt_Itreshold);              // Уставка cosFi
   DelayOff = (u32)load_s16(ramPnt, progPnt->Pnt_Tcntrl);
   DelayCntr = (u32)load_s16(ramPnt, progPnt->Pnt_Tdis);
   ApvEnable = load_s16 (ramPnt, progPnt->Pnt_ApvEnable);
   Reaction = (Underload_Enum)load_s16 (ramPnt, progPnt->Pnt_Reaction);
   ActiveFault = ActiveWarning = 0;
   //MaxDelayCntr =  DelayCntr * K_TIME_CALL_MICRO_TAKT;
   MaxDelayOff  =  DelayOff * K_TIME_CALL_MICRO_TAKT;
   
   if (ApvEnable) {
      save_s16 (ramPnt, progPnt->Pnt_Reaction, UNDERLOAD_ERR);     
   }
      
  // ШИМ выключен - заряжаем таймеры
  if (!PwmEn)  { 
    sPnt->tmrDelayOff = DelayOff * K_TIME_CALL_MICRO_TAKT; 
    sPnt->tmrDelayCntr = DelayCntr * K_TIME_CALL_MICRO_TAKT; 
  }   
   
  if (sPnt->tmrDelayCntr)     sPnt->tmrDelayCntr--;
    
   // Сброс начальных значений
   clrBitMicro(ramPnt, progPnt->Pnt_Alarm);       // сброс предупреждения
   clrBitMicro(ramPnt, progPnt->Pnt_Warning);       // сброс инфо предупреждения   
   clrBitMicro(ramPnt, progPnt->Pnt_Fault);   // сброс аварии
   save_s16(ramPnt, progPnt->Pnt_Tcntdown, 0); 
      
     // Проверка собственно аварии
  if (PwmEn) {
    if (!sPnt->tmrDelayCntr) { // Ждем таймаут при старте       
      if (Current_I < Ilevel) { // Есть превышение?
        ActiveWarning = 1;
        if (sPnt->tmrDelayOff)      sPnt->tmrDelayOff --;
        else                        ActiveFault = 1;
      } else {
        if (sPnt->tmrDelayOff < MaxDelayOff)      sPnt->tmrDelayOff ++;
      }
    }      
  }

            
    switch (Reaction){
    case UNDERLOAD_OFF:
      break;
      
    case UNDERLOAD_WARN:
      if (ActiveWarning) {
            setBitMicro(ramPnt, progPnt->Pnt_Alarm);  // даем предупреждения
      }
      break;
      
    case UNDERLOAD_ERR:
      if (ActiveWarning) {
        setBitMicro(ramPnt, progPnt->Pnt_Warning );
        save_s16(ramPnt, progPnt->Pnt_Tcntdown, (u16)(sPnt->tmrDelayOff/K_TIME_CALL_MICRO_TAKT));         
      }
      
      if (ActiveFault) {
            setBitMicro(ramPnt, progPnt->Pnt_Fault);  // даем аварию
      }
      break;
      
    default:
      break;
      
    }    
            
       
       
//   // Если ШИМ отключен... 
//   if(testBitMicro(ramPnt, progPnt->Pnt_PWM_ON) == 0){
//       sPnt->CounterOFF_4ms = (u32)load_s16(ramPnt, progPnt->Pnt_Tdis) * K_TIME_CALL_MICRO_TAKT;
//       sPnt->CounterWait_4ms = 0;
//       save_s16(ramPnt, progPnt->Pnt_Tcntdown, 0);        // обнулить счетчик 
//       sPnt->DelayUndrld --;
//       
//       if(sPnt->DelayUndrld <= 1){
//          sPnt->DelayUndrld = 1;
//          clrBitMicro(ramPnt, progPnt->Pnt_Fault);   // сброс аварии
//       }
//       clrBitMicro(ramPnt, progPnt->Pnt_Alarm);       // сброс предупреждения
//       clrBitMicro(ramPnt, progPnt->Pnt_Warning);       // сброс инфо предупреждения
//       return &progPnt->Pnt_End;
//   }
//
//   if(Reaction){  	 // Если разрешен контроль недогруза
//       if(sPnt->CounterOFF_4ms > 0){
//           sPnt->CounterOFF_4ms --;          
//           return &progPnt->Pnt_End;                      // ВЫХОД!!!
//       }
//      
//       if(Current_I < Ilevel){
//            sPnt->CounterWait_4ms ++;
//            if( (sPnt->CounterWait_4ms > TimeWait) && (Reaction == UNDERLOAD_ERR) ){
//                 // Авария
//                 clrBitMicro(ramPnt, progPnt->Pnt_Warning);       // если уже авария, то предупреждение не выводить
//                 setBitMicro(ramPnt, progPnt->Pnt_Fault);
//                 sPnt->CounterWait_4ms = TimeWait;
//                 sPnt->DelayUndrld = K_TIME_CALL_MICRO_TAKT; 
//            }else{
//                 // обратный отчет времени до аварии
//                 setBitMicro(ramPnt, progPnt->Pnt_Warning);                               // предупреждение об аварии 
//                 Tmp = (TimeWait - sPnt->CounterWait_4ms) / K_TIME_CALL_MICRO_TAKT;     //250;
//                 save_s16(ramPnt, progPnt->Pnt_Tcntdown, (s16)Tmp);
//            }
//       }
//       else{     // нет недогруза
//            sPnt->CounterWait_4ms =0;
//            save_s16(ramPnt, progPnt->Pnt_Tcntdown, 0);
//            clrBitMicro(ramPnt, progPnt->Pnt_Warning);       // Сброс аварии
//            clrBitMicro(ramPnt, progPnt->Pnt_Fault);   // Сброс аварии
//       }
//  
//   }           
//   else{  // если запрешен контроль недогруз
//       sPnt->CounterOFF_4ms = 0;
//       sPnt->CounterWait_4ms = 0;
//       save_s16(ramPnt, progPnt->Pnt_Tcntdown, 0);
//       clrBitMicro(ramPnt, progPnt->Pnt_Warning);	     // Сброс аварии
//       clrBitMicro(ramPnt, progPnt->Pnt_Alarm);	     // Сброс предупреждения.       
//       clrBitMicro(ramPnt, progPnt->Pnt_Fault);  // Сброс аварии
//   }

   return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUNDERLOAD_1[14]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Cos_tek
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Inom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Ust%
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  COSnom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Tdis
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Tcntrl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Fcntrl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Pcntrl
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  PWM_ON
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Enable
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Warning
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Tcntdown
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //13  ti>Tcntrl
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14  Alarm
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UNDERLOAD", "DD120", TblFuncUNDERLOAD_1, FuncUNDERLOAD_1, sizeof(TblFuncUNDERLOAD_1), sizeof(Underload_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUNDERLOAD_1,
#endif
//********************************************************
