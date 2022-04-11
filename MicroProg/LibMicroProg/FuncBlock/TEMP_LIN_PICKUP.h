#ifdef    GET_FUNC_CODE

typedef struct{
	union{
        _iq     InternalRef;
        f32     fInternalRef;
    	};
    u32  CountTime;
}TempLinState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In;                           //1  In
    u16  Pnt_TempUp;                       //2  TempUp
    u16  Pnt_TempDw;                       //3  TempDw
    u16  Pnt_ResVal;                       //4  ResVal
    u16  Pnt_Freze;                        //5  Freze
    u16  Pnt_Res;                          //6  Res
    u16  Pnt_FPickUp;                      //7  FPickUp
    u16  Pnt_PickUpIsOn;                   //8  PickUpIsOn
    u16  Pnt_Status;                       //9  Status
    u16  Pnt_Out;                          //10  Out
    u16  Pnt_Delay;                        //11  Delay
	u16  Pnt_End;
}FuncTEMP_LIN_PICKUP_type;
//********************************************************

#define CYCLE_MICRO_PROG   4 //Период микропрограммы

u16 *FuncTEMP_LIN_PICKUP_1(FuncTEMP_LIN_PICKUP_type *progPnt, u32 ramPnt)
{
    TempLinState_type   *sPnt;
    _iq                 TempUp, TempDw;
    _iq                 Ref, delta, diff;
    s16                 Freze, StateRefFlags, res;
    u16                 Time;

    sPnt = (TempLinState_type *)(ramPnt + progPnt->Pnt_State);
   
	if( FlagIsFirstCall() )                        //начальная инициализация
    {
		sPnt->CountTime = 0;                       //ссбрасываем счетчик
	}

	if(testBitMicro(ramPnt, progPnt->Pnt_Res))  //Если привод в работе то считаем
	{
		sPnt->CountTime++;
	}
	else                                        //Если привод в останове то сбрасываем счётчик
	{
		sPnt->CountTime = 0;
	}
	
	Time = (load_s16(ramPnt, progPnt->Pnt_Delay) * 100) / CYCLE_MICRO_PROG;   //Переводим секунды в количество циклов микропрограммы
	
	if(sPnt->CountTime >= Time)
	{
		sPnt->CountTime = Time;                              //для предотвращения переполнения счетчика
		Freze = testBitMicro(ramPnt, progPnt->Pnt_Freze);    //снимаем програмную блокировку с генератора темпа
	}
	else
	{
		Freze = 1; //Останавливаем генератор темпа
	}
	
	
	
	StateRefFlags = 0;
    res = testBitMicro(ramPnt, progPnt->Pnt_Res);

    if(testBitMicro(ramPnt, progPnt->Pnt_PickUpIsOn) == 0){
        TempUp = (_iq)load_s32(ramPnt, progPnt->Pnt_TempUp);
        TempUp = _IQmpy(TempUp, _IQ(FLOAT_TIME_CALL_MICRO));

        TempDw = (_iq)load_s32(ramPnt, progPnt->Pnt_TempDw);
        TempDw = _IQmpy(TempDw, _IQ(FLOAT_TIME_CALL_MICRO));
        if(TempUp == 0)     TempUp = 1;
        if(TempDw == 0)     TempDw = 1;
        
        Ref   = (_iq)load_s32(ramPnt, progPnt->Pnt_In);
        if (Ref<0)      StateRefFlags |= SRFLAG_REVREF;
        
		//Freze = testBitMicro(ramPnt, progPnt->Pnt_Freze);
        if(res == 0){                        //Генератор темпа остановлен
            if( Ref >= _IQ(0) ){
                sPnt->InternalRef = load_s32(ramPnt, progPnt->Pnt_ResVal);
            }
            else{
                sPnt->InternalRef = - load_s32(ramPnt, progPnt->Pnt_ResVal);
            }
                
            StateRefFlags |= SRFLAG_RESET;
        }
        else if(Freze != 0){                 //Генератор темпа на паузе
            StateRefFlags |= SRFLAG_FREZE;
        }
        else{                                //Генератор темпа в работе  
            diff = Ref - sPnt->InternalRef;
            
            if(sPnt->InternalRef >= 0)  delta = (diff < 0) ? -TempDw : TempUp;
            else{
                delta = (diff < 0) ? -TempUp : TempDw;
                StateRefFlags |= SRFLAG_REVCUR;
            }
            
            if(_IQabs(diff) < _IQabs(delta)){
                sPnt->InternalRef = Ref;
                StateRefFlags |= SRFLAG_EqRef;
            }
            else{
                sPnt->InternalRef += delta;
            }
            
            if(diff != 0){
                if(delta<0) StateRefFlags |= SRFLAG_DOWN;
                if(delta>0) StateRefFlags |= SRFLAG_UP;
            }
        }
        
        if(sPnt->InternalRef != 0)      StateRefFlags |= SRFLAG_NONZERO;
    }
    else{ // Включен подхват
        StateRefFlags |= SRFLAG_PickUp;
		mtoc.systemControl.FOutStart = load_s32(ramPnt, progPnt->Pnt_ResVal);
        sPnt->InternalRef = load_s32(ramPnt, progPnt->Pnt_FPickUp); // Прозрачно при подхвате
    }

    if(res == 0) { // Обнулить в останове
    	save_s32(ramPnt, progPnt->Pnt_Out, 0);
    }
    else{
    	save_s32(ramPnt, progPnt->Pnt_Out, sPnt->InternalRef);
    }

    save_s16(ramPnt, progPnt->Pnt_Status, StateRefFlags);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_LIN_PICKUP_1[11]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  TempUp
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  TempDw
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  ResVal
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Freze
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Res
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  FPickUp
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  PickUpIsOn
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Status
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Out
		S16_PIN_TYPE | OUTPUT_PIN_MODE     //11  Delay
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEMP_LIN_PICKUP", "DA", TblFuncTEMP_LIN_PICKUP_1, FuncTEMP_LIN_PICKUP_1, sizeof(TblFuncTEMP_LIN_PICKUP_1), sizeof(TempLinState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_LIN_PICKUP_1,
#endif
//********************************************************
