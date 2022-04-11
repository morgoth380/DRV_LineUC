#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Enable;                       //1  Enable
    u16  Pnt_BRAKE;                        //2  BRAKE
    u16  Pnt_OS_BRAKE;                     //3  OS_BRAKE
    u16  Pnt_InPUT;                        //4  InPUT
    u16  Pnt_En_OS_BRAKE;                  //5  En_OS_BRAKE
    u16  Pnt_INZIP1;                       //6  INZIP1
    u16  Pnt_INZIP2;                       //7  INZIP2
    u16  Pnt_INZIP3;                       //8  INZIP3
    u16  Pnt_INZIP4;                       //9  INZIP4
    u16  Pnt_OUTZIP4;                      //10  OUTZIP4
    u16  Pnt_OUTZIP3;                      //11  OUTZIP3
    u16  Pnt_OUTZIP2;                      //12  OUTZIP2
    u16  Pnt_OUTZIP1;                      //13  OUTZIP1
    u16  Pnt_SAVE_INT;                     //14  SAVE_INT
    u16  Pnt_End;
}FuncSAVE_INTEGR_type;
//********************************************************
u16 *FuncSAVE_INTEGR_1(FuncSAVE_INTEGR_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSAVE_INTEGR_1[14]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Enable
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  BRAKE
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  OS_BRAKE
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  InPUT
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  En_OS_BRAKE
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  INZIP1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  INZIP2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  INZIP3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  INZIP4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10  OUTZIP4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //11  OUTZIP3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  OUTZIP2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  OUTZIP1
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //14  SAVE_INT
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SAVE_INTEGR", "DD", TblFuncSAVE_INTEGR_1, FuncSAVE_INTEGR_1, sizeof(TblFuncSAVE_INTEGR_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSAVE_INTEGR_1,
#endif
//********************************************************
