#ifdef    GET_FUNC_CODE

typedef struct{
        _iq  a_prev;
        _iq  b_prev;
}CMP_PAR_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_A;                            //1  A
    u16  Pnt_B;                            //2  B
    u16  Pnt_B_out;                        //3  B_out
    u16  Pnt_A_out;                        //4  A_out
    u16  Pnt_End;
}FuncCMP_PAR_type;
//********************************************************
u16 *FuncCMP_PAR_1(FuncCMP_PAR_type *progPnt, u32 ramPnt)
{
    CMP_PAR_State_type    *sPnt;
    _iq a, b;
    sPnt = (CMP_PAR_State_type *)(ramPnt + progPnt->Pnt_State);

    if (flagIsFirstCall){
		sPnt->a_prev = _IQ(1.0f);
		sPnt->b_prev = 0;
        return &progPnt->Pnt_End;
    }

    a = load_s32(ramPnt, progPnt->Pnt_A);
    b = load_s32(ramPnt, progPnt->Pnt_B);
    if (a != sPnt->a_prev){
        if (a <= b){
            save_s32(ramPnt, progPnt->Pnt_A, sPnt->a_prev);
            save_s32(ramPnt, progPnt->Pnt_A_out, sPnt->a_prev);
        }else{
            sPnt->a_prev = a;
            save_s32(ramPnt, progPnt->Pnt_A_out, a);
        }
    }
    if (b != sPnt->b_prev){
        if (b >= a){
            save_s32(ramPnt, progPnt->Pnt_B, sPnt->b_prev);
            save_s32(ramPnt, progPnt->Pnt_A_out, sPnt->b_prev);
        }else{
            sPnt->b_prev = b;
            save_s32(ramPnt, progPnt->Pnt_B_out, b);
        }
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCMP_PAR_1[4]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  A
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  B
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //3  B_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //4  A_out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CMP_PAR", "DA", TblFuncCMP_PAR_1, FuncCMP_PAR_1, sizeof(TblFuncCMP_PAR_1), sizeof(CMP_PAR_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMP_PAR_1,
#endif
//********************************************************
