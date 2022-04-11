#ifdef    GET_FUNC_CODE
typedef struct{
    _iq     F5;
    _iq     F4;
    _iq     F3;
    _iq     F2;
    _iq     F1;
    _iq     U5;
    _iq     U4;
    _iq     U3;
    _iq     U2;
    _iq     U1;
}UF_5_FILTRState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_F;                            //1  F
    u16  Pnt_F5;                           //2  F5
    u16  Pnt_F4;                           //3  F4
    u16  Pnt_F3;                           //4  F3
    u16  Pnt_F2;                           //5  F2
    u16  Pnt_F1;                           //6  F1
    u16  Pnt_U5;                           //7  U5
    u16  Pnt_U4;                           //8  U4
    u16  Pnt_U3;                           //9  U3
    u16  Pnt_U2;                           //10  U2
    u16  Pnt_U1;                           //11  U1
    u16  Pnt_Error;                        //12  Error
    u16  Pnt_U;                            //13  U
    u16  Pnt_Tfiltr;                       //14  Tfiltr
    u16  Pnt_End;
}FuncUF_5_FILTR_type;
//********************************************************
u16 *FuncUF_5_FILTR_1(FuncUF_5_FILTR_type *progPnt, u32 ramPnt)
{
    UF_5_FILTRState_type    *sPnt;
    s16                     Tfiltr;
    _iq                     F;
    
    sPnt = (UF_5_FILTRState_type *)(ramPnt + progPnt->Pnt_State);
    F =  load_s32(ramPnt, progPnt->Pnt_F);
    Tfiltr = load_s16(ramPnt, progPnt->Pnt_Tfiltr);
    if(flagIsFirstCall){            //начальная инициализация
        Tfiltr = 0;
    }

    sPnt->F1 = iq_Filtr_prm(sPnt->F1, load_s32(ramPnt, progPnt->Pnt_F1), Tfiltr);
    sPnt->F2 = iq_Filtr_prm(sPnt->F2, load_s32(ramPnt, progPnt->Pnt_F2), Tfiltr);
    sPnt->F3 = iq_Filtr_prm(sPnt->F3, load_s32(ramPnt, progPnt->Pnt_F3), Tfiltr);
    sPnt->F4 = iq_Filtr_prm(sPnt->F4, load_s32(ramPnt, progPnt->Pnt_F4), Tfiltr);
    sPnt->F5 = iq_Filtr_prm(sPnt->F5, load_s32(ramPnt, progPnt->Pnt_F5), Tfiltr);

    sPnt->U1 = iq_Filtr_prm(sPnt->U1, load_s32(ramPnt, progPnt->Pnt_U1), Tfiltr);
    sPnt->U2 = iq_Filtr_prm(sPnt->U2, load_s32(ramPnt, progPnt->Pnt_U2), Tfiltr);
    sPnt->U3 = iq_Filtr_prm(sPnt->U3, load_s32(ramPnt, progPnt->Pnt_U3), Tfiltr);
    sPnt->U4 = iq_Filtr_prm(sPnt->U4, load_s32(ramPnt, progPnt->Pnt_U4), Tfiltr);
    sPnt->U5 = iq_Filtr_prm(sPnt->U5, load_s32(ramPnt, progPnt->Pnt_U5), Tfiltr);
    
    if((sPnt->F5 < sPnt->F4)||(sPnt->F4 < sPnt->F3)||(sPnt->F3 < sPnt->F2)||(sPnt->F2 < sPnt->F1)){
        save_s32(ramPnt, progPnt->Pnt_U, 0);
        setBitMicro(ramPnt, progPnt->Pnt_Error);
    }
    else{
        if     (F > sPnt->F4)   F = iq_Fy_x1x2y1y2x(sPnt->F4, sPnt->F5, sPnt->U4, sPnt->U5, F);
        else if(F > sPnt->F3)   F = iq_Fy_x1x2y1y2x(sPnt->F3, sPnt->F4, sPnt->U3, sPnt->U4, F);
        else if(F > sPnt->F2)   F = iq_Fy_x1x2y1y2x(sPnt->F2, sPnt->F3, sPnt->U2, sPnt->U3, F);
        else if(F > sPnt->F1)   F = iq_Fy_x1x2y1y2x(sPnt->F1, sPnt->F2, sPnt->U1, sPnt->U2, F);
        else                    F = iq_Fy_x1x2y1y2x(0         , sPnt->F1, 0         , sPnt->U1, F);
        
        if(_IQabs(F) > _IQ(1.0))    F = _IQ(1.0);
        save_s32(ramPnt, progPnt->Pnt_U, F);
        clrBitMicro(ramPnt, progPnt->Pnt_Error);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUF_5_FILTR_1[14]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  F5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  F4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  F1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  U5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  U4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  U3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  U2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  U1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Error
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //13  U
        S16_PIN_TYPE | INPUT_PIN_MODE     //14  Tfiltr
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UF_5_FILTR", "DD", TblFuncUF_5_FILTR_1, FuncUF_5_FILTR_1, sizeof(TblFuncUF_5_FILTR_1), sizeof(UF_5_FILTRState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUF_5_FILTR_1,
#endif
//********************************************************
