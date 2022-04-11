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
    u16  Pnt_Fail[20];                     //7...26  Fail1...Fail20  Список квитируемых аварий
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

           //Быстрые аварии
           u16  None            : 1;    // 0
           u16  SwU             : 1;    // 1  Авария силовой ключ фаза U
           u16  SwV             : 1;    // 2  Авария силовой ключ фаза V
           u16  SwW             : 1;    // 3  Авария силовой ключ фаза W
           u16  SwT             : 1;    // 4  Авария силовой ключ тормозной
           u16  MtzU_Analog     : 1;    // 5  Авария аналоговая МТЗ по фазе U
           u16  MtzV_Analog     : 1;    // 6  Авария аналоговая МТЗ по фазе V
           u16  MtzW_Analog     : 1;    // 7  Авария аналоговая МТЗ по фазе W
           u16  MtzU_Prog       : 1;    // 8  Авария программная МТЗ по фазе U
           u16  MtzV_Prog       : 1;    // 9  Авария программная МТЗ по фазе V
           u16  MtzW_Prog       : 1;    // 10 Авария программная МТЗ по фазе W
           u16  UdMin           : 1;    // 11 Авария по Ud минимум
           u16  UdMax           : 1;    // 12 Авария по Ud максимум
           u16  UdCharge        : 1;    // 13 Авария зарядное время Ud
           u16  Tsw             : 1;    // 14 Авария по температуре ключей
           u16  Mtz_Leakage     : 1;    // 15 Авария по МТЗ утечки
           u16  EncoderLoss     : 1;    // 16 Обрыв Энкодера
           u16  ruU             : 1;    // 17 Авария обрыв фазы нагрузки U
           u16  ruV             : 1;    // 18 Авария обрыв фазы нагрузки V
           u16  ruW             : 1;    // 19 Авария обрыв фазы нагрузки W
           u16  tormCheck       : 1;    // 20 Авария обрыва тормозного резистора
           u16  reserv          : 11;   // 21...31 резерв

           //Медленные аварии
           u16  StopKey         : 1;    // 32 Аварийный стоп
           u16  Power           : 1;    // 33 Авария питания
           u16  Contactor       : 1;    // 34 Авария контактора
           u16  TormRes         : 1;    // 35 Авария тормозного резистора
           u16  Fasirovka       : 1;    // 36 Авария фазировки
           u16  Tzar            : 1;    // 37 Авария времени заряда
           u16  Overload        : 1;    // 38 Авария перегрузки
           u16  Ain1            : 1;    // 39 Авария аналогового входа 1
           u16  Ain2            : 1;    // 40 Авария аналогового входа 2
           u16  Ain3            : 1;    // 41 Авария аналогового входа 3
           u16  Ain4            : 1;    // 42 Авария аналогового входа 4
           u16  Ain5            : 1;    // 43 Авария аналогового входа 5
           u16  Fmax            : 1;    // 44 Авария превышения частоты
           u16  Fmin            : 1;    // 45 Авария пониженной частоты
           u16  PhaseU          : 1;    // 46 Авария фазы U
           u16  PhaseV          : 1;    // 47 Авария фазы V
           u16  PhaseW          : 1;    // 48 Авария фазы W
           u16  TmaxDrv         : 1;    // 49 Авария перегрева двигателя
           u16  Modbus          : 1;    // 50 Авария соединения по Modbus
           u16  ReversZapr      : 1;    // 51 Авария реверса
           u16  TmaxHeater      : 1;    // 52 Авария максимальной температуры нагревателя
           u16  NoWater         : 1;    // 53 Авария отсутствия воды
           u16  Avtovent        : 1;    // 54 Авария режима "Автовентиль"
           u16  ZaprFreq        : 1;    // 55 Авария "Запрещенная частота"
           u16  Nedogruz        : 1;    // 56 Авария недогруза
           u16  KrayPoint       : 1;    // 57 Авария крайней точки рабочей кривой
           u16  ObrRemnya       : 1;    // 58 Авария обрыва ремня
           u16  Potok           : 1;    // 59 Авария отсутствия потока
           u16  SuhoyHod        : 1;    // 60 Авария сухого хода
           u16  ShortCycle      : 1;    // 61 Авария короткого цикла
           u16  DisbCur         : 1;    // 62 Авария дисбаланса токов
           u16  MODBUS             : 1;    // 63 Авария ведомого устройства Modbus
         } bit;
        u64 all;
    } AllFailsType;

//33-SlowFail_supplyPhLoss
//SlowFailMask_ZarTime        37
//#define SlowFailERROR_DI            48      //Внешняя авария
//#define SlowFailPhase3ERR          53       //Авария 3 фаз
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
	u16       resetFail3; //предыдущее состояние квитирования аварий от дискретного входа
	u64    prevFailState; //предыдущие состояния битов-флагов аварий
//	u16   countPnt_NotUsed;
}FAILCOMMON_State_type;

typedef struct{
    u64    prevFailState;
}failnvState_type;

#define     nV    (*(failnvState_type*)0)

//typedef union{	???? повторное объявление
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
//	u32                        curSlowFailMask; //маски текущих аварий  curFastFailMask,
	COMMONPARAMC28			 *mPnt28;
	COMMONPARAMM3			  *mPnt3;
	FAILCOMMON_State_type	   *sPnt;
 	failnvState_type          *nvAdr;
	AllFailsType      handResetMask; //маска битов-флагов, задающих режим ручного квитирования для соответствующих аварий
	AllFailsType      curFailState;   //маска битов-флагов состояния текущих аварий
	sPnt 			= (FAILCOMMON_State_type *)(ramPnt + progPnt->Pnt_State);	
	mPnt28  		= &GlobalC28;
	mPnt3   		= &GlobalM3;
	fMask.v1 		= 0ULL;
	nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nV);

	if (flagIsFirstCall)
	{
	    save_s16(ramPnt, progPnt->Pnt_HandFailReset2, 0);                              //возврат в состояние "Нет квитирования".
		//sPnt->countPnt_NotUsed=0;
		//Закрыли запоминание несквитированной аварии
	   // RdFramMicro(&(sPnt->prevFailState), &(nvAdr->prevFailState), sizeof(u64));       //на случай, если во время ожидания ручного квитирования было исчезновение напряжения
	    return &progPnt->Pnt_End;
	}
	if(load_s16(ramPnt, progPnt->Pnt_NotUsed )==0)  // квитировать все аварии 180905 суходольский
	{  handResetMask.all = 0xFFFFFFFFFFFFFFFFULL; 
	}
	else  // квитировать по списку
	 { 

	//--Формируем 64-разрядное слово, биты которого определяют активна ли функция ручного квитирования для соответсвующих битам аварий
		handResetMask.all = 0;  //начальный сброс слова-маски, биты которого разрешают/запрещают ручное квитирование для соотв. аварий
		for(i = 0; i < 20; i++) //20 - максимально возможное количество аварий, квитируемых вручную
		{
		  
			if (load_s16(ramPnt, progPnt->Pnt_Fail[i])) //определяем назначена ли определенная авария для текущего входа квитируемой аварии
			{
				handResetMask.all |= (u64)1 << load_s16(ramPnt, progPnt->Pnt_Fail[i]); //лог. 1 будет в бите с обозначением, соотв. коду квитируемой вручную аварии (0...63)
			}
		 
		}
	}

//-- Если включен режим ручного сброса то будем помнить аварии до поступления сигнала сброса

	hendReset = load_s16(ramPnt, progPnt->Pnt_AckFailOff);        //считываем состояние сигнала включения/выключения режима ручного квитирования аварий
//	if(hendReset==1)
//	{GlobalM3.Kvitirovanie|=0x0400;
//     	Kvitirovanie|=0x8000;
//	}
// 	else
//	{GlobalM3.Kvitirovanie &=~(0x0400);
//	    Kvitirovanie &=~(0x8000);
//	}
	ManResMode  =  !testBitMicro(ramPnt, progPnt->Pnt_ManResEn);  //считываем текущий активный режим Ручной/Авто
	curFailState.all = 0;                                         //начальная инициализация переменной, биты которой отображают фактическое наличие/отсутствие аварий (по факту, а не с учетом квитированы ли аварии вручную или нет)
	if (hendReset && ManResMode)                                  //если активирован режим ручного квитирования аварий и режим работы привода не автоматический
	{
	    curFailState.all = (u64)(mPnt28->FastFailMask.all);       //считываем слово битов-флагов текущих быстрых аварий. Младшие 32 бита curFailState будут содержать флаги быстрых аварий
	    fMask.v2[0] = (~((u32)curFailState.all) & (u32)(sPnt->prevFailState) & (u32)handResetMask.all) | (u32)curFailState.all; //состояние аварий для микропрограммы с учетом того, что для некоторых аварий активирован ручной сброс
	    sPnt->prevFailState |= (u64)fMask.v2[0] & handResetMask.all; //запоминаем текущее состояние аварий (тех, для которых разрешено ручное квитирование)
	 //Закрыли запоминание несквитированной аварии
		// WrFramMicro(&(nvAdr->prevFailState), &(sPnt->prevFailState), sizeof(u64)); //сохраняем на случай исчезновения питания во время ожидания сигнала ручного квитирования
	}
	else //иначе ручное квитирование отключено и передаем в программу аварии по факту их наличия
	{
	    sPnt->prevFailState = 0;
		//Закрыли запоминание несквитированной аварии
	 //   WrFramMicro(&(nvAdr->prevFailState), &(sPnt->prevFailState), sizeof(u64)); //при отключенном ручном квитировании предудщее состояние всегда по нулям
	    fMask.v2[0] = mPnt28->FastFailMask.all;                                    //считываем слово битов-флагов текущих быстрых аварий
	}


//-- Считываем входы аварий	(не быстрых)
	FPnt = &(progPnt->Pnt_F_StopKey);     //адрес переменной с величиной смещения для доступа к сигналу аварийного стопа
	for(i = 0; i < 32; i++)               //перебираем 32 входа аварий начиная с аварийного стопа
	{
		if(testBitMicro(ramPnt, *FPnt++)) //считываем состояние текущего входа аварии. Если по текущему FPnt авария есть..
		{
			fMask.v2[1] |= ((u32)1 << i);					   //fMask bits 32-63  //Накапливаем аварии в ячейку fMask.v2[1] (формируем слово с битами-флагами медленных аварий)
			curFailState.all |= ((u64)1 << (i+32));            //Текущее физическое состояние аварий аварий
			if(NumFailDRVLocal == 0) NumFailDRVLocal = (32+i); //Если авария зафиксирована и это первая зафиксированная авария, то тогда запоминаем  номер аварии, то есть ловим первую попавшуюся аварию
		}
		else //иначе, по текущему  FPnt фактически  аварии нет, но если авария была и активирован ручной сброс то аварию будем хранить и далее до поступления ручного сброса
		{											 //Квитирование аварий
			if((SLOWFAILWOACK & ((u64)1 << i)) == 0) //на данный момент это условие всегда истинно, так как i принимает максимальное значение 31, а SLOWFAILWOACK = 1 << 62
			{
			    if (hendReset && ManResMode)         //если активирован ручной сброс (при этом аварии по текущему i нету, раз мы здесь)
			    {
			        fMask.v2[1] |= (u32)(sPnt->prevFailState >> 32) & (u32)(handResetMask.all >> 32) & (1 << i);
			        curFailState.all &= ~((u64)1 << (i+32)); //Запоминаем физическое отсутствие аварий для ситуации поступления сигнала квитирования аварий
			        if (((fMask.v2[1] & (1 << i)) == 0) && (NumFailDRVLocal == (32+i))) //если подтверждено что аварии нету, и именно нет той аварии, которая была обнаружена первой
			        {
			            NumFailDRVLocal = 0; //если данная авария была обнаружена и ранее и был сохранен ее номер, но теперь ее нет, то сбросить ее номер. Сбрасываем номер той аварии которая была обнаружена первой и номер которой был сохранен
			        }
			        else if (((fMask.v2[1] & (1 << i)) != 0) && (NumFailDRVLocal == 0))//иначе, фактически аварии нет, но она была и для нее активировано ручное квитирование
			        {
			            NumFailDRVLocal = (32+i);
			        }
			    }
			    else //иначе ручное квитирование отключено, и обнуляем флаг-бит текущей аварии
			    {
			        fMask.v2[1] &= ~((u32)1 << i);                     //fMask bits 32-63. Обнуляем флаг-бит, соответствующий текущему номеру отсутствующей аварии
			        if(NumFailDRVLocal == (32+i)) NumFailDRVLocal = 0; //если данная авария была обнаружена и ранее и был сохранен ее номер, но теперь ее нет, то сбросить ее номер. Сбрасываем номер той аварии которая была обнаружена первой и номер которой был сохранен
			    }
			}
		}
	}

	if (hendReset && ManResMode) //если разрешено ручное квитирование, то запишем состояние аварий в FRAM
	{
	    sPnt->prevFailState |= ((u64)fMask.v2[1] << 32) & handResetMask.all ;     //запоминаем  текущее состояние флагов-битов медленных аварий
		//Закрыли запоминание несквитированной аварии
	  //  WrFramMicro(&(nvAdr->prevFailState), &(sPnt->prevFailState), sizeof(u64)); //запись в FRAM состояние флагов-битов  аварий
	}


	mPnt3->NumSlowFail = NumFailDRVLocal;                     //передаем номер первой попавшейся медленной аварии в глобальный объект. Сохраняем номер медленной аварии
	if((mPnt28->NumFailDRV > 0) && (mPnt28->NumFailDRV < 32)) //если была быстрая авария..
	{
    	NumFailDRVLocal = mPnt28->NumFailDRV;                 //то переинициализируем NumFailDRVLocal номером быстрой аварии (номер медленной до этого уже считан и сохранен)
    }
	else //иначе, фактически быстрой аварии нет, но если она была и для нее активировано ручное квитирование, то fMask.v2[0] будет все равно содержать ее признак до момента ручного квитирования
	{
	    for (i = 0; i < 32; i++)
	    {
	        if (fMask.v2[0] & (1 << i)) //если по текущему i хранится признак аварии
	        {
	            NumFailDRVLocal = i;    //сохраняем ее номер и выходим из цикла
	            break;
	        }
	    }
	}


#ifdef _PROJECT_FOR_CCS_
	if(mPnt28->FastFailMask.all == 0) //если быстрых аварий нет
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

	//-Ручное квитирование аварий
	if (hendReset && ManResMode) //если разрешено ручное квитирование аварий
	{
	    resetFail1 = testBitMicro(ramPnt, progPnt->Pnt_HandFailReset1); //сброс 1
	    resetFail2 = load_s16(ramPnt, progPnt->Pnt_HandFailReset2);     //сброс 2
	    if((!sPnt->resetFail3) && testBitMicro(ramPnt, progPnt->Pnt_HandFailReset3)) //если не было квитирования от дискретного входа, но теперь есть
	    {
	        resetFail3 = 1; //сброс 3
	        sPnt->resetFail3 = 1; //запоминаем то, что сброс от дискретного входа осуществлено квитирование аварий
	    }
	    else if (!testBitMicro(ramPnt, progPnt->Pnt_HandFailReset3)) //если сигнал сброса от дискретного входа исчез
	    {
	        sPnt->resetFail3 = 0; //разрешим повторное квитирование от дискретного входа
	        resetFail3 = 0;
	    }
	     if (resetFail1 || resetFail2 || resetFail3)                 //если поступил сигнал ручного квитирования
	    {
	        fMask.v1 &= ~(handResetMask.all & (~curFailState.all));    //Обнуляем те флаги аварий, для которых разрешен ручной сброс и фактически эти аварии отсутствуют
	        clrBitMicro(ramPnt, progPnt->Pnt_HandFailReset1);    //возврат в состояние "Нет квитирования"
	        save_s16(ramPnt, progPnt->Pnt_HandFailReset2, 0);    //возврат в состояние "Нет квитирования"
	        sPnt->prevFailState = fMask.v1;
			 //0x8000- разрешено ручно квитир  0ч4000 - произошло квитирование  0ч2 - 0ч25  0x1 0x23
           
        if((NumFailDRVLocal==0x23)||(NumFailDRVLocal==0x35))GlobalM3.FlagCommand.bit.Kvitirov_R=1; //обрыв 3фаз или  авария торм сопрот

			//Закрыли запоминание несквитированной аварии

	    }
	}

    //-- Отключение не актуальных аварий
//  load_s16(ramPnt, progPnt->Pnt_AckFailOff);          //???? Отключить режим принудительного квитирования аварий

    if(!load_s16(ramPnt, progPnt->Pnt_FailStopOn)) //если отключение заряда Ud при аварии аварийного стопа не активировано
    {
        fMask.v1 &= ~( (u64)1<<SlowFail_failButton); //исключим отключение заряда Ud при нажатии аварийного останова
    }
    if( testBitMicro(ramPnt, progPnt->Pnt_FairModOn) )  fMask.v1 &= ~(((u64)1<<SlowFail_failButton)|0xFFFF);        //Отключение аварий пожарного режима(все fMask.v2[1] кроме failButton)

//-- Сохранение масок	
	save_s16(ramPnt, progPnt->Pnt_FailMask0, fMask.v4[0]); //быстрые аварии
	save_s16(ramPnt, progPnt->Pnt_FailMask1, fMask.v4[1]); //быстрые аварии
	save_s16(ramPnt, progPnt->Pnt_FailMask3, fMask.v4[2]); //медленные аварии
	save_s16(ramPnt, progPnt->Pnt_FailMask4, fMask.v4[3]); //медленные аварии
	FailMask_set();	

	save_s16(ramPnt, progPnt->Pnt_FailFixDrv      , mPnt28->FailValue.NumFail);
	save_s16(ramPnt, progPnt->Pnt_FailTekDrv      , NumFailDRVLocal  ); //передаем в микропрограмму номер быстрой (если она была) или медленной аварии (если не было быстрой)
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
