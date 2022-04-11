#ifdef    GET_FUNC_CODE

typedef struct{
    float	TempVal;
}REFCONTROL_Data_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Fref;                         //1  Fref
    u16  Pnt_DinRevers;                    //2  DinRevers
    u16  Pnt_UstRevOff;                    //3  UstRevOff
    u16  Pnt_Start;                        //4  Start
    u16  Pnt_FreqStop;                     //5  FreqStop
    u16  Pnt_DCHold;                       //6  DCHold
    u16  Pnt_EvacOn;                       //7  EvacOn
    u16  Pnt_EvacDirMode;                  //8  EvacDirMode
    u16  Pnt_RevOff;                       //9  RevOff
    u16  Pnt_Sign_F;                       //10 Sign_F
    u16  Pnt_Fout;                         //11 Fout
    u16  Pnt_End;
}FuncREF_CONTROL_type;

typedef enum{
  LightDirection = 0,
  StationDirection = 1,
  ForwardDirection = 2,  
  BackwardDirection = 3,  
}EvacDirSet_enum;

//********************************************************
u16 *FuncREF_CONTROL_1(FuncREF_CONTROL_type *progPnt, u32 ramPnt)
{
    REFCONTROL_Data_type	*sPnt;    
    EvacDirSet_enum             EvacDirMode;
    u16                         EvacOn;
    
    sPnt    = (REFCONTROL_Data_type *)(ramPnt + progPnt->Pnt_State);
    EvacDirMode = (EvacDirSet_enum)load_s16(ramPnt, progPnt->Pnt_EvacDirMode);
    EvacOn      = testBitMicro(ramPnt, progPnt->Pnt_EvacOn);
    
    if((testBitMicro(ramPnt, progPnt->Pnt_Start   ))||     /*Обратная логика, Pnt_Start == 1 - шим отключен*/
       (testBitMicro(ramPnt, progPnt->Pnt_DCHold  ))||     /*Активно удержание пост. током */
         (testBitMicro(ramPnt, progPnt->Pnt_FreqStop))){	 /*Выполняется частотный останов */
		sPnt->TempVal = 0;
    } else{ 
      sPnt->TempVal = load_float(ramPnt, progPnt->Pnt_Fref);
    }
    
    // Проверяем эвакуацию.
    // Если активна эвакуация и выбран режим Легкого направления, блокируем сигнал реверса, чтоб не мешал
    // Направление сформирует блок RAMP_CNTRL_MODE2
    if (EvacOn && (EvacDirMode == LightDirection)) {
      sPnt->TempVal = sPnt->TempVal;
    } else {
      if( load_s16(ramPnt, progPnt->Pnt_DinRevers)){
        sPnt->TempVal = -sPnt->TempVal;
      }
    }

    if((sPnt->TempVal < 0) && load_s16(ramPnt, progPnt->Pnt_UstRevOff)){
        sPnt->TempVal = 0;
        setBitMicro(ramPnt, progPnt->Pnt_RevOff);
    }
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_RevOff);
    }
    
    if(sPnt->TempVal < 0){
        setBitMicro(ramPnt, progPnt->Pnt_Sign_F);
    }
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_Sign_F);
    }
	    
    save_float(ramPnt, progPnt->Pnt_Fout, sPnt->TempVal);	
	
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncREF_CONTROL_1[11]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Fref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  DinRevers
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  UstRevOff
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Start
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  FreqStop
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  DCHold
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  EvacOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  EvacDirMode
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  RevOff
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10 Sign_F
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //11 Fout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("REF_CONTROL", "DD", TblFuncREF_CONTROL_1, FuncREF_CONTROL_1, sizeof(TblFuncREF_CONTROL_1), sizeof(REFCONTROL_Data_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncREF_CONTROL_1,
#endif
//********************************************************
