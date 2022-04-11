#ifdef    GET_FUNC_CODE
    typedef struct{
        u16  Pnt_Nfunc;
        u16  Pnt_Arg_1;                        //1  Arg_1
        u16  Pnt_Arg_2;                        //2  Arg_2
        u16  Pnt_Arg_3;                        //3  Arg_3
        u16  Pnt_End;
    }FuncAND_2_type;
    //********************************************************
    u16 *FuncAND_2_1(FuncAND_2_type *progPnt, u32 ramPnt)
    {
    	s16		Arg1, Arg2;
        
    	Arg1 = testBitMicro(ramPnt, progPnt->Pnt_Arg_1);
    	Arg2 = testBitMicro(ramPnt, progPnt->Pnt_Arg_2);
    	if(Arg1&Arg2)	setBitMicro(ramPnt, progPnt->Pnt_Arg_3);
    	else	    	clrBitMicro(ramPnt, progPnt->Pnt_Arg_3);
    	return &progPnt->Pnt_End;
    };
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
    const char TblFuncAND_2_1[3]={
            BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
            BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Arg_2
            BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  Arg_3
    };
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
    //SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
    OpisFuncBlock("AND-2", "DD", TblFuncAND_2_1, FuncAND_2_1, sizeof(TblFuncAND_2_1),0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAND_2_1,
#endif
//********************************************************
