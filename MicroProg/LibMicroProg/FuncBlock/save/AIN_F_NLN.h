#ifdef    GET_FUNC_CODE
//====================================================================
typedef struct{
        s16         CalibrCode[kolv_calibrAin];
}nvAinNLNState_type;

const nvAinNLNState_type  nvAinNLNState_default[] = {
0       ,        // 0 NotWr_calibrAin     Не Вписывать
0       ,        // 1 WrZer_calibrAin     КалибрКод 0
2100    ,        // 2 WrZerUP_calibrAin   КалибрКод 0 двухполярный
4095    ,        // 3 Wr5_calibrAin       КалибрКод 5 В
4095    ,        // 4 Wr10_calibrAin      КалибрКод +10 В
4095    ,        // 5 Wr20_calibrAin      КалибрКод 20 mA
0       ,        // 6 P1_calibrAin        КалибрКод 1-й точки нелинейности
0       ,        // 7 P2_calibrAin        КалибрКод 2-й точки нелинейности
0       ,        // 8 P3_calibrAin        КалибрКод 3-й точки нелинейности
};
//====================================================================
typedef struct{
        filtr_type  filtr;
        s16         CalibrCode[kolv_calibrAin];
}AinNLNState_type;
//====================================================================
#define     nvAinNLNState           (*(nvAinNLNState_type*)0)

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Ain;                          //1  Ain
    u16  Pnt_Max;                          //2  Max
    u16  Pnt_Min;                          //3  Min
    u16  Pnt_Stepen;                       //4  Stepen
    u16  Pnt_EdIsm;                        //5  EdIsm
    u16  Pnt_Type;                         //6  Type
    u16  Pnt_Tfiltr;                       //7  Tfiltr
    u16  Pnt_Calibr;                       //8  Calibr
    u16  Pnt_P1;                           //9  P1
    u16  Pnt_P2;                           //10  P2
    u16  Pnt_P3;                           //11  P3
    u16  Pnt_HighLim;                      //12  Max
    u16  Pnt_LowLim;                       //13  Min
    u16  Pnt_Min_f;                        //14  Min_f
    u16  Pnt_Max_f;                        //15  Max_f
    u16  Pnt_Sens_f;                       //16  Sens_f
    u16  Pnt_TekCode;                      //17  TekCode
    u16  Pnt_Format;                       //18  Format
    u16  Pnt_OutProc;                      //19  Out_%
    u16  Pnt_Out;                          //20  Out
    u16  Pnt_End;
}FuncAIN_F_NLN_type;
//********************************************************
u16 *FuncAIN_F_NLN_1(FuncAIN_F_NLN_type *progPnt, u32 ramPnt)
{
    AinNLNState_type    *sPnt;
    s16             TmpFail, Pn;
    s16             deltaCode, a, b, CodeMin, CodeZer, CodeZer1, CodeMax, MaxVal, MinVal, P1, P2, P3;
    s32             Val, tmp, Val1;
    void            *nvAdr;

    sPnt = (AinNLNState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    if(flagIsFirstCall){            //начальная инициализация
        nvAdr = GetNvAdr(progPnt->Pnt_nvState, nvAinNLNState.CalibrCode);
        RdFramMicro(sPnt->CalibrCode, nvAdr, sizeof(nvAinNLNState_type));
        if(sPnt->CalibrCode[Wr10_calibrAin] == 0 ){
            memcpy( sPnt->CalibrCode, nvAinNLNState_default, kolv_calibrAin * sizeof(s16));
            WrFramMicro(nvAdr, sPnt->CalibrCode, sizeof(nvAinNLNState_type));
        }
    }
    Val = load_s32(ramPnt, progPnt->Pnt_Ain);
        Val = Val>>12;
    save_s16(ramPnt, progPnt->Pnt_TekCode, (s16)Val);
    a = load_s16(ramPnt, progPnt->Pnt_Calibr);
    if(a){
        save_s16(ramPnt, progPnt->Pnt_Calibr, 0);
        if(a < kolv_calibrAin){
            nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvAinNLNState.CalibrCode[a]);
            sPnt->CalibrCode[a] = (s16)Val;
            WrFramMicro(nvAdr, &Val, sizeof(s16));
        }
    }
    a = load_s16(ramPnt, progPnt->Pnt_Stepen)&0xFF;
    b = load_s16(ramPnt, progPnt->Pnt_EdIsm)&0xFF;
    save_s16(ramPnt, progPnt->Pnt_Format, b|(a<<8));        //посчитали формат

    a = load_s16(ramPnt, progPnt->Pnt_Tfiltr);
    Val = (s32)Filtr(&sPnt->filtr, (s16)Val, a);

    CodeZer = sPnt->CalibrCode[WrZer_calibrAin];
    CodeMin = CodeZer;
    a = load_s16(ramPnt, progPnt->Pnt_Type);
    switch(a){
    case V0_10_UP_TypeAin:
            CodeMax = sPnt->CalibrCode[Wr10_calibrAin];
            CodeZer = sPnt->CalibrCode[WrZerUP_calibrAin];
            CodeMin = 2*CodeZer - CodeMax;
            MaxVal = load_s16(ramPnt, progPnt->Pnt_Max);
            save_s16(ramPnt, progPnt->Pnt_Min, -MaxVal);
            MinVal = load_s16(ramPnt, progPnt->Pnt_Min);
            P1 = load_s16(ramPnt, progPnt->Pnt_P1);
            if (P1 < MaxVal && P1 > 0) {Pn = 1;
            save_s16(ramPnt, progPnt->Pnt_P1, -P1);
            save_s16(ramPnt, progPnt->Pnt_P2, 0);
            save_s16(ramPnt, progPnt->Pnt_P3, P1);
            sPnt->CalibrCode[P2_calibrAin] = sPnt->CalibrCode[WrZerUP_calibrAin];
            sPnt->CalibrCode[P3_calibrAin] = sPnt->CalibrCode[P1_calibrAin];
            sPnt->CalibrCode[P1_calibrAin] = sPnt->CalibrCode[WrZerUP_calibrAin]*2 - sPnt->CalibrCode[P3_calibrAin];}
            else if (P1 >= MaxVal){
            save_s16(ramPnt, progPnt->Pnt_P2, P1);
            save_s16(ramPnt, progPnt->Pnt_P3, P1);
            }
            break;
    case V0_5_TypeAin:
            CodeMax = sPnt->CalibrCode[Wr5_calibrAin];
            MinVal = load_s16(ramPnt, progPnt->Pnt_Min);
            if (MinVal < 0) save_s16(ramPnt, progPnt->Pnt_Min, 0);
            break;
    case V0_10_TypeAin:
            CodeMax = sPnt->CalibrCode[Wr10_calibrAin];
            MinVal = load_s16(ramPnt, progPnt->Pnt_Min);
            if (MinVal < 0) save_s16(ramPnt, progPnt->Pnt_Min, 0);
            break;
        case mA4_20_TypeAin:
            CodeMax = sPnt->CalibrCode[Wr20_calibrAin];
            MinVal = load_s16(ramPnt, progPnt->Pnt_Min);
            if (MinVal < 0) save_s16(ramPnt, progPnt->Pnt_Min, 0);
            break;
    default :   CodeMax = CodeZer;  break;
    }

    deltaCode = CodeMax - CodeMin;  //  +
    TmpFail = 0;
    if(Val < CodeMin){
        if(Val < (CodeMin - (deltaCode>>5)))    TmpFail = 1;    //1/32 = 3,125% заползание вверх за границы
        Val = CodeMin;
    }

    if(Val > CodeMax){
        if(Val > (CodeMax + (deltaCode>>5)))    TmpFail = 1;    //1/32 = 3,125% заползание вниз за границы
        Val = CodeMax;
    }
        Val1 = Val;
        P1 = load_s16(ramPnt, progPnt->Pnt_P1);
        P2 = load_s16(ramPnt, progPnt->Pnt_P2);
        P3 = load_s16(ramPnt, progPnt->Pnt_P3);
        MinVal = load_s16(ramPnt, progPnt->Pnt_Min);
        MaxVal = load_s16(ramPnt, progPnt->Pnt_Max);
        Pn = 0;
        if (P1 < MaxVal && P1 > MinVal) Pn = 1;
        if (P2 < MaxVal && P2 > MinVal) Pn = 2;
        if (P3 < MaxVal && P3 > MinVal) Pn = 3;
    if(deltaCode == 0){
        tmp = 0;
        Val = 0;
    }
    else{
            CodeZer1 = CodeMin;
                MinVal = load_s16(ramPnt, progPnt->Pnt_Min);
                MaxVal = load_s16(ramPnt, progPnt->Pnt_Max);
                deltaCode = CodeMax - CodeMin;
            if (Val <= sPnt->CalibrCode[P1_calibrAin] && Val < CodeMax && Val > CodeMin && Pn > 0)
            {
                MaxVal = P1;
                deltaCode = sPnt->CalibrCode[P1_calibrAin] - CodeMin;
            }
            else
            if (Val <= sPnt->CalibrCode[P2_calibrAin] && Val < CodeMax && Val > CodeMin  && Pn > 1)
            {
                MinVal = P1;
                MaxVal = P2;
                deltaCode = sPnt->CalibrCode[P2_calibrAin] - sPnt->CalibrCode[P1_calibrAin];
                CodeZer1 = sPnt->CalibrCode[P1_calibrAin];
            }
            else
            if (Val <= sPnt->CalibrCode[P3_calibrAin] && Val < CodeMax && Val > CodeMin  && Pn > 2)
            {
                MinVal = P2;
                MaxVal = P3;
                deltaCode = sPnt->CalibrCode[P3_calibrAin] - sPnt->CalibrCode[P2_calibrAin];
                CodeZer1 = sPnt->CalibrCode[P2_calibrAin];
            }
            else
            if (Pn == 1) {
                MinVal = P1;
                deltaCode = CodeMax - sPnt->CalibrCode[P1_calibrAin];
                CodeZer1 = sPnt->CalibrCode[P1_calibrAin];
            }else if (Pn == 2) {
                MinVal = P2;
                deltaCode = CodeMax - sPnt->CalibrCode[P2_calibrAin];
                CodeZer1 = sPnt->CalibrCode[P2_calibrAin];
            }else if (Pn == 3) {
                MinVal = P3;
                deltaCode = CodeMax - sPnt->CalibrCode[P3_calibrAin];
                CodeZer1 = sPnt->CalibrCode[P3_calibrAin];
                }
        tmp = (((s32)(Val1 - (s32)CodeZer1))<<12)/ deltaCode;
        a = MaxVal - MinVal;    //  +
        Val1 = (((s32)(a * tmp))>>12) + MinVal;
        Val1 = s16LimControl(Val1);
                MinVal = load_s16(ramPnt, progPnt->Pnt_Min);
                MaxVal = load_s16(ramPnt, progPnt->Pnt_Max);
                tmp = (((s32)(Val - (s32)CodeMin))<<12)/(CodeMax - CodeMin);
                if (MinVal < 0)
                {
                    tmp *= 2;
                    tmp = tmp - (s32)(1 << 12);
                }
    }
    save_s32(ramPnt, progPnt->Pnt_OutProc, (s32)(tmp << 12));
    save_s16(ramPnt, progPnt->Pnt_Out, (s16)Val1);

    MinVal = load_s16(ramPnt, progPnt->Pnt_LowLim);
    MaxVal = load_s16(ramPnt, progPnt->Pnt_HighLim);

    if (Val < MinVal)       setBitMicro(ramPnt, progPnt->Pnt_Min_f);
    else if (Val > MaxVal)  setBitMicro(ramPnt, progPnt->Pnt_Max_f);        //
    else if (TmpFail)       setBitMicro(ramPnt, progPnt->Pnt_Sens_f);       //авария датчика
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_Min_f);
        clrBitMicro(ramPnt, progPnt->Pnt_Max_f);
        clrBitMicro(ramPnt, progPnt->Pnt_Sens_f);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAIN_F_NLN_1[20]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ain
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Stepen
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  EdIsm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Tfiltr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Calibr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  P1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  P2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  P3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Min
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14  Min_f
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //15  Max_f
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //16  Sens_f
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  TekCode
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  Format
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //19  Out_%
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //20  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AIN_F_NLN", "DA", TblFuncAIN_F_NLN_1, FuncAIN_F_NLN_1, sizeof(TblFuncAIN_F_NLN_1), sizeof(AinNLNState_type), sizeof(nvAinNLNState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAIN_F_NLN_1,
#endif
//********************************************************
