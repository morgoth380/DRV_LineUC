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
}UF_5_FILTR_X2State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_F;                              //01 F
    u16  Pnt_F5_1;                           //02 F5
    u16  Pnt_F4_1;                           //03 F4
    u16  Pnt_F3_1;                           //04 F3
    u16  Pnt_F2_1;                           //05 F2
    u16  Pnt_F1_1;                           //06 F1
    u16  Pnt_U5_1;                           //07 U5
    u16  Pnt_U4_1;                           //08 U4
    u16  Pnt_U3_1;                           //09 U3
    u16  Pnt_U2_1;                           //10 U2
    u16  Pnt_U1_1;                           //11 U1
    u16  Pnt_F5_2;                           //12 F5
    u16  Pnt_F4_2;                           //13 F4
    u16  Pnt_F3_2;                           //14 F3
    u16  Pnt_F2_2;                           //15 F2
    u16  Pnt_F1_2;                           //16 F1
    u16  Pnt_U5_2;                           //17 U5
    u16  Pnt_U4_2;                           //18 U4
    u16  Pnt_U3_2;                           //19 U3
    u16  Pnt_U2_2;                           //20 U2
    u16  Pnt_U1_2;                           //21 U1
    u16  Pnt_Tfiltr;                         //22 Tfiltr
    u16  Pnt_Num;                            //23 Num
    u16  Pnt_Error;                          //24 Error
    u16  Pnt_U;                              //25 U
    u16  Pnt_End;
}FuncUF_5_FILTR_X2_type;
//********************************************************
// 1 Перезагрузка   1 Перезагрузка  1 Перезагрузка    
//      s32              s32              s32
// Функция получает аргументы типа s32
//********************************************************
u16 *FuncUF_5_FILTR_X2_1(FuncUF_5_FILTR_X2_type *progPnt, u32 ramPnt)
{
    UF_5_FILTR_X2State_type *sPnt;
    s16                     Tfiltr;
    _iq                     F;
    s16                     Num;
    
    sPnt = (UF_5_FILTR_X2State_type *)(ramPnt + progPnt->Pnt_State);
    Num = testBitMicro(ramPnt, progPnt->Pnt_Num);
    F =  load_s32(ramPnt, progPnt->Pnt_F);
    Tfiltr = load_s16(ramPnt, progPnt->Pnt_Tfiltr);
    if(flagIsFirstCall){            //начальная инициализация
        Tfiltr = 0;
    }
    if (Num == 0){
        sPnt->F1 = iq_Filtr_prm(sPnt->F1, load_s32(ramPnt, progPnt->Pnt_F1_1), Tfiltr);
        sPnt->F2 = iq_Filtr_prm(sPnt->F2, load_s32(ramPnt, progPnt->Pnt_F2_1), Tfiltr);
        sPnt->F3 = iq_Filtr_prm(sPnt->F3, load_s32(ramPnt, progPnt->Pnt_F3_1), Tfiltr);
        sPnt->F4 = iq_Filtr_prm(sPnt->F4, load_s32(ramPnt, progPnt->Pnt_F4_1), Tfiltr);
        sPnt->F5 = iq_Filtr_prm(sPnt->F5, load_s32(ramPnt, progPnt->Pnt_F5_1), Tfiltr);

        sPnt->U1 = iq_Filtr_prm(sPnt->U1, load_s32(ramPnt, progPnt->Pnt_U1_1), Tfiltr);
        sPnt->U2 = iq_Filtr_prm(sPnt->U2, load_s32(ramPnt, progPnt->Pnt_U2_1), Tfiltr);
        sPnt->U3 = iq_Filtr_prm(sPnt->U3, load_s32(ramPnt, progPnt->Pnt_U3_1), Tfiltr);
        sPnt->U4 = iq_Filtr_prm(sPnt->U4, load_s32(ramPnt, progPnt->Pnt_U4_1), Tfiltr);
        sPnt->U5 = iq_Filtr_prm(sPnt->U5, load_s32(ramPnt, progPnt->Pnt_U5_1), Tfiltr);
    }else{
        sPnt->F1 = iq_Filtr_prm(sPnt->F1, load_s32(ramPnt, progPnt->Pnt_F1_2), Tfiltr);
        sPnt->F2 = iq_Filtr_prm(sPnt->F2, load_s32(ramPnt, progPnt->Pnt_F2_2), Tfiltr);
        sPnt->F3 = iq_Filtr_prm(sPnt->F3, load_s32(ramPnt, progPnt->Pnt_F3_2), Tfiltr);
        sPnt->F4 = iq_Filtr_prm(sPnt->F4, load_s32(ramPnt, progPnt->Pnt_F4_2), Tfiltr);
        sPnt->F5 = iq_Filtr_prm(sPnt->F5, load_s32(ramPnt, progPnt->Pnt_F5_2), Tfiltr);

        sPnt->U1 = iq_Filtr_prm(sPnt->U1, load_s32(ramPnt, progPnt->Pnt_U1_2), Tfiltr);
        sPnt->U2 = iq_Filtr_prm(sPnt->U2, load_s32(ramPnt, progPnt->Pnt_U2_2), Tfiltr);
        sPnt->U3 = iq_Filtr_prm(sPnt->U3, load_s32(ramPnt, progPnt->Pnt_U3_2), Tfiltr);
        sPnt->U4 = iq_Filtr_prm(sPnt->U4, load_s32(ramPnt, progPnt->Pnt_U4_2), Tfiltr);
        sPnt->U5 = iq_Filtr_prm(sPnt->U5, load_s32(ramPnt, progPnt->Pnt_U5_2), Tfiltr);
    }
    
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
const char TblFuncUF_5_FILTR_X2_1[25]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //01 F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //02 F5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //03 F4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //04 F3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //05 F2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //06 F1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //07 U5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //08 U4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //09 U3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10 U2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11 U1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12 F5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13 F4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14 F3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15 F2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16 F1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17 U5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18 U4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19 U3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20 U2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21 U1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22 Tfiltr
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //23 Num
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24 Error
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //25 U
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("UF_5_FILTR_X2", "DA", TblFuncUF_5_FILTR_X2_1, FuncUF_5_FILTR_X2_1, sizeof(TblFuncUF_5_FILTR_X2_1), sizeof(UF_5_FILTR_X2State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUF_5_FILTR_X2_1,
#endif
//********************************************************
