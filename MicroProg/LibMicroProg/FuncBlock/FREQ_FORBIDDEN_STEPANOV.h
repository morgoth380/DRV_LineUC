#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
//    u16  Pnt_State;
    u16  Pnt_Fin;                          //1  Fin
    u16  Pnt_On;                           //2  On
    u16  Pnt_F1_low;                       //3  F1_low
    u16  Pnt_F1_high;                      //4  F1_high
    u16  Pnt_F2_low;                       //5  F2_low
    u16  Pnt_F2_high;                      //6  F2_high
    u16  Pnt_F3_low;                       //7  F3_low
    u16  Pnt_F3_high;                      //8  F3_high
    u16  Pnt_F4_low;                       //9  F4_low
    u16  Pnt_F4_high;                      //10  F4_high
    u16  Pnt_F5_low;                       //11  F5_low
    u16  Pnt_F5_high;                      //12  F5_high
    u16  Pnt_Res;                          //13  Res
    u16  Pnt_Forbidden;                    //13  Forbidden
    u16  Pnt_Fout;                         //14  Fout
    u16  Pnt_End;
}FuncFREQ_FORBIDDEN_type;
//********************************************************
// On  - уставка вкл/откл
// Res - сброс при останове
//********************************************************
u16 *FuncFREQ_FORBIDDEN_1(FuncFREQ_FORBIDDEN_type *progPnt, u32 ramPnt)
{
    _iq         Fin, mFin, Fout, Flow, Fhigh;
    
    Fin     = load_s32(ramPnt, progPnt->Pnt_Fin);       // Частота на входе
    Fout    = load_s32(ramPnt, progPnt->Pnt_Fout);      // Предыдущая частота на выходе
    
    clrBitMicro(ramPnt, progPnt->Pnt_Forbidden);

    if( (load_s16(ramPnt, progPnt->Pnt_On)      == 0)||
        (testBitMicro(ramPnt, progPnt->Pnt_Res) == 0))
        {
        save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        return &progPnt->Pnt_End;
    }

    if(Fin >= _IQ(0.)){
        if (Fin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F1_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (Fin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F1_high))){
            if( Fout <= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout, Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
        else if (Fin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F2_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (Fin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F2_high))){
            if( Fout <= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout, Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
        else if (Fin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F3_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (Fin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F3_high))){
            if( Fout <= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout, Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
        else if (Fin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F4_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (Fin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F4_high))){
            if( Fout <= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout, Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
        else if (Fin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F5_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (Fin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F5_high))){
            if( Fout <= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout, Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
    }
    else{
        mFin = -Fin;
        
        if (mFin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F1_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (mFin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F1_high))){
            Flow = -Flow;
            if( Fout >= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout,  Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, -Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
        else if (mFin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F2_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (mFin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F2_high))){
            Flow = -Flow;
            if( Fout >= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout,  Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, -Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
        else if (mFin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F3_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (mFin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F3_high))){
            Flow = -Flow;
            if( Fout >= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout,  Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, -Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
        else if (mFin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F4_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (mFin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F4_high))){
            Flow = -Flow;
            if( Fout >= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout,  Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, -Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
        else if (mFin <= (Flow = load_s32(ramPnt, progPnt->Pnt_F5_low))) {
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
        }
        else if (mFin <= (Fhigh = load_s32(ramPnt, progPnt->Pnt_F5_high))){
            Flow = -Flow;
            if( Fout >= Flow )  save_s32(ramPnt, progPnt->Pnt_Fout,  Flow);      // Идем снизу 
            else                save_s32(ramPnt, progPnt->Pnt_Fout, -Fhigh);     // Идем сверху
            setBitMicro(ramPnt, progPnt->Pnt_Forbidden);
        }
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFREQ_FORBIDDEN_1[15]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Fin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  On
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  F1_low
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F1_high
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F2_low
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  F2_high
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  F3_low
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  F3_high
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  F4_low
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  F4_high
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  F5_low
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  F5_high
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  Res
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14  Forbidden
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //15  Fout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew("FREQ_FORBIDDEN", "DA", TblFuncFREQ_FORBIDDEN_1, FuncFREQ_FORBIDDEN_1, sizeof(TblFuncFREQ_FORBIDDEN_1), 0, 0, 1, sizeof(FuncFREQ_FORBIDDEN_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFREQ_FORBIDDEN_1,
#endif
//********************************************************
