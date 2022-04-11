#ifndef GLOBALVAR_H_
#define GLOBALVAR_H_

#include "GlobalDefines.h"
#include "type.h"
#include "meastime.h"
#include "logger.h"


// Структура с глобальными переменными для взаимодействия системы регулирования с системой управления ПЧ
typedef struct{
      AT24Functional_type               PresentFunctional;  
      
      MeasTime_type  			MeasTime[MaxTC];       // Измерение временных интервалов
      DC_Brake_type                     DCBrake;
      logSetup_type                     logSetup;              // Настройки логгера
      u16                               debLockWriteLog;       // Блокирование логгера на время чтения            
      u16                               debLockWriteLogReq;
      u32                               VfdSerialNumber;        // Серийный номер ПЧ
      
      f32                               Fpwm;                  // Частота ШИМ (кГц)
      DC_PWM_type                       PwmDCmode;                // Частота ШИМа на постоянно м токе (кГц)
      //float                             DeadTime;            // Мертвое время (мкс)
      DeadTimeInit_type                 DeadTimeInit;          // Структура параметров настройки модуля компенсации deadTime
      float                             PwmCalcTime_us;        // Время выполнения такта ШИМ
      float                             VariableFpwm;          // Текущая переменная частота ШИМа
      f32                               ImaxVFD;               // Максимальный допустимый активный ток ПЧ (дополнительное ограничение на регулятор скорости)
      
      float                             Fref;                  // Задание частоты
      float                             Uref;                  // Задание напряжения
      u16                               DirectRotation;        // Направление вращения 
      float                             StepAngle;             // Шаг угла
      M4FlagCommand_type                FlagCommand;           // Команды с микропрограммы
      FlagState_type                    FlagState;             // Флаги состояний
      u16                               CPU_restart;           // 
      u16				SetUstDefault;         // Установка уставок по умолчанию через микропрограмму.
      
      FailData_type                     FailData;              // Данные про аварии 
      FailsSetup_type                   FailsSetup;            // Аварийные уставки
      
      u16                               ChargeOk;              // Временные сигналы заряда
      u16                               ChargeInProgress;      // Временные сигналы заряда
      
      TekValParam_type                  ValUd;                 // Значение Ud 
      TekValParam_type                  ValIu;                 // Значение Iu
      TekValParam_type                  ValIv;                 // Значение Iv 
      TekValParam_type                  ValIw;                 // Значение Iw
      TekValParam_type                  ValUab;                // Значение Uab
      TekValParam_type                  ValUbc;                // Значение Ubc
      TekValParam_type                  ValUca;                // Значение Uca
            
      TekValParam_type                  ValUu;                 // Значение Uu
      TekValParam_type                  ValUv;                 // Значение Uv 
      TekValParam_type                  ValUw;                 // Значение Uw
      
      u16                               LoadLossFault;         // Авария потери нагрузки из С28  
      float                             Uout;                  // Выходное напряжение ПЧ
      float                             Iflash;                // Выходное мгновенное значение полного тока
      float                             Iact;                  // Активный ток двигателя
      float                             Ireact;                // Реактивный ток двигателя
      float                             Ifull;                 // Полный ток двигателя
      float                             cosFi;                 // cosFi на выходе ПЧ
      float                             TIGBT;                 // температура ключей
      float                             Pfull;                 // Полная мощность 
      float                             Pact;                  // Активная мощность
      float                             Preact;                // Реактивная мощность
      float                             Wr;                    // Текущая скорость ротора
      float                             We;                    // Текущая скорость поля
      float                             Me;                    // Текущий момент двигателя
      float                             Angle;                 // Текущий угол поля
      float                             Fmax;                  // Максимальная частота
            
      float                             Isn;                   // Номинальный ток двигателя
      
      LowSpeedAinAdc_type               LowSpeedAinAdc;        // Результаты работы АЦП1 (медленные измирения)
      u16                               AdcVoltRef_PwmOff;     // Значение АЦП точного напряжения 1.2 В при отключенном ШИМе
      u16                               AdcVoltRef_PwmOn;      // Значение АЦП точного напряжения 1.2 В при включенном ШИМе
      f32                               AdcVoltRef_Kkor;       // 
      u16                               AdcOffsetIu;
      u16                               AdcOffsetIv;
      u16                               AdcOffsetIw;      
            
      ControlDrvType_type               DriveType;             // Тип управления ПЧ (скаляр, Вектор без ОС ...)
      EnginParam_type                   EnginParam;            // Параметры двигателя
      TormSwParam_type                  TormSwParam;           // Параметры тормозного ключа
            
      MEASURE_LM_RS_type                MeasureLmRs;           // Режим измерения параметров двигателя
      f32                               OriginAccelTime;       // Исходное время разгона для режима измерения параметров
      u16		                DebugDinOut;
      s16	                        NumSlowFail;            // Аварии из микропрограммы
      s16                               UinErr;
      
      u16                               Din1;
      u16                               Din2;
      u16                               Din3;
      u16                               Din4;
      u16                               Din5;
      u16                               Din6;
      u16                               Din7;
      u16                               Din8;      // Дополнительные входы для блока расширения
      u16                               Din9;      // Дополнительные входы для блока расширения
      u16                               Din10;     // Дополнительные входы для блока расширения
      u16                               Din11;     // Дополнительные входы для блока расширения
      u16                               Din12;     // Дополнительные входы для блока расширения
      u16                               Din13;     // Дополнительные входы для блока расширения
      u16                               Dout1;
      u16                               Dout2;
      u16                               Dout3;
      u16                               Dout4;  // Дополнительные выходы для блока расширения
      u16                               Dout5;  // Дополнительные выходы для блока расширения
      s16                               Ain1;                   // Аналоговый вход в % от входного уровня. 0 - 100 %, назависимо от типа входа
      s16                               Ain2;                   // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой
      s16                               Aout1;                  // Выдаем значение процентов от аналогового выхода, с 1 знаком после запятой
      s16                               Aout2;                  // Выдаем значение процентов от аналогового выхода, с 1 знаком после запятой      
      s16                               Ain3;                   // Аналоговый вход в % от входного уровня. 0 - 100 %, назависимо от типа входа
      s16                               Ain4;                   // Выдаем значение процентов от аналогового входа, с 1 знаком после запятой
      s16                               Aout3;                  // Выдаем значение процентов от аналогового выхода, с 1 знаком после запятой
      s16                               Aout4;                  // Выдаем значение процентов от аналогового выхода, с 1 знаком после запятой      
        
      f32                               Wout;                   // тек. скорость              
      s16		                AccelOverrun;           // TEST
      s16		                WoutAccel; 	        // TEST текущее ускорение
      f32                               F_ZAD;                  // 
      u16                               Pr_Tormoz;
      s16		                BlockOverload;
//      u16                               Revers;                 // Направление вращения двигателя
      u16                               EqRefVal;               // выход на задание
      
      f32                               Rsw; 			// Сопротивление ключа инвертора для мощности 7.5 кВт (Ом)
      f32                               Udf;  			// Напряжение включения обратного диода (В)
      f32                               dI; 			// Минимальный ток переключения компесации МВ (А)
      f32                               SoftCurrentTime;        // Время плавного сброса тока 

      f32                               KpFe;                   // Kp наблюдателя потока
      f32                               KiFe;                   // Ki наблюдателя потока
      
      f32                               TorqueScale;            // Поправочный коэф. для расчета момента
      f32                               RejectFreq;
      f32                               RejectBand;
      f32                               NewRefFiltrTime;        // Постоянная времени фильтра задания скорости
      f32                               KpForced;
      f32                               KiForced; 
      f32                               SpdForced;              // Ускорение регулятора скорости
      f32                               KpSpd;                  // Kp регулятора скорости            
      f32                               KiSpd;                  // Ki регулятора скорости
      f32                               KpSpdCalc;              // Kp регулятора скорости расчетное значение на основании момента инерции            
      f32                               KiSpdCalc;              // Ki регулятора скорости расчетное значение на основании момента инерции
      f32                               SpdForceTime;           // Время ускорения регулятора скорости
      f32                               piSpdOut;               // Выход регулятора скорости (используется для режима эвакуации)

      float                             KpCur;                  // Kp регулятора тока            
      float                             KiCur;                  // Ki регулятора тока
      float                             KpCurCalc;              // Kp регулятора тока расчетное значение на основании параметров двигателя            
      float                             KiCurCalc;              // Ki регулятора тока расчетное значение на основании параметров двигателя
    
      // Алгоритм коррекции фазы потокосцепления
      u16                               FluxThetaCorMode;       // Режим коррекции фазы потока
      f32                               FluxThetaOffset;        // Фиксированное смещение фазы
      f32                               FluxThetaPidKp;         // Кп регулятора фазы
      f32                               FluxThetaPidKi;         // Ки решулятора фазы
      //----------------------------------------------------------
      
      float                             KpSpd2;                 // 
      float                             KiSpd2;                 //
      float                             Filter_KprRS;           //
      float                             Filter_KintRS;          //
      float                             TorqueNomPu;            //
      float                             TorqueMax;              //
      float                             TorqueMin;              //
      float                             MoutDisplay;            //
      //u16                               REGIM_COUNT_J;          //
    
      float                             Tspd;                   //
      float                             TregSpd;                //
      float                             FcutSpd;                //
      float                             CompDelaySpd;           //
      float                             Kez;                    //
      float                             Mez;                    // 
      float                             PhiRTorqIact;           // Текущий поток возбуждения для расчета момента через ток, ое
      float                             FluxBASE;               // База по потоку двигателя
      float                             deltaUdCharge;          //
      f32                               TudcFilt;
      f32                               TfiltVolWeak;
      
      u16                               BrakeState;
      u16                               BrakeAlgStatus;
      u16                               PickupFreqFound;
      u16                               PickUpReadyToGo;
      float                             IsaturationPU;          // Процент тока возбуждения
      float                             FlsuPickup;
      float                             Utab_uf;                // Напряжение стандартной UF по заданной частоте Подхвата.
      float                             FinLsu;                 // Входное задание частоты генератора темпа
      u16                               GTState;                // Тек. режим - что делает ЗИ
      u16                               GTStatus;               // Тек. состояние ЗИ - статус
      u16                               PickupEnable;
      u16                               vPrUP;
      u16                               PickupStateFOC;         // Состояние подхвата в ВУ
      u16                               FirstRiseIdTime;
      u16                               SecondRiseIdTime;
      u16                               FirstFixedIdTime;
      u16                               RiseTorqueTime;
      float                             CurRegMultiple;
      float                             FluxExtimMultiple;
            
      u16                               status_WIFI;            // Для обработки в блоке PORT_UART

      float                             Debug0;      
      float                             Debug1;
      float                             Debug2;
      float                             Debug3;
      float                             Debug4;
      float                             Debug5;
      float                             Debug6;
      float                             Debug7;
      float                             Debug8;
      float                             Debug9;
      float                             Debug10;

      DrvType_Enum                      DrvType;               // Тип ПЧ (малой мощности и высокой мощности)
      DrvType_Enum                      DrvTypePreset;         // Тип силового блока выбираемый вручную
      CP24version_type                  versionCP24;            // Версия блока СР24
      UinPhasing_type                   UinPhasingFlag;         // Чередование фаз
      u16                               SvgenMode;              // Тип генератора ШИМа
      
      u16                               CrcProgFlash;           // CRC ПО
      u16                               VerPO;                  // версия ПО
      
      SimulationBlock_type              Simulation;
      
      PMSMAntirollback_type             pmsmAntirollback;
      extConnectedBlocksType            extConnectedBlocks;
      u16                               debugStartRx;   // Для отладки обмена с внешними блока
      EncoderParam_type                 EncoderParam;          // Параметры энкодера      
      u16                               Rsignal;
      calcEncoDataType                  EncoderCalcParam;      // Приемные данные от энкодера
      DinStateTxType                    DinDoutExtData; 
      AinAoutExtType                    AinAoutExtData;          //структура внешнего блока AinAout
      // Группа переменных для лифта
      s16                               s16_Brake;
      s16                               s16_Contactor;
      s16                               s16_PwmOnM3;
      s16                               s16_AlgState;
      s16                               s16_StartStop;
      s16                               s16_LiftPuskOut;
      s16                               s16_VprUpState;
      s16                               s16_F_Ref;
      s16                               s16_PuskStateDRV;
      f32                               timeLoadContyCheck;
      f32                               FixStopTime;
      
      PretorqueBlock_type               Pretorque;
      
      u16                               DF_NumSpeed;      
      u16		                timeUI_Up;		// время подъёма напряжения и тока пуска, мсек
      f32                               ImaxEvac;               // Максимальный активный ток в режиме эвакуации.
      f32                               MultIdKoefGM;           // Коэф. повышения тока возбуждения в генераторном режиме
      u16                               GenMode;
      
      FlashDMABlock_type                BLOCK_DMA;
      u16                               AdjCoefFault;  // Ошибка расположения калибровок в памяти FRAM
      
}GlobalVar;
                
extern GlobalVar        GlobalM4;

// Обменная структура для обмена с загрузчиком
#ifdef _PROJECT_FOR_STM32_
__no_init BootCommonData_type BootCommonData @ 0x20000000;   // Обменная структура с загрузчиком
#else
 BootCommonData_type BootCommonData;
#endif //_PROJECT_FOR_STM32_



#endif //GLOBALVAR_H_