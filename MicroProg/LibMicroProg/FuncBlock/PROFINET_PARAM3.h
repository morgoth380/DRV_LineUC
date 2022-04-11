#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Status1;                      //1  Status1
    u16  Pnt_Status2;                      //2  Status2
    u16  Pnt_drv_fail;                     //3  drv_fail
    u16  Pnt_RotFreq1;                     //4  RotFreq1
    u16  Pnt_Current1;                     //5  Current1
    u16  Pnt_currTorq1;                    //6  currTorq1
    u16  Pnt_outVoltage1;                  //7  outVoltage1
    u16  Pnt_driveTemp1;                   //8  driveTemp1
    u16  Pnt_encoPos1;                     //9  encoPos1
    u16  Pnt_currFreq1;                    //10  currFreq1
    u16  Pnt_DC_Volt1;                     //11  DC_Volt1
    u16  Pnt_queryCnt1;                    //12  queryCnt1
    u16  Pnt_curPower1;                    //13  curPower1
    u16  Pnt_freqRef1;                     //14  freqRef1
    u16  Pnt_torqRef1;                     //15  torqRef1
    u16  Pnt_torqMax1;                     //16  torqMax1
    u16  Pnt_torqMin1;                     //17  torqMin1
    u16  Pnt_queryCnt1_1;                  //18  queryCnt1
    u16  Pnt_command1;                     //19  command1
    u16  Pnt_base_freq;                    //20  base_freq
    u16  Pnt_currentBase;                  //21  currentBase
    u16  Pnt_voltBase;                     //22  voltBase
    u16  Pnt_powerBase;                    //23  powerBase
    u16  Pnt_RefACSForm;                   //24  RefACSForm
    u16  Pnt_maxFreqPU;                    //25  maxFreqPU
    u16  Pnt_maxPosPU;                     //26  maxPosPU
    u16  Pnt_drvNum;                       //27  drvNum
    u16  Pnt_Reserve1;                     //28  Reserve1
    u16  Pnt_Reserve2;                     //29  Reserve2
    u16  Pnt_Reserve3;                     //30  Reserve3
    u16  Pnt_Reserve4;                     //31  Reserve4
    u16  Pnt_Koeff1;                       //32  Koeff1
    u16  Pnt_Koeff2;                       //33  Koeff2
    u16  Pnt_Koeff3;                       //34  Koeff3
    u16  Pnt_Koeff4;                       //35  Koeff4
    u16  Pnt_Koeff5;                       //36  Koeff5
    u16  Pnt_Koeff6;                       //37  Koeff6
    u16  Pnt_Koeff7;                       //38  Koeff7
    u16  Pnt_Koeff8;                       //39  Koeff8
    u16  Pnt_Koeff9;                       //40  Koeff9
    u16  Pnt_Koeff10;                      //41  Koeff10
    u16  Pnt_ZipIn1;                       //42  ZipIn1
    u16  Pnt_ZipIn2;                       //43  ZipIn2
    u16  Pnt_ZipIn3;                       //44  ZipIn3
    u16  Pnt_ZipIn4;                       //45  ZipIn4
    u16  Pnt_ZipIn5;                       //46  ZipIn5
    u16  Pnt_ZipIn6;                       //47  ZipIn6
    u16  Pnt_StatusOut;                    //48  StatusOut
    u16  Pnt_Fail;                         //49  Fail
    u16  Pnt_Warning;                      //50  Warning
    u16  Pnt_RotFreq2;                     //51  RotFreq2
    u16  Pnt_Current2;                     //52  Current2
    u16  Pnt_currTorq2;                    //53  currTorq2
    u16  Pnt_outVoltage1_1;                //54  outVoltage1
    u16  Pnt_driveTemp2;                   //55  driveTemp2
    u16  Pnt_encoPos2;                     //56  encoPos2
    u16  Pnt_currFreq2;                    //57  currFreq2
    u16  Pnt_DC_Volt2;                     //58  DC_Volt2
    u16  Pnt_queryCnt2;                    //59  queryCnt2
    u16  Pnt_curPower2;                    //60  curPower2
    u16  Pnt_freqRef2;                     //61  freqRef2
    u16  Pnt_torqRef2;                     //62  torqRef2
    u16  Pnt_torqMax2;                     //63  torqMax2
    u16  Pnt_torqMin2;                     //64  torqMin2
    u16  Pnt_queryCnt2_1;                  //65  queryCnt2
    u16  Pnt_CmdBit;                       //66  CmdBit
    u16  Pnt_driveMode;                    //67  driveMode
    u16  Pnt_ACS_enable;                   //68  ACS_enable
    u16  Pnt_RampStop;                     //69  RampStop
    u16  Pnt_FreqExtrStop;                 //70  FreqExtrStop
    u16  Pnt_FreqStop;                     //71  FreqStop
    u16  Pnt_GT_freez;                     //72  GT_freez
    u16  Pnt_WorkEnable;                   //73  WorkEnable
    u16  Pnt_ResOut1;                      //74  ResOut1
    u16  Pnt_ResOut2;                      //75  ResOut2
    u16  Pnt_ResOut3;                      //76  ResOut3
    u16  Pnt_ResOut4;                      //77  ResOut4
    u16  Pnt_ResOut5;                      //78  ResOut5
    u16  Pnt_ZipOut1;                      //79  ZipOut1
    u16  Pnt_ZipOut2;                      //80  ZipOut2
    u16  Pnt_ZipOut3;                      //81  ZipOut3
    u16  Pnt_ZipOut4;                      //82  ZipOut4
    u16  Pnt_ZipOut5;                      //83  ZipOut5
    u16  Pnt_ZipOut6;                      //84  ZipOut6
    u16  Pnt_End;
}FuncPROFINET_PARAM3_type;
//********************************************************
u16 *FuncPROFINET_PARAM3_1(FuncPROFINET_PARAM3_type *progPnt, u32 ramPnt)
{

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPROFINET_PARAM3_1[84]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Status1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Status2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  drv_fail
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  RotFreq1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Current1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  currTorq1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  outVoltage1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  driveTemp1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  encoPos1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  currFreq1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  DC_Volt1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  queryCnt1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  curPower1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  freqRef1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  torqRef1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  torqMax1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  torqMin1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  queryCnt1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  command1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  base_freq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  currentBase
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  voltBase
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  powerBase
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  RefACSForm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  maxFreqPU
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  maxPosPU
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  drvNum
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Reserve1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //29  Reserve2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  Reserve3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  Reserve4

        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //32  Koeff1
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //33  Koeff2
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //34  Koeff3
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //35  Koeff4
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //36  Koeff5
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //37  Koeff6
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //38  Koeff7
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //39  Koeff8
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //40  Koeff9
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //41  Koeff10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  ZipIn1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43  ZipIn2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //44  ZipIn3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //45  ZipIn4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //46  ZipIn5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //47  ZipIn6

        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //48  StatusOut
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //49  Fail
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //50  Warning
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //51  RotFreq2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //52  Current2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //53  currTorq2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //54  outVoltage1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //55  driveTemp2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //56  encoPos2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //57  currFreq2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //58  DC_Volt2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //59  queryCnt2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //60  curPower2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //61  freqRef2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //62  torqRef2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //63  torqMax2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //64  torqMin2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //65  queryCnt2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //66  CmdBit
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //67  driveMode
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //68  ACS_enable
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //69  RampStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //70  FreqExtrStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //71  FreqStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //72  GT_freez
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //73  WorkEnable
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //74  ResOut1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //75  ResOut2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //76  ResOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //77  ResOut4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //78  ResOut5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //79  ZipOut1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //80  ZipOut2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //81  ZipOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //82  ZipOut4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //83  ZipOut5
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //84  ZipOut6
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
//OpisFuncBlock("PROFINET_PARAM3", "DD", TblFuncPROFINET_PARAM3_1, FuncPROFINET_PARAM3_1, sizeof(TblFuncPROFINET_PARAM3_1), 0, 0, 1),
OpisFuncBlock("PROFINET_PARAM3", "DD", TblFuncPROFINET_PARAM3_1, FuncPROFINET_PARAM3_1, sizeof(TblFuncPROFINET_PARAM3_1), sizeof(profinet_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPROFINET_PARAM3_1,
#endif
//********************************************************
