#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_StartPump;                    //1  StartPump
    u16  Pnt_StopPump;                     //2  StopPump
    u16  Pnt_StartLift;                    //3  StartLift
    u16  Pnt_StopLift;                     //4  StopLift
    u16  Pnt_Start3;                       //5  Start3
    u16  Pnt_Stop3;                        //6  Stop3
    u16  Pnt_Start4;                       //7  Start4
    u16  Pnt_Stop4;                        //8  Stop4
    u16  Pnt_Functional;                   //9  Functional
    u16  Pnt_StopOut;                      //10  StopOut
    u16  Pnt_StartOut;                     //11  StartOut
    u16  Pnt_End;
}FuncSTART_STOP_MUX_type;

//********************************************************
u16 *FuncSTART_STOP_MUX_1(FuncSTART_STOP_MUX_type *progPnt, u32 ramPnt)
{
   // Модуль формирования команд Старт и Стоп, в зависимости от выбранного функционала.
   // Чтобы не было конфликтов между командоаппаратами
  AT24Functional_type   Functional;
  
  Functional = (AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_Functional);
  
  switch (Functional) {
      case UsePumpFunc:        
        tranzitBitMicro (ramPnt, progPnt->Pnt_StartOut, progPnt->Pnt_StartPump);
        tranzitBitMicro (ramPnt, progPnt->Pnt_StopOut, progPnt->Pnt_StopPump);
        break;

      case UseLiftFunc:        
        tranzitBitMicro (ramPnt, progPnt->Pnt_StartOut, progPnt->Pnt_StartLift);
        tranzitBitMicro (ramPnt, progPnt->Pnt_StopOut, progPnt->Pnt_StopLift);
        break;
        
      default:        
        tranzitBitMicro (ramPnt, progPnt->Pnt_StartOut, progPnt->Pnt_StartPump);
        tranzitBitMicro (ramPnt, progPnt->Pnt_StopOut, progPnt->Pnt_StopPump);
        break;
  }
  
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSTART_STOP_MUX_1[11]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  StartPump
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  StopPump
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  StartLift
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  StopLift
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Start3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Stop3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Start4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Stop4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Functional
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  StopOut
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //11  StartOut
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("START_STOP_MUX", "DD", TblFuncSTART_STOP_MUX_1, FuncSTART_STOP_MUX_1, sizeof(TblFuncSTART_STOP_MUX_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSTART_STOP_MUX_1,
#endif
//********************************************************
