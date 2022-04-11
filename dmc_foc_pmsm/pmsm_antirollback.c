/** =================================================================================
* \file		: pmsm_antirollback.c
*
* \brief	: Реализация модуля компенсации отката
* \details	: включает функции инициализации и расчета модуля.
*
* \autor	: Sosnovchyk D.M.
*
* \date		: 11.02.2021 \version	: 2.0
*
* \todo		: 1. 
*
* \copyright: (C)TRIOLCORP, 2021
*
=====================================================================================*/

#include "Global_include.h"
#include "pmsm_antirollback.h"
#include "Pjk_const.h"
//#include "type.h"

#define PMSM_ANTIROLLBACK_OFF   0
#define PMSM_ANTIROLLBACK_ID_FIX_FREQ     1 // Режим примагничивания к полюсу в зависимости от частоты. Ниже 2 Гц намагничение уменьшается до нуля
#define PMSM_ANTIROLLBACK_ID_FIX_TIME	  2 // Режим примагничивания к полюсу в зависимости от времени. Падение начинается после снятия тормоза
#define PMSM_ANTIROLLBACK_ENCO_PHASING    3 // Режим фазировки энкодера

#define FREQ_TIME_MODE           1
#define BEFORE_ACCELERATE_MODE   2

void pmsm_antirollback_calc(PMSMANTIROLLBACK_handle v)
{
    f32	        tempId, timeKStep, timeKStepDown; // Kf

	// Перенаправление
    v->outId = v->inId;
    v->outIq = v->inIq;
    v->outSpeedWr = v->inSpeedWr;
    v->outTheta = v->inTheta;    

    //расчёт шага коэффициента плавного изменения тока
    //ф-ция вызывается с частотой Fpwm
    //коэффициент 1000 - для перевода времени из мсек в сек
    timeKStep = (1000.0f / (v->Fpwm * v->timeUI_Up));
    timeKStepDown = 1000.0f / (v->Fpwm * v->timeStepDown);

    // Если ШИМ выключен - взводим флаг разрешения работы алгоритма в старте
    if(!v->pwmOn) {
    	v->onStartWork  = 1;
    	v->fixIdDone    = 0;
    	v->fixIdDoneCnt = (u16)(v->FixTimeId * v->Fpwm / 1000.f); // Определяем число тиков в тактах ШИМа
    }

    // Определяем режим работы алгоритмов
    if (v->bitPhasing && v->Enable) {            
      // Стадия фазировки энкодера
      
      // 1. Пока ШИМ выключен, мы фазу на выход обнуляем. В этом случае и задание частоты бужет ноль.
      if (!v->pwmOn) {
        v->outTheta = 0.0f;        
      }
            
      // 2. Рассчитываем выходную фазу в режиме фазировки энкодера. Вроде как она нужна в радианах !?!?!?
      v->outTheta = v->outTheta + ((1.0f/v->Fpwm) * IQ_to_FLOAT(v->inSpeedWrz, Herz_base) * _2PI);
      // Ограничиваем фазу по диапазону.
      if(v->outTheta > _2PI) {
        v->outTheta -= _2PI;
      } else if (v->outTheta < -_2PI) {
        v->outTheta += _2PI;
      }
      
      // 3. Формируем задание выходного тока удержания с плавным подъемом. Сброс тока в конце работы будет проходить штатным плавным сбросом тока.
      // PMSM_ANTIROLLBACK_ID_FIX_TIME: v->fixId up to v->inId
      if (v->onStartWork /*&& v->fixIdDoneCnt*/) { // Если ШИМ включен, частота задания 0, и счетчик фиксации еще не ноль - даем рост возбуждения
        tempId = (v->fixId * v->timeKoef); //плавное нарастание/спад тока возбуждения
        // Если вычисленная величина больше входного значения, заменяем ее. Иначе оставляем входное значение.
        if (tempId > v->inId) {
          v->outId = tempId; // Заменяем, иначе тут будет значение минимального тока 
        }
        
        if(v->pwmOn) {
          //повышаем коэффициент плавного изменения тока от 0 до 1
          if (v->timeKoef < 1.0f)       v->timeKoef += timeKStep;
          else                          v->timeKoef = 1.0f;          
        } else {
          //сброс коэффициента плавного изменения тока
          v->timeKoef = 0;
        }
      }
      
/*      
      // Фазу выдаем равной нулю, для отбивки по нулевой позиции
      // Скорость также выдаем равной нулю, хотя по сути КС будет разомкнут
      v->outSpeedWr = v->outTheta = 0.0f;
      // Формируем задание по вектору намагничения равным величине fixId
      v->outId = v->fixId;
      // Вектор активной компоненты задаем равным нулю, для устранения движения.
      v->outIq = 0.0f;
*/
    } else if (v->Enable){ // Если работа алгоритма разрешена - запускаем
      switch(v->Mode){ //
      case /*BEFORE_ACCELERATE_MODE*/ PMSM_ANTIROLLBACK_ID_FIX_TIME:
        // v->fixId до v->inId
        if (v->onStartWork /*&& fabsf(v->inSpeedWrz) == 0.0f*/ && v->fixIdDoneCnt) { // Если ШИМ включен, частота задания 0, и счетчик фиксации еще не ноль - даем рост возбуждения
          tempId = (v->fixId * v->timeKoef); //плавное нарастание/спад тока возбуждения
          // Если вычисленная величина больше входного значения, заменяем ее. Иначе оставляем входное значение.
          if (tempId > v->inId) {
            v->outId = tempId; // Заменяем
          }
          
          if(v->pwmOn) {
            //повышаем коэффициент плавного изменения тока от 0 до 1
            if(v->timeKoef < 1.0f) v->timeKoef += timeKStep;
            else {
              v->timeKoef = 1.0f;
              v->fixIdDone = 1;
              // Уменьшаем счетчик фиксации тока возбуждения
              v->fixIdDoneCnt --;
            }
          }
          else {
            //сброс коэффициента плавного изменения тока
            v->timeKoef = 0;
          }
        } else if (v->onStartWork && !v->fixIdDoneCnt) { // Если счетчик удержания упал до нуля, пора сбрасывать ток возбуждения
          //Уменьшаем коэффициент плавного изменения тока от 1 до 0
          if(v->timeKoef > timeKStepDown) v->timeKoef -= timeKStepDown;
          else { // Если дотикали до нуля, сбрасываем флаг разрешения работы
            v->timeKoef = 0.0f;
            v->onStartWork = 0;
          }
          
          tempId = (v->fixId * v->timeKoef); //плавное нарастание/спад тока возбуждения
          // Если вычисленная величина больше входного значения
          if (tempId > v->inId) {
            v->outId = tempId; // Заменяем
          }
        }
        break;
        
      case /*FREQ_TIME_MODE*/ PMSM_ANTIROLLBACK_ID_FIX_FREQ: // Режим по умолчанию - Возрастает при включении ШИМа, сброс по времени и от заданной частоты.
        // v->fixId до v->inId
        if (v->onStartWork && fabsf(v->inSpeedWrz) <= v->FrequencyDown) { // Если старт - даем рост возбуждения
          tempId = v->fixId * v->timeKoef; //плавное нарастание/спад тока возбуждения
          // Если вычисленная величина больше входного значениянн
          if (tempId > v->inId) {
            v->outId = tempId; // Заменяем
          }
          
          if(v->pwmOn) {
            //повышаем коэффициент плавного изменения тока от 0 до 1
            if(v->timeKoef < 1.0f) v->timeKoef += timeKStep;
            else v->timeKoef = 1.0f;
          }
          else {
            //сброс коэффициента плавного изменения тока
            v->timeKoef = 0;
          }
        } else if (v->onStartWork && fabsf(v->inSpeedWrz) > v->FrequencyDown) { // Если пошла езда,
          //Уменьшаем коэффициент плавного изменения тока от 1 до 0
          if(v->timeKoef > timeKStepDown) v->timeKoef -= timeKStepDown;
          else { // Если дотикали до нуля, сбрасываем флаг разрешения работы
            v->timeKoef = 0.0f;
            v->onStartWork = 0;
          }
          
          tempId = v->fixId * v->timeKoef; //плавное нарастание/спад тока возбуждения
          // Если вычисленная величина больше входного значения
          if (tempId > v->inId) {
            v->outId = tempId; // Заменяем
          }
        }
        break;
        
        case PMSM_ANTIROLLBACK_OFF:
        default:
          // по сути ничего не делаем. все данные уже перенесены выше          
        break;
      }
    }


    // Проверка токоограничения по выходному полному току    
    arm_sqrt_f32((v->outId * v->outId) + (v->outIq * v->outIq), &v->Imag);
    // Сравниваем выход полного тока, с уставкой МТЗ, и при необходимости зарезаем уровень реактивного тока
    if(v->Imag > v->maxIfull){  // Если модуль вектора тока больше максимально разрешенного значения, снижаем уровень реактивного тока      
      arm_sqrt_f32((v->maxIfull * v->maxIfull) - (v->outIq * v->outIq), &v->outId);        
      // Вторичный расчет для проверки что модуль в заданных пределах
      arm_sqrt_f32((v->outId * v->outId) + (v->outIq * v->outIq), &v->Imag);
    }
    

}

void pmsm_antirollback_init(PMSMANTIROLLBACK_handle v)
{

}

/*

void pmsm_antirollback_calc(PMSMANTIROLLBACK_handle v)
{
	_iq		Kf, tempId, timeKStep, timeKStepDown;
	// Перенаправление
    v->outId = v->inId;
    v->outIq = v->inIq;
    v->outSpeedWr = v->inSpeedWr;
    v->outTheta = v->inTheta;

    //расчёт шага коэффициента плавного изменения тока
    //ф-ция вызывается с частотой Fpwm
    //коэффициент 1000 - для перевода времени из мсек в сек
    timeKStep = _IQ(1000.0 / (v->Fpwm * v->timeUI_Up));
    timeKStepDown = _IQ(1000.0 / (v->Fpwm * GlobalM3.pmsmAntirollBack_timeStepDown));

    // Если ШИМ выключен - взводим флаг разрешения работы алгоритма в старте
    if(!v->pwmOn) {
    	v->onStartWork = 1;
    }

    switch (v->bitPhasing){
    	// Стадия фазировки энкодера
    	case 1:
    		// Фазу выдаем равной нулю, для отбивки по нулевой позиции
    		// Скорость также выдаем равной нулю, хотя по сути КС будет разомкнут
    		v->outSpeedWr = v->outTheta = _IQ(0.0);
    		// Формируем задание по вектору намагничения равным величине fixId
    		v->outId = v->fixId;
    		// Вектор активной компоненты задаем равным нулю, для устранения движения.
    		v->outIq = _IQ(0.0);
    	break;

    	// Режим антиотката при пуске и стопе, в зависимости от времени
    	case 0:
    	default:
    		// v->fixId до v->inId
    		if (v->onStartWork && _IQabs(v->inSpeedWrz) <= _IQ(GlobalM3.pmsmAntirollBackFreqDown)) { // Если старт - даем рост возбуждения
    		    tempId = _IQmpy(v->fixId, v->timeKoef); //плавное нарастание/спад тока возбуждения
    		    // Если вычисленная величина больше входного значениянн
    		    if (tempId > v->inId) {
   		    		v->outId = tempId; // Заменяем
    		    }

    		    if(v->pwmOn) {
					//повышаем коэффициент плавного изменения тока от 0 до 1
					if(v->timeKoef < _IQ(1)) v->timeKoef += timeKStep;
					else v->timeKoef = _IQ(1);
    		    }
    		    else {
    		    	//сброс коэффициента плавного изменения тока
					v->timeKoef = 0;
    		    }
    		} else if (v->onStartWork && _IQabs(v->inSpeedWrz) > _IQ(GlobalM3.pmsmAntirollBackFreqDown)) { // Если пошла езда,
				//Уменьшаем коэффициент плавного изменения тока от 1 до 0
				if(v->timeKoef > timeKStepDown) v->timeKoef -= timeKStepDown;
				else { // Если дотикали до нуля, сбрасываем флаг разрешения работы
					v->timeKoef = _IQ(0.0);
					v->onStartWork = 0;
				}

    		    tempId = _IQmpy(v->fixId, v->timeKoef); //плавное нарастание/спад тока возбуждения
    		    // Если вычисленная величина больше входного значения
    		    if (tempId > v->inId) {
   		    		v->outId = tempId; // Заменяем
    		    }
    		}
    	break;

//    	// Режим антиотката при пуске, от частоты
//    	case 0:
//    	default:
//    		// от 0 до 2 Гц по линейному закону снижаем возбуждение от величины
//    		// v->fixId до v->inId
//    		if (_IQabs(v->inSpeedWrz) < _IQ_2_HZ && v->onStartWork) { // Если частота ниже 2Гц и разрешена работа алгоритма в старте - поднимаем возбуждение
//    		    Kf = _IQ(1) - _IQdiv(_IQabs(v->inSpeedWrz), _IQ_2_HZ); // Коэф ослабления поля возбуждения, при использовании Fсреза 2Гц упрощаем расчет
//    		    tempId = _IQmpy(v->fixId, Kf);
//    		    tempId = _IQmpy(tempId, v->timeKoef); //плавное нарастание/спад тока возбуждения
//    		    // Если вычисленная величина больше входного значения
//    		    if (tempId > v->inId) {
//   		    		v->outId = tempId; // Заменяем
//    		    }
//
//    		    if(v->pwmOn) {
//					//повышаем коэффициент плавного изменения тока от 0 до 1
//					if(v->timeKoef < _IQ(1)) v->timeKoef += timeKStep;
//					else v->timeKoef = _IQ(1);
//    		    }
//    		    else {
//    		    	//сброс коэффициента плавного изменения тока
//					v->timeKoef = 0;
//    		    }
//    		} else {
//    			v->onStartWork = 0;
//    		}
//    	break;

    }
}

 */

