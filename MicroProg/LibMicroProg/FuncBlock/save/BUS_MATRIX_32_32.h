#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Sel[32];                      //1-32   Sel_0
    u16  Pnt_Mode;                         //33  Mode
    u16  Pnt_Bit_0_15;                     //34
    u16  Pnt_Bit_16_31;                    //35
    u16  Pnt_Error;                        //36  Error
    u16  Pnt_Out[32];                       //37-68 Pnt_Out
    u16  Pnt_End;
}FuncBUS_MATRIX_32_32_type;
//********************************************************
u16 *FuncBUS_MATRIX_32_32_1(FuncBUS_MATRIX_32_32_type *progPnt, u32 ramPnt)
{
    s16     NumFunc, Nbit;
    u32     Num, InBitMask;
    
    InBitMask = load_s16(ramPnt, progPnt->Pnt_Bit_0_15);
    InBitMask |= ((u32)load_s16(ramPnt, progPnt->Pnt_Bit_16_31)<<16);
    for(NumFunc=0; NumFunc<32; NumFunc++){
        Num  = load_s16(ramPnt, progPnt->Pnt_Sel[NumFunc]);        //номер входного бита
        Nbit = progPnt->Pnt_Out[NumFunc];        //номер бита функции
        if(Num == 0){
            clrBitMicro(ramPnt, Nbit);
        }
        else{
            Num--;
            defBitMicro(ramPnt, Nbit, (u16)((InBitMask>>Num)&1));
        }
    }
    clrBitMicro(ramPnt, progPnt->Pnt_Error);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBUS_MATRIX_32_32_1[68]={
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
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Sel_24
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  Sel_25
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  Sel_26
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Sel_27
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  Sel_28
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  Sel_29
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  Sel_30
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  Sel_31
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  Mode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  Bit_0-15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  Bit_16-31
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //36  Error
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //37  Out_0 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //38  Out_1 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //39  Out_2 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //40  Out_3 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //41  Out_4 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //42  Out_5 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //43  Out_6 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //44  Out_7 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //45  Out_8 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //46  Out_9 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //47  Out_10
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //48  Out_11
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //49  Out_12
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //50  Out_13
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //51  Out_14
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //52  Out_15
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //53  Out_16
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //54  Out_17
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //55  Out_18
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //56  Out_19
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //57  Out_20
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //58  Out_21
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //59  Out_22
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //60  Out_23
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //61  Out_24
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //62  Out_25
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //63  Out_26
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //64  Out_27
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //65  Out_28
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //66  Out_29
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //67  Out_30
         BIT_PIN_TYPE | OUTPUT_PIN_MODE     //68  Out_31
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("BUS_MATRIX_32_32", "DD", TblFuncBUS_MATRIX_32_32_1, FuncBUS_MATRIX_32_32_1, sizeof(TblFuncBUS_MATRIX_32_32_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBUS_MATRIX_32_32_1,
#endif
//********************************************************
