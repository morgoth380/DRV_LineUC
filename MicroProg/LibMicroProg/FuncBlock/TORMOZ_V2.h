#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    u16  Pnt_TormEn;                    //1  TormEn
    u16  Pnt_U_0;                       //2  U_0%
    u16  Pnt_U_100;                     //3  U_100%
    u16  Pnt_tormCheck;                 //3  tormCheck
    u16  Pnt_Ibr;                       //5  Ibr
    u16  Pnt_Rbr;                       //6  Rbr
    u16  Pnt_Tact;                      //7  Tact
    u16  Pnt_E;                         //8  E
    u16  Pnt_Change;                    //9  Change
    u16  Pnt_TimeToOff;                 //10  TimeToOff
    u16  Pnt_Fail;                      //11 Fail
    u16  Pnt_NoCheckFail;               //12 NoCheckFail
    u16  Pnt_End;                
}FuncTORMOZ_V2_type;

//********************************************************
u16 *FuncTORMOZ_V2_1(FuncTORMOZ_V2_type *progPnt, u32 ramPnt)
{
    float temp;
    
    // 1) Передать в обменную структуру вкл/выкл тормозного ключа, and Check break resistor state
    GlobalM4.FlagCommand.bit.TormSwEnable = load_s16(ramPnt, progPnt->Pnt_TormEn);
    //GlobalM4.TormSwParam.TormCheckEn = load_s16(ramPnt, progPnt->Pnt_TormResCheck); // UNBLOCK AFTER REALIZATION
    GlobalM4.TormSwParam.TormCheckEn = load_s16(ramPnt, progPnt->Pnt_tormCheck);
    
    // 2) Передадим в обменную структуру напряжения открывания и закрывания тормозного ключа.
    temp = load_float(ramPnt, progPnt->Pnt_U_0);
    GlobalM4.TormSwParam.UOff = IQ_to_FLOAT(temp, Volt_base); 
    
    temp = load_float(ramPnt, progPnt->Pnt_U_100);
    GlobalM4.TormSwParam.UOn = IQ_to_FLOAT(temp, Volt_base); 
    
    if (GlobalM4.TormSwParam.TormCheckEn && !GlobalM4.FlagCommand.bit.TormSwEnable) {
        setBitMicro(ramPnt, progPnt->Pnt_NoCheckFail); // Can't check break resistor, because TormSwitch not active
	} else {
		clrBitMicro(ramPnt, progPnt->Pnt_NoCheckFail); // Can't check break resistor, because TormSwitch not active
	}
    
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTORMOZ_V2_1[12]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  TormEn
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  U_0%
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  U_100%
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  tormCheck
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Ibr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Rbr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Tact
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  E
	S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Change
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10  TimeToOff
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11 Fail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //12 NoCheckFail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TORMOZ_V2", "DA", TblFuncTORMOZ_V2_1, FuncTORMOZ_V2_1, sizeof(TblFuncTORMOZ_V2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTORMOZ_V2_1,
#endif
//********************************************************
