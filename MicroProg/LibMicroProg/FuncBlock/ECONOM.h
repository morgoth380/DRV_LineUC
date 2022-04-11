#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Uin;                          //1  Uin
    u16  Pnt_Fref;                         //2  Fref
    u16  Pnt_TypeDrv;                      //3  TypeDrv
    u16  Pnt_CosFi;                        //4  CosFi
    u16  Pnt_Enable;                       //5  Enable
    u16  Pnt_CosRef;                       //6  CosRef
    u16  Pnt_Kp;                           //7  Kp
    u16  Pnt_Ti;                           //8  Ti
    u16  Pnt_Td;                           //9  Td
    u16  Pnt_TimeScal;                     //10  TimeScal
    u16  Pnt_maxPid;                       //11  maxPid
    u16  Pnt_minPid;                       //12  minPid
    u16  Pnt_Factiv;                       //13  Factiv
    u16  Pnt_Uout;                         //14  Uout
    u16  Pnt_End;
}FuncECONOM_type;
//********************************************************
u16 *FuncECONOM_1(FuncECONOM_type *progPnt, u32 ramPnt)
{
	_iq		iqTmp;
	
	iqTmp = load_s32(ramPnt, progPnt->Pnt_Uin);
	save_s32(ramPnt, progPnt->Pnt_Uout, iqTmp);
	
	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncECONOM_1[14]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Uin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Fref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TypeDrv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  CosFi
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Enable
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  CosRef
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Ti
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Td
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  TimeScal
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  maxPid
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  minPid
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  Factiv
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //14  Uout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("ECONOM", "DD", TblFuncECONOM_1, FuncECONOM_1, sizeof(TblFuncECONOM_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncECONOM_1,
#endif
//********************************************************
