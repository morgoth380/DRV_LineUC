#ifdef  GET_FUNC_CODE
#include "InputOutput.h"
#include "GlobalVar.h"

#include "DrvExtAnalogMK.h"

#ifdef _PROJECT_FOR_STM32_
     
  #include "DrvLedStatus.h"
  #include "DrvDinDout.h"
  #include "DrvFan.h"
  #include "DrvDiscrT.h"  
  #include "DrvCharge.h" 
  extern LED LedStatus;           // Объект управления светодиодами статуса на передней панели ПЧ
  extern DINDOUT DinDout;         // Объект управления DinDout
  extern FAN FanControl;          // Объект управления вентеляторами
  extern TDISCRDRV TDiscr;        // Объект управления дискретным термодатчиком 
  extern CHARGE Charge;           // Объект управления зарядом 
  
  #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)    
  #include "DrvExtAnalogMK.h"  
  #include "DrvBackupPowerCont.h"  
  extern EXTANALOG Mkar;          // Объект для убмена с MKAR
  extern BACKUPPOW BackupPowStat; // Объект контроля наличия резервного питания  
  
  #endif  
#endif //_PROJECT_FOR_STM32_

//************************ Дискретный датчик температуры *************************//  
#define T_TILTR                 5               // Постоянная времени фильтра в циклах МП
   
//******************************* Дискретные входы *******************************//
#define QuantityDins            13               // количество дискретных входов всего
#define QuantityDins_CP24       7               // количество дискретных входов платы CP_24
const u16 tblFiltrDin[]= {1,2,4,8,16,32,64,128,250,500,1000,2000,4000}; // такты мипрогрограммы в пересчетре значения фильтрации
//*********************************************************************************//
//******************************* Дискретные выходы *******************************//
#define QuantityDouts           5                               // количество дискретных выходов всего
#define QuantityDouts_CP24      5                               // количество дискретных выходов платы CP_24
//*********************************************************************************//
//******************************* Аналоговые входы ********************************//
#define QuantityAin_CP24        2                               // количество аналоговых входов платы CP_24      
#define QuantityAllAin          5                               // количество аналоговых входов платы CP_24      
#define TypeAin_0_10V           0          
#define TypeAin_4_20mA          1

#define OnTypeAin1_0_10V()              GPIOA->BSRR = (u32)AIN1_0_10V_MK_Pin;
#define OffTypeAin1_0_10V()             GPIOA->BSRR = (u32)AIN1_0_10V_MK_Pin << 16U;
#define OnTypeAin1_4_20mA()             GPIOC->BSRR = (u32)AIN1_4_20mA_MK_Pin;
#define OffTypeAin1_4_20mA()            GPIOC->BSRR = (u32)AIN1_4_20mA_MK_Pin << 16U;
#define OnTypeAin2_0_10V()              GPIOC->BSRR = (u32)AIN2_0_10V_MK_Pin;
#define OffTypeAin2_0_10V()             GPIOC->BSRR = (u32)AIN2_0_10V_MK_Pin << 16U;
#define OnTypeAin2_4_20mA()             GPIOA->BSRR = (u32)AIN2_4_20mA_MK_Pin;
#define OffTypeAin2_4_20mA()            GPIOA->BSRR = (u32)AIN2_4_20mA_MK_Pin << 16U;

#define DisableKey_0_10V(numAin)        if(numAin == 0)  {OffTypeAin1_0_10V();}   \
                                        else {OffTypeAin2_0_10V();}                      
                                        
#define DisableKey_4_20mA(numAin)       if(numAin == 0)  {OffTypeAin1_4_20mA();}  \
                                        else {OffTypeAin2_4_20mA();}                       
                                        
#define SetTypeAin1MK_0_10V()           OffTypeAin1_4_20mA();OnTypeAin1_0_10V();
#define SetTypeAin1MK_4_20mA()          OffTypeAin1_0_10V();OnTypeAin1_4_20mA();
#define SetTypeAin2MK_0_10V()           OffTypeAin2_4_20mA();OnTypeAin2_0_10V();
#define SetTypeAin2MK_4_20mA()          OffTypeAin2_0_10V();OnTypeAin2_4_20mA();
  
#define AdcCodeKZ_0_10V                 3950    // количестно кодов АЦП для детектирования аварии КЗ датчика 
#define AdcCodeKZ_4_20mA                3500    // количестно кодов АЦП для детектирования аварии КЗ датчика                          
#define AdcCodeHH                       100     // количестно кодов АЦП для детектирования аварии обрыва катчика 

// Калибровочные точки по входам
#define CalMaxValueAin_0_10V       1000 // 10 V
#define CalMinValueAin_0_10V       100  // 1V
#define CalMinValueAin_4_20mA      400
#define CalMaxValueAin_4_20mA      2000

// Диапазон сигнала после пересчета
#define MaxValueAin_0_10V       1000 // 10 V
#define MinValueAin_0_10V       0    // 0V
#define MinValueAin_4_20mA      400
#define MaxValueAin_4_20mA      2000
                                        
                                        
#define TimeEnableKeyAin                K_TIME_CALL_MICRO_TAKT * 2 // счетчик тактов МП (250 = 2 с)

void SetTypeAin(u16 typeAinUst, u8 numAin);

typedef struct{
  u16 Pnt_AdcCode;     //95 AdcCode
  u16 Pnt_Ain;         //96 Ain
  u16 Pnt_SensFailAin; //97 SensFailAin
}AinOprm_type;

// Режим аналогового входа
typedef enum{
    CalOff = 0,                // Калибровка отключена (штатный режим работы)
    CalMin,                    // Калибровка минимального значения 
    CalMax,                    // Калибровка максимального значения
}ModeAin_type;

typedef enum{
    AOtype_0_10B     = 0,               //!< Тип входа/выхода напряжение 0 - 10 В.
    AOtype_4_20mA    = 1,               //!< Тип входа/выхода ток 4 - 20 мА.
}AO_type;

typedef enum{
    ccain_1V        = 0,                //Код АЦП при калибровке 1 В.
    ccain_10V          ,                //Код АЦП при калибровке 10 В.
    ccain_4mA          ,                //Код АЦП при калибровке 4 мА.
    ccain_20mA         ,                //Код АЦП при калибровке 20 мА.
    ccain_res1         ,
    ccain_res2         ,
    ccain_CRC                           //CRC данных калибровки.
}CalibrCodeAin_type;

typedef enum{                           //!< Состояние датчика на аналоговом входе
    AinState_Ok     = 0,                 //!< Норма
    AinState_HH     = 1,                 //!< Обрыв
    AinState_KZ     = 2                  //!< Короткое замыкание
}AOstate_type;

// Описатель аналогового входа (для секции static) - блок СР24
typedef struct{
    s16                 AdcCode	;       // То, что снимают с АЦП
    AO_type	        AinType	;       // Тип аналового входа
    u16                 CntDisableAin;  // Счетчик тактов МП после КЗ затчика и выключения ключа
    u16                 AinState;       // Состояние аналогового входа( ОК, обрыв, КЗ)
    //u16	        CalibrCodeAin[5];
    u16	                CalibrCodeAin[7];
    s16                 AinValuePercent;   // Значение входа в процентах от типа входа, 1 знак после запятой
}AinMemInt_type;

typedef struct{
  u16 Pnt_TypeExtSlot;     
  u16 Pnt_ExtSlotFail;     
  u16 Pnt_ExtSlotCRC; 
  u16 Pnt_ExtSlotPO; 
}ExtSlotOpis_type;


/** Калибровочные константы аналоговых входов по умолчанию */
const u16 nvCalibrCodeAin_default[7] = {
      370,        //ccain_1V
      3700,     //ccain_10V
      620,      //ccain_4mA
      3050,     //ccain_20mA
      0,        // reserved for CP24B
      0,        // reserved for CP24B
      0         //ccain_CRC
};

//*********************************************************************************//
//******************************* Аналоговые выходы *******************************//
void SetTypeAout(u16 typeAoutUst, u8 numAout);

#define QuantityAout_CP24        2                               // количество аналоговых выходов платы CP_24         
#define TypeAout_0_10V           0          
#define TypeAout_4_20mA          1

#define OnTypeAout1_0_10V()              GPIOD->BSRR = (u32)AOUT_1_0_10V_MK_Pin;
#define OffTypeAout1_0_10V()             GPIOD->BSRR = (u32)AOUT_1_0_10V_MK_Pin << 16U;

#define OnTypeAout1_4_20mA()             GPIOD->BSRR = (u32)AOUT_1_4_20mA_MK_Pin;
#define OffTypeAout1_4_20mA()            GPIOD->BSRR = (u32)AOUT_1_4_20mA_MK_Pin << 16U;
#define OnTypeAout2_0_10V()              GPIOD->BSRR = (u32)AOUT_2_0_10V_MK_Pin;
#define OffTypeAout2_0_10V()             GPIOD->BSRR = (u32)AOUT_2_0_10V_MK_Pin << 16U;
#define OnTypeAout2_4_20mA()             GPIOD->BSRR = (u32)AOUT_2_4_20mA_MK_Pin;
#define OffTypeAout2_4_20mA()            GPIOD->BSRR = (u32)AOUT_2_4_20mA_MK_Pin << 16U;

#define SetTypeAout1MK_0_10V()           OffTypeAout1_4_20mA();OnTypeAout1_0_10V();
#define SetTypeAout1MK_4_20mA()          OffTypeAout1_0_10V();OnTypeAout1_4_20mA();
#define SetTypeAout2MK_0_10V()           OffTypeAout2_4_20mA();OnTypeAout2_0_10V();
#define SetTypeAout2MK_4_20mA()          OffTypeAout2_0_10V();OnTypeAout2_4_20mA();

#define MaxValueAout_0_10V              1000                                                    // 10.00 В
#define MinValueAout_0_10V              100                                                     // 1.00 В
#define MinValueAout_4_20mA             400                                             // 4,00 мА
#define MaxValueAout_4_20mA             2000                                            // 20,00 мА


// Режим аналогового выхода
typedef enum{
    CallAoutOff = 0,         // Калибровка отключена
    CallTestAout,            // Активен тест аналогового выхода
    CallMinAout,             // Калибровка по минимуму   
    CallMaxAout,             // Калибровка по максимуму
    ResetCallAout,             // Сброс калибровок аналогового выхода  
}ModeAout_type;

typedef enum{
    Code_0V        = 0,                 //калибровка 0 В.
    Code_10V          ,                 //калибровка 10 В.
    Code_4mA          ,                 //калибровка 4 мА.
    Code_20mA         ,                 //калибровка 20 мА.
    Code_CRC                            //CRC данных калибровки.
}CalibrCodeAout_type;
//*********************************************************************************//
/** Калибровочные константы аналоговых выходов по умолчанию */
const u16 nvCalibrCodeAout_default[5] = {
      900,       //!< Code_1V
      9030,     //!< Code_10V
      1525,     //!< Code_4mA
      7560,     //!< Code_20mA
      0         //ccain_CRC
};
//*********************************************************************************//
// Описатель аналогового выхода (для секции static) - CP24 внутренние Авых
typedef struct{
    s16         AoutType;                 //!< Тип аналогового выхода
    u16         FlagWrNv;
    u16         DacCode;
    u16	        CalibrCodeAout[5];
    float       ValAout;
    s16         AoutValPercent; // Значение аналогового выхода в % с 1 знаком после запятой
}AoutMemInt_type;
//*********************************************************************************//
typedef struct{
    u16 Pnt_TestAout;   //53 Тестовый сигнал для калибровки аналогового выхода
    u16 Pnt_RefAout;    //54 Входное значение Авых 0..1.0   -1..+1
    u16 Pnt_TypeAout;   //55 Тип аналогового выхода
    u16 Pnt_CalibAout;  //56 Команда на калибровку аналогового выхода
}RefTypeAout_type;
//*********************************************************************************//
typedef struct{
    u16 Pnt_FormatAout; //Формат
    u16 Pnt_ValAout;    //Значение Авых в вольтах и милиамперах
}FormatValAout_Type;
//*********************************************************************************//
#ifdef _PROJECT_FOR_STM32_
// Объявление для IAR
typedef struct{
    StatTdiscr_Enum Status;  // Состояние датчика
    u16 TimeCount;           // Счетчик для фильтра
}Tdiscr_Type;
#else
// Объявление для VISUAL 2005
typedef struct{
    u16 Status;              // Состояние датчика
    u16 TimeCount;           // Счетчик для фильтра
}Tdiscr_Type;
#endif

//*********************************************************************************//
typedef struct{
    AinMemInt_type  AinMemInt[5];
    AoutMemInt_type AoutMemInt[5];              //2
    AinMemExt_type  AinMemExt[5]; 
    AoutMemExt_type AoutMemExt[5];              //2
    s16          FiltrDinCnt[16];       //  счетчик для фильтрации DIN 
    u16          maskFiltrDin;          //  маска обработанных дискретных входов
    u16          cntErrMkar;            //!< Счётчик аварий связи или CRC с Mkar
//    u16          ModeAinPrev[3];       	//1  ModeAin1Prev ModeAin2Prev ModeAin3Prev
//    u16          ModeAoutPrev;         	//4  ModeAoutPrev

    u32          res1Cumlul;				//Для фильтра
    u32          res2Cumlul;				//Для фильтра
    u32          res3Cumlul;				//Для фильтра
    u16          p10v;
    u16          p4ma;
    u16          p5ma;
    u16          p20ma;
    u32          txCounter;
    Tdiscr_Type  Tdiscr;    
    u16          stateDrive;
    u16          timeAfterStop;
    u16          Fan_Enable_in_Work;
}StatePERIF_V2_type;

typedef struct{
    u16                Pnt_Nfunc;
    u16                Pnt_State;
    //u16                Pnt_nvState;
    u16                Pnt_Zar;            //1  Not Used (Zar)
    u16                Pnt_Start;          //2  Start
    u16                Pnt_LEDWifi;        //3  LEDWifi
    u16                Pnt_LEDFail;        //4  LEDFail
    u16                Pnt_LEDReady;       //5  LEDReady
    u16                Pnt_LEDRun;         //6  LEDRun
    u16                Pnt_Dout1_12;       //7  Dout1_12
    u16                Pnt_Inv_R[12];      //8...19 Inv_R1...Inv_R12
    u16                Pnt_Inv_Din[13];    //20...32 Inv_Din1...Inv_Din13
    u16                Pnt_Filtr_Din[13];  //33...45 Filtr_Din1...Filtr_Din13
    u16                Pnt_TypeAin[5];     //46...50 TypeAin1...TypeAin5
    u16                Pnt_KalibrAin[5];   //52...55 KalibrAin1...KalibrAin5
    RefTypeAout_type   ValTypeAout[4];     //56...71
    u16                Pnt_IgbtFanRef;     //72 IgbtFanRef
    u16                Pnt_IgbtFanTest;    //73 IgbtFanTest
    u16                Pnt_BoardFanRef;    //74 BoardFanRef
    u16                Pnt_BoardFanTest;   //75 BoardFanTest
    u16                Pnt_MkarFail;       //76 MkarFail
    u16                Pnt_BckPwrStat;     //77 BckPwrStat
    u16                Pnt_MkarCRC;        //78 MkarCRC
    u16                Pnt_MkarVerPO;      //79 MkarVerPO
    u16                Pnt_Rele[12];       //80...91 Rele1...Rele12
    u16                Pnt_Din1_12;        //92 Din1-13
    u16                Pnt_T_diskr;        //93 T_diskr
    u16                Pnt_Stop;           //94 Stop
    u16                Pnt_PTC;            //95 PTC
    u16                Pnt_Din[13];        //96...108 Din1..Din13
    AinOprm_type       AinOprm[5];         //109...123
    FormatValAout_Type FormatValAout[4];   //124...131
    u16                Pnt_End;
}FuncPERIF_V2_type;
void AdcCalc(FuncPERIF_V2_type *progPnt, u32 ramPnt, StatePERIF_V2_type *sPnt, nvAinCalState_type *nvAdr, s16 Num);
void AoutCalc(FuncPERIF_V2_type *progPnt, u32 ramPnt, StatePERIF_V2_type *sPnt,nvAoutCalState_type *nvAdr, s16 Num);
void Start_def(s16 val);

#define DelayWorkFAN    60*K_TIME_CALL_MICRO_TAKT // задержка работы вентилятора 60 секунд

//********************************************************
u16 *FuncPERIF_V2_1(FuncPERIF_V2_type *progPnt, u32 ramPnt)
{
#ifdef _PROJECT_FOR_STM32_
  

    StatePERIF_V2_type  *sPnt;
    nvAinCalState_type  *nvAdrAin;
    nvAoutCalState_type *nvAdrAout;
    StatTdiscr_Enum     StatTdiscr;    
    u16                 *valPnt, *invPnt,*filtrPnt,  maskDin, maskDout,  value, filtrDinUst, tekMaskState, tekDinExt;        
    u16                 TaskFanIGBT, TaskFanBoard, ValTestFanIgbt, ValTestFanBoard;
    u16                 StatBackupPow;
   

      
    sPnt	        = (StatePERIF_V2_type *)(ramPnt + progPnt->Pnt_State);
    nvAdrAin            = (nvAinCalState_type* )&nv.AInput.CalibrCode;
    nvAdrAout           = (nvAoutCalState_type* )&nv.AOutput.CalibrCode;
        
    if(flagIsFirstCall)                                         
    {      
//      // Тестовая оценка адреса калибровочных значений.
//      // Зная что сдвиг у меня сейчас на 4 байта, сделаем сканирование на -8....+8 байт, с шагом в 2 байта.
//      #if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
//        GlobalM4.nvCalibrOffset = -8;
//        GetNvOffset = 0;
//        
//        for (GlobalM4.nvCalibrOffset = -8; GlobalM4.nvCalibrOffset <= 8; GlobalM4.nvCalibrOffset += 2){
//          numAin = 0; // Тест по самому первому элементу
//          nvAdrAinTest = (nvAinCalState_type *)((s32)nvAdrAin + GlobalM4.nvCalibrOffset);
//          RdFramMicro(sPnt->AinMemInt[numAin].CalibrCodeAin, nvAdrAinTest->CalibrCode[numAin], sizeof(nvAdrAin->CalibrCode[0]));	
//          if(!Crc((unsigned char *)(sPnt->AinMemInt[numAin].CalibrCodeAin), sizeof(nvAdrAin->CalibrCode[0]))) {
//            GetNvOffset = 1;  
//            break; //Найдено правильное смещение
//          }
//        } 
//        // Если цикл закончен и ничего не найдено, значит данных нет, или смещение слишком большое.
//        if (!GetNvOffset)   GlobalM4.nvCalibrOffset = 0;
//      #endif  // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
//        
//      #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
//        GlobalM4.nvCalibrOffset = -8;
//        GetNvOffset = 0;
//        
//        for (GlobalM4.nvCalibrOffset = -8; GlobalM4.nvCalibrOffset <= 8; GlobalM4.nvCalibrOffset += 2){
//          numAin = 0; // Тест по самому первому элементу
//          nvAdrAinTest = (nvAinCalState_type *)((s32)nvAdrAin + GlobalM4.nvCalibrOffset);
//          RdFramMicro((void*)&sPnt->AinMemExt[numAin].Calibr, nvAdrAinTest->CalibrCode[numAin], sizeof(AinCalibr_type));	
//          if(!Crc((u8*)(&sPnt->AinMemExt[numAin].Calibr), sizeof(AinCalibr_type))) {
//            GetNvOffset = 1;  
//            break; //Найдено правильное смещение
//          }
//        } 
//        // Если цикл закончен и ничего не найдено, значит данных нет, или смещение слишком большое.
//        if (!GetNvOffset)   GlobalM4.nvCalibrOffset = 0;                     
//      #endif  // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
//         
//      // Правим смещение адресов калибровок 
//      nvAdrAin            = (nvAinCalState_type *)((s32)nvAdrAin + GlobalM4.nvCalibrOffset);
//      nvAdrAout           = (nvAoutCalState_type *)((s32)nvAdrAout + GlobalM4.nvCalibrOffset);    
//        
      // Получить состояние дискретных входов
      tekMaskState = DinDout.getDin(&DinDout);
      sPnt->maskFiltrDin = tekMaskState;                                // чтобы убрать ложные срабатывания по подаче питания
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      #if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
//      for(u8 numAin = 0; numAin < QuantityAin_CP24; numAin++)           // вычитать калибровки аналоговых выходов
//      {
//        RdFramMicro(sPnt->AinMemInt[numAin].CalibrCodeAin, nvAdrAin->CalibrCode[numAin], sizeof(nvAdrAin->CalibrCode[0])); // Размеры надо учитывать исходя из калибровок для СР24, здесь они 5 байт	
//        if(Crc((unsigned char *)(sPnt->AinMemInt[numAin].CalibrCodeAin), sizeof(nvAdrAin->CalibrCode[0])))                                                                   // Если ошибка crc 
//        { 
//            memcpy(sPnt->AinMemInt[numAin].CalibrCodeAin, nvCalibrCodeAin_default, sizeof(sPnt->AinMemInt[0].CalibrCodeAin));                                                         // Записываем значения калибровочных коэффициентов по умолчанию
//            sPnt->AinMemInt[numAin].CalibrCodeAin[ccain_CRC] = Crc((unsigned char *)(sPnt->AinMemInt[numAin].CalibrCodeAin), sizeof(sPnt->AinMemInt[0].CalibrCodeAin) - sizeof(u16));    // Записываем значение crc
//            if(sizeof(nv.AInput.CalibrCode) == sizeof(nvAdrAin->CalibrCode))                                                                                              // Размерности должны совпадать чтобы не затерать чужие данные
//              WrFramMicro(nvAdrAin->CalibrCode[numAin], sPnt->AinMemInt[numAin].CalibrCodeAin, sizeof(sPnt->AinMemInt[0].CalibrCodeAin));	
//        }
//      } 
      // Старый вариант с работой по адресам nv
      for(u8 numAin = 0; numAin < QuantityAin_CP24; numAin++)           // вычитать калибровки аналоговых выходов
      {
        RdFramMicro(sPnt->AinMemInt[numAin].CalibrCodeAin, nvAdrAin->CalibrCode[numAin], sizeof(nvAdrAin->CalibrCode[0]));	
        if(Crc((unsigned char *)(sPnt->AinMemInt[numAin].CalibrCodeAin), sizeof(nvAdrAin->CalibrCode[0])))                                                                   // Если ошибка crc 
        { 
            memcpy(sPnt->AinMemInt[numAin].CalibrCodeAin, nvCalibrCodeAin_default, sizeof(nvAdrAin->CalibrCode[0]));                                                         // Записываем значения калибровочных коэффициентов по умолчанию
            sPnt->AinMemInt[numAin].CalibrCodeAin[ccain_CRC] = Crc((unsigned char *)(sPnt->AinMemInt[numAin].CalibrCodeAin), sizeof(nvAdrAin->CalibrCode[0]) - sizeof(u16));    // Записываем значение crc
            if(sizeof(nv.AInput.CalibrCode) == sizeof(nvAdrAin->CalibrCode))    // Глупая проверка!!! Это одно и тоже                                                                                          // Размерности должны совпадать чтобы не затерать чужие данные
              WrFramMicro(nvAdrAin->CalibrCode[numAin], sPnt->AinMemInt[numAin].CalibrCodeAin, sizeof(nvAdrAin->CalibrCode[0]));	
        }
      }      
      for(u8 numAout = 0; numAout < QuantityAout_CP24; numAout++)       // вычитать калибровки аналоговых выходов
      {
        RdFramMicro(sPnt->AoutMemInt[numAout].CalibrCodeAout, nvAdrAout->CalibrCode[numAout], sizeof(nvAdrAout->CalibrCode[0]));		// Вычитываем калибровки аналоговых выходов
        if(Crc((unsigned char *)(sPnt->AoutMemInt[numAout].CalibrCodeAout), sizeof(nvAdrAout->CalibrCode[0])))                                                                   // Если ошибка crc 
        {
          memcpy(sPnt->AoutMemInt[numAout].CalibrCodeAout, nvCalibrCodeAout_default, sizeof(nvAdrAout->CalibrCode[0])); 
          sPnt->AoutMemInt[numAout].CalibrCodeAout[Code_CRC] = Crc((unsigned char *)(sPnt->AoutMemInt[numAout].CalibrCodeAout), sizeof(nvAdrAout->CalibrCode[0]) - sizeof(u16));
          if(sizeof(nv.AOutput.CalibrCode) == sizeof(nvAdrAout->CalibrCode))                                            // Размерности должны совпадать чтобы не затерать чужие данные
             WrFramMicro(nvAdrAout->CalibrCode[numAout], sPnt->AoutMemInt[numAout].CalibrCodeAout, sizeof(nvAdrAout->CalibrCode[0]));	
        }
      }
      
      // Первичный вызов аналогового входа, для конфигурирования типа входа
      sPnt->AinMemInt[0].AdcCode = 0;     // Аналоговый вход 1 
      sPnt->AinMemInt[1].AdcCode = 0;     // Аналоговый вход 2 
      for(u8 numAin = 0; numAin < QuantityAin_CP24; numAin++) {
        AdcCalc(progPnt, ramPnt, sPnt, nvAdrAin, numAin);
      }
      // Первичный вызов аналогового выхода, для конфигурирования типа выхода
      for(u8 numAout = 0; numAout < QuantityAout_CP24; numAout++) {
        AoutCalc(progPnt, ramPnt, sPnt, nvAdrAout, numAout);
      }            
      #endif  // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
      // Проверка коеффициентов калибровки Ain хранимых в FRAM
      for(u8 numAin = 0; numAin < QuantityAin_CP24; numAin++){           // вычитать калибровки аналоговых выходов
        // Прочитать калибровки одного аналогового выхода
        RdFramMicro((void*)&sPnt->AinMemExt[numAin].Calibr, nvAdrAin->CalibrCode[numAin], sizeof(AinCalibr_type));
	
        // Проверить crc
        if(Crc((u8*)(&sPnt->AinMemExt[numAin].Calibr), sizeof(AinCalibr_type))){ 
            sPnt->AinMemExt[numAin].Calibr.Val1V = CALIBR_AIN_DEF_1V;       // Коеффициент калибровки для точки 0 В.
            sPnt->AinMemExt[numAin].Calibr.Val10V = CALIBR_AIN_DEF_10V;     // Коеффициент калибровки для точки 10 В.
            sPnt->AinMemExt[numAin].Calibr.Val4mA = CALIBR_AIN_DEF_4MA;     // Коеффициент калибровки по умолчанию для точки 4ma
            sPnt->AinMemExt[numAin].Calibr.Val20mA = CALIBR_AIN_DEF_20MA;   // Коеффициент калибровки по умолчанию для точки 20ma
            sPnt->AinMemExt[numAin].Calibr.Val4mA = CALIBR_AIN_DEF__50С;    // Коеффициент калибровки по умолчанию для точки -50С
            sPnt->AinMemExt[numAin].Calibr.Val20mA = CALIBR_AIN_DEF_200С;   // Коеффициент калибровки по умолчанию для точки 200С
            
            // crc коеффициенты колибровок по умолчанию
            sPnt->AinMemExt[numAin].Calibr.crc = Crc((u8*)(&sPnt->AinMemExt[numAin].Calibr), sizeof(AinCalibr_type) - sizeof(u16));
          
            // Сохранить коеффициенты калибровки в FRAM
            if(sizeof(nv.AInput.CalibrCode[numAin]) == sizeof(AinCalibr_type)){                                                        // Размерности должны совпадать чтобы не затерать чужие данные
               WrFramMicro(nvAdrAin->CalibrCode[numAin], &sPnt->AinMemExt[numAin].Calibr, sizeof(AinCalibr_type));
            }
        }
      } 
      
      // Проверка коеффициентов калибровки Aout хранимых в FRAM
      for(u8 numAout = 0; numAout < QuantityAout_CP24; numAout++){
         
         // Прочитать калибровки одного аналогового выхода
         RdFramMicro((void*)&sPnt->AoutMemExt[numAout].Calibr, nvAdrAout->CalibrCode[numAout], sizeof(AoutCalibr_type));		// Вычитываем калибровки аналоговых выходов
         
         // Проверить crc
         if(Crc((u8*)(&sPnt->AoutMemExt[numAout].Calibr), sizeof(AoutCalibr_type))){
            // Калибровки во FRAM не корректны!!!
            // Значит берем калибровки по умолчанию!!!
            sPnt->AoutMemExt[numAout].Calibr.Val1V = CALIBR_AOUT_DEF_1V;       // Коеффициент калибровки для точки 0 В.
            sPnt->AoutMemExt[numAout].Calibr.Val10V = CALIBR_AOUT_DEF_10V;     // Коеффициент калибровки для точки 10 В.
            sPnt->AoutMemExt[numAout].Calibr.Val4mA = CALIBR_AOUT_DEF_4MA;     // Коеффициент калибровки по умолчанию для точки 4ma
            sPnt->AoutMemExt[numAout].Calibr.Val20mA = CALIBR_AOUT_DEF_20MA;   // Коеффициент калибровки по умолчанию для точки 20ma
            // crc коеффициенты колибровок по умолчанию
            sPnt->AoutMemExt[numAout].Calibr.crc = Crc((u8*)(&sPnt->AoutMemExt[numAout].Calibr), sizeof(AoutCalibr_type) - sizeof(u16));
            
            // Сохранить коеффициенты калибровки в FRAM
            if(sizeof(nv.AOutput.CalibrCode[numAout]) == sizeof(AoutCalibr_type)){                                            // Размерности должны совпадать чтобы не затерать чужие данные
                WrFramMicro(nvAdrAout->CalibrCode[numAout], &sPnt->AoutMemExt[numAout].Calibr, sizeof(AoutCalibr_type));
            }
        }
      }      
      #endif  // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

      // Получить состояние дискретных входов
      tekMaskState = DinDout.getDin(&DinDout); 
//      tekDinExt = (0xFFFF ^ GlobalM4.DinDoutExtData.DinState.dinStateWord);
//      tekMaskState = tekMaskState | (tekDinExt << 7);      
      
      invPnt	= progPnt->Pnt_Inv_Din;
      valPnt	= progPnt->Pnt_Din;
      filtrPnt	= progPnt->Pnt_Filtr_Din;
      maskDin = value = 0;
            
      for(u8 numDin = 0; numDin < QuantityDins; numDin++, valPnt++, invPnt++,filtrPnt++)
      { // Вычитываем дискретные сигналы первый раз без фильтрации
        if(tekMaskState & (1 << numDin)) {                          // если на входе активный сигнал ( лог "1")
            sPnt->maskFiltrDin |= (1 << numDin);                  // выставляем логическую "1" в маску дискретных входов
        } else {
          sPnt->maskFiltrDin &=  (~(1 << numDin));              // выставляем логический "0" в маску дискретных входов
        }
        
        if(numDin < QuantityDins_CP24)                            // если входы "свои" (платы CP24)
          value = (sPnt->maskFiltrDin & (1 << numDin)) ? 0:1;     // состояние дискретного входа (на схеме если не замыкать контакты, то "О", при замыкании "1")
        else
          value = (sPnt->maskFiltrDin & (1 << numDin)) ? 1:0;     // состояние дискретного входа
          
          value = (value ^ load_s16(ramPnt, *invPnt));              // инверсия входа,  если она есть.
          maskDin |= (value << numDin);                             // заполнения битовой маски состояния дискретных входов с учетом инрерсии
          
          save_s16(ramPnt, *valPnt, value);                         // запись сстояния дискретного входа
      }
      save_s16(ramPnt, progPnt->Pnt_Din1_12, maskDin);            //  запись значения всех дискретных входов в виде маски      
      
      return &progPnt->Pnt_End;
    }  // flagIsFirstCall      
//********************************************************************************//    
//****************************** ДИСКРЕТНЫЕ ВХОДЫ ******************************//
    
    // Получить состояние дискретных входов
    tekMaskState = DinDout.getDin(&DinDout); // Бортовые сигналы с железа приходят инверсными
    tekDinExt = (0xFFFF ^ GlobalM4.DinDoutExtData.DinState.dinStateWord);    
    tekMaskState = tekMaskState | (tekDinExt << 7);
    
    invPnt	= progPnt->Pnt_Inv_Din;
    valPnt	= progPnt->Pnt_Din;
    filtrPnt	= progPnt->Pnt_Filtr_Din;
    maskDin = value = 0;
    

    for(u8 numDin = 0; numDin < QuantityDins; numDin++, valPnt++, invPnt++,filtrPnt++)
    {
      filtrDinUst = load_s16(ramPnt, *filtrPnt);                // значения уставки фильтрации
      filtrDinUst = tblFiltrDin[filtrDinUst];                   // пересчет на такты микропрограммы
      
      if(tekMaskState & (1 << numDin))                          // если на входе активный сигнал ( лог "1")
      {
        if(sPnt->FiltrDinCnt[numDin] < filtrDinUst)                         
          sPnt->FiltrDinCnt[numDin]++;                          // увеличения счетчика для фильтрации
        else
          sPnt->maskFiltrDin |= (1 << numDin);                  // выставляем логическую "1" в маску дискретных входов
      }
      else if(sPnt->FiltrDinCnt[numDin] > 0)                    // если на входе логичиский "0"
          sPnt->FiltrDinCnt[numDin]--;                          // уменьшения счетчика
      else
          sPnt->maskFiltrDin &=  (~(1 << numDin));              // выставляем логический "0" в маску дискретных входов
      
//      if(numDin < QuantityDins_CP24)                            // если входы "свои" (платы CP24)
        value = (sPnt->maskFiltrDin & (1 << numDin)) ? 0:1;     // состояние дискретного входа (на схеме если не замыкать контакты, то "О", при замыкании "1")
//      else
//        value = (sPnt->maskFiltrDin & (1 << numDin)) ? 1:0;     // состояние дискретного входа
          
      value = (value ^ load_s16(ramPnt, *invPnt));              // инверсия входа,  если она есть.
      maskDin |= (value << numDin);                             // заполнения битовой маски состояния дискретных входов с учетом инрерсии
      
      save_s16(ramPnt, *valPnt, value);                         // запись сстояния дискретного входа
    }
    save_s16(ramPnt, progPnt->Pnt_Din1_12, maskDin);            //  запись значения всех дискретных входов в виде маски
    
    // переменные для вывода в логер
    GlobalM4.Din1 = (maskDin & (1<<0)) ? 1 : 0;
    GlobalM4.Din2 = (maskDin & (1<<1)) ? 1 : 0;
    GlobalM4.Din3 = (maskDin & (1<<2)) ? 1 : 0;
    GlobalM4.Din4 = (maskDin & (1<<3)) ? 1 : 0;
    GlobalM4.Din5 = (maskDin & (1<<4)) ? 1 : 0;
    GlobalM4.Din6 = (maskDin & (1<<5)) ? 1 : 0;
    GlobalM4.Din7 = (maskDin & (1<<6)) ? 1 : 0;
    GlobalM4.Din8 = (maskDin & (1<<7)) ? 1 : 0;
    GlobalM4.Din9 = (maskDin & (1<<8)) ? 1 : 0;    
    GlobalM4.Din10 = (maskDin & (1<<9)) ? 1 : 0;    
    GlobalM4.Din11 = (maskDin & (1<<10)) ? 1 : 0;    
    GlobalM4.Din12 = (maskDin & (1<<11)) ? 1 : 0;    
    GlobalM4.Din13 = (maskDin & (1<<12)) ? 1 : 0;        
//********************************************************************************//    
//****************************** ДИСКРЕТНЫЕ ВЫХОДЫ *******************************//
    
    tekMaskState        = load_s16(ramPnt, progPnt->Pnt_Dout1_12);
    valPnt	        = progPnt->Pnt_Rele;
    invPnt	        = progPnt->Pnt_Inv_R;
    maskDout = 0;
    for(u8 numDout = 0; numDout < QuantityDouts; numDout++, valPnt++, invPnt++)
    {
        value = (tekMaskState & (1 << numDout)) ? 1:0;          // определение задания состояния выхода
        value = (value ^ load_s16(ramPnt, *invPnt));            // инверсия значения выхода
        maskDout |= (value << numDout);                         // формирования маски с учетом инверсии
        save_s16(ramPnt, *valPnt, value);                       // запись состояния дискретного выхода
    }
    // Установить дискретные выходы
    DinDout.DoutMask = maskDout;
    DinDout.setDout(&DinDout);
    
    // переменные для вывода в логер
    GlobalM4.Dout1 = (maskDout & (1<<0)) ? 1 : 0;
    GlobalM4.Dout2 = (maskDout & (1<<1)) ? 1 : 0;
    GlobalM4.Dout3 = (maskDout & (1<<2)) ? 1 : 0;
    GlobalM4.Dout4 = (maskDout & (1<<3)) ? 1 : 0;
    GlobalM4.Dout5 = (maskDout & (1<<4)) ? 1 : 0;
//****************************************************************************//    
//***************** Управление зарядным резистором и его реле ****************//       
    Charge.ReleState = (GPIO_PinState)testBitMicro(ramPnt, progPnt->Pnt_Start);
    Charge.ZarState = (GPIO_PinState)testBitMicro(ramPnt, progPnt->Pnt_Zar);    
    Charge.set(&Charge);            

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -         
    #if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1) 
    
    AinMemInt_type 	*mPnt;
    AoutMemInt_type 	*aoPnt;
    //********************************************************************************//    
    //****************************** АНАЛОГОВЫЕ ВХОДЫ ********************************// 
    
    sPnt->AinMemInt[0].AdcCode = GlobalM4.LowSpeedAinAdc.Ain1;     // Аналоговый вход 1 
    sPnt->AinMemInt[1].AdcCode = GlobalM4.LowSpeedAinAdc.Ain2;     // Аналоговый вход 2 
    for(u8 numAin = 0; numAin < QuantityAin_CP24; numAin++)
    {
      AdcCalc(progPnt, ramPnt, sPnt, nvAdrAin, numAin);
    }
      
    // Копируем данные для вывода на логер
    mPnt = &(sPnt->AinMemInt[0]);
    GlobalM4.Ain1 = mPnt->AinValuePercent; // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой
    mPnt = &(sPnt->AinMemInt[1]);
    GlobalM4.Ain2 = mPnt->AinValuePercent; // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой

    aoPnt = &(sPnt->AoutMemInt[0]);
    GlobalM4.Aout1 = aoPnt->AoutValPercent; // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой
    aoPnt = &(sPnt->AoutMemInt[1]);
    GlobalM4.Aout2 = aoPnt->AoutValPercent; // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой

    
    //********************************************************************************//    
    //****************************** АНАЛОГОВЫЕ ВЫХОДЫ *******************************//
    for(u8 numAout = 0; numAout < QuantityAout_CP24; numAout++)
    {
      AoutCalc(progPnt, ramPnt, sPnt, nvAdrAout, numAout);
    }
    #endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -          
        
    
    //*************** Связь с внешним блоком расширения AIO2 ********************//    
    sPnt->AinMemExt[2].AdcVal = GlobalM4.AinAoutExtData.Adc1Val;
    sPnt->AinMemExt[3].AdcVal = GlobalM4.AinAoutExtData.Adc2Val;
    float ValAin1 = (s16)GlobalM4.AinAoutExtData.Ain1Val/1000.0f;
    float ValAin2 = (s16)GlobalM4.AinAoutExtData.Ain2Val/1000.0f;
    u16 Adc1Val = GlobalM4.AinAoutExtData.Adc1Val;
    u16 Adc2Val = GlobalM4.AinAoutExtData.Adc2Val;
    AinState_Enum    AinState1 = GlobalM4.AinAoutExtData.AinState1;
    AinState_Enum    AinState2 = GlobalM4.AinAoutExtData.AinState2;
     
    save_s16(ramPnt, progPnt->AinOprm[2].Pnt_AdcCode, Adc1Val);
    save_s16(ramPnt, progPnt->AinOprm[3].Pnt_AdcCode, Adc2Val);
    save_float(ramPnt, progPnt->AinOprm[2].Pnt_Ain, ValAin1);
    save_float(ramPnt, progPnt->AinOprm[3].Pnt_Ain, ValAin2);
        
    // Данные для логера Ain3 / Ain4
    GlobalM4.Ain3 = (s16)(ValAin1*1000.0f); // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой
    GlobalM4.Ain4 = (s16)(ValAin2*1000.0f); // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой        
    
    /*     Записываем состояние входа 3 - 4             */
    //defBitMicro(ramPnt, progPnt->AinOprm[2].Pnt_SensFailAin, AinState1);
    save_s16(ramPnt, progPnt->AinOprm[2].Pnt_SensFailAin, AinState1);
    save_s16(ramPnt, progPnt->AinOprm[3].Pnt_SensFailAin, AinState2);
    //defBitMicro(ramPnt, progPnt->AinOprm[3].Pnt_SensFailAin, AinState2);
    
     StatusWordExtType StatusCalibr;
      // Аналоговый вход блока расширения
    GlobalM4.AinAoutExtData.Ain1Type = (AinExtType_Enum)(load_s16(ramPnt, progPnt->Pnt_TypeAin[2]) + 1);  
    GlobalM4.AinAoutExtData.Ain2Type = (AinExtType_Enum)(load_s16(ramPnt, progPnt->Pnt_TypeAin[3]) + 1);  
     // Аналоговый выход для блока расширения
    GlobalM4.AinAoutExtData.Aout1ExtType = (AoutExtType_Enum)(load_s16(ramPnt, progPnt->ValTypeAout[2].Pnt_TypeAout) + 1);    // Тип Aout1  
    GlobalM4.AinAoutExtData.Aout2ExtType = (AoutExtType_Enum)(load_s16(ramPnt, progPnt->ValTypeAout[3].Pnt_TypeAout) + 1);    // Тип Aout1  
    
      // Вычитываем команду на калибровку для блока расширения 
    StatusCalibr.bits.calibrAin3 = load_s16(ramPnt, progPnt->Pnt_KalibrAin[2]);
    save_s16(ramPnt, progPnt->Pnt_KalibrAin[2], 0);
    StatusCalibr.bits.calibrAin4 = load_s16(ramPnt, progPnt->Pnt_KalibrAin[3]);
    save_s16(ramPnt, progPnt->Pnt_KalibrAin[3], 0);
    StatusCalibr.bits.calibrAout3 = load_s16(ramPnt, progPnt->ValTypeAout[2].Pnt_CalibAout);
    if(StatusCalibr.bits.calibrAout3 != CallAoutOff)
      save_s16(ramPnt, progPnt->ValTypeAout[2].Pnt_CalibAout, CallTestAout);
    
    StatusCalibr.bits.calibrAout4 = load_s16(ramPnt, progPnt->ValTypeAout[3].Pnt_CalibAout);
    if(StatusCalibr.bits.calibrAout4 != CallAoutOff)
      save_s16(ramPnt, progPnt->ValTypeAout[3].Pnt_CalibAout, CallTestAout);
    
    GlobalM4.AinAoutExtData.Status = StatusCalibr.word;
    
    u16 ValAout1 = GlobalM4.AinAoutExtData.Aout1Val;
    u16 ValAout2 = GlobalM4.AinAoutExtData.Aout2Val;
    // Данные для логера Aout3/4
    GlobalM4.Aout3 = (s16)(ValAout1); // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой
    GlobalM4.Aout4 = (s16)(ValAout2); // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой    
    
     // Задание Аналоговый выход для блока расширения
    if(StatusCalibr.bits.calibrAout3 == CallTestAout){
       GlobalM4.AinAoutExtData.TaskAout1 = load_s16(ramPnt, progPnt->ValTypeAout[2].Pnt_TestAout);    // Тип Aout1  //sPnt->AoutMemExt[2].DacCode;            //Задание для блока расширения
       save_s16(ramPnt, progPnt->FormatValAout[2].Pnt_ValAout, 0);
    }else if(StatusCalibr.bits.calibrAout3 == CallAoutOff){
       GlobalM4.AinAoutExtData.TaskAout1 = (u16)(load_float(ramPnt, progPnt->ValTypeAout[2].Pnt_RefAout) * 1000.0f); 
       save_s16(ramPnt, progPnt->FormatValAout[2].Pnt_ValAout, ValAout1);
    }
    if(StatusCalibr.bits.calibrAout4 == CallTestAout){
       GlobalM4.AinAoutExtData.TaskAout2 = load_s16(ramPnt, progPnt->ValTypeAout[3].Pnt_TestAout);    // Тип Aout1  //sPnt->AoutMemExt[2].DacCode;            //Задание для блока расширения
       save_s16(ramPnt, progPnt->FormatValAout[3].Pnt_ValAout, 0);
    }else if(StatusCalibr.bits.calibrAout4 == CallAoutOff){
       GlobalM4.AinAoutExtData.TaskAout2 = (u16)(load_float(ramPnt, progPnt->ValTypeAout[3].Pnt_RefAout) * 1000.0f); 
       save_s16(ramPnt, progPnt->FormatValAout[3].Pnt_ValAout, ValAout2);
    }

    u16 format;
    u16 TestValue = load_s16(ramPnt, progPnt->ValTypeAout[2].Pnt_TestAout);
    
    switch(GlobalM4.AinAoutExtData.Aout1ExtType){
      case Aout0_10v:
          format        = (2 << 8) | VRasm;        // Формат
          if(StatusCalibr.bits.calibrAout3 == CallMinAout)
             TestValue = MinValueAout_0_10V;
          if(StatusCalibr.bits.calibrAout3 == CallMaxAout)
             TestValue = MaxValueAout_0_10V;
      break;
      case Aout4_20ma:
          format        = (2 << 8) | mARasm;        // Формат
          if(StatusCalibr.bits.calibrAout3 == CallMinAout)
             TestValue = MinValueAout_4_20mA;
          if(StatusCalibr.bits.calibrAout3 == CallMaxAout)
             TestValue = MaxValueAout_4_20mA;
      break;
    }
    save_s16(ramPnt, progPnt->FormatValAout[2].Pnt_FormatAout, (s16)format);
    save_s16(ramPnt, progPnt->ValTypeAout[2].Pnt_TestAout, TestValue);
    
    TestValue = load_s16(ramPnt, progPnt->ValTypeAout[3].Pnt_TestAout);
    switch(GlobalM4.AinAoutExtData.Aout2ExtType){
      case Aout0_10v:
          format        = (2 << 8) | VRasm;        // Формат
          if(StatusCalibr.bits.calibrAout4 == CallMinAout)
             TestValue = MinValueAout_0_10V;
          if(StatusCalibr.bits.calibrAout4 == CallMaxAout)
             TestValue = MaxValueAout_0_10V;
      break;
      case Aout4_20ma:
          format        = (2 << 8) | mARasm;        // Формат
          if(StatusCalibr.bits.calibrAout4 == CallMinAout)
             TestValue = MinValueAout_4_20mA;
          if(StatusCalibr.bits.calibrAout4 == CallMaxAout)
             TestValue = MaxValueAout_4_20mA;
      break;
    }
    
    save_s16(ramPnt, progPnt->FormatValAout[3].Pnt_FormatAout, (s16)format);
    save_s16(ramPnt, progPnt->ValTypeAout[3].Pnt_TestAout, TestValue);
    
    
    
     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -          
    #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    //********************************************************************************//    
    //****************************** АНАЛОГОВЫЕ ВХОДЫ ********************************//     
    sPnt->AinMemExt[0].AdcVal = Mkar.DataRx->Ain1Val;          // Аналоговый вход 1 
    sPnt->AinMemExt[1].AdcVal = Mkar.DataRx->Ain2Val;          // Аналоговый вход 2 
    sPnt->AinMemExt[0].AinStatus = Mkar.DataRx->AinState1;
    sPnt->AinMemExt[1].AinStatus  = Mkar.DataRx->AinState2;
    for(u8 numAin = 0; numAin < QuantityAin_CP24; numAin++){
      AdcCalc(progPnt, ramPnt, sPnt, nvAdrAin, numAin);
    }
    //********************************************************************************//    
    //****************************** АНАЛОГОВЫЕ ВЫХОДЫ *******************************//
    for(u8 numAout = 0; numAout < QuantityAout_CP24; numAout++){
      AoutCalc(progPnt, ramPnt, sPnt, nvAdrAout, numAout);
    }
    
    //****************************************************************************//    
    // Данные для логера Ain1 / Ain2
    AinMemExt_type 	*mPnt;
    AoutMemExt_type 	*aoPnt;
    
    mPnt = &(sPnt->AinMemExt[0]);
    GlobalM4.Ain1 = mPnt->AinValuePercent; // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой
    mPnt = &(sPnt->AinMemExt[1]);
    GlobalM4.Ain2 = mPnt->AinValuePercent; // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой        
    
    aoPnt = &(sPnt->AoutMemExt[0]);
    GlobalM4.Aout1 = aoPnt->AoutValPercent; // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой
    aoPnt = &(sPnt->AoutMemExt[1]);
    GlobalM4.Aout2 = aoPnt->AoutValPercent; // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой    
    //****************************************************************************//    
    
    //****************************************************************************//    
    //*************** Связь с внешним аналоговым контроллером ********************//
          
    // Аналоговый вход
    Mkar.DataTx->Ain1Type = sPnt->AinMemExt[0].Type;      
    Mkar.DataTx->Ain2Type = sPnt->AinMemExt[1].Type;      
   
    // Аналоговый выход
    Mkar.DataTx->Aout1ExtType = sPnt->AoutMemExt[0].Type;   // Тип Aout1  
    Mkar.DataTx->Aout2ExtType = sPnt->AoutMemExt[1].Type;    // Тип Aout2 
   
    Mkar.DataTx->TaskAout1 = sPnt->AoutMemExt[0].DacCode;    // Задание для Aout1 
    Mkar.DataTx->TaskAout2 = sPnt->AoutMemExt[1].DacCode;    // Задание для Aout2
     
    Mkar.hand(&Mkar);    
    
    // Проверим наличие связи с МКАР
    if(Mkar.MkarLinkStat == MkarOkLink){
       save_s16(ramPnt, progPnt->Pnt_MkarCRC, Mkar.DataRx->CrcPO);
       save_s16(ramPnt, progPnt->Pnt_MkarVerPO, Mkar.DataRx->VerPO);       
    }
    else{
       save_s16(ramPnt, progPnt->Pnt_MkarCRC, 0);
       save_s16(ramPnt, progPnt->Pnt_MkarVerPO, 0);       
    }    
    #endif  // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -      
    
    
//********************************************************************************//    
//**************************** Управление вентилятором ***************************//   
    // Считываем актуальные задания вентиляторов
    ValTestFanIgbt = load_s16(ramPnt, progPnt->Pnt_IgbtFanTest);
    ValTestFanBoard = load_s16(ramPnt, progPnt->Pnt_BoardFanTest);
    TaskFanIGBT = load_s16(ramPnt, progPnt->Pnt_IgbtFanRef);
    TaskFanBoard = load_s16(ramPnt, progPnt->Pnt_BoardFanRef);
        
    if(sPnt->timeAfterStop)
        sPnt->timeAfterStop--;
    
    if(sPnt->stateDrive != GlobalM4.FlagCommand.bit.PwmOn){
        sPnt->stateDrive = GlobalM4.FlagCommand.bit.PwmOn;
          if(!sPnt->stateDrive)                               // если был останов, то заводим таймер на 1 минуту работы вентилятора
              sPnt->timeAfterStop = DelayWorkFAN;
          else
              sPnt->timeAfterStop = 0;
    }
    
    if(GlobalM4.FlagCommand.bit.ChargeOk) {        // если Ud заряжено                             
        if( GlobalM4.FlagCommand.bit.PwmOn ){
            // Разрешаем работу вентилятора          
            sPnt->Fan_Enable_in_Work = 1;
        }
        else if((sPnt->timeAfterStop) && (sPnt->Fan_Enable_in_Work)){
            sPnt->Fan_Enable_in_Work = 1; // После останова работаем еще 1 минуту            
        }
        else if (!GlobalM4.FlagCommand.bit.PwmOn){              // Вентилятор выключается только после СТОПА ПЧ и снижения температуры, гистерезис 1 градус             
             sPnt->timeAfterStop = sPnt->Fan_Enable_in_Work = 0;
        }
    }
    else{ // Иначе если проблема с сетью - отключить вентилятор
        sPnt->timeAfterStop = sPnt->Fan_Enable_in_Work = 0;
        ValTestFanIgbt = ValTestFanBoard = 0;
    }   
    
    // Проверка тестового режима работы
    if(ValTestFanIgbt == 0){
      // Если вентиль в работе 
      if (sPnt->Fan_Enable_in_Work)     FanControl.IgbtFanTask = ((float)TaskFanIGBT) * 0.1f;
      else                              FanControl.IgbtFanTask = 0.0f;
    } else {
      if (GlobalM4.FlagCommand.bit.ChargeOk) { // Тестовый режим разрешен только при нормальном заряде
        FanControl.IgbtFanTask = ((float)ValTestFanIgbt) * 0.1f;
      }
    }    
    
    if(ValTestFanBoard == 0){
      // Если вентиль в работе 
      if (sPnt->Fan_Enable_in_Work) FanControl.BoardFanTask = ((float)TaskFanBoard) * 0.1f;       
      else                          FanControl.BoardFanTask = 0.0f;                   
    } else {
      if (GlobalM4.FlagCommand.bit.ChargeOk) { // Тестовый режим разрешен только при нормальном заряде
        FanControl.BoardFanTask = ((float)ValTestFanBoard) * 0.1f; 
      }
    }
    // Обновить состояние вентилятора
    FanControl.update (&FanControl);
    
//****************************************************************************//    
//***************** Светодиоды для пульта ************************************//
    LedStatus.LedReady = (LedStatus_type)testBitMicro(ramPnt, progPnt->Pnt_LEDReady);          
    LedStatus.LedWiFi = (LedStatus_type)testBitMicro(ramPnt, progPnt->Pnt_LEDWifi); 
    LedStatus.LedFail = (LedStatus_type)testBitMicro(ramPnt, progPnt->Pnt_LEDFail);
    LedStatus.LedWork = (LedStatus_type)testBitMicro(ramPnt, progPnt->Pnt_LEDRun);           
    LedStatus.set(&LedStatus);    
        
//****************************************************************************//    
//*************** Контроль наличия внешнего резервного питания ***************//
#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)    
    StatBackupPow = BackupPowStat.getStatus(&BackupPowStat);
    // пока без фильтра
    defBitMicro(ramPnt, progPnt->Pnt_BckPwrStat, (s16)StatBackupPow);
#endif            
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -         
//****************************************************************************//    
//***************** Дискретный термодатчик ***********************************//
    StatTdiscr = TDiscr.GetStatus(&TDiscr);    // Получить состояние датчика температуры
//#ifdef _TEST_MODE_ 
//    StatTdiscr = TdiscrOk;
//#endif    
    // Подфильтруем сигнал от датчика
    if(StatTdiscr == TdiscrErr){
       sPnt->Tdiscr.TimeCount++;
       if(sPnt->Tdiscr.TimeCount > T_TILTR){
          sPnt->Tdiscr.TimeCount = T_TILTR;
          sPnt->Tdiscr.Status = TdiscrErr;
       }
    }
    else if(sPnt->Tdiscr.TimeCount > 0){
       sPnt->Tdiscr.TimeCount--;
    }
    else{
       sPnt->Tdiscr.Status = TdiscrOk;
    }
    // Вывод сигнала в МП  
    defBitMicro(ramPnt, progPnt->Pnt_T_diskr, (s16)sPnt->Tdiscr.Status);

#endif  // _PROJECT_FOR_STM32_  
    

// Активируем работы с дискретными входами в режиме симуляции в VS2005
#ifdef _PROJECT_FOR_VS2005_
	{
    u16                 *valPnt, *invPnt,*filtrPnt,  maskDin, maskDout,  value, filtrDinUst, tekMaskState, tekDinExt, numDin, numDout;        
    StatePERIF_V2_type  *sPnt;      
    sPnt	        = (StatePERIF_V2_type *)(ramPnt + progPnt->Pnt_State);

    // Получить состояние дискретных входов
    //tekMaskState = DinDout.getDin(&DinDout); // Бортовые сигналы с железа приходят инверсными
    //tekDinExt = (0xFFFF ^ GlobalM4.DinDoutExtData.DinState.dinStateWord);    
    tekMaskState = 0x0FFF; // Иммитация всех дискретных сигналов
    
    invPnt	= progPnt->Pnt_Inv_Din;
    valPnt	= progPnt->Pnt_Din;
    filtrPnt	= progPnt->Pnt_Filtr_Din;
    maskDin = value = 0;
    

    for(numDin = 0; numDin < QuantityDins; numDin++, valPnt++, invPnt++,filtrPnt++)
    {
      filtrDinUst = load_s16(ramPnt, *filtrPnt);                // значения уставки фильтрации
      filtrDinUst = tblFiltrDin[filtrDinUst];                   // пересчет на такты микропрограммы
      
      if(tekMaskState & (1 << numDin))                          // если на входе активный сигнал ( лог "1")
      {
        if(sPnt->FiltrDinCnt[numDin] < filtrDinUst)                         
          sPnt->FiltrDinCnt[numDin]++;                          // увеличения счетчика для фильтрации
        else
          sPnt->maskFiltrDin |= (1 << numDin);                  // выставляем логическую "1" в маску дискретных входов
      }
      else if(sPnt->FiltrDinCnt[numDin] > 0)                    // если на входе логичиский "0"
          sPnt->FiltrDinCnt[numDin]--;                          // уменьшения счетчика
      else
          sPnt->maskFiltrDin &=  (~(1 << numDin));              // выставляем логический "0" в маску дискретных входов
      
//      if(numDin < QuantityDins_CP24)                            // если входы "свои" (платы CP24)
        value = (sPnt->maskFiltrDin & (1 << numDin)) ? 0:1;     // состояние дискретного входа (на схеме если не замыкать контакты, то "О", при замыкании "1")
//      else
//        value = (sPnt->maskFiltrDin & (1 << numDin)) ? 1:0;     // состояние дискретного входа
          
      value = (value ^ load_s16(ramPnt, *invPnt));              // инверсия входа,  если она есть.
      maskDin |= (value << numDin);                             // заполнения битовой маски состояния дискретных входов с учетом инрерсии
      
      save_s16(ramPnt, *valPnt, value);                         // запись сстояния дискретного входа
    }
    save_s16(ramPnt, progPnt->Pnt_Din1_12, maskDin);            //  запись значения всех дискретных входов в виде маски
    
    // VS 2005
    //****************************** ДИСКРЕТНЫЕ ВЫХОДЫ *******************************//
    
    tekMaskState        = load_s16(ramPnt, progPnt->Pnt_Dout1_12);
    valPnt	        = progPnt->Pnt_Rele;
    invPnt	        = progPnt->Pnt_Inv_R;
    maskDout = 0;
    for(numDout = 0; numDout < QuantityDouts; numDout++, valPnt++, invPnt++)
    {
        value = (tekMaskState & (1 << numDout)) ? 1:0;          // определение задания состояния выхода
        value = (value ^ load_s16(ramPnt, *invPnt));            // инверсия значения выхода
        maskDout |= (value << numDout);                         // формирования маски с учетом инверсии
        save_s16(ramPnt, *valPnt, value);                       // запись состояния дискретного выхода
    }
    
    // переменные для вывода в логер
    GlobalM4.Dout1 = (maskDout & (1<<0)) ? 1 : 0;
    GlobalM4.Dout2 = (maskDout & (1<<1)) ? 1 : 0;
    GlobalM4.Dout3 = (maskDout & (1<<2)) ? 1 : 0;
    GlobalM4.Dout4 = (maskDout & (1<<3)) ? 1 : 0;
    GlobalM4.Dout5 = (maskDout & (1<<4)) ? 1 : 0;
	}
#endif

    return &progPnt->Pnt_End;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -         
#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1) 
//********************** Задание типа аналогового входа *************************//
void SetTypeAin(u16 typeAinUst, u8 numAin)
{  
#ifdef _PROJECT_FOR_STM32_
    switch(typeAinUst)
    {
      case TypeAin_0_10V:
        if(numAin == 0)
          {SetTypeAin1MK_0_10V();}
        else
          {SetTypeAin2MK_0_10V();}
      break;
      case TypeAin_4_20mA:
        if(numAin == 0)
        {SetTypeAin1MK_4_20mA();}
        else
        {SetTypeAin2MK_4_20mA();}
      break;
    }

#endif
}
//********************** Считывание аналоговых входов *************************//
void AdcCalc(FuncPERIF_V2_type *progPnt, u32 ramPnt, StatePERIF_V2_type *sPnt, nvAinCalState_type	*nvAdr, s16 Num)
{
#ifdef _PROJECT_FOR_STM32_
  AinOprm_type          *adcAinSensPnt;
  AinMemInt_type 	        *mPnt;
  s16                   AinType, kalibr;
  s16	                CodeMin, CodeMax, state;
  f32                   value, OutMin, OutMax, InCalMin, InCalMax;
  
  mPnt = &(sPnt->AinMemInt[Num]);
  adcAinSensPnt	= &(progPnt->AinOprm[Num]);

  AinType = load_s16(ramPnt, progPnt->Pnt_TypeAin[Num]);  
  
  if((flagIsFirstCall)  ||  (AinType != (u16)mPnt->AinType))                                     // при подаче питания всегда конфигурим тип аналогового входа 
  {
    SetTypeAin(AinType,Num);
    mPnt->AinType = (AO_type)AinType;
  }
    // Калибровка аналогового входа 1, 2  //
  kalibr = load_s16(ramPnt, progPnt->Pnt_KalibrAin[Num]); 
  save_s16(ramPnt, progPnt->Pnt_KalibrAin[Num], 0);
    // Калибровка минимума, максимума, смещения 1 или смещения 2 //
  if(kalibr > 0)                        
  {
      switch(kalibr)
      {
          case CalMin:
              switch(AinType)
              {
                  case AOtype_0_10B:
                      kalibr = ccain_1V;
                  break;
                  case AOtype_4_20mA:
                      kalibr = ccain_4mA;
                  break;
              }
              mPnt->CalibrCodeAin[kalibr] = mPnt->AdcCode;
              sPnt->AinMemInt[Num].CalibrCodeAin[ccain_CRC] = Crc((unsigned char *)(sPnt->AinMemInt[Num].CalibrCodeAin), sizeof(nvAdr->CalibrCode[0]) - sizeof(u16)); //
              if(sizeof(nv.AInput.CalibrCode) == sizeof(nvAdr->CalibrCode))                                                                                     // Размерности должны совпадать чтобы не затерать чужие данные
                  WrFramMicro(nvAdr->CalibrCode[Num], sPnt->AinMemInt[Num].CalibrCodeAin, sizeof(nvAdr->CalibrCode[0]));	//
          break;
          case CalMax:
              switch(AinType)
              {
                  case AOtype_0_10B:
                      kalibr = ccain_10V;
                  break;
                  case AOtype_4_20mA:
                      kalibr = ccain_20mA;
                  break;
              }
              mPnt->CalibrCodeAin[kalibr] = mPnt->AdcCode;
              sPnt->AinMemInt[Num].CalibrCodeAin[ccain_CRC] = Crc((unsigned char *)(sPnt->AinMemInt[Num].CalibrCodeAin), sizeof(nvAdr->CalibrCode[0]) - sizeof(u16)); //       
              if(sizeof(nv.AInput.CalibrCode) == sizeof(nvAdr->CalibrCode))                                                                                             // Размерности должны совпадать чтобы не затерать чужие данные
                  WrFramMicro(nvAdr->CalibrCode[Num], sPnt->AinMemInt[Num].CalibrCodeAin, sizeof(nvAdr->CalibrCode[0]));       
          break;
          default:
          break;
      }
  }
  CodeMax = 1;
  CodeMin = 0;
  
  switch(AinType)
  {
      case AOtype_0_10B:
          CodeMin       = (mPnt->CalibrCodeAin[ccain_1V]);
          CodeMax       = (mPnt->CalibrCodeAin[ccain_10V]);
          OutMin        = (f32)MinValueAin_0_10V;
          OutMax        = (f32)MaxValueAin_0_10V;
          InCalMin      = (f32)CalMinValueAin_0_10V;
          InCalMax      = (f32)CalMaxValueAin_0_10V;          
          break;
      case AOtype_4_20mA:
          CodeMin       = mPnt->CalibrCodeAin[ccain_4mA];
          CodeMax       = mPnt->CalibrCodeAin[ccain_20mA];
          OutMin        = (f32)MinValueAin_4_20mA;
          OutMax        = (f32)MaxValueAin_4_20mA;        
          InCalMin      = (f32)CalMinValueAin_4_20mA;
          InCalMax      = (f32)CalMaxValueAin_4_20mA;           
          break;
  }
  state = AinState_Ok;
  
  if(mPnt->AinState == AinState_Ok)                             // состояние было "ОК"
  {
       switch(AinType)
      {
        case AOtype_0_10B:
          if(mPnt->AdcCode >= AdcCodeKZ_0_10V)
          {
            DisableKey_0_10V(Num);
            mPnt->CntDisableAin = TimeEnableKeyAin;             // заводим таймер на 2 секунду 
            mPnt->AinState = state = AinState_KZ;
          }
        break;
        case AOtype_4_20mA:
          if(mPnt->AdcCode >= AdcCodeKZ_4_20mA)
          {
            DisableKey_4_20mA(Num);
            mPnt->CntDisableAin = TimeEnableKeyAin;             // заводим таймер на 2 секунду 
            mPnt->AinState = state = AinState_KZ;
          }
          else if((mPnt->AdcCode <= AdcCodeHH))                 // обрыв для входа 4-20 мА
            state = AinState_HH;
        break;
      }
  }
  else 
     state = mPnt->AinState;
  
  if(state == AinState_Ok) {
    // считаем и пишем значения если с входом все "ОК"
    // Получить значение Ain
    // Находим значение внутри диапазона калибровки 
    value = f32_Fy_x1x2y1y2x(CodeMin, CodeMax, InCalMin, InCalMax, mPnt->AdcCode); 
    // Приводим в % к диапазону 
    value = (value - OutMin)/(OutMax-OutMin);
    
    //if(value < 0)
   //     value = 0;
   // else if (value > 1.f)
   //     value = 1.f;
    
    save_float(ramPnt,adcAinSensPnt->Pnt_Ain , value);
    mPnt->AinValuePercent = (s16)(value*1000.0f); // Значение входа в процентах с 1 знаком после запятой
  }
  else if(mPnt->CntDisableAin == TimeEnableKeyAin)              // если было КЗ -> 1
    save_float(ramPnt,adcAinSensPnt->Pnt_Ain , 1);
  else if(state == AinState_HH)                                 // если обрыв то -> 0
    save_float(ramPnt,adcAinSensPnt->Pnt_Ain , 0);
  
  save_s16(ramPnt,adcAinSensPnt->Pnt_AdcCode , mPnt->AdcCode); 
  //defBitMicro(ramPnt, adcAinSensPnt->Pnt_SensFailAin, state);
  save_s16(ramPnt, adcAinSensPnt->Pnt_SensFailAin, state);  
    
  if(mPnt->AinState == AinState_KZ)
  {
    if(mPnt->CntDisableAin == 0)                                // через 2 сек снова настоить тип аналогового входа
    {
      SetTypeAin(AinType,Num);                                                  
      mPnt->AinState = AinState_Ok;                             // считаем что состояние ОК
    }
    else
    {
       mPnt->CntDisableAin--;
    }
  }
#endif 
}

void AoutCalc(FuncPERIF_V2_type *progPnt, u32 ramPnt, StatePERIF_V2_type *sPnt, nvAoutCalState_type	*nvAdr, s16 Num)
{
    RefTypeAout_type    *AoutPnt;
    AoutMemInt_type        *mPnt;
    FormatValAout_Type	*fvPnt;
    
    float       Ref_Aout;
    s16         CodeMin, CodeMax, AoutType;
    f32         InMin, InMax;    
    u16         Calibrate, TestVal, format;
    u32         CalibrValue;
    
    fvPnt	= &(progPnt->FormatValAout[Num]);
    mPnt        = &(sPnt->AoutMemInt[Num]);
    AoutPnt     = (&progPnt->ValTypeAout[Num]);
    AoutType    = load_s16(ramPnt, AoutPnt->Pnt_TypeAout);
    
    if((flagIsFirstCall) || (AoutType != (u16)mPnt->AoutType))                                     // при подаче питания всегда конфигурим тип аналогового входа 
    {
      SetTypeAout(AoutType,Num);
      mPnt->AoutType = AoutType;
    }
    Ref_Aout    = load_float(ramPnt, AoutPnt->Pnt_RefAout );
    Calibrate   = load_s16(ramPnt, AoutPnt->Pnt_CalibAout);
    TestVal     = load_s16(ramPnt, AoutPnt->Pnt_TestAout);     
    mPnt->AoutValPercent = (s16)(Ref_Aout * 1000.0f); // Относительное значение аналогового выхода в процентах с одним знаком после запятой
    
    switch(AoutType)
    {
      case AOtype_0_10B:
          CodeMin       = (mPnt->CalibrCodeAout[Code_0V]);
          CodeMax       = (mPnt->CalibrCodeAout[Code_10V]);
          InMin         = (f32)MinValueAout_0_10V;
          InMax         = (f32)MaxValueAout_0_10V;          
          format        = (2 << 8) | VRasm;        // Формат
      break;
      case AOtype_4_20mA:
          CodeMin       = mPnt->CalibrCodeAout[Code_4mA];
          CodeMax       = mPnt->CalibrCodeAout[Code_20mA];
          InMin         = (f32)MinValueAout_4_20mA;
          InMax         = (f32)MaxValueAout_4_20mA;          
          format        = (2 << 8) | mARasm;      // Формат
      break;
    }
    
    switch(Calibrate)
    {
      case CallAoutOff:{           // Штатный режим работы - расчет задания для pwm
         // Расчет для отображения текущего значения
         switch(AoutType){
            case AOtype_0_10B:{
               mPnt->ValAout = InMax * Ref_Aout;
               mPnt->DacCode = (s16)f32_Fy_x1x2y1y2x(InMin, InMax, CodeMin, CodeMax, mPnt->ValAout);                                            
            }break;
            case AOtype_4_20mA:{
               mPnt->ValAout = (Ref_Aout * (InMax - InMin) + InMin);               
               mPnt->DacCode = (s16)f32_Fy_x1x2y1y2x(InMin, InMax, CodeMin, CodeMax, mPnt->ValAout);               
            }break;
        }
      }break;        
            
      case CallTestAout:{         // Тестовый режим - задание по уставке
         switch(AoutType){
            case AOtype_0_10B:{
               mPnt->DacCode = (s16)f32_Fy_x1x2y1y2x(InMin, InMax, CodeMin, CodeMax, TestVal);                             
            }break;
            
            case AOtype_4_20mA:{                           
               mPnt->DacCode = (s16)f32_Fy_x1x2y1y2x(InMin, InMax, CodeMin, CodeMax, TestVal);              
            }break;
         }
      }break;      
        
      case CallMinAout:{          // Калибровка по минимуму   
        if(AoutType == AOtype_4_20mA ){
           mPnt->FlagWrNv = 1;
           CalibrValue = (u32)((InMin / (f32)TestVal) * (f32)mPnt->CalibrCodeAout[Code_4mA]);
           if (CalibrValue == 0) { // Защита от нуля. Если вдруг ноль получился, ставим значение по умолчанию
             CalibrValue = CALIBR_AIN_DEF_4MA;     // Коеффициент калибровки по умолчанию для точки 4ma
           }
           mPnt->CalibrCodeAout[Code_4mA] = (u16)CalibrValue;
           TestVal = (s16)InMin;           
        } else if(AoutType == AOtype_0_10B) {
           mPnt->FlagWrNv = 1;
           CalibrValue = (u32)((InMin / (f32)TestVal) * (f32)mPnt->CalibrCodeAout[Code_0V]);
           if (CalibrValue == 0) { // Защита от нуля. Если вдруг ноль получился, ставим значение по умолчанию
             CalibrValue = CALIBR_AIN_DEF_1V;       // Коеффициент калибровки для точки 0 В.             
           }           
           mPnt->CalibrCodeAout[Code_0V] = (u16)CalibrValue;
           TestVal = (s16)InMin;          
        }                
        save_s16(ramPnt, AoutPnt->Pnt_TestAout, TestVal);
        save_s16(ramPnt, AoutPnt->Pnt_CalibAout, CallTestAout);
      }break;
          
      case CallMaxAout:{          // Калибровка по максимуму
        if(AoutType == AOtype_4_20mA){
           mPnt->FlagWrNv = 1;
           CalibrValue = (u32)((InMax / (f32)TestVal) * (f32)mPnt->CalibrCodeAout[Code_20mA]);           
           // Проверим полученное значение коеффициента калибровки на валидность
           if(CalibrValue > PeriodPWM_Aout){
               CalibrValue = PeriodPWM_Aout;   // Уперлись в максимум
           }           
           mPnt->CalibrCodeAout[Code_20mA] = CalibrValue;
           TestVal = (s16)InMax;
           save_s16(ramPnt, AoutPnt->Pnt_TestAout, TestVal);
           save_s16(ramPnt, AoutPnt->Pnt_CalibAout, CallTestAout);
        } else if(AoutType == AOtype_0_10B) {
          CalibrValue = (u32)((InMax / (f32)TestVal) * (f32)mPnt->CalibrCodeAout[Code_10V]);
           // Проверим полученное значение коеффициента калибровки на валидность
           if(CalibrValue > PeriodPWM_Aout){
               CalibrValue = PeriodPWM_Aout;
           }
           mPnt->CalibrCodeAout[Code_10V] = CalibrValue;
           TestVal = (s16)InMax;
           save_s16(ramPnt, AoutPnt->Pnt_TestAout, TestVal);           
           mPnt->FlagWrNv = 1;
           save_s16(ramPnt, AoutPnt->Pnt_CalibAout, CallTestAout);
        }
      }break;
      
      case ResetCallAout:{
        mPnt->FlagWrNv = 1;
        if(AoutType == AOtype_4_20mA){
          mPnt->CalibrCodeAout[Code_4mA] = CALIBR_AOUT_DEF_4MA;     // Коеффициент калибровки по умолчанию для точки 4ma
          mPnt->CalibrCodeAout[Code_20mA] = CALIBR_AOUT_DEF_20MA;   // Коеффициент калибровки по умолчанию для точки 20ma        
        } else if(AoutType == AOtype_0_10B){
          mPnt->CalibrCodeAout[Code_0V] = CALIBR_AOUT_DEF_1V;       // Коеффициент калибровки для точки 0 В.
          mPnt->CalibrCodeAout[Code_10V] = CALIBR_AOUT_DEF_10V;     // Коеффициент калибровки для точки 10 В.        
        }
        save_s16(ramPnt, AoutPnt->Pnt_CalibAout, CallTestAout);
      }break;
      
    }
    if(mPnt->FlagWrNv)
    {
      if(sizeof(nv.AOutput.CalibrCode[0]) == sizeof(nvAdr->CalibrCode[0]))
      {
        mPnt->CalibrCodeAout[Code_CRC] = Crc((unsigned char *)(mPnt->CalibrCodeAout), sizeof(nvAdr->CalibrCode[0]) - sizeof(u16));
        WrFramMicro(nvAdr->CalibrCode[Num], sPnt->AoutMemInt[Num].CalibrCodeAout, sizeof(nvAdr->CalibrCode[0]));
      }
      mPnt->FlagWrNv = 0;
    }
     
    save_s16(ramPnt, fvPnt->Pnt_FormatAout, (s16)format);
    save_s16(ramPnt, fvPnt->Pnt_ValAout, (s16)mPnt->ValAout);
      
  #ifdef _PROJECT_FOR_STM32_
    if(!flagIsFirstCall) {        
        //SetValuePWM_Aout((s16)GlobalM4.Debug10, Num); // Тест Авых
        SetValuePWM_Aout((s16)mPnt->DacCode,Num);   // задание на ШИМ;
    }
  #endif // _PROJECT_FOR_STM32_
}
//********************** Задание типа аналогового выхода *************************//
void SetTypeAout(u16 typeAoutUst, u8 numAout)
{  
#ifdef _PROJECT_FOR_STM32_
    switch(typeAoutUst)
    {
      case TypeAin_0_10V:
        if(numAout == 0)
          {SetTypeAout1MK_0_10V();}
        else
          {SetTypeAout2MK_0_10V();}
      break;
      case TypeAin_4_20mA:
        if(numAout == 0)
        {SetTypeAout1MK_4_20mA();}
        else
        {SetTypeAout2MK_4_20mA();}
      break;
    }

#endif // _PROJECT_FOR_STM32_
}
#endif  //   #if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)     
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -         

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -         
#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1) 
//*****************************************************************************//
//********************** Обработчик аналоговых входов *************************//
void AdcCalc(FuncPERIF_V2_type *progPnt, u32 ramPnt, StatePERIF_V2_type *sPnt, nvAinCalState_type *nvAdr, s16 Num){
  AinOprm_type          *adcAinSensPnt;
  AinMemExt_type        *mPnt;
  AinExtType_Enum       AinType;
  ModeAin_type          ModeAin;
  
  s16	                CodeMin, CodeMax, CodeAdc;
  f32                   value, OutMin, OutMax, InCalMin, InCalMax;
  
  mPnt = &(sPnt->AinMemExt[Num]);
  adcAinSensPnt	= &(progPnt->AinOprm[Num]);
  
  // Получить тип аналогового входа
  AinType = (AinExtType_Enum)(load_s16(ramPnt, progPnt->Pnt_TypeAin[Num]) + 1);  
  mPnt->Type = AinType;                                      // установить тип входа
    
  // Калибровка аналогового входа 1, 2  //
  ModeAin = (ModeAin_type)load_s16(ramPnt, progPnt->Pnt_KalibrAin[Num]); 
  save_s16(ramPnt, progPnt->Pnt_KalibrAin[Num], 0);
  // Калибровка минимума, максимума, смещения 1 или смещения 2 //
  if(ModeAin != CalOff)                        
  {
      switch(ModeAin)
      {
          case CalMin:{
             if(AinType == Ain0_10v){
                mPnt->Calibr.Val1V = mPnt->AdcVal;
             }
             else if(AinType == Ain4_20ma){
                mPnt->Calibr.Val4mA = mPnt->AdcVal;
             }
             else if(AinType == AinPt1000){
                mPnt->Calibr.Val_50C = mPnt->AdcVal;
             }
             
             // Посчитать crc            
             mPnt->Calibr.crc = Crc((u8*)(&mPnt->Calibr), sizeof(AinCalibr_type) - sizeof(u16)); 
             // Сохранить в FRAM
             if(sizeof(nv.AInput.CalibrCode[Num]) == sizeof(AinCalibr_type)){                         // Размерности должны совпадать чтобы не затерать чужие данные
                 WrFramMicro(nvAdr->CalibrCode[Num], &mPnt->Calibr, sizeof(AinCalibr_type));	         //
             }
          }break;
          
          case CalMax:{
             if(AinType == Ain0_10v){
                mPnt->Calibr.Val10V = mPnt->AdcVal;
             }
             else if(AinType == Ain4_20ma){
                mPnt->Calibr.Val20mA = mPnt->AdcVal;
             }
             else if(AinType == AinPt1000){
                mPnt->Calibr.Val200C = mPnt->AdcVal;
             }
             
             // Посчитать crc            
             mPnt->Calibr.crc = Crc((u8*)(&mPnt->Calibr), sizeof(AinCalibr_type) - sizeof(u16)); 
             // Сохранить в FRAM
             if(sizeof(nv.AInput.CalibrCode[Num]) == sizeof(AinCalibr_type)){                         // Размерности должны совпадать чтобы не затерать чужие данные
                 WrFramMicro(nvAdr->CalibrCode[Num], &mPnt->Calibr, sizeof(AinCalibr_type));	         //
             }       
          }break;
          default:
          break;
      }
  }
  CodeMax = 1;
  CodeMin = 0;
  CodeAdc  = mPnt->AdcVal;
  
  // Получить калибровочные значения и формат для уставок в зависимести от типа
  switch(AinType){
     case Ain0_10v:{
        CodeMin = mPnt->Calibr.Val1V;       // Коеффициент калибровки для точки 1 В 
        CodeMax = mPnt->Calibr.Val10V;      // Коеффициент калибровки для точки 10 В
        OutMin = (f32)MinValueAin_0_10V;
        OutMax = (f32)MaxValueAin_0_10V;
        InCalMin = (f32)CalMinValueAin_0_10V;
        InCalMax = (f32)CalMaxValueAin_0_10V;
        if(CodeAdc > AdcCodeKZ_0_10V)
            CodeAdc = AdcCodeKZ_0_10V;
        
     }break;
     case Ain4_20ma:{
        CodeMin = mPnt->Calibr.Val4mA;      // Коеффициент калибровки для точки 4 мА.
        CodeMax = mPnt->Calibr.Val20mA;     // Коеффициент калибровки для точки 20 мА.
        OutMin = (f32)MinValueAin_4_20mA;
        OutMax = (f32)MaxValueAin_4_20mA;        
        InCalMin = (f32)CalMinValueAin_4_20mA;
        InCalMax = (f32)CalMaxValueAin_4_20mA;  
        if(CodeAdc > AdcCodeKZ_4_20mA)
            CodeAdc = AdcCodeKZ_4_20mA;
     }break;
     case AinPt1000:{
        CodeMin = mPnt->Calibr.Val_50C;     // Коеффициент калибровки для точки -50C.
        CodeMax = mPnt->Calibr.Val200C;     // Коеффициент калибровки для точки 200C.
        OutMin = (f32)MinValueAin_PT1000;
        OutMax = (f32)MaxValueAin_PT1000;                
        InCalMin = (f32)CalMinValueAin_PT1000;
        InCalMax = (f32)CalMaxValueAin_PT1000;                        
     }break;
  }
   
   
   
  // Разбор статуса Ain
  if(mPnt->AinStatus == AinOk){                          // с входом все "ОК" 
     // Получить значение Ain
     // Находим значение внутри диапазона калибровки 
     value = f32_Fy_x1x2y1y2x(CodeMin, CodeMax, InCalMin, InCalMax, CodeAdc); 
     // Приводим в % к диапазону 
     value = (value - OutMin)/(OutMax-OutMin);
     
     // Ограничение входа Ain
    // if(value < 0){
     //   value = 0;
   //  }    
   //  else if(value > 1.0f){
   //     value = 1.f;
   //  }
    
     save_float(ramPnt,adcAinSensPnt->Pnt_Ain , value);
     mPnt->AinValuePercent = (s16)(value*1000.0f); // Значение входа в процентах с 1 знаком после запятой
  }
  else if(mPnt->CntDisable == TimeEnableKeyAin){               // если было КЗ -> 1
     save_float(ramPnt,adcAinSensPnt->Pnt_Ain , 1);
  }
  else if(mPnt->AinStatus == AinHh){                           // если обрыв то -> 0
     save_float(ramPnt,adcAinSensPnt->Pnt_Ain , 0);
  }
  
  save_s16(ramPnt,adcAinSensPnt->Pnt_AdcCode , mPnt->AdcVal); 
  //defBitMicro(ramPnt, adcAinSensPnt->Pnt_SensFailAin, mPnt->AinStatus);
  save_s16(ramPnt, adcAinSensPnt->Pnt_SensFailAin, mPnt->AinStatus);
}


//******************************************************************************
//********************** Обработчик аналоговых выходов ***********************//
//******************************************************************************
void AoutCalc(FuncPERIF_V2_type *progPnt, u32 ramPnt, StatePERIF_V2_type *sPnt, nvAoutCalState_type *nvAdr, s16 Num){
    RefTypeAout_type    *AoutPnt;
    AoutMemExt_type        *mPnt;
    FormatValAout_Type	*fvPnt;
    AoutExtType_Enum    AoutType;
    ModeAout_type       ModeAout;  
    
    float       Ref_Aout;
    s16         CodeMin, CodeMax;
    f32         InMin, InMax;
    u16         TestVal, format;
    u32         CalibrValue;
    
    fvPnt	= &(progPnt->FormatValAout[Num]);
    mPnt        = &(sPnt->AoutMemExt[Num]);
    AoutPnt     = (&progPnt->ValTypeAout[Num]);
    AoutType    = (AoutExtType_Enum)(load_s16(ramPnt, AoutPnt->Pnt_TypeAout) + 1);    // Тип аналогового выхода (+1 для корректного привидения типов)
    Ref_Aout    = load_float(ramPnt, AoutPnt->Pnt_RefAout);
    ModeAout    = (ModeAout_type)load_s16(ramPnt, AoutPnt->Pnt_CalibAout);
    TestVal     = load_s16(ramPnt, AoutPnt->Pnt_TestAout);
    mPnt->AoutValPercent = (s16)(Ref_Aout * 1000.0f); // Относительное значение аналогового выхода в процентах с одним знаком после запятой
      
    // Сохранить тип выхода
    mPnt->Type = AoutType;
        
    // Получить калибровочные значения и формат для уставок в зависимести от типа
    switch(AoutType){
      case Aout0_10v:
          CodeMin = mPnt->Calibr.Val1V;       // Коеффициент калибровки для точки 0 В 
          CodeMax = mPnt->Calibr.Val10V;      // Коеффициент калибровки для точки 10 В          
          InMin   = (f32)MinValueAout_0_10V;
          InMax   = (f32)MaxValueAout_0_10V;
          format  = (2 << 8) | VRasm;          // Формат
      break;
      case Aout4_20ma:
          CodeMin = mPnt->Calibr.Val4mA;      // Коеффициент калибровки для точки 4 мА.
          CodeMax = mPnt->Calibr.Val20mA;     // Коеффициент калибровки для точки 20 мА.          
          InMin   = (f32)MinValueAout_4_20mA;
          InMax   = (f32)MaxValueAout_4_20mA;          
          format = (2 << 8) | mARasm;         // Формат
      break;
    }   
    
    // Выбор режима работы аналогового выхода
    switch(ModeAout){
      case CallAoutOff:{           // Штатный режим работы - расчет задания для pwm
         // Расчет для отображения текущего значения
         switch(AoutType){
            case Aout0_10v:{
               mPnt->ValAout = InMax * Ref_Aout;
               mPnt->DacCode = (s16)f32_Fy_x1x2y1y2x(InMin, InMax, CodeMin, CodeMax, mPnt->ValAout);                                            
            }break;
            case Aout4_20ma:{
               mPnt->ValAout = (Ref_Aout * (InMax - InMin) + InMin);               
               mPnt->DacCode = (s16)f32_Fy_x1x2y1y2x(InMin, InMax, CodeMin, CodeMax, mPnt->ValAout);               
            }break;
        }
      }break;       
     
      case CallTestAout:{         // Тестовый режим - задание по уставке
         switch(AoutType){
            case Aout0_10v:{
               mPnt->DacCode = (s16)f32_Fy_x1x2y1y2x(InMin, InMax, CodeMin, CodeMax, TestVal);                             
            }break;
            
            case Aout4_20ma:{                           
               mPnt->DacCode = (s16)f32_Fy_x1x2y1y2x(InMin, InMax, CodeMin, CodeMax, TestVal);              
            }break;
         }
      }break;
      
      case CallMinAout:{          // Калибровка по минимуму   
        if(AoutType == Aout4_20ma ){
           mPnt->FlagWrNv = 1;
           CalibrValue = (u32)((InMin / (f32)TestVal) * (f32)mPnt->Calibr.Val4mA);
           if (CalibrValue == 0) { // Защита от нуля. Если вдруг ноль получился, ставим значение по умолчанию
             CalibrValue = CALIBR_AIN_DEF_4MA;     // Коеффициент калибровки по умолчанию для точки 4ma
           }
           mPnt->Calibr.Val4mA = (u16)CalibrValue;
           TestVal = (s16)InMin;           
        } else if(AoutType == Aout0_10v) {
           mPnt->FlagWrNv = 1;
           CalibrValue = (u32)((InMin / (f32)TestVal) * (f32)mPnt->Calibr.Val1V);
           if (CalibrValue == 0) { // Защита от нуля. Если вдруг ноль получился, ставим значение по умолчанию
             CalibrValue = CALIBR_AIN_DEF_1V;       // Коеффициент калибровки для точки 0 В.             
           }           
           mPnt->Calibr.Val1V = (u16)CalibrValue;
           TestVal = (s16)InMin;          
        }                
        save_s16(ramPnt, AoutPnt->Pnt_TestAout, TestVal);
        save_s16(ramPnt, AoutPnt->Pnt_CalibAout, CallTestAout);
      }break; 
      
      case CallMaxAout:{          // Калибровка по максимуму
        if(AoutType == Aout4_20ma){
           mPnt->FlagWrNv = 1;
           CalibrValue = (u32)((InMax / (f32)TestVal) * (f32)mPnt->Calibr.Val20mA);           
           // Проверим полученное значение коеффициента калибровки на валидность
           if(CalibrValue > TASK_PWM_MAX){
               CalibrValue = TASK_PWM_MAX;   // Уперлись в максимум
           }           
           mPnt->Calibr.Val20mA = CalibrValue;
           TestVal = (s16)InMax;
           save_s16(ramPnt, AoutPnt->Pnt_TestAout, TestVal);
           save_s16(ramPnt, AoutPnt->Pnt_CalibAout, CallTestAout);
        } else if(AoutType == Aout0_10v) {
          CalibrValue = (u32)((InMax / (f32)TestVal) * (f32)mPnt->Calibr.Val10V);
           // Проверим полученное значение коеффициента калибровки на валидность
           if(CalibrValue > TASK_PWM_MAX){
               CalibrValue = TASK_PWM_MAX;
           }
           mPnt->Calibr.Val10V = CalibrValue;
           TestVal = (s16)InMax;
           save_s16(ramPnt, AoutPnt->Pnt_TestAout, TestVal);           
           mPnt->FlagWrNv = 1;
           save_s16(ramPnt, AoutPnt->Pnt_CalibAout, CallTestAout);
        }
      }break;
      
      case ResetCallAout:{
        mPnt->FlagWrNv = 1;
        if(AoutType == Aout4_20ma){
          mPnt->Calibr.Val4mA = CALIBR_AOUT_DEF_4MA;     // Коеффициент калибровки по умолчанию для точки 4ma
          mPnt->Calibr.Val20mA = CALIBR_AOUT_DEF_20MA;   // Коеффициент калибровки по умолчанию для точки 20ma        
        } else if(AoutType == Aout0_10v){
          mPnt->Calibr.Val1V = CALIBR_AOUT_DEF_1V;       // Коеффициент калибровки для точки 0 В.
          mPnt->Calibr.Val10V = CALIBR_AOUT_DEF_10V;     // Коеффициент калибровки для точки 10 В.        
        }
        save_s16(ramPnt, AoutPnt->Pnt_CalibAout, CallTestAout);
      }break;
    }
    
    // Записать новые значения коеффициентов калибровки во FRAM
    if(mPnt->FlagWrNv){
      if(sizeof(nv.AOutput.CalibrCode[0]) == sizeof(nvAdr->CalibrCode[0])){
        // Посчитать crc на новые калибровочные коеффициенты
        mPnt->Calibr.crc = Crc((u8*)(&mPnt->Calibr), sizeof(AoutCalibr_type) - sizeof(u16));
        // Записать во FRAM калибровочные коеффициенты
        WrFramMicro(nvAdr->CalibrCode[Num], &mPnt->Calibr, sizeof(AoutCalibr_type));
      }
      mPnt->FlagWrNv = 0;    // Сбросить флаг необходимости записи во FRAM
    }
     
    save_s16(ramPnt, fvPnt->Pnt_FormatAout, (s16)format);
    save_s16(ramPnt, fvPnt->Pnt_ValAout, (s16)mPnt->ValAout);
}
#endif // #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1) 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -         

//****************************************************************************//
#endif // GET_FUNC_CODE
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPERIF_V2_1[131]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Not Used (Zar)
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Start
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  LEDWifi
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  LEDFail
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  LEDReady
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  LEDRun
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Dout1_12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Inv_R1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Inv_R2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Inv_R3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Inv_R4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Inv_R5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Inv_R6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Inv_R7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Inv_R8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Inv_R9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Inv_R10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Inv_R11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Inv_R12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Inv_Din1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Inv_Din2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Inv_Din3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Inv_Din4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  Inv_Din5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Inv_Din6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  Inv_Din7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  Inv_Din8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Inv_Din9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  InvDin10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  InvDin11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  InvDin12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  InvDin13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  Filtr_Din1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  Filtr_Din2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  Filtr_Din3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  Filtr_Din4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  Filtr_Din5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38  Filtr_Din6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //39  Filtr_Din7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //40  Filtr_Din8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41  Filtr_Din9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  Filtr_Din10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43  Filtr_Din11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //44  Filtr_Din12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //45  Filtr_Din13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //46  TypeAin1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //47  TypeAin2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //48  TypeAin3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //49  TypeAin4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //50  TypeAin5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //51  KalibrAin1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //52  KalibrAin2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //53  KalibrAin3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //54  KalibrAin4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //55  KalibrAin5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //56  TestAout1
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //57  Aout1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //58  TypeAout1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //59  KalibrAout1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //60  TestAout2
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //61  Aout2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //62  TypeAout2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //63  KalibrAout2        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //64  TestAout3
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //65  Aout3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //66  TypeAout3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //67  KalibrAout3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //68  TestAout4
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //69  Aout4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //70  TypeAout4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //71  KalibrAout4        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //72  IgbtFanRef
        S16_PIN_TYPE | INPUT_PIN_MODE,     //73  IgbtFanTest
        S16_PIN_TYPE | INPUT_PIN_MODE,     //74  BoardFanRef
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //75  BoardFanTest
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //76  MkarFail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //77  BckPwrStat
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //78  MkarCRC
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //79  MkarVerPO
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //80  Rele1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //81  Rele2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //82  Rele3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //83  Rele4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //84  Rele5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //85  Rele6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //86  Rele7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //87  Rele8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //88  Rele9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //89  Rele10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //90  Rele11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //91  Rele12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //92  Din1-13
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //93  T_diskr
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //94  Stop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //95  PTC
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //96  Din1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //97  Din2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //98  Din3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //99  Din4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //100  Din5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //101  Din6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //102  Din7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //103  Din8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //104  Din9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //105  Din10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //106  Din11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //107  Din12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //108  Din13
       
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //109  AdcCode1
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //110  Ain1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //111  SensFailAin1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //112  AdcCode2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //113  Ain2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //114  SensFailAin2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //115  AdcCode3
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //116  Ain3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //117  SensFailAin3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //118  AdcCode4
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //119  Ain4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //120  SensFailAin4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //121  AdcCode5
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,  //122  Ain5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //123  SensFailAin5        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //124  FormatAout1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //125  ValAout1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //126  FormatAout2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //127  ValAout2       
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //128  FormatAout3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //129  ValAout3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //130  FormatAout4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //131  ValAout4
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PERIF_V2", "DA", TblFuncPERIF_V2_1, FuncPERIF_V2_1, sizeof(TblFuncPERIF_V2_1), sizeof(StatePERIF_V2_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPERIF_V2_1,
#endif
//********************************************************
    