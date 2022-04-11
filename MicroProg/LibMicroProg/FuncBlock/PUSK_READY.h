#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Pusk[6];                      //1  Pusk_1
    u16  Pnt_ChargeOk;                     //7  ChargeOk
    u16  Pnt_TimeMTZ;                      //8  TimeMTZ
    u16  Pnt_Vibeg;                        //9  Vibeg
    u16  Pnt_DiskrNoSpd;                   //10  DiskrNoSpd
    u16  Pnt_EnablePusk;                   //11  EnablePusk
    u16  Pnt_FAILTEK;                      //12  FAILTEK    
    u16  Pnt_MeasParam;                    //13  MeasParam        
    u16  Pnt_NoSpdFault;                   //14  NoSpdWarn
    u16  Pnt_NoSpdWarn;                    //15  NoSpdFault    
    u16  Pnt_WrnPuskForbid;                //16  WrnPuskForbid
    u16  Pnt_NReadyPusk;                   //17  NReadyPusk
    u16  Pnt_ReadyPusk;                    //18  ReadyPusk
    u16  Pnt_Pusk_Cmn;                     //19  Pusk_Cmn
    u16  Pnt_End;
}FuncPUSK_READY_type;

//********************************************************
typedef struct {
  u16     NoSpdWarnCnt;                  // Счетчик для предупреждения об блокировки пуска без скорости 
}StateFuncPUSK_READY_1;
  
//********************************************************
u16 *FuncPUSK_READY_1(FuncPUSK_READY_type *progPnt, u32 ramPnt)
{
     s16		        i, notRdyReason;
     u16                        NoSpdStatus;
     u16                        MeasParam, EnablePusk;
     StateFuncPUSK_READY_1      *sPnt;
          
     sPnt = (StateFuncPUSK_READY_1 *)(ramPnt + progPnt->Pnt_State);
     
     // Сбросить сигналы аварий и предупреждения при блокировке пуска без дискретной скорости.
     clrBitMicro (ramPnt, progPnt->Pnt_NoSpdFault);
     clrBitMicro (ramPnt, progPnt->Pnt_NoSpdWarn);
     clrBitMicro (ramPnt, progPnt->Pnt_WrnPuskForbid);
     
     NoSpdStatus = load_s16 (ramPnt, progPnt->Pnt_DiskrNoSpd);
     MeasParam   = load_s16 (ramPnt, progPnt->Pnt_MeasParam);
     EnablePusk  = testBitMicro (ramPnt, progPnt->Pnt_EnablePusk);
     
     notRdyReason = load_s16(ramPnt, progPnt->Pnt_FAILTEK); //получаем номер аварии
	 // Обработка исключительных состояние только в реальном ПЧ. В симуляции данная часть заблокирована.
#ifdef _PROJECT_FOR_STM32_      
     if(notRdyReason); //если авария есть
     else if(testBitMicro(ramPnt, progPnt->Pnt_ChargeOk) == 0) 
          notRdyReason = notRdyReason_charge;                           //если звено постоянного тока не заряжено
     else if(testBitMicro(ramPnt, progPnt->Pnt_Vibeg)    == 1)	
          notRdyReason = notRdyReason_vibeg;
//     else if (NoSpdStatus)
//          notRdyReason = SlowFail_StartNoSpeedFault;
#endif
     if((notRdyReason)||(GlobalM4.FlagCommand.bit.PwmOn)||(GlobalM4.FlagCommand.bit.mWorkEnb)){    // Текущая авария или ПЧ уже в работе или в режиме измерения параметров АД
          //если есть блокировка пуска
          clrBitMicro(ramPnt, progPnt->Pnt_ReadyPusk);            // не даем сигнала готовности пуска
          clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);             // не даем команду пуска
     }
     else{ //иначе пускаться ничего не мешает
          if(!GlobalM4.FlagCommand.bit.PwmOn){                 // если на данный момент PWM отключено
              setBitMicro(ramPnt, progPnt->Pnt_ReadyPusk);     // даем сигнал готовности
          }
          else{ 
              clrBitMicro(ramPnt, progPnt->Pnt_ReadyPusk);     // иначе PWM уже работает и сбрасываем сигнал готовности
          }
          for(i = 0; i < 6; i++){				  // Отрабатываем пуск при наличии сигнала (ищем любой сигнал пуска)
              if(testBitMicro(ramPnt, progPnt->Pnt_Pusk[i])){
                  setBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);
                  break;
              }
              else if(i == 5){ // Заблокирован пуск по профинету
                  clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);	  // Сбрасываем если дошли до конца и пусков не было
              }
          }
          
          // Проверим блокировку пуска без скорости, если пришла команда на пуск
          if (testBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn) && NoSpdStatus && !MeasParam) {
            // Блок пуска - т.к. скорости нету.
            clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);
            // Проверяем, что делаем - предупреждение или аварию формируем
            #define  NO_SPD_WARN_VIEW       5*K_TIME_CALL_MICRO_TAKT
            switch (NoSpdStatus) {
            case NoSpdWarnMode:
              sPnt->NoSpdWarnCnt = NO_SPD_WARN_VIEW; // Заряжаем счетчик
              break;
            case NoSpdFaultMode:
              setBitMicro (ramPnt, progPnt->Pnt_NoSpdFault); 
              break;
            default:
              break;             
            }            
                        
          } else if (!EnablePusk && testBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn)) {
            // Если был пуск, но не было разрешения, то блокируем пуск и выдаем предупреждение
            setBitMicro (ramPnt, progPnt->Pnt_WrnPuskForbid);  // Предупреждение защелкиваем в блоке предупреждений!
            clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);
          }
     }
     save_s16(ramPnt, progPnt->Pnt_NReadyPusk, notRdyReason); 

     // Отображалка предупреждения при блокировке пуска без дискретной скорости
     if (sPnt->NoSpdWarnCnt) {
       setBitMicro (ramPnt, progPnt->Pnt_NoSpdWarn);
       sPnt->NoSpdWarnCnt --;
     }     
     
     return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPUSK_READY_1[19]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Pusk_1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Pusk_2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Pusk_3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Pusk_4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Pusk_5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Pusk_6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  ChargeOk
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  TimeMTZ
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Vibeg
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  DiskrNoSPd
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  EbalePusk
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  FAILTEK
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  MeasParam
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14  NoSpdWarn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //15  NoSpdFault
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //16  WrnPuskForbid
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  NReadyPusk
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //18  ReadyPusk
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //19  Pusk_Cmn
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PUSK_READY", "DD", TblFuncPUSK_READY_1, FuncPUSK_READY_1, sizeof(TblFuncPUSK_READY_1), sizeof(StateFuncPUSK_READY_1), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPUSK_READY_1,
#endif
//********************************************************
