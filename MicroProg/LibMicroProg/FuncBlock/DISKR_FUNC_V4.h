#ifdef    GET_FUNC_CODE
typedef struct {
	u16 Pnt_Nfunc;
	u16 Pnt_State;
	u16 Pnt_nvState;
	u16 Pnt_AutoMode;               //1  AutoMode
	u16 Pnt_EnRefDin;               //2  EnRefDin
	u16 Pnt_FailTekDrv;             //3  FailTekDrv
	u16 Pnt_EnControlDin;           //4  EnControlDin
	u16 Pnt_Prior;                  //5  Prior
	u16 Pnt_Prior_revers;           //6  Prior_revers
	u16 Pnt_ReadyPusk;              //7  StandLift_Speed
	u16 Pnt_SpdMiss_PuskDis;        //8  SpdMiss_PuskDis
	u16 Pnt_DinFunc[13];           //9  Din1Func....Din13Func
	u16 Pnt_Sel_Stop2;              //22  Sel_Stop2
	u16 Pnt_Sel_StopVibeg;          //23  Sel_StopVibeg
	u16 Pnt_Sel_Forward;            //24  Sel_Forward
	u16 Pnt_Sel_Bacward;            //25  Sel_Bacward
	u16 Pnt_Sel_V1;                 //26  Sel_V1
	u16 Pnt_Sel_V2;                 //27  Sel_V2
	u16 Pnt_Sel_V3;                 //28  Sel_V3
	u16 Pnt_Sel_V4;                 //29  Sel_V4
	u16 Pnt_Sel_Bit0;               //30  Sel_Bit0
	u16 Pnt_Sel_Bit1;               //31  Sel_Bit1
	u16 Pnt_Sel_Bit2;               //32  Sel_Bit2
	u16 Pnt_Sel_Bit3;               //33  Sel_Bit3
	u16 Pnt_Sel_Up;                 //34  Sel_Up
	u16 Pnt_Sel_Down;               //35  Sel_Down
	u16 Pnt_Sel_Control;            //36  Sel_Control
	u16 Pnt_Sel_Ref;                //37  Sel_Ref
	u16 Pnt_Sel_FastTorm;           //38  Sel_FastTorm
	u16 Pnt_Sel_Evaс;               //39  Sel_Evaс
	u16 Pnt_Sel_ContOs;             //40  Sel_ContOs
	u16 Pnt_Sel_InspectEn;          //41  Sel_InspectEn
	u16 Pnt_Sel_TorqSpeed;          //42  Sel_TorqSpeed
	u16 Pnt_Sel_UF;                 //43  Sel_UF
	u16 Pnt_Sel_DryEn;              //44  Sel_DryEn
	u16 Pnt_Sel_Water;              //45  Sel_Water
	u16 Pnt_Sel_Clean;              //46  Sel_Clean
	u16 Pnt_Sel_ResWorkCnt;         //47  Sel_ResWorkCnt
	u16 Pnt_Sel_Relay[12];          //48  Sel_Relay1
	u16 Pnt_Sel_Motor[4];		//60  Sel_Motor1
	u16 Pnt_InputNew1;		//64  InputNew1
	u16 Pnt_InputNew2;		//65  InputNew2
	u16 Pnt_Sel_BrakeOs;		//66  InputNew3
	u16 Pnt_InputNew4;		//67  InputNew4
	u16 Pnt_InputNew5;		//68  InputNew5
	u16 Pnt_DinFailResetType;	//69  DinFailResetType
	u16 Pnt_Input_1;                //70  Input_1
	u16 Pnt_Input_2;                //71  Input_2
	u16 Pnt_AutoModeBit;            //72  AutoModeBit        
	u16 Pnt_NoSpdStop;              //73  NoSpdStop
	u16 Pnt_UserFault1;             //74  UserFault1
	u16 Pnt_UserFault2;             //75  UserFault2        
        u16 Pnt_WarnDFExist;            //76 WarnDFExist
        u16 Pnt_WarnDinBusy;            //77 WarnDinBusy        
	u16 Pnt_DF_EnPusk;              //78  DF_EnPusk
        u16 Pnt_DF_Pusk;                //79  DF_Pusk
	u16 Pnt_DF_Stop;                //80  DF_Stop
	u16 Pnt_DF_StopVibeg;           //81  DF_StopVibeg
	u16 Pnt_DF_Revers;              //82  DF_Revers
	u16 Pnt_DF_NumSel;              //83  DF_NumSel
	u16 Pnt_DF_Fup;                 //84  DF_Fup
	u16 Pnt_DF_Fdown;               //85  DF_Fdown
	u16 Pnt_DF_SelControl;          //86  DF_SelControl
	u16 Pnt_DF_SelChanRef;          //87  DF_SelChanRef
	u16 Pnt_DF_FastTorm;            //88  DF_FastTorm
	u16 Pnt_DF_Evak;                //89  DF_Evak
	u16 Pnt_DF_ContOs;              //90  DF_ContOs
	u16 Pnt_DF_InspectEn;           //91  DF_InspectEn
	u16 Pnt_DF_TorqSpeed;           //92  DF_TorqSpeed
	u16 Pnt_DF_SelUF;               //93  DF_SelUF
	u16 Pnt_PuskDryPulse;           //94  PuskDryPulse
	u16 Pnt_DF_NoWater;             //95  DF_NoWater
	u16 Pnt_DF_StartClean;          //96  DF_StartClean
	u16 Pnt_DF_ResCntWork;          //97  DF_ResCntWork
	u16 Pnt_DOut_K1_12;             //98  DOut_K1_12
	u16 Pnt_DinSelTime;             //99  DinSelTime
        u16 Pnt_DinExtStop;             //100  DinExtStop
	u16 Pnt_MotEn1_4;               //101  MotEn1_4
	u16 Pnt_DinFailReset;           //102  DinFailReset
	u16 Pnt_OutNew1;                //103  OutNew1
	u16 Pnt_OutNew2;                //104  OutNew2
	u16 Pnt_DF_BrakeOs;		//105  Brake_OS
	u16 Pnt_StopKey;                //106 StopKey
	u16 Pnt_End;
}FuncDISKR_FUNC_V4_type;
//********************************************************
#define		FDF	        (*(FuncDISKR_FUNC_V4_type*)0)
#define MAX_DISKR_IN 13
#define DISCRETE_FUNC_UST_SIZE      50
//********************************************************
typedef enum {
	LiftBitCode_SCB = 0,
	LiftNumSpeed_SCB,
	Joystick_SCB
}SpeedContrBit_type;
//********************************************************
typedef enum {
	SimpleTwoWireMode	= 0, // Управление через Вперед/Назад
	TwoWireMode	        = 1, // Управление через Вперед/Назад + Пуск1/Пуск2 + РазрПуск1/РазрПуск2;
	TripleWireMode		= 2  // // Управление через Вперед/Назад + Пуск1/Пуск2 + Стоп1/Стоп2;
}DiskrControlMode_type;
//********************************************************
typedef enum {
	Log1_pdf=0,
	Log0_pdf,
	Rise_pdf,
	Fall_pdf
}PolarDF_type;
//********************************************************
typedef enum {
	Revers_psr=0,
	Pusk1_psr,
	Pusk2_psr,
	PuskEn1_psr,
	PuskEn2_psr,
	Stop1_psr,
	Stop2_psr,
	Forward_psr,
	Backward_psr
}PsrDF_type;
//********************************************************
typedef enum {
	Revers_en=0,
	Pusk1_en,
	Pusk2_en,
	PuskEn1_en,
	PuskEn2_en,
	Stop1_en,
	Stop2_en,
	Forward_en,
	Backward_en
}EnableDF_type;
//********************************************************
typedef enum { // Коды для режимов дискретного управления
	DiskrNoneMode = 0,
	Simple2WireMode = (1<<Forward_en)+(1<<Backward_en),
	Simple2WireForward = (1<<Forward_en),
	Simple2WireBackward = (1<<Backward_en),
	Full2WireMode = Simple2WireMode + (1<<Pusk1_en)+(1<<Pusk2_en)+(1<<PuskEn1_en)+(1<<PuskEn2_en),
	Full2WireForward = Simple2WireForward + (1<<Pusk1_en)+(1<<PuskEn1_en),
	Full2WireBackward = Simple2WireBackward + (1<<Pusk2_en)+(1<<PuskEn2_en),
	Full3WireMode = Simple2WireMode + (1<<Pusk1_en)+(1<<Pusk2_en)+(1<<Stop1_en)+(1<<Stop2_en),
	Full3WireForward = Simple2WireForward + (1<<Pusk1_en)+(1<<Stop1_en)+(1<<Stop2_en),
	Full3WireBackward = Simple2WireBackward + (1<<Pusk2_en)+(1<<Stop1_en)+(1<<Stop2_en),
	// Дополнительные объявления для проверки режимов пуска/стопа трехпроводной схемы
	Start3Wire = /*Simple2WireMode +*/ (1<<Pusk1_en)+(1<<Pusk2_en),
	Start3WireForward = /*Simple2WireForward +*/ (1<<Pusk1_en),
	Start3WireBackward = /*Simple2WireBackward +*/ (1<<Pusk2_en),
	Stop3Wire = (1<<Stop1_en)+(1<<Stop2_en),
	Stop3WireForward = (1<<Stop1_en),
	Stop3WireBackward = (1<<Stop2_en),
}DiskrCntrlMode_type;
//********************************************************
typedef enum { // Назначение функций на дискреты
            PuskFwdLevel        = 1,
            PuskFwdRise         = 2,
            PuskFwdFall         = 3,
            PuskBkwdLevel       = 4,
            PuskBkwdRise        = 5,
            PuskBkwdFall        = 6,
            StopRise            = 7,
            StopFall            = 8,
            ErrorReset          = 9,
            PuskPermis          =10,
            PuskDenied          =11,
            EmergencyStop       =12,
            FreeRunStop         =13,
            DinUpFreq           =14,
            DinDownFreq         =15,
            FreqBit0            =16,
            FreqBit1            =17,
            FreqBit2            =18,
            FreqBit3            =19,
            FixedFreq1          =20,
            FixedFreq2          =21,
            FixedFreq3          =22,
            FixedFreq4          =23,
            AccelChangeLift     =24,// Изменение темпа в лифте
            SelExternStop       =25,// Внешний стоп в лифте 
            ContactorFdb        =26,// ОС Контактора Лифт
            BrakeFdb1           =27, // ОС от тормоза 1 
            Evacuation          =28, // Выбор режима эвакуации
            DinSwitchUF         =29, // Выбор переключения х-ки UF
            DinSwitchControl    =30, // Выбор переключения канала управления
            DinSwitchReference  =31, // Выбор переключения канала задания            
            UserFault1          =32, // Авария пользователя 1
            UserFault2          =33, // Авария пользователя 2
            
            ReverseRise         =40,
            ReverseFall         =41,
            BrakeFdb2           =42, // ОС от тормоза 2            
}DiskrFuncMode_type;
//********************************************************
typedef union {
	u64 v1;
	u32 v2[2];
	u16 v4[4];
}u64u32u16_type;
//********************************************************
typedef enum {
    Log1Mode = 0,
    Log0Mode = 1,
    RiseMode = 2, // Фронт
    FallMode = 3, // Спад
} DiscrSignalMode_enum;

typedef enum {
  NoSpdDisable = 0,
  NoSpdWarnMode = 1,
  NoSpdFaultMode = 2,  
} NoSpdMode_enum;

#define DOUBLE_DIN_FUNC_SIZE  3
typedef struct {
    u16   func;
    u16   pUst;
    u16   prevDin;
} DoubleDinFunc_type;
//********************************************************
typedef enum { // Перечень виртуальных дискретных функций, необходимых для работы - аналог уставок на схеме, которые были раньше
               // ВНИМАНИЕ!! ПОРЯДОК ФУНКЦИЙ НЕ ИЗМЕНЯТЬ. ПО КОДУ ЕСТЬ ЦИКЛЫ И ПОРЯДОК СЛЕДОВАНИЯ ВАЖЕН!!!
            FuncNone = 0,
            FuncForward,
            FuncBackward,
            // Перебор в цикле
            FuncReverse,
            FuncPusk1,
            FuncPusk2,
            FuncPuskEn1, // Пока не используются, добавлены для стыковки
            FuncPuskEn2, // Пока не используются, добавлены для стыковки
            FuncStop1,
            FuncStop2,
            // Перебор в цикле
            FuncPolarReverse,
            FuncPolarPusk1,
            FuncPolarPusk2,              
            FuncPolarPuskEn1, // Пока не используются, добавлены для стыковки
            FuncPolarPuskEn2, // Пока не используются, добавлены для стыковки              
            FuncPolarStop1,
            FuncPolarStop2,
            //------------------            
            FuncDinFailReset, 
            FuncPuskPermission, // Разрешение пуска, он же внешняя блокировка
            FuncPuskDenied,     // Запрет пуска
            FuncEmergencyStop,  // Аварийный останов
            FuncFreeRunStop,    // Останов выбегом
            FuncDinUpFreq, 
            FuncDinDownFreq, 
            FuncFreqBit0, 
            FuncFreqBit1, 
            FuncFreqBit2,  // №20 
            FuncFreqBit3, 
            FuncFixedFreq1, 
            FuncFixedFreq2,
            FuncFixedFreq3,
            FuncFixedFreq4, 
            FuncAccelChangeLift,
            FuncExternalStop,
            FuncContactorFdb,
            FuncBrakeFdb1,
            FuncEvacuation, // №30 
            FuncDinSwitchUF, //  Переключение UF
            FuncDinSwitchControl, // Переключение канала управления
            FuncDinSwitchReference, // Переключение канала задания
            FuncUserFault1,         // Авария пользователя 1
            FuncUserFault2,
            
            FuncBrakeFdb2, 
            FuncLastFuncName,		// Последнее значение функции, далее которого все я считаю ошибкой
} DiscreteFuncUst_enum; 
//********************************************************
typedef struct {
  union {
    struct {
      u16 ePuskPuls           : 1;
      u16 PuskPuls            : 1;
      u16 AutoPuls            : 1;
      u16 PuskDryPulse        : 1;
      u16 ReversePuls         : 1;
      u16 BlockStartCmd       : 1;
    }fl;
    u16 allFl;
  }pulsVal;
  u16     ValDFunc;                       //битовая маска дискретных функций
  u16     AutoMode;   
  u16     prevEnControlDin;				  // Разрешение управления по дискретам-  предыдущее состояние
  u16     forward_Pusk;                  // Статики команд направления, при двухпроводной схеме, для отработки фронта в работе
  u16     backward_Pusk;                 // Статики команд направления, при двухпроводной схеме, для отработки фронта в работе
  u16     prevWorkEnb;                   // Предыдущее состояние флага измерения параметров
  u16     dualDirErr;
  u16     EvacTimeoutCnt;                // Счетчик разрешения формирования сигнала эвакуации. Пока счетчик не досчитал до нуля, сможем формировать сигнал эвакуации
  
  //u16     DinDiskrFunc[MAX_DISKR_IN];    // Копия состояния дискретных входов и их назначенных дискретных функций в соответствии со схемой 
  u16     DiscreteFuncSetDin[DISCRETE_FUNC_UST_SIZE]; // Виртуальные уставки дискретных функций - заменитель уставок, в которых храним назначенные дискретные входы - Список DiscreteFuncUst_enum
  u16     DinFuncTbl[MAX_DISKR_IN];      // Предыдущее состояние назначений дискретных входов
  DoubleDinFunc_type     DoubleDinFunc[DOUBLE_DIN_FUNC_SIZE];        
}StateFuncDISKR_FUNC_V4;
//********************************************************
typedef struct{
	u16     AutoMode;                       // Значение режима - Ручной / Авто
        u16     DinFuncTbl[MAX_DISKR_IN];      // Назначение дискретных входов и их назначенных дискретных функций в соответствии со схемой 
        u16     crc;
}nvFuncDISKR_FUNC_V4;
//********************************************************
const u16 TblPriorSpeed[2][16]= {
	{0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4},		/*	Старший По убыванию     */
        {0,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1},		/*	Младший  По возрастанию	*/
};
const u16 TblBitSpeed[16]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}; /*	0,СД,СР,СН,СВ,П1,П2,П3	*/

#define         nvDiskrFunc    (*(nvFuncDISKR_FUNC_V4*)0)
void SetNewDiscreteFunction (u32 ramPnt, FuncDISKR_FUNC_V4_type *progPnt, StateFuncDISKR_FUNC_V4 *sPnt, u16 diskrNum, u16 diskrFunc);
void ClearDiscreteFunction  (u32 ramPnt, FuncDISKR_FUNC_V4_type *progPnt, StateFuncDISKR_FUNC_V4 *sPnt, u16 diskrNum, u16 diskrFunc);

//********************************************************
u16 *FuncDISKR_FUNC_V4_1(FuncDISKR_FUNC_V4_type *progPnt, u32 ramPnt)
{
	s16 autoMode; // PuskDisable
	u16 temp, diskrNum, funcIndex, ChangeDiscrSet;
	s16 u1, u2, e1, e2, f1, f2, i, j;
	s16 pusk, UstRecoveryFlagValue;
	u16 *selPnt; // *outPnt;
	u64u32u16_type Input;
	StateFuncDISKR_FUNC_V4 *sPnt;
        nvFuncDISKR_FUNC_V4     *nvAdr, nvMem;
	u16	Rise, Fall, FuncDiskr, EnableDiskrFunc, PuskStatus, crc, SwitchToControlDin;
        u16     diskrFunc; //DIN1_FUNC, DIN2_FUNC, DIN3_FUNC, DIN4_FUNC, DIN5_FUNC, DIN6_FUNC, DIN7_FUNC, DIN8_FUNC, DIN9_FUNC, DIN10_FUNC, DIN11_FUNC, DIN12_FUNC, DIN13_FUNC;
        void    *nvAdr1;
        u16     *dinPnt;
        u16     DinFuncTbl[MAX_DISKR_IN];
        NoSpdMode_enum  NoSpdMode;
	u16	ForwardDir, BackwardDir, EvacOn;
        u8 readyPusk;
        u16     pFunc, doubleDin, doubleDinFunc, doubleDinUstDiscard, mainDinFunc;//, nvSaveFlag;        
        
	sPnt = (StateFuncDISKR_FUNC_V4 *)(ramPnt + progPnt->Pnt_State);
        nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvDiskrFunc);
	autoMode = load_s16(ramPnt, progPnt->Pnt_AutoMode);
	defBitMicro(ramPnt, progPnt->Pnt_AutoModeBit, autoMode);
        EvacOn = 0; // Начальная инициализация        
        pFunc = 0;

        if(flagIsFirstCall) {  //начальная инициализация и вычитка из FRAM
          RdFramMicro(&nvMem, nvAdr, sizeof(nvFuncDISKR_FUNC_V4));
          crc = crcPacket((unsigned char *)&nvMem,  sizeof(nvFuncDISKR_FUNC_V4) - sizeof(u16));
          
          nvAdr1 = (void *)((u32)&nv.UstRecoveryFlag);
          RdFramMicro( &UstRecoveryFlagValue, nvAdr1,  sizeof(u16)); // Проверяем флаг восстановления уставок
          
          if (crc != nvMem.crc || UstRecoveryFlagValue) {
            nvMem.AutoMode = autoMode; // load_s16(ramPnt, progPnt->Pnt_AutoMode); // Читаем режим с уставки
            // Читаем настройки дискрeтных входов
            dinPnt = progPnt->Pnt_DinFunc;
            for (i=0; i<MAX_DISKR_IN; i++, dinPnt++){
              nvMem.DinFuncTbl[i] = load_s16(ramPnt, *dinPnt); //123123
            }
            nvMem.crc      = crcPacket((unsigned char *)&nvMem,  sizeof(nvFuncDISKR_FUNC_V4) - sizeof(u16));
            WrFramMicro(nvAdr, &nvMem, sizeof(nvFuncDISKR_FUNC_V4));
          }   
          
          sPnt->AutoMode    = nvMem.AutoMode;
          save_s16(ramPnt, progPnt->Pnt_AutoMode, sPnt->AutoMode);
          sPnt->prevEnControlDin = (u16)testBitMicro(ramPnt, progPnt->Pnt_EnControlDin); // Запоминаем текущее состояние управления по дискретам.
          sPnt->dualDirErr = 0;
          sPnt->EvacTimeoutCnt = 5*K_TIME_CALL_MICRO_TAKT; // Заводим таймер для эвакуации на 5 секунд по старту.
          // Заносим данные в систему параметров        
          dinPnt = progPnt->Pnt_DinFunc;
          for (i=0; i<MAX_DISKR_IN; i++, dinPnt++){
            sPnt->DinFuncTbl[i] = nvMem.DinFuncTbl[i]; // Копию в sPnt
            save_s16(ramPnt, *dinPnt, sPnt->DinFuncTbl[i]); // и в уставки
          }
                      
          for (i=0; i<DISCRETE_FUNC_UST_SIZE; i++){            
            sPnt->DiscreteFuncSetDin [i] = 0; // Обнулить в назначения всех дискретных входов у функций
          }
             
          // Набиваем массив дублеров уставок дискретных ФУНКЦИЙ
          sPnt->DoubleDinFunc[0].func = DinSwitchControl;
          sPnt->DoubleDinFunc[0].pUst = progPnt->Pnt_Sel_Control;
          sPnt->DoubleDinFunc[1].func = DinSwitchReference;
          sPnt->DoubleDinFunc[1].pUst = progPnt->Pnt_Sel_Ref;
          sPnt->DoubleDinFunc[2].func = DinSwitchUF;
          sPnt->DoubleDinFunc[2].pUst = progPnt->Pnt_Sel_UF;           
          
        } else { // В обычном режиме работы сверяем 
          if (autoMode != sPnt->AutoMode) { // Если режим поменяли, надо его сохранить
            RdFramMicro(&nvMem, nvAdr, sizeof(nvFuncDISKR_FUNC_V4));
            nvMem.AutoMode = autoMode; // load_s16(ramPnt, progPnt->Pnt_AutoMode); // Читаем режим с уставки
            nvMem.crc      = crcPacket((unsigned char *)&nvMem,  sizeof(nvFuncDISKR_FUNC_V4) - sizeof(u16));
            WrFramMicro(nvAdr, &nvMem, sizeof(nvFuncDISKR_FUNC_V4));
            sPnt->AutoMode = autoMode;
          }
        }

        //-----------------------------------------------
        
        clrBitMicro(ramPnt, progPnt->Pnt_WarnDFExist);
        clrBitMicro(ramPnt, progPnt->Pnt_WarnDinBusy);
        // Основная обработка дискретных функций       
        // 1. Читаем назначение дискретных входов - новое на данный момент входа в п/п
        dinPnt = progPnt->Pnt_DinFunc;
        for (i=0; i<MAX_DISKR_IN; i++, dinPnt++){
          DinFuncTbl[i] = load_s16(ramPnt, *dinPnt);
        }        
        
        // 2. Вычитка назначений уставок двойников из общего массива указателей и проверка свободен ли дискретный вход для этой функции.        

        for (i=0; i<DOUBLE_DIN_FUNC_SIZE; i++){
          doubleDinUstDiscard = 0; // Флаг отказа сохранения данной функции
//          #ifdef _PROJECT_FOR_STM32_ 
          pFunc = sPnt->DoubleDinFunc[i].pUst;
          doubleDin = load_s16 (ramPnt, pFunc); // Читаем назначение функции на Двх    
		  if (doubleDin > FuncLastFuncName)	doubleDin = 0; // Если назначение функции выходит за диапазон, обнуляем его
	   //   #else
//		    pFunc = sPnt->DoubleDinFunc[i].pUst;
//		    doubleDin = load_s16 (ramPnt, pFunc); // Читаем назначение функции на Двх
//          //#endif

          doubleDinFunc = sPnt->DoubleDinFunc[i].func;
          
          if (doubleDin == sPnt->DoubleDinFunc[i].prevDin)	continue;

          if (doubleDin) { // Если не ноль, значит есть назначение
            doubleDin --; // Значение уменьшаем на 1, для правильного позиционирования по массиву DinFuncTbl
            // Проверяем занятость дискретного входа
            if (DinFuncTbl[doubleDin] && DinFuncTbl[doubleDin] != doubleDinFunc) { // Если в таблице у этого дискрета есть назначение - там не ноль, и при этом это не тоже самое назначение
              // Тогда выводим предупреждение - Вход занят
              setBitMicro(ramPnt, progPnt->Pnt_WarnDinBusy);
              doubleDinUstDiscard = 1; // Отказать в назначении функции 
              save_s16 (ramPnt, pFunc, 0); // СБросить назначение дублирующей уставки - МОЖЕТ БЫТЬ НЮАНС при работе, если выбор идет через Двх, здесь могу сбросить настройку!!!!!
            }
                        
            // Отказ в назначении есть?
            if (!doubleDinUstDiscard) {
              // Если предыдущее назначение дискрета не равно нулю, а новое равно нулю - значит функцию сбросили
              if (sPnt->DinFuncTbl[doubleDin] && !DinFuncTbl[doubleDin]) {
                DinFuncTbl[doubleDin] = 0;
              } else {
                // Присваиваем функцию, но проверим перенос дина на другой. И при этом мне надо сбросить старый Дин.
                if (doubleDin != (sPnt->DoubleDinFunc[i].prevDin-1)) { // Если Дин поменялся через вторичную функцию (prevDin-1 - потому что doubleDin ранее уменьшен на 1, для нормальной индексации по DinFuncTbl)
                  DinFuncTbl[sPnt->DoubleDinFunc[i].prevDin - 1] = 0; // Обнуляем сторое значение Дин функции.
                }
                DinFuncTbl[doubleDin] = doubleDinFunc; // Присвоить функцию.  
              }
            }                         
          } else {
            if (sPnt->DoubleDinFunc[i].prevDin && !doubleDin) {
              DinFuncTbl[sPnt->DoubleDinFunc[i].prevDin - 1] = 0;
            }
          }
        }
                                                  
        for (i=0; i < MAX_DISKR_IN; i++){ // Мастер значение
          for (j=0; j < MAX_DISKR_IN; j++) { // изменяемое значение
            if ( (i != j) && (DinFuncTbl[i] == DinFuncTbl[j]) && DinFuncTbl[i]) {
              // Значит j вход надо сбросить, с кем-то повторяется              
              DinFuncTbl[j] = 0;
              dinPnt = progPnt->Pnt_DinFunc + j;
              save_s16(ramPnt, *dinPnt, 0); // Сброс функции 
              setBitMicro(ramPnt, progPnt->Pnt_WarnDFExist);
            }                       
          }          
        }         
            
        // Проверяем назначение функций на дискретные входы
        ChangeDiscrSet = 0; // Обнулить флаг изменения назначенных функций
        for (i=0; i < MAX_DISKR_IN; i++){  
          diskrFunc = DinFuncTbl[i]; // Определяем тип функции, а номер дискрета находится в i
          
          // Делаем проверку на изменение дискретной функции у входа
          if ((sPnt->DinFuncTbl[i] != diskrFunc) && sPnt->DinFuncTbl[i]) { // 123123
            ClearDiscreteFunction (ramPnt, progPnt, sPnt, diskrNum, sPnt->DinFuncTbl[i]); // 123123 Очищаем ранее назначенную функцию
            ChangeDiscrSet = 1; // Выявлен запрос на обнуление функции, ставим флаг - в системе что-то поменялось
          }
          
          // Выполняем настройку параметров системы управления в цикле по массиву функций                    
          diskrNum = i+1;
          SetNewDiscreteFunction (ramPnt, progPnt, sPnt, diskrNum, diskrFunc);

          if (sPnt->DinFuncTbl[i] != diskrFunc) { // 123123
            ChangeDiscrSet = 1; // Выявлен запрос на обнуление функции, ставим флаг - в системе что-то поменялось
          }
          sPnt->DinFuncTbl[i] = diskrFunc; // 123123 Обновить State
          dinPnt = progPnt->Pnt_DinFunc + i;
          save_s16(ramPnt, *dinPnt, diskrFunc); // обновление функции в параметре
          
        }
        
//        // После окончательного выявления назначений делаем копию для предыдущего состояния
//        for (i=0; i < MAX_DISKR_IN; i++){
//          // Проверяем на изменение и если что, ставим флаг сохранения в nv (FRAM)
//          if (sPnt->DinFuncTbl[i] != DinFuncTbl[i])     nvSaveFlag = 1;
//          
//          sPnt->DinFuncTbl[i] = DinFuncTbl[i]; 
//          dinPnt = progPnt->Pnt_DinFunc + i;
//          save_s16(ramPnt, *dinPnt, DinFuncTbl[i]); // обновление функции в параметре
//        }
        // Переносим назначения на вторичные уставки
        for (j=0; j<DOUBLE_DIN_FUNC_SIZE; j++){          
          doubleDinFunc = sPnt->DoubleDinFunc[j].func; 
          //#ifdef _PROJECT_FOR_STM32_ 
          pFunc = sPnt->DoubleDinFunc[j].pUst;
          save_s16 (ramPnt, pFunc, 0); // Сперва обнулить
//          #else
//            pFunc = sPnt->DoubleDinFunc[j].pUst;
//            save_s16 (ramPnt, pFunc, 0); // Сперва обнулить
//          #endif
          sPnt->DoubleDinFunc[j].prevDin = 0;
          
          for (i=0; i<MAX_DISKR_IN; i++){
            mainDinFunc = DinFuncTbl[i];
            if (doubleDinFunc == mainDinFunc) { 
            //#ifdef _PROJECT_FOR_STM32_ 
                save_s16 (ramPnt, pFunc, i+1); //Ставим дискрет на вторичную уставку.
	        //  #else
		//		 save_s16 (ramPnt, pFunc, i+1); //Ставим дискрет на вторичную уставку.
	        // #endif
              sPnt->DoubleDinFunc[j].prevDin = i+1;
              break;
            }                        
          }
        }
        
        // Были изменения - заносим в FRAM
        if (ChangeDiscrSet) {
            // Читаем Fram
            RdFramMicro(&nvMem, nvAdr, sizeof(nvFuncDISKR_FUNC_V4));
            for (i=0; i<MAX_DISKR_IN; i++, dinPnt++){
              nvMem.DinFuncTbl[i] =  sPnt->DinFuncTbl[i];
            }                        
            nvMem.crc      = crcPacket((unsigned char *)&nvMem,  sizeof(nvFuncDISKR_FUNC_V4) - sizeof(u16));
            WrFramMicro(nvAdr, &nvMem, sizeof(nvFuncDISKR_FUNC_V4));          
        }
        
        //-----------------------------------------------
                                        
	Input.v2[0] = load_s32(ramPnt, progPnt->Pnt_Input_1);
	Input.v2[1] = load_s32(ramPnt, progPnt->Pnt_Input_2);       
        
        if(flagIsFirstCall)
        {
            // Используем виртуальные уставки функций
            u1 = sPnt->DiscreteFuncSetDin[FuncForward]; // направление вперед  
            u2 = sPnt->DiscreteFuncSetDin[FuncBackward];  // направление назад

            sPnt->forward_Pusk  = (s16)((Input.v1>>u1)&1); // наличие команды вперед - потенциальный сигнал или 0 или 1
            sPnt->backward_Pusk = (s16)((Input.v1>>u2)&1); // наличие команды назад  - потенциальный сигнал или 0 или 1
        }
        
        SwitchToControlDin = 0;
        // Если предыдущий режим управления по дискретам, а прошлый нет - то ставим флаг переключения на дискретное управление
        if (sPnt->prevEnControlDin == 0  && testBitMicro(ramPnt, progPnt->Pnt_EnControlDin) == 1) {
          SwitchToControlDin = 1;
        }
        sPnt->prevEnControlDin = (u16)testBitMicro(ramPnt, progPnt->Pnt_EnControlDin); // 
        
        // Контроль завершения измерения параметров, и синхронизация состояния дискретных сигналов
        if (sPnt->prevWorkEnb && !GlobalM4.FlagCommand.bit.mWorkEnb) {
          sPnt->pulsVal.fl.BlockStartCmd = 1;
        }        
        sPnt->prevWorkEnb = (u16)GlobalM4.FlagCommand.bit.mWorkEnb;
        
        
	if( testBitMicro(ramPnt, progPnt->Pnt_EnControlDin) == 0 ){
		clrBitMicro(ramPnt, progPnt->Pnt_DF_Pusk);
		clrBitMicro(ramPnt, progPnt->Pnt_DF_Stop);
		clrBitMicro(ramPnt, progPnt->Pnt_DF_StopVibeg);
		save_s16(ramPnt, progPnt->Pnt_DF_Revers, 0);
		sPnt->pulsVal.allFl = 0;
                PuskStatus = (1<<Stop1_psr) | (1<<Stop2_psr);
	}
	else{	// рассчитываем пуски и стопы если разрешено управление по дискретным входам
		//-- Рассчитываем логические уровни сигналов: реверс, пуск1, пуск2, разреш1, разреш2, стоп1, стоп2.
		EnableDiskrFunc = 0;
                funcIndex = FuncReverse;
		for(i = 1, e2 = 0; i != (1 << 7); i <<= 1, funcIndex++)
                {
                  u1 = sPnt->DiscreteFuncSetDin[funcIndex];
                  if (u1){ // Фиксируем факт, что вход назначен на дискретный вход, а не пустой
                    EnableDiskrFunc |= i;
                  }
                  
                  if((Input.v1 >> u1) & 1){ // Фиксируем текущее состояние входа функции
                    e2 |= i;									//Маска входных сигналов с учетом функций, назначенных дискретным входам
                  }
		}
                // На первом проходе, при аварии или при переключении на дискретное управление, надо запомнить стартовое состояние дискретных входов. Иначе фронт/спад могут глюкнуть.
                if (flagIsFirstCall || load_s16(ramPnt, progPnt->Pnt_FailTekDrv) || sPnt->pulsVal.fl.BlockStartCmd ||
                       SwitchToControlDin || ChangeDiscrSet || (!testBitMicro(ramPnt, progPnt->Pnt_ReadyPusk) && (!GlobalM4.FlagCommand.bit.PwmOn && !GlobalM4.FlagCommand.bit.mWorkEnb)) ) { 
                  sPnt->ValDFunc = e2;                  
                  u1 = sPnt->DiscreteFuncSetDin[FuncForward]; // направление вперед  
                  u2 = sPnt->DiscreteFuncSetDin[FuncBackward];  // направление назад                  
                  sPnt->forward_Pusk  = (s16)((Input.v1>>u1)&1); // наличие команды вперед - потенциальный сигнал или 0 или 1
                  sPnt->backward_Pusk = (s16)((Input.v1>>u2)&1); // наличие команды назад  - потенциальный сигнал или 0 или 1                  
		}
                                                               
		// Выполняем анализ срабатывания по заявленным переходам для: реверс, пуск1, пуск2, разреш1, разреш2, стоп1, стоп2.
		Rise = (~sPnt->ValDFunc) & e2;
		Fall = (sPnt->ValDFunc) & (~e2);
		sPnt->ValDFunc = e2; // е2 - это по сути состояния функций (Пуск, стоп, разрешение пуска) 
                funcIndex = FuncPolarReverse; // Начальный индекс для доступа к полярности
		FuncDiskr = 0;
		for(i = 1; i != (1 << 7); i <<= 1, funcIndex++){
                    u2 = sPnt->DiscreteFuncSetDin[funcIndex];
                    switch(u2)
                    {
                        case Log1_pdf: //если по состоянию лог.1	
                          e1 =   e2&i;	
                          break; 
                        case Log0_pdf: //если по состоянию лог.0	
                          e1 = !(e2&i);	
                          break; 
                        case Rise_pdf://по переднему фронту	
                          e1 = Rise&i;
                          if (e1) {
                              sPnt->ValDFunc &= (~i); // Для выявленного фронта надо сбросить предыдущее состояние в ноль, чтобы повторять сигнал фронта, до смены уровня сигнала на входе
                          }
                          break; 
                        case Fall_pdf://по заднему фронту	
                          e1 = Fall&i;
                          if (e1) {
                              sPnt->ValDFunc |= i; // Для выявленного спада надо вернуть предыдущее состояние в 1, чтобы повторять сигнал спада, до смены уровня сигнала на входе.
                          }
                          break; 
                        default: ;
                    }
                    if(e1){
                            FuncDiskr |= i;
                    }
		}
		// Как только ШИМ выключается - сбрасываем флаг блокировки пуска, и система будет ждать смены состояния для сигналов фронт/спад
		if (!GlobalM4.FlagCommand.bit.PwmOn) {
			sPnt->pulsVal.fl.BlockStartCmd = 0;
		}
		// Заносим в регистр FuncDiskr состояние направлений движения
                u1 = sPnt->DiscreteFuncSetDin[FuncForward]; // направление вперед  
                u2 = sPnt->DiscreteFuncSetDin[FuncBackward];  // направление назад                  
                
                if (u1) {EnableDiskrFunc |= (1<<Forward_en); }
                if (u2) {EnableDiskrFunc |= (1<<Backward_en); }
                ForwardDir  = (s16)((Input.v1>>u1)&1); // наличие команды вперед - потенциальный сигнал или 0 или 1
		BackwardDir = (s16)((Input.v1>>u2)&1); // наличие команды назад  - потенциальный сигнал или 0 или 1
                
                if(sPnt->forward_Pusk)
                {
                    if (sPnt->forward_Pusk != ForwardDir)
                        sPnt->forward_Pusk = 0;
                    else
                        ForwardDir = 0;
                }
                if(sPnt->backward_Pusk)
                {
                    if (sPnt->backward_Pusk != BackwardDir)
                        sPnt->backward_Pusk = 0;
                    else 
                        BackwardDir = 0;
                }
                
		FuncDiskr |= ForwardDir  ? (1<<Forward_psr)  : 0; 			// Ставим направление вперед
		FuncDiskr |= BackwardDir ? (1<<Backward_psr) : 0;			// Ставим направление назад

		//Далее нужен командоаппарат switch который на основании набора установленных сигналов EnableDiskrFunc будет
		// определять по какому алгоритму управления мы будем работать
		// Не забыть про таблицу приоритетов пуска
		// Не забыть про блокировку пуска без скорости
		// -------------------------------------------------
		// Проверяем команды пуска для всех схем подключения
		PuskStatus = 0; // По выходу будет содержать код направления куда надо дать пуск, или требование остановится
                
		switch (EnableDiskrFunc)        // Анализируем выбранные дискретные функции и определяем текущий режим управления
                { 
			case (Simple2WireMode): // Самый простой двухпроводный режим по двум сигналам
				PuskStatus = FuncDiskr & Simple2WireMode;
				if (!PuskStatus) {        // Если PuskStatus = 0, значит для двухпроводной схемы надо дать стоп-команду
                                  PuskStatus = (1<<Stop1_psr) | (1<<Stop2_psr);
				}
				break;
			//-----------------------
			case (Simple2WireForward): // Самый простой двухпроводный режим вперед
				PuskStatus = FuncDiskr & Simple2WireForward;
				break;
			//-----------------------
			case (Simple2WireBackward): // Самый простой двухпроводный режим назад
				PuskStatus = FuncDiskr & Simple2WireBackward;
				break;
			//-----------------------
			case (Full2WireMode): // Полная двухпроводная схема - проверяем комбинацию для пуска вперед и назад, и ставим активное направление
				PuskStatus |= ((FuncDiskr & Full2WireForward) == Full2WireForward)   ? Simple2WireForward  : 0;
				PuskStatus |= ((FuncDiskr & Full2WireBackward) == Full2WireBackward) ? Simple2WireBackward : 0;
				break;
			//-----------------------
			case (Full2WireForward): // Полная двухпроводная схема Вперед - проверяем комбинацию для пуска вперед, и ставим активное направление
				PuskStatus |= ((FuncDiskr & Full2WireForward) == Full2WireForward)   ? Simple2WireForward  : 0;
				break;
			//-----------------------
			case (Full2WireBackward): // Полная двухпроводная схема назад - проверяем комбинацию для пуска назад, и ставим активное направление
				PuskStatus |= ((FuncDiskr & Full2WireBackward) == Full2WireBackward) ? Simple2WireBackward : 0;
				break;
			//-----------------------
			case (Full3WireMode): // Полная Трехпроводная схема - проверяем комбинацию для пуска вперед и назад, и ставим активное направление
				PuskStatus |= ((FuncDiskr & Start3WireForward) == Start3WireForward)   ? Simple2WireForward  : 0;
				PuskStatus |= ((FuncDiskr & Start3WireBackward) == Start3WireBackward) ? Simple2WireBackward : 0;
				break;
			//-----------------------
			case (Full3WireForward): // Полная трехпроводная схема Вперед - проверяем комбинацию для пуска вперед, и ставим активное направление
				PuskStatus |= ((FuncDiskr & Start3WireForward) == Start3WireForward)   ? Simple2WireForward  : 0;
				break;
			//-----------------------
			case (Full3WireBackward): // Полная трехпроводная схема назад - проверяем комбинацию для пуска назад, и ставим активное направление
				PuskStatus |= ((FuncDiskr & Start3WireBackward) == Start3WireBackward) ? Simple2WireBackward : 0;
				break;
			//-----------------------
			default:
			case (DiskrNoneMode):
				PuskStatus = (1<<Stop1_psr) | (1<<Stop2_psr);
                                sPnt->ValDFunc = e2;
				break;
			//-----------------------
		}
		if (PuskStatus){
                  PuskStatus ++;
                  PuskStatus --;
		}

		// -------------------------------------------------
		// Проверяем команды стоп для всех схем подключения
		switch (EnableDiskrFunc){ // Анализируем выбранные дискретные функции и определяем текущий режим управления
			case (Simple2WireMode): // Для двухпроводных схем проверяем команду стоп
			case (Simple2WireForward):
			case (Simple2WireBackward):
			case (Full2WireMode):
			case (Full2WireForward):
			case (Full2WireBackward):
				if (!PuskStatus){ // Если PuskStatus = 0, значит для двухпроводных схем надо дать стоп-команду
                                  PuskStatus = (1<<Stop1_psr) | (1<<Stop2_psr);
				}
				break;
			//-----------------------
			case (Full3WireMode): // Для трехпроводных схем проверяем команду стоп
			case (Full3WireForward):
			case (Full3WireBackward):
				if (FuncDiskr & Stop3Wire) { // Если установлена команда Стоп - то заносим её в PuskStatus
                                  PuskStatus = FuncDiskr & Stop3Wire;
                                  sPnt->pulsVal.fl.BlockStartCmd = 1; // Блокируем команды пуска по фронтам/спадам
				}
				break;
			//-----------------------
			default:
			case (DiskrNoneMode):
				PuskStatus = (1<<Stop1_psr) | (1<<Stop2_psr);
				break;
			//-----------------------
		}                

		//  В этой точке в PuskStatus будет содержаться:
		//  или команда пуска - PuskStatus = (1<<Forward_en)| (1<<Backward_en)
		//  или команда стоп  - PuskStatus = (1<<Stop1_psr) | (1<<Stop2_psr)
		//  или ничего не делать - PuskStatus = 0
		GlobalM4.DebugDinOut = PuskStatus;

                // Ветка сброса аварии Два направления
                if (sPnt->dualDirErr && !sPnt->backward_Pusk && !sPnt->forward_Pusk) { // Если авария включена и сняты оба направления
                    sPnt->dualDirErr = 0; //  Сброс аварии
                }                
                
		// Проверяем приоритеты управления Вперед/Назад
                EvacOn = GlobalM4.FlagCommand.bit.EvacOn;
		if( PuskStatus == ((1<<Forward_en) | (1<<Backward_en)) ) {        // Если пришли два сигнала направления
                  e1 = load_s16(ramPnt, progPnt->Pnt_Prior_revers);	//Pnt_Prior_revers: 0-вперёд; 1-назад; 2-Автомат; 3-Останов
                  if (e1 == 0){ // Приоритет Вперед	 
                    PuskStatus = (1<<Forward_en);
                  } else if(e1 == 1) { // Приоритет Назад	 
                    PuskStatus = (1<<Backward_en);
                  } else if(!EvacOn && (e1 == 2)) { // Авария - два направления 
                    PuskStatus = (1<<Stop1_psr) | (1<<Stop2_psr); // Блокируем пуски и выдаем ваарию
                    sPnt->dualDirErr = 1;
                  } else if(!EvacOn && e1 == 3) { // Если режим Останов - даем команду стоп, если нет эвакуации
                    PuskStatus = (1<<Stop1_psr) | (1<<Stop2_psr);
                  }
		}

                // Установка аварии Два направления. Именно здесь, пока все выходные сигналы не разобраны.
                defBitMicro(ramPnt, progPnt->Pnt_DF_InspectEn, sPnt->dualDirErr);                
		// Анализ команд Старт/Стоп завершен
		//-----------------------------------

		// Анализируем выдачу сигнала реверс. Сброс в стопе, при PuskStatus=0 - сигнал реверса не меняем
		if (PuskStatus == ((1<<Stop1_psr) | (1<<Stop2_psr)))	
                    save_s16(ramPnt, progPnt->Pnt_DF_Revers, 0); // Реверса нету
		else if (PuskStatus == (1<<Backward_en))				
                    save_s16(ramPnt, progPnt->Pnt_DF_Revers, 1); // Есть реверс
		else if (PuskStatus == (1<<Forward_en))				    
                    save_s16(ramPnt, progPnt->Pnt_DF_Revers, 0); // Нет реверса
		//**********************************************************************

                //-- Расчёт	стопа
                u1 = PuskStatus & ((1<<Stop1_psr)|(1<<Stop2_psr));
                defBitMicro(ramPnt, progPnt->Pnt_DF_Stop, u1);
                if (u1) {
                  u1 ++;
                  u1 --;
                }
                //-- Расчёт пуск
                pusk = PuskStatus & ((1<<Forward_en)|(1<<Backward_en));
                
                u1 = pusk;
                u2 = load_s16(ramPnt, progPnt->Pnt_FailTekDrv);
                readyPusk = testBitMicro(ramPnt, progPnt->Pnt_ReadyPusk);
                f1 = u1 && (u2==0) && (!autoMode) && (readyPusk);				//ManualPusk = RisePusk && (FailTekDrv == 0) && (!autoMode)
                f2 = u1 && (u2==0) && (autoMode) && (readyPusk);	                        //AutoPusk = pusk && (FailTekDrv == 0) && autoMode
                // С Автоматическим режимом не понял, что тут происходит?!?!?
                u1 = (!sPnt->pulsVal.fl.AutoPuls && f2);			//Отлавливаем фронт AutoPusk
                sPnt->pulsVal.fl.AutoPuls = f2;					//сохраняем значение текущего такта
                u1 = u1 || f1;							//RiseAutoPusk || ManualPusk
                temp = u1;
                if(temp == 1)
                  temp = 1;
                defBitMicro(ramPnt, progPnt->Pnt_DF_Pusk, u1);        
                //---------------------------------------------------------------------                
                
	}

        //-- Расчёт дискретных скоростей
        e1 = load_s16(ramPnt, progPnt->Pnt_Sel_TorqSpeed);						//0-Бит Код; 1-Номер Скорости
        if(e1 == LiftBitCode_SCB){		//0-Бит- Код
          e1 = sPnt->DiscreteFuncSetDin[FuncFreqBit0];          
          e2 = sPnt->DiscreteFuncSetDin[FuncFreqBit1];          
          f1 = sPnt->DiscreteFuncSetDin[FuncFreqBit2];          
          f2 = sPnt->DiscreteFuncSetDin[FuncFreqBit3];          
                    
          e1 = (s16)((Input.v1>>e1)&1);
          e2 = (s16)((Input.v1>>e2)&1);
          f1 = (s16)((Input.v1>>f1)&1);
          f2 = (s16)((Input.v1>>f2)&1);
          
          f1 = ((e1<<0)|(e2<<1)|(f1<<2)|(f2<<3));							//Вначале идут 16 скоростей по джойстику, потом 8 лифтовых
          f2 = TblBitSpeed[f1];									//Вначале идут 16 скоростей по джойстику, потом 8 лифтовых
        }
        else if(e1 == LiftNumSpeed_SCB){	//1-Номер Скорости
          e1 = sPnt->DiscreteFuncSetDin[FuncFixedFreq1];          
          e2 = sPnt->DiscreteFuncSetDin[FuncFixedFreq2];          
          f1 = sPnt->DiscreteFuncSetDin[FuncFixedFreq3];          
          f2 = sPnt->DiscreteFuncSetDin[FuncFixedFreq4]; 
                    
          e1 = (s16)((Input.v1>>e1)&1);
          e2 = (s16)((Input.v1>>e2)&1);
          f1 = (s16)((Input.v1>>f1)&1);
          f2 = (s16)((Input.v1>>f2)&1);
          
          f1 = (e1<<0)|(e2<<1)|(f1<<2)|(f2<<3);
          e1 = load_s16(ramPnt, progPnt->Pnt_Prior);
          f2 = TblPriorSpeed[e1][f1];				        //Выбор направления приоритета
        }
        save_s16(ramPnt, progPnt->Pnt_DF_NumSel, f2);				//Запись номера скорости
                
        //---------------------------------------------------------------------
        // Проверяем блокировку пуска без задания скорости, если активен дискретный канал управления        
        NoSpdMode = (NoSpdMode_enum)load_s16(ramPnt, progPnt->Pnt_SpdMiss_PuskDis);
        save_s16 (ramPnt, progPnt->Pnt_NoSpdStop, 0); // Выход будет отражать как состояние блокировки, так и режим отображения: 1 - Warn, 2-Fault 
        if( (NoSpdMode > NoSpdDisable)  && testBitMicro(ramPnt, progPnt->Pnt_EnRefDin)){
          if((f1==0) && (GlobalM4.FlagCommand.bit.EvacOn == 0)) {	
            save_s16 (ramPnt, progPnt->Pnt_NoSpdStop, NoSpdMode);  // Если скорость не задана, на выход выдаю по сути режим для данной защиты.
            // А в блоке пусков будем формировать соответствующий режим - предупреждение или аварию          
          }
        }
        //---------------------------------------------------------------------                
        //-- Расчёт останова выбегом
        e1 = sPnt->DiscreteFuncSetDin[FuncFreeRunStop];         
        e1 = (s16)((Input.v1>>e1)&1);
        defBitMicro(ramPnt, progPnt->Pnt_DF_StopVibeg, e1);        
        
	//--Расчет квитирования аварий от дискретного входа
        e1 = sPnt->DiscreteFuncSetDin[FuncDinFailReset];                 
	e1 = (s16)((Input.v1>>e1)&1);                         //определяем наличие сигнала на соответствующем данной функции дискретном входе
	defBitMicro(ramPnt, progPnt->Pnt_DinFailReset, e1);   //выдаем состояние сигнала квитирования

	//--Расчет изменения темпа от дискретного входа
        e1 = sPnt->DiscreteFuncSetDin[FuncAccelChangeLift];                         
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DinSelTime, e1);   
        
	//--Расчет внешнего торможения от дискретного входа
        e1 = sPnt->DiscreteFuncSetDin[FuncExternalStop];                                 
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DinExtStop, e1);   
        
	//--Расчет ОС от тормоза по каналу 1
        e1 = sPnt->DiscreteFuncSetDin[FuncBrakeFdb1];                                 
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DF_BrakeOs, e1);   
        
	// Запрет пуска 
        u1 = sPnt->DiscreteFuncSetDin[FuncPuskDenied];                                         
	f1 = (s16)((Input.v1>>u1)&1); // Активность функции	
	if (u1) {// Проверяем - есть ли назначение данной функции?
          defBitMicro(ramPnt, progPnt->Pnt_OutNew1, !f1); // Выдаем состояние запрета Пуска - если на входе 0
        } else {
          clrBitMicro(ramPnt, progPnt->Pnt_OutNew1); // Снимаем запрет пуска если функция не активирована
        }
        
	// Разрешение пуска 
        u1 = sPnt->DiscreteFuncSetDin[FuncPuskPermission];                                         
	f1 = (s16)((Input.v1>>u1)&1); // Активность функции	
	if (u1) {// Проверяем - есть ли назначение данной функции?
          defBitMicro(ramPnt, progPnt->Pnt_DF_EnPusk, f1); // Если сигнал разрешения есть, выдаем его.
        } else { 
          setBitMicro(ramPnt, progPnt->Pnt_DF_EnPusk); // Если функция разрешения не активна - то по умолчанию разрешаем пуски
        }      
                       
	// Аварийная остановка - Аварийный стоп - Функция инверсная , поэтому обработчик немного усложнен
        u1 = sPnt->DiscreteFuncSetDin[FuncEmergencyStop];                                                 
	f1 = (s16)((Input.v1>>u1)&1); // Активность функции
        if (u1) { // Проверяем - есть ли назначение данной функции?
          defBitMicro(ramPnt, progPnt->Pnt_StopKey, !f1); // Выдаем сигнал аварии - работа инверсная.
        } else {
          defBitMicro(ramPnt, progPnt->Pnt_StopKey, 0); // Нет аварии
        }
            
	//--Расчет сигнала Din Up
        e1 = sPnt->DiscreteFuncSetDin[FuncDinUpFreq];                                 
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DF_Fup, e1);         
        
	//--Расчет сигнала Din Down
        e1 = sPnt->DiscreteFuncSetDin[FuncDinDownFreq];                                 
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DF_Fdown, e1); 
                
	// Выбор канала задания
	e1 = sPnt->DiscreteFuncSetDin[FuncDinSwitchReference];//load_s16(ramPnt, progPnt->Pnt_Sel_Ref); 
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DF_SelChanRef, e1);         

	// Выбор канала управления
        e1 = sPnt->DiscreteFuncSetDin[FuncDinSwitchControl];//load_s16(ramPnt, progPnt->Pnt_Sel_Control);
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DF_SelControl, e1);         

	// Выбор типа UF
	e1 = sPnt->DiscreteFuncSetDin[FuncDinSwitchUF];//load_s16(ramPnt, progPnt->Pnt_Sel_UF); 
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DF_SelUF, e1);         
        
	//--Расчет сигнала FuncContactorFdb
        e1 = sPnt->DiscreteFuncSetDin[FuncContactorFdb];                                 
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_DF_ContOs, e1);         
        
	//--Расчет сигнала FuncEvacuation
        if (sPnt->EvacTimeoutCnt) { // Обновляем сигнал разрешения эвакуации пока таймер считает
          sPnt->EvacTimeoutCnt--;        
          e1 = sPnt->DiscreteFuncSetDin[FuncEvacuation];                                 
          e1 = (s16)((Input.v1>>e1)&1);                         
          defBitMicro(ramPnt, progPnt->Pnt_DF_Evak, e1);         
        }

	// Авария пользователя 1
	e1 = sPnt->DiscreteFuncSetDin[FuncUserFault1];
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_UserFault1, e1);         

	// Авария пользователя 2
	e1 = sPnt->DiscreteFuncSetDin[FuncUserFault2];
	e1 = (s16)((Input.v1>>e1)&1);                         
	defBitMicro(ramPnt, progPnt->Pnt_UserFault2, e1);                 
        
        
//-- Расчёт дискретных выходов
	selPnt = progPnt->Pnt_Sel_Relay;
	for(u1=0, u2=0; u1 < 12; u1++){
		e1 = load_s16(ramPnt, *selPnt++)&0x3F;
		e1 = (s16)((Input.v1>>e1)&1);
		u2 |=  (e1<<u1);
	}
	save_s16(ramPnt, progPnt->Pnt_DOut_K1_12, u2);

	//-- Расчёт разрешения двигателей
	selPnt = progPnt->Pnt_Sel_Motor;
	for(u1=0, u2=0; u1 < 4; u1++){
		e1 = load_s16(ramPnt, *selPnt++)&0x3F;
		e1 = (s16)((Input.v1>>e1)&1);
		u2 |=  (e1<<u1);
	}
	save_s16(ramPnt, progPnt->Pnt_MotEn1_4, u2);

//	//-- Расчёт остальных функций
//	selPnt = &progPnt->Pnt_Sel_Up;
//	outPnt = &progPnt->Pnt_DF_Fup;
//	for(u1=(u16)(&FDF.Pnt_Sel_ResWorkCnt - &FDF.Pnt_Sel_Up); u1 >= 0; u1--){
//		e1 = load_s16(ramPnt, *selPnt++)&0x3F;
//		e1 = (s16)((Input.v1>>e1)&1);
//		defBitMicro(ramPnt, *outPnt++, e1);
//	}       
        
//	// Получаем состояние функции ОС по механическому тормозу лифта
//	save_s16(ramPnt, progPnt->Pnt_DF_BrakeOs, 0); // Обнуляем ОС по тормозу
//	// Вычитываем код выбранной комбинации дискретных входов: 0-Откл, 1-Дин5, 2-Дин6, 3-Дин7, 4-Дин8, 5-Дин5+6, 6-Дин6+7, 7-Дин7+8
//	e1 = load_s16(ramPnt, progPnt->Pnt_Sel_BrakeOs) & 0x3F;
//	if (e1) {
//		switch (e1) {
//			case 1: e1 = 5; e2 = 0; break;
//			case 2: e1 = 6; e2 = 0; break;
//			case 3: e1 = 7; e2 = 0; break;
//			case 4: e1 = 8; e2 = 0; break;
//			case 5: e1 = 5; e2 = e1+1; break;
//			case 6: e1 = 6; e2 = e1+1; break;
//			case 7: e1 = 7; e2 = e1+1; break;
//		}
//
//		// Получаем состояние колодок
//		e1 = (s16)((Input.v1>>e1)&1);
//		e2 = (s16)((Input.v1>>e2)&1);
//		// И упаковываем их в одно слово
//		e1 = e1 + (e2<<4); // Первая колодка 0x0F, вторая 0xF0
//
//		save_s16(ramPnt, progPnt->Pnt_DF_BrakeOs, e1);
//	}


//-- Инвертируем функцию наличия воды в DF_NoWater
	e1 = testBitMicro(ramPnt, progPnt->Pnt_DF_NoWater);
	defBitMicro(ramPnt, progPnt->Pnt_DF_NoWater, !e1);			//инвертируем
//-- Формируем импульс пуска сушки
	//e1 = testBitMicro(ramPnt, progPnt->Pnt_PuskDryPulse);		//формируем импульс
	//e1 = (!sPnt->pulsVal.fl.PuskDryPulse && e1);				//Отлавливаем фронт PuskDryPulse
	//sPnt->pulsVal.fl.PuskDryPulse = e1;							//сохраняем значение текущего такта
	//defBitMicro(ramPnt, progPnt->Pnt_PuskDryPulse, e1);

	return &progPnt->Pnt_End;

};
//==============================================================================
void SetNewDiscreteFunction (u32 ramPnt, FuncDISKR_FUNC_V4_type *progPnt, StateFuncDISKR_FUNC_V4 *sPnt, u16 diskrNum, u16 diskrFunc)
{  
  switch (diskrFunc) {
  case PuskFwdLevel: // 1
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncForward] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPusk1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk1] = Log1Mode;
    sPnt->DiscreteFuncSetDin[FuncStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop1] = Log1Mode;
    sPnt->DiscreteFuncSetDin[FuncStop2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop2] = Log1Mode;    
    
    break;
    
  case PuskFwdRise: // 2
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncForward] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPusk1] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk1] = RiseMode;     
    break;
    
  case PuskFwdFall: // 3
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncForward] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPusk1] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk1] = FallMode;
    break;
    
  case PuskBkwdLevel: // 4 
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBackward] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPusk2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk2] = Log1Mode;
    sPnt->DiscreteFuncSetDin[FuncStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop1] = Log1Mode;
    sPnt->DiscreteFuncSetDin[FuncStop2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop2] = Log1Mode;    
    break;
    
  case PuskBkwdRise: // 5
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBackward] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPusk2] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk2] = RiseMode;    
    break;
    
  case PuskBkwdFall: // 6
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBackward] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPusk2] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk2] = FallMode;    
    break;
    
  case StopRise: // 7
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncStop1] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPolarStop1] = RiseMode;
    sPnt->DiscreteFuncSetDin[FuncStop2] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPolarStop2] = RiseMode;     
    break;
    
  case StopFall: // 8
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncStop1] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPolarStop1] = FallMode;
    sPnt->DiscreteFuncSetDin[FuncStop2] = diskrNum;
    sPnt->DiscreteFuncSetDin[FuncPolarStop2] = FallMode;     
    
    break;
    
  case ErrorReset: // 9                      
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncDinFailReset] = diskrNum;    
    break;
    
  case PuskPermis: // 10 - FuncPuskPermission
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncPuskPermission] = diskrNum;        
    break;
    
  case PuskDenied: // 11 - Здесь должна быть инверсия к 10 пункту - FuncPuskDenied
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncPuskDenied] = diskrNum;    
    break;
    
  case EmergencyStop: // 12 - FuncEmergencyStop
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncEmergencyStop] = diskrNum;    
    break;
    
  case FreeRunStop: // 13   - FuncFreeRunStop                    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreeRunStop] = diskrNum;     
    break;
    
  case DinUpFreq: // 14 - FuncDinUpFreq, 
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncDinUpFreq] = diskrNum;     
    break;
    
  case DinDownFreq: // 15 - FuncDinDownFreq
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncDinDownFreq] = diskrNum;         
    break;
    
  case FreqBit0: // 16  - FuncFreqBit0
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreqBit0] = diskrNum;         
    break;
    
  case FreqBit1: // 17  - FuncFreqBit1
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreqBit1] = diskrNum;         
    break;
    
  case FreqBit2: // 18 - FuncFreqBit2
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreqBit2] = diskrNum;     
    break;
    
  case FreqBit3: // 19 - FuncFreqBit3
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreqBit3] = diskrNum;         
    break; 
    
  case FixedFreq1: // 20 - FuncFixedFreq1
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFixedFreq1] = diskrNum;         
    break;
    
  case FixedFreq2: // 21 - FuncFixedFreq2
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFixedFreq2] = diskrNum;         
    break;
    
  case FixedFreq3: // 22 - FuncFixedFreq3

    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFixedFreq3] = diskrNum;     
    break;
    
  case FixedFreq4: // 23 - FuncFixedFreq4
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFixedFreq4] = diskrNum;         
    break;
    
  case AccelChangeLift: // 24 - FuncAccelChangeLift
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncAccelChangeLift] = diskrNum;         
    break;    

  case SelExternStop: // 25 - FuncExternalStop
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncExternalStop] = diskrNum;         
    break;    
       
  case ContactorFdb: // 26 - FuncContactorFdb
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncContactorFdb] = diskrNum;             
    break;      
    
  case BrakeFdb1: // 27 - FuncBrakeFdb1
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBrakeFdb1] = diskrNum;    
    break; 
                 
  case Evacuation: // 28 - FuncEvacuation
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncEvacuation] = diskrNum;    
    break;
    
  case BrakeFdb2: // 28 - FuncBrakeFdb2
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBrakeFdb2] = diskrNum;    
    break;    
        
  case DinSwitchUF: // 29
    sPnt->DiscreteFuncSetDin[FuncDinSwitchUF] = diskrNum;  
    break;
    
  case DinSwitchControl: // 30
    sPnt->DiscreteFuncSetDin[FuncDinSwitchControl] = diskrNum;  
    break;
    
  case DinSwitchReference: // 31
    sPnt->DiscreteFuncSetDin[FuncDinSwitchReference] = diskrNum;      
    break;

  case UserFault1: // 32
    sPnt->DiscreteFuncSetDin[FuncUserFault1] = diskrNum;      
    break;

  case UserFault2: // 33
    sPnt->DiscreteFuncSetDin[FuncUserFault2] = diskrNum;      
    break;
        
  case ReverseRise: // 34   
    break;
    
  case ReverseFall: // 35
    break;               
  }  
}

void ClearDiscreteFunction (u32 ramPnt, FuncDISKR_FUNC_V4_type *progPnt, StateFuncDISKR_FUNC_V4 *sPnt, u16 diskrNum, u16 diskrFunc)
{
  // Отключение ранее назначенной функции
  switch (diskrFunc) {
  case PuskFwdLevel: // 1
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncForward] = 0;
    sPnt->DiscreteFuncSetDin[FuncPusk1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk1] = 0;
    sPnt->DiscreteFuncSetDin[FuncStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncStop2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop2] = 0;    
    break;
    
  case PuskFwdRise: // 2
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncForward] = 0;
    sPnt->DiscreteFuncSetDin[FuncPusk1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk1] = 0;    
    break;
    
  case PuskFwdFall: // 3
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncForward] = 0;
    sPnt->DiscreteFuncSetDin[FuncPusk1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk1] = 0;    
    break;
    
  case PuskBkwdLevel: // 4 
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBackward] = 0;
    sPnt->DiscreteFuncSetDin[FuncPusk2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk2] = 0;
    sPnt->DiscreteFuncSetDin[FuncStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncStop2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop2] = 0;     
    break;
    
  case PuskBkwdRise: // 5
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBackward] = 0;
    sPnt->DiscreteFuncSetDin[FuncPusk2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk2] = 0;    
    break;
    
  case PuskBkwdFall: // 6
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBackward] = 0;
    sPnt->DiscreteFuncSetDin[FuncPusk2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarPusk2] = 0;     
    break;
    
  case StopRise: // 7 
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncStop2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop2] = 0;    
    break;
    
  case StopFall: // 8
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop1] = 0;
    sPnt->DiscreteFuncSetDin[FuncStop2] = 0;
    sPnt->DiscreteFuncSetDin[FuncPolarStop2] = 0;
    break;
    
  case ErrorReset: // 9                          
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncDinFailReset] = 0;    
    break;
    
  case PuskPermis: // 10   
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncPuskPermission] = 0;
    break;
    
  case PuskDenied:     
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncPuskDenied] = 0;     
    break;
    
  case EmergencyStop: // 12    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncEmergencyStop] = 0;    
    break;
    
  case FreeRunStop: // 13                           
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreeRunStop] = 0;    
    break;
    
  case DinUpFreq: // 14    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncDinUpFreq] = 0;    
    break;
    
  case DinDownFreq: // 15     
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncDinDownFreq] = 0;     
    break;
    
  case FreqBit0: // 16   
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreqBit0] = 0;     
    break;
    
  case FreqBit1: // 17 
//    save_s16 (ramPnt, progPnt->Pnt_Sel_Bit1, 0);    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreqBit1] = 0;    
    break;
    
  case FreqBit2: // 18 
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreqBit2] = 0;    
    break;
    
  case FreqBit3: // 19    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFreqBit3] = 0;    
    break; 
    
  case FixedFreq1: // 20    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFixedFreq1] = 0;    
    break;
    
  case FixedFreq2: // 21    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFixedFreq2] = 0;     
    break;
    
  case FixedFreq3: // 22     
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFixedFreq3] = 0;     
    break;
    
  case FixedFreq4: // 23 
//    save_s16 (ramPnt, progPnt->Pnt_Sel_V4, 0);    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncFixedFreq4] = 0;     
    break;
    
  case AccelChangeLift: // 24    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncAccelChangeLift] = 0;     
    break;    

  case SelExternStop: // 25   
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncExternalStop] = 0;     
    break;    
    
  case ContactorFdb: // 26   
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncContactorFdb] = 0;     
    break;      
    
  case BrakeFdb1: // 27   
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBrakeFdb1] = 0;     
    break; 
    
  case BrakeFdb2: // 28    
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncBrakeFdb2] = 0;     
    break;     
    
  case Evacuation:   
    // Замена на виртуальные уставки-функции
    sPnt->DiscreteFuncSetDin[FuncEvacuation] = 0;     
    break;
    
  case DinSwitchUF:
    sPnt->DiscreteFuncSetDin[FuncDinSwitchUF] = 0;  
    break;
    
  case DinSwitchControl:
    sPnt->DiscreteFuncSetDin[FuncDinSwitchControl] = 0;  
    break;
    
  case DinSwitchReference:
    sPnt->DiscreteFuncSetDin[FuncDinSwitchReference] = 0;      
    break;
            
  case UserFault1: // 32
    sPnt->DiscreteFuncSetDin[FuncUserFault1] = 0;      
    break;

  case UserFault2: // 33
    sPnt->DiscreteFuncSetDin[FuncUserFault2] = 0;      
    break;
    
  case ReverseRise: // 29    
    break;
    
  case ReverseFall: // 30
    break;               
  }    
}

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDISKR_FUNC_V4_1[106]= {
	S16_PIN_TYPE | INPUT_PIN_MODE,//1  AutoMode
	BIT_PIN_TYPE | INPUT_PIN_MODE,//2  EnRefDin
	S16_PIN_TYPE | INPUT_PIN_MODE,//3  FailTekDrv
	BIT_PIN_TYPE | INPUT_PIN_MODE,//4  EnControlDin
	S16_PIN_TYPE | INPUT_PIN_MODE,//5  Prior
	S16_PIN_TYPE | INPUT_PIN_MODE,//6  Prior_revers
	BIT_PIN_TYPE | INPUT_PIN_MODE,//7  Pnt_ReadyPusk
	S16_PIN_TYPE | INPUT_PIN_MODE,//8  SpdMiss_PuskDis
	S16_PIN_TYPE | INPUT_PIN_MODE,//9   sDin1Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//10  Din2Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//11  Din3Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//12  Din4Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//13  Din5Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//14  Din6Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//15  Din7Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//16  Din8Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//17  Din9Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//18  Din10Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//19  Din11Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//20  Din12Func
	S16_PIN_TYPE | INPUT_PIN_MODE,//21  Din13Func
        
	S16_PIN_TYPE | INPUT_PIN_MODE,//22  Sel_Stop2
	S16_PIN_TYPE | INPUT_PIN_MODE,//23  Sel_StopVibeg
	S16_PIN_TYPE | INPUT_PIN_MODE,//24  Sel_Forward
	S16_PIN_TYPE | INPUT_PIN_MODE,//25  Sel_Bacward
	S16_PIN_TYPE | INPUT_PIN_MODE,//26  Sel_V1
	S16_PIN_TYPE | INPUT_PIN_MODE,//27  Sel_V2
	S16_PIN_TYPE | INPUT_PIN_MODE,//28  Sel_V3
	S16_PIN_TYPE | INPUT_PIN_MODE,//29  Sel_V4
	S16_PIN_TYPE | INPUT_PIN_MODE,//30  Sel_Bit0
	S16_PIN_TYPE | INPUT_PIN_MODE,//31  Sel_Bit1
	S16_PIN_TYPE | INPUT_PIN_MODE,//32  Sel_Bit2
	S16_PIN_TYPE | INPUT_PIN_MODE,//33  Sel_Bit3
        
	S16_PIN_TYPE | INPUT_PIN_MODE,//34  Sel_Up
	S16_PIN_TYPE | INPUT_PIN_MODE,//35  Sel_Down
	S16_PIN_TYPE | INPUT_PIN_MODE,//36  Sel_Control
	S16_PIN_TYPE | INPUT_PIN_MODE,//37  Sel_Ref
	S16_PIN_TYPE | INPUT_PIN_MODE,//38  Sel_FastTorm
	S16_PIN_TYPE | INPUT_PIN_MODE,//39  Sel_Evaс
	S16_PIN_TYPE | INPUT_PIN_MODE,//40  Sel_ContOs
	S16_PIN_TYPE | INPUT_PIN_MODE,//41  Sel_InspectEn
	S16_PIN_TYPE | INPUT_PIN_MODE,//42  Sel_TorqSpeed
	S16_PIN_TYPE | INPUT_PIN_MODE,//43  Sel_UF
	S16_PIN_TYPE | INPUT_PIN_MODE,//44  Sel_DryEn
	S16_PIN_TYPE | INPUT_PIN_MODE,//45  Sel_Water
	S16_PIN_TYPE | INPUT_PIN_MODE,//46  Sel_Clean
	S16_PIN_TYPE | INPUT_PIN_MODE,//47  Sel_ResWorkCnt
        
	S16_PIN_TYPE | INPUT_PIN_MODE,//48  Sel_Relay1
	S16_PIN_TYPE | INPUT_PIN_MODE,//49  Sel_Relay2
	S16_PIN_TYPE | INPUT_PIN_MODE,//50  Sel_Relay3
	S16_PIN_TYPE | INPUT_PIN_MODE,//51  Sel_Relay4
	S16_PIN_TYPE | INPUT_PIN_MODE,//52  Sel_Relay5
	S16_PIN_TYPE | INPUT_PIN_MODE,//53  Sel_Relay6
	S16_PIN_TYPE | INPUT_PIN_MODE,//54  Sel_Relay7
	S16_PIN_TYPE | INPUT_PIN_MODE,//55  Sel_Relay8
	S16_PIN_TYPE | INPUT_PIN_MODE,//56  Sel_Relay9
	S16_PIN_TYPE | INPUT_PIN_MODE,//57  Sel_Relay10
	S16_PIN_TYPE | INPUT_PIN_MODE,//58  Sel_Relay11
	S16_PIN_TYPE | INPUT_PIN_MODE,//59  Sel_Relay12
        
	S16_PIN_TYPE | INPUT_PIN_MODE,//60  Sel_Motor1
	S16_PIN_TYPE | INPUT_PIN_MODE,//61  Sel_Motor2
	S16_PIN_TYPE | INPUT_PIN_MODE,//62  Sel_Motor3
	S16_PIN_TYPE | INPUT_PIN_MODE,//63  Sel_Motor4
	S16_PIN_TYPE | INPUT_PIN_MODE,//64  InputNew1
	S16_PIN_TYPE | INPUT_PIN_MODE,//65  InputNew2
	S16_PIN_TYPE | INPUT_PIN_MODE,//66  InputNew3
	S16_PIN_TYPE | INPUT_PIN_MODE,//67  InputNew4
	S16_PIN_TYPE | INPUT_PIN_MODE,//68  InputNew5
	S16_PIN_TYPE | INPUT_PIN_MODE,//69  DinFailResetType
	S32_PIN_TYPE | INPUT_PIN_MODE,//70  Input_1
	S32_PIN_TYPE | INPUT_PIN_MODE,//71  Input_2

	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//72  AutoModeBit
	S16_PIN_TYPE | OUTPUT_PIN_MODE,//73  NoSpdStop
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//74  NosSpdWarn
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//75  NOSpdFault

	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//76  WarnDFExist
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//77  WarnDinBusy               
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,//78  DF_EnPusk
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,//79  DF_Pusk
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//80  DF_Stop
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//81  DF_StopVibeg
	S16_PIN_TYPE | OUTPUT_PIN_MODE,//82  DF_Revers
	S16_PIN_TYPE | OUTPUT_PIN_MODE,//83  DF_NumSel
        
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//84  DF_Fup
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//85  DF_Fdown
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//86  DF_SelControl
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//87  DF_SelChanRef
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//88  DF_FastTorm
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//89  DF_Evak
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//90  DF_ContOs
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//91  DF_InspectEn
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//92  DF_TorqSpeed
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//93  DF_SelUF
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//94  PuskDryPulse
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//95  DF_NoWater
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//96  DF_StartClean
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//97  DF_ResCntWork
        
	S16_PIN_TYPE | OUTPUT_PIN_MODE,//98  DOut_K1_12
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//99  DF_SEL_TOME
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//100  DF_EXT_STOP
	S16_PIN_TYPE | OUTPUT_PIN_MODE,//101  MotEn1_4
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//102  DinFailReset

	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//103  OutNew1
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//104  OutNew2
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//105  Brake OS
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,//106  StopKey
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DISKR_FUNC_V4", "DD", TblFuncDISKR_FUNC_V4_1, FuncDISKR_FUNC_V4_1, sizeof(TblFuncDISKR_FUNC_V4_1), sizeof(StateFuncDISKR_FUNC_V4), sizeof(nvFuncDISKR_FUNC_V4), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
(pFuncMicro_type)FuncDISKR_FUNC_V4_1,
#endif
//********************************************************
