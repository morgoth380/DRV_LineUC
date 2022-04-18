#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Pusk[10];
    u16  Pnt_Charge;                       //11  Charge
    u16  Pnt_TimeMTZ;                      //12  TimeMTZ
    u16  Pnt_Vibeg;                        //13  Vibeg
    u16  Pnt_DiskrNoSpd;                   //14  DiskrNoSpd
    u16  Pnt_EnablePusk;                   //15  EnablePusk
    u16  Pnt_FAILTEK;                      //16  FAILTEK
    u16  Pnt_MeasParam;                    //17  MeasParam
    u16  Pnt_In1;                          //18  In1
    u16  Pnt_In2;                          //19  In2
    u16  Pnt_Out1;                         //20  Out1
    u16  Pnt_Out2;                         //21  Out2
    u16  Pnt_NoSpdFault;                   //22  NoSpdFault
    u16  Pnt_NoSpdWarn;                    //23  NoSpdWarn
    u16  Pnt_WrnPuskForbid;                //24  WrnPuskForbid
    u16  Pnt_NReadyPusk;                   //25  NReadyPusk
    u16  Pnt_ReadyPusk;                    //26  ReadyPusk
    u16  Pnt_Pusk_Cmn;                     //27  Pusk_Cmn
    u16  Pnt_End;
}FuncPUSK_READY_2_type;
//********************************************************
typedef struct {
  u16     NoSpdWarnCnt;                  // —четчик дл€ предупреждени€ об блокировки пуска без скорости 
}StateFuncPUSK_READY_1;

u16 *FuncPUSK_READY_2_1(FuncPUSK_READY_2_type *progPnt, u32 ramPnt)
{
     s16		        i, notRdyReason;
     u16                        NoSpdStatus;
     u16                        MeasParam, EnablePusk;
     u16                        sleepBlink;
     u16                        blinkEnable;
     u16                        readyStatus;
     StateFuncPUSK_READY_1      *sPnt;
          
     sPnt = (StateFuncPUSK_READY_1 *)(ramPnt + progPnt->Pnt_State);
     
     // —бросить сигналы аварий и предупреждени€ при блокировке пуска без дискретной скорости.
     clrBitMicro (ramPnt, progPnt->Pnt_NoSpdFault);
     clrBitMicro (ramPnt, progPnt->Pnt_NoSpdWarn);
     clrBitMicro (ramPnt, progPnt->Pnt_WrnPuskForbid);
     
     blinkEnable = testBitMicro(ramPnt, progPnt->Pnt_In2); //Sleep mode status
     sleepBlink = testBitMicro(ramPnt, progPnt->Pnt_In1);  //Blink signal
     
     NoSpdStatus = load_s16 (ramPnt, progPnt->Pnt_DiskrNoSpd);
     MeasParam   = load_s16 (ramPnt, progPnt->Pnt_MeasParam);
     EnablePusk  = testBitMicro (ramPnt, progPnt->Pnt_EnablePusk);
     
     notRdyReason = load_s16(ramPnt, progPnt->Pnt_FAILTEK); //получаем номер аварии
	 // ќбработка исключительных состо€ние только в реальном ѕ„. ¬ симул€ции данна€ часть заблокирована.
#ifdef _PROJECT_FOR_STM32_      
     if(notRdyReason); //если авари€ есть
     else if(testBitMicro(ramPnt, progPnt->Pnt_Charge) == 0) 
          notRdyReason = notRdyReason_charge;                           //если звено посто€нного тока не зар€жено
     else if(testBitMicro(ramPnt, progPnt->Pnt_Vibeg)    == 1)	
          notRdyReason = notRdyReason_vibeg;
//     else if (NoSpdStatus)
//          notRdyReason = SlowFail_StartNoSpeedFault;
#endif
     if((notRdyReason)||(GlobalM4.FlagCommand.bit.PwmOn)||(GlobalM4.FlagCommand.bit.mWorkEnb)){    // “екуща€ авари€ или ѕ„ уже в работе или в режиме измерени€ параметров јƒ
          //если есть блокировка пуска
          clrBitMicro(ramPnt, progPnt->Pnt_ReadyPusk);            // не даем сигнала готовности пуска
          clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);             // не даем команду пуска
     }
     else{ //иначе пускатьс€ ничего не мешает
          if(!GlobalM4.FlagCommand.bit.PwmOn){                   // если на данный момент PWM отключено
              readyStatus = (blinkEnable == 1) ? sleepBlink : 1; //If sleep mode is active then ready signal blinks
              defBitMicro(ramPnt, progPnt->Pnt_ReadyPusk, readyStatus);     // даем сигнал готовности
          }
          else{ 
              clrBitMicro(ramPnt, progPnt->Pnt_ReadyPusk);     // иначе PWM уже работает и сбрасываем сигнал готовности
          }
          for(i = 0; i < 10; i++){				  // ќтрабатываем пуск при наличии сигнала (ищем любой сигнал пуска)
              if(testBitMicro(ramPnt, progPnt->Pnt_Pusk[i])){
                  setBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);
                  break;
              }
              else if(i == 5){ // «аблокирован пуск по профинету
                  clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);	  // —брасываем если дошли до конца и пусков не было
              }
          }
          
          // ѕроверим блокировку пуска без скорости, если пришла команда на пуск
          if (testBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn) && NoSpdStatus && !MeasParam) {
            // Ѕлок пуска - т.к. скорости нету.
            clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);
            // ѕровер€ем, что делаем - предупреждение или аварию формируем
            #define  NO_SPD_WARN_VIEW       5*K_TIME_CALL_MICRO_TAKT
            switch (NoSpdStatus) {
            case NoSpdWarnMode:
              sPnt->NoSpdWarnCnt = NO_SPD_WARN_VIEW; // «ар€жаем счетчик
              break;
            case NoSpdFaultMode:
              setBitMicro (ramPnt, progPnt->Pnt_NoSpdFault); 
              break;
            default:
              break;             
            }            
                        
          } else if (!EnablePusk && testBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn)) {
            // ≈сли был пуск, но не было разрешени€, то блокируем пуск и выдаем предупреждение
            setBitMicro (ramPnt, progPnt->Pnt_WrnPuskForbid);  // ѕредупреждение защелкиваем в блоке предупреждений!
            clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Cmn);
          }
     }
     save_s16(ramPnt, progPnt->Pnt_NReadyPusk, notRdyReason); 

     // ќтображалка предупреждени€ при блокировке пуска без дискретной скорости
     if (sPnt->NoSpdWarnCnt) {
       setBitMicro (ramPnt, progPnt->Pnt_NoSpdWarn);
       sPnt->NoSpdWarnCnt --;
     }     
 
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPUSK_READY_2_1[27]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Pusk_1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Pusk_2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Pusk_3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Pusk_4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Pusk_5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Pusk_6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Pusk_7
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Pusk_8
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Pusk_9
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Pusk_10
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  Charge
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  TimeMTZ
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  Vibeg
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  DiskrNoSpd
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  EnablePusk
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  FAILTEK
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  MeasParam
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //18  In1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //19  In2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //20  Out1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //21  Out2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  NoSpdFault
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  NoSpdWarn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  WrnPuskForbid
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  NReadyPusk
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  ReadyPusk
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //27  Pusk_Cmn
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PUSK_READY_2", "DD", TblFuncPUSK_READY_2_1, FuncPUSK_READY_2_1, sizeof(TblFuncPUSK_READY_2_1), sizeof(StateFuncPUSK_READY_1), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPUSK_READY_2_1,
#endif
//********************************************************
