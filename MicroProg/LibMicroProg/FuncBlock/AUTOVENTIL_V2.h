#ifdef    GET_FUNC_CODE

typedef union{
    struct{
        u16 Ena1        :1; //Бит разрешения использования дополнительного двигателя 1 (0 - запрещен, 1 - разрешен)
        u16 Ena2        :1; //Бит разрешения использования дополнительного двигателя 2 (0 - запрещен, 1 - разрешен)
        u16 Ena3        :1; //Бит разрешения использования дополнительного двигателя 3 (0 - запрещен, 1 - разрешен)
        u16 Ena4        :1; //Бит разрешения использования дополнительного двигателя 4 (0 - запрещен, 1 - разрешен)
        u16 ON1         :1; //Бит, отображающий работу дополнительного двигателя 1 (0 - не работает, 1 - работает)
        u16 ON2         :1; //Бит, отображающий работу дополнительного двигателя 2 (0 - не работает, 1 - работает)
        u16 ON3         :1; //Бит, отображающий работу дополнительного двигателя 3 (0 - не работает, 1 - работает)
        u16 ON4         :1; //Бит, отображающий работу дополнительного двигателя 4 (0 - не работает, 1 - работает)
        u16 M1_AC       :1; //Бит управления, отвечающий за переключение дополнительного двигателя 1 на работу от сети (0 - отключен от сети, 1 - подключен к сети переменного тока)
        u16 M2_AC       :1; //Бит управления, отвечающий за переключение дополнительного двигателя 2 на работу от сети (0 - отключен от сети, 1 - подключен к сети переменного тока)
        u16 M3_AC       :1; //Бит управления, отвечающий за переключение дополнительного двигателя 3 на работу от сети (0 - отключен от сети, 1 - подключен к сети переменного тока)
        u16 M4_AC       :1; //Бит управления, отвечающий за переключение дополнительного двигателя 4 на работу от сети (0 - отключен от сети, 1 - подключен к сети переменного тока)
        u16 M1_FC       :1; //Бит управления, отвечающий за переключение дополнительного двигателя 1 на работу от ПЧ (0 - отключен от ПЧ, 1 - подключен к ПЧ)
        u16 M2_FC       :1; //Бит управления, отвечающий за переключение дополнительного двигателя 2 на работу от ПЧ (0 - отключен от ПЧ, 1 - подключен к ПЧ)
        u16 M3_FC       :1; //Бит управления, отвечающий за переключение дополнительного двигателя 3 на работу от ПЧ (0 - отключен от ПЧ, 1 - подключен к ПЧ)
        u16 M4_FC       :1; //Бит управления, отвечающий за переключение дополнительного двигателя 4 на работу от ПЧ (0 - отключен от ПЧ, 1 - подключен к ПЧ)
        u16 EnMH1       :1; //Бит разрешения/запрещения счета моточасов двигателя 1 (0 - запрещен подсчет моточасов, 1 - разрешен подсчет моточасов)
        u16 EnMH2       :1; //Бит разрешения/запрещения счета моточасов двигателя 2 (0 - запрещен подсчет моточасов, 1 - разрешен подсчет моточасов)
        u16 EnMH3       :1; //Бит разрешения/запрещения счета моточасов двигателя 3 (0 - запрещен подсчет моточасов, 1 - разрешен подсчет моточасов)
        u16 EnMH4       :1; //Бит разрешения/запрещения счета моточасов двигателя 4 (0 - запрещен подсчет моточасов, 1 - разрешен подсчет моточасов)
        u16 firstStart  :1; //Признак первого запуска системы автовентиль
        u16 conRotMot   :4; //флаги ротации двигателя 1...4 (0 - запрещена, 1 - разрешена)
        u16 fail        :1; //Признак аварийного состояни системы автовентиль
        u16 prior       :1; //Приоритет подключения дополнительного двигателя (0 - по моточасам, 1 - по приоритету пользователя)
        u16 connectNewFC:1; //Бит - флаг подключения дополнительного двигателя к ПЧ (0 - не подключать, 1 - подключить)
        u16 SleepEn     :1; //Бит - флаг разрешения режима СОН.
        u16 rezerv2     :1;
        u16 rezerv3     :1;
        u16 rezerv4     :1;
    };
    u32 all;
}Mot_ENA;

typedef enum{
    NotUsed    = 0,   //!< Не используется ни один из дополнительных двигателей
	FInUse     = 1,   //!< Подключен первый дополнительный двигатель
	SInUse     = 2,   //!< Подключен второй дополнительный двигатель
	FSInUse    = 3,   //!< Подключены первый и второй дополнительные двигатели
	TInUse     = 4,   //!< Подключен третий дополнительный двигатель
	FTInUse    = 5,   //!< Подключены первый и третий дополнительные двигатели
	STInUse    = 6,   //!< Подключены второй и третий дополнительные двигатели
	FSTInUse   = 7,   //!< Подключены первый, второй и третий дополнительные двигатели
	FoInUse    = 8,   //!< Подключен четвертый дополнительный двигатель
	FFoInUse   = 9,   //!< Подключены первый и четвертый дополнительные двигатели
	SFoInUse   = 10,  //!< Подключены второй и четвертый дополнительные двигатели
	FDFoInUse  = 11,  //!< Подключены первый, второй и четвертый дополнительные двигатели
	TFoInUse   = 12,  //!< Подключены третий и четвертый дополнительные двигатели
	FTFoInUse  = 13,  //!< Подключены первый, третий и четвертый дополнительные двигатели
	STFoInUse  = 14,  //!< Подключены второй, третий и четвертый дополнительные двигатели
	AllInUse   = 15   //!< Подключены все четыре дополнительные двигатели
}UseMot_type;

typedef enum{
    NONeedRot   = 0,   //!< Ни у одного из двигателе время простоя не превышает времени ротации
	FNeedRot    = 1,   //!< У первого двигателя время простоя превышает время ротации
	SNeedRot    = 2,   //!< У второго двигателя время простоя превышает время ротации
	FSNeedRot   = 3,   //!< У первого и второго двигателей время простоя превышает время ротации
	TNeedRot    = 4,   //!< У третьего двигателя время простоя превышает время ротации
	FTNeedRot   = 5,   //!< У первого и третьего двигателей время простоя превышает время ротации
	STNeedRot   = 6,   //!< У второго и третьего двигателей время простоя превышает время ротации
	FSTNeedRot  = 7,   //!< У первого, второго и третьего двигателей время простоя превышает время ротации
	FoNeedRot   = 8,   //!< У четвертого двигателя время простоя превышает время ротации
	FFoNeedRot  = 9,   //!< У первого и четвертого двигателей время простоя превышает время ротации
	SFoNeedRot  = 10,  //!< У второго и четвертого двигателей время простоя превышает время ротации
	FSFoNeedRot = 11,  //!< У первого, второго и четвертого двигателей время простоя превышает время ротации
	TFoNeedRot  = 12,  //!< У третьего и четвертого двигателей время простоя превышает время ротации
	FTFoNeedRot = 13,  //!< У первого, третьего и четвертого двигателей время простоя превышает время ротации
	STFoNeedRot = 14,  //!< У второго, третьего и четвертого двигателей время простоя превышает время ротации
	AllNeedRot  = 15   //!< У всех 4х двигателей время простоя больше времени ротации
}RotMot_type;

typedef enum{
    NotReady = 0,     //!< Ни один из двигателей не готов к подключению
	FReady   = 1,     //!< Двигатель номер 1 готов к подключению
	SReady   = 2,     //!< Двигатель номер 2 готов к подключению
	TReady   = 3,     //!< Двигатель номер 3 готов к подключению
	FoReady  = 4      //!< Двигатель номер 4 готов к подключению
}ReadyMot_type;

typedef enum{
	PumpSel = 0,
	LiftSel
}FuncPumpLift_type;

typedef enum{
    SleepDisable = 0,
	SleepEnable
}AUTOVENTIL_V2_Sleep_type;

typedef struct{
    u16 MotPrior[NMOT];
    u32 CntStand[NMOT];
    u32 CntMoto[NMOT];
}nvAUTOVENTIL_V2State_type;

#define     nvSfAvtoventil     (*(nvAUTOVENTIL_V2State_type*)0)

typedef struct{
    u32                       wait;             //!< Время задержки на отключение/включение следующего мотора
    u32                       T_non_rot;        //!< Время задержки ротации
    u16                       T_Razm;           //!< Задержка на размагничивание в тиках микропрограммы
    u16                       razmagnich_now;   //!< Признак размагничивания (0 - в данный момент нет размагничивания, 1 - происходит размагничивание)
    u32                       MH[NMOT];            //!< Моточасы двигателей в минутах
    u16                       D_rdy_for;        //!< Номер двигателя готового для подключения
    u16                       systemOverload;   //!< Загрузка системы автовентиль (0 - есть еще ресурс, 1 - полностью загружена)
    u16                       systemEdle;       //!< Состояние системы автовентиля (0 - в работе, 1 - в простое)
    u16                       MPR[NMOT];           //!< Приоритет дополнительного мотора
    u16                       TmrMin;           //!< Таймер отсчета минутных импульсов
//    u32                       CntStand[NMOT];      //!< Счетчики отсчета простоя дополнительных двигателей в минутах
    nvAUTOVENTIL_V2State_type Prior;
    Mot_ENA                   Mot;
    u32                       prevWait;
}AUTOVENTIL_V2State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_APV_NW;                       //1  APV_NW                                 !< Авария от дискретного входа - нет воды
    u16  Pnt_WorkFlag;                     //2  WorkFlag
    u16  Pnt_Sel;                          //3  Sel
    u16  Pnt_Enable;                       //4  Enable
    u16  Pnt_Fref;                         //5  Fref
    u16  Pnt_Err_Pid;                      //6  Err_Pid
    u16  Pnt_maxPID;                       //7  maxPID
    u16  Pnt_minPID;                       //8  minPID
    u16  Pnt_Dopusk;                       //9  Dopusk
    u16  Pnt_T_Dop[4];                     //10 - 13  T_Dop
    u16  Pnt_T_Razm;                       //14  T_Razm                                !< Задержка на размагничивание
    u16  Pnt_T_rot;                        //15  T_rot
    u16  Pnt_Inom;                         //16  Inom
    u16  Pnt_InomDop[4];                   //17 - 20  Inom1 - Inom4                    !< Номинальный ток дополнительного двигателя
    u16  Pnt_Imax;                         //21  Imax                                  !< Максимальный ток двигателя
    u16  Pnt_ImaxDop[4];                   //22 - 25  Imax1 - Imax4                    !< Максимальный ток дополнительного двигателя
    u16  Pnt_EnMot;                        //26  EnMot                                 !< Разрешение работы дополнительного двигателя 1...4
    u16  Pnt_Prior;                        //27  Prior
    u16  Pnt_Ustpr[4];                     //28 - 31  Ustpr1 ... Ustpr4                !< Уставка - пользовательский приоритет дополнительного двигателя
    u16  Pnt_ResCnt[4];                    //32 - 35  ResCnt1 ... ResCnt4              !< Сброс счетчика моточасов
    u16  Pnt_SleepSel;                     //36  SleepSel
    u16  Pnt_StndSpeed;                    //37  StndSpeed
    u16  Pnt_TekFdrv;                      //38  TekFdrv
    u16  Pnt_GT;                           //39  GT                                    !< Бит сброса генератора темпа
    u16  Pnt_Fail;                         //40  Fail
    u16  Pnt_WaterFail;                    //41  WaterFail
    u16  Pnt_F_Out;                        //42  F_Out
    u16  Pnt_AVOn;                         //43  AVOn
    u16  Pnt_SleepAct;                     //44  SleepAct
    u16  Pnt_NetCtrM[4];                   //45 - 48  NetCtrM1 ... NetCtrM4            !< Бит управления реле - подключение/отключение двигателя от сети переменного тока
    u16  Pnt_FCCtrM[4];                    //49 - 52  FCCtrM1 ... FCCtrM4              !< Бит управления реле - подключение/отключение двигателя от ПЧ
    u16  Pnt_Imot;                         //53  Imot                                  !< Номинальный ток двигателя, подключенного к ПЧ
    u16  Pnt_ImaxMot;                      //54  ImaxMot                               !< Максимальный ток двигателя, подключенного к ПЧ
    u16  Pnt_StateDop[4];                  //55 - 58  State1 ... State4                !< Бит индикации разрешения использования дополнительного двигателя
    u16  Pnt_ActiveMot;                    //59  ActiveMot                             !< Номер мотора, подключенного к ПЧ
    u16  Pnt_CntMot[4];                    //60 - 63  CntMot1 ... CntMot4              !< Счетчики моточасов двигателей в часах
    u16  Pnt_CntStop[4];                   //64 - 67  CntStop1 ... CntStop4            !< Счетчики простоя двигателей в часах
    u16  Pnt_End;
}FuncAUTOVENTIL_V2_type;

void prior(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
void busywork(AUTOVENTIL_V2State_type *sPnt);
void connect_New_FC(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
void contactor_man(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
void internal_fail(AUTOVENTIL_V2State_type *sPnt);
u16 mot_disconnect_FC(AUTOVENTIL_V2State_type *sPnt);
void mot_connect_AC(u16 numb, AUTOVENTIL_V2State_type *sPnt);
void mot_state(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
void mot_increase(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
void motors_control(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
s16 mot_decrease_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
s16 mot_decrease_PR(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
void MotoStandHour(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt, nvAUTOVENTIL_V2State_type *nvAdr);
void prior_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
s16 mot_connect_FC_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
s16 mot_connect_FC_PR(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);
void rotatsnuti(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt);

//********************************************************
u16 *FuncAUTOVENTIL_V2_1(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt)
{
    AUTOVENTIL_V2State_type   *sPnt;
    nvAUTOVENTIL_V2State_type *nvAdr;
    u8                        i, EnMot, MotPr;
    s16                        tmp;
    u16  temp;
    sPnt = (AUTOVENTIL_V2State_type *)(ramPnt + progPnt->Pnt_State);
    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfAvtoventil);

    if(flagIsFirstCall){
    	tmp = 59 * load_s16(ramPnt, progPnt->Pnt_StndSpeed) + 1;;
        RdFramMicro(&sPnt->Prior, nvAdr, sizeof(nvAUTOVENTIL_V2State_type));
        for(i = 0; i < NMOT; i++){
            sPnt->MPR[i] = sPnt->Prior.MotPrior[i];
            save_s16(ramPnt, progPnt->Pnt_Ustpr[i], (s16)sPnt->MPR[i]);
            save_s32(ramPnt, progPnt->Pnt_CntMot[i], (s32)sPnt->Prior.CntMoto[i]);              //!< Сохраняем моточасы в для отображения в минутах
            save_s16(ramPnt, progPnt->Pnt_CntStop[i], (s16)(sPnt->Prior.CntStand[i]/tmp));      //!< Сохраняем время простоя для отображения в часах
        }

        sPnt->TmrMin = MINUTE_MICRO_TAKT;                                      //!< Установка таймера на отсчет 1 минуты
        sPnt->T_non_rot = K_TIME_CALL_MICRO_TAKT * 600;                        //!< 10 минут задержки для устранения ротации двух моторов подряд
        save_s32(ramPnt, progPnt->Pnt_Imot, load_s32(ramPnt, progPnt->Pnt_Inom));
        save_s16(ramPnt, progPnt->Pnt_ImaxMot, load_s16(ramPnt, progPnt->Pnt_Imax));
        save_s16(ramPnt, progPnt->Pnt_ActiveMot, 0);
        clrBitMicro(ramPnt, progPnt->Pnt_AVOn);
        sPnt->Mot.all = 0;        //Mot.MinTick = 0;
        sPnt->prevWait = 20 * K_TIME_CALL_MICRO_TAKT;
        return &progPnt->Pnt_End;
    }

    MotoStandHour(progPnt, ramPnt, sPnt, nvAdr);

    prior(progPnt, ramPnt, sPnt);                                              //!< Вычитывание и изменение приоритетов дополнительных двигателей

    if((sPnt->MPR[0] != sPnt->Prior.MotPrior[0]) ||
       (sPnt->MPR[1] != sPnt->Prior.MotPrior[1]) ||
       (sPnt->MPR[2] != sPnt->Prior.MotPrior[2]) ||
	   (sPnt->MPR[3] != sPnt->Prior.MotPrior[3])){

        for(i = 0; i < NMOT; i++){
    		sPnt->Prior.MotPrior[i] = sPnt->MPR[i];
    	}
      WrFramMicro(nvAdr, &sPnt->Prior.MotPrior, sizeof(sPnt->Prior.MotPrior));
    }

    prior_MH(progPnt, ramPnt, sPnt);
    sPnt->Mot.fail = 0;
     temp = load_s16(ramPnt, progPnt->Pnt_Enable);
    if (temp)
      {
        temp = sizeof(u16);
      }

    if(load_s16(ramPnt, progPnt->Pnt_Enable) && testBitMicro(ramPnt, progPnt->Pnt_WorkFlag) && testBitMicro(ramPnt, progPnt->Pnt_Sel)){                                                 //!< Проверяем разрешена ли работа

    	setBitMicro(ramPnt, progPnt->Pnt_AVOn);
        mot_state(progPnt, ramPnt, sPnt);
        busywork(sPnt);
        if(sPnt->razmagnich_now != 1){
            if(sPnt->Mot.conRotMot == 0){
                if(sPnt->T_non_rot == 0){
                	rotatsnuti(progPnt, ramPnt, sPnt);
                	sPnt->T_non_rot = K_TIME_CALL_MICRO_TAKT * 600;                            //!< 10 минут задержки для устранения ротации двух моторов подряд
                }
                else{
                	sPnt->T_non_rot--;
                }

                if(sPnt->Mot.firstStart == 0){
                    sPnt->wait = sPnt->prevWait;//20 * K_TIME_CALL_MICRO_TAKT;//ОТЛАДКА!!!!!
                    sPnt->T_Razm = load_s16(ramPnt, progPnt->Pnt_T_Razm) * K_TIME_CALL_MICRO_TAKT / 10;
                    if(sPnt->systemOverload == 0){
                        clrBitMicro(ramPnt, progPnt->Pnt_GT);
                        EnMot = sPnt->Mot.Ena1 + (sPnt->Mot.Ena2 << 1) + (sPnt->Mot.Ena3 << 2) + (sPnt->Mot.Ena4 << 3);      //!< Проверить разрешенные двигатели и найти двигатель с минимальным (наивысшим) приоритетом
                        for(i = 0, MotPr = NMOT; i < NMOT; i++){
                        	if(EnMot & ((u8)1 << i)) MotPr = i;
                        }
                        if(MotPr != NMOT){
                            for(i = 0; i < NMOT; i++){
                            	if((sPnt->MPR[i] <= sPnt->MPR[MotPr]) && ((1 << i) & EnMot)) MotPr = i;
                            }
                            sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop[MotPr]) * K_TIME_CALL_MICRO_TAKT;
                            sPnt->prevWait = sPnt->wait;
                            switch(MotPr){
                                case 0:
                                    sPnt->Mot.ON1 = 1;
                                    sPnt->Mot.M1_FC = 1;
                                break;
                                case 1:
                                	sPnt->Mot.ON2 = 1;
                                	sPnt->Mot.M2_FC = 1;
                                break;
                                case 2:
                                	sPnt->Mot.ON3 = 1;
                                	sPnt->Mot.M3_FC = 1;
                                break;
                                case 3:
                                	sPnt->Mot.ON4 = 1;
                                	sPnt->Mot.M4_FC = 1;
                                break;
                            }
                        }
                        else{
                        	internal_fail(sPnt);
                        }
                        sPnt->Mot.firstStart = 1;
                    }
                    else{
                    	setBitMicro(ramPnt, progPnt->Pnt_GT);
                    }
                }
                else{
                	motors_control(progPnt, ramPnt, sPnt);
                }
            }
            else{
                if(load_s32(ramPnt, progPnt->Pnt_TekFdrv) == 0){//load_s32(ramPnt, progPnt->Pnt_Fref)
                    if(sPnt->Mot.conRotMot & 0x01){
                    	sPnt->Mot.M1_FC = 1;
                    	sPnt->Mot.conRotMot = 0;
                        clrBitMicro(ramPnt, progPnt->Pnt_GT);
                    }
                    else if(sPnt->Mot.conRotMot & 0x02){
                    	sPnt->Mot.M2_FC = 1;
                    	sPnt->Mot.conRotMot = 0;
                        clrBitMicro(ramPnt, progPnt->Pnt_GT);
                    }
                    else if(sPnt->Mot.conRotMot & 0x04){
                    	sPnt->Mot.M3_FC = 1;
                    	sPnt->Mot.conRotMot = 0;
                        clrBitMicro(ramPnt, progPnt->Pnt_GT);
                    }
                    else if(sPnt->Mot.conRotMot & 0x08){
                    	sPnt->Mot.M4_FC = 1;
                    	sPnt->Mot.conRotMot = 0;
                        clrBitMicro(ramPnt, progPnt->Pnt_GT);
                    }
                    else{
                    	internal_fail(sPnt);
                    }
                }
                sPnt->T_non_rot = K_TIME_CALL_MICRO_TAKT * 600;                            //!< 10 минут задержки для устранения ротации двух моторов подряд
            }
        }
		else{
			mot_increase(progPnt, ramPnt, sPnt);
		}                                         //!< Сброс генератора темпа
        contactor_man(progPnt, ramPnt, sPnt);
        save_s32(ramPnt, progPnt->Pnt_F_Out, load_s32(ramPnt, progPnt->Pnt_Fref));
    }
    else{
    	clrBitMicro(ramPnt, progPnt->Pnt_AVOn);
    	clrBitMicro(ramPnt, progPnt->Pnt_GT);
    	setBitMicro(ramPnt, progPnt->Pnt_SleepAct);//Разрешение режима СОН, когда автовентиль отключен
        for(i = 0; i < NMOT; i++){
        	clrBitMicro(ramPnt, progPnt->Pnt_NetCtrM[i]);
        	clrBitMicro(ramPnt, progPnt->Pnt_FCCtrM[i]);
        }
        sPnt->Mot.all = 0; //Mot.firstStart = 0;
        sPnt->razmagnich_now = 0;
        sPnt->T_non_rot = K_TIME_CALL_MICRO_TAKT * 600; // 10 минут задержки для устранения ротации двух моторов подряд
        save_s32(ramPnt, progPnt->Pnt_Imot, load_s32(ramPnt, progPnt->Pnt_Inom));
        save_s16(ramPnt, progPnt->Pnt_ImaxMot, load_s16(ramPnt, progPnt->Pnt_Imax));
        save_s16(ramPnt, progPnt->Pnt_ActiveMot, 0);
        if(testBitMicro(ramPnt, progPnt->Pnt_WorkFlag))
        {
            save_s32(ramPnt, progPnt->Pnt_F_Out, load_s32(ramPnt, progPnt->Pnt_Fref));
        }
        else save_s32(ramPnt, progPnt->Pnt_F_Out, 0);
        sPnt->prevWait = 20 * K_TIME_CALL_MICRO_TAKT;
    }

    defBitMicro(ramPnt, progPnt->Pnt_Fail, sPnt->Mot.fail);

    return &progPnt->Pnt_End;
};

/*! \fn                  void MotoStandHour(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt, nvAUTOVENTIL_V2State_type *nvAdr)
 *  \brief               Функция подсчета моточасов и часов простоя дополнительных двигателей
 *  \param
 *  \param
 *  \param
 *  \return
 */
void MotoStandHour(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt, nvAUTOVENTIL_V2State_type *nvAdr)
{
    u8 i, MotInUse, MotEnable = 0;
    s16 tmp;

    MotInUse = sPnt->Mot.EnMH1 + (sPnt->Mot.EnMH2 << 1) + (sPnt->Mot.EnMH3 << 2) + (sPnt->Mot.EnMH4 << 3);
    if(testBitMicro(ramPnt, progPnt->Pnt_AVOn)){
    	MotEnable = sPnt->Mot.Ena1 + (sPnt->Mot.Ena2 << 1) + (sPnt->Mot.Ena3 << 2) + (sPnt->Mot.Ena4 << 3);
    }
	if(sPnt->TmrMin){
		sPnt->TmrMin--;
	}
    else{
    	tmp = 59 * load_s16(ramPnt, progPnt->Pnt_StndSpeed) + 1;
    	sPnt->TmrMin = MINUTE_MICRO_TAKT;                                                                //!< Установка таймера на отсчет 1 минуты
        for(i = 0; i < NMOT; i++){
        	if(MotInUse&((u8)1 << i)){                                                                   //!< Счетчик моточасов
        		sPnt->Prior.CntMoto[i]++;
        		WrFramMicro(&nvAdr->CntMoto[i], &sPnt->Prior.CntMoto[i], sizeof(nvAdr->CntMoto[i]));     //!< Запись в энергонезависимую память моточасов в минутах
        		save_s32(ramPnt, progPnt->Pnt_CntMot[i], (s32)sPnt->Prior.CntMoto[i]);              //!< Сохраняем моточасы в для отображения в минутах
        	}
        	else if(MotEnable&((u8)1 << i)){
        		sPnt->Prior.CntStand[i]++;                                                                     //!< Счетчик простоя двигателя в минутах
        		WrFramMicro(&nvAdr->CntStand[i], &sPnt->Prior.CntStand[i], sizeof(nvAdr->CntStand[i]));     //!< Запись в энергонезависимую память моточасов в минутах
        		save_s16(ramPnt, progPnt->Pnt_CntStop[i], (s16)(sPnt->Prior.CntStand[i]/tmp));                  //!< Сохраняем время простоя для отображения в часах
        	}
        }
    }

    for(i = 0; i < NMOT; i++){                                                                           //!< Автоматический сброс счетчика простоя при включении двигателя
    	if((MotInUse&((u8)1 << i)) && sPnt->Prior.CntStand[i]){                                                //!< Сброс счетчика простоя работающего двигателя
    		sPnt->Prior.CntStand[i] = 0;
    	    save_s16(ramPnt, progPnt->Pnt_CntStop[i], 0);                                                  //!< Сохраняем время простоя для отображения в часах
    	}
        if(load_s16(ramPnt, progPnt->Pnt_ResCnt[i])){                                                    //!< Отработка сброса счетчика моточасов
        	save_s16(ramPnt, progPnt->Pnt_ResCnt[i], 0);
        	sPnt->Prior.CntMoto[i] = 0;
    		WrFramMicro(&nvAdr->CntMoto[i], &sPnt->Prior.CntMoto[i], sizeof(nvAdr->CntMoto[i]));         //!< Запись в энергонезависимую память моточасов в минутах
    		save_s32(ramPnt, progPnt->Pnt_CntMot[i], (s32)sPnt->Prior.CntMoto[i]);                  //!< Сохраняем моточасы в для отображения в часах
        }
    }
}

/*! \fn                  void motors_control(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция регулирования по заданию ПИД - регулятора
 *  \param
 *  \param
 *  \param
 *  \return
 */
void motors_control(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    s32 in, max, min, delta, err;
    s16 NumMot;

    in = load_s32(ramPnt, progPnt->Pnt_Fref);                      //!< Осуществляем регулирование
    err = load_s32(ramPnt, progPnt->Pnt_Err_Pid);
    delta = load_s32(ramPnt, progPnt->Pnt_Dopusk);
    min = load_s32(ramPnt, progPnt->Pnt_minPID);
    max = load_s32(ramPnt, progPnt->Pnt_maxPID);

    if(err < (-delta)){
        if(in < (min + delta)){                                                                //!< Если частота опустилась до минимальной - начинаем отсчет времени на переключение
            if(sPnt->systemEdle == 0){                                                         //!< Если система не в простое, т.е. подключены дополнительные двигатели, то начинаем отсчет на отключение
                if(sPnt->wait == 0){                                                           //!< Когда время истекло - отключаем двиагетль
                    if(load_s16(ramPnt, progPnt->Pnt_Prior)){
                    	NumMot = mot_decrease_PR(progPnt, ramPnt, sPnt);
                    }
                    else{
                    	NumMot = mot_decrease_MH(progPnt, ramPnt, sPnt);
                    }
                    if((NumMot >= 0) && (NumMot < NMOT)){
                    	sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop[NumMot]) * K_TIME_CALL_MICRO_TAKT;
                    	sPnt->prevWait = sPnt->wait;
                    }
                    else{
                    	sPnt->wait = sPnt->prevWait;// sPnt->wait = 20 * K_TIME_CALL_MICRO_TAKT;//ОТЛАДКА!!!!
                    }
                }
                else{
                	sPnt->wait--;
                }
            }
            else{
            	sPnt->wait = sPnt->prevWait;//sPnt->wait = 20 * K_TIME_CALL_MICRO_TAKT;    //!< Если система в простое, перезагружаем таймер
            }
        }
        else{
        	sPnt->wait = sPnt->prevWait;// sPnt->wait = 20 * K_TIME_CALL_MICRO_TAKT;//ОТЛАДКА!!!!
        }
    }
    else if(err > delta){                                                                   //!< Если частота превысила верхний порог
        if(in > (max - delta)){                                                             //!< Если частота превысила пороговое значение - начинаем отсчет времени на переключение
            if(sPnt->systemOverload == 0){                                                  //!< Если система не занята полностью, т.е. есть свободные дополнительные двигатели, то начинаем отсчет на подключение
            	if(load_s32(ramPnt, progPnt->Pnt_TekFdrv) == max){                          //!< Если разогнались до максимума
                	if(sPnt->wait == 0){                                                        //!< Когда время истекло - подключаем двиагетль
                       	mot_increase(progPnt, ramPnt, sPnt);
                    }
                    else{
                        sPnt->wait--;
                        sPnt->T_Razm = load_s16(ramPnt, progPnt->Pnt_T_Razm) * K_TIME_CALL_MICRO_TAKT / 10;
                        sPnt->D_rdy_for = 0;
                    }
            	}
            }
            else{
            	sPnt->wait = sPnt->prevWait;//                    	sPnt->wait = 20 * K_TIME_CALL_MICRO_TAKT;//ОТЛАДКА!!!!
            }
        }
    }
    else{
    	sPnt->wait = sPnt->prevWait;//                    	sPnt->wait = 20 * K_TIME_CALL_MICRO_TAKT;//ОТЛАДКА!!!!
    }
}

/*! \fn                  void mot_decrease_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция переключения(ротации) между дополнительными двигателя по количеству моточасов
 *  \param
 *  \param
 *  \param
 *  \return
 */
s16 mot_decrease_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    u8 i, State, max;

    State = (sPnt->Mot.M1_AC + (sPnt->Mot.M2_AC << 1) + (sPnt->Mot.M3_AC << 2) + (sPnt->Mot.M4_AC << 3));

    for(i = 0, max = NMOT; i < NMOT; i++){
    	if(State & ((u8)1 << i)){   //!< Присваиваем максимум последнему номеру из подключенных двигателей
    		max = i;
    	}
    }

    if(max != NMOT){                       //!< Если есть хоть 1 подключенный двигатель
        for(i = 0; i < NMOT; i++){
        	if(State & ((u8)1 << i)){      //!< Находим максимум среди подключенных двигателей
        		if(sPnt->MH[i] > sPnt->MH[max]) max = i;
        	}
        }
        switch(max){                       //!<Отключаем двигатель с максимальным количеством моточасов
            case 0: sPnt->Mot.M1_AC = 0;
            break;
            case 1: sPnt->Mot.M2_AC = 0;
            break;
            case 2: sPnt->Mot.M3_AC = 0;
            break;
            case 3: sPnt->Mot.M4_AC = 0;
            break;
        }
    }
    else{                                  //!< Нет ни одного подключенного двигателя - отключать нечего. Переходим в режим СОН, если он разрешен.
    	if(load_s16(ramPnt, progPnt->Pnt_SleepSel) == SleepEnable){
    		sPnt->Mot.SleepEn = 1;                 //Разрешение режима СОН, когда все двигатели отключены от сети
    	}
    	//internal_fail(); setBitMicro(ramPnt, progPnt->Pnt_SleepAct);
    	return -1;
    }

    return (s16)max;
}

/*! \fn                  void mot_decrease_PR(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция переключения(ротации) между дополнительными двигателя по приоритету пользователя
 *  \param
 *  \param
 *  \param
 *  \return
 */
s16 mot_decrease_PR(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    u8 i, State, max;

    State = (sPnt->Mot.M1_AC + (sPnt->Mot.M2_AC << 1) + (sPnt->Mot.M3_AC << 2) + (sPnt->Mot.M4_AC << 3));

    for(i = 0, max = NMOT; i < NMOT; i++){
    	if(State & ((u8)1 << i)){   //!< Присваиваем максимум последнему номеру из подключенных двигателей
    		max = i;
    	}
    }

    if(max != NMOT){                       //!< Если есть хоть 1 подключенный двигатель
        for(i = 0; i < NMOT; i++){
        	if(State & ((u8)1 << i)){      //!< Находим максимум среди подключенных двигателей
        		if(sPnt->MPR[i] > sPnt->MPR[max]) max = i;
        	}
        }
        switch(max){                       //!<Отключаем двигатель с наивысшим приоритетом, установленным пользователем (наивысший приоритет 4, 1 - наименьший)
            case 0: sPnt->Mot.M1_AC = 0;
            break;
            case 1: sPnt->Mot.M2_AC = 0;
            break;
            case 2: sPnt->Mot.M3_AC = 0;
            break;
            case 3: sPnt->Mot.M4_AC = 0;
            break;
        }
    }
    else{                                  //!< Нет ни одного подключенного двигателя - отключать нечего. Переходим в режим СОН, если он разрешен.
    	if(load_s16(ramPnt, progPnt->Pnt_SleepSel) == SleepEnable){
    		sPnt->Mot.SleepEn = 1;             //Разрешение режима СОН, когда все двигатели отключены от сети
    	}
    	//internal_fail();    setBitMicro(ramPnt, progPnt->Pnt_SleepAct);
    	return -1;
    }

    return (s16)max;
}

/*! \fn                  void mot_increase(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция переключения дополнительного двигателя на сеть переменного тока
 *  \param
 *  \param
 *  \param
 *  \return
 */
void mot_increase(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    s16 NumMot;
	if(sPnt->D_rdy_for == 0){                          //!< Если ни один из двигателей не готов
        sPnt->D_rdy_for = mot_disconnect_FC(sPnt);
    }

	if(sPnt->T_Razm == 1){
		setBitMicro(ramPnt, progPnt->Pnt_GT);
	}

    if(sPnt->T_Razm == 0){                              //!< Если таймер размагничивания дотикал - подключаем двигатель на сеть
        mot_connect_AC(sPnt->D_rdy_for, sPnt);
        if(load_s16(ramPnt, progPnt->Pnt_Prior)){ NumMot = mot_connect_FC_PR(progPnt, ramPnt, sPnt);}
        else{                                     NumMot = mot_connect_FC_MH(progPnt, ramPnt, sPnt);}
        sPnt->razmagnich_now = 0;
        sPnt->T_Razm = load_s16(ramPnt, progPnt->Pnt_T_Razm) * K_TIME_CALL_MICRO_TAKT / 10;
        if((NumMot >= 0) && (NumMot < NMOT)){
        	sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop[NumMot]) * K_TIME_CALL_MICRO_TAKT;
        	sPnt->prevWait = sPnt->wait;
        }
        else{
        	sPnt->wait = sPnt->prevWait;//20 * K_TIME_CALL_MICRO_TAKT; //20 секунд ОТЛАДКА!!!!
        }
        clrBitMicro(ramPnt, progPnt->Pnt_GT);
    }
    else{
        sPnt->T_Razm--;
        sPnt->razmagnich_now = 1;
    }
}

/*! \fn                  void mot_state(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция проверки разрешения работы дополнительных двигателей
 *  \param
 *  \param
 *  \param
 *  \return
 */
void mot_state(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    s16 EnableMotors, MotFC;//, State;
    u8 i = 0;

    connect_New_FC(progPnt, ramPnt, sPnt);
    EnableMotors = load_s16(ramPnt, progPnt->Pnt_EnMot);

    MotFC = sPnt->Mot.M1_FC + (sPnt->Mot.M2_FC << 1) + (sPnt->Mot.M3_FC << 2) + (sPnt->Mot.M4_FC << 3);

    if(MotFC){
        for(i = 0; i < NMOT; i++){
        	if(MotFC & (1 << i)){
                save_s32(ramPnt, progPnt->Pnt_Imot, load_s32(ramPnt, progPnt->Pnt_InomDop[i]));
                save_s16(ramPnt, progPnt->Pnt_ImaxMot, load_s16(ramPnt, progPnt->Pnt_ImaxDop[i]));
        		i++;
        		break;
        	}
        }
    }

    save_s16(ramPnt, progPnt->Pnt_ActiveMot, i);

    sPnt->Mot.Ena1 = ((EnableMotors & 1) > 0);    //!< Устанавить разрешение использование дополнительных двигателей
    sPnt->Mot.Ena2 = ((EnableMotors & 2) > 0);
    sPnt->Mot.Ena3 = ((EnableMotors & 4) > 0);
    sPnt->Mot.Ena4 = ((EnableMotors & 8) > 0);

    sPnt->Mot.ON1 &= sPnt->Mot.Ena1;          //!< Сбросить биты работы запрещенных двигателей
    sPnt->Mot.ON2 &= sPnt->Mot.Ena2;
    sPnt->Mot.ON3 &= sPnt->Mot.Ena3;
    sPnt->Mot.ON4 &= sPnt->Mot.Ena4;

    for(i = 0; i < NMOT; i++){
        if(((EnableMotors & ((u16)1 << i)) == 0) && (MotFC & ((u16)1 << i))){                  //!< Если работа мотора запрещена и бит работы от ПЧ установлен
        	MotFC &= ~((u16)1 << i);                                                           //!< Сбрасываем бит работы от ПЧ
        	setBitMicro(ramPnt, progPnt->Pnt_GT);                                              //!< Сбросить генератор темпа
        	sPnt->Mot.connectNewFC = 1;
        }
    }

    sPnt->Mot.M1_FC = ((MotFC & 1) > 0);            //!< Разрешение/запрещение работы дополнительного двигателя от ПЧ
    sPnt->Mot.M2_FC = ((MotFC & 2) > 0);
    sPnt->Mot.M3_FC = ((MotFC & 4) > 0);
    sPnt->Mot.M4_FC = ((MotFC & 8) > 0);

    sPnt->Mot.M1_AC &= sPnt->Mot.Ena1;          //!< Сбросить биты работы от сети запрещенных двигателей
    sPnt->Mot.M2_AC &= sPnt->Mot.Ena2;
    sPnt->Mot.M3_AC &= sPnt->Mot.Ena3;
    sPnt->Mot.M4_AC &= sPnt->Mot.Ena4;

    //State = (sPnt->Mot.M1_AC + (sPnt->Mot.M2_AC << 1) + (sPnt->Mot.M3_AC << 2) + (sPnt->Mot.M4_AC << 3));
    if(sPnt->Mot.SleepEn){
    	setBitMicro(ramPnt, progPnt->Pnt_SleepAct);//Разрешаем режим СОН
    }
    else{
    	clrBitMicro(ramPnt, progPnt->Pnt_SleepAct);//Запрещение режима СОН, когда к сети подключен хоть 1 двигатель
    }
}

/*! \fn                  u16 mot_disconnect_FC()
 *  \brief               Функция отключения дополнительного двигателя от ПЧ
 *  \param
 *  \param
 *  \param
 *  \return
 */
u16 mot_disconnect_FC(AUTOVENTIL_V2State_type *sPnt)
{
    u8 WorkByFC, Motor = 0;

    WorkByFC = sPnt->Mot.M1_FC + (sPnt->Mot.M2_FC << 1) + (sPnt->Mot.M3_FC << 2) + (sPnt->Mot.M4_FC << 3);            //!< Двигатель, работающий от ПЧ

    switch((UseMot_type)WorkByFC){
        case FInUse:                                                                          //!< Если от ПЧ работает первый двигатель
        	sPnt->Mot.M1_FC = 0;
        	Motor = 1;
        break;
        case SInUse:                                                                          //!< Если от ПЧ работает второй двигатель
        	sPnt->Mot.M2_FC = 0;
        	Motor = 2;
        break;
        case TInUse:                                                                          //!< Если от ПЧ работает третий двигатель
        	sPnt->Mot.M3_FC = 0;
        	Motor = 3;
        break;
        case FoInUse:                                                                         //!< Если от ПЧ работает четвертый двигатель
        	sPnt->Mot.M4_FC = 0;
            Motor = 4;
        break;
        default:                                                                              //!< Если от ПЧ не работает ни один из двигателей или больше одного
            internal_fail(sPnt);
        break;
    }
    return Motor;
}

/*! \fn                  void mot_connect_AC(u16 numb)
 *  \brief               Функция подключения дополнительного двигателя на сеть переменного тока
 *  \param
 *  \param
 *  \param
 *  \return
 */
void mot_connect_AC(u16 numb, AUTOVENTIL_V2State_type *sPnt)
{
    switch((ReadyMot_type)numb){
        case FReady:
            if(sPnt->Mot.Ena1 && ((sPnt->Mot.Ena2 && (sPnt->Mot.ON2 == 0)) || (sPnt->Mot.Ena3 && (sPnt->Mot.ON3 == 0)) || (sPnt->Mot.Ena4 && (sPnt->Mot.ON4 == 0)))){
            	sPnt->Mot.M1_AC = 1;
            	sPnt->Mot.ON1 = 1;
            }
            else internal_fail(sPnt);
        break;
        case SReady:
            if(sPnt->Mot.Ena2 && ((sPnt->Mot.Ena1 && (sPnt->Mot.ON1 == 0)) || (sPnt->Mot.Ena3 && (sPnt->Mot.ON3 == 0)) || (sPnt->Mot.Ena4 && (sPnt->Mot.ON4 == 0)))){
            	sPnt->Mot.M2_AC = 1;
            	sPnt->Mot.ON2 = 1;
            }
            else internal_fail(sPnt);
        break;
        case TReady:
            if(sPnt->Mot.Ena3 && ((sPnt->Mot.Ena2 && (sPnt->Mot.ON2 == 0)) || (sPnt->Mot.Ena1 && (sPnt->Mot.ON1 == 0)) || (sPnt->Mot.Ena4 && (sPnt->Mot.ON4 == 0)))){
            	sPnt->Mot.M3_AC = 1;
            	sPnt->Mot.ON3 = 1;
            }
            else internal_fail(sPnt);
        break;
        case FoReady:
            if(sPnt->Mot.Ena4 && ((sPnt->Mot.Ena1 && (sPnt->Mot.ON1 == 0)) || (sPnt->Mot.Ena2 && (sPnt->Mot.ON2 == 0)) || (sPnt->Mot.Ena3 && (sPnt->Mot.ON3 == 0)))){
            	sPnt->Mot.M4_AC = 1;
            	sPnt->Mot.ON4 = 1;
            }
            else internal_fail(sPnt);
        break;
        default:
            internal_fail(sPnt);
        break;
    }
}

/*! \fn                  void mot_connect_FC_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция подключения дополнительного двигателя к ПЧ с наименьшим количеством моточасов
 *  \param
 *  \param
 *  \param
 *  \return
 */
s16 mot_connect_FC_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    u8 i, State, min;

    State = ((sPnt->Mot.ON1 == 0) && sPnt->Mot.Ena1) + (((sPnt->Mot.ON2 == 0) && sPnt->Mot.Ena2) << 1) + (((sPnt->Mot.ON3 == 0) && sPnt->Mot.Ena3) << 2) + (((sPnt->Mot.ON4 == 0) && sPnt->Mot.Ena4) << 3);

    for(i = 0, min = NMOT; i < NMOT; i++){            //!< Присваиваем минимум последнему номеру из отключенных двигателей
        if(State & ((u8)1 << i)){
        	min = i;
        }
    }

    if(State == 0x0F || State == 0){                                    //!< Или все двигатели отключены или все подключены
    	//internal_fail(sPnt);
    	return -1;
    }
    else{	                                                            //!< Если есть хоть 1 отключенный двигатель
        for(i = 0; i < NMOT; i++){
            if(State & ((u8)1 << i)){                                   //!< Находим минимум среди подключенных двигателей
                if(sPnt->MH[i] < sPnt->MH[min]) min = i;
            }
        }
        switch(min){                                                    //!< Подключаем двигатель с минимальным количеством моточасов
            case 0:
            	sPnt->Mot.ON1 = 1;
            	sPnt->Mot.M1_FC = 1;
            break;
            case 1:
            	sPnt->Mot.ON2 = 1;
            	sPnt->Mot.M2_FC = 1;
            break;
            case 2:
            	sPnt->Mot.ON3 = 1;
            	sPnt->Mot.M3_FC = 1;
            break;
            case 3:
            	sPnt->Mot.ON4 = 1;
            	sPnt->Mot.M4_FC = 1;
            break;
        }
    }

    return (s16)min;
}

/*! \fn                  void mot_connect_FC_PR(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция подключения дополнительного двигателя к ПЧ по приоритету пользователя
 *  \param
 *  \param
 *  \param
 *  \return
 */
s16 mot_connect_FC_PR(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    u8 i, State, min;

    State = ((sPnt->Mot.ON1 == 0) && sPnt->Mot.Ena1) + (((sPnt->Mot.ON2 == 0) && sPnt->Mot.Ena2) << 1) + (((sPnt->Mot.ON3 == 0) && sPnt->Mot.Ena3) << 2) + (((sPnt->Mot.ON4 == 0) && sPnt->Mot.Ena4) << 3);

    for(i = 0, min = NMOT; i < NMOT; i++){            //!< Присваиваем минимум последнему номеру из отключенных двигателей
        if(State & ((u8)1 << i)){
        	min = i;
        }
    }

    if(State == 0x0F || State == 0){                                    //!< Или все двигатели отключены или все подключены
    	//internal_fail(sPnt);
    	return -1;
    }
    else{	                                                            //!< Если есть хоть 1 отключенный двигатель
        for(i = 0; i < NMOT; i++){
            if(State & ((u8)1 << i)){                                   //!< Находим минимум среди отключенных двигателей
                if(sPnt->MPR[i] < sPnt->MPR[min]) min = i;
            }
        }
        switch(min){                                                    //!< Подключаем двигатель с минимальным(наивысшим) приоритетом
            case 0:
            	sPnt->Mot.ON1 = 1;
            	sPnt->Mot.M1_FC = 1;
            break;
            case 1:
            	sPnt->Mot.ON2 = 1;
            	sPnt->Mot.M2_FC = 1;
            break;
            case 2:
            	sPnt->Mot.ON3 = 1;
            	sPnt->Mot.M3_FC = 1;
            break;
            case 3:
            	sPnt->Mot.ON4 = 1;
            	sPnt->Mot.M4_FC = 1;
            break;
        }
    }

    return (s16)min;
}

/*! \fn                  void contactor_man(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция управления дискретными выходами(реле) - переключение двигателей на сеть или ПЧ
 *  \param
 *  \param
 *  \param
 *  \return
 */
void contactor_man(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
	u8 MCtrByNet, MCtrByFC, i, Fail, WorkMot;

	WorkMot = 0;
	Fail = 0;
	MCtrByNet = sPnt->Mot.M1_AC + ((u8)sPnt->Mot.M2_AC << 1) + ((u8)sPnt->Mot.M3_AC << 2) + ((u8)sPnt->Mot.M4_AC << 3); //Двигатели, которые надо переключить на сеть переменного тока
	MCtrByFC = sPnt->Mot.M1_FC + ((u8)sPnt->Mot.M2_FC << 1) + ((u8)sPnt->Mot.M3_FC << 2) + ((u8)sPnt->Mot.M4_FC << 3);  //Двигатели, которые надо подключить к ПЧ

	for(i = 0; i < NMOT; i++){
        if(MCtrByNet&((u8)1 << i)){           //!< Если двигатель надо подключить на сеть переменного тока
        	if(MCtrByFC&((u8)1 << i)){        //!< Если двигатель надо подключить к ПЧ
        		Fail = 1;                     //!< Если один и тот же двигатель надо подключить и на сеть и на ПЧ - инидицируем состояние аварии автовентиля
        		MCtrByNet &= ~((u8)1 << i);   //!< Отключаем от сети
        		MCtrByFC &= ~((u8)1 << i);    //!< Отключаем от ПЧ
        	}
        	else{                             //!< Если двигатель надо отключить от ПЧ и подключить на сеть переменного тока
        		WorkMot |= ((u8)1 << i);
        	}
        }
        else{                                 //!< Если двигатель надо отключить от сети переменного тока
            if(MCtrByFC&((u8)1 << i)){        //!< Если двигатель надо подключить к ПЧ
            	WorkMot |= ((u8)1 << i);
            }
        }
        defBitMicro(ramPnt, progPnt->Pnt_NetCtrM[i], MCtrByNet&((u8)1 << i));
        GlobalM3.GT_Work = MCtrByNet;
        defBitMicro(ramPnt, progPnt->Pnt_FCCtrM[i], MCtrByFC&((u8)1 << i));
	}

	sPnt->Mot.ON1 = ((WorkMot & 1) > 0);
	sPnt->Mot.ON2 = ((WorkMot & 2) > 0);
	sPnt->Mot.ON3 = ((WorkMot & 4) > 0);
	sPnt->Mot.ON4 = ((WorkMot & 8) > 0);
	if(Fail){
		internal_fail(sPnt);
	}

	sPnt->Mot.EnMH1 = sPnt->Mot.ON1;
	sPnt->Mot.EnMH2 = sPnt->Mot.ON2;
	sPnt->Mot.EnMH3 = sPnt->Mot.ON3;
	sPnt->Mot.EnMH4 = sPnt->Mot.ON4;
}

/*! \fn                  void busywork(AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция проверки состояния системы автовентиль
 *  \param
 *  \param
 *  \param
 *  \return
 */
void busywork(AUTOVENTIL_V2State_type *sPnt)
{
    if((sPnt->Mot.ON1 || (sPnt->Mot.Ena1 == 0)) &&  //!< Если все двигатели в работе - система загружена полностью
       (sPnt->Mot.ON2 || (sPnt->Mot.Ena2 == 0)) &&
       (sPnt->Mot.ON3 || (sPnt->Mot.Ena3 == 0)) &&
       (sPnt->Mot.ON4 || (sPnt->Mot.Ena4 == 0))){

        sPnt->systemOverload = 1;    //!< Система загружена полностью
    }
    else{
    	sPnt->systemOverload = 0;   //!< Система не загружена полностью
    }

    if((sPnt->Mot.ON1 == 0) && (sPnt->Mot.ON2 == 0) && (sPnt->Mot.ON3 == 0) && (sPnt->Mot.ON4 == 0)){
        sPnt->systemEdle = 1;      //!< Система автовентиль в простое
    }
    else{
    	sPnt->systemEdle = 0;     //!< Система автовентиль в работе
    }
}

/*! \fn                  void prior(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Вычитывание и изменение приоритетов дополнительных двигателей
 *  \param
 *  \param
 *  \param
 *  \return
 */
void prior(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    s16 mpr[NMOT], EnableMotors;
    u8 i, j;

    EnableMotors = load_s16(ramPnt, progPnt->Pnt_EnMot);

    for(i = 0; i < NMOT; i++){                                                              //!< Вычитывание уставок приоритеов дополнительных двигателей
    	save_s16(ramPnt, progPnt->Pnt_StateDop[i], ((EnableMotors & (1 << i)) > 0));
    	mpr[i] = load_s16(ramPnt, progPnt->Pnt_Ustpr[i]);
        if(sPnt->MPR[i] == 0) sPnt->MPR[i] = mpr[i];
    }

    for(i = 0; i < NMOT; i++){
        if(sPnt->MPR[i] != mpr[i]){                              //!< Если изменился приоритет двигателя
        	for(j = 0; j < NMOT; j++){ //!< Меняем местами приоритеты двух двигателей и выходим из цикла
        	    if(sPnt->MPR[j] == mpr[i]){
        	    	mpr[j] = sPnt->MPR[i];
        	    	sPnt->MPR[i] = mpr[i];
        	    	sPnt->MPR[j] = mpr[j];
        	    	save_s16(ramPnt, progPnt->Pnt_Ustpr[j], sPnt->MPR[j]);
        	    	return;
        	    }
        	}
        }
    }

    if((mpr[0] == mpr[1]) || (mpr[0] == mpr[2]) || (mpr[0] == mpr[3]) ||
       (mpr[1] == mpr[2]) || (mpr[1] == mpr[3]) || (mpr[2] == mpr[3])){          //!< Проверка на совпадение приоритетов двигателей

        for(i = 0; i < NMOT; i++){                                                //!< Если совпадают приоритеты - переназначаем приоритеты дополнительных двигателей по умолчанию
        	sPnt->MPR[i] = i + 1;
        	save_s16(ramPnt, progPnt->Pnt_Ustpr[i], sPnt->MPR[i]);
        }
    }
}

/*! \fn                  void prior_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Вычитывание моточасов дополнительных двигателей
 *  \param
 *  \param
 *  \param
 *  \return
 */
void prior_MH(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    u8 i;

    for(i = 0; i < NMOT; i++){
        sPnt->MH[i] = (u32)load_s32(ramPnt, progPnt->Pnt_CntMot[i]); //Вычитываение моточасов двигателей в минутах
    }
}

/*! \fn                  void rotatsnuti(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция переключения(ротации) двигателей при достижении времени простоя двигателя времени ротации
 *  \param
 *  \param
 *  \param
 *  \return
 */
void rotatsnuti(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
    s16 T_dvigMax, T_dvig[4];
    u8  i, StandLimMot, MotFC, min,  mask; // rotate

    T_dvigMax = load_s16(ramPnt, progPnt->Pnt_T_rot);                              //!< Вычитываем время ротации в часах

    if(T_dvigMax > 0){
    	MotFC = sPnt->Mot.M1_FC + (sPnt->Mot.M2_FC << 1) + (sPnt->Mot.M3_FC << 2) + (sPnt->Mot.M4_FC << 3); //!< Двигатель, подключенный к ПЧ
        for(i = 0, StandLimMot = 0, min = NMOT; i < NMOT; i++){
            T_dvig[i] = load_s16(ramPnt, progPnt->Pnt_CntStop[i]);	               //!< Вычитываем время простоя двигателей в часах
            if(T_dvig[i] >= T_dvigMax){
            	StandLimMot |= ((u8)1 << i);
            	min = i;
            }
        }

        if((RotMot_type)StandLimMot == AllNeedRot){                                //!< У всех 4-х двигателей время простоя больше времени ротации
        	internal_fail(sPnt);
        	//rotate = 0;
        }
        else if(MotFC && StandLimMot){
            for(i = 0, mask = 0; i < NMOT; i++){
                if((StandLimMot & ((u8)1 << i)) && (sPnt->MH[i] <= sPnt->MH[min])){  //!< Находим номер двигателя, у которого минимум моточасов, из двигателей ожидающих ротации
                	mask = (1 << i);
                	break;
                }
            }

            setBitMicro(ramPnt, progPnt->Pnt_GT);
            mot_disconnect_FC(sPnt);

            sPnt->Mot.conRotMot = mask & 0x0F;// sPnt->Mot.conRotM2 = ((mask & 2) > 0);sPnt->Mot.conRotM3 = ((mask & 4) > 0);sPnt->Mot.conRotM4 = ((mask & 8) > 0);
        }
        else if(StandLimMot && (MotFC == 0)){
        	internal_fail(sPnt); //!< Проверить!!!!!!!!
        }
    }
}

/*! \fn                  void connect_New_FC(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
 *  \brief               Функция подключения дополнительного двигателя к ПЧ по приоритету
 *  \param
 *  \param
 *  \param
 *  \return
 */
void connect_New_FC(FuncAUTOVENTIL_V2_type *progPnt, u32 ramPnt, AUTOVENTIL_V2State_type *sPnt)
{
	s16 Reload;
    if(sPnt->Mot.connectNewFC && (load_s32(ramPnt, progPnt->Pnt_TekFdrv) == 0)){//load_s32(ramPnt, progPnt->Pnt_Fref) == 0)
    	sPnt->Mot.prior = load_s16(ramPnt, progPnt->Pnt_Prior) & 1;         //!< Вычитываем приоритет подключения дополнительных двигателей
        if(sPnt->Mot.prior){
        	Reload = mot_connect_FC_PR(progPnt, ramPnt, sPnt);       //!< 1 - приоритет, заданный пользователем
        }
        else{
        	Reload = mot_connect_FC_MH(progPnt, ramPnt, sPnt);       //!< 0 - приоритет по моточасам
        }
        if(Reload != -1){
            sPnt->Mot.connectNewFC = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_GT);
        }
    }
}

void internal_fail(AUTOVENTIL_V2State_type *sPnt)
{
	sPnt->Mot.fail = 1;
}

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAUTOVENTIL_V2_1[67]={
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  APV_NW
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  WorkFlag
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Sel
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Enable
		S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Fref
		S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Err_Pid
		S32_PIN_TYPE | INPUT_PIN_MODE,     //7  maxPID
		S32_PIN_TYPE | INPUT_PIN_MODE,     //8  minPID
		S32_PIN_TYPE | INPUT_PIN_MODE,     //9  Dopusk
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  T_Dop1
		S16_PIN_TYPE | INPUT_PIN_MODE,     //11  T_Dop2
		S16_PIN_TYPE | INPUT_PIN_MODE,     //12  T_Dop3
		S16_PIN_TYPE | INPUT_PIN_MODE,     //13  T_Dop4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  T_Razm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  T_rot
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  Inom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Inom1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  Inom2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19  Inom3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  Inom4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Imax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Imax1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Imax2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  Imax3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Imax4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  EnMot
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  Prior
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Ustpr1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  Ustpr2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  Ustpr3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  Ustpr4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  ResCnt1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  ResCnt2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  ResCnt3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  ResCnt4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  SleepSel
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  StndSpeed
        S32_PIN_TYPE | INPUT_PIN_MODE,     //38  TekFdrv
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //39  GT
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //40  Fail
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //41  WaterFail
		S32_PIN_TYPE | OUTPUT_PIN_MODE,    //42  F_Out
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //43  AVOn
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //44  SleepAct
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //45  NetCtrM1
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //46  NetCtrM2
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //47  NetCtrM3
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //48  NetCtrM4
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //49  FCCtrM1
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //50  FCCtrM2
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //51  FCCtrM3
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //52  FCCtrM4
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //53  Imot
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //54  ImaxMot
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //55  State1
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //56  State2
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //57  State3
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //58  State4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //59  ActiveMot
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //60  CntMot1
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //61  CntMot2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //62  CntMot3
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //63  CntMot4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //64  CntStop1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //65  CntStop2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //66  CntStop3
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //67  CntStop4
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AUTOVENTIL_V2", "DD", TblFuncAUTOVENTIL_V2_1, FuncAUTOVENTIL_V2_1, sizeof(TblFuncAUTOVENTIL_V2_1), sizeof(AUTOVENTIL_V2State_type), sizeof(nvAUTOVENTIL_V2State_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAUTOVENTIL_V2_1,
#endif
//********************************************************
