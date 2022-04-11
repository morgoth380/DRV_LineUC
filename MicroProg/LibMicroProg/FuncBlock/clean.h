#ifdef    GET_FUNC_CODE

#define NONE          0
#define BY_START      1
#define BY_FLOW       2
#define BY_START_FLOW  3
#define BY_DI         4

typedef struct{
        u64  timeFWD;
        u64  timeREV;
        u16  cntClean;
        u16  clrCnt;
        union{
			struct{
				u16   st_RDY        :1;
				u16   st_INPROG_FWD :1;
				u16   st_INPROG_REV :1;
				u16   st_DONE       :1;
			}bit;
			u16 All;
        }st;
}CLEAN_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Type;                         //1  Type
    u16  Pnt_TimeFWD;                      //2  TimeFWD
    u16  Pnt_TimeREV;                      //3  TimeREV
    u16  Pnt_FreqFWD;                      //4  FreqFWD
    u16  Pnt_FreqREV;                      //5  FreqREV
    u16  Pnt_Start_flw;                    //6  Start_flw
    u16  Pnt_Start_DI;                     //7  Start_DI
    u16  Pnt_PWM;                          //8  PWM
    u16  Pnt_FREQ_STOP;                    //9  FREQ_STOP
    u16  Pnt_GT_RDY;                       //10  GT_RDY
    u16  Pnt_State_OUT;                    //11  State_OUT
    u16  Pnt_Fout;                         //12  Fout
    u16  Pnt_NumbCl;                       //13  NumbCl
    u16  Pnt_End;
}FuncCLEAN_type;
//********************************************************
u16 *FuncCLEAN_1(FuncCLEAN_type *progPnt, u32 ramPnt)
{
	u16 type;
	CLEAN_State_type *sPnt;
	sPnt = (CLEAN_State_type *)(ramPnt + progPnt->Pnt_State);

	if(flagIsFirstCall){
		sPnt->st.bit.st_RDY = 1;
		sPnt->st.bit.st_DONE = 0;
		sPnt->st.bit.st_INPROG_FWD = 0;
		sPnt->st.bit.st_INPROG_REV = 0;
		sPnt->timeFWD = 0;
		sPnt->timeREV = 0;
		sPnt->cntClean = 0;
		save_s16(ramPnt, progPnt->Pnt_NumbCl, sPnt->cntClean);
		return &progPnt->Pnt_End;
	}

	if(sPnt->st.bit.st_RDY){
		type = load_s16(ramPnt, progPnt->Pnt_Type);
		switch(type){
		case NONE:
			break;
		case BY_START:
			if(testBitMicro(ramPnt, progPnt->Pnt_PWM)){
				setBitMicro(ramPnt, progPnt->Pnt_State_OUT);
				save_s32(ramPnt, progPnt->Pnt_Fout, load_s32(ramPnt, progPnt->Pnt_FreqFWD));
				sPnt->timeFWD = load_s16(ramPnt, progPnt->Pnt_TimeFWD) * K_TIME_CALL_MICRO_TAKT;
				sPnt->st.bit.st_RDY = 0;
				sPnt->st.bit.st_INPROG_FWD = 1;
			}
			break;
		case BY_FLOW:
			if(testBitMicro(ramPnt, progPnt->Pnt_Start_flw)){
				setBitMicro(ramPnt, progPnt->Pnt_State_OUT);
				save_s32(ramPnt, progPnt->Pnt_Fout, load_s32(ramPnt, progPnt->Pnt_FreqFWD));
				sPnt->timeFWD = load_s16(ramPnt, progPnt->Pnt_TimeFWD) * K_TIME_CALL_MICRO_TAKT;
				sPnt->st.bit.st_RDY = 0;
				sPnt->st.bit.st_INPROG_FWD = 1;
			}
			break;
		case BY_START_FLOW:
			if(((testBitMicro(ramPnt, progPnt->Pnt_PWM)) && (sPnt->cntClean == 0)) || (testBitMicro(ramPnt, progPnt->Pnt_Start_flw))){
				setBitMicro(ramPnt, progPnt->Pnt_State_OUT);
				save_s32(ramPnt, progPnt->Pnt_Fout, load_s32(ramPnt, progPnt->Pnt_FreqFWD));
				sPnt->timeFWD = load_s16(ramPnt, progPnt->Pnt_TimeFWD) * K_TIME_CALL_MICRO_TAKT;
				sPnt->st.bit.st_RDY = 0;
				sPnt->st.bit.st_INPROG_FWD = 1;
			}
			break;
		case BY_DI:
			if(testBitMicro(ramPnt, progPnt->Pnt_Start_DI)){
				setBitMicro(ramPnt, progPnt->Pnt_State_OUT);
				save_s32(ramPnt, progPnt->Pnt_Fout, load_s32(ramPnt, progPnt->Pnt_FreqFWD));
				sPnt->timeFWD = load_s16(ramPnt, progPnt->Pnt_TimeFWD) * K_TIME_CALL_MICRO_TAKT;
				sPnt->st.bit.st_RDY = 0;
				sPnt->st.bit.st_INPROG_FWD = 1;
			}
			break;
		default:
			break;
		}
	}else{
		if((sPnt->st.bit.st_INPROG_FWD) && (!testBitMicro(ramPnt, progPnt->Pnt_GT_RDY))){
			if(sPnt->timeFWD > 0){
				sPnt->timeFWD--;
			}else{
				save_s32(ramPnt, progPnt->Pnt_Fout, -load_s32(ramPnt, progPnt->Pnt_FreqREV));
				sPnt->timeREV = load_s16(ramPnt, progPnt->Pnt_TimeREV) * K_TIME_CALL_MICRO_TAKT;
				sPnt->timeFWD = 0;
				sPnt->st.bit.st_INPROG_FWD = 0;
				sPnt->st.bit.st_INPROG_REV = 1;
			}
		}
		if((sPnt->st.bit.st_INPROG_REV) && (!testBitMicro(ramPnt, progPnt->Pnt_GT_RDY))){
			if(sPnt->timeREV > 0){
				sPnt->timeREV--;
			}else{
				clrBitMicro(ramPnt, progPnt->Pnt_State_OUT);
				save_s32(ramPnt, progPnt->Pnt_Fout, 0);
				sPnt->timeREV = 0;
				sPnt->timeFWD = 0;
				sPnt->st.bit.st_INPROG_REV = 0;
				if (load_s16(ramPnt, progPnt->Pnt_Type) >= BY_FLOW ){
					sPnt->st.bit.st_RDY = 1;
					sPnt->cntClean++;
					save_s16(ramPnt, progPnt->Pnt_NumbCl, sPnt->cntClean);
				}else{
				    sPnt->st.bit.st_DONE = 1;
				}
			}
		}
	}
	if(!testBitMicro(ramPnt, progPnt->Pnt_PWM)){
		sPnt->st.bit.st_INPROG_FWD = 0;
		sPnt->st.bit.st_INPROG_REV = 0;
		sPnt->st.bit.st_DONE = 0;
		sPnt->st.bit.st_RDY = 1;
		sPnt->timeREV = 0;
		sPnt->timeFWD = 0;
		clrBitMicro(ramPnt, progPnt->Pnt_State_OUT);
		save_s32(ramPnt, progPnt->Pnt_Fout, 0);
		sPnt->clrCnt = 1;
	}else{
		if(sPnt->clrCnt){
			sPnt->cntClean = 0;
			save_s16(ramPnt, progPnt->Pnt_NumbCl, sPnt->cntClean);
			sPnt->clrCnt = 0;
		}
	}

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCLEAN_1[13]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  TimeFWD
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TimeREV
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  FreqFWD
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  FreqREV
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Start_flw
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Start_DI
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  PWM
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  FREQ_STOP
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  GT_RDY
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11  State_OUT
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //12  Fout
        S16_PIN_TYPE | OUTPUT_PIN_MODE    //13  NumbCl
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CLEAN", "DA", TblFuncCLEAN_1, FuncCLEAN_1, sizeof(TblFuncCLEAN_1), sizeof(CLEAN_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCLEAN_1,
#endif
//********************************************************
