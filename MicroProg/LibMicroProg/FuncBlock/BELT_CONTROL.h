#ifdef    GET_FUNC_CODE

#define _OFF 			0
#define _ON_WARNING 	1
#define _ON_CRASH 		2

#define OK 				0
#define WARNING 		1
#define CRASH 			2

typedef struct{

	u32 	counter;
	
}FuncBELT_CONTROL_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_I_ACT;                        //1  I_ACT
    u16  Pnt_I_WB;                         //2  I_WB
    u16  Pnt_Delay_;                       //3  Delay_
    u16  Pnt_Mode;                         //4  Mode
	u16	 Pnt_PWM_on;					   //5 PWM_on
    u16  Pnt_Failure;                      //6  Failure
	u16  Pnt_Warning;                      //7  Warning
    u16  Pnt_End;
}FuncBELT_CONTROL_type;
//********************************************************
u16 *FuncBELT_CONTROL_1(FuncBELT_CONTROL_type *progPnt, u32 ramPnt)
{
    FuncBELT_CONTROL_State_type *sPnt;

    s32 i_tek, i_wb;
	u16 mode; 
	
    sPnt = (FuncBELT_CONTROL_State_type *)(ramPnt + progPnt->Pnt_State);
	
	mode = load_s16(ramPnt, progPnt->Pnt_Mode);
	i_tek = load_s32(ramPnt, progPnt->Pnt_I_ACT);
	i_wb = load_s32(ramPnt, progPnt->Pnt_I_WB);
	
	if (testBitMicro(ramPnt, progPnt->Pnt_PWM_on)){							//Если шим выкл, то ничего не делаем
		
		if (flagIsFirstCall){	//Первый вызов	
			sPnt->counter	 = 	(u32) (load_s16(ramPnt, progPnt->Pnt_Delay_) * K_TIME_CALL_MICRO_TAKT);	//заряжаем таймер ООП
		}
		if (i_tek <= i_wb){
			if (sPnt->counter == 0){
				switch (mode){
				case _OFF: 			
					sPnt->counter	 = 	(u32) (load_s16(ramPnt, progPnt->Pnt_Delay_) * K_TIME_CALL_MICRO_TAKT);
					clrBitMicro(ramPnt, progPnt->Pnt_Failure);
					clrBitMicro(ramPnt, progPnt->Pnt_Warning);
					break;
				case _ON_WARNING:	
					clrBitMicro(ramPnt, progPnt->Pnt_Failure);
					setBitMicro(ramPnt, progPnt->Pnt_Warning);
					break;
				case _ON_CRASH:
					clrBitMicro(ramPnt, progPnt->Pnt_Warning);
					setBitMicro(ramPnt, progPnt->Pnt_Failure);
					break;
				}
			}
			else {
				clrBitMicro(ramPnt, progPnt->Pnt_Failure);
				clrBitMicro(ramPnt, progPnt->Pnt_Warning);
				sPnt->counter --;
			}
			
		}
		else{ 
			sPnt->counter	 = 	(u32) (load_s16(ramPnt, progPnt->Pnt_Delay_) * K_TIME_CALL_MICRO_TAKT);
			clrBitMicro(ramPnt, progPnt->Pnt_Failure);
			clrBitMicro(ramPnt, progPnt->Pnt_Warning);
		}	
	}
	else {
		sPnt->counter	 = 	(u32) (load_s16(ramPnt, progPnt->Pnt_Delay_) * K_TIME_CALL_MICRO_TAKT);
		clrBitMicro(ramPnt, progPnt->Pnt_Failure);
		clrBitMicro(ramPnt, progPnt->Pnt_Warning);
	}
	
	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBELT_CONTROL_1[7]={
        S32_PIN_TYPE | INPUT_PIN_MODE,    	//1  I_ACT
        S32_PIN_TYPE | INPUT_PIN_MODE,    	//2  I_WB
        S16_PIN_TYPE | INPUT_PIN_MODE,    	//3  Delay_
        S16_PIN_TYPE | INPUT_PIN_MODE,    	//4  Mode
		BIT_PIN_TYPE | INPUT_PIN_MODE,	  	//5  PWM_on
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    	//6  Failure
		BIT_PIN_TYPE | OUTPUT_PIN_MODE     	//7  Warning
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("BELT_CONTROL", "DA", TblFuncBELT_CONTROL_1, FuncBELT_CONTROL_1, sizeof(TblFuncBELT_CONTROL_1), sizeof(FuncBELT_CONTROL_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBELT_CONTROL_1,
#endif
//********************************************************
