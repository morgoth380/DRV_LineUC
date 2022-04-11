#ifdef    GET_FUNC_CODE

#ifdef _PROJECT_FOR_CCS_
#include "DrvEXT2.h"
#endif  //_PROJECT_FOR_CCS_

#define ext2_iq_filtr(valOld, valNew, Kfiltr)       (((valOld = valOld + (valNew - (valOld >> Kfiltr))) + ((1<<Kfiltr)-1)) >> Kfiltr)

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_ModeAin1;                     //1  ModeAin1
    u16  Pnt_ModeAin2;                     //2  ModeAin2
    u16  Pnt_ModeAin3;                     //3  ModeAin3
    u16  Pnt_ModeAout;                     //4  ModeAout
    u16  Pnt_CommAin1;                     //5  CommAin1
    u16  Pnt_CommAin2;                     //6  CommAin2
    u16  Pnt_CommAin3;                     //7  CommAin3
    u16  Pnt_CommAout;                     //8  CommAout
    u16  Pnt_OutTestMode;                  //9  OutTestMode
    u16  Pnt_T1_filter;                    //10  T1_filter
    u16  Pnt_T2_filter;                    //11  T2_filter
    u16  Pnt_T3_filter;                    //12  T3_filter
    u16  Pnt_Aout;                         //13  Aout
    u16  Pnt_TestAout;                     //14  TestAout
    u16  Pnt_EXT2_CRC;                     //15  EXT2_CRC
    u16  Pnt_StateM3;                      //16  StateM3
    u16  Pnt_StateM2;                      //17  StateM2
    u16  Pnt_StateM1;                      //18  StateM1
    u16  Pnt_Meas3;                        //19  Meas3
    u16  Pnt_Meas2;                        //20  Meas2
    u16  Pnt_Meas1;                        //21  Meas1
    u16  Pnt_End;
}FuncEXT_2_type;

//Command
typedef enum{
    nop,

    getMeas,                //Запрос измеренных значений
    getCrc,                 //Запрос CRC

    saveOffset1,
    saveOffset2,

    setTempMeas,            //Использовать вход как измеритель температуры
    setVoltageMeas,         //Использовать вход как измеритель напряжения
    setCurrent0_5maMeas,    //Использовать вход как измеритель тока 0_5 мА
    setCurrent4_20maMeas,   //Использовать вход как измеритель тока 4_4 мА
    saveOffsetU1,           //Сохранить смещение по первому измерителю канала
    saveOffsetU2,           //Сохранить смещение по второму измерителю канала
    save_pt100_R_0,         //Сохранить значение сопротивления pt100 при 0 °C
    save_pt100_R_150,       //Сохранить значение сопротивления pt100 при 150 °C
    save_pt1000_R_0,        //Сохранить значение сопротивления pt1000 при 0 °C
    save_pt1000_R_150,      //Сохранить значение сопротивления pt1000 при 150 °C
    save0v,                 //Сохранить значение при 0 В на входе канала
    save10v,                //Сохранить значение при 10 В на входе канала
    save0ma,
    save4ma,
    save5ma,
    save20ma,

    set_out_0_10v,          //Установить выход в режим 0_10 V, устаноановить выходное напряжение
    set_out_0_5ma,          //Установить выход в режим 0_5 мА, устаноановить выходной ток
    set_out_4_20ma,         //Установить выход в режим 4_20 мА, устаноановить выходной ток

    save_out_10v,
    save_out_4ma,
    save_out_5ma,
    save_out_20ma,

    set_out_voltage_pwm,    //Установить выход в режим 0_10 V, устаноановить выходной коэф. заполнения ШИМ
    set_out_0_5ma_pwm,      //Установить выход в режим 0_5 мА, устаноановить выходной коэф. заполнения ШИМ
    set_out_4_20ma_pwm,     //Установить выход в режим 4_20 мА, устаноановить выходной коэф. заполнения ШИМ
}Ext2cmd_type;

typedef enum{
    ch1,
    ch2,
    ch3,
    out,
}channelNum_type;


typedef struct{
	Ext2cmd_type        command;
    channelNum_type     channel;
    u16                 val;
}command_type;

//
typedef enum{
    chPt100_Ok,
    chPt1000_Ok,
    chVoltage_Ok,
    ch0_5ma_Ok,
    ch4_20ma_Ok,
    chShortCircuit,
    chBreakCircuit,
    chError
}chState_type;

typedef enum{
    chOut_Ok,
    chOut_CommandDone,
    chOut_Error,
}chOutState_type;

typedef struct{
    u16				res1;
    u16				res2;
    u16				res3;
    chState_type    state1      :4;
    chState_type    state2      :4;
    chState_type    state3      :4;
    chOutState_type stateOut    :4;
}resState_type;

typedef union{
    resState_type   res;
    u16				crc;
}txMsg_type;

typedef struct{
    u16 			ModeAin1Prev;       	//1  ModeAin1Prev
    u16  			ModeAin2Prev;         	//2  ModeAin2Prev
    u16  			ModeAin3Prev;        	//3  ModeAin3Prev
    u16  			ModeAoutPrev;         	//4  ModeAoutPrev
    command_type	commFofExt2;
    u32				res1Cumlul;				//Для фильтра
    u32				res2Cumlul;				//Для фильтра
    u32				res3Cumlul;				//Для фильтра

}Ext2_type;

//********************************************************
u16 *FuncEXT_2_1(FuncEXT_2_type *progPnt, u32 ramPnt)
{
#ifdef _PROJECT_FOR_CCS_
	s16 			r1, r2, r3;
	s16 			fr1, fr2, fr3;
	//s16 			t1, t2, t3;
	u16				commandNeedTx;
	command_type	commFofExt2;
	txMsg_type      *pMsgExtToNVSA;
	Ext2_type    	*sPnt;

	sPnt = (Ext2_type *)(ramPnt + progPnt->Pnt_State);
	commandNeedTx = 0;

	if(flagIsFirstCall){
		commFofExt2.command = getCrc;
		commFofExt2.channel = ch1;
		commandNeedTx = 1;
		sPnt->commFofExt2.command = getCrc;
		sPnt->commFofExt2.channel = ch1;
	}

	/**************************************
	* Отправляем команды, если изменился режим входа / выхода
	*/
	if((sPnt->ModeAin1Prev != load_s16(ramPnt, progPnt->Pnt_ModeAin1))&&(commandNeedTx == 0)){
		commFofExt2.command = (Ext2cmd_type)(setTempMeas + load_s16(ramPnt, progPnt->Pnt_ModeAin1));
		commFofExt2.channel = ch1;
		sPnt->ModeAin1Prev = load_s16(ramPnt, progPnt->Pnt_ModeAin1);
		commandNeedTx = 1;
	}
	if((sPnt->ModeAin2Prev != load_s16(ramPnt, progPnt->Pnt_ModeAin2))&&(commandNeedTx == 0)){
			commFofExt2.command = (Ext2cmd_type)(setTempMeas + load_s16(ramPnt, progPnt->Pnt_ModeAin2));
			commFofExt2.channel = ch2;
			sPnt->ModeAin2Prev = load_s16(ramPnt, progPnt->Pnt_ModeAin2);
			commandNeedTx = 1;
	}
	if((sPnt->ModeAin3Prev != load_s16(ramPnt, progPnt->Pnt_ModeAin3))&&(commandNeedTx == 0)){
			commFofExt2.command = (Ext2cmd_type)(setTempMeas + load_s16(ramPnt, progPnt->Pnt_ModeAin3));
			commFofExt2.channel = ch1;
			sPnt->ModeAin3Prev = load_s16(ramPnt, progPnt->Pnt_ModeAin3);
			commandNeedTx = 1;
	}
	if((sPnt->ModeAoutPrev != load_s16(ramPnt, progPnt->Pnt_ModeAout))&&(commandNeedTx == 0)){
			commFofExt2.command = (Ext2cmd_type)(set_out_0_10v + load_s16(ramPnt, progPnt->Pnt_ModeAout));
			commFofExt2.channel = ch1;
			sPnt->ModeAoutPrev = load_s16(ramPnt, progPnt->Pnt_ModeAout);
			commandNeedTx = 1;
	}

	/**************************************
	* Отправляем команды калибровки
	*/
	if((load_s16(ramPnt, progPnt->Pnt_CommAin1) != 0)&&(commandNeedTx == 0)){
		commFofExt2.command = (Ext2cmd_type)(saveOffsetU1 + load_s16(ramPnt, progPnt->Pnt_CommAin1) - 1);
		commFofExt2.channel = ch1;
		save_s16(ramPnt, progPnt->Pnt_CommAin1, 0);	//Сбрасываем
		commandNeedTx = 1;
	}
	if((load_s16(ramPnt, progPnt->Pnt_CommAin2) != 0)&&(commandNeedTx == 0)){
		commFofExt2.command = (Ext2cmd_type)(saveOffsetU1 + load_s16(ramPnt, progPnt->Pnt_CommAin2) - 1);
		commFofExt2.channel = ch2;
		save_s16(ramPnt, progPnt->Pnt_CommAin2, 0);	//Сбрасываем
		commandNeedTx = 1;
	}
	if((load_s16(ramPnt, progPnt->Pnt_CommAin3) != 0)&&(commandNeedTx == 0)){
		commFofExt2.command = (Ext2cmd_type)(saveOffsetU1 + load_s16(ramPnt, progPnt->Pnt_CommAin3) - 1);
		commFofExt2.channel = ch3;
		save_s16(ramPnt, progPnt->Pnt_CommAin3, 0);	//Сбрасываем
		commandNeedTx = 1;
	}
	if((load_s16(ramPnt, progPnt->Pnt_CommAout) != 0)&&(commandNeedTx == 0)){
		if(load_s16(ramPnt, progPnt->Pnt_CommAout) == 8){	//Уст ШИМ
			commFofExt2.command = (Ext2cmd_type)(set_out_voltage_pwm + load_s16(ramPnt, progPnt->Pnt_ModeAout));
		}
		else{
			commFofExt2.command = (Ext2cmd_type)(set_out_0_10v + load_s16(ramPnt, progPnt->Pnt_CommAout) - 1);
		}
		commFofExt2.val = ((s64)(load_s32(ramPnt, progPnt->Pnt_TestAout)) * 10000) / _IQ(1);
		commFofExt2.channel = ch1;
		save_s16(ramPnt, progPnt->Pnt_CommAout, 0);	//Сбрасываем
		commandNeedTx = 1;
	}

	/**************************************
	* Если небыло никаких команд, то нужно обновить значение выхода
	*/
	if((load_s16(ramPnt, progPnt->Pnt_OutTestMode) == 0)&&(commandNeedTx == 0)){
		commFofExt2.command = (Ext2cmd_type)(set_out_0_10v + load_s16(ramPnt, progPnt->Pnt_ModeAout));
		commFofExt2.val = ((s64)(load_s16(ramPnt, progPnt->Pnt_Aout)) * 10000) / _IQ(1);
		commandNeedTx = 1;
	}

    /**************************************
	* Распаковываем посылку
	*/
	pMsgExtToNVSA = (txMsg_type*)ucRXMsgData;
	//CRC
	if((commFofExt2.command != getCrc)&&(sPnt->commFofExt2.command == getCrc)){
		save_s16(ramPnt, progPnt->Pnt_EXT2_CRC, pMsgExtToNVSA->crc);
		sPnt->commFofExt2.command = nop;
	}

	//Результат в процентах Х_ХХ
	r1 = pMsgExtToNVSA->res.res1;
	r2 = pMsgExtToNVSA->res.res2;
	r3 = pMsgExtToNVSA->res.res3;

	//Фильтруем
	fr1 = ext2_iq_filtr(sPnt->res1Cumlul, r1, load_s16(ramPnt, progPnt->Pnt_T1_filter));
	fr2 = ext2_iq_filtr(sPnt->res2Cumlul, r2, load_s16(ramPnt, progPnt->Pnt_T2_filter));
	fr3 = ext2_iq_filtr(sPnt->res3Cumlul, r3, load_s16(ramPnt, progPnt->Pnt_T3_filter));

	save_s32(ramPnt, progPnt->Pnt_Meas1, ((s64)fr1 * _IQ(1)) / 10000);
	save_s32(ramPnt, progPnt->Pnt_Meas2, ((s64)fr2 * _IQ(1)) / 10000);
	save_s32(ramPnt, progPnt->Pnt_Meas3, ((s64)fr3 * _IQ(1)) / 10000);

	//State
	save_s16(ramPnt, progPnt->Pnt_StateM1, pMsgExtToNVSA->res.state1);
	save_s16(ramPnt, progPnt->Pnt_StateM2, pMsgExtToNVSA->res.state2);
	save_s16(ramPnt, progPnt->Pnt_StateM3, pMsgExtToNVSA->res.state3);

	//Нужно отправить ?
	if(commandNeedTx != 0){
		memcpy(ucTXMsgData, &commFofExt2, sizeof(command_type));
		g_MsgNeedSend = 1;
		Semaphore_post(SEM_EXT2);
	}

#endif  //_PROJECT_FOR_CCS_
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncEXT_2_1[21]={
        S16_PIN_TYPE | INPUT_PIN_MODE,   //1  ModeAin1
        S16_PIN_TYPE | INPUT_PIN_MODE,   //2  ModeAin2
        S16_PIN_TYPE | INPUT_PIN_MODE,   //3  ModeAin3
        S16_PIN_TYPE | INPUT_PIN_MODE,   //4  ModeAout
        S16_PIN_TYPE | INPUT_PIN_MODE,   //5  CommAin1
        S16_PIN_TYPE | INPUT_PIN_MODE,   //6  CommAin2
        S16_PIN_TYPE | INPUT_PIN_MODE,   //7  CommAin3
        S16_PIN_TYPE | INPUT_PIN_MODE,   //8  CommAout
        S16_PIN_TYPE | INPUT_PIN_MODE,   //9  OutTestMode
        S16_PIN_TYPE | INPUT_PIN_MODE,   //10  T1_filter
        S16_PIN_TYPE | INPUT_PIN_MODE,   //11  T2_filter
        S16_PIN_TYPE | INPUT_PIN_MODE,   //12  T3_filter
		S32_PIN_TYPE | INPUT_PIN_MODE,   //13  Aout
		S32_PIN_TYPE | INPUT_PIN_MODE,   //14  TestAout
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //15  EXT2_CRC
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //16  StateM3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //17  StateM2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //18  StateM1
		S32_PIN_TYPE | OUTPUT_PIN_MODE,   //19  Meas3
		S32_PIN_TYPE | OUTPUT_PIN_MODE,   //20  Meas2
		S32_PIN_TYPE | OUTPUT_PIN_MODE    //21  Meas1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock(	"EXT_2",
				"DA",
				TblFuncEXT_2_1,
				FuncEXT_2_1,
				sizeof(TblFuncEXT_2_1),
				sizeof(Ext2_type),
				0,
				1
				),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncEXT_2_1,
#endif
//********************************************************



	/*###############################################################*/
	//	//Вычисляем Meas
	//	save_s32(ramPnt, progPnt->Pnt_Meas1, (((s64)fr1)*_IQ(1))/10000);
	//	save_s32(ramPnt, progPnt->Pnt_Meas2, (((s64)fr2)*_IQ(1))/10000);
	//	save_s32(ramPnt, progPnt->Pnt_Meas3, (((s64)fr3)*_IQ(1))/10000);

	//	//
	//	if(load_s16(ramPnt, progPnt->Pnt_ModeAin1) == 0){	//Режим канала температурный
	//		t1 = (((s32)fr1 * 250) / 1000) -500;
	//		save_s16(ramPnt, progPnt->Pnt_MeasT1, t1);
	//		if(load_s16(ramPnt, progPnt->Pnt_T1_failureOnOff) != 0){	//Включена проверка
	//			if(t1 <= load_s16(ramPnt, progPnt->Pnt_T1_failure_Min))		save_s16(ramPnt, progPnt->Pnt_T1_failure, 1);
	//			if(t1 >= load_s16(ramPnt, progPnt->Pnt_T1_failure_Max))		save_s16(ramPnt, progPnt->Pnt_T1_failure, 1);
	//			if((t1 > load_s16(ramPnt, progPnt->Pnt_T1_failure_Min))&&
	//			   (t1 < load_s16(ramPnt, progPnt->Pnt_T1_failure_Max))){
	//				save_s16(ramPnt, progPnt->Pnt_T1_failure, 0);
	//			}
	//		}else{	//Выключена проверка
	//			save_s16(ramPnt, progPnt->Pnt_T1_failure, 0);
	//		}
	//	}else{	//Режим канала не температурный
	//		save_s16(ramPnt, progPnt->Pnt_MeasT1, 0);
	//		save_s16(ramPnt, progPnt->Pnt_T1_failure, 0);
	//	}
