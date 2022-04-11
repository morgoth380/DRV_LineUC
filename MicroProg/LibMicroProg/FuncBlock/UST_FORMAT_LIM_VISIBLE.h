#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Arg_1;                        //1  Arg_1
    u16  Pnt_HighLim;                      //2  HighLim
    u16  Pnt_LowLim;                       //3  LowLim
    u16  Pnt_Format;                       //4  Format
	u16  Pnt_Visible;					   //5  Visible	
    u16  Pnt_End;
}FuncUST_FORMAT_LIM_VISIBLE_type;
//********************************************************
u16 *FuncUST_FORMAT_LIM_VISIBLE_1(FuncUST_FORMAT_LIM_VISIBLE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_FORMAT_LIM_VISIBLE_1[5]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,   //1  Arg_1
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //2  HighLim
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //3  LowLim
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,    //4  Format
		S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,    //5  Visible
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_FORMAT_LIM_VISIBLE", "UST", TblFuncUST_FORMAT_LIM_VISIBLE_1, FuncUST_FORMAT_LIM_VISIBLE_1, sizeof(TblFuncUST_FORMAT_LIM_VISIBLE_1), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_FORMAT_LIM_VISIBLE_1,
#endif
//********************************************************
//********************************************************
//********************************************************
//2 Перезагрузка	2 Перезагрузка	2 Перезагрузка	
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncUST_FORMAT_LIM_VISIBLE_2(FuncUST_FORMAT_LIM_VISIBLE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_FORMAT_LIM_VISIBLE_2[5]={
        S32_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,   //1  Arg_1
        S32_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //2  HighLim
        S32_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //3  LowLim
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,    //4  Format
		S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,    //5  Visible
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_FORMAT_LIM_VISIBLE", "UST", TblFuncUST_FORMAT_LIM_VISIBLE_2, FuncUST_FORMAT_LIM_VISIBLE_2, sizeof(TblFuncUST_FORMAT_LIM_VISIBLE_2), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_FORMAT_LIM_2,
#endif
//********************************************************
//********************************************************
//3 Перезагрузка	3 Перезагрузка	3 Перезагрузка	
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
u16 *FuncUST_FORMAT_LIM_VISIBLE_3(FuncUST_FORMAT_LIM_VISIBLE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUST_FORMAT_LIM_VISIBLE_3[5]={
        S32_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,   //1  Arg_1
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //2  HighLim
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //3  LowLim
        S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //4  Format
		S16_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG,   //5  Visible
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UST_FORMAT_LIM_VISIBLE", "UST", TblFuncUST_FORMAT_LIM_VISIBLE_3, FuncUST_FORMAT_LIM_VISIBLE_3, sizeof(TblFuncUST_FORMAT_LIM_VISIBLE_3), 0, 0, 0),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUST_FORMAT_LIM_VISIBLE_3,
#endif
//********************************************************
