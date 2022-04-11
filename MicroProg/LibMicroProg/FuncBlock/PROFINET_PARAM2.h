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
        commandBit = 0  , //! 0 ������� ���������� (����/����)
        OFF2_bit        , //! 1 ������� ���������� �������� �������
        OFF3_bit        , //! 2 ������� ���������� �������� � ������ ����������� ����������
        reserve1        , //! 3 ������
        ctrl1GT         , //! 4 ���������� 1 ������ ������ ���������� �����.
        ctrl2GT         , //! 5 ���������� 2 ������ ���������� �����
        ctrl3GT         , //! 6 ���������� 3 ������ ���������� �����
        failReset       , //! 7 ������ ������������� ������
        drive1Enable    , //! 8 ����� ��������������� 1 (���������� ������ �� 1)
        drive2Enable    , //! 9 ����� ��������������� 2 (���������� ������ �� 2)
        ICSctrlEnable   , //! 10 ���������� ���������� �� ����
        reserve2        , //! 11 ������
        drive1Master    , //! 12 ���� ���� ��� ����� 1, �� ��1 - �������, ����� - �������
        drive2Master    , //! 13 ���� ���� ��� ����� 1, �� ��2 - �������, ����� - �������
        reserve3        , //! 14 ������
        reserve4          //! 15 ������
}Cmd_bits;

/*******��������� ������� ���������� �� ���� PROFINET*********/
typedef enum{
	Off2State,          //!��������� "���������� ������� �������"
	Off3State,          //!��������� "���������� ������� ���������"
	driveFaultState,    //!��������� "������ ��"
	interlockState,     //!��������� "������"
	notReadyState,      //!��������� "�� ����� � �����"
	readyState,         //!��������� "����� � �����"
	operationEnState,   //!��������� "������" ("��� �������")
	rampOutEnState,     //!��������� "��������� ������ ������ ���������� �����"
	accDeccEnState,     //!��������� "�������� ������/����������"
	operateState,       //!��������� "������"
}profinetCmdStateType;

typedef enum{
    drvNum1 = 0,
    drvNum2 = 1
}DrvNum;

typedef enum{
    notUseMode    = 3, //! ����� "������� �� ������"
    singleMode    = 0, //! ����� "���������"
    slaveMode     = 2, //! ����� "�������"
    masterMode    = 1, //! ����� "�������"
    errModeSelect = 4  //! ������� �������� ��� ������
}DrvMode;

typedef enum{
    local_remote = 0, //!�������/�������������
    reserve  = 1,     //!
    freqDown = 3,     //! ��� �������� ���������� ����������
    ready    = 4,     //! �����
    drv_fail = 5,     //! ������
    drv_ON   = 7,     //! ������
}DrvStatusWord1;

typedef struct{
	profinetCmdStateType     profinetCmdState;
	u16 StateCnt;
	u16 offStateCnt;
	u16  prevPWMOnCmd;
	u16  prevPWMOffCmd;
}profinet_State_type;

#define ENABLED 1
#define NOM_TORQ 2507 //������� ������


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
    	sPnt->profinetCmdState = interlockState; //!�������� ��������� "������. �������� ��������� ���������� ����".
        return &progPnt->Pnt_End;
    }
	mPnt28 = &GlobalC28;
    rotFreq = load_s32(ramPnt, progPnt->Pnt_RotFreq1);    //!������� �������� ������� �������� ������
    save_s32(ramPnt, progPnt->Pnt_RotFreq2, rotFreq);     //!������ ������� �������� ������� �������� ������
    maxPosPU = load_s16(ramPnt, progPnt->Pnt_maxPosPU);   //!32767

    /*********************������� �������������� � ������***********************************/
    temp = load_s16(ramPnt, progPnt->Pnt_Reserve1);  //!������� ��������������
    save_s16(ramPnt, progPnt->Pnt_Warning, temp);    //!������� �������������� ��� ��� PROFINET
    temp = load_s16(ramPnt, progPnt->Pnt_drv_fail);  //!������� ������
    save_s16(ramPnt, progPnt->Pnt_Fail, temp);       //!������� ������ ��� ��� PROFINET
    /***************************************************************************************/

    drvStatusIn1 = load_s16(ramPnt, progPnt->Pnt_Status1);   //!������ 1

    current = load_s32(ramPnt, progPnt->Pnt_Current1);    //!������� ������ ���

    temp = load_s16(ramPnt, progPnt->Pnt_currentBase);    //!���� �� ����

    temp = (u16)((_IQtoF(current) * temp) / 10.0 + 0.5);  //!����� �������� ������� ���� ���������

    save_s16(ramPnt, progPnt->Pnt_Current2, temp);        //!������ ��� ��� ���

    temp2 = load_s16(ramPnt, progPnt->Pnt_currTorq1);      //!������� ������

    temp2 = (s16)(((float)temp2 / NOM_TORQ) * 100 * 100 + 0.5);

    save_s16(ramPnt, progPnt->Pnt_currTorq2, temp2);       //!������� ������ ��� ���

    outVoltage = load_s32(ramPnt, progPnt->Pnt_outVoltage1); //!������� �������� ���������� ��

    voltageBase = load_s16(ramPnt, progPnt->Pnt_voltBase);   //!���� �� ����������

    temp = (u16)(_IQtoF(outVoltage) * voltageBase + 0.5);    //!����� �������� ��������� ����������

    save_s16(ramPnt, progPnt->Pnt_outVoltage1_1, temp);

    temp = load_s16(ramPnt, progPnt->Pnt_driveTemp1);       //!������� ����������� ��

    temp = (u16)(((float)temp / 10.0) + 0.5);               //!�������� ����������� �� � ������ ��������

    save_s16(ramPnt, progPnt->Pnt_driveTemp2, temp);

    temp = load_s16(ramPnt, progPnt->Pnt_encoPos1);         //!���������� ������� � ����� ������ ����� �������, ������. � ������

    temp = (u16)((((float)temp / 10.0) / 360.0) * maxPosPU + 0.5);

    temp = (u16)(_IQtoIQ15(_IQabs(mPnt28->Enco_Angle))) & 0x7FFF;

    save_s16(ramPnt, progPnt->Pnt_encoPos2, temp);

    outFreq = load_s32(ramPnt, progPnt->Pnt_currFreq1);      //!������� �������� ������� ����

    save_s32(ramPnt, progPnt->Pnt_currFreq2, outFreq);       //!������ ������� �������� ������� ����

    DC_Voltage = load_s32(ramPnt, progPnt->Pnt_DC_Volt1); //!���������� ����� Ud

    temp = (u16)(_IQtoF(DC_Voltage) * voltageBase * 10.0 + 0.5);        //!�������� � ������ ��������

    save_s16(ramPnt, progPnt->Pnt_DC_Volt2, temp);

    curPower = load_s32(ramPnt, progPnt->Pnt_curPower1);  //!������� ��������, ���

    temp = load_s16(ramPnt, progPnt->Pnt_powerBase);      //!���� �� ��������

    temp = (u16)((_IQtoF(curPower) * temp) / 10.0 + 0.5);        //!���������� �������� ��������, ����������� � ������

    save_s16(ramPnt, progPnt->Pnt_curPower2, temp);

    controlWord = load_s16(ramPnt, progPnt->Pnt_command1);     //!����� ����������

    profiCtrl = load_s16(ramPnt, progPnt->Pnt_RefACSForm);     //!���������� ���������� �� �� ��� PROFINET

	if((profiCtrl == ENABLED) && (controlWord & (1 << ICSctrlEnable))){
		stateBit9 = 1; //!���� ����������� ���������� �� ���� ��� ����� ��������� �������
		switch(sPnt->profinetCmdState){
		case interlockState:                                 //!��������� "������". �� ���� - ��� �������� ��������� 0-�� ���� ����� ����������. �������� ��������� ����� ������ �������
			setBitMicro(ramPnt, progPnt->Pnt_ResOut2);       //!��������� ������ ������ ���������� ����� - �������� ���������
			setBitMicro(ramPnt, progPnt->Pnt_GT_freez);      //!��������� ������ � ���������� - �������� ���������
			setBitMicro(ramPnt, progPnt->Pnt_ResOut5);       //!������������� ������� ������� �� ���� ���������� ����� - �������� ���������
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!������ ��
			temp = controlWord & ((u16)1 << commandBit);     //!��������� ���� ����/����
			if(failOff){ //!���� ������ ��
				sPnt->profinetCmdState = driveFaultState; //!������� � ��������� "������ ��"
			}else if(temp){                               //!���� ���� �� ����, ����� ������ � ���� ���������
				stateBit6 = 1;                            //!��������� ��������� "������"
			}else if(!temp){                              //!���� ����
				stateBit6 = 0;                            //!���������� ���� ��������� "������"
				sPnt->profinetCmdState = notReadyState;   //!��������� � ��������� "�� ����� � �����"
			}
			break;
		case notReadyState: //!��������� "�� ����� � �����"
			stateBit0 = 0;  //!����� ����� "���������� � �����"
			off2 = !(controlWord & ((u16)1 << OFF2_bit));            //!������ ����������� �������� �������
			off3 = !(controlWord & (1 << OFF3_bit));                 //!������ ����������� �������� � ���������� �������� ����������
			if((drvStatusIn1 >> 8) & (1 << drv_fail)){               //!���� ����  ������ ��
				sPnt->profinetCmdState = driveFaultState;            //!������� � ��������� "������ ��"
			}else if(off2 || off3){                                  //!���� ����� ������� ����������� ��������..
				sPnt->profinetCmdState = sPnt->profinetCmdState;     //!..������ �� ������. �������� � ��������� notReadyState
			}else{                                                   //!����� ������� ���������� ���
				sPnt->profinetCmdState = ((drvStatusIn1 >> 8) & (1 << ready)) ? readyState : sPnt->profinetCmdState; //!������� � ��������� "����� � �����" ���� �� �����
			}
			break;
		case readyState:      //!��������� "����� � �����"
			stateBit0 = 1;    //!��������� ����� "����� � �����" ���� ������� � ��������� ����� � �� �� ����� �����
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!������ ����������� �������� �������
			off3 = !(controlWord & (1 << OFF3_bit));         //!������ ����������� �������� � ���������� �������� ����������
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail);  //!������ ��
			if(failOff){
				sPnt->profinetCmdState = driveFaultState;     //!���� �� ��������� � ������, ������� � ��������� "������ ��"
			}else if (off2){
				sPnt->profinetCmdState = Off2State;           //!���� ���� off2, ��������� � ��������� "���������� ������� �������"
			}else if (off3){
				sPnt->profinetCmdState =  Off3State;          //!���� ���� off3, ��������� � ��������� "���������� ������� ���������"
			}else{                                            //!����� ���������� ���
				temp = controlWord & ((u16)1 << commandBit);   //!��������� ���� ����/����
				sPnt->profinetCmdState = temp ? operationEnState: sPnt->profinetCmdState; //!������� � ��������� "������" ("��� �������")
			}
			break;
		case operationEnState:                               //!��������� "������" ("��� �������")
			temp = controlWord & ((u16)1 << commandBit);     //!��������� ���� ����/����
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!������ ��
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!������ ����������� �������� �������
			off3 = !(controlWord & (1 << OFF3_bit));         //!������ ����������� �������� � ���������� �������� ����������
			if (failOff){                                    //!���� ���� ������ ��
				sPnt->profinetCmdState = driveFaultState;    //! ������� � ��������� "������ ��"
			}else if (!temp){                              //!���� ��������� ������� ����
				PWMOffCmd = 1;                             //!��������� ���
				sPnt->profinetCmdState = readyState;       //!������� � ��������� "����� � �����"
			}else if (off2){                               //!������ ����������� �������� �������
				sPnt->profinetCmdState = Off2State;        //!���� ���� off2, ��������� � ��������� "���������� ������� �������"
			}else if (off3){                               //!������ ����������� �������� � ���������� �������� ����������
				sPnt->profinetCmdState = Off3State;        //!���� ���� off3, ��������� � ��������� "���������� ������� ���������"
			}else{                                         //!�����, ������� ���������� ���, ����� �������� ���
				PWMOnCmd = 1;                              //!�������� ���
				sPnt->profinetCmdState = (controlWord & ((u16)1 << ctrl1GT)) ? rampOutEnState : sPnt->profinetCmdState; //!��������� ������ ������ ���������� �����, ������� � ��������� "��������� ������ ������ ���������� �����"
			}
			break;
		case rampOutEnState:                                 //!��������� "��������� ������ ������ ���������� �����"
			temp = controlWord & ((u16)1 << commandBit);     //!��������� ���� ����/����
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!������ ��
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!������ ����������� �������� �������
			off3 = !(controlWord & (1 << OFF3_bit));         //!������ ����������� �������� � ���������� �������� ����������
			if (failOff){                                    //!���� ���� ������ ��
				sPnt->profinetCmdState = driveFaultState;    //! ������� � ��������� "������ ��"
			}else if (!temp){                              //!���� ��������� ������� ����
				PWMOffCmd = 1;                             //!��������� ���
				sPnt->profinetCmdState = readyState;       //!������� � ��������� "����� � �����"
			}else if (off2){                               //!������ ����������� �������� �������
				sPnt->profinetCmdState = Off2State;        //!���� ���� off2, ��������� � ��������� "���������� ������� �������"
			}else if (off3){                               //!������ ����������� �������� � ���������� �������� ����������
				sPnt->profinetCmdState = Off3State;        //!���� ���� off3, ��������� � ��������� "���������� ������� ���������"
			}else if (!(controlWord & ((u16)1 << ctrl1GT))){ //!���� ����� ���������� ����� ����� ��������
				setBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!��������� ������ ������ ���������� �����
				sPnt->profinetCmdState = operationEnState; //!������� � ��������� "������" ("��� �������")
			}else{                                         //!�����, ���������� ���.
				clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!��������� ������ ������ ���������� �����
				sPnt->profinetCmdState = (controlWord & ((u16)1 << ctrl2GT)) ? accDeccEnState : sPnt->profinetCmdState; //!��������� ������/����������, ������� � ��������� "�������� ������/����������"
			}
			break;
		case accDeccEnState:                                 //!��������� "�������� ������/����������"
			temp = controlWord & ((u16)1 << commandBit);     //!��������� ���� ����/����
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!������ ��
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!������ ����������� �������� �������
			off3 = !(controlWord & (1 << OFF3_bit));         //!������ ����������� �������� � ���������� �������� ����������
			if (failOff){                                    //!���� ���� ������ ��
				sPnt->profinetCmdState = driveFaultState;    //! ������� � ��������� "������ ��"
			}else if (!temp){                                //!���� ��������� ������� ����
				PWMOffCmd = 1;                               //!��������� ���
				sPnt->profinetCmdState = readyState;         //!������� � ��������� "����� � �����"
			}else if (off2){                                 //!������ ����������� �������� �������
				sPnt->profinetCmdState = Off2State;          //!���� ���� off2, ��������� � ��������� "���������� ������� �������"
			}else if (off3){                                 //!������ ����������� �������� � ���������� �������� ����������
				sPnt->profinetCmdState = Off3State;          //!���� ���� off3, ��������� � ��������� "���������� ������� ���������"
			}else if (!(controlWord & ((u16)1 << ctrl2GT))){ //!���� ������/���������� ����� ����� ��������
				setBitMicro(ramPnt, progPnt->Pnt_GT_freez);  //!��������� ������/����������
				sPnt->profinetCmdState = rampOutEnState;     //!������� � ��������� "��������� ������ ������ ���������� �����"
			}else if (!(controlWord & ((u16)1 << ctrl1GT))){ //!���� ������ ���������� ����� ���������
				setBitMicro(ramPnt, progPnt->Pnt_ResOut2);   //!��������� ������ ������ ���������� �����
				sPnt->profinetCmdState = operationEnState;   //!�������� � ��������� "������" ("��� �������")
			}else{ //!����� ���������� ���, ��� ���������, ����� ���������� � ����. ��������� ���� ���������
				clrBitMicro(ramPnt, progPnt->Pnt_GT_freez);  //!��������� ������/����������
				sPnt->profinetCmdState = (controlWord & ((u16)1 << ctrl3GT)) ? operateState : sPnt->profinetCmdState; //!��������� ������/����������, ������� � ��������� "�������� ������/����������"
			}
			break;
		case operateState:                                   //!��������� "������". ��� �������
			temp = controlWord & ((u16)1 << commandBit);     //!��������� ���� ����/����
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail); //!������ ��
			off2 = !(controlWord & ((u16)1 << OFF2_bit));    //!������ ����������� �������� �������
			off3 = !(controlWord & (1 << OFF3_bit));         //!������ ����������� �������� � ���������� �������� ����������
			if (failOff){                                    //!���� ���� ������ ��
				sPnt->profinetCmdState = driveFaultState;    //! ������� � ��������� "������ ��"
			}else if (!temp){                                //!���� ��������� ������� ����
				PWMOffCmd = 1;                               //!��������� ���
				sPnt->profinetCmdState = readyState;         //!������� � ��������� "����� � �����"
			}else if (off2){                                 //!������ ����������� �������� �������
				sPnt->profinetCmdState = Off2State;          //!���� ���� off2, ��������� � ��������� "���������� ������� �������"
			}else if (off3){                                 //!������ ����������� �������� � ���������� �������� ����������
				sPnt->profinetCmdState = Off3State;          //!���� ���� off3, ��������� � ��������� "���������� ������� ���������"
			}else if(!(controlWord & ((u16)1 << ctrl1GT))){  //!���� ��������� ������ ������ ���������� �����
				setBitMicro(ramPnt, progPnt->Pnt_ResOut2);   //!��������� ������ ������ ���������� �����
				sPnt->profinetCmdState = operationEnState;   //!������� � ��������� "������" ("��� �������")
			}else if (!(controlWord & ((u16)1 << ctrl2GT))){ //!���� �������� ������/����������
				setBitMicro(ramPnt, progPnt->Pnt_GT_freez);  //!��������� ������/����������
				sPnt->profinetCmdState = rampOutEnState;     //!������� � ��������� "��������� ������ ������ ���������� �����"
			}else if(!(controlWord & ((u16)1 << ctrl3GT))){  //!���� ��������� ������ ����� ���������� �����
				setBitMicro(ramPnt, progPnt->Pnt_ResOut5);   //!������������� ������� ������� �� ���� ���������� �����
				sPnt->profinetCmdState = accDeccEnState;     //!������� � ��������� "�������� ������/����������"
			}else{
				clrBitMicro(ramPnt, progPnt->Pnt_ResOut5);   //!��������� ������ ������� �� ���� ���������� �����
			}
			break;
		case Off2State:                                    //!��������� "���������� ������� �������"
			setBitMicro(ramPnt, progPnt->Pnt_RampStop);    //!��� ���������� - �����
			stateBit4 = 0;                                 //!���� ����������� �������� ������� ��� ����� ��������� (� ������ ������ ���������� ������� ����������� ����� ���) �� ����� ���� ������ ���� � �� ����������� �����
			PWMOffCmd = 1;                                 //!������� ����
			rampTime = load_s16(ramPnt, progPnt->Pnt_Reserve4);     //!����� ������
			if(sPnt->offStateCnt < (u16)((rampTime /4.0) * 1000)){  //!���� ��������� ������� ������
				sPnt->offStateCnt++;
			}else{                                                   //!��������� ��������� ������� ������
				sPnt->offStateCnt = 0;
				stateBit4 = 1; //!������� ������� ����������� �������� off2
				clrBitMicro(ramPnt, progPnt->Pnt_RampStop); //!������� �������������� ��������� ���� ���������� �������
				sPnt->profinetCmdState = interlockState;    //!��������� � ��������� "������ ����� ������"
			}
			break;
		case Off3State:                                    //!��������� "���������� ������� ���������"
			setBitMicro(ramPnt, progPnt->Pnt_FreqStop);    //!��� ���������� - ���������
			setBitMicro(ramPnt, progPnt->Pnt_FreqExtrStop);//!����� ��������� ����������� ������� ����������
			stateBit5 = 0;                                 //!���� ����������� �������� � ���������� ������
			PWMOffCmd = 1;                                 //!������� ����
			PWM_On = (drvStatusIn1 >> 8) & (1 << drv_ON);  //!�������� ��������� "�������/��������"
			if(!PWM_On){                                   //!���� ��������� ���������� ���
				clrBitMicro(ramPnt, progPnt->Pnt_FreqStop);     //!������� �������������� ��������� ���������� ���� ����������
				clrBitMicro(ramPnt, progPnt->Pnt_FreqExtrStop); //!������� �������������� ��������� ����������� ������� ����������
				stateBit5 = 1;                                  //!������� ������� ����������� �������� off3
				sPnt->profinetCmdState = interlockState;        //!��������� � ��������� "������ ����� ������"
			}
			break;
		case driveFaultState:                                 //!��������� "������ ��"
			failOff = (drvStatusIn1 >> 8) & (1 << drv_fail);  //!���� ����  ������ ��
			if(!failOff){                                     //!������ �� ����� ��������� ������ ����� ������������ ������ � ������������
				sPnt->profinetCmdState = interlockState;      //!��������� � ��������� "������ ����� ������"
				stateBit3 = 0;                                //!������� ���� ������
			}
			break;
		}

		rotFreq = load_s32(ramPnt, progPnt->Pnt_queryCnt1);           //!��������� ����� ������� ��������
		if (rotFreq < 0){                                             //!���� ����� ������� �������������
			save_s32(ramPnt, progPnt->Pnt_freqRef2, _IQabs(rotFreq)); //!������ ���������� �������� �������
			setBitMicro(ramPnt, progPnt->Pnt_ResOut3);                //!������ ������ �������
		}else{ //!����� ������� �������������
			save_s32(ramPnt, progPnt->Pnt_freqRef2, rotFreq); //!������ ���������� �������� �������
			clrBitMicro(ramPnt, progPnt->Pnt_ResOut3);        //!�������� ������ �������
		}
	/*		temp_iq = load_s32(ramPnt, progPnt->Pnt_torqMax1);
			AllPrmPnt->UST_TORQUE_MAX = _IQtoF(temp_iq) * GlobalM3.TorqBASE * 10.f;
			save_s32(ramPnt, progPnt->Pnt_torqMax2, temp_iq);
			temp_iq = load_s32(ramPnt, progPnt->Pnt_torqMin1);
			AllPrmPnt->UST_TORQUE_MIN = _IQtoF(temp_iq) * GlobalM3.TorqBASE * 10.f;
			save_s32(ramPnt, progPnt->Pnt_torqMin2, temp_iq);*/

        /**********����������� ������ ������ �� (�������, ������� � �.�.)***************/
        drv1En = (controlWord >> drive1Enable) & 0x01;     //!��� 8  ����� ����������
        drv2En = (controlWord >> drive2Enable) & 0x01;     //!��� 9  ����� ����������
        drv1Master = (controlWord >> drive1Master) & 0x01; //!��� 12 ����� ����������
        drv2Master = (controlWord >> drive2Master) & 0x01; //!��� 13 ����� ����������
        drvNum = load_s16(ramPnt, progPnt->Pnt_drvNum);    //!����� ��������������� � ������� �������/������� (1 ��� 2). ������ �� �� ����� ���� ��� �������, ���� �������
		 if(drv1En && drv2En){
			 if((drv1Master == 1) && (drv2Master == 0)){
				 driveMode = masterMode;  //!����� "�������"
				 stateBit13_14 = 1;       //!�������
			 }else if((drv1Master == 0) && (drv2Master == 1)){
				 driveMode = slaveMode;   //!����� "�������"
				 stateBit13_14 = 2;
			 }
		 }else if(drv1En && !drv2En){
			 if(drvNum == drvNum1){
				 driveMode = singleMode;  //!����� "���������"
				 stateBit13_14 = 0;
			 }else if(drvNum == drvNum2){
				 driveMode = notUseMode; //!����� "������� �� ������"
				 stateBit13_14 = 3;
			 }
		 }else if (!drv1En && drv2En){
			 if(drvNum == drvNum1){
				 driveMode = notUseMode; //!����� "������� �� ������"
				 stateBit13_14 = 3;
			 }else if(drvNum == drvNum2){
				 driveMode = singleMode;  //!����� "���������"
				 stateBit13_14 = 0;
			 }
		 }

		 save_s16(ramPnt, progPnt->Pnt_driveMode, driveMode); //!������ ��� ������ ������ ��. driveMode = 0, 1, 2, 3
		 defBitMicro(ramPnt, progPnt->Pnt_WorkEnable, controlWord & (1 << failReset)); //!������������ ������
	}else{ //!���������� �� ���� ���������
		sPnt->profinetCmdState = interlockState;         //!����� ��������������� ���������� �� profinet � �������� ���������
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2);       //!��������� ����� ���������� �����
		clrBitMicro(ramPnt, progPnt->Pnt_GT_freez);      //!���������  ������ � ����������
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut5);       //!���������  ���� ���������� �����
		stateBit13_14 = load_s16(ramPnt, progPnt->Pnt_Reserve3);     //!���������� �� profinet ��������� - ��������� ����� (�������, ������� � �. �.), �������� �������
		stateBit0 = ((drvStatusIn1 >> 8) & (1 << ready)) >> ready;   //!��������� �� "�����"
		stateBit4 = 1;                                               //!��� ����������� ���������� �������
		stateBit5 = 1;                                               //!��� ����������� ���������� � ���������� ������
		stateBit9 = 0;                                               //!��� ��������� "������"
	}
	stateBit2 = ((drvStatusIn1 >> 8) & (1 << drv_ON)) >> drv_ON;
	stateBit3 = ((drvStatusIn1 >> 8) & (1 << drv_fail)) >> drv_fail; //!��������� ������� "������"
	stateBit11 = GlobalC28.FlagState.bit.vPrUp;            //!���� "��������� ����������"
	stateBit12 = GlobalM3.FlagState.bit.DRVNormRectiefer;  //!���� ����������� ����������� - �� DRV_UD
	temp = load_s16(ramPnt, progPnt->Pnt_queryCnt1_1);     //!������� ��������. ���� � ��� �� ������
	save_s16(ramPnt, progPnt->Pnt_ResOut4, temp);          //!������ �������� �������� �������
	temp = load_s16(ramPnt, progPnt->Pnt_Reserve1);        //!��� ��������������
	stateBit7 = temp ? 1 : 0;                              //!��� ��������������

	driveStatus = stateBit0  | (stateBit2 << 2) | (stateBit3 << 3) | (stateBit4 << 4) | (stateBit5 << 5) | (stateBit6 << 6) |
				  (stateBit7 << 7) | (stateBit9 << 9) | (stateBit11 << 11) | (stateBit12 << 12) |  (stateBit13_14 << 13);
	save_s16(ramPnt, progPnt->Pnt_StatusOut, driveStatus);

	/***********������������ �������� ����*********/
	if(PWMOnCmd){                        //!���� ����� �����������
		sPnt->StateCnt = 4;              //!������� ������
		setBitMicro(ramPnt, progPnt->Pnt_CmdBit); //!��������� ������ ����
	}
	if(sPnt->StateCnt){
		sPnt->StateCnt--;
	}else{                                        //!��������� ��������
		clrBitMicro(ramPnt, progPnt->Pnt_CmdBit); //!������� �������
	}

	/*******������������ �������� ����***********/
	if(PWMOffCmd){ //!���� ����� ������������
		sPnt->StateCnt = 4;                //!������� ������
		setBitMicro(ramPnt, progPnt->Pnt_ResOut1); //!��������� ������ ����
	}
	if(sPnt->StateCnt){
		sPnt->StateCnt--;
	}else{                                          //!��������� ��������
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut1);  //!������� �������
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
