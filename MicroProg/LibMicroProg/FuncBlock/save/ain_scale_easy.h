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
    u16  Pnt_Format;                       //6  Format
    u16  Pnt_Out_perc;                     //7  Out_%
    u16  Pnt_Out;                          //8  Out
    u16  Pnt_End;
}FuncAIN_SCALE_EASY_type;
//********************************************************
u16 *FuncAIN_SCALE_EASY_1(FuncAIN_SCALE_EASY_type *progPnt, u32 ramPnt)
{
    //f32                   in, inq, out, lmin, lmax;
    _iq                   in, min, max, out;
    s16                   a, b;
    //f32                   min, max;

    a = load_s16(ramPnt, progPnt->Pnt_Stepen)&0xFF;
    b = load_s16(ramPnt, progPnt->Pnt_EdIsm)&0xFF;
    save_s16(ramPnt, progPnt->Pnt_Format, b|(a<<8));        //посчитали формат

    //in = _IQtoF(load_s32(ramPnt, progPnt->Pnt_Ain));
    //min = _IQtoF(load_s32(ramPnt, progPnt->Pnt_Min));
    //max = _IQtoF(load_s32(ramPnt, progPnt->Pnt_Max));

    //out = in * (max-min) + min;
    //save_s32(ramPnt, progPnt->Pnt_Out, _IQ(out));
    //save_s32(ramPnt, progPnt->Pnt_Out_perc, _IQ(inq));

    in = load_s32(ramPnt, progPnt->Pnt_Ain);
    max = load_s32(ramPnt, progPnt->Pnt_Max);
    min = load_s32(ramPnt, progPnt->Pnt_Min);

    out = _IQmpy(in, max - min) + min;
    save_s32(ramPnt, progPnt->Pnt_Out, out);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAIN_SCALE_EASY_1[8]={
		S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ain
		S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Max
		S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Stepen
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  EdIsm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Format
		S32_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Out_%
		S32_PIN_TYPE | OUTPUT_PIN_MODE     //8  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AIN_SCALE_EASY", "DA", TblFuncAIN_SCALE_EASY_1, FuncAIN_SCALE_EASY_1, sizeof(TblFuncAIN_SCALE_EASY_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAIN_SCALE_EASY_1,
#endif
//********************************************************
