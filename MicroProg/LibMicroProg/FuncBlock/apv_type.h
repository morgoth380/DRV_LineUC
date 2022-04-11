#ifndef APV_TYPE_H
#define APV_TYPE_H

//====================================================================
// Глобальный блок АПВ
//====================================================================
typedef union{ u16 all;
  struct{
    u16         APV             :1;     // Запрос на АПВ
    u16         APVLatch        :1;     // Квитанция о защелкнутом АПВ
    u16         AutoMode        :1;     // Режим "авто" включен ==1 или нажата кнопка СТОП и ни каких АПВ до ПУСК
    u16         PowerLossAPV    :1;     // Флаг осуществленного запроса АПВ при восстановлении напряжения после аварии

    u16         StopKeyState    :1;     // Для вылавливания фронтов СТОП
    u16         PuskKeyState    :1;     // Для вылавливания фронтов ПУСК
    u16         SecondFlag      :1;     // Для синхронизации локальных блоков
    u16         Blink           :1;
    u16	        StopResetApvCnt	:1;	// Запрос на сброс счетчиков АПВ по нажатию кнопки Стоп
  } bits;
}APVstatus_type;


typedef struct{
    APVstatus_type      APVstatus;          // Флаги если выключат питание
    u32                 Time;               // Для переустановки времени ????
    u16                 APV_Freeze;         // Счетчик блоков АПВ, которые уже сбросили свои счетчики.
    u32                 ApvResetCntTime;
    u16                 Reserved;           // Для выравнивания
    u16                 crc;
}APVState_type;
 // APVState_type и nvAPVState_type - ДОЛЖНЫ БЫТЬ ОДИНАКОВЫМИ. ИСПОЛЬЗУЮТСЯ В ГЛОБАЛЬНОЙ ПЕРЕМЕННОЙ APVState
typedef struct{
    APVstatus_type      APVstatus;          // Флаги если выключат питание
    u32                 Time;               // Для переустановки времени ????
    u16                 APV_Freeze;         // Счетчик блоков АПВ, которые уже сбросили свои счетчики.
    u32                 ApvResetCntTime;
    u16                 Reserved;           // Для выравнивания
    u16                 crc;
}nvAPVState_type;

typedef struct{
	u16 CountCall;
	u16      PowerLossFlag;
	u32      PowerFailTime, TimeToAPV, ProgFailTime;
}POWER_LOSS_APVStateType;

typedef struct{
    u16 latch;
}APV1_State_type;

#define SZ_APV_STATUS   sizeof(APVstatus_type)
//====================================================================
// Локальный блок АПВ
//====================================================================
typedef union{
    struct{
        u16         ApvLatch        :1;         // Запрос на АПВ до нормализации сигнала
        u16         ResetState      :1;         // Предыдущее состояние входного сигнала Сброс счётчика АПВ
        u16         prevFailState   :1;         // Предыдущее состояние аварии
        u16         reserv          :14;
    };
    u16     all;
}localAPVflag_type;

typedef struct{
    u32                 TimeToAPV;              // Время для глобального блока АПВ   Сохраняется в FRAM
    u16                 CntApv;                 // Счётчик количества АПВ
    u16 		ApvCntPower;            // Флаг засчитывания апв по питанию
    localAPVflag_type   flag;                   // Флаги состояния
    u16                 pickUpReset;            // подхват аварии при заблокированном АПВ

    u16                 tmrDelayControl;        // Задержка, в секундах, запрет обработки ошибок на время пуска
    u16                 tmrDelayOff;            // Задержка, в секундах, выдачи сигнала Авария
    u16                 numBlock;               // Номер блока в массиве
    u16                 ApvOnOff;               // Статус АПВ - Вкл / Выкл, для контроля включения авторежима
}LOCAL_APVState_type;

typedef struct{
    u32                 TimeToAPV;              // Время до АПВ хранится в каждом локальном блоке
    u16                 CntApv;                 // Счётчик количества АПВ
    u16 		ApvCntPower;
    localAPVflag_type   flag;                   // Флаги состояния

}LOCAL_APVnvState_type;
//====================================================================
#define NUM_APV_BLOCK   12
extern u32      APVpnt[NUM_APV_BLOCK];  // Массив c временем запуска по АПВ для локальных блоков
extern u32      globFailTime;
extern u16      normalWaitAPV;
                                                // 0- нет запроса на АПВ !=0 время запроса
extern APVState_type    APVState;               // Структура с состоянием глобального блока АПВ
extern u16 ApvCount;
//====================================================================

#endif
