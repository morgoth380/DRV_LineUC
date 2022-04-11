#ifdef    GET_FUNC_CODE

#define _46PERCENTPMAX 7829369
#define _5PERCENTPMAX  838810

typedef struct{
	_iq		CurrentFreq;
	u32 	counter;

}FuncFREQ_PWR_SIMULATOR_State_type;


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Speed;                        //1  Speed
    u16  Pnt_Curve_type;                   //2  Curve_type
    u16  Pnt_Stop_Freq;                    //3  Stop_Freq
    u16  Pnt_Pwr_Coeff;                    //4  Pwr_Coeff
	u16  Pnt_F_MAX;                        //5  F_max	
    u16  Pnt_EN;                           //6  on/off
    u16  Pnt_F_TEK_D;                      //7  F_TEK_D
    u16  Pnt_P_TEK_D;                      //8  P_TEK_D

    u16  Pnt_End;
}FuncFREQ_PWR_SIMULATOR_type;
//********************************************************
u16 *FuncFREQ_PWR_SIMULATOR_1(FuncFREQ_PWR_SIMULATOR_type *progPnt, u32 ramPnt)
{	//_________________________________________________________________________
    FuncFREQ_PWR_SIMULATOR_State_type	 *sPnt;
	s16 coeff;
	u32 stop_f,f_max, t, p_tek;
	//_________________________________________________________________________
	sPnt = (FuncFREQ_PWR_SIMULATOR_State_type *)(ramPnt + progPnt->Pnt_State);
	coeff = load_s16(ramPnt, progPnt->Pnt_Pwr_Coeff);
	//_________________________________________________________________________
	if(flagIsFirstCall){
	sPnt->CurrentFreq = 0;
	sPnt->counter = 1;
	}
	if(load_s16(ramPnt, progPnt->Pnt_EN) > 0){
		f_max = load_s32(ramPnt, progPnt->Pnt_F_MAX);
		stop_f = (s32) (load_s16(ramPnt, progPnt->Pnt_Stop_Freq) * f_max)/100;
		t = (s32) load_s16(ramPnt, progPnt->Pnt_Speed) * K_TIME_CALL_MICRO_TAKT;
		if(t == 0){
			sPnt->CurrentFreq = (_iq) stop_f;
		}
		else{
			u64 temp;
			temp = ((u64)stop_f) * sPnt->counter;
			temp = temp / t;
			sPnt->CurrentFreq = (_iq) temp;
		}
		if(load_s16(ramPnt, progPnt->Pnt_Curve_type) > 0){
			save_s32(ramPnt, progPnt->Pnt_F_TEK_D, sPnt->CurrentFreq);
			p_tek =  _5PERCENTPMAX;
			save_s32(ramPnt, progPnt->Pnt_P_TEK_D, p_tek);
		}
		else{
			u64 temp;
			save_s32(ramPnt, progPnt->Pnt_F_TEK_D, sPnt->CurrentFreq);
			temp = (u64) (_46PERCENTPMAX * coeff) / 100; 
			temp = (u64) (temp * sPnt->CurrentFreq)/f_max;
			p_tek =  _46PERCENTPMAX - (u32) temp + _5PERCENTPMAX;
			save_s32(ramPnt, progPnt->Pnt_P_TEK_D, p_tek);
		} 
		if(sPnt->counter < t){
			sPnt->counter++;
		}
	}
	else{
		sPnt->counter = 1;
		sPnt->CurrentFreq = 0;
		save_s32(ramPnt, progPnt->Pnt_P_TEK_D, 0);
		save_s32(ramPnt, progPnt->Pnt_F_TEK_D, 0);
	}
	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFREQ_PWR_SIMULATOR_1[8]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     	//1  Speed
        S16_PIN_TYPE | INPUT_PIN_MODE,     	//2  Curve_type
        S16_PIN_TYPE | INPUT_PIN_MODE,     	//3  Stop_Freq
        S16_PIN_TYPE | INPUT_PIN_MODE,     	//4  Pwr_Coeff
        S32_PIN_TYPE | INPUT_PIN_MODE,    	//5  F_max
        S16_PIN_TYPE | INPUT_PIN_MODE,    	//6  on/off
		S32_PIN_TYPE | OUTPUT_PIN_MODE,    //7  F_TEK_D
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //8  P_TEK_D
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FREQ_PWR_SIMULATOR", "DA", TblFuncFREQ_PWR_SIMULATOR_1, FuncFREQ_PWR_SIMULATOR_1, sizeof(TblFuncFREQ_PWR_SIMULATOR_1), sizeof(FuncFREQ_PWR_SIMULATOR_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFREQ_PWR_SIMULATOR_1,
#endif
//********************************************************
