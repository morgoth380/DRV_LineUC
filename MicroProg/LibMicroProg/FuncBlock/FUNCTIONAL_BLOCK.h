#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_FuncIn;                       //1  FuncIn
    u16  Pnt_PumpFuncEn;                   //2  PumpFuncEn
    u16  Pnt_LiftFuncEn;                   //3  LiftFuncEn
    u16  Pnt_N3FuncEn;                     //4  N3FuncEn
    u16  Pnt_N4FuncEn;                     //5  N4FuncEn
    u16  Pnt_N5FuncEn;                     //6  N5FuncEn
    u16  Pnt_N6FuncEn;                     //7  N6FuncEn
    u16  Pnt_N7FuncEn;                     //8  N7FuncEn
    u16  Pnt_N8FuncEn;                     //9  N8FuncEn
    u16  Pnt_FuncOut;                      //10  FuncOut
    u16  Pnt_End;
}FuncFUNCTIONAL_BLOCK_type;

enum {
     nvPumpFunctional = 1<<0,
     nvLiftFunctional = 1<<1,
     
     nvFullFunctionalMask = 0x03, // Разрешенные для изменения функционалы
} VFD_Functional_enum;

//typedef enum{
//	UsePumpFunc = 0,    
//	UseLiftFunc = 1,     
//}AT24Functional_type;
//********************************************************
u16 *FuncFUNCTIONAL_BLOCK_1(FuncFUNCTIONAL_BLOCK_type *progPnt, u32 ramPnt)
{
  // Блок проверяет доступность функционала в ПО.
  
  AT24Functional_type   Functional;
  FileFunctional_type 	FlagsFunctional, FlagsFunctionalPrev;
  u16			crc;
  Functional_type 	*fPnt;  
  u16                   LiftFuncEn; // PumpFuncEn
      
  Functional = (AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_FuncIn);  

  // Проверяем - разрешен ли функционал
  //PumpFuncEn = load_s16(ramPnt, progPnt->Pnt_PumpFuncEn);
  LiftFuncEn = load_s16(ramPnt, progPnt->Pnt_LiftFuncEn);
  
  // Проверяем функционалы
  switch (Functional) {
  case UseLiftFunc:
    if (!LiftFuncEn) {
      Functional = UsePumpFunc; // Если функционал запрещен - возвращаем к базовому насосу.
      save_s16(ramPnt, progPnt->Pnt_FuncIn, Functional);       
    }
    break;

  case UsePumpFunc:
  default:
    Functional = UsePumpFunc; // Если функционал запрещен - возвращаем к базовому насосу.    
    break;    
  }

  // Обновляем данные в файле функционала
  GlobalM4.PresentFunctional = Functional; 
  // Выдаем на выход фактический функционал.
  save_s16(ramPnt, progPnt->Pnt_FuncOut, Functional); 
          
  fPnt = &curFunct;
  fPnt->functMask = 0;
  RdFramMicro(&FlagsFunctional, &nv.fileCurFunct, sizeof(FileFunctional_type));
  RdFramMicro(&FlagsFunctionalPrev, &nv.fileCurFunctPrev, sizeof(FileFunctional_type));
  crc = (1<<((u16)(Functional)));
  
  if( (FlagsFunctional.curFunct.functMask & (u16)nvFullFunctionalMask) != (1<<Functional)) {
    // Вытираем маской доступные биты функционала и добавляем новый
    FlagsFunctional.curFunct.functMask = (FlagsFunctional.curFunct.functMask & (u16)(~nvFullFunctionalMask));
    crc = (1<<((u16)(Functional)));
    crc = (crc & (u16)nvFullFunctionalMask);
    FlagsFunctional.curFunct.functMask |= ((1<<Functional) & (u16)nvFullFunctionalMask);
    crc = crcPacket((unsigned char *) &FlagsFunctional, sizeof(FlagsFunctional.curFunct)); 
    FlagsFunctional.crc = crc;
    WrFramMicro(&nv.fileCurFunct, &FlagsFunctional, sizeof(FileFunctional_type));      
  }

  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFUNCTIONAL_BLOCK_1[10]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  FuncIn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  PumpFuncEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  LiftFuncEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  N3FuncEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  N4FuncEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  N5FuncEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  N6FuncEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  N7FuncEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  N8FuncEn
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //10  FuncOut
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FUNCTIONAL_BLOCK", "DD", TblFuncFUNCTIONAL_BLOCK_1, FuncFUNCTIONAL_BLOCK_1, sizeof(TblFuncFUNCTIONAL_BLOCK_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFUNCTIONAL_BLOCK_1,
#endif
//********************************************************
