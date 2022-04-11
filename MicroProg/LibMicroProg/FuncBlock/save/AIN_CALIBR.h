#ifdef    GET_FUNC_CODE
//====================================================================


const nvAinCalState_type  nvAinCalState_default = {
0       ,        // 0 NotWr_calibrAin      Не Вписывать
0       ,        // 1 WrZer_5mA_calibrAin  КалибрКод 0 для датчика 0 - 5 мА
0       ,        // 2 WrZer_20mA_calibrAin КалибрКод нуля для датчика 4 - 20 мА
0       ,        // 3 WrZer_10V_calibrAin  КалибрКод нуля для датчика 0 - 10 В
//2050    ,        // 4 WrZerUP_calibrAin    КалибрКод 0 двухполярный
4095    ,        // 5 Wr5_calibrAin        КалибрКод 5 мА
4095    ,        // 6 Wr10_calibrAin       КалибрКод 10 В
4095    ,        // 7 Wr20_calibrAin       КалибрКод 20 mA
};
//====================================================================
typedef struct{
        filtr_type  filtr;
        s16         CalibrCode[kolv_calibrAin];
        s16         CalibInc;
        s32         SumCalibrCode;
}AinCalState_type;
//====================================================================
//#define     nvAinCalState           (*(nvAinCalState_type*)0)

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Ain;                          //1  Ain
    u16  Pnt_Type;                         //2  Type
    u16  Pnt_Tfiltr;                       //3  Tfiltr
    u16  Pnt_Calibr;                       //4  Calibr
    u16  Pnt_HighLim;                      //5  Max
    u16  Pnt_LowLim;                       //6  Min
    u16  Pnt_NumAIN;                       //7   номер физического аналогового входа
    u16  Pnt_Min_f;                        //8  Min_f
    u16  Pnt_Max_f;                        //9  Max_f
    u16  Pnt_Sens_f;                       //10  Sens_f
    u16  Pnt_TekCode;                      //11  TekCode
    u16  Pnt_OutProc;                      //12  Out_%
    u16  Pnt_End;
}FuncAIN_CALIBR_type;
//********************************************************
u16 *FuncAIN_CALIBR_1(FuncAIN_CALIBR_type *progPnt, u32 ramPnt)
{
    AinCalState_type    *sPnt;
    s16             TmpFail;
    s16             deltaCode, a, CodeMin, CodeZer, CodeMax, UP = 1;//, MinVal, MaxVal;
    s32             Val, tmp;
    void            *nvAdr;
    sPnt = (AinCalState_type *)(ramPnt + progPnt->Pnt_State);
    if(flagIsFirstCall){            //начальная инициализация

//        nvAdr = (void *)((u32)&nv.a1.CalibrCode + (sizeof(nvAinCalState_type) * load_s16(ramPnt, progPnt->Pnt_NumAIN)));
        nvAdr = (void *)((u32)&nv.AInput[load_s16(ramPnt, progPnt->Pnt_NumAIN)].CalibrCode);
        RdFramMicro(sPnt->CalibrCode, nvAdr, sizeof(nvAinCalState_type));

        if(sPnt->CalibrCode[Wr10_calibrAin] == 0 ){
            memcpy( sPnt->CalibrCode, &nvAinCalState_default, sizeof(nvAinCalState_type));
            WrFramMicro(nvAdr, sPnt->CalibrCode, sizeof(nvAinCalState_type));
        }

        sPnt->CalibInc = 0;
        sPnt->SumCalibrCode = 0;
    }
    Val = load_s16(ramPnt, progPnt->Pnt_Ain);
///    save_s16(ramPnt, progPnt->Pnt_TekCode, (s16)Val);
    a = load_s16(ramPnt, progPnt->Pnt_Calibr);
    if(a){
        if(a < kolv_calibrAin){
            if (sPnt->CalibInc < 250){
                sPnt->SumCalibrCode += (s16)Val;
                sPnt->CalibInc++;
            }else {
                save_s16(ramPnt, progPnt->Pnt_Calibr, 0);
                sPnt->CalibrCode[a] = (s16)(sPnt->SumCalibrCode/(s32)sPnt->CalibInc);
//                nvAdr = (void *)((u32)&nv.a1.CalibrCode[a] + (sizeof(nvAinCalState_type) * load_s16(ramPnt, progPnt->Pnt_NumAIN)));
                nvAdr = (void *)((u32)&nv.AInput[load_s16(ramPnt, progPnt->Pnt_NumAIN)].CalibrCode[a]);
                WrFramMicro(nvAdr, &sPnt->CalibrCode[a], sizeof(s16));
                sPnt->CalibInc = 0;
                sPnt->SumCalibrCode = 0;
            }
        }else save_s16(ramPnt, progPnt->Pnt_Calibr, 0);
    }

    a = load_s16(ramPnt, progPnt->Pnt_Tfiltr);
    Val = (s32)Filtr(&sPnt->filtr, (s16)Val, a);

    CodeZer = 0;
    a = load_s16(ramPnt, progPnt->Pnt_Type);
    switch(a){
//    case V0_10_UP_TypeAin:
//            CodeMax = sPnt->CalibrCode[Wr10_calibrAin];
//            CodeZer = sPnt->CalibrCode[WrZerUP_calibrAin];
//            CodeMin = 2*CodeZer - CodeMax;
//            UP = 0;
//            break;
    case mA0_5_TypeAin:
            CodeMax = sPnt->CalibrCode[Wr5_calibrAin];
            CodeZer = sPnt->CalibrCode[WrZer_5mA_calibrAin];
            CodeMin = CodeZer;
            break;
    case V0_10_TypeAin:
            CodeMax = sPnt->CalibrCode[Wr10_calibrAin];
            CodeZer = sPnt->CalibrCode[WrZer_10V_calibrAin];
            CodeMin = CodeZer;
            break;
    case mA4_20_TypeAin:
            CodeMax = sPnt->CalibrCode[Wr20_calibrAin];
            CodeZer = sPnt->CalibrCode[WrZer_20mA_calibrAin];
            CodeMin = CodeZer;
            break;
    default :   CodeMax = CodeMin = CodeZer;  break;
    }

    deltaCode = CodeMax - CodeMin;  //  +
    TmpFail = 0;
    if(Val < CodeMin){
        if(Val < (CodeMin - (deltaCode>>5)))    TmpFail = 1;    // 1/32 = 3,125% заползание вверх за границы
        Val = CodeMin;
    }
    if(Val > CodeMax){
        if(Val > (CodeMax + (deltaCode>>5)))    TmpFail = 1;    // 1/32 = 3,125% заползание вниз за границы
        Val = CodeMax;
    }
    if(deltaCode == 0){
        tmp = 0;
        Val = 0;
    }else{
        tmp = (((s32)(Val - (s32)CodeMin))<<12)/(CodeMax - CodeMin);
        if (UP == 0){
            tmp *= 2;
            tmp = tmp - (s32)(1 << 12);
        }
    }
    save_s32(ramPnt, progPnt->Pnt_OutProc, (s32)(tmp << 12));

//    MinVal = load_s16(ramPnt, progPnt->Pnt_LowLim);
//    MaxVal = load_s16(ramPnt, progPnt->Pnt_HighLim);

    if (TmpFail == 1){
    	setBitMicro(ramPnt, progPnt->Pnt_Sens_f);
    }else{
    	clrBitMicro(ramPnt, progPnt->Pnt_Sens_f);
    }
//    if (Val < MinVal)       setBitMicro(ramPnt, progPnt->Pnt_Min_f);
//    else if (Val > MaxVal)  setBitMicro(ramPnt, progPnt->Pnt_Max_f);        //
//    else if (TmpFail)       setBitMicro(ramPnt, progPnt->Pnt_Sens_f);       //авария датчика
//    else{
//        clrBitMicro(ramPnt, progPnt->Pnt_Min_f);
//        clrBitMicro(ramPnt, progPnt->Pnt_Max_f);
//        clrBitMicro(ramPnt, progPnt->Pnt_Sens_f);
//    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAIN_CALIBR_1[12]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Ain
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Tfiltr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Calibr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  NumAIN
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Min_f
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Max_f
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Sens_f
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //11  TekCode
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Out_%
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AIN_CALIBR", "AIN", TblFuncAIN_CALIBR_1, FuncAIN_CALIBR_1, sizeof(TblFuncAIN_CALIBR_1), sizeof(AinCalState_type), sizeof(nvAinCalState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAIN_CALIBR_1,
#endif
//********************************************************
