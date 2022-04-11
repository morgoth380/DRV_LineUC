#ifdef    GET_FUNC_CODE
//#include "Common_defines.h"
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
    u16  Pnt_PuskCmm;                      //60  PuskCmm
    u16  Pnt_ChargeInProc;                 //61  ChargeInProc
    u16  Pnt_MTZ_LEAKAGE;                  //62  MTZ_LEAKAGE
    u16  Pnt_FAIL_ZIP;                     //63  FAIL_ZIP
    u16  Pnt_FailMask0;                    //64  FailMask0
    u16  Pnt_FailMask1;                    //65  FailMask1
    u16  Pnt_FailMask3;                    //66  FailMask3
    u16  Pnt_FailMask4;                    //67  FailMask4
    u16  Pnt_PowerErr;                     //68  PowerErr
    u16  Pnt_FailTekDrv;                   //69  FailTekDrv
    u16  Pnt_FailFixDrv;                   //70  FailFixDrv
    u16  Pnt_FailTekDrvBit;                //71  FailTekDrvBit
    u16  Pnt_FailTekDrvBitNot;             //72  FailTekDrvBitNot
    u16  Pnt_OFF_ZAR;                      //73  OFF_ZAR
    u16  Pnt_End;
}FuncFAIL_COMMON_V2_type;

//********************************************************
typedef union{
		struct{
		   u8  FailState         : 1;    // Флаг состояния аварии
		   u8  HandReset         : 1;    // Флаг ручного сброса аварии
		   u8  prevFailState	 : 1;    // Предыдущее состояние аварии с прошлого такта работы
                   u8  failZarrOff       : 1;    // Метка отключения заряда для выбранной аварии.
                   u8  DelayFaultView    : 2;    // Задержка отображения Аварии
		 } bit;
		u8 all;
	} FailType_struct;

typedef enum {
    FAULT_VIEW_1_SEC = 0,
    FAULT_VIEW_5_SEC = 1,
    FAULT_VIEW_15_SEC = 2,
    FAULT_VIEW_60_SEC = 3,        
} DelayFaultView_Enum;

typedef enum {
  PWM_UNKNOWN  = 0,
  PWM_ON_TRESH = 1,
  PWM_OFF_TRESH =2,
  PWM_ON_STATE = 3,
  PWM_OFF_STATE =4,
  
} PwmTrig_type;
//********************************************************
#define		MAX_FAIL_NUMBER		96
#define		FAIL_BUF_SIZE		MAX_FAIL_NUMBER/32 + 1
//********************************************************

#define     SLOWFAILWOACK ((u64)1<<SlowFail_CurrImbalance)
#define		POWERERRMASK ((u64)1<<FastFailMask_UdMin | (u64)1<<FastFailMask_UdMax | (u64)1<<SlowFail_supplyPhLoss | (u64)1<<SlowFailPhase3ERR)
#define		OFFZARMASK    0xFFFFFFFFFFFFFFFFULL

typedef struct{
	u16			zarOffTimer;
	u16			resetFail3; //предыдущее состояние квитирования аварий от дискретного входа
	u32			prevFailState; //предыдущие состояния битов-флагов аварий
	u16			faultCounter;
	FailType_struct		FailArray[MAX_FAIL_NUMBER]; // Создаем массив на MAX_FAIL_NUMBER аварий
        u16                     prevNumFailDRVLocal;
        u16                     prevPwmOn;
        PwmTrig_type            PwmTrigger;        
}FAILCOMMON_State_type;

//typedef enum {
//	ALL_FAIL_ACK = 0, // Квитировать все аварии
//	LIST_FAIL_ACK = 1, // Квитировать аварии по списку
//	ALL_EXCEPT_LIST_ACK = 2, // Квитировать все аварии, кроме списка
//}ACK_FAIL_MODE;

typedef enum {
	ALL_FAIL_ACK     = 0, // Квитировать все аварии
        ALL_FAIL_AUTO_ACK = 1, // Автосброс всех аварий
	ALL_EXCEPT_LIST_ACK = 2, // Квитировать все аварии, кроме списка, значит автосброс для аварий из списка
	LIST_FAIL_ACK    = 3, // Квитировать аварии по списку, автосброс аварий вне списка        
}ACK_FAIL_MODE;


typedef struct{
    u32    prevFailState;
}failnvState_type;

#define     nV    (*(failnvState_type*)0)
//****************************************************************************************
u16 *FuncFAIL_COMMON_V2_1(FuncFAIL_COMMON_V2_type  *progPnt, u32 ramPnt)
{
//#ifdef _PROJECT_FOR_STM32_
    u16	                i, handReset, ManResMode, NumFailDRVLocal = 0;
    u16	                AckFailMode, FailViewTime;
    DelayFaultView_Enum FailView;
    u16                 ZarrOffStatus; //ChargeStatus;
    u16	                FailWord1, FailWord2;
    u32	                FailDWord[FAIL_BUF_SIZE];
    FailData_type       *pFailData;
    u16                 resetFail1, resetFail2, resetFail3, index;
    FAILCOMMON_State_type   *sPnt;
    GlobalVar           *mPnt;

    pFailData           = &GlobalM4.FailData;
    mPnt                = &GlobalM4;
    sPnt                = (FAILCOMMON_State_type *)(ramPnt + progPnt->Pnt_State);
    ZarrOffStatus       = 0;
//      ChargeStatus = testBitMicro (ramPnt, progPnt->Pnt_ChargeInProc);
//	AllFailsType      handResetMask; //маска битов-флагов, задающих режим ручного квитирования для соответствующих аварий
//	AllFailsType      curFailState;   //маска битов-флагов состояния текущих аварий

    
    if (flagIsFirstCall) {
        save_s16(ramPnt, progPnt->Pnt_HandFailReset2, 0);                              //возврат в состояние "Нет квитирования".
        sPnt->faultCounter = 0;
        for (i = 0; i < MAX_FAIL_NUMBER; i++){
            sPnt->FailArray[i].all = 0;
        }
        // Заполнить аварии которые должны отключать заряд.
        sPnt->FailArray[FastFailMask_UdMax].bit.failZarrOff = 1;
        sPnt->FailArray[FastFailMask_UdMin].bit.failZarrOff = 1;
        sPnt->FailArray[SlowFailMaxUin].bit.failZarrOff = 1;
        sPnt->FailArray[FastFailMask_UinMax].bit.failZarrOff = 1;
        sPnt->FailArray[FastFailMask_UinMin].bit.failZarrOff = 1;        
        sPnt->FailArray[SlowFail_failButton].bit.failZarrOff = 1;                
        sPnt->FailArray[FastFailMask_SwU].bit.failZarrOff = 1;                
        sPnt->FailArray[FastFailMask_SwV].bit.failZarrOff = 1;                
        sPnt->FailArray[FastFailMask_SwW].bit.failZarrOff = 1;                
        sPnt->FailArray[FastFailMask_SwT].bit.failZarrOff = 1;        
        sPnt->FailArray[FastFailMask_MtzW_Comp].bit.failZarrOff = 1;                        
        sPnt->FailArray[FastFailMask_MtzZPT].bit.failZarrOff = 1;                  
                                             
        // Задаем время светимости для ряда аварий
        sPnt->FailArray[FastFailMask_ExtStop].bit.DelayFaultView = FAULT_VIEW_5_SEC;
        sPnt->FailArray[FastFailMask_UdMax].bit.DelayFaultView = FAULT_VIEW_5_SEC;
        sPnt->FailArray[FastFailMask_ErrMeasPrm].bit.DelayFaultView = FAULT_VIEW_15_SEC;
        sPnt->FailArray[FastFailMask_MeasPrmBrkPhase].bit.DelayFaultView = FAULT_VIEW_15_SEC;            
        sPnt->FailArray[SlowFailMask_Contactor].bit.DelayFaultView = FAULT_VIEW_5_SEC; 
        sPnt->FailArray[FastFailMask_ruU].bit.DelayFaultView = FAULT_VIEW_5_SEC; 
        
        sPnt->FailArray[SlowFailLoadLoss].bit.DelayFaultView = FAULT_VIEW_5_SEC;         
        sPnt->FailArray[SlowFail_ReleaseBrkWoPwm].bit.DelayFaultView = FAULT_VIEW_5_SEC; 
        sPnt->FailArray[SlowFail_StartNoSpeedFault].bit.DelayFaultView = FAULT_VIEW_5_SEC; 
        sPnt->FailArray[SlowFail_MeasParamInt].bit.DelayFaultView = FAULT_VIEW_5_SEC;         
        sPnt->FailArray[SlowFail_UserFault1].bit.DelayFaultView = FAULT_VIEW_5_SEC;         
        sPnt->FailArray[SlowFail_UserFault2].bit.DelayFaultView = FAULT_VIEW_5_SEC;         
                                
//        sPnt->FailArray[FastFailMask_SwU].bit.DelayFaultView = FAULT_VIEW_15_SEC;                
//        sPnt->FailArray[FastFailMask_SwV].bit.DelayFaultView = FAULT_VIEW_15_SEC;                
//        sPnt->FailArray[FastFailMask_SwW].bit.DelayFaultView = FAULT_VIEW_15_SEC;
//        sPnt->FailArray[FastFailMask_SwT].bit.DelayFaultView = FAULT_VIEW_15_SEC;
        
        return &progPnt->Pnt_End;
    }

    // Формируем триггер работы ШИМа
    if (GlobalM4.FlagCommand.bit.PwmOn && !sPnt->prevPwmOn) {
        sPnt->PwmTrigger = PWM_ON_TRESH;        
    } else if (!GlobalM4.FlagCommand.bit.PwmOn && sPnt->prevPwmOn) {
        sPnt->PwmTrigger = PWM_OFF_TRESH;        
    } else if (GlobalM4.FlagCommand.bit.PwmOn && sPnt->prevPwmOn) {
        sPnt->PwmTrigger = PWM_ON_STATE;        
    } else if (!GlobalM4.FlagCommand.bit.PwmOn && !sPnt->prevPwmOn) {
        sPnt->PwmTrigger = PWM_OFF_STATE;        
    }

    if (sPnt->faultCounter)		
        sPnt->faultCounter--;
    
    //-----------------------------------------------------------
    // !!!!!!  Заполняем массив данными о текущих авариях  !!!!!!
    memset(FailDWord, 0, sizeof(FailDWord)); // Чистим массив будущих данных для битов аварий
                                                                // Чистим текущие аварии в sPnt
    for (i = 0; i < MAX_FAIL_NUMBER; i++) {
        sPnt->FailArray[i].bit.FailState = 0;
    }
  
    //-- Считываем блоками входы аварий	(не быстрых)
    // Запоминаем быстрые аварии
    FailDWord[0] = pFailData->FastFailMask0;
    pFailData->FastFailMask0 = 0;                               // Маска быстрых аварий отработана можно обнулить

    
    FailWord1 = load_s16(ramPnt, progPnt->Pnt_F_StopKey );      // Первые 16 аварии 
    FailWord2 = load_s16(ramPnt, progPnt->Pnt_F_Power );        // Вторые 16 аварии
    FailDWord[1] = FailWord1;                                   // Заносим 32 аварии
    FailDWord[1] |= (u32)FailWord2 << 16;
                                                                // Считываем следующие 32 аварии и заносим в массив
    FailWord1 = load_s16(ramPnt, progPnt->Pnt_F_Contactor );    // Третьи 16 аварии 
    FailWord2 = load_s16(ramPnt, progPnt->Pnt_F_TormRes );      // Четвертые 16 аварии 
    FailDWord[2] = FailWord1;                                   // Заносим 32 аварии М3 ядра в следующую точку    
    FailDWord[2] |= (u32)FailWord2 << 16;                       // Заносим 32 аварии М3 ядра в следующую точку
    
    // И так далее ...
    // Парсим массив аварий и заносим аварии в свои слоты
    for (i = 0; i < MAX_FAIL_NUMBER; i++) {
          if (FailDWord[i/32] & (1<<(i%32))) { // Выделяем маской аварии из массива - находим номер массива и номер бита в нем.
                  sPnt->FailArray[i].bit.FailState = 1;
          }
    }

//    // DEBUG !!!! - Две аварии
//    if (GlobalM4.Debug9) {
//      sPnt->FailArray[(u16)GlobalM4.Debug10].bit.FailState = 1;
//      sPnt->FailArray[(u16)GlobalM4.Debug9].bit.FailState = 1;
//    }

//    // DEBUG !!!! - Быстрая авария
//    if (GlobalM4.Debug10 != GlobalM4.Debug8) {
//      sPnt->FailArray[(u16)GlobalM4.Debug10].bit.FailState = 1;
//    }
//    GlobalM4.Debug8 = GlobalM4.Debug10;

    
    //-----------------------------------------------------------
    AckFailMode   = load_s16(ramPnt, progPnt->Pnt_AckFailOff);        //считываем состояние автоквитирования
    ManResMode  = !testBitMicro(ramPnt, progPnt->Pnt_ManResEn);     //считываем текущий активный режим Ручной/Авто
    if (AckFailMode == ALL_FAIL_AUTO_ACK) { // Проверяем режима квитирования на автосброс
      handReset = 0; //Автосброс
    } else { 
      handReset = 1; // СТавим флаг ручного сброса аварий 
    }
    
    //AckFailMode = load_s16(ramPnt, progPnt->Pnt_NotUsed );
    switch (AckFailMode) {                                                                          // Выбираем алгоритм квитирования аварий
        case ALL_FAIL_ACK:
              // Все аварии квитируются
              for (i = 0; i < MAX_FAIL_NUMBER; i++){
                  sPnt->FailArray[i].bit.HandReset = 1;
              }
              break;
              
        case ALL_FAIL_AUTO_ACK:
              // Все аварии автоквитируются
              for (i = 0; i < MAX_FAIL_NUMBER; i++){
                  sPnt->FailArray[i].bit.HandReset = 0;
              }
              break;
              
        case LIST_FAIL_ACK: // Квитировать аварии по списку, автосброс аварий вне списка
              for (i = 0; i < MAX_FAIL_NUMBER; i++){
                  sPnt->FailArray[i].bit.HandReset = 0;
              }// Сбросить все флаги квитирования
              for (i = 0; i < 20; i++) {                                                      // 20 - максимально возможное количество аварий, квитируемых вручную
                    index = load_s16(ramPnt, progPnt->Pnt_Fail[i]);                         // Индекс квитируемой аварии
                    if (index) {                                                            // определяем назначена ли определенная авария для текущего входа квитируемой аварии                                                         // Заполняем массив аварий данными ручного квитирования
                      sPnt->FailArray[index].bit.HandReset = 1;
                    }
              }
              break;

        case ALL_EXCEPT_LIST_ACK: // Квитировать все аварии, кроме списка, значит автосброс для аварий из списка
              for (i = 0; i < MAX_FAIL_NUMBER; i++){ 
                  sPnt->FailArray[i].bit.HandReset = 1;
              }
              // Установить все флаги квитирования
              for (i = 0; i < 20; i++) {                                                      // 20 - максимально возможное количество аварий, которые можно исключить из списка квитирования
                  index = load_s16(ramPnt, progPnt->Pnt_Fail[i]);                         // Индекс квитируемой аварии
                  if (index){                                                            // определяем назначена ли определенная авария для текущего входа квитируемой аварии                                                           // Удаляем флаг ручного квитирования для аварий из списка
                      sPnt->FailArray[index].bit.HandReset = 0;
                  }
              }
              break;

        default:
              // Все аварии квитируются
              for (i = 0; i < MAX_FAIL_NUMBER; i++){ 
                  sPnt->FailArray[i].bit.HandReset = 1; 
              }
              break;
    }
    //-- Если включен режим ручного сброса то будем помнить аварии до поступления сигнала сброса
    
    // Новый метод - Восстановление авариий с прошлого такта
    if (handReset && ManResMode) {              // если активирован режим ручного квитирования аварий и режим работы привода не автоматический
                                                // Восстанавливаем аварии в текущие, для тех у кого активно квитирование
        for (i = 0; i < MAX_FAIL_NUMBER; i++) {
                if (sPnt->FailArray[i].bit.HandReset == 1) {
                        sPnt->FailArray[i].bit.FailState |= sPnt->FailArray[i].bit.prevFailState;
                }
        }
    } 
    else { //иначе ручное квитирование отключено и передаем в программу аварии по факту их наличия
          // Сброс предшествующих аварий  
        for (i = 0; i < MAX_FAIL_NUMBER; i++){ 
            sPnt->FailArray[i].bit.prevFailState = 0; 
        }
    }

    // Проверяем - есть ли активная авария
    for(i = 1; i < MAX_FAIL_NUMBER; i++) {              // перебираем все аварии
        if( sPnt->FailArray[i].bit.FailState ) {        // Проверяем состояние аварии
            NumFailDRVLocal = i;		        // Фиксируем первую аварию и выходим
            break;
        }
    }

    // Делаем копию аварии в глобальную структуру, для применения в журнале аварий.
    mPnt->NumSlowFail = NumFailDRVLocal; //передаем номер первой попавшейся медленной аварии в глобальный объект. Сохраняем номер медленной аварии
    if(mPnt->NumSlowFail){
        
    }
    // Если разрешено ручное квитирование, то сохраним состояние аварий для предыдущего такта
    if (handReset && ManResMode) {
          for(i = 0; i < MAX_FAIL_NUMBER; i++) {                        // перебираем все аварии
                if( sPnt->FailArray[i].bit.FailState ) {                // Проверяем состояние аварии
                        sPnt->FailArray[i].bit.prevFailState = 1;       // Если есть, запоминаем ее.
                        break;
                }
          }
    }
    //-Ручное квитирование аварий
    GlobalM4.FlagCommand.bit.Kvitirov_R = 0;                                                //Сбросить старый запрос на квитирование
    if (handReset && ManResMode) {                                                          //если разрешено ручное квитирование аварий
        resetFail1 = testBitMicro(ramPnt, progPnt->Pnt_HandFailReset1);                     //сброс 1
        resetFail2 = load_s16(ramPnt, progPnt->Pnt_HandFailReset2);                         //сброс 2
        if((!sPnt->resetFail3) && testBitMicro(ramPnt, progPnt->Pnt_HandFailReset3)) {      //если не было квитирования от дискретного входа, но теперь есть
            resetFail3 = 1;                                                                 //сброс 3
            sPnt->resetFail3 = 1;                                                           //запоминаем то, что сброс от дискретного входа осуществлено квитирование аварий
        } else if (!testBitMicro(ramPnt, progPnt->Pnt_HandFailReset3)) {                    //если сигнал сброса от дискретного входа исчез
            sPnt->resetFail3 = 0;                                                           //разрешим повторное квитирование от дискретного входа
            resetFail3 = 0;
        }

        if (resetFail1 || resetFail2 || resetFail3) {                                       // если поступил сигнал ручного квитирования                                                                         // Сканируем аварии для которых разрешено квитирование и сбрасываем их
            for(i = 0; i < MAX_FAIL_NUMBER; i++) {                                          // перебираем все аварии
                if( sPnt->FailArray[i].bit.HandReset ) {                                    // Проверяем запрос на квитирование аварии
                      sPnt->FailArray[i].bit.FailState = 0;                                 // Сброс аварий
                      sPnt->FailArray[i].bit.prevFailState = 0;
                }
            }
            sPnt->faultCounter = 0;                                                         // Сбросить счетчик - чтобы обновилась авария
            clrBitMicro(ramPnt, progPnt->Pnt_HandFailReset1);                               //возврат в состояние "Нет квитирования"
            save_s16(ramPnt, progPnt->Pnt_HandFailReset2, 0);                               //возврат в состояние "Нет квитирования"
            GlobalM4.FlagCommand.bit.Kvitirov_R = 1;                                        // Разрешаем квитирование по команде
        } else {                                                                            // А если есть аварии с автоквитированием, то проверяем, ушла ли авария.
            resetFail1 = resetFail2 = resetFail3 = 0;                                       // Временные переменные для анализа
            for(i = 0; i < MAX_FAIL_NUMBER; i++) {                                  // перебираем все аварии
                if(sPnt->FailArray[i].bit.prevFailState && !sPnt->FailArray[i].bit.FailState) { // Если текущей аварии нету, но есть на предыдущем такте
                      sPnt->FailArray[i].bit.FailState = 0; // Сброс аварий
                      sPnt->FailArray[i].bit.prevFailState = 0;
                      resetFail1 = 1; // Если авария с автоквитированием - разрешаем дать команду квитирования
                } 
                else if (sPnt->FailArray[i].bit.FailState){ // Проверка - а есть ли текущие аварии
                        resetFail2 = 1; // Есть активная авария, ставим команду запрета аквтоквитирования
                }
            }

                    if (resetFail1 && !resetFail2){ // Если была сброшена авария с автоквитированием, и нет текущей аварии
                            GlobalM4.FlagCommand.bit.Kvitirov_R = 1; // Даем команду на квитирование
                    }
        }
    } else {
            if (!NumFailDRVLocal && sPnt->faultCounter) {
                    GlobalM4.FlagCommand.bit.Kvitirov_R = 1; // Разрешаем квитирование по команде
            }
    }      
//-- Сохранение масок
	
	save_s16(ramPnt, progPnt->Pnt_FailMask0, FailDWord[0]); //быстрые аварии
	save_s16(ramPnt, progPnt->Pnt_FailMask1, FailDWord[0] >> 16); //быстрые аварии
	save_s16(ramPnt, progPnt->Pnt_FailMask3, FailDWord[1]); //медленные аварии
	save_s16(ramPnt, progPnt->Pnt_FailMask4, FailDWord[1] >> 16); //медленные аварии
        
        // Если пришла команда пуск - то мы сбрасываем причину останова.
        if (testBitMicro(ramPnt, progPnt->Pnt_PuskCmm)) {
          save_s16(ramPnt, progPnt->Pnt_FailFixDrv, 0);
          pFailData->NumFailStop = 0;
        }
        
        // шим Включен и есть авариия - записываем в аварию Фикс
        // для медленных аварий, если обнаружили аварию и ПЧ еще в работе( не был вызван FailControl)
        if((/*sPnt->PwmTrigger == PWM_OFF_TRESH*/GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) && (NumFailDRVLocal)){
            pFailData->NumFailStop = NumFailDRVLocal;
        }
            
        
//        if (sPnt->prevNumFailDRVLocal != NumFailDRVLocal){
//          NumFailDRVLocal ++;
//          NumFailDRVLocal --;
//        }
        
	// Фиксация аварии на экране на 3 секунды
	if (!sPnt->faultCounter ||(sPnt->prevNumFailDRVLocal != NumFailDRVLocal && NumFailDRVLocal && sPnt->prevNumFailDRVLocal) ) { // Нормально работает при двух авариях в системе
          save_s16(ramPnt, progPnt->Pnt_FailFixDrv      , pFailData->NumFailStop);
          save_s16(ramPnt, progPnt->Pnt_FailTekDrv      , NumFailDRVLocal  ); // передаем в микропрограмму номер быстрой (если она была) или медленной аварии (если не было быстрой)
          defBitMicro(ramPnt, progPnt->Pnt_FailTekDrvBit   , NumFailDRVLocal);
          defBitMicro(ramPnt, progPnt->Pnt_FailTekDrvBitNot,!NumFailDRVLocal);
          sPnt->prevNumFailDRVLocal = NumFailDRVLocal; // Запоминаем текущую аварию, если счетчик в нуле          
	}        

	// Если есть авария - то удлиняем время отображения аварии, если квитирование запрещено
	// Запуск счетчика обязательно идет после защелкивания аварий
	if (NumFailDRVLocal) {
                FailView = (DelayFaultView_Enum)(sPnt->FailArray[NumFailDRVLocal].bit.DelayFaultView);
                switch (FailView) {
                    case FAULT_VIEW_5_SEC:
                        FailViewTime = 5;
                    break;
                    case FAULT_VIEW_15_SEC:
                        FailViewTime = 15;
                    break;
                    case FAULT_VIEW_60_SEC:
                        FailViewTime = 60;
                    break;
                    default:
                    case FAULT_VIEW_1_SEC:
                        FailViewTime = 1;
                    break;
                } 
                
		if(handReset == 0){
			sPnt->faultCounter = FailViewTime*K_TIME_CALL_MICRO_TAKT;
		}else{
			sPnt->faultCounter = FailViewTime*K_TIME_CALL_MICRO_TAKT;
		}
	}

//	// Выдать на схеме бит выхода - если в системе есть авария тока Утечки
//	if (sPnt->FailArray[FastFailMask_MTZ_Leakage].bit.FailState){
//		setBitMicro(ramPnt, progPnt->Pnt_MTZ_LEAKAGE);
//	} else {
//		 clrBitMicro(ramPnt, progPnt->Pnt_MTZ_LEAKAGE);
//	}

	// Выдать на схеме бит выхода - если в системе есть авария Сверхток
	if (sPnt->FailArray[FastFailMask_MtzW_Comp].bit.FailState){
		setBitMicro(ramPnt, progPnt->Pnt_MTZ_LEAKAGE);
	} else {
		 clrBitMicro(ramPnt, progPnt->Pnt_MTZ_LEAKAGE);
	}
                
	//===========================================================
	// для АПВ по аварии
	if(sPnt->FailArray[FastFailMask_UdMin].bit.FailState ||
		 sPnt->FailArray[SlowFail_supplyPhLoss].bit.FailState || sPnt->FailArray[SlowFailPhase3ERR].bit.FailState) {

		setBitMicro(ramPnt, progPnt->Pnt_PowerErr); //Pnt_NotUsed
	} else {
		clrBitMicro(ramPnt, progPnt->Pnt_PowerErr);
	}
	//===========================================================
        // Проверяем, есть ли авария, которая должна отключить заряд.
        for(i = 0; i < MAX_FAIL_NUMBER; i++) { 
          if (sPnt->FailArray[i].bit.FailState && sPnt->FailArray[i].bit.failZarrOff /*&& ChargeStatus*/) {
              ZarrOffStatus = 1;
              break;
          }
        }
        
	if(testBitMicro(ramPnt, progPnt->Pnt_OFF_ZAR)){ // Если отключен заряд
              if(!ZarrOffStatus /*!NumFailDRVLocal*/) { // И нету аварий которые его должны отключать, то разрешаем заряд
                  clrBitMicro(ramPnt, progPnt->Pnt_OFF_ZAR); // Разрешаем заряд
              }
	} 
        else{
          //Добавить маски для аварий, который должны отключать звено.
            //Сделать Цикл проверки состояний аварий и проверять на поле - ZAROFF (добавить его)                    
              if(/*(NumFailDRVLocal) &&*/ ZarrOffStatus ) {
                setBitMicro(ramPnt, progPnt->Pnt_OFF_ZAR); // Блокировка заряда для любых аварий кроме заряда
              }          
        }
        
//        if (!sPnt->faultCounter) {        
//            sPnt->prevNumFailDRVLocal = NumFailDRVLocal; // Запоминаем текущую аварию, если счетчик в нуле
//        }
                
        clrBitMicro(ramPnt, progPnt->Pnt_HandFailReset1);    //возврат в состояние "Нет квитирования"
        save_s16(ramPnt, progPnt->Pnt_HandFailReset2, 0);    //возврат в состояние "Нет квитирования"
        
        sPnt->prevPwmOn = GlobalM4.FlagCommand.bit.PwmOn; // Делаем копию состояния ШИМа
//#endif

	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFAIL_COMMON_V2_1[73]={
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
        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  F_StopKey
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  F_Power
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  F_Contactor
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  F_TormRes
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  F_Fasirovka
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //32  F_Tzar
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  F_Overload
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  F_Ain1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  F_Ain2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  F_Ain3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  F_Ain4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38  F_Ain5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //39  F_Fmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //40  F_Fmin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41  F_PhaseU
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  F_PhaseV
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43  F_PhaseW
        S16_PIN_TYPE | INPUT_PIN_MODE,     //44  F_TmaxDrv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //45  F_Modbus
        S16_PIN_TYPE | INPUT_PIN_MODE,     //46  F_ReversZapr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //47  F_TmaxHeater
        S16_PIN_TYPE | INPUT_PIN_MODE,     //48  F_NoWater
        S16_PIN_TYPE | INPUT_PIN_MODE,     //49  F_Avtovent
        S16_PIN_TYPE | INPUT_PIN_MODE,     //50  F_ZaprFreq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //51  F_Nedogruz
        S16_PIN_TYPE | INPUT_PIN_MODE,     //52  F_KrayPoint
        S16_PIN_TYPE | INPUT_PIN_MODE,     //53  F_ObrRemnya
        S16_PIN_TYPE | INPUT_PIN_MODE,     //54  F_Potok
        S16_PIN_TYPE | INPUT_PIN_MODE,     //55  F_SuhoyHod
        S16_PIN_TYPE | INPUT_PIN_MODE,     //56  F_ShortCycle
        S16_PIN_TYPE | INPUT_PIN_MODE,     //57  F_DisbCur
        S16_PIN_TYPE | INPUT_PIN_MODE,     //58  F_Res
        S16_PIN_TYPE | INPUT_PIN_MODE,     //59  FailStopOn
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //60  FairModOn
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //61  ChargeInProc        
        
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //62  MTZ_LEAKAGE
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //63  FAIL_ZIP 
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //64  FailMask0
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //65  FailMask1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //66  FailMask3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //67  FailMask4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //68  PowerErr
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //69  FailTekDrv
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //70  FailFixDrv
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //71  FailTekDrvBit
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //72  FailTekDrvBitNot
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //73  OFF_ZAR
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
