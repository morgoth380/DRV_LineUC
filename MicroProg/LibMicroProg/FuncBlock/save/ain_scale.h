#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Ain;                          //1  Ain
    u16  Pnt_Max;                          //2  Max
    u16  Pnt_Min;                          //3  Min
    u16  Pnt_Stepen;                       //4  Stepen
    u16  Pnt_EdIsm;                        //5  EdIsm
    u16  Pnt_P1;                           //6  P1
    u16  Pnt_P2;                           //7  P2
    u16  Pnt_P3;                           //8  P3
    u16  Pnt_M1;                           //9  M1
    u16  Pnt_M2;                           //10  M2
    u16  Pnt_M3;                           //11  M3
    u16  Pnt_Format;                       //12  Format
    u16  Pnt_Out_proc;                     //13  Out_%
    u16  Pnt_Out;                          //14  Out
    u16  Pnt_End;
}FuncAIN_SCALE_type;
//********************************************************
u16 *FuncAIN_SCALE_1(FuncAIN_SCALE_type *progPnt, u32 ramPnt)
{
    f32                   in, inq, out, P1, P2, P3, M1, M2, M3, lmin, lmax;
    s16                   a, b;
    f32                   min, max;


    a = load_s16(ramPnt, progPnt->Pnt_Stepen)&0xFF;
    b = load_s16(ramPnt, progPnt->Pnt_EdIsm)&0xFF;
    save_s16(ramPnt, progPnt->Pnt_Format, b|(a<<8));        //посчитали формат

    in = _IQtoF(load_s32(ramPnt, progPnt->Pnt_Ain));
    P1 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_P1));
    P2 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_P2));
    P3 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_P3));
    M1 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_M1));
    M2 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_M2));
    M3 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_M3));
    min = _IQtoF(load_s32(ramPnt, progPnt->Pnt_Min));
    max = _IQtoF(load_s32(ramPnt, progPnt->Pnt_Max));

    if (in < P1){
        lmax = (max-min) * M1 + min;
        lmin = min;
        in = in / P1;
        inq = in * M1;
    }else
        if (in < P2){
            lmin = (max-min) * M1 + min;
            lmax = (max-min) * M2 + min;
            in = (in - P1)/(P2 - P1);
            inq = in * (M2 - M1) + M1;
        }else
            if (in < P3){
                lmin = (max-min) * M2 + min;
                lmax = (max-min) * M3 + min;
                in = (in - P2)/(P3 - P2);
                inq = in * (M3 - M2) + M2;
            }else{
                lmin = (max-min) * M3 + min;
                lmax = max;
                in = (in - P3)/(1 - P3);
                inq = in * (1 - M3) + M3;
            }

    out = in * (lmax-lmin) + lmin;
    save_s32(ramPnt, progPnt->Pnt_Out, _IQ(out));
    save_s32(ramPnt, progPnt->Pnt_Out_proc, _IQ(inq));

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAIN_SCALE_1[14]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ain
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Stepen
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  EdIsm
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  P1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  P2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  P3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  M1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  M2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  M3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Format
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //13  Out_%
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //13  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AIN_SCALE", "AIN", TblFuncAIN_SCALE_1, FuncAIN_SCALE_1, sizeof(TblFuncAIN_SCALE_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAIN_SCALE_1,
#endif
//********************************************************
