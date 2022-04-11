#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_EvacEnable;        //1  EvacEnable
    u16  Pnt_EvacDirSet;        //2  EvacDirSet
    u16  Pnt_EvacImax;          //3  EvacImax
    u16  Pnt_EvacUdmin;         //4  EvacUdmin
    u16  Pnt_EvacUdcharge;      //5  EvacUdcharge
    u16  Pnt_DinEvacStart;      //6  DinEvacStart
    u16  Pnt_LiftBrakeOff;      //7  LiftBrakeOff
    u16  Pnt_PwmOn;             //8  PwmOn
    u16  Pnt_DF_Reverse;        //9  DF_Reverse
    u16  Pnt_Functional;        //10  Functional
    u16  Pnt_EvacOn;            //11 EvacOn
    u16  Pnt_EvacDirOut;        //12 EvacDirOut
    u16  Pnt_UdminOut;          //13 UdminOut
    u16  Pnt_UdchargeOut;       //14 UdchargeOut            
    u16  Pnt_End;
}FuncEVAC_V3_type;
//********************************************************
#define     nvSfEVAC     (*(nvEVACState_type*)0)

typedef enum {
     EvacEnable = 1,
     EvacDisable = 0,
} EvacEnable_enum;

//enum EVACDir{
//		FWDDir= 0,
//		BKWDDir,
//		AUTODir,
//};

typedef struct{
    u16  evacDir;
}nvEVACState_type;

typedef struct{
    u16  evacDir;
    EvacOutDir_enum     EvacOutDirection;  
    u16	 timerPiSpd;

    struct{
          u16  disFramWR: 1;
          u16  PwmOn	: 1;
          u16  Reserve1	: 14;
        }bit;
}EVACState_type;

//typedef enum{
//  LightDirection = 0,
//  StationDirection = 1,
//  ForwardDirection = 2,  
//  BackwardDirection = 3,  
//}EvacDirSet_enum;

//typedef enum{
//  SearchEvacDir = 0,
//  ForwardEvacDir = 1,
//  BackwardEvacDir = -1,  
//}EvacOutDirection;

#define WAIT_TIME_GET_DIRECTION         0.5  // Время определения легкого направления - 1 sec

u16 *FuncEVAC_V3_1(FuncEVAC_V3_type *progPnt, u32 ramPnt)
{
    u16                 PwmOn, LiftBrakeOff, EvacStart, Rev_CMD;
//    EvacOutDir_enum     EvacOutDirection;
    EvacDirSet_enum     EvacDirSet;
    EvacEnable_enum     EvacMode;
    f32                 Udmin, Udcharge, ImaxEvac, temp;
    EVACState_type      *sPnt;
    nvEVACState_type    *nvAdr;      
    
    sPnt    = (EVACState_type *)(ramPnt + progPnt->Pnt_State);
    nvAdr   = GetNvAdr(progPnt->Pnt_nvState, &nvSfEVAC);    
    EvacMode = (EvacEnable_enum)load_s16(ramPnt, progPnt->Pnt_EvacEnable);
    
    Rev_CMD = load_s16(ramPnt, progPnt->Pnt_DF_Reverse);  // Читаем сигнал реверса
            
    if (flagIsFirstCall) {
        sPnt->EvacOutDirection = ForwardEvacDir;
        RdFramMicro( sPnt, nvAdr, sizeof(nvEVACState_type));  // Восстановить состояние
        sPnt->bit.disFramWR = 1;
        return &progPnt->Pnt_End; // !!!!
    }
        
    // Если установлен режим лифта - то работаем
    if( ((AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_Functional) == UseLiftFunc) && (EvacMode == EvacEnable) ) {
    
      // Загружаем входные данные
      PwmOn = testBitMicro(ramPnt, progPnt->Pnt_PwmOn);
      LiftBrakeOff = testBitMicro(ramPnt, progPnt->Pnt_LiftBrakeOff);
      EvacStart = testBitMicro(ramPnt, progPnt->Pnt_DinEvacStart);
                       
      // Если режим Эвакуации активирован - надо передать на выход данные о уровнях напряжений в данном режиме работы
      if (EvacStart) { 
          Udmin  = load_float(ramPnt, progPnt->Pnt_EvacUdmin);
          Udcharge  = load_float(ramPnt, progPnt->Pnt_EvacUdcharge);

          ImaxEvac = load_float(ramPnt, progPnt->Pnt_EvacImax); 
          temp = FLOAT_to_IQ(GlobalM4.EnginParam.Isn, Current_base); // Приводим номинальный ток к базе по току
          GlobalM4.ImaxEvac = ImaxEvac * temp; // И умножаем на процент разрешенного тока для эвакуации
          
          save_float (ramPnt, progPnt->Pnt_UdminOut, Udmin);
          save_s16 (ramPnt, progPnt->Pnt_UdchargeOut, (u16)Udcharge);
          // Сообщаем системе, что пошла работать эвакуация.
          defBitMicro(ramPnt, progPnt->Pnt_EvacOn, EvacStart);
          GlobalM4.FlagCommand.bit.EvacOn = EvacStart; 
            
          EvacDirSet  = (EvacDirSet_enum)load_s16(ramPnt, progPnt->Pnt_EvacDirSet);                    
          // Алгоритм определения направления
          // Алгоритм формирует три варианта на выходе Pnt_EvacDirOut
          // 0 - Идет определение направления, а значит блок Ramp_Control будет выдавать нулевой задание скорости
          // 1 - Едем вперед
          // -1 - Едем назад
          if (!PwmOn) { // PwmOff - Задаем ожидаемое направление движения пока ШИМ не запущен
            
              switch (EvacDirSet) {
                  case LightDirection:
                    sPnt->EvacOutDirection = SearchEvacDir; // Идет поиск направления - задание на выходе 0
                    // В стопе заряжаем таймер легкого направления вращения для режима ВУ
                    sPnt->timerPiSpd = (u16)(WAIT_TIME_GET_DIRECTION * K_TIME_CALL_MICRO_TAKT);                    
                    break;
                  case BackwardDirection:
                    sPnt->EvacOutDirection = BackwardEvacDir; // Обратное направление - скорость с -
                    break;                           
                  case StationDirection: // Прямое направление, направление от СУ - скорость с +
                  case ForwardDirection:
                  default:
                    sPnt->EvacOutDirection = ForwardEvacDir;
                    break;  
              }
          } else { // PwmOn
            // При запуске ШИМа определяем - надо направление искать или нет, и снят ли тормоз
            if ( (EvacDirSet == LightDirection) && LiftBrakeOff) {
               // Определяем направление движения в зависимости от режима управления ПЧ (Скаляр или ВУ)
              
              if (TYPEDRV_ANYSCALAR & (1<<GlobalM4.DriveType)) {
              // Эвакуация для скаляров - вытащить из памяти направление
                sPnt->bit.disFramWR = 1;
                if (sPnt->evacDir){  // Проверяем сигнал из Фрамки                         
                    sPnt->EvacOutDirection = BackwardEvacDir;                        
                } else {
                    sPnt->EvacOutDirection = ForwardEvacDir;
                }
                
              } else if(TYPEDRV_ANYFOC & (1<<GlobalM4.DriveType)) {
                // Эвакуация для векторных режимов - анализ выхода контура скорости
                if (sPnt->timerPiSpd)     sPnt->timerPiSpd--; // Ведем обратный отсчет времени до выдачи направления                        
                
                // На последнем шаге фиксируем направление
                if (sPnt->timerPiSpd == 1) {
                  if (GlobalM4.piSpdOut > 0) {
                    sPnt->EvacOutDirection = BackwardEvacDir;                
                  } else { 
                    sPnt->EvacOutDirection = ForwardEvacDir;                
                  }
                }                                          
              }  
            }
          }
                    
          save_s16 (ramPnt, progPnt->Pnt_EvacDirOut, sPnt->EvacOutDirection);  // EvacOutDirection - Возможно его сделать статиком, иначе как дублировать выход направления         
          //---------------------------------          
      } else {
        // Если выбран скаляр, то надо следить за токами и зафиксировать на момент выключения будущее направление.
        if (TYPEDRV_ANYSCALAR & (1<<GlobalM4.DriveType)) {        
          clrBitMicro(ramPnt, progPnt->Pnt_EvacOn);
          if(!sPnt->bit.disFramWR && !GlobalM4.FlagCommand.bit.ChargeOk){
            WrFramMicro( nvAdr, sPnt, sizeof(nvEVACState_type));
            sPnt->bit.disFramWR = 1;
          } else {
            if(PwmOn == 1){
              if(sPnt->bit.PwmOn == 0){
                sPnt->bit.PwmOn = 1;
                sPnt->bit.disFramWR = 0;
              }
              // Анализируем токи и направление
              if(GlobalM4.Iact < 0){
                if(sPnt->evacDir != Rev_CMD){
                  sPnt->evacDir   = Rev_CMD;  //Generator mode
                  WrFramMicro( nvAdr, sPnt, sizeof(nvEVACState_type));
                }
              }else{
                if(sPnt->evacDir == Rev_CMD){
                  sPnt->evacDir   =(!Rev_CMD);  //Motor mode - Boolean value
                  WrFramMicro( nvAdr, sPnt, sizeof(nvEVACState_type));
                }
              }              
              // Тестовый вывод направления, которое определяет скаляр.
              save_s16 (ramPnt, progPnt->Pnt_EvacDirOut, sPnt->evacDir); // !!! ТЕСТ !!!
              
            } else {
              sPnt->bit.PwmOn = 0;
            }
          }
                    
        } // if (TYPEDRV_ANYSCALAR & (1<<GlobalM4.DriveType)) 
        
      }
    }
    
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncEVAC_V3_1[14]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  EvacEnable
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  EvacDirSet
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  EvacImax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  EvacUdmin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  EvacUdcharge
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  DinEvacStart
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  LiftBrakeOff
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  PwmOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  DF_Reverse
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Functional
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11  EvacOn
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  EvacDirOut
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //13  UdminOut
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14  UdchargeOut            
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("EVAK_V3", "DA", TblFuncEVAC_V3_1, FuncEVAC_V3_1, sizeof(TblFuncEVAC_V3_1), sizeof(EVACState_type), sizeof(nvEVACState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncEVAC_V3_1,
#endif
//********************************************************
