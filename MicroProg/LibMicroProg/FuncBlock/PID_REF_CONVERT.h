#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Ain1Max;                      //1  Ain1Max
    u16  Pnt_Ain1Min;                      //2  Ain1Min
    u16  Pnt_Ain1Frmt;                     //3  Ain1Frmt
    u16  Pnt_Ain2Max;                      //4  Ain2Max
    u16  Pnt_Ain2Min;                      //5  Ain2Min
    u16  Pnt_Ain2Frmt;                     //6  Ain2Frmt
    u16  Pnt_Ain3Max;                      //7  Ain3Max
    u16  Pnt_Ain3Min;                      //8  Ain3Min
    u16  Pnt_Ain3Frmt;                     //9  Ain3Frmt
    u16  Pnt_Ain4Max;                      //10  Ain4Max
    u16  Pnt_Ain4Min;                      //11  Ain4Min
    u16  Pnt_Ain4Frmt;                     //12  Ain4Frmt
    u16  Pnt_Ain5Max;                      //13  Ain5Max
    u16  Pnt_Ain5Min;                      //14  Ain5Min
    u16  Pnt_Ain5Frmt;                     //15  Ain5Frmt
    u16  Pnt_PidRefPer;                    //16  PidRefPer  
    u16  Pnt_SelRefPid;                    //17  SelRefPid
    u16  Pnt_RsrvUpDwn;                    //18  RsrvUpDwn    
    u16  Pnt_SelAin_N;                     //19  SelAin_N
    u16  Pnt_PidUstIn;                     //20  PidUstIn
    u16  Pnt_PidFrmt;                      //21  PidFrmt
    u16  Pnt_PidMin;                       //22  PidMin
    u16  Pnt_PidMax;                       //23  PidMax
    u16  Pnt_UpDwnPid;                     //24  UpDwnPid
    u16  Pnt_PidOut;                       //25  PidOut
    u16  Pnt_End;
}FuncPID_REF_CONVERT_type;
//********************************************************
typedef enum {
	AIN_1_SELECT = 1,
	AIN_2_SELECT = 2,
        AIN_3_SELECT = 3,
	AIN_4_SELECT = 4,                
}AIN_SELECT_enum;
//********************************************************
u16 *FuncPID_REF_CONVERT_1(FuncPID_REF_CONVERT_type *progPnt, u32 ramPnt)
{
    float	Max, Min, PidIn, PidOut;
    u16		Select_Ain, Format;
    f32         ZadaniePid;


    PidIn = load_float (ramPnt, progPnt->Pnt_PidUstIn);
    Select_Ain = load_s16 (ramPnt, progPnt->Pnt_SelAin_N);

    switch(Select_Ain){
        case AIN_1_SELECT:
                Max = load_float(ramPnt, progPnt->Pnt_Ain1Max);
                Min = load_float(ramPnt, progPnt->Pnt_Ain1Min);
                Format = load_s16(ramPnt, progPnt->Pnt_Ain1Frmt);
                break;
        case AIN_2_SELECT:
                Max = load_float(ramPnt, progPnt->Pnt_Ain2Max);
                Min = load_float(ramPnt, progPnt->Pnt_Ain2Min);
                Format = load_s16 (ramPnt, progPnt->Pnt_Ain2Frmt);
                break;
        case AIN_3_SELECT:
                Max = load_float(ramPnt, progPnt->Pnt_Ain3Max);
                Min = load_float(ramPnt, progPnt->Pnt_Ain3Min);
                Format = load_s16(ramPnt, progPnt->Pnt_Ain3Frmt);
                break;
        case AIN_4_SELECT:
                Max = load_float(ramPnt, progPnt->Pnt_Ain4Max);
                Min = load_float(ramPnt, progPnt->Pnt_Ain4Min);
                Format = load_s16(ramPnt, progPnt->Pnt_Ain4Frmt);
                break;                
        default:
                Max = 1.0f;
                Min = 0.0f;
                Format = 1 << 8;
                Format += 30;
        break;
    }
        
    save_float(ramPnt, progPnt->Pnt_PidMax, Max);
    save_float(ramPnt, progPnt->Pnt_PidMin, Min);
    save_s16(ramPnt, progPnt->Pnt_PidFrmt, Format);

    PidOut = (PidIn-Min)/(Max-Min);
    save_float(ramPnt, progPnt->Pnt_PidOut, PidOut);

    // Вывод текущего задания при работе от цифрового потенциометра
    if (load_s16(ramPnt, progPnt->Pnt_SelRefPid) != 2 /*== 3*/) { // Проверка на выбор цифрового потенциометра (номер смотреть на схеме)
      ZadaniePid = load_float(ramPnt, progPnt->Pnt_PidRefPer); // Читаем задание для ПИДа в %
      PidOut = (Max-Min) * ZadaniePid + Min; // И переводим в значение относительно пределов сигнала ОС
      save_float(ramPnt, progPnt->Pnt_UpDwnPid, PidOut);  // После чего, выводим его прямо в уставку задания ПИДа
    }
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPID_REF_CONVERT_1[25]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ain1Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Ain1Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Ain1Frmt
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Ain2Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Ain2Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Ain2Frmt
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Ain3Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Ain3Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Ain3Frmt
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  Ain4Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  Ain4Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Ain4Frmt
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  Ain5Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  Ain5Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Ain5Frmt        
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  PidRefPer
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  SelRefPid
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  RsrvUpDwn        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  SelAin_N
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  PidUstIn
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //21  PidFrmt
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //22  PidMin
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //23  PidMax
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //24  UpDwnPid
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //25  PidOut
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PID_REF_CONVERT", "DA", TblFuncPID_REF_CONVERT_1, FuncPID_REF_CONVERT_1, sizeof(TblFuncPID_REF_CONVERT_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPID_REF_CONVERT_1,
#endif
//********************************************************
