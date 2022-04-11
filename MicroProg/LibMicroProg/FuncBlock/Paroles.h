#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_P1_In;                         //1  введенный пароль 1 
    u16  Pnt_P1_OK;                         //2  правильный пароль 1
    u16  Pnt_P2_In;                         //3  введенный пароль 2
    u16  Pnt_P2_OK;                         //4  правильный пароль 2
    u16  Pnt_P3_In;                         //5  введенный пароль 3
    u16  Pnt_P3_OK;                         //6  правильный пароль 3
    u16  Pnt_End;
}FuncParoles_type;

typedef struct{
	u16     Paroles_P1_Etalon;
        u16     Paroles_P2_Etalon;
        u16     Paroles_P3_Etalon;
        u16     crc;
}nvFuncParoles_1;

typedef struct{
	u16     Paroles_P1_Etalon;
        u16     Paroles_P2_Etalon;
        u16     Paroles_P3_Etalon;
}stateFuncParoles_1;

#define         nvParolesFunc    (*(nvFuncParoles_1*)0)
//********************************************************
u16 *FuncParoles_1(FuncParoles_type *progPnt, u32 ramPnt)
{ 
    nvFuncParoles_1     *nvAdr, nvMem;
    stateFuncParoles_1  *sPnt;
    u16                 Paroles_P1_Etalon, Paroles_P2_Etalon, Paroles_P3_Etalon;
    u16                 nvSave, crc;
    
    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvParolesFunc);
    sPnt  = (stateFuncParoles_1 *)(ramPnt + progPnt->Pnt_State); 
    
    nvSave = 0;
    Paroles_P1_Etalon = load_s16(ramPnt, progPnt->Pnt_P1_OK); 
    Paroles_P2_Etalon = load_s16(ramPnt, progPnt->Pnt_P2_OK);
    Paroles_P3_Etalon = load_s16(ramPnt, progPnt->Pnt_P3_OK);
    
    if(flagIsFirstCall)  //начальная инициализация и вычитка из FRAM параметра режима управления - AutoMode
    {
      RdFramMicro(&nvMem, nvAdr, sizeof(nvFuncParoles_1));
      crc = crcPacket((unsigned char *)&nvMem,  sizeof(nvFuncParoles_1) - sizeof(u16));
            
      if (crc != nvMem.crc)
      {
        nvMem.Paroles_P1_Etalon = Paroles_P1_Etalon;
        nvMem.Paroles_P2_Etalon = Paroles_P2_Etalon;
        nvMem.Paroles_P3_Etalon = Paroles_P3_Etalon;
        nvMem.crc               = crcPacket((unsigned char *)&nvMem,  sizeof(nvFuncParoles_1) - sizeof(u16));
        WrFramMicro(nvAdr, &nvMem, sizeof(nvFuncParoles_1));
      } 
      // переносим в ОЗУ текущие значения
      sPnt->Paroles_P1_Etalon = nvMem.Paroles_P1_Etalon;
      sPnt->Paroles_P2_Etalon = nvMem.Paroles_P2_Etalon;
      sPnt->Paroles_P3_Etalon = nvMem.Paroles_P3_Etalon;
      
      //  Переносим значения паролей из ФРАМКи 
      save_s16(ramPnt, progPnt->Pnt_P1_OK, nvMem.Paroles_P1_Etalon); 
      save_s16(ramPnt, progPnt->Pnt_P2_OK, nvMem.Paroles_P2_Etalon); 
      save_s16(ramPnt, progPnt->Pnt_P3_OK, nvMem.Paroles_P3_Etalon); 
    } else { // В обычном режиме работы сверяем 
      if (Paroles_P1_Etalon != sPnt->Paroles_P1_Etalon)         nvSave = 1;
      if (Paroles_P2_Etalon != sPnt->Paroles_P2_Etalon)         nvSave = 1;
      if (Paroles_P3_Etalon != sPnt->Paroles_P3_Etalon)         nvSave = 1;
        
      // Если выявлено изменение паролей, сохраняем  
      if (nvSave == 1) {
        // Если режим поменяли, надо его сохранить
        nvMem.Paroles_P1_Etalon = Paroles_P1_Etalon;
        nvMem.Paroles_P2_Etalon = Paroles_P2_Etalon;
        nvMem.Paroles_P3_Etalon = Paroles_P3_Etalon;
        nvMem.crc               = crcPacket((unsigned char *)&nvMem,  sizeof(nvFuncParoles_1) - sizeof(u16));
        WrFramMicro(nvAdr, &nvMem, sizeof(nvFuncParoles_1));
      }
    }
        
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncParoles_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  A????
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  B????
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  A????
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  B????
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  A????
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  B????
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("PAROLES", "DD", TblFuncParoles_1, FuncParoles_1, sizeof(TblFuncParoles_1), sizeof(stateFuncParoles_1), sizeof(nvFuncParoles_1), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncParoles_1,
#endif
