#ifdef    GET_FUNC_CODE

typedef enum{
    off_scc,
    currControl_scc,
    rise_scc,
    runUp_scc,
    fall_scc,
    runLow_scc,

    size_scc = 0x1234
}stateCC_type;

#define _IQ(A) ((long) ((A) * 16777216.0L))

typedef struct{
    stateCC_type    state;
    u16             cnt;
    u32             time;
    s32             fRegOut;
    f32             FminLimit;
}stateCURRENT_CONTROL_type;

// Время в секундах в тики микропрограммы
#define timeToMp( time ) ( ( (time) * (1000/(INT_TIME_CALL_MICRO_mS))) - 1 )

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Fin;                          //1  Fin
    u16  Pnt_Ifull;                        //2  Ifull
    u16  Pnt_off_on;                       //3  off/on
    u16  Pnt_cycleOn;                      //4  cycleOn
    u16  Pnt_Iref;                         //5  Iref
    u16  Pnt_Iup;                          //6  Iup
    u16  Pnt_Time_up;                      //7  Time_up
    u16  Pnt_Idown;                        //8  Idown
    u16  Pnt_Time_down;                    //9  Time_down
    u16  Pnt_numPulse;                     //10  numPulse
    u16  Pnt_Fmax;                         //11  Fmax
    u16  Pnt_Fmin;                         //12  Fmin
    u16  Pnt_Tacc;                         //13  Tacc
    u16  Pnt_Tdec;                         //14  Tdec
    u16  Pnt_TdecReg;                      //15  TaccReg
	u16  Pnt_TaccReg;                      //16  TdecReg
	u16  Pnt_RegEn;                        //17  RegEn
    u16  Pnt_Fout;                         //18  Fout
    u16  Pnt_End;
}FuncCURRENT_CONTROL_type;
//********************************************************
u16 *FuncCURRENT_CONTROL_1(FuncCURRENT_CONTROL_type *progPnt, u32 ramPnt)
{
    stateCURRENT_CONTROL_type   *sPnt;
    f32                         Ifull, Fref;
    f32                         Iref;
    s16			        RegTacc, RegTdec;
  
//    f32         iUp, iDown;
//    f32         fIup, fIdown;

    sPnt = (stateCURRENT_CONTROL_type *)(ramPnt + progPnt->Pnt_State);

    if (flagIsFirstCall) {
      save_s16(ramPnt, progPnt->Pnt_off_on, 0);
      save_s16(ramPnt, progPnt->Pnt_cycleOn, 0);
    }
        
    if( load_s16(ramPnt, progPnt->Pnt_off_on) == 0 ){
        sPnt->state = off_scc;
    }
    else if( !PwmIsOn() ){
        sPnt->state = off_scc;
    }

    Ifull   = load_float(ramPnt, progPnt->Pnt_Ifull);
    RegTacc = load_s16(ramPnt, progPnt->Pnt_Tacc);
    RegTdec = load_s16(ramPnt, progPnt->Pnt_Tdec);
    
//iUp     =  load_float(ramPnt, progPnt->Pnt_Iup);
//iDown   =  load_float(ramPnt, progPnt->Pnt_Idown);

    switch( sPnt->state ){
        case off_scc:
            Fref = load_float(ramPnt, progPnt->Pnt_Fin);
            save_float(ramPnt, progPnt->Pnt_Fout, Fref);

            if(PwmIsOn()){
                sPnt->FminLimit = 50.2f * 10.0f / MPOpis.Base[NUM_FREQ_BASE]; // Привяжем к частоте сети 50.0 Гц
                sPnt->state = currControl_scc;
            } else {
              clrBitMicro (ramPnt, progPnt->Pnt_RegEn); // регулятор отключен
            }

            return &progPnt->Pnt_End;                       //break;

        case currControl_scc:
            if( load_s16(ramPnt, progPnt->Pnt_cycleOn) != 0 ){
                sPnt->cnt   = load_s16(ramPnt, progPnt->Pnt_numPulse);
                sPnt->time  = timeToMp( load_s16(ramPnt, progPnt->Pnt_Time_up) );
                Iref        = load_float(ramPnt, progPnt->Pnt_Iup);

                sPnt->state = rise_scc;
            }
            else{
                Iref = load_float(ramPnt, progPnt->Pnt_Iref);
            }
            break;

        case rise_scc:
            Iref = load_float(ramPnt, progPnt->Pnt_Iup);
            if(Ifull > Iref){
                sPnt->state = runUp_scc;
            }
            break;

        case runUp_scc:
            Iref = load_float(ramPnt, progPnt->Pnt_Iup);
            if(sPnt->time != 0)
                { sPnt->time--; }
            else{
                sPnt->state = fall_scc;
            }
            break;

        case fall_scc:
            Iref = load_float(ramPnt, progPnt->Pnt_Idown);

            if(Ifull < Iref){
                sPnt->time  = timeToMp( load_s16(ramPnt, progPnt->Pnt_Time_down) );

                sPnt->state = runLow_scc;
            }
            break;

        case runLow_scc:
            Iref = load_float(ramPnt, progPnt->Pnt_Idown);

            if(sPnt->time != 0)
                { sPnt->time--; }
            else{
                if( sPnt->cnt != 1 ){
                    sPnt->cnt--;
                    sPnt->time  = timeToMp( load_s16(ramPnt, progPnt->Pnt_Time_up) );

                    sPnt->state = rise_scc;
                }
                else{
                    save_s16(ramPnt, progPnt->Pnt_cycleOn, 0);
                    sPnt->state = currControl_scc;
                }
            }
            break;
        default:
            sPnt->state = off_scc;
            break;
    }

    // Если я здесь, то регулятор работает. Заменяем время разгона/торможения
    save_s16(ramPnt, progPnt->Pnt_TaccReg, RegTacc);
    save_s16(ramPnt, progPnt->Pnt_TdecReg, RegTdec);
    setBitMicro (ramPnt, progPnt->Pnt_RegEn);
    
    // Собственно регулятор тока. Регулятор примитивного типа - релаксатор.
    if( Ifull >= Iref )
    { Fref = sPnt->FminLimit; }
    else
    { Fref = load_float(ramPnt, progPnt->Pnt_Fmax); }
    
    save_float(ramPnt, progPnt->Pnt_Fout, Fref);
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCURRENT_CONTROL_1[18]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Fin
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //2  Ifull
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  off/on
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  cycleOn
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Iref
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Iup
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Time_up
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Idown
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Time_down
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  numPulse
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  Fmax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  Fmin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Tacc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Tdec
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //15  TaccReg
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  TdecReg
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //17  RegEn
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //18  Fout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CURRENT_CONTROL", "DA", TblFuncCURRENT_CONTROL_1, FuncCURRENT_CONTROL_1, sizeof(TblFuncCURRENT_CONTROL_1), sizeof(stateCURRENT_CONTROL_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCURRENT_CONTROL_1,
#endif
//********************************************************
