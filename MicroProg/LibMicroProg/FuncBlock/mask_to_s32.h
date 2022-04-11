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
    u16  Pnt_In16;                         //6  In16
    u16  Pnt_In17;                         //7  In17
    u16  Pnt_In18;                         //8  In18
    u16  Pnt_In19;                         //9  In19
    u16  Pnt_In20;                         //10  In20
    u16  Pnt_Out0_20;                      //11  Out0_20
    u16  Pnt_Out0_7;                       //12  Out0_7
    u16  Pnt_End;
}FuncMASK_TO_S32_type;
//********************************************************
u16 *FuncMASK_TO_S32_1(FuncMASK_TO_S32_type *progPnt, u32 ramPnt)
{
	u16	in0_11, in12_20;

	in12_20 = 0;

	in0_11 = load_s16(ramPnt, progPnt->Pnt_In0_11) & 0x0FFF;
	in12_20 = (	(testBitMicro(ramPnt, progPnt->Pnt_In12)<<12)|
				(testBitMicro(ramPnt, progPnt->Pnt_In13)<<13)|
				(testBitMicro(ramPnt, progPnt->Pnt_In14)<<14)|
				(testBitMicro(ramPnt, progPnt->Pnt_In15)<<15)|
				(testBitMicro(ramPnt, progPnt->Pnt_In16)<<16)|
				(testBitMicro(ramPnt, progPnt->Pnt_In17)<<17)|
				(testBitMicro(ramPnt, progPnt->Pnt_In18)<<18)|
				(testBitMicro(ramPnt, progPnt->Pnt_In19)<<19)|
				(testBitMicro(ramPnt, progPnt->Pnt_In20)<<20)
			  );

	save_s32(ramPnt, progPnt->Pnt_Out0_20, in0_11 | in12_20);
	save_s16(ramPnt, progPnt->Pnt_Out0_7, in0_11 & 0x00FF);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMASK_TO_S32_1[12]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In0_11
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  In12
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  In13
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  In14
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  In15
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  In16
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  In17
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  In18
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  In19
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  In20
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Out0_20
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //12  Out0_7
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MASK_TO_S32", "DD", TblFuncMASK_TO_S32_1, FuncMASK_TO_S32_1, sizeof(TblFuncMASK_TO_S32_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMASK_TO_S32_1,
#endif
//********************************************************
