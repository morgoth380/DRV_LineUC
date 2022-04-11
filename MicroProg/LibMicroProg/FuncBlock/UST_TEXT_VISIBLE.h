#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_Text;                         //2  Text
	u16  Pnt_Visible;                         //3  Visible
    u16  Pnt_End;
}FuncUST_TEXT_VISIBLE_type;
//********************************************************
u16 *FuncUST_TEXT_VISIBLE_1(FuncUST_TEXT_VISIBLE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_TEXT_VISIBLE_1[3]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,   //1  Arg_1
        TXT_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //2  Arg_2
		S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //3  Visible
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_TEXT_VISIBLE", "UST", TblFuncUST_TEXT_VISIBLE_1, FuncUST_TEXT_VISIBLE_1, sizeof(TblFuncUST_TEXT_VISIBLE_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_TEXT_VISIBLE_1,
#endif
//********************************************************
