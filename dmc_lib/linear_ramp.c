/*=====================================================================================
 File name:        LINEAR_RAMP.C  (float version)                  
                    
 Originator: Triol

 Description:  The linear ramp generator

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 05-04-2021 Version 1.00
-------------------------------------------------------------------------------------*/

#include "linear_ramp.h"

void linear_ramp_calc(register LINEARRAMP *v)
{
    // Линейный ЗИ входного сигнала, пока для положительных значений
  if (v->StartFlag && !v->WorkFlag) {
      // Запуск ЗИ
      v->WorkFlag = 1; // Начали работу
      v->Out = v->StartValue; // Начальный старт.
      return; // Первый такт ничего не делаем.
  }

  if (v->WorkFlag && !v->WorkDone) {
    v->RampStep = (v->EndValue - v->StartValue) / (v->Fpwm * v->RampTime); // Находим шаг приращения - на каждой итерации. ТАК НАДО!!!
    v->Out = v->Out + v->RampStep;

    if (v->Out > v->EndValue) {
      v->Out = v->EndValue;
      v->WorkDone = 1;
    }
  } else if (v->WorkDone){
    v->Out = v->EndValue; // По окончании выдаем конечное значение.
  } else {
    v->Out = v->EndValue; // Если не активен - тоже выдаем конечную точку
  }
}

void linear_ramp_reset(LINEARRAMP *v)
{
  v->WorkDone = v->WorkFlag = v->StartFlag = 0;
}

