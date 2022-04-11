#ifndef _CURVOLTMEAS_H_
#define _CURVOLTMEAS_H_

#include "type.h"

#define FREQ_POWER      50.0f   // Частота сети



typedef struct {  s32    Filter;             // Отфильтрованные значения
                  float  Gain;               // Коэффициент пересчёта в float
                  s16    Offset;             // Смещение измерений
               } ADC_TYPE;

typedef struct {  u16  DrvStart      :1;      // Флаг отображающий работу привода 
                  u16  Period        :1;      // Флаг периода выходной частоты
                  u16  PeriodUin     :1;      // Флаг периода входной сети
                  u16  PeriodUinCopy :1;      // Флаг периода входной сети копия
                  u16  CurInv        :1;      // Флаг инвертирования токов
                  u16  TwoCur        :1;      // Флаг два датчика тока
               } ADCF_TYPE;

typedef struct { float  Sogi_Y;   	     // Прямой сигнал
		 float  Sogi_YPrime;         // Сопровождающий сигнал
		 float  Sogi_Ampl;	     // Амплитуда сигнала
		 float  Sogi_Ampl_Filtr;     // Сигнал Амплитуды фильтрованный
		 float  Sogi_RMS;	     // Сигнал RMS
		} SOGI_TYPE;


typedef struct {  s16       Iu_adc;          // Вход:      Значение тока Iu в отсчётах АЦП
                  s16       Iv_adc;          // Вход:      Значение тока Iv в отсчётах АЦП
                  s16       Iw_adc;          // Вход:      Значение тока Iw в отсчётах АЦП
                  s16       Ud_adc;          // Вход:      Значение напряжения Ud в отсчётах АЦП
                  s16       Uab_adc;         // Вход:      Значение напряжения Uab в отсчётах АЦП
                  s16       Ubc_adc;         // Вход:      Значение напряжения Ubc в отсчётах АЦП
                  s16       Uca_adc;         // Вход:      Значение напряжения Uca в отсчётах АЦП
                  u16       PowerType;       // Вход:      Тип силового блока PB 
                  float     Us;              // Вход:      Выходное напряжение инвертора
                  float     Fout;            // Вход:      Выходная частота инвертора
                  float     Angle;           // Вход:      Угол формируемого напряжения  
                  ADCF_TYPE Flags;           // Вход:      Флаги необходимые для рассчетов
                  float     Iu;              // Выход:     Значение тока Iu
                  float     Iv;              // Выход:     Значение тока Iv
                  float     Iw;              // Выход:     Значение тока Iw
                  float     Iflash;          // Выход:     Полный ток двигателя, мгновенный                  
                  float     Uab;             // Выход:     Значение напряжения Uab
                  float     Ubc;             // Выход:     Значение напряжения Ubc
                  float     Uca;             // Выход:     Значение напряжения Uca
                  float     Il;              // Выход:     Значение тока Утечки
                  float	    Il_Filtr;	     // Выход:     Значение тока Утечки фильтрованное, для снижения уровня шумов в канале.
                  float     Iu_RMS;          // Выход:     Действующее значение тока Iu
                  float     Iv_RMS;          // Выход:     Действующее значение тока Iv
                  float     Iw_RMS;          // Выход:     Действующее значение тока Iw
                  float     Il_RMS;          // Выход:     Действующее значение тока утечки
                  float     UdcFast;         // Выход:     Физическое значение (Не Фильтрованное)
                  float     Udc;             // Выход:     Физическое значение IQ (Фильтрованное)
                  float     Uab_RMS;         // Выход:     Действующее значение напряжения Uab
                  float     Ubc_RMS;         // Выход:     Действующее значение напряжения Ubc
                  float     Uca_RMS;         // Выход:     Действующее значение напряжения Uca
                  float     Iact;            // Выход:     Значение активного тока
                  float     Ireact;          // Выход:     Значение реактивного тока 
                  float     Ifull;           // Выход:     Полный ток
                  float     Ifull1;          // Выход:     Полный ток
                  float     Pact;            // Выход:     Активная мощность
                  float     Preact;          // Выход:     Реактивная мощность
                  float     Pfull;           // Выход:     Полная мощность
                  float     cosFi;           // Выход:     Косинус Фи 
                  s16       ImaxOffset;      // Выход: 
                  ADC_TYPE  Iu_adcParam;     // Параметры: всё что связано с током Iu в отсчётах АЦП                                               
                  ADC_TYPE  Iv_adcParam;     // Параметры: всё что связано с током Iv в отсчётах АЦП  
                  ADC_TYPE  Iw_adcParam;     // Параметры: всё что связано с током Iw в отсчётах АЦП  
                  ADC_TYPE  Uab_adcParam;    // Параметры: всё что связано с напряжением Uab в отсчётах АЦП  
                  ADC_TYPE  Ubc_adcParam;    // Параметры: всё что связано с напряжением Ubc в отсчётах АЦП
                  ADC_TYPE  Uca_adcParam;    // Параметры: всё что связано с напряжением Uca в отсчётах АЦП
                  float     UdGain;          // Параметры: Юстировочное число напряжения Ud
                  float     IuSqrSum;        // Параметр:  Интегратор по току Iu
                  float     IvSqrSum;        // Параметр:  Интегратор по току Iv
                  float     IwSqrSum;        // Параметр:  Интегратор по току Iw
                  float     IlSqrSum;        // Параметр:  Интегратор по току утечки
                  float     UabSqrSum;       // Параметр:  Интегратор по напряжению Uab
                  float     UbcSqrSum;       // Параметр:  Интегратор по напряжению Ubc
                  float     UcaSqrSum;       // Параметр:  Интегратор по напряжению Uca
                  float     IuSqrSumCopy;    // Параметр:  Копия интегратора по току Iu
                  float     IvSqrSumCopy;    // Параметр:  Копия интегратора по току Iv
                  float     IwSqrSumCopy;    // Параметр:  Копия интегратора по току Iw
                  float     IlSqrSumCopy;    // Параметр:  Копия интегратора по току утечки
                  float     UabSqrSumCopy;   // Параметр:  Интегратор по напряжению Uab (копия)
                  float     UbcSqrSumCopy;   // Параметр:  Интегратор по напряжению Ubc (копия)
                  float     UcaSqrSumCopy;   // Параметр:  Интегратор по напряжению Uca (копия)
                  u16       cntCosAdd;       // Параметр:  Cчётчик измеренных значений за период
                  u16       cntCosAddCopy;   // Параметр:  Копия счётчика измеренных значений за период
                  u16       cntUinCosAdd;
                  u16       cntUinCosAddCopy;
                  float     Uas;             // Альфа входного напряжения
                  float     Ubs;             // Бета входного напряжения
                  float     Theta;           // Угол сети
                  u16       UinPhasingFlag;  // Чередование входных фаз 
                  SOGI_TYPE Sogi_Iu;	     // Альтернатива расчету RMS
                  SOGI_TYPE Sogi_Iv; 	     // Альтернатива расчету RMS
                  SOGI_TYPE Sogi_Iw; 	     // Альтернатива расчету RMS
                  SOGI_TYPE Sogi_Il;	     // Альтернатива расчету RMS
                  float	    Sogi_2PiTs;      // Константа для расчетов
                  float	    Tsample;         // Период ШИМа
                  float	    We;	             // Скорость поля
                  float     ThetaI;          // Трехфазный угол тока
                  float     Ias;             // Альфа компонента тока
                  float     Ibs;             // Бета компонента тока
                  void  (*fast_calc)();      // Pointer to calculation function
                  void  (*period_calc)();    // Pointer to calculation function
                  void  (*meas20ms_calc)();  // Pointer to calculation function
                  void  (*CalibrOffset)();   // Pointer to calculation function                  
                  } CURVOLTMEAS;            

typedef CURVOLTMEAS * CURVOLTMEAS_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the CURVOLTMEAS_type object.
-----------------------------------------------------------------------------*/                        
#define ADC_DEFAULTS         {  (s32)2048 << 16,    \
                                0,                  \
                                2048,               \
                             }

#define CURVOLTMEAS_DEFAULTS {  0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                {0,0,0,0,0,0},   \
                                0,               \
				0,               \
				0,               \
				0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
				0,               \
				0,               \
				0,               \
                                0,               \
				0,               \
				0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                ADC_DEFAULTS,    \
                                ADC_DEFAULTS,    \
                                ADC_DEFAULTS,    \
                                ADC_DEFAULTS,    \
                                ADC_DEFAULTS,    \
                                ADC_DEFAULTS,    \
                                0,               \
				0,               \
				0,               \
                                0,               \
                                0,               \
                                0,               \
				0,               \
				0,               \
				0,               \
                                0,               \
                                0,               \
				0,               \
				0,               \
				0,               \
                                0,               \
                                0,               \
                                0,               \
				0,               \
				0,               \
				0,               \
                                0,               \
                                0,               \
                                0,               \
				{0,0,0,0,0},	 \
				{0,0,0,0,0},	 \
				{0,0,0,0,0},	 \
				{0,0,0,0,0},	 \
				0,		 \
				0,		 \
				0,		 \
				0,		 \
				0,		 \
				0,		 \
                                (void (*)(u32))meas_curvolt_calc, \
                                (void (*)(u32))meas_period_calc,  \
                                (void (*)(u32))meas_20ms_calc,    \
                                (void (*)(u32))meas_curvolt_calc,    \
                               }

/*------------------------------------------------------------------------------
Prototypes for the functions in CurVoltMeas.C
------------------------------------------------------------------------------*/
void meas_curvolt_calc(CURVOLTMEAS_handle);
void meas_period_calc(CURVOLTMEAS_handle);
void meas_20ms_calc(CURVOLTMEAS_handle);
void meas_CalibrOffset(CURVOLTMEAS *v);

#endif // CURVOLTMEAS_H
