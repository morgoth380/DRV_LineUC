#ifdef    GET_FUNC_CODE

u16 *FuncTEXN_CONTROL_4_1(FuncTEXN_CONTROL_4_type *progPnt, u32 ramPnt)
{
	FuncFAIL_CONTROL((FuncFAIL_CONTROL_64_type *)progPnt, ramPnt);
	return	&progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEXN_CONTROL_4_1[31]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Man/Auto
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  KolvFail
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //3  MaskTek_3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  MaskTek_2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  MaskTek_1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  MaskTek_0
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //7  N_TekFail
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //8  APV_budet
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Type_torm
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10  TimeDoOff
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //11  NearStop
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  N_FAIL
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  TimeDoAPV
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14  NearPusk
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //15  N_PUSK
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //16  Tek_Fail
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Request
         S32_PIN_TYPE | INPUT_PIN_MODE,     //18  Time
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  TypeTorm
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //20  Tek_Fail
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Request
         S32_PIN_TYPE | INPUT_PIN_MODE,     //22  Time
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  TypeTorm
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //24  Tek_Fail
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Request
         S32_PIN_TYPE | INPUT_PIN_MODE,     //26  Time
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  TypeTorm
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //28  Tek_Fail
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  Request
         S32_PIN_TYPE | INPUT_PIN_MODE,     //30  Time
        S16_PIN_TYPE | INPUT_PIN_MODE      //31  TypeTorm
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("TEXN_CONTROL_4", "DD", TblFuncTEXN_CONTROL_4_1, FuncTEXN_CONTROL_4_1, sizeof(TblFuncTEXN_CONTROL_4_1), sizeof(FailControlState_type), sizeof(nvFailControlState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEXN_CONTROL_4_1,
#endif
//********************************************************
