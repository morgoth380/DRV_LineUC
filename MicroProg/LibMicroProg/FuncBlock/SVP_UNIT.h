#ifdef    GET_FUNC_CODE

typedef struct {
    u16 Pnt_Nfunc;
    //u16 Pnt_State;
    //u16  Pnt_nvState;
    u16 Pnt_PuskIn;		  //1  PuskIn
    u16 Pnt_StopIn;		  //2  StopIn
    u16 Pnt_DrvStatus;		  //3  DrvStatus
    u16 Pnt_FailTekDrv;		  //4  FailTekDrv
    u16 Pnt_KprObser;		  //5  KprObser
    u16 Pnt_KintObser;		  //6  KintObser
    u16 Pnt_TorqRegAutoset;	  //7  KprTorqMast
    u16 Pnt_KpTorqReg;		  //8  KprTorqSlav
    u16 Pnt_KintTorqReg;	  //9  KintTorq
    u16 Pnt_RS2RestTorq;	  //10  Kvuravn
    u16 Pnt_Krassog;		  //11  Krassog
    u16 Pnt_SplitTorque;	  //12  SplitTorque ;
    u16 Pnt_TorqMax;		  //13  TorqMax
    u16 Pnt_TorqMin;		  //14  TorqMin
    u16 Pnt_PhaseErr;		  //15  PhaseErr
    u16 Pnt_InputErr;		  //16  InputErr
    u16 Pnt_ConnErr;		  //17  ConnErr
    u16 Pnt_StatusWord;		  //18  StatusWord
    u16 Pnt_ChangeMasSL;          //19  ChangeMasSL
    u16 Pnt_WaitPhase;            //20  WaitPhase
    u16 Pnt_TipTorm;              //21  TipTorm
    u16 Pnt_TimeVub_In;           //22  TimeVub_In
    u16 Pnt_ModeDrive;            //23  ModeDrive
    u16 Pnt_ZipIn1;               //24  ZipIn1
    u16 Pnt_ZipIn2;               //25  ZipIn2
    u16 Pnt_GetReady;             //26  GetReady
    u16 Pnt_Function;             //27  Function
    u16 Pnt_nStartReady;          //28  nStartReady
    u16 Pnt_StartReady;           //29  StartReady
    u16 Pnt_WarnNeupr;            //30  WarnNeupr
    u16 Pnt_VybegTime;            //31  VybegTime
    u16 Pnt_Brake_Type;           //32  Brake_Type
    u16 Pnt_UartMB;               //33  UartMB
    u16 Pnt_ErrMBConn;            //34  ErrMBConn
    u16 Pnt_Err_DI;               //35  Err_DI
    u16 Pnt_PhaseErr1;            //36  PhaseErr1
    u16 Pnt_NomTorq;              //37  NomTorq
    u16 Pnt_ZipOut1;              //38  ZipOut1
    u16 Pnt_ZipOut1_1;            //39  ZipOut1
    u16 Pnt_ZipOut1_2;            //40  ZipOut1
    u16 Pnt_ReleCooler;           //41  ReleCooler
    u16 Pnt_ReleMasSl;            //42  ReleMasSl
    u16 Pnt_W_Flag;               //43  W_Flag
    u16 Pnt_Stop_En;              //44  Stop_En
    u16 Pnt_Pusk_En;              //45  Pusk_En
    u16 Pnt_End;
}FuncSVP_UNIT_type;

//********************************************************
u16 *FuncSVP_UNIT_1(FuncSVP_UNIT_type *progPnt, u32 ramPnt)
{
    float TorqLimitMax, TorqLimitMin; // RestTorq;
    

    if(flagIsFirstCall) {
        clrBitMicro(ramPnt, progPnt->Pnt_ReleMasSl); // rele
        clrBitMicro(ramPnt, progPnt->Pnt_Err_DI);//авария расхождения скоростей мастер - слейв
        GlobalM4.FlagCommand.bit.BitWAitPhase = 0;
        save_s16(ramPnt,progPnt->Pnt_ZipIn1, 0);
    
        return &progPnt->Pnt_End;
    }

    // Обработка предварительных уставок, которые не обязаны вызываться в блоке СВП
    //=========================================================================

    // Определяем какие коэффициенты наблюдателя пойдут в работу - авто или ручные
    if( load_s16(ramPnt, progPnt->Pnt_WaitPhase) == 0){
         GlobalM4.FlagCommand.bit.BitAutoKest = 0;
    }
    else{
         GlobalM4.FlagCommand.bit.BitAutoKest = 1;
    }    
    // Вычитываем текущее ограничение момента
    TorqLimitMax = (float)load_s16(ramPnt, progPnt->Pnt_TorqMax) * 0.1f;
    TorqLimitMin = (float)load_s16(ramPnt, progPnt->Pnt_TorqMin) * 0.1f;
    GlobalM4.TorqueMax = FLOAT_to_IQ(TorqLimitMax, Torq_base);      
    GlobalM4.TorqueMin = FLOAT_to_IQ(TorqLimitMin, Torq_base);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSVP_UNIT_1[45]= {

    BIT_PIN_TYPE | INPUT_PIN_MODE, //1   PuskIn
    BIT_PIN_TYPE | INPUT_PIN_MODE, //2   StopIn
    S16_PIN_TYPE | INPUT_PIN_MODE, //3   StatusDrv
    S16_PIN_TYPE | INPUT_PIN_MODE, //4   FailTekDrv
    S16_PIN_TYPE | INPUT_PIN_MODE, //5   KprObser
    S16_PIN_TYPE | INPUT_PIN_MODE, //6   KintObser
    S16_PIN_TYPE | INPUT_PIN_MODE, //7   KprTorqMast
    S16_PIN_TYPE | INPUT_PIN_MODE, //8   KprTorqSlav
    S16_PIN_TYPE | INPUT_PIN_MODE, //9   KintTorq
    S16_PIN_TYPE | INPUT_PIN_MODE, //10  Kvuravn
    S16_PIN_TYPE | INPUT_PIN_MODE, //11  Krassog
    S16_PIN_TYPE | INPUT_PIN_MODE, //12  TorqZad
    S16_PIN_TYPE | INPUT_PIN_MODE, //13  TorqMax
    S16_PIN_TYPE | INPUT_PIN_MODE, //14  TorqMin
    BIT_PIN_TYPE | INPUT_PIN_MODE, //15  PhaseErr
    S16_PIN_TYPE | INPUT_PIN_MODE, //16  InputErr
    S16_PIN_TYPE | INPUT_PIN_MODE, //17  ConnErr
    S16_PIN_TYPE | INPUT_PIN_MODE, //18  StatusWord
    BIT_PIN_TYPE | INPUT_PIN_MODE, //19  ChangeMasSL
    S16_PIN_TYPE | INPUT_PIN_MODE, //20  WaitPhase
    S16_PIN_TYPE | INPUT_PIN_MODE, //21  TipTorm
    S16_PIN_TYPE | INPUT_PIN_MODE, //22  TimeVub_In
    S16_PIN_TYPE | INPUT_PIN_MODE, //23  ModeDrive
    S16_PIN_TYPE | INPUT_PIN_MODE, //24  ZipIn1
    S16_PIN_TYPE | INPUT_PIN_MODE, //25  ZipIn2
    BIT_PIN_TYPE | INPUT_PIN_MODE, //26  GetReady
    S16_PIN_TYPE | INPUT_PIN_MODE, //27  Function
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//28  nStartReady
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//29  StartReady
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//30  WarnNeupr
    S16_PIN_TYPE | OUTPUT_PIN_MODE,//31  VybegTime
    S16_PIN_TYPE | OUTPUT_PIN_MODE,//32  Brake_Type
    S16_PIN_TYPE | OUTPUT_PIN_MODE,//33  UartMB
    S16_PIN_TYPE | OUTPUT_PIN_MODE,//34  ErrMBConn
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//35  Err_DI
    S16_PIN_TYPE | OUTPUT_PIN_MODE,//36  PhaseErr1
    S16_PIN_TYPE | OUTPUT_PIN_MODE,//37  NomTorq
    BIT_PIN_TYPE | OUTPUT_PIN_MODE, //38  ZipOut1
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//39  ZipOut1
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//40  ZipOut1
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//41  ReleCooler
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//42  ReleMasSl
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//43  W_Flag
    BIT_PIN_TYPE | OUTPUT_PIN_MODE,//44  Stop_En
    BIT_PIN_TYPE | OUTPUT_PIN_MODE //45  Pusk_En
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SVP_UNIT", "DD", TblFuncSVP_UNIT_1, FuncSVP_UNIT_1, sizeof(TblFuncSVP_UNIT_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
(pFuncMicro_type)FuncSVP_UNIT_1,
#endif
//********************************************************
