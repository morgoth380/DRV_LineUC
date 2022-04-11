#ifdef    GET_FUNC_CODE
typedef struct{
    _iq         angle;
}StateFuncDBG_ADD_SIN_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Inp;                          //1  Inp
    u16  Pnt_On_Off;                       //2  On_Off
    u16  Pnt_Amp;                          //3  Amp
    u16  Pnt_F;                            //4  F
    u16  Pnt_Out;                          //5  Out
    u16  Pnt_End;
}FuncDBG_ADD_SIN_type;
//********************************************************
//extern
s16                      On_Off_test, F_test;
//extern
_iq                      Amp_test;
//********************************************************



u16 *FuncDBG_ADD_SIN_1(FuncDBG_ADD_SIN_type *progPnt, u32 ramPnt)
{
/*    _iq                             step, amp;
    StateFuncDBG_ADD_SIN_type       *sPnt;
    f32                             tmp;
    
    sPnt = (StateFuncDBG_ADD_SIN_type *)(ramPnt + progPnt->Pnt_State);
*/

    On_Off_test     = load_s16(ramPnt, progPnt->Pnt_On_Off);
    if(On_Off_test){
        Amp_test        = load_s32(ramPnt, progPnt->Pnt_Amp);
        F_test          = load_s16(ramPnt, progPnt->Pnt_F);
    }
    else{
        Amp_test = 0;
        F_test   = 0;
    }
    
/*
    if(amp == 0){
        tmp = Wr_z;
        save_float(ramPnt, progPnt->Pnt_Out, tmp);    // Без изменения
        sPnt->angle  = _IQ(0.);
    }
    else{
        F = load_s16(ramPnt, progPnt->Pnt_F);
        step =( _IQ(1.0/2500.) * F);
        sPnt->angle += step;
        if(sPnt->angle >= _IQ(1.0)) sPnt->angle -= _IQ(1.0);
        tmp = Wr_z * _IQtoF(_IQ(1.0) + _IQmpy( amp, _IQsinPU(sPnt->angle)));
//        if(tmp>0) tmp += 0.5;            //Округление
//      Wr_z = tmp;
        save_float(ramPnt, progPnt->Pnt_Out, tmp);
    }
*/

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDBG_ADD_SIN_1[5]={
        S16_PIN_TYPE | INPUT_PIN_MODE,   //1  Inp
        S16_PIN_TYPE | INPUT_PIN_MODE,   //2  On_Off
        S32_PIN_TYPE | INPUT_PIN_MODE,   //3  Amp
        S16_PIN_TYPE | INPUT_PIN_MODE,   //4  F
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE   //5  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew("DBG_ADD_SIN", "DA", TblFuncDBG_ADD_SIN_1, FuncDBG_ADD_SIN_1, sizeof(TblFuncDBG_ADD_SIN_1), 
sizeof(StateFuncDBG_ADD_SIN_type), 0, 1, sizeof(FuncDBG_ADD_SIN_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDBG_ADD_SIN_1,
#endif
//********************************************************
