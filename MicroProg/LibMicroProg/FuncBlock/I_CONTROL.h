#ifdef    GET_FUNC_CODE

typedef struct {
    _iq     Uout_old    ;
//    s16     s           ;
}FuncI_CONTROLstate_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_I_ust;                        //1  I_ust
    u16  Pnt_I_tek;                        //2  I_tek
    u16  Pnt_U_ref;                        //3  U_ref
    u16  Pnt_K;                            //4  K
    u16  Pnt_F_comp;                       //5  F_comp
    u16  Pnt_F_REF;                        //6  F_REF
    u16  Pnt_Activ;                        //7  Activ
    u16  Pnt_EN;                           //8  EN
    u16  Pnt_U_out;                        //9  U_out
    u16  Pnt_End;
}FuncI_CONTROL_type;
//********************************************************
u16 *FuncI_CONTROL_1(FuncI_CONTROL_type *progPnt, u32 ramPnt)
{
    f32                             K;
    s32                             Uref, Uout, Iust,Itek;
    FuncI_CONTROLstate_type         *sPnt;

    sPnt = (FuncI_CONTROLstate_type *)(ramPnt + progPnt->Pnt_State);

    if (flagIsFirstCall){
        sPnt->Uout_old = 0;
//        sPnt->s        = 0;
    }
    Uref = load_s32(ramPnt, progPnt->Pnt_U_ref);
    if ((load_s16(ramPnt, progPnt->Pnt_Activ) == 1)){
        if (testBitMicro(ramPnt, progPnt->Pnt_EN)){
            Uout = sPnt->Uout_old;
            Iust = load_s32(ramPnt, progPnt->Pnt_I_ust);
            Itek = load_s32(ramPnt, progPnt->Pnt_I_tek);
            K = ((f32)load_s16(ramPnt, progPnt->Pnt_K)) / 10000.0f;
            if((load_s32(ramPnt, progPnt->Pnt_F_REF) < load_s32(ramPnt, progPnt->Pnt_F_comp))){
                Uout += _IQmpy((Iust - Itek), _IQ(K));
                if (Uout < 0){
                    Uout = 0;
                }else{
                    if (Uout > _IQ(1.0f)){
                        Uout = _IQ(1.0f);
                    }
                }
                sPnt->Uout_old = Uout;
                save_s32(ramPnt, progPnt->Pnt_U_out, sPnt->Uout_old);
            }else{
                if (sPnt->Uout_old == Uref){
                    sPnt->Uout_old = Uref;
                }else{
                    sPnt->Uout_old = iq_Filtr_prm(sPnt->Uout_old, Uref, 9);
                }
                save_s32(ramPnt, progPnt->Pnt_U_out, sPnt->Uout_old);
            }
        }else{
            sPnt->Uout_old = 0;
            save_s32(ramPnt, progPnt->Pnt_U_out, sPnt->Uout_old);
        }
    }else{
        save_s32(ramPnt, progPnt->Pnt_U_out, Uref);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncI_CONTROL_1[9]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  I_ust
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  I_tek
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  U_ref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  K
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F_comp
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  F_ref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Activ
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  EN
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //9  U_out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("I_CONTROL", "DA", TblFuncI_CONTROL_1, FuncI_CONTROL_1, sizeof(TblFuncI_CONTROL_1), sizeof(FuncI_CONTROLstate_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncI_CONTROL_1,
#endif
//********************************************************
