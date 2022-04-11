#ifdef    GET_FUNC_CODE

//#define   mS_TO_TicMicroProg(mS)    ((mS < INT_TIME_CALL_MICRO_mS)    ? 1 : _IQmpyI32int(_IQ(1/INT_TIME_CALL_MICRO_mS), (u32)mS))
//#define   S_TO_TicMicroProg(S)    
typedef struct{
        _iq     Iold;                //старое значение реактивного тока
        _iq     dI;                    //дельта тока
        _iq     Ud_control;        //разобраться
        _iq     Uout;                //задание выходного напряжения
        _iq     Fout;                //задание частоты
        _iq     Fmax;                //частота начала сканирования
        s32     sign;            //разобраться
        s16     timeDt;                //таймер дифференцирования

        s16     F_seek_OK;            //флаг синхронная частота найдена
        s16     Pick_Up_OK;            //флаг подхват осуществлён
        u16     nasischenie;    //Tover
        s16     rev;                //флаг надо начинать с -Fmax
        s16     k;                //разобраться
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
    u16  Pnt_F_MAX;                        //8  F_MAX
    u16  Pnt_F_start_scan;                 //9  F_start_scan
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
/*
    PICK_UP_State_type              *sPnt;
    u16                             start;
    _iq                             Uf, add;

    sPnt = (PICK_UP_State_type *)(ramPnt + progPnt->Pnt_State);

    if(flagIsFirstCall){
        sPnt->timeDt = 0;
        sPnt->k = 0;
        sPnt->nasischenie = load_s16(ramPnt, progPnt->Pnt_T_nasischen) * 250;        //S_TO_TicMicroProg
        sPnt->Pick_Up_OK = 0;
        setBitMicro(ramPnt, progPnt->Pnt_UnHold_GT);
        sPnt->Fout = 0;
        sPnt->Uout = 0;
        sPnt->rev = 0;
    }

    Uf = load_s32(ramPnt, progPnt->Pnt_UF_TEK);
    sPnt->Fmax = load_s32(ramPnt, progPnt->Pnt_F_MAX);
    start = testBitMicro(ramPnt, progPnt->Pnt_Start);

    if (sPnt->Pick_Up_OK && start > 0){
        start = 0;
    }else{
        sPnt->Pick_Up_OK = 0;
    }

    if (load_s16(ramPnt, progPnt->Pnt_EN)){
        if (start){
            clrBitMicro(ramPnt, progPnt->Pnt_UnHold_GT);
            if (sPnt->timeDt < 10){
                sPnt->timeDt++;
                if (sPnt->timeDt == 1)
                    sPnt->Iold = measMem.Ireact_out;
            }else{
                sPnt->dI = (s16)(_IQtoF(sPnt->Iold - measMem.Ireact_out)*((u16)load_s16(ramPnt, progPnt->Pnt_K_di_dT)));
                sPnt->timeDt =0;
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
                    sPnt->Ud_control = measMem.Ud;
                }else{
                    if (sPnt->Fout != 0 || load_s32(ramPnt, progPnt->Pnt_F_start_scan) == 0 || sPnt->rev == 1){
                        if (sPnt->Uout < Uf){
                            if ((measMem.UdFast - sPnt->Ud_control) < load_s32(ramPnt, progPnt->Pnt_d_Ud) ){
                                sPnt->Uout += _IQ(1.0/(f32)(load_s16(ramPnt, progPnt->Pnt_Temp_nar_U) * 2.5));
                            }else{
                                sPnt->Fout = _IQabs(sPnt->Fout);
                                sPnt->Fout += _IQ(_IQtoF(sPnt->Fmax)/(f32)(load_s16(ramPnt, progPnt->Pnt_Temp_pods_F) * 250.0));
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
            sPnt->rev = 0;
            sPnt->sign = signRef/_IQabs(signRef);
            sPnt->F_seek_OK = 0;
            sPnt->nasischenie = load_s16(ramPnt, progPnt->Pnt_T_nasischen) * 250;
            setBitMicro(ramPnt, progPnt->Pnt_UnHold_GT);
        }
    }
*/    
setBitMicro(ramPnt, progPnt->Pnt_UnHold_GT);        //убрать
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPICK_UP_1[16]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Start
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  T_nasischen
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Temp_scan_F
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Temp_pods_F
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Temp_nar_U
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  U_scan
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  d_Ud
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  F_MAX
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  F_start_scan
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  K_di/dT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  UF_TEK
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  EN
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //13  Fail
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //14  U_P_UP
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //15  F_P_UP
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //16  Hold_GT
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
