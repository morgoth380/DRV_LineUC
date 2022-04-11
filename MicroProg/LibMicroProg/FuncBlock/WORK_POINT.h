#ifdef    GET_FUNC_CODE

#define MODE_OFF  0
#define MODE_FAIL 1
#define MODE_WARN 2

typedef struct{
    u32  cnt;
}FuncWORK_POINT_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_On_Off;                       //1  On_Off
    u16  Pnt_FB_PID;                       //2  FB_PID
    u16  Pnt_FB_PID_UST;                   //3  FB_PID_UST
    u16  Pnt_F;                            //4  F
    u16  Pnt_F_max;                        //5  F_max
    u16  Pnt_Time;                         //6  Time
    u16  Pnt_Warn;                         //7  Warn
    u16  Pnt_Fail;                         //8  Fail
    u16  Pnt_End;
}FuncWORK_POINT_type;
//********************************************************
u16 *FuncWORK_POINT_1(FuncWORK_POINT_type *progPnt, u32 ramPnt)
{
	u16 mode;
	FuncWORK_POINT_State_type *sPnt;
	sPnt = (FuncWORK_POINT_State_type *)(ramPnt + progPnt->Pnt_State);

	mode = load_s16(ramPnt, progPnt->Pnt_On_Off);
	if(mode != MODE_OFF){
		if((load_s32(ramPnt, progPnt->Pnt_FB_PID_UST) > load_s32(ramPnt, progPnt->Pnt_FB_PID)) &&
				(load_s32(ramPnt, progPnt->Pnt_F) == load_s32(ramPnt, progPnt->Pnt_F_max))){
			if(sPnt->cnt == 1){
				if(mode == MODE_FAIL){
					setBitMicro(ramPnt, progPnt->Pnt_Fail);
				}
				if(mode == MODE_WARN){
					setBitMicro(ramPnt, progPnt->Pnt_Warn);
				}
			}else{
				if(sPnt->cnt == 0){
					sPnt->cnt = load_s16(ramPnt, progPnt->Pnt_Time) * K_TIME_CALL_MICRO_TAKT;
				}else{
					sPnt->cnt--;
				}
			}
		}else{
			sPnt->cnt = 0;
			clrBitMicro(ramPnt, progPnt->Pnt_Fail);
			clrBitMicro(ramPnt, progPnt->Pnt_Warn);
		}
	}else{
		sPnt->cnt = 0;
		clrBitMicro(ramPnt, progPnt->Pnt_Fail);
		clrBitMicro(ramPnt, progPnt->Pnt_Warn);
	}
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncWORK_POINT_1[8]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  On_Off
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  FB_PID
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  FB_PID_UST
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F_max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Time
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Warn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //8  Fail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("WORK_POINT", "DA", TblFuncWORK_POINT_1, FuncWORK_POINT_1, sizeof(TblFuncWORK_POINT_1), sizeof(FuncWORK_POINT_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncWORK_POINT_1,
#endif
//********************************************************
