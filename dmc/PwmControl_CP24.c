#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "PwmControl.h"
#include "DrvPWM.h"
#include "DrvADC.h"
#include "CurVoltMeas.h"
#include "DrvCompZPT.h"
#include "measTime.h"
#include "GlobalVar.h"

extern PWMGEN Pwm;                 // Обект драйвера ШИМ 
extern ADCDRV Adc;                 // Обект драйвера АЦП
extern CURVOLTMEAS Meas;           // Обект измеритетельной системы
extern COMPZPTDRV CompZPT;

void ReInitMTZcomp(void);          // Перенастроить МТЗ компаратора
u8 CheckAdcVal(void);              // Проверить значение АЦП по токовым каналам

// Запрос на включение ШИМ
void RequestOn(PWMCONTROL *p){
  p->FlagStart = 1;
  p->FlagStop = 0;
}

// Запрос на выключение ШИМ
void RequestOff(PWMCONTROL *p){
  p->FlagStop = 1;
  p->FlagStart = 0;
}

// Аппарат состояний модуля ШИМ
void StateMachine(PWMCONTROL *p){
 
  switch(p->StatusPwm){
     case PwmStop:{                 // Шим отключен
       if(p->FlagStart){            // ждем команды пуск 
         p->FlagStart = 0;
         p->StatusPwm = PwmInitFeedTopKey;  // Переходим в состояние настройки модуля ШИМ к запитке драйвера верхних ключей
       }
       
       Adc.CompStop(&Adc);          // Отключить компаратор МТЗ
       
       p->FlagStop = 0;             // На всякий случай
       p->Timer = 0;                // Обнуляем счетчик времени              
     } break;                 
     case PwmInitFeedTopKey:{          // Настройка модуля Шим для запитки драйвера верхних ключей
       Pwm.InitPumping(&Pwm);          // Настроить Настроить драйвер ШИМ для накачки

       CompZPT.ModeComp = CompWaitErr; // Настроить прерывание от компаратора в Ud (для защиты от КЗ в нагрузке)
       CompZPT.ReInit(&CompZPT);
       
       p->StatusPwm = PwmFeedTopKey;   // Переходим в накачку питания для верхних ключей    
     } break;    
     case PwmFeedTopKey:{              // Запитка драйвера верхних ключей и датчиков тока
       // Ожидаем вменяемое значения от измерительной системы
       if(CheckAdcVal() == 1){
          p->Timer = 0;                      // АЦП запустилось!!! 
          p->StatusPwm = PwmCalibrOffset;    // Переходим в состояние калибровки оффсет       
          Meas.InitCalibrOffset(&Meas);
          return;
       }
       
       // Проверим таймаут 
       p->Timer++;
       if(p->Timer > TIMOUT_PUMPING){                     // Накачка не происходит за указанное время - авария!!!
          p->StatusPwm = PwmErr;                          // Переходим в состояние Error
          GlobalM4.FailData.FailsFlag.bit.TimOutPump = 1; // Установить флаг аварии (обработаем в FailControl)         
          return;
       }
       
       // Проверим статус компаратора
       if(CompZPT.ModeComp == CompErr){
          p->StatusPwm = PwmErr;        // Переходим в состояние Error         
          return;
       }

       Pwm.Pumping(&Pwm);              // Тут дергаем нижними ключами (Накачка)       
        
     } break; 
     case PwmCalibrOffset:{            // Калибровка каналов измерения (определение Оффсет)
       if(p->FlagStop){                // Проверим может пользователь дал команду СТОП 
          p->FlagStop = 0;
          p->StatusPwm = PwmStop;      // Переходим в состояние СТОП
         
          CompZPT.ModeComp = CompCountFront; // Настроить прерывание от компаратора на счет импульсов
          CompZPT.ReInit(&CompZPT);                    
          return;
       }
              
       p->Timer++;                         // Измеряем время калибровки оффсет
       if(p->Timer > TIME_OFFSET_CALIBR){  // Проверим длительность калибровки оффсет
          p->StatusPwm = PwmInitCompMtz;   // Переходим в состояние настройки компаратора МТЗ          
       }
       Meas.CalibrOffset(&Meas);     // Калибровка каналов измерения тока  
       // Проверим статус компаратора
       if(CompZPT.ModeComp == CompErr){
          p->StatusPwm = PwmErr;           // Переходим в состояние Error         
          return;
       }
       
       Pwm.Pumping(&Pwm);           // Тут дергаем нижними ключами (Накачка)
     } break;
     case PwmInitCompMtz:{          // Настройка компаратора перед пуском
       if(CheckAdcVal() == 0){
         p->StatusPwm = PwmErr;     // Нет смысла запускать ШИМ так как каналы измерения не настроены         
         return;
       }
       
       CompZPT.ModeComp = CompCountFront; // Настроить прерывание от компаратора на счет импульсов
       CompZPT.ReInit(&CompZPT);
       
       Pwm.DeInitPumping(&Pwm);     // Отключить режим накачки 
                                    // Настроить вывод для опредиления частоты следования импульсов от компаратора в цепи Ud
       ReInitMTZcomp();             // Переинициализация настроек компаратора МТЗ 
       Adc.CompStart(&Adc);         // Запустить компаратор
       
       p->StatusPwm = prePwmStart;     // Переходим в состояние запуск ШИМ 
       
       Pwm.On(&Pwm); // Делаем запуск ШИМа до начала вторичной калибровки датчиков тока
       p->Timer = 0;
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
       Meas.CalibrOffset(&Meas);     // Калибровка каналов измерения тока                           
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
       Pwm.DeInitPumping(&Pwm);     // Отключить режим накачки
       if(p->FlagStop){             // Ждем от командо аппарата ПЧ команду на останов 
          p->FlagStop = 0;
          p->StatusPwm = PwmStop;   // Переходим в состояние СТОП
         
          CompZPT.ModeComp = CompCountFront; // Настроить прерывание от компаратора на счет импульсов
          CompZPT.ReInit(&CompZPT);
       }
     } break;
  }
}

// Перенастроить МТЗ компаратора
void ReInitMTZcomp(void){
  s16 CompMax, CompMin;
  float Gain, Offset;
  
  // 1) Получить среднее на три канала значение Gain и Ofset
  Gain = ((GlobalM4.ValIu.Gain + GlobalM4.ValIv.Gain + GlobalM4.ValIw.Gain)/3.0f) * 16.0f;                  // *16 - это перевод Gain c 12 бит в 8 бит 
  Offset = ((float)((GlobalM4.ValIu.OffSet + GlobalM4.ValIv.OffSet + GlobalM4.ValIw.OffSet) >> 4))/3.0f;    // >> 4 - Это перевод OffSet с 12 бит в 8 бит
  
  // 2) Расчитать значения для компаратора
  CompMax = (s16)(Offset + (GlobalM4.FailsSetup.Imtz / (Gain * 2.0f)));
  CompMin = (s16)(Offset - (GlobalM4.FailsSetup.Imtz / (Gain * 2.0f)));
      
  // 4) Применить полученные настройки в компаратор ADC
  Adc.CompMax = CompMax;
  Adc.CompMin = CompMin;
  Adc.CompInit(&Adc);
}

// Проверить значение АЦП по токовым каналам
u8 CheckAdcVal(void){
  if((ADC_MIN < GlobalM4.ValIu.AdcVal)&&(GlobalM4.ValIu.AdcVal < ADC_MAM)&&(ADC_MIN < GlobalM4.ValIv.AdcVal)&&
     (GlobalM4.ValIv.AdcVal < ADC_MAM)&&(ADC_MIN < GlobalM4.ValIw.AdcVal)&&(GlobalM4.ValIw.AdcVal < ADC_MAM)){
     
     return 1;    // Ok!
  }
  else{
     return 0;    // Err
  }
}
    
#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)