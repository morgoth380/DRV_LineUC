#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In0_11;                       //1  In0_11
    u16  Pnt_In12;                         //2  In12
    u16  Pnt_In13;                         //3  In13
    u16  Pnt_In14;                         //4  In14
    u16  Pnt_In15;                         //5  In15
    u16  Pnt_Out0_15;                      //6  Out0_15
    u16  Pnt_Out0_7;                       //7  Out0_7
    u16  Pnt_End;
}FuncMASK_TO_S16_type;
//********************************************************
u16 *FuncMASK_TO_S16_1(FuncMASK_TO_S16_type *progPnt, u32 ramPnt)
{
	u16	in0_11, in12_15;

	in12_15 = 0;

	in0_11 = load_s16(ramPnt, progPnt->Pnt_In0_11) & 0x0FFF;
	in12_15 = (	(testBitMicro(ramPnt, progPnt->Pnt_In12)<<12)|
				(testBitMicro(ramPnt, progPnt->Pnt_In13)<<13)|
				(testBitMicro(ramPnt, progPnt->Pnt_In14)<<14)|
				(testBitMicro(ramPnt, progPnt->Pnt_In15)<<15)
			  );

	save_s16(ramPnt, progPnt->Pnt_Out0_15, in0_11 | in12_15);
	save_s16(ramPnt, progPnt->Pnt_Out0_7, in0_11 & 0x00FF);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMASK_TO_S16_1[7]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In0_11
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  In12
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  In13
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  In14
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  In15
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Out0_15
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //7  Out0_7
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MASK_TO_S16", "DD", TblFuncMASK_TO_S16_1, FuncMASK_TO_S16_1, sizeof(TblFuncMASK_TO_S16_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMASK_TO_S16_1,
#endif
//********************************************************
