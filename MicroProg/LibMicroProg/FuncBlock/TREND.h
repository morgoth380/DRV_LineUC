#ifdef    GET_FUNC_CODE

#define TIME_12MS_MICRO_TAKT  3
#define TIME_100MS_MICRO_TAKT 25

typedef struct{
    u32 cnt;
    u32 UnixtimeStop;
	u32 start_time;
	u32 internal_time;
	u16 prev_start_timer_value;
	u16 trend_cap_iterator;
	u16 call_by_timer_bit;
	u16 warningTime;
}TREND_State_type;

typedef enum{
    TrendOff        = 0,
	StartTrendByPWM    ,
	StartTrendByTmr
}TrendStartRec_type;

typedef enum{
    Tick12ms  = 0,
	Tick100ms    ,
	Tick1sec     ,
	Tick1min
}TrendRecTime_type;

typedef struct{
    u32 unixtime;
    u16 value;
}Trend_not_complete_type;

typedef enum{
    SelwRot = 0,
	SelwStat   ,
	SelIu      ,
	SelIv      ,
	SelIw      ,
	SelUout    ,
	SelUd      ,
	SelPfull
}TrendSelect_type;

const u16 TimeCapScale[4] = {12, 100, 1, 1};
const u16 TimeCapFormat[4] = {mSRasm, mSRasm, SRasm, minRasm};

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Diskr;                        //2  Diskr
    u16  Pnt_T_write;                      //3  T_write
    u16  Pnt_Timer;                        //4  Timer
    u16  Pnt_Type;                         //5  Type
    u16  Pnt_UnixTime;                     //6  UnixTime
    u16  Pnt_PWM_ON;                       //7  PWM_ON
    u16  Pnt_sel;                          //8  PWM_ON
	u16  Pnt_FM_Mutex;					   //9  FM_Mutex
	u16  Pnt_TrndWarning;				   //10 TrendWarning
    u16  Pnt_End;
}FuncTREND_type;
//********************************************************
u16 *FuncTREND_1(FuncTREND_type *progPnt, u32 ramPnt)
{
    Trend_not_complete_type trend_temp_value;
    Trend_rec_type trend_output_value;
    void *nvAdr;
    TREND_State_type *sPnt;
    MPOpis_type *moPnt;
    u32 unixtime;
    _iq tmpVal;
    s16 value, format;
    u16 type, timer, trend_cap, overflow, sel, tmp;
	
    sPnt = (TREND_State_type *)(ramPnt + progPnt->Pnt_State);
    moPnt = (MPOpis_type *)&MPOpis;

    if(flagIsFirstCall){
    	sPnt->trend_cap_iterator = 0;
    	sPnt->internal_time = 0;
    }

    tmpVal = load_s32(ramPnt, progPnt->Pnt_In);             //Записываемая величина
    sel = load_s16(ramPnt, progPnt->Pnt_sel);               //Тип записывемой величины
    type = load_s16(ramPnt, progPnt->Pnt_Type);             //Режим записи
	
    if(testBitMicro(ramPnt, progPnt->Pnt_FM_Mutex)){		//Включена блокировка записи (защита от перетарания журнала пожарного режима)
		if(type != TrendOff){
			setBitMicro(ramPnt, progPnt->Pnt_TrndWarning);
			sPnt->warningTime = K_TIME_CALL_MICRO_TAKT * 5;	//5 секунд пускай мигает предупреждение!
			save_s16(ramPnt, progPnt->Pnt_Type, TrendOff);
		}
        if(sPnt->warningTime) sPnt->warningTime--;
        else clrBitMicro(ramPnt, progPnt->Pnt_TrndWarning);
	}
	else{
        switch(sel){
            case SelwRot:
            case SelwStat: tmp = NUM_FREQ_BASE;
            break;
            case SelIu:
            case SelIv:
            case SelIw: tmp = NUM_CUR_BASE;
            break;
            case SelUout:
            case SelUd: tmp = NUM_VOLT_BASE;
            break;
            case SelPfull: tmp = NUM_POWER_BASE;
            break;
            default: tmp = NUM_1000_BASE;
            break;
        }

        value = (s16)(((s64)tmpVal * moPnt->Base[tmp]) >> GLOBAL_Q);
        unixtime = load_s32(ramPnt, progPnt->Pnt_UnixTime);
		
        timer = load_s16(ramPnt, progPnt->Pnt_Timer);

        tmp = (u16)load_s16(ramPnt, progPnt->Pnt_Diskr);             //Дискретность записи
        format = TimeCapFormat[tmp];
        trend_cap = TimeCapScale[tmp];
        save_s16(ramPnt, progPnt->Pnt_T_write, (s16)trend_cap);
        save_s16(ramPnt, progPnt->Pnt_TimeFormat, (s16)format);
        if(sPnt->trend_cap_iterator == NUM_REC_TREND) overflow = 1;
        else overflow = 0;
		
        switch(type){
            case TrendOff:
                sPnt->call_by_timer_bit = 1;								//Сбрасываем "Первое вхождение" по таймеру
                sPnt->internal_time = 0;									//Обнуляем внутрений счетчик
                sPnt->trend_cap_iterator = 0;  								//Обнуляем счетчик ячеек
                sPnt->prev_start_timer_value = 0;
                return &progPnt->Pnt_End;
            case StartTrendByPWM:
                sPnt->call_by_timer_bit = 1;									//Сбрасываем "Первое вхождение" по таймеру
                if(testBitMicro(ramPnt, progPnt->Pnt_PWM_ON)) break;
                else{
                    sPnt->internal_time = 0;											//Обнуляем внутрений счетчик
                    sPnt->trend_cap_iterator = 0;  										//Обнуляем счетчик ячеек
                    return &progPnt->Pnt_End;
                }
            case StartTrendByTmr:
                if(sPnt->prev_start_timer_value != timer){
                    sPnt->call_by_timer_bit = 0; 								//Если значение "Таймера старта" изменилось, то сбрасываем "Первое вхождение" по таймеру
                    sPnt->trend_cap_iterator = 0;  								//Обнуляем счетчик ячеек
                }
                if(sPnt->call_by_timer_bit == 0){                               //Если это первое вхождение, то заливаем время заданное "Таймером старта"
                    sPnt->start_time = timer * K_TIME_CALL_MICRO_TAKT;
                    sPnt->prev_start_timer_value = timer;
                    sPnt->call_by_timer_bit = 1;
                }
                if(sPnt->start_time != 0){                                      //Если время 	заданное "Таймером старта" не отсчитано,
                    sPnt->start_time--;		// то уменьшаем счетную переменную и выходим из функции
                    return &progPnt->Pnt_End;
                }
            break;
        }
        if(overflow){
            save_s16(ramPnt, progPnt->Pnt_Type, TrendOff);
            return &progPnt->Pnt_End;				//Если записали все 1500 ячеек, то не продолжаем запись
        }
		switch(tmp){                                          //tmp == load_s16(ramPnt, progPnt->Pnt_Diskr) Время одного цикла микропрограммы = 4мс, по этому 1 тик внутренего таймера = 4 мс
            case Tick12ms:                                    // каждые 12 мс
                if((sPnt->internal_time % TIME_12MS_MICRO_TAKT) == 0){
                    trend_temp_value.unixtime = unixtime;
                    trend_temp_value.value = (u16)value;
                    trend_output_value.unixtime = unixtime;
                    trend_output_value.value = (u16)value;
                    trend_output_value.crc = crcPacket((u8 *)&trend_temp_value, sizeof(Trend_not_complete_type) / 2);
                    nvAdr = (void *)((u32)&nv.trend_file.rec[sPnt->trend_cap_iterator]);							//запись в FRAM
                    WrFramMicro(nvAdr, (void *)&trend_output_value, sizeof(Trend_rec_type));
                    sPnt->trend_cap_iterator++;
                    if(sPnt->trend_cap_iterator >= (u16)NUM_REC_TREND) overflow = 1;
                }
            break;
            case Tick100ms:                                                           // каждые 100 мс
                if((sPnt->internal_time % TIME_100MS_MICRO_TAKT) == 0){
                    trend_temp_value.unixtime = unixtime;
                    trend_temp_value.value = (u16)value;
                    trend_output_value.unixtime = unixtime;
                    trend_output_value.value = (u16)value;
                    trend_output_value.crc = crcPacket((u8 *)&trend_temp_value, sizeof(Trend_not_complete_type) / 2);
                    nvAdr = (void *)((u32)&nv.trend_file.rec[sPnt->trend_cap_iterator]);  //запись в FRAM
                    WrFramMicro(nvAdr, (void *)&trend_output_value, sizeof(Trend_rec_type));
                    sPnt->trend_cap_iterator++;
                    if(sPnt->trend_cap_iterator >= (u16)NUM_REC_TREND) overflow = 1;
                }
            break;
            case Tick1sec:	                                                             // каждую с
                if((sPnt->internal_time % K_TIME_CALL_MICRO_TAKT) == 0){
                    trend_temp_value.unixtime = unixtime;
                    trend_temp_value.value = (u16)value;
                    trend_output_value.unixtime = unixtime;
                    trend_output_value.value = (u16)value;
                    trend_output_value.crc = crcPacket((u8 *)&trend_temp_value, sizeof(Trend_not_complete_type) / 2);
                    nvAdr = (void *)((u32)&nv.trend_file.rec[sPnt->trend_cap_iterator]); //запись в FRAM
                    WrFramMicro(nvAdr, (void *)&trend_output_value, sizeof(Trend_rec_type));
                    sPnt->trend_cap_iterator++;
                    if(sPnt->trend_cap_iterator >= (u16)NUM_REC_TREND) overflow = 1;
                }
            break;
            case Tick1min:                                                                  // каждую мин
                if((sPnt->internal_time % MINUTE_MICRO_TAKT) == 0){
                    trend_temp_value.unixtime = unixtime;
                    trend_temp_value.value = (u16)value;
                    trend_output_value.unixtime = unixtime;
                    trend_output_value.value = (u16)value;
                    trend_output_value.crc = crcPacket((u8 *)&trend_temp_value, sizeof(Trend_not_complete_type) / 2);
                    nvAdr = (void *)((u32)&nv.trend_file.rec[sPnt->trend_cap_iterator]); //запись в FRAM
                    WrFramMicro(nvAdr, (void *)&trend_output_value, sizeof(Trend_rec_type));
                    sPnt->trend_cap_iterator++;
                    if(sPnt->trend_cap_iterator >= (u16)NUM_REC_TREND) overflow = 1;
                }
            break;
        }

        if(sPnt->internal_time == MINUTE_MICRO_TAKT) sPnt->internal_time = 0;
        else sPnt->internal_time++;
	}
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTREND_1[10]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Diskr
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //3  T_write
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Timer
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Type
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  UnixTime
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  PWM_ON
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Type
		BIT_PIN_TYPE | INPUT_PIN_MODE,	   //9  FM_Mutex
		BIT_PIN_TYPE | OUTPUT_PIN_MODE	   //10 TrendWarning
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TREND", "DA", TblFuncTREND_1, FuncTREND_1, sizeof(TblFuncTREND_1), sizeof(TREND_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTREND_1,
#endif
//********************************************************
