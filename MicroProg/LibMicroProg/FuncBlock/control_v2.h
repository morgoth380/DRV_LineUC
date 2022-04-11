#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_ChCntr1;                    //1  ChCntr1
    u16  Pnt_ChCntr2;                   //2  ChCntr2
    u16  Pnt_UstSel;                    //3  UstSel
    u16  Pnt_DinSel;                    //4  DinSel
    u16  Pnt_PwmEn;                     //5  PwmEn
    u16  Pnt_En_LC;                     //6  En_WIFI
    u16  Pnt_En_RC;                     //7  En_RC
    u16  Pnt_En_Din;                    //8  En_Din
    u16  Pnt_En_WIFI;                   //9  En_Pult
    u16  Pnt_ActiveCntrl;                   //9  Active Control    
    u16  Pnt_End;
}FuncCONTROL_V2_type;
//********************************************************
typedef enum{
	LC_sc = 0,      //!< Местный пульт
        WIFI_sc  ,      //!< WIFI
	RC_sc    ,      //!< АСУ
	Din_sc          //!< Дискретные входы
}srcControl_type;
//********************************************************
u16 *FuncCONTROL_V2_1(FuncCONTROL_V2_type *progPnt, u32 ramPnt)
{
    s16		num;

    // Если ШИМ выключен - можно менять каналы
    if ( testBitMicro(ramPnt, progPnt->Pnt_PwmEn) == 0) {      
      num = load_s16(ramPnt, progPnt->Pnt_UstSel);
      
      if(num > 1) 
        num = testBitMicro(ramPnt, progPnt->Pnt_DinSel);
      
      if(num == 0) 
        num = load_s16(ramPnt, progPnt->Pnt_ChCntr1);
      else
        num = load_s16(ramPnt, progPnt->Pnt_ChCntr2);
      
      clrBitMicro(ramPnt, progPnt->Pnt_En_LC);   //!< Отключить управление местным пультом
      clrBitMicro(ramPnt, progPnt->Pnt_En_WIFI); //!< Отключить управление по WiFi
      clrBitMicro(ramPnt, progPnt->Pnt_En_RC);   //!< Отключить управление по каналу АСУ
      clrBitMicro(ramPnt, progPnt->Pnt_En_Din);  //!< Отключить управление по дискретным входам
      
      switch((srcControl_type)num){
      case LC_sc:
        setBitMicro(ramPnt, progPnt->Pnt_En_LC);   //!< Включить управление местным пультом
        break;
      case WIFI_sc:
        setBitMicro(ramPnt, progPnt->Pnt_En_WIFI);  //!< Включить управление по WiFi
        break;
      case RC_sc:
        setBitMicro(ramPnt, progPnt->Pnt_En_RC);   //!< Включить управление по каналу АСУ
        break;
      case Din_sc:
        setBitMicro(ramPnt, progPnt->Pnt_En_Din);  //!< Включить управление по дискретным входам
        break;
        
      default:
        break;
      } 
      
      save_s16 (ramPnt, progPnt->Pnt_ActiveCntrl, (u16)num);      
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCONTROL_V2_1[10]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  ChCntr1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  ChCntr2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  UstSel
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  DinSel
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  PwmEn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //6  En_LC
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //7  En_RC
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  En_Din
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //9  EnSleep
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //10 Active Control
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONTROL_V2", "DA", TblFuncCONTROL_V2_1, FuncCONTROL_V2_1, sizeof(TblFuncCONTROL_V2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONTROL_V2_1,
#endif
//********************************************************
