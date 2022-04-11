#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Press[5];                     //1 - 5  Press_1 - Press_5
    u16  Pnt_Fail_in[5];                   //6 - 10  Fail_in1 - Fail_in5
    u16  Pnt_Sig_in1;                      //11  Sig_in1
    u16  Pnt_Sig_in2;                      //12  Sig_in2
    u16  Pnt_LimDiff;                      //13  LimDiff
    u16  Pnt_AlarmDiff;                    //14  AlarmDiff
    u16  Pnt_Hister;                       //15  Hister
    u16  Pnt_Enable;                       //16  Enable
    u16  Pnt_AlarmPR;                      //17  AlarmPR
    u16  Pnt_Sig_out;                      //18  Sig_out
    u16  Pnt_End;
}FuncPRESS_RELE_type;
//********************************************************
u16 *FuncPRESS_RELE_1(FuncPRESS_RELE_type *progPnt, u32 ramPnt)
{
	s32 Source, Hist, Limit, Alrm, diff;
	s16 tmp, failAin;

    if(load_s16(ramPnt, progPnt->Pnt_Enable)){
    	tmp = load_s16(ramPnt, progPnt->Pnt_Sig_in1);         //Номер источника давления №1
    	failAin = load_s16(ramPnt, progPnt->Pnt_Fail_in[tmp]);
    	Source = load_s32(ramPnt, progPnt->Pnt_Press[tmp]);   //Источник давления №1
    	tmp = load_s16(ramPnt, progPnt->Pnt_Sig_in2);         //Номер источника давления №2
    	failAin += load_s16(ramPnt, progPnt->Pnt_Fail_in[tmp]);
    	diff = abs(Source - load_s32(ramPnt, progPnt->Pnt_Press[tmp]));
    	Hist = load_s32(ramPnt, progPnt->Pnt_Hister);         //Гистерезис срабатывания реле разности давления
    	Limit = load_s32(ramPnt, progPnt->Pnt_LimDiff);       //Порог срабатывания реле разности давления
    	if(Hist > Limit){
    		Hist = Limit;
    	}
    	Alrm = load_s32(ramPnt, progPnt->Pnt_AlarmDiff);      //Порог срабатывания предупреждения

    	/*if(failAin){                                          //Если есть аварии по аналоговым входам - отключаем обработку реле разности
        	clrBitMicro(ramPnt, progPnt->Pnt_AlarmPR);
        	clrBitMicro(ramPnt, progPnt->Pnt_Sig_out);
    	}
    	else{*/
        	if(diff >= Limit){
        		setBitMicro(ramPnt, progPnt->Pnt_Sig_out);
        	}
        	else if(diff < (Limit - Hist)){
        		clrBitMicro(ramPnt, progPnt->Pnt_Sig_out);
        	}

        	if(diff > Alrm){
        		setBitMicro(ramPnt, progPnt->Pnt_AlarmPR);
        	}
        	else{
        		clrBitMicro(ramPnt, progPnt->Pnt_AlarmPR);
        	}
 //   	}
    }
    else{
    	clrBitMicro(ramPnt, progPnt->Pnt_AlarmPR);
    	clrBitMicro(ramPnt, progPnt->Pnt_Sig_out);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPRESS_RELE_1[18]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Press_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Press_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Press_3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Press_4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Press_5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Fail_in1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Fail_in2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Fail_in3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Fail_in4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Fail_in5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Sig_in1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Sig_in2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  LimDiff
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  AlarmDiff
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  Hister
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Enable
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //17  AlarmPR
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //18  Sig_out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PRESS_RELE", "DA", TblFuncPRESS_RELE_1, FuncPRESS_RELE_1, sizeof(TblFuncPRESS_RELE_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPRESS_RELE_1,
#endif
//********************************************************
