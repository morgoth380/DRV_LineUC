#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_F;                            //1  F
    u16  Pnt_F4;                           //2  F4
    u16  Pnt_F3;                           //3  F3
    u16  Pnt_F2;                           //4  F2
    u16  Pnt_F1;                           //5  F1
    u16  Pnt_U4;                           //6  U4
    u16  Pnt_U3;                           //7  U3
    u16  Pnt_U2;                           //8  U2
    u16  Pnt_U1;                           //9  U1
    u16  Pnt_Error;                        //10  Error
    u16  Pnt_U;                            //11  U
    u16  Pnt_End;
}FuncUF_4_type;
//********************************************************
// 1 Перезагрузка   1 Перезагрузка  1 Перезагрузка    
//      s16              s16              s16
// Функция получает аргументы типа s16
//********************************************************
u16 *FuncUF_4_1(FuncUF_4_type *progPnt, u32 ramPnt)
{
    s16     F1, F2, F3, F4;
    s16     U1, U2, U3, U4;
    s32     F;
    
    F =  load_s16(ramPnt, progPnt->Pnt_F);
    F1 = load_s16(ramPnt, progPnt->Pnt_F1);    
    F2 = load_s16(ramPnt, progPnt->Pnt_F2);    
    F3 = load_s16(ramPnt, progPnt->Pnt_F3);    
    F4 = load_s16(ramPnt, progPnt->Pnt_F4);    

    U1 = load_s16(ramPnt, progPnt->Pnt_U1);    
    U2 = load_s16(ramPnt, progPnt->Pnt_U2);    
    U3 = load_s16(ramPnt, progPnt->Pnt_U3);    
    U4 = load_s16(ramPnt, progPnt->Pnt_U4);    
    if((F4 < F3)||(F3 < F2)||(F2 < F1)){
        save_s16(ramPnt, progPnt->Pnt_U, 0);
        setBitMicro(ramPnt, progPnt->Pnt_Error);
    }
    else{
        if(F > F3)      F = Fy_x1x2y1y2x((s32)F3, (s32)F4, (s32)U3, (s32)U4, F);
        else if(F > F2) F = Fy_x1x2y1y2x((s32)F2, (s32)F3, (s32)U2, (s32)U3, F);
        else if(F > F1) F = Fy_x1x2y1y2x((s32)F1, (s32)F2, (s32)U1, (s32)U2, F);
        else            F = Fy_x1x2y1y2x((s32)0 , (s32)F1, (s32)0 , (s32)U1, F);
        
        F = s16LimControl(F);
        save_s16(ramPnt, progPnt->Pnt_U, (s16)F);
        clrBitMicro(ramPnt, progPnt->Pnt_Error);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUF_4_1[11]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  F
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  F4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  F3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  F2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  F1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  U4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  U3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  U2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  U1
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //10  Error
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //11  U
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("UF_4", "DA", TblFuncUF_4_1, FuncUF_4_1, sizeof(TblFuncUF_4_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUF_4_1,
#endif
//********************************************************
//2 Перезагрузка    2 Перезагрузка  2 Перезагрузка    
//      s32              s32              s32
// Функция получает аргументы в формате IQ
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncUF_4_2(FuncUF_4_type *progPnt, u32 ramPnt)
{
    _iq     F1, F2, F3, F4;
    _iq     U1, U2, U3, U4;
    _iq     F;
    
    F =  load_s32(ramPnt, progPnt->Pnt_F);
    F1 = load_s32(ramPnt, progPnt->Pnt_F1);    
    F2 = load_s32(ramPnt, progPnt->Pnt_F2);    
    F3 = load_s32(ramPnt, progPnt->Pnt_F3);    
    F4 = load_s32(ramPnt, progPnt->Pnt_F4);    

    U1 = load_s32(ramPnt, progPnt->Pnt_U1);    
    U2 = load_s32(ramPnt, progPnt->Pnt_U2);    
    U3 = load_s32(ramPnt, progPnt->Pnt_U3);    
    U4 = load_s32(ramPnt, progPnt->Pnt_U4);    
    if((F4 < F3)||(F3 < F2)||(F2 < F1)){
        save_s32(ramPnt, progPnt->Pnt_U, 0);
        setBitMicro(ramPnt, progPnt->Pnt_Error);
    }
    else{
        if(F > F3)      F = iq_Fy_x1x2y1y2x(F3, F4, U3, U4, F);
        else if(F > F2) F = iq_Fy_x1x2y1y2x(F2, F3, U2, U3, F);
        else if(F > F1) F = iq_Fy_x1x2y1y2x(F1, F2, U1, U2, F);
        else            F = iq_Fy_x1x2y1y2x(0 , F1, 0 , U1, F);
        
        if(_IQabs(F) > _IQ(1.0))    F = _IQ(1.0);
        save_s32(ramPnt, progPnt->Pnt_U, F);
        clrBitMicro(ramPnt, progPnt->Pnt_Error);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUF_4_2[11]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  F4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  F3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  U4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  U3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  U2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  U1
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //10  Error
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //11  U
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("UF_4", "DA", TblFuncUF_4_2, FuncUF_4_2, sizeof(TblFuncUF_4_2), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUF_4_2,
#endif
//********************************************************
