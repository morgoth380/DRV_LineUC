#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "PwmControl.h"
#include "DrvPWM.h"
#include "CurVoltMeas.h"
#include "GlobalVar.h"

extern PWMGEN Pwm;                 // Объкт драйвера ШИМ 
extern CURVOLTMEAS Meas;           // Объект измеритетельной системы

// Запрос на включение ШИМ
// Переопределение для совместимости с измерением параметров двигателя
void RequestOn(PWMCONTROL *p){
//  p->FlagStart = 1;
//  p->FlagStop = 0;
//  Pwm.On(&Pwm);
//  p->StatusPwm = PwmWork;
  
  p->FlagStart = 1;
  p->FlagStop = 0;  
}

// Запрос на выключение ШИМ
// Переопределение для совместимости с измерением параметров двигателя
void RequestOff(PWMCONTROL *p){
//  p->FlagStop = 1;
//  p->FlagStart = 0;
//  Pwm.Off(&Pwm);
//  p->StatusPwm = PwmStop;
  
  p->FlagStop = 1;
  p->FlagStart = 0;  
}

// Аппарат состояний модуля ШИМ
void StateMachine(PWMCONTROL *p){
  switch(p->StatusPwm){
     case PwmStop:{                 // Шим отключен
       if(p->FlagStart){            // ждем команды пуск 
         p->FlagStart = 0;
         p->StatusPwm = PwmCalibrOffset;  
       }       
       p->FlagStop = 0;             // На всякий случай
       p->Timer = 0;                // Обнуляем счетчик времени       
       //GlobalM4.AdcKcor = 1;       
       //GlobalM4.AdcKcorValPwmOff = GlobalM4.LowSpeedAinAdc.Vbat;
     } break;                 
     
     case PwmCalibrOffset:{            // Калибровка каналов измерения (определение Оффсет)
       p->StatusPwm = prePwmStart;     // Переходим в состояние запуск ШИМ        
       Pwm.On(&Pwm); // Делаем запуск ШИМа до начала вторичной калибровки датчиков тока
       p->Timer = 0;
       //GlobalM4.AdcKcor = 0.95; // Заглушечный коэффициент до получения нормальной корректировки
     } break;
     
     case prePwmStart:{                // Тут включаем ШИМ
       // !!! ШИМ уже включен тактом выше
       p->Timer++;                         // Измеряем время калибровки оффсет
       if(p->Timer > TIME_OFFSET_CALIBR){  // Проверим длительность калибровки оффсет
          p->StatusPwm = PwmStart;         // Переходим в состояние финального запуска ШИМ       
       }
       Pwm.Ta = 0;
       Pwm.Tb = 0;
       Pwm.Tc = 0;
       Pwm.Update(&Pwm);
       Meas.CalibrOffset(&Meas);     // Калибровка каналов измирения тока   
       //GlobalM4.AdcKcorValPwmOn = GlobalM4.LowSpeedAinAdc.Vbat; 
       //GlobalM4.AdcKcor = (f32)GlobalM4.AdcKcorValPwmOff / (f32)GlobalM4.AdcKcorValPwmOn;
     } break;
       
     case PwmStart:{                // Тут включаем ШИМ
       Pwm.On(&Pwm);                // Включить ШИМ!!!         
       p->StatusPwm = PwmWork;      // Переходим в состояние работа
            
     } break;
     case PwmWork:{                 // Штатная работа модуля ШИМ (ШИМ включен!!!) 
       if(p->FlagStop){             // Проверим может пользователь дал команду СТОП 
          p->FlagStop = 0;
          p->StatusPwm = PwmStop;   // Переходим в состояние СТОП
          Pwm.Off(&Pwm);            // Отключить ШИМ!!! 
       } 
     } break;
     
     case PwmErr:{                  // Авария модуля ШИМ (не смогли запитать датчики тока)
       if(p->FlagStop){             // Ждем от командо аппарата ПЧ команду на останов 
          p->FlagStop = 0;
          p->StatusPwm = PwmStop;   // Переходим в состояние СТОП
       }
     } break;
  }
}
    
#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)