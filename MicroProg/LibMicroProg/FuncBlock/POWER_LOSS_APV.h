#ifdef    GET_FUNC_CODE
#include    "apv_type.h"
#define    THIRD_CALL  3 //третий проход программы
#define    RUN         1
#define    POWER_LOSS_FAULT     1

typedef struct{ 
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Off_Block_APV;                //1  Off/Block/APV
    u16  Pnt_APVTime_sec;                  //2  APVTime_sec
    u16  Pnt_APVTime_min;                  //3  APVTime_min
    u16  Pnt_KolvAPV;                      //4  KolvAPV
    u16  Pnt_On_Off;                       //5  On/Off APV
    u16  Pnt_RTC;                          //6  RTC
    u16  Pnt_APVUsCount;                   //7  APVUsCount
    u16  Pnt_VybegTime;                    //8  VybegTime
    u16  Pnt_PWM_ON;                       //9  PWM_ON
    u16  Pnt_WorkRunMode;                  //10  WorkRunMode
    u16  Pnt_NormStopMode;                 //11  NormStopMode
    u16  Pnt_End;
}FuncPOWER_LOSS_APV_type;

typedef enum {
   POWER_OFF = 0,
   POWER_FAULT = 1,
   POWER_APV = 2,
} POWER_LOSS_Enum;

//********************************************************
u16 *FuncPOWER_LOSS_APV_1(FuncPOWER_LOSS_APV_type *progPnt, u32 ramPnt)
{
    u32              CurrentTime, VybegTime;
    u16              MaxNumOfAPV, PowerApvCount, EngineRun;
    void             *nvAdr1; //указатель на флаг исчезновения питания PowerLossFlag
    void             *nvAdr2; //указатель на время аварии по питанию
    void             *nvAdr3; //указатель на время аварии НЕ по питанию, то есть это время останова, инициированного программой
    APVState_type  *globsPnt; //структура для обмена данными с модулями LOCAL_APV2 и POWER_LOSS_APV
    POWER_LOSS_APVStateType *sPnt;
    POWER_LOSS_Enum   Reaction;

    sPnt   = (POWER_LOSS_APVStateType *)(ramPnt + progPnt->Pnt_State);
    nvAdr1 = (void *)((u32)&nv.EngineRunFlag);                       //вычисляем адрес флага аварии по питанию
    nvAdr2 = (void *)((u32)&nv.PowerFailTime);                       //вычисляем адрес переменной с временем аварии по питанию
    nvAdr3 = (void *)((u32)&nv.stopTime)     ;                       //вычисляем адрес переменной с временем программного останова по аварии

    globsPnt    = &APVState; //указатель на обменную структуру
    
    // Проверка включенного АПВ - если да, ставим режим аварии
    if (load_s16(ramPnt, progPnt->Pnt_On_Off)) {
        save_s16(ramPnt, progPnt->Pnt_Off_Block_APV, (u16)POWER_FAULT);
        Reaction = POWER_APV;
    } else {
        // Читаем режим работы
        Reaction = (POWER_LOSS_Enum)load_s16(ramPnt, progPnt->Pnt_Off_Block_APV); //считываем состояние сигнала управления по напряжению - Откл или авария      
    }
    
    if(flagIsFirstCall)                                              //если это самый первый проход программы (питание только подано)
    {
       RdFramMicro( &(sPnt->PowerLossFlag), nvAdr1, sizeof(u16));    //считываем состояние флага работающего двигателя. Если было аварийное исчезновение питания, то будет установлен в 1
       if (!sPnt->PowerLossFlag)                                     //если не было аварии по питанию
       {
           sPnt->ProgFailTime = 0;                                   //сбрасываем время программного останова по аварии
           WrFramMicro(nvAdr3, &(sPnt->ProgFailTime),  sizeof(u32)); //Как-бы начальный сброс времени программного аварийного останова в 0 (исходное значение при отсутствии аварии)
           globFailTime = 0;                                         //в apv1.h ничего не передавать не будем
       }
       else //иначе была авария отключения питания. Считываем время программного останова и последнее записанное время работы двигателя
       {
           RdFramMicro(&(sPnt->PowerFailTime), nvAdr2, sizeof(u32)); //считываем время аварийного останова по питанию. если было полное исчезновение питания, то будет содержать время исчезновения питания
           RdFramMicro(&(sPnt->ProgFailTime),  nvAdr3, sizeof(u32)); //считываем время аварийного останова, записываемое программно (если программа успеет обработать аварию по питанию)
       }
       return &progPnt->Pnt_End;
    }
    else if (sPnt->CountCall == THIRD_CALL) // ждем когда подготовятся все входные данные. Тело оператора выполнится только при третьем проходе программы
    {
        sPnt->CountCall++;        //чтобы тело выполнилось только 1 раз.  Задача блока определить было ли исчезновение питания во время работы двигателя, и дать запрос на АПВ.
        if (!sPnt->PowerLossFlag) { //если при первом вызове было выявлено, что не было аварии питания
          return &progPnt->Pnt_End; //то работа данного блока завершается
        }

        //Off_Block_Apv = load_s16(ramPnt, progPnt->Pnt_Off_Block_APV);   //считываем состояние сигнала управления блоком АПВ по напряжению.
        MaxNumOfAPV = load_s16(ramPnt, progPnt->Pnt_KolvAPV)        ;   //считываем максимальное разрешенное количество попыток АПВ по напряжению
        PowerApvCount = load_s16(ramPnt, progPnt->Pnt_APVUsCount)   ;   //считываем текущее количество попыток АПВ по питанию

        if ((globsPnt->APVstatus.bits.AutoMode != 0) && (Reaction == POWER_APV) && (MaxNumOfAPV >= PowerApvCount)) //если разрешен автоматический режим, разрешено АПВ по питанию, и не превышено макс. число АПВ
        {
            globFailTime = (sPnt->ProgFailTime)? (sPnt->ProgFailTime): (sPnt->PowerFailTime); //в apv1.h передаем время аварии (это или время, записанное программой в момент аварии, или время аварийного отключения питания)
            VybegTime = load_s16(ramPnt, progPnt->Pnt_VybegTime);       //считываем длительность инерционного вращения вала двигателя
            CurrentTime = load_s32(ramPnt, progPnt->Pnt_RTC)    ;       //текущее время
            //если время выбега больше, чем прошло времени с момента аварийного останова, то учтем это
            VybegTime = (VybegTime > (CurrentTime - globFailTime))? VybegTime - (CurrentTime - globFailTime): 0;
            sPnt->TimeToAPV = load_s16(ramPnt, progPnt->Pnt_APVTime_min)*60 + load_s16(ramPnt, progPnt->Pnt_APVTime_sec);
            sPnt->TimeToAPV += VybegTime;
            APVpnt[8] = sPnt->TimeToAPV;           //заносим данные в ячейку 8 где у нас авария по входной сети
            globsPnt->APVstatus.bits.APV = 1;           //передаем запрос на АПВ т.к. напряжение пропадало и АПВ разрешено
            globsPnt->APVstatus.bits.PowerLossAPV = 1;  //передаем в локальный блок АПВ контроля напряжения признак осуществленного запроса
            //Теперь после обработки запроса на АПВ все данные аварии должны быть сброшены в блоке apv1.h
        }
        else //иначе аварии по питанию или вообще не было, или АПВ запрещено (глобально или именно по питанию). Сбрасываем признаки аварии.
        {
            globsPnt->APVstatus.bits.APV = 0;                             //Запрос на АПВ по восстановлению питания не даем
            globFailTime =  sPnt->PowerFailTime = sPnt->ProgFailTime = 0; //Сброс данных, связанных с аварией
			sPnt->PowerLossFlag =0;
            WrFramMicro(nvAdr1, &sPnt->PowerLossFlag, sizeof(u16));  //сброс флага работы двигателя (если авария вообще была). Также флаг сбрасывается в apv1.h после истечения времени ожидания запроса или после его реализации
            WrFramMicro(nvAdr2, &sPnt->PowerFailTime, sizeof(u32));  //сброс времени пропадания питания (если авария вообще была)
            WrFramMicro(nvAdr3, &sPnt->ProgFailTime,  sizeof(u32));  //сброс времени аварийного программного останова
        }
    }
    else if (sPnt->CountCall < THIRD_CALL)                            //ждем готовности входных данных для данного функ. блока
    {
        (sPnt->CountCall)++;                                          //тикнем один раз, а при следующем проходе программы данный функциональный блок сможет обработать корректные входные данные
    }

    if (testBitMicro(ramPnt, progPnt->Pnt_WorkRunMode) && (globsPnt->APVstatus.bits.AutoMode != 0) && (Reaction == POWER_APV)) //если в модуле DRV сформирована команда ПУСК двигателя в РАБОЧИЙ режим
    {
        if (globsPnt->APVstatus.bits.SecondFlag == 1)
        {
            EngineRun = 1;
            WrFramMicro(nvAdr1, &EngineRun, sizeof(u16));      //Сохраняем признак включенного в рабочий режим двигателя. Флаг сбросится при обработке запроса на АПВ или при любой из команд СТОП
            CurrentTime = load_s32(ramPnt, progPnt->Pnt_RTC);  //записываем тек. время
            WrFramMicro(nvAdr2, &CurrentTime, sizeof(u32));    //сохраняем текущее время. При аварии исчезновения питания будет служить временем аварии
        }
    }

   /********************************************************************************************************************************************************************************************/

    return &progPnt->Pnt_End;

};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPOWER_LOSS_APV_1[11]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Off/Block/APV
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  APVTime_sec
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  APVTime_min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  KolvAPV
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  On/Off
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  RTC
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  APVUsCount
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  VybegTime
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  PWM_ON
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  WorkRunMode
        BIT_PIN_TYPE | INPUT_PIN_MODE      //11  NormStopMode
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("POWER_LOSS_APV", "DA", TblFuncPOWER_LOSS_APV_1, FuncPOWER_LOSS_APV_1, sizeof(TblFuncPOWER_LOSS_APV_1), sizeof(POWER_LOSS_APVStateType), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPOWER_LOSS_APV_1,
#endif
//********************************************************