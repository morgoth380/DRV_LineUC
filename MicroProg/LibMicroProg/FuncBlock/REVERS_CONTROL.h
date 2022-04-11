#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_In;                           //1  In
    u16  Pnt_DinRevers;                    //2  DinRevers
    u16  Pnt_UstRevOff;                    //3  UstRevOff
    u16  Pnt_RevOff;                       //4  RevOff
    u16  Pnt_Sign;                         //5  Sign
    u16  Pnt_Out;                          //6  Out
    u16  Pnt_End;
}FuncREVERS_CONTROL_type;
//********************************************************
u16 *FuncREVERS_CONTROL_1(FuncREVERS_CONTROL_type *progPnt, u32 ramPnt)
{
	_iq		iqTmp;

	iqTmp = load_s32(ramPnt, progPnt->Pnt_In);
	if( testBitMicro(ramPnt, progPnt->Pnt_DinRevers) )	iqTmp = -iqTmp;
	if( (iqTmp < 0)&& load_s16(ramPnt, progPnt->Pnt_UstRevOff) ){
		iqTmp = 0;
		setBitMicro(ramPnt, progPnt->Pnt_RevOff);
	}
	else{
		clrBitMicro(ramPnt, progPnt->Pnt_RevOff);
	}
	save_s32(ramPnt, progPnt->Pnt_Out, iqTmp);
	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncREVERS_CONTROL_1[6]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  DinRevers
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  UstRevOff
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  RevOff
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Sign
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //6  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("REVERS_CONTROL", "DA", TblFuncREVERS_CONTROL_1, FuncREVERS_CONTROL_1, sizeof(TblFuncREVERS_CONTROL_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncREVERS_CONTROL_1,
#endif
//********************************************************
