#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_UdTormMax;                    //1  UdTormMax
    u16  Pnt_UdTormMin;                    //2  UdTormMin
    u16  Pnt_UdTek;                        //3  UdTek
    u16  Pnt_Action;                       //4  Action
    u16  Pnt_StopTorm;                     //5  StopTorm
    u16  Pnt_ChangeTime;                   //6  ChangeTime
    u16  Pnt_FreezeGT;                     //7  FreezeGT
    u16  Pnt_End;
}FuncUD_ACT_VAR_type;

typedef enum {
	disabled=0,
	lockRate,
	changeRate,
}ADPTBRKDWNtype_enum;

//********************************************************
u16 *FuncUD_ACT_VAR_1(FuncUD_ACT_VAR_type *progPnt, u32 ramPnt)
{
	u16		action, control;
	s32		UdTek, UdTormMin, UdTormMax;
return &progPnt->Pnt_End;	
	UdTek	  = load_s32(ramPnt, progPnt->Pnt_UdTek);
	UdTormMin = load_s32(ramPnt, progPnt->Pnt_UdTormMin);
	UdTormMax = load_s32(ramPnt, progPnt->Pnt_UdTormMax);
    action	  = load_s16(ramPnt, progPnt->Pnt_Action)&0x3;

	if(UdTek < UdTormMin)      control = 0;
    else if(UdTek > UdTormMax) control = 1;

	clrBitMicro(ramPnt, progPnt->Pnt_StopTorm);	 // Пока этим выходом не управляем
	defBitMicro(ramPnt, progPnt->Pnt_ChangeTime, ((control<<changeRate)&(1<<action)));
	defBitMicro(ramPnt, progPnt->Pnt_FreezeGT,   ((control<<lockRate  )&(1<<action)));

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUD_ACT_VAR_1[7]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  UdTormMax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  UdTormMin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  UdTek
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Action
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  StopTorm
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //6  ChangeTime
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //7  FreezeGT
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UD_ACT_VAR", "DA", TblFuncUD_ACT_VAR_1, FuncUD_ACT_VAR_1, sizeof(TblFuncUD_ACT_VAR_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUD_ACT_VAR_1,
#endif
//********************************************************
