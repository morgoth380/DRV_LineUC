#ifdef    GET_FUNC_CODE

// MACROS lower level variables
#define _M_IREACT      GlobalC28.Ireact
#define _M_Udc         GlobalC28.Udc
#define _M_UdcFAST     GlobalC28.UdcFast

typedef struct{
        s16     i;
        _iq     Iold;
        _iq     dI;
        s16     F_seek_OK;
        s16     Pick_Up_OK;
        _iq     Ud_control;
        u16     nasischenie;
        s16     k;
        _iq     Uout;
        _iq     Fout;
        _iq     Fmax;
        s32     sign;
        s16     rev;
        s16     FmaxFlag;
}PICK_UP_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Start;                        //1  Start
    u16  Pnt_T_nasischen;                  //2  T_nasischen
    u16  Pnt_Temp_scan_F;                  //3  Temp_scan_F
    u16  Pnt_Temp_pods_F;                  //4  Temp_pods_F
    u16  Pnt_Temp_nar_U;                   //5  Temp_nar_U
    u16  Pnt_U_scan;                       //6  U_scan
    u16  Pnt_d_Ud;                         //7  d_Ud
    u16  Pnt_F_start_scan;                 //8  F_start_scan
    u16  Pnt_K_di_dT;                      //10  K_di/dT
    u16  Pnt_UF_TEK;                       //11  UF_TEK
    u16  Pnt_EN;                           //12  EN
    u16  Pnt_Fail;                         //13  Fail
    u16  Pnt_U_P_UP;                       //14  F_P_U
    u16  Pnt_F_P_UP;                       //15  F_P_UP
    u16  Pnt_UnHold_GT;                    //16  Hold_GT
    u16  Pnt_End;
}FuncPICK_UP_type;
//********************************************************

u16 *FuncPICK_UP_1(FuncPICK_UP_type *progPnt, u32 ramPnt)
{
    PICK_UP_State_type              *sPnt;
    u16                             start;
    _iq                             Uf, add;

    COMMONPARAMM3 *mPnt3;
    mPnt3 = &GlobalM3;
    
    sPnt = (PICK_UP_State_type *)(ramPnt + progPnt->Pnt_State);

    if(flagIsFirstCall){
        sPnt->i = 0;
        sPnt->k = 0;
        sPnt->nasischenie = load_s16(ramPnt, progPnt->Pnt_T_nasischen) * 250;
        sPnt->Pick_Up_OK = 0;
        setBitMicro(ramPnt, progPnt->Pnt_UnHold_GT);
        sPnt->Fout = 0;
        sPnt->Uout = 0;
        sPnt->rev = 0;
        sPnt->FmaxFlag = 0;
    }

    Uf = load_s32(ramPnt, progPnt->Pnt_UF_TEK);
    sPnt->Fmax = load_s32(ramPnt, progPnt->Pnt_F_start_scan);
    start = testBitMicro(ramPnt, progPnt->Pnt_Start);

    if (sPnt->Pick_Up_OK && start > 0){
        start = 0;
    }else{
        sPnt->Pick_Up_OK = 0;
    }

    if (load_s16(ramPnt, progPnt->Pnt_EN)){
        if (start){
            clrBitMicro(ramPnt, progPnt->Pnt_UnHold_GT);
            if (sPnt->i < 10){
                sPnt->i++;
                if (sPnt->i == 1)
                    sPnt->Iold = _M_IREACT;
            }else{
                sPnt->dI = (s16)(_IQtoF(sPnt->Iold - _M_IREACT)*((u16)load_s16(ramPnt, progPnt->Pnt_K_di_dT)));
                sPnt->i =0;
            }
            if (sPnt->nasischenie != 0){
                sPnt->Uout = load_s32(ramPnt, progPnt->Pnt_U_scan);
                sPnt->Fout = sPnt->Fmax;
                sPnt->Fout *= (s32)sPnt->sign;
                sPnt->nasischenie--;
            }else{
                if (_IQabs(sPnt->Fout) > 0 && sPnt->F_seek_OK == 0 && sPnt->dI <= 0){
                    sPnt->Fout = _IQabs(sPnt->Fout);
                    add = _IQ(_IQtoF(sPnt->Fmax)/(f32)(load_s16(ramPnt, progPnt->Pnt_Temp_scan_F) * 250.0));
                    if (sPnt->Fout > add)
                        sPnt->Fout -= add;
                    else
                        sPnt->Fout = 0;
                    sPnt->Fout *= (s32)sPnt->sign;
                    sPnt->Ud_control = _M_Udc;
                }else{
                    if (sPnt->Fout != 0 || sPnt->rev == 1){
                        if (sPnt->Uout < Uf){
                            if (((_M_UdcFAST - sPnt->Ud_control) < load_s32(ramPnt, progPnt->Pnt_d_Ud) )||(sPnt->FmaxFlag)){
                                sPnt->Uout += _IQ(1.0/(f32)(load_s16(ramPnt, progPnt->Pnt_Temp_nar_U) * 2.5));
                            }else{
                                sPnt->Fout  = _IQabs(sPnt->Fout);
                                if (sPnt->Fout < sPnt->Fmax){
                                    sPnt->Fout += _IQ(_IQtoF(sPnt->Fmax)/(f32)(load_s16(ramPnt, progPnt->Pnt_Temp_pods_F) * 250.0));
                                    sPnt->FmaxFlag = 0;
                                }
                                else{
                                    sPnt->FmaxFlag = 1;
                                }
                                sPnt->Fout *= (s32)sPnt->sign;
                            }
                        }else{
                            sPnt->Pick_Up_OK = 1;
//                            sPnt->rev = 0;
                        }
                        sPnt->F_seek_OK = 1;
                    }else{
                        sPnt->rev++;
                        sPnt->sign = - sPnt->sign;
                        sPnt->nasischenie = load_s16(ramPnt, progPnt->Pnt_T_nasischen) * 250;
                        sPnt->Uout = 0;
                    }
                }
            }
            
            save_s32(ramPnt, progPnt->Pnt_F_P_UP, sPnt->Fout);
            save_s32(ramPnt, progPnt->Pnt_U_P_UP, sPnt->Uout);
        }else{
            sPnt->FmaxFlag = 0;
            sPnt->rev = 0;
            sPnt->sign = mPnt3->signRef/_IQabs(mPnt3->signRef);
            sPnt->F_seek_OK = 0;
            sPnt->nasischenie = load_s16(ramPnt, progPnt->Pnt_T_nasischen) * 250;
            setBitMicro(ramPnt, progPnt->Pnt_UnHold_GT);
        }
    }
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPICK_UP_1[15]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Start
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  T_nasischen
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Temp_scan_F
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Temp_pods_F
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Temp_nar_U
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  U_scan
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  d_Ud
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  F_start_scan
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  K_di/dT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  UF_TEK
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  EN
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Fail
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //13  U_P_UP
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //14  F_P_UP
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //15  Hold_GT
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PICK_UP", "DA", TblFuncPICK_UP_1, FuncPICK_UP_1, sizeof(TblFuncPICK_UP_1), sizeof(PICK_UP_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPICK_UP_1,
#endif
//********************************************************
