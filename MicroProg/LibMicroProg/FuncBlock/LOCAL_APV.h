
#ifdef    GET_FUNC_CODE
//====================================================================
#include    "apv_type.h"
#ifndef _PROJECT_FOR_CCS_
    #include "GUI.h"
#endif
//====================================================================
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Fail_IN;                      //01 Fail_IN                    //Вход сигнала Ошибка
    u16  Pnt_Off_Block_Apv;                //02 Off/Block/Apv 0-1-2        //Вход сигнала управления формированием АПВ
    u16  Pnt_DeleyControl;                 //03 DeleyControl               //Задержка на время пуска
    u16  Pnt_DeleyOff;                     //04 DeleyOff                   //Задержка формирования ошибки
    u16  Pnt_NormalWait;                   //05 NormalWait                 //Ожидать нормализации
    u16  Pnt_ApvTime_sec;                  //06 ApvTime_sec                //Уставка Время АПВ сек.
    u16  Pnt_ApvTime_min;                  //07 ApvTime_min                //Уставка время АПВ мин. Тапв = ( мин + сек )
    u16  Pnt_KolvApv;                      //08 KolvApv                    //Уставка Допустимое количаство АПВ
    u16  Pnt_ResetApv;                     //09 ResetApv                   //Вход Сброс счётчика АПВ
    u16  Pnt_On_Off;                       //10 On/Off                     //Включить\выключить блок АПВ
    u16  Pnt_RTC;                          //11 UnixTime                   //Реальное время контроллера
    u16  Pnt_Vybeg_Time;                   //12 VybegTime                  //Реальное время выбега!
    u16  Pnt_ApvCnt;                       //13 ApvCnt                     //Выход счётчика АПВ
    u16  Pnt_Warning;                      //14 Warning                    //Выход предупреждение о Аварии
    u16  Pnt_Fail;                         //15 Fail                       //Выход Авария
    u16  Pnt_End;
}FuncLOCAL_APV_type;
//********************************************************
//********************************************************
u16 *FuncLOCAL_APV_1(FuncLOCAL_APV_type *progPnt, u32 ramPnt)
{

    u16                             Off_Block_Apv, Warning, Fail, FailIn, NormalWait;
    u32                             TimeToAPV;
    LOCAL_APVState_type             *sPnt;
    LOCAL_APVnvState_type           *nvAdr;
    APVState_type                   *globsPnt;

    volatile static count = 0;
    ++count;

//====================================================================
#ifndef _PROJECT_FOR_CCS_   //Защита от проверок в Visual при отладке
    if (StateTestMicroProg != 0) return &progPnt->Pnt_End;
#endif
//====================================================================
    sPnt        = (LOCAL_APVState_type *)(ramPnt + progPnt->Pnt_State); //sPnt - указатель на статическую область памяти
    nvAdr       = GetNvAdr( progPnt->Pnt_nvState, 0);                   //nvAdr - указатель на энергонезависимую область памяти
    globsPnt    = &APVState;                                            //указатель на глобальную структуру, определенную в  модуле АПВ apv1.h
//====================================================================


    if(flagIsFirstCall)
    {
        static u16      numBlock = 0;                             //Счетчик локальных блоков АПВ (определяется количеством вызовов данного функционального блока: сколько вызовов, столько и контроллируемых параметров)
        RdFramMicro( sPnt, nvAdr, sizeof(LOCAL_APVnvState_type)); //Восстановить состояние (считывание из FRAM в статический объект)
        APVpnt[numBlock] = sPnt->TimeToAPV;      // Сохраняем интервал времени  до АПВ
        sPnt->numBlock = numBlock++;            // Получаю номерок
        sPnt->tmrDeleyControl  = 0;             // Обнулить таймер задержки на опрашивание входа аварии
        sPnt->tmrDeleyOff      = 0;             // Обнулить таймер длительности наличия сигнала авария
        sPnt->flag.ApvLatch    = 0;             // Обнулить флаг ожидания восстановления параметра
        return &progPnt->Pnt_End;
    }

    Off_Block_Apv = load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv); //Состояние входа управления локальным блоком АПВ
    FailIn = testBitMicro(ramPnt, progPnt->Pnt_Fail_IN);          //Состояние входа "Авария"

    if(Off_Block_Apv == 0)           //Если локальный блок АПВ отключен..
        {
            sPnt->tmrDeleyControl   = 0; //Обнулить таймер задержки на опрашивание входа аварии
            sPnt->tmrDeleyOff       = 0; //Обнулить таймер длительности наличия сигнала авария
            sPnt->flag.all          = 0; //Сбросить все флаги
            if(sPnt->TimeToAPV != 0)
            {
                WrFramMicro(&nvAdr->TimeToAPV, &sPnt->TimeToAPV, sizeof(nvAdr->TimeToAPV));
            }
            sPnt->TimeToAPV         = 0;
            Warning = 0;
            Fail = 0;
        }
//====================================================================
//              Разбираемся с текущими авариями
//====================================================================
    else if(testBitMicro(ramPnt, progPnt->Pnt_On_Off) == 0) // Контроль отключен флаги - в останове
    {  //Контроля нет - все в сбросе
        Warning = 0;                      //сброс флага
        Fail    = 0;                      //сброс флага
        sPnt->tmrDeleyControl       = 0;  //сброс счетчика
        sPnt->tmrDeleyOff           = 0;  //сброс счетчика
    }
    else if( sPnt->tmrDeleyControl < load_s16(ramPnt, progPnt->Pnt_DeleyControl)+1) // Контроль включен - отсчитываем время задержки перед проверкой входа аварии
    {
        if(globsPnt->APVstatus.SecondFlag == 1)  //если секунда прошла..
            sPnt->tmrDeleyControl++;            //инкремент счетчика задержки проверки входа аварии
        Warning = 0;
        Fail    = 0;
    }
    else if(FailIn == 0) //когда задержка на проверку входа аварии прошла, то проверяем вход аварии. если ее нет..
    {
        Warning = 0; //сброс флага
        Fail    = 0; //сброс флага
        /*
        if( sPnt->tmrDeleyOff !=0 ) //проверяем была ли до этого авария, то есть досчитал ли счетчик длительности аварии до любого отличного от нуля значения. Если была, отсчитываем обратно
        {
            if(globsPnt->APVstatus.SecondFlag ==1)
                sPnt->tmrDeleyOff--;                            // Таймер аварии
        }
        */
        sPnt->tmrDeleyOff = 0;
    }
    else //иначе на входе есть авария: FailIn = 1
    {
        Warning = 1;                                        // Предупреждение на выход
        Fail    = 0;
        if( sPnt->tmrDeleyOff < load_s16(ramPnt, progPnt->Pnt_DeleyOff )) //делаем выдержку прежде чем подтвердить аварию
        {
            if(globsPnt->APVstatus.SecondFlag == 1) { sPnt->tmrDeleyOff++; }  // Таймер аварии
        }
        else
        {                                                           // Время вышло пора принимать меры !!
            Fail    = 1;                                            // Подтверждаем факт наличия аварийного останова
        }
    }
    //после вышеприведенных операторов имеем либо подтвержденную аварию на входе функционального блока, либо отсутствие подтверждения аварии (сигнал аварии на входе может присутствовать, но факт аварии может быть еще не подтвержден)
//====================================================================
//       Разбираемся с запросами на АПВ
//====================================================================
//====================================================================
    if(globsPnt->APVstatus.APV == 0) //если в глобальном блоке отработан запрос на АПВ, отправленный из текущего локального блока..
    {
        sPnt->flag.ApvLatch = 0;     //..то здесь сбрасываем флаг ожидания восстановления параметра (означает, что восстановления дождались и попытались запустить двигатель)
    }
//====================================================================
#ifdef _PROJECT_FOR_CCS_
#warning подумать куда правильней вставить считывание времени до АПВ
#endif
    if(globsPnt->APVstatus.AutoMode != 0) //если АПВ разрешено глобально
    {
        if((Off_Block_Apv == 2) && (globsPnt->APVstatus.StopKeyState == 0)) //проверяем разрешена ли работа локального блока АПВ
        {
            NormalWait = load_s16(ramPnt, progPnt->Pnt_NormalWait);      //считываем параметр, задающий нужно ли ждать нормализации перед началом отсчета таймера самозапуска
            TimeToAPV = load_s16(ramPnt, progPnt->Pnt_ApvTime_min)*60 + load_s16(ramPnt, progPnt->Pnt_ApvTime_sec)+ load_s16(ramPnt, progPnt->Pnt_Vybeg_Time); //формируем значение задержки на АПВ при восстановлении параметра
            TimeToAPV = TimeToAPV + load_s32(ramPnt, progPnt->Pnt_RTC);   //формируем значение задержки на АПВ при восстановлении параметра
            sPnt->TimeToAPV         = TimeToAPV;                          //сохраняем полученное время в статическом объекте
            if(Fail != 0) //если есть подтвержденная авария..
            {
            	APVpnt[sPnt->numBlock]  = TimeToAPV;                          //сохраняем полученное время в обменном массиве. Это значение времени используется в глобальном блоке АПВ
            	if(!NormalWait && (load_s16(ramPnt, progPnt->Pnt_KolvApv) > sPnt->CntApv) && !(sPnt->flag.ApvLatch))//..то проверим нужно ли ждать нормализации параметра. Если ждать НЕ НУЖНО и не превышено количество попыток АПВ то сразу формируем запрос на АПВ
            	{
            		sPnt->flag.ApvLatch = 1;                                      //Флаг ожидания восстановления параметра
            		sPnt->CntApv++;                                               //засчитываем факт попытки АПВ
            		globsPnt->APVstatus.APV = 1;                                  //формируем запрос на АПВ без ожидания восстановления параметра. По этому флагу глобальный блок АПВ начнет отсчет времени задержки до АПВ
            	}
            	else if((load_s16(ramPnt, progPnt->Pnt_KolvApv) > sPnt->CntApv) && !(sPnt->flag.ApvLatch))//иначе, если НУЖНО ждать восстановления параметра и не превышено количество АПВ
            	{
            		sPnt->flag.ApvLatch = 1;                                      //фиксируем факт ожидания восстановления параметра
            		globsPnt->APVstatus.APV = 0;                                  //Пока запрос не формируем
            	}
            }
            else if ((sPnt->flag.ApvLatch) && !(globsPnt->APVstatus.APV))   //Fail = 0 - авария исчезла, дождались восстановления параметра, и проверяем был ли установлен флаг ожидания восстановления параметра (это будет в случае если  NormalWait != 0)
            {
            	globsPnt->APVstatus.APV = 1; //в локальный блок отправляем запрос на АПВ. если при этом время ожидания не восстановления параметра не прошло, то произ. АПВ. Иначе ничего не будет и запрос сбросится
            	sPnt->CntApv++;              //засчитываем факт осуществления запроса АПВ
            	APVpnt[sPnt->numBlock]  = TimeToAPV;
            }
        }
        else
        {
        	sPnt->flag.ApvLatch     = 0;
        	sPnt->TimeToAPV         = 0;
        	APVpnt[sPnt->numBlock]  = 0;
        }
    }
    else //Если АПВ глобально запрещено..
    {
        if(sPnt->flag.ApvLatch != 0) //и при этом было ожидание восстановления параметра
        {
            sPnt->flag.ApvLatch     = 0; //то сброс флага ожидания восстановления параметра
            sPnt->TimeToAPV         = 0; //сброс интервала выдержки перед АПВ
            APVpnt[sPnt->numBlock]  = 0; //сброс номера блока локального АПВ
            WrFramMicro(nvAdr, sPnt, sizeof(LOCAL_APVnvState_type)); //сохранение текущих параметров АПВ в FRAM
        }
    }
//====================================================================
//       Разбираемся с запросами на сброс счетчика АПВ
//====================================================================
    if(testBitMicro(ramPnt, progPnt->Pnt_ResetApv) == 1)
    {
        if(sPnt->flag.ResetState == 0)
        {
            sPnt->flag.ResetState   = 1;
            sPnt->CntApv            = 0;
            WrFramMicro(&nvAdr->CntApv, &sPnt->CntApv, sizeof(sPnt->CntApv));
        }
    }
    else {sPnt->flag.ResetState   = 0;}

//====================================================================
//                    Сохраняем состояние
//====================================================================
    save_s16(ramPnt, progPnt->Pnt_ApvCnt, sPnt->CntApv );
    defBitMicro(ramPnt, progPnt->Pnt_Warning,   Warning);
    defBitMicro(ramPnt, progPnt->Pnt_Fail,      Fail   );
//====================================================================
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLOCAL_APV_1[15]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Fail_IN
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Off/Block/Apv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  DeleyControl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  DeleyOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  NormalWait
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  ApvTime_sec
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  ApvTime_min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  KolvApv
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  ResetApv
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10 On/Off
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11 RTC
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12 VybegTime
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13 ApvCnt
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14 Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //15 Fail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LOCAL_APV", "DA", TblFuncLOCAL_APV_1, FuncLOCAL_APV_1, sizeof(TblFuncLOCAL_APV_1),sizeof(LOCAL_APVState_type), sizeof(LOCAL_APVnvState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLOCAL_APV_1,
#endif
//********************************************************
/* Мусорка
 globsPnt->APVstatus.APV ==0
//=======================

    if( (load_s16(ramPnt, progPnt->Pnt_NormalWait) == 1) && sPnt->flag.ApvLatch == 1 ){      // Ожидаем нормализацию
        if( sPnt->flag.secFlag == 1){
            sPnt->flag.secFlag = 0;
            sPnt->TimeToAPV++;                              // Передвинуть время включения
            WrFramMicro(&nvAdr->TimeToAPV, &sPnt->TimeToAPV, sizeof(nvAdr->TimeToAPV));
        }
    }
    else{                                                   // запускаем процедуру
        if(load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv) == 2){
            if (!sPnt->flag.ApvLatch && testBitMicro(ramPnt, progPnt->Pnt_Fail)){
                ////sPnt->flag.FailState = 1;
                tmp = load_s16(ramPnt, progPnt->Pnt_KolvApv);
                if ((tmp > sPnt->CntApv || tmp == 0)&&(sPnt->flag.Stop == 0)){        //превысили счетчи || можно запускаться без конца
                    sPnt->flag.ApvLatch = 1;
                    TimeToAPV = load_s16(ramPnt, progPnt->Pnt_ApvTime_min)*60 + load_s16(ramPnt, progPnt->Pnt_ApvTime_sec);
                    sPnt->TimeToAPV = TimeToAPV + load_s32(ramPnt, progPnt->Pnt_RTC);   // Время для глобального блока АПВ
                    sPnt->CntApv++;
                    WrFramMicro(&nvAdr, &sPnt, sizeof(LOCAL_APVnvState_type));  // Новый счетчик и новое время
                }
            }
        }
    }
//=======================
*/





