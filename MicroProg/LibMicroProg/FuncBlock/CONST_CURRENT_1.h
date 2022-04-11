#ifdef    GET_FUNC_CODE

typedef struct{
    u32  time;
    u16  load_prev:1;
    u16  dc_pulse:1;
    u16  dc_pulse_prev:1;
}CONST_CURRENT_1State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Time_STP;                     //1  Time_STP
    u16  Pnt_Time_DRY;                     //2  Time_DRY
    u16  Pnt_DRY_STRT;                     //3  DRY_STRT
    u16  Pnt_DC_STRT;                      //4  DC_STRT
    u16  Pnt_RU_OK;                        //5  RU_OK
    u16  Pnt_STP_IMP;                      //6  STP_IMP
    u16  Pnt_NPWM;                         //7  NPWM
    u16  Pnt_STP_PULSE;                    //8  STP_PULSE
    u16  Pnt_DC_STOP;                      //9  DC_STOP
    u16  Pnt_End;
}FuncCONST_CURRENT_1_type;
//********************************************************
u16 *FuncCONST_CURRENT_1_1(FuncCONST_CURRENT_1_type *progPnt, u32 ramPnt)
{
    u16 load;
    CONST_CURRENT_1State_type             *sPnt;
    sPnt        = (CONST_CURRENT_1State_type *)(ramPnt + progPnt->Pnt_State);
    load = testBitMicro(ramPnt, progPnt->Pnt_DC_STRT);

    if (testBitMicro(ramPnt, progPnt->Pnt_RU_OK) || testBitMicro(ramPnt, progPnt->Pnt_STP_IMP) || testBitMicro(ramPnt, progPnt->Pnt_NPWM)){
        clrBitMicro(ramPnt, progPnt->Pnt_DC_STOP);
        if ((!sPnt->dc_pulse_prev) && sPnt->dc_pulse){
            setBitMicro(ramPnt, progPnt->Pnt_STP_PULSE);
        }else{
            clrBitMicro(ramPnt, progPnt->Pnt_STP_PULSE);
        }
        sPnt->dc_pulse = 1;
        sPnt->dc_pulse_prev = sPnt->dc_pulse;
        sPnt->load_prev = 0;
    }else{
        if ((!sPnt->load_prev) && load){
            if (testBitMicro(ramPnt, progPnt->Pnt_DRY_STRT) == 0){
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
            sPnt->time--;
        }else{
            clrBitMicro(ramPnt, progPnt->Pnt_DC_STOP);
            if (!testBitMicro(ramPnt, progPnt->Pnt_RU_OK)){
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
const char TblFuncCONST_CURRENT_1_1[9]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Time_STP
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Time_DRY
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  DRY_STRT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  DC_STRT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  RU_OK
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  STP_IMP
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  NPWM
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  STP_PULSE
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //9  DC_STOP
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONST_CURRENT_1", "DA", TblFuncCONST_CURRENT_1_1, FuncCONST_CURRENT_1_1, sizeof(TblFuncCONST_CURRENT_1_1), sizeof(CONST_CURRENT_1State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONST_CURRENT_1_1,
#endif
//********************************************************
