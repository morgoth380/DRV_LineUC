#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Bit_0;                        //1  Bit_0
    u16  Pnt_Din1_Din12;                   //2  Din1_Din12
    u16  Pnt_Bit_13;                       //3  Bit_13
    u16  Pnt_Bit_14;                       //4  Bit_14
    u16  Pnt_Bit_15;                       //5  Bit_15
    u16  Pnt_Bit_16;                       //6  Bit_16
    u16  Pnt_Bit_17;                       //7  Bit_17
    u16  Pnt_Bit_18;                       //8  Bit_18
    u16  Pnt_Bit_19;                       //9  Bit_19
    u16  Pnt_Bit_20;                       //10  Bit_20
    u16  Pnt_Bit_21;                       //11  Bit_21
    u16  Pnt_Bit_22;                       //12  Bit_22
    u16  Pnt_Bit_23;                       //13  Bit_23
    u16  Pnt_Bit_24;                       //14  Bit_24
    u16  Pnt_Bit_25;                       //15  Bit_25
    u16  Pnt_Bit_26;                       //16  Bit_26
    u16  Pnt_Bit_27;                       //17  Bit_27
    u16  Pnt_Bit_28;                       //18  Bit_28
    u16  Pnt_Bit_29;                       //19  Bit_29
    u16  Pnt_Bit_30;                       //20  Bit_30
    u16  Pnt_Bit_31;                       //21  Bit_31
    u16  Pnt_Bit_32;                       //22  Bit_32
    u16  Pnt_Bit_33;                       //23  Bit_33
    u16  Pnt_Bit_34;                       //24  Bit_34
    u16  Pnt_Bit_35;                       //25  Bit_35
    u16  Pnt_Bit_36;                       //26  Bit_36
    u16  Pnt_Bit_37;                       //27  Bit_37
    u16  Pnt_Bit_38;                       //28  Bit_38
    u16  Pnt_Bit_39;                       //29  Bit_39
    u16  Pnt_Bit_40;                       //30  Bit_40
    u16  Pnt_Bit_41;                       //31  Bit_41
    u16  Pnt_Bit_42;                       //32  Bit_42
    u16  Pnt_Bit_45;                       //33  Bit_45
    u16  Pnt_Bit_43;                       //34  Bit_43
    u16  Pnt_Bit_44;                       //35  Bit_44
    u16  Pnt_Bit_46;                       //36  Bit_46
    u16  Pnt_Bit_47;                       //37  Bit_47
    u16  Pnt_Bit_48;                       //38  Bit_48
    u16  Pnt_Bit_49;                       //39  Bit_49
    u16  Pnt_Bit_50;                       //40  Bit_50
    u16  Pnt_Bit_51;                       //41  Bit_51
    u16  Pnt_Bit_52;                       //42  Bit_52
    u16  Pnt_Out_31_63;                    //43  Out_31_63
    u16  Pnt_Out_0_31;                     //44  Out_0_31
    u16  Pnt_End;
}FuncDIN_2U64_type;
//********************************************************
u16 *FuncDIN_2U64_1(FuncDIN_2U64_type *progPnt, u32 ramPnt)
{
	u16		i;
	u32		in0_31;
	u32		in32_63;
	u16*	ptrBit;

	in0_31 = (u32)((load_s16(ramPnt, progPnt->Pnt_Din1_Din12)&0x1FFF)<<1);
	ptrBit=(u16*)(&(progPnt->Pnt_Bit_14));
	for(i=14; i<32; i++){
		in0_31 |= (u32)((testBitMicro(ramPnt, *(ptrBit)))<<i);
		ptrBit++;
	}

	ptrBit=(u16*)(&(progPnt->Pnt_Bit_32));
	for(i=0, in32_63 = 0; i<(53-32); i++){
		in32_63|= (u32)((testBitMicro(ramPnt, *(ptrBit)))<<i);
		ptrBit++;
	}

	save_s32(ramPnt, progPnt->Pnt_Out_31_63, in32_63);
	save_s32(ramPnt, progPnt->Pnt_Out_0_31 , in0_31);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDIN_2U64_1[44]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Bit_0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Din1_Din12
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Bit_13
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Bit_14
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Bit_15
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Bit_16
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Bit_17
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Bit_18
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Bit_19
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Bit_20
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  Bit_21
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  Bit_22
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  Bit_23
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  Bit_24
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  Bit_25
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //16  Bit_26
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //17  Bit_27
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //18  Bit_28
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //19  Bit_29
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //20  Bit_30
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //21  Bit_31
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //22  Bit_32
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //23  Bit_33
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //24  Bit_34
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //25  Bit_35
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //26  Bit_36
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //27  Bit_37
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //28  Bit_38
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //29  Bit_39
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //30  Bit_40
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //31  Bit_41
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //32  Bit_42
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //33  Bit_45
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //34  Bit_43
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //35  Bit_44
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //36  Bit_46
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //37  Bit_47
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //38  Bit_48
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //39  Bit_49
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //40  Bit_50
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //41  Bit_51
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //42  Bit_52
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //43  Out_31_63
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //44  Out_0_31
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DIN_2U64", "DD", TblFuncDIN_2U64_1, FuncDIN_2U64_1, sizeof(TblFuncDIN_2U64_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDIN_2U64_1,
#endif
//********************************************************
