#ifndef	nonVolatile_H
#define nonVolatile_H
#include "type.h"
#include "commonmicro.h"


//**************************************************************************
/*============================================================================*/
//         Тип записи в журнале предупреждений
/*============================================================================*/
//**************************************************************************    
typedef struct {
                u16 StateWord_CRC;  //1,2
                u16 NumRec;         //3,4
                u32 timeUnix;       //5,6,7,8
                u16 TIGBT;          // 9,10
                u16 Ud;             // 11,12
                u16 Ifull;          // 13,14
                u16 Iact;           // 15,16
                u16 Iu;             // 17,18
                u16 Iw;             // 19,20
                u16 Iv;             // 21,22
                u16 Fout;           // 23,24
                u16 Uab;            // 25, 26
                u16 Ubc;            // 27, 28
                u16 Uca;            // 29, 30
                u16 rsvd[1];        // 32
               }RecJrn_type;
#define MAX_JRN_FAIL 32
#define MAX_JRN_WARN 32
#define NUM_REC_JRN_ELCOUNTERO 60

typedef struct {            // 34 слова
    u16 indexRec;
    u16 quantRec;
    u16 numRec;
    u16 sizeRec;
    u16 rsrvd;
    u16 crc;
}Index_type;
  
typedef struct {            // 34 слова
    Index_type  RecIndex;
    RecJrn_type RecJrn[MAX_JRN_WARN];
}Jrn_type;

typedef struct {
    u32 FileCrc;
    u16 OfsLastItem;
}HeadOfFileConsum_type;

typedef struct {
    u8 Year;
    u8 Month;
    u32 ConsumptionValue;
}ItemConsum_type;

typedef struct {
	HeadOfFileConsum_type  HeadOfFileConsum;
	ItemConsum_type        ItemConsum[NUM_REC_JRN_ELCOUNTERO];
}ElcountJrn_type;

/*============================================================================*/
//         Тип записи в журнале аварий
/*============================================================================*/

/*============================================================================*/
typedef struct {
    Jrn_type            WarnJrn;
    Jrn_type            FailJrn;
    u16                 HolidayOdd[23] ;           // Массив с битами для календаря  четный год
    u16                 HolidayEven[23] ;           // Массив с битами для календаря  четный год
    ElcountJrn_type     ElCountero;
}fram_file_type;
//**************************************************************************
#define StringLength 32
#define FunctionalStringLength 128

typedef struct{
    u32 SerialNumber;
    u32 timeUnix;
    u8 Functional[FunctionalStringLength];
    u8 Ust1[StringLength];
    u8 Ust2[StringLength];
    u8 Ust3[StringLength];
    u8 Tree[StringLength];
    u8 PultFirmware[StringLength]; // OP24S
    u16 crc;   // Вероятно надо равнять до u32
}DK_file_Old_type;

typedef struct{
    u32 SerialNumber;
    u32 timeUnix;
    u8 Functional[FunctionalStringLength];
    u8 Ust1[StringLength];
    u8 Ust2[StringLength];
    u8 Ust3[StringLength];
    u8 Tree[StringLength];
    u8 PultFirmware[StringLength]; // OP24S
    u16 crc;   // Вероятно надо равнять до u32
    u16 newFile;   // Вероятно надо равнять до u32
    
    // Отступ для стыковки со старым форматом.
    u8 PultOP24BFirmware[StringLength]; // Добавлены новый текстовые ссылки на файлы загрузки
    u8 MkarFirmware[StringLength];
    u8 TreeGz[StringLength];
    u8 PultOP24BFont[StringLength]; // Шрифты чб пульта
    u8 PultOP24BGuide[StringLength]; // Гид чб пульта
    u8 Reserved1[StringLength];
    u8 Reserved2[StringLength];
}DK_file_type;
//*************
//**************************************************************************
#define NUM_REC_TREND  1500
#define NUM_REC_FIRE_MODE 64 // т.к. аварий 64
typedef struct{
    u32                 unixtime;
    u16                 value;
    u16                 crc;
}Trend_rec_type;
typedef struct{
	Trend_rec_type      rec[NUM_REC_TREND];
}Trend_file_type;

typedef struct{
    u32                 unixtime;
    u16                 errormask1;
    u16                 errormask2;
    u16                 errormask3;
    u16                 errormask4;
    u16			Ud;                       
    u16			Iact;                      
    u16			Ifull;                     
    u16			Ireact;                   
    u16			Iu;                        
    u16			Iv;                       
    u16			Iw;                       
    u16			Fout;
	//Сюда можно добавить еще ток утечки, при этом размер будет 30 байт!
    u16			crc;
}Fire_mode_rec_type;
typedef struct{
	Fire_mode_rec_type      rec[NUM_REC_FIRE_MODE];
}Fire_mode_list_type;

typedef struct{
    Functional_type     curFunct;
    u16                 crc;
}FileFunctional_type;
typedef struct{                         // Структура в которой хранятся калибровочные значения для аналоговых входов
       		u16			CalibrCode[5][7];
}nvAinCalState_type;

typedef struct{                         // Структура в которой хранятся калибровочные значения для аналоговых выходов
       		s16			CalibrCode[2][5];
}nvAoutCalState_type;

typedef struct{				// Структура в которой хранятся калибровочные значения каналов измерения токов и напряжения
		   float	K_Iu;
	    	   float	K_Iv;
	    	   float	K_Iw;
	    	   float	K_Udc;
	    	   float	K_Il;
                   float	K_Uab;
	    	   float	K_Ubc;
	    	   float	K_Uca;
}nvAdjCoef_type;

//******************************************************************************
//  Структура хранения Модбас адресов параметров в соответствии с их линейными адресами
#define SZ_PARAM_ADR 2048       
typedef struct {
      u16       crcStruct; // CRC на всю структуру данных
      u16       crcMicro;    // CRC предыдущей микропрограммы
      u16       UstKolv;   // Количество уставок 
      u16       UstAdr[SZ_PARAM_ADR]; 
      u16       alignToU32;  // Дополнительный u16 для правильного расчета адресов - четное кол-во u16. Без него неправильно формируются адреса. Размеры данных и их адреса не стыкуются.
}nvParamAdr_type;
//******************************************************************************

//******************************************************************************
typedef struct{
    f32     Rr         ; // 1
    f32     Rs         ; // 2
    f32     Lm         ; // 3
    f32     Lls        ; // 4
    f32     Lsd	       ; // 5  // Input: Индуктивность статора PMSM по оси D
    f32     Lsq	       ; // 6  // Input: Индуктивность статора PMSM по оси Q
    f32	    Ixx	       ; // 7  // Ток холостого хода двигателя, вычисленный в режиме измерения с вращением или введенный руками.
    f32     Jr         ; // 8
    f32     PsiR       ; // 9  // Input: Магнитный поток ротора PMSM
    f32     RegWr_Kp   ; // 10
    f32     RegWr_Ki   ; // 11
    f32     KpCur      ; // 12 // Коэффициенты регулятора контура тока
    f32     KiCur      ; // 13 // Коэффициенты регулятора контура тока
    s16     Encoder_D  ; // 1
    u16     MTZCurrent ; // 2  // Максимальный ток ПЧ
    u16     TorqueAutoSet; // 3 Режим расчета момента двигателя - Авто или введенное значение
}nvEngineParam_type;
//******************************************************************************
#define SZ_MICROPROG_DATA 4096
#define	MTZ_BUF_SIZE	6

// количество неиспользуемых слов в структуре nv_type
// Один! фиктивный u32 для коррекции смещения начиная от mtzTimeBuff иначе идет не стыковка и потеря калибровок СЛЕДИТЬ ЗА ЭТИМ !!!!
#define sizeSpace  (16384 * sizeof(u16)  - sizeof(u16) * SZ_MICROPROG_DATA - sizeof(u16) * 5 -			                                \
        sizeof(u32)*5 /* u32 реально 4 элемента*/ - sizeof(fram_file_type) - sizeof(FileFunctional_type) * 2 - sizeof(u32)*(MTZ_BUF_SIZE) -     \
        sizeof(nvAoutCalState_type) - sizeof(DK_file_type) - sizeof(DK_file_Old_type) - sizeof (nvParamAdr_type) -		                                                \
        sizeof(nvAinCalState_type ) - sizeof(nvAdjCoef_type) - sizeof(Trend_file_type) - sizeof(nvEngineParam_type))                          

typedef struct{
       u16 spaceData[sizeSpace/sizeof(u16)];
}Space_type;
// u32 -не хватает 1 
// 5 u16
// 5 u32
//**************************************************************************
 typedef struct	{
        u16                 MicroProg[SZ_MICROPROG_DATA]    ;    //AllNV
        u16                 MemSignatura	; // +
        u16                 MicroSignatura	; // +
        u16                 errImaxCnt      ;     // +
        u16                 UstRecoveryFlag ;     // +
        u32                 stopTime        ;     // +
        u32                 mtzTime         ;     // +
        u32                 mtzTimeSum      ;     // +
        fram_file_type      file            ;     // +
        DK_file_Old_type    DK_file_Old     ;     // Оставлен старый формат файла, для позиционирования памяти
        nvParamAdr_type     UstModbusAdr    ;     // +
        DK_file_type        DK_file         ;     // Новый расширенный формат файла конфигурации.
        
	union {
	     Trend_file_type     trend_file;      // +
	     Fire_mode_list_type fire_list;
	};
 
        Space_type          Space;                              // пустое место для возможности увеличения объема предыдущих данных без затирания следующих
        u32		    mtzTimeBuff[MTZ_BUF_SIZE];          //+ Буфер событий по авариям МТЗ - чистое UNIX время        
        nvAoutCalState_type AOutput;				//+ nvAinCalState_type  AInput[NUMBER_AIN_MAX];
        nvAinCalState_type  AInput;				//+ nvAoutCalState_type AOutput;
        nvAdjCoef_type      adjCoefcnts;                        //+ Калибровки каналов измерения тока и ЗПТ
        nvEngineParam_type  nvEngineParam;                      //+ Параметры двигателя

        FileFunctional_type fileCurFunct;               //+ Структура текущий функционал
        FileFunctional_type fileCurFunctPrev;           //+ Структура предыдущий функционал
        u16                 EngineRunFlag   ;           //+ Флаг аварии по исчезновению питания. Этот флаг устанавливается при пуске двигателя, сбрасывается программой при останове. Если при подаче питания не сброшен то было исчезновение напряжения
        u32                 PowerFailTime   ;           //+ Переменная для непрерывной записи времени. Время обнуляется при останове двигателя программой
        } nv_type;                                              // структура данных FRAM
//**************************************************************************    
#define		nv	        (*(nv_type*)0)
//**************************************************************************

//**************************************************************************
#endif	    
