#ifdef    GET_FUNC_CODE
typedef struct{
        filtr_type  filtr;
        s16         CalibrCode[kolv_calibrAin];
}AinState_type;

typedef struct{
        s16         CalibrCode[kolv_calibrAin];
    
}nvAinState_type;
#define     nvAinState          (*(nvAinState_type*)0)

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
    u16  Pnt_HighLim;                      //9  HighLim
    u16  Pnt_LowLim;                       //10 LowLim
    u16  Pnt_Min_f;                        //11 Min_f
    u16  Pnt_Max_f;                        //12 Max_f
    u16  Pnt_Sens_f;                       //13 Sens_f
    u16  Pnt_TekCode;                      //14 TekCode
    u16  Pnt_Format;                       //15 Format
    u16  Pnt_OutProc;                      //16 OutProc
    u16  Pnt_Out;                          //17 Out
    u16  Pnt_End;
}FuncAIN_F_type;
//********************************************************
u16 *FuncAIN_F_1(FuncAIN_F_type *progPnt, u32 ramPnt)
{
    AinState_type   *sPnt;
    s16             TmpFail;
    s16             deltaCode, a, b, CodeMin, CodeZer, CodeMax, MaxVal, MinVal;
    s32             Val, tmp;
    void            *nvAdr;
    
    sPnt = (AinState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    if(flagIsFirstCall){            //начальная инициализация
        nvAdr = GetNvAdr(progPnt->Pnt_nvState, nvAinState.CalibrCode);
        RdFramMicro(sPnt->CalibrCode, nvAdr, sizeof(nvAinState_type));        
    }
    Val = load_s16(ramPnt, progPnt->Pnt_Ain);
    save_s16(ramPnt, progPnt->Pnt_TekCode, (s16)Val);
    a = load_s16(ramPnt, progPnt->Pnt_Calibr);
    if(a){
        save_s16(ramPnt, progPnt->Pnt_Calibr, 0);
        if(a < kolv_calibrAin){
            nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvAinState.CalibrCode[a]);
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
    case Bm10_10_TypeAin:    
        CodeMax = sPnt->CalibrCode[Wr10_calibrAin];        //MinVal - 0в MaxVal - 10в
        CodeMin = 2*CodeZer - CodeMax;
        break;
    case mA0_5_TypeAin:     CodeMax = sPnt->CalibrCode[Wr5_calibrAin];  break;
    case B0_10_TypeAin:     CodeMax = sPnt->CalibrCode[Wr10_calibrAin]; break;
    case mA4_20_TypeAin :    
        CodeMax = sPnt->CalibrCode[Wr20_calibrAin];
        CodeZer = CodeZer + (CodeMax - CodeZer + 2)/5;
    break;
    default :   CodeMax = CodeZer;  break;
    }
    
    deltaCode = CodeMax - CodeMin;    //    +
    TmpFail = 0;
    if(Val < CodeMin){
        if(Val < (CodeMin - (deltaCode>>5)))    TmpFail = 1;    //1/32 = 3,125% заползание вверх за границы
        Val = CodeMin;
    }
    
    if(Val > CodeMax){
        if(Val > (CodeMax + (deltaCode>>5)))    TmpFail = 1;    //1/32 = 3,125% заползание вниз за границы
        Val = CodeMax;
    }
    
    if(deltaCode == 0){
        tmp = 0;
        Val = 0;
    }
    else{
        tmp = (((s32)(Val - (s32)CodeZer))<<12)/ deltaCode;
        MinVal = load_s16(ramPnt, progPnt->Pnt_Min);
        MaxVal = load_s16(ramPnt, progPnt->Pnt_Max);
        a = MaxVal - MinVal;    //    +
        Val = (((s32)(a * tmp))>>12) + MinVal;
        Val = s16LimControl(Val);
    }
    save_s16(ramPnt, progPnt->Pnt_OutProc, (s16)tmp);
    save_s16(ramPnt, progPnt->Pnt_Out, (s16)Val);
    
    MinVal = load_s16(ramPnt, progPnt->Pnt_LowLim);
    MaxVal = load_s16(ramPnt, progPnt->Pnt_HighLim);

    if (Val < MinVal)       setBitMicro(ramPnt, progPnt->Pnt_Min_f);
    else if (Val > MaxVal)  setBitMicro(ramPnt, progPnt->Pnt_Max_f);        //
    else if (TmpFail)       setBitMicro(ramPnt, progPnt->Pnt_Sens_f);        //авария датчика
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
const char TblFuncAIN_F_1[17]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Ain    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Max    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Min    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Stepen 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  EdIsm  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Type   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Tfiltr 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Calibr 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  HighLim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10 LowLim 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11 Min_f  
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //12 Max_f  
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //13 Sens_f 
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14 TekCode
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //15 Format 
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16 OutProc
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17 Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("AIN_F", "DA", TblFuncAIN_F_1, FuncAIN_F_1, sizeof(TblFuncAIN_F_1), sizeof(AinState_type), sizeof(nvAinState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAIN_F_1,
#endif
//********************************************************
