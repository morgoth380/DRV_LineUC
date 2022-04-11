#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Max;                          //1  Max
    u16  Pnt_Min;                          //2  Min
    u16  Pnt_Stepen;                       //3  Stepen
    u16  Pnt_EdIsm;                        //4  EdIsm
    u16  Pnt_Ain;                          //5  Ain
    u16  Pnt_Tfiltr;                       //6  Tfiltr
    u16  Pnt_FailMax;                      //7  FailMax
    u16  Pnt_FailMin;                      //8  FailMin
    u16  Pnt_HistMin;                      //9  HistMin
    u16  Pnt_HardFail;                     //10  HardFail
    u16  Pnt_AlrmFail;                     //11  AlrmFail
    u16  Pnt_Out;                          //12  Out
    u16  Pnt_Format;                       //13  Format
    u16  Pnt_Out__;                        //14  Out_%
    u16  Pnt_Fail;                         //15  Fail
    u16  Pnt_CodeFail;                     //16  CodeFail
    u16  Pnt_Alrm;                         //17  Alrm
    u16  Pnt_CodeAlrm;                     //18  CodeAlrm
    u16  Pnt_End;
}FuncAIN_SCALE2_V2_type;

//********************************************************
u16 *FuncAIN_SCALE2_V2_1(FuncAIN_SCALE2_V2_type *progPnt, u32 ramPnt)
{


    return &progPnt->Pnt_End;
}
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAIN_SCALE2_V2_1[18]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Stepen
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  EdIsm
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Ain
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Tfiltr
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  FailMax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  FailMin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  HistMin
	BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  HardFail
	S16_PIN_TYPE | INPUT_PIN_MODE,     //11 AlrmFail
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Out
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  Format
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //14  Out_%
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //15  Fail
	S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  CodeFail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //17  Alrm
	S16_PIN_TYPE | OUTPUT_PIN_MODE     //18  CodeAlrm
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AIN_SCALE2_V2", "DA", TblFuncAIN_SCALE2_V2_1, FuncAIN_SCALE2_V2_1, sizeof(TblFuncAIN_SCALE2_V2_1), sizeof(AINSCALE2_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAIN_SCALE2_V2_1,
#endif
//********************************************************
