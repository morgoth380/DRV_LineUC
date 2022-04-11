#ifdef    GET_FUNC_CODE

#define _OFF 			0
#define _ON_WARNING 	1
#define _ON_CRASH 		2
#define _ON_SLEEP 		3

#define _25PERCENT  _IQ(0.25f)		
#define _50PERCENT  _IQ(0.5f)
#define _75PERCENT  _IQ(0.75f)
#define LIM_PERCENT _IQ(0.001f)  //допуск 0.1%
#define _00_DOT_1PERCENT   1677
#define _100PERCENT 16777216
#define BELOW_ZERO	-666

#define F_delta	(F_max - F_min)
//**********************************************************
typedef struct{
        _iq         AT_P1;
        _iq         AT_P2;
		_iq         AT_P3;
		_iq         AT_P4;
		_iq         AT_P5;
		
}nvFuncNO_FLOW_DRY_RUN_State_type;
#define     nvSfFuncNO_FLOW_DRY_RUN_State_type     (*(nvFuncNO_FLOW_DRY_RUN_State_type*)0)
//**********************************************************
typedef struct{
	_iq		p1;
	_iq		p2;
	_iq		p3;
	_iq		p4;
	_iq		p5;
	
	u32 	counter;
	u32		counter_i; //i = idle = холостой ход
	u32		counter_sleep; // спящий режим
	u16		blink_sleep; // спящий режим


	u16     flag_sleep;
	u16     DriveOnPrevState;
	nvFuncNO_FLOW_DRY_RUN_State_type	nonvolatile_at;
}FuncNO_FLOW_DRY_RUN_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_F_TEK;                        //1  F_TEK
    u16  Pnt_P_ACT;                        //2  P_ACT
    u16  Pnt_Curve_P1;                     //3  Curve_P1
    u16  Pnt_Curve_P2;                     //4  Curve_P2
    u16  Pnt_Curve_P3;                     //5  Curve_P3
    u16  Pnt_Curve_P4;                     //6  Curve_P4
    u16  Pnt_Curve_P5;                     //7  Curve_P5
    u16  Pnt_DRV_Fmax;                     //8  DRV_Fmax
    u16  Pnt_DRV_Fmin;                     //9  DRV_Fmin
    u16  Pnt_NF_delay;                     //10  NF_delay
    u16  Pnt_DR_delay;                     //11  DR_delay
    u16  Pnt_PWM_ON;                       //12  PWM_ON
    u16  Pnt_Mode_NF;                      //13  Mode_NF
    u16  Pnt_AutoTuning;                   //14  AutoTuning
    u16  Pnt_Mode_DR;                      //15  Mode_DR
    u16  Pnt_AT_perc;                      //16  AT_perc
    u16  Pnt_T_sleep;                      //17  T_sleep
    u16  Pnt_FC_FAIL;                      //18  FC_FAIL
    u16  Pnt_NF_SBlink;                    //19  NF_SBlink
    u16  Pnt_Drive_On;                     //20  Drive_On
    u16  Pnt_DR_Warning;                   //21  DR_Warning
    u16  Pnt_DR_Failure;                   //22  DR_Failure
    u16  Pnt_Stop_Sleep;                   //23  Stop_Sleep
    u16  Pnt_NF_SleepW;                    //24  NF_SleepW
    u16  Pnt_NF_Warning;                   //25  NF_Warning
    u16  Pnt_NF_Failure;                   //26  NF_Failure
    u16  Pnt_End;
}FuncNO_FLOW_DRY_RUN_type;
//********************************************************
u16 *FuncNO_FLOW_DRY_RUN_1(FuncNO_FLOW_DRY_RUN_type *progPnt, u32 ramPnt)
{	
	FuncNO_FLOW_DRY_RUN_State_type	 *sPnt;
	nvFuncNO_FLOW_DRY_RUN_State_type	 *nvAdr;
    
    _iq F_tek, Autotuning, F_min, F_max, mode;
	s32 P_min, P_act;
	u16 imode, DriveOnState;
    sPnt = (FuncNO_FLOW_DRY_RUN_State_type *)(ramPnt + progPnt->Pnt_State);
	nvAdr       = GetNvAdr(progPnt->Pnt_nvState, &nvSfFuncNO_FLOW_DRY_RUN_State_type);
	
    P_act = load_s32(ramPnt, progPnt->Pnt_P_ACT);
	mode = load_s16(ramPnt, progPnt->Pnt_Mode_NF);
    imode = load_s16(ramPnt, progPnt->Pnt_Mode_DR);
	//Первый вызов

	return &progPnt->Pnt_End;
	if (flagIsFirstCall){
		//работа с FRAM
		RdFramMicro(&sPnt->nonvolatile_at, nvAdr, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
		//Загружаем из FRAM в уставки
		save_s32(ramPnt, progPnt->Pnt_Curve_P1, sPnt->nonvolatile_at.AT_P1);				
		save_s32(ramPnt, progPnt->Pnt_Curve_P2, sPnt->nonvolatile_at.AT_P2);
		save_s32(ramPnt, progPnt->Pnt_Curve_P3, sPnt->nonvolatile_at.AT_P3);
		save_s32(ramPnt, progPnt->Pnt_Curve_P4, sPnt->nonvolatile_at.AT_P4);
		save_s32(ramPnt, progPnt->Pnt_Curve_P5, sPnt->nonvolatile_at.AT_P5);
		
		clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
		clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
		clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
		clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
		clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);// спящий режим
		clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);// спящий режим
		DriveOnState = 0;//clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);// спящий режим
		clrBitMicro(ramPnt, progPnt->Pnt_NF_SBlink);// спящий режим
		
		sPnt->p1 = sPnt->nonvolatile_at.AT_P1;
		sPnt->p2 = sPnt->nonvolatile_at.AT_P2;
		sPnt->p3 = sPnt->nonvolatile_at.AT_P3;
		sPnt->p4 = sPnt->nonvolatile_at.AT_P4;
		sPnt->p5 = sPnt->nonvolatile_at.AT_P5;
		
		sPnt->flag_sleep = 0;
		sPnt->blink_sleep = 0;
		sPnt->counter	 = 	 ( (s32) load_s16(ramPnt, progPnt->Pnt_NF_delay) * K_TIME_CALL_MICRO_TAKT);	//заряжаем таймер ООП
		sPnt->counter_i  = 	 ( (s32) load_s16(ramPnt, progPnt->Pnt_DR_delay) * K_TIME_CALL_MICRO_TAKT);	//заряжаем таймер Сухого Хода
		sPnt->counter_sleep  = 	  load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT;	//заряжаем таймер Cна // спящий режим
		sPnt->DriveOnPrevState = 0;
	}

	if(sPnt->p1 != load_s32(ramPnt, progPnt->Pnt_Curve_P1)){
			sPnt->p1 = load_s32(ramPnt, progPnt->Pnt_Curve_P1);
			sPnt->nonvolatile_at.AT_P1 = sPnt->p1;
			WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
		}	
	if(sPnt->p2 != load_s32(ramPnt, progPnt->Pnt_Curve_P2)){
			sPnt->p2 = load_s32(ramPnt, progPnt->Pnt_Curve_P2);
			sPnt->nonvolatile_at.AT_P2 = sPnt->p2;
			WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
		}	
	if(sPnt->p3 != load_s32(ramPnt, progPnt->Pnt_Curve_P3)){
			sPnt->p3 = load_s32(ramPnt, progPnt->Pnt_Curve_P3);
			sPnt->nonvolatile_at.AT_P3 = sPnt->p3;
			WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
		}	
	if(sPnt->p4 != load_s32(ramPnt, progPnt->Pnt_Curve_P4)){
			sPnt->p4 = load_s32(ramPnt, progPnt->Pnt_Curve_P4);
			sPnt->nonvolatile_at.AT_P4 = sPnt->p4;
			WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
		}	
	if(sPnt->p5 != load_s32(ramPnt, progPnt->Pnt_Curve_P5)){
			sPnt->p5 = load_s32(ramPnt, progPnt->Pnt_Curve_P5);
			sPnt->nonvolatile_at.AT_P5 = sPnt->p5;
			WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
		}	
//**********************************************Спящий режим
	if ((mode == _ON_SLEEP) && (sPnt->flag_sleep)){
		if(testBitMicro(ramPnt, progPnt->Pnt_FC_FAIL)){
			sPnt->flag_sleep = 0;
			clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);// спящий режим
			clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);// спящий режим
			clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);// спящий режим
			clrBitMicro(ramPnt, progPnt->Pnt_NF_SBlink);
			sPnt->counter_sleep  =  (load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT);
		}else{
			if (sPnt->counter_sleep == 0){
				clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);// спящий режим
				clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);// спящий режим
				clrBitMicro(ramPnt, progPnt->Pnt_NF_SBlink);

				sPnt->flag_sleep = 0;
				DriveOnState = 1;//setBitMicro(ramPnt, progPnt->Pnt_Drive_On);
				sPnt->counter_sleep  =  (load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT);

			}
			else{
				clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);// спящий режим
				sPnt->counter_sleep--;
				if (!(sPnt->counter_sleep % 250)){
					sPnt->blink_sleep =~ sPnt->blink_sleep;
					defBitMicro(ramPnt, progPnt->Pnt_NF_SBlink,sPnt->blink_sleep);
				}
			}
		}
	}
	//************
    if(testBitMicro(ramPnt, progPnt->Pnt_PWM_ON)){							//Если шим выкл, то ничего не делаем
		
//****************
		if (sPnt->flag_sleep){
			sPnt->flag_sleep = 0;
			clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
			clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
			DriveOnState = 0;//clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);
			clrBitMicro(ramPnt, progPnt->Pnt_NF_SBlink);
			sPnt->counter_sleep  =  (load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT);
		}
		//****************
		
		Autotuning = load_s16(ramPnt, progPnt->Pnt_AutoTuning);
		F_tek = _IQabs(load_s32(ramPnt, progPnt->Pnt_F_TEK));
		F_max = load_s32(ramPnt, progPnt->Pnt_DRV_Fmax);
		F_min = load_s32(ramPnt, progPnt->Pnt_DRV_Fmin);
			
		//************************************Автонастройка кривой*************************************
		if(Autotuning){					//Время разгона до 400 Гц = 60 сек!
			_iq Lim_Percent = _IQmpy(F_max, LIM_PERCENT);
			_iq	lim_coeff = load_s16(ramPnt, progPnt->Pnt_AT_perc);
			lim_coeff = lim_coeff * _00_DOT_1PERCENT;
		//При автонастройке игнорим аварии и варнинги от СХ и ОП и Спящий режим	
			clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);	
			clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
			clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
			clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
            clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);//
			clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);//
			DriveOnState = 0;//clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);//
			
			if( (F_tek > (F_min - Lim_Percent)) && (F_tek < (F_min + Lim_Percent)) ){							
				//F_tek = F_min +- 0.1%F_max
				sPnt->nonvolatile_at.AT_P1 = P_act + lim_coeff;  		// Измеренное значение * ЛИМ_коэфф!
			}
			else if( (F_tek > (_IQmpy(F_delta, _25PERCENT) + F_min - Lim_Percent)) && (F_tek < (_IQmpy(F_delta, _25PERCENT) + F_min + Lim_Percent)) ){
				//F_tek = 25%F_delta +- 0.1%F_max
				sPnt->nonvolatile_at.AT_P2 = P_act + lim_coeff;  		// Измеренное значение * ЛИМ_коэфф!
			}
			else if( (F_tek > (_IQmpy(F_delta, _50PERCENT) + F_min - Lim_Percent)) && (F_tek < (_IQmpy(F_delta, _50PERCENT) + F_min + Lim_Percent)) ){	
				//F_tek = 50%F_delta +- 0.1%F_max
				sPnt->nonvolatile_at.AT_P3 = P_act + lim_coeff;  		// Измеренное значение * ЛИМ_коэфф!
			}
			else if( (F_tek > (_IQmpy(F_delta, _75PERCENT) + F_min - Lim_Percent)) && (F_tek < (_IQmpy(F_delta, _75PERCENT) + F_min + Lim_Percent)) ){	
				//F_tek = 75%F_delta +- 0.1%F_max
				sPnt->nonvolatile_at.AT_P4 = P_act + lim_coeff;  		// Измеренное значение * ЛИМ_коэфф!
			}
			else if( (F_tek > (F_max - Lim_Percent)) && (F_tek < (F_max + Lim_Percent)) ){							
				//F_tek = F_max +- 0.1%F_max
				sPnt->nonvolatile_at.AT_P5 = P_act + lim_coeff;  		// Измеренное значение * ЛИМ_коэфф!
				WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));//Заливаем все намеряное в FRAM
				save_s16(ramPnt, progPnt->Pnt_AutoTuning, 0);	//кончилась автонастройка
				save_s32(ramPnt, progPnt->Pnt_Curve_P1, sPnt->nonvolatile_at.AT_P1);				
				save_s32(ramPnt, progPnt->Pnt_Curve_P2, sPnt->nonvolatile_at.AT_P2);
				save_s32(ramPnt, progPnt->Pnt_Curve_P3, sPnt->nonvolatile_at.AT_P3);
				save_s32(ramPnt, progPnt->Pnt_Curve_P4, sPnt->nonvolatile_at.AT_P4);
				save_s32(ramPnt, progPnt->Pnt_Curve_P5, sPnt->nonvolatile_at.AT_P5);
			}
#ifdef _PROJECT_FOR_CCS_
#warning    УСТАНОВКА ЗНАЧЕНИЯ DriveOnState=0, ТАК КАК РАНЕЕ ЭТА ПЕРЕМЕННАЯ НЕ ИНИЦИАЛИЗИРУЕТСЯ И НА ЭТО РУГАЕТСЯ Visual Studio. ЗНАЧЕНИЕ НАДО УСТАНОВИТЬ ПРАВДИВЫМ.
			DriveOnState = 0;
#endif

			defBitMicro(ramPnt, progPnt->Pnt_Drive_On, (s16)DriveOnState);
			sPnt->DriveOnPrevState = DriveOnState;
			return &progPnt->Pnt_End;
		}
		//*********************************************************************************************
		//************************************Система определения отсутствия потока********************
		//Получаем минимальное допустимое значение мощности на текущей частоте
		if (F_tek >= (_IQmpy(F_delta, _75PERCENT) + F_min)){
			P_min = iq_Fy_x1x2y1y2x((_IQmpy(F_delta, _75PERCENT) + F_min),F_max,sPnt->p4, sPnt->p5, F_tek);
		}
		else if(F_tek >= (_IQmpy(F_delta, _50PERCENT) + F_min)){
			P_min = iq_Fy_x1x2y1y2x((_IQmpy(F_delta, _50PERCENT) + F_min),(_IQmpy(F_delta, _75PERCENT) + F_min),sPnt->p3, sPnt->p4, F_tek);
		}
		else if(F_tek >= (_IQmpy(F_delta, _25PERCENT) + F_min)){
			P_min = iq_Fy_x1x2y1y2x((_IQmpy(F_delta, _25PERCENT) + F_min),(_IQmpy(F_delta, _50PERCENT)+ F_min),sPnt->p2, sPnt->p3, F_tek);
		}
		else if(F_tek >= F_min){
			P_min = iq_Fy_x1x2y1y2x(F_min,(_IQmpy(F_delta, _25PERCENT)+ F_min),sPnt->p1, sPnt->p2, F_tek);
		}
		else{
			P_min = BELOW_ZERO;
		}
		//Сравниваем текущее значение активной мощности с минимально допустимум для текущей частоты
		if (P_act < P_min){
			if(mode > _OFF){
				//система определения отсутствия потока
				if(sPnt->counter == 0){
					if (mode == _ON_WARNING ){
						clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
						clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);

						clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);	
						setBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
					}
					else if (mode == _ON_CRASH ){
						clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
						clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);

						clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);	
						setBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
					}
					else if (mode == _ON_SLEEP ){						//Проверка спящего режима
						clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
						clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);

						setBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
						setBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
						sPnt->flag_sleep = 1; // спящий режим
						
					}
				}
				else{
					sPnt->counter--;
				}
			}
			else{
				clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
				clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
				clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
				clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
				sPnt->counter	 = 	(s32) (load_s16(ramPnt, progPnt->Pnt_NF_delay) * K_TIME_CALL_MICRO_TAKT);
			}
			//***********************************система защиты от cухого хода****************************************
			if ((F_tek == F_max) && (imode > _OFF)){
				if (sPnt->counter_i == 0){
					if(imode == _ON_WARNING){
						clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
						setBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
					}
					else{												//(imode == _ON_CRASH)
						clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
						setBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
					}
				}
				else{
					sPnt->counter_i--;
				}
			}
			else{
				clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
				clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
				sPnt->counter_i = (s16) (load_s16(ramPnt, progPnt->Pnt_DR_delay) * K_TIME_CALL_MICRO_TAKT);
			}
			//****************************************************************************************************

		}
		else{
			clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
			clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
			clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
			clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
			
			clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
			clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
			DriveOnState = 0;//clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);
			
			sPnt->counter_i      =  (s16) (load_s16(ramPnt, progPnt->Pnt_DR_delay) * K_TIME_CALL_MICRO_TAKT);
			sPnt->counter        =  (s16) (load_s16(ramPnt, progPnt->Pnt_NF_delay) * K_TIME_CALL_MICRO_TAKT);
			sPnt->counter_sleep  =        (load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT);
		}	
	//**********************************************************************************************
    }
	else{
		clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
		clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
		clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
		clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
		sPnt->counter        =  (s16) (load_s16(ramPnt, progPnt->Pnt_NF_delay) * K_TIME_CALL_MICRO_TAKT);
	}

    defBitMicro(ramPnt, progPnt->Pnt_Drive_On, (s16)(DriveOnState && ((!sPnt->DriveOnPrevState)&1)));
    sPnt->DriveOnPrevState = DriveOnState;
    
    return &progPnt->Pnt_End;
	
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncNO_FLOW_DRY_RUN_1[26]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  F_TEK
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  P_ACT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Curve_P1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Curve_P2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Curve_P3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Curve_P4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Curve_P5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  DRV_Fmax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  DRV_Fmin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  NF_delay
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  DR_delay
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  PWM_ON
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Mode_NF
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  AutoTuning
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Mode_DR
    	S16_PIN_TYPE | INPUT_PIN_MODE,     //16  AT_perc
    	S16_PIN_TYPE | INPUT_PIN_MODE,     //17  T_sleep
        BIT_PIN_TYPE | INPUT_PIN_MODE,    //18  FC_FAIL
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //19  NF_SBlink
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,	   //20  Drive_On
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,	   //21  DR_Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  DR_Failure
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  Stop_Sleep
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  NF_SleepW
    	BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  NF_Warning
    	BIT_PIN_TYPE | OUTPUT_PIN_MODE     //26  NF_Failure
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("NO_FLOW_DRY_RUN", "DA", TblFuncNO_FLOW_DRY_RUN_1, FuncNO_FLOW_DRY_RUN_1, sizeof(TblFuncNO_FLOW_DRY_RUN_1), sizeof(FuncNO_FLOW_DRY_RUN_State_type),sizeof(nvFuncNO_FLOW_DRY_RUN_State_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncNO_FLOW_DRY_RUN_1,
#endif
//********************************************************
