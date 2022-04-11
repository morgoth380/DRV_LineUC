#ifdef    GET_FUNC_CODE
typedef struct{
        s16	InRabPrv[10];
        s16	InVihPrv[10];
}FuncTRANZITState_type;
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16 Pnt_InRab[10];  // 1...10 моменты времени перехода с одного интервала работы на другой при работе в рабочий день. максимум 23:59
    u16 Pnt_InVih[10];  //11...20 моменты времени перехода с одного интервала работы на другой при работе в выходной день. максимум 23:59
    u16 Pnt_OutRab[10]; //21...30 величины задания, соответствующие различным интервалам работы при работе в рабочий день.
    u16 Pnt_OutVih[10]; //31...40 величины задания, соответствующие различным интервалам работы при работе в выходной день.
/*
    u16  Pnt_IN1;                          //1  IN1
    u16  Pnt_IN2;                          //2  IN2
    u16  Pnt_IN3;                          //3  IN3
    u16  Pnt_IN4;                          //4  IN4
    u16  Pnt_IN5;                          //5  IN5
    u16  Pnt_IN6;                          //6  IN6
    u16  Pnt_IN7;                          //7  IN7
    u16  Pnt_IN8;                          //8  IN8
    u16  Pnt_IN9;                          //9  IN9
    u16  Pnt_IN10;                         //10  IN10
    u16  Pnt_IN11;                         //11  IN11
    u16  Pnt_IN12;                         //12  IN12
    u16  Pnt_IN13;                         //13  IN13
    u16  Pnt_IN14;                         //14  IN14
    u16  Pnt_IN15;                         //15  IN15
    u16  Pnt_IN16;                         //16  IN16
    u16  Pnt_IN17;                         //17  IN17
    u16  Pnt_IN18;                         //18  IN18
    u16  Pnt_IN19;                         //19  IN19
    u16  Pnt_IN20;                         //20  IN20
    u16  Pnt_OUT1;                         //21  OUT1
    u16  Pnt_OUT2;                         //22  OUT2
    u16  Pnt_OUT3;                         //23  OUT3
    u16  Pnt_OUT4;                         //24  OUT4
    u16  Pnt_OUT5;                         //25  OUT5
    u16  Pnt_OUT6;                         //26  OUT6
    u16  Pnt_OUT7;                         //27  OUT7
    u16  Pnt_OUT8;                         //28  OUT8
    u16  Pnt_OUT9;                         //29  OUT9
    u16  Pnt_OUT10;                        //30  OUT10
    u16  Pnt_OUT11;                        //31  OUT11
    u16  Pnt_OUT12;                        //32  OUT12
    u16  Pnt_OUT13;                        //33  OUT13
    u16  Pnt_OUT14;                        //34  OUT14
    u16  Pnt_OUT15;                        //35  OUT15
    u16  Pnt_OUT16;                        //36  OUT16
    u16  Pnt_OUT17;                        //37  OUT17
    u16  Pnt_OUT18;                        //38  OUT18
    u16  Pnt_OUT19;                        //39  OUT19
    u16  Pnt_OUT20;                        //40  OUT20
*/

    u16  Pnt_End;
}FuncTRANZIT_type;
//********************************************************
u16 *FuncTRANZIT_1(FuncTRANZIT_type *progPnt, u32 ramPnt)
{
	u16 i;
	s16 min,max,cur;
	FuncTRANZITState_type    *sPnt;
    sPnt = (FuncTRANZITState_type *)(ramPnt + progPnt->Pnt_State);

    if(flagIsFirstCall)
    {
    	for(i=0;i<10;i++)
    	{
    		//cur = load_s16(ramPnt, progPnt->Pnt_InRab[i]);
    		//save_s16(ramPnt, progPnt->Pnt_OutRab[i], cur);
    		sPnt->InRabPrv[i] = 0;
    		//cur = load_s16(ramPnt, progPnt->Pnt_InVih[i]);
    		//save_s16(ramPnt, progPnt->Pnt_OutVih[i], cur);
    		sPnt->InVihPrv[i] = 0;
    	}
    }

//////////////////////////

    save_s16(ramPnt, progPnt->Pnt_OutRab[0], load_s16(ramPnt, progPnt->Pnt_InRab[0]));
	for (i = 1; i < 9; i ++) //
	{
		cur = load_s16(ramPnt, progPnt->Pnt_InRab[i]);    //счит. время перехода на следующее задание (относительно текущего)
		min = load_s16(ramPnt, progPnt->Pnt_InRab[i-1]);  //счит. время перехода на предыдущее задание
		max = load_s16(ramPnt, progPnt->Pnt_InRab[i+1]);  //
		if (cur != sPnt->InRabPrv[i]) //если введено значение, отличное от предыдущего сохраненного
		{
			if (cur < min ) //если перепутаны времена? то есть если текущее время меньше предыдущего
			{
				save_s16(ramPnt, progPnt->Pnt_InRab[i], min);
				cur = min;
			}
			else if (cur > max )
			{
				save_s16(ramPnt, progPnt->Pnt_InRab[i], max);
				cur = max;
			}
			save_s16(ramPnt, progPnt->Pnt_OutRab[i], cur);
			sPnt->InRabPrv[i] = cur;
		}
	}
	min = load_s16(ramPnt, progPnt->Pnt_InRab[8]);
	cur = load_s16(ramPnt, progPnt->Pnt_InRab[9]);
	if (cur < min )
	{
		save_s16(ramPnt, progPnt->Pnt_InRab[9], min);
		cur = min;
	}
	save_s16(ramPnt, progPnt->Pnt_OutRab[9], cur);
//////////////////////////////////
    save_s16(ramPnt, progPnt->Pnt_OutRab[0], load_s16(ramPnt, progPnt->Pnt_InRab[0]));
	for (i = 1; i < 9; i ++)
	{
		cur = load_s16(ramPnt, progPnt->Pnt_InVih[i]);
		min = load_s16(ramPnt, progPnt->Pnt_InVih[i-1]);
		max = load_s16(ramPnt, progPnt->Pnt_InVih[i+1]);
		if (cur != sPnt->InVihPrv[i])
		{
			if (cur < min )
			{
				save_s16(ramPnt, progPnt->Pnt_InVih[i], min);
				cur = min;
			}
			else if (cur > max )
			{
				save_s16(ramPnt, progPnt->Pnt_InVih[i], max);
				cur = max;
			}
			save_s16(ramPnt, progPnt->Pnt_OutVih[i], cur);
			sPnt->InVihPrv[i] = cur;
		}
	}
	min = load_s16(ramPnt, progPnt->Pnt_InVih[8]);
	cur = load_s16(ramPnt, progPnt->Pnt_InVih[9]);
	if (cur < min )
	{
		save_s16(ramPnt, progPnt->Pnt_InVih[9], min);
		cur = min;
	}
	save_s16(ramPnt, progPnt->Pnt_OutVih[9], cur);
//////////////////////////////////

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTRANZIT_1[40]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  IN1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  IN2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  IN3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  IN4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  IN5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  IN6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  IN7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  IN8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  IN9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  IN10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  IN11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  IN12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  IN13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  IN14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  IN15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  IN16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  IN17
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  IN18
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  IN19
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  IN20
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //21  OUT1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //22  OUT2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  OUT3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  OUT4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  OUT5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  OUT6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  OUT7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  OUT8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  OUT9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  OUT10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  OUT11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  OUT12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //33  OUT13
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //34  OUT14
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //35  OUT15
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //36  OUT16
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //37  OUT17
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //38  OUT18
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //39  OUT19
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //40  OUT20
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TRANZIT", "DD", TblFuncTRANZIT_1, FuncTRANZIT_1, sizeof(TblFuncTRANZIT_1), sizeof(FuncTRANZITState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTRANZIT_1,
#endif
//********************************************************
