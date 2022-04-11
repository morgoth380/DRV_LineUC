#ifdef    GET_FUNC_CODE

#include    <math.h>
#include    "stdlib.h"
#include    "Pjk_const.h"

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PhasingEn;                    //1  PhasingEn
    u16  Pnt_PhasNomSpd;                   //2  PhasNomSpd
    u16  Pnt_ContactorOn;                  //3  ContactorOn
    u16  Pnt_PwmOn;                        //4  PwmOn
    u16  Pnt_FailTek;                      //5  FailTek
    u16  Pnt_Fout;                         //6  Fout
    u16  Pnt_Fenco;                        //7  Fenco
    u16  Pnt_PhaseTek;                     //8  PhaseTek
	u16  Pnt_EncoPhasing;                  //9  EncoPhasing
	u16  Pnt_EncoIncVis;                   //10  EncoIncVis
	u16  Pnt_AnyEncoVis;                   //11  AnyEncoVis
    u16  Pnt_Functional;                   //12  Functional
    u16  Pnt_PhasingWork;                  //13  PhasingWork
    u16  Pnt_PhasingSpd;                   //14  PhasingSpd
    u16  Pnt_PhasingDone;                  //15  PhasingDone
    u16  Pnt_F_Rtimeout;                   //16  F_Rtimeout - Таймаут нахождения R метки
    u16  Pnt_F_Speed;                      //17  F_Speed - Ошибка скорости фазирования энкодера
    u16  Pnt_F_Abort;                      //18  F_Abort
    u16  Pnt_F_LiftDrop;                   //19  F_LiftDrop
    u16  Pnt_End;
}FuncENCO_PHASING_BLOCK_type;

typedef enum{
	AUTOPHASING_ON_WAIT      = 0,
	PWM_ON_WAIT              = 1,
	START_ZERO_REF_HOLD_WAIT = 2,
	LOW_LEWEL_PHASING_WAIT   = 3,
	SPD_SIGN_PHASING         = 4,
	REF_TO_ZERO_SET          = 5,
	PHASE_SHIFT_RESET        = 6,
	ZERO_SPD_WAIT            = 7,
	PHASE_CALC               = 8,
	REF_SET2                 = 9,
	REACH_SPD_WAIT           = 10,
	ZERO_REF_SET2            = 11,
	ZERO_SPD_WAIT2           = 12,
	PHASE_CALC2              = 13,
//	PHASING_OFF              = 14,
	AUTOPHASING_ERR          = 14,
	PWM_OFF_WAIT             = 15,
	RESTART_AUTOPHASING		 = 16,
    ENCO_PHASING_DONE        = 17,
}phasingStateType;

typedef struct{
    phasingStateType phasingState;  
//	  _iq minSpdPhasingVal;
	  u16 zeroHoldStateCnt1;
	  u16 zeroHoldStateCnt2;
	  u16 state1Cnt;
	  u16 state2Cnt;
	  u16 state3Cnt;
//	  u16 measTime;
	  u16 waitSpdToRefCnt;
	  u16 waitReachSpdCnt;
//	  u16 autoPhasingEnState;
	  u16 waitStartZeroRefHoldCnt;	  
	  s16 firstPhaseShift;
//	  s16 prevRefChanSetting;
//	  _iq prevLocalRefSetting;
//	  u16 prevFreqUnit;
//	  u16 waitRSignalCnt;
	  u16 errHoldCnt;
//	  u32 spdSignPhasingWaitCnt;
	  f32 currentRotateAngle; //текущий угол проворота при контроле слишком долгого ожидания выхода на скорость при фазировании по знаку
	  u16 zeroSpdWaitCnt;
	  u16 reachSpdTimeOutCnt;
	  u16 zeroSpdWaitTimeOutCnt2;
}AUTOPHASING_State_type;

#define PHASING_PARAM_ADDR	(3*128+5)	// Группа 03, параметр 05. Энкодер. Фазировка
#define PHASE_SHIFT_ADDR	(3*128+6)	// Группа 03, параметр 06. Энкодер. Смещение фазы. 
//#define REF_PARAM_ADDR     905
//#define FREQ_UNIT_ADDR     384  //единицы измерения частоты
//#define PHASING_ON_ADDR    265  //адрес параметра включения автофазировки
//#define SPD_REF_VAL        20
#define SPD_STATE_CNT			(1 * K_TIME_CALL_MICRO_TAKT)
#define SPD_MEAS_TIME			(2 * K_TIME_CALL_MICRO_TAKT) //время подтверждения знаков скорости - 2 сек
#define ERR_HOLD_TIME			(5 * K_TIME_CALL_MICRO_TAKT)
#define WAIT_SPD_REACH_CNT		(11* K_TIME_CALL_MICRO_TAKT) // Максимальное время + 1 секунда для времени разгона к скорости фазирования
#define WAIT_ZERO_SPD_REACH_CNT (7 * K_TIME_CALL_MICRO_TAKT)
#define SPD_REACH_TIMEOUT_CNT	(7 * K_TIME_CALL_MICRO_TAKT)

#define MIN_SPD_PHASING_VAL_HZ		1.0f       // минимальная скорость фазирования, Гц. Установлена в системе параметров
#define DELTA_ENCO_SPD_HZ			0.3f       // допустимое отклонение скорости энкодера от задния, Гц
#define MIN_ZERO_SPD_PHASING_VAL_HZ	0.01f      // Зазор по нулевой скорости при фазировке.


s16 phaseShiftCalc(f32 ipark1Angle, f32 encoPos);
u16 writeMicroProgParam(u16 addr, s32 prmVal);

#define _5PI           15.7075f   // (PI * 5.0f)     

#define   AUTOPHASING_OFF       0
#define   AUTOPHASING_1_STEP    1
#define   AUTOPHASING_2_STEP    2

//********************************************************
u16 *FuncENCO_PHASING_BLOCK_1(FuncENCO_PHASING_BLOCK_type *progPnt, u32 ramPnt)
{
    AUTOPHASING_State_type	*sPnt;
    u16         PWMOn, ContactorOn, LiftBreakOff, failNum, autoPhasingEn;
    f32         Fout, Fenco, encoSpd, encoPhaseDeg, encoPhaseRad, vfdPhaseRad;
    f32         refSpd, mechSpd;
    u16         startZeroRefHoldTime, polPairsNum, fail;
    u16         phaseParam, IncrementalEncoExist, AnyEncoExist;
    s16         shiftVal, phaseShift;
	f32			minSpdPhasingVal, deltaEncoSpd, minZeroSpdPhasingVal;

    sPnt = (AUTOPHASING_State_type *)(ramPnt + progPnt->Pnt_State);
        
  if(flagIsFirstCall){
    sPnt->phasingState = AUTOPHASING_ON_WAIT;    
    clrBitMicro(ramPnt, progPnt->Pnt_PhasingWork);
    clrBitMicro(ramPnt, progPnt->Pnt_PhasingDone);
    clrBitMicro(ramPnt, progPnt->Pnt_F_Rtimeout);
    clrBitMicro(ramPnt, progPnt->Pnt_F_Speed);
    clrBitMicro(ramPnt, progPnt->Pnt_F_Abort);
    clrBitMicro(ramPnt, progPnt->Pnt_F_LiftDrop);    
    return &progPnt->Pnt_End;
  }  
  
  
  if((AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_Functional) == UseLiftFunc){
       
    PWMOn = testBitMicro(ramPnt, progPnt->Pnt_PwmOn);
    ContactorOn = testBitMicro(ramPnt, progPnt->Pnt_ContactorOn);
    // Добавить вход от тормоза
    LiftBreakOff = GlobalM4.FlagCommand.bit.BreakOff;
    failNum = load_s16(ramPnt, progPnt->Pnt_FailTek);
    
    autoPhasingEn = load_s16(ramPnt, progPnt->Pnt_PhasingEn);
    
    refSpd				= load_float(ramPnt, progPnt->Pnt_PhasNomSpd);		// Скорость задания при фазировании
    Fout				= load_float(ramPnt, progPnt->Pnt_Fout);			// Скорость с выхода генератора темпа
    encoSpd = Fenco		= load_float(ramPnt, progPnt->Pnt_Fenco);			// Скорость с блока энкодера
    encoPhaseDeg		= load_s16(ramPnt, progPnt->Pnt_PhaseTek);			// Фаза энкодера в градусах
    encoPhaseRad		= encoPhaseDeg * _DEG_TO_RAD * 0.10f;				// перевод в радианы
    vfdPhaseRad			= GlobalM4.Angle;									// В радианах угол привода (из такта ШИМа)
    polPairsNum			= GlobalM4.EnginParam.Zp;							// ЧИсло пар полюсов для расчета мех. скорости
       	
	minSpdPhasingVal	= FLOAT_to_IQ (MIN_SPD_PHASING_VAL_HZ, Herz_base);	// Пересчет минимальной скорости движения в о.е
	deltaEncoSpd		= FLOAT_to_IQ (DELTA_ENCO_SPD_HZ, Herz_base);		// Пересчет отклонения по скорости в о.е
	minZeroSpdPhasingVal= FLOAT_to_IQ (MIN_ZERO_SPD_PHASING_VAL_HZ, Herz_base);// Пересчет отклонения минимальной нулевой скорости в о.е
	
    // Тестовые параметры!!!
    startZeroRefHoldTime= 3*K_TIME_CALL_MICRO_TAKT; // Пока на 3 секунд зависнуть и поймать кабину. //load_s16(ramPnt, progPnt->Pnt_startHdTime) * K_TIME_CALL_MICRO_TAKT / 10;  //Время удержания на пусковой частоте
    phaseParam			= load_s16(ramPnt, progPnt->Pnt_EncoPhasing);//AllPrmPnt->ENCO_PHASING; // Текущее значение фазировки энкодера - прямая или инверсная.       
    IncrementalEncoExist= load_s16(ramPnt, progPnt->Pnt_EncoIncVis); //AllPrmPnt->EXT_ENCO_INC_VISIBLE; // Проверяем, что подключен инкремент
    AnyEncoExist		= load_s16(ramPnt, progPnt->Pnt_AnyEncoVis); //AllPrmPnt->EXT_ANY_ENCO_VISIBLE; // Проверяем, что подключен любой энкодер

    // Если нету блока энкодера - то вызывать модуль не сможем
    if (!AnyEncoExist)
      return &progPnt->Pnt_End;
    
    //--------------------------------------------------- 
    // Проверка аварийных ситуаций и внешнее отключение ШИМа
    if ( (sPnt->phasingState >= START_ZERO_REF_HOLD_WAIT) && (sPnt->phasingState < AUTOPHASING_ERR) ) {
      if( PWMOn == 0 && !failNum){			  // Прерывание автофазировки  - отключился ШИМ и не было аварии       
        sPnt->phasingState = AUTOPHASING_ERR;               // Переход в состояние удержания аварии
        setBitMicro(ramPnt, progPnt->Pnt_F_Abort);          // Авария - Фазировка прервана
      } else if ( failNum ) {				  // ШИМ отключился по причине Аварии
        sPnt->phasingState = PWM_OFF_WAIT;
      }
    }
    //--------------------------------------------------- 

    switch(sPnt->phasingState){
    case AUTOPHASING_ON_WAIT:                              //ожидание включения параметра автофазирования
      // сбросить активность режимов и аварий
      clrBitMicro(ramPnt, progPnt->Pnt_PhasingWork);
      clrBitMicro(ramPnt, progPnt->Pnt_PhasingDone);
      clrBitMicro(ramPnt, progPnt->Pnt_F_Rtimeout);
      clrBitMicro(ramPnt, progPnt->Pnt_F_Speed);
      clrBitMicro(ramPnt, progPnt->Pnt_F_Abort);
      clrBitMicro(ramPnt, progPnt->Pnt_F_LiftDrop);
      
      if(autoPhasingEn){                                 //если автофазирование включено, переход к ожиданию запуска лифта
        sPnt->phasingState = PWM_ON_WAIT;                // Переход к ожиданию включения ШИМ
      }
      break;

    //--------------------------------------
    case PWM_ON_WAIT: //ожидание включения ШИМ
      sPnt->currentRotateAngle = 0;
      sPnt->state1Cnt = 0;
      sPnt->state2Cnt = 0;
      sPnt->state3Cnt = 0;
      sPnt->waitSpdToRefCnt = 0;
      sPnt->waitStartZeroRefHoldCnt = 0;
      sPnt->errHoldCnt = 0;
      sPnt->zeroSpdWaitCnt = 0;
      sPnt->zeroSpdWaitTimeOutCnt2 = 0;
      sPnt->zeroHoldStateCnt1 = 0;
      sPnt->zeroHoldStateCnt2 = 0;
      sPnt->waitReachSpdCnt = 0;
      sPnt->firstPhaseShift = 0;
      sPnt->reachSpdTimeOutCnt = 0;

      setBitMicro(ramPnt, progPnt->Pnt_PhasingWork); // Сигнализируем, что идет фазирование энкодера

      sPnt->phasingState = (PWMOn == 1 ) ? START_ZERO_REF_HOLD_WAIT : sPnt->phasingState; //ШИМ включен - переход  к работе
      sPnt->phasingState = (autoPhasingEn == AUTOPHASING_OFF) ? PWM_OFF_WAIT : sPnt->phasingState; //если автофазирование выключено - переход к возврату параметров
      sPnt->phasingState = (failNum != 0) ? PWM_OFF_WAIT : sPnt->phasingState;
      break;

    //--------------------------------------
    case START_ZERO_REF_HOLD_WAIT: //состояние ожидания завершения удержания на нуле при старте
      save_float (ramPnt, progPnt->Pnt_PhasingSpd, 0.0f); // Ставим нулевое задание скорости      
      
      if (LiftBreakOff) {  // Если тормоз сняли, начинаем работать        
        // ВОТ ЗДЕСЬ ПРОВЕРЯЮ НА ПАДЕНИЕ КАБИНЫ - ДОПОЛНИТЕЛЬНАЯ ПРОВЕРКА, ЕСЛИ Я СТОЮ НЕ НА НУЛЕ, ТО ЗНАЧИТ КАБИНА ПАДАЕТ И Я ЕЕ НЕ ДЕРЖУ
        if((fabsf(Fenco) >= 0) && (fabsf(Fenco) < minZeroSpdPhasingVal)){
          sPnt->zeroHoldStateCnt1++;
          if(sPnt->zeroHoldStateCnt1 >= startZeroRefHoldTime){ //если стабильно удерживали на нуле
            sPnt->zeroHoldStateCnt1 = 0;
            sPnt->phasingState = LOW_LEWEL_PHASING_WAIT;
          }
        } else {
          sPnt->zeroHoldStateCnt1       = (sPnt->zeroHoldStateCnt1 != 0)        ? (--sPnt->zeroHoldStateCnt1)           : sPnt->zeroHoldStateCnt1;
          
          sPnt->waitStartZeroRefHoldCnt++;
          fail = (sPnt->waitStartZeroRefHoldCnt >= startZeroRefHoldTime) ? 1 : 0;	
          defBitMicro(ramPnt , progPnt->Pnt_F_LiftDrop, fail);								// ставим бит - авария падения кабины
          sPnt->phasingState = (fail != 0) ? AUTOPHASING_ERR : sPnt->phasingState;					// Переход в состояние авария          
        }
      }
      
//      if( PWMOn == 0 && !failNum){ // Прерывание автофазировки  - отключился ШИМ и не было аварии       
//        sPnt->phasingState = AUTOPHASING_ERR;                   // Переход в состояние удержания аварии
//        setBitMicro(ramPnt, progPnt->Pnt_F_Abort);          // Авария - Фазировка прервана
//      } else if ( failNum ) { //ШИМ отключился по причине Аварии
//        sPnt->phasingState = PWM_OFF_WAIT;
//      }      
      break;

    //--------------------------------------
    case LOW_LEWEL_PHASING_WAIT: //Ожидание завершения низкоуровневой фазировки обработчика энкодеров
      // Проверяем флаг от энкодера - что фазировка по R сигналу завершена.             
      sPnt->phasingState = (GlobalM4.EncoderCalcParam.header.bits.Rsygnal == 1) ? SPD_SIGN_PHASING : sPnt->phasingState;
      save_float (ramPnt, progPnt->Pnt_PhasingSpd, refSpd); // Ставим задание скорости движения
            
//      if( PWMOn == 0 && !failNum){																// Прерывание автофазировки  - отключился ШИМ и не было аварии       
//        sPnt->phasingState = AUTOPHASING_ERR;													// Переход в состояние удержания аварии
//        setBitMicro(ramPnt, progPnt->Pnt_F_Abort);												// Авария - Фазировка прервана
//      } else if ( failNum ) {																	// ШИМ отключился по причине Аварии
//        sPnt->phasingState = PWM_OFF_WAIT;
//      }
       
      mechSpd = IQ_to_FLOAT((encoSpd / polPairsNum), Herz_base);								// предпологаемая механическая частота вращения ротора в Гц, по данным энкодера, а не генератора темпа
      sPnt->currentRotateAngle = sPnt->currentRotateAngle + (mechSpd * FLOAT_TIME_CALL_MICRO);  // Интегрируем скорость и находим суммарную механическую фазу

      fail = (fabsf(sPnt->currentRotateAngle) >= _5PI) ? 1 : 0;									// более 2,5 оборота - авария - не дождались R-сигнал
      defBitMicro(ramPnt , progPnt->Pnt_F_Rtimeout, fail);										// ставим бит - авария таймаута ожидания R метки
      sPnt->phasingState = (fail != 0) ? AUTOPHASING_ERR : sPnt->phasingState;					// Переход в состояние авария
      break;

    //-------------------------------------- 
    case SPD_SIGN_PHASING: //фазирование по знаку скорости
	 // Делаем проверку текущей скорости и ее отклонения. Если я попадаю в диапазон отклонения, то мне даже не обязательно выходить на скорость задания
	 // А этап можно завершить раньше 
     if ((fabsf(Fout) >= minSpdPhasingVal) && (fabsf(encoSpd) >= (fabsf(Fout) - deltaEncoSpd)) && (fabsf(encoSpd) <= (fabsf(Fout) + deltaEncoSpd))){ //фазирование - поcле формирования минимального задания и корректной скорости энкодера
        sPnt->waitSpdToRefCnt = (sPnt->waitSpdToRefCnt != 0) ? (sPnt->waitSpdToRefCnt - 1) : sPnt->waitSpdToRefCnt;
        if( (Fout > 0) && (encoSpd < 0) ){ //если задание и скорость энкодера противоположны по знаку
          sPnt->state2Cnt = (sPnt->state2Cnt != 0) ? (sPnt->state2Cnt - 1) : sPnt->state2Cnt;
          sPnt->state3Cnt = (sPnt->state3Cnt != 0) ? (sPnt->state3Cnt - 1) : sPnt->state3Cnt;
          ++sPnt->state1Cnt; //счетчик состояния "Задание и скорость энкодера противоположны по знаку"
          if(sPnt->state1Cnt == SPD_MEAS_TIME){ //от 1 Гц до задания 2 гц остается половина времени уставки разгона
            phaseParam = !phaseParam; //Меняем коэффициент фазировки на противоположный
            //AllPrmPnt->ENCO_PHASING = phaseParam; // !!!!
            writeMicroProgParam(PHASING_PARAM_ADDR, phaseParam);
          }
        }else if ( (Fout < 0) && (encoSpd > 0) ){ //если задание и скорость энкодера противоположны по знаку
          sPnt->state1Cnt = (sPnt->state1Cnt != 0) ? (sPnt->state1Cnt - 1) : sPnt->state1Cnt;
          sPnt->state3Cnt = (sPnt->state3Cnt != 0) ? (sPnt->state3Cnt - 1) : sPnt->state3Cnt;
          ++sPnt->state2Cnt; //счетчик состояния "Задание и скорость энкодера противоположны по знаку"
          if(sPnt->state2Cnt == SPD_MEAS_TIME){
            phaseParam = !phaseParam; //Меняем коэффициент фазировки на противоположный
            //AllPrmPnt->ENCO_PHASING = phaseParam; // !!!!
            writeMicroProgParam(PHASING_PARAM_ADDR, phaseParam);
          }
        }else{ //состояние одинаковых знаков
          sPnt->state1Cnt = (sPnt->state1Cnt != 0) ? (sPnt->state1Cnt - 1) : sPnt->state1Cnt;
          sPnt->state2Cnt = (sPnt->state2Cnt != 0) ? (sPnt->state2Cnt - 1) : sPnt->state2Cnt;
          ++sPnt->state3Cnt;                                                            //счетчик состояния "Задание и скорость энкодера совпадают по знаку"
          sPnt->phasingState = (sPnt->state3Cnt == SPD_MEAS_TIME) ? REF_TO_ZERO_SET : sPnt->phasingState; //если есть стабильное движение с устаявшимися знаками - переход в фазировку по положению
        }
      } else if(fabsf(Fout) >= minSpdPhasingVal){ //если задание сформировано, но скорость энкодера вне допустимого диапазона
        sPnt->waitSpdToRefCnt++; //счетчик ожидания выхода на задание
        if(sPnt->waitSpdToRefCnt == WAIT_SPD_REACH_CNT){
          sPnt->waitSpdToRefCnt = 0;
          sPnt->phasingState = AUTOPHASING_ERR;      //Переход в состояние удержания аварии
		  setBitMicro(ramPnt, progPnt->Pnt_F_Speed); //Авария ошибка скорости фазирования энкодера. По какой-то причине я не еду на нужной скорости!!! Но это не падение кабины
        }
        sPnt->state1Cnt = 0;
        sPnt->state2Cnt = 0;
      }
      
//      if( PWMOn == 0 && !failNum){ // Прерывание автофазировки  - отключился ШИМ и не было аварии       
//        sPnt->phasingState = AUTOPHASING_ERR;                   // Переход в состояние удержания аварии
//        setBitMicro(ramPnt, progPnt->Pnt_F_Abort);          // Авария - Фазировка прервана
//      } else if ( failNum ) { //ШИМ отключился по причине Аварии
//        sPnt->phasingState = PWM_OFF_WAIT;
//      }      
      break;

     //-------------------------------------- 
    case REF_TO_ZERO_SET:        //Состояние установки задания в 0
      save_float (ramPnt, progPnt->Pnt_PhasingSpd, 0); // Ставим задание скорости движения
      sPnt->phasingState = PHASE_SHIFT_RESET;
      break;

      //-------------------------------------- 
    case PHASE_SHIFT_RESET: //Состояние установки смещения в 0
      //AllPrmPnt->ENCO_ANGLE_SHIFT = 0; // !!!!!
	  shiftVal = 0;
	  writeMicroProgParam(PHASE_SHIFT_ADDR, shiftVal);
      sPnt->phasingState = ZERO_SPD_WAIT;
      sPnt->zeroHoldStateCnt1 = 0; // Сбросить счетчик удержания нуля
      break;

      //-------------------------------------- 
    case ZERO_SPD_WAIT: //ожидание останова и удержания на нуле
      // !!! А если по какой то причине я не выйду на ноль, здесь я не вижу перехода в аварию!!!
      if((fabsf(Fout) >= 0) && (fabsf(Fout) < minZeroSpdPhasingVal)){
        sPnt->zeroHoldStateCnt1++;
        if(sPnt->zeroHoldStateCnt1 >= K_TIME_CALL_MICRO_TAKT){ //если стабильно удерживали на нуле в течении 1 сек
          sPnt->phasingState = (IncrementalEncoExist) ? ENCO_PHASING_DONE : PHASE_CALC ;//переход в расчет фазового сдвига или в СТОП
          sPnt->zeroHoldStateCnt1 = 0;
        }
      } else {
        sPnt->zeroHoldStateCnt1 = (sPnt->zeroHoldStateCnt1 != 0) ? (sPnt->zeroHoldStateCnt1 - 1) : sPnt->zeroHoldStateCnt1;
      }
      
//      if( PWMOn == 0 && !failNum){ // Прерывание автофазировки  - отключился ШИМ и не было аварии       
//        sPnt->phasingState = AUTOPHASING_ERR;                   // Переход в состояние удержания аварии
//        setBitMicro(ramPnt, progPnt->Pnt_F_Abort);          // Авария - Фазировка прервана
//      } else if ( failNum ) { //ШИМ отключился по причине Аварии
//        sPnt->phasingState = PWM_OFF_WAIT;
//      }
      sPnt->zeroSpdWaitCnt++;
      fail = (sPnt->zeroSpdWaitCnt >= WAIT_ZERO_SPD_REACH_CNT) ? 1 : 0;
      
      defBitMicro(ramPnt , progPnt->Pnt_F_Speed, fail); // авария таймаута - не выходим на скорость ноль за нужное время
      sPnt->phasingState = (fail != 0) ? AUTOPHASING_ERR : sPnt->phasingState;      
      break;

      //-------------------------------------- 
    case PHASE_CALC://Расчет смещения
      if (autoPhasingEn == AUTOPHASING_1_STEP) { //Один такт
        shiftVal = phaseShiftCalc(vfdPhaseRad, encoPhaseRad);
        writeMicroProgParam(PHASE_SHIFT_ADDR, shiftVal);
        //AllPrmPnt->ENCO_ANGLE_SHIFT = shiftVal; // !!!!!
        sPnt->phasingState = ENCO_PHASING_DONE;	 //Фазировка завершена
      } else if (autoPhasingEn == AUTOPHASING_2_STEP) { //два такта
        sPnt->firstPhaseShift = phaseShiftCalc(vfdPhaseRad, encoPhaseRad); //Первый расчет смещения энкодера
        sPnt->phasingState = REF_SET2; //Повторно выходим на задание
      }
      break;

      //-------------------------------------- 
    case REF_SET2: //Выход на задание
      save_float (ramPnt, progPnt->Pnt_PhasingSpd, refSpd); // Ставим задание скорости движения
      sPnt->phasingState = REACH_SPD_WAIT; //переход в ожидание выхода на задание
      break;

      //-------------------------------------- 
    case REACH_SPD_WAIT: //ожидание выхода на задание
     if ((fabsf(Fout) >= minSpdPhasingVal) && (fabsf(encoSpd) >= (fabsf(Fout) - deltaEncoSpd)) && (fabsf(encoSpd) <= (fabsf(Fout) + deltaEncoSpd))){ //фазирование - поcле формирования минимального задания и корректной скорости энкодера
        sPnt->waitReachSpdCnt++;
        if(sPnt->waitReachSpdCnt == SPD_STATE_CNT){
          sPnt->phasingState = ZERO_REF_SET2; //переход в состояние установки задания в ноль
          sPnt->waitReachSpdCnt = 0;
        }
      } else {
        sPnt->waitReachSpdCnt = (sPnt->waitReachSpdCnt != 0) ? (sPnt->waitReachSpdCnt - 1) : sPnt->waitReachSpdCnt;
      }
      
//      if( PWMOn == 0 && !failNum){ // Прерывание автофазировки  - отключился ШИМ и не было аварии       
//        sPnt->phasingState = AUTOPHASING_ERR;                   // Переход в состояние удержания аварии
//        setBitMicro(ramPnt, progPnt->Pnt_F_Abort);          // Авария - Фазировка прервана
//      } else if ( failNum ) { //ШИМ отключился по причине Аварии
//        sPnt->phasingState = PWM_OFF_WAIT;
//      }
      
      sPnt->reachSpdTimeOutCnt++;
      fail = (sPnt->reachSpdTimeOutCnt >= SPD_REACH_TIMEOUT_CNT) ? 1 : 0;

      defBitMicro(ramPnt, progPnt->Pnt_F_Speed, fail); // авария таймаута - не выходим на скорость задания за нужное время
      sPnt->phasingState = (fail != 0) ? AUTOPHASING_ERR : sPnt->phasingState;
      break;

      //-------------------------------------- 
    case  ZERO_REF_SET2: //Установка задания в 0      
      save_float (ramPnt, progPnt->Pnt_PhasingSpd, 0); // Ставим задание скорости движения
      sPnt->phasingState = ZERO_SPD_WAIT2;
      break;

      //-------------------------------------- 
    case ZERO_SPD_WAIT2: //ожидание останова и удержания на нулевой частоте           
      if((fabsf(Fout) >= 0) && (fabsf(Fout) < minZeroSpdPhasingVal)){
        sPnt->zeroHoldStateCnt2++;
        if(sPnt->zeroHoldStateCnt2 >= K_TIME_CALL_MICRO_TAKT){ //если стабильно удерживали на нуле в течении 1 сек
          sPnt->phasingState = PHASE_CALC2 ;//переход в расчет фазового сдвига 
          sPnt->zeroHoldStateCnt2 = 0;
        }
      } else {
        sPnt->zeroHoldStateCnt2 = (sPnt->zeroHoldStateCnt2 != 0) ? (sPnt->zeroHoldStateCnt2 - 1) : sPnt->zeroHoldStateCnt2;
      }      
      
//      if(PWMOn == 0 && !failNum){ // Прерывание автофазировки  - отключился ШИМ и не было аварии       
//        sPnt->phasingState = AUTOPHASING_ERR;                   // Переход в состояние удержания аварии
//        setBitMicro(ramPnt, progPnt->Pnt_F_Abort);          // Авария - Фазировка прервана
//      } else if ( failNum ) { //ШИМ отключился по причине Аварии
//        sPnt->phasingState = PWM_OFF_WAIT;
//      }
      
      sPnt->zeroSpdWaitTimeOutCnt2++;
      fail = (sPnt->zeroSpdWaitTimeOutCnt2 >= WAIT_ZERO_SPD_REACH_CNT) ? 1 : 0;

      defBitMicro(ramPnt , progPnt->Pnt_F_Speed, fail); // авария таймаута - не выходим на скорость ноль за нужное время
      sPnt->phasingState = (fail != 0) ? AUTOPHASING_ERR : sPnt->phasingState;      
      break;

      //-------------------------------------- 
    case PHASE_CALC2: //Второй такт расчета смещения
      phaseShift = phaseShiftCalc(vfdPhaseRad, encoPhaseRad);
      if((abs(sPnt->firstPhaseShift - phaseShift) >= 0) && (abs(sPnt->firstPhaseShift - phaseShift) <= 150)){
        shiftVal = (u16)(((f32)phaseShift + (f32)sPnt->firstPhaseShift) / 2 + 0.5);
        writeMicroProgParam(PHASE_SHIFT_ADDR, shiftVal);
        //AllPrmPnt->ENCO_ANGLE_SHIFT = shiftVal; // !!!!!
        sPnt->phasingState = ENCO_PHASING_DONE;
      } else if((sPnt->firstPhaseShift - phaseShift) < 0){
        if((((sPnt->firstPhaseShift - phaseShift) + 3600) >= 0) && (((sPnt->firstPhaseShift - phaseShift) + 3600) <= 150)){
          shiftVal = (u16)(((f32)phaseShift + (f32)sPnt->firstPhaseShift) / 2 + 0.5);
          writeMicroProgParam(PHASE_SHIFT_ADDR, shiftVal);
          //AllPrmPnt->ENCO_ANGLE_SHIFT = shiftVal; // !!!!!
          sPnt->phasingState = ENCO_PHASING_DONE;
        } else {
          setBitMicro(ramPnt, progPnt->Pnt_F_Speed); //Авария "Неверное число пар полюсов"
          sPnt->phasingState = AUTOPHASING_ERR;
        }
      } else if((sPnt->firstPhaseShift - phaseShift) > 0){
        if(((3600 - (sPnt->firstPhaseShift - phaseShift)) >= 0) && ((3600 - (sPnt->firstPhaseShift - phaseShift)) <= 150)){
          shiftVal = (u16)(((f32)phaseShift + (f32)sPnt->firstPhaseShift) / 2 + 0.5);
          writeMicroProgParam(PHASE_SHIFT_ADDR, shiftVal);
          //AllPrmPnt->ENCO_ANGLE_SHIFT = shiftVal; // !!!!!
          sPnt->phasingState = ENCO_PHASING_DONE;
        } else {
          setBitMicro(ramPnt, progPnt->Pnt_F_Speed); //Авария "Неверное число пар полюсов"
          sPnt->phasingState = AUTOPHASING_ERR;
        }
      }
      break;
      //-------------------------------------- 
    case AUTOPHASING_ERR:   //Состояние удержания аварий автофазирования
      sPnt->errHoldCnt++; //Время удержания аварий - 5 с
      sPnt->phasingState = (sPnt->errHoldCnt == ERR_HOLD_TIME) ? PWM_OFF_WAIT : sPnt->phasingState;
      save_s16(ramPnt, progPnt->Pnt_PhasingEn, 0); // Отключить фазирование после аварии.
      break;
      //-------------------------------------- 
    case PWM_OFF_WAIT:   //Ожидание выключения ШИМ 
      sPnt->phasingState = (PWMOn == 0) ? RESTART_AUTOPHASING : sPnt->phasingState; //ШИМ выключен - перезапуск алгоритма
      break;
	  //-------------------------------------- 
    case RESTART_AUTOPHASING:						//перезапускаем алгоритм измерения
      sPnt->phasingState = AUTOPHASING_ON_WAIT;     //переход к ожиданию включения автофазировки
      save_s16(ramPnt, progPnt->Pnt_PhasingEn, 0);	// Отключить фазирование
      break;
      //-------------------------------------- 
    case ENCO_PHASING_DONE:
      setBitMicro(ramPnt, progPnt->Pnt_PhasingDone); // Фазировка завершена
      save_s16(ramPnt, progPnt->Pnt_PhasingEn, 0); // Отключить фазирование
      sPnt->phasingState = PWM_OFF_WAIT; // Переход на ожидание отключения ШИМа
      break;
    }
  }
  
  return &progPnt->Pnt_End;
};

//********************************************************
/**
  * @brief Расчет смещения фазы
  * @param ipark1Angle - текущий угол поля в iQ
  * @param encoPos - текущий угол энкодера
  * @retval смещение фазы энкодера
  */
s16 phaseShiftCalc(f32 vfdInputAngle, f32 encoInputAngle){
  s16 vfdAng;
  s16 encoAng;
  s16 phaseShift;
  
  // ПЕРЕПРОВЕРИТЬ, ЧТО ФАЗА ПРИХОДИТ В ое или все же в РАДИАНАХ.
  // МНЕ НАДО ПОЛУЧИТЬ ГРАДУСЫ
  encoAng = vfdAng = 0;
  // Перевод фазы в градусы
  if(vfdInputAngle >= 0){
	  vfdAng = (s16)(10.0f*(vfdInputAngle * _RAD_TO_DEG) + 0.5); //перевод в s16 + один знак после запятой
  }else if(vfdInputAngle < 0){
	  vfdAng = (s16)(10.0f*(vfdInputAngle * _RAD_TO_DEG) - 0.5); //перевод в s16 + один знак после запятой
  }
  
  // Перевод фазы энкодера в градусы
  if(encoInputAngle >= 0){
	  encoAng = (s16)(10.0f * (encoInputAngle * _RAD_TO_DEG) + 0.5); //перевод в s16
  }else if(encoInputAngle < 0){
	  encoAng = (s16)(10.0f * (encoInputAngle * _RAD_TO_DEG) - 0.5); //перевод в s16
  }  

  if((vfdAng >= 0) && (encoAng >= 0)){
	  phaseShift = vfdAng - encoAng;
	  phaseShift = (phaseShift < 0) ? phaseShift + 3600 : phaseShift;
  }else if((vfdAng < 0) && (encoAng >= 0)){
	  vfdAng+=3600;
	  phaseShift = vfdAng - encoAng;
	  phaseShift = (phaseShift < 0) ? phaseShift + 3600 : phaseShift;
  }else if((vfdAng >= 0) && (encoAng < 0)){
	  encoAng+= 3600;
	  phaseShift = encoAng - vfdAng;
	  phaseShift = (phaseShift < 0) ? phaseShift + 3600 : phaseShift;
  }else if((vfdAng < 0) && (encoAng < 0)){
	  phaseShift = abs(vfdAng) - abs(encoAng);
	  phaseShift = (phaseShift < 0) ? phaseShift + 3600 : phaseShift;
  }
  return(phaseShift);
}
//********************************************************
#define WRITE_UST 0
#define READ_UST  1
u16 writeMicroProgParam(u16 addr, s32 prmVal) {
	s16 tmp;
	wm_type wmem;

	wmem.AdrPrm = addr;
	wmem.s32.Value = prmVal;
	tmp = MakeOpis(&wmem, WRITE_UST);
	if (tmp == 0){
		return 0;
	}else{
		return 1;
	}
}
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncENCO_PHASING_BLOCK_1[19]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  PhasingEn
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  PhasNomSpd
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  ContactorOn
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  PwmOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  FailTek
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Fout
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Fenco
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  PhaseTek
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  EncoPhasing
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  EncoIncVis
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  AnyEncoVis
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Functional
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //13  PhasingWork
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //14  PhasingSpd
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //15  PhasingDone
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //16  F_Rtimeout
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //17  F_Speed
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //18  F_Abort
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //19  F_LiftDrop
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("ENCO_PHASING_BLOCK", "DD", TblFuncENCO_PHASING_BLOCK_1, FuncENCO_PHASING_BLOCK_1, sizeof(TblFuncENCO_PHASING_BLOCK_1), sizeof(AUTOPHASING_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncENCO_PHASING_BLOCK_1,
#endif
//********************************************************
