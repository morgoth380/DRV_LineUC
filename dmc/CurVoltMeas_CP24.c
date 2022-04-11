#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "Global_Include.h"
#include "CurVoltMeas.h"
#include "GlobalVar.h"  
#include "Filter.h"
#include "Const.h"

#define SOGI_K_DEMPF	1.0f // 

void SOGI_Calculation (SOGI_TYPE *v, float input, float Fout, float _2PiTs, float K1, float K2);    // Расчет RMS по мгновенным значениям


// Расчет мгновенных значений
void meas_curvolt_calc(CURVOLTMEAS *v)
{
    float         cosU, sinU;
    float         As, Bs;                     // Выходные токи в двуфазной системе координат
    float         Iact, Ireact;               // Промежуточные вычисления активных и реактивных токов(не фильтрованные)
    float	  K1, K2, wfilt_Ts, loc_We;

//    // Переводим значения токов и напряжений к физическим величинам
//    v->Iu  = (float)(v->Iu_adc - v->Iu_adcParam.Offset) * (v->Iu_adcParam.Gain * 2.0f);   
//    v->Iv  = (float)(v->Iv_adc - v->Iv_adcParam.Offset) * (v->Iv_adcParam.Gain * 2.0f);
//    v->Iw  = (float)(v->Iw_adc - v->Iw_adcParam.Offset) * (v->Iw_adcParam.Gain * 2.0f);

    // Переводим значения токов и напряжений к физическим величинам
    // Выходные токи
    if(v->Flags.CurInv){    // Инверсия токов!!!
        v->Iu  = (float)(v->Iu_adcParam.Offset - v->Iu_adc) * (v->Iu_adcParam.Gain * 2.0f);   
        v->Iv  = (float)(v->Iv_adcParam.Offset - v->Iv_adc) * (v->Iv_adcParam.Gain * 2.0f);
        v->Iw  = (float)(v->Iw_adcParam.Offset - v->Iw_adc) * (v->Iw_adcParam.Gain * 2.0f);
    }
    else{                    // Без инверсии токов
        v->Iu  = (float)(v->Iu_adc - v->Iu_adcParam.Offset) * (v->Iu_adcParam.Gain * 2.0f);   
        v->Iv  = (float)(v->Iv_adc - v->Iv_adcParam.Offset) * (v->Iv_adcParam.Gain * 2.0f);
        v->Iw  = (float)(v->Iw_adc - v->Iw_adcParam.Offset) * (v->Iw_adcParam.Gain * 2.0f);
    }
    
    v->UdcFast = (float)v->Ud_adc * v->UdGain;
    v->Udc = f32_Filtr(v->Udc, v->UdcFast, 0.1f);   // Подфильтруем немного Ud

    // Вычисления действующих, активного и реактивного токов
    v->IuSqrSum += (v->Iu * v->Iu);
    v->IvSqrSum += (v->Iv * v->Iv);
    v->IwSqrSum += (v->Iw * v->Iw);
    
    v->cntCosAdd++;

    // Преобразование Кларка (переход из трёхфазной системы координат в двухфазную)
    As = (0.66666666666667f * v->Iu) - ((v->Iv + v->Iw) * 0.3333333333f);  // 2/3* Iu - 1/3 * (Iv + Iw)
    Bs = 0.57735026918963f * (v->Iv - v->Iw);                              // 1/sqrt(3) * (Iv - Iw)
    
    arm_sqrt_f32((As * As) + (Bs * Bs), &v->Iflash);
        
    // Преобразование Парка относительно вектора напряжения и его фильтрация апериодическим фильтром
    cosU = arm_cos_f32(v->Angle);
    sinU = arm_sin_f32(v->Angle);

    // Активный ток
    Iact   = ((As * cosU) + (Bs * sinU)) * 0.707106781186f;     // (Id*cos(fi) + Iq*sin(fi))*(1/sqrt(2))
    v->Iact = f32_Filtr(v->Iact, Iact, 0.03125f);               // Подфильтруем немного Iact
                   
    // Реактивный ток
    Ireact = ((Bs * cosU) - (As * sinU)) * 0.707106781186f;     // (Id*sin(fi) - Iq*cos(fi))*(1/sqrt(2))
    if(v->Fout < 0)  Ireact = -Ireact;                          // Проверим знак выходной частоты 
    v->Ireact = f32_Filtr(v->Ireact, Ireact, 0.03125f);         // Подфильтруем немного Iact
    
    if(!v->Flags.DrvStart){
        // принудительное обнуление токов в останове
        v->Iu_RMS = v->Iv_RMS = v->Iw_RMS = v->Il_RMS = 0;
        v->Iact = v->Ireact = v->Ifull = 0;
        v->Iu = v->Iv = v->Iw = 0.0f;            // Мгновенные значения тока  
        
        v->Sogi_2PiTs = _2PI * v->Tsample;       // Расчет константы
        v->Sogi_Iu.Sogi_Ampl = v->Sogi_Iu.Sogi_Y = v->Sogi_Iu.Sogi_YPrime = 0;
        v->Sogi_Iv.Sogi_Ampl = v->Sogi_Iv.Sogi_Y = v->Sogi_Iv.Sogi_YPrime = 0;
        v->Sogi_Iw.Sogi_Ampl = v->Sogi_Iw.Sogi_Y = v->Sogi_Iw.Sogi_YPrime = 0;
        v->Sogi_Il.Sogi_Ampl = v->Sogi_Il.Sogi_Y = v->Sogi_Il.Sogi_YPrime = 0;
    }

    if(v->Flags.Period) {
        v->IuSqrSumCopy = v->IuSqrSum;
        v->IvSqrSumCopy = v->IvSqrSum;
        v->IwSqrSumCopy = v->IwSqrSum;

        v->IlSqrSumCopy = v->IlSqrSum;
        v->cntCosAddCopy = v->cntCosAdd;

        v->IuSqrSum = v->IvSqrSum = v->IwSqrSum = v->IlSqrSum = 0;            // обнуляем интеграторы

        v->cntCosAdd = 0;                                                     // обнуляем счётчик                  
        v->Flags.Period = 0;
    }
 
    //*************************************************************************
    // Расчет RMS
    //*************************************************************************
    loc_We = fabsf(v->Fout);        // Надо использовать абсолютное значение
    if (loc_We < 0.2f) {            // И надо использовать значение не ниже 0,2 Гц
    	loc_We = 2.0f;              // Если частота ниже 0.2Гц то ставим 2Гц
    }
    
    // Расчитать коеффициенты фильтра
    wfilt_Ts = (v->Tsample * _2PI * loc_We) / 4.0f; // частота среза фильтра в 4 раза меньше текущей частоты сигнала (зафильтровать паразитный сигнал в амплитуде равный двойной частоте сигнала)
    K1 = wfilt_Ts /(1.0f + wfilt_Ts);
    K2 = 1.0f / (1.0f + wfilt_Ts);

    SOGI_Calculation (&v->Sogi_Iu, v->Iu, loc_We, v->Sogi_2PiTs, K1, K2);
    SOGI_Calculation (&v->Sogi_Iv, v->Iv, loc_We, v->Sogi_2PiTs, K1, K2);
    SOGI_Calculation (&v->Sogi_Iw, v->Iw, loc_We, v->Sogi_2PiTs, K1, K2);

}

void SOGI_Calculation (SOGI_TYPE *v, float input, float Fout, float _2PiTs, float K1, float K2){
	float	SOGI_error1, SOGI_error2;

	SOGI_error1 = (input - v->Sogi_Y) * SOGI_K_DEMPF;
	SOGI_error2 = SOGI_error1 - v->Sogi_YPrime;

	v->Sogi_Y = v->Sogi_Y + (SOGI_error2 * _2PiTs * Fout);
	v->Sogi_YPrime = v->Sogi_YPrime + (v->Sogi_Y * _2PiTs * Fout);

	// Вычисление амплитуды сигнала
	arm_sqrt_f32((v->Sogi_Y * v->Sogi_Y) + (v->Sogi_YPrime * v->Sogi_YPrime), &v->Sogi_Ampl);
	v->Sogi_Ampl_Filtr = (K2 * v->Sogi_Ampl_Filtr) + (K1 * v->Sogi_Ampl);                         // Фильтруем значение

	// Вычисление действующего значения сигнала
	v->Sogi_RMS = v->Sogi_Ampl_Filtr * _1DIV_SQRT_2;

}

void meas_period_calc(CURVOLTMEAS *v){
    // Вычисление действующего значения                                               
    arm_sqrt_f32(v->IuSqrSumCopy / v->cntCosAddCopy, &v->Iu_RMS);                         
    arm_sqrt_f32(v->IvSqrSumCopy / v->cntCosAddCopy, &v->Iv_RMS);                         
    arm_sqrt_f32(v->IwSqrSumCopy / v->cntCosAddCopy, &v->Iw_RMS);         
    
    v->IuSqrSumCopy = v->IvSqrSumCopy = v->IwSqrSumCopy = v->IlSqrSumCopy = 0;  // обнуляем интеграторы

    v->cntCosAddCopy = 0;                                                       // обнуляем счётчик                 
}

void meas_20ms_calc(CURVOLTMEAS *v){
     float tmp1;

    // расчет полного тока
    arm_sqrt_f32((v->Iact * v->Iact) + (v->Ireact * v->Ireact), &v->Ifull);
    
    // Смещение середины АЦП для канала Imax
    //v->ImaxOffset = (s16)((v->Iu_adcParam.Offset + v->Iv_adcParam.Offset + v->Iu_adcParam.Offset) * 0.3333333f + 0.5f);   // Среднеарифметическое с округлением

    // Расчет активной, полной мощности и косунис Фи
    v->Pact   = v->Us * SQRT_FROM_THREE * v->Iact * 0.001f;                     // * 0.001f - это перевод результата с Вт в кВт
    v->Preact = v->Us * SQRT_FROM_THREE * v->Ireact * 0.001f;
    v->Pfull  = v->Us * SQRT_FROM_THREE * v->Ifull * 0.001f;

   // if(v->Pact < 0)
   //     v->Pact = 0;

    v->Preact = -v->Preact;
    if(v->Preact < 0)
        v->Preact = 0;

    if(v->Pfull <= 0.001f){
        v->cosFi = 0;
        v->Pfull = 0;
    }else{
        tmp1 = v->Pact / v->Pfull;
        v->cosFi = (tmp1 > 1.0f) ? 1.0f : tmp1;
    }
}

// Функция калибровки Оффсет каналов измерения тока (Вызывать перед каждым пуском)
void meas_CalibrOffset(CURVOLTMEAS *v){
  // Отфильтровуем постоянную составляющую АЦП для токов
    s16_Filtr(v->Iu_adcParam.Offset, v->Iu_adc, v->Iu_adcParam.Filter, 5);
    s16_Filtr(v->Iv_adcParam.Offset, v->Iv_adc, v->Iv_adcParam.Filter, 5);
    s16_Filtr(v->Iw_adcParam.Offset, v->Iw_adc, v->Iw_adcParam.Filter, 5);
}

void meas_InitCalibrOffset(CURVOLTMEAS *v){
    // Начальная инициализация значение фильтра по первому значению после накачки
    v->Iu_adcParam.Offset = v->Iu_adcParam.Filter = v->Iu_adc;
    v->Iv_adcParam.Offset = v->Iv_adcParam.Filter = v->Iv_adc;
    v->Iw_adcParam.Offset = v->Iw_adcParam.Filter = v->Iw_adc;
}


#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)