#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Sel[24];				       //1-24   Sel_0_23
    u16  Pnt_Input;                        //25     Input
    u16  Pnt_Out[24];                      //26-49  Out_23_0
    u16  Pnt_End;
}FuncBUS_MATRIX24_type;
//********************************************************
u16 *FuncBUS_MATRIX24_1(FuncBUS_MATRIX24_type *progPnt, u32 ramPnt)
{
	s32     InBitMask;
    s16     NumFunc, Nbit;
    u16     Num, *pntIn, *pntOut;

    InBitMask  = load_s32(ramPnt, progPnt->Pnt_Input);

    pntIn  = progPnt->Pnt_Sel;
    pntOut = progPnt->Pnt_Out + 23;
    NumFunc=0;

    do{
        Num  = load_s16(ramPnt, *pntIn);        //номер входного бита
        Nbit = *pntOut;                         //номер бита функции

        if(Num == 0)    clrBitMicro(ramPnt, Nbit);
        else{
			Num--;
			defBitMicro(ramPnt, Nbit, (u16)((InBitMask>>Num) & 1)); 
		}

        pntIn++;  pntOut--; NumFunc++;
    }while( NumFunc < 24 );

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBUS_MATRIX24_1[49]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Sel_0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Sel_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Sel_2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Sel_3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Sel_4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Sel_5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Sel_6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Sel_7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Sel_8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Sel_9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Sel_10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Sel_11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Sel_12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Sel_13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Sel_14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Sel_15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Sel_16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Sel_17
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Sel_18
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Sel_19
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Sel_20
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Sel_21
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Sel_22
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  Sel_23
        S32_PIN_TYPE | INPUT_PIN_MODE,     //25  Input
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  Out_23
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //27  Out_22
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //28  Out_21
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //29  Out_20
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //30  Out_19
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //31  Out_18
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //32  Out_17
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //33  Out_16
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //34  Out_15
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //35  Out_14
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //36  Out_13
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //37  Out_12
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //38  Out_11
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //39  Out_10
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //40  Out_9
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //41  Out_8
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //42  Out_7
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //43  Out_6
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //44  Out_5
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //45  Out_4
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //46  Out_3
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //47  Out_2
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //48  Out_1
		BIT_PIN_TYPE | OUTPUT_PIN_MODE     //49  Out_0
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("BUS_MATRIX24", "DA", TblFuncBUS_MATRIX24_1, FuncBUS_MATRIX24_1, sizeof(TblFuncBUS_MATRIX24_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBUS_MATRIX24_1,
#endif
//********************************************************
