#ifdef    GET_FUNC_CODE

typedef struct{
    s16     StartOK;
}CURR_CONTR_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_I_set;                        //1  I_set
    u16  Pnt_I_lim_strt;                   //2  I_lim_strt
    u16  Pnt_I_nom;                        //3  I_nom
    u16  Pnt_Ilim_EN;                      //4  Ilim_EN
    u16  Pnt_Mode_I_F;                     //5  Mode_I_F
    u16  Pnt_I_full;                       //6  I_full
    u16  Pnt_F_MAX;                        //7  F_MAX
    u16  Pnt_F_MIN;                        //8  F_MIN
    u16  Pnt_F_in;                         //9  F_in
    u16  Pnt_F_strt;                       //10  F_strt
    u16  Pnt_SignF;                        //11  SignF
    u16  Pnt_GT_UP;                        //12  GT_UP
    u16  Pnt_PWM_ON;                       //13  PWM_ON
    u16  Pnt_STOP_FR;                      //14  STOP_FR
    u16  Pnt_F_ref;                        //15  F_ref
    u16  Pnt_End;
}FuncCURR_CONTR_type;
//********************************************************
u16 *FuncCURR_CONTR_1(FuncCURR_CONTR_type *progPnt, u32 ramPnt)
{
	
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCURR_CONTR_1[15]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  I_set
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  I_lim_strt
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  I_nom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Ilim_EN
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Mode_I_F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  I_full
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  F_MAX
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  F_MIN
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  F_in
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  F_strt
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  SignF
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  GT_UP
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  PWM_ON
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  STOP_FR
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //15  F_ref
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CURR_CONTR", "DA", TblFuncCURR_CONTR_1, FuncCURR_CONTR_1, sizeof(TblFuncCURR_CONTR_1), sizeof(CURR_CONTR_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCURR_CONTR_1,
#endif
//********************************************************
