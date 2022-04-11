#ifdef    GET_FUNC_CODE
#include    "apv_type.h"
//====================================================================
typedef enum{
  Ain1_block = 2,
  Ain2_block,
}numOFBlock_enum;
typedef enum{
  Off_action = 0,
  Block_actoin,
  APV_action,
  FixSpeed_action
}ainFailAction_enum;

#define     nvAPV    (*(LOCAL_APVnvState_type*)0)
//====================================================================
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Fail_IN;                      //1  Fail_IN
    u16  Pnt_Off_Block_Apv;                //2  Off_Block_Apv
    u16  Pnt_DelayControl;                 //3  DeleyControl
    u16  Pnt_DelayOff;                     //4  DeleyOff
    u16  Pnt_NormalWait;                   //5  NormalWait
    u16  Pnt_ApvTime_sec;                  //6  ApvTime_sec
    u16  Pnt_ApvTime_min;                  //7  ApvTime_min
    u16  Pnt_KolvApv;                      //8  KolvApv
    u16  Pnt_ResetApv;                     //9  ResetApv
    u16  Pnt_On_Off;                       //10  On/Off
    u16  Pnt_RTC;                          //11  RTC
    u16  Pnt_VybegTime;                    //12  VybegTime
    u16  Pnt_NumOfBlock;                   //13  NumOfBlock
    u16  Pnt_PWM_ON;                       //14  PWM_ON
    u16  Pnt_AutoModeSet;                  //15  AutoModeSet
    u16  Pnt_ApvCnt;                       //16  ApvCnt
    u16  Pnt_TimeToStop;                   //17  TimeToStop
    u16  Pnt_Warning;                      //18  Warning
    u16  Pnt_Fail;                         //19  Fail
    u16  Pnt_End;
}FuncLOCAL_APV_V3_type;
//********************************************************
#define AUTO_MODE       1       // Автоматический режим работы ПЧ - чтобы АПВ работали
//********************************************************
u16 *FuncLOCAL_APV_V3_1(FuncLOCAL_APV_V3_type *progPnt, u32 ramPnt)
{
    u16                         Off_Block_Apv, Warning, Fail, FailIn, PowerLossFlag, num_Of_Block;
    u32                         TimeToAPV, ProgFailTime, PowerFailTime;
    LOCAL_APVState_type         *sPnt;
    LOCAL_APVnvState_type       *nvAdr;
    APVState_type               *globsPnt;
    void                        *nvAdr1;                                //указатель на флаг исчезновения питания PowerLossFlag
    void                        *nvAdr2;                                //указатель на время аварии НЕ по питанию, то есть это время останова, инициированного программой
    void                        *nvAdr3;
    u16	static		        NumFailBlock;
    u16                         delayOff, delayControl, timeToStop, EngineRun;
    u16 PowerFailFlag   = 0;
    
    sPnt  = (LOCAL_APVState_type *)(ramPnt + progPnt->Pnt_State);       //указатель на статическую область памяти
    nvAdr1 = (void *)((u32)&nv.EngineRunFlag);                    	//вычисляем адрес флага аварии по питанию
    nvAdr2 = (void *)((u32)&nv.stopTime)     ;                    	//вычисляем адрес переменной с временем программного останова по аварии
    nvAdr3 = (void *)((u32)&nv.PowerFailTime);
    nvAdr       = GetNvAdr( progPnt->Pnt_nvState, &nvAPV);           	//адрес области FRAM
    globsPnt    = &APVState;
  
    if(flagIsFirstCall) //если это самый первый проход программы
    {  // В первого прохода программы каждому функциональному блоку будет присвоен свой уникальный номер,//
       // А после после последнего вызова функционального блока в первом проходе numBlock будет содержать//
       // количество функциональных блоков (количество контроллируемых аварий)                           //
        RdFramMicro( sPnt, nvAdr, sizeof(LOCAL_APVnvState_type));       // Восстановить состояние: считываем в статическую область памяти данные из FRAM
        sPnt->numBlock = load_s16(ramPnt, progPnt->Pnt_NumOfBlock);     // Получаем номер функционального блока
        sPnt->tmrDelayControl  = 0;                                     // Обнулить таймеры
        sPnt->tmrDelayOff      = 0;
        sPnt->flag.ApvLatch    = 0;                                     // Для возможности АПВ по питанию ????
        globsPnt->APVstatus.bits.APV = 0;
        sPnt->ApvOnOff = load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv); // Читаем начальное состояние активности ПАВ для блока (Вкд/Выкл)

        return &progPnt->Pnt_End;
    }
  
    delayOff            = load_s16(ramPnt, progPnt->Pnt_DelayOff );
    delayControl        = load_s16(ramPnt, progPnt->Pnt_DelayControl);
      
    num_Of_Block = load_s16(ramPnt, progPnt->Pnt_NumOfBlock);
    
    Off_Block_Apv = load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv);       //считываем состояние сигнала управления блоком АПВ. (реакция ПЧ на отклонение параметра от нормы)
    
    // Предварительная заглушка для работы аварий и АПВ в блоках Перегруз и недогруз
    //if (num_Of_Block == 1 || num_Of_Block == 2 || num_Of_Block == 3 || num_Of_Block == 4 || num_Of_Block == 5 || num_Of_Block == 6 || num_Of_Block == 8) {
    if (num_Of_Block != 7) {
       if (Off_Block_Apv == 0)  Off_Block_Apv = 1;
       else if (Off_Block_Apv == 1) { // Если АПВ включено
         if (Off_Block_Apv != sPnt->ApvOnOff) { // Если значения не равны, значит включено АПВ, и мы однократно ставим авторежиме работы ПЧ
           save_s16(ramPnt, progPnt->Pnt_AutoModeSet, AUTO_MODE); // И активируем работу в авторежиме
         }
         Off_Block_Apv = 2; // Ставим данный признак для блока         
       }
    }
    
    sPnt->ApvOnOff = load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv); // Синхронизируем значение с уставкой. Локальную переменную Off_Block_Apv не брать

    FailIn = testBitMicro(ramPnt, progPnt->Pnt_Fail_IN);                //считываем вход сигнала отклонения контролируемого физического параметра от нормы
        
//====================================================================
//          Контроль аварии
//==================================================================== 
    if((Off_Block_Apv == 0) || (Off_Block_Apv == 3))                    //если формирование аварийного стопа и АПВ из-за текущего типа аварии запрещено
    {   sPnt->tmrDelayControl   = 0;
        sPnt->tmrDelayOff       = 0;
        sPnt->flag.all          = 0;
        sPnt->TimeToAPV         = 0;
        Warning                 = 0;
        Fail                    = 0;                                    //от текущего проверяемого типа аварии Стоп не формируем
    }
//====================================================================
//             Текущие аварии
//====================================================================
    else if(testBitMicro(ramPnt, progPnt->Pnt_On_Off) == 0)             // Контроль отключен флаги - в останове. Все сбрасываем
    {
        sPnt->tmrDelayControl  = 0;
        sPnt->tmrDelayOff      = 0;
        Warning                = 0;
        Fail                   = 0;
    }
// если в останове, то фиксируем аварию по факту
    else if (!testBitMicro(ramPnt, progPnt->Pnt_PWM_ON))
    {
        sPnt->tmrDelayControl  = 0;
        sPnt->tmrDelayOff      = 0;
        Warning                = 0;
        Fail                   = FailIn;
    }
//иначе, контроль включен. проверяем состояние таймера. Пока таймер не досчитал...(Ждем прежде чем обрабатывать ошибки)
    else if(sPnt->tmrDelayControl < delayControl + 1)                                               
    {                                                                   // Контроль включен - в работе - время запуска
        if(globsPnt->APVstatus.bits.SecondFlag == 1){                        //SecondFlag - флаг того, что секунда прошла
            sPnt->tmrDelayControl++;                                    //считаем..
        }
        Warning = 0;
        Fail    = 0;
    }
//Как только досчитали , проверим: если нет аварии на входе (если нет отклонения физического параметра от нормы)
    else if(FailIn == 0) 
    {
        sPnt->tmrDelayOff = 0;
        Warning = 0;
        Fail    = 0;
    }
    else                                                                // Иначе FailIn = 1 - есть отклонение параметра от нормы
    {                                                                   // Авария на входе
        Warning = 1;                                                    // Предупреждение на выход
        Fail    = 0;
//если текущий таймер ошибки не достиг значения времени, когда ошибка должна быть сформирована
        if( sPnt->tmrDelayOff < delayOff ) 
        {
            if(globsPnt->APVstatus.bits.SecondFlag == 1) { 
                sPnt->tmrDelayOff++;                                    // Таймер аварии
            }
            timeToStop = delayOff - sPnt->tmrDelayOff; 
        }
//иначе таймер досчитал - формируем ошибку
        else 
        {                                                               // Время вышло пора принимать меры !!
            Fail    = 1;                                                // Подтверждаем состояние аварии (аварию на выход)
            sPnt->tmrDelayOff = 0;                                      // Обнуляем таймер
        }
    }
    if(Fail && (sPnt->numBlock != 8))                                  // если был стоп не по аварии сети
    {
        PowerFailFlag   = 0;
        WrFramMicro(nvAdr1, &PowerFailFlag, sizeof(u16))    ;           // сброс флага аварии по питанию
    }
//====================================================================
//       Работа с АПВ
//====================================================================
    if(globsPnt->APVstatus.bits.APV == 0){    //если нет запроса на АПВ в глобальном блоке (или АПВ отработано)
        sPnt->flag.ApvLatch = 0;        // В Глобальном блоке нет запросов на АПВ (находимся в работе)
    }
//====================================================================
//если есть глобальное разрешение АПВ (если включен автоматический режим)
//(состояние этой переменной устанавливается в apv1.h по входному параметру функц. блока apv1.h)
//если включен автоматический режим
    if(globsPnt->APVstatus.bits.AutoMode != 0) 
    {   
      // Если для данного блока АПВ не включен, надо обнулить время аварий, 
      // иначе может быть ложный запуск по другой аварии которая не была причиной останова
      //RdFramMicro(&ProgFailTime,  nvAdr2,  sizeof(u32));                       //сброс времени аварийного программного останова PowerFailTime
      // Если выбран режим просто аварии, и был сброс флага аварии, обнулить значения времени аварий
      if (Off_Block_Apv == 1 && sPnt->flag.prevFailState && !Fail) {
        PowerLossFlag = ProgFailTime  = PowerFailTime = 0;
        WrFramMicro(nvAdr1, &PowerLossFlag, sizeof(u16));
        WrFramMicro(nvAdr2, &ProgFailTime,  sizeof(u32));  //сброс времени аварийного программного останова PowerFailTime
        WrFramMicro(nvAdr3, &PowerFailTime,  sizeof(u32));        
      }
            
// АПВ разрешены и нет сигнала стопа?
        if((Off_Block_Apv == 2) && (globsPnt->APVstatus.bits.StopKeyState == 0))     //если для текущего контролируемого параметра разрешен АПВ и НЕТ СИГНАЛА глобального СТОП??
        {    
            if(Fail != 0)                                                       //если было обнаружена и подтверждена авария
            {
//проверим флаг осуществленного запроса на АПВ. Если для текущего типа аварии запрос не отправляли      
                if( sPnt->flag.ApvLatch == 0)                    
                {       // Сюда первый раз попали в еще работе и со снятым запросом на АПВ
                	// Если превышен счет по АПВ, сбросить флаг - двигатель в работе.
                	// Сброс должен происходить ранее, чем проверка превышения и отработка счетчиков АПВ
                    normalWaitAPV = load_s16(ramPnt, progPnt->Pnt_NormalWait);             //счит. сигнал кот. означает нужно ли ждать нормализации конкретного параметра
                    if (load_s16(ramPnt, progPnt->Pnt_KolvApv) == sPnt->CntApv) {
                        EngineRun = 0;
                        WrFramMicro(nvAdr1, &EngineRun, sizeof(u16));           //Сохраняем признак включенного в рабочий режим двигателя. Флаг сбросится при обработке запроса на АПВ или при любой из команд СТОП
                    }
//если не превышено количество попыток АПВ от аварии данного типа
                    if (load_s16(ramPnt, progPnt->Pnt_KolvApv) > sPnt->CntApv)  
                    {                                                                   //если количество АПВ не достигло предельного значения
                        sPnt->flag.ApvLatch = 1;                                        //флаг осуществленного запроса АПВ
                        TimeToAPV = load_s16(ramPnt, progPnt->Pnt_ApvTime_sec);
                        //TimeToAPV = TimeToAPV + load_s32(ramPnt, progPnt->Pnt_RTC);   // Время для глобального блока АПВ (с учетом текущего времени ?)
                        sPnt->TimeToAPV         = TimeToAPV;
                        APVpnt[sPnt->numBlock]  = TimeToAPV;
                        sPnt->CntApv++;                                               //засчитываем факт осуществления АПВ
//если есть авария питания
                        if (sPnt->numBlock == 8){ 
                        	sPnt->ApvCntPower = 1;
                        }
                        globsPnt->APVstatus.bits.APV = 1;                                  //ФОРМИРУЕМ ЗАПРОС НА АПВ! теперь функц. блок  apv1 должен запустить двигатель
                        WrFramMicro( nvAdr, sPnt, sizeof(LOCAL_APVnvState_type));     //сохраняем в FRAM количесвто попыток АПВ
                    }
//если обнаружена авария, но превышено количество АПВ при данном типе аварии, и это первая авария кот. произошла в работе двигателя и от кот. возможен запрос
                    else if (testBitMicro(ramPnt, progPnt->Pnt_PWM_ON))
                    {
//тогда если уже до этого не было запроса на АПВ. То есть двигатель работает и происходит авария при которой разрешено АПВ. Но количество попыток превышено
//в этом случае нужно сбросить данные АПВ, чтобы флаг работы двигателя в FRAM не остался установленным
//Здесь все нужно сбросить!
                        PowerLossFlag = 0;
                        ProgFailTime  = 0;
                        PowerFailTime = 0;
                        WrFramMicro(nvAdr1, &PowerLossFlag, sizeof(u16));
                        WrFramMicro(nvAdr2, &ProgFailTime,  sizeof(u32));  //сброс времени аварийного программного останова PowerFailTime
                        WrFramMicro(nvAdr3, &PowerFailTime,  sizeof(u32));
                    }
                }
            }
             //Chemeris! не понятно зачем для 8 блока сохранять параметры
            else if (testBitMicro(ramPnt, progPnt->Pnt_PWM_ON)) {
            	if (sPnt->numBlock == 8) {
            		WrFramMicro( nvAdr, sPnt, sizeof(LOCAL_APVnvState_type));     //сохраняем в FRAM статик блока АПВ
            	}
            }

        }
        else if(sPnt->TimeToAPV != 0) //(Off_Block_Apv == 1)   // АПВ запрещены
        {
           sPnt->flag.ApvLatch     = 0;
           sPnt->TimeToAPV         = 0;
           APVpnt[sPnt->numBlock]  = 0;
        }
    }
    else  if(sPnt->flag.ApvLatch != 0) //иначе автоматический режим отключен
    {
       sPnt->flag.ApvLatch     = 0;
       sPnt->TimeToAPV         = 0;
       APVpnt[sPnt->numBlock]  = 0;
    }
//====================================================================
//       Сброс счетчика АПВ
//====================================================================
    if(testBitMicro(ramPnt, progPnt->Pnt_ResetApv) == 1) {                      //если пришел запрос на сброс счетчиков АПВ
        if(sPnt->flag.ResetState == 0) {                                        //если до этого было состояние  "отсутствие сброса". Чтобы команда сброса проходила один раз
            sPnt->flag.ResetState   = 1;                                        //запоминаем состояние "есть запрос на сброс"
            sPnt->CntApv            = 0;                                        //сбрасываем счетчик текущего блока
            WrFramMicro(&nvAdr->CntApv, &sPnt->CntApv, sizeof(sPnt->CntApv));   //записываем новое состояние
        }
    }
    else if (globsPnt->APVstatus.bits.StopResetApvCnt) {
        if(sPnt->flag.ResetState == 0) {                                        //если до этого было состояние  "отсутствие сброса". Чтобы команда сброса проходила один раз
            sPnt->flag.ResetState   = 1;                                        //запоминаем состояние "есть запрос на сброс"
            sPnt->CntApv            = 0;                                        //сбрасываем счетчик текущего блока
            //globsPnt->ApvResetCnt ++;                                           // Увеличили счетчик сброшенных блоков, на случай если они расположены не последовательно
            WrFramMicro(&nvAdr->CntApv, &sPnt->CntApv, sizeof(sPnt->CntApv));   //записываем новое состояние
        }
    }
    else {                                                                      //иначе нет запроса на сброс счетчика АПВ
    	sPnt->flag.ResetState  = 0;
    } //запоминаем состояние "отсутствие сброса"
    
//Если при подаче питания блоком POWER_LOSS_APV было обнаружено аварийное отключение питания и, как следствие, была попытка АПВ, то блок контроля аварий
//по питанию должен засчитать попытку АПВ. Для того, чтобы блок контроля аварий по питанию знал о произведенной
//попытке АПВ, в блоке POWER_LOSS_APV устанавливается флаг PowerLossAPV, который ниже и проверяется при вызове блока с номером 1
    if ((sPnt->numBlock == 8) && (globsPnt->APVstatus.bits.PowerLossAPV == 1)) //если текущий вызов блока для контроля напряжения и был осуществлено АПВ по восстановлению напряжения
    {
    	if(sPnt->ApvCntPower == 0) {
    		sPnt->CntApv ++;                                                //засчитываем АПВ по восстановлению напряжения
    	}
    	sPnt->ApvCntPower = 0;
        globsPnt->APVstatus.bits.PowerLossAPV = 0;                                   //сбрасываем флаг. АПВ по восстановлению напряжения учли
        WrFramMicro(&nvAdr->CntApv, &sPnt->CntApv, sizeof(sPnt->CntApv));       //сохраняем количество АПВ по напряжению
    }

//Если была авария, при которой разрешен останов двигателя, но не разрешено АПВ, то необходимо, чтобы после останова двигателя
//был сброшен флаг работающего двигателя. Это необходимо на случай, если после описанной аварии было отключено питание
//и затем повторно подано и при этом исключить непредвиденный пуск (который мог бы быть, если бы флаг оставался установленным)
//Обеспечим сброс всех данных АПВ после фактического останова двигателя. Для этого в условный оператор добавлен флаг-подхват
//который устанавливается при первом входе и сбрасывается после второго входа в тело сброса данных АПВ
    if ((Off_Block_Apv == 1) && ((testBitMicro(ramPnt, progPnt->Pnt_PWM_ON) && Fail) || sPnt->pickUpReset))
    {
        if (!(sPnt->pickUpReset))                                               //если это первый вход в сброс данных АПВ
        {
            (sPnt->pickUpReset)++;                                              //подхватываемся
        }
        else                                                                    //иначе это второй сброс
        {
            sPnt->pickUpReset = 0;
        }
        PowerLossFlag = 0;
        ProgFailTime  = 0;
        PowerFailTime = 0;
        WrFramMicro(nvAdr1, &PowerLossFlag, sizeof(u16));
        WrFramMicro(nvAdr2, &ProgFailTime,  sizeof(u32));                       //сброс времени аварийного программного останова PowerFailTime
        WrFramMicro(nvAdr3, &PowerFailTime,  sizeof(u32));
    }
//====================================================================
// Если после останова авария не пропала => Заморозка АПВ.
//====================================================================  
    if((Fail != 0) && (globsPnt->APVstatus.bits.APV == 1))
    {
        NumFailBlock = num_Of_Block;
        globsPnt->APV_Freeze = 1;
        normalWaitAPV = load_s16(ramPnt, progPnt->Pnt_NormalWait);             //счит. сигнал кот. означает нужно ли ждать нормализации конкретного параметра
    }
    else if((globsPnt->APV_Freeze != 0) && (NumFailBlock == num_Of_Block))
        globsPnt->APV_Freeze = 0;
//====================================================================
//                    Сохраняем состояние
//====================================================================
#ifdef _PROJECT_FOR_STM32_
    save_s16(ramPnt, progPnt->Pnt_ApvCnt, sPnt->CntApv );
    save_s16(ramPnt, progPnt->Pnt_TimeToStop, timeToStop );
    defBitMicro(ramPnt, progPnt->Pnt_Warning,   Warning);
    defBitMicro(ramPnt, progPnt->Pnt_Fail,      Fail   );
    sPnt->flag.prevFailState = Fail;
#endif
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLOCAL_APV_V3_1[19]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Fail_IN
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Off_Block_Apv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  DeleyControl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  DeleyOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  NormalWait
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  ApvTime_sec
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  ApvTime_min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  KolvApv
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  ResetApv
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  On/Off
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  RTC
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  VybegTime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  NumOfBlock
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  PWM_ON
        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //15  AutoModeSet
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  ApvCnt
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  TimeToStop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //18  Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //19  Fail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LOCAL_APV_V3", "DA", TblFuncLOCAL_APV_V3_1, FuncLOCAL_APV_V3_1, sizeof(TblFuncLOCAL_APV_V3_1),sizeof(LOCAL_APVState_type), sizeof(LOCAL_APVnvState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLOCAL_APV_V3_1,
#endif
//********************************************************