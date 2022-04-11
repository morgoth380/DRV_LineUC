#ifdef    GET_FUNC_CODE
typedef struct{
    f32     InternalRef ;
//  s32     delta       ;
//
//  u16     Up          ;
//  u16     Dw          ;
//
//  u32     MUp         ;
//  u32     MDown       ;
//  u32     M2          ;
}TempLinS_State_Time_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_TimeUp;                       //2  TimeUp
    u16  Pnt_TimeDn;                       //3  TimeDn
    u16  Pnt_TimeUp2;                      //4  TimeUp2
    u16  Pnt_TimeDn2;                      //5  TimeDn2
    u16  Pnt_WrNom;                        //6  WrNom
    u16  Pnt_WrMax;                        //7  WrMax
    u16  Pnt_ResValOut;                    //8  ResValOut
    u16  Pnt_Type;                         //9  Type
    u16  Pnt_Freze;                        //10  Freze
    u16  Pnt_Res;                          //11  Res
    u16  Pnt_Status;                       //12  Status
    u16  Pnt_Out_IQ16;                     //13  Out_IQ16
    u16  Pnt_Out;                          //14  Out
    u16  Pnt_End;
}FuncTEMP_LIN_S_TIME_type;
//********************************************************
u16 *FuncTEMP_LIN_S_TIME_1(FuncTEMP_LIN_S_TIME_type *progPnt, u32 ramPnt)
{
    TempLinS_State_Time_type    *sPnt;
    f32                 WrNom, WrMax, delta, Time;
    _iq                 In, Out;
//  s32                 dval, diff, dir2;

    _iq                 Ref;
//  s16                 In, Up, Dw, StateRefFlags;

    sPnt = (TempLinS_State_Time_type *)(ramPnt + progPnt->Pnt_State );

    if( testBitMicro(ramPnt, progPnt->Pnt_Res == 0 ){
        Ref = load_s16(ramPnt, progPnt->ResValOut);
        save_s32(ramPnt, progPnt->Pnt_Out, _IQ(0.) );
        sPnt->InternalRef = 0.;
        return &progPnt->Pnt_End;
    }

    In    = _IQtoF(load_s32(ramPnt, progPnt->Pnt_In));
    WrNom = _IQtoF(load_s32(ramPnt, progPnt->Pnt_WrNom));
    WrMax = _IQtoF(load_s32(ramPnt, progPnt->Pnt_WrMax));

// ================== Линейная характеристика =================    
// ================== Линейная характеристика =================    
    if( abs(sPnt->InternalRef) <= abs(WrNom) ){
        tmp  = abs(sPnt->InternalRef) < abs(In);
        if( tmp < 0 )){
            Time    = (f32)load_s16(ramPnt, progPnt->Pnt_TimeUp) / FLOAT_TIME_CALL_MICRO;
            delta   = WrNom / Time;
            if( abs(tmp) < delta){
                sPnt->InternalRef = In;
            }
            else{
                sPnt->InternalRef += delta;
            }
        }
        else{
            Time    = (f32)load_s16(ramPnt, progPnt->Pnt_TimeDn) / FLOAT_TIME_CALL_MICRO;
            sPnt->InternalRef -= delta;
            if()
        }
    }
    else if( abs(sPnt->InternalRef) < abs(WrMax) ){
        if( abs(sPnt->InternalRef) < abs(In))){
        }
        
   }
        
    save_s32(ramPnt, progPnt->Pnt_Out, _IQ( sPnt->InternalRef ) );

//  Up = load_s16(ramPnt, progPnt->Pnt_TempUp);
//  Dw = load_s16(ramPnt, progPnt->Pnt_TempDw);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_LIN_S_TIME_1[14]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  TimeUp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TimeDn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  TimeUp2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  TimeDn2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  WrNom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  WrMax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  ResValOut
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Type
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Freze
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  Res
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Status
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  Out_IQ16
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //14  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew
("TEMP_LIN_S_TIME", "DA", TblFuncTEMP_LIN_S_TIME_1, FuncTEMP_LIN_S_TIME_1, sizeof(TblFuncTEMP_LIN_S_TIME_1), sizeof(TempLinS_State_Time_type), 0, 1, sizeof(FuncTEMP_LIN_S_TIME_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_LIN_S_TIME_1,
#endif
//********************************************************
