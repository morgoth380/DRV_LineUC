#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_RingUp;                       //1  RingUp
    u16  Pnt_Fail_V;                       //2  Fial_V
    u16  Pnt_Fail_W;                       //3  Fail_W
    u16  Pnt_Fail_U;                       //4  Fail_U
    u16  Pnt_OK;                           //5  End
    u16  Pnt_End;
}FuncRINGUP_type;
//********************************************************
u16 *FuncRINGUP_1(FuncRINGUP_type *progPnt, u32 ramPnt)
{
    if (testBitMicro(ramPnt, progPnt->Pnt_RingUp) && RUState == 0 &&measMem.Fout == 0){
        if (RingUp == 0){
            measMem.Angle = _IQ(0.0);
            
        }
        RingUp = 1;
        clrBitMicro(ramPnt, progPnt->Pnt_OK);
    }else{
        if (RUState == 4){
            setBitMicro(ramPnt, progPnt->Pnt_OK);
            RingUp = 0;
        }else{
            clrBitMicro(ramPnt, progPnt->Pnt_OK);
        }
        if (RUState == 1){
            setBitMicro(ramPnt, progPnt->Pnt_Fail_U);
        }else
            if (RUState == 2){
                setBitMicro(ramPnt, progPnt->Pnt_Fail_V);
            }else
                if (RUState == 3){
                    setBitMicro(ramPnt, progPnt->Pnt_Fail_W);
                }
    }
    if (!testBitMicro(ramPnt, progPnt->Pnt_RingUp)){
        RUState = 0;
        AngCount = 0;
        RingUp = 0;
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRINGUP_1[5]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  RingUp
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Fial_V
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Fail_W
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //4  Fail_U
        BIT_PIN_TYPE | OUTPUT_PIN_MODE    //5  OK
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("RINGUP", "DA", TblFuncRINGUP_1, FuncRINGUP_1, sizeof(TblFuncRINGUP_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRINGUP_1,
#endif
//********************************************************
