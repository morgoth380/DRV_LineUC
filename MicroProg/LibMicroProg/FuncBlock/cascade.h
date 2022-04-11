#ifdef    GET_FUNC_CODE
//********************************************************
typedef struct{
        u16         wait;
        u16         waitTime;
        u16         startDopMot;
        u16         start;
        u16         startPID;
        u16         waitWithFC;
        u16         waitWithoutFC;
        u16         systemOverload;
        u16         systemEdle;
}CascadeState_type;
//********************************************************

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Ust;                          //1  Ust
    u16  Pnt_Err_Pid;                      //2  Err_Pid
    u16  Pnt_maxPID;                       //3  maxPID
    u16  Pnt_minPID;                       //4  minPID
    u16  Pnt_Dopusk;                       //5  Dopusk
    u16  Pnt_T_Dop;                        //6  T_Dop
    u16  Pnt_T_Otk;                        //7  T_Otk
    u16  Pnt_T_PID;                        //8  T_PID
    u16  Pnt_CMH_1;                        //9  CMH_1
    u16  Pnt_CMH_2;                        //10  CMH_2
    u16  Pnt_CMH_3;                        //11  CMH_3
    u16  Pnt_CMH_4;                        //12  CMH_4
    u16  Pnt_Stat_0;                       //13  Stat_0
    u16  Pnt_Stat_1;                       //14  Stat_1
    u16  Pnt_Stat_2;                       //15  Stat_2
    u16  Pnt_Stat_3;                       //16  Stat_3
    u16  Pnt_Stat_4;                       //17  Stat_4
    u16  Pnt_EN;                           //18  EN
    u16  Pnt_Work;                         //19  Work
    u16  Pnt_FC_ON;                        //20  FC_ON
    u16  Pnt_Fref_OK;                      //21  Fref_OK
    u16  Pnt_Swtch_DN;                     //22  Swtch_DN
    u16  Pnt_Swtch_UP;                     //23  Swtch_UP
    u16  Pnt_Mot_4;                        //24  Mot_4
    u16  Pnt_Mot_3;                        //25  Mot_3
    u16  Pnt_Mot_2;                        //26  Mot_2
    u16  Pnt_Mot_1;                        //27  Mot_1
    u16  Pnt_Out;                          //28  Out
    u16  Pnt_End;
}FuncCASCADE_type;

void switchMot(u16 *start, FuncCASCADE_type *progPnt, u32 ramPnt, CascadeState_type *sPnt);

void selectByTime(s32 *timing, s16 *prior);

void work(FuncCASCADE_type *progPnt, u32 ramPnt, CascadeState_type *sPnt);

//********************************************************
u16 *FuncCASCADE_1(FuncCASCADE_type *progPnt, u32 ramPnt)
{
    CascadeState_type    *sPnt;
    s16                   FC_ON;

    sPnt = (CascadeState_type *)(ramPnt + progPnt->Pnt_State);
    FC_ON = testBitMicro(ramPnt, progPnt->Pnt_FC_ON);
    if (FC_ON == 0){
    	sPnt->systemOverload = 0;
    }
    if (flagIsFirstCall){                   //первичная инициализация структуры
        sPnt->start = 0;
        sPnt->startDopMot = 0;
        sPnt->startPID = 0;
        sPnt->wait = 0;
        sPnt->waitTime = 0;
        sPnt->waitWithFC = load_s16(ramPnt, progPnt->Pnt_T_Dop)*250*60;         // Перевод в минуты
        sPnt->waitWithoutFC = load_s16(ramPnt, progPnt->Pnt_T_Otk)*250*60;
    }
    //Проверка не стал ли на обслуживание или в аварию дополнительный мотор
    if (load_s16(ramPnt, progPnt->Pnt_Stat_1) > 0)
        defBitMicro(ramPnt, progPnt->Pnt_Mot_1, 0);
    if (load_s16(ramPnt, progPnt->Pnt_Stat_2) > 0)
        defBitMicro(ramPnt, progPnt->Pnt_Mot_2, 0);
    if (load_s16(ramPnt, progPnt->Pnt_Stat_3) > 0)
        defBitMicro(ramPnt, progPnt->Pnt_Mot_3, 0);
    if (load_s16(ramPnt, progPnt->Pnt_Stat_4) > 0)
        defBitMicro(ramPnt, progPnt->Pnt_Mot_4, 0);
    //Проверка не стал ли на обслуживание или в аварию основной мотор
    if (load_s16(ramPnt, progPnt->Pnt_Stat_0) > 0)
        save_s32(ramPnt, progPnt->Pnt_Out, 0);

    if (testBitMicro(ramPnt, progPnt->Pnt_Mot_1) == 0 && testBitMicro(ramPnt, progPnt->Pnt_Mot_2) == 0
            && testBitMicro(ramPnt, progPnt->Pnt_Mot_3) == 0 && testBitMicro(ramPnt, progPnt->Pnt_Mot_4) == 0){
        sPnt->systemEdle = 1;
    }else{
        if (((testBitMicro(ramPnt, progPnt->Pnt_Mot_1) == 1) || (load_s16(ramPnt, progPnt->Pnt_Stat_1) > 0))
                && ((testBitMicro(ramPnt, progPnt->Pnt_Mot_2) == 1) || (load_s16(ramPnt, progPnt->Pnt_Stat_2) > 0))
                && ((testBitMicro(ramPnt, progPnt->Pnt_Mot_3) == 1) || (load_s16(ramPnt, progPnt->Pnt_Stat_3) > 0))
                && ((testBitMicro(ramPnt, progPnt->Pnt_Mot_4) == 1) || (load_s16(ramPnt, progPnt->Pnt_Stat_4) > 0))){
            sPnt->systemOverload = 1;
        }else{
            sPnt->systemEdle = 0;
            sPnt->systemOverload = 0;
        }
    }

    if (sPnt->wait > 0){                    // проверка не ожидаем ли мы выхода на режим от генератора темпа
        sPnt->startDopMot = 1;         //если ожидаем - приготовиться запускать или останавливать двигатель
        if (FC_ON > 0 && load_s16(ramPnt, progPnt->Pnt_Stat_0) == 0)
            save_s32(ramPnt, progPnt->Pnt_Out, load_s32(ramPnt, progPnt->Pnt_Ust));
        // Если пришел сигнал что генератор тема вышел на режим перестаем ждать
        if (!testBitMicro(ramPnt, progPnt->Pnt_Fref_OK)){
            sPnt->wait--;
//            if (sPnt->start == 0)
//                defBitMicro(ramPnt, progPnt->Pnt_Swtch_DN, 1);
//            else
//                defBitMicro(ramPnt, progPnt->Pnt_Swtch_UP, 1);
        }
    }else{                                  //если не ожидаем 1. Проверяем не надо ли переключать моторы
        if (sPnt->startDopMot == 1)
            if (sPnt->start == 1){
                //выбрать и стартануть мотор, установить ожидание по времени для включения PID
                sPnt->startDopMot = 0;
                switchMot(&sPnt->start, progPnt, ramPnt, sPnt);
                sPnt->waitTime = load_s16(ramPnt, progPnt->Pnt_T_PID)*250;
                sPnt->startPID = 1;
            }else{
                //выбрать и остановить мотор, установить ожидание по времени для включения PID
                sPnt->startDopMot = 0;
                switchMot(&sPnt->start, progPnt, ramPnt, sPnt);
                sPnt->waitTime = load_s16(ramPnt, progPnt->Pnt_T_PID)*250;
                sPnt->startPID = 1;
            }
        //если не ожидаем и переключать не надо 2. Проверяем не надо ли переключать PID
        if (sPnt->waitTime > 0){
            sPnt->startPID = 1;        //если ожидаем - приготовится переключаться на PID
            sPnt->waitTime--;               //waitTime есть количество 4 мс циклов в заданое время
            if (FC_ON > 0 && load_s16(ramPnt, progPnt->Pnt_Stat_0) == 0)
                save_s32(ramPnt, progPnt->Pnt_Out, load_s32(ramPnt, progPnt->Pnt_Ust));
        }else{
            if (sPnt->startPID == 1){           //если не ожидаем - проверить не надо ли включать PID
                defBitMicro(ramPnt, progPnt->Pnt_Swtch_DN, 0);  //если что - переключаемся на задание от PID-а
                defBitMicro(ramPnt, progPnt->Pnt_Swtch_UP, 0);
                sPnt->startPID = 0;
            }else{
                //работа нормальная
                work(progPnt, ramPnt, sPnt);
            }
        }
    }

    return &progPnt->Pnt_End;
};
//Выбор мотора из свободных с наименьшим количеством часов наработки
void switchMot(u16 *start, FuncCASCADE_type *progPnt, u32 ramPnt, CascadeState_type *sPnt){

    s32        timing[4];
    s16        Stat[4], MotEn[4], prior[4], i, num = 0, on = 0;
    u16        MotAddr[4];

    timing[0] = load_s32(ramPnt, progPnt->Pnt_CMH_1);
    timing[1] = load_s32(ramPnt, progPnt->Pnt_CMH_2);
    timing[2] = load_s32(ramPnt, progPnt->Pnt_CMH_3);
    timing[3] = load_s32(ramPnt, progPnt->Pnt_CMH_4);
    Stat[0] = load_s16(ramPnt, progPnt->Pnt_Stat_1);
    Stat[1] = load_s16(ramPnt, progPnt->Pnt_Stat_2);
    Stat[2] = load_s16(ramPnt, progPnt->Pnt_Stat_3);
    Stat[3] = load_s16(ramPnt, progPnt->Pnt_Stat_4);
    MotEn[0] = testBitMicro(ramPnt, progPnt->Pnt_Mot_1);
    MotEn[1] = testBitMicro(ramPnt, progPnt->Pnt_Mot_2);
    MotEn[2] = testBitMicro(ramPnt, progPnt->Pnt_Mot_3);
    MotEn[3] = testBitMicro(ramPnt, progPnt->Pnt_Mot_4);
    MotAddr[0] = progPnt->Pnt_Mot_1;
    MotAddr[1] = progPnt->Pnt_Mot_2;
    MotAddr[2] = progPnt->Pnt_Mot_3;
    MotAddr[3] = progPnt->Pnt_Mot_4;

    selectByTime(timing, prior);

    while ((num < 4) && (on == 0)){
        i = prior[num] - 1;
            if (*start > 0){
                if (Stat[i] == 0)
                    if (MotEn[i] == 0){
                        defBitMicro(ramPnt, MotAddr[i], 1);
                        on = 1;
                    }else i = prior[++num];
                else i = prior[++num];
            }else{
                if (Stat[3 - i] == 0)
                    if (MotEn[3 - i] == 1){
                        defBitMicro(ramPnt, MotAddr[3 - i], 0);
                        on = 1;
                    }else i = prior[++num];
                else i = prior[++num];
            }
    }

}

//Формирование массива приоритетов
void selectByTime(s32 *timing, s16 *prior){


    if (timing[0] < timing[1])
        if (timing[2] < timing[3])
            if (timing[1] < timing[3]){
                prior[0] = 1;
                prior[1] = 3;
                if (timing[1] < timing[3]){
                    prior[2] = 2;
                    prior[3] = 4;
                }else{
                    prior[2] = 4;
                    prior[3] = 2;
                }
            }else{
                prior[0] = 3;
                prior[1] = 1;
                if (timing[1] < timing[3]){
                    prior[2] = 2;
                    prior[3] = 4;
                }else{
                    prior[2] = 4;
                    prior[3] = 2;
                }
            }
        else
            if (timing[0] < timing[3]){
               prior[0] = 1;
               prior[1] = 4;
               if (timing[1] < timing[2]){
                    prior[2] = 2;
                    prior[3] = 3;
                }else{
                    prior[2] = 3;
                    prior[3] = 2;
                }
            }else{
                prior[0] = 4;
                prior[1] = 1;
                if (timing[1] < timing[2]){
                    prior[2] = 2;
                    prior[3] = 3;
                }else{
                    prior[2] = 3;
                    prior[3] = 2;
                }
            }
    else
        if (timing[2] < timing[3])
            if (timing[1] < timing[2]){
                prior[0] = 2;
                prior[1] = 3;
                if (timing[0] < timing[3]){
                    prior[2] = 1;
                    prior[3] = 4;
                }else{
                    prior[2] = 4;
                    prior[3] = 1;
                }
            }else{
                prior[0] = 3;
                prior[1] = 2;
                if (timing[0] < timing[3]){
                    prior[2] = 1;
                    prior[3] = 4;
                }else{
                    prior[2] = 4;
                    prior[3] = 1;
                }
            }
        else
            if (timing[1] < timing[3]){
                prior[0] = 2;
                prior[1] = 4;
                if (timing[0] < timing[2]){
                    prior[2] = 1;
                    prior[3] = 3;
                }else{
                    prior[2] = 3;
                    prior[3] = 1;
                }
            }else{
                prior[0] = 4;
                prior[1] = 2;
                if (timing[0] < timing[2]){
                    prior[2] = 1;
                    prior[3] = 3;
                }else{
                    prior[2] = 3;
                    prior[3] = 1;
                }
            }
}

//Обработка необходимости переключения моторов
void work(FuncCASCADE_type *progPnt, u32 ramPnt, CascadeState_type *sPnt){

    s32        in, max, min, delta, err;
    s16        razresh, WorkFlag, FC_ON;

    WorkFlag = testBitMicro(ramPnt, progPnt->Pnt_Work);
    razresh = load_s16(ramPnt, progPnt->Pnt_EN);
    FC_ON = testBitMicro(ramPnt, progPnt->Pnt_FC_ON);
    min = load_s32(ramPnt, progPnt->Pnt_minPID);
    max = load_s32(ramPnt, progPnt->Pnt_maxPID);
    // Проверяем разрешена ли работа
    if (WorkFlag > 0){
        // если разрешена работа - проверяем разрешен ли каскад
        if (razresh > 0){
            //если разрешен каскад - проверяем работает ли частотник
            if (FC_ON > 0 && load_s16(ramPnt, progPnt->Pnt_Stat_0) == 0){
                // Если частотник работает - осуществляем регулирование с ПЧ
                in = load_s32(ramPnt, progPnt->Pnt_Ust);
                err = load_s32(ramPnt, progPnt->Pnt_Err_Pid);
                delta = load_s32(ramPnt, progPnt->Pnt_Dopusk);
                save_s32(ramPnt, progPnt->Pnt_Out, in);
                if (in <= (min + delta) && err < -delta){
                    //Если частота опустилась до минимальной + допуск - начинаем отсчет времени на переключение
                    if (sPnt->waitWithFC > 0){
                        if (!testBitMicro(ramPnt, progPnt->Pnt_Fref_OK))
                            sPnt->waitWithFC--;
                    }else{
                        if (sPnt->systemEdle == 0){
                            sPnt->wait = 10;
                            sPnt->start = 0;
                            sPnt->waitWithFC = load_s16(ramPnt, progPnt->Pnt_T_Dop)*250*60;
                            defBitMicro(ramPnt, progPnt->Pnt_Swtch_DN, 1);
                        }
                    }
                }else{
                    //Если частота не зашла за нижний порог проверяем не превысила ли она верхний
                    if (in >= (max - delta) && err > delta){
                        //Если частота превысила пороговое значение - начинаем отсчет времени на переключение
                    if (sPnt->waitWithFC > 0){
                        if (!testBitMicro(ramPnt, progPnt->Pnt_Fref_OK))
                            sPnt->waitWithFC--;
                    }else{
                        if (sPnt->systemOverload == 0){
                            sPnt->wait = 10;
                            sPnt->start = 1;
                            sPnt->waitWithFC = load_s16(ramPnt, progPnt->Pnt_T_Dop)*250*60;
                            defBitMicro(ramPnt, progPnt->Pnt_Swtch_UP, 1);
                        }
                    }
                    }else{
                        //Если не превысила - оставляем прежнее состояние системы
                        //На выход подаем частоту, состояние доп. моторов не меняем
                        save_s32(ramPnt, progPnt->Pnt_Out, load_s32(ramPnt, progPnt->Pnt_Ust));
                        sPnt->waitWithFC = load_s16(ramPnt, progPnt->Pnt_T_Dop)*250*60;
                    }
                }

            }else{
                //Если частотник НЕ работает - проверяем можнно ли работать без него
                if (razresh == 2){
                    //Если можно работать без частотника - осуществляем регулирование без ПЧ
                in = load_s32(ramPnt, progPnt->Pnt_Ust);
                err = load_s32(ramPnt, progPnt->Pnt_Err_Pid);
                delta = load_s32(ramPnt, progPnt->Pnt_Dopusk);
                save_s32(ramPnt, progPnt->Pnt_Out, 0);
                if (in <= (min + delta) && err < -delta){
                    //Если частота опустилась до минимальной + допуск - начинаем отсчет времени на переключение
                    if (sPnt->waitWithoutFC > 0){
                        sPnt->waitWithoutFC--;
                    }else{
                        if (sPnt->systemEdle == 0){
                            sPnt->wait = 10;
                            sPnt->start = 0;
                            sPnt->waitWithoutFC = load_s16(ramPnt, progPnt->Pnt_T_Dop)*250*60;
//                            defBitMicro(ramPnt, progPnt->Pnt_Swtch_DN, 1);
                        }
                    }
                }else{
                    //Если частота не зашла за нижний порог проверяем не превысила ли она верхний
                    if (in >= (max - delta)  && err > delta){
                        //Если частота превысила пороговое значение - начинаем отсчет времени на переключение
                    if (sPnt->waitWithoutFC > 0){
                        sPnt->waitWithoutFC--;
                    }else{
                        if (sPnt->systemOverload == 0){
                        sPnt->wait = 10;
                        sPnt->start = 1;
                        sPnt->waitWithoutFC = load_s16(ramPnt, progPnt->Pnt_T_Dop)*250*60;
//                        defBitMicro(ramPnt, progPnt->Pnt_Swtch_UP, 1);
                        }
                    }
                    }else{
                        //Если не превысила - оставляем прежнее состояние системы
                        //На выход подаем частоту, состояние доп. моторов не меняем
                        save_s32(ramPnt, progPnt->Pnt_Out, 0);
                        sPnt->waitWithoutFC = load_s16(ramPnt, progPnt->Pnt_T_Dop)*250*60;
                    }
                }
                }else{
                    //Если нельзя работать без частотника - замораживаем систему
                    //На двигатель подам нулевую частоту
                    save_s32(ramPnt, progPnt->Pnt_Out, 0);
                    //состояния дополнительных моторов не меняем
                }
            }
        }else{
            // Если запрещена работа каскада - пропускаем управление с ПИДа на мотор и стопим все остальные
            save_s32(ramPnt, progPnt->Pnt_Out, load_s32(ramPnt, progPnt->Pnt_Ust));
            defBitMicro(ramPnt, progPnt->Pnt_Mot_1, 0);
            defBitMicro(ramPnt, progPnt->Pnt_Mot_2, 0);
            defBitMicro(ramPnt, progPnt->Pnt_Mot_3, 0);
            defBitMicro(ramPnt, progPnt->Pnt_Mot_4, 0);

        }
        // Если запрещена работа - стопим все!
    }else{
        defBitMicro(ramPnt, progPnt->Pnt_Swtch_DN, 0);
        defBitMicro(ramPnt, progPnt->Pnt_Swtch_UP, 0);
        save_s32(ramPnt, progPnt->Pnt_Out, 0);
        defBitMicro(ramPnt, progPnt->Pnt_Mot_1, 0);
        defBitMicro(ramPnt, progPnt->Pnt_Mot_2, 0);
        defBitMicro(ramPnt, progPnt->Pnt_Mot_3, 0);
        defBitMicro(ramPnt, progPnt->Pnt_Mot_4, 0);
    }
}

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCASCADE_1[28]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ust
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Err_Pid
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  maxPID
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  minPID
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Dopusk
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  T_Dop
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  T_Otk
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  T_PID
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  CMH_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  CMH_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  CMH_3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  CMH_4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Stat_0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Stat_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Stat_2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Stat_3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Stat_4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  EN
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //19  Work
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //20  FC_ON
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //21  Fref_OK
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Swtch_DN
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  Swtch_UP
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  Mot_4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  Mot_3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  Mot_2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //27  Mot_1
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //28  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CASCADE", "DA", TblFuncCASCADE_1, FuncCASCADE_1, sizeof(TblFuncCASCADE_1), sizeof(CascadeState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCASCADE_1,
#endif
//********************************************************
