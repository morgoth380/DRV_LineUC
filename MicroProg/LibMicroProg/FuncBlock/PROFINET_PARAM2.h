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
        commandBit = 0  , //! 0 команда управления (ПУСК/СТОП)
        OFF2_bit        , //! 1 команда аварийного останова выбегом
        OFF3_bit        , //! 2 команда аварийного останова с темпом экстренного торможения
        reserve1        , //! 3 резерв
        ctrl1GT         , //! 4 разрешение 1 работы выхода генератора темпа.
        ctrl2GT         , //! 5 разрешение 2 работы генератора темпа
        ctrl3GT         , //! 6 разрешение 3 работы генератора темпа
        failReset       , //! 7 сигнал кввитирования аварий
        drive1Enable    , //! 8 выбор преобразователя 1 (разрешение работы ПЧ 1)
        drive2Enable    , //! 9 выбор преобразователя 2 (разрешение работы ПЧ 2)
        ICSctrlEnable   , //! 10 разрешение управления по сети
        reserve2        , //! 11 резерв
        drive1Master    , //! 12 если этот бит равен 1, то ПЧ1 - ведущий, иначе - ведомый
        drive2Master    , //! 13 если этот бит равен 1, то ПЧ2 - ведущий, иначе - ведомый
        reserve3        , //! 14 резерв
        reserve4          //! 15 резерв
}Cmd_bits;

/*******Состояния системы управления по сети PROFINET*********/
typedef enum{
	Off2State,          //!Состояние "Экстренный останов выбегом"
	Off3State,          //!Состояние "Экстренный останов частотный"
	driveFaultState,    //!Состояние "Авария ПЧ"
	interlockState,     //!Состояние "Запрет"
	notReadyState,      //!Состояние "Не готов к пуску"
	readyState,         //!Состояние "Готов к пуску"
	operationEnState,   //!Состояние "Работа" ("ШИМ включен")
	rampOutEnState,     //!Состояние "Разрешена работа выхода генератора темпа"
	accDeccEnState,     //!Состояние "Разрешен разгон/торможение"
	operateState,       //!Состояние "Работа"
}profinetCmdStateType;

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
    local_remote = 0, //!Местный/дистанционный
    reserve  = 1,     //!
    freqDown = 3,     //! Бит процесса частотного торможения
    ready    = 4,     //! Готов
    drv_fail = 5,     //! Авария
    drv_ON   = 7,     //! Работа
}DrvStatusWord1;

typedef struct{
	profinetCmdStateType     profinetCmdState;
	u16 StateCnt;
	u16 offStateCnt;
	u16  prevPWMOnCmd;
	u16  prevPWMOffCmd;
}profinet_State_type;

#define ENABLED 1
#define NOM_TORQ 2507 //базовый момент


u16 *FuncPROFINET_PARAM2_1(FuncPROFINET_PARAM2_type *progPnt, u32 ramPnt)
{
    _iq rotFreq, outFreq, current, outVoltage, DC_Voltage, curPower, temp_iq;
    u16 voltageBase, controlWord = 0;
    u16 driveMode;
    u16 maxPosPU, drvNum;
    s16 temp2;
    u16 temp, stateBit0 = 0, stateBit2 = 0, stateBit3 = 0, stateBit4 = 1, stateBit5 = 1;
    u16 stateBit6 = 0, stateBit7 = 0, stateBit9 = 0, stateBit11 = 0, stateBit12 = 0;
    u16 stateBit13_14 = 0;
    u16 drvStatusIn1;
    u16 driveStatus, profiCtrl;
    u16 PWM_On;
    u16 off2, off3;
    u16 drv1En, drv2En, drv1Master, drv2Master;
    u16 failOff;
    u16 PWMOnCmd = 0;
    u16 PWMOffCmd = 0;
    u16 rampTime;
    profinet_State_type *sPnt;
    COMMONPARAMC28 *mPnt28;
    sPnt = (profinet_State_type *)(ramPnt + progPnt->Pnt_State);
    
    if(flagIsFirstCall){
    	sPnt->StateCnt = 0;
    	sPnt->offStateCnt = 0;
    	sPnt->prevPWMOnCmd = 0;
    	sPnt->profinetCmdState = interlockState; //!Исходное состояние "Запрет. Ожидание обнуления командного бита".
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

    current = load_s32(ramPnt, progPnt->Pnt_Current1);    //!текущий полный ток

    temp = load_s16(ramPnt, progPnt->Pnt_currentBase);    //!база по току

    temp = (u16)((_IQtoF(current) * temp) / 10.0 + 0.5);  //!целое значение полного тока двигателя

    save_s16(ramPnt, progPnt->Pnt_Current2, temp);        //!полный ток для АСУ

    temp2 = load_s16(ramPnt, progPnt->Pnt_currTorq1);      //!текущий момент

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

    controlWord = load_s16(ramPnt, progPnt->Pnt_command1);     //!слово управления

    profiCtrl = load_s16(ramPnt, progPnt->Pnt_RefACSForm);     //!Разрешение управления ПЧ от АСУ PROFINET

	if((profiCtrl == ENABLED) && (controlWord & (1 << ICSctrlEnable))){
		stateBit9 = 1; //!Флаг включенного управления по сети для слова состояния привода
		switch(sPnt->profinetCmdState){
		case interlockState:                                 //!Состояние "Запрет". По сути - это ожидание обнуления 0-го бита слова управления. Исходное состояние после подачи питания
			setBitMicro(ramPnt, progPnt->Pnt_ResOut2);       //!Запрещаем работу выхода генератора темпа - исходное состояние
			setBitMicro(ramPnt, progPnt->Pnt_GT_freez);      //!Запрещаем разгон и торможение - исходное состояние
			setBitMicro(ramPnt, progPnt->Pnt_ResOut5);       //!Устанавливаем нулевое задание на вход генератора темпа - исходное состояние
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!авария ПЧ
			temp = controlWord & ((u16)1 << commandBit);     //!Состояние бита ПУСК/СТОП
			if(failOff){ //!если авария ПЧ
				sPnt->profinetCmdState = driveFaultState; //!Переход в состояние "Авария ПЧ"
			}else if(temp){                               //!Пока пуск не снят, будем висеть в этом состоянии
				stateBit6 = 1;                            //!Индикация состояния "Запрет"
			}else if(!temp){                              //!пуск снят
				stateBit6 = 0;                            //!сбрасываем флаг состояния "Запрет"
				sPnt->profinetCmdState = notReadyState;   //!переходим в состояние "Не готов к пуску"
			}
			break;
		case notReadyState: //!Состояние "Не готов к пуску"
			stateBit0 = 0;  //!Сброс флага "Готовности к пуску"
			off2 = !(controlWord & ((u16)1 << OFF2_bit));            //!сигнал экстренного останова выбегом
			off3 = !(controlWord & (1 << OFF3_bit));                 //!сигнал экстренного останова с экстренным временем торможения
			if((drvStatusIn1 >> 8) & (1 << drv_fail)){               //!если есть  авария ПЧ
				sPnt->profinetCmdState = driveFaultState;            //!переход в состояние "Авария ПЧ"
			}else if(off2 || off3){                                  //!если висят команды экстренного останова..
				sPnt->profinetCmdState = sPnt->profinetCmdState;     //!..ничего не делаем. Крутимся в состоянии notReadyState
			}else{                                                   //!Иначе никаких блокировок нет
				sPnt->profinetCmdState = ((drvStatusIn1 >> 8) & (1 << ready)) ? readyState : sPnt->profinetCmdState; //!Переход в состояние "Готов к пуску" если ПЧ готов
			}
			break;
		case readyState:      //!Состояние "Готов к пуску"
			stateBit0 = 1;    //!Установка флага "Готов к пуску" если перешли в состояние готов и ПЧ по факту ГОТОВ
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!сигнал экстренного останова выбегом
			off3 = !(controlWord & (1 << OFF3_bit));         //!сигнал экстренного останова с экстренным временем торможения
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail);  //!авария ПЧ
			if(failOff){
				sPnt->profinetCmdState = driveFaultState;     //!если ПЧ находится в аварии, переход в состояние "Авария ПЧ"
			}else if (off2){
				sPnt->profinetCmdState = Off2State;           //!если есть off2, переходим в состояние "Экстренный останов выбегом"
			}else if (off3){
				sPnt->profinetCmdState =  Off3State;          //!если есть off3, переходим в состояние "Экстренный останов частотный"
			}else{                                            //!Иначе блокировок нет
				temp = controlWord & ((u16)1 << commandBit);   //!Состояние бита ПУСК/СТОП
				sPnt->profinetCmdState = temp ? operationEnState: sPnt->profinetCmdState; //!переход в состояние "Работа" ("ШИМ включен")
			}
			break;
		case operationEnState:                               //!Состояние "Работа" ("ШИМ включен")
			temp = controlWord & ((u16)1 << commandBit);     //!Состояние бита ПУСК/СТОП
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!авария ПЧ
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!сигнал экстренного останова выбегом
			off3 = !(controlWord & (1 << OFF3_bit));         //!сигнал экстренного останова с экстренным временем торможения
			if (failOff){                                    //!если есть авария ПЧ
				sPnt->profinetCmdState = driveFaultState;    //! Переход в состояние "Авария ПЧ"
			}else if (!temp){                              //!если поступила команда СТОП
				PWMOffCmd = 1;                             //!выключаем ШИМ
				sPnt->profinetCmdState = readyState;       //!Переход в состояние "Готов к пуску"
			}else if (off2){                               //!сигнал экстренного останова выбегом
				sPnt->profinetCmdState = Off2State;        //!если есть off2, переходим в состояние "Экстренный останов выбегом"
			}else if (off3){                               //!сигнал экстренного останова с экстренным временем торможения
				sPnt->profinetCmdState = Off3State;        //!если есть off3, переходим в состояние "Экстренный останов частотный"
			}else{                                         //!Иначе, никаких блокировок нет, можем включать ШИМ
				PWMOnCmd = 1;                              //!Включаем ШИМ
				sPnt->profinetCmdState = (controlWord & ((u16)1 << ctrl1GT)) ? rampOutEnState : sPnt->profinetCmdState; //!Разрешена работа выхода генератора темпа, переход в состояние "Разрешена работа выхода генератора темпа"
			}
			break;
		case rampOutEnState:                                 //!Состояние "Разрешена работа выхода генератора темпа"
			temp = controlWord & ((u16)1 << commandBit);     //!Состояние бита ПУСК/СТОП
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!авария ПЧ
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!сигнал экстренного останова выбегом
			off3 = !(controlWord & (1 << OFF3_bit));         //!сигнал экстренного останова с экстренным временем торможения
			if (failOff){                                    //!если есть авария ПЧ
				sPnt->profinetCmdState = driveFaultState;    //! Переход в состояние "Авария ПЧ"
			}else if (!temp){                              //!если поступила команда СТОП
				PWMOffCmd = 1;                             //!выключаем ШИМ
				sPnt->profinetCmdState = readyState;       //!Переход в состояние "Готов к пуску"
			}else if (off2){                               //!сигнал экстренного останова выбегом
				sPnt->profinetCmdState = Off2State;        //!если есть off2, переходим в состояние "Экстренный останов выбегом"
			}else if (off3){                               //!сигнал экстренного останова с экстренным временем торможения
				sPnt->profinetCmdState = Off3State;        //!если есть off3, переходим в состояние "Экстренный останов частотный"
			}else if (!(controlWord & ((u16)1 << ctrl1GT))){ //!если выход генератора темпа снова запрещен
				setBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!Запрещаем работу выхода генератора темпа
				sPnt->profinetCmdState = operationEnState; //!Возврат в состояние "Работа" ("ШИМ включен")
			}else{                                         //!Иначе, блокировок нет.
				clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!Разрешаем работу выхода генератора темпа
				sPnt->profinetCmdState = (controlWord & ((u16)1 << ctrl2GT)) ? accDeccEnState : sPnt->profinetCmdState; //!Разрешена разгон/торможение, переход в состояние "Разрешен разгон/торможение"
			}
			break;
		case accDeccEnState:                                 //!Состояние "Разрешен разгон/торможение"
			temp = controlWord & ((u16)1 << commandBit);     //!Состояние бита ПУСК/СТОП
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!авария ПЧ
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!сигнал экстренного останова выбегом
			off3 = !(controlWord & (1 << OFF3_bit));         //!сигнал экстренного останова с экстренным временем торможения
			if (failOff){                                    //!если есть авария ПЧ
				sPnt->profinetCmdState = driveFaultState;    //! Переход в состояние "Авария ПЧ"
			}else if (!temp){                                //!если поступила команда СТОП
				PWMOffCmd = 1;                               //!выключаем ШИМ
				sPnt->profinetCmdState = readyState;         //!Переход в состояние "Готов к пуску"
			}else if (off2){                                 //!сигнал экстренного останова выбегом
				sPnt->profinetCmdState = Off2State;          //!если есть off2, переходим в состояние "Экстренный останов выбегом"
			}else if (off3){                                 //!сигнал экстренного останова с экстренным временем торможения
				sPnt->profinetCmdState = Off3State;          //!если есть off3, переходим в состояние "Экстренный останов частотный"
			}else if (!(controlWord & ((u16)1 << ctrl2GT))){ //!если разгон/торможения снова снова запрещен
				setBitMicro(ramPnt, progPnt->Pnt_GT_freez);  //!Запрещаем разгон/торможение
				sPnt->profinetCmdState = rampOutEnState;     //!Возврат в состояние "Разрешена работа выхода генератора темпа"
			}else if (!(controlWord & ((u16)1 << ctrl1GT))){ //!Если работа генератора темпа запрещена
				setBitMicro(ramPnt, progPnt->Pnt_ResOut2);   //!Запрещаем работу выхода генератора темпа
				sPnt->profinetCmdState = operationEnState;   //!возвтрат в состояние "Работа" ("ШИМ включен")
			}else{ //!Иначе блокировок нет, все разрешено, можем переходить к след. состоянию если разрешено
				clrBitMicro(ramPnt, progPnt->Pnt_GT_freez);  //!Разрешаем разгон/торможение
				sPnt->profinetCmdState = (controlWord & ((u16)1 << ctrl3GT)) ? operateState : sPnt->profinetCmdState; //!Разрешена разгон/торможение, переход в состояние "Разрешен разгон/торможение"
			}
			break;
		case operateState:                                   //!Состояние "Работа". Все поехало
			temp = controlWord & ((u16)1 << commandBit);     //!Состояние бита ПУСК/СТОП
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!авария ПЧ
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!сигнал экстренного останова выбегом
			off3 = !(controlWord & (1 << OFF3_bit));         //!сигнал экстренного останова с экстренным временем торможения
			if (failOff){                                    //!если есть авария ПЧ
				sPnt->profinetCmdState = driveFaultState;    //! Переход в состояние "Авария ПЧ"
			}else if (!temp){                                //!если поступила команда СТОП
				PWMOffCmd = 1;                               //!выключаем ШИМ
				sPnt->profinetCmdState = readyState;         //!Переход в состояние "Готов к пуску"
			}else if (off2){                                 //!сигнал экстренного останова выбегом
				sPnt->profinetCmdState = Off2State;          //!если есть off2, переходим в состояние "Экстренный останов выбегом"
			}else if (off3){                                 //!сигнал экстренного останова с экстренным временем торможения
				sPnt->profinetCmdState = Off3State;          //!если есть off3, переходим в состояние "Экстренный останов частотный"
			}else if(!(controlWord & ((u16)1 << ctrl1GT))){  //!если запрещена работа выхода генератора темпа
				setBitMicro(ramPnt, progPnt->Pnt_ResOut2);   //!Запрещаем работу выхода генератора темпа
				sPnt->profinetCmdState = operationEnState;   //!переход в состояние "Работа" ("ШИМ включен")
			}else if (!(controlWord & ((u16)1 << ctrl2GT))){ //!если запрещен разгон/торможение
				setBitMicro(ramPnt, progPnt->Pnt_GT_freez);  //!Запрещаем разгон/торможение
				sPnt->profinetCmdState = rampOutEnState;     //!переход в состояние "Разрешена работа выхода генератора темпа"
			}else if(!(controlWord & ((u16)1 << ctrl3GT))){  //!если запрещена работа входа генератора темпа
				setBitMicro(ramPnt, progPnt->Pnt_ResOut5);   //!Устанавливаем нулевое задание на вход генератора темпа
				sPnt->profinetCmdState = accDeccEnState;     //!Возврат в состояние "Разрешен разгон/торможение"
			}else{
				clrBitMicro(ramPnt, progPnt->Pnt_ResOut5);   //!разрешаем проход задания на вход генератора темпа
			}
			break;
		case Off2State:                                    //!состояние "Экстренный останов выбегом"
			setBitMicro(ramPnt, progPnt->Pnt_RampStop);    //!Тип торможения - выбег
			stateBit4 = 0;                                 //!Флаг экстренного останова выбегом для слова состояния (в данном случае экстренный останов инициирован самой АСУ) Но такой стоп должен быть и от дискретного входа
			PWMOffCmd = 1;                                 //!Команда СТОП
			rampTime = load_s16(ramPnt, progPnt->Pnt_Reserve4);     //!Время выбега
			if(sPnt->offStateCnt < (u16)((rampTime /4.0) * 1000)){  //!Ждем истечения времени выбега
				sPnt->offStateCnt++;
			}else{                                                   //!Дождались истечения времени выбега
				sPnt->offStateCnt = 0;
				stateBit4 = 1; //!Снимаем признак экстренного останова off2
				clrBitMicro(ramPnt, progPnt->Pnt_RampStop); //!Снимаем принудительную установку типа торможения выбегом
				sPnt->profinetCmdState = interlockState;    //!Переходим в состояние "Запрет после аварии"
			}
			break;
		case Off3State:                                    //!состояние "Экстренный останов частотный"
			setBitMicro(ramPnt, progPnt->Pnt_FreqStop);    //!Тип торможения - частотное
			setBitMicro(ramPnt, progPnt->Pnt_FreqExtrStop);//!выбор параметра экстренного времени торможения
			stateBit5 = 0;                                 //!Флаг экстренного останова с экстренным темпом
			PWMOffCmd = 1;                                 //!Команда СТОП
			PWM_On = (drvStatusIn1 >> 8) & (1 << drv_ON);  //!текущеее состояние "включен/отключен"
			if(!PWM_On){                                   //!если дождались отключения ШИМ
				clrBitMicro(ramPnt, progPnt->Pnt_FreqStop);     //!Снимаем принудительную установку частотного типа торможения
				clrBitMicro(ramPnt, progPnt->Pnt_FreqExtrStop); //!Снимаем принудительную установку экстренного времени торможения
				stateBit5 = 1;                                  //!Снимаем признак экстренного останова off3
				sPnt->profinetCmdState = interlockState;        //!Переходим в состояние "Запрет после аварии"
			}
			break;
		case driveFaultState:                                 //!Состояние "Авария ПЧ"
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail);  //!если есть  авария ПЧ
			if(!failOff){                                     //!Выйдем из этого состояния только после исчезновения аварии и квитирования
				sPnt->profinetCmdState = interlockState;      //!Переходим в состояние "Запрет после аварии"
				stateBit3 = 0;                                //!Снимаем флаг аварии
			}
			break;
		}

		rotFreq = load_s32(ramPnt, progPnt->Pnt_queryCnt1);           //!считываем новое задание скорости
		if (rotFreq < 0){                                             //!если новое задание отрицательное
			save_s32(ramPnt, progPnt->Pnt_freqRef2, _IQabs(rotFreq)); //!выдаем абсолютное значение задания
			setBitMicro(ramPnt, progPnt->Pnt_ResOut3);                //!выдаем сигнал реверса
		}else{ //!новое задание положительное
			save_s32(ramPnt, progPnt->Pnt_freqRef2, rotFreq); //!выдаем абсолютное значение задания
			clrBitMicro(ramPnt, progPnt->Pnt_ResOut3);        //!обнуляем сигнал реверса
		}
	/*		temp_iq = load_s32(ramPnt, progPnt->Pnt_torqMax1);
			AllPrmPnt->UST_TORQUE_MAX = _IQtoF(temp_iq) * GlobalM3.TorqBASE * 10.f;
			save_s32(ramPnt, progPnt->Pnt_torqMax2, temp_iq);
			temp_iq = load_s32(ramPnt, progPnt->Pnt_torqMin1);
			AllPrmPnt->UST_TORQUE_MIN = _IQtoF(temp_iq) * GlobalM3.TorqBASE * 10.f;
			save_s32(ramPnt, progPnt->Pnt_torqMin2, temp_iq);*/

        /**********Определение режима работы ПЧ (Ведущий, Ведомый и т.д.)***************/
        drv1En = (controlWord >> drive1Enable) & 0x01;     //!Бит 8  слова управления
        drv2En = (controlWord >> drive2Enable) & 0x01;     //!Бит 9  слова управления
        drv1Master = (controlWord >> drive1Master) & 0x01; //!Бит 12 слова управления
        drv2Master = (controlWord >> drive2Master) & 0x01; //!Бит 13 слова управления
        drvNum = load_s16(ramPnt, progPnt->Pnt_drvNum);    //!Номер преобразователя в системе ведущий/ведомый (1 или 2). Каждый из ПЧ может быть как ведущим, таки ведомым
		 if(drv1En && drv2En){
			 if((drv1Master == 1) && (drv2Master == 0)){
				 driveMode = masterMode;  //!Режим "Ведущий"
				 stateBit13_14 = 1;       //!ведущий
			 }else if((drv1Master == 0) && (drv2Master == 1)){
				 driveMode = slaveMode;   //!Режим "Ведомый"
				 stateBit13_14 = 2;
			 }
		 }else if(drv1En && !drv2En){
			 if(drvNum == drvNum1){
				 driveMode = singleMode;  //!Режим "Одиночный"
				 stateBit13_14 = 0;
			 }else if(drvNum == drvNum2){
				 driveMode = notUseMode; //!Режим "Выведен из работы"
				 stateBit13_14 = 3;
			 }
		 }else if (!drv1En && drv2En){
			 if(drvNum == drvNum1){
				 driveMode = notUseMode; //!Режим "Выведен из работы"
				 stateBit13_14 = 3;
			 }else if(drvNum == drvNum2){
				 driveMode = singleMode;  //!Режим "Одиночный"
				 stateBit13_14 = 0;
			 }
		 }

		 save_s16(ramPnt, progPnt->Pnt_driveMode, driveMode); //!Выдаем код режима работы ПЧ. driveMode = 0, 1, 2, 3
		 defBitMicro(ramPnt, progPnt->Pnt_WorkEnable, controlWord & (1 << failReset)); //!квитирование аварий
	}else{ //!Управление по сети запрещено
		sPnt->profinetCmdState = interlockState;         //!Сброс командоаппарата управления по profinet в исходное состояние
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2);       //!Разрешаем выход генератора темпа
		clrBitMicro(ramPnt, progPnt->Pnt_GT_freez);      //!Разрешаем  разгон и торможение
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut5);       //!Разрешаем  вход генератора темпа
		stateBit13_14 = load_s16(ramPnt, progPnt->Pnt_Reserve3);     //!управление от profinet запрещено - считываем режим (ведущий, ведомый и т. д.), заданный вручную
		stateBit0 = ((drvStatusIn1 >> 8) & (1 << ready)) >> ready;   //!Состояние ПЧ "Готов"
		stateBit4 = 1;                                               //!Нет экстренного торможения выбегом
		stateBit5 = 1;                                               //!Нет экстренного торможения с экстренным темпом
		stateBit9 = 0;                                               //!Нет состояния "Запрет"
	}
	stateBit2 = ((drvStatusIn1 >> 8) & (1 << drv_ON)) >> drv_ON;
	stateBit3 = ((drvStatusIn1 >> 8) & (1 << drv_fail)) >> drv_fail; //!Состояние привода "Авария"
	stateBit11 = GlobalC28.FlagState.bit.vPrUp;            //!флаг "двигатель намагничен"
	stateBit12 = GlobalM3.FlagState.bit.DRVNormRectiefer;  //!Флаг исправности выпрямителя - Из DRV_UD
	temp = load_s16(ramPnt, progPnt->Pnt_queryCnt1_1);     //!Счетчик запросов. ниче с ним не делаем
	save_s16(ramPnt, progPnt->Pnt_ResOut4, temp);          //!Выдаем значение счетчика обратно
	temp = load_s16(ramPnt, progPnt->Pnt_Reserve1);        //!код предупреждения
	stateBit7 = temp ? 1 : 0;                              //!бит предупреждения

	driveStatus = stateBit0  | (stateBit2 << 2) | (stateBit3 << 3) | (stateBit4 << 4) | (stateBit5 << 5) | (stateBit6 << 6) |
				  (stateBit7 << 7) | (stateBit9 << 9) | (stateBit11 << 11) | (stateBit12 << 12) |  (stateBit13_14 << 13);
	save_s16(ramPnt, progPnt->Pnt_StatusOut, driveStatus);

	/***********Формирование импульса ПУСК*********/
	if(PWMOnCmd){                        //!если нужно запуститься
		sPnt->StateCnt = 4;              //!заводим таймер
		setBitMicro(ramPnt, progPnt->Pnt_CmdBit); //!Формируем сигнал ПУСК
	}
	if(sPnt->StateCnt){
		sPnt->StateCnt--;
	}else{                                        //!выдержали интервал
		clrBitMicro(ramPnt, progPnt->Pnt_CmdBit); //!снимаем команду
	}

	/*******Формирование импульса СТОП***********/
	if(PWMOffCmd){ //!если нужно остановиться
		sPnt->StateCnt = 4;                //!заводим таймер
		setBitMicro(ramPnt, progPnt->Pnt_ResOut1); //!Формируем сигнал СТОП
	}
	if(sPnt->StateCnt){
		sPnt->StateCnt--;
	}else{                                          //!выдержали интервал
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut1);  //!снимаем команду
	}

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
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //53  RampStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //54  FreqExtrStop
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //55  FreqStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //56  GT_freez
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //57  WorkEnable
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //58  ResOut1
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //59  ResOut2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //60  ResOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //61  ResOut4
		BIT_PIN_TYPE | OUTPUT_PIN_MODE     //62  ResOut5
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PROFINET_PARAM2", "DA", TblFuncPROFINET_PARAM2_1, FuncPROFINET_PARAM2_1, sizeof(TblFuncPROFINET_PARAM2_1), sizeof(profinet_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPROFINET_PARAM2_1,
#endif
//********************************************************
