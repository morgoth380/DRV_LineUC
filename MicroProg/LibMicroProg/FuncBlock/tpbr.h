#ifdef    GET_FUNC_CODE
typedef struct{
    f32  Integral;
    u16  Sec05;
}TPBR_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Pbr;                          //1  Pbr
    u16  Pnt_Rbr;                          //2  Rbr
    u16  Pnt_Tact;                         //3  Tact
    u16  Pnt_E;                            //4  E
    u16  Pnt_TtoAcc;                       //5  TtoAcc
    u16  Pnt_ti_Tact;                      //6  ti>Tact
    u16  Pnt_End;
}FuncTPBR_type;
//********************************************************
u16 *FuncTPBR_1(FuncTPBR_type *progPnt, u32 ramPnt)
{
  TPBR_State_type   *sPnt;
    _iq data;
    u16 tpbrEn;
    s16 tAct, tActUst;
    f32 dS;
    f32 Tau, Ud, R, icmp, Pust, UdNaTau, ibr;
    COMMONPARAMC28 *mPnt;
    COMMONPARAMM3 *mPnt3;
    mPnt = &GlobalC28;
    mPnt3 = &GlobalM3;
    
    sPnt = (TPBR_State_type *)(ramPnt + progPnt->Pnt_State);
    
    Pust = ((f32)load_s16(ramPnt, progPnt->Pnt_Pbr) / (f32)10.0) * 1000.0f;
    R = (f32)load_s16(ramPnt, progPnt->Pnt_Rbr) / (f32)100.0;

    tpbrEn = testBitMicro(ramPnt, progPnt->Pnt_E);
    
    if (tpbrEn && mPnt3->FlagCommand.bit.TormSwEnable){
        data = mPnt->UdcFast - mPnt3->Ud0;
        if (data > _IQ(0.)){
            Tau = (f32)(_IQtoF(data) / _IQtoF(mPnt3->Ud100 - mPnt3->Ud0));
        }
        else{
            Tau = 0.;
        }
        Ud  = _IQtoF(mPnt->UdcFast) * MPOpis.Base[NUM_VOLT_BASE];// * (f32)1.4142135623730950488016887242097;
        //P = (Ud * Ud * Tau * Tau) / R;
        if (Tau < 0.0001){
            UdNaTau = 0;
            icmp = Pust / (f32)0.0001;
        }
        else{
            UdNaTau = Ud * Tau;
            icmp = Pust / UdNaTau;
        }        
        
        ibr = UdNaTau / R;//_IQtoF(IbrUst) * MPOpis.Base[NUM_CUR_BASE];
        dS = ((ibr - icmp) / icmp) * (f32)0.5;
        
        if (dS > 0 || sPnt->Integral > 0){
            if (sPnt->Sec05 < K_TIME_CALL_MICRO_TAKT / 2){
                sPnt->Sec05++;
            }
            else{
                sPnt->Sec05 = 0;
                sPnt->Integral += dS;
                tActUst = load_s16(ramPnt, progPnt->Pnt_Tact);
                if (sPnt->Integral > (f32)tActUst){
                    sPnt->Integral = (f32)tActUst;
                }
                else if (sPnt->Integral < (f32)0.0){
                    sPnt->Integral = (f32)0.;
                }
                 
                tActUst = load_s16(ramPnt, progPnt->Pnt_Tact);
                if (dS < 0.){
                    tAct = -(s16)sPnt->Integral;// - tActUst;
                    if (tAct == 0){
                        clrBitMicro(ramPnt, progPnt->Pnt_ti_Tact);
                    }
                }
                else{
                    tAct = tActUst - (s16)sPnt->Integral;
                    if (tAct == 0){
                        setBitMicro(ramPnt, progPnt->Pnt_ti_Tact);
                    }
                }
                save_s16(ramPnt, progPnt->Pnt_TtoAcc, tAct);
            }
        }
    }
    else{
        sPnt->Sec05 = 0;
        sPnt->Integral = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_ti_Tact);
        save_s16(ramPnt, progPnt->Pnt_TtoAcc, 0);
    }
    return &progPnt->Pnt_End;
};

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTPBR_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Pbr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Rbr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Tact
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  E
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  TtoAcc
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //6  ti>Tact
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TPBR", "DD", TblFuncTPBR_1, FuncTPBR_1, sizeof(TblFuncTPBR_1), sizeof(TPBR_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTPBR_1,
#endif
//********************************************************
