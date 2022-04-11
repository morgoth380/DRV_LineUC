#ifdef    GET_FUNC_CODE

typedef struct{
    u16     Ramp_down_time;
}STOP_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In[4];                         //1  NETW
//    u16  Pnt_DIN;                          //2  DIN
//    u16  Pnt_SLEEP;                        //3  SLEEP
//    u16  Pnt_STBST;                        //4  STBST
    u16  Pnt_EN_FFS;                       //5  EN_FFS
    u16  Pnt_STP_DC;                       //6  STP_DC
    u16  Pnt_EXT_BR;                       //7  EXT_BR
    u16  Pnt_S_KEY;                        //8  S_KEY
    u16  Pnt_PWM_F;                        //9  PWM_F
    u16  Pnt_CONT;                         //10  CONT
    u16  Pnt_S_W_PU;                       //11  S_W_PU
    u16  Pnt_FNETW;                        //12  FNETW
    u16  Pnt_S_E_BR;                       //13  S_E_BR
    u16  Pnt_DC_OFF;                       //14  DC_OFF
    u16  Pnt_RS_OFF;                       //15  RS_OFF
    u16  Pnt_F_REF;                        //16  F_REF
    u16  Pnt_F_PUSK;                       //17  F_PUSK
    u16  Pnt_RD_TM;                        //18  RD_TM
    u16  Pnt_USTME;                        //19  USTME
    u16  Pnt_USTTYPE;                      //20  USTTYPE
    u16  Pnt_NPWM;                         //21  NPWM
    u16  Pnt_SEL;                          //22  SEL
    u16  Pnt_ST_IMP;                       //23  ST_IMP
    u16  Pnt_COMBI;                        //24  COMBI
    u16  Pnt_FREQ;                         //25  FREQ
    u16  Pnt_RMPDN;                        //26  RMPDN
    u16  Pnt_End;
}FuncSTOP_type;
//********************************************************
u16 *FuncSTOP_1(FuncSTOP_type *progPnt, u32 ramPnt)
{
    u16 i;
    u16 stop = 0;
    s16 selType, tmp;
    STOP_State_type *sPnt;

    COMMONPARAMM3 *mPnt3;
    mPnt3 = &GlobalM3;

    sPnt = (STOP_State_type *)(ramPnt + progPnt->Pnt_State);

    if (flagIsFirstCall){
        sPnt->Ramp_down_time = 0;
    }
// стопы------------------------------------------------------------------------
    for (i = 0; i < 4; i++){
        if (testBitMicro(ramPnt, progPnt->Pnt_In[i]) != 0){
            stop = 1;
            break;
        }
    }
    if (stop == 1){
        selType = load_s16(ramPnt, progPnt->Pnt_SEL);
        switch (selType){
            case RAMPDOWN:
                setBitMicro(ramPnt, progPnt->Pnt_RMPDN);
                sPnt->Ramp_down_time = load_s16(ramPnt, progPnt->Pnt_RD_TM) * K_TIME_CALL_MICRO_TAKT;
                break;

            case FREQUNCY:
                if (load_s16(ramPnt, progPnt->Pnt_USTME) == 0){
                    tmp = load_s16(ramPnt, progPnt->Pnt_USTTYPE);
                    if ((tmp == TYPE_DRV_VECT_DAT) || (tmp == TYPE_DRV_VECT)){
                        setBitMicro(ramPnt, progPnt->Pnt_RMPDN);
                    }else{
                        setBitMicro(ramPnt, progPnt->Pnt_FREQ);
                    }
                }else{
                    setBitMicro(ramPnt, progPnt->Pnt_FREQ);
                }
                break;

            case COMBI:
                if (load_s16(ramPnt, progPnt->Pnt_USTME) == 0){
                    tmp = load_s16(ramPnt, progPnt->Pnt_USTTYPE);
                    if ((tmp == TYPE_DRV_VECT_DAT) || (tmp == TYPE_DRV_VECT)){
                        setBitMicro(ramPnt, progPnt->Pnt_RMPDN);
                    }else{
                        setBitMicro(ramPnt, progPnt->Pnt_FREQ);
                        setBitMicro(ramPnt, progPnt->Pnt_COMBI);
                    }
                }else{
                    setBitMicro(ramPnt, progPnt->Pnt_FREQ);
                    setBitMicro(ramPnt, progPnt->Pnt_COMBI);
                }
                break;

            default: break;
        }
        setBitMicro(ramPnt, progPnt->Pnt_ST_IMP);
    }else{
        clrBitMicro(ramPnt, progPnt->Pnt_ST_IMP);
        if (sPnt->Ramp_down_time <= 0){
        clrBitMicro(ramPnt, progPnt->Pnt_RMPDN);
        }else{
            sPnt->Ramp_down_time--;
        }
    }

    if (testBitMicro(ramPnt, progPnt->Pnt_NPWM) != 0){
        clrBitMicro(ramPnt, progPnt->Pnt_COMBI);
        clrBitMicro(ramPnt, progPnt->Pnt_FREQ);
    }else{
        if (mPnt3->signRef > 0){
            if ((load_s32(ramPnt, progPnt->Pnt_F_REF) <= load_s32(ramPnt, progPnt->Pnt_F_PUSK)) &&
                    ((testBitMicro(ramPnt, progPnt->Pnt_EN_FFS) == 1) ||
                    ((testBitMicro(ramPnt, progPnt->Pnt_FREQ) == 1) && (testBitMicro(ramPnt, progPnt->Pnt_EXT_BR) == 0) &&
                    (testBitMicro(ramPnt, progPnt->Pnt_STP_DC) == 0) ))){
                setBitMicro(ramPnt, progPnt->Pnt_RMPDN);
            }
        }else{
            if ((load_s32(ramPnt, progPnt->Pnt_F_REF) >= (-load_s32(ramPnt, progPnt->Pnt_F_PUSK))) &&
                    ((testBitMicro(ramPnt, progPnt->Pnt_EN_FFS) == 1) ||
                    ((testBitMicro(ramPnt, progPnt->Pnt_FREQ) == 1) && (testBitMicro(ramPnt, progPnt->Pnt_EXT_BR) == 0) &&
                    (testBitMicro(ramPnt, progPnt->Pnt_STP_DC) == 0) ))){
                setBitMicro(ramPnt, progPnt->Pnt_RMPDN);
            }
        }
        if ((testBitMicro(ramPnt, progPnt->Pnt_S_KEY) == 1) ||  (testBitMicro(ramPnt, progPnt->Pnt_PWM_F) == 1) ||
                (testBitMicro(ramPnt, progPnt->Pnt_S_E_BR) == 1) ||(testBitMicro(ramPnt, progPnt->Pnt_CONT) == 1) ||
                (testBitMicro(ramPnt, progPnt->Pnt_DC_OFF) == 1) ||(testBitMicro(ramPnt, progPnt->Pnt_RS_OFF) == 1) ||
                (testBitMicro(ramPnt, progPnt->Pnt_S_W_PU) == 1) ||(testBitMicro(ramPnt, progPnt->Pnt_FNETW) == 1)){
            setBitMicro(ramPnt, progPnt->Pnt_RMPDN);
            if ((testBitMicro(ramPnt, progPnt->Pnt_S_E_BR) == 0) && (testBitMicro(ramPnt, progPnt->Pnt_DC_OFF) == 0) &&
                    (testBitMicro(ramPnt, progPnt->Pnt_RS_OFF) == 0)){
                        sPnt->Ramp_down_time = load_s16(ramPnt, progPnt->Pnt_RD_TM) * K_TIME_CALL_MICRO_TAKT;
            }
        }
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSTOP_1[26]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  NETW
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  DIN
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  SLEEP
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  STBST
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  EN_FFS
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  STP_DC
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  EXT_BR
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  S_KEY
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  PWM_F
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  CONT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  S_W_PU
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  FNETW
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  S_E_BR
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  DC_OFF
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  RS_OFF
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  F_REF
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  F_PUSK
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  RD_TM
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  USTME
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  USTTYPE
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //21  NPWM
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  SEL
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  ST_IMP
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  COMBI
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  FREQ
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //26  RMPDN
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("STOP", "DA", TblFuncSTOP_1, FuncSTOP_1, sizeof(TblFuncSTOP_1), sizeof(STOP_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSTOP_1,
#endif
//********************************************************
