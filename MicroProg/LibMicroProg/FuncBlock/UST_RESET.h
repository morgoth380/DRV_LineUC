#ifdef    GET_FUNC_CODE

typedef struct{
	u16	Cnt;
}UST_RESETState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In;                           //1  In
    u16  Pnt_ResVal;                       //2  ResVal
    u16  Pnt_Delay;                        //3  Delay
    u16  Pnt_Out;                          //4  Out
    u16  Pnt_End;
}FuncUST_RESET_type;
//********************************************************
u16 *FuncUST_RESET_1(FuncUST_RESET_type *progPnt, u32 ramPnt)
{
	UST_RESETState_type    *sPnt;
	s16	s16temp,s16temp1;
    sPnt = (UST_RESETState_type *)(ramPnt + progPnt->Pnt_State);

	s16temp = load_s16(ramPnt, progPnt->Pnt_ResVal);

	if (flagIsFirstCall)
    {
		save_s16(ramPnt, progPnt->Pnt_In, s16temp);
		save_s16(ramPnt, progPnt->Pnt_Out, s16temp);
		sPnt->Cnt = load_s16(ramPnt, progPnt->Pnt_Delay);
    }
	else
	{
		s16temp1 = load_s16(ramPnt, progPnt->Pnt_In);

		if (s16temp1 != s16temp)
		{
			if (sPnt->Cnt == 0)
			{
				save_s16(ramPnt, progPnt->Pnt_In, s16temp);
				save_s16(ramPnt, progPnt->Pnt_Out, s16temp);
				sPnt->Cnt = load_s16(ramPnt, progPnt->Pnt_Delay);
			}
			else
			{
				sPnt->Cnt--;
				save_s16(ramPnt, progPnt->Pnt_Out, s16temp1);
			}
		}
	}
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_RESET_1[4]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  ResVal
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Delay
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //4  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_RESET", "DA", TblFuncUST_RESET_1, FuncUST_RESET_1, sizeof(TblFuncUST_RESET_1), sizeof(UST_RESETState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_RESET_1,
#endif
//********************************************************
//!!!!!!!!!!!!!!!!!ÏÅÐÅÃÐÓÇÊÀ S32!!!!!!!!!!!!!!!!!!!!!!!!!
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncUST_RESET_2(FuncUST_RESET_type *progPnt, u32 ramPnt)
{
	UST_RESETState_type    *sPnt;
	s32	s32temp,s32temp1;
    sPnt = (UST_RESETState_type *)(ramPnt + progPnt->Pnt_State);

	s32temp = load_s32(ramPnt, progPnt->Pnt_ResVal);

	if (flagIsFirstCall)
    {
		save_s32(ramPnt, progPnt->Pnt_In, s32temp);
		save_s32(ramPnt, progPnt->Pnt_Out, s32temp);
		sPnt->Cnt = load_s16(ramPnt, progPnt->Pnt_Delay);
    }
	else
	{
		s32temp1 = load_s32(ramPnt, progPnt->Pnt_In);

		if (s32temp1 != s32temp)
		{
			if (sPnt->Cnt == 0)
			{
				save_s32(ramPnt, progPnt->Pnt_In, s32temp);
				save_s32(ramPnt, progPnt->Pnt_Out, s32temp);
				sPnt->Cnt = load_s16(ramPnt, progPnt->Pnt_Delay);
			}
			else
			{
				sPnt->Cnt--;
				save_s32(ramPnt, progPnt->Pnt_Out, s32temp1);
			}
		}
	}
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_RESET_2[4]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  ResVal
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Delay
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //4  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_RESET", "DA", TblFuncUST_RESET_2, FuncUST_RESET_2, sizeof(TblFuncUST_RESET_2), sizeof(UST_RESETState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_RESET_2,
#endif
//********************************************************

