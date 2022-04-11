#ifdef    GET_FUNC_CODE

typedef struct{
    s32  f_evak;
    u16  mode:1;
}nvEVAKState_type;

typedef struct{
    s32  f_evak;
    u16  mode:1;
    u16  write_ok:1;
    u16  evak_ativated:1;
}EVAKState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_EV_EN;                        //1  EV_EN
    u16  Pnt_EV_ACT;                       //2  EV_ACT
    u16  Pnt_U_FAIL;                       //3  U_FAIL
    u16  Pnt_F_DRV;                        //4  F_DRV
    u16  Pnt_F_UST;                        //5  F_EV
    u16  Pnt_Wr;                           //6  Wr
    u16  Pnt_We;                           //7  We
    u16  Pnt_F_REF;                        //8  F_REF
    u16  Pnt_PWM;                          //9  PWM
    u16  Pnt_MOT_GEN;                      //10  MOT_GEN
    u16  Pnt_F_EV;                         //11  F_EV
    u16  Pnt_EV_ON;                        //12  EV_ON
    u16  Pnt_End;
}FuncEVAK_type;
//********************************************************
u16 *FuncEVAK_1(FuncEVAK_type *progPnt, u32 ramPnt)
{
    _iq                        Fout;
    EVAKState_type             *sPnt;
    nvEVAKState_type           *nvAdr;

    sPnt        = (EVAKState_type *)(ramPnt + progPnt->Pnt_State);
    nvAdr       = GetNvAdr(progPnt->Pnt_nvState, 0);

    if (flagIsFirstCall){
        RdFramMicro( sPnt, nvAdr, sizeof(nvEVAKState_type)); // Восстановить состояние
        sPnt->evak_ativated = 0;
        sPnt->write_ok = 0;
    }

    if (testBitMicro(ramPnt, progPnt->Pnt_PWM) == 1){
        if (sPnt->evak_ativated == 0){
            if (testBitMicro(ramPnt, progPnt->Pnt_U_FAIL) == 1){
                if(sPnt->write_ok == 0){
                    WrFramMicro( nvAdr, sPnt, sizeof(nvEVAKState_type));
                    sPnt->write_ok =1;
                }
            }else{
                sPnt->write_ok =0;
                if (load_s32(ramPnt, progPnt->Pnt_Wr) > load_s32(ramPnt, progPnt->Pnt_We)){
                    setBitMicro(ramPnt, progPnt->Pnt_MOT_GEN);
                    sPnt->mode = 1;
                }else{
                    clrBitMicro(ramPnt, progPnt->Pnt_MOT_GEN);
                    sPnt->mode = 0;
                }
                if (sPnt->mode == 1){
                    sPnt->f_evak = load_s32(ramPnt, progPnt->Pnt_F_UST);
                }else{
                    sPnt->f_evak = -load_s32(ramPnt, progPnt->Pnt_F_UST);
                }
            }
        }else{
            setBitMicro(ramPnt, progPnt->Pnt_EV_ON);
            Fout = load_s32(ramPnt, progPnt->Pnt_F_REF);
            if (sPnt->f_evak > 0){
                if (Fout < 0){
                    Fout = -Fout;
                }
            }else{
                if (Fout > 0){
                    Fout = -Fout;
                }
            }
            if (_IQabs(Fout) > _IQabs(sPnt->f_evak)){
                save_s32(ramPnt, progPnt->Pnt_F_EV, sPnt->f_evak);
            }else{
                save_s32(ramPnt, progPnt->Pnt_F_EV, Fout);
            }
        }
    }else{
        if ((load_s16(ramPnt, progPnt->Pnt_EV_EN) == 1) && (testBitMicro(ramPnt, progPnt->Pnt_EV_ACT)) && (testBitMicro(ramPnt, progPnt->Pnt_U_FAIL))){
            sPnt->evak_ativated = 1;
            setBitMicro(ramPnt, progPnt->Pnt_EV_ON);
        }else{
            sPnt->evak_ativated = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_EV_ON);
        }
        save_s32(ramPnt, progPnt->Pnt_F_EV, 0);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncEVAK_1[12]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  EV_EN
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  EV_ACT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  U_FAIL
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F_DRV
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F_UST
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Wr
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  We
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  F_REF
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  PWM
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  MOT_GEN
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //11  F_EV
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //12  EV_ON
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("EVAK", "DA", TblFuncEVAK_1, FuncEVAK_1, sizeof(TblFuncEVAK_1), sizeof(EVAKState_type), sizeof(nvEVAKState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncEVAK_1,
#endif
//********************************************************
