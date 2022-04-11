#include "TormSW.h"
#include "GlobalVar.h"
#include "Filter.h"

// Настроить тормозной ключ
void SwInit(TORMSW_handle s){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // 1) Включить тактирование порта
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // 2) Закрыть тормозной ключ
  HAL_GPIO_WritePin(TORM_SW_MK_GPIO_Port, TORM_SW_MK_Pin, GPIO_PIN_RESET);
  s->State = TormSwOff;
  
  // 3) Настроить порт
  GPIO_InitStruct.Pin = TORM_SW_MK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(TORM_SW_MK_GPIO_Port, &GPIO_InitStruct);
}

// Управление ключом
void SwControl(TORMSW_handle s){
  
  if (s->TormCheck) { 
    // Проверка тормозного резистора
    s->Udslow = f32_Filtr(s->Udslow, s->UdFast, 0.05f);   // Подфильтруем немного Ud  
    
    if ( (s->TormCheck == 1) && (s->TestResState == None) && (s->TestResResult == Stopped) ) { // Если приходит флаг начала проверки тормозного резистора и тест еще не запущен
      s->TestResState = InitSendSequence; // Запуск тестирования
    } else if (s->TestResState != None) { // Если запуск произошел - тестируем резистор      
      s->TestResResult = s->CheckTorm(s); // Подпрограмма возвращает результат проверки
      // s->TestResResult - По факту это надо проверять в FailControl и формировать аварию если она есть.
    }    
    
  } else { // Если проверка тормозного резистора не активна, работаем по штатному алгоритму работы
    // Начальная инициализация блока тестирования тормозного резистора
    s->TestResState = None; 
    s->TestResResult = Stopped;
    
    if (s->PwmOn) { // Если ШИМ включен - контролируем тормозной ключ
        if((s->UdFast <= s->UcloseSw)||(s->EnSw == 0)){                               // Ud в норме ключ запираем
          HAL_GPIO_WritePin(TORM_SW_MK_GPIO_Port, TORM_SW_MK_Pin, GPIO_PIN_RESET);
          s->State = TormSwOff;
        }
        else if(s->UdFast >= s->UopenSw){                                             // Открыть тормозной ключ  
          HAL_GPIO_WritePin(TORM_SW_MK_GPIO_Port, TORM_SW_MK_Pin, GPIO_PIN_SET);
          s->State = TormSwOn;
        }  
    } else { // Если ШИМ выключен, отключаем тормозной ключ
      HAL_GPIO_WritePin(TORM_SW_MK_GPIO_Port, TORM_SW_MK_Pin, GPIO_PIN_RESET);
      s->State = TormSwOff;      
    }
    
  } 
}

// Выбор кода для отработки наличия тормозного резистора
//#define  BARKER_CODE_TEST_TORMOZ
#define  GREBENKA_CODE_TEST_TORMOZ
//=======================================================================================
// Излучаемая последовательность с дополнительными полями слева и справа равными -1.
// Дополнения битами -1 необходимы, чтобы гарантировано закрыть тормозной ключ после теста
const s16 Barker_7M[11]= {-1,-1,1,1,1,-1,-1,1,-1,-1,-1}; // Слева и справа добавлены по два доп.бита
// Оригинальная Последовательность для анализа
const s16 Barker_7[7]= {1,1,1,-1,-1,1,-1};
//=======================================================================================
// Излучаемая последовательность с дополнительными полями слева и справа, равными -1.
// Дополнения битами -1 необходимы, чтобы гарантировано закрыть тормозной ключ после теста
const s16 Grebenka_7M[9]= {-1,1,-1,1,-1,1,-1,1,-1}; // Слева и справа добавлены по два доп.бита
// Оригинальная Последовательность для анализа
const s16 Grebenka_7[7]= {1,-1,1,-1,1,-1,1};
//=======================================================================================
// Макросы для управления тормозным ключом (Управление активным нулем)
// Включение бита "-1" - при этом надо сперва выключить тормозной ключ, а потом убрать разрешение с буфера ключей
#define		SET_TORM_M_1()    	HAL_GPIO_WritePin(TORM_SW_MK_GPIO_Port, TORM_SW_MK_Pin, GPIO_PIN_RESET)
//=======================================================================================
// Включение бита " 1 " - при этом надо сперва поставить разрешение на буфер ключей, а потом включить тормозной ключ нулем/
#define		SET_TORM_1()   	  	HAL_GPIO_WritePin(TORM_SW_MK_GPIO_Port, TORM_SW_MK_Pin, GPIO_PIN_SET)
//=======================================================================================
// Проверка наличия тормозного резистора
CheckTormResStatus_Enum CheckTorm(TORMSW_handle v)
{
    #define		MeanSize	32  // Количество отсчетов при усреднении

    u16			cnt;
    s16 		SampleBitArray[sizeof(Barker_7)/sizeof(s16)]; // Массив отсчетов в кодах -1/+1 длиной как реальная последовательность Баркера
    f32 		Ym, Yp, Yo, dY; // Расчетные значения для анализа
    static f32	        UdcFast_Mean; // переменная вычисления среднего значения отсчета

#ifdef BARKER_CODE_TEST_TORMOZ
    switch (v->TestResState) {
    // Расчет по Баркеру
    case None:
        return (Stopped);
    
    case InitSendSequence:
      // В соответствии с частотой ШИМа вычисляем сколько тиков уходит на один бит сигнала
      v->BitTime_Sec = 0.05;
      v->BitTimeTick = ( (f32)(v->Fpwm) * v->BitTime_Sec); // Длительность битовой посылки в тиках ШИМа
      v->SampleTick = v->BitTimeTick * 0.5f; // Точка семплирования - Отсчеты делаем на 1/2 длительности бита
      v->cntTick = v->BitTimeTick; // счетчик длительности одного бита
      v->sampleBit = 0; // Указатель на отсчеты
      v->TestResState ++;
      UdcFast_Mean = 0;
      break;
  
    case SendSequence:
      // Устанавливаем ключ в состояние текущего бита последовательности
      if ( Barker_7M[v->sampleBit] == 1 ) {
        SET_TORM_1();
      }
      else {
        SET_TORM_M_1();
      }
      // Оценка времени семплирования данных
      if ( (v->cntTick >= (v->SampleTick-MeanSize/2.0f)) && (v->cntTick < (v->SampleTick + MeanSize/2.0f)) ) {
        // Суммируем отсчеты для усреднения
        UdcFast_Mean += v->Udslow;
      }
      else if ( !(v->cntTick) ) {
        // Если счет завершен, переход на новый бит, с запоминанием среднего значения отсчета
        v->SampleArray[v->sampleBit] = UdcFast_Mean / MeanSize; // Находим среднее значение отсчета и запоминаем его
        v->cntTick = v->BitTimeTick; // счетчик длительности одного бита
        v->sampleBit++;
        UdcFast_Mean = 0; // Не забыть сбросить переменную средней величины
        
        if (v->sampleBit == sizeof(Barker_7M)/sizeof(s16)) {
          // Если это последний бит в последовательности
          v->TestResState ++; // Переход на стадию анализа
        }
      }
      
      v->cntTick --;
      break; // Сразу переходим на этап оценки последовательности
      
      case TestSequence:
        // Конец операции проверки
        v->TestResState = None;
        
        // Находим среднее значение напряжения Ud для битов -1
        cntBit = 0; Ym = 0;
        for(cnt = 0; cnt < sizeof(Barker_7M)/sizeof(s16); cnt++)
        {
          if ( Barker_7M[cnt] == -1 ) {
            Ym += v->SampleArray[cnt];
            cntBit ++;
          }
        }
        Ym = Ym / cntBit;
        
        // Находим среднее значение напряжения Ud для битов 1
        cntBit = 0; Yp = 0;
        for(cnt = 0; cnt < sizeof(Barker_7M)/sizeof(s16); cnt++)
        {
          if ( Barker_7M[cnt] == 1 ) {
            Yp += v->SampleArray[cnt];
            cntBit ++;
          }
        }
        Yp = Yp / cntBit;
        
        // Находим пороговый уровень сравнения, как среднее значение
        Yo = (Ym + Yp ) * 0.5f;
        
        // Определяем битовую принадлежность для исходной последовательности.
        for (cnt=2,cntBit=0; cntBit < sizeof(SampleBitArray)/sizeof(s16); cnt++, cntBit++) {
          if (v->SampleArray[cnt] >= Yo) { // Читаем массив отсчетов Ud
            SampleBitArray[cntBit] = -1; // Заполняем отсчеты в -1/+1
          } else {
            SampleBitArray[cntBit] = 1;
          }
        }
        
        // Вычисляем корреляционную функцию
        v->CorelationResult = 0;
        for (cnt=0; cnt < sizeof(SampleBitArray)/sizeof(s16); cnt++) {
          v->CorelationResult += SampleBitArray[cnt] * Barker_7[cnt];
        }
        
        // Тест на процентный уход уровней от средней точки
        dY = (Ym-Yo)/Yo * 100.0f; // (Ym-Yo)/Yo = десятые доли процентов
        if ( dY < 0.2f ) // Если уход меньше 0,2 % - то похоже резистора вообще нету, а это шумит Ud
          v->CorelationResult = 0;
        
        //GlobalC28.tmpCorelRes = v->CorelationResult;
        if ( v->CorelationResult > (sizeof(SampleBitArray)/sizeof(s16) - 2) )
          v->TestResResult = Torm_OK; // Запоминаем результат операции, для передачи в блок ошибок
        else
          v->TestResResult = Breakdown; // Иначе - резистора нету и это не наш код
        
        return (v->TestResResult);
  } // Расчет по Баркеру
#endif
  
#ifdef GREBENKA_CODE_TEST_TORMOZ
    
  switch (v->TestResState) {    
  // Расчет по Гребенке
  case None:
    return (Stopped);
    
  case InitSendSequence:
    // В соответствии с частотой ШИМа вычисляем сколько тиков уходит на один бит сигнала
    v->BitTime_Sec = 0.05;    
    v->BitTimeTick = (u16)( (v->Fpwm) * v->BitTime_Sec); // Длительность битовой посылки в тиках ШИМа
    v->SampleTick = (u16)(v->BitTimeTick * 0.5f); // Точка семплирования - Отсчеты делаем на 1/2 длительности бита
    v->cntTick = v->BitTimeTick; // счетчик длительности одного бита
    v->sampleBit = 0; // Указатель на отсчеты
    v->TestResState ++;
    UdcFast_Mean = 0;
    //GPIO_MODE_SELECT(); // Выбираем режим управления через GPIO
    break;
    
  case SendSequence:
    // Устанавливаем ключ в состояние текущего бита последовательности
    if ( Grebenka_7M[v->sampleBit] == 1 ) {
      SET_TORM_1();
      v->State = TormSwOn;
    }
    else {
      SET_TORM_M_1();
      v->State = TormSwOff;      
    }
    // Оценка времени семплирования данных
    if ( (v->cntTick >= (v->SampleTick-MeanSize/2)) && (v->cntTick < (v->SampleTick + MeanSize/2)) ) {
      // Суммируем отсчеты для усреднения
      UdcFast_Mean += v->Udslow;
    }
    else if ( !(v->cntTick) ) {
      // Если счет завершен, переход на новый бит, с запоминанием среднего значения отсчета
      v->SampleArray[v->sampleBit] = UdcFast_Mean/MeanSize; // Находим среднее значение отсчета и запоминаем его
      v->cntTick = v->BitTimeTick; // счетчик длительности одного бита
      v->sampleBit++;
      UdcFast_Mean = 0; // Не забыть сбросить переменную средней величины
      
      if (v->sampleBit == sizeof(Grebenka_7M)/sizeof(s16)) {
        // Если это последний бит в последовательности
        v->TestResState ++; // Переход на стадию анализа
      }
    }    
    v->cntTick --;
    break; // Сразу переходим на этап оценки последовательности
    
  case TestSequence:
    // Конец операции проверки
    v->TestResState = None;
    // Начальное заполнение буферами значениями -1
    SampleBitArray[0] = -1;
    SampleBitArray[1] = -1;
    SampleBitArray[2] = -1;
    SampleBitArray[3] = -1;
    SampleBitArray[4] = -1;
    SampleBitArray[5] = -1;
    SampleBitArray[6] = -1;
    
    // Задаем цикл проверки принадлежности битов
    for(cnt = 0; cnt < (u16)((sizeof(Grebenka_7M)/sizeof(s16))/2); cnt++)
    {
      Ym = 0.5f*(v->SampleArray[cnt*2] + v->SampleArray[cnt*2+2]); // Находим ноль для данного бита
      Yp = v->SampleArray[cnt*2+1]; // Запоминаем значение 1
      // Находим пороговый уровень сравнения, как среднее значение
      Yo = (Ym + Yp) * 0.5f;
      // Проверка на шум
      dY = (Ym-Yo)/ Yo * 100.0f; // (Ym-Yo)/Yo * 100 = десятые доли процентов
      if ( dY > 0.2f ){ // Если уход меньше 0,2 % - то похоже резистора вообще нету, а это шумит Ud
        if (Yp >= Yo) {
          SampleBitArray[cnt*2] = -1; // Заполняем отсчеты в -1/+1
        }
        else {
          SampleBitArray[cnt*2] = 1;
        }
      }      
    }
    
    // Вычисляем корреляционную функцию
    v->CorelationResult = 0;
    for (cnt=0; cnt < sizeof(SampleBitArray)/sizeof(s16); cnt++) {
      v->CorelationResult += SampleBitArray[cnt] * Grebenka_7[cnt];
    }
    
    //GlobalC28.tmpCorelRes = v->CorelationResult;
    v->TestResResult = Breakdown; // Иначе - резистора нету и это не наш код
    if ( (v->CorelationResult > (sizeof(SampleBitArray)/sizeof(s16) - 2)) && (v->CorelationResult > 0) ) {
      v->TestResResult = Torm_OK; // Запоминаем результат операции, для передачи в блок ошибок
    }
    
    return (v->TestResResult);
    
    //break;    
  } // Расчет по Гребенке
#endif // GREBENKA_CODE_TEST_TORMOZ
        
  return (Processing); // Продложение операции
}
