/* =================================================================================
File name:   DRIVE_CONST.H                   
                    
Originator:  Triol 

Description: Структура обмена параметрами с ядром ВУ асинхронным двигателем

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 25-12-2010 Version 1.00
 26-01-2012 Version 2.00: Удалены не используемые переменные, наведена красота
                          изменено название файла (ранее drive_param.h)
 22-03-2012 Version 3.00: Отредактирована обменная структура
 23-09-2020 Version 4.00: PAN (Перевел во float)
------------------------------------------------------------------------------*/
#ifndef __DRIVECONST_H__
#define __DRIVECONST_H__

#include "type.h"
#include "GlobalDefines.h"

typedef struct { 
                ////// Input Parameters ///////
                u16    DriveType;       // Input: Выбор типа управления двигателем
                float  Fpwm;            // Input: Частота дискретизации (Гц)
                float  Rcab;            // Input: Сопротивление длинной линии (более 100м) (Ом) 
                float  TregSpd;         // Input: Период обсчета контура скорости (с)
                float  FcutSpd;         // Input: Частота среза фильтра измеренной скорости (Гц)
                float  TcutSpd;         // Постоянная времени частоты среза КС
                float  CompDelaySpd;    // Input: Компенсация задержки фильтрованной скорости
                u16    EncoFlt_N;       // Размер фильтра задается через систему параметров
                 ////// Parameters for Measurement Motor Parameters ///////
                float  Fpwm_MMP;        // Input: Частота дискретизации режима измерения параметров (Гц)
                float  TUsn_MMP;        // Input: Время выхода напряжения на уровень ном. напряжения двигателя (Usn)
                ////// Parameters for Dead Time Compensation ///////
                //float  DeadTime;        // Input: Dead Time (mks)
                DeadTimeInit_type DeadTimeInit;          // Структура параметров настройки модуля компенсации deadTime
                u16    SvgenMode;       // Input: Svgen mode - space vector vs DPWM60 
                float  Rsw;             // Input: Сопротивление ключа инвертора (Ом)
                float  Udf;             // Input: Напряжение включения обратного диода (В)
                float  dI;              // Input: Минимальный ток переключения компенсации МВ (А)
                ////// ACI Motor Parameters ///////
                float  Isn;             // Input: Номинальный ток двигателя, действующее значение (pu)
                float  Usn;             // Input: Номинальное напряжение АД, действ. линейное значение (pu)
                float  Fnom;            // Input: Номинальная частота питающего напряжения двигателя (pu)
                float  Wnom;            // Input: Номинальная частота ротора  (pu)
                float  Wmax;            // Input: Максимальная частота ротора (pu)
                u16    Zp;              // Input: Число пар полюсов
                float  Rs;              // Input: Сопротивление статора (Ом)
                float  Rr;              // Input: Сопротивление ротора  (Ом)
                float  Lm;              // Input: Индуктивность намагничивания (Гн) 
                float  Lls;             // Input: Индуктивность рассеивания статора (Гн)
                float  Jr;              // Input: Момент инерции (кг*м2)
                float  Ixx;			  // Ток ХХ двигателя.
                //////    PMSM Parameters     ///////
                float  Lsd;             // Input: Индуктивность статора по оси D 
                float  Lsq;             // Input: Индуктивность статора по оси Q
                float  PsiR;            // Input: Магнитный поток ротора
		float  Tpusk;           // Input: Время задержки на пусковой частоте (сек)
		float  Upusk;           // Input: Напряжение на пусковой частоте (В)
                ////// Encoder Parameter ///////
                u32    Encoder_N;       // Input: Число меток энкодера
                u32    Encoder_Nbit;    // Input: Число битов энкодера
                u16    Encoder_D;       // Input: Направление вращения энкодера
                u16    Encoder_Type;    // Input: Тип энкодера
                u16    Enco_NumFnom;    // Input: Количество меток на период ШИМ, при номинальной частоте
                u16    Enco_Shift;      // Смещение нулевой позиции энкодера
                ////// Regulator Reference Parameters ///////
                float  Imax;            // Input: Максимальный ток, действующее значение (pu)
                float  Wrz;             // Input: Задание скорости ротора (pu)
                float  Mez;             // Input: Задание по электромагнитному моменту (pu)
                float  Kez;             // Input: Коэффициент задания по ЭДС 
                //////     Fast Inputs        ///////
                float  IsU;             // Input: Ток статора по фазе U, мгновенное значение (pu)
                float  IsV;             // Input: Ток статора по фазе V, мгновенное значение (pu)
                float  IsW;             // Input: Ток статора по фазе W, мгновенное значение (pu)
		float  IUforDT;         // Input: Ток для мертвого времени по фазе U, мгновенное значение (pu)
		float  IVforDT;         // Input: Ток для мертвого времени по фазе V, мгновенное значение (pu)
		float  IWforDT;         // Input: Ток для мертвого времени по фазе W, мгновенное значение (pu)
                float  Udc;             // Input: Напряжение звена постоянного тока (pu)
                float  Uscl;            // Input: Напряжение, для скалярного управления, действующие значение  (pu)
                float  UsUV;            // Input: Линейное напряжение статора по фазе U, мгновенное значение (pu)
                float  UsVW;            // Input: Линейное напряжение статора по фазе V, мгновенное значение (pu)
                float  UsWU;            // Input: Линейное напряжение статора по фазе W, мгновенное значение (pu)
                //////      Bases        ///////
                f32     FreqBASE;        // Input: Frequence base
                f32     VoltBASE;        // Input: Voltage base  
                f32     CurrBASE;        // Input: Current base  
                f32     PowerBASE;       // Input: Power base  
                f32     TorqBASE;        // Input: Torque base  
                f32     FluxBASE;        // Input: Flux base 
                f32     KBase;           //
                ////// Speed Regulator Parameters ///////
                f32     KpSpd;            // Input: Пропорциональный коэффициент регулятора скорости               
                f32     KiSpd;            // Input: Интегральный коэффициент регулятора скорости
                f32     KpForced;         // Input: Пропорциональный коэффициент регулятора скорости при фрсировке (антиоткат)
                f32     SpdForced;        
                f32     KiForced;         // Input: Интегральный коэффициент регулятора скорости при форсировке (антиоткат)
                f32     RejectFreq; 	// Частота режекции режекторного фильтра
                f32     RejectBand;       // Полоса режекции режекторного фильтра
                f32     NewRefFiltrOut;   // Задание скорости после фильтра
                f32     Tzad;             // Выход регулятора скорости
                f32     Tsl;              // Постоянная контура скорости, для измерения параметров
                ////// Current Regulator Parameters ///////
                f32     KpCur;            // Input: Пропорциональный коэффициент регулятора тока               
                f32     KiCur;            // Input: Интегральный коэффициент регулятора тока

                f32  TfiltVolWeak;    // Input: Постоянная времени фильтра ограничения регулятора потокосципления
                f32  TudcFilt;
                
                ////// Position Regulator Parameters ///////
                float posRegGain;		  // Input: Пропорциональный коэффициент регулятора положения
                float zPos;             // Input: Задание положения ротора (pu)
                //////      Outputs      ///////
                float  WrH;             // Output: Верхний предел скорости ограничивающего диапазона (pu)
                float  WrL;             // Output: Нижний  предел скорости ограничивающего диапазона (pu)
                float  Idn;             // Output: Номинальный ток намагничивания АД, амплитудное значение (pu)
                float  Istn;            // Output: Номинальный ток АД, амплитудное значение (pu)
                float  PhiR_nom;        // Output: Номинальное потокосцепление ротора (pu)
                float  PhiR_min;        // Output: Минимальное рабочее потокосцепление ротора (pu)
                float  Mnom;            // Output: Номинальный момент двигателя (Нм) 
                float  Ls;              // Output: Индуктивность статора (Гн)
                float  Lr;              // Output: Индуктивность ротора (Гн)
                float  sigmaLs;         // Output: Полная индуктивность рассеивания(Гн)
                float  sigma;
                u16    DivRegSpd;       // Output: Делитель частоты вызова регулятора скорости и расчетчика скорости
                u16    jamming_dampingControl;
                //////  Variable Outputs ///////
                float  Mout;            //
                float  Mout1;           //
                float  Me;              // Output: Текущий электромагнитный момент (pu)
                float  Us;              // Output: Текущие напряжение статора, действующие значение (pu)
                float  Pm;              // Output: Текущая мощность на валу двигателя без учета КПД (pu)
                float  Wr;              // Output: Текущая скорость ротора умноженная на число пар полюсов (pu)
                float  We;              // Output: Текущая скорость поля (pu)
                float  PhiR;            // Output: Текущие потокосцепление ротора, амплитудное значение (pu)
                float  Angle;           // Output: Текущий угол выходного поля (pu)
                float  StepAngle;       // Output: Прирост угла выходного поля (pu)
                u16    Period;          // Output: Флаг периода выходной частоты поля (сброс внешний)
                u16    Pol;             // Output: Число полюсов
                //////  References for PWM ///////
                s16    Tu;              // Output: Время открытия ключа по фазе U (задание на ШИМ) (pu)
                s16    Tv;              // Output: Время открытия ключа по фазе V (задание на ШИМ) (pu)
                s16    Tw;              // Output: Время открытия ключа по фазе W (задание на ШИМ) (pu)
                //////  Voltage Outputs ///////
                float  UsU;             // Output: Напряжение статора по фазе U, мгновенное значение (pu)
                float  UsV;             // Output: Напряжение статора по фазе V, мгновенное значение (pu)
                float  UsW;             // Output: Напряжение статора по фазе W, мгновенное значение (pu)
                float  UsAlpha;         // Output: Напряжение статора по оси Alpha, мгновенное значение (pu)
                float  UsBeta;          // Output: Напряжение статора по оси Beta, мгновенное значение (pu)
                //////  Inside Variables ///////
                float  Tpwm;            // Variable: Sampling period (sec) 
                float  Wnom_Hz;         // Variable: Номинальная частота ротора  (Hz)
                float  Wmax_Hz;         // Variable: Максимальная частота ротора (Hz)
                float  Imax_f32;        // Variable: Максимальный ток, действующее значение (A)
                float  ImaxAmpl_f32;    // Variable: Максимальный ток, амплитудное значение (A)
//                float  KpSpd_iq;        // Variable: Пропорциональный коэффициент регулятора скорости 
//                float  KiSpd_iq;        // Variable: Интегральный коэффициент регулятора скорости     
                float  PhiR_min_iq;     // Variable: Минимальное рабочее потокосцепление ротора (pu)
                float  Idn_iq;          // Variable: Номинальный ток намагничивания АД, амплитудное значение (pu)
                float  Ustmax;          // Variable: Задание для регулятора ЭДС (pu)
                float  UmaxIqId;        // Variable: Ограничение напряжения для регулятора тока (pu)
                
                float  Ixx_Ampl;	// Ток холостого хода двигателя
                float  EmfRef_iq;	// Задание на регулятор противоЭДС
                float  EmfFdb_K;        // Коэф. для перевода обратной связи EMF
                //////  Debug Parameters ///////
                u16    SigSel1;         // Input: Выбор выводимого сигнала 1
                u16    SigSel2;         // Input: Выбор выводимого сигнала 2
                float  Signal1;         // Output: Сигнал №1
                float  Signal2;         // Output: Сигнал №2

                float  Debug0;          // Input: Debug0
                float  Debug1;          // Input: Debug1
                float  Debug2;          // Input: Debug2
                float  Debug3;          // Input: Debug3

                u16    State_Drive;    // 08.02.2018  Cуходольский состояние привода    0 - старая программа 1 - одиночный  2 - master рег скорости  4 slave рег скорости 5 slave рег момента

                u16    StartWithPickup; // Флаг перехода с Подхвата
                u16    AbsErrCnt;       // Output: Absolut error counter
                u16    RingUpError; 
                //////  Function pointers ///////
                void  (*pwm_update)();                  // Указатель на функцию обновления задания на ШИМ
                void  (*pwm_on_off)();                  // Указатель на функцию вкл/выкл ШИМ
                void  (*loger_func)(u32, u32);          // Указатель на функцию логгера (два входных аргумента)
               } DRIVECONST;

typedef DRIVECONST *DRIVECONST_handle;

//DRIVECONST DrvConst;
/*------------------------------------------------------------------------------
Prototypes for the functions in DRIVE_CONST.C
------------------------------------------------------------------------------*/
void DriveInit(DRIVECONST_handle);
#endif 


