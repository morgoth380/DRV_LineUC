#ifdef    GET_FUNC_CODE

typedef struct{
    u32  time;
    u16  load_prev:1;
    u16  dc_pulse:1;
    u16  dc_pulse_prev:1;
    u16  RU_OK:1;
    u16  dry:1;
}CONST_CURRENT_1State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Time_STP;                     //1  Time_STP
    u16  Pnt_Time_DRY;                     //2  Time_DRY
    u16  Pnt_F_DCSTP;                      //3  F_DCSTP
    u16  Pnt_F_REF_DRV;                    //4  F_REF_DRV
    u16  Pnt_RU_EN;                        //5  RingUp_ENABLE
    u16  Pnt_DRY_EN;                       //6  DRY_EN
    u16  Pnt_PWM_DRV;                      //7  PWM_ON_DRV
    u16  Pnt_DRY_STRT_IMP;                 //8  DRY_STRT_IMP
    u16  Pnt_DC_STRT;                      //9  DC_STRT
    u16  Pnt_PICK_UP;                      //10  PickUp
    u16  Pnt_STP_IMP;                      //11  STP_IMP
    u16  Pnt_NPWM;                         //12  NPWM
    u16  Pnt_PWM_FAIL;                     //13  PWM_FAIL
    u16  Pnt_STOP_ALL;                     //14  STOP_ALL
    u16  Pnt_DRY_STRT;                     //15  DRY_STRT
    u16  Pnt_STATUS_DC;                    //16  STATUS_DC
    u16  Pnt_FAIL_W;                       //17  FAIL_W
    u16  Pnt_FAIL_V;                       //18  FAIL_V
    u16  Pnt_FAIL_U;                       //19  FAIL_U
    u16  Pnt_STP_PULSE;                    //20  STP_PULSE
    u16  Pnt_DC_STOP;                      //21  DC_STOP
    u16  Pnt_End;
}FuncCONST_CURRENT_type;
//********************************************************
u16 *FuncCONST_CURRENT_1(FuncCONST_CURRENT_type *progPnt, u32 ramPnt)
{
    u16 load, stopping_DC, state_DC, drying_or_ring_up, ring_up;
    
    COMMONPARAMM3 *mPnt;
    COMMONPARAMC28 *cPnt;
    CONST_CURRENT_1State_type             *sPnt;
    cPnt = &GlobalC28;
    mPnt = &GlobalM3;
    sPnt        = (CONST_CURRENT_1State_type *)(ramPnt + progPnt->Pnt_State);

// Включение сушки++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (testBitMicro(ramPnt, progPnt->Pnt_DRY_STRT_IMP) == 1){
        sPnt->dry = 1;
    }
    if ((testBitMicro(ramPnt, progPnt->Pnt_STP_PULSE) == 1) || (load_s16(ramPnt, progPnt->Pnt_DRY_EN) == 0) ||
            (testBitMicro(ramPnt, progPnt->Pnt_PICK_UP) == 1) || (load_s32(ramPnt, progPnt->Pnt_F_REF_DRV) > 0) ||
            (testBitMicro(ramPnt, progPnt->Pnt_PWM_FAIL) == 1) || (testBitMicro(ramPnt, progPnt->Pnt_STOP_ALL) == 1)){
        sPnt->dry = 0;
    }
    defBitMicro(ramPnt, progPnt->Pnt_DRY_STRT, sPnt->dry);

// Включение прозвонки или сушки++++++++++++++++++++++++++++++++++++++++++++++++
    if ((testBitMicro(ramPnt, progPnt->Pnt_PICK_UP) == 0) && (testBitMicro(ramPnt, progPnt->Pnt_PWM_DRV) == 1) &&
            (load_s16(ramPnt, progPnt->Pnt_RU_EN) == 1) && (testBitMicro(ramPnt, progPnt->Pnt_DC_STRT) == 0) 
            //&&(testBitMicro(ramPnt, progPnt->Pnt_DRY_STRT) == 0)
            ){
        ring_up = 1;
    }else{
        ring_up = 0;
    }
    if ((ring_up == 1) || (sPnt->dry == 1)){
        drying_or_ring_up = 1;
    }else{
        drying_or_ring_up = 0;
    }

// Торможение+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if ((load_s32(ramPnt, progPnt->Pnt_F_REF_DRV) <= load_s32(ramPnt, progPnt->Pnt_F_DCSTP)) && testBitMicro(ramPnt, progPnt->Pnt_DC_STRT)){
        stopping_DC = 1;
    }else{
        stopping_DC = 0;
    }
    
    if (drying_or_ring_up){
        load = 1;
    }else{
        load = stopping_DC;
    }
// Статус+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    state_DC = 0;
    if (ring_up){
    	state_DC = (1<<0);
    }
    if (sPnt->dry == 1){
        if (load_s16(ramPnt, progPnt->Pnt_RU_EN) == 1){
            if (sPnt->RU_OK){
                state_DC = (1<<1);
            }
        }else{
            state_DC = (1<<1);
        }
    }
    if (stopping_DC){
    	state_DC = (1<<2);
    }
    save_s16(ramPnt, progPnt->Pnt_STATUS_DC, state_DC);

// Прозвонка++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (ring_up && (mPnt->RUState == 0) && (mPnt->Fref == 0)){
        if (mPnt->RingUp == 0){
            mPnt->Angle = _IQ(0.0);
        }
        mPnt->RingUp = 1;
//        sPnt->RU_OK = 0;
    }else{
        if (mPnt->RUState == 4){
            sPnt->RU_OK = 1;
            mPnt->RingUp = 0;
        }/*else{
            sPnt->RU_OK = 0;
        }*/
        if (mPnt->RUState == 1){
            setBitMicro(ramPnt, progPnt->Pnt_FAIL_U);
        }else
            if (mPnt->RUState == 2){
                setBitMicro(ramPnt, progPnt->Pnt_FAIL_V);
            }else
                if (mPnt->RUState == 3){
                    setBitMicro(ramPnt, progPnt->Pnt_FAIL_W);
                }
    }
    if (!ring_up){
        if(testBitMicro(ramPnt, progPnt->Pnt_NPWM)){
            sPnt->RU_OK = 0;
            mPnt->RUState = 0;
            mPnt->AngCount = 0;
            mPnt->RingUp = 0;
        }
    }else{
    	mPnt->AngCount = cPnt->cAngCount;
    	mPnt->RUState = cPnt->cRUState;
    }

// времена++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    if (sPnt->RU_OK || testBitMicro(ramPnt, progPnt->Pnt_STP_IMP) || testBitMicro(ramPnt, progPnt->Pnt_NPWM)){
        clrBitMicro(ramPnt, progPnt->Pnt_DC_STOP);
        if ((!sPnt->dc_pulse_prev) && sPnt->dc_pulse){
            setBitMicro(ramPnt, progPnt->Pnt_STP_PULSE);
        }else{
            clrBitMicro(ramPnt, progPnt->Pnt_STP_PULSE);
        }
        sPnt->dc_pulse = 1;
        sPnt->dc_pulse_prev = sPnt->dc_pulse;
        sPnt->load_prev = 0;
        sPnt->time = 0;
    }else{
        if ((!sPnt->load_prev) && load){
            if (sPnt->dry == 0){
                sPnt->time = (u32)load_s16(ramPnt, progPnt->Pnt_Time_STP) * K_TIME_CALL_MICRO_TAKT;
            }else{
                sPnt->time = (u32)load_s16(ramPnt, progPnt->Pnt_Time_DRY) * K_TIME_CALL_MICRO_TAKT;
            }
        }
        sPnt->load_prev = load;
        if (sPnt->time > 0){
            setBitMicro(ramPnt, progPnt->Pnt_DC_STOP);
            sPnt->dc_pulse = 0;
            sPnt->dc_pulse_prev = 0;
            if (!ring_up){
                sPnt->time--;
            }
        }else{
            clrBitMicro(ramPnt, progPnt->Pnt_DC_STOP);
            if (!sPnt->RU_OK){
                if (sPnt->dc_pulse == 0){
                    setBitMicro(ramPnt, progPnt->Pnt_STP_PULSE);
                    sPnt->dc_pulse = 1;
                }
            }
        }
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCONST_CURRENT_1[21]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Time_STP
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Time_DRY
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  F_DCSTP
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F_REF_DRV
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  RingUp_ENABLE
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  DRY_EN
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  PWM_ON_DRV
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  DRY_STRT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  DC_STRT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  PickUp
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  STP_IMP
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  NPWM
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  PWM_FAIL
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  STOP_ALL
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //15  DRY_STRT
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  STATUS_DC
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //17  FAIL_W
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //18  FAIL_V
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //19  FAIL_U
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //20  STP_PULSE
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //21  DC_STOP
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONST_CURRENT", "DA", TblFuncCONST_CURRENT_1, FuncCONST_CURRENT_1, sizeof(TblFuncCONST_CURRENT_1), sizeof(CONST_CURRENT_1State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONST_CURRENT_1,
#endif
//********************************************************
