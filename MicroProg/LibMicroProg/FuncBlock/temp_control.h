#ifdef    GET_FUNC_CODE

#include    "Filter.h"

// значаения в кодах АЦП для расчетра температуры ключей
#define X1_adc_tIGBT            1365
#define X2_adc_tIGBT            3413

#define MIN_TEMP_BOARD          -500      // Минимальная температура датчика (Uвых = 0В)
#define MAX_TEMP_BOARD          1500      // Максимальная температура датчика (Uвых = 2.0В)
#define ADC_TO_2V               2682      // Измерянное значение в кодах АЦП при Uвх = 2.0В        

#define FAN_TASK_MAX            100.0f    // Максимальное задание для вентелятора (в %)
#define DELTA_T_MAX             30.0f     // (Тmax - DELTA_T_MAX) = FAN_TASK_MAX 
#define GIST_ON_OFF_FAN         2.0f      // Гистерезис на включение и отключение вентиляторов  

#define KOEFF_FILTR_T           7         // Фильтрация температуры 

#define SIZE_POLINOM_T          27        // Количество точек в полиноме 

// Тип для полинома температуры
typedef struct{
    s16     Temp;
    s16     AdcVal;
}HighPowTable_type;

// Полином для ПЧ большей мощности
const HighPowTable_type TableHighPowTemp [SIZE_POLINOM_T] = {
  // Температура(*10)  // Коды АЦП
  200,  	       0,
  250,                 125,
  300,                 278,
  350,                 444,
  400,                 620,
  450,                 813,
  500,                 1013,
  550,                 1226,
  600,                 1437,
  650,                 1648,
  700,                 1862,
  750,                 2072,
  800,                 2258,
  850,                 2466,
  900,                 2652,
  950,                 2827,
  1000,                2992,
  1050,                3147,
  1100,                3283,
  1150,                3416,
  1200,                3537,
  1250,                3648,
  1300,                3750,
  1350,                3844,
  1400,                3930,
  1450,                4007,
  1500,                4095,
};

s16 CalcTempHighPow(s16 AdcTemp){
  s16 Tmin, Tmax, AdcMin, AdcMax, Temp;
  u16 i;
  
  // 1) Найти в таблице полинома значения точки линейного участка по значению АЦП
  for(i=0;i<SIZE_POLINOM_T;i++){
    if(AdcTemp < TableHighPowTemp[i].AdcVal){
      break;
    }
  }
  
  // 2) Получить из таблици коеффициенты для линейного участка
  Tmin = TableHighPowTemp[i-1].Temp;
  Tmax = TableHighPowTemp[i].Temp;
  AdcMin = TableHighPowTemp[i-1].AdcVal;
  AdcMax = TableHighPowTemp[i].AdcVal;
  
  // 3) Расчитать текущую температуру на линейном участке
  Temp = (s16)Fy_x1x2y1y2x(AdcMin, AdcMax, Tmin, Tmax, AdcTemp);
  
  return Temp;
}


// Получить задание для вентелятора
f32 GetTaskFan(f32 TekTemp, f32 TempFanOn, f32 TempFanMax, f32 PrevFanTask, f32 MinFanTask){
  f32 FanTask = 0.0f;
  
  if(TempFanOn >= TempFanMax){            // не корректные настройки, В таком случае релейное управление интенсивностью
     if(TekTemp > TempFanOn){
         FanTask = FAN_TASK_MAX;
     }
     else if(TekTemp < (TempFanOn - GIST_ON_OFF_FAN)){
         FanTask = 0.0f;
     }
     else{                                 // Гистерезис
        if(PrevFanTask > MinFanTask){    // При гистерезисе работаем на минимальном задании
            PrevFanTask = MinFanTask;
         }
       
         FanTask = PrevFanTask;
     }
  }
  else{                                   
    if(TekTemp > TempFanMax){
        FanTask = FAN_TASK_MAX;           // Включить вентелятор на 100%
     }
     else if(TekTemp >= TempFanOn){
        FanTask = ((TekTemp - TempFanMax)/(TempFanOn - TempFanMax))*(MinFanTask - FAN_TASK_MAX) + FAN_TASK_MAX;   // Уравнение кривой по двум точкам
     }
     else if(TekTemp < (TempFanOn - GIST_ON_OFF_FAN)){
        FanTask = 0.0f;                 // Минимальное задание 
     }
     else{                                   // Гистерезис
         if(PrevFanTask > MinFanTask){     // При гистерезисе работаем на минимальном задании
            PrevFanTask = MinFanTask;
         }
       
         FanTask = PrevFanTask;
     }
  }
  
  // Ограничим интенсивность задания от 0 до 100%
  if(FanTask > FAN_TASK_MAX){
     FanTask = FAN_TASK_MAX;
  }
  else if(FanTask < 0.0f){
     FanTask = 0.0f;
  }

  return FanTask;
}


// static
typedef struct{
	
    s32	 AdcFiltrIGBTu;        // Static для фильтров
    s32	 AdcFiltrIGBTv;
    s32	 AdcFiltrIGBTw;
    s32	 AdcFiltrBoard;
    
    f32 prevTaskFanIGBT;     // Задание для вентелятора IGBT (для гистерезиса)
    f32 prevTaskFanBrd;      // Задание для вентелятора блока (для гистерезиса)
    
}TEMP_CONTROL_State_type;


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_CalT_IGBT1;                   //1  CalT_IGBT1
    u16  Pnt_CalT_IGBT2;                   //2  CalT_IGBT2
    u16  Pnt_TMinFanIGBT;                  //3  TMinFanIGBT
    u16  Pnt_TMaxFanIGBT;                  //4  TMaxFanIGBT
    u16  Pnt_TmaxIGBT;                     //5  TmaxIGBT
    u16  Pnt_CalT_Brd1;                    //6  CalT_Brd1
    u16  Pnt_CalT_Brd2;                    //7  CalT_Brd2
    u16  Pnt_TMinFanBrd;                   //8  TMinFanBrd
    u16  Pnt_TMaxFanBrd;                   //9  TMaxFanBrd
    u16  Pnt_MinFanIGBT;                   //10 MinFanIGBT 
    u16  Pnt_MinFanBrd;                    //11 MinFanBrd    
    u16  Pnt_TempMax;                      //12  TempMax
    u16  Pnt_TempU;                        //13  TempU
    u16  Pnt_TempV;                        //14  TempV
    u16  Pnt_TempW;                        //15  TempW
    u16  Pnt_FAN_IGBT;                     //16  FAN_IGBT
    u16  Pnt_TBoard;                       //17  TBoard
    u16  Pnt_FAN_Brd;                      //18  FAN_Brd
    u16  Pnt_res0;                         //19  res0
    u16  Pnt_res1;                         //20  res1
    u16  Pnt_End;
}FuncTEMP_CONTROL_type;
//********************************************************
u16 *FuncTEMP_CONTROL_1(FuncTEMP_CONTROL_type *progPnt, u32 ramPnt)
{
  TEMP_CONTROL_State_type   *sPnt;
  s16	                    tIGBTtemp, tIGBTall, tIGBTu, tIGBTv, tIGBTw, tBoard;
  s16                       t_1_v, t_2_v, Tmax_meas, Tmax;
  f32                       TempMinFan, TempMaxFan, TekTemp;
  f32                       Tcor;
  u16                       MaxValueTIGBT_InADC;  
  f32                       MinFanIGBT, MinFanBoard;
  
  
  sPnt = (TEMP_CONTROL_State_type *)(ramPnt + progPnt->Pnt_State);
  tIGBTtemp = tIGBTall = tIGBTu = tIGBTv = tIGBTw = 0;

  Tcor = 0.001 * load_s16(ramPnt, progPnt->Pnt_CalT_Brd1); // Вычитываем корректировочный коэф. на температуру 
  
  // 1) Инициализация при старте
  if(flagIsFirstCall){
    GlobalM4.LowSpeedAinAdc.TempIGBTU = 0;
    GlobalM4.LowSpeedAinAdc.TempIGBTV = 0;
    GlobalM4.LowSpeedAinAdc.TempIGBTW = 0;    
    GlobalM4.LowSpeedAinAdc.TempBoard = 0;    
  }
  
  // 2) Получить калибровочные коеффициенты для температуры ключей
  t_1_v = load_s16(ramPnt, progPnt->Pnt_CalT_IGBT1);
  t_2_v = load_s16(ramPnt, progPnt->Pnt_CalT_IGBT2);
  
#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
     MaxValueTIGBT_InADC = 0;      // Максимальное значение температуры в АЦП (0 в АЦП - Максимальная темпепатура)
  
     tIGBTtemp = (s16)Fy_x1x2y1y2x(X1_adc_tIGBT, X2_adc_tIGBT, t_1_v, t_2_v, GlobalM4.LowSpeedAinAdc.TempIGBTU);   // Преобразовать коды АЦП в градусы 
     s16_Filtr(tIGBTu, tIGBTtemp, sPnt->AdcFiltrIGBTu, KOEFF_FILTR_T);                                             // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempU, tIGBTu);                                                                 // Выводим в уставку
     
     // Получить текущую температуру ключей по фазе V
     tIGBTtemp = (s16)Fy_x1x2y1y2x(X1_adc_tIGBT, X2_adc_tIGBT, t_1_v, t_2_v, GlobalM4.LowSpeedAinAdc.TempIGBTV);   // Преобразовать коды АЦП в градусы 
     s16_Filtr(tIGBTv, tIGBTtemp, sPnt->AdcFiltrIGBTv, KOEFF_FILTR_T);                                             // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempV, tIGBTv);
  
     // Получить текущую температуру ключей по фазе W
     tIGBTtemp = (s16)Fy_x1x2y1y2x(X1_adc_tIGBT, X2_adc_tIGBT, t_1_v, t_2_v, GlobalM4.LowSpeedAinAdc.TempIGBTW);   // Преобразовать коды АЦП в градусы 
     s16_Filtr(tIGBTw, tIGBTtemp, sPnt->AdcFiltrIGBTw, KOEFF_FILTR_T);                                             // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempW, tIGBTw);
      // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#elif defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
     
  // 3) Расчитать температуру по фазам в зависимости от типа ПЧ
  if(GlobalM4.DrvType == LowPow_PB24){   // Для ПЧ малой мощности
     MaxValueTIGBT_InADC = 4096;      // Максимальное значение температуры в АЦП (4096 в АЦП - Максимальная темпепатура)
     
     // Получить текущую температуру ключей по фазе U
     tIGBTtemp = (s16)Fy_x1x2y1y2x(X1_adc_tIGBT, X2_adc_tIGBT, t_1_v, t_2_v, GlobalM4.LowSpeedAinAdc.TempIGBTU);   // Преобразовать коды АЦП в градусы 
     s16_Filtr(tIGBTu, tIGBTtemp, sPnt->AdcFiltrIGBTu, KOEFF_FILTR_T);                                             // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempU, tIGBTu);                                                                 // Выводим в уставку
     save_s16(ramPnt, progPnt->Pnt_TempV, tIGBTu); // копия                                                        // Выводим в уставку
     save_s16(ramPnt, progPnt->Pnt_TempW, tIGBTu); // копия                                                        // Выводим в уставку
     
     tIGBTw = tIGBTv = tIGBTu;
     
     // Учитывая, что температура измеряется только одна, то две другие температуры скрыты
//     // Получить текущую температуру ключей по фазе V
//     tIGBTtemp = (s16)Fy_x1x2y1y2x(X1_adc_tIGBT, X2_adc_tIGBT, t_1_v, t_2_v, GlobalM4.LowSpeedAinAdc.TempIGBTV);   // Преобразовать коды АЦП в градусы 
//     s16_Filtr(tIGBTv, tIGBTtemp, sPnt->AdcFiltrIGBTv, KOEFF_FILTR_T);                                             // Подфильтруем температуру
//     save_s16(ramPnt, progPnt->Pnt_TempV, tIGBTv);
//  
//     // Получить текущую температуру ключей по фазе W
//     tIGBTtemp = (s16)Fy_x1x2y1y2x(X1_adc_tIGBT, X2_adc_tIGBT, t_1_v, t_2_v, GlobalM4.LowSpeedAinAdc.TempIGBTW);   // Преобразовать коды АЦП в градусы 
//     s16_Filtr(tIGBTw, tIGBTtemp, sPnt->AdcFiltrIGBTw, KOEFF_FILTR_T);                                             // Подфильтруем температуру
//     save_s16(ramPnt, progPnt->Pnt_TempW, tIGBTw);
  } else if (GlobalM4.DrvType == HighPow_Combo) {// Для ПЧ большей мощности (Combo)
     MaxValueTIGBT_InADC = 4096;      // Максимальное значение температуры в АЦП (4096 в АЦП - Максимальная темпепатура)

     // Получить текущую температуру ключей по фазе U
     tIGBTtemp = CalcTempHighPow(GlobalM4.LowSpeedAinAdc.TempIGBTU);         // Преобразовать коды АЦП в градусы 
     tIGBTtemp = (s16)(tIGBTtemp * Tcor);                                    // Корректировка температуры коэффициентом
     s16_Filtr(tIGBTu, tIGBTtemp, sPnt->AdcFiltrIGBTu, KOEFF_FILTR_T);       // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempU, tIGBTu);                           // Выводим в уставку
   
     // Получить текущую температуру ключей по фазе V
     tIGBTtemp = CalcTempHighPow(GlobalM4.LowSpeedAinAdc.TempIGBTV);         // Преобразовать коды АЦП в градусы 
     tIGBTtemp = (s16)(tIGBTtemp * Tcor);                                    // Корректировка температуры коэффициентом
     s16_Filtr(tIGBTv, tIGBTtemp, sPnt->AdcFiltrIGBTv, KOEFF_FILTR_T);       // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempV, tIGBTv);                           // Выводим в уставку
     
     // Получить текущую температуру ключей по фазе W
     tIGBTtemp = CalcTempHighPow(GlobalM4.LowSpeedAinAdc.TempIGBTW);         // Преобразовать коды АЦП в градусы 
     tIGBTtemp = (s16)(tIGBTtemp * Tcor);                                    // Корректировка температуры коэффициентом
     s16_Filtr(tIGBTw, tIGBTtemp, sPnt->AdcFiltrIGBTw, KOEFF_FILTR_T);       // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempW, tIGBTw);                           // Выводим в уставку
  } else if (GlobalM4.DrvType == HighPow_PB24) {
	#ifdef _PROJECT_FOR_STM32_     
	#warning Настройка измерений температуры силовых блоков РВ24-М40. Исправить!!!
	#endif
     MaxValueTIGBT_InADC = 4096;      // Максимальное значение температуры в АЦП (4096 в АЦП - Максимальная темпепатура)

     // Получить текущую температуру ключей по фазе U
     tIGBTtemp = CalcTempHighPow(GlobalM4.LowSpeedAinAdc.TempIGBTU);         // Преобразовать коды АЦП в градусы 
     tIGBTtemp = (s16)(tIGBTtemp * Tcor);                                    // Корректировка температуры коэффициентом
     s16_Filtr(tIGBTu, tIGBTtemp, sPnt->AdcFiltrIGBTu, KOEFF_FILTR_T);       // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempU, tIGBTu);                           // Выводим в уставку
   
     // Получить текущую температуру ключей по фазе V
     tIGBTtemp = CalcTempHighPow(GlobalM4.LowSpeedAinAdc.TempIGBTV);         // Преобразовать коды АЦП в градусы 
     tIGBTtemp = (s16)(tIGBTtemp * Tcor);                                    // Корректировка температуры коэффициентом
     s16_Filtr(tIGBTv, tIGBTtemp, sPnt->AdcFiltrIGBTv, KOEFF_FILTR_T);       // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempV, tIGBTv);                           // Выводим в уставку
     
     // Получить текущую температуру ключей по фазе W
     tIGBTtemp = CalcTempHighPow(GlobalM4.LowSpeedAinAdc.TempIGBTW);         // Преобразовать коды АЦП в градусы 
     tIGBTtemp = (s16)(tIGBTtemp * Tcor);                                    // Корректировка температуры коэффициентом
     s16_Filtr(tIGBTw, tIGBTtemp, sPnt->AdcFiltrIGBTw, KOEFF_FILTR_T);       // Подфильтруем температуру
     save_s16(ramPnt, progPnt->Pnt_TempW, tIGBTw);                           // Выводим в уставку    
  } else {
	  // Если блок не определен - то сразу выходим, иначе начинаются глюки по температуре
	  return &progPnt->Pnt_End;
  }
    
#endif // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
  
  // 4) Получить наибольшую температуру ключей 
  tIGBTall = tIGBTu;
  if(tIGBTv > tIGBTall){
     tIGBTall = tIGBTv;
  }
  if(tIGBTw > tIGBTall){
     tIGBTall = tIGBTw;
  }
  
  save_s16(ramPnt, progPnt->Pnt_TempMax, tIGBTall);        // Выводим текущее значение температуры ключей (найбольшее значение)
  
  // 5) Сохранить текущую температуру ключей в глобальную структуру (для обработчика аварий)
  GlobalM4.TIGBT = (float)tIGBTall * 0.1f;
      
  // 6) Обработка аварийного значения (уставки Tmax) 
  Tmax = load_s16(ramPnt, progPnt->Pnt_TmaxIGBT);                                                // Прочитать значение аварийной уставки 
  Tmax_meas = (s16)Fy_x1x2y1y2x(X1_adc_tIGBT, X2_adc_tIGBT, t_1_v, t_2_v, MaxValueTIGBT_InADC);  // Максимально возможная температура при ADC = 4096
  Tmax_meas = (s16)((float)Tmax_meas * 0.98f);                                                   // -2% от макс значения
    
  if(Tmax > Tmax_meas){                                                                          // Значение уставки больше максимально возможной температуры
      Tmax = Tmax_meas;
      save_s16(ramPnt, progPnt->Pnt_TmaxIGBT, Tmax);
  }
  
  GlobalM4.FailsSetup.Tmax = (float)load_s16(ramPnt, progPnt->Pnt_TmaxIGBT) * 0.1f;               // Максимальная температура ключей (Уставка)
  
  // 7) Получить текущую температуру блока CP24B
  tIGBTtemp = (s16)Fy_x1x2y1y2x(0, ADC_TO_2V, MIN_TEMP_BOARD, MAX_TEMP_BOARD, GlobalM4.LowSpeedAinAdc.TempBoard);  // Преобразовать коды АЦП в градусы 
  s16_Filtr(tBoard, tIGBTtemp, sPnt->AdcFiltrBoard, KOEFF_FILTR_T);                                                // Подфильтруем температуру
  save_s16(ramPnt, progPnt->Pnt_TBoard, tBoard);
   
  // 8) Расчитать задание на интенсивность работы вентилятора охлаждение IGBT
  TempMinFan = (float)load_s16(ramPnt, progPnt->Pnt_TMinFanIGBT) * 0.1f;        // Температура при которой включается вентелятор на минимальных оборотах 
  TempMaxFan = (float)load_s16(ramPnt, progPnt->Pnt_TMaxFanIGBT) * 0.1f;        // Температура при которой вентелятор выходит на максимальную скорость
  TekTemp = (float)tIGBTall * 0.1f;
  MinFanIGBT = (float)load_s16(ramPnt, progPnt->Pnt_MinFanIGBT);                // Минимальное задание вентилятора IGBT
    
  sPnt->prevTaskFanIGBT = GetTaskFan(TekTemp, TempMinFan, TempMaxFan, sPnt->prevTaskFanIGBT, MinFanIGBT);

  save_s16(ramPnt, progPnt->Pnt_FAN_IGBT, (u16)((sPnt->prevTaskFanIGBT * 10.0f) + 0.5f));         // Выдать задание для вентелятора IGBT с одним знаком после запятой
  
  // 9) Расчитать задание на интенсивность работы вентелятора охлаждение блока
  TempMinFan = (float)load_s16(ramPnt, progPnt->Pnt_TMinFanBrd) * 0.1f;         // Температура при которой включается вентелятор на минимальных оборотах 
  TempMaxFan = (float)load_s16(ramPnt, progPnt->Pnt_TMaxFanBrd) * 0.1f;         // Температура при которой вентелятор выходит на максимальную скорость
  TekTemp = (float)tBoard * 0.1f;                                               // Текущая температура блока 
  MinFanBoard = (float)load_s16(ramPnt, progPnt->Pnt_MinFanBrd);                // Минимальное задание вентилятора СУ
  
  sPnt->prevTaskFanBrd = GetTaskFan(TekTemp, TempMinFan, TempMaxFan, sPnt->prevTaskFanBrd, MinFanBoard);
  
  save_s16(ramPnt, progPnt->Pnt_FAN_Brd, (u16)((sPnt->prevTaskFanBrd * 10.0f) + 0.5f));   // Выдать задание для вентелятора блока с одним знаком после запятой
    
  
  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_CONTROL_1[20]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  CalT_IGBT1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  CalT_IGBT2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TMinFanIGBT
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  TMaxFanIGBT
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  TmaxIGBT
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  CalT_Brd1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  CalT_Brd2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  TMinFanBrd
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  TMaxFanBrd
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  MinFanIGBT
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  MinFanBrd
        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  TempMax
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  TempU
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14  TempV
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //15  TempW
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  FAN_IGBT
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  TBoard
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  FAN_Brd
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //19  res0
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //20  res1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEMP_CONTROL", "DA", TblFuncTEMP_CONTROL_1, FuncTEMP_CONTROL_1, sizeof(TblFuncTEMP_CONTROL_1), sizeof(TEMP_CONTROL_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_CONTROL_1,
#endif
//********************************************************
