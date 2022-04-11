#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_AckFailOff;                   //1  AckFailOff
    u16  Pnt_HandFailReset1;               //2  HandFailReset1
    u16  Pnt_HandFailReset2;               //3  HandFailReset2
    u16  Pnt_HandFailReset3;               //4  HandFailReset3
    u16  Pnt_ManResEn;                     //5  ManResEn
    u16  Pnt_NotUsed;                      //6  NotUsed
    u16  Pnt_Fail[20];                     //7...26  Fail1...Fail20  ������ ����������� ������
    u16  Pnt_F_StopKey;                    //27  F_StopKey
    u16  Pnt_F_Power;                      //28  F_Power
    u16  Pnt_F_Contactor;                  //29  F_Contactor
    u16  Pnt_F_TormRes;                    //30  F_TormRes
    u16  Pnt_F_Fasirovka;                  //31  F_Fasirovka
    u16  Pnt_F_Tzar;                       //32  F_Tzar
    u16  Pnt_F_Overload;                   //33  F_Overload
    u16  Pnt_F_Ain1;                       //34  F_Ain1
    u16  Pnt_F_Ain2;                       //35  F_Ain2
    u16  Pnt_F_Ain3;                       //36  F_Ain3
    u16  Pnt_F_Ain4;                       //37  F_Ain4
    u16  Pnt_F_Ain5;                       //38  F_Ain5
    u16  Pnt_F_Fmax;                       //39  F_Fmax
    u16  Pnt_F_Fmin;                       //40  F_Fmin
    u16  Pnt_F_PhaseU;                     //41  F_PhaseU
    u16  Pnt_F_PhaseV;                     //42  F_PhaseV
    u16  Pnt_F_PhaseW;                     //43  F_PhaseW
    u16  Pnt_F_TmaxDrv;                    //44  F_TmaxDrv
    u16  Pnt_F_Modbus;                     //45  F_Modbus
    u16  Pnt_F_ReversZapr;                 //46  F_ReversZapr
    u16  Pnt_F_TmaxHeater;                 //47  F_TmaxHeater
    u16  Pnt_F_NoWater;                    //48  F_NoWater
    u16  Pnt_F_Avtovent;                   //49  F_Avtovent
    u16  Pnt_F_ZaprFreq;                   //50  F_ZaprFreq
    u16  Pnt_F_Nedogruz;                   //51  F_Nedogruz
    u16  Pnt_F_KrayPoint;                  //52  F_KrayPoint
    u16  Pnt_F_ObrRemnya;                  //53  F_ObrRemnya
    u16  Pnt_F_Potok;                      //54  F_Potok
    u16  Pnt_F_SuhoyHod;                   //55  F_SuhoyHod
    u16  Pnt_F_ShortCycle;                 //56  F_ShortCycle
    u16  Pnt_F_DisbCur;                    //57  F_DisbCur
    u16  Pnt_F_Res;                        //58  F_Res
    u16  Pnt_FailStopOn;                   //59  FailStopOn
    u16  Pnt_FairModOn;                    //60  FairModOn
    u16  Pnt_FailMask0;                    //61  FailMask0
    u16  Pnt_FailMask1;                    //62  FailMask1
    u16  Pnt_FailMask3;                    //63  FailMask3
    u16  Pnt_FailMask4;                    //64  FailMask4
    u16  Pnt_PowerErr;                     //65  PowerErr
    u16  Pnt_FailTekDrv;                   //66  FailTekDrv
    u16  Pnt_FailFixDrv;                   //67  FailFixDrv
    u16  Pnt_FailTekDrvBit;                //68  FailTekDrvBit
    u16  Pnt_FailTekDrvBitNot;             //69  FailTekDrvBitNot
    u16  Pnt_OFF_ZAR;                      //70  OFF_ZAR
    u16  Pnt_End;
}FuncFAIL_COMMON_V2_type;


typedef union{
        struct{

           //������� ������
           u16  None            : 1;    // 0
           u16  SwU             : 1;    // 1  ������ ������� ���� ���� U
           u16  SwV             : 1;    // 2  ������ ������� ���� ���� V
           u16  SwW             : 1;    // 3  ������ ������� ���� ���� W
           u16  SwT             : 1;    // 4  ������ ������� ���� ���������
           u16  MtzU_Analog     : 1;    // 5  ������ ���������� ��� �� ���� U
           u16  MtzV_Analog     : 1;    // 6  ������ ���������� ��� �� ���� V
           u16  MtzW_Analog     : 1;    // 7  ������ ���������� ��� �� ���� W
           u16  MtzU_Prog       : 1;    // 8  ������ ����������� ��� �� ���� U
           u16  MtzV_Prog       : 1;    // 9  ������ ����������� ��� �� ���� V
           u16  MtzW_Prog       : 1;    // 10 ������ ����������� ��� �� ���� W
           u16  UdMin           : 1;    // 11 ������ �� Ud �������
           u16  UdMax           : 1;    // 12 ������ �� Ud ��������
           u16  UdCharge        : 1;    // 13 ������ �������� ����� Ud
           u16  Tsw             : 1;    // 14 ������ �� ����������� ������
           u16  Mtz_Leakage     : 1;    // 15 ������ �� ��� ������
           u16  EncoderLoss     : 1;    // 16 ����� ��������
           u16  ruU             : 1;    // 17 ������ ����� ���� �������� U
           u16  ruV             : 1;    // 18 ������ ����� ���� �������� V
           u16  ruW             : 1;    // 19 ������ ����� ���� �������� W
           u16  tormCheck       : 1;    // 20 ������ ������ ���������� ���������
           u16  reserv          : 11;   // 21...31 ������

           //��������� ������
           u16  StopKey         : 1;    // 32 ��������� ����
           u16  Power           : 1;    // 33 ������ �������
           u16  Contactor       : 1;    // 34 ������ ����������
           u16  TormRes         : 1;    // 35 ������ ���������� ���������
           u16  Fasirovka       : 1;    // 36 ������ ���������
           u16  Tzar            : 1;    // 37 ������ ������� ������
           u16  Overload        : 1;    // 38 ������ ����������
           u16  Ain1            : 1;    // 39 ������ ����������� ����� 1
           u16  Ain2            : 1;    // 40 ������ ����������� ����� 2
           u16  Ain3            : 1;    // 41 ������ ����������� ����� 3
           u16  Ain4            : 1;    // 42 ������ ����������� ����� 4
           u16  Ain5            : 1;    // 43 ������ ����������� ����� 5
           u16  Fmax            : 1;    // 44 ������ ���������� �������
           u16  Fmin            : 1;    // 45 ������ ���������� �������
           u16  PhaseU          : 1;    // 46 ������ ���� U
           u16  PhaseV          : 1;    // 47 ������ ���� V
           u16  PhaseW          : 1;    // 48 ������ ���� W
           u16  TmaxDrv         : 1;    // 49 ������ ��������� ���������
           u16  Modbus          : 1;    // 50 ������ ���������� �� Modbus
           u16  ReversZapr      : 1;    // 51 ������ �������
           u16  TmaxHeater      : 1;    // 52 ������ ������������ ����������� �����������
           u16  NoWater         : 1;    // 53 ������ ���������� ����
           u16  Avtovent        : 1;    // 54 ������ ������ "�����������"
           u16  ZaprFreq        : 1;    // 55 ������ "����������� �������"
           u16  Nedogruz        : 1;    // 56 ������ ���������
           u16  KrayPoint       : 1;    // 57 ������ ������� ����� ������� ������
           u16  ObrRemnya       : 1;    // 58 ������ ������ �����
           u16  Potok           : 1;    // 59 ������ ���������� ������
           u16  SuhoyHod        : 1;    // 60 ������ ������ ����
           u16  ShortCycle      : 1;    // 61 ������ ��������� �����
           u16  DisbCur         : 1;    // 62 ������ ���������� �����
           u16  MODBUS             : 1;    // 63 ������ �������� ���������� Modbus
         } bit;
        u64 all;
    } AllFailsType;

//33-SlowFail_supplyPhLoss
//SlowFailMask_ZarTime        37
//#define SlowFailERROR_DI            48      //������� ������
//#define SlowFailPhase3ERR          53       //������ 3 ���
//SlowFail_brakeResFail 35
#define     SLOWFAILWOACK ((u64)1<<SlowFail_CurrImbalance)
#define		POWERERRMASK ((u64)1<<FastFailMask_UdMin|(u64)1<<FastFailMask_UdMax|(u64)1<<SlowFail_supplyPhLoss)
//#define		OFFZARMASK ((u64)1<<FastFailMask_SwU        |(u64)1<<FastFailMask_SwV        |(u64)1<<FastFailMask_SwW        |(u64)1<<FastFailMask_SwT        |\
//						(u64)1<<FastFailMask_MtzU_Analog|(u64)1<<FastFailMask_MtzV_Analog|(u64)1<<FastFailMask_MtzW_Analog|(u64)1<<FastFailMask_MtzU_Prog  |\
//						(u64)1<<FastFailMask_MtzV_Prog  |(u64)1<<FastFailMask_MtzW_Prog  |(u64)1<<FastFailMask_UdMin      |(u64)1<<FastFailMask_UdMax      |\
//						(u64)1<<FastFailMask_UdCharge   |(u64)1<<FastFailMask_Tsw        |(u64)1<<FastFailMask_MTZ_Leakage|(u64)0<<FastFailMask_EncoderLoss|\
//						(u64)1<<SlowFail_failButton     |(u64)1<<SlowFail_supplyPhLoss   |(u64)1<<SlowFailMask_ZarTime |  (u64)1<<SlowFail_brakeResFail|\
//                        (u64)1<<SlowFailERROR_DI |(u64)1<<SlowFailPhase3ERR)
    //|(u64)SlowFail_Slave_Err   |(u64)SlowFailModbus
#define		OFFZARMASK    0xFFFFFFFFFFFFFFFFULL
typedef struct{
	u16      zarOffTimer;
	u16       resetFail3; //���������� ��������� ������������ ������ �� ����������� �����
	u64    prevFailState; //���������� ��������� �����-������ ������
//	u16   countPnt_NotUsed;
}FAILCOMMON_State_type;

typedef struct{
    u64    prevFailState;
}failnvState_type;

#define     nV    (*(failnvState_type*)0)

//typedef union{	???? ��������� ����������
//	u64 v1;
//	u32 v2[2];
//	u16 v4[4];
//} u64u32u16_type;

//********************************************************
u16 *FuncFAIL_COMMON_V2_1(FuncFAIL_COMMON_V2_type  *progPnt, u32 ramPnt)
{
	u16						*FPnt,i, hendReset, ManResMode, NumFailDRVLocal = 0;
//	u16                       Kvitirovanie=0;
	//u64u32u16_type			fMask;
	union{
		u64 v1;
		u32 v2[2];
		u16 v4[4];
	} fMask;
	u16						   resetFail1, resetFail2, resetFail3;
//	u32                        curSlowFailMask; //����� ������� ������  curFastFailMask,
	COMMONPARAMC28			 *mPnt28;
	COMMONPARAMM3			  *mPnt3;
	FAILCOMMON_State_type	   *sPnt;
 	failnvState_type          *nvAdr;
	AllFailsType      handResetMask; //����� �����-������, �������� ����� ������� ������������ ��� ��������������� ������
	AllFailsType      curFailState;   //����� �����-������ ��������� ������� ������
	sPnt 			= (FAILCOMMON_State_type *)(ramPnt + progPnt->Pnt_State);	
	mPnt28  		= &GlobalC28;
	mPnt3   		= &GlobalM3;
	fMask.v1 		= 0ULL;
	nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nV);

	if (flagIsFirstCall)
	{
	    save_s16(ramPnt, progPnt->Pnt_HandFailReset2, 0);                              //������� � ��������� "��� ������������".
		//sPnt->countPnt_NotUsed=0;
		//������� ����������� ���������������� ������
	   // RdFramMicro(&(sPnt->prevFailState), &(nvAdr->prevFailState), sizeof(u64));       //�� ������, ���� �� ����� �������� ������� ������������ ���� ������������ ����������
	    return &progPnt->Pnt_End;
	}
	if(load_s16(ramPnt, progPnt->Pnt_NotUsed )==0)  // ����������� ��� ������ 180905 ������������
	{  handResetMask.all = 0xFFFFFFFFFFFFFFFFULL; 
	}
	else  // ����������� �� ������
	 { 

	//--��������� 64-��������� �����, ���� �������� ���������� ������� �� ������� ������� ������������ ��� �������������� ����� ������
		handResetMask.all = 0;  //��������� ����� �����-�����, ���� �������� ���������/��������� ������ ������������ ��� �����. ������
		for(i = 0; i < 20; i++) //20 - ����������� ��������� ���������� ������, ����������� �������
		{
		  
			if (load_s16(ramPnt, progPnt->Pnt_Fail[i])) //���������� ��������� �� ������������ ������ ��� �������� ����� ����������� ������
			{
				handResetMask.all |= (u64)1 << load_s16(ramPnt, progPnt->Pnt_Fail[i]); //���. 1 ����� � ���� � ������������, �����. ���� ����������� ������� ������ (0...63)
			}
		 
		}
	}

//-- ���� ������� ����� ������� ������ �� ����� ������� ������ �� ����������� ������� ������

	hendReset = load_s16(ramPnt, progPnt->Pnt_AckFailOff);        //��������� ��������� ������� ���������/���������� ������ ������� ������������ ������
//	if(hendReset==1)
//	{GlobalM3.Kvitirovanie|=0x0400;
//     	Kvitirovanie|=0x8000;
//	}
// 	else
//	{GlobalM3.Kvitirovanie &=~(0x0400);
//	    Kvitirovanie &=~(0x8000);
//	}
	ManResMode  =  !testBitMicro(ramPnt, progPnt->Pnt_ManResEn);  //��������� ������� �������� ����� ������/����
	curFailState.all = 0;                                         //��������� ������������� ����������, ���� ������� ���������� ����������� �������/���������� ������ (�� �����, � �� � ������ ����������� �� ������ ������� ��� ���)
	if (hendReset && ManResMode)                                  //���� ����������� ����� ������� ������������ ������ � ����� ������ ������� �� ��������������
	{
	    curFailState.all = (u64)(mPnt28->FastFailMask.all);       //��������� ����� �����-������ ������� ������� ������. ������� 32 ���� curFailState ����� ��������� ����� ������� ������
	    fMask.v2[0] = (~((u32)curFailState.all) & (u32)(sPnt->prevFailState) & (u32)handResetMask.all) | (u32)curFailState.all; //��������� ������ ��� �������������� � ������ ����, ��� ��� ��������� ������ ����������� ������ �����
	    sPnt->prevFailState |= (u64)fMask.v2[0] & handResetMask.all; //���������� ������� ��������� ������ (���, ��� ������� ��������� ������ ������������)
	 //������� ����������� ���������������� ������
		// WrFramMicro(&(nvAdr->prevFailState), &(sPnt->prevFailState), sizeof(u64)); //��������� �� ������ ������������ ������� �� ����� �������� ������� ������� ������������
	}
	else //����� ������ ������������ ��������� � �������� � ��������� ������ �� ����� �� �������
	{
	    sPnt->prevFailState = 0;
		//������� ����������� ���������������� ������
	 //   WrFramMicro(&(nvAdr->prevFailState), &(sPnt->prevFailState), sizeof(u64)); //��� ����������� ������ ������������ ��������� ��������� ������ �� �����
	    fMask.v2[0] = mPnt28->FastFailMask.all;                                    //��������� ����� �����-������ ������� ������� ������
	}


//-- ��������� ����� ������	(�� �������)
	FPnt = &(progPnt->Pnt_F_StopKey);     //����� ���������� � ��������� �������� ��� ������� � ������� ���������� �����
	for(i = 0; i < 32; i++)               //���������� 32 ����� ������ ������� � ���������� �����
	{
		if(testBitMicro(ramPnt, *FPnt++)) //��������� ��������� �������� ����� ������. ���� �� �������� FPnt ������ ����..
		{
			fMask.v2[1] |= ((u32)1 << i);					   //fMask bits 32-63  //����������� ������ � ������ fMask.v2[1] (��������� ����� � ������-������� ��������� ������)
			curFailState.all |= ((u64)1 << (i+32));            //������� ���������� ��������� ������ ������
			if(NumFailDRVLocal == 0) NumFailDRVLocal = (32+i); //���� ������ ������������� � ��� ������ ��������������� ������, �� ����� ����������  ����� ������, �� ���� ����� ������ ���������� ������
		}
		else //�����, �� ��������  FPnt ����������  ������ ���, �� ���� ������ ���� � ����������� ������ ����� �� ������ ����� ������� � ����� �� ����������� ������� ������
		{											 //������������ ������
			if((SLOWFAILWOACK & ((u64)1 << i)) == 0) //�� ������ ������ ��� ������� ������ �������, ��� ��� i ��������� ������������ �������� 31, � SLOWFAILWOACK = 1 << 62
			{
			    if (hendReset && ManResMode)         //���� ����������� ������ ����� (��� ���� ������ �� �������� i ����, ��� �� �����)
			    {
			        fMask.v2[1] |= (u32)(sPnt->prevFailState >> 32) & (u32)(handResetMask.all >> 32) & (1 << i);
			        curFailState.all &= ~((u64)1 << (i+32)); //���������� ���������� ���������� ������ ��� �������� ����������� ������� ������������ ������
			        if (((fMask.v2[1] & (1 << i)) == 0) && (NumFailDRVLocal == (32+i))) //���� ������������ ��� ������ ����, � ������ ��� ��� ������, ������� ���� ���������� ������
			        {
			            NumFailDRVLocal = 0; //���� ������ ������ ���� ���������� � ����� � ��� �������� �� �����, �� ������ �� ���, �� �������� �� �����. ���������� ����� ��� ������ ������� ���� ���������� ������ � ����� ������� ��� ��������
			        }
			        else if (((fMask.v2[1] & (1 << i)) != 0) && (NumFailDRVLocal == 0))//�����, ���������� ������ ���, �� ��� ���� � ��� ��� ������������ ������ ������������
			        {
			            NumFailDRVLocal = (32+i);
			        }
			    }
			    else //����� ������ ������������ ���������, � �������� ����-��� ������� ������
			    {
			        fMask.v2[1] &= ~((u32)1 << i);                     //fMask bits 32-63. �������� ����-���, ��������������� �������� ������ ������������� ������
			        if(NumFailDRVLocal == (32+i)) NumFailDRVLocal = 0; //���� ������ ������ ���� ���������� � ����� � ��� �������� �� �����, �� ������ �� ���, �� �������� �� �����. ���������� ����� ��� ������ ������� ���� ���������� ������ � ����� ������� ��� ��������
			    }
			}
		}
	}

	if (hendReset && ManResMode) //���� ��������� ������ ������������, �� ������� ��������� ������ � FRAM
	{
	    sPnt->prevFailState |= ((u64)fMask.v2[1] << 32) & handResetMask.all ;     //����������  ������� ��������� ������-����� ��������� ������
		//������� ����������� ���������������� ������
	  //  WrFramMicro(&(nvAdr->prevFailState), &(sPnt->prevFailState), sizeof(u64)); //������ � FRAM ��������� ������-�����  ������
	}


	mPnt3->NumSlowFail = NumFailDRVLocal;                     //�������� ����� ������ ���������� ��������� ������ � ���������� ������. ��������� ����� ��������� ������
	if((mPnt28->NumFailDRV > 0) && (mPnt28->NumFailDRV < 32)) //���� ���� ������� ������..
	{
    	NumFailDRVLocal = mPnt28->NumFailDRV;                 //�� ������������������ NumFailDRVLocal ������� ������� ������ (����� ��������� �� ����� ��� ������ � ��������)
    }
	else //�����, ���������� ������� ������ ���, �� ���� ��� ���� � ��� ��� ������������ ������ ������������, �� fMask.v2[0] ����� ��� ����� ��������� �� ������� �� ������� ������� ������������
	{
	    for (i = 0; i < 32; i++)
	    {
	        if (fMask.v2[0] & (1 << i)) //���� �� �������� i �������� ������� ������
	        {
	            NumFailDRVLocal = i;    //��������� �� ����� � ������� �� �����
	            break;
	        }
	    }
	}


#ifdef _PROJECT_FOR_CCS_
	if(mPnt28->FastFailMask.all == 0) //���� ������� ������ ���
	{
		IntMasterDisable();
		NumFailDRVClr_set();
		IntMasterEnable();
	}
#endif //-- _PROJECT_FOR_CCS_

#ifndef _PROJECT_FOR_CCS_
	NumFailDRVLocal = 0;
	fMask.v1 		= 0ULL;
#endif //-- _PROJECT_FOR_CCS_

	//-������ ������������ ������
	if (hendReset && ManResMode) //���� ��������� ������ ������������ ������
	{
	    resetFail1 = testBitMicro(ramPnt, progPnt->Pnt_HandFailReset1); //����� 1
	    resetFail2 = load_s16(ramPnt, progPnt->Pnt_HandFailReset2);     //����� 2
	    if((!sPnt->resetFail3) && testBitMicro(ramPnt, progPnt->Pnt_HandFailReset3)) //���� �� ���� ������������ �� ����������� �����, �� ������ ����
	    {
	        resetFail3 = 1; //����� 3
	        sPnt->resetFail3 = 1; //���������� ��, ��� ����� �� ����������� ����� ������������ ������������ ������
	    }
	    else if (!testBitMicro(ramPnt, progPnt->Pnt_HandFailReset3)) //���� ������ ������ �� ����������� ����� �����
	    {
	        sPnt->resetFail3 = 0; //�������� ��������� ������������ �� ����������� �����
	        resetFail3 = 0;
	    }
	     if (resetFail1 || resetFail2 || resetFail3)                 //���� �������� ������ ������� ������������
	    {
	        fMask.v1 &= ~(handResetMask.all & (~curFailState.all));    //�������� �� ����� ������, ��� ������� �������� ������ ����� � ���������� ��� ������ �����������
	        clrBitMicro(ramPnt, progPnt->Pnt_HandFailReset1);    //������� � ��������� "��� ������������"
	        save_s16(ramPnt, progPnt->Pnt_HandFailReset2, 0);    //������� � ��������� "��� ������������"
	        sPnt->prevFailState = fMask.v1;
			 //0x8000- ��������� ����� ������  0�4000 - ��������� ������������  0�2 - 0�25  0x1 0x23
           
        if((NumFailDRVLocal==0x23)||(NumFailDRVLocal==0x35))GlobalM3.FlagCommand.bit.Kvitirov_R=1; //����� 3��� ���  ������ ���� ������

			//������� ����������� ���������������� ������

	    }
	}

    //-- ���������� �� ���������� ������
//  load_s16(ramPnt, progPnt->Pnt_AckFailOff);          //???? ��������� ����� ��������������� ������������ ������

    if(!load_s16(ramPnt, progPnt->Pnt_FailStopOn)) //���� ���������� ������ Ud ��� ������ ���������� ����� �� ������������
    {
        fMask.v1 &= ~( (u64)1<<SlowFail_failButton); //�������� ���������� ������ Ud ��� ������� ���������� ��������
    }
    if( testBitMicro(ramPnt, progPnt->Pnt_FairModOn) )  fMask.v1 &= ~(((u64)1<<SlowFail_failButton)|0xFFFF);        //���������� ������ ��������� ������(��� fMask.v2[1] ����� failButton)

//-- ���������� �����	
	save_s16(ramPnt, progPnt->Pnt_FailMask0, fMask.v4[0]); //������� ������
	save_s16(ramPnt, progPnt->Pnt_FailMask1, fMask.v4[1]); //������� ������
	save_s16(ramPnt, progPnt->Pnt_FailMask3, fMask.v4[2]); //��������� ������
	save_s16(ramPnt, progPnt->Pnt_FailMask4, fMask.v4[3]); //��������� ������
	FailMask_set();	

	save_s16(ramPnt, progPnt->Pnt_FailFixDrv      , mPnt28->FailValue.NumFail);
	save_s16(ramPnt, progPnt->Pnt_FailTekDrv      , NumFailDRVLocal  ); //�������� � �������������� ����� ������� (���� ��� ����) ��� ��������� ������ (���� �� ���� �������)
	defBitMicro(ramPnt, progPnt->Pnt_FailTekDrvBit   , NumFailDRVLocal);
	defBitMicro(ramPnt, progPnt->Pnt_FailTekDrvBitNot,!NumFailDRVLocal);

	//defBitMicro(ramPnt, progPnt->Pnt_PowerErr, (fMask.v1 & POWERERRMASK));
	if(fMask.v1 & POWERERRMASK) setBitMicro(ramPnt, progPnt->Pnt_PowerErr); //Pnt_NotUsed
	else clrBitMicro(ramPnt, progPnt->Pnt_PowerErr);

	if(testBitMicro(ramPnt, progPnt->Pnt_OFF_ZAR)){
		if(!(fMask.v1/* & OFFZARMASK*/))
		{
		    clrBitMicro(ramPnt, progPnt->Pnt_OFF_ZAR);
		}
	}
	else{
		if(sPnt->zarOffTimer){
			if(sPnt->zarOffTimer==1)
			{
			    setBitMicro(ramPnt, progPnt->Pnt_OFF_ZAR);
			}
			sPnt->zarOffTimer--;
		}
		else if(fMask.v1 !=0) sPnt->zarOffTimer=(200/INT_TIME_CALL_MICRO_mS);
	}
	/*if(sPnt->countPnt_NotUsed>0)
	{sPnt->countPnt_NotUsed--;
	}
	else
	{Kvitirovanie &=~(0x4000);
	}*/
		//save_s16(ramPnt, progPnt->Pnt_NotUsed, Kvitirovanie) ; //Pnt_NotUsed
             
	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFAIL_COMMON_V2_1[70]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1   AckFailOff
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2   HandFailReset1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3   HandFailReset2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4   HandFailReset3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5   ManResEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6   NotUsed
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7   Fail1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8   Fail2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9   Fail3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Fail4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Fail5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Fail6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Fail7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Fail8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Fail9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Fail10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Fail11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Fail12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Fail13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Fail14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Fail15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Fail16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Fail17
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  Fail18
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Fail19
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  Fail20
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //27  F_StopKey
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //28  F_Power
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //29  F_Contactor
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //30  F_TormRes
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //31  F_Fasirovka
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //32  F_Tzar
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //33  F_Overload
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //34  F_Ain1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //35  F_Ain2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //36  F_Ain3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //37  F_Ain4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //38  F_Ain5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //39  F_Fmax
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //40  F_Fmin
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //41  F_PhaseU
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //42  F_PhaseV
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //43  F_PhaseW
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //44  F_TmaxDrv
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //45  F_Modbus
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //46  F_ReversZapr
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //47  F_TmaxHeater
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //48  F_NoWater
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //49  F_Avtovent
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //50  F_ZaprFreq
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //51  F_Nedogruz
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //52  F_KrayPoint
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //53  F_ObrRemnya
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //54  F_Potok
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //55  F_SuhoyHod
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //56  F_ShortCycle
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //57  F_DisbCur
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //58  F_Res
        S16_PIN_TYPE | INPUT_PIN_MODE,     //59  FailStopOn
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //60  FairModOn
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //61  FailMask0
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //62  FailMask1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //63  FailMask3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //64  FailMask4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //65  PowerErr
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //66  FailTekDrv
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //67  FailFixDrv
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //68  FailTekDrvBit
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //69  FailTekDrvBitNot
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //70  OFF_ZAR









};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FAIL_COMMON_V2", "DD", TblFuncFAIL_COMMON_V2_1, FuncFAIL_COMMON_V2_1, sizeof(TblFuncFAIL_COMMON_V2_1), sizeof(FAILCOMMON_State_type), sizeof(failnvState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFAIL_COMMON_V2_1,
#endif
//********************************************************
