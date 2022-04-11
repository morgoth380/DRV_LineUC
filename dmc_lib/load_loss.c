/*=====================================================================================
 File name:    LOAD_LOSS.C 
                    
 Originator:     Triol / Sosnovchik

 Description:  Load loss module

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 06-01-2022 Version 1.00      
-------------------------------------------------------------------------------------*/

#include "Global_Include.h"
#include "type.h"
#include "load_loss.h"
#include "Pjk_const.h"

LOADLOSSmodule	loadloss1 = LOADLOSS_DEFAULTS;
#define		MINIMAL_FREQ_LOAD_LOSS	0.2F
#define         MAX_FAULT_CNT           4
//========================================================================
void LoadLoss_calc(LOADLOSSmodule *v)
{   
  
    u16         TimeLoadLossCheck; // Предел счета
    f32         absFout;
    
    absFout = fabsf(v->Fout);
    
    // Частота задается в Гц
    if (absFout < MINIMAL_FREQ_LOAD_LOSS) { // Если выходная частота менее 5 Гц, то надо установить значение для расчета 5 Гц
        TimeLoadLossCheck = (u16)(((1 / MINIMAL_FREQ_LOAD_LOSS) / 12) / v->Tpwm); // Расчет максимального значения счета для текущей частоты и периода ШИМа. Разбиения периода на 12 секторов
    } else {
        TimeLoadLossCheck = (u16)(((1 / absFout) / 12) / v->Tpwm); // Расчет максимального значения счета для текущей частоты и периода ШИМа
    }
    
    if (!v->PwmOn || absFout < MINIMAL_FREQ_LOAD_LOSS) { // Если ШИМ выключен, запоминаем последний угол для начала работы
      v->prevTheta = v->currentThetaI; // Текущий угол тока вычисленный по Альфа/Бета
      v->timer = v->cntFault = v->Fault = 0;
      v->estimateThetaTmp = 0;
    } else { // Если ШИМ включен - работаем
      v->timer ++; // Увеличиваем счетчик
      
      // Calculate to temporary variable
      v->estimateThetaTmp = v->estimateThetaTmp + _2PI * v->Fout * v->Tpwm; // Делаем расчет ожидаемого приращения фазы
      // Проверяем значение счетчика
      if (v->timer >= TimeLoadLossCheck) { // Достигли верха счета, проверяем фазу
        v->timer = 0; // Сбросить счетчика
        v->dTheta = v->currentThetaI - v->prevTheta; // Приращение фазы
        v->prevTheta = v->currentThetaI; // Перезапомнить фазу
        v->estimateTheta = v->estimateThetaTmp;
        
        if (v->dTheta < -_PI) {
          v->dTheta = v->dTheta + _2PI;
        } 
        if (v->dTheta > _PI ) {
          v->dTheta = v->dTheta - _2PI;
        } 
        
        // Сравниваем приращение фазы с ожидаемым приращением. Если фактическое меньше в два раза от ожидаемого, увеличиваем счетчик аварии
        if (fabsf(v->dTheta) < fabsf(v->estimateTheta / 4) && v->estimateTheta != 0){ // deg2rad(360/12-10) 
          v->cntFault = v->cntFault + 1;
        } else {
          if (v->cntFault) {
            v->cntFault = v->cntFault - 1;
          }
        }
        
        v->estimateThetaTmp = 0; // Reset estimate theta
        
        // Если счетчик аварии больше верхнего значения
        if (v->cntFault >= MAX_FAULT_CNT) {
          v->Fault = 1;        // Ставим маркер аварии
          v->cntFault = MAX_FAULT_CNT;
        }
      }
          
    }           
}
//========================================================================
void LoadLoss_init(LOADLOSSmodule *v)
{   

}
//========================================================================
