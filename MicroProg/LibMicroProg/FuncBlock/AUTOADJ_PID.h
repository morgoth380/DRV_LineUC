#ifdef    GET_FUNC_CODE

typedef struct{
	f32/*_iq*/     Kp;
    s16     Ti;
    s16     Td;
    u16     autoflag;
}NvStateFuncAUTOADJ_PID_type;

typedef struct{
	u32     cnt;
    f32     fbDelta;
    f32     fbPred;
    f32     fb10;
    f32     fb63;
    u32     t0;
    u32     t0i;
	
	f32/*_iq*/     fout;
	
	NvStateFuncAUTOADJ_PID_type nvAA;
}StateFuncAUTOADJ_PID_type;

#define     nvSAUTOADJ_PID     (*(NvStateFuncAUTOADJ_PID_type*)0)

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Delta_F;                      //1  Delta_F
    u16  Pnt_Hyster;                       //2  Hyster
    u16  Pnt_On_Off;                       //3  On_Off
    u16  Pnt_FB_PID;                       //4  FB_PID
    u16  Pnt_Temp;                         //5  Temp
    u16  Pnt_Wait;                         //6  Wait
    u16  Pnt_Type;                         //7  Type
    u16  Pnt_Time;                         //8  Time
    u16  Pnt_PWMbit;                       //9  PWMbit
    u16  Pnt_GTUPbit;                      //10  GTUPbit
    u16  Pnt_GTDNbit;                      //11  GTDNbit
    u16  Pnt_GTEQbit;                      //12  GTEQbit
    u16  Pnt_GTZERO;                       //13  GTZERO
    u16  Pnt_BIT_AA;                       //14  BIT_AA
    u16  Pnt_Fout;                         //15  Fout
    u16  Pnt_Td;                           //16  Td
    u16  Pnt_Ti;                           //17  Ti
    u16  Pnt_Kp;                           //18  Kp
    u16  Pnt_End;
}FuncAUTOADJ_PID_type;
//********************************************************
u16 *FuncAUTOADJ_PID_1(FuncAUTOADJ_PID_type *progPnt, u32 ramPnt)
{
/*	
	_iq deltaF;
	u32 Kp;
	f32 temp, fb, tga;
	u16 Ti, Td, Start;
	MPOpis_type                     *moPnt;
	StateFuncAUTOADJ_PID_type *sPnt;
	NvStateFuncAUTOADJ_PID_type           *nvAdr;


    nvAdr       = GetNvAdr(progPnt->Pnt_nvState, &nvSAUTOADJ_PID);
	sPnt = (StateFuncAUTOADJ_PID_type *)(ramPnt + progPnt->Pnt_State);
	moPnt = (MPOpis_type *)&MPOpis;

	if(flagIsFirstCall){
		sPnt->fbPred = 0;
		sPnt->fb10 = 0;
		sPnt->fb63 = 0;
		sPnt->cnt = 0;
		RdFramMicro(&sPnt->nvAA, nvAdr, sizeof(NvStateFuncAUTOADJ_PID_type));
		if(sPnt->nvAA.autoflag == 1){
			save_s32(ramPnt, progPnt->Pnt_Kp, sPnt->nvAA.Kp);
			save_s16(ramPnt, progPnt->Pnt_Ti, sPnt->nvAA.Ti);
			save_s16(ramPnt, progPnt->Pnt_Td, sPnt->nvAA.Td);
		}
		return &progPnt->Pnt_End;
	}
	Kp = load_s32(ramPnt, progPnt->Pnt_Kp);
	Ti = load_s16(ramPnt, progPnt->Pnt_Ti);
	Td = load_s16(ramPnt, progPnt->Pnt_Td);
	if((sPnt->nvAA.autoflag == 1) & ((sPnt->nvAA.Kp != Kp) | (sPnt->nvAA.Ti != Ti) | (sPnt->nvAA.Td != Td))){
		sPnt->nvAA.Kp = Kp;
		sPnt->nvAA.Ti = Ti;
		sPnt->nvAA.Td = Td;
		WrFramMicro(nvAdr, &sPnt->nvAA, sizeof(NvStateFuncAUTOADJ_PID_type));
	}

	temp = (float)load_s16(ramPnt, progPnt->Pnt_Temp);   //Темп изменения частоты Гц/с для автонастройки коэффициентов ПИД-регулятора.!!!!!!!!!!!!!!!!!
	if(load_s16(ramPnt, progPnt->Pnt_On_Off)){
		fb = _IQtoF(load_s32(ramPnt, progPnt->Pnt_FB_PID));
		sPnt->fbDelta = _IQtoF(load_s32(ramPnt, progPnt->Pnt_Hyster));
		deltaF = load_s32(ramPnt, progPnt->Pnt_Delta_F);

		if(sPnt->fb10 == 0){
			//sPnt->fbPred = fb;
			//sPnt->fbDelta = fbmax - sPnt->fbPred;
			sPnt->fb10 = fb + sPnt->fbDelta * 0.1f;
			sPnt->fb63 = fb + sPnt->fbDelta * 0.63f;
			sPnt->cnt = 0;
		}

        if(testBitMicro(ramPnt, progPnt->Pnt_PWMbit)){
	        if(testBitMicro(ramPnt, progPnt->Pnt_GTUPbit) ||
	           testBitMicro(ramPnt, progPnt->Pnt_PWMbit)  ||
	           testBitMicro(ramPnt, progPnt->Pnt_GTEQbit) ||
	           testBitMicro(ramPnt, progPnt->Pnt_GTZERO)){

	        	Start = 0;
	        }
	        else Start = 1;
		}
		else Start = 0;

        if(Start){
			if(sPnt->fout < deltaF){
				sPnt->fout += _IQ(temp / ((f32)moPnt->Base[NUM_FREQ_BASE]) / FLOAT_TIME_CALL_MICRO);
				if(sPnt->fout > deltaF) sPnt->fout = deltaF;
			}
			else{
				if((fb > sPnt->fb10) & (sPnt->t0 == 0)) sPnt->t0 = sPnt->cnt;
				if((fb > sPnt->fb63) & (sPnt->t0i == 0)) sPnt->t0i = sPnt->cnt;
				sPnt->cnt++;
			}
			if(sPnt->t0i != 0){
				tga = 100 * sPnt->fbDelta / (_IQtoF(deltaF) * ((f32)(sPnt->t0i - sPnt->t0) / 250.f));
				switch(load_s16(ramPnt, progPnt->Pnt_Type)){
				case 0:
					sPnt->nvAA.Kp = _IQ(1 / tga * ((f32)sPnt->t0) / K_TIME_CALL_MICRO_TAKT);
					sPnt->nvAA.Ti = 0;
					sPnt->nvAA.Td = 0;
					sPnt->nvAA.autoflag = 1;
					WrFramMicro(nvAdr, &sPnt->nvAA, sizeof(NvStateFuncAUTOADJ_PID_type));
					save_s32(ramPnt, progPnt->Pnt_Kp, sPnt->nvAA.Kp);
					save_s16(ramPnt, progPnt->Pnt_Ti, sPnt->nvAA.Ti);
					save_s16(ramPnt, progPnt->Pnt_Td, sPnt->nvAA.Td);
					break;
				case 1:
					sPnt->nvAA.Kp = _IQ(0.8f / tga * ((f32)sPnt->t0) / K_TIME_CALL_MICRO_TAKT);
					sPnt->nvAA.Ti = (u16)(3 * ((f32)sPnt->t0) / K_TIME_CALL_MICRO_TAKT);
					sPnt->nvAA.Td = 0;
					sPnt->nvAA.autoflag = 1;
					WrFramMicro(nvAdr, &sPnt->nvAA, sizeof(NvStateFuncAUTOADJ_PID_type));
					save_s32(ramPnt, progPnt->Pnt_Kp, sPnt->nvAA.Kp);
					save_s16(ramPnt, progPnt->Pnt_Ti, sPnt->nvAA.Ti);
					save_s16(ramPnt, progPnt->Pnt_Td, sPnt->nvAA.Td);
					break;
				case 2:
					sPnt->nvAA.Kp = _IQ(1.2f / tga * ((f32)sPnt->t0) / K_TIME_CALL_MICRO_TAKT);
					sPnt->nvAA.Ti = (u16)(2 * ((f32)sPnt->t0) / K_TIME_CALL_MICRO_TAKT);
					sPnt->nvAA.Td = (u16)(0.4f * ((f32)sPnt->t0) / K_TIME_CALL_MICRO_TAKT);
					sPnt->nvAA.autoflag = 1;
					WrFramMicro(nvAdr, &sPnt->nvAA, sizeof(NvStateFuncAUTOADJ_PID_type));
					save_s32(ramPnt, progPnt->Pnt_Kp, sPnt->nvAA.Kp);
					save_s16(ramPnt, progPnt->Pnt_Ti, sPnt->nvAA.Ti);
					save_s16(ramPnt, progPnt->Pnt_Td, sPnt->nvAA.Td);
					break;
				}
				save_s16(ramPnt, progPnt->Pnt_On_Off, 0);
			}

			save_s32(ramPnt, progPnt->Pnt_Fout, sPnt->fout);
			setBitMicro(ramPnt, progPnt->Pnt_BIT_AA);
		}
        else{
			save_s16(ramPnt, progPnt->Pnt_On_Off, 0);
		}
	}
    else{
        if(sPnt->fout > 0){
            sPnt->fout -= _IQ(temp / ((f32)moPnt->Base[NUM_FREQ_BASE]) / FLOAT_TIME_CALL_MICRO);
		}
        else{
			clrBitMicro(ramPnt, progPnt->Pnt_BIT_AA);
		}
		if(sPnt->fout < 0) sPnt->fout = 0;
		sPnt->fbPred = 0;
		sPnt->fb10 = 0;
		sPnt->fb63 = 0;
		sPnt->cnt = 0;
		sPnt->t0i = 0;
		sPnt->t0 = 0;
		save_s32(ramPnt, progPnt->Pnt_Fout, sPnt->fout);
	}
*/
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAUTOADJ_PID_1[18]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Delta_F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Hyster
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  On_Off
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  FB_PID
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Temp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Wait
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Type
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Time
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  PWMbit
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  GTUPbit
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  GTDNbit
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  GTEQbit
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  GTZERO
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14  BIT_AA
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //15  Fout
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  Td
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  Ti
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //18  Kp
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AUTOADJ_PID", "DA", TblFuncAUTOADJ_PID_1, FuncAUTOADJ_PID_1, sizeof(TblFuncAUTOADJ_PID_1), /*sizeof(StateFuncAUTOADJ_PID_type)*/0, /*sizeof(NvStateFuncAUTOADJ_PID_type)*/0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAUTOADJ_PID_1,
#endif
//********************************************************
