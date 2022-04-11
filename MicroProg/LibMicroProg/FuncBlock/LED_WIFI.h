#ifdef    GET_FUNC_CODE

#define DelayBlinkTime      K_TIME_CALL_MICRO_TAKT / 2     // время для смены состояния 125*4 = 500 = 0.5 сек
#define FOPDelayBlinkTime   K_TIME_CALL_MICRO_TAKT / 5     // 0.2 sec
#define FOPTimeout          2*K_TIME_CALL_MICRO_TAKT       // 2 sec

#define LedOFF  0
#define LedON   1


#define NoConnect_wifi  0       // нет подключений к WIFI
#define Wait_wifi       1       // есть подключения, ожидание обмена
#define Work_wifi       2       // есть подключение и есть обмен
#define FOP_wifi        3       // Работа с ФОПом
  
typedef struct{
  u16 cntBlink;                 // счетик для смены стостояния
  u16 stateLed;                 // текущее состояние светодиода( вкл или откл)           
  u16 cntResetFOPTimeout;          // Счетчик для сброса флага чтения ФОПа
}FuncLED_WIFI_State_type;

extern u16 WIFI_FOP_Action;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Link;                         //1  Link
    u16  Pnt_Access;                       //2  Access
    u16  Pnt_LED;                          //3  LED
    u16  Pnt_End;
}FuncLED_WIFI_type;
//********************************************************
u16 *FuncLED_WIFI_1(FuncLED_WIFI_type *progPnt, u32 ramPnt)
{
    FuncLED_WIFI_State_type *sPnt;
    u16 status_WIFI, WIFI_Net_Err;
    
    status_WIFI = WIFI_Net_Err = 0;
    sPnt = (FuncLED_WIFI_State_type *)(ramPnt + progPnt->Pnt_State);
    WIFI_Net_Err = testBitMicro(ramPnt, progPnt->Pnt_Link);
    GlobalM4.status_WIFI = status_WIFI = load_s16(ramPnt, progPnt->Pnt_Access);
        
    if(WIFI_Net_Err) {
        clrBitMicro(ramPnt, progPnt->Pnt_LED);
        return &progPnt->Pnt_End;
    }
      
    // Обработка запроса записи ФОПа
    if (sPnt->cntResetFOPTimeout)       sPnt->cntResetFOPTimeout--;
    if (WIFI_FOP_Action) {      
      WIFI_FOP_Action = 0;
      status_WIFI = FOP_wifi;
      sPnt->cntResetFOPTimeout = FOPTimeout;
    } else if (sPnt->cntResetFOPTimeout) {
      status_WIFI = FOP_wifi;
    }
    //-------------------------------
    
    switch(status_WIFI) {
      case NoConnect_wifi:
        sPnt->stateLed = LedOFF;
        break;
        
      case Wait_wifi:
        if(sPnt->cntBlink == 0)
        {
          if (sPnt->stateLed == LedOFF)
              sPnt->stateLed = LedON;
          else 
              sPnt->stateLed = LedOFF;

          sPnt->cntBlink = DelayBlinkTime;
        }
        else
            sPnt->cntBlink--;
        
        break;
        
      case Work_wifi:
        sPnt->stateLed = LedON;
        break;
        
      case FOP_wifi:        
        if(sPnt->cntBlink == 0)
        {
          if (sPnt->stateLed == LedOFF)
              sPnt->stateLed = LedON;
          else 
              sPnt->stateLed = LedOFF;

          sPnt->cntBlink = FOPDelayBlinkTime;
        }
        else
            sPnt->cntBlink--;
                
        
        break;
                
    }
    defBitMicro(ramPnt, progPnt->Pnt_LED, sPnt->stateLed);
    return &progPnt->Pnt_End;
};
#endif


//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLED_WIFI_1[3]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Link
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Access
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  LED
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LED_WIFI", "DD", TblFuncLED_WIFI_1, FuncLED_WIFI_1, sizeof(TblFuncLED_WIFI_1), sizeof(FuncLED_WIFI_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLED_WIFI_1,
#endif
//********************************************************
