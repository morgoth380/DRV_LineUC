#ifdef    GET_FUNC_CODE

#include    "apv_type.h"

//===================================================================
u32     APVpnt[NUM_APV_BLOCK];  // Массив c временем запуска по АПВ для локальных блоков
u16     normalWaitAPV;          // Время нормализации АПВ    
u32     globFailTime;
                                                  // 0- нет запроса на АПВ !=0 время запроса
APVState_type            APVState;                // Структура с состоянием глобального блока АПВ. Объект виден для модуля LOCAL_APV.h
#define     nvAPVState   (*(nvAPVState_type*)0)
#define     ONE_DAY_SEC   24*60*60
//#define     KolvBlockAPV     4                    // число блоков АПВ
//===================================================================
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_RTC;                          //1  RTC
    u16  Pnt_PuskKey;                      //2  Pusk
    u16  Pnt_Stop;                         //3  Stop
    u16  Pnt_EN_Time;                      //4  EN_Time
    u16  Pnt_En;                           //5  En
    u16  Pnt_PWM_ON;                       //6  PWM_ON
    u16  Pnt_Not_Ready;                    //7  Not_Ready
    u16  Pnt_Err_Time_APV;                 //8  Err_Time_APV;
    u16  Pnt_Blink;                        //9  Blink
    u16  Pnt_Pusk;                         //10 Pusk
    u16  Pnt_TimeAPV_min;                  //11 TimeAPV_min
    u16  Pnt_TimeAPV_sec;                  //12 TimeAPV_sec
    u16  Pnt_End;
}FuncAPV1_type;
//********************************************************
u16 *FuncAPV1_1(FuncAPV1_type *progPnt, u32 ramPnt)
{
  
    APVState_type       *globPnt;
    nvAPVState_type     *nvAdr1;
    void                *nvAdr2;
    void                *nvAdr3;
    s32                 deltaTm, deltaTs;
    u32                 Time, TimeToAPV, APV_EN_Time, failTime, progFailTime;
    u16                 numBlockApv, i, notReady, PowerFailFlag, crc;
    APV1_State_type     *sPnt;
    u8                  puskKey, stopKey;
    
    numBlockApv = PowerFailFlag = 0;
    progFailTime = 0;
//===================================================================

//===================================================================
    globPnt  = &APVState;
    nvAdr1 = GetNvAdr(progPnt->Pnt_nvState, &nvAPVState);
    nvAdr2 = (void *)((u32)&nv.stopTime);                       //Адрес переменной со временем аврийного останова
    nvAdr3 = (void *)((u32)&nv.EngineRunFlag);                  //вычисляем адрес флага работающего двигателя. служит как флаг аварии по питанию
    Time  = load_s32(ramPnt, progPnt->Pnt_RTC);                 //Текущее время.
    sPnt = (APV1_State_type *)(ramPnt + progPnt->Pnt_State);
    APV_EN_Time = 0;                                            //Переменная для времени аварийного останова?
    notReady  = load_s16(ramPnt, progPnt->Pnt_Not_Ready);          // Текущая авария ПЧ
        
//===================================================================
// Первый вызов программы
//====================================================================

    if(flagIsFirstCall)
    {
        RdFramMicro(globPnt, nvAdr1, sizeof(nvAPVState_type));  //По включению питания восстановить состояние //Считываем с FRAM в APVState настройки АПВ?
        crc = crcPacket((u8*)globPnt,  sizeof(nvAPVState_type) - sizeof(u16));
        
        if (crc != globPnt->crc) {
          globPnt->APVstatus.all = 0;
          globPnt->Time = 0;
          globPnt->APV_Freeze = 0;
          globPnt->ApvResetCntTime = 0;
          globPnt->crc = crcPacket((u8*)globPnt,  sizeof(nvAPVState_type) - sizeof(u16));
          WrFramMicro(nvAdr1, globPnt, sizeof(nvAPVState_type)); // Записать данные в Фрам
        }
        
        if (Time == 0){                                         //Защита от неустановленных часов.
          return &progPnt->Pnt_End; 
        }             
        globPnt->Time = Time;                                   //сохраняем текущее считанное время
        globFailTime = 0;                                       //пока блок power loss apv сам не проинициализирует ее
        for(i = 0; i < NUM_APV_BLOCK; i++){                     //начальное обнуление массива для хранения времени до АПВ каждого локального блока АПВ
            APVpnt[i] = 0;
        }
        return &progPnt->Pnt_End;                               // Дать возможность локальным блокам прописать указатели
    }
    // Защита от неустановленных часов
    if (Time == 0)  { 
      return &progPnt->Pnt_End; 
    } 
    //считываем значение бита разрешения режима АПВ
    globPnt->APVstatus.bits.AutoMode = testBitMicro(ramPnt, progPnt->Pnt_En);

    globPnt->APVstatus.bits.SecondFlag  = 0;
    if( globPnt->Time != Time ){                                // Время 1 сек
        globPnt->Time = Time;                                   // обновляем текущее время
        globPnt->APVstatus.bits.SecondFlag  = 1;                     // Для синхронизации локальных блоков
    }
    stopKey = testBitMicro(ramPnt, progPnt->Pnt_Stop);
    puskKey = testBitMicro(ramPnt, progPnt->Pnt_PuskKey);
//====================================================================
// Проверка наличия глобальной команды СТОП
//====================================================================
    if(stopKey || puskKey){               //АПВ запрещено - глобальная команда СТОП    
        if(stopKey)
            globPnt->APVstatus.bits.StopKeyState     = 1;                    //установка статуса стопа       
        else
        {
            globPnt->APVstatus.bits.PuskKeyState     = 1;                    //установка статуса пуска
            clrBitMicro(ramPnt, progPnt->Pnt_Err_Time_APV);
        }
        globPnt->APVstatus.bits.APV              = 0;                        //сброс запроса АПВ
        globPnt->APVstatus.bits.SecondFlag       = 0;
        globPnt->APVstatus.bits.Blink            = 0;
        globPnt->APVstatus.bits.StopResetApvCnt  = 1;			//Запрос на сброс всех счетчиков АПВ, т.к. пришла команда СТОП
        globFailTime                        = progFailTime = 0;         //Запрет АПВ. Сброс данных АПВ
        PowerFailFlag                       = 0;
        WrFramMicro(nvAdr3, &PowerFailFlag, sizeof(u16))    ;           //Запрет АПВ. Сброс флага аварии
        WrFramMicro(nvAdr2, &progFailTime,  sizeof(u32))    ;           //Запрет АПВ. Сброс времени аварии          
        for(i = 1; i < NUM_APV_BLOCK; i++) {                
          APVpnt[i] = 0;            // Если выполнен пуск или стоп, обнулить все времена по АПВ
        }        
    }
    else                                                                //иначе нет глобальной команды СТОП
    {
        if (globPnt->APVstatus.bits.StopResetApvCnt) {                       // сброс флага очистки счетчиков АПВ(&& (globPnt->ApvResetCnt >= KolvBlockAPV) )
            globPnt->APVstatus.bits.StopResetApvCnt = 0;
        }
        if(globPnt->APVstatus.bits.StopKeyState)
            globPnt->APVstatus.bits.StopKeyState = 0;
        else if(globPnt->APVstatus.bits.PuskKeyState)
            globPnt->APVstatus.bits.PuskKeyState = 0; 
    }

//=====================================================================
    clrBitMicro(ramPnt, progPnt->Pnt_Pusk);                         //Сброс выхода Пуск АПВ (сброс команды на пуск АПВ)
//============================================================
    deltaTm = 0;                                                    //сколько осталось минут до пуска
    deltaTs = 0;                                                    //сколько осталось секунд до пуска

    RdFramMicro(&progFailTime, nvAdr2, sizeof(u32));                //считываем время аварийного останова, реализованного программой (не 0 - если была авария)


    APV_EN_Time = (u32)load_s16(ramPnt, progPnt->Pnt_EN_Time) * 60; //интервал времени, в течении кот. активная функция АПВ (в секундах)
                                                                    //если из POWER_LOSS_APV передано время аварийного исчезновения напряжения, 
                                                                    // то globFailTime будет отлично от нуля
    failTime = (globFailTime)? globFailTime: progFailTime;          //если было передано время аварии по питанию
    APV_EN_Time += failTime;                                        //получаем время, до истечения которого активна функция АПВ

    
    // Проверка счетчика обнуления кол-ва АПВ по времени нормальной работы
    if (globPnt->ApvResetCntTime && globPnt->ApvResetCntTime < Time) // Если время прошло после пуска с АПВ - значит можно сбросить счетчики      
    { // Хотя не ясно, я ведь могу сидеть в аварии долго и просто потом от бездейтсвия тоже сброшу
        globPnt->APVstatus.bits.StopResetApvCnt  = 1;
        //Запрос на сброс всех счетчиков АПВ, т.к. пришла команда СТОП      
        globPnt->ApvResetCntTime = 0;
        
        globPnt->crc = crcPacket((u8*)globPnt,  sizeof(nvAPVState_type) - sizeof(u16));
        WrFramMicro(nvAdr1, globPnt, sizeof(nvAPVState_type)); // Записать данные в Фрам
    }
        
    if ((globPnt->APVstatus.bits.AutoMode != 0) && (APV_EN_Time > Time))    // пока APV_EN_Time больше ткущего времени ждем запрос на АПВ
    {
        TimeToAPV = 0; //переменная времени запуска
        for(i = 1; i < NUM_APV_BLOCK; i++)
        {
            if( APVpnt[i] > TimeToAPV )           //Перебираю существующие блоки АПВ
            {                                     //Выбираю наибольшее время отличное от 0
                TimeToAPV = APVpnt[i];            //Новая уставка больше - заменяю
                numBlockApv = i;
            }
        }
        if ((globPnt->APV_Freeze != 0) || (notReady != 0))
        { 
            deltaTs = TimeToAPV;
            deltaTm = deltaTs/60;
            deltaTs = deltaTs%60;        
        }
        else if(globPnt->APVstatus.bits.APV == 1) //если есть запрос на АПВ и задано время АПВ (формируется в блоке LOCAL_APV.h)
        {
            //deltaTs = TimeToAPV - Time; //отсчет времени. Когда значение Time достигнет TimeToAPV то нужно дать команду запуска
            if (TimeToAPV > 0)                                            //Если до момента пуска еще не досчитали
            {                                                             //При наличии взведённого флага - идет отработка АПВ,
                deltaTm = TimeToAPV/60;                                   //Количество минут
                deltaTs = TimeToAPV%60;                                   //Количество секунд (0...59)
                if(globPnt->APVstatus.bits.SecondFlag == 1)                          //Если секунда прошла
                {
                    if(normalWaitAPV != 0)
                        normalWaitAPV--;
                    else
                    {
                        APVpnt[numBlockApv]--;
                        globPnt->APVstatus.bits.Blink = !globPnt->APVstatus.bits.Blink;       //Мигаем выходом
                    }
                }
             }
            else if (TimeToAPV <= 0)                                    //Пришло время пора пускаться
            {
                globPnt->APVstatus.bits.APV      = 0;                        //Запрос обработали - сброс данных АПВ
                globPnt->APVstatus.bits.APVLatch = 0;                        //Запрос обработали - сброс данных АПВ
                setBitMicro(ramPnt, progPnt->Pnt_Pusk);                 //Команда пуск
                globFailTime  =    progFailTime =  0;                   //Отработали запрос на АПВ. Сброс данных АПВ
                WrFramMicro(nvAdr3, &PowerFailFlag, sizeof(u16))    ;   //Отработали запрос на АПВ. Сброс флага аварии
				PowerFailFlag  =0;
                WrFramMicro(nvAdr2, &progFailTime, sizeof(u32))     ;           //Отработали запрос на АПВ. Сброс времени аварии
                globPnt->APVstatus.bits.Blink = 0                           ;        //Команду на АПВ дали. Больше не мигаем индикатором готовности
                sPnt->latch = 0;
                
                globPnt->ApvResetCntTime = Time + ONE_DAY_SEC; // Задаем величину задержки для сброса счетчиков после пука через АПВ + 86400 сек

                globPnt->crc = crcPacket((u8*)globPnt,  sizeof(nvAPVState_type) - sizeof(u16));
                WrFramMicro(nvAdr1, globPnt, sizeof(nvAPVState_type)); // Записать данные в Фрам
                
                for(i = 1; i < NUM_APV_BLOCK; i++) {                
                    APVpnt[i] = 0;            // Если выполнен пуск, обнулить все времена по АПВ
                }                
            }
        }
    }
    else if (!testBitMicro(ramPnt, progPnt->Pnt_PWM_ON) && failTime)//иначе автоматический режим запрещен или время активности функции АПВ прошло, и пуск так и не дали
    {
        if(globPnt->APVstatus.bits.APV == 1)                                 // если должно быть АПВ, но время активности АВП вышло
            setBitMicro(ramPnt, progPnt->Pnt_Err_Time_APV);
        
        failTime = globFailTime  =  PowerFailFlag  =  progFailTime =  0;        //Отработали запрос на АПВ. Сброс данных АПВ
        WrFramMicro(nvAdr3, &PowerFailFlag, sizeof(u16))    ;                   //Отработали запрос на АПВ. Сброс флага аварии
        WrFramMicro(nvAdr2, &progFailTime,  sizeof(u32))    ;                   //Отработали запрос на АПВ. Сброс времени аварии
        globPnt->APVstatus.bits.Blink   = 0                         ;                //Больше не мигаем индикатором готовности
        sPnt->latch = 0;
        globPnt->APVstatus.bits.APV      = 0; 
        
    }

    save_s16(ramPnt, progPnt->Pnt_TimeAPV_min,(u16)deltaTm)       ;             //сколько минут осталось до пуска
    save_s16(ramPnt, progPnt->Pnt_TimeAPV_sec,(u16)deltaTs)       ;              //сколько секунд осталось до пуска
    defBitMicro(ramPnt, progPnt->Pnt_Blink, globPnt->APVstatus.bits.Blink);          //мигаем индикатором готовности

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAPV1_1[12]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  RTC
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Pusk
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Stop
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  EN_Time
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  En
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  PWM_ON
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  TekFail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Err_Time_APV
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Blink
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Pusk
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //11 TimeAPV_min
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //12  TimeAPV_sec
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("APV1", "DA", TblFuncAPV1_1, FuncAPV1_1, sizeof(TblFuncAPV1_1), sizeof(APV1_State_type), sizeof(nvAPVState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAPV1_1,
#endif
//********************************************************