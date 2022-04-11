#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Sel[8];                       	//1-8  	Sel_0_7
    u16  Pnt_Input;                        	//9  	Input
    u16  Pnt_Out[8];						//10-17	Out_7_0
    u16  Pnt_End;
}FuncBUS_MATRIX8_type;
//********************************************************
u16 *FuncBUS_MATRIX8_1(FuncBUS_MATRIX8_type *progPnt, u32 ramPnt)
{
    s16     NumFunc, Nbit;
    u16     Num, InBitMask, *pntIn, *pntOut;

    InBitMask  = load_s16(ramPnt, progPnt->Pnt_Input);

    pntIn  = progPnt->Pnt_Sel;
    pntOut = progPnt->Pnt_Out + 7;
    NumFunc=0;

    do{
        Num  = load_s16(ramPnt, *pntIn);        //номер входного бита
        Nbit = *pntOut;                         //номер бита функции

        if(Num == 0)    clrBitMicro(ramPnt, Nbit);
        else            { Num--;  defBitMicro(ramPnt, Nbit, (u16)((InBitMask>>Num) & 1)); }

        pntIn++;  pntOut--; NumFunc++;
    }while( NumFunc <8 );

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBUS_MATRIX8_1[17]={
        S16_PIN_TYPE | INPUT_PIN_MODE,   //1  Sel_0
        S16_PIN_TYPE | INPUT_PIN_MODE,   //2  Sel_1
        S16_PIN_TYPE | INPUT_PIN_MODE,   //3  Sel_2
        S16_PIN_TYPE | INPUT_PIN_MODE,   //4  Sel_3
        S16_PIN_TYPE | INPUT_PIN_MODE,   //5  Sel_4
        S16_PIN_TYPE | INPUT_PIN_MODE,   //6  Sel_5
        S16_PIN_TYPE | INPUT_PIN_MODE,   //7  Sel_6
        S16_PIN_TYPE | INPUT_PIN_MODE,   //8  Sel_7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Input
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //10  Out_7
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //11  Out_6
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //12  Out_5
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //13  Out_4
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //14  Out_3
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //15  Out_2
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //16  Out_1
		BIT_PIN_TYPE | OUTPUT_PIN_MODE    //17  Out_0
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("BUS_MATRIX8", "DA", TblFuncBUS_MATRIX8_1, FuncBUS_MATRIX8_1, sizeof(TblFuncBUS_MATRIX8_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBUS_MATRIX8_1,
#endif
//********************************************************
