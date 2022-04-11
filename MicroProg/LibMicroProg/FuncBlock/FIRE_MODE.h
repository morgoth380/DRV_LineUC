#ifdef    GET_FUNC_CODE

#define SOURCE_PID 7
#define SOURCE_FM  9
#define OFF 	   0
#define ON		   1
#define     nvSfFuncFIRE_MODE_State_type     (*(nvFuncFIRE_MODE_State_type*)0)
//**********************************************************
typedef struct{
	u16 fm_mutex;	//защита от перезаписи
	
} nvFuncFIRE_MODE_State_type;
typedef struct{

	u16 fire_mode_on;
	u16 Old_ErrMask1;
	u16 Old_ErrMask2;
	u16 Old_ErrMask3;
	u16 Old_ErrMask4;
	u16 fire_mode_iterator;
	
	nvFuncFIRE_MODE_State_type	nvFMmutex;
}FuncFIRE_MODE_State_type;
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Block_FM;                     //1  Block_FM
    u16  Pnt_Sensor_Din;                   //2  Sensor_Din
    u16  Pnt_Sensor_Type;                  //3  Sensor_Type
    u16  Pnt_Din0_7;                       //4  Din0_7
    u16  Pnt_Task_Source;                  //5  Task_Source
    u16  Pnt_PID_Source;                   //6  PID_Source
    u16  Pnt_PID_Feedback;                 //7  PID_Feedback
    u16  Pnt_FM_Ud;                        //8  FM_Ud
    u16  Pnt_FM_Iact;                      //9  FM_Iact
    u16  Pnt_FM_Ifull;                     //10  FM_Ifull
    u16  Pnt_FM_Ireact;                    //11  FM_Ireact
    u16  Pnt_FM_Iu;                        //12  FM_Iu
    u16  Pnt_FM_Iv;                        //13  FM_Iv
    u16  Pnt_FM_Iw;                        //14  FM_Iw
    u16  Pnt_FM_Fout;                      //15  FM_Fout
    u16  Pnt_FM_Unixtime;                  //16  FM_Unixtime
    u16  Pnt_FM_ErrMask1;                  //17  FM_ErrMask1
    u16  Pnt_FM_ErrMask2;                  //18  FM_ErrMask2
    u16  Pnt_FM_ErrMask3;                  //19  FM_ErrMask3
    u16  Pnt_FM_ErrMask4;                  //20  FM_ErrMask4
	u16  Pnt_UnlockJournal;				   //21  UnlockJournal
	u16  Pnt_PID_FB;					   //22	 PID_FB
	u16  Pnt_PID_task;					   //23	 PID_task
	u16  Pnt_Task_sc1;					   //24	 Task_sc1
	u16  Pnt_Task_sc2;					   //25	 Task_sc2
	u16	 Pnt_FM_Mutex;					   //26  FM_Mutex
    u16  Pnt_FM_On;                        //27  FM_On
    u16  Pnt_End;
}FuncFIRE_MODE_type;
//********************************************************
u16 *FuncFIRE_MODE_1(FuncFIRE_MODE_type *progPnt, u32 ramPnt)
{	typedef struct{
    u32                 unixtime;
    u16                 errormask1;
    u16                 errormask2;
	u16                 errormask3;
	u16                 errormask4;
	u16					Ud;                       
    u16					Iact;                      
    u16					Ifull;                     
    u16					Ireact;                   
    u16					Iu;                        
    u16					Iv;                       
    u16					Iw;                       
    u16					Fout;
	//Сюда можно добавить еще ток утечки, при этом размер будет 32 байтa и свободное место нужно будет считать относительно журнала ПР, а не журнала тренда
	}Fire_mode_not_complete_type;
	
	void *nvAdr;
	nvFuncFIRE_MODE_State_type	 	*nvAdrMutex;
	FuncFIRE_MODE_State_type	 	*sPnt;
	MPOpis_type                     *moPnt;
	Fire_mode_not_complete_type		fire_mode_not_complete_value;
	Fire_mode_rec_type				fire_mode_rec;
    u16                 			tmp;
	
	nvAdrMutex       = GetNvAdr(progPnt->Pnt_nvState, &nvSfFuncFIRE_MODE_State_type);
	sPnt = (FuncFIRE_MODE_State_type *)(ramPnt + progPnt->Pnt_State);
	moPnt = (MPOpis_type *)&MPOpis;
	if (flagIsFirstCall){
		sPnt->fire_mode_on = 0;
		sPnt->fire_mode_iterator = 0;
		sPnt->Old_ErrMask1 = load_s16(ramPnt, progPnt->Pnt_FM_ErrMask1);
		sPnt->Old_ErrMask2 = load_s16(ramPnt, progPnt->Pnt_FM_ErrMask2);
		sPnt->Old_ErrMask3 = load_s16(ramPnt, progPnt->Pnt_FM_ErrMask3);
		sPnt->Old_ErrMask4 = load_s16(ramPnt, progPnt->Pnt_FM_ErrMask4);
		save_s16(ramPnt, progPnt->Pnt_UnlockJournal, OFF);
		RdFramMicro(&sPnt->nvFMmutex, nvAdrMutex, sizeof(nvFuncFIRE_MODE_State_type));
	}
	if (sPnt->nvFMmutex.fm_mutex != 0){
		setBitMicro(ramPnt, progPnt->Pnt_FM_Mutex);
	}
	else{
		clrBitMicro(ramPnt, progPnt->Pnt_FM_Mutex);
	}
	if (load_s16(ramPnt, progPnt->Pnt_UnlockJournal) == ON){
		sPnt->nvFMmutex.fm_mutex = 0;
		WrFramMicro(nvAdrMutex, &sPnt->nvFMmutex, sizeof(nvFuncFIRE_MODE_State_type));
		save_s16(ramPnt, progPnt->Pnt_UnlockJournal, OFF);
	}
	if (sPnt->fire_mode_on){
		u16 error_mask1;
		u16 error_mask2;
		u16 error_mask3;
		u16 error_mask4;
		u16 got_new_err_mask1;
		u16 got_new_err_mask2;
		u16 got_new_err_mask3;
		u16 got_new_err_mask4;

		if(sPnt->nvFMmutex.fm_mutex == 0){
			sPnt->nvFMmutex.fm_mutex = 1;
			WrFramMicro(nvAdrMutex, &sPnt->nvFMmutex, sizeof(nvFuncFIRE_MODE_State_type));
		}
		//Запись критических событий
		
		error_mask1 = load_s16(ramPnt, progPnt->Pnt_FM_ErrMask1);
		error_mask2 = load_s16(ramPnt, progPnt->Pnt_FM_ErrMask2);
		error_mask3 = load_s16(ramPnt, progPnt->Pnt_FM_ErrMask3);
		error_mask4 = load_s16(ramPnt, progPnt->Pnt_FM_ErrMask4);
		//для формирования необходимой таблицы истинности используем (~a) & b		
		// 0 0 = 0 
		// 0 1 = 1
		// 1 0 = 0
		// 1 1 = 0
		got_new_err_mask1 = ((~sPnt->Old_ErrMask1) & error_mask1);
		got_new_err_mask2 = ((~sPnt->Old_ErrMask2) & error_mask2);
		got_new_err_mask3 = ((~sPnt->Old_ErrMask3) & error_mask3);
		got_new_err_mask4 = ((~sPnt->Old_ErrMask4) & error_mask4);

		if ( got_new_err_mask1 || got_new_err_mask2 || got_new_err_mask3 || got_new_err_mask4){
		
			sPnt->Old_ErrMask1 |= error_mask1;									//Запись текущих значений масок ошибок для хранения
			sPnt->Old_ErrMask2 |= error_mask2;
			sPnt->Old_ErrMask3 |= error_mask3;
			sPnt->Old_ErrMask4 |= error_mask4;		
			//формирование crc
			fire_mode_not_complete_value.unixtime 	= load_s32(ramPnt, progPnt->Pnt_FM_Unixtime);
			fire_mode_not_complete_value.errormask1 = error_mask1;
			fire_mode_not_complete_value.errormask2 = error_mask2;
			fire_mode_not_complete_value.errormask3 = error_mask3;
			fire_mode_not_complete_value.errormask4 = error_mask4;
				//переводим в натуральные величины для экономии места u32	_IQmpyI32int(_iq , u32) 
			fire_mode_not_complete_value.Ud			= (u16) _IQmpyI32int(load_s32(ramPnt, progPnt->Pnt_FM_Ud),moPnt->Base[NUM_VOLT_BASE]);
			fire_mode_not_complete_value.Iact		= (u16) _IQmpyI32int(load_s32(ramPnt, progPnt->Pnt_FM_Iact),moPnt->Base[NUM_CUR_BASE]);	
			fire_mode_not_complete_value.Ifull		= (u16) _IQmpyI32int(load_s32(ramPnt, progPnt->Pnt_FM_Ifull),moPnt->Base[NUM_CUR_BASE]);
			fire_mode_not_complete_value.Ireact		= (u16) _IQmpyI32int(load_s32(ramPnt, progPnt->Pnt_FM_Ireact),moPnt->Base[NUM_CUR_BASE]);
			fire_mode_not_complete_value.Iu			= (u16) _IQmpyI32int(load_s32(ramPnt, progPnt->Pnt_FM_Iu),moPnt->Base[NUM_CUR_BASE]);
			fire_mode_not_complete_value.Iv			= (u16) _IQmpyI32int(load_s32(ramPnt, progPnt->Pnt_FM_Iv),moPnt->Base[NUM_CUR_BASE]);
			fire_mode_not_complete_value.Iw			= (u16) _IQmpyI32int(load_s32(ramPnt, progPnt->Pnt_FM_Iw),moPnt->Base[NUM_CUR_BASE]);
			fire_mode_not_complete_value.Fout		= (u16) _IQmpyI32int(load_s32(ramPnt, progPnt->Pnt_FM_Fout),moPnt->Base[NUM_FREQ_BASE]);
				
			fire_mode_rec.crc = crcPacket((u8 *)&fire_mode_not_complete_value, sizeof(Fire_mode_not_complete_type) / 2);
			//формирование записи для журнала
			fire_mode_rec.unixtime 		= fire_mode_not_complete_value.unixtime;
			fire_mode_rec.errormask1 	= fire_mode_not_complete_value.errormask1;
			fire_mode_rec.errormask2 	= fire_mode_not_complete_value.errormask2;
			fire_mode_rec.errormask3 	= fire_mode_not_complete_value.errormask3;
			fire_mode_rec.errormask4 	= fire_mode_not_complete_value.errormask4;
			fire_mode_rec.Ud			= fire_mode_not_complete_value.Ud;
			fire_mode_rec.Iact			= fire_mode_not_complete_value.Iact;	
			fire_mode_rec.Ifull			= fire_mode_not_complete_value.Ifull;
			fire_mode_rec.Ireact		= fire_mode_not_complete_value.Ireact;
			fire_mode_rec.Iu			= fire_mode_not_complete_value.Iu;
			fire_mode_rec.Iv			= fire_mode_not_complete_value.Iv;
			fire_mode_rec.Iw			= fire_mode_not_complete_value.Iw;
			fire_mode_rec.Fout			= fire_mode_not_complete_value.Fout;
			//Запись в ФРАМ
			nvAdr = (void*) ((u32)&nv.fire_list.rec[sPnt->fire_mode_iterator]);									
			WrFramMicro(nvAdr, (void *)&fire_mode_rec, sizeof(Fire_mode_rec_type));
			
			sPnt->fire_mode_iterator++; //след запись будет в след ячейку

			
		}
	}
	
	if (load_s16(ramPnt, progPnt->Pnt_Block_FM) == 0 ){					//Активация ПР
		tmp = (u16)load_s16(ramPnt, progPnt->Pnt_Sensor_Din);
		if (tmp != 0){
			tmp--;
			if ( load_s16(ramPnt, progPnt->Pnt_Sensor_Type) == 0){
				//Норм разромкнут
				if( (load_s16(ramPnt, progPnt->Pnt_Din0_7) >> tmp) & 0x01){
					setBitMicro(ramPnt, progPnt->Pnt_FM_On);
					sPnt->fire_mode_on = 1;
					if (load_s16(ramPnt, progPnt->Pnt_Task_Source)){
						//Если так, то у нас выбран ПИД
						save_s16(ramPnt, progPnt->Pnt_Task_sc1, SOURCE_PID);
						save_s16(ramPnt, progPnt->Pnt_Task_sc2, SOURCE_PID);
						save_s16(ramPnt, progPnt->Pnt_PID_FB, load_s16(ramPnt, progPnt->Pnt_PID_Feedback));
						save_s16(ramPnt, progPnt->Pnt_PID_task, load_s16(ramPnt, progPnt->Pnt_PID_Source));
					}
					else{
						//Если так, то у нас фиксированная скорость
						save_s16(ramPnt, progPnt->Pnt_Task_sc1, SOURCE_FM);
						save_s16(ramPnt, progPnt->Pnt_Task_sc2, SOURCE_FM);
					}
				}
			}
			else {
				//Норм замкнут
				if( (( ~ (load_s16(ramPnt, progPnt->Pnt_Din0_7))) >> tmp) & 0x01){
					setBitMicro(ramPnt, progPnt->Pnt_FM_On);
					sPnt->fire_mode_on = 1;
					if (load_s16(ramPnt, progPnt->Pnt_Task_Source)){
						//Если так, то у нас выбран ПИД
						save_s16(ramPnt, progPnt->Pnt_Task_sc1, SOURCE_PID);
						save_s16(ramPnt, progPnt->Pnt_Task_sc2, SOURCE_PID);
						save_s16(ramPnt, progPnt->Pnt_PID_FB, load_s16(ramPnt, progPnt->Pnt_PID_Feedback));
						save_s16(ramPnt, progPnt->Pnt_PID_task, load_s16(ramPnt, progPnt->Pnt_PID_Source));
					}
					else{
						//Если так, то у нас фиксированная скорость
						save_s16(ramPnt, progPnt->Pnt_Task_sc1, SOURCE_FM);
						save_s16(ramPnt, progPnt->Pnt_Task_sc2, SOURCE_FM);
					}
				}
			}
		}
	}
	else {
		clrBitMicro(ramPnt, progPnt->Pnt_FM_On);
		sPnt->fire_mode_on = 0;
	}
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFIRE_MODE_1[27]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Block_FM
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Sensor_Din
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Sensor_Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Din0_7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Task_Source
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  PID_Source
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  PID_Feedback
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  FM_Ud
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  FM_Iact
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  FM_Ifull
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  FM_Ireact
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  FM_Iu
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  FM_Iv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  FM_Iw
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  FM_Fout
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  FM_Unixtime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  FM_ErrMask1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  FM_ErrMask2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  FM_ErrMask3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  FM_ErrMask4
		S16_PIN_TYPE | INPUT_PIN_MODE,	   //21  UnlockJournal
		S16_PIN_TYPE | INPUT_PIN_MODE,	   //22	 PID_FB
		S16_PIN_TYPE | INPUT_PIN_MODE,	   //23	 PID_task
		S16_PIN_TYPE | INPUT_PIN_MODE,	   //24	 Task_sc1
		S16_PIN_TYPE | INPUT_PIN_MODE,	   //25	 Task_sc2
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  FM_Mutex
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //27  FM_On
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FIRE_MODE", "DA", TblFuncFIRE_MODE_1, FuncFIRE_MODE_1, sizeof(TblFuncFIRE_MODE_1), sizeof(FuncFIRE_MODE_State_type), sizeof(nvFuncFIRE_MODE_State_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFIRE_MODE_1,
#endif
//********************************************************
