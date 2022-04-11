#include "Global_Include.h"
#include "GlobalVar.h"
#include "Pjk_const.h"
#include "ScalarControl.h"
#include "SvGen_dq.h"
#include "DrvPWM.h"

#define K_MOD_SPACEPWM  1.4142135623730950488016887242097f

SVGENDQ SvGenDq = SVGENDQ_DEFAULTS;
extern PWMGEN Pwm;

void ScalarControlCalc(SCALARCONTROL_handle v){
  
    // 1) Определим коеффициент модуляции
    v->Ds = (v->Uscl * K_MOD_SPACEPWM) / v->Udc;
    
    if(v->Ds > 1.0f)  v->Ds = 1.0f;                   // Ограничение коеффициента модуляции

    // 2) Определим фактическое выходное напряжение
    v->Uout = (v->Udc * v->Ds) / K_MOD_SPACEPWM;

    // 3) Прирощение угла поля статора
    v->Angle = v->Angle + (_2PI * v->StepAngle * v->Fscl);                         // _2PI * -так как нужно перевести угол в радианы

    if(v->Angle > _2PI){
        v->Angle -= _2PI;
        v->Period = 1;                 // Флаг завершения периода выходного напряжения
    }
    else if(v->Angle < -_2PI){
        v->Angle += _2PI;
        v->Period = 1;                 // Флаг завершения периода выходного напряжения
    }

    // 4) Синус и Косинус угла поля статора
    v->sinT = arm_sin_f32(v->Angle);
    v->cosT = arm_cos_f32(v->Angle);
        
    // 5) Расчитать задание для ШИМ
    SvGenDq.Ualpha = v->Ds * v->cosT;
    SvGenDq.Ubeta  = v->Ds * v->sinT;
    //SvGenDq.calc(&SvGenDq);
    
  
    
    switch (v->SvgenMode) {
        case 0: SvGenDq.calc(&SvGenDq); break;
        case 1: SvGenDq.dpwm60(&SvGenDq); break;
        case 2:
          if (v->Fscl > 10.0) {
            SvGenDq.dpwm60(&SvGenDq);
          } else {
            SvGenDq.calc(&SvGenDq);
          }  
          break;
        default: SvGenDq.calc(&SvGenDq); break;
    }
      
    Pwm.Ta = SvGenDq.Ta;
    Pwm.Tb = SvGenDq.Tb;
    Pwm.Tc = SvGenDq.Tc;
    Pwm.Update(&Pwm);
}


