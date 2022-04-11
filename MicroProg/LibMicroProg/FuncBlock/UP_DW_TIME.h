#ifdef    GET_FUNC_CODE

typedef struct{
    s16     timerClr;
    s16     State;
    _iq16   InternalRef;
}UpDwState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_Up;                           //1  Up
    u16  Pnt_Down;                         //2  Down
    u16  Pnt_Time;                         //3  Time
    u16  Pnt_Max;                          //4  Max
    u16  Pnt_Min;                          //5  Min
    u16  Pnt_Out;                          //6  Out
    u16  Pnt_End;
}FuncUP_DW_TIME_type;
//********************************************************
u16 *FuncUP_DW_TIME_1(FuncUP_DW_TIME_type *progPnt, u32 ramPnt)
{
    UpDwState_type  *sPnt;
    s16             State;
    _iq16           diff, Ref, Temp;
    
    sPnt = (UpDwState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    if(sPnt->timerClr > (500000U / (u32)INT_TIME_CALL_MICRO_uS)){        //раз в 500mS
        sPnt->State = 0;
    }
    
    if(testBitMicro(ramPnt, progPnt->Pnt_Up))           State = (1<<0);
    else if(testBitMicro(ramPnt, progPnt->Pnt_Down))    State = (1<<1);
    else{
        State = 0;
        sPnt->timerClr++;
    }
    if(State){
        sPnt->timerClr = 0;
        sPnt->State = State;
        if(State &(1<<0))   State = progPnt->Pnt_Max;
        else                State = progPnt->Pnt_Min;
        Ref = _IQ16( load_s16(ramPnt, State) );
        diff = Ref - sPnt->InternalRef;        
        Temp = _IQ16( load_s16(ramPnt, progPnt->Pnt_Time) ) * _IQ16(0.1) * _IQ16(FLOAT_TIME_CALL_MICRO);
        Temp = (diff < 0) ? -Temp : Temp;
        
        if(abs(diff) < (_IQ16(0.5) + abs(Temp))){
            sPnt->InternalRef = Ref;
        }
        else{
            sPnt->InternalRef += Temp;
        }
    }
    save_s16(ramPnt, progPnt->Pnt_Out, (s16)(sPnt->InternalRef >>16));
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUP_DW_TIME_1[6]={
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Up
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Down
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Time
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Min
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //6  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("UP_DW_TIME", "DA", TblFuncUP_DW_TIME_1, FuncUP_DW_TIME_1, sizeof(TblFuncUP_DW_TIME_1), sizeof(UpDwState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUP_DW_TIME_1,
#endif
//********************************************************
