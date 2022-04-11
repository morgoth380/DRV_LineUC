#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
	u16  Pnt_A[2];                         //1  A0,A1
	u16  Pnt_B[2];                         //3  B0,B1
    u16  Pnt_Num;                          //5  Num
    u16  Pnt_OutB;                         //6  OutB
    u16  Pnt_OutA;                         //7  OutA
    u16  Pnt_End;
}FuncMUX2X2_type;
//********************************************************
u16 *FuncMUX2X2_1(FuncMUX2X2_type *progPnt, u32 ramPnt)
{
	s16         Num;

	Num = load_s16(ramPnt, progPnt->Pnt_Num)&0x01;
	save_s32(ramPnt, progPnt->Pnt_OutA, load_s32(ramPnt, progPnt->Pnt_A[Num]));
	save_s32(ramPnt, progPnt->Pnt_OutB, load_s32(ramPnt, progPnt->Pnt_B[Num]));

	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX2X2_1[7]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  A0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  A1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  B0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  B1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Num
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  OutB
        S32_PIN_TYPE | INPUT_PIN_MODE      //7  OutA
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MUX2X2", "DA", TblFuncMUX2X2_1, FuncMUX2X2_1, sizeof(TblFuncMUX2X2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX2X2_1,
#endif
//********************************************************
