#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Sel[16];                      //1  Sel_0_16
    u16  Pnt_Mode;                         //17  Mode
    u16  Pnt_Bit_0_15;                     //18  Bit_0-15
    u16  Pnt_Bit_16_31;                    //19  Bit_16-31
    u16  Pnt_Error;                        //20  Error
    u16  Pnt_Out[16];                      //21  Out_0_16
    u16  Pnt_End;
}FuncBUS_MATRIX_16_32_type;
//********************************************************
u16 *FuncBUS_MATRIX_16_32_1(FuncBUS_MATRIX_16_32_type *progPnt, u32 ramPnt)
{

    s16     NumFunc, Nbit;
    u16     Num, *pntIn, *pntOut, yngr, oldr;
    u32     InBitMask;

    yngr =  load_s16(ramPnt, progPnt->Pnt_Bit_0_15 );
    oldr = load_s16(ramPnt, progPnt->Pnt_Bit_16_31);
    
    InBitMask  = yngr | (u32)oldr<<16;

    pntIn  = progPnt->Pnt_Sel;
    pntOut = progPnt->Pnt_Out;
    NumFunc=0; 
    do{
        Num  = load_s16(ramPnt, *pntIn);        //номер входного бита
        Nbit = *pntOut;                            //номер бита функции

        if(Num == 0)    clrBitMicro(ramPnt, Nbit);
        else            { Num--;  defBitMicro(ramPnt, Nbit, (u16)((InBitMask>>Num) & 1)); }
            
        pntIn++;  pntOut++; NumFunc++;
    }while( NumFunc <16 );

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBUS_MATRIX_16_32_1[36]={
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
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Mode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Bit_0-15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Bit_16-31
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //20  Error
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  Out_0
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Out_1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  Out_2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  Out_3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  Out_4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  Out_5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //27  Out_6
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //28  Out_7
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //29  Out_8
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //30  Out_9
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //31  Out_10
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //32  Out_11
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //33  Out_12
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //34  Out_13
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //35  Out_14
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //36  Out_15
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew("BUS_MATRIX_16_32", "DD", TblFuncBUS_MATRIX_16_32_1, FuncBUS_MATRIX_16_32_1, sizeof(TblFuncBUS_MATRIX_16_32_1), 0, 0, 1, sizeof(FuncBUS_MATRIX_16_32_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBUS_MATRIX_16_32_1,
#endif
//********************************************************
