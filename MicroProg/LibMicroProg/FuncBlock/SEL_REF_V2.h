#ifdef    GET_FUNC_CODE


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_SelRef1;                      //1  SelRef1
    u16  Pnt_SelRef2;                      //2  SelRef2
    u16  Pnt_UstSel;                       //3  UstSel
    u16  Pnt_DinSel_1_2;                   //4  DinSel_1_2
    u16  Pnt_In0_15[16];                   //5-20  In0_15
    u16  Pnt_PwmEn;                        //21  PwmEn
    u16  Pnt_Out;                          //22  Out
    u16  Pnt_CH_REF_LP;                    //23  CH_REF_LP
    u16  Pnt_CH_REF_RP;                    //24  CH_REF_RP
    u16  Pnt_CH_PID;                       //25  CH_PID
    u16  Pnt_GT_type;                      //26  GT_TYPE
    u16  Pnt_CH_Din;                       //27 CH_Din
    u16  Pnt_ActiveRef;                    //28  Active Ref
    u16  Pnt_End;
}FuncSEL_REF_V2_type;



enum{
	selCh1,
	selCh2,
	setDinSel
};

enum{
  	chanel_LP       = 0,
	chanel_WIFI     = 1,
	chanel_RP       = 2,
	chanel_DI       = 3,
	chanel_A1       = 4,
	chanel_A2       = 5,
	chanel_PID      = 6,
        chanel_UP_DOWN  = 7,
	chanel_A3       = 8,
        chanel_A4       = 9,        
};
typedef enum {
	LinearMode = 0,
	S_curveMode = 1,
} GT_enum;


//********************************************************
typedef struct{
        u16       activeSelRef; 
}SEL_REF_V2_State_type;

//********************************************************
u16 *FuncSEL_REF_V2_1(FuncSEL_REF_V2_type *progPnt, u32 ramPnt)
{
    float   Out;
    s16     selRef;
    SEL_REF_V2_State_type 	*sPnt;
    
    selRef = 0;
    sPnt = (SEL_REF_V2_State_type *)(ramPnt + progPnt->Pnt_State);
    
    // Если ШИМ выключен - можно менять каналы
    if (testBitMicro (ramPnt, progPnt->Pnt_PwmEn) == 0) {
      switch(load_s16(ramPnt, progPnt->Pnt_UstSel)){                              // Получаем номер канала задания (1-й, 2-й или задается состоянием дискретного входа)
        case(selCh1):{                         // Если канал задания имеет номер 1 (selCh1 = 0)
          selRef = load_s16(ramPnt, progPnt->Pnt_SelRef1);       // Считываем конкретный тип источника задания канала 1
          //Out = load_float(ramPnt, progPnt->Pnt_In0_15[selRef]); // По полученному номеру считываем собственно значение задания, то есть значение частоты
        }break;
        case(selCh2):{                         // Если канал задания имеет номер 2 (selCh2 = 1)
          selRef = load_s16(ramPnt, progPnt->Pnt_SelRef2);       // Считываем конкретный тип источника задания канала 2
          //Out = load_float(ramPnt, progPnt->Pnt_In0_15[selRef]);
        }break;
        case(setDinSel):{                      // Если номер активного канала задания задается состоянием одного из дискретных входов (setDinSel = 2), то проверяем какой канал задания выбран
          if((testBitMicro(ramPnt, progPnt->Pnt_DinSel_1_2)) == 0){   // Если выбран 1-й канал
            selRef = load_s16(ramPnt, progPnt->Pnt_SelRef1);         // Считываем конкретный тип источника задания канала 1
            //Out = load_float(ramPnt, progPnt->Pnt_In0_15[selRef]);   // По полученному номеру считываем собственно значение задания, то есть значение частоты
          }
          else{                                                       // Если выбран второй канал                           
            selRef = load_s16(ramPnt, progPnt->Pnt_SelRef2);         // Считываем конкретный тип источника задания канала 2
            //Out = load_float(ramPnt, progPnt->Pnt_In0_15[selRef]);   // По полученному номеру считываем собственно значение задания, то есть значение частоты
          }
        }break;
      default:{
        Out = 0;
      }
      }
      sPnt->activeSelRef = selRef; // Запоминаем активный канал
    }
      
    Out = load_float(ramPnt, progPnt->Pnt_In0_15[sPnt->activeSelRef]);   // По полученному активному каналу берем задание частоты
    
    if(sPnt->activeSelRef == chanel_PID){      // Если тип сигнала задания - ПИД регулятор
      clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_LP);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_RP);
      setBitMicro(ramPnt, progPnt->Pnt_CH_PID);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_Din);                      
    }
    else if(sPnt->activeSelRef == chanel_RP){  // Если тип сигнала задания - АСУ
      clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_LP);
      setBitMicro(ramPnt, progPnt->Pnt_CH_REF_RP);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_PID);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_Din);                            
    } else if (sPnt->activeSelRef == chanel_DI) {
      clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_LP);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_RP);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_PID);
      setBitMicro(ramPnt, progPnt->Pnt_CH_Din);  
    } else{ // Pnt_CH_REF_LP иначе, тип источника задания - или аналогоые входы, или дискретные входы, или местный пульт               
      setBitMicro(ramPnt, progPnt->Pnt_CH_REF_LP);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_RP);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_PID);
      clrBitMicro(ramPnt, progPnt->Pnt_CH_Din);                            
    }
    
    // Если задание от аналогового входа, то меняем тип генератора темпа
    if ((sPnt->activeSelRef == chanel_A1) || (sPnt->activeSelRef == chanel_A2) || (sPnt->activeSelRef == chanel_PID) || 
        (sPnt->activeSelRef == chanel_UP_DOWN) || (sPnt->activeSelRef == chanel_A3) || (sPnt->activeSelRef == chanel_A4)) {
      save_s16 (ramPnt, progPnt->Pnt_GT_type, LinearMode);
    }
    save_float(ramPnt, progPnt->Pnt_Out, Out);  // Выдаем в микропрограмму задание частоты
    
    // Выводим текущий активный канал задания
    save_s16 (ramPnt, progPnt->Pnt_ActiveRef, sPnt->activeSelRef);
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSEL_REF_V2_1[28]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  SelRef1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  SelRef2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  UstSel
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  DinSel_1_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Local_0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Local_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Asu
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Din
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  Ain1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  Ain2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  Ain3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  Ain4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  Ain5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  Kalendar
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  Cascade
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  UpDown
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  FireFixed
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  In13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19  In14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  In15
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  PwmEn        
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  CH_REF_LP
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  CH_REF_RP
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  CH_PID
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  GT_TYPE
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //27  CH_Din
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //28  Active Ref
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SEL_REF_V2", "DA", TblFuncSEL_REF_V2_1, FuncSEL_REF_V2_1, sizeof(TblFuncSEL_REF_V2_1), sizeof(SEL_REF_V2_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSEL_REF_V2_1,
#endif
//********************************************************
