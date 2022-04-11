#ifndef _GLOBALDEFINES_H_
#define _GLOBALDEFINES_H_

#include "type.h"

// Обменная структура с загрузчиком!
typedef struct{
    u16     VerBoot;             // Версия ПО (целая часть)
    u16     PodVerBoot;          // Подверсия загрузчика (дробная часть)
    u16     CrcBoot;             // КС на ПО загрузчика
    u16     invCrcBoot;          // КС на ПО загрузчика с инвертированное значение
    u32     FlagFromMainPO;      // Флаг о том что пришли с загрузчика  
}BootCommonData_type;  


//----------------------------------------------------------------------------------------------------------------------------------
// Описание параметров одного канала измерения
typedef struct {   
     float  Imot;       // Задание тока симуляции
     float  Udc;        // Задание ЗПТ симуляции
     float  Uin;        // Задание входной сети симуляции     
     u16    Enable;         // Режим разрешен
} SimulationBlock_type;

// Описание параметров одного канала измерения
typedef struct {   
     u16    mPretorqueSet;       // Разрешение на выполнение задания предмомента
     u16    mPretorqueReset;     // Команда на сброс задания предмомента
     float  mPretorqueTime;      // Время подъема интеграла в режиме предмомента     
     u16    mPretorqueCatch;     // Предмомент захвачен
     u16    Enable;              // Разрешение работы предмомента
     u16    Mode;                // Режим работы Предмомента.
} PretorqueBlock_type;

// Описание параметров одного канала измерения
typedef struct {   
     float  Current;        // Мгновенное значение
     float  Frequency;      // Частота начального торможения   
     u16    Enable;         // Режим разрешен
     u16    ActionDC;       // Включить торможение DC
} DC_Brake_type;

// Описание параметров одного канала измерения
typedef struct {   
     float  FpwmDC;        // Частота ШИМ на постоянном токе
     u16    PwmDCenable;   // Режим работы ШИМа на постоянном токе разрешен
} DC_PWM_type;

// Описание параметров одного канала измерения
typedef struct {   
     float  FlashVal;        // Мгновенное значение
     float  Val;             // RMS
     float  Gain;            // Коэффициент пересчёта в float
     s16    AdcVal;          // Значение в кодах АЦП
     s16    OffSet;          // Смищение нуля АЦП
} TekValParam_type;

// Настройка защит
typedef struct {  
     f32  UdMax;           // Значение верхнего значения напряжения ЗПТ
     f32  UdMin;           // Значение нижнего значения напряжения ЗПТ
     f32  Imtz;            // Максимальный ток (МТЗ)
     f32  Tmax;            // Максимальная температура ключей
     f32  Udmax_Charge;    // Значение верхнего значения напряжения ЗПТ при заряде (для сброса Udmax)
     
     // Защиты для блока СР24В
     u16    ControlUin;      // Вкл/откл контроля напряжения сети
     f32  UinMax;          // Значение верхнего значения напряжения входной сети
     f32  UinMin;          // Значение нижнего значения напряжения входной сети     
     f32  Udustir;         // Юстировочное значение для ЗПТ          
} FailsSetup_type;

// Структура для медленных измерений
typedef struct {  
     u16  TempIGBTU;       // Температура ключей по фазе U
     u16  TempIGBTV;       // Температура ключей по фазе V
     u16  TempIGBTW;       // Температура ключей по фазе W
     u16  TempBoard;       // Температура на плате блока  
     u16  Ain2;            // Аналоговый вход 2
     u16  Ain1;            // Аналоговый вход 1
     u16  AdcVoltRef;      // Опорное напряжение АЦП
     u16  Vbat;            // Напряжение батарейки
} LowSpeedAinAdc_type;

// Тип управления ПЧ
typedef enum{
    SCALAR_NOT_MV = 0,       // Скаляр без мёртвого времени
    SCALAR,                  // Скаляр
    ACIM_FOC_SENSOR,         // ВУ АД с ОС
    ACIM_FOC_SENSOR_LESS,    // ВУ АД без ОС
    PMSM_FOC_SENSOR,         // ВУ ВД с ОС
    PMSM_FOC_SENSOR_LESS,    // ВУ ВД без ОС
}ControlDrvType_type;
// В LIB_GLOBAL_DEFINE есть дополнительные определения для режимов управления!!!
#define TYPEDRV_ANYFOC    ((1 << ACIM_FOC_SENSOR)|(1 << ACIM_FOC_SENSOR_LESS)|(1 << PMSM_FOC_SENSOR)|(1 << PMSM_FOC_SENSOR_LESS))
#define TYPEDRV_ACIMFOC   ((1 << ACIM_FOC_SENSOR)|(1 << ACIM_FOC_SENSOR_LESS))
#define TYPEDRV_PMSMFOC   ((1 << PMSM_FOC_SENSOR)|(1 << PMSM_FOC_SENSOR_LESS))
#define TYPEDRV_ANYSCALAR ((1 << SCALAR_NOT_MV)|(1 << SCALAR))
#define TYPEDRV_ANYACIM   ((1 << ACIM_FOC_SENSOR)|(1 << ACIM_FOC_SENSOR_LESS)|(1 << SCALAR_NOT_MV)|(1 << SCALAR))

// Параметры двигателя
typedef struct {  
     u16  Zp;              // Количество полюсов АД
     f32  Ixx;             // ток холостого хода АД
     f32  Imax;            // Максимальный моменто образующий ток АД
     f32  Isn;             // Номинальный ток двигателя
     f32  Usn;             // Номинальное напряжение АД, действ. линейное значение (pu)
     f32  Fnom;            // Номинальная частота питающего напряжения двигателя (pu)
     f32  Wnom;            // Номинальная частота ротора  (pu)
     f32  Wmax;            // Максимальная частота ротора (pu)
     f32  Torque_n;        // Номинальный момент
     f32  Vnom;            // Номинальная скорость двигателя в об/мин   
     f32  Pnom;            // Номинальная мощность двигателя   
     f32  cosFi;           // Номинальный косинус двигателя
     f32  Kpd;             // Номинальный КПД двигателя     
     
     f32  Rs;              // Input: Сопротивление статора (Ом)
     f32  Rr;              // Input: Сопротивление ротора  (Ом)
     f32  Lm;              // Input: Индуктивность намагничивания (Гн) 
     f32  Lls;             // Input: Индуктивность рассеивания статора (Гн)
     f32  Jr;              // Input: Момент инерции (кг*м2)          
     f32  Lsd;             // Input: Индуктивность статора по оси D 
     f32  Lsq;             // Input: Индуктивность статора по оси Q
     f32  PsiR;            // Input: Магнитный поток ротора

     // Измеренные параметры двигателя
     f32  MeasRs;          // Rs 
     f32  MeasRr;          // Rr
     f32  MeasLm;          // Lm
     f32  MeasLls;         // Lls
     f32  MeasLsd;         // Lsd
     f32  MeasLsq;         // Lsq
     f32  MeasIxx;         // Ixx
     
     f32  Estim_Jr;        // Оценка Jr
     f32  Estim_Kp;        // Оценка Кп для расчета оценки Jr
}EnginParam_type;

// Параметры тормозного ключа
typedef struct {  
     float    UOn;       // Напряжение открывания ключа
     float    UOff;      // Напряжение закрывания ключа
     u16      TormCheckEn; // Разрешение на проверку тормозного резистора - статус параметра разрешения проверки.
     u16      TormCheckStart; // Флаг проверки тормозного резистора
     u16      TormCheckEnd;   // Флаг окончания проверки тормозного резистора
}TormSwParam_type;

// Структура параметров настройки модуля компенсации deadTime
typedef struct {
    f32  DeadTime;                // Dead Time (mks)
    f32  Rsw;                     // Сопротивление ключа инвертора (Ом)
    f32  Udf;                     // Напряжение включения обратного диода (В)
    f32  Utf;                     // прямое падение напр. на IGBT, В
    f32  dI;                      // Минимальный ток переключения компенсации МВ (А)
    u16  nIGBT;                   // количество IGBT в параллель
    u16  nCell;                   // количество ячеек инверторов
    f32  Ton;                     // время включения IGBT, мкс
    f32  Toff;                    // время отключения IGBT, мкс
    u16  ModeDtc;                 // Выбор алгоритма компенсации дедтайма
}DeadTimeInit_type;


// Выбор источника тока возбуждения
typedef enum{
	RefIxx = 0,	    // задание тока намагничевания всегда равно току холостого хода
	RefRegEMF,      // задание тока намагничевания берем с регулятора противоЭДС
	RefRegPsir,     // задание тока намагничевания берем с регулятора потока
	//SourceIdRef = 1234
}SourceIdRef_type;

typedef enum{
  RESERVED = 0,
  ECN1313  = 1,
  ECN1325  = 2,
  SSI_GRAY = 3,
  SSI_BIN  = 4 
}serialModesType;

typedef enum{
  PULSES_PER_TURN = 0,
  BITS_PER_TURN = 1,
}encoResolModeType;

typedef  enum{
  DIRECT_PHASING   = 0,
  INVERSE_PHASING  = 1
}spdPhasingType;

typedef enum{
  AUTO_SPD_PHASING = 0,
  DIRECT_SPD_PHASING = 1,
  INVERSE_SPD_PHASING = 2
}fastSpdPhasingType;

typedef enum{
  ENCO_EMUL_OFF = 0,
  ENCO_EMUL_A   = 1,
  ENCO_EMUL_A_B = 2
}encoEmulModeType;

// Параметры энкодера
typedef struct { 
     serialModesType serialMode;        //Тип последовательного порта
     u16 encoBitResol;
     u16 encoPulseResol;
     encoResolModeType encoResolMode;
     u16 encoAngleShift;
     u16 spdFltTime;
     u16 EncoFlt_N;       // Фильтр энкодера
     u16 processingPeriod;
     u16 fastSpdUse;
     u16 autoPhasingFlg;
     spdPhasingType speedPhasing;
     fastSpdPhasingType fastSpeedPhasing;  //Знак скорости, вычисленной аналоговым инкрементным сигналам
     u8 ADC_Amplitude;
     encoEmulModeType encoEmulMode;
     u16 encoEmulResol;
     u16 EncoBlockPresent; // Флаг наличия блока расширения энкодера в системе
}EncoderParam_type;

// Параметры энкодера
typedef struct { 
    f32         UnbreakTorqPMSM; // Постоянный ток удержания PMSM
    u16         EnEncoPhasing;   //  Активен режим фазирования энкодера
    u16         timeUI_Up;       // Время подъема в мс
    u16         pmsmAntirollBackMode;
    u16         FixTimeId;      // Время фиксации тока 
    u16         timeStepDown;   // Время снижения тока
    f32         FrequencyDown; // Частота снижения тока
    u16         pmsmAntirollBackEnable; // Разрешить работу алгоритма
}PMSMAntirollback_type;


typedef enum{
    PositivePhaseSequence = 0,
    NegativePhaseSequence = 1,
    BrakeInputPhase = 2,
    UndefinedSequence = 3,
}UinPhasing_type;

typedef union{                      //ставится из микропрограммы
    struct {
        u64  PwmOn            : 1;  // 0 - Пуск/Стоп ШИМ
        u64  PwmReInit        : 1;  // 1 - Запрос на переинициализацию модуля ШИМ
        u64  SysCalc          : 1;  // 2 - Запрос на переинициализацию системы регулирования
        u64  AciParamChange   : 1;  // 3 - Флаг по изминению параметра настройки ВУ пользователем (устанавливается в MakeOpis_at24)
        
        u64  PwmParamChange   : 1;  // 4 - Флаг по изминению параметра настройки ШИМ пользователем (устанавливается в MakeOpis_at24)        
        u64  ChargeOk         : 1;  // 5 - Флаг заряда
        u64  TormSwEnable     : 1;  // 6 -
        u64  TormConstCurrOn  : 1;  // 7 -
        
        u64  mWorkEnb         : 1;  // 8 - Пуск/Стоп режима измерения параметров
        u64  WrotCtrl         : 1;  // 9 - Выбор режима управления по скорости или по моменту
        u64  DrvReInit        : 1;  // 10 - Переинициализация по изменению параметров привода
        u64  CurrInv          : 1;  // 11- Инвертирование токов        
        u64  Reserved12       : 1;  // 12 - Резерв 12.
          
        u64  FiltrRefSpdEnb   : 1;  // 13 - Включение фильтра задания скорости
        u64  ManualSpdReg     : 1;  // 14 - Включение ручной настройки регулятора скорости
        u64  ARollbackEnb     : 1;  // 15 - Активация антиоткатной функции
        
        u64  SoftCurDwn       : 1;  // 16 - Плавный сброс тока ниже пусковой частоты
        u64  ManualCurReg     : 1;  // 17 - Включение ручной настройки регулятора контура тока
        u64  RingUpFreezeGT   : 1;  // 18 - Сигнал заморозки работы генератора темпа, при включении прозвонки фаз. Актуально в лифтовом функционале, так как на этапе прозвонки уже идет время удержания на пусковой частоте
        u64  PCPmodeEnable    : 1;  // 19 - Бит разрешения работы в режиме PCP -  в данном режиме блокируется обработка Udmin
        u64  ZarrResistor     : 1;  // 20 - Бит сигнализации заряда через резисторы, а значит нельзя врубать на полную ШИМ. (используется в обработчики РСР)
        u64  Kvitirov_R	      : 1;  // 21 признак квитирования
        u64  BitGotovWorkSVP  : 1;  // 22 бит бит готовности для свп Суходольский 14052018
        u64  EnLeakageCurrent : 1;  // 23 - Разрешение контроля тока утечки
        u64  ForceEnb         : 1;  // 24 - Антиоткат
        u64  BitWAitPhase     : 1;  // 25 бит включения моргания светодиода
        u64  BitStopPusk      : 1;  // 26 запрет пуска - НЕ ЯСНО ГДЕ ИСПОЛЬЗУЕТСЯ!!!
        u64  SplitTorque      : 1;  // 27 Бит выбора режима деления момента между приводами в режиме Ведущий-Ведомый.
        u64  BitAutoKest      : 1;  // 28 Автонастройка коэфф наблюдателя  0 - авто 1 ручная
        u64  BitEnable_J      : 1;  // 29 измерения момента инерции
        u64  BitJumpToNoEncod : 1;  // 30 разреш перехода в бездатч из датч
        u64  BitAccelOverrun  : 1;  // 31 Флаг сигнализации превышения ускорения при разгоне
        u64  FlagWriteJrn     : 1;  // 32 Флаг для записи аварийного журнала
        u64  PerifInitComplete: 1;  // 33 - Инициализация переферии завершина        
        u64  ChargeInProgress : 1;  // 34 - Запущен процесс заряда ЗПТ
        u64  ContactorErrEn   : 1;  // 35 Бит блокирования аварий по контактору. Используется для быстрого сброса аварии STO        
        u64  RingUp           : 1;  // 36 Статус активного режима прозвонки фаз двигателя.        
        u64  BreakEnable      : 1;  // 37 BreakEnable для Лифта
        u64  BreakOff         : 1;  // 38 BreakOff для PMSM_Vector_Cntrl
        u64  Din7toMTZ        : 1;  // 39 Флаг разрешения выдавать сигнал МТЗ на ДИН7
        u64  EvacOn           : 1;  // 40 Активный режим эвакуации
        u64  StartCmd         : 1;  // 41 Пришла команда пуска
        u64  EMFRegEnb        : 2;  // 42-43 - Включение регулятора ослабления поля
        //u32  Reserved         : 1;  // 30, 31

    } bit;
    u64     all;
} M4FlagCommand_type;

typedef union{
  u16 all;
  struct{
   u16 EXT1_DMA    : 1;
   u16 EXT2_DMA    : 1;
   u16 FlashDMA_Req: 1;
  }bits;
}FlashDMABlock_type;

// Флаги состояний системы регулирования и измерения
typedef union{                        
    struct {
        u16 FailLockFlag        : 1;     // Флаг-защелка аварий выставляется при аварии, сбрасывается при пуске
        u16 vPrUp               : 1;     // Флаг достижения нормального возбуждения двигателем
        u16 MTZ_hold            : 1;     // Флаг защелкивания аварии МТЗ
        u16 mWorkFin            : 1;
        u16 MTZ_timeout         : 1;     // Флаг блокировки работы по таймауту МТЗ - ждать сброса
        u16 PickUpFOC           : 1;
        u16 PickUpReadyToGo     : 1;
        u16 PickupFreqFound     : 1;
        
    }bit;
    u16 all;
} FlagState_type;

typedef enum{
	UsePumpFunc = 0,    
	UseLiftFunc = 1,     
}AT24Functional_type;

typedef enum {
  MEASURE_NONE              = 0,
  MEASURE_NO_ROTAT_LM_ESTIM = 1,
  MEASURE_NO_ROTAT_LM_MEAS  = 2,
  MEASURE_ROTATION_LM_MEAS  = 3,
  
} MEASURE_LM_RS_type;
                    


// Fail Defines
#define FastFailMask_NONE           0
#define FastFailMask_SwU            1       // Ошибка по сигналу от силового ключа U
#define FastFailMask_SwV            2       // Ошибка по сигналу от силового ключа V
#define FastFailMask_SwW            3       // Ошибка по сигналу от силового ключа W
#define FastFailMask_SwT            4       // Ошибка по сигналу от силового тормозного ключа
#define FastFailMask_MtzW_Comp      5       // МТЗ по компаратору
#define FastFailMask_MtzZPT         6       // МТЗ в цепи ЗПТ от компаратора 
#define FastFailMask_TimOutPump     7      // Timout накачки
#define FastFailMask_MtzU_Prog      8       // Программная МТЗ по фазе U
#define FastFailMask_MtzV_Prog      9       // Программная МТЗ по фазе V
#define FastFailMask_MtzW_Prog      10      // Программная МТЗ по фазе W
#define FastFailMask_UdMin          11      // Низкое Ud
#define FastFailMask_UdMax          12      // Высокое Ud
#define FastFailMask_ExtStop        13      // Внешний запрет на ШИМ
#define FastFailMask_Tsw            14      // Температура ключей
#define FastFailMask_UinMax         15      // U сети max 
#define FastFailMask_UinMin         16      // U сети min
#define FastFailMask_ruU	    17      // Авария обрыв фазы нагрузки U
#define FastFailMask_ruV	    18      // Авария обрыв фазы нагрузки V
#define FastFailMask_ruW	    19      // Авария обрыв фазы нагрузки W
#define FastFailMask_chkTorm	    20      // Авария - обрыв тормозного резистора. Проверка при пуске.
#define FastFailMask_MTZ_Timeout    21      // Блокировка по таймауту МТЗ    
#define FastFailMask_ErrMeasPrm     22      // ошибка измерения параметров
#define FastFailMask_MeasPrmBrkPhase 23      // ошибка измерения параметров - Обрыв фазы

#define FastFailMask_AdjCoefFault   31      // ошибка размещения калибровок в памяти FRAM - сдвинулся адрес

#define SlowFail_failButton	    32      // Кнопка ав. стоп
#define SlowFail_supplyPhLoss	    33      // Обрыв вх. фазы
#define SlowFailMask_Contactor      34      // ошибка контактора
#define SlowFail_brakeResFail	    35      // Авар. торм сопр
#define SlowFail_InSequenceFail	    36      // Авария чередования фаз
#define SlowFailMask_ZarTime        37      // Время заряда
#define SlowFailLoadLoss            46      // Обрыв фаз двигателя
#define SlowFailERROR_DI            48      //Внешняя авария
#define SlowFailERROR_DI            48      //Внешняя авария
#define SlowFailModbus              50      //Авария modbus
#define SlowFailMaxUin              51      //Авария Максимума входной сети
#define SlowFailPhase3ERR           53      //Авария 3 фаз
#define SlowFail_CurrImbalance      62      // Дисбаланс токов на выходе
#define SlowFail_Slave_Err          63      // Ошибка слейва

#define	SlowFail_NoSlavePresent	    66	    // Режим Ведомый не выбран. Ведомый не в сети.
#define	SlowFail_ReleaseBrkWoPwm    75      // Снятый тормоз лифта, при выключенном ШИМе
#define	SlowFail_StartNoSpeedFault  76      // Пуск Без скорости
#define	SlowFail_MeasParamInt       77      // Прервано измерение параметров

#define	SlowFail_UserFault1         84      // Авария пользователя 1
#define	SlowFail_UserFault2         85      // Авария пользователя 2

// Why the drive is not ready
#define notRdyReason_charge	    80      // заряд Ud в процессе
#define notRdyReason_timeMTZ        81      // отсчет времени после МТЗ
#define notRdyReason_vibeg	    82      // выбег в процессе

// Маска аварий
typedef union{
    struct {
    	u16	None	        : 1;    // 0 
    	u16	SwU	    	: 1;    // 1  Авария силовой ключ фаза U
    	u16	SwV	    	: 1;    // 2  Авария силовой ключ фаза V
    	u16	SwW	    	: 1;    // 3  Авария силовой ключ фаза W
    	u16	SwT	    	: 1;    // 4  Авария силовой ключ тормрзной
        u16     MtzW_Comp       : 1;    // 5  Авария аппаратная МТЗ по компаратору 
        u16     MtzZPT          : 1;    // 6  МТЗ по компаратору в цепи ЗПТ(-)
        u16     TimOutPump      : 1;    // 7  Таймаут по накачке питания драйверов верхних ключей
        u16     MtzU_Prog       : 1;    // 8  Авария программная МТЗ по фазе U
        u16     MtzV_Prog       : 1;    // 9  Авария программная МТЗ по фазе V
        u16     MtzW_Prog       : 1;    // 10 Авария программная МТЗ по фазе W 
        u16     UdMin           : 1;    // 11 Авария по Ud минимум
        u16     UdMax           : 1;    // 12 Авария по Ud максимум
        u16     ExtStop         : 1;    // 13 Внешний запрет на ШИМ
        u16     Tsw	        : 1;    // 14 Авария температура ключей
        u16     UinMax          : 1;    // 15  U сети max 
        u16     UinMin          : 1;    // 16  U сети min
        u16     RingUpErr       : 1;    // 17 - Ошибка при прозвонке двигателя
        u16     Res18           : 1;
        u16     Res19           : 1;
        u16     tormCheck       : 1;
        u16     MTZ_Timeout     : 1;    // 21 - Таймаут МТЗ
        u16     ErrMeasPrm      : 1;    // 22 Ошибка Измерения параметров двигателя
        u16     MeasPrmBrkPhase : 1;    // 23 Ошибка Измерения параметров двигателя - Обрыв фазы
        u16     Reserved24      : 1;
        u16     Reserved25      : 1;          
        u16     Reserved26      : 1;          
        u16     Reserved27      : 1;          
        u16     Reserved28      : 1;          
        u16     Reserved29      : 1;          
        u16     Reserved30      : 1;          
        u16     AdjCoefFault    : 1;    //  Ошибка  размещения калибровокы        
    } bit;
    u32      all;
}FastFailMask_type;

// Значения на момент аварийного останова
typedef struct {
    float  Ifull;
    float  Iact;
    float  Iw;
    float  Iv;
    float  Iu;
    float  Ud;
    float  Fref;
    float  TIGBT;       // температура ключей
    u16    NumFail;
    f32    Uab;
    f32    Ubc;
    f32    Uca;
}FailValue_type;

// Структура флагов для обработки аарий
typedef union{
    struct {
        u16  MtzComp            : 1;     // флаг наличия аварии МТЗ от компаратора
        u16  MtzZPT             : 1;     // флаг наличия аварии МТЗ от компаратора в ЗПТ
        u16  ExtStop            : 1;     // флаг наличия события на внешнее отключение ШИМ
        u16  TimOutPump         : 1;     // флаг наличия события на таймаут накачки (перед пуском)
        u16  ErrMeasPrm         : 1;     // флаг ошибочного измерения параметров
        u16  SwT                : 1;     // флаг наличия аварии тормозного ключа
        u16  SwU                : 1;     // флаг наличия аварии силового ключа по фазе U
        u16  SwV                : 1;     // флаг наличия аварии силового ключа по фазе V
        u16  SwW                : 1;     // флаг наличия аварии силового ключа по фазе W        
        u16  MeasPrmBrkPhase    : 1;     // флаг Обрыв фазы при измерении параметров двигателя
    } bit;
    u16      all;
} FailsFlag_type;

// Структура с данными о авариях
typedef struct{
    u16                     NumFail;              // Текущая авария
    u16                     NumFailStop;          // Причина останова 
    
    u32                     FastFailMask0;        // Маска аварий 0
    u16                     FastFailMask1;        // Маска аварий 1
    
    FailsFlag_type          FailsFlag;            // Структура флагов для обработки аарий
    FailValue_type          FailValue;            // Значение параметров на момент аварийного отключения ШИМ  
    
}FailData_type;


// Тип ПЧ 
typedef enum{
    DrvTypeAutoSet = 0,    // Режима автовыбора
    LowPow_PB24,       // ПЧ низкой мощности
    HighPow_Combo,     // ПЧ высокой мощности
    HighPow_PB24,       // ПЧ высокой мощности на базе PB24
    DrvTypeFault,      // Тип ПЧ не установлен - Ошибка
}DrvType_Enum;

// Типы блоков СР24
typedef enum{
    CP24_v21     = 3,
    CP24_v22     = 2,
    CP24_v23     = 1,
    CP24_v24     = 0,
}CP24version_type;

typedef union{
  u16 word;
  struct{
    u16 telegramType     : 1; //??? ???????: ?????????????? ????? ?????????? ??? ?????
    fastSpdPhasingType fastSpdSign    : 2; //Sign of speed, calculated  by incremental analog signals
    spdPhasingType  spdPhasingSign    : 1;
    u16 PWM_On         : 1;
    u16 drvType        : 1;
    u16 autoPhasing    : 1;
    u16 fastSpdUse     : 1;
    u16 incrementalMode                 : 1;
    serialModesType serialMode          : 3;
    u16 sinCosMode                      : 1;
    u16 encoEmulMode                    : 2; //???????? ????????
    encoResolModeType encoResolMode     : 1;
  }bits;
}headerType;

typedef struct{
  headerType header; 
  u16 reserv;
  u16 crc;
}identDataType;

#define SEND_DATA_DELAY_TYME   1.0F // значение в секундах
#define EXT_PROCESSING_PERIOD  0.0004F // значение в секундах
#define COMM_BLOCK_PROCESSING_PERIOD  0.5F // значение в секундах

typedef enum{
EXT_DATA_SEND_DELAY =    0, // Задержка перед стартом работы
EXT_BLOCK_IDENT     =    1, // Идентификация типа блока
EXT_BLOCK_EXCHANGE  =    2, // Обмен с блоком
EXT_BLOCK_SKIP      =    3, // Обмен с блоком отключен
EXT_SLAVE_UART      =    4, // Прием данных с коммуникационного блока в режиме Slave UART'a
EXT_WAIT_SOFT_INFO  =    5, // Ждем данные о ПО для блока коммуникации
}telegramModeType;

#define IDENT_TELEGRAM 0
#define EXCHANGE_TELEGRAM 1

typedef union{
  u8 word;
  struct{
    u8 Dout1  : 1;
    u8 Dout2  : 1;
    u8 reserv : 6;
  }bit;
}DoutStateType;

typedef struct{
  headerType header; 
  DoutStateType DoutState;
  u16 crc;
}DoutStateRxType;

typedef enum{
  NOT_DEF             = 0,
  INCREMENT_EXT_BLOCK = 1,
  ENDAT_EXT_BLOCK     = 2,
  SIN_COS_EXT_BLOCK   = 3,
  RS485_EXT_BLOCK     = 4,
  DIN_DOUT_EXT_BLOCK  = 5,
  AIN_AOUT_EXT_BLOCK  = 6,
  CAN_EXT_BLOCK       = 7,
  PROFIBUS_EXT_BLOCK  = 8,
  ETHER_EXT_BLOCK     = 9,
  PROFINET_EXT_BLOCK  = 10
}extBlockModeType;

typedef struct{
  extBlockModeType extType;
  u16 crc;
  u16 softVersion;
  u16 softSubVersion;
}extBlockParamType;

typedef struct{
  extBlockParamType ext1;
  extBlockParamType ext2;
  extBlockParamType ext3;
}extConnectedBlocksType;

typedef enum{
  EXT_TIM_SET = 0,
  EXT_TIM_CNT = 1,
  EXT_TIM_NOP = 2,
  EXT_TIM_CLR = 3,
}timsCmdType;

//Структура запроса в обработчик энкодеров
typedef struct{
  headerType header;     
  u16 bitResolution;
  u16 pulseResolution;
  u16 encoAngleShift;
  u16 motorPolePairsNum;
  u16 processingPeriod;
  u16 angleFltBufNum;
  u16 spdFltTime;
  u8 ADC_Amplitude;
  u8 encoEmulResol;
  u16 crc;
}encoDataType;

#define EXT_BLOCK_ANSWER_WAIT  0.1F //Время ожидания установки блока расширения при извлечении "на горячую"
#define EXT_BLOCK_TIMEOUT_CNT  1.0F // Таймаут на потерю связи 1 с - если совсем нет ответовы
#define EXT_BLOCK_ANWER_TIMEOUT  0.005F // Таймаут на Нет ответа от блока расширения
#define ENCO_TX_SYNCH_TYME 2        //Кол-во отправок пакетов в обработчик энкодеров без ожидания ответа. Для синхронизации обмена при подаче питания

//Формат ответа на телеграмму идентификации
typedef struct{
  extBlockModeType extBlock;
  u16 crc;
}identAnswType;

typedef enum{
  ANALOG_INCR_MODE = 0,
  DIGITAL_MODE     = 1
}spdCalcModeType;

typedef union{
  u8 word;
  struct{
    u8 encoErr                  : 5; //Код аварии энкодера
    u8 Rsygnal                  : 1; //Сигнал референтной метки для логгера
    spdCalcModeType spdCalcMode : 1; //Режим измерения скорости
    u8 R_PhasingFlg             : 1; //Флаг завершения фазирования с референтной меткой
  }bits;
}ansHeaderType;

typedef union{
  u16 word;
  struct{
   u16 version    : 8;
   u16 subVersion : 8;
  }bits;
}softVersionType;

//Структура ответа от обработчика энкодеров
#ifdef _PROJECT_FOR_STM32_
typedef __packed struct{
  ansHeaderType    header;        //Заголовок пакета (структура битовых полей)
  float            electricTheta; //Позиция энкодера
  float            electricSpd;   //Электрическая скорость
  softVersionType  softVersion;   //Версия ПО обработчика энкодеров
  u16              softCrc;       //CRC ПО обработчика энкодеров
  u16              dataCRC;       //CRC принимаемого пакета
}RxCalcEncoDataType;
#else //_PROJECT_FOR_STM32_
typedef struct{
  ansHeaderType    header;        //Заголовок пакета (структура битовых полей)
  float            electricTheta; //Позиция энкодера
  float            electricSpd;   //Электрическая скорость
  softVersionType  softVersion;   //Версия ПО обработчика энкодеров
  u16              softCrc;       //CRC ПО обработчика энкодеров
  u16              dataCRC;       //CRC принимаемого пакета
}RxCalcEncoDataType;
#endif //_PROJECT_FOR_STM32_

typedef union{
  u8 dinStateWord;
  struct{
    u8 Din1    : 1;
    u8 Din2    : 1;
    u8 Din3    : 1;
    u8 Din4    : 1;
    u8 Din5    : 1;
    u8 Din6    : 1;
    u8 reserv1 : 1;
    u8 reserv2 : 1;
  }bit;
}DinStateType;

typedef struct{
  u16 softVersion;
  u16 softSubVersion;
  u16 softCRC;
  DinStateType DinState;
  u16 crc; //контрольная сумма пакета
}DinStateTxType;

// Типы аналоговых выходов МКАР
typedef enum{
    AoutOff = 0,       // Аналоговый вход отключен
    Aout0_10v,         // Тип входа 0-10В
    Aout4_20ma,        // Тип входа 4-20ма
}AoutExtType_Enum;
// Типы аналоговых входов МКАР
typedef enum{
    AinOff = 0,         // Аналоговый вход отключен
    Ain0_10v,           // Тип входа 0-10В
    Ain4_20ma,          // Тип входа 4-20ма
    AinPt1000,          // Тип входа Pt1000 
}AinExtType_Enum;
typedef enum{
    AinOk = 0,          // Аналоговый вход исправен
    AinHh,              // Аналоговый вход в состоянии ХХ
    AinKz,              // Аналоговый вход в состоянии КЗ
}AinState_Enum;

typedef union{
  u16 word;
  struct{
    u16 calibrAin3  : 2;  
    u16 calibrAin4  : 2;  
    u16 calibrAout3  : 4;  
    u16 calibrAout4  : 4;  
    u16 reserv                     : 4;
  }bits;
}StatusWordExtType;

typedef struct{
  AinExtType_Enum Ain1Type;      // ??? ??????????? ????? 1
  AinExtType_Enum Ain2Type;      // ??? ??????????? ????? 2
  
  AinState_Enum AinState1;       // ?????? Ain1 
  AinState_Enum AinState2;       // ?????? Ain2
 
  AoutExtType_Enum Aout1ExtType; // ??? ??? ??????????? ?????? 1
  AoutExtType_Enum Aout2ExtType; // ??? ??? ??????????? ?????? 2
   
  u16 Status;                  //В статусе команда на калибровку
  
  s16 Ain1Val;           // Значение Ain1
  s16 Ain2Val;           // Значение Ain2
  
  u16 Adc1Val;                   // ??????? ???????? Ain1 ? ????? ??? 
  u16 Adc2Val;                   // ??????? ???????? Ain2 ? ????? ??? 
  
  u16 TaskAout1;                 // ??????? ??? ??????????? ?????? 1 
  u16 TaskAout2;                 // ??????? ??? ??????????? ?????? 2  
  
   u16 Aout1Val;           // Значение Ain1
   u16 Aout2Val;           // Значение Ain2
   u16 AinAoutBlockPresent; // Флаг наличия блока аналоговых входов/выходов
}AinAoutExtType;

typedef struct {   
    u16 crc;                       // ?? ?? ?????
    
   AinExtType_Enum Ain1Type;      // ??? ??????????? ????? 1
   AinExtType_Enum Ain2Type;      // ??? ??????????? ????? 2
   
   AoutExtType_Enum Aout1ExtType; // ??? ??? ??????????? ?????? 1
   AoutExtType_Enum Aout2ExtType; // ??? ??? ??????????? ?????? 2
   
   u16 Status;                  //В статусе команда на калибровку
   
   u16 TaskAout1;                 // ??????? ??? ??????????? ?????? 1 
   u16 TaskAout2;                 // ??????? ??? ??????????? ?????? 2  
  
}AinAoutTx_type;    

// ????????? ?????? ??? ????????
typedef struct {   
   u16 crc;                       // ?? ?? ?????
   
   u16 softVersion;
   u16 softSubVersion;
   u16 softCRC;
   
   u16 Ain1Val;                   // ??????? ???????? Ain1 ? ????? ??? 
   u16 Ain2Val;                   // ??????? ???????? Ain2 ? ????? ???    
   
   u16 Adc1Val;                   // ??????? ???????? Ain1 ? ????? ??? 
   u16 Adc2Val;                   // ??????? ???????? Ain2 ? ????? ???    
   
   u16 Aout1Val;                   // ??????? ???????? Ain1 ? ????? ??? 
   u16 Aout2Val;                   // ??????? ???????? Ain2 ? ????? ???    
   
   AinState_Enum AinState1;       // ?????? Ain1 
   AinState_Enum AinState2;       // ?????? Ain2
   
}AinAoutRx_type;  

typedef struct{
  headerType header; 
  AinAoutTx_type AinAout;
  u16 crc;
}AinAoutStateTxType;

typedef struct{
  ansHeaderType    header;        //Заголовок пакета (структура битовых полей)
  float            electricTheta; //Позиция энкодера
  float            electricSpd;   //Электрическая скорость
  softVersionType  softVersion;   //Версия ПО обработчика энкодеров
  u16              softCrc;       //CRC ПО обработчика энкодеров
  u16              dataCRC;       //CRC принимаемого пакета
  u16              DMA_RxErrCnt;     //Счетчик ошибок CRC
}calcEncoDataType;

typedef enum{
  EXT1 = 0,
  EXT2 = 1,
  EXT3 = 2
}extNumType;

typedef struct{
  u16 BlockID; 
  u16 softVersion;
  u16 softSubVersion;
  u16 softCRC;
  u16 crc; //контрольная сумма пакета
}EthernetSoftType;

typedef struct{
  headerType header; 
  u16 NetAddr1;
  u16 Baudrate1;
  u16 NetAddr2;
  u16 Baudrate2;  
  u16 crc;
}ExtRs2ExchangeType;

typedef struct{
  u16  HeartBeatID; 
  u16 BlockID;
  u16 crc; //контрольная сумма пакета
}HeartBeatType;


#endif