#ifdef    GET_FUNC_CODE

typedef struct{
    u32		Prescaller;
    u16		Value;
    u16		Type;
    u16		PrevLoad;
}TIMER_2State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_Data;                         //1  Data
    u16  Pnt_Load;                         //2  Load
    u16  Pnt_Type;                         //3  Type
    u16  Pnt_Time;                         //4  Time
    u16  Pnt__Count;                       //5  ~Count
    u16  Pnt_Count;                        //6  Count
    u16  Pnt_OutPuls;                      //7  OutPuls
    u16  Pnt_End;
}FuncTIMER_2_type;
//********************************************************
extern const u32	TblPrescal[5];
/*
const u32	TblPrescal[5]={
    (                                     0),    // мс  
    (    100UL / INT_TIME_CALL_MICRO_mS - 1),    // 100 мс 
    (   1000UL / INT_TIME_CALL_MICRO_mS - 1),    // сек
    (  60000UL / INT_TIME_CALL_MICRO_mS - 1),    // мин
    (3600000UL / INT_TIME_CALL_MICRO_mS - 1)    // час
};
*/
//********************************************************
u16 *FuncTIMER_2_1(FuncTIMER_2_type *progPnt, u32 ramPnt)
{
	u16	            	Load, Out;
	TIMER_2State_type	*sPnt;

	sPnt = (TIMER_2State_type *)(ramPnt + progPnt->Pnt_State);
	Load = testBitMicro(ramPnt, progPnt->Pnt_Load);
    
	Out = 0;
	if(sPnt->Value == 0){
    	clrBitMicro(ramPnt, progPnt->Pnt_Count);
    	setBitMicro(ramPnt, progPnt->Pnt__Count);
    }

	if((sPnt->PrevLoad == 0) && (Load == 1)){        //Rise
    	sPnt->Type  = load_s16(ramPnt, progPnt->Pnt_Type);
    	sPnt->Value = load_s16(ramPnt, progPnt->Pnt_Data);

        if(sPnt->Value != 0){
               setBitMicro(ramPnt, progPnt->Pnt_Count);
           	clrBitMicro(ramPnt, progPnt->Pnt__Count);
        }

    	if(sPnt->Type == 0){            //мс
        	if(sPnt->Value < INT_TIME_CALL_MICRO_mS)	sPnt->Value = 1;
        	else		sPnt->Value = (sPnt->Value + (INT_TIME_CALL_MICRO_mS>>1)) / INT_TIME_CALL_MICRO_mS;
        }
    	sPnt->Prescaller = TblPrescal[sPnt->Type];
    }
	else{
    	if(sPnt->Prescaller > 0)	sPnt->Prescaller--;
    	else{
        	if(sPnt->Value > 1){
            	sPnt->Value--;
            	sPnt->Prescaller = TblPrescal[sPnt->Type];
            }
        	else if(sPnt->Value == 1){
            	clrBitMicro(ramPnt, progPnt->Pnt_Count);
            	setBitMicro(ramPnt, progPnt->Pnt__Count);
            	Out = 1;
            	sPnt->Value = 0;
            }
        }
    }
	sPnt->PrevLoad = Load;
    
	defBitMicro(ramPnt, progPnt->Pnt_OutPuls, Out);
	if(sPnt->Type != 0)	save_s16(ramPnt, progPnt->Pnt_Time, sPnt->Value);
	else	        	save_s16(ramPnt, progPnt->Pnt_Time, sPnt->Value * INT_TIME_CALL_MICRO_mS);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTIMER_2_1[7]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Data
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Load
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Type
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Time
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  ~Count
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Count
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //7  OutPuls
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew("TIMER_2", "DD", TblFuncTIMER_2_1, FuncTIMER_2_1, sizeof(TblFuncTIMER_2_1), sizeof(TIMER_2State_type), 0, 1, sizeof(FuncTIMER_2_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTIMER_2_1,
#endif
//********************************************************
