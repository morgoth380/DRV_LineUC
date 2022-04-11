#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Fin;                          //1  Fin
    u16  Pnt_Start;                        //2  Start
    u16  Pnt_TimeScanF;                    //3  TimeScanF
    u16  Pnt_ReversScan;                   //4  ReversScan
    u16  Pnt_Uscan;                        //5  Uscan
    u16  Pnt_Tnasisch;                     //6  Tnasisch
    u16  Pnt_K_di_dT;                      //7  K_di/dT
    u16  Pnt_FstartScan;                   //8  FstartScan
    u16  Pnt_Enable;                       //9  Enable
    u16  Pnt_Fail;                         //10  Fail
    u16  Pnt_ZerTime_GT;                   //11  ZerTime_GT
    u16  Pnt_Fout;                         //12  Fout
    u16  Pnt_mull_Uout;                    //13  mull_Uout
    u16  Pnt_End;
}FuncPICKUP_2_type;
//********************************************************
u16 *FuncPICKUP_2_1(FuncPICKUP_2_type *progPnt, u32 ramPnt)
{
	_iq		iqTmp;
#ifdef _PROJECT_FOR_CCS_
#warning !Функция PICKUP_2 пустая!
#endif
	iqTmp = load_s32(ramPnt, progPnt->Pnt_Fin);
	save_s32(ramPnt, progPnt->Pnt_Fout     ,  iqTmp  );
	save_s32(ramPnt, progPnt->Pnt_mull_Uout, _IQ(1.0));

	return &progPnt->Pnt_End;

};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPICKUP_2_1[13]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Fin
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Start
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TimeScanF
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  ReversScan
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Uscan
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Tnasisch
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  K_di/dT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  FstartScan
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Enable
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //10  Fail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //11  ZerTime_GT
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //12  Fout
        S32_PIN_TYPE | OUTPUT_PIN_MODE      //13  mull_Uout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PICKUP_2", "DA", TblFuncPICKUP_2_1, FuncPICKUP_2_1, sizeof(TblFuncPICKUP_2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPICKUP_2_1,
#endif
//********************************************************
