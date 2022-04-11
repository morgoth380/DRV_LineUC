#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "Global_Include.h"
#include "CurVoltMeas.h"
#include "GlobalVar.h"  
#include "Filter.h"
#include "Pjk_const.h"

#define SOGI_K_DEMPF	      1.0f     // 

void SOGI_Calculation (SOGI_TYPE *v, float input, float Fout, float _2PiTs, float K1, float K2);    // Расчет RMS по мгновенным значениям


// Расчет мгновенных значений
void meas_curvolt_calc(CURVOLTMEAS *v)
{
    float         cosU, sinU;
    float         As, Bs;                     // Выходные токи в двуфазной системе координат
    float         Uas, Ubs, Ias, Ibs, UasOld, UbsOld, IasOld, IbsOld;   // Входные напряжения в двуфазной системе координат
    float         Theta;
    float         Iact, Ireact;               // Промежуточные вычисления активных и реактивных токов(не фильтрованные)
    float	  K1, K2, wfilt_Ts, loc_We;
    float	  TkvU, TkvOmega, TkvI, TkvIOmega;             // Коеффициенты для фильтра Коваленкова 
    float         Uabloc, Ubcloc, Ucaloc;
    
    // Вычисление смещения каналов измерения входных напряжений
    s16_Filtr(v->Uab_adcParam.Offset, v->Uab_adc, v->Uab_adcParam.Filter, 15);
    s16_Filtr(v->Ubc_adcParam.Offset, v->Ubc_adc, v->Ubc_adcParam.Filter, 15);
    //s16_Filtr(v->Uca_adcParam.Offset, v->Uca_adc, v->Uca_adcParam.Filter, 16);
    
    // Переводим значения токов и напряжений к физическим велечинам
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
            
    // Входное напряжение
#warning А какой выбор делать при измерении входных напряжений для РВ24_М40??    
    if (v->PowerType == HighPow_Combo) { // Combo Uab , Uac
        v->Uab = (float)(v->Uab_adc - v->Uab_adcParam.Offset) * (v->Uab_adcParam.Gain * 2.0f);   // Uab             
        v->Uca = (float)(v->Uca_adcParam.Offset - v->Uca_adc) * (v->Uca_adcParam.Gain * 2.0f);  // тут инверсия!!!  - Uac        
        v->Ubc = (-(float)(v->Uab_adc - v->Uab_adcParam.Offset) - (float)(v->Uca_adcParam.Offset - v->Uca_adc)) * (v->Ubc_adcParam.Gain * 2.0f); //
    } else { // PB-24 Uab, Ucb
        v->Uab = (float)(v->Uab_adc - v->Uab_adcParam.Offset) * (v->Uab_adcParam.Gain * 2.0f); // Uab
        v->Ubc = (float)(v->Ubc_adcParam.Offset - v->Ubc_adc) * (v->Ubc_adcParam.Gain * 2.0f);  // тут инверсия!!!  - Ubc
        v->Uca = (-(float)(v->Uab_adc - v->Uab_adcParam.Offset) - (float)(v->Ubc_adcParam.Offset - v->Ubc_adc)) * (v->Uca_adcParam.Gain * 2.0f); //
    }
        
    // Напряжение в ЗПТ                  
    v->UdcFast = (float)v->Ud_adc * v->UdGain;
    v->Udc = f32_Filtr(v->Udc, v->UdcFast, 0.1f);   // Подфильтруем немного Ud    
    
    // Udc without filter
    v->Udc = v->UdcFast;
    
    
    // Вычисления действующих, активного и реактивного токов (накапливаем сумму квадратов)
    // Выходные токи
    v->IuSqrSum += (v->Iu * v->Iu);
    v->IvSqrSum += (v->Iv * v->Iv);
    v->IwSqrSum += (v->Iw * v->Iw);
    v->cntCosAdd++;
    // Входное напряжение
    v->UabSqrSum += (v->Uab * v->Uab);
    v->UbcSqrSum += (v->Ubc * v->Ubc);
    v->UcaSqrSum += (v->Uca * v->Uca);
    v->cntUinCosAdd++;
    
    // Преобразование Кларка (переход из трёхфазной системы координат в двухфазную)
    As = Ias = (0.66666666666667f * v->Iu) - ((v->Iv + v->Iw) * 0.3333333333f);  // 2/3* Iu - 1/3 * (Iv + Iw)
    Bs = Ibs = 0.57735026918963f * (v->Iv - v->Iw);                              // 1/sqrt(3) * (Iv - Iw)
    
    // Подфильтруем Альфа и Бета измеренных токов (Фильтр без задержек - Коваленкова)
    // Расчитать коеффициенты для фильтра
    TkvI      = v->Tsample /1e-3f; //* fabsf(v->Fout) * 0.25f;                   //  постоянная времени фильтра = 1/4 текущего периода выходного частоты
    TkvIOmega = v->Tsample * 2.0f * _PI  * fabsf(v->Fout);//FREQ_POWER;
    // Получить значение фильтра за прошлый период        
    IasOld = v->Ias;
    IbsOld = v->Ibs;
    // Собственно фильтр...
    Bs = v->Ias = IasOld + TkvI * (Ias - IasOld) - TkvIOmega * IbsOld;
    As = v->Ibs = IbsOld + TkvI * (Ibs - IbsOld) + TkvIOmega * IasOld;    
    // Находим мгновенный трехфазный угол тока. При обрыве фазы будет или Pi, или -Pi
    v->ThetaI = atan2f(v->Ibs, v->Ias); 
    //v->ThetaI = atan2f(Ibs, Ias); 
    //=========================================================================       
    // Преобразование Кларка (переход из трёхфазной системы координат в двухфазную)
    As = Ias = (0.66666666666667f * v->Iu) - ((v->Iv + v->Iw) * 0.3333333333f);  // 2/3* Iu - 1/3 * (Iv + Iw)
    Bs = Ibs = 0.57735026918963f * (v->Iv - v->Iw);                              // 1/sqrt(3) * (Iv - Iw)
    
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
    //=========================================================================   
    // Преобразование Кларка (для входных напряжений)
    Uas = (0.66666666666667f * v->Uab) - ((v->Ubc + v->Uca) * 0.3333333333f);  // 2/3* Uab - 1/3 * (Ubc + Uca)
    Ubs = 0.57735026918963f * (v->Ubc - v->Uca);                               // 1/sqrt(3) * (Ubc - Uca)
   
    // Подфильтруем Альфа и Бета входных напряжений (Фильтр без задержек - Коваленкова)
    // Расчитать коеффициенты для фильтра
    TkvU     = v->Tsample  / 4.0e-3f;                   // 3.5 - 4.5мс постоянная времени фильтра
    TkvOmega = v->Tsample * 2.0f * _PI  * FREQ_POWER;
    // Получить значение фильтра за прошлый период        
    UasOld = v->Uas;
    UbsOld = v->Ubs;
    // Собственно фильтр...
    v->Uas = UasOld + TkvU * (Uas - UasOld) - TkvOmega * UbsOld;
    v->Ubs = UbsOld + TkvU * (Ubs - UbsOld) + TkvOmega * UasOld;
    
    // Определим угол входной сети
    Theta = atan2f(v->Ubs, v->Uas);                                          // определение угла тета по первой гармонике сети
    
    // Определение последовательности фаз и периода входной сети
    if((v->Theta > _5PI_DIV6) && (Theta < -_5PI_DIV6)){
        v->Flags.PeriodUin = 1;         // Прямое чередование фаз
        v->Flags.PeriodUinCopy = 1; 
        v->UinPhasingFlag = (u16)PositivePhaseSequence;
    }
    else if((v->Theta < -_5PI_DIV6) && (Theta > _5PI_DIV6)){
        v->Flags.PeriodUin = 1;         // Обратное чередование фаз
        v->Flags.PeriodUinCopy = 1;
        v->UinPhasingFlag = (u16)NegativePhaseSequence;
    }
    v->Theta = Theta;
    
    
    
    // В останове ПЧ обнулить выходные токи    
    if(!v->Flags.DrvStart){
       // Отфильтровуем постоянную составляющую АЦП для токов
//       s16_Filtr(v->Iu_adcParam.Offset, v->Iu_adc, v->Iu_adcParam.Filter, 8);
//       s16_Filtr(v->Iv_adcParam.Offset, v->Iv_adc, v->Iv_adcParam.Filter, 8);
//       s16_Filtr(v->Iw_adcParam.Offset, v->Iw_adc, v->Iw_adcParam.Filter, 8);
       
      meas_CalibrOffset(v); // Калибровка нулей фазных токов
      
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

    // Период выходной частоты завершен
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
 
    // Период входной сети завершен
    if(v->Flags.PeriodUin) {
        v->UabSqrSumCopy = v->UabSqrSum;
        v->UbcSqrSumCopy = v->UbcSqrSum;
        v->UcaSqrSumCopy = v->UcaSqrSum;

        v->cntUinCosAddCopy = v->cntUinCosAdd;

        v->UabSqrSum = v->UbcSqrSum = v->UcaSqrSum = 0;       // обнуляем интеграторы
        v->cntUinCosAdd = 0;                                  // обнуляем счётчик                  
        v->Flags.PeriodUin = 0;

        // расчитать RMS входного напряжения        
//        arm_sqrt_f32(v->UabSqrSumCopy / v->cntUinCosAddCopy, &v->Uab_RMS);                         
//        arm_sqrt_f32(v->UbcSqrSumCopy / v->cntUinCosAddCopy, &v->Ubc_RMS);                         
//        arm_sqrt_f32(v->UcaSqrSumCopy / v->cntUinCosAddCopy, &v->Uca_RMS);         
        arm_sqrt_f32(v->UabSqrSumCopy / v->cntUinCosAddCopy, &Uabloc);                         
        arm_sqrt_f32(v->UbcSqrSumCopy / v->cntUinCosAddCopy, &Ubcloc);                         
        arm_sqrt_f32(v->UcaSqrSumCopy / v->cntUinCosAddCopy, &Ucaloc);         
            
        v->Uab_RMS = f32_Filtr(v->Uab_RMS, Uabloc, 0.1f);   // Подфильтруем немного входные напряжения
        v->Ubc_RMS = f32_Filtr(v->Ubc_RMS, Ubcloc, 0.1f);   // Подфильтруем немного входные напряжения
        v->Uca_RMS = f32_Filtr(v->Uca_RMS, Ucaloc, 0.1f);   // Подфильтруем немного входные напряжения

            
        v->UabSqrSumCopy = v->UbcSqrSumCopy = v->UcaSqrSumCopy = 0;    // обнуляем интеграторы
        v->cntUinCosAddCopy = 0;                                       // обнуляем счётчик
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

// Функция калибровки Оффсет каналов измерения тока (Вызывать перед каждым пуском)
void meas_CalibrOffset(CURVOLTMEAS *v){
  // Отфильтровуем постоянную составляющую АЦП для токов
    s16_Filtr(v->Iu_adcParam.Offset, v->Iu_adc, v->Iu_adcParam.Filter, 6);
    s16_Filtr(v->Iv_adcParam.Offset, v->Iv_adc, v->Iv_adcParam.Filter, 6);
    s16_Filtr(v->Iw_adcParam.Offset, v->Iw_adc, v->Iw_adcParam.Filter, 6);
}

void meas_period_calc(CURVOLTMEAS *v){
  // Вычисление действующего значения выходного тока                                              
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
    
    // Расчет активной, полной мощности и косунис Фи
    v->Pact   = v->Us * _SQRT_3 * v->Iact * 0.001f;                     // * 0.001f - это перевод результата с Вт в кВт
    v->Preact = v->Us * _SQRT_3 * v->Ireact * 0.001f;
    v->Pfull  = v->Us * _SQRT_3 * v->Ifull * 0.001f;

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
#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)