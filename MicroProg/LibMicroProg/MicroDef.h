//_DEBUG;_WCHAR_T;LARGE_MODEL
//
//****************************************************************************
//флаги в описателе параметра
#define TypeVar_fprm	        	0U	    //5бит    //format
#define MasterVis_fprm	        	5U	    //1бит
#define MasterChange_fprm	    	6U	    //1бит
#define TexnVis_fprm	        	7U	    //1бит
#define TexnChange_fprm	        	8U	    //1бит
#define UserVis_fprm	        	9U	    //1бит
#define UserChange_fprm	        	10U	    //1бит
#define Power_fprm	        	11U	    //3бит    //Колич.отображаемых символов после десятичной точки для числовых значений
#define EditType_fprm	        	14U	    //3бита
#define nonVolatile_fprm	    	17U	    //1бит
#define LimChange_fprm	        	18U	    //1бит
#define FormatChange_fprm	    	19U	    //1бит
#define BaseNumber_fprm	        	20U	    //4бит
#define FuncNumber_fprm	        	24U	    //4бит

//#define Next_fprm	                28U	    //1бит

#define TypeVar_fprm_mask	    	0x0000001F
#define Access_fprm_mask	    	0x000007E0
#define EditType_fprm_mask	    	0x07//0x3
#define BaseNumber_fprm_mask		0xF
#define FuncNumber_fprm_mask		0xF // После сдвига вправо
//****************************************************************************
// Номера для баз в массиве баз
#define NUM_FREQ_BASE	            1
#define NUM_VOLT_BASE	            2
#define NUM_CUR_BASE	            3
#define NUM_POWER_BASE	            4
#define NUM_1000_BASE	            5
#define NUM_HZ_BASE	                6
#define NUM_BASE_REF_PID            7

//EditType_fprm
#define Non_RdkPrm	            	0
#define Stop_RdkPrm	            	1
#define Always_RdkPrm	        	2
//****************************************************************************
//TypeVar_fprm
#define	sec2Frmt	            	0	    //старшие два байта 4 байтного параметра	2pice
#define	NtxtFrmt	            	1	    //номер текста                            	1 s16
#define	Hex16Frmt	            	2	    //как int16Frmt                            	2 s16
#define	int16Frmt	            	3	    //знаковое числовое значение 16 бит        	2 s16
#define	HourMinFrmt	            	4	    //время:минуты, часы                    	2 s16
#define	MinSecFrmt	            	5	    //время:минуты, часы                    	2 s16
#define	floatFrmt	            	6	    //числовое значение в формате float	     	4 float
#define	int32Frmt	            	7	    //знаковое числовое значение 32 бит        	4 s32
#define	HmsFrmt	                	8	    //часы:минуты:секунды                   	4 s32
#define	UTimeFrmt	            	9	    //линейное время по Unix	            	4 s32
#define	refFormat	                10	    //ссылка:группа, параметр                   2 s16
#define	MonDayFrmt	                11	    // дата: день:месяц                        	2 s16
#define	uns16Frmt	            	12	    // беззнаковое числовое значение 16 бит        2 s16
#define	uns32Frmt	            	13	    // беззнаковое числовое значение 32 бит        4 u32
#define	kolvFormat	                14	    // количество форматов в приводе

#define	mask4byteFormat	            ((1<<floatFrmt)|(1<<int32Frmt)|(1<<uns32Frmt)|(1<<HmsFrmt)|(1<<UTimeFrmt))
//**************************************************************************
//коды единиц измерения
#define	bezRasm	                	0	    //
#define	mVRasm	                	1	    //мВ
#define	VRasm	                	2	    //В
#define	kVRasm	                	3	    //кВ
#define	mARasm	                	4	    //мА
#define	ARasm	                	5	    //А
#define	kARasm	                	6	    //кА
#define	HzRasm	                	7	    //Гц
#define	kHzRasm	                	8	    //кГц
#define	VARasm	                	9	    //ВА
#define	kVARasm	                	10	    //кВА
#define	WRasm	                	11	    //Вт
#define	kWRasm	                	12	    //кВт
#define	mOmRasm	                	13	    //мОм
#define	OmRasm	                	14	    //Ом
#define	kOmRasm	                	15	    //кОм
#define	MGOmRasm	            	16	    //МOм
#define	mkHLRasm	            	17	    //мкГн
#define	mHLRasm	                	18	    //мГн
#define	mkSRasm	                	19	    //мкс
#define	mSRasm	                	20	    //мс
#define	SRasm	                	21	    //сек
#define	minRasm	                	22	    //мин
#define	HourRasm	            	23	    //ч
#define	mmRasm	                	24	    //мм
#define	cmRasm	                	25	    //см
#define	dmRasm	                	26	    //дм
#define	mRasm	                	27	    //м
#define	obMinRsm	            	28	    //об/мин
#define	CelsRasm	            	29	    //°С
#define	ProcRasm	            	30	    //%
#define	mKvRasm	                	31	    //м2
#define	mKubRasm	            	32	    //м3
#define	mKubPminRasm	        	33	    //м2/мин
#define	mKubPhRasm	            	34	    //м3/ч
#define	opisRasm	            	35	    //опис
#define	atmRasm	                	36	    //атм
#define	litrRasm	            	37	    //л
#define	gRasm	                	38	    //g
#define	HzSRasm	                	39	    //Гц/с
#define	VSRasm	                	40	    //В/с
#define	ProcSRasm	            	41	    //%/с
#define	kmRasm	                	42	    //км
#define	m_S2Rasm	            	43	    //м/с2
#define	kPaRasm	                	44	    //кПа
#define	MPaRasm	                	45	    //МПа
#define	kg_cm2Rasm	            	46	    //кг/см2
#define	psiRasm	                	47	    //psi
#define	FarengRasm	            	48	    //°F
#define	kubSecRasm	            	49	    //куб/с
#define	kW_hRasm	            	50	    //кВт/ч
#define	MW_hRasm	            	51	    //МВт/ч
#define	VpKm_hRasm	            	52	    //В/км
#define	MWar_hRasm	            	53	    //Мвар/ч
#define	rad_hRasm	            	54	    //рад
#define	rad_sec_hRasm	        	55	    //рад/с
#define	rad_sec2_hRasm	        	56	    //рад/с2
#define	kg_m2_hRasm	            	57	    //кг/м2
#define	m_SRasm						58	    //м/сек
#define	Nm_hRasm	            	59	    //Нм
#define	grad_hRasm	            	60	    //град
#define	Hn_hRasm	            	61	    //Гн
#define	_1_sec_hRasm	        	62	    //1/с
#define	kg_hRasm	        	    63	    //kg
#define	mm_2_hRasm	                64	    //mm2

#define	kolvoEdIsm	            	65
//****************************************************************************
// Такт 4 мс
#define FLOAT_TIME_CALL_MICRO_mS        4.0f                    //ms
#define	INT_TIME_CALL_MICRO_mS		4                       //mS
#define	K_TIME_CALL_MICRO_TAKT          250                     //Перевод уставок в секундах в такты микропрог. Кол-во тактов = Т(сек)* K_INT_TIME_CALL_MICRO_mS_V_s
#define	INT_TIME_CALL_MICRO_uS		4000UL                  //uS
#define	FLOAT_TIME_CALL_MICRO		0.004F                  //S
#define MINUTE_MICRO_TAKT           (K_TIME_CALL_MICRO_TAKT * 60)  //!< Количество тактов микропрограммы в 1 минуте

// Такт 8 мс
//#define	INT_TIME_CALL_MICRO_mS		8//4            //mS
//#define	K_TIME_CALL_MICRO_TAKT      125//250          //Перевод уставок в секундах в такты микропрог. Кол-во тактов = Т(сек)* K_INT_TIME_CALL_MICRO_mS_V_s
//#define	INT_TIME_CALL_MICRO_uS		8000UL//4000UL       //uS
//#define	FLOAT_TIME_CALL_MICRO		0.008F //0.004F       //uS
//#define MINUTE_MICRO_TAKT           (K_TIME_CALL_MICRO_TAKT * 60)  //!< Количество тактов микропрограммы в 1 минуте
//****************************************************************************
#define	FLOAT_PIN_TYPE	        	0
#define	S32_PIN_TYPE	        	1
#define	S16_PIN_TYPE	        	2
#define	BIT_PIN_TYPE	        	3
#define	TXT_PIN_TYPE	        	4
#define MASK_PIN_TYPE               0x7

#define	INPUT_PIN_MODE              (0<<4)
#define	OUTPUT_PIN_MODE             (1<<4)
#define	PASSIVE_PIN_MODE            (2<<4)
#define	INDEPEND_2_FLAG             (1<<6)
#define	INDEPEND_FLAG               (1<<7)
#define MASK_PIN_MODE               (0x3<<3)
//**************************************************************************
//биты запросов по слотам обработки аварий
#define	REQUEST_OK	            	0
#define	REQUEST_STOP_MASK	    	0x01	    //стоп
#define	REQUEST_APV_MASK	    	0x02	    //АПВ
#define	REQUEST_NormWait_MASK		0x04	    //ждать нормализации
#define	REQUEST_PUSK_MASK	    	0x08	    //пуск
//********************************************************
//тип торможения
#define	SELECT_MODE_STOP	    	0
#define	FAIL_MODE_STOP	        	1
//********************************************************
#define	Params_blockType         	1
#define	Const_blockType          	2
#define	FORMULA_STRING_blockType 	3
#define	OpisType_blockType       	4
#define	TextString_blockType     	5
#define	Function_blockType       	6
//**************************************************************************
#define SRFLAG_UP                   (1<<0)        // Задатчик темпа работает на увеличение частоты
#define SRFLAG_DOWN                 (1<<1)        // Задатчик темпа работает на уменьшение частоты
#define SRFLAG_REVCUR               (1<<2)        // Задатчик темпа работает на реверс
#define SRFLAG_REVREF               (1<<3)        // Задание для задатчика темпа на реверс
#define SRFLAG_NONZERO              (1<<4)        // Ззадание не нулевое
#define SRFLAG_RESET                (1<<5)        //
#define SRFLAG_FREZE                (1<<6)        //
//#define SRF_FORCEZ                (1<<8)
//********************************************************
#define RAMPDOWN                     0				//тип торможения - выбег
#define FREQUNCY                     1				//тип торможения - частотное
#define COMBI                        2				//тип торможения - комбинированый
//********************************************************
//Способы обработки FuncKX_B_SCALE_1
#define	Stand_kx_b	            	0
#define	Lim_kx_b	            	1
#define	NotSens_kx_b	        	2
#define	NotSens0_kx_b	        	3
//********************************************************

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//аварии из микропрограммы
#define ERR_ZAR_TIME     (1<<8)
#define ERR_MB_CONNECT	(1<<9)
//****************************************************************************
//кода операций в формулах
#define	END_OPERATION	        	0         //
#define	MUL_OPERATION	        	1         //*        Умножение
#define	DIV_OPERATION	        	2         ///        Деление
#define	QUO_OPERATION	        	3         //%        Остаток от деления
#define	ADD_OPERATION	        	4         //+        Сложение
#define	SUB_OPERATION	        	5         //-        Вычитание
#define	LSH_OPERATION	        	6         //<<    Сдвиг влево
#define	RSH_OPERATION	        	7         //>>    Сдвиг вправо
#define	LOW_OPERATION	        	8         //<        Меньше
#define	LEQ_OPERATION	        	9         //<=    Меньше или равно
#define	HEQ_OPERATION	        	10	    //>=    Больше или равно
#define	EQU_OPERATION	        	11	    //==    Равно
#define	NEQ_OPERATION	        	12	    //!=    Не равно
#define	AND_OPERATION	        	13	    //&        Поразрядное И
#define	OR_OPERATION	        	14	    //|        Поразрядное ИЛИ
#define	XOR_OPERATION	        	15	    //^        Поразрядное исключающее ИЛИ
#define	LAND_OPERATION	        	16	    //&&    Логическое И
#define	LOR_OPERATION	        	17	    //||    Логическое ИЛИ
#define	NOT_OPERATION	        	18	    //!        Отрицание
//********************************************************
// Для автовентиля
#define NMOT       4    //!< Количество дополнительных двигателей в системе автовентиль
#define RAZRESHON  1
#define ZAPRESCHEN 0
#define ON  1
#define OFF 0
#define RDYMOT_FAIL 0
#define RDYMOT_1    1
#define RDYMOT_2    2
#define RDYMOT_3    3
//********************************************************
//Типы аналоговых входов
#define	mA0_5_TypeAin	        	0
#define	mA4_20_TypeAin	        	1
#define	V0_10_TypeAin	        	2
#define	V0_10_UP_TypeAin	    	3

//Типы аналоговых выходов
#define	mA0_5_TypeAout	        	0
#define	mA4_20_TypeAout	        	1
#define	V0_10_TypeAout	        	2

#define NUMBER_AIN_MAX              2               // Количество аналоговых входов

#define	NotWr_calibrAin	        	0	    //Не Вписывать
#define	WrZer_5mA_calibrAin	    	1	    //КалибрКод 0 для датчика 0 - 5 мА
#define	WrZer_20mA_calibrAin	    2	    //КалибрКод нуля для датчика 4 - 20 мА
#define	WrZer_10V_calibrAin	    	3	    //КалибрКод нуля для датчика 0 - 10 В
//#define WrZerUP_calibrAin           4               //КалибрКод 0 двухполярный
#define	Wr5_calibrAin	        	4	    //КалибрКод 5mA
#define	Wr20_calibrAin	        	5	    //КалибрКод 20mA
#define	Wr10_calibrAin	        	6	    //КалибрКод +10в
#define	kolv_calibrAin	        	7	    //количество калибровок

#define	NotWr_calibrAout	       	0	    //Не Вписывать
#define	WrZer_5mA_calibrAout	   	1	    //КалибрКод 0 для сигнала 0 - 5 мА
#define	WrZer_20mA_calibrAout	    2	    //КалибрКод нуля для сигнала 4 - 20 мА
#define	WrZer_10V_calibrAout	   	3	    //КалибрКод нуля для сигнала 0 - 10 В
#define	Wr5_calibrAout	        	4	    //КалибрКод 5mA
#define	Wr20_calibrAout	        	5	    //КалибрКод 20mA
#define	Wr10_calibrAout	        	6	    //КалибрКод +10в
#define	kolv_calibrAout	        	7	    //количество калибровок

//********************************************************

#define TXT_FILTR_TO_CYCLES(x)		(u16)(1 << x)					//перевод значения уставки фильтра в циклы микропрограммы
