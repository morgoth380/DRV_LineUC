#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_OFF_ZAR;                      //1  OFF_ZAR
    u16  Pnt_DelayOffZar;                  //2  DelayOffZar
    u16  Pnt_UbridgeOnNom;                 //3  UbridgeOnNom
    u16  Pnt_UdminNom;                     //4  UdminNom
    u16  Pnt_EvacOn;                       //5  EvacOn - Включен режим эвакуации
    u16  Pnt_UdminEvac;                    //6  UdminEvac
    u16  Pnt_Udmax_Charge;                 //7  UdmaxCharge
    u16  Pnt_Udmax;                        //8  Udmax
    u16  Pnt_T_ZAR;                        //9  T_ZAR
    u16  Pnt_UinMin;                       //10 UinMin
    u16  Pnt_UinMax;                       //11 UinMax
    u16  Pnt_UinMode;                      //12 UinMode    
    u16  Pnt_AbcAcbEn;                     //13  AbcAcbEn
    u16  Pnt_ObrFazEn;                     //14  ObrFazEn
    u16  Pnt_StatusWord;                   //15  StatusWord
    u16  Pnt_WaitPhase;                    //16  WaitPhase    
    u16  Pnt_PowerModule;                  //17  PowerModule        
    u16  Pnt_visCP24B;                     //18  visCP24B
    u16  Pnt_visCP24;                      //19  visCP24    
    u16  Pnt_UinMaxFail;                   //20  UinMaxFail
    u16  Pnt_Err_3Phase;                   //21  Err_3Phase
    u16  Pnt_Fazirovka;                    //22  Fazirovka
    u16  Pnt_UinErr;                       //23  UinErr
    u16  Pnt_FazirovkaErr;                 //24  Fazirovka_Err
    u16  Pnt_CHARGE_OK;                    //25  CHARGE_OK
    u16  Pnt_ERR_ZAR;                      //26  ERR_ZAR
	u16  Pnt_UdMaxPu;                      //27  UdMaxPu
	u16  Pnt_UdMinAbs;                     //28  UdMinAbs
    u16  Pnt_LowHiPower;                   //29  LowHiPower
    u16  Pnt_Ud_KZ_Fail;                   //30  Ud_KZ_Fail    
    u16  Pnt_WarningCharge;                //31  WarningCharge
    u16  Pnt_Start;                        //32  Start
    u16  Pnt_Zar;                          //33  Zar
    u16  Pnt_End;
}FuncDRV_UD_type;

//******************************************************************************
typedef enum {
	phaseItrlvABC=0,	// phase interleaving is- ACB
	phaseItrlvACB,
	phaseLoss,
}PHASESTATE_code_enum;

//#define CALC_TO_uS              100/150.f
//#define CALC_TO_mS              100/150000.f
//#define	PCP_RELE_TIMEOUT	10  // 10 тактов микропрограммы = 40 мс
#define UD_CHECK_TIME   0.1f*K_TIME_CALL_MICRO_TAKT // Такт проверки скорости фронта Ud 100 мс.
#define MAX_DELTA_UD    5.0f  
#define UD_CONTROL_KZ         50.0f                       // Контрольная величина напряжения при заряде Combo
#define TIME_CONTROL_KZ       1.0f                        // Контрольное время при заряде Combo
#define TIME_RESTART_CHARG    2.0f                        // Время перезапуска заряда для Combo при выявлении КЗ
// Максимальная скорость роста напряжения в ЗПТ
//******************************************************************************
// Объявление переменных с глобальным временем жизни
typedef struct{
    u16 powerUpDelay;
    u16 chargeTimer;
    u16 Count3Phase;
    u16 PrReady;
    u16 ErrorZarad;
    u16 PrAvariaThrPhase;
    u16 UdCheckTimer;
    f32 UdPrevValue;
    f32 deltaUd;
    u16 RestartСhargeTimer;
    u16 ControlKzTimer;
}DRVUD_State_type;
//********************************************************
u16 *FuncDRV_UD_1(FuncDRV_UD_type *progPnt, u32 ramPnt)
{
    DRVUD_State_type    *sPnt;
    FailsSetup_type     *pFailsSetup;
    f32                 UbridgeOn, Udmin, Udmax, Udmax_Charge, Uin_max, UbridgeOn_perc;
    //u16                 Status;
    f32                 divUabUbc, divUbcUca, divUcaUab;
    f32			UinMin, UinMax;
    u16                 ManualPowerBlock; 
      
    sPnt                = (DRVUD_State_type *)(ramPnt + progPnt->Pnt_State);
    pFailsSetup         = &GlobalM4.FailsSetup;
        
    if(flagIsFirstCall){
        sPnt->powerUpDelay	= 2 * K_TIME_CALL_MICRO_TAKT;                                      // Блокировка отработки аварии обрыва фаз нпри пуске на 2 секунды
        sPnt->chargeTimer 	= (load_s16(ramPnt, progPnt->Pnt_T_ZAR) * K_TIME_CALL_MICRO_TAKT); //
        sPnt->Count3Phase       = K_TIME_CALL_MICRO_TAKT;
        sPnt->PrReady           = 0;
        sPnt->PrAvariaThrPhase  = 0;
        sPnt->ErrorZarad        = 0;
        sPnt->UdCheckTimer      = UD_CHECK_TIME; 
        sPnt->UdPrevValue       = 0;
        sPnt->deltaUd           = 100.0; // Задаем первоначальный скачок напряжения равным 100В
        
        
//        #warning Надо проверить а что раньше стартует. Таск 20 мс или микропрограмма. Мне надо понимать смогу ли я передать тип блока в систему выбора авариий ключей
        // Если не ноль, значит есть предустановка типа силового блока        
        // Читаем текущую настройку силового блока
        ManualPowerBlock = load_s16(ramPnt, progPnt->Pnt_PowerModule);
        GlobalM4.DrvTypePreset = (DrvType_Enum)ManualPowerBlock;        
    }
    
    //--	Контроль  напряжения Ud и управление зарядом --//
    Uin_max = 1.414f*IQ_to_FLOAT(1, Volt_base); // Предварительная настройка параметра, для блока СР24, где сеть не измеряем.
    //UbridgeOn = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_UbridgeOnNom), Volt_base);
    UbridgeOn_perc = (f32)load_s16(ramPnt, progPnt->Pnt_UbridgeOnNom) / 100.0f; // Процент напряжения для открытия

    Udmin = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_UdminNom), Volt_base);
    Udmax = (f32)load_s16(ramPnt, progPnt->Pnt_Udmax); //IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Udmax), Volt_base);
    Udmax_Charge = (f32)load_s16(ramPnt, progPnt->Pnt_Udmax_Charge); //IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Udmax_Charge),Volt_base);
        
    // Проверяем текущее значение Udmax с Юстировочным числом по Ud
    if ((pFailsSetup->Udustir*0.98f) < Udmax) { // Если Юстировочное, меньше Udmax - перебиваем значение вручную
      Udmax = (pFailsSetup->Udustir*0.98f); // Ставим Udmax = 0.98 от юстировочного числа     
      save_float(ramPnt, progPnt->Pnt_Udmax,FLOAT_to_IQ(Udmax, Volt_base)); // И сохраняем в уставку
    }           
    pFailsSetup->UdMax	      = Udmax;
    pFailsSetup->UdMin	      = Udmin;
    pFailsSetup->Udmax_Charge = Udmax_Charge;
        
	save_s16(ramPnt, progPnt->Pnt_UdMinAbs, (s16)Udmin);
	save_float(ramPnt, progPnt->Pnt_UdMaxPu, FLOAT_to_IQ(Udmax, Volt_base));

    //******************************************************************************    
    // Контроль Uin
    //******************************************************************************    	
    UinMin = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_UinMin), Volt_base);
    UinMax = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_UinMax), Volt_base);
    
    // Проверяем активность режима эвакуации, сигнал активен только когда разрешен лифт и стартовал режим эвакуации
    if (!testBitMicro(ramPnt, progPnt->Pnt_EvacOn)) { // Если эвакуации нету, разрешаем обработку аварий по входной сети
      pFailsSetup->ControlUin = (u16)load_s16(ramPnt, progPnt->Pnt_UinMode);
    } else { // Если идет работа в эвакуации - блокируем проверку напряжений входной сети
      pFailsSetup->ControlUin = 0;
    }
    
    pFailsSetup->UinMax	= UinMax;
    pFailsSetup->UinMin	= UinMin;    
    //******************************************************************************            
    #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
        // если прошивка СР24В - то все коротко                   
        save_s16 (ramPnt, progPnt->Pnt_LowHiPower, GlobalM4.DrvType);
        // Выдаем сигналы видимости для определенных блоков
        save_s16 (ramPnt, progPnt->Pnt_visCP24, 0); 
        save_s16 (ramPnt, progPnt->Pnt_visCP24B, 1); // Видимость для блока CP24B
    #elif defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
        save_s16 (ramPnt, progPnt->Pnt_LowHiPower, GlobalM4.versionCP24 + 4); // Сдвижка для отображения правильного типа блока
        // Выдаем сигналы видимости для определенных блоков        
        save_s16 (ramPnt, progPnt->Pnt_visCP24, 1); // Видимость для блока CP24        
        save_s16 (ramPnt, progPnt->Pnt_visCP24B, 0); 
    #endif
    
    // Сброс аварий сети до их проверки - Предварительная инициализация аварий по сети
    clrBitMicro(ramPnt, progPnt->Pnt_FazirovkaErr); // Начальный сброс аварии фазировки    
    clrBitMicro (ramPnt, progPnt->Pnt_Err_3Phase);
    clrBitMicro (ramPnt, progPnt->Pnt_UinErr);
    save_s16(ramPnt, progPnt->Pnt_Fazirovka, (u16)UndefinedSequence);
    
#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    
    Uin_max = ((GlobalM4.ValUab.Val >= GlobalM4.ValUbc.Val) && (GlobalM4.ValUab.Val >= GlobalM4.ValUca.Val)) ? GlobalM4.ValUab.Val : ((GlobalM4.ValUbc.Val >= GlobalM4.ValUca.Val) ? GlobalM4.ValUbc.Val : GlobalM4.ValUca.Val);
    // Сделать проверку, а вдруг отключат защиту по входной сети и одной из фаз не будет, тогда будет ошибка определения порога уровня зарядного Ud
    if (Uin_max < UinMin)  Uin_max = UinMin;
    Uin_max *= 1.414f; // Амплитуда входной сети
	// Перед деление входных напряжений, делаем проверку на не ноль, и только после этого будем делить.
	// А если знаменатель ноль - то ставим отношение равное 2.0
	divUabUbc = (GlobalM4.ValUbc.Val != 0) ? GlobalM4.ValUab.Val / GlobalM4.ValUbc.Val : 2.0f;
	divUbcUca = (GlobalM4.ValUca.Val != 0) ? GlobalM4.ValUbc.Val / GlobalM4.ValUca.Val : 2.0f;
	divUcaUab = (GlobalM4.ValUab.Val != 0) ? GlobalM4.ValUca.Val / GlobalM4.ValUab.Val : 2.0f;
        
    // Проверка обрыва фаз. Разрешена проверка?
    if (load_s16(ramPnt, progPnt->Pnt_ObrFazEn)) {
        // если два линейных меньше 20 В - авария
        if ( GlobalM4.ValUab.Val < 30.0 &&  GlobalM4.ValUbc.Val < 30.0) {
            setBitMicro (ramPnt, progPnt->Pnt_Err_3Phase);
        } else if ( ((divUabUbc < 0.7f || divUabUbc > 1.5f) && (divUbcUca < 0.7f || divUbcUca > 1.5f)) || 
                    ((divUabUbc < 0.7f || divUabUbc > 1.5f) && (divUcaUab < 0.7f || divUcaUab > 1.5f)) || 
                    ((divUbcUca < 0.7f || divUbcUca > 1.5f) && (divUcaUab < 0.7f || divUcaUab > 1.5f)) ) {
          setBitMicro (ramPnt, progPnt->Pnt_UinErr);
        }
        
    }
    
    // Выдаем текущую фазировку - по факту, с учетом аварий по сети
    if ( testBitMicro (ramPnt, progPnt->Pnt_Err_3Phase) || testBitMicro (ramPnt, progPnt->Pnt_UinErr) ) {
      save_s16(ramPnt, progPnt->Pnt_Fazirovka, (u16)BrakeInputPhase);       
    } else { // Аварий сети нету - выдаем фактическую фазировку и проверяем аварию фазировкиы     
      save_s16(ramPnt, progPnt->Pnt_Fazirovka, GlobalM4.UinPhasingFlag);
      
      // Проверка текущего режима проверки Фазировки входной сети
      if ( load_s16 (ramPnt, progPnt->Pnt_AbcAcbEn) == 1 ) {// Вычитка режима обработки аварии
        // Активирован режим проверки фазировки входной сети
        if ( GlobalM4.UinPhasingFlag == NegativePhaseSequence) { 
          // Если выявлена обратная последовательность - выдаем сигнал аварии
          setBitMicro(ramPnt, progPnt->Pnt_FazirovkaErr);        
        }      
      }            
    }    
#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    
    UbridgeOn = UbridgeOn_perc * Uin_max;
    
    //-------------------------------------------------------------------------
    // Проверяем активность режима эвакуации, сигнал активен только когда разрешен лифт и стартовал режим эвакуации
    if (testBitMicro(ramPnt, progPnt->Pnt_EvacOn)) {
      // Заблокировать все аварии по сети
      clrBitMicro(ramPnt, progPnt->Pnt_FazirovkaErr); // Начальный сброс аварии фазировки    
      clrBitMicro (ramPnt, progPnt->Pnt_Err_3Phase);
      clrBitMicro (ramPnt, progPnt->Pnt_UinErr);
      // Вычитываем значение Udmin для эвакуации
      //Udmin = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_UdminEvac), Volt_base);
      UbridgeOn = 1.05f*Udmin; // Ud окончания заряда даем на уровне 5% от Udmin. Т.к. очень вероятно что здесь будут аккумуляторы в ЗПТ
    }
    //-------------------------------------------------------------------------
            
    // 2) Получить тип ПЧ - РАБОТАЕМ СО ВСЕМИ ПЧ ОДИНАКОВО !!! 
    if( 1 /*GlobalM4.DrvType == LowPow*/){   // Управление зарядом для ПЧ малой мощности (СР24 и СР24В Low)
        if (testBitMicro(ramPnt, progPnt->Pnt_OFF_ZAR)){ //блокировка
            clrBitMicro(ramPnt, progPnt->Pnt_Zar);
            clrBitMicro(ramPnt, progPnt->Pnt_Start);
            clrBitMicro(ramPnt, progPnt->Pnt_WarningCharge);
            GlobalM4.FlagCommand.bit.ChargeOk = 0;       // При отключении заряда, откидываем  ChargeOk в ноль
            clrBitMicro(ramPnt, progPnt->Pnt_CHARGE_OK);
            GlobalM4.FlagCommand.bit.ChargeInProgress = 0; // Заряд не происходит
            
            sPnt->chargeTimer = (load_s16(ramPnt, progPnt->Pnt_T_ZAR) * K_TIME_CALL_MICRO_TAKT); //
            sPnt->ControlKzTimer = (u16)(TIME_CONTROL_KZ * (float)K_TIME_CALL_MICRO_TAKT);          // Таймер контроля КЗ в ЗПТ 
                        
            if(sPnt->ErrorZarad > 1){
                sPnt->ErrorZarad--;
                if(sPnt->ErrorZarad == 3){
                    clrBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR);
                }
            } 
        } 
        else {   
            // Ожидание стартового заряда 
            if(sPnt->chargeTimer){ // Ветвь стартового заряда Ud
                GlobalM4.FlagCommand.bit.ChargeInProgress = 1; // Сигнал - что идет заряд ЗПТ
                sPnt->chargeTimer--;
                setBitMicro(ramPnt, progPnt->Pnt_WarningCharge);         // Сформировать предупреждение "Идет заряд ЗПТ!!!"
                setBitMicro(ramPnt, progPnt->Pnt_Zar);
                // Находим приращение 
                if (!sPnt->UdCheckTimer) {
                    sPnt->deltaUd = (GlobalM4.ValUd.Val - sPnt->UdPrevValue); // Приращение находим 
                    sPnt->UdCheckTimer = UD_CHECK_TIME;            // и таймер перезапускаем
                    sPnt->UdPrevValue = GlobalM4.ValUd.Val;
                } else  {
                    sPnt->UdCheckTimer --;
                }
                GlobalM4.deltaUdCharge = sPnt->deltaUd; // TEST  
                // 1. Приращение звена не должно быть больше 5 В за интервал наблюдения
                // 2. Мост разрешено открыть если напряжение выше напряжения включения моста и менее Udmax_Charge.
                if (sPnt->deltaUd < MAX_DELTA_UD && GlobalM4.ValUd.Val >= UbridgeOn){              // Сравниваем приращение напряжение звена из глобальной структуры с напряжением в 5В
                    if( GlobalM4.ValUd.Val <= Udmax_Charge ){        // Проверим напряжение ЗПТ с Ubmax_Charge
                        setBitMicro(ramPnt, progPnt->Pnt_Start);     // Шунтируем зарядный резистор!!!
                        clrBitMicro(ramPnt, progPnt->Pnt_Zar);       // Прекратить процес заряда                        
                        sPnt->chargeTimer = 0;
                    } else {
                        GlobalM4.FlagCommand.bit.ChargeOk = 0;
                        clrBitMicro(ramPnt, progPnt->Pnt_CHARGE_OK);
                        //авария - ошибка в входной сети - напряжение на входе уже выше, чем ожидается.
                        setBitMicro(ramPnt, progPnt->Pnt_UinMaxFail);  
                    }
                }
                else{
                    if(sPnt->chargeTimer == 1){                    // Не удалось зарядить ЗПТ за заданное время  
                        setBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR); // Выставляем аварию за один тик до окончания отсчета
                        clrBitMicro(ramPnt, progPnt->Pnt_WarningCharge);         // ЗПТ заряжено! сбросить предупреждение                    
                        sPnt->ErrorZarad = 10;
                        sPnt->chargeTimer = 0;
                    }
                }
                
                // Контроль КЗ в ЗПТ
                sPnt->ControlKzTimer--;
                if(sPnt->ControlKzTimer == 0){
                    sPnt->ControlKzTimer++;
                    
                    // Проверить напряжение в ЗПТ после TIME_CONTROL_KZ времени от начала заряда
                    if(GlobalM4.ValUd.Val < UD_CONTROL_KZ){
                        // АВАРИЯ!!! КЗ в ЗПТ
                        setBitMicro(ramPnt, progPnt->Pnt_Ud_KZ_Fail);      // Сформировать аварию "КЗ в ЗПТ"
                        sPnt->ErrorZarad = 10;                           // Время удержания аварии в активном состоянии
                    }
                }                
                
            }
            // если уже была авария заряда Ud, после чего зарядили Ud
            else if (testBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR)){
              if (GlobalM4.ValUd.Val >= UbridgeOn) { // Если подзарядились
                clrBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR);
                clrBitMicro(ramPnt, progPnt->Pnt_Ud_KZ_Fail);                   
                // начать заряд (повторно)  
                sPnt->chargeTimer = (load_s16(ramPnt, progPnt->Pnt_T_ZAR) * K_TIME_CALL_MICRO_TAKT); //
                // Таймер контроля КЗ в ЗПТ
                sPnt->ControlKzTimer = (u16)(TIME_CONTROL_KZ * (float)K_TIME_CALL_MICRO_TAKT);                      
              }
              clrBitMicro(ramPnt, progPnt->Pnt_Start);
              clrBitMicro(ramPnt, progPnt->Pnt_Zar);             // Начать заряд              
            }
            else{  // Ветвь нормальной работы после заряда Ud 
                GlobalM4.FlagCommand.bit.ChargeInProgress = 0; // Заряд не происходит
                clrBitMicro(ramPnt, progPnt->Pnt_WarningCharge);         // ЗПТ заряжено! сбросить предупреждение
                // Штатная обработка, которая повторно не проверяет отключение
                if((GlobalM4.ValUd.Val >= Udmin) && (GlobalM4.ValUd.Val <= Udmax)){
                    if(!testBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR)){
                        GlobalM4.FlagCommand.bit.ChargeOk = 1;
                        setBitMicro(ramPnt, progPnt->Pnt_CHARGE_OK);
                    }
                }
                else{
                    GlobalM4.FlagCommand.bit.ChargeOk = 0;
                    clrBitMicro(ramPnt, progPnt->Pnt_CHARGE_OK);
                    clrBitMicro(ramPnt, progPnt->Pnt_Zar);         // Отключаем выпрямитель
                    clrBitMicro(ramPnt, progPnt->Pnt_Start);
                    // Повторное включение после пропадания блокировки заряда Pnt_OFF_ZAR, которая приходит из Fail_Common_V2
                    // Перезапуск идет через штатную ветку выше.
                } 
            } 
        }
    }    
    
    GlobalM4.ChargeOk = GlobalM4.FlagCommand.bit.ChargeOk;
    GlobalM4.ChargeInProgress = GlobalM4.FlagCommand.bit.ChargeInProgress;      
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDRV_UD_1[33]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  OFF_ZAR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  DelayOffZar
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  UbridgeOnNom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  UdminNom
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  EvacOn
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  UdminEvac
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  UdmaxCharge
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Udmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  T_ZAR        
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  UinMin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  UinMax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  UinMode                
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13 AbcAcbEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14 ObrFazEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15 StatusWord
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16 WaitPhase 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17 PowerModule        
        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  visCP24
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //19  visCP24B            
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //20  UinMaxFail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  ERR_3Phase
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Fazirovka
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  UinErr
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  FazirovkaErr
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  CHARGE_OK
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  ERR_ZAR
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //27  UdMaxPu
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  UdMinAbs
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  HiLowPower
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //30  Ud_KZ_Fail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //31  WarningCharge
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //32  Start
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //33  Zar
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DRV_UD", "DA", TblFuncDRV_UD_1, FuncDRV_UD_1, sizeof(TblFuncDRV_UD_1), sizeof(DRVUD_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDRV_UD_1,
#endif
//********************************************************
