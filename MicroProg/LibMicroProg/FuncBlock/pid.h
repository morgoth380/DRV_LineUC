#ifdef    GET_FUNC_CODE

 typedef struct {
    s64     body        ;    //32.32 integral
    s64     diff        ;    //32.32 filtered error derivative

    u32     MKInt       ;
    u32     MKProp      ;
    u32     MKDif       ;
    u32     MVDif       ;
    
    s16     olderr      ;
    
    s16     FrmtKi      ;    //0    1
    s16     Ti          ;    //1    60000   mS/ S
    s16     Kp          ;    //0    10000   X.XX%
    s16     Td          ;    //0    10000   mS
    s16     KspdVDif    ;    //0    10000   X.XX%
}PidState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Ti;                           //2  Ti
    u16  Pnt_Kp;                           //3  Kp
    u16  Pnt_Td;                           //4  Td
    u16  Pnt_mS_S;                         //5  mS/S
    u16  Pnt_Res;                          //6  Res
    u16  Pnt_maxPid;                       //7  maxPid
    u16  Pnt_minPid;                       //8  minPid
    u16  Pnt_100;                          //9  100%
    u16  Pnt_Out;                          //10 Out
    u16  Pnt_End;
}FuncPID_type;
//********************************************************
void RePid(PidState_type *sPnt)
{
    static const u32 TablKTC[] = //KInt units
    {
        (u32)(0x100000000ULL*1000ULL/(u64)INT_TIME_CALL_MICRO_uS),        // ms
        (u32)(0x100000000ULL/(u64)INT_TIME_CALL_MICRO_uS)                // s
    };
    if (sPnt->Ti)   sPnt->MKInt = (u32)(TablKTC[sPnt->FrmtKi&1]/(u32)sPnt->Ti);
    else            sPnt->MKInt = 0;

    sPnt->MKProp = (((u32)sPnt->Kp)<<16)/100;
    if (sPnt->Td)   sPnt->MKDif = (u32)(TablKTC[0]/(u32)sPnt->Td);
    else            sPnt->MKDif = 0;
    sPnt->MVDif = (((u32)sPnt->KspdVDif)<<16)/100;
}
//****************************************************************************
u16 *FuncPID_1(FuncPID_type *progPnt, u32 ramPnt)
{
    PidState_type   *sPnt;
    s32             FrmtKi, Ti, Kp, Td, err;    //
    s32             res, mx;

    sPnt = (PidState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    if(testBitMicro(ramPnt, progPnt->Pnt_Res)){
        sPnt->diff = 0;
        sPnt->body = 0;
    }
    else{
        FrmtKi  = load_s16(ramPnt, progPnt->Pnt_mS_S);    //
        Ti      = load_s16(ramPnt, progPnt->Pnt_Ti);    //
        Kp      = load_s16(ramPnt, progPnt->Pnt_Kp);    //
        Td      = load_s16(ramPnt, progPnt->Pnt_Td);    //
        if((FrmtKi != sPnt->FrmtKi)||(Ti != sPnt->Ti)||(Kp != sPnt->Kp)||(Td != sPnt->Td)){
            RePid(sPnt);
        }
        err = load_s16(ramPnt, progPnt->Pnt_In);
        
        sPnt->diff += (((err - sPnt->olderr)*(s64)sPnt->MVDif)<<16)
                     - (sPnt->diff>>32)*sPnt->MKDif
                     - (((u64)(sPnt->diff&0xffffffff)*sPnt->MKDif)>>32);    // Differential lowpass filter
        sPnt->olderr = (s16)err;
        sPnt->body += (s64)err * sPnt->MKInt;        // Integral
        res = (s32)((sPnt->body + sPnt->diff + (((s64)err*sPnt->MKProp)<<16) +0x80000000UL)>>32);    // Int+Prop+Diff
    
        // Limiter
        mx = load_s16(ramPnt, progPnt->Pnt_minPid);
        if(res < mx)                res = mx;
        if((sPnt->body>>32) < mx)   sPnt->body = (s64)mx<<32;
    
        mx = load_s16(ramPnt, progPnt->Pnt_maxPid);
        if(res > mx)                res = mx;
        if(sPnt->body>>32 > mx)     sPnt->body = (s64)mx<<32;
    
        save_s16(ramPnt, progPnt->Pnt_Out, (s16)res);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPID_1[10]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Ti
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Kp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Td
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  mS/S
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Res
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  maxPid
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  minPid
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  100%
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //10  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("PID", "PID", TblFuncPID_1, FuncPID_1, sizeof(TblFuncPID_1), sizeof(FuncPID_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPID_1,
#endif
//********************************************************
