/************************** (C) Copyright 2013 Triol ***************************
* File Name          : master_set_lift.h
* Author             : Kats Evgeniy
* TS Idea            : 2013.02.19
* TS Version         : 2013.02.20
* Description        : Файл содержит функцию, которая реализует мастер настройки
*                      для лифтового исполнения                        
*******************************************************************************/
#ifdef    GET_FUNC_CODE

// Стадии мастера настройки
typedef enum{
    LiftState_level_START,
    LiftState_level_1,
    LiftState_level_2,
    LiftState_level_3,
    LiftState_level_4,
    LiftState_level_5,
    LiftState_level_6,
    LiftState_level_7,
    LiftState_level_8,
    LiftState_level_9,
    LiftState_level_10,
    LiftState_level_11,
    LiftState_level_END,
}LiftState_type;

// Номера параметров для каждой стадии мастера настройки
const u16 uIDPrm[] = {
        0x0000,             //Начальное смещение
        0x0000,             //00.00 Номинальный ток двигателя
        0x0003,             //00.03 Номинальное напряжение асинхронного двигателя
        0x0002,             //00.02 Номинальная частота асинхронного двигателя
        0x0001,             //00.01 Номинальная скорость асинхронного двигателя
        0x0004,             //00.04 Число пар полюсов асинхронного двигателя
        0x1219,             //36.25 Максимальный фазный выходной ток, действующее значение
        0x0086,             //01.06 Момент инерции
        0x1706,             //46.06 Тип управления (использование энкодера)
        0x0008,             //00.08 Тип энкодера
        0x0006,             //00.06 Число меток (импульсов) на оборот
        0x0005,             //00.05 Фразировка энкодера
};
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt__Prm0;                         //1  &Prm
    u16  Pnt__Prm1;                         //1  &Prm
    u16  Pnt__Prm2;                         //1  &Prm
    u16  Pnt_NumState;                     //2  NumState
    u16  Pnt_SelectEnc;                    //3  SelectEnc
    u16  Pnt_End;
}FuncMASTER_SET_LIFT_type;
//********************************************************
u16 *FuncMASTER_SET_LIFT_1(FuncMASTER_SET_LIFT_type *progPnt, u32 ramPnt)
{   
    u16 NumState;   // Текущее состояние мастера настройки
    u16 IDPrmTek;   // Текущее значение ссылочного параметра (номер редактируемого параметра в мастере) 
    u16 SelEnc;     // Значение параметра выбора настройки энкодера ("0" - Нет, "1" - ДА)


    NumState = load_s16(ramPnt, progPnt->Pnt_NumState);
    IDPrmTek = load_s16(ramPnt, progPnt->Pnt__Prm0);
    SelEnc = load_s16(ramPnt, progPnt->Pnt_SelectEnc);

    switch (NumState)                                   // Определяем текущее состояние мастера настройки (МН) 
    {
    case (LiftState_level_START):                       // Обработка начального уровня МН "Номинальный ток двигателя"
        if (IDPrmTek != uIDPrm[ LiftState_level_1 ])    // Если значение ссылочного параметра не соответствует номеру параметра для данного уровня
        {
            IDPrmTek = uIDPrm[ LiftState_level_1 ];     // То присваиваем ссылочному параметру необходимый номер параметра
        }
        NumState = LiftState_level_1;
        break;
    case (LiftState_level_1):                           // Обработка 1-го уровня МН "Номинальный ток двигателя"
        if (IDPrmTek != uIDPrm[ LiftState_level_1 ])    // Если значение ссылочного параметра не соответствует номеру параметра для данного уровня
        {
            IDPrmTek = uIDPrm[ LiftState_level_1 ];     // То присваиваем ссылочному параметру необходимый номер параметра
        }
        break;
    case (LiftState_level_2):                           // Обработка 2-го уровня МН "Номинальное напряжение асинхронного двигателя"
        if (IDPrmTek != uIDPrm[ LiftState_level_2])
        {
            IDPrmTek = uIDPrm[ LiftState_level_2];
        }
        break;
    case (LiftState_level_3):                           // Обработка 3-го уровня МН "Номинальная частота асинхронного двигателя"
        if (IDPrmTek != uIDPrm[ LiftState_level_3])
        {
            IDPrmTek = uIDPrm[ LiftState_level_3];
        }
        break;
    case (LiftState_level_4):                           // Обработка 4-го уровня МН "Номинальная скорость асинхронного двигателя"
        if (IDPrmTek != uIDPrm[ LiftState_level_4])
        {
            IDPrmTek = uIDPrm[ LiftState_level_4];
        }
        break;
    case (LiftState_level_5):                           // Обработка 5-го уровня МН "Число пар полюсов асинхронного двигателя"
        if (IDPrmTek != uIDPrm[ LiftState_level_5])
        {
            IDPrmTek = uIDPrm[ LiftState_level_5];
        }
        break;
    case (LiftState_level_6):                           // Обработка 6-го уровня МН "Максимальный фазный выходной ток, действующее значение"
        if (IDPrmTek != uIDPrm[ LiftState_level_6])
        {
            IDPrmTek = uIDPrm[ LiftState_level_6];
        }
        break;
    case (LiftState_level_7):                           // Обработка 7-го уровня МН "Момент инерции"
        if (IDPrmTek != uIDPrm[ LiftState_level_7])
        {
            IDPrmTek = uIDPrm[ LiftState_level_7];
        }
        break;
    case (LiftState_level_8):                           // Обработка 8-го уровня МН "Тип управления (использование энкодера)"
        if (IDPrmTek != uIDPrm[ LiftState_level_8])
        {
            IDPrmTek = uIDPrm[ LiftState_level_8];
        }
        break;
    case (LiftState_level_9):                           // Обработка 9-го уровня МН "Тип энкодера"
        if (SelEnc)                                     // Если выбрана настройка энкодера
        {
            if (IDPrmTek != uIDPrm[ LiftState_level_9]) // То если значение ссылочного параметра не соответствует номеру параметра для данного уровня 
            {
                IDPrmTek = uIDPrm[ LiftState_level_9];  // То присваиваем ссылочному параметру необходимый номер параметра
            }
        }
        else
        {
            IDPrmTek = uIDPrm[ LiftState_level_8 ];     // Иначе присваиваем ссылочному параметру номер параметра "Тип управления"         
            NumState = LiftState_level_8;
        }
        break;
    case (LiftState_level_10):                          // Обработка 10-го уровня МН "Число меток (импульсов) на оборот"
        if (!SelEnc)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrm[ LiftState_level_8 ];     // То присваиваем ссылочному параметру номер параметра "Тип управления" 
            NumState = LiftState_level_8;
        }
        else if (IDPrmTek != uIDPrm[ LiftState_level_10])
        {
            IDPrmTek = uIDPrm[ LiftState_level_10];
        }
        break;
    case (LiftState_level_11):                          // Обработка 11-го уровня МН "Фразировка энкодера"
        if (!SelEnc)                                    // Если выбрана настройка без энкодера
        {
            IDPrmTek = uIDPrm[ LiftState_level_8 ];     // То присваиваем ссылочному параметру номер параметра "Тип управления" 
            NumState = LiftState_level_8;
        }
        else if (IDPrmTek != uIDPrm[ LiftState_level_11])
        {
            IDPrmTek = uIDPrm[ LiftState_level_11];
        }
        break;
    case (LiftState_level_END):                         // Обработка уровня МН при "переполнении"
        if (!SelEnc)
        {
            IDPrmTek = uIDPrm[ LiftState_level_8 ];
            NumState = LiftState_level_8;
        }
        else if (IDPrmTek != uIDPrm[ LiftState_level_11])
        {
            IDPrmTek = uIDPrm[ LiftState_level_11 ];
            NumState = LiftState_level_11;
        }
        else
        {
            NumState = LiftState_level_11;
        }
        break;
    default:                                                // Обработка при выходе номера за допустимые приделы (исключительная ситуация)
        NumState = LiftState_level_1;
        IDPrmTek = uIDPrm[ LiftState_level_1 ];
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
const char TblFuncMASTER_SET_LIFT_1[5]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  &Prm0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  &Prm1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  &Prm2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  NumState
        S16_PIN_TYPE | INPUT_PIN_MODE      //5  SelectEnc
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MASTER_SET_LIFT", "DD", TblFuncMASTER_SET_LIFT_1, FuncMASTER_SET_LIFT_1, sizeof(TblFuncMASTER_SET_LIFT_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMASTER_SET_LIFT_1,
#endif
//********************************************************
