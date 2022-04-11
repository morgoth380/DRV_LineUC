#ifdef    GET_FUNC_CODE
#define MK_TIMER_TIME M3_FREQ/1000000 // Коэффициент приведения тактов микроконтроллера к микросекундам
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Time7;                        //1  Time7
    u16  Pnt_Time6;                        //2  Time6
    u16  Pnt_Time5;                        //3  Time5
    u16  Pnt_Time4;                        //4  Time4
    u16  Pnt_Time3;                        //5  Time3
    u16  Pnt_Time2;                        //6  Time2
    u16  Pnt_Time1;                        //7  Time1
    u16  Pnt_Time0;                        //8  Time0
    u16  Pnt_End;
}FuncMK_TIME_type;
//********************************************************
u16 *FuncMK_TIME_1(FuncMK_TIME_type *progPnt, u32 ramPnt)
{
    u32 tmp = 0;
    COMMONPARAMM3 *mPnt;
    COMMONPARAMC28 *cPnt;
    mPnt = &GlobalM3;
    cPnt = &GlobalC28;

    if (flagIsFirstCall){
        mPnt->mk_Time_4_ms.Time_takt = 0;
        mPnt->mk_Time_111_us.Time_takt = 0;
        mPnt->mk_Time_4_ms.Time_takt_max = 0;
        mPnt->mk_Time_111_us.Time_takt_max = 0;
        mPnt->mk_Time_4_ms.Time_start_prev =0;
        mPnt->mk_Time_111_us.Time_start_prev =0;
        mPnt->mk_Time_4_ms.Time_stop = 0;
        mPnt->mk_Time_111_us.Time_stop = 0;
        mPnt->mk_Time_4_ms.Time_start = 0;
        mPnt->mk_Time_111_us.Time_start = 0;
        return &progPnt->Pnt_End;
    }

    if (mPnt->mk_Time_4_ms.Time_start_prev > mPnt->mk_Time_4_ms.Time_stop){
        tmp = (mPnt->mk_Time_4_ms.Time_start_prev - mPnt->mk_Time_4_ms.Time_stop);
        if (tmp > mPnt->mk_Time_4_ms.Time_takt_max)
                mPnt->mk_Time_4_ms.Time_takt_max = tmp;
        tmp /= MK_TIMER_TIME;
        save_s16(ramPnt, progPnt->Pnt_Time0, (s16)tmp);
        tmp = mPnt->mk_Time_4_ms.Time_takt_max;
        tmp /= MK_TIMER_TIME;
        save_s16(ramPnt, progPnt->Pnt_Time1, (s16)tmp);
    }
    mPnt->mk_Time_4_ms.Time_start_prev = mPnt->mk_Time_4_ms.Time_start;
    tmp = mPnt->mk_Time_111_us.Time_takt;
    tmp /= MK_TIMER_TIME;
    save_s16(ramPnt, progPnt->Pnt_Time2, (s16)tmp);
    tmp = mPnt->mk_Time_111_us.Time_takt_max;
    tmp /= MK_TIMER_TIME;
    save_s16(ramPnt, progPnt->Pnt_Time3, (s16)tmp);
    tmp = cPnt->mk_Time_PWM.Time_takt;
    tmp /= MK_TIMER_TIME;
    save_s16(ramPnt, progPnt->Pnt_Time4, (s16)tmp);
    tmp = cPnt->mk_Time_PWM.Time_takt_max;
    tmp /= MK_TIMER_TIME;
    save_s16(ramPnt, progPnt->Pnt_Time5, (s16)tmp);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMK_TIME_1[8]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //1  Time7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Time6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Time5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Time4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Time3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Time2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Time1
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //8  Time0
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MK_TIME", "DA", TblFuncMK_TIME_1, FuncMK_TIME_1, sizeof(TblFuncMK_TIME_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMK_TIME_1,
#endif
//********************************************************
