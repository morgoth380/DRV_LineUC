#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Pusk_In;                      //1  Pusk_In
    u16  Pnt_Stop_In;                      //2  Stop_In
    u16  Pnt_FailTek;                      //3  FailTek
    u16  Pnt_ContMode;                     //4  Pnt_ContMode
    u16  Pnt_TimeoutCont;                  //5  Pnt_TimeoutCont
    u16  Pnt_ContactorDin;                 //6  Pnt_ContactorDin
    u16  Pnt_ContErrEn;                    //7  Pnt_ContErrEn
    u16  Pnt_PretorqEnable;                //8  Pnt_PretorqEnable
    u16  Pnt_PretorqTime;                  //9  Pnt_PretorqTime
    u16  Pnt_BrakeEnable;                  //10  Pnt_BrakeEnable
    u16  Pnt_BrakeMode;                    //11  Pnt_BrakeMode
    u16  Pnt_IoffBrake;                    //12  Pnt_IoffBrake
    u16  Pnt_FtormOn;                      //13  Pnt_FtormOn
    u16  Pnt_FtormOff;                     //14  Pnt_FtormOff
    u16  Pnt_DelayBrkOff;                  //15  Pnt_DelayBrkOff
    u16  Pnt_DelayBrkOn;                   //16  Pnt_DelayBrkOn
    u16  Pnt_TimOutBrkFB;                  //17  Pnt_TimOutBrkFB
    u16  Pnt_DinBrake;                     //18  Pnt_DinBrake
    u16  Pnt_Ifull;                        //19  Pnt_Ifull
    u16  Pnt_Inom;                         //20  Pnt_Inom
    u16  Pnt_StatusDRV;                    //21  Pnt_StatusDRV
    u16  Pnt_Fout;	                   //22  Fout 
    u16  Pnt_StateGT;                      //23  Pnt_StateGT		
    u16  Pnt_StopFreq;                     //24  Pnt_StopFreq    
    u16  Pnt_UnbkrMode;                    //25  Pnt_UnbkrMode
    u16  Pnt_UnbkrCurr;                    //26  Pnt_UnbkrCurr
    u16  Pnt_UnbkrFreset;                  //27  Pnt_UnbkrFreset
    u16  Pnt_UnbkrTfix;                    //28  Pnt_UnbkrTfix    
    u16  Pnt_EncPhasWork;                  //29  Pnt_EncPhasWork
    u16  Pnt_EncPhasDone;                  //30  Pnt_EncPhasDone  
    u16  Pnt_EncPhasCurr;                  //31  Pnt_EncPhasCurr  
    u16  Pnt_LiftMaxTorq;                  //32  Pnt_LiftMaxTorq  
    u16  Pnt_FunctionLft;                  //33  FunctionLft
    
    u16  Pnt_MaxTorqOut;                   //34  Pnt_MaxTorqOut
    u16  Pnt_F_VprUp;                      //35  F_VprUp
    u16  Pnt_F_Brake;                      //36  F_Brake
    u16  Pnt_F_LiftBrkwoPWM;               //37  Резерв
    u16  Pnt_HLD_ZR;                       //38  HLD_ZR
    u16  Pnt_BrakeOff;                     //39  BrakeOff
    u16  Pnt_ProzvonOn;                    //40  ProzvonOn
    u16  Pnt_ContNoFB;                     //41  Pnt_ContNoFB
    u16  Pnt_ContBrkFB;                    //42  Pnt_ContBrkFB
    u16  Pnt_ContactorOn;                  //43  ContactorOn
    u16  Pnt_HoldGT;                       //44  HoldGT !!!!*****НЕ ИСПОЛЬЗУЕТСЯ*****!!!!
    u16  Pnt_WorkFlag;                     //45  WorkFlag
    u16  Pnt_Stop_Out;                     //46  Stop_Out
    u16  Pnt_Pusk_Out;                     //47  Pusk_Out
    u16  Pnt_End;
}FuncLIFT_CMD_V3_type;


//********************************************************
#define vPrUp_BREAK_TMR_MAX_TIME		5 // Максимальное время отведенное на намагничивание двигателя, в случае ожидания ответа из C28 ядра.
#define failFixedVprUpTmr_MAX_TIME		5 // Время на фиксацию ошибки намагничивания
#define failFixedBrakeTmr_MAX_TIME		2 // Время на фиксацию ошибки намагничивания
#define failFixedContactorTmr_MAX_TIME	        0.5 // Время на фиксацию ошибки аварии контактора 
#define failFixedContNoFBTmr_MAX_TIME	        3 // Error Fix time when FB isn't receive

#define PRETORQUEMODE_OFF          0
#define PRETORQUEMODE_ACTIVEMODE   1
#define PRETORQUEMODE_COMBINATION  2

#define	TIME_CURRENT_UP_ENCO_PHASING	500	// Время подъема тока двигателя в режиме автофазирования энкодера

//********************************************************
// Объявление переменных с глобальным временем жизни
// Объявление состояний
typedef enum { // Состояния командоаппарата из DRV_V4
	DRVinStopMode 	= (1<<0), // Статус стопа
	DRVinPuskMode  	= (1<<1), // Статус - старт
	DRVinFailMode 	= (1<<2),
	DRVinSftCurDwn	= (1<<5), // ПЧ находится в состоянии плавного сброса тока
	DRVinRunMode 	= (1<<6), // Привод поехал
    DRVinContyChk 	= (1<<7), // Inverter in County Check Mode
}DRIVE_algState_enum;


typedef union{ 
	 struct{       
            u16 stopCmd       :1;
	    u16 runCmd        :1;
	    u16 failCmd       :1;
	    u16 PwmOn         :1;    
	    u16 dcInjectEna   :1;
	    u16 softCurDwnEna :1;
            u16 driveRunning  :1; 
            u16 countyChk     :1; 
	    u16 reserve       :8;
	 }bit;
         u16 all;
} StatusDrv_enum;

typedef enum {
	liftStopped		=0,
	contactorControls	=1,		// Стартуем контактор
	startDRV 		=2,		// Даем команду на запуск DRV_V5
	ackPwmOn		=3,		// Ждем ОС что ШИМ запустился, а значит уже пошла прозвонка
	loadContyTest		=4,		// Прозвонка двигателя
        preBrakeBlock           =5,             // Set Brake off timeout
        waitPhirUp              =6,             // Wait vPrUp flag
        encoPhasingStart        =7,             // Предустановки для режима фазирования энкодера
	pretorqueSet		=8,		// Активируем предмомент (накачка интеграла РС)
	brakeControls		=9,		// Работа с тормозом (wait here while blocktime not Zero)
	liftRunning		=10,		// Лифт поехал
	liftCMDFail		=11,		// Авария
}LIFTCMD_algState_enum;

typedef struct{
    LIFTCMD_algState_enum		algState;
	union{	u16			value;
	struct{	u16			stopCmd      :1;
		u16			runCmd       :1;
		u16			reserve      :14;
		}bit;
	}algCommand;
        u16				contSwOnTmr;
	u16				contSwOffTmr;
        u16				brakeHldOnTmr;
	u16				brakeHldOffTmr;
	u16			        brakeBlockingTime;
	u16				pretorqueDelayTime;
        u16                             encoPhasingDelayTime;
	u16				brakeOnDelay;
	u16				vPrUpBreakTmr; // Таймер срыва зависания, если намагничивание не получено за отведенное время
	u16				failFixedTmr;  // Таймер блокировки сброса аварии
	u16				StopFixTime; // Дополнительное время фиксации в стопе
	u16				StopTrig;
}LIFTCMD_State_type;

//typedef enum{
//	UsePumpFunc = 0,    //!< Выбран функционал насоса
//	UseLiftFunc = 1     //!< Выбран функционал лифта
//}FunctionalLiftCMD_type;

typedef enum {
	contactorCtrlDis=0,
	contactorCtrlWithoutFdb,        //Control without feedback from comtactor
	contactorCtrlWithFdb,		//check feedback from comtactor
}LIFTCMD_contMode_enum;

typedef enum {
	brakeCtrlDis=0, // ОС тормоза не смотрим
	brakeCtrlWithoutFdb, // Отработка No Feedback
	brakeCtrlWithFdb, // Brake with Feedback
} LIFTCMD_brakeMode_enum;

typedef struct{
	u16 addr;
	s32 value;
}WriteData;


//********************************************************
u16 *FuncLIFT_CMD_V3_1(FuncLIFT_CMD_V3_type *progPnt, u32 ramPnt)
{
    LIFTCMD_State_type	*sPnt;

    u16			        StateGT, tmp_StopIn, tmp_StartIn;
    StatusDrv_enum              tmp_StatusDRV;
    u16                         EncoderPhasingWork, EncoderPhasingDone;
    //u16		        tmpLoadContyCheckTime;
    u16			        BrakeState; //, Brake2; // BrakeMode;
    u16                         ContactorMode, locTimeUI_Up; /*EnablePretorqueMode,*/
    f32                         LiftMaxTorque, locUnbreakTorqPMSM;
    LIFTCMD_brakeMode_enum      BrakeMode;

    sPnt    = (LIFTCMD_State_type *)(ramPnt + progPnt->Pnt_State);

    if((AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_FunctionLft) == UseLiftFunc){
      if(flagIsFirstCall){
        clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Out);
        clrBitMicro(ramPnt, progPnt->Pnt_Stop_Out);
        sPnt->algState			= liftStopped;						// алгоритм в состояние останов
        clrBitMicro(ramPnt, progPnt->Pnt_BrakeOff);					// инициируем наложение тормоза
        clrBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);   //!!! Этот сигнал не используется
        sPnt->brakeHldOnTmr		= 0;
        sPnt->brakeHldOffTmr	= 0;
        sPnt->brakeOnDelay		= 0;
        clrBitMicro(ramPnt, progPnt->Pnt_ProzvonOn);				// инициируем отключение контактора и прозвонки
        clrBitMicro(ramPnt, progPnt->Pnt_ContactorOn);
        sPnt->contSwOnTmr		= 0;
        sPnt->contSwOffTmr		= 0;
        sPnt->vPrUpBreakTmr		= 0;
        sPnt->failFixedTmr		= 0;
        sPnt->StopFixTime		= 0;
        sPnt->StopTrig			= 0;
        sPnt->pretorqueDelayTime        = 0;
        sPnt->encoPhasingDelayTime      = 0;
        return &progPnt->Pnt_End;
      }

      //*********************************************************************************************
      // SET MAXIMUM TORQUE LIMIT FOR VFD IN LIFT MODE AS 3x OF NOMINAL TORQUE
      LiftMaxTorque = load_float(ramPnt, progPnt->Pnt_LiftMaxTorq);
      save_float(ramPnt, progPnt->Pnt_MaxTorqOut, LiftMaxTorque);
      //-----------------------------------------------------------------------      
      
//      // Анализ входных данных
      tmp_StopIn = (u16)(testBitMicro(ramPnt, progPnt->Pnt_Stop_In));
      tmp_StatusDRV.all = load_s16(ramPnt, progPnt->Pnt_StatusDRV);
      tmp_StartIn = (u16)(testBitMicro(ramPnt, progPnt->Pnt_Pusk_In));
      
      // Фиксируем состояние колодок тормоза
      BrakeState = testBitMicro(ramPnt, progPnt->Pnt_DinBrake); // Brake feedback
//      Brake1 = load_s16(ramPnt, progPnt->Pnt_DinBrake) & 0x0F;
      // Фильтруем состояние ОС колодок - в зависимости от режима - одна или две.
//      if (load_s16(ramPnt, progPnt->Pnt_BrakeMode) == brakeCtrlDis) { Brake1 = Brake2 = 0;}
//      else if (load_s16(ramPnt, progPnt->Pnt_BrakeMode) == brakeCtrlSingle) { Brake2 = 0; }
      
      BrakeMode = (LIFTCMD_brakeMode_enum)load_s16(ramPnt, progPnt->Pnt_BrakeEnable);
      clrBitMicro(ramPnt, progPnt->Pnt_F_LiftBrkwoPWM); // Reset fault - Released brake without PWM
      ContactorMode = load_s16(ramPnt, progPnt->Pnt_ContMode);
//      temp = load_float(ramPnt, progPnt->Pnt_FtormOff);
      
      //		// ***** проверка времени прозвонки и времени блокировки на снятие тормоза *****
      //		tmpLoadContyCheckTime = (u16)((load_s16(ramPnt, progPnt->Pnt_DelayBrkOff) * 0.01f) * GlobalM4.Fpwm); // Определяем время блокировки снятия тормоза в тактах ШИМА, так как в течение этого времени должна проходить стадия прозвонки
      //		//Если время блокировки тормоза меньше, чем уставновленное время прозвонки, то получится что можем не успеть отработать прозвонку, а тормоз уже будет разблокирован и будет подан ШИМ
      //		if (tmpLoadContyCheckTime < GlobalM4.timeLoadContyCheck) {
      //			// Поэтому если время меньше, его надо сделать, не меньше чем время прозвонки
      //			save_s16(ramPnt, progPnt->Pnt_DelayBrkOff, (u16)(100*GlobalM4.timeLoadContyCheck * GlobalM4.Tpwm)); // И подменить это значение в данных - для визуального контроля в пуске
      //		}

      //******************************************************************************    
      // Параметры preTorque
      //******************************************************************************           
      GlobalM4.Pretorque.Mode = load_s16(ramPnt, progPnt->Pnt_PretorqEnable); // Check state of Pretorque mode
      //GlobalM4.Pretorque.Enable = GlobalM4.Pretorque.Mode ? 1 : 0; // Если режим не ноль, значит разрешаем работу Предмомента
      GlobalM4.Pretorque.mPretorqueTime = 0.01f*load_s16(ramPnt, progPnt->Pnt_PretorqTime); // !!! Load pretorque restore time, in seconds
      
      //******************************************************************************    
      // Параметры pmsmAntirollback
      //******************************************************************************     
      #ifdef _PROJECT_FOR_STM32_ 
      #warning Нужно нормально подключить параметры к системе PMSM, а то не красиво так втягивать данные          
      #endif
      // UnbreakTorqPMSM & timeUI_Up предварительно копируем в локальную переменную, так как ниже может быть ее перезагрузка другим значением
      // А если так не сделать, в такте ШИМ могут возникать прыжки в значении тока удержания и времени, если между перезаписью будет прерывание.
      locUnbreakTorqPMSM	                = 0.001f * load_s16(ramPnt, progPnt->Pnt_UnbkrCurr); //1.5f; //150% - Значение задается в процентах относительно номинального тока двигателя
      locTimeUI_Up		                = (u16)(GlobalM4.Pretorque.mPretorqueTime*1000.0f); // GlobalM4.pmsmAntirollback.timeUI_Up - Время подъема тока примагничения делам таким же как и подъем тока предмомента
      GlobalM4.pmsmAntirollback.EnEncoPhasing   = 0; // !!!
      
      GlobalM4.pmsmAntirollback.pmsmAntirollBackMode = load_s16(ramPnt, progPnt->Pnt_UnbkrMode); //   
      //#define PMSM_ANTIROLLBACK_OFF   0
      //#define PMSM_ANTIROLLBACK_ID_FIX_FREQ     1 // Режим примагничивания к полюсу в зависимости от частоты. Ниже 2 Гц намагничение уменьшается до нуля
      //#define PMSM_ANTIROLLBACK_ID_FIX_TIME	  2 // Режим примагничивания к полюсу в зависимости от времени. Падение начинается после снятия тормоза    
      //#define PMSM_ANTIROLLBACK_ENCO_PHASING    3 // Режим фазировки энкодера      
      GlobalM4.pmsmAntirollback.FixTimeId	= (u16)(10.00f * load_s16(ramPnt, progPnt->Pnt_UnbkrTfix));  //  в мс
      GlobalM4.pmsmAntirollback.timeStepDown	= (u16)(GlobalM4.Pretorque.mPretorqueTime * 1000.0f); // в мс - Время сброса тока примагничения делам таким же как и подъем тока предмомента
      GlobalM4.pmsmAntirollback.FrequencyDown	= load_float(ramPnt, progPnt->Pnt_UnbkrFreset); //AllPrmPnt->UNBREAKTORQPMSM_FRESET; //FLOAT_to_IQ(1.0f, Herz_base); // 

      //******************************************************************************    
      // Параметры для фазировки энкодера, должны быть ниже параметров pmsmAntirollback
      //******************************************************************************                       
      EncoderPhasingWork = testBitMicro(ramPnt, progPnt->Pnt_EncPhasWork);
      EncoderPhasingDone = testBitMicro(ramPnt, progPnt->Pnt_EncPhasDone);
      // Если активен режим фазировки энкодера, то нам надо подменить ток удержания кабины лифта
      if (EncoderPhasingWork) {
        locUnbreakTorqPMSM	= 0.001f * load_s16(ramPnt, progPnt->Pnt_EncPhasCurr); //150% - Значение задается в процентах относительно номинального тока двигателя        
        locTimeUI_Up		= TIME_CURRENT_UP_ENCO_PHASING;						   // Постоянная величина 500 мс.
      }
            
      // Разное   
      GlobalM4.FlagCommand.bit.ContactorErrEn = load_s16(ramPnt, progPnt->Pnt_ContErrEn); // Read the parameter of Contactor Error block. Send to PWM 
      // А вот теперь подгружаем в переменную фактическое значение тока удержания и времени подьема тока
      GlobalM4.pmsmAntirollback.UnbreakTorqPMSM         = locUnbreakTorqPMSM;
      GlobalM4.pmsmAntirollback.timeUI_Up	        = locTimeUI_Up;
      //*********************************************************************************************      
      if(!load_s16(ramPnt, progPnt->Pnt_FailTek)){ //если нет аварий       
//        if((testBitMicro(ramPnt, progPnt->Pnt_Stop_In) || (load_s16(ramPnt, progPnt->Pnt_StatusDRV) & DRVinStopMode)) && 
//           (!sPnt->algCommand.bit.stopCmd) && (sPnt->algState != liftStopped)){
        if(testBitMicro(ramPnt, progPnt->Pnt_Stop_In) || (load_s16(ramPnt, progPnt->Pnt_StatusDRV) & DRVinStopMode)) {
          // Check the normal stop action
          if ( (!sPnt->algCommand.bit.stopCmd) && (sPnt->algState != liftStopped) ) {
             sPnt->algCommand.bit.stopCmd=1;
          }                       
        } else if(testBitMicro(ramPnt, progPnt->Pnt_Pusk_In) && (!sPnt->algCommand.bit.stopCmd) && (load_s16(ramPnt, progPnt->Pnt_StatusDRV)==0)) {
          sPnt->algCommand.bit.runCmd=1;
        }
        
        if(sPnt->contSwOnTmr) 		sPnt->contSwOnTmr--;
        if(sPnt->contSwOffTmr) 		sPnt->contSwOffTmr--;
        
        if(sPnt->brakeHldOnTmr) 	sPnt->brakeHldOnTmr--;
        if(sPnt->brakeHldOffTmr) 	sPnt->brakeHldOffTmr--;
        if(sPnt->brakeBlockingTime)     sPnt->brakeBlockingTime--;
        if(sPnt->brakeOnDelay)		sPnt->brakeOnDelay--;
        if(sPnt->vPrUpBreakTmr)		sPnt->vPrUpBreakTmr--;
        if(sPnt->StopFixTime)		sPnt->StopFixTime--;
        if(sPnt->pretorqueDelayTime)    sPnt->pretorqueDelayTime--;
        if(sPnt->encoPhasingDelayTime)  sPnt->encoPhasingDelayTime--;
        GlobalM4.Pretorque.mPretorqueReset = 0;
      }
      else if(sPnt->algState != liftCMDFail){
        sPnt->algState	= liftCMDFail;
        sPnt->algCommand.bit.stopCmd=1;
        GlobalM4.Pretorque.mPretorqueReset = 1; // We get fail - reset pretorque value.
        GlobalM4.Pretorque.mPretorqueCatch = 0; // Заблокировать алгоритм восстановления предмомента
      }		//иначе переводим командоаппарат в "Аварийный" режим
      

      if(sPnt->algCommand.bit.stopCmd){		//Приоритет СТОПа выше, отрабатывается первым
        sPnt->algCommand.bit.runCmd = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Out);
        
        // Если текущая частота равна пусковой, и надо дать команду стоп, то необходимо создать дополнительную задержку на выключение, равную времени удержания в стопе.
        // Иначе если я стоял на нуле, и дать стоп - все срабатывает сразу, и ШИМ самым первым.
        
        StateGT = load_s16(ramPnt, progPnt->Pnt_StateGT);
        if ((StateGT & (RefZero|EquRV|Freeze)) && (sPnt->StopTrig == 0)) { // Если на момент останова ПЧ уже был на пусковой частоте, надо задержать команду стоп
          sPnt->StopFixTime = (u16)GlobalM4.FixStopTime;
          sPnt->StopTrig = 1;
        }
        if (!sPnt->StopFixTime)
          setBitMicro(ramPnt, progPnt->Pnt_Stop_Out);
        
        switch(sPnt->algState){
        case liftRunning:					//Тормоз снят, контактор включен, прозвонка окончена
          sPnt->algState--;
        case brakeControls:
          if(BrakeMode == brakeCtrlDis){ // If brake control disable - go nest step
            sPnt->algState--;
          }
          else if(testBitMicro(ramPnt, progPnt->Pnt_BrakeOff)){				// Если наложение тормоза не инициировано
            setBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);					//!!! Этот сигнал не используется //Выставляем задание снятия/наложения тормоза
            // контролируем частоту наложения тормоза,
            if((fabsf(load_float(ramPnt, progPnt->Pnt_Fout))) <= (load_float(ramPnt, progPnt->Pnt_FtormOff))){
              if(1) { 
                if(sPnt->brakeOnDelay == 1){
                  clrBitMicro(ramPnt, progPnt->Pnt_BrakeOff);				// инициируем наложение тормоза при снижении частоты
                  //запускаем выдержку времени на срабатывание тормоза
                  sPnt->brakeHldOnTmr = (u16)((0.01f*K_TIME_CALL_MICRO_TAKT)*load_s16(ramPnt, progPnt->Pnt_TimOutBrkFB)+ 0.5);
                }
                else if(sPnt->brakeOnDelay == 0){
                  //запускаем выдержку времени на наложение тормоза
                  sPnt->brakeOnDelay = (u16)((0.01f*K_TIME_CALL_MICRO_TAKT)*load_s16(ramPnt, progPnt->Pnt_DelayBrkOn)+0.5);
                }
              }
            }
          }
          else if(!sPnt->brakeHldOnTmr){
            clrBitMicro(ramPnt, progPnt->Pnt_HLD_ZR); //!!! Этот сигнал не используется
            sPnt->brakeBlockingTime = (u16)((0.01f*K_TIME_CALL_MICRO_TAKT)*load_s16(ramPnt, progPnt->Pnt_DelayBrkOff)+0.5);
            if(BrakeMode == brakeCtrlWithFdb && BrakeState){ // Если разрешена ОС по тормозу и любая из колодок не разомкнулась - авария
              setBitMicro(ramPnt, progPnt->Pnt_F_Brake);
              // Здесь заводить таймер на фиксацию ошибки не надо, она сразу попадет в фиксы, так как ШИМ будет включен.
              sPnt->failFixedTmr = failFixedBrakeTmr_MAX_TIME * K_TIME_CALL_MICRO_TAKT; // Заводим таймер для фиксации ошибки
              sPnt->algState=liftCMDFail;
            }
            else{
              sPnt->algState--;
            }
          }
          // Проверка состояния, если тормоза наложились до истечения времени (Здесь два тормоза должын откинуться сразу, поэтму нужен &&)
          else if(BrakeMode==brakeCtrlWithFdb && !BrakeState){
            sPnt->brakeHldOnTmr = 0;
          }
          break;
        case pretorqueSet:      // При остановке сразу переходим на следующий этап
          sPnt->algState--;
        case encoPhasingStart:  // При остановке сразу переходим на следующий этап
          sPnt->algState--;
        case waitPhirUp:        // При остановке сразу переходим на следующий этап
            sPnt->algState--;          
        case preBrakeBlock:     // При остановке сразу переходим на следующий этап       
          sPnt->algState--;
        case loadContyTest:	//Прозвонка активна, контактор включен, тормоз наложен, ШИМ включен
          if (GlobalM4.FlagCommand.bit.PwmOn == 0) { // Ждем пока отключиться ШИМ в DRV_V4
            if(testBitMicro(ramPnt, progPnt->Pnt_ProzvonOn)){
              clrBitMicro(ramPnt, progPnt->Pnt_ProzvonOn);
            }            
            // !!!! Не могу понять зачем эта часть - формально она не нужна, внизу уже есть защита.
            // !!!! Проверить работу в останове и убрать все.
            if (sPnt->contSwOffTmr == 1) {
              sPnt->algState--; // И когда таймер дотикал - переходим на этап отключения контактора
            } else if (!sPnt->contSwOffTmr) { // Как только ШИМ отключился, заводим таймер задержки отключения контактора
              sPnt->contSwOffTmr = 5; //(u16)(K_TIME_CALL_MICRO_TAKT * load_s16(ramPnt, progPnt->Pnt_DelayMotOn) * 0.001f + 0.5);
            }
          }
          break;
        case ackPwmOn: // Проверка - отключился ли ШИМ, и только потом отключим контактор.
          if (!GlobalM4.FlagCommand.bit.PwmOn && !GlobalM4.FlagCommand.bit.mWorkEnb)	sPnt->algState--;
          break;
        case startDRV: // При остановке сразу переходим на следующий этап
          sPnt->algState--;
          break;
        case contactorControls:						//Контактор включен, тормоз наложен, прозвонка выключена, ШИМ отключен
          if(ContactorMode == contactorCtrlDis){
            sPnt->algState = liftStopped;
          }
          else if(testBitMicro(ramPnt, progPnt->Pnt_ContactorOn)){
            if((fabsf(load_float(ramPnt, progPnt->Pnt_Fout))) == 0){
              if (ContactorMode == contactorCtrlWithFdb) { // Загружаем таймер ожидания ОС от контактора
                sPnt->contSwOffTmr = (u16)(K_TIME_CALL_MICRO_TAKT * load_s16(ramPnt, progPnt->Pnt_TimeoutCont) * 0.001f + 0.5); 
              } else { // Либо Фиксированная задержка при других режимах активации контактора
                sPnt->contSwOffTmr = 5; 
              }
              clrBitMicro(ramPnt, progPnt->Pnt_ContactorOn);
            }
          }
          else if( (ContactorMode == contactorCtrlWithFdb ) && (!testBitMicro(ramPnt, progPnt->Pnt_ContactorDin))){
            sPnt->algState = liftStopped;
          }
          else if(!sPnt->contSwOffTmr){
            if(ContactorMode == contactorCtrlWithoutFdb){
              sPnt->algState = liftStopped;
            }
            else if( (ContactorMode == contactorCtrlWithFdb) && (!testBitMicro(ramPnt, progPnt->Pnt_ContactorDin))){
              // Как-то не хорошо в этом месте проверять состояние ноги ОС контактора. Мы и так уже в шаге от останова. А если сигнал еще не снят - то мы упадем в аварию.
              sPnt->algState = liftStopped;
            } else {
              setBitMicro(ramPnt, progPnt->Pnt_ContNoFB);
              sPnt->failFixedTmr = failFixedContNoFBTmr_MAX_TIME * K_TIME_CALL_MICRO_TAKT;
              sPnt->algState = liftCMDFail;
            }
          }
          break;
        case liftStopped:			// Остановлен- контактор отключен, тормоз наложен, прозвонка выключена
//          if ( testBitMicro(ramPnt, progPnt->Pnt_Stop_In) && !tmp_StatusDRV.stopCmd) {
//
//          }          
          // Check main DRV state - if Stop, stop the lift finally
          if ( !tmp_StatusDRV.all ) {
              sPnt->algCommand.bit.stopCmd = 0;
              clrBitMicro(ramPnt, progPnt->Pnt_Stop_Out);
              sPnt->StopTrig = 0;
          }
        break;
        case liftCMDFail:			// Внутренняя или внешняя "Авария"
          if(testBitMicro(ramPnt, progPnt->Pnt_ContNoFB)){
            if(sPnt->failFixedTmr) 	{ sPnt->failFixedTmr--; 		        }
            else  			{ clrBitMicro(ramPnt, progPnt->Pnt_ContNoFB);   }
          }
          else if(testBitMicro(ramPnt, progPnt->Pnt_ContBrkFB)){
            if(sPnt->failFixedTmr) 	{ sPnt->failFixedTmr--; 			}
            else  			{ clrBitMicro(ramPnt, progPnt->Pnt_ContBrkFB);  }
          }
          else if(testBitMicro(ramPnt, progPnt->Pnt_F_Brake)){
            if(sPnt->failFixedTmr) 	{		sPnt->failFixedTmr--; 						}
            else  					{		clrBitMicro(ramPnt, progPnt->Pnt_F_Brake); 	}
          }
          else if(testBitMicro(ramPnt, progPnt->Pnt_F_VprUp)){ // Ошибка намагничивания АД
            if(sPnt->failFixedTmr) 	{		sPnt->failFixedTmr--; 						}
            else  					{		clrBitMicro(ramPnt, progPnt->Pnt_F_VprUp); 	}
          }
          else if(!load_s16(ramPnt, progPnt->Pnt_FailTek)){
            sPnt->algState = liftStopped;
          }
          sPnt->StopTrig = 0;
          //break;  не используется
        default:	break;
        }
      }
      /*
      * 1. Приходит сигнал направления движения
      * 2. Сразу после прихода направления - включается сигнал "Контактор"
      * 3. Ждем ОС от Контактора:
      *    а) если контроль по времени или просто управление контактором, то следующий этап сработает после времени заданного в параметре Pnt_DelayMotOn
      *    б) если задан режим ОС по контактам - Ждем до финального конца жизни, пока он не придет. А если пришел сразу - то сразу идем на след. уровень
      * 4. Через время блокировки Pnt_TholdBrake - разрешаем отпустить тормоз - это время должно полностью перекрыть время на тест тормозного резистора и прозвонку, иначе будет ерунда.
      *    Это хреновый вариант отпускания тормоза - надо завести ОС из DRV_V4, которая будет показывать что уже прошло время проверки тормозного резистора и прозвонки,
      *    и можно сразу отпускать тормоз или через время Pnt_TholdBrake, но никак не ранее отпускать тормоз.
      */
      else if(sPnt->algCommand.bit.runCmd){	// ПУСК отрабатывается только если отработал СТОП
        switch(sPnt->algState){
        case liftStopped: // Лифт еще в стопе
          sPnt->algState++; // Запуск процедуры работа лифта
          
        case contactorControls: //Управление контактором
          if(ContactorMode == contactorCtrlDis){
            sPnt->algState++;
          }
          else if(!testBitMicro(ramPnt, progPnt->Pnt_ContactorOn)){ // Если контактор еще не включен - включаем и заводим таймер задержки
            if (ContactorMode == contactorCtrlWithFdb) { // Загружаем таймер ожидания ОС от контактора
              sPnt->contSwOnTmr = (u16)(K_TIME_CALL_MICRO_TAKT * load_s16(ramPnt, progPnt->Pnt_TimeoutCont) * 0.001f + 0.5);  
            } else { 
              sPnt->contSwOnTmr = 5; 
            }// Либо Фиксированная задержка при других режимах активации контактора
            setBitMicro(ramPnt, progPnt->Pnt_ContactorOn);
          }
          else if((ContactorMode == contactorCtrlWithFdb) && (testBitMicro(ramPnt, progPnt->Pnt_ContactorDin))) {
            sPnt->algState++; // Если нормально пришла ОС по контактам для режимов контроля по времени или контактам, переходим на следующий уровень, вне зависимости от времени таймера
          }
          else if(!sPnt->contSwOnTmr){ // А если дотикал таймер, проверям доп. условия.
            if(ContactorMode == contactorCtrlWithoutFdb){
              sPnt->algState++; // Если просто разрешено управление контактром - переходим на след.этап после задержки.
            }
            else  if(testBitMicro(ramPnt, progPnt->Pnt_ContactorDin)){
              sPnt->algState++;
            }
            else if(ContactorMode == contactorCtrlWithFdb){  // Если для режима контроля по времени не пришла ОС - выдаем аварию
              setBitMicro(ramPnt, progPnt->Pnt_ContNoFB);
              sPnt->failFixedTmr = failFixedContNoFBTmr_MAX_TIME * K_TIME_CALL_MICRO_TAKT;
              sPnt->algState = liftCMDFail;
            }
          }
          break;
        case startDRV:
          // Здесь уже контактор должен быть включен, а значит даем команду пуска на инвертор
          GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable = 0; // Пока блокируем работу примагничения
          setBitMicro(ramPnt, progPnt->Pnt_Pusk_Out);	     	//Инициируем запуск инвертора
          sPnt->algState++;
          break;
        case ackPwmOn:
          // Если ШИМ завелся, переходим дальше, иначе ждем запуска
          if (GlobalM4.FlagCommand.bit.PwmOn)	sPnt->algState++;
          break;
        case loadContyTest:		// Идет прозвонка в работе ПЧ, а мы делаем блокировку на снятие тормоза. с Учетом времени прозвонки                                        
          if (load_s16(ramPnt, progPnt->Pnt_StatusDRV) & DRVinContyChk)       break; // While County check in progress - waiting
          else                                                                sPnt->algState++; // If over - go to the next step
          break;
          
        case preBrakeBlock: // Set Brake off & vPhirUp timeout
          sPnt->brakeBlockingTime = (u16)((0.01f*K_TIME_CALL_MICRO_TAKT)*load_s16(ramPnt, progPnt->Pnt_DelayBrkOff) + 0.5); //Set brake off timeout
          sPnt->vPrUpBreakTmr	= vPrUp_BREAK_TMR_MAX_TIME * K_TIME_CALL_MICRO_TAKT; // Заводим таймер выхода, если не пришел сигнал намагничивания. Включается для всех.                                   
          sPnt->algState++;
          break;
          
        case waitPhirUp:
          if(GlobalM4.FlagState.bit.vPrUp == 1) { // If vPrUp is set, go to the next step                          
              sPnt->algState++;          
          }
          break;
          
        case encoPhasingStart:
          // Проверка режима фазировки энкодера - если там ноль, то сразу идем дальше
          if (!EncoderPhasingWork) {                                    // Фазировка энкодера не включена?
            sPnt->algState++;                                           // Идем на следующий этапа работы preTorque
          } else {                                                      // А если активен режим фазирования, нужна задержка на время подъема тока удержания
            if (!sPnt->encoPhasingDelayTime) {                          // Счетчик пустой? Начинаем запуск процедуры                                                                                                    
              sPnt->encoPhasingDelayTime = (u16)(GlobalM4.pmsmAntirollback.timeUI_Up * 0.001f * K_TIME_CALL_MICRO_TAKT);// Заряжаем таймер времени плавного подъема тока в двигателе
              GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable = 1;     // Разрешаем работу алгоритма примагничения
              GlobalM4.FlagCommand.bit.RingUpFreezeGT = 1;              // Заморозить генератор темпа                            
            } else if (sPnt->encoPhasingDelayTime == 1) {               // Если счетчик фазировки досчитал
              if (!sPnt->brakeBlockingTime) {                           // Check Brake Off timeout - если ноль, завершаем работу здесь
                //GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable = 0; // Нельзя здесь отключать, у него свой алгоритм отключения в самом блоке
                GlobalM4.FlagCommand.bit.RingUpFreezeGT = 0;            // Отморозить генератор темпа
                sPnt->algState = brakeControls;                         // Пропускаем preTorque и идем на контроль тормоза
              } else {                                                  // А если счетчик блокировки тормоза еще считает,
                sPnt->encoPhasingDelayTime ++;                          // то мы принудительно увеличиваем счетчик фазировки вверх на 1, чтобы остаться здесь в ожидании
              }
            }
          }
          break;
          
        case pretorqueSet: // При пуске активируем режим создания предмомента - здесь уже DRV_V5 должен перейти в нормальную работу
          // Сюда надо интегрировать как компенсацию по аналоговому входу, так и по последнему значению момента в РС          
          GlobalM4.Pretorque.Enable = GlobalM4.Pretorque.Mode ? 1 : 0; // Разрешение работы в зависимости от выбранного режима.
          // Проверка первичного заряда таймера предмомента
          if (GlobalM4.Pretorque.Enable && !GlobalM4.FlagCommand.bit.EvacOn) { // Если разрешено и нет эвакуации - работаем
            if (!sPnt->pretorqueDelayTime) { // Счетчик пустой? Начинаем запуск процедуры              
              if (GlobalM4.Pretorque.mPretorqueCatch && (GlobalM4.Pretorque.Mode == PRETORQUEMODE_ACTIVEMODE)){
                GlobalM4.Pretorque.mPretorqueSet = 1; // Запускаем pretorqueSet если разрешен Активный режим и был захват
                GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable = 0;
              } else if (GlobalM4.Pretorque.Mode == PRETORQUEMODE_COMBINATION) { // Для Смешанного режима
                if (GlobalM4.Pretorque.mPretorqueCatch) { // Был захват
                  GlobalM4.Pretorque.mPretorqueSet = 1; // Запускаем pretorqueSet если был захват
                  GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable = 0; // Блокируем работу примагничения
                } else { //  иначе стартует примагничение
                  GlobalM4.Pretorque.mPretorqueSet = 0; // Запрещаем pretorqueSet
                  GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable = 1; // Разрешаем работу примагничения
                }
              }
              
              // Заряжаем таймер, он будет в любом случае работать - или предмомент или примагничение
              sPnt->pretorqueDelayTime = (u16)(GlobalM4.Pretorque.mPretorqueTime * K_TIME_CALL_MICRO_TAKT);
              GlobalM4.Pretorque.mPretorqueCatch = 1; // Ставим флаг, что Предмомент захвачен, и его можно будет использовать далее
              GlobalM4.FlagCommand.bit.RingUpFreezeGT = 1; // Заморозить генератор темпа                            
            } else if (sPnt->pretorqueDelayTime == 1) { // Если счетчик досчитал
              if (!sPnt->brakeBlockingTime) { // Check Brake Off timeout
                GlobalM4.Pretorque.mPretorqueSet = 0; // Сброс pretorqueSet
                //GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable = 0; // Нельзя здесь отключать, у него свой алгоритм отключения в самом блоке
                GlobalM4.FlagCommand.bit.RingUpFreezeGT = 0; // Отморозить генератор темпа
                sPnt->algState++;
              } else { // If brakeBlockingTime still count, wait in the pretorque State
                sPnt->pretorqueDelayTime ++; // Увеличиваем счетчик обратно на 1, чтобы ждать конца блокировки тормоза
              }
            }
          } else {
            GlobalM4.Pretorque.mPretorqueCatch = 1; // Ставим сигнал, что был захват предмомента. Сброшенное состояние либо после аварии либо при старте.
            sPnt->algState++;
          }
          break;
          
        case brakeControls: //Управление тормозом
          if(BrakeMode == brakeCtrlDis){ // If brake cntrl is OFF - go next   /*!load_s16(ramPnt, progPnt->Pnt_BrakeEnable)*/
            // setBitMicro(ramPnt, progPnt->Pnt_BrakeOff); // !!!   Set internal signal - Brake Off
            clrBitMicro(ramPnt, progPnt->Pnt_BrakeOff); // Disable brake - No Action
            sPnt->algState = liftRunning;
          } else if (sPnt->brakeBlockingTime){  // While brakeBlockingTime not Zero, skip Brake off
            break;                                          
          }
          else if(!testBitMicro(ramPnt, progPnt->Pnt_BrakeOff)){  //Если снятие тормоза не инициировано
            //setBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);			//!!! Этот сигнал не используется //Выставляем задание снятия/наложения тормоза
            // контролируем частоту снятия тормоза,
            if(fabsf(load_float(ramPnt, progPnt->Pnt_Fout)) >= load_float(ramPnt, progPnt->Pnt_FtormOn)){
              //контролируем текущий ток двигателя,
              if(load_float(ramPnt, progPnt->Pnt_Ifull) >= (load_float(ramPnt, progPnt->Pnt_Inom) * load_float(ramPnt, progPnt->Pnt_IoffBrake))){ // !!! По идее лучше заменить Ifull на активный Iact
                if(!sPnt->brakeBlockingTime){
                  if ((load_s16(ramPnt, progPnt->Pnt_StatusDRV) & DRVinRunMode)) {
                    // Если состояние командоаппарата DRV_V4 перешло в Running, значит все проверки окончены, можно разрешать снимать тормоз.
                    if(((TYPEDRV_ACIMFOC&(1<<GlobalM4.DriveType)) == 0)||(GlobalM4.FlagState.bit.vPrUp == 1)) { // Если режим ВУ АД и достигнут поток, можно снимать тормоз. Либо тормоз снимаем если не ВУ АД.
                      setBitMicro(ramPnt, progPnt->Pnt_BrakeOff);				// инициируем снятие тормоза при превышении током уставки
                    } else {
                      sPnt->brakeBlockingTime++; // Если поток еще не нарос, то добавляем тик на счетчик и ждем намагничивания.
                    }
                  }
                  else {
                    sPnt->brakeBlockingTime++; // Таймер дотикал а мы еще не перешли по командоаппарату в Running, надо продолжить ожидание
                  }
                  //запускаем выдержку времени на снятие тормоза - время получение ОС по входу. Реально сигнал снятия уже подан.
                  sPnt->brakeHldOffTmr = (u16)((0.01f*K_TIME_CALL_MICRO_TAKT) * load_s16(ramPnt, progPnt->Pnt_TimOutBrkFB)+0.5);
                }
              }
            }
          }
          else if(!sPnt->brakeHldOffTmr){
            clrBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);   //!!! Этот сигнал не используется
            clrBitMicro(ramPnt, progPnt->Pnt_ProzvonOn);	// синхронизируем конец прозвонки со снятием тормоза
            if((BrakeMode == brakeCtrlWithFdb && !BrakeState)) {   // Проверка режима с Fdb
                 // Если тормоз не откинулся - авария
                 setBitMicro(ramPnt, progPnt->Pnt_F_Brake);
                 sPnt->failFixedTmr = failFixedBrakeTmr_MAX_TIME * K_TIME_CALL_MICRO_TAKT; // Заводим таймер для фиксации ошибки, так как при выключенном ШИМе мы её можем не увидеть никогда на экране.
                 sPnt->algState=liftCMDFail;
               }
            else{
              sPnt->algState=liftRunning;
            }
          }
          // Проверка - откинулись ли тормоза
          else if( (BrakeMode == brakeCtrlWithFdb && BrakeState) ) {   // Проверка режима с Fdb                  
                    sPnt->brakeHldOffTmr=0;
                  }
          break;
                    
        case liftRunning:					//Запущен
          if(!testBitMicro(ramPnt, progPnt->Pnt_Pusk_Out)){
            setBitMicro(ramPnt, progPnt->Pnt_Pusk_Out);
          } else if (EncoderPhasingWork) {
            if (EncoderPhasingDone) {
              sPnt->algCommand.bit.stopCmd = 1; // Как только фазировка закончена, даем стоп.
            }
          }
          break;
          
        case liftCMDFail:					//Внутренняя или внешняя "Авария"
          break;
        }
      }
      //	else{							//Действия при отсутствии сигналов СТОП/ПУСК
      //									//Вдруг понадобиться
      //	}
      
      // Отработаем исключительные ситуации
      //!!! Goncharov 07.10.2016
      //Контроль состояния контактора после включения, для случаев когда включена ОС
      if(ContactorMode == contactorCtrlWithFdb){
        if(sPnt->algState != liftCMDFail){
          if((sPnt->algState > contactorControls)&&(testBitMicro(ramPnt, progPnt->Pnt_ContactorDin)==0)){
            setBitMicro(ramPnt, progPnt->Pnt_ContBrkFB); // SET FAULT BREAK FEEDBACK
            sPnt->failFixedTmr = failFixedContactorTmr_MAX_TIME * K_TIME_CALL_MICRO_TAKT;
            sPnt->algState=liftCMDFail;
          }
        }
      }
      
      // If PWM off and Brake release -  its a bad situation. Cabin can drop down
      if((GlobalM4.FlagCommand.bit.PwmOn == 0) && testBitMicro(ramPnt, progPnt->Pnt_BrakeOff) && BrakeMode != brakeCtrlDis) {
          sPnt->algState = liftCMDFail;
          // Set Fault - Brake release, but PWM is off.
          setBitMicro(ramPnt, progPnt->Pnt_F_LiftBrkwoPWM);
      }
      
      // Если дотикал таймер vPrUpBreakTmr, у нас управление АД, и при этом мы в стадии >= waitPhirUp при пуске лифта, то значит возбуждение АД не получено.
      // Выдаем аварию
      if (!sPnt->vPrUpBreakTmr && (sPnt->algState >= waitPhirUp) && sPnt->algCommand.bit.runCmd && ((TYPEDRV_ACIMFOC&(1<<GlobalM4.DriveType)) == 1)) {
        sPnt->algState=liftCMDFail;
        setBitMicro(ramPnt, progPnt->Pnt_F_VprUp); // Выдаем сигнал аварии
        sPnt->failFixedTmr = failFixedVprUpTmr_MAX_TIME * K_TIME_CALL_MICRO_TAKT; // Заводим таймер для фиксации ошибки
      }
      
      if((sPnt->algState == liftCMDFail)||(load_s16(ramPnt, progPnt->Pnt_FailTek))){
        setBitMicro(ramPnt, progPnt->Pnt_Stop_Out);
        clrBitMicro(ramPnt, progPnt->Pnt_Pusk_Out);
        // алгоритм в состояние останов
        //sPnt->algState			 = liftStopped; //!!! Не трогаем алгстейт чтобы отключение аппаратуры выполнилось по коммандоаппарату
        sPnt->algCommand.bit.stopCmd = 1;
        sPnt->algCommand.bit.runCmd  = 0;
        // инициируем наложение тормоза
        clrBitMicro(ramPnt, progPnt->Pnt_BrakeOff);
        clrBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);   //!!! Этот сигнал не используется
        sPnt->brakeHldOnTmr     = 0;
        sPnt->brakeHldOffTmr    = 0;
        sPnt->brakeBlockingTime	= 0;
        sPnt->brakeOnDelay		= 0;
        sPnt->vPrUpBreakTmr		= 0;
        // инициируем отключение контактора и прозвонки
        clrBitMicro(ramPnt, progPnt->Pnt_ProzvonOn);
        clrBitMicro(ramPnt, progPnt->Pnt_ContactorOn);
        sPnt->contSwOnTmr  = 0;
        sPnt->contSwOffTmr = 0;
      }
      
      //Управляем светодиодом Работа
      if(GlobalM4.FlagCommand.bit.PwmOn) setBitMicro(ramPnt, progPnt->Pnt_WorkFlag);
      else                             clrBitMicro(ramPnt, progPnt->Pnt_WorkFlag);
    }

    GlobalM4.FlagCommand.bit.BreakEnable = load_s16(ramPnt, progPnt->Pnt_BrakeEnable);
    // Тестовые сигналы управления с выводом
    GlobalM4.s16_Brake = GlobalM4.FlagCommand.bit.BreakOff = (u16)(testBitMicro(ramPnt, progPnt->Pnt_BrakeOff)); // Сигнал тормоза
    GlobalM4.s16_Contactor = (u16)(testBitMicro(ramPnt, progPnt->Pnt_ContactorOn)); // Управление контактором
    GlobalM4.s16_PwmOnM3 = (u16)(GlobalM4.FlagCommand.bit.PwmOn); // Управление ШИМом
    GlobalM4.s16_AlgState = (u16)(sPnt->algState); // Состояние командоаппарата лифта
    GlobalM4.s16_StartStop = (u16)(sPnt->algCommand.value);
    GlobalM4.s16_LiftPuskOut = (u16)(testBitMicro(ramPnt, progPnt->Pnt_Pusk_Out));
    GlobalM4.s16_VprUpState = GlobalM4.FlagState.bit.vPrUp;
    GlobalM4.s16_F_Ref = (s16)(GlobalM4.Fref*100.0f);
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLIFT_CMD_V3_1[47]={
	BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Pnt_Pusk_In
	BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Pnt_Stop_In
	S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Pnt_FailTek
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Pnt_ContMode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Pnt_TimeoutCont
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Pnt_ContactorDin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Pnt_ContErrEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Pnt_PretorqEnable
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Pnt_PretorqTime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Pnt_BrakeEnable
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Pnt_BrakeMode
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  Pnt_IoffBrake
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  Pnt_FtormOn
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  Pnt_FtormOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Pnt_DelayBrkOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Pnt_DelayBrkOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Pnt_TimOutBrkFB  
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //18  Pnt_DinBrake
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19  Pnt_Ifull
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  Pnt_Inom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Pnt_StatusDRV	
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22  Pnt_Fout 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Pnt_StateGT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //24  Pnt_StopFreq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Pnt_UnbkrMode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  Pnt_UnbkrCurr        
        S32_PIN_TYPE | INPUT_PIN_MODE,     //27  Pnt_UnbkrFreset        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Pnt_UnbkrTfix                
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //29  Pnt_EncPhasWork        
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //30  Pnt_EncPhasDone      
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  Pnt_EncPhasCurr                
        S32_PIN_TYPE | INPUT_PIN_MODE,     //32  Pnt_LiftMaxTorq                        
	S16_PIN_TYPE | INPUT_PIN_MODE,     //33  FunctionLft
        
	S32_PIN_TYPE | OUTPUT_PIN_MODE,    //34  Pnt_MaxTorqOut  
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //35  Fbrake  
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //36  F_Brake
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //37  F_LiftBrkwoPWM
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //38  HLD_ZR
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //39  BrakeOff
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //40  ProzvonOn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //41  ContNoFB
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //42  ContBrkFB
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //43  ContactorOn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //44  HoldGT !!!!*****НЕ ИСПОЛЬЗУЕТСЯ*****!!!!
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //45  WorkFlag
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //46  Stop_Out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //47  Pusk_Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LIFT_CMD_V3", "DD", TblFuncLIFT_CMD_V3_1, FuncLIFT_CMD_V3_1, sizeof(TblFuncLIFT_CMD_V3_1), sizeof(LIFTCMD_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLIFT_CMD_V3_1,
#endif
//********************************************************
