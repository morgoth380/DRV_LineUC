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
    _iq         Fin, mFin, Fout, Flow1, Fhigh1, Flow2, Fhigh2, Flow3, Fhigh3, Flow4, Fhigh4, Flow5, Fhigh5;
    
    Fin     = load_s32(ramPnt, progPnt->Pnt_Fin);       // Частота на входе
    Fout    = load_s32(ramPnt, progPnt->Pnt_Fout);      // Предыдущая частота на выходе
    
    clrBitMicro(ramPnt, progPnt->Pnt_Forbidden);

    if(load_s16(ramPnt, progPnt->Pnt_On) == 0){
    	save_s32(ramPnt, progPnt->Pnt_Fout, Fin);
    	return &progPnt->Pnt_End;
    }else{
        if(Fin <= _IQ(0.0)){
            Fin  = _IQabs(Fin);
            Fout = _IQabs(Fout);
            mFin = _IQ(-1.0f);
        }else{
            mFin = _IQ(1.0f);
        }

        Flow1  = load_s32(ramPnt, progPnt->Pnt_F1_low );
        Fhigh1 = load_s32(ramPnt, progPnt->Pnt_F1_high);
        Flow2  = load_s32(ramPnt, progPnt->Pnt_F2_low );
        Fhigh2 = load_s32(ramPnt, progPnt->Pnt_F2_high);
        Flow3  = load_s32(ramPnt, progPnt->Pnt_F3_low );
        Fhigh3 = load_s32(ramPnt, progPnt->Pnt_F3_high);
        Flow4  = load_s32(ramPnt, progPnt->Pnt_F4_low );
        Fhigh4 = load_s32(ramPnt, progPnt->Pnt_F4_high);
        Flow5  = load_s32(ramPnt, progPnt->Pnt_F5_low );
        Fhigh5 = load_s32(ramPnt, progPnt->Pnt_F5_high);

        if(Fhigh1 < Flow1 ) Fhigh1 = Flow1;
        if(Flow2  < Flow1 ) Flow2  = Fhigh1;
        if(Flow2  < Fhigh1) Flow2  = Fhigh1;
        if(Fhigh2 < Flow2 ) Fhigh2 = Flow2;
        if(Flow3  < Flow2 ) Flow3  = Fhigh2;
        if(Flow3  < Fhigh2) Flow3  = Fhigh2;
        if(Fhigh3 < Flow3 ) Fhigh3 = Flow3;
        if(Flow4  < Flow3 ) Flow4  = Fhigh3;
        if(Flow4  < Fhigh3) Flow4  = Fhigh3;
        if(Fhigh4 < Flow4 ) Fhigh4 = Flow4;
        if(Flow5  < Flow4 ) Flow5  = Fhigh4;
        if(Flow5  < Fhigh4) Flow5  = Fhigh4;
        if(Fhigh5 < Flow5 ) Fhigh5 = Flow5;




        if (Fin <= Flow1){Fout = Fin;}
        else{
            if (Fin < Fhigh1){
                if (Fout >= Fhigh1) Fout = Fhigh1;
                else Fout = Flow1;
            }
            else{
                if (Fin <= Flow2){Fout = Fin;}
                else{
                    if (Fin < Fhigh2){
                        if (Fout >= Fhigh2) Fout = Fhigh2;
                        else Fout = Flow2;
                    }
                    else{
                        if (Fin <= Flow3){Fout = Fin;}
                        else{
                            if (Fin < Fhigh3){
                                if (Fout >= Fhigh3) Fout = Fhigh3;
                                else Fout = Flow3;
                            }
                            else{
                                if (Fin <= Flow4){Fout = Fin;}
                                else{
                                    if (Fin < Fhigh4){
                                        if (Fout >= Fhigh4) Fout = Fhigh4;
                                        else Fout = Flow4;
                                    }
                                    else{
                                        if (Fin <= Flow5){Fout = Fin;}
                                        else{
                                            if (Fin < Fhigh5){
                                                if (Fout >= Fhigh5) Fout = Fhigh5;
                                                else Fout = Flow5;
                                            }
                                            else {Fout = Fin;}
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Fout = _IQmpy(Fout, mFin);
        Fin = _IQmpy(Fin, mFin);
        if(testBitMicro(ramPnt, progPnt->Pnt_Res) == 0){
            save_s32(ramPnt, progPnt->Pnt_Fout, Fin );
        }else{
            save_s32(ramPnt, progPnt->Pnt_Fout, Fout);
        }
        save_s32(ramPnt, progPnt->Pnt_F1_low , Flow1 );
        save_s32(ramPnt, progPnt->Pnt_F1_high, Fhigh1);
        save_s32(ramPnt, progPnt->Pnt_F2_low , Flow2 );
        save_s32(ramPnt, progPnt->Pnt_F2_high, Fhigh2);
        save_s32(ramPnt, progPnt->Pnt_F3_low , Flow3 );
        save_s32(ramPnt, progPnt->Pnt_F3_high, Fhigh3);
        save_s32(ramPnt, progPnt->Pnt_F4_low , Flow4 );
        save_s32(ramPnt, progPnt->Pnt_F4_high, Fhigh4);
        save_s32(ramPnt, progPnt->Pnt_F5_low , Flow5 );
        save_s32(ramPnt, progPnt->Pnt_F5_high, Fhigh5);
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
