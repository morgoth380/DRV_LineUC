#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Wdv;                          //1  Wdv
    u16  Pnt_Kdv;                          //2  Kdv
    u16  Pnt_Idv;                          //3  Idv
    u16  Pnt_Kdv_1;                        //4  Kdv
    u16  Pnt_Torq_dv;                      //5  Torq_dv
    u16  Pnt_K_Torq;                       //6  K_Torq
    u16  Pnt_Ustat;                        //7  Ustat
    u16  Pnt_K_Ustat;                      //8  K_Ustat
    u16  Pnt_State;                        //9  State
    u16  Pnt_K_State;                      //10  K_State
    u16  Pnt_F_stator;                     //11  F_stator
    u16  Pnt_K_Fstator;                    //12  K_Fstator
    u16  Pnt_Udc;                          //13  Udc
    u16  Pnt_K_Udc;                        //14  K_Udc
    u16  Pnt_Power;                        //15  Power
    u16  Pnt_K_Power;                      //16  K_Power
    u16  Pnt_T_preob;                      //17  T_preob
    u16  Pnt_K_Tpreob;                     //18  K_Tpreob
    u16  Pnt_T_drive;                      //19  T_drive
    u16  Pnt_K_Tdrive;                     //20  K_Tdrive
    u16  Pnt_ZIP1;                         //21  ZIP1
    u16  Pnt_ZIP2;                         //22  ZIP2
    u16  Pnt_ZIP3;                         //23  ZIP3
    u16  Pnt_ZIP4;                         //24  ZIP4
    u16  Pnt_ZIP5;                         //25  ZIP5
    u16  Pnt_ZIP6;                         //26  ZIP6
    u16  Pnt_ZIP7;                         //27  ZIP7
    u16  Pnt_ZIP8;                         //28  ZIP8
    u16  Pnt_ZIP9;                         //29  ZIP9
    u16  Pnt_ZIP10;                        //30  ZIP10
    u16  Pnt_OUT10;                        //31  OUT10
    u16  Pnt_OUT9;                         //32  OUT9
    u16  Pnt_OUT8;                         //33  OUT8
    u16  Pnt_OUT7;                         //34  OUT7
    u16  Pnt_OUT6;                         //35  OUT6
    u16  Pnt_OUT5;                         //36  OUT5
    u16  Pnt_OUT4;                         //37  OUT4
    u16  Pnt_OUT3;                         //38  OUT3
    u16  Pnt_OUT2;                         //39  OUT2
    u16  Pnt_OUT1;                         //40  OUT1
    u16  Pnt_T_DRV_OUT;                    //41  T_DRV_OUT
    u16  Pnt_T_PR_OUT;                     //42  T_PR_OUT
    u16  Pnt_POWER_OUT;                    //43  POWER_OUT
    u16  Pnt_Udc_OUT;                      //44  Udc_OUT
    u16  Pnt_F_STAT_OUT;                   //45  F_STAT_OUT
    u16  Pnt_STATE_OUT;                    //46  STATE_OUT
    u16  Pnt_Ustate_OUT;                   //47  Ustate_OUT
    u16  Pnt_Torq_OUT;                     //48  Torq_OUT
    u16  Pnt_Idv_OUT;                      //49  Idv_OUT
    u16  Pnt_Wdv_OUT;                      //50  Wdv_OUT
    u16  Pnt_End;
}FuncKOEFF_ASU_type;

extern u16		SequenceModuleCall; // Для прореживания вызова части модулей
//********************************************************
u16 *FuncKOEFF_ASU_1(FuncKOEFF_ASU_type *progPnt, u32 ramPnt)
{  u16 temp;
   f32 tempF;
   _iq maxFreq;
   s16 s16_temp;
   
//   return &progPnt->Pnt_End;
#ifdef PARTLY_EXECUTE_MICRO
 	if ((SequenceModuleCall & 3) != 2) // Вызываем данную функцию только при значении 2
 		return &progPnt->Pnt_End;
#endif
//====================1===скорость==========================
    maxFreq = load_s32(ramPnt, progPnt->Pnt_ZIP2);//!Максимальная частота


    temp  = load_s16(ramPnt, progPnt->Pnt_Wdv);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_Kdv), d10);
	if(temp > 0){
		tempF = tempF/(f32)temp;
		s16_temp = (s16)(_IQtoF(maxFreq) * 80.0 / tempF + 0.5);
	}

	save_s16(ramPnt, progPnt->Pnt_OUT1, s16_temp);   //!Ограничение FMAX
	save_s16(ramPnt, progPnt->Pnt_OUT2, -s16_temp);  //!Ограничение FMIN

	save_float(ramPnt,progPnt->Pnt_Wdv_OUT,tempF ); //проп
//====================2===ток===========Idv===============
	temp  = load_s16(ramPnt, progPnt->Pnt_Idv);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_Kdv_1), d10);
	if(temp > 0){
		tempF = tempF / (f32)temp;
	}
	save_float(ramPnt,progPnt->Pnt_Idv_OUT,tempF); //проп
//====================3===момент===========Torq_dv===============
	temp  = load_s16(ramPnt, progPnt->Pnt_Torq_dv);//20000
	tempF = load_s16(ramPnt, progPnt->Pnt_K_Torq);
	if(temp > 0){
		tempF = tempF/(f32)temp;
	}
	save_float(ramPnt,progPnt->Pnt_Torq_OUT, tempF); //проп
//====================4==напряжение статора===========Ustate_OUT==============
	temp  = load_s16(ramPnt, progPnt->Pnt_Ustat);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_K_Ustat), d10);
	if(temp > 0){
		tempF = tempF/(f32)temp;
	} 
	save_float(ramPnt,progPnt->Pnt_Ustate_OUT,tempF); //проп
//====================5==положение вала===========State==============
	temp  = load_s16(ramPnt, progPnt->Pnt_State);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_K_State), d10);
	if(temp > 0){
		tempF = tempF / (f32)temp;
	} 
	save_float(ramPnt,progPnt->Pnt_STATE_OUT,tempF); //проп
//====================6==частота статора==========F_stator=============
	temp  = load_s16(ramPnt, progPnt->Pnt_F_stator);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_K_Fstator), d10);
	if(temp > 0){
		tempF = tempF / (f32)temp;
	}
	save_float(ramPnt,progPnt->Pnt_F_STAT_OUT,tempF); //проп
//====================7==напряж постоянного тока==========Udc===========
#ifdef _PROJECT_FOR_CCS_
#warning - Лишний расчет коэф для Udc, его надо привязать к простому напряжению статора
#endif
	temp  = load_s16(ramPnt, progPnt->Pnt_Udc);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_K_Udc), d10);
	if(temp > 0){
		tempF = tempF / (f32)temp;
	}
	save_float(ramPnt, progPnt->Pnt_Udc_OUT, tempF); //проп
//====================8==мощность==========Power==========
	temp  = load_s16(ramPnt, progPnt->Pnt_Power);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_K_Power), d10);
	if(temp > 0){
		tempF = tempF / (f32)temp;
	}
	save_float(ramPnt, progPnt->Pnt_POWER_OUT, tempF); //проп
//====================9==темпер преобр==========T_preob==========
	temp  = load_s16(ramPnt, progPnt->Pnt_T_preob);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_K_Tpreob), d10);
	if(temp > 0){
		tempF = tempF / (f32)temp;
	}
	save_float(ramPnt, progPnt->Pnt_T_PR_OUT, tempF); //проп
//====================9==темпер двигат==========T_drive==========
	temp  = load_s16(ramPnt, progPnt->Pnt_T_drive);//20000
	tempF = int2f(load_s16(ramPnt, progPnt->Pnt_K_Tdrive), d10);
	if(temp > 0){
		tempF = tempF / (f32)temp;
	}
	save_float(ramPnt, progPnt->Pnt_T_DRV_OUT, tempF); //проп
 
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncKOEFF_ASU_1[50]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Wdv
		S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Kdv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Idv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Kdv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Torq_dv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  K_Torq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Ustat
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  K_Ustat
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  State
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  K_State
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  F_stator
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  K_Fstator
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Udc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  K_Udc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Power
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  K_Power
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  T_preob
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  K_Tpreob
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  T_drive
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  K_Tdrive
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  ZIP1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22  ZIP2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  ZIP3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  ZIP4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  ZIP5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  ZIP6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  ZIP7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  ZIP8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  ZIP9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  ZIP10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  OUT10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  OUT9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //33  OUT8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //34  OUT7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //35  OUT6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //36  OUT5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //37  OUT4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //38  OUT3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //39  OUT2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //40  OUT1
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //41  T_DRV_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //42  T_PR_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //43  POWER_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //44  Udc_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //45  F_STAT_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //46  STATE_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //47  Ustate_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //48  Torq_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //49  Idv_OUT
		FLOAT_PIN_TYPE | OUTPUT_PIN_MODE     //50  Wdv_OUT
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("KOEFF_ASU", "U", TblFuncKOEFF_ASU_1, FuncKOEFF_ASU_1, sizeof(TblFuncKOEFF_ASU_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncKOEFF_ASU_1,
#endif
//********************************************************
