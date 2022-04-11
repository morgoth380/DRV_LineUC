#ifdef    GET_FUNC_CODE

// Реакция системы на дисбаланс
typedef enum{
//    DisbOff = 0,
//    DisWarning,
//    DisError,    
    IMBAL_OFF = 0,
    IMBAL_WARN = 1,
    IMBAL_ERR = 2,    
}DisbMode_type;

typedef struct{
	u16   timer;
	u16   startStop;
        u16   tmrDelayOff;
}FuncCURRENT_IMBALANCEState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Iu;                           //1  Iu
    u16  Pnt_Iv;                           //2  Iv
    u16  Pnt_Iw;                           //3  Iw
    u16  Pnt_Time;                         //4  Time
    u16  Pnt_Perc;                         //5  Perc
    u16  Pnt_Mode;                         //6  Mode
    u16  Pnt_PWM;                     	   //7  PW_ON
    u16  Pnt_Imbalance;                    //8  Tek_dis
    u16  Pnt_Alarm;                        //9  Alarm
    u16  Pnt_Tcntdown;                     //10  Tcntdown
    u16  Pnt_Warning;                      //11  Warning
    u16  Pnt_Fault;                        //12 Fault
    u16  Pnt_End;
}FuncCURRENT_IMBALANCE_type;
//********************************************************
u16 *FuncCURRENT_IMBALANCE_1(FuncCURRENT_IMBALANCE_type *progPnt, u32 ramPnt)
{
    FuncCURRENT_IMBALANCEState_type *sPnt; 
    float Ucurr, Vcurr, Wcurr; 
    DisbMode_type	Reaction; 
    float Inom, dI, DisbMax, Imidl;
    float max, min, tmp;
    u16   PwmEn;    
    u16   ActiveFault, ActiveWarning;
    u32   DelayOff, MaxDelayOff;    
        

    sPnt = (FuncCURRENT_IMBALANCEState_type *)(ramPnt + progPnt->Pnt_State);
    dI = 0;
    // Получить фазные токи (RMS)
    Ucurr = load_s16(ramPnt, progPnt->Pnt_Iu);
    Vcurr = load_s16(ramPnt, progPnt->Pnt_Iv);
    Wcurr = load_s16(ramPnt, progPnt->Pnt_Iw);

    Ucurr *= 0.1f;
    Vcurr *= 0.1f;
    Wcurr *= 0.1f;
      
    DisbMax = load_float(ramPnt, progPnt->Pnt_Perc);                // Максимальный дисбаланс (аварийная уставка)
    Reaction = (DisbMode_type)load_s16(ramPnt, progPnt->Pnt_Mode);  // Тип реакции системы на дисбаланс
    PwmEn = testBitMicro(ramPnt, progPnt->Pnt_PWM);
    DelayOff = MaxDelayOff = (u32)((load_s16(ramPnt, progPnt->Pnt_Time)*0.001f) * K_TIME_CALL_MICRO_TAKT);  // Получаем время задержки из системы параметров
    ActiveFault = ActiveWarning = 0;
    
    // Первичная инициализация
    if(flagIsFirstCall){
        sPnt->timer = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_Fault);
        clrBitMicro(ramPnt, progPnt->Pnt_Warning);
        save_float(ramPnt, progPnt->Pnt_Imbalance, 0);
    }

    // ШИМ выключен - заряжаем таймеры
    if (!PwmEn)  { 
      sPnt->tmrDelayOff = DelayOff; 
    }      
    
   // Сброс начальных значений
   clrBitMicro(ramPnt, progPnt->Pnt_Alarm);       // сброс предупреждения
   clrBitMicro(ramPnt, progPnt->Pnt_Warning);       // сброс инфо предупреждения   
   clrBitMicro(ramPnt, progPnt->Pnt_Fault);   // сброс аварии
   save_s16(ramPnt, progPnt->Pnt_Tcntdown, 0); 
   save_float(ramPnt, progPnt->Pnt_Imbalance, 0.0f);   // Если не обрабатываем дисбаланс, обнулить его!!!
           
   // 
   if(PwmEn){
     if(fabsf(GlobalM4.Fref) >= 0.5f){
       max = min = Ucurr;
       tmp = Vcurr;
       
       if(max < tmp)	{ max = tmp; }
       if(min > tmp)	{ min = tmp; }
       tmp = Wcurr;
       if(max < tmp)	{ max = tmp; }
       if(min > tmp)	{ min = tmp; }
       
       Inom = GlobalM4.Isn;  // Номинальный ток двигателя
       
       Imidl = (Ucurr + Vcurr + Wcurr + Inom) / 4.0f;
       dI = (max - min) / Imidl; 
       
       save_float(ramPnt, progPnt->Pnt_Imbalance, dI);  // Текущий дисбаланс, %                                      
     }
   }
          
   // Проверка собственно аварии
   if (PwmEn) {
     if (dI > DisbMax) { // Есть превышение?
       ActiveWarning = 1;
       if (sPnt->tmrDelayOff)      sPnt->tmrDelayOff --;
       else                        ActiveFault = 1;
     } else {
       if (sPnt->tmrDelayOff < MaxDelayOff)      sPnt->tmrDelayOff ++;
     }          
   }    
     
   switch (Reaction){
   case IMBAL_OFF:
     break;
     
   case IMBAL_WARN:
     if (ActiveWarning) {
       setBitMicro(ramPnt, progPnt->Pnt_Alarm);  // даем предупреждения
     }
     break;
     
   case IMBAL_ERR:
     if (ActiveWarning) {
       setBitMicro(ramPnt, progPnt->Pnt_Warning);
       save_s16(ramPnt, progPnt->Pnt_Tcntdown, (u16)(sPnt->tmrDelayOff/K_TIME_CALL_MICRO_TAKT));         
     }
     
     if (ActiveFault) {
       setBitMicro(ramPnt, progPnt->Pnt_Fault);  // даем аварию
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
const char TblFuncCURRENT_IMBALANCE_1[12]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Iu
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Iv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Iw
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Time
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Perc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Mode
	BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  PWM_ON
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Tek_dis
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Alarm
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Tcntdown
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //12 Fault
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CURRENT_IMBALANCE", "DA", TblFuncCURRENT_IMBALANCE_1, FuncCURRENT_IMBALANCE_1, sizeof(TblFuncCURRENT_IMBALANCE_1), sizeof(FuncCURRENT_IMBALANCEState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCURRENT_IMBALANCE_1,
#endif
//********************************************************
