#ifdef    GET_FUNC_CODE

typedef struct{
	
	s16 	t_recycle;
	s16		t_restart;
	s16		t_nextstart;
	s16		utime;
	s16		t_stop;
	s16		t_start;
	s16     flag_sw;
	s16     t_stopstart;

	
}FuncSHORT_CYCLE_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PWM_on;                       //1  PWM_on
    u16  Pnt_Utime;                        //2  Utime
    u16  Pnt_Mode;                         //3  Mode
    u16  Pnt_T_recycle;                    //4  T_recycle
    u16  Pnt_T_restart;                    //5  T_restart
    u16  Pnt_Failure;                      //6  Failure
    u16  Pnt_End;
}FuncSHORT_CYCLE_type;
//********************************************************
u16 *FuncSHORT_CYCLE_1(FuncSHORT_CYCLE_type *progPnt, u32 ramPnt)
{
      FuncSHORT_CYCLE_State_type	 *sPnt;
	
	s16 mode, utime, sw;
	
	
	sPnt = (FuncSHORT_CYCLE_State_type *)(ramPnt + progPnt->Pnt_State);
	
	mode  = load_s16(ramPnt, progPnt->Pnt_Mode);//вычитываем состояние уставки вкл/выкл
	
	if (mode){
		utime = load_s16(ramPnt, progPnt->Pnt_Utime);
		sw = testBitMicro(ramPnt, progPnt->Pnt_PWM_on);

		if (flagIsFirstCall){ //Первый запуск
			sPnt->t_recycle = load_s16(ramPnt, progPnt->Pnt_T_recycle)* 60 ;
			sPnt->t_restart = load_s16(ramPnt, progPnt->Pnt_T_restart)* 60 ;
			sPnt->t_stop = 0;
			sPnt->t_nextstart = 0;
			sPnt->flag_sw = 0;
		}
		//================== ловим старт
		if ((sw == 1)&&(sPnt->flag_sw == 0)){	//если привод в работе
			utime = load_s16(ramPnt, progPnt->Pnt_Utime);
			sPnt->t_recycle = load_s16(ramPnt, progPnt->Pnt_T_recycle)* 60;
			sPnt->t_start = utime;//следующее время пуска
			sPnt->t_nextstart = utime + sPnt->t_recycle;//следующее время пуска
			sPnt->flag_sw = 1;
		}
		//================== ловим стопы
		if ((sw != 1)&&(sPnt->flag_sw == 1)){//если привод в останове
			utime = load_s16(ramPnt, progPnt->Pnt_Utime);
			sPnt->t_restart = load_s16(ramPnt, progPnt->Pnt_T_restart)* 60 ;
			sPnt->t_stop = utime;//время останова
			sPnt->t_stopstart = sPnt->t_stop + sPnt->t_restart;//время останова
			sPnt->flag_sw = 0;
		}
		//==================
		if (sPnt->t_stop > sPnt->t_start){
			if ((sPnt->t_stop < utime)&&(sPnt->t_nextstart > utime)){
				setBitMicro(ramPnt, progPnt->Pnt_Failure);
			}
			else if ((sPnt->t_stop < utime)&&(sPnt->t_stopstart > utime)){
				setBitMicro(ramPnt, progPnt->Pnt_Failure);
			}
			else{
				clrBitMicro(ramPnt, progPnt->Pnt_Failure);
			}
		}
	}
	else{
		clrBitMicro(ramPnt, progPnt->Pnt_Failure);
	}
   
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSHORT_CYCLE_1[6]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  PWM_on
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Utime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Mode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  T_recycle
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  T_restart
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //6  Failure
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SHORT_CYCLE", "DA", TblFuncSHORT_CYCLE_1, FuncSHORT_CYCLE_1, sizeof(TblFuncSHORT_CYCLE_1), sizeof(FuncSHORT_CYCLE_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSHORT_CYCLE_1,
#endif
//********************************************************
