#ifdef    GET_FUNC_CODE
typedef struct{
    union{
    _iq     InternalRef;
    f32     fInternalRef;
    };
}TempLinState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_TempUp;                       //2  TempUp
    u16  Pnt_TempDw;                       //3  TempDw
    u16  Pnt_ResVal;                       //4  ResVal
    u16  Pnt_Freze;                        //5  Freze
    u16  Pnt_Res;                          //6  Res
    u16  Pnt_Status;                       //7  Status
    u16  Pnt_Out;                          //8  Out
    u16  Pnt_End;
}FuncTEMP_LIN_type;
//********************************************************
// Перезагрузка S32
//********************************************************
u16 *FuncTEMP_LIN_1(FuncTEMP_LIN_type *progPnt, u32 ramPnt)
{
    TempLinState_type   *sPnt;
    _iq                 TempUp, TempDw;
    _iq                 Ref, delta, diff;
    s16                 Freze, StateRefFlags;
    
    sPnt = (TempLinState_type *)(ramPnt + progPnt->Pnt_State);
    TempUp = (_iq)load_s32(ramPnt, progPnt->Pnt_TempUp);
    TempUp = _IQmpy(TempUp, _IQ(FLOAT_TIME_CALL_MICRO));

    TempDw = (_iq)load_s32(ramPnt, progPnt->Pnt_TempDw);
    TempDw = _IQmpy(TempDw, _IQ(FLOAT_TIME_CALL_MICRO));
    if(TempUp == 0)     TempUp = 1;
    if(TempDw == 0)     TempDw = 1;
    
    StateRefFlags = 0;
    Ref   = (_iq)load_s32(ramPnt, progPnt->Pnt_In);
    if (Ref<0)      StateRefFlags |= SRFLAG_REVREF;
    
    Freze = testBitMicro(ramPnt, progPnt->Pnt_Freze);
    if(!testBitMicro(ramPnt, progPnt->Pnt_Res)){
        sPnt->InternalRef = load_s32(ramPnt, progPnt->Pnt_ResVal);
        StateRefFlags |= SRFLAG_RESET;
    }
    else if(Freze != 0){
        StateRefFlags |= SRFLAG_FREZE;
    }
    else{
        diff = Ref - sPnt->InternalRef;
        
        if(sPnt->InternalRef >= 0)  delta = (diff < 0) ? -TempDw : TempUp;
        else{
            delta = (diff < 0) ? -TempUp : TempDw;
            StateRefFlags |= SRFLAG_REVCUR;
        }
        
        if(_IQabs(diff) < _IQabs(delta)){
            sPnt->InternalRef = Ref;
        }
        else{
            sPnt->InternalRef += delta;
        }
        
        if(diff != 0){
            if(delta<0) StateRefFlags |= SRFLAG_DOWN;
            if(delta>0) StateRefFlags |= SRFLAG_UP;
        }
    }
    
    if(sPnt->InternalRef != 0)      StateRefFlags |= SRFLAG_NONZERO;
    
    save_s32(ramPnt, progPnt->Pnt_Out, sPnt->InternalRef);
    save_s16(ramPnt, progPnt->Pnt_Status, StateRefFlags);


    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_LIN_1[8]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  TempUp
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  TempDw
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  ResVal
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Freze
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Res
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //7  Status  
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //9  Out     
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("TEMP_LIN", "DA", TblFuncTEMP_LIN_1, FuncTEMP_LIN_1, sizeof(TblFuncTEMP_LIN_1), sizeof(TempLinState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_LIN_1,
#endif
//********************************************************
#ifdef    GET_FUNC_CODE

//********************************************************
// Перезагрузка s16
//********************************************************
u16 *FuncTEMP_LIN_2(FuncTEMP_LIN_type *progPnt, u32 ramPnt)
{
    TempLinState_type   *sPnt;
    f32                 TempUp, TempDw, diff, delta;
    f32                 Ref;
    s16                 Out;
    s16                 Freze, StateRefFlags;
    
    sPnt = (TempLinState_type *)(ramPnt + progPnt->Pnt_State);
    TempUp = load_s16(ramPnt, progPnt->Pnt_TempUp);
    TempUp *= FLOAT_TIME_CALL_MICRO;

    TempDw = load_s16(ramPnt, progPnt->Pnt_TempDw);
    TempDw *= FLOAT_TIME_CALL_MICRO;

    if(TempUp == 0.)    TempUp = .001F;
    if(TempDw == 0.)    TempDw = .001F;
    
    StateRefFlags = 0;
    Ref   = load_s16(ramPnt, progPnt->Pnt_In);
    if (Ref<0)      StateRefFlags |= SRFLAG_REVREF;
    
    Freze = testBitMicro(ramPnt, progPnt->Pnt_Freze);
    if(!testBitMicro(ramPnt, progPnt->Pnt_Res)){
        sPnt->fInternalRef = load_s16(ramPnt, progPnt->Pnt_ResVal);
        StateRefFlags |= SRFLAG_RESET;
    }
    else if(Freze != 0){
        StateRefFlags |= SRFLAG_FREZE;
    }
    else{
        diff = Ref - sPnt->fInternalRef;
        
        if(sPnt->fInternalRef >= 0) delta = (diff < 0) ? -TempDw : TempUp;
        else{
            delta = (diff < 0) ? -TempUp : TempDw;
            StateRefFlags |= SRFLAG_REVCUR;
        }
        
        if(fabs(diff) < fabs(delta)){
            sPnt->fInternalRef = Ref;
        }
        else{
            sPnt->fInternalRef += delta;
        }
        
        if(diff != 0){
            if(delta<0) StateRefFlags |= SRFLAG_DOWN;
            if(delta>0) StateRefFlags |= SRFLAG_UP;
        }
    }
    
    if(sPnt->fInternalRef != 0)     StateRefFlags |= SRFLAG_NONZERO;
    Out = (s16)sPnt->fInternalRef;
    save_s16(ramPnt, progPnt->Pnt_Out, Out);
    save_s16(ramPnt, progPnt->Pnt_Status, StateRefFlags);


    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_LIN_2[8]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  TempUp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TempDw
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  ResVal
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Freze
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Res
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //7  Status  
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //9  Out     
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("TEMP_LIN", "DA", TblFuncTEMP_LIN_2, FuncTEMP_LIN_2, sizeof(TblFuncTEMP_LIN_2), sizeof(TempLinState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_LIN_2,
#endif
//********************************************************
