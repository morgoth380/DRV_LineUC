/*=====================================================================================
 File name:        PID_REG3.C  (IQ version)                  
                    
 Originator: Triol

 Description:  The PI current controller               

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 1.00
-------------------------------------------------------------------------------------*/
#include "Global_Include.h"
#include "pi_reg_spd.h"
#include "Pjk_const.h"

#define  MAX_SPD_FIX_CNT  60

void pi_reg_spd_calc(register PIREGSPD *v)
{  
    // Штатный фильтр задания скорости для симметричного оптимума
    v->Ref_filtr = (v->K1 * v->Ref_filtr) + (v->K2 * v->Ref);

    // Compute the error
    v->Err = v->K3 * (v->Ref_filtr - v->Fdb);
    
    // Compute the proportional output
    v->Up =  v->Kp * v->Err;
    
    // Compute the integral output
    if (!v->FreezeUi) { // Если фдаг замрозки сброшен считаем интегральную часть
        v->Ui += v->Ki * v->Err;         
    }
    
    // Check restore Ui flag - pretorque mode
    if (v->RestorePrevUiFlag) {
      // We need restore last Ui value        
      v->Ui = v->Ui_prev * v->RestoreTime/v->RestorePrevUiTime; 
      v->Up = 0; // Если идет восстановление интеграла, пропорционал обнуляем
      
      if (v->RestoreTime < v->RestorePrevUiTime) 
        v->RestoreTime+= v->Tpwm;      
    }
    
    // Check OutMax/OutMin for drive maximal current
    // Imax может менять интерактивно по необходимости, при этом значение момента
    // ограничения не меняем. Это дополнительный внешний контур ограничения тока
    if (v->OutMax > v->Imax) {
      v->OutMax = v->Imax;
    }
    if (v->OutMin < -v->Imax) {
      v->OutMin = -v->Imax;
    }
        
    // Saturate the integral output
    if (v->Ui > v->OutMax)
        v->Ui =  v->OutMax;
    else if (v->Ui < v->OutMin)
        v->Ui =  v->OutMin;

    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui;

    // Saturate the output
    if (v->OutPreSat > v->OutMax)
        v->Out =  v->OutMax;
    else if (v->OutPreSat < v->OutMin)
        v->Out =  v->OutMin;
    else
        v->Out = v->OutPreSat;
    // -----------------------------------------------    
    // Проверяем посадку на ограничение регулятора скорости
    // Для работы через генератор темпа, и для выдачи сигнала входа в ограничение.
    if( (v->Out >= v->OutMax) || (v->Out <= v->OutMin) ){
    	v->SpdOutMaxFixCnt++;                         // Если сижу в ограничении - начинает тикать счетчик
    	if(v->SpdOutMaxFixCnt > MAX_SPD_FIX_CNT){     // Проверяем счетчик, если досчитали
    	     v->SpdOutMaxFix = 1;                     // Ставим флаг, что мы в жестком ограничении
    	     v->SpdOutMaxFixCnt = MAX_SPD_FIX_CNT;    // Фиксим значение
    	}
    } else {                                          // если не в ограничении - уменьшаем счетчик в два раза быстрее
    	if(v->SpdOutMaxFixCnt > 5){
            v->SpdOutMaxFixCnt -= 2; 
        }
    	else{
          v->SpdOutMaxFix = 0; 
        }
    }
    
    // Фиксация последнего значения интеграла, по внешнему маркеру
    if (v->SavePrevUiFlag) {
      v->Ui_prev = v->Ui;
    }
}

void pi_reg_spd_reset(PIREGSPD *v)
{
    v->Err = 0;
    v->ErrT = 0; 
    v->ErrP = 0; 
    v->Up = 0;
    v->Ui = 0;
    v->Ref_filtr = 0;
    v->OutPreSat = 0;
    v->Out =  0;
    v->UiOutMax = 0;
    v->UiOutMin = 0;
    v->OutMin = 0;
    v->OutMax = 0;
    v->RestoreTime = 0;
    
    if (v->ResetUiPreviuos) { // Если есть запрос на обнуление предыдущего значения интеграла
      v->Ui_prev = 0;
    }
}


