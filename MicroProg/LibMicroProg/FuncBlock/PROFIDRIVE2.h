#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Fault;                        //1  Fault
    u16  Pnt_Speed;                        //2  Speed
    u16  Pnt_Freq;                         //3  Freq
    u16  Pnt_Current;                      //4  Current
    u16  Pnt_ResIn1;                       //5  ResIn1
    u16  Pnt_ResIn2;                       //6  ResIn2
    u16  Pnt_ResIn3;                       //7  ResIn3
    u16  Pnt_ResIn4;                       //8  ResIn4
    u16  Pnt_ResIn5;                       //9  ResIn5
    u16  Pnt_ResIn6;                       //10  ResIn6
    u16  Pnt_ResIn7;                       //11  ResIn7
    u16  Pnt_ResIn8;                       //12  ResIn8
    u16  Pnt_ResIn9;                       //13  ResIn9
    u16  Pnt_ResIn10;                      //14  ResIn10
    u16  Pnt_ResIn11;                      //15  ResIn11
    u16  Pnt_ResIn12;                      //16  ResIn12
    u16  Pnt_OperInhib;                    //17  OperInhib
    u16  Pnt_Enable;                       //18  Enable
    u16  Pnt_Contactor;                    //19  Contactor
    u16  Pnt_Voltage;                      //20  Voltage
    u16  Pnt_ResOut1;                      //21  ResOut1
    u16  Pnt_ResOut2;                      //22  ResOut2
    u16  Pnt_ResOut3;                      //23  ResOut3
    u16  Pnt_ResOut4;                      //24  ResOut4
    u16  Pnt_ResOut5;                      //25  ResOut5
    u16  Pnt_ResOut6;                      //26  ResOut6
    u16  Pnt_ResOut7;                      //27  ResOut7
    u16  Pnt_ResOut8;                      //28  ResOut8
    u16  Pnt_ResOut9;                      //29  ResOut9
    u16  Pnt_ResOut10;                     //30  ResOut10
    u16  Pnt_ResOut11;                     //31  ResOut11
    u16  Pnt_ResOut12;                     //32  ResOut12
    u16  Pnt_ResOut13;                     //33  ResOut13
    u16  Pnt_ResOut14;                     //34  ResOut14
    u16  Pnt_ResOut15;                     //35  ResOut15
    u16  Pnt_ResOut16;                     //36  ResOut16
    u16  Pnt_ResOut17;                     //37  ResOut17
    u16  Pnt_ResOut18;                     //38  ResOut18
    u16  Pnt_ResOut19;                     //39  ResOut19
    u16  Pnt_ResOut20;                     //40  ResOut20
    u16  Pnt_End;
}FuncPROFIDRIVE_type;
//********************************************************
u16 *FuncPROFIDRIVE_1(FuncPROFIDRIVE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPROFIDRIVE_1[40]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Fault
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Speed
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Freq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Current
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  ResIn1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  ResIn2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  ResIn3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  ResIn4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  ResIn5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  ResIn6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  ResIn7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  ResIn8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  ResIn9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  ResIn10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  ResIn11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  ResIn12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  OperInhib
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Enable
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Contactor
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Voltage
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //21  ResOut1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //22  ResOut2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  ResOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  ResOut4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  ResOut5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  ResOut6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  ResOut7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  ResOut8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  ResOut9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  ResOut10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  ResOut11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  ResOut12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //33  ResOut13
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //34  ResOut14
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //35  ResOut15
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //36  ResOut16
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //37  ResOut17
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //38  ResOut18
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //39  ResOut19
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //40  ResOut20
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PROFIDRIVE", "U", TblFuncPROFIDRIVE_1, FuncPROFIDRIVE_1, sizeof(TblFuncPROFIDRIVE_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPROFIDRIVE_1,
#endif
//********************************************************
