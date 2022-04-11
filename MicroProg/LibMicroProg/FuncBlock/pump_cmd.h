#ifdef    GET_FUNC_CODE

typedef struct{
    u16 tmrMax;   //!< Таймер
    u16 tmrMin;   //!< Таймер
    u16 tmrSec;   //!< Таймер, отсчитывающий интервал 1 секунду
    u16 countLEDREADY;

    union{
    	struct{
    	    u16 Sleep     : 1;  //!< Бит, отображающий состояние привода (0 - состояние привода определяется битами DrvOn и DrvOff, 1 - привод в режиме СОН)
    	    u16 DrvOn     : 1;  //!< Привод в работе
    	    u16 DrvOff    : 1;  //!< Привод в останове
    	    u16 Change    : 1;  //!< Признак обновления
    	    u16 RunLed    : 1;  //!< Сигнал для зеленого светодиода
    	    u16 ReadyLed  : 1;  //!< Сигнал для желтого светодиода
    	    u16 FailLed   : 1;  //!< Сигнал для красного светодиода
    	    u16 Blink     : 1;  //!< Сигнал для формирования мигающих желтого(АПВ) и зеленого(СОН) светодиодов
    	    u16 Start     : 1;  //!< Сигнал пуск ПЧ
    	    u16 Stop      : 1;  //!< Сигнал останов ПЧ
    	    u16 Fail      : 1;  //!< Сигнал авария ПЧ
    	    u16 reserv    : 5;  //!<
        }bit;
        u16 all;
    }flags;
}PCMDSLEEP_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PuskIn;                       //1  PuskIn
    u16  Pnt_StopIn;                       //2  StopIn
    u16  Pnt_DrvStatus;                    //3  DrvStatus
    u16  Pnt_FailTekDrv;                   //4  FailTekDrv
    u16  Pnt_AV_Reload;                    //5  AV_Reload
    u16  Pnt_AV_InWork;                    //6  AV_InWork
    u16  Pnt_InFBpid;                      //7  InFBpid
    u16  Pnt_MaxSleep;                     //8  MaxSleep
    u16  Pnt_MinSleep;                     //9  MinSleep
    u16  Pnt_TimeOff;                      //10  TimeOff
    u16  Pnt_TimeOn;                       //11  TimeOn
    u16  Pnt_MinMax;                       //12  MinMax
    u16  Pnt_En1;                          //13  En1
    u16  Pnt_En2;                          //14  En2
    u16  Pnt_AVSleepEn;                    //15  AVSleepEn
    u16  Pnt_NF_Pusk;                      //16  NF_Pusk
    u16  Pnt_NF_Stop;                      //17  NF_Stop
    u16  Pnt_GetReady;                     //18  GetReady
    u16  Pnt_SigAPV;                       //19  SigAPV
    u16  Pnt_Function;                     //20  Function
    u16  Pnt_nStartReady;                  //21  nStartReady
    u16  Pnt_StartReady;                   //22  StartReady
    u16  Pnt_EnPump;                       //23  EnPump
    u16  Pnt_StopSleep;                    //24  StopSleep
    u16  Pnt_StartSleep;                   //25  StartSleep
    u16  Pnt_SleepModeOn;                  //26  SleepModeOn
    u16  Pnt_WflagVent;                    //27  WflagVent
    u16  Pnt_Hold_GT;                      //28  Hold_GT
    u16  Pnt_W_Flag;                       //29  W_Flag
    u16  Pnt_Stop_En;                      //30  Stop_En
    u16  Pnt_Pusk_En;                      //31  Pusk_En
    u16  Pnt_End;
}FuncPUMP_CMD_type;

typedef enum{
	FuncPump = 0,    //!< Выбран функционал насоса
	FuncLift = 1,     //!< Выбран функционал лифта
	FuncSVP  =  2      /*  Выбран функционал СВП*/
}FunctionalPumpCMD_type;

typedef enum{
    minimum = 0,    //!< Выбран режим СОН с минимумом
	maximum = 1     //!< Выбран режим СОН с максимумом
}ModeSleepPumpCMD_type;

#define CALC_TO_uS  100/150.f
#define CALC_TO_mS  100/150000.f
//********************************************************
u16 *FuncPUMP_CMD_1(FuncPUMP_CMD_type *progPnt, u32 ramPnt)
{
	void    *nvAdr1; //АПВ Для обращения к полю EngineRunFlag энергонезависимой структуры
	u16 CountToAPVFlag = 0;
    PCMDSLEEP_State_type *sPnt;
    COMMONPARAMM3        *mPnt3;
    _iq in, max, min;
   //s16    FailsTek;
  //  u16 prevState = 0;
    sPnt = (PCMDSLEEP_State_type *)(ramPnt + progPnt->Pnt_State);
    mPnt3 = &GlobalM3;

#ifdef _PROJECT_FOR_CCS_
    GlobalM3.Time1 = (Timestamp_get32() - GlobalM3.Time1a) * CALC_TO_mS;
#endif

    if(flagIsFirstCall){
    	sPnt->tmrSec = K_TIME_CALL_MICRO_TAKT;
    	sPnt->tmrMax = load_s16(ramPnt, progPnt->Pnt_TimeOff);
    	sPnt->tmrMin = load_s16(ramPnt, progPnt->Pnt_TimeOn);
    	sPnt->flags.all = 0;
    }


    /****************Для мигания индикатором готовности при остчете до АПВ*****************/
    nvAdr1 = (void *)((u32)&nv.EngineRunFlag); //вычисляем адрес флага аварии по питанию
    RdFramMicro( &CountToAPVFlag, nvAdr1,    sizeof(u16)); //считываем состояние. Если CountToAPVFlag = 1, но PWM отключено - то идет отсчет до АПВ
    /**************************************************************************************/


// if((GlobalM3.FlagCommand.bit.BitGotovWorkSVP==1)&&((GlobalM3.FlagCommand.bit.ChangeRegimSlave==1)))//|| (GlobalM3.FlagCommand.bit.ChangeRegim==1)))//||)//моргать светодиодом при смене режимаGlobalM3.FlagCommand.bit.ChangeRegimSlave
//    	{
//    		if(sPnt->countLEDREADY >0)sPnt->countLEDREADY--;
//    		else
//    		{   sPnt->countLEDREADY =K_TIME_CALL_MICRO_TAKT;
//    	       if(testBitMicro(ramPnt, progPnt->Pnt_StartReady))
//
//    	        	{
//    	        		clrBitMicro(ramPnt, progPnt->Pnt_StartReady);
//						setBitMicro(ramPnt, progPnt->Pnt_W_Flag);
//    	        	}
//    	        	else
//    	        	{
//    	        		setBitMicro(ramPnt, progPnt->Pnt_StartReady);
//						clrBitMicro(ramPnt, progPnt->Pnt_W_Flag);
//    	        	}
//    		}
//    	}
  //if((GlobalM3.FlagCommand.bit.BitWAitPhase==1)&&(GlobalM3.FlagCommand.bit.BitGotovWorkSVP==1))//моргать светодиодом при осутсвии напряж
  //  	{
  //  		if(sPnt->countLEDREADY >0)sPnt->countLEDREADY--;
  //  		else
  //  		{   sPnt->countLEDREADY =K_TIME_CALL_MICRO_TAKT;
  //  	       if(testBitMicro(ramPnt, progPnt->Pnt_StartReady))

  //  	        	{
  //  	        		clrBitMicro(ramPnt, progPnt->Pnt_StartReady);
  //  	        	}
  //  	        	else
  //  	        	{
  //  	        		setBitMicro(ramPnt, progPnt->Pnt_StartReady);
  //  	        	}
  //  		}
  //  	}
  //  else
  //  {

	if((FunctionalPumpCMD_type)load_s16(ramPnt, progPnt->Pnt_Function) == FuncPump){          //!< Если используется функционал насоса
		setBitMicro(ramPnt, progPnt->Pnt_EnPump);                                             //!< Сигнал разрешения работы насосных модулей
//!< Отработка старт стоп и аварий
//		FailsTek = load_s16(ramPnt, progPnt->Pnt_FailTekDrv);//DEBUG!!!
		if(load_s16(ramPnt, progPnt->Pnt_FailTekDrv)){ //если есть авария
			sPnt->flags.bit.Fail = 1; //запоминаем то что есть авария
		}
		else{ //если нет аварии
			sPnt->flags.bit.Fail = 0; //запоминаем что нет аварии
		}




		if((sPnt->flags.bit.Fail || testBitMicro(ramPnt, progPnt->Pnt_StopIn)||(load_s16(ramPnt, progPnt->Pnt_DrvStatus) & DRVinStopMode)) && (!sPnt->flags.bit.Stop)) //(load_s16(ramPnt, progPnt->Pnt_DrvStatus) & DRVinStopMode) 
		{ //!< Если авария привода или пришел сигнал стоп - останавливаем
        	sPnt->flags.bit.Stop = 1;
        	sPnt->flags.bit.Start = 0;
        	clrBitMicro(ramPnt, progPnt->Pnt_WflagVent);                             //!< Индикация останова для модуля автовентиль
	    }
        else if(testBitMicro(ramPnt, progPnt->Pnt_PuskIn) && (load_s16(ramPnt, progPnt->Pnt_DrvStatus) == 0)){ //иначе, когда нет аварии и нет ошибки проверяем наличие команды пуск
        	sPnt->flags.bit.Start = 1; // есть команда пуск
        	sPnt->flags.bit.Stop = 0; //нет командв стоп
        	setBitMicro(ramPnt, progPnt->Pnt_WflagVent);                             //!< Индикация работы для модуля автовентиль
        }

//!< Отработка режима СОН
        if(testBitMicro(ramPnt, progPnt->Pnt_AVSleepEn) && testBitMicro(ramPnt, progPnt->Pnt_En1) && load_s16(ramPnt, progPnt->Pnt_En2) && sPnt->flags.bit.Start)
        {

            if(sPnt->tmrSec)
            {
            	sPnt->tmrSec--;
            }
            else
            {
            	sPnt->flags.bit.Blink = !sPnt->flags.bit.Blink;
                sPnt->tmrSec = K_TIME_CALL_MICRO_TAKT;
                in  = _IQabs(load_s32(ramPnt, progPnt->Pnt_InFBpid));                             //!< Вычитываем задание для обратной связи ПИД - регулятора
                max = _IQabs(load_s32(ramPnt, progPnt->Pnt_MaxSleep));                            //!< Максимальный порог для сравнения
                min = _IQabs(load_s32(ramPnt, progPnt->Pnt_MinSleep));                            //!< Минимальный порог для сравнения
                if(in >= max){                                                                    //!< Задание обратной связи для ПИД превысило максимальный порог
                    if(sPnt->tmrMax){
                    	sPnt->tmrMax--;
                    	sPnt->flags.bit.Change = 0;
                    }
                    else{                                                                         //!< Переход в режим СОН
                        sPnt->flags.bit.DrvOn = 0;
                        sPnt->flags.bit.DrvOff = 1;
                        sPnt->flags.bit.Sleep = 1;
                        sPnt->flags.bit.Change = 1;
                    }
                    sPnt->tmrMin = load_s16(ramPnt, progPnt->Pnt_TimeOn);
                }
                else if(in <= min){
                    if(sPnt->tmrMin){
                    	sPnt->tmrMin--;
                    	sPnt->flags.bit.Change = 0;
                    }
                    else{                                                                         //!< Выход из режима СОН
                        sPnt->flags.bit.DrvOn = 1;
                        sPnt->flags.bit.DrvOff = 0;
                        sPnt->flags.bit.Sleep = 0;
                        sPnt->flags.bit.Change = 1;
                    }
                    sPnt->tmrMax = load_s16(ramPnt, progPnt->Pnt_TimeOff);
                }
                else{                                                                             //!< Не меняем состояние привода
                    if(sPnt->tmrMin < load_s16(ramPnt, progPnt->Pnt_TimeOn)){
                    	sPnt->tmrMin++;
                    }
                    if(sPnt->tmrMax < load_s16(ramPnt, progPnt->Pnt_TimeOff)){
                    	sPnt->tmrMax++;
                    }
                    sPnt->flags.bit.Change = 0;
                    //sPnt->stateOff  = 0;
                }

                if(((ModeSleepPumpCMD_type)load_s16(ramPnt, progPnt->Pnt_MinMax) == minimum) && sPnt->flags.bit.Change){   //!< Переход в режим СОН при значении входного параметра меньше минимального порога
                    sPnt->flags.bit.DrvOn = !sPnt->flags.bit.DrvOn;
                    sPnt->flags.bit.DrvOff = !sPnt->flags.bit.DrvOff;
                    sPnt->flags.bit.Sleep = !sPnt->flags.bit.Sleep;
                }
                //defBitMicro(ramPnt, progPnt->Pnt_StartSleep, (sPnt->flags.bit.DrvOn | testBitMicro(ramPnt, progPnt->Pnt_NF_Pusk)));
                //defBitMicro(ramPnt, progPnt->Pnt_StopSleep,  (sPnt->flags.bit.DrvOff | testBitMicro(ramPnt, progPnt->Pnt_NF_Stop)));

                if(sPnt->flags.bit.Start){
                    if(sPnt->flags.bit.Sleep)
                    {
                    	clrBitMicro(ramPnt, progPnt->Pnt_WflagVent);                             //!< Индикация останова для модуля автовентиль
                        clrBitMicro(ramPnt, progPnt->Pnt_Pusk_En);
                        setBitMicro(ramPnt, progPnt->Pnt_Stop_En);
                    }
                    else
                    {
                    	setBitMicro(ramPnt, progPnt->Pnt_WflagVent);                             //!< Индикация работы для модуля автовентиль
                        setBitMicro(ramPnt, progPnt->Pnt_Pusk_En);
                        clrBitMicro(ramPnt, progPnt->Pnt_Stop_En);
                    }
                }
            }
        }
        else
        {
        	//tranzitBitMicro(ramPnt, progPnt->Pnt_StartSleep, progPnt->Pnt_NF_Pusk);
        	//tranzitBitMicro(ramPnt, progPnt->Pnt_StopSleep, progPnt->Pnt_NF_Stop);
            sPnt->tmrMax = load_s16(ramPnt, progPnt->Pnt_TimeOff);
            sPnt->tmrMin = load_s16(ramPnt, progPnt->Pnt_TimeOn);
            if(testBitMicro(ramPnt, progPnt->Pnt_AV_InWork))
            {
                if(sPnt->flags.bit.Start) //если есть команда пуск
                {
                	if(testBitMicro(ramPnt, progPnt->Pnt_AV_Reload))
                	{
                        clrBitMicro(ramPnt, progPnt->Pnt_Pusk_En);
                        setBitMicro(ramPnt, progPnt->Pnt_Stop_En);
                	}
                	else{
                        setBitMicro(ramPnt, progPnt->Pnt_Pusk_En); //даем команду пуск
                        clrBitMicro(ramPnt, progPnt->Pnt_Stop_En);
                	}
                }

            }
            else
            {
                defBitMicro(ramPnt, progPnt->Pnt_Pusk_En, sPnt->flags.bit.Start);
                defBitMicro(ramPnt, progPnt->Pnt_Stop_En, sPnt->flags.bit.Stop);
            }
            sPnt->flags.bit.DrvOn = 0;
            sPnt->flags.bit.DrvOff = 0;
            sPnt->flags.bit.Sleep = 0;//???????????????? progPnt->Pnt_NF_Pusk
            sPnt->flags.bit.Blink = 0;
            //sPnt->blink     = 1;
        }
//////////////////////////////////////////////////////////////////////////////
        if(sPnt->flags.bit.Fail) //если есть авария
        {
        	if (testBitMicro(ramPnt, progPnt->Pnt_SigAPV))//но при этом приходит сигнал индикации отсчета до АПВ
        	{
        		setBitMicro(ramPnt, progPnt->Pnt_StartReady);
        	}
        	else
        	{
        		clrBitMicro(ramPnt, progPnt->Pnt_StartReady);
        	}
        	clrBitMicro(ramPnt, progPnt->Pnt_W_Flag);
        	sPnt->flags.bit.Sleep = 0;
        }
        else //иначе аварии нет
        {
            if(mPnt3->FlagCommand.bit.PwmOn)
            { //и если привод включен
                setBitMicro(ramPnt, progPnt->Pnt_W_Flag);            //!< Индикация состояния - работа
                clrBitMicro(ramPnt, progPnt->Pnt_StartReady);        //Сброс состояния ГОТОВ
            }
            else if(sPnt->flags.bit.Sleep)
            {
                defBitMicro(ramPnt, progPnt->Pnt_W_Flag, sPnt->flags.bit.Blink);     //!< Индикация состояния - СОН

                tranzitBitMicro(ramPnt, progPnt->Pnt_StartReady, progPnt->Pnt_SigAPV); //!< Индикация состояния АПВ
    		}
            else if (!mPnt3->FlagCommand.bit.PwmOn && (CountToAPVFlag == 1))//если двигатель не работает но флаг работы установлен то идет отсчет до АПВ
            {
            	if (testBitMicro(ramPnt, progPnt->Pnt_SigAPV))// при этом приходит сигнал индикации отсчета до АПВ
            	{
            	   setBitMicro(ramPnt, progPnt->Pnt_StartReady); //светодиод светится
            	}
            	else
            	{
            	   clrBitMicro(ramPnt, progPnt->Pnt_StartReady); //светодиод не светится
            	}
            }
    		else //иначе нет отсчета до АПВ и мы находимся не в работе
    		{
    			clrBitMicro(ramPnt, progPnt->Pnt_W_Flag);                            //!< Индикация состояния не в работе
    			tranzitBitMicro(ramPnt, progPnt->Pnt_StartReady, progPnt->Pnt_GetReady);
    		}

        }


        /************Это наверное придется убрать. или заблокировать во время отсчета обычную индикацию АПВ**********/
        /*
        u16 prevState = 0;
        if (testBitMicro(ramPnt, progPnt->Pnt_SigAPV)) //если пришел сигнал индикации отсчета времени до момента АПВ
        {
        	setBitMicro(ramPnt, progPnt->Pnt_GetReady); //включаем диод индикации готовности - мигаем
        	prevState = 1; //запоминаем то, что давали индикацию отсчета до АПВ
        }
        else if (prevState == 1) //если мы давали индикацию отсчета до АПВ
        {
        	clrBitMicro(ramPnt, progPnt->Pnt_GetReady); //отключаем светодиод готовности - мигаем
        	prevState = 0;
        }
        */

		defBitMicro(ramPnt, progPnt->Pnt_SleepModeOn, sPnt->flags.bit.Sleep);                       //!< Передача состояния СОН в статусное слово №2.
    }
	else{                                                                                     //!< Если используется функционал лифта, то сигнал готовности пропускаем без изменений
		tranzitBitMicro(ramPnt, progPnt->Pnt_StartReady, progPnt->Pnt_GetReady);//!!!!!!!!
		clrBitMicro(ramPnt, progPnt->Pnt_EnPump);                                 //!< Сигнал запрета работы насосных модулей
		clrBitMicro(ramPnt, progPnt->Pnt_StartSleep);                             //!< Отключаем сигнал активации режима СОН
		clrBitMicro(ramPnt, progPnt->Pnt_StopSleep);                              //!< Отключаем сигнал выхода из режима СОН
		clrBitMicro(ramPnt, progPnt->Pnt_WflagVent);                              //!< Сигнал для работы модуля автовентиль
		clrBitMicro(ramPnt, progPnt->Pnt_SleepModeOn);
		sPnt->flags.all = 0;
	}
	if(GlobalM3.FlagCommand.bit.BitWAitPhase) clrBitMicro(ramPnt, progPnt->Pnt_StartReady);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPUMP_CMD_1[31]={
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  PuskIn
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  StopIn
		S16_PIN_TYPE | INPUT_PIN_MODE,     //3  StatusDrv
		S16_PIN_TYPE | INPUT_PIN_MODE,     //4  FailTekDrv
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  AV_Reload
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  AV_InWork
		S32_PIN_TYPE | INPUT_PIN_MODE,     //7  InFBpid
		S32_PIN_TYPE | INPUT_PIN_MODE,     //8  MaxSleep
		S32_PIN_TYPE | INPUT_PIN_MODE,     //9  MinSleep
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  TimeOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  TimeOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  MinMax
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  En1
		S16_PIN_TYPE | INPUT_PIN_MODE,     //14  En2
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  AVSleepEn
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //16  NF_Pusk
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //17  NF_Stop
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //18  GetReady
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //19  SigAPV
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Function
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  nStartReady
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  StartReady
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  EnPump
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  StopSleep
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  StartSleep
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  SleepModeOn
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //27  WflagVent
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //28  Hold_GT
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //29  W_Flag
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //30  Stop_En
		BIT_PIN_TYPE | OUTPUT_PIN_MODE     //31  Pusk_En
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PUMP_CMD", "DD", TblFuncPUMP_CMD_1, FuncPUMP_CMD_1, sizeof(TblFuncPUMP_CMD_1), sizeof(PCMDSLEEP_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPUMP_CMD_1,
#endif
//********************************************************

