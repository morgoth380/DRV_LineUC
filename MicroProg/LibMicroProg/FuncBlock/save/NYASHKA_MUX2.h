#ifdef    GET_FUNC_CODE

 typedef struct {
	 s32     prev_out;
     s16     prev_num;
     s16     jump;
}FuncNYASHKA_MUX2State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In[2];                        //1  In0
    u16  Pnt_Quant;                        //3  Quant
    u16  Pnt_Num;                          //4  Num
    u16  Pnt_Out;                          //5  Out
    u16  Pnt_End;
}FuncNYASHKA_MUX2_type;
//********************************************************
u16 *FuncNYASHKA_MUX2(FuncNYASHKA_MUX2_type *progPnt, u32 ramPnt)
{
    s16 Num;
    s32 tmp;
    FuncNYASHKA_MUX2State_type    *sPnt;
    sPnt = (FuncNYASHKA_MUX2State_type *)(ramPnt + progPnt->Pnt_State);
    
    Num = testBitMicro(ramPnt, progPnt->Pnt_Num);
    if (Num != sPnt->prev_num){
    	sPnt->jump = 1;
    	sPnt->prev_num = Num;
    }

    if(sPnt->jump == 1){
    	tmp = sPnt->prev_out + load_s32(ramPnt, progPnt->Pnt_Quant);
    	if(tmp > 0){
		    sPnt->prev_out = tmp;
    	}
		if(sPnt->prev_out >= load_s32(ramPnt, progPnt->Pnt_In[Num])){
			sPnt->prev_out = load_s32(ramPnt, progPnt->Pnt_In[Num]);
			sPnt->jump = 0;
		}
    }else{
    	sPnt->prev_out = load_s32(ramPnt, progPnt->Pnt_In[Num]);
    }
    save_s32(ramPnt, progPnt->Pnt_Out, sPnt->prev_out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncNYASHKA_MUX2[5]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Quant
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Num
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //5  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("NYASHKA_MUX2", "DA", TblFuncNYASHKA_MUX2, FuncNYASHKA_MUX2, sizeof(TblFuncNYASHKA_MUX2), sizeof(FuncNYASHKA_MUX2State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncNYASHKA_MUX2,
#endif
//********************************************************
