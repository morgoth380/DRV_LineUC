#ifdef    GET_FUNC_CODE

#define   MAX_WARN_QUEUE         8
#define   MAX_WARN_NUMBER        64
#define   WARN_BUF_SIZE		 MAX_WARN_NUMBER/32 + 1

// Список предупреждений, которые настраиваются частным образом
// Номер предупреждения равен номеру в списке микропрограммы минус 1 !!!!
#define   WARN_PUSK_FORBIDDEN           2
#define   WARN_PUSK_ON_POWER            3
#define   WARN_ENCODER_PHASING_DONE     6
//#define   WARN_PUSK_DRIVE               12
//#define   WARN_STOP_DRIVE               13
#define   WARN_DUPLICATE_NETADR         26 
#define   WARN_DISKR_INPUT_BUSY         28
#define   WARN_DISKR_FUNC_EXIST         29

typedef union{
		struct{
                   u32  WarnState         : 1;    // Флаг состояния предупреждения
		   u32  PrevWarnState	  : 1;    // Предыдущее состояние предупреждения с прошлого такта работы
                   u32  WarnPriority      : 1;    // Приоритет предупреждения
                   u32  DelayWarnView     : 2;    // Задержка отображения Аварии
                   u32  DelayCnt          :16;    // Фактический таймер задержки отображения всего 65535
		 } bit;
		u32 all;
	} WarningType_struct;

typedef struct{
        u16     WarnNumber;
        u16     WarnPriority;
} WarningQueue_struct;

typedef enum {
    LOW_PRIORITY    = 0,
    HIGH_PRIORITY   = 1,        
} WarnPriority_Enum;

typedef enum {
    WARN_VIEW_WHILE_SET = 0,
    WARN_VIEW_5_SEC = 1,
    WARN_VIEW_15_SEC = 2,
    WARN_VIEW_60_SEC = 3,   
}DelayWarnView_Enum;

typedef struct{
        u16   num_from_is;        
        // New Warnings
        u16   AutoListCnt;
        u16   ListTrigger;
        WarningType_struct      WarnArray[MAX_WARN_NUMBER]; // Создаем массив на MAX_WARN_NUMBER предупреждений
        s16                     WarnViewIndex;
        u16                     warnAmount;
        u16                     warnAmountHigh;
}WARNINGS_State_type;

//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_WarnMask0;                        //1  mask0
    u16  Pnt_WarnMask1;                        //2  mask1
    u16  Pnt_WarnMask2;                        //3  mask2
    u16  Pnt_WarnMask3;                        //4  mask3
    u16  Pnt_WarnMask4;                        //5  mask4
    u16  Pnt_WarnMask5;                        //6  mask5
    u16  Pnt_WarnMask6;                        //7  mask6
    u16  Pnt_WarnMask7;                        //8  mask7    
    u16  Pnt_NextWarn;                         //9  NextWarn
    u16  Pnt_AutoListTime;                     //10  AutoListTime
    u16  Pnt_NextWarnASU;                      //11  NextWarnASU
    u16  Pnt_WarnBit;                          //12  WarnBit
    u16  Pnt_WarnOut;                          //13  WarnOut
    u16  Pnt_WarnNum;                          //14  WarnNum    
    u16  Pnt_WarnAmount;                       //15  WarnAmount
    u16  Pnt_End;
}FuncWARNINGS_type;
//********************************************************
u16 *FuncWARNINGS_1(FuncWARNINGS_type *progPnt, u32 ramPnt)
{
    //u16 num, num_tek, amount = 0;
    //u64 mask;
    
    s16         warnAmountHigh;
    u16         num_from_is;    
    u16         i, WarnWord1, WarnWord2, warnAmount;
    u32	        WarnDWord[WARN_BUF_SIZE];
    WARNINGS_State_type *sPnt;

    sPnt = (WARNINGS_State_type *)(ramPnt + progPnt->Pnt_State);

    
    if (flagIsFirstCall){
      sPnt->AutoListCnt = 0;
      sPnt->ListTrigger = 0;
      sPnt->WarnViewIndex = 0; // Предупреждений нет!!
      
      // Обнулить массив предупрежденийы 
      for (i = 0; i < MAX_WARN_NUMBER; i++){
        sPnt->WarnArray[i].all = 0;
      }

      sPnt->WarnArray[WARN_PUSK_ON_POWER].bit.WarnPriority = HIGH_PRIORITY;
      sPnt->WarnArray[WARN_PUSK_ON_POWER].bit.DelayWarnView = WARN_VIEW_WHILE_SET;            
      sPnt->WarnArray[WARN_PUSK_FORBIDDEN].bit.WarnPriority = HIGH_PRIORITY;
      sPnt->WarnArray[WARN_PUSK_FORBIDDEN].bit.DelayWarnView = WARN_VIEW_5_SEC;      
      sPnt->WarnArray[WARN_DUPLICATE_NETADR].bit.WarnPriority = HIGH_PRIORITY;
      sPnt->WarnArray[WARN_DUPLICATE_NETADR].bit.DelayWarnView = WARN_VIEW_5_SEC;      
      sPnt->WarnArray[WARN_DISKR_FUNC_EXIST].bit.WarnPriority = HIGH_PRIORITY;
      sPnt->WarnArray[WARN_DISKR_FUNC_EXIST].bit.DelayWarnView = WARN_VIEW_5_SEC;
      sPnt->WarnArray[WARN_DISKR_INPUT_BUSY].bit.WarnPriority = HIGH_PRIORITY;
      sPnt->WarnArray[WARN_DISKR_INPUT_BUSY].bit.DelayWarnView = WARN_VIEW_5_SEC;      
      sPnt->WarnArray[WARN_ENCODER_PHASING_DONE].bit.WarnPriority = HIGH_PRIORITY;
      sPnt->WarnArray[WARN_ENCODER_PHASING_DONE].bit.DelayWarnView = WARN_VIEW_15_SEC;
      
//      sPnt->WarnArray[WARN_PUSK_DRIVE].bit.WarnPriority = HIGH_PRIORITY;
//      sPnt->WarnArray[WARN_PUSK_DRIVE].bit.DelayWarnView = WARN_VIEW_5_SEC;            
//      sPnt->WarnArray[WARN_STOP_DRIVE].bit.WarnPriority = HIGH_PRIORITY;
//      sPnt->WarnArray[WARN_STOP_DRIVE].bit.DelayWarnView = WARN_VIEW_5_SEC;            
      
      save_s16(ramPnt, progPnt->Pnt_NextWarn, 0); // Сброс листания предупреждений
    }
    //--------------------------------------------------------------------------
    // Таймер автообновления предупреждений
    if (sPnt->AutoListCnt) {    
        sPnt->AutoListCnt--;     
    } else {
        sPnt->AutoListCnt = K_TIME_CALL_MICRO_TAKT * load_s16(ramPnt, progPnt->Pnt_AutoListTime);
        sPnt->ListTrigger = 1;        
    }
    
    // Проверяем внешние сигналы прокрутки
    if (load_s16(ramPnt, progPnt->Pnt_NextWarn) || testBitMicro(ramPnt, progPnt->Pnt_NextWarnASU)) {
        sPnt->AutoListCnt = K_TIME_CALL_MICRO_TAKT * load_s16(ramPnt, progPnt->Pnt_AutoListTime);
        sPnt->ListTrigger = 1;  
        save_s16(ramPnt, progPnt->Pnt_NextWarn, 0);
    }
    //--------------------------------------------------------------------------
    
    // Читаем перечень предупреждений из входов   
    // !!!!!!  Заполняем массив данными о текущих предупреждениях  !!!!!!
    memset(WarnDWord, 0, sizeof(WarnDWord)); // Чистим массив будущих данных для битов аварий
                                                                // Чистим текущие аварии в sPnt
    for (i = 0; i < MAX_WARN_NUMBER; i++) {
        sPnt->WarnArray[i].bit.WarnState = 0;
    }
  
    //-- Считываем блоками входы аварий	(не быстрых)
    WarnWord1 = load_s16(ramPnt, progPnt->Pnt_WarnMask0 );      // Первые 16 предупреждений 
    //WarnWord1 = GlobalM4.Debug2;
    WarnWord2 = load_s16(ramPnt, progPnt->Pnt_WarnMask1 );      // Вторые 16 предупреждений
    WarnDWord[0] = WarnWord1;                                   // Заносим 32 предупреждений
    WarnDWord[0] |= (u32)WarnWord2 << 16;
                                                              // Считываем следующие 32 предупреждений и заносим в массив
    WarnWord1 = load_s16(ramPnt, progPnt->Pnt_WarnMask2 );    // Третьи 16 предупреждений 
    WarnWord2 = load_s16(ramPnt, progPnt->Pnt_WarnMask3 );    // 4-е 16 предупреждений 
    WarnDWord[1] = WarnWord1;            
    WarnDWord[1] |= (u32)WarnWord2 << 16;                     // Заносим 32 предупреждений
    // И так далее ...
    
    
    warnAmount = warnAmountHigh = 0;

    // До разбора предупреждений, восстановим защелкнутые предупреждения
    for (i = 0; i < MAX_WARN_NUMBER; i++) {
      if (sPnt->WarnArray[i].bit.DelayWarnView != WARN_VIEW_WHILE_SET) {
        if (sPnt->WarnArray[i].bit.DelayCnt) { // Если счетчик отображения не ноль уменьшаем его
            sPnt->WarnArray[i].bit.DelayCnt --;
        } else { // Если ноль - значит выкинуть предупреждение из защелки
          sPnt->WarnArray[i].bit.PrevWarnState = 0;
        }
        // Если предудыщий статус не ноль, восстановим
        if (sPnt->WarnArray[i].bit.PrevWarnState) { 
            sPnt->WarnArray[i].bit.WarnState = 1;
			warnAmount ++; // Увеличиваем текущее кол-во предупреждений
			// Если приоритет высокий - увеличить кол-во приоритетных предупреждений
			if (sPnt->WarnArray[i].bit.WarnPriority == HIGH_PRIORITY)     warnAmountHigh ++;
        }        
      }
    }
       
    // Парсим массив предупреждений и заносим в свои слоты
    for (i = 0; i < MAX_WARN_NUMBER; i++) {
          if (WarnDWord[i/32] & (1<<(i%32))) { // Выделяем маской аварии из массива - находим номер массива и номер бита в нем.
                  sPnt->WarnArray[i].bit.WarnState = 1;                  
                  warnAmount ++; // Увеличиваем кол-во предупреждений.  
                  if (sPnt->WarnArray[i].bit.WarnPriority == HIGH_PRIORITY)     warnAmountHigh ++;
                  
                  // Проверяем режим отображения предупреждения
                  if (sPnt->WarnArray[i].bit.DelayWarnView != WARN_VIEW_WHILE_SET) {
                    sPnt->WarnArray[i].bit.PrevWarnState = 1; // Защелкиваем предупреждение
                    // Заводим таймер на фиксацию предупреждения
                    switch (sPnt->WarnArray[i].bit.DelayWarnView) {
                        case WARN_VIEW_15_SEC:
                          sPnt->WarnArray[i].bit.DelayCnt = (u16)(15*K_TIME_CALL_MICRO_TAKT);
                          break;
                        case WARN_VIEW_60_SEC:
                          sPnt->WarnArray[i].bit.DelayCnt = (u16)(60*K_TIME_CALL_MICRO_TAKT);
                          break;
                        case WARN_VIEW_5_SEC:
                        default:
                          sPnt->WarnArray[i].bit.DelayCnt = (u16)(5*K_TIME_CALL_MICRO_TAKT);                          
                          break;                      
                    }
                  }
          }
    } 
        
    //**************************************************************************
    
    if (warnAmount) { // Кол-во больше нуля
      if(sPnt->warnAmount != warnAmount){
        sPnt->warnAmount = warnAmount;
        sPnt->warnAmountHigh = warnAmountHigh;
        sPnt->num_from_is = 0;
        sPnt->AutoListCnt = K_TIME_CALL_MICRO_TAKT * load_s16(ramPnt, progPnt->Pnt_AutoListTime); // Перезапустить таймер прокрутки
      }
      
      num_from_is = sPnt->num_from_is;
        
	  i = -1;
      while (1) {
	    i++;
		if ( i > (MAX_WARN_NUMBER-1))	i = i-MAX_WARN_NUMBER;
        if (sPnt->WarnArray[i].bit.WarnState == 0)      continue;
        
        sPnt->WarnViewIndex = i; // Нашли предупреждение, запомнили номер
        // Проверяем приоритетные предупреждения
        if (warnAmountHigh && sPnt->WarnArray[i].bit.WarnPriority == HIGH_PRIORITY) {
          if (!num_from_is) {
            break; // Нашли приоритетеное предупреждение          
          } else {
            num_from_is--;
			warnAmountHigh --;
            continue; // Иначе ищем дальше.
          }
		} else if (warnAmountHigh) { continue; }
                  
        if (!num_from_is) break; // Если уже ноль num_from_is - то мы встретили свое предупреждение для вывода
        num_from_is--;  // Иначе ищем наше предупреждение.
      }
      
      // Проверяем листание
      if(sPnt->ListTrigger) {        
        sPnt->num_from_is++;
        if(sPnt->num_from_is >= sPnt->warnAmount){
          sPnt->num_from_is -= sPnt->warnAmount;
        }   
      }
            
      // Вывод активного предупреждения
      save_s16(ramPnt, progPnt->Pnt_WarnOut, sPnt->WarnViewIndex+1);
      save_s16(ramPnt, progPnt->Pnt_WarnAmount, sPnt->warnAmount);
      save_s16(ramPnt, progPnt->Pnt_WarnNum, sPnt->num_from_is + 1);
      setBitMicro(ramPnt, progPnt->Pnt_WarnBit);  // Сигнал, что есть предупреждение
      sPnt->ListTrigger = 0;
            
    } else { // Нету предупреждений
      sPnt->WarnViewIndex = -1;
      sPnt->warnAmount = 0;
      sPnt->num_from_is = 0;
      
      save_s16(ramPnt, progPnt->Pnt_WarnAmount, 0);
      save_s16(ramPnt, progPnt->Pnt_WarnOut, 0);
      save_s16(ramPnt, progPnt->Pnt_WarnNum, 0);
      clrBitMicro(ramPnt, progPnt->Pnt_WarnBit);  // Сигнал, что нет предупреждений    
      sPnt->ListTrigger = 0;      
    }
        
     
    
    
//    // Делаем поиск самого приоритетного предупреждения
//    for (i=0; i<sPnt->WarnQueueLastPos) {
//         
//        sPnt->WarnQueueCurWarn = sPnt->WarnQueue[sPnt->WarnQueueLastPos].WarnPriority
//    }

    //--------------------------------------------------------------    
    
//    mask = (u64)((u16)load_s16(ramPnt, progPnt->Pnt_WarnMask0));
//    mask |= ((u64)((u16)load_s16(ramPnt, progPnt->Pnt_WarnMask1)) << 16);
//    mask |= ((u64)((u16)load_s16(ramPnt, progPnt->Pnt_WarnMask2)) << 32);
//    mask |= ((u64)((u16)load_s16(ramPnt, progPnt->Pnt_WarnMask3)) << 48);
//
//    //mask = 5;
//    
//    // Определим количество предупреждений по маске
//    for(num = 0; num < MAX_WARN_NUMBER; num++){
//        if((mask >> num) & 0x1){
//             amount++;
//        }
//    }
//    
//    if(amount > 0){       // Предупреждения присутствуют
//        if(sPnt->amount != amount){
//            sPnt->amount = amount;
//            sPnt->num_from_is = 0;
//        }
//        if(testBitMicro(ramPnt, progPnt->Pnt_NextWarn) || sPnt->ListTrigger){
//            sPnt->num_from_is++;
//            if(sPnt->num_from_is >= sPnt->amount){
//                sPnt->num_from_is -= sPnt->amount;
//            }
//        }
//        num_tek = sPnt->num_from_is;
//        
//        for(num = 0; num < MAX_WARN_NUMBER; num++){
//            if((mask >> num) & 0x1){
//                if(num_tek == 0){
//                     save_s16(ramPnt, progPnt->Pnt_WarnOut, num + 1);
//                     save_s16(ramPnt, progPnt->Pnt_WarnAmount, sPnt->amount);
//                     save_s16(ramPnt, progPnt->Pnt_WarnNum, sPnt->num_from_is + 1);
//                     setBitMicro(ramPnt, progPnt->Pnt_WarnBit);  // Сигнал, что есть предупреждение
//                     sPnt->ListTrigger = 0;
//                     return &progPnt->Pnt_End;
//                }
//                else{
//                     num_tek--;
//                }
//            }
//        }
//    } else {   // Нет активных предупреждений
//         sPnt->amount = 0;
//         sPnt->num_from_is = 0;
//    }
//
//    save_s16(ramPnt, progPnt->Pnt_WarnAmount, 0);
//    save_s16(ramPnt, progPnt->Pnt_WarnOut, 0);
//    save_s16(ramPnt, progPnt->Pnt_WarnNum, 0);
//    clrBitMicro(ramPnt, progPnt->Pnt_WarnBit);  // Сигнал, что нет предупреждений    
//    sPnt->ListTrigger = 0;
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncWARNINGS_1[15]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  mask0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  mask1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  mask2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  mask3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  mask4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  mask5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  mask6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  mask7        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  NextWarn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10 AutoListTime
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  NextWarnASU
        
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  WarnBit
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  WarnOut
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14  WarnNum
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //15  WarnAmount
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("WARNINGS", "DA", TblFuncWARNINGS_1, FuncWARNINGS_1, sizeof(TblFuncWARNINGS_1), sizeof(WARNINGS_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncWARNINGS_1,
#endif
//********************************************************
