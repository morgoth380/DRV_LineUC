/************************** (C) Copyright 2013 Triol ***************************
* File Name          : master_set_pump.h
* Author             : Kats Evgeniy
* TS Idea            : 2013.02.19
* TS Version         : 2013.02.21
* Description        : Файл содержит функцию, которая реализует мастер настройки
*                      для насосного исполнения
*******************************************************************************/
#ifdef    GET_FUNC_CODE

// Стадии мастера настройки
typedef enum{
    PumpState_level_START,
    PumpState_level_1,
    PumpState_level_2,
    PumpState_level_3,
    PumpState_level_4,
    PumpState_level_5,
    PumpState_level_6,
    PumpState_level_7,
    PumpState_level_8,
    PumpState_level_9,
    PumpState_level_10,
    PumpState_level_11,
    PumpState_level_12,
    PumpState_level_13,
    PumpState_level_14,
    PumpState_level_15,
    PumpState_level_16,
    PumpState_level_17,
    PumpState_level_END,
}PumpState_type;

// Номера параметров для каждой стадии мастера настройки
const u16 uIDPrmPump[] = {
        0x0000,             //Начальное смещение
        0x0000,             //00.00 Номинальный ток двигателя
        0x0200,             //04.00 Частота пуска
        0x0201,             //04.01 Время фиксации пусковой частоты
        0x0180,             //03.00 Время разгона
        0x1704,             //46.04 Использование ПИД регулятора
        0x0D01,             //26.01 Пропорциональный коэффициент
        0x0D02,             //26.02 Интегральный коэффициент
        0x0D03,             //26.03 Дифференциальный коэффициент
        0x1705,             //46.05 Использование каскадного контроллера
        0x0A09,             //20.09 Частота включения асинхронного двигателя
        0x0A0A,             //20.10 Частота выключения асинхронного двигателя
        0x0A02,             //20.02 Режим двигателя №1
        0x0A03,             //20.03 Режим двигателя №2
        0x0A04,             //20.04 Режим двигателя №3
        0x0A05,             //20.05 Режим двигателя №4
        0x0A80,             //21.00 Активировать подхват
        0x1203,             //36.03 Контроль входного напряжения
};

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt__Prm0;                         //1  &Prm
    u16  Pnt__Prm1;                         //1  &Prm
    u16  Pnt__Prm2;                         //1  &Prm
    u16  Pnt_NumState;                     //2  NumState
    u16  Pnt_SelPID;                       //3  SelPID
    u16  Pnt_SelCascad;                    //4  SelCascad
    u16  Pnt_End;
}FuncMASTER_SET_PUMP_type;
//********************************************************
u16 *FuncMASTER_SET_PUMP_1(FuncMASTER_SET_PUMP_type *progPnt, u32 ramPnt)
{
    u16 NumState;   // Текущее состояние мастера настройки
    u16 IDPrmTek;   // Текущее значение ссылочного параметра (номер редактируемого параметра в мастере) 
    u16 SelPID;     // Значение параметра выбора настройки ПИД регулятора ("0" - Нет, "1" - ДА)
    u16 SelCascad;  // Значение параметра выбора настройки каскадного регулятора ("0" - Нет, "1" - ДА)


    NumState = load_s16(ramPnt, progPnt->Pnt_NumState);
    IDPrmTek = load_s16(ramPnt, progPnt->Pnt__Prm0);
    SelPID = load_s16(ramPnt, progPnt->Pnt_SelPID);
    SelCascad = load_s16(ramPnt, progPnt->Pnt_SelCascad);

    switch (NumState)                                   // Определяем текущее состояние мастера настройки (МН) 
    {
    case (PumpState_level_START):                       // Обработка начального уровня МН "Номинальный ток двигателя"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_1 ])    // Если значение ссылочного параметра не соответствует номеру параметра для данного уровня
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_1 ];     // То присваиваем ссылочному параметру необходимый номер параметра
        }
        NumState = PumpState_level_1;
        break;
    case (PumpState_level_1):                           // Обработка 1-го уровня МН "Номинальный ток двигателя"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_1 ])    // Если значение ссылочного параметра не соответствует номеру параметра для данного уровня
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_1 ];     // То присваиваем ссылочному параметру необходимый номер параметра
        }
        break;
    case (PumpState_level_2):                           // Обработка 2-го уровня МН "Частота пуска"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_2])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_2];
        }
        break;
    case (PumpState_level_3):                           // Обработка 3-го уровня МН "Время фиксации пусковой частоты"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_3])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_3];
        }
        break;
    case (PumpState_level_4):                           // Обработка 4-го уровня МН "Время разгона"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_4])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_4];
        }
        break;
    case (PumpState_level_5):                           // Обработка 5-го уровня МН "Использование ПИД регулятора"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_5])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_5];
        }
        break;
    case (PumpState_level_6):                           // Обработка 6-го уровня МН "Пропорциональный коэффициент"
        if (SelPID)                                     // Если выбрана настройка энкодера
        {
            if (IDPrmTek != uIDPrmPump[ PumpState_level_6]) // То если значение ссылочного параметра не соответствует номеру параметра для данного уровня 
            {
                IDPrmTek = uIDPrmPump[ PumpState_level_6];  // То присваиваем ссылочному параметру необходимый номер параметра
            }
        }
        else
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // Иначе присваиваем ссылочному параметру номер параметра "Использование каскадного контроллера"         
            NumState = PumpState_level_9;
        }
        break;
    case (PumpState_level_7):                           // Обработка 7-го уровня МН "Интегральный коэффициент"
        if (!SelPID)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_5 ];     // То присваиваем ссылочному параметру номер параметра "Использование каскадного контроллера" 
            NumState = PumpState_level_5;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_7])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_7];
        }
        break;
    case (PumpState_level_8):                           // Обработка 8-го уровня МН "Дифференциальный коэффициент"
        if (!SelPID)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_5 ];     // То присваиваем ссылочному параметру номер параметра "Использование каскадного контроллера" 
            NumState = PumpState_level_5;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_8])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_8];
        }
        break;
    case (PumpState_level_9):                           // Обработка 9-го уровня МН "Использование каскадного контроллера"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_9]) // Если значение ссылочного параметра не соответствует номеру параметра для данного уровня 
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9];  // То присваиваем ссылочному параметру необходимый номер параметра
        }
        break;
    case (PumpState_level_10):                          // Обработка 10-го уровня МН "Частота включения асинхронного двигателя"
        if (SelCascad)                                     // Если выбрана настройка энкодера
        {
            if (IDPrmTek != uIDPrmPump[ PumpState_level_10]) // То если значение ссылочного параметра не соответствует номеру параметра для данного уровня 
            {
                IDPrmTek = uIDPrmPump[ PumpState_level_10];  // То присваиваем ссылочному параметру необходимый номер параметра
            }
        }
        else
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_16 ];     // Иначе присваиваем ссылочному параметру номер параметра "Активировать подхват"         
            NumState = PumpState_level_16;
        }
        break;
    case (PumpState_level_11):                          // Обработка 11-го уровня МН "Частота выключения асинхронного двигателя"
        if (!SelCascad)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // То присваиваем ссылочному параметру номер параметра "Активировать подхват" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_11])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_11];
        }
        break;
    case (PumpState_level_12):                          // Обработка 12-го уровня МН "Режим двигателя №1"
        if (!SelCascad)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // То присваиваем ссылочному параметру номер параметра "Активировать подхват" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_12])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_12];
        }
        break;
    case (PumpState_level_13):                          // Обработка 13-го уровня МН "Режим двигателя №2"
        if (!SelCascad)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // То присваиваем ссылочному параметру номер параметра "Активировать подхват" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_13])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_13];
        }
        break;
    case (PumpState_level_14):                          // Обработка 14-го уровня МН "Режим двигателя №3"
        if (!SelCascad)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // То присваиваем ссылочному параметру номер параметра "Активировать подхват" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_14])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_14];
        }
        break;
    case (PumpState_level_15):                          // Обработка 15-го уровня МН "Режим двигателя №4"
        if (!SelCascad)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // То присваиваем ссылочному параметру номер параметра "Активировать подхват" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_15])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_15];
        }
        break;
    case (PumpState_level_16):                           // Обработка 16-го уровня МН "Активировать подхват"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_16])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_16];
        }
        break;
    case (PumpState_level_17):                           // Обработка 17-го уровня МН "Контроль входного напряжения"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_17])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_17];
        }
        break;
    case (PumpState_level_END):                         // Обработка уровня МН при "переполнении"
        NumState = PumpState_level_17;
        break;
    default:                                                // Обработка при выходе номера за допустимые приделы (исключительная ситуация)
        NumState = PumpState_level_1;
        IDPrmTek = uIDPrmPump[ PumpState_level_1 ];
        break;
    }
    save_s16(ramPnt, progPnt->Pnt_NumState,  NumState);
    save_s16(ramPnt, progPnt->Pnt__Prm0,  IDPrmTek);
    save_s16(ramPnt, progPnt->Pnt__Prm1,  IDPrmTek);
    save_s16(ramPnt, progPnt->Pnt__Prm2,  IDPrmTek);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMASTER_SET_PUMP_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  &Prm0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  &Prm1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  &Prm2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  NumState
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  SelPID
        S16_PIN_TYPE | INPUT_PIN_MODE      //6  SelCascad
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MASTER_SET_PUMP", "DD", TblFuncMASTER_SET_PUMP_1, FuncMASTER_SET_PUMP_1, sizeof(TblFuncMASTER_SET_PUMP_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMASTER_SET_PUMP_1,
#endif
//********************************************************
