#ifndef _CURVOLTMEAS_H_
#define _CURVOLTMEAS_H_

#include "type.h"

typedef struct {  s32    Filter;             // Отфильтрованные значения
                  float  Gain;               // Коэффициент пересчёта в float
                  s16    Offset;             // Смещение измерений
               } ADC_TYPE;

typedef struct {  u16  DrvStart     :1;      // Флаг отображающий работу привода 
                  u16  Period       :1;      // Флаг периода выходной частоты
                  u16  CalibrOffset :1;      // Флаг инвертирования токов
                  u16  TwoCur       :1;      // Флаг два датчика тока
                  u16  CurInv       :1;      // Инверсия токов
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
                  float     Us;              // Вход:      Выходное напряжение инвертора
                  float     Fout;            // Вход:      Выходная частота инвертора
                  float     Angle;           // Вход:      Угол формируемого напряжения  
                  ADCF_TYPE Flags;           // Вход:      Флаги необходимые для рассчетов
                  float     Iu;              // Выход:     Значение тока Iu
                  float     Iv;              // Выход:     Значение тока Iv
                  float     Iw;              // Выход:     Значение тока Iw
                  float     Il;              // Выход:     Значение тока Утечки
                  float     Iflash;          // Выход:     Полный ток двигателя, мгновенный
                  float	    Il_Filtr;	     // Выход:     Значение тока Утечки фильтрованное, для снижения уровня шумов в канале.
                  float     Iu_RMS;          // Выход:     Действующее значение тока Iu
                  float     Iv_RMS;          // Выход:     Действующее значение тока Iv
                  float     Iw_RMS;          // Выход:     Действующее значение тока Iw
                  float     Il_RMS;          // Выход:     Действующее значение тока утечки
                  float     UdcFast;         // Выход:     Физическое значение (Не Фильтрованное)
                  float     Udc;             // Выход:     Физическое значение IQ (Фильтрованное)
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
                  float     UdGain;          // Параметры: Юстировочное число напряжения Ud
                  float     IuSqrSum;        // Параметр:  Интегратор по току Iu
                  float     IvSqrSum;        // Параметр:  Интегратор по току Iv
                  float     IwSqrSum;        // Параметр:  Интегратор по току Iw
                  float     IlSqrSum;        // Параметр:  Интегратор по току утечки
                  float     IuSqrSumCopy;    // Параметр:  Копия интегратора по току Iu
                  float     IvSqrSumCopy;    // Параметр:  Копия интегратора по току Iv
                  float     IwSqrSumCopy;    // Параметр:  Копия интегратора по току Iw
                  float     IlSqrSumCopy;    // Параметр:  Копия интегратора по току утечки
                  u16       cntCosAdd;       // Параметр:  Cчётчик измеренных значений за период
                  u16       cntCosAddCopy;   // Параметр:  Копия счётчика измеренных значений за период
                  SOGI_TYPE Sogi_Iu;	     // Альтернатива расчету RMS
                  SOGI_TYPE Sogi_Iv; 	     // Альтернатива расчету RMS
                  SOGI_TYPE Sogi_Iw; 	     // Альтернатива расчету RMS
                  SOGI_TYPE Sogi_Il;	     // Альтернатива расчету RMS
                  float	    Sogi_2PiTs;      // Константа для расчетов
                  float	    Tsample;         // Период ШИМа
                  float	    We;	             // Скорость поля
                  void  (*fast_calc)();      // Pointer to calculation function
                  void  (*period_calc)();    // Pointer to calculation function
                  void  (*meas20ms_calc)();  // Pointer to calculation function
                  void  (*CalibrOffset)();   // Pointer to calculation function
                  void  (*InitCalibrOffset)();   // Pointer to calculation function
                  } CURVOLTMEAS;            

typedef CURVOLTMEAS * CURVOLTMEAS_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the CURVOLTMEAS_type object.
-----------------------------------------------------------------------------*/                        
#define ADC_DEFAULTS         {  (s32)2048 << 16,    \
                                0,    \
                                2048, \
                             }


#define SQRT_FROM_THREE         1.732050808f

#define CURVOLTMEAS_DEFAULTS {  0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                0,               \
                                {0,0,0,0},       \
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
                                (void (*)(u32))meas_curvolt_calc, \
                                (void (*)(u32))meas_period_calc,  \
                                (void (*)(u32))meas_20ms_calc,    \
                                (void (*)(u32))meas_CalibrOffset, \
                                (void (*)(u32))meas_InitCalibrOffset, \
                               }

/*------------------------------------------------------------------------------
Prototypes for the functions in CurVoltMeas.C
------------------------------------------------------------------------------*/
void meas_curvolt_calc(CURVOLTMEAS_handle);
void meas_period_calc(CURVOLTMEAS_handle);
void meas_20ms_calc(CURVOLTMEAS_handle);
void meas_CalibrOffset(CURVOLTMEAS_handle);   // Функция калибровки Оффсет каналов измирения тока
void meas_InitCalibrOffset(CURVOLTMEAS_handle);   // Функция калибровки Оффсет каналов измирения тока


#endif // CURVOLTMEAS_H
