#ifdef    GET_FUNC_CODE

typedef enum{
  NoFailsAin = 0,
  minFailAin    ,
  maxFailAin
}CodeFailAin_type;

typedef enum{
  DisableAinReaction = 0,
  AinWarning,
  AinFault,
  AinFixedSpeed,
}AinReaction_type;


//INT_TIME_CALL_MICRO_mS
//#define TimeMP 4

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Max;                          //1  Max
    u16  Pnt_Min;                          //2  Min
    u16  Pnt_Stepen;                       //3  Stepen
    u16  Pnt_EdIsm;                        //4  EdIsm
    u16  Pnt_Ain;                          //5  Ain
    u16  Pnt_Tfiltr;                       //6  Tfiltr
    u16  Pnt_FailMax;                      //7  FailMax
    u16  Pnt_FailMin;                      //8  FailMin
    u16  Pnt_CH_PID;                       //9  Выбран канал задания ПИД
    u16  Pnt_P1;                           //10 P1
    u16  Pnt_P2;                           //11 P2
    u16  Pnt_P3;                           //12 P3
    u16  Pnt_M1;                           //13 M1
    u16  Pnt_M2;                           //14 M2
    u16  Pnt_M3;                           //15 M3
    u16  Pnt_HardFail;                     //16 HardFail
    u16  Pnt_Reaction;                     //17 Reaction    
    u16  Pnt_DelayContr;                   //18 DelayContr
    u16  Pnt_DelayOff;                     //19 DelayOff
    u16  Pnt_ApvEnable;                    //20 ApvEnable
    u16  Pnt_PwmEn;                        //21 PwmEn    
    u16  Pnt_Out;                          //22 Out
    u16  Pnt_Format;                       //23 Format
    u16  Pnt_Out__;                        //24 Out_%
    u16  Pnt_Fail;                         //25 Fail
    u16  Pnt_CodeFail;                     //26 CodeFail
	u16  Pnt_AlrmFixSpd;                   //27 CodeFail
    u16  Pnt_Alrm;                         //28 Alrm
    u16  Pnt_CodeAlrm;                     //29 CodeAlrm
    u16  Pnt_TimeToStop;                   //30 TimeToStop
    u16  Pnt_WarnToStop;                   //31 WarnToStop
    u16  Pnt_End;
}FuncAIN_SCALE3_V2_type;

typedef struct{
  float ainValue;
  u16   CntFiltrAin;                            // счетчик для фильтра
  u16	tmrDelayControl;
  u16	tmrDelayOff;
  u16   FixedSpeedWarn;
}AINSCALE3_State_type;

//********************************************************

u16 *FuncAIN_SCALE3_V2_1(FuncAIN_SCALE3_V2_type *progPnt, u32 ramPnt)
{
  
    AINSCALE3_State_type      *sPnt;
    s16                       pow, razm;
    u16                       TFiltrAin, delayOff, delayControl, ApvEn, PwmEn, AinFailActive, maxDelayOff;
    AinReaction_type          Reaction;
    CodeFailAin_type          CodeFail;
    u16                       PidEn;
    float                     P1, P2, P3, M1, M2, M3, tekAinValue;
    float                     ainMax, ainMin , scaleFactor, ResultValue, scaleOut, prcntOut /*,HistFailMin*/;
    sPnt = (AINSCALE3_State_type *)(ramPnt + progPnt->Pnt_State);

    CodeFail = NoFailsAin;
    AinFailActive = 0;
    PidEn = (u16)testBitMicro(ramPnt, progPnt->Pnt_CH_PID);
    
    // Тикаем таймером предзадержки
    if (sPnt->tmrDelayControl)	sPnt->tmrDelayControl--;
    
    if(flagIsFirstCall)
    {
    	sPnt->ainValue = 0;
        sPnt->CntFiltrAin = 0;
    	clrBitMicro(ramPnt, progPnt->Pnt_Fail);      //Обнуляем ошибки по аналоговому входу
        clrBitMicro(ramPnt, progPnt->Pnt_Alrm);
    	save_s16(ramPnt, progPnt->Pnt_CodeFail, 0);
        save_s16(ramPnt, progPnt->Pnt_CodeAlrm, 0);
    }
    else
    {
      ApvEn		 = load_s16 (ramPnt, progPnt->Pnt_ApvEnable);
      PwmEn		 = testBitMicro (ramPnt, progPnt->Pnt_PwmEn);
      delayOff     = load_s16(ramPnt, progPnt->Pnt_DelayOff);
      delayControl = load_s16(ramPnt, progPnt->Pnt_DelayContr);
      Reaction     = (AinReaction_type)load_s16(ramPnt, progPnt->Pnt_Reaction);
      maxDelayOff  = delayOff * K_TIME_CALL_MICRO_TAKT; // Предел счета таймера отключения при аварии
      
      if (ApvEn) { // Если АПВ для Аналогового входа активировали - включаем аварию 
        save_s16(ramPnt, progPnt->Pnt_Reaction, AinFault);
      }
      
      // Если ШИМ отключен, заводим таймер задержки начала счета аварии
      if (PwmEn == 0) {
        sPnt->tmrDelayControl = delayControl * K_TIME_CALL_MICRO_TAKT; // Переводим в такты
        sPnt->FixedSpeedWarn = 0;
      } 
      
      pow = load_s16(ramPnt, progPnt->Pnt_Stepen)&0xFF; //степень (количество знаков после десятичной точки)
      razm = load_s16(ramPnt, progPnt->Pnt_EdIsm)&0xFF; //единицы измерения
      save_s16(ramPnt, progPnt->Pnt_Format, ((pow << 8) | razm));        //Сформировали формат
      
      TFiltrAin = load_s16(ramPnt, progPnt->Pnt_Tfiltr);
      tekAinValue = load_float(ramPnt, progPnt->Pnt_Ain);
      
      sPnt->ainValue = FiltrValue(TFiltrAin, tekAinValue,sPnt->ainValue);
      
      ainMax = (load_float(ramPnt, progPnt->Pnt_Max) ); //максимальное значение параметра, преобразовуемого датчиком (например, давление)
      ainMin = (load_float(ramPnt, progPnt->Pnt_Min) ); //минимальное значение параметра, преобразовуемого датчиком (например, давление)
      
      P1 = load_float(ramPnt, progPnt->Pnt_P1);
      P2 = load_float(ramPnt, progPnt->Pnt_P2);
      P3 = load_float(ramPnt, progPnt->Pnt_P3);
      M1 = load_float(ramPnt, progPnt->Pnt_M1);
      M2 = load_float(ramPnt, progPnt->Pnt_M2);
      M3 = load_float(ramPnt, progPnt->Pnt_M3);
      
      scaleFactor = ainMax - ainMin; //диапазон изменения параметра, преобразовуемого датчиком, аналогвый выход которого поступает на вход ПЧ
      
      if(sPnt->ainValue < P1){
        ainMax = ((float)M1 * scaleFactor) + ainMin;
        ResultValue = (sPnt->ainValue / (float)P1);
        prcntOut = (ResultValue * (float)M1);
      }
      else if(sPnt->ainValue < P2){
        ainMax = ((float)M2 * scaleFactor) + ainMin;
        ainMin = ((float)M1 * scaleFactor) + ainMin;
        ResultValue = ((sPnt->ainValue - (float)P1) / ((float)(P2 - P1)));
        prcntOut = (ResultValue * (M2 - M1)) + M1;
      }
      else if(sPnt->ainValue < P3){
        ainMax = ((float)M3 * scaleFactor) + ainMin;
        ainMin = ((float)M2 * scaleFactor) + ainMin;
        ResultValue = ((sPnt->ainValue - (float)P2) / ((float)(P3 - P2)));
        prcntOut =  (ResultValue * ((float)(M3 - M2))) + M2;
      }
      else{
        ainMin = ((float)M3 * scaleFactor) + ainMin;
        ResultValue = ((sPnt->ainValue - P3) /  ((float)(1.f - P3)));
        prcntOut = (ResultValue * ((float)(1.f - M3))) + M3;
      }
      
      scaleOut = (ResultValue * (ainMax - ainMin)) + ainMin;  //
      save_float(ramPnt, progPnt->Pnt_Out, scaleOut);         // Текущее значение аналогового сигнала на входе датчика
      save_float(ramPnt, progPnt->Pnt_Out__, prcntOut);       // Out_%  Текущее значение аналогового сигнала на входе датчика в %
      
      ainMax = load_float(ramPnt, progPnt->Pnt_FailMax);                      //Значение
      ainMin = load_float(ramPnt, progPnt->Pnt_FailMin);
      
      // Делаем поиск аварии по аналоговому входу
      if(load_s16(ramPnt, progPnt->Pnt_HardFail)){                        //Если авария из блока Perif
        if(scaleOut < ainMin){
          CodeFail = minFailAin;
        }
        else{
          CodeFail = maxFailAin;
        }
      } else if(scaleOut < ainMin) {
        CodeFail = minFailAin;
      } else if(scaleOut > ainMax) {
        CodeFail = maxFailAin;
      } else {
        CodeFail = NoFailsAin;
      }
      
      // Если нет аварии - счетчик растет вверх
      if (CodeFail == NoFailsAin) {
        sPnt->tmrDelayOff++;
        if (sPnt->tmrDelayOff > maxDelayOff)  sPnt->tmrDelayOff = maxDelayOff;
      } else { // Если есть авария, проверяем какое условие нам подходит с ШИМом или без
        if (PwmEn) { // Если ШИМ включен - то надо подождать пока предтаймер дотикает до нуля
          if (!sPnt->tmrDelayControl) { // Проверяем - дотикал ли предтаймер
            if (sPnt->tmrDelayOff)    sPnt->tmrDelayOff--; // Таймер тикает вниз
            else		      AinFailActive = 1;   // Дотикал - ставим Флаг, что авария может быть доступной
          }
        } else { // Если ШИМ выключен, то сразу можно тикать до возникновения аварии
          AinFailActive = 1;
//          if (sPnt->tmrDelayOff)    sPnt->tmrDelayOff--; // Таймер тикает вниз
//          else		      AinFailActive = 1;   // Дотикал - ставим Флаг, что авария может быть доступной
          sPnt->tmrDelayOff = maxDelayOff;
        }	
        
      }
      
      // Предварительный сброс всех статусов
      clrBitMicro(ramPnt, progPnt->Pnt_Alrm);
      save_s16(ramPnt, progPnt->Pnt_TimeToStop, 0); // Время до аварии
      clrBitMicro(ramPnt, progPnt->Pnt_WarnToStop); // Предупреждение в блок отсчета до аварии
      clrBitMicro(ramPnt, progPnt->Pnt_Fail);   //выдаем сигнал аварии
      save_s16(ramPnt, progPnt->Pnt_CodeFail, 0);  //выдаем код аварии          
      clrBitMicro(ramPnt, progPnt->Pnt_AlrmFixSpd);
     // Определяем тип реакции на аналоговый вход
      switch (Reaction){
      case DisableAinReaction:
        break;
        
      case AinWarning:
        if (CodeFail != NoFailsAin) { // Если есть авария - выдаем предупреждение
           setBitMicro(ramPnt, progPnt->Pnt_Alrm);  // даем предупреждения
        }
        break;
        
      case AinFault:
        // Если предтаймер дотикал и есть авария, но еще не активная, выдаем время до отключения и предупрждение отключения
        if (!sPnt->tmrDelayControl && CodeFail != NoFailsAin && !AinFailActive) {
          save_s16 (ramPnt, progPnt->Pnt_TimeToStop, (u16)(sPnt->tmrDelayOff/K_TIME_CALL_MICRO_TAKT)); // Время до аварии
          setBitMicro (ramPnt, progPnt->Pnt_WarnToStop); // Предупреждение в блок отсчета до аварии
        }
        
        if (AinFailActive) { // Если авария активна - выдаем ее
          defBitMicro(ramPnt, progPnt->Pnt_Fail, CodeFail);   //выдаем сигнал аварии
          save_s16(ramPnt, progPnt->Pnt_CodeFail, CodeFail);  //выдаем код аварии          
        }
        break;
        
      case AinFixedSpeed:
        // Если предтаймер дотикал и есть авария, но еще не активная, выдаем время до отключения и предупрждение отключения
//        if (!sPnt->tmrDelayControl && CodeFail != NoFailsAin && !AinFailActive) {
//          save_s16 (ramPnt, progPnt->Pnt_TimeToStop, (u16)(sPnt->tmrDelayOff/K_TIME_CALL_MICRO_TAKT)); // Время до аварии
//          setBitMicro (ramPnt, progPnt->Pnt_WarnToStop); // Предупреждение в блок отсчета до аварии
//        }
        if (PidEn == 1) { // Если режим ПИДа выбран в задании, то не защелкиваем предупреждение
          sPnt->FixedSpeedWarn = 0;
        }
        
        if (AinFailActive && PwmEn) { // Если авария активна - выдаем сообщение что перешли на фиксированную скорость
          save_s16(ramPnt, progPnt->Pnt_CodeFail, CodeFail);  //выдаем код аварии 
          sPnt->FixedSpeedWarn = 1; // Защелкиваем предупреждение
        }
        
        if (sPnt->FixedSpeedWarn && PwmEn) { // Выводим предупреждение о работе на фиксированной частоте
           setBitMicro(ramPnt, progPnt->Pnt_Alrm);  // даем предупреждения о работе на фиксированной частоте
        } else if (AinFailActive && !PwmEn) {
           setBitMicro(ramPnt, progPnt->Pnt_Alrm);  // даем предупреждения
        }
        break;
        
      default:
        break;
       
      }
        
      save_s16(ramPnt, progPnt->Pnt_CodeAlrm, 0);                             //нет кода предупреждения
    }

    return &progPnt->Pnt_End;
};

#endif

//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAIN_SCALE3_V2_1[31]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Stepen
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  EdIsm
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Ain
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Tfiltr
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  FailMax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  FailMin
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Not used (HistlMin)
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10 P1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11 P2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12 P3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13 M1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14 M2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15 M3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16 HardFail
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17 Reaction
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18 DelayContr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19 DelayOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20 ApvEnable                
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //21 PwmEn                        
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //22 Out
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23 Format
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //24 Out_%
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25 Fail
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26 CodeFail
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //27 AlrmFixSpd		
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //28 Alrm
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29 CodeAlrm
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30 TimeToStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //31 WarnToStop
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AIN_SCALE3_V2", "DA", TblFuncAIN_SCALE3_V2_1, FuncAIN_SCALE3_V2_1, sizeof(TblFuncAIN_SCALE3_V2_1), sizeof(AINSCALE3_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAIN_SCALE3_V2_1,
#endif
//********************************************************
