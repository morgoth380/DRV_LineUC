#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
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
    //************************************************************
    u16  Pnt_StatusOut;                    //32  StatusOut
    u16  Pnt_Fail;                         //33  Fail
    u16  Pnt_Warning;                      //34  Warning
    u16  Pnt_RotFreq2;                     //35  RotFreq2
    u16  Pnt_Current2;                     //36  Current2
    u16  Pnt_currTorq2;                    //37  currTorq2
    u16  Pnt_outVoltage1_1;                //38  outVoltage1
    u16  Pnt_driveTemp2;                   //39  driveTemp2
    u16  Pnt_encoPos2;                     //40  encoPos2
    u16  Pnt_currFreq2;                    //41  currFreq2
    u16  Pnt_DC_Volt2;                     //42  DC_Volt2
    u16  Pnt_queryCnt2;                    //43  queryCnt2
    u16  Pnt_curPower2;                    //44  curPower2
    u16  Pnt_freqRef2;                     //45  freqRef2
    u16  Pnt_torqRef2;                     //46  torqRef2
    u16  Pnt_torqMax2;                     //47  torqMax2
    u16  Pnt_torqMin2;                     //48  torqMin2
    u16  Pnt_queryCnt2_1;                  //49  queryCnt2
    u16  Pnt_CmdBit;                       //50  CmdBit
    u16  Pnt_driveMode;                    //51  driveMode
    u16  Pnt_ACS_enable;                   //52  ACS_enable
    u16  Pnt_RampStop;                     //53  RampStop
    u16  Pnt_FreqExtrStop;                 //54  FreqExtrStop
    u16  Pnt_FreqStop;                     //55  FreqStop
    u16  Pnt_GT_freez;                     //56  GT_freez
    u16  Pnt_WorkEnable;                   //57  WorkEnable
    u16  Pnt_ResOut1;                      //58  ResOut1
    u16  Pnt_ResOut2;                      //59  ResOut2
    u16  Pnt_ResOut3;                      //60  ResOut3
    u16  Pnt_ResOut4;                      //61  ResOut4
    u16  Pnt_ResOut5;                      //62  ResOut5
    u16  Pnt_End;
}FuncPROFINET_PARAM2_type;
//********************************************************

typedef enum{
        commandBit = 0  , //! 0 бит управления (ПУСК/СТОП)
        OFF2_bit        , //! 1 бит останова выбегом
        OFF3_bit        , //! 2 бит останова с темпом экстренного торможения
        reserve1        , //! 3 резерв
        ctrl1GT         , //! 4 разрешение 1 работы генератора темпа
        ctrl2GT         , //! 5 разрешение 2 работы генератора темпа
        ctrl3GT         , //! 6 разрешение 3 работы генератора темпа
        failReset       , //! 7 сигнал кввитирования аварий
        drive1Enable    , //! 8 выбор преобразователя 1 (разрешение работы ПЧ 1)
        drive2Enable    , //! 9 выбор преобразователя 2 (разрешение работы ПЧ 2)
        ICSctrlEnable   , //! 10 разрешение управления от АСУ
        reserve2        , //! 11 резерв
        drive1Master    , //! 12 если этот бит равен 1, то ПЧ1 - ведущий, иначе - ведомый
        drive2Master    , //! 13 если этот бит равен 1, то ПЧ2 - ведущий, иначе - ведомый
        reserve3        , //! 14 резерв
        reserve4          //! 15 резерв
}Cmd_bits;

typedef enum{
    drvNum1 = 0,
    drvNum2 = 1
}DrvNum;

typedef enum{
    notUseMode    = 3, //! Режим "Выведен из работы"
    singleMode    = 0, //! Режим "Одиночный"
    slaveMode     = 2, //! Режим "Ведомый"
    masterMode    = 1, //! Режим "Ведущий"
    errModeSelect = 4  //! Получен неверный код режима
}DrvMode;

typedef enum{
    code1 =    7, //! 0b1110 ПЧ1 - ведущий,         ПЧ2 - ведомый
    code2 =    1, //! 0b1000 ПЧ1 - одиночный,       ПЧ2 - не используется
    code3 =   11, //! 0b1101 ПЧ1 - ведомый,         ПЧ2 - ведущий
    code4 =    2  //! 0b0100 ПЧ1 - не используется, ПЧ2 - одиночный
}DrvModeBitCode;

typedef enum{
    local_remote = 0, //!Местный/дистанционный
    reserve  = 1,     //!
    freqDown = 3,     //! Бит процесса частотного торможения
    ready    = 4,     //! Готов
    drv_fail = 5,     //! Авария
    drv_ON   = 7,     //! Работа
}DrvStatusWord1;

typedef enum{
    rampDown      = 8, //!флаг выбега
    extrFreqStop = 13, //!Флаг частотного экстренного торможения. Добавить в резервный бит 13 статусного слова 2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    startForbid  = 15, //!Флаг наличия запрета пуска. Добавить в резервный бит 15 статусного слова 2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}DrvStatusWord2;

typedef enum{
    warn       = 0,  //!
    OFF2       = 1,  //!
    OFF3       = 2,  //!
    interlock  = 3,  //!
}DrvStatusWord4;

typedef enum{
    rampDec       = 0,  //!
    freqDec       = 1   //!
}DecModes;

#define FREQ_BASE 80
#define DISABLED 0
#define ENABLED 1
#define СHANEL2 1
#define ACS_CHEN 2
#define NOM_TORQ 2507 //базовый момент


u16 *FuncPROFINET_PARAM2_1(FuncPROFINET_PARAM2_type *progPnt, u32 ramPnt)
{
    _iq rotFreq, outFreq, current, outVoltage, DC_Voltage, curPower, freqRef2, temp_iq;
    static _iq prevRef = 0;
    u16 voltageBase, controlWord = 0, torqRef;
    u16 bit0, bit1, bit2, bit3, driveSelect, driveMode;
    float foutFreq, fbaseFreq;
    u16 maxPosPU, drvNum;
    s16 temp2;
    u16 temp, stOutBit0, stOutBit1, stOutBit2, stOutBit3, stOutBit4, stOutBit5;
    u16 stOutBit6, stOutBit7, stOutBit8, stOutBit9, stOutBit10, stOutBit11, stOutBit12;
    u16 stOutBit13_14;
    u16 drvStatusIn1, drvStatusIn2;
    u16 driveStatus, profiCtrl;
    u16 PWM_On;
    u16 off2, off3;
    u16 drv1En, drv2En, drv1Master, drv2Master;
    static u16 extrFreqDEC = 0;
    static u16 extrRampDEC = 0;
    static u16 firstICS_En = 0;

    static u16 decModeChange = 0; //!Флаг перенастройки режима торможения от слова управления сети profinet
    static u16 decMode = 0;       //!режим торможения на который перенастраиваемся
    static u16 prevChanNum = 0;
    static u16 changeFlag = 0;

    COMMONPARAMC28 *mPnt28;
    

    static u16 prevCmdBit = 0;
    static u16 cmdCnt = 0;
    if(flagIsFirstCall){
        return &progPnt->Pnt_End;
    }
	mPnt28 = &GlobalC28;
    rotFreq = load_s32(ramPnt, progPnt->Pnt_RotFreq1);    //!текущее значение частоты вращения ротора
    save_s32(ramPnt, progPnt->Pnt_RotFreq2, rotFreq);     //!выдаем текущее значение частоты вращения ротора

    maxPosPU = load_s16(ramPnt, progPnt->Pnt_maxPosPU);   //!32767

    /*********************Текущее предупреждение и авария***********************************/
    temp = load_s16(ramPnt, progPnt->Pnt_Reserve1);  //!Текущее предупреждение
    save_s16(ramPnt, progPnt->Pnt_Warning, temp);    //!Текущее предупреждение для АСУ PROFINET
    temp = load_s16(ramPnt, progPnt->Pnt_drv_fail);  //!Текущая авария
    save_s16(ramPnt, progPnt->Pnt_Fail, temp);       //!Текущая авария для АСУ PROFINET


    /***************************************************************************************/

    drvStatusIn1 = load_s16(ramPnt, progPnt->Pnt_Status1);   //!Статус 1
    drvStatusIn2 = load_s16(ramPnt, progPnt->Pnt_Status2);   //!Статус 2

    temp_iq = load_s32(ramPnt, progPnt->Pnt_currentBase);

    current = load_s32(ramPnt, progPnt->Pnt_Current1);    //!текущий полный ток
    temp = load_s16(ramPnt, progPnt->Pnt_currentBase);    //!база по току
    temp = (u16)((_IQtoF(current) * temp) / 10.0 + 0.5);  //!целое значение полного тока двигателя
    save_s16(ramPnt, progPnt->Pnt_Current2, temp);        //!полный ток для АСУ

    temp2 = load_s16(ramPnt, progPnt->Pnt_currTorq1);      //!текущий момент
    torqRef = load_s16(ramPnt, progPnt->Pnt_torqRef1);    //!задание момента
/*
    if (torqRef){
        temp = (s16)(((float)temp / NOM_TORQ) * 100 * 100 + 0.5);     //!относительный текущий момент в процентах с двумя знаками после запятой, приведенный к целому (20.00% = 2000)
    }else{
        temp = 0;
    }
    */
    temp2 = (s16)(((float)temp2 / NOM_TORQ) * 100 * 100 + 0.5);
    save_s16(ramPnt, progPnt->Pnt_currTorq2, temp2);       //!текущий момент для АСУ

    outVoltage = load_s32(ramPnt, progPnt->Pnt_outVoltage1); //!текущее выходное напряжение ПЧ
    voltageBase = load_s16(ramPnt, progPnt->Pnt_voltBase);   //!база по напряжению
    temp = (u16)(_IQtoF(outVoltage) * voltageBase + 0.5);    //!целое значение выходного напряжения
    save_s16(ramPnt, progPnt->Pnt_outVoltage1_1, temp);

    temp = load_s16(ramPnt, progPnt->Pnt_driveTemp1);       //!текущая температура ПЧ
    temp = (u16)(((float)temp / 10.0) + 0.5);               //!приводим температуру ПЧ к целому значению
    save_s16(ramPnt, progPnt->Pnt_driveTemp2, temp);

    temp = load_s16(ramPnt, progPnt->Pnt_encoPos1);         //!абсолютная позиция с одним знаком после запятой, привед. к целому
    temp = (u16)((((float)temp / 10.0) / 360.0) * maxPosPU + 0.5);


    temp = (u16)(_IQtoIQ15(_IQabs(mPnt28->Enco_Angle))) & 0x7FFF;

    save_s16(ramPnt, progPnt->Pnt_encoPos2, temp);

    outFreq = load_s32(ramPnt, progPnt->Pnt_currFreq1);      //!текущая выходная частота поля
    save_s32(ramPnt, progPnt->Pnt_currFreq2, outFreq);       //!выдаем текущую выходную частоту поля

    DC_Voltage = load_s32(ramPnt, progPnt->Pnt_DC_Volt1); //!напряжение звена Ud
    temp = (u16)(_IQtoF(DC_Voltage) * voltageBase * 10.0 + 0.5);        //!приводим к целому значению
    save_s16(ramPnt, progPnt->Pnt_DC_Volt2, temp);

    curPower = load_s32(ramPnt, progPnt->Pnt_curPower1);  //!текущая мощность, кВт
    temp = load_s16(ramPnt, progPnt->Pnt_powerBase);      //!база по мощности
    temp = (u16)((_IQtoF(curPower) * temp) / 10.0 + 0.5);        //!абсолютное значение мощности, приведенное к целому
    save_s16(ramPnt, progPnt->Pnt_curPower2, temp);

    /****************Преобразование значений, полученных от АСУ в формат ПЧ************************/
    profiCtrl = load_s16(ramPnt, progPnt->Pnt_RefACSForm);       //!Разрешение управления ПЧ от АСУ PROFINET
    if (profiCtrl == ENABLED){                                   //!Если разрешено управление от сети PROFINET
        rotFreq = load_s32(ramPnt, progPnt->Pnt_queryCnt1);  //!считываем новое задание скорости
        if (rotFreq < 0){ //!если новое задание отрицательное
            save_s32(ramPnt, progPnt->Pnt_freqRef2, _IQabs(rotFreq)); //!выдаем абсолютное значение задания
            setBitMicro(ramPnt, progPnt->Pnt_ResOut3);        //!выдаем сигнал реверса
        }else{ //!новое задание положительное
            save_s32(ramPnt, progPnt->Pnt_freqRef2, rotFreq); //!выдаем абсолютное значение задания
            clrBitMicro(ramPnt, progPnt->Pnt_ResOut3);        //!обнуляем сигнал реверса
        }

        temp_iq = load_s32(ramPnt, progPnt->Pnt_torqMax1);
//        //GlobalM3.TorqueMax = _IQabs(temp_iq); // При управлении от профинет - берем задания ограничений и дублируем штатные переменные в SVP_UNIT
//        AllPrmPnt->UST_TORQUE_MAX = _IQtoF(temp_iq) * GlobalM3.TorqBASE * 10.f;
//        save_s32(ramPnt, progPnt->Pnt_torqMax2, temp_iq);
        temp_iq = load_s32(ramPnt, progPnt->Pnt_torqMin1);
//        //GlobalM3.TorqueMin = _IQabs(temp_iq); // При управлении от профинет - берем задания ограничений и дублируем штатные переменные в SVP_UNIT
//        AllPrmPnt->UST_TORQUE_MIN = _IQtoF(temp_iq) * GlobalM3.TorqBASE * 10.f;
//        save_s32(ramPnt, progPnt->Pnt_torqMin2, temp_iq);


        controlWord = load_s16(ramPnt, progPnt->Pnt_command1);  //!слово управления

        /*****************Управление командами ПУСК/СТОП от PROFINET***************************************/
        //temp = controlWord & ((u16)1 << commandBit); //!Проверка бита 0 слова управления
        temp = controlWord & ((u16)1 << commandBit);   //!Состояние бита ПУСК/СТОП

        if (firstICS_En){                                //!Если только что разрешили управление от АСУ, то firstICS_En = 0
            if (temp != prevCmdBit){                     //!если состояние изменилось
                if(temp){                                //!если пришел сигнал 1
                    defBitMicro(ramPnt, progPnt->Pnt_CmdBit, 1); //!выдаем ПУСК
                    cmdCnt = 5;                          //!тикнем 5 раз прежде чем снять команду
                }else{
                    defBitMicro(ramPnt, progPnt->Pnt_ResOut1, 1);
                    cmdCnt = 5;                          //!тикнем 5 раз прежде чем снять команду
                }
                prevCmdBit = temp;
            }
            if (cmdCnt){
                cmdCnt--;
            }
            else{ //!если счетчик дотикал
                defBitMicro(ramPnt, progPnt->Pnt_CmdBit, 0);  //!снимаем сигнал ПУСК
                defBitMicro(ramPnt, progPnt->Pnt_ResOut1, 0); //!снимаем сигнал СТОП
            }
        }else if ((!firstICS_En) && (!temp)){ //!Если при разрешении управления от АСУ, команда ПУСК не висит..
            firstICS_En = 1; //!..Разрешаем формирование команды пуска (для того, что бы не было непредвиденного пуска при разрешении управления от АСУ)
        }


        PWM_On = (drvStatusIn1 >> 8) & (1 << drv_ON);    //!текущеее состояние "включен/отключен"
        off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!сигнал экстренного останова выбегом
        off3 = !(controlWord & (1 << OFF3_bit));         //!сигнал экстренного останова с экстренным временем торможения

        /***********************Управление остановом по выбегу*************************************************/
        if (off2 && !extrFreqDEC){                         //!если экстренный останов выбегом но при этом не экстренного частотного останова:
            save_s16(ramPnt, progPnt->Pnt_RampStop, 0);    //!перенастраиваем тип торможения на выбег
            stOutBit4 = 0;                                 //!Флаг экстренного останова выбегом (в данном случае экстренный останов инициирован самой АСУ) Но такой стоп должен быть и от дискретного входа
        }
        else {
            stOutBit4 = 1;      //!Сняли команду экстренного останова - ставим флаг
        }


       /*******************************Управление экстренным частотным торможением***********************************************************/
        if (off3 && !off2){           //!если нужно остановиться с экстренным заданным темпом
                extrFreqDEC = 1;
                save_s16(ramPnt, progPnt->Pnt_RampStop, extrFreqDEC);     //!перенастраиваем тип торможения на частотое
                defBitMicro(ramPnt, progPnt->Pnt_FreqExtrStop, extrFreqDEC); //!бит выбора экстренного времени торможения
                stOutBit5 = 0;                                           //!флаг экстренного частотного торможения
        }else if (extrFreqDEC && (!PWM_On)){ //!снимем время экстренного торможения только после фактического останова привода
            extrFreqDEC = 0;
            defBitMicro(ramPnt, progPnt->Pnt_FreqExtrStop, extrFreqDEC); //!бит выбора обычного времени торможения (т. е. не экстренного)
        }else if (!extrFreqDEC){                                         //!если экстренного торможения не было то обычное время торможения
            stOutBit5 = 1;
            defBitMicro(ramPnt, progPnt->Pnt_FreqExtrStop, extrFreqDEC); //!бит выбора обычного времени торможения (т. е. не экстренного)
        }
       /************************************************************************************************************************************/

       /*
        if (!(controlWord & ((u16)1 << ctrl1GT))){
            save_s16(ramPnt, progPnt->Pnt_ResOut2, 0); //!время торможения = 0
            save_s32(ramPnt, progPnt->Pnt_freqRef2, 0);//!задание = 0
        }
      */
        if ((!(controlWord & ((u16)1 << ctrl2GT))) && PWM_On){    //!заморозка разгона/торможения
            defBitMicro(ramPnt, progPnt->Pnt_GT_freez, 1);
        }else{
            defBitMicro(ramPnt, progPnt->Pnt_GT_freez, 0);   //!ШИМ отключен - нет заморозки частоты
        }

        if (!(controlWord & ((u16)1 << ctrl3GT))){      //!обычное частотное торможение
            save_s32(ramPnt, progPnt->Pnt_freqRef2, 0); //!задание = 0
        }

        drv1En = (controlWord >> drive1Enable) & 0x01; //!Бит 8  слова управления
        drv2En = (controlWord >> drive2Enable) & 0x01; //!Бит 9  слова управления
        drv1Master = (controlWord >> drive1Master) & 0x01; //!Бит 12 слова управления
        drv2Master = (controlWord >> drive2Master) & 0x01; //!Бит 13 слова управления

        //driveSelect = (bit0 << 0) | (bit1 << 1) | (bit2 << 2) | (bit3 << 3); //!4-х битный код режима работы ПЧ
        drvNum = load_s16(ramPnt, progPnt->Pnt_drvNum); //!Номер преобразователя в системе ведущий/ведомый (1 или 2). Каждый из ПЧ может быть как ведущим, таки ведомым

     if(drv1En && drv2En){
    	 if((drv1Master == 1) && (drv2Master == 0)){
    		 driveMode = masterMode;  //!Режим "Ведущий"
    		 stOutBit13_14 = 1;       //!ведущий
    	 }else if((drv1Master == 0) && (drv2Master == 1)){
    		 driveMode = slaveMode;   //!Режим "Ведомый"
    		 stOutBit13_14 = 2;
    	 }

     }else if(drv1En && !drv2En){
    	 if(drvNum == drvNum1){
    		 driveMode = singleMode;  //!Режим "Одиночный"
    		 stOutBit13_14 = 0;
    	 }else if(drvNum == drvNum2){
    		 driveMode = notUseMode; //!Режим "Выведен из работы"
    		 stOutBit13_14 = 3;
    	 }

     }else if (!drv1En && drv2En){
    	 if(drvNum == drvNum1){
    		 driveMode = notUseMode; //!Режим "Выведен из работы"
    		 stOutBit13_14 = 3;
    	 }else if(drvNum == drvNum2){
    		 driveMode = singleMode;  //!Режим "Одиночный"
    		 stOutBit13_14 = 0;
    	 }
     }

     save_s16(ramPnt, progPnt->Pnt_driveMode, driveMode); //!Выдаем код режима работы ПЧ. driveMode = 0, 1, 2, 3

     if (controlWord & (1 << ICSctrlEnable)){                   //!если управление по сети разрешено
         prevChanNum = (!changeFlag) ? load_s16(ramPnt, progPnt->Pnt_Reserve4): prevChanNum; //!перед перенастройкой запоминаем текущий номер канала управления
         changeFlag = 1;                                        //!флаг изменения номера канала управления
         save_s16(ramPnt, progPnt->Pnt_ACS_enable, СHANEL2);    //!переключение на канал управления 2 ("разрешение управления по сети")
 //ВРЕМЕННО        save_s16(ramPnt, progPnt->Pnt_ResOut2, ACS_CHEN);      //!настройка канала управления 2 на АСУ
     }else if (changeFlag){                                     //!если управление от АСУ запрещено, но до этого было разрешено
         save_s16(ramPnt, progPnt->Pnt_ACS_enable, prevChanNum);//!возарат в предыдущий номер канала управления
         changeFlag = 0;
     }
     defBitMicro(ramPnt, progPnt->Pnt_WorkEnable, controlWord & (1 << failReset)); //!квитирование аварий

  }else{
      stOutBit13_14 = load_s16(ramPnt, progPnt->Pnt_Reserve3);  //!управление от profinet запрещено - считываем режим (ведущий, ведомый и т. д.), заданный вручную
      stOutBit4 = 1;
      stOutBit5 = 1;
      firstICS_En = 0;
      if (changeFlag){  //!Если перед отключением управления от PROFINET было разрешение управления по сети от слова управления..
          save_s16(ramPnt, progPnt->Pnt_ACS_enable, prevChanNum);//!возврат в предыдущий номер канала управления
          changeFlag = 0;
      }
  }

    temp = load_s16(ramPnt, progPnt->Pnt_queryCnt1_1); //счетчик запросов. ниче с ним не делаем
    save_s16(ramPnt, progPnt->Pnt_ResOut4, temp);

    stOutBit0 = ((drvStatusIn1 >> 8) & (1 << ready)) >> ready;       //!Состояние привода "Готов"
    stOutBit2 = ((drvStatusIn1 >> 8) & (1 << drv_ON)) >> drv_ON;     //!Состояние привода "Работа"
    stOutBit0 = stOutBit0 || stOutBit2;                              //!Если привод в работе, то сигнала "Готов" все равно будем выдавать
    stOutBit3 = ((drvStatusIn1 >> 8) & (1 << drv_fail)) >> drv_fail; //!Состояние привода "Авария"
    stOutBit9 = profiCtrl ? 1 : 0;                                   //!бит индикации разрешенного управления от АСУ
    temp = load_s16(ramPnt, progPnt->Pnt_Reserve1);      //!код предупреждения
    stOutBit7 = temp ? 1 : 0;                            //!бит предупреждения
    //stOutBit4 = (drvStatusIn2 >> 8) & (1 << rampDown) ? 1 : 0; //!флаг процесса останова выбегом
    //stOutBit5 =  (drvStatusIn1 >> 8) & (1 << freqDown) ? 1 : 0; //!должен быть флаг экстренного частотного отсанова. но в данном случае просто флаг частотного останова

    stOutBit6 = testBitMicro(ramPnt, progPnt->Pnt_Reserve2) ? 1 : 0;    //!состояние блокировки от дискретного входа. 1 - нет блокировки
    stOutBit11 = GlobalC28.FlagState.bit.vPrUp;                     //!флаг "двигатель намагничен"
    stOutBit12 = GlobalM3.FlagState.bit.DRVNormRectiefer;			//  Флаг успешной работы выпрямителя - Из DRV_UD
    driveStatus = stOutBit0  | (stOutBit2 << 2) | (stOutBit3 << 3) | (stOutBit4 << 4) | (stOutBit5 << 5) | (stOutBit6 << 6) |
                  (stOutBit7 << 7) | (stOutBit9 << 9) | (stOutBit11 << 11) | (stOutBit12 << 12) |  (stOutBit13_14 << 13);
    save_s16(ramPnt, progPnt->Pnt_StatusOut, driveStatus);

 save_s16(ramPnt, progPnt->Pnt_ResOut2, 0); //ВРЕМЕННО!!!!!!!!
  save_s16(ramPnt, progPnt->Pnt_ResOut5, 0); //ВРЕМЕННО!!!!!!!!

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPROFINET_PARAM2_1[62]={
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
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  queryCnt1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  curPower1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  freqRef1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  torqRef1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  torqMax1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  torqMin1
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
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  StatusOut
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //33  Fail
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //34  Warning
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //35  RotFreq2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //36  Current2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //37  currTorq2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //38  outVoltage1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //39  driveTemp2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //40  encoPos2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //41  currFreq2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //42  DC_Volt2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //43
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //44  curPower2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //45  freqRef2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //46  torqRef2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //47  torqMax2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //48  torqMin2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //49  queryCnt2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //50  CmdBit
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //51  driveMode
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //52  ACS_enable
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //53  RampStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //54  FreqExtrStop
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //55  FreqStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //56  GT_freez
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //57  WorkEnable
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //58  ResOut1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //59  ResOut2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //60  ResOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //61  ResOut4
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //62  ResOut5
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PROFINET_PARAM2", "DA", TblFuncPROFINET_PARAM2_1, FuncPROFINET_PARAM2_1, sizeof(TblFuncPROFINET_PARAM2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPROFINET_PARAM2_1,
#endif
//********************************************************
