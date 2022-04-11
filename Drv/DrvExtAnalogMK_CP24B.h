#ifndef _EXTANALOGMK_H_
#define _EXTANALOGMK_H_

#include "type.h"
#include "GlobalDefines.h"
#include "GlobalVar.h"
#ifdef _PROJECT_FOR_STM32_
   #include "stm32f4xx_hal.h"
#endif

// Назначение ног для подключение каналов измирения
#define EXTANALOG_TX_PIN        GPIO_PIN_10
#define EXTANALOG_TX_PORT       GPIOC
#define EXTANALOG_RX_PIN        GPIO_PIN_11
#define EXTANALOG_RX_PORT       GPIOC


//*********************************************************************************//
//******************************* Аналоговые выходы *******************************//
#define QuantityAout_CP24        2                 // количество аналоговых выходов платы CP_24B         
#define TASK_PWM_MAX             5000              // Максимальное задание для DAC

#define MaxValueAout_0_10V       1000 // 10 V
#define MinValueAout_0_10V       100  // 1V
#define MinValueAout_4_20mA      400
#define MaxValueAout_4_20mA      2000

#define CALIBR_AOUT_DEF_1V       462               // Коеффициент калибровки по умолчанию для точки 1V
#define CALIBR_AOUT_DEF_10V      4600              // Коеффициент калибровки по умолчанию для точки 10V
#define CALIBR_AOUT_DEF_4MA      620              // Коеффициент калибровки по умолчанию для точки 4ma
#define CALIBR_AOUT_DEF_20MA     3050              // Коеффициент калибровки по умолчанию для точки 20ma

//*********************************************************************************//
//******************************* Аналоговые входы *******************************//
#define QuantityAin_CP24        2            // количество аналоговых входов платы CP_24         

// Калибровочные точки по входам
#define CalMaxValueAin_0_10V       1000 // 10 V
#define CalMinValueAin_0_10V       100  // 1V
#define CalMinValueAin_4_20mA      400
#define CalMaxValueAin_4_20mA      2000
#define CalMinValueAin_PT1000      -500
#define CalMaxValueAin_PT1000      2000

// Диапазон сигнала после пересчета
#define MaxValueAin_0_10V       1000 // 10 V
#define MinValueAin_0_10V       0    // 0V
#define MinValueAin_4_20mA      400
#define MaxValueAin_4_20mA      2000
#define MinValueAin_PT1000      -500
#define MaxValueAin_PT1000      2000

#define CALIBR_AIN_DEF_1V       462          // Коеффициент калибровки по умолчанию для точки 1V
#define CALIBR_AIN_DEF_10V      4600         // Коеффициент калибровки по умолчанию для точки 10V
#define CALIBR_AIN_DEF_4MA      620          // Коеффициент калибровки по умолчанию для точки 4ma
#define CALIBR_AIN_DEF_20MA     3050         // Коеффициент калибровки по умолчанию для точки 20ma
#define CALIBR_AIN_DEF__50С      970         // Коеффициент калибровки по умолчанию для точки -50С
#define CALIBR_AIN_DEF_200С     2000         // Коеффициент калибровки по умолчанию для точки 200С


#define AdcCodeKZ_0_10V         3950         // количестно кодов АЦП для детектирования аварии КЗ датчика 
#define AdcCodeKZ_4_20mA        3500         // количестно кодов АЦП для детектирования аварии КЗ датчика 
#define AdcCodeHH               100          // количестно кодов АЦП для детектирования аварии обрыва катчика 

#define TimeEnableKeyAin        K_TIME_CALL_MICRO_TAKT * 2 // счетчик тактов МП (250 = 2 с)
//*********************************************************************************//

// Статус связи с MKAR
typedef enum{
    MkarNoInit = 0,        // Драйвер не настроен
    MkarNoLink,            // Нет связи с MKAR 
    MkarOkLink             // Связь с MKAR присутствует
}MkarLinkStat_Enum;

// Статус связи с MKAR
typedef enum{
    MkarVersionUnknown = 0,
    //MkarLowVersion = 1,   
    Mkar_16891 = 1,
    Mkar_16892 = 2,
    //MkarTopVersion = 2,
}MkarVersion_Enum;
        
// Коеффициенты калибровки аналогового выхода
typedef struct{
    u16  Val1V;                 // Коеффициент калибровки для точки 1 В.
    u16  Val10V;                // Коеффициент калибровки для точки 10 В.
    u16  Val4mA;                // Коеффициент калибровки для точки 4 мА.
    u16  Val20mA;               // Коеффициент калибровки для точки 20 мА.
    u16  Val_50C;               // Коеффициент калибровки для точки -50град.
    u16  Val200C;               // Коеффициент калибровки для точки 200град.
    u16  crc;                   // CRC данных калибровки.
}AinCalibr_type;

// Описатель аналогового входа (для секции static) - блок СР24В MKARs
typedef struct{
    s16              AdcVal;     // То, что снимают с АЦП
    AinExtType_Enum  Type;       // Тип аналового входа
    u16              CntDisable; // Счетчик тактов МП после КЗ затчика и выключения ключа
    AinState_Enum    AinStatus;  // Состояние аналогового входа( ОК, обрыв, КЗ)
    AinCalibr_type   Calibr;     // Калибровочные коеффициенты    
    s16              AinValuePercent;   // Значение входа в процентах от типа входа, 1 знак после запятой
}AinMemExt_type;

// Коеффициенты калибровки аналогового выхода
typedef struct{
    u16  Val1V;              // Коеффициент калибровки для точки 0 В.
    u16  Val10V;             // Коеффициент калибровки для точки 10 В.
    u16  Val4mA;             // Коеффициент калибровки для точки 4 мА.
    u16  Val20mA;            // Коеффициент калибровки для точки 20 мА.
    u16  crc;                // CRC данных калибровки.
}AoutCalibr_type;

// Описатель аналогового выхода (для секции static) - CP24B MKAR
typedef struct{
    AoutExtType_Enum   Type;                     // Тип аналогового выхода
    u16                DacCode;                  // Задание для DAC в кодах DAC  
    u16                FlagWrNv;
    AoutCalibr_type    Calibr;                   // Коеффициенты калибровки
    float              ValAout;
    s16                AoutValPercent; // Значение аналогового выхода в % с 1 знаком после запятой
}AoutMemExt_type;

// структуры вернул к старому варианту без блока расширения
//
#ifdef _PROJECT_FOR_STM32_
// Структура пакета для отправки
typedef __packed struct {   
   u16 crc;                       // КС на пакет
   
   AinExtType_Enum Ain1Type;      // Тип аналогового входа 1
   AinExtType_Enum Ain2Type;      // Тип аналогового входа 2
   
   AoutExtType_Enum Aout1ExtType; // Тип для аналогового выхода 1
   AoutExtType_Enum Aout2ExtType; // Тип для аналогового выхода 2
   
   u16 TaskAout1;                 // Задание для аналогового выхода 1 
   u16 TaskAout2;                 // Задание для аналогового выхода 2  
   
   u16 ExtBlockTask[15];           //Задание для блока расширения
   u16 extTaskCRC;
   //u8 reserv[20];                 // Зарезервированное место
} DataTx_type;

// Структура пакета для отправки
typedef __packed struct {   
   u16 crc;                       // КС на пакет
   
   u16 VerPO;                     // Версия ПО МКАР 
   u16 CrcPO;                     // КС на ПО МКАР
   
   u16 Ain1Val;                   // Текущее значение Ain1 в кодах АЦП 
   u16 Ain2Val;                   // Текущее значение Ain2 в кодах АЦП

   AinState_Enum AinState1;       // Статус Ain1 
   AinState_Enum AinState2;       // Статус Ain2   
    
  // u8 reserv[10];                 // Зарезервированное место
   extBlockParamType extBlockType;
   //extBlockModeType extBlockType;       //тип блока расширения
   
   u16 extBlockData[15];                  // по типу блока можно будет определить где заканчиваются данные
} DataRx_type;
#else
typedef struct {   
   u16 crc;                       // КС на пакет
   
   AinExtType_Enum Ain1Type;      // Тип аналогового входа 1
   AinExtType_Enum Ain2Type;      // Тип аналогового входа 2
   
   AoutExtType_Enum Aout1ExtType; // Тип для аналогового выхода 1
   AoutExtType_Enum Aout2ExtType; // Тип для аналогового выхода 2
   
   u16 TaskAout1;                 // Задание для аналогового выхода 1 
   u16 TaskAout2;                 // Задание для аналогового выхода 2  
   
   u16 ExtBlockTask[15];           //Задание для блока расширения
   u16 extTaskCRC;
   //u8 reserv[20];                 // Зарезервированное место
} DataTx_type;

// Структура пакета для отправки
typedef struct {   
   u16 crc;                       // КС на пакет
   
   u16 VerPO;                     // Версия ПО МКАР 
   u16 CrcPO;                     // КС на ПО МКАР
   
   u16 Ain1Val;                   // Текущее значение Ain1 в кодах АЦП 
   u16 Ain2Val;                   // Текущее значение Ain2 в кодах АЦП

   AinState_Enum AinState1;       // Статус Ain1 
   AinState_Enum AinState2;       // Статус Ain2   
    
 // u8 reserv[10];                 // Зарезервированное место
   extBlockParamType extBlockType;
   //extBlockModeType extBlockType;       //тип блока расширения
   
   u16 extBlockData[15];                  // по типу блока можно будет определить где заканчиваются данные
} DataRx_type;
#endif   
// ----
// !!!! КОнец блокирования новых структур

// Структура пакета для отправки
typedef struct {   
   u16 crc;                       // КС на пакет
   
   AinExtType_Enum Ain1Type;      // Тип аналогового входа 1
   AinExtType_Enum Ain2Type;      // Тип аналогового входа 2
   
   AoutExtType_Enum Aout1ExtType; // Тип для аналогового выхода 1
   AoutExtType_Enum Aout2ExtType; // Тип для аналогового выхода 2
   
   u16 TaskAout1;                 // Задание для аналогового выхода 1 
   u16 TaskAout2;                 // Задание для аналогового выхода 2  
   
   u8 reserv[20];                 // Зарезервированное место
} DataTx_16891_type;    

// Структура пакета для отправки
typedef struct {   
   u16 crc;                       // КС на пакет
   
   u16 VerPO;                     // Версия ПО МКАР 
   u16 CrcPO;                     // КС на ПО МКАР
   
   u16 Ain1Val;                   // Текущее значение Ain1 в кодах АЦП 
   u16 Ain2Val;                   // Текущее значение Ain2 в кодах АЦП

   AinState_Enum AinState1;       // Статус Ain1 
   AinState_Enum AinState2;       // Статус Ain2   
    
   u8 reserv[10];                 // Зарезервированное место
} DataRx_16891_type; 

/*-----------------------------------------------------------------------------
Define the structure of the EXTANALOG Driver Object 
-----------------------------------------------------------------------------*/
typedef struct { 
   MkarVersion_Enum   versionMKAR;     // Версия ПО МКАР
   MkarLinkStat_Enum  MkarLinkStat;    // Статус связи с МКАР
   DataRx_type *DataRx;                // Указатель на принятые данные
   DataTx_type *DataTx;                // Указатель на данные которые необходимо передать
   void (*init)();                   // Настройка 
   void (*hand)();                   // Вызов обработчика
} EXTANALOG;    

/*-----------------------------------------------------------------------------
Define a EXTANALOG_handle
-----------------------------------------------------------------------------*/
typedef EXTANALOG *EXTANALOG_handle;

/*------------------------------------------------------------------------------
Default Initializers for EXTANALOG Object 
------------------------------------------------------------------------------*/
#define EXTANALOG_DEFAULTS { Mkar_16892,             \
                             MkarNoInit,              \
                             (DataRx_type*)0,         \
                             (DataTx_type*)0,         \
                             (void (*)(u32))DrvMkarInit,    \
                             (void (*)(u32))DrvHandlerMkar, \
                           }

/*------------------------------------------------------------------------------
 Prototypes for the functions
------------------------------------------------------------------------------*/
void DrvMkarInit(EXTANALOG_handle);      // Настроить Драйвер MKAR
void DrvHandlerMkar(EXTANALOG_handle);   // вызов обработчика
void SetDataExtBlock(DataRx_type* ExtBlockData);          //Присвоить данные блока дискр. вх/вых.
void FillDataExtBlock();

#endif // _EXTANALOGMK_H_