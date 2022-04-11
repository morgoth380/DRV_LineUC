#ifdef    GET_FUNC_CODE

typedef struct{
    s16     StartOK;
}CURR_CONTR_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_I_set;                        //1  I_set
    u16  Pnt_I_lim_strt;                   //2  I_lim_strt
    u16  Pnt_I_nom;                        //3  I_nom
    u16  Pnt_Ilim_EN;                      //4  Ilim_EN
    u16  Pnt_Mode_I_F;                     //5  Mode_I_F
    u16  Pnt_I_full;                       //6  I_full
    u16  Pnt_F_MAX;                        //7  F_MAX
    u16  Pnt_F_MIN;                        //8  F_MIN
    u16  Pnt_F_in;                         //9  F_in
    u16  Pnt_F_strt;                       //10  F_strt
    u16  Pnt_SignF;                        //11  SignF
    u16  Pnt_GT_UP;                        //12  GT_UP
    u16  Pnt_PWM_ON;                       //13  PWM_ON
    u16  Pnt_STOP_FR;                      //14  STOP_FR
    u16  Pnt_F_ref;                        //15  F_ref
    u16  Pnt_End;
}FuncCURR_CONTR_type;
//********************************************************
u16 *FuncCURR_CONTR_1(FuncCURR_CONTR_type *progPnt, u32 ramPnt)
{
/*	CURR_CONTR_State_type   *sPnt;
	COMMONPARAMM3			*mPnt3;
	s16						sign;
	s32						Fst, Fmax, Fmin, Fref;
	s32						Iset, Ilim, Inom, Ifull;
	
	sPnt = (CURR_CONTR_State_type *)(ramPnt + progPnt->Pnt_State);		// флажок завершения пуска
	if(testBitMicro(ramPnt, progPnt->Pnt_PWM_ON) == 0){					// работа запрещена
		save_s32(ramPnt, progPnt->Pnt_F_ref, (s32)0);
        sPnt->StartOK = 0;
		return &progPnt->Pnt_End;	
    }
    if(testBitMicro(ramPnt, progPnt->Pnt_STOP_FR) ){					// частотное торможение активировано
		save_s32(ramPnt, progPnt->Pnt_F_ref, Fst);
		return &progPnt->Pnt_End;
	}
    
	mPnt3 = &GlobalM3;
	Fst		= load_s32(ramPnt, progPnt->Pnt_F_strt);
	Fmin	= load_s32(ramPnt, progPnt->Pnt_F_MIN);
	Fmax	= load_s32(ramPnt, progPnt->Pnt_F_MAX)
	mPnt3->signRef = _IQ(1);
	if(testBitMicro(ramPnt, progPnt->Pnt_SignF)){					//Отрицательные частоты
		Fst = -Fst;
		Fmin= -Fmin;
		Fmax= -Fmax;
		mPnt3->signRef = _IQ(-1);
	}
	Ifull	= load_s32(ramPnt, progPnt->Pnt_I_full);
	Inom	= load_s32(ramPnt, progPnt->Pnt_I_nom);
	Ilim	= _IQmpy( load_s32(ramPnt, progPnt->Pnt_I_lim_strt), Inom);
	Iset	= _IQmpy( load_s32(ramPnt, progPnt->Pnt_I_set), Inom);
	Fref	= load_s32(ramPnt, progPnt->Pnt_F_in);
	if(load_s16(ramPnt, progPnt->Pnt_Mode_I_F) == 0){								// опрделение режима задания ток/частота - частота = 0; ток = 1;
	    if((sPnt->StartOK == 0) && (load_s16(ramPnt, progPnt->Pnt_Ilim_EN) == 1)){	// проверка на завершение запуска и активность режима токоограничения при пуске
	        if((testBitMicro(ramPnt, progPnt->Pnt_GT_UP) == 0)){					// Fref = Fin
				sPnt->StartOK = 1;
	        }else{
	            if(Ilim < Ifull){													// если идет запуск
	                Fref = Fst;
	            }
	        }
	    }
	}else{	// режим задания - ток
	    if((sPnt->StartOK == 0) && (load_s16(ramPnt, progPnt->Pnt_Ilim_EN) == 1)){	// проверка на завершение запуска и активность режима токоограничения при пуске
	        if(testBitMicro(ramPnt, progPnt->Pnt_GT_UP) == 0){						// проверка выхода на режим
	            sPnt->StartOK = 1;
	        }else{
	            if(Ilim < Ifull){													// если идет запуск сравниваем выходной ток с током ограничения пуска
	                Fref = Fst;
	            }
	        }
	    }else{																		// запуск завершен, либо отключено токоограничение при пуске
	        if (Iset < Ifull){														// сравниваем выходной ток с током задания
				Fref = Fmin;														// если ток на выходе превышает уставку - выдаем минимальную частоту
	        }else{																	// если ток на выходе ниже уставки - выдаем максимальную частоту
				Fref = Fmax;
	        }
	    }
	}
    save_s32(ramPnt, progPnt->Pnt_F_ref, Fref);
*/    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCURR_CONTR_1[15]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  I_set
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  I_lim_strt
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  I_nom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Ilim_EN
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Mode_I_F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  I_full
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  F_MAX
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  F_MIN
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  F_in
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  F_strt
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  SignF
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  GT_UP
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  PWM_ON
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  STOP_FR
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //15  F_ref
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CURR_CONTR", "DA", TblFuncCURR_CONTR_1, FuncCURR_CONTR_1, sizeof(TblFuncCURR_CONTR_1), sizeof(CURR_CONTR_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCURR_CONTR_1,
#endif
//********************************************************
