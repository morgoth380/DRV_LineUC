#ifdef    GET_FUNC_CODE

typedef struct{
    s32     InternalRef ;
    s32     delta       ;
    
    u16     Up          ;
    u16     Dw          ;
    
    u32     MUp         ;
    u32     MDown       ;
    u32     M2          ;
}TempLinS_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_TempUp;                       //2  TempUp
    u16  Pnt_TempDw;                       //3  TempDw
    u16  Pnt_ResValOut;                    //4  ResValOut
    u16  Pnt_Type;                         //5  Type
    u16  Pnt_Freze;                        //6  Freze
    u16  Pnt_Res;                          //7  Res
    u16  Pnt_Status;                       //8  Status
    u16  Pnt_Out_IQ16;                     //9  Out_IQ16
    u16  Pnt_Out;                          //10 Out
    u16  Pnt_End;
}FuncTEMP_LIN_S_type;
//********************************************************
//#define SRFLAG_UP         (1<<0)        //
//#define SRFLAG_DOWN       (1<<1)        //
//#define SRFLAG_REVCUR     (1<<2)        //
//#define SRFLAG_REVREF     (1<<3)        //
//#define SRFLAG_NONZERO    (1<<4)        //
//#define SRF_FORCEZ        (1<<8)
//********************************************************
void ReTemp(TempLinS_State_type *sPnt)
{
    u32     MUp, MDown, M2;

    MUp   = (u32)(((u64)INT_TIME_CALL_MICRO_uS*(u64)(1ULL<<24))/(2*1000000ULL))*sPnt->Up;        //выход в формате входа
    MDown = (u32)(((u64)INT_TIME_CALL_MICRO_uS*(u64)(1ULL<<24))/(2*1000000ULL))*sPnt->Dw;    //выход в формате входа
    
    M2 = (MUp < MDown) ?  MUp : MDown;
    M2 = (u32)(((u64)INT_TIME_CALL_MICRO_uS * (u64)sPnt->Dw * (u64)MDown )/(1000000ULL*100));

    sPnt->MUp   = MUp;
    sPnt->MDown = MDown;
    sPnt->M2    = M2;
}
//****************************************************************************
u16 *FuncTEMP_LIN_S_1(FuncTEMP_LIN_S_type *progPnt, u32 ramPnt)
{
    TempLinS_State_type *sPnt;
    s32                 dval, diff, dir2, Ref;
    s16                 Up, Dw, StateRefFlags;
    
    sPnt = (TempLinS_State_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    Up = load_s16(ramPnt, progPnt->Pnt_TempUp);
    Dw = load_s16(ramPnt, progPnt->Pnt_TempDw);
    if((Up != sPnt->Up)||(Dw != sPnt->Dw)){
        sPnt->Up = Up;
        sPnt->Dw = Dw;
        ReTemp(sPnt);
    }

    StateRefFlags = 0;
    Ref = load_s16(ramPnt, progPnt->Pnt_In);
    if (Ref<0)      StateRefFlags |= SRFLAG_REVREF;

    //if(!((CtrlWord & CW_ROUTZERO)&&(StatWord & SW_RDYREF)) && !(WorkState.Flags & WSF_FORCEZ)){
    if(testBitMicro(ramPnt, progPnt->Pnt_Res)){
        sPnt->InternalRef = sPnt->delta = 0;
    }
    //else if((CtrlWord & CW_RHOLD)||(WorkState.Flags & WSF_FORCEZ)) {
    else if(!load_s16(ramPnt, progPnt->Pnt_Freze)){
        diff=((Ref<<15) - sPnt->InternalRef);

        dval=(sPnt->delta+0x1000)>>13;
        dval=(dval*dval*2)/sPnt->M2;

        if (sPnt->InternalRef>=0)       dir2=(diff<0)?-(int)sPnt->MDown:(int)sPnt->MUp;
        else                            dir2=(diff<0)?-(int)sPnt->MUp  :(int)sPnt->MDown;

        if(load_s16(ramPnt, progPnt->Pnt_Type)){
            if ((labs(diff)>>16)<=dval) dir2=0;

            if ((dir2-sPnt->delta)>(int)sPnt->M2)
                sPnt->delta+=(sPnt->M2+1);
            else if ((dir2-sPnt->delta)<-(int)sPnt->M2)
                sPnt->delta-=(sPnt->M2+1);
            else
                sPnt->delta=dir2;
        }
        else sPnt->delta=dir2;

        if (abs(diff)<(1<<14)+abs(sPnt->delta>>8)) {
            sPnt->InternalRef=Ref<<15;
            sPnt->delta=0;
        }
        else
            sPnt->InternalRef+=(sPnt->delta>>8);

        if (sPnt->InternalRef<0) {
            StateRefFlags |= SRFLAG_REVCUR;
            dval=-sPnt->delta;
        }
        else dval=sPnt->delta;

        if (dval<0)
            StateRefFlags |= SRFLAG_DOWN;
        if (dval>0)
            StateRefFlags |= SRFLAG_UP;
    }

    if (sPnt->InternalRef != 0)
        StateRefFlags |= SRFLAG_NONZERO;

    save_s16(ramPnt, progPnt->Pnt_Out, (s16)(sPnt->InternalRef>>15));
    save_s16(ramPnt, progPnt->Pnt_Status, 0);
    save_s32(ramPnt, progPnt->Pnt_Out_IQ16, sPnt->InternalRef);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_LIN_S_1[10]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  TempUp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TempDw
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  ResValOut
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Freze
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Res
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //8  Status  
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //9  Out_IQ16
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //10 Out     
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("TEMP_LIN/S", "DA", TblFuncTEMP_LIN_S_1, FuncTEMP_LIN_S_1, sizeof(TblFuncTEMP_LIN_S_1), sizeof(TempLinS_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_LIN_S_1,
#endif
//********************************************************
