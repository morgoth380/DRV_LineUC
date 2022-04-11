/************************** (C) Copyright 2013 Triol ***************************
* File Name          : macros_type_control.h
* Author             : Kats Evgeniy
* TS Idea            : 2013.02.25
* TS Version         : 2013.02.25
* Description        : Файл содержит функцию, которая реализует макрос настройки
*                      "типа управления" для насосного исполнения
* Fixed by:          : Andrey Kozlov
* Version            : 2013.09.09
*                    : Исправлено несохранение значений, устаноавливаемых макросом
*                    : путём введения NVState.
*******************************************************************************/
#ifdef    GET_FUNC_CODE
// Команда установки
typedef enum{
    ComandSetup_NO,
    ComandSetup_YES,
}Comandsetup_type;
// Структура инициализации макроса
typedef struct{
    u16  ChannalControl1;                   // Канал управления1
    u16  Pusk1;                             // Пуск 1
    u16  Pusk2;                             // Пуск 2
    u16  Stop1;                             // Стоп 1
    u16  Stop2;                             // Стоп 2
    u16  PermPusk1;                         // Разрешение Пуска 1
    u16  PermPusk2;                         // Разрешение Пуска 2
    u16  Forward;                           // Вперед
    u16  Revers;                            // Назад
    u16  nvVirgin;                          // Признако того, что макрос вызывался
}PrmOutMacros_type;
#define nvSignat 0x5AA5
// Массив структур инициализации макросов, каждой строке соответствует свой макрос
PrmOutMacros_type PrmOutMacros[] = {
    //Канал Упр., Пуск1,  Пуск2,  Стоп1,  Стоп2,  Разреш Пуска 1, Разреш Пуска 2, Вперед, Назад,    Virgin
    {   1,          0,      0,      0,      0,      0,              0,              0,      0,      nvSignat},    // Макрос управления с местного пульта (по умолчанию)
    {   3,          1,      2,      0,      0,      1,              2,              1,      2,      nvSignat},    // Макрос двухпроводного управления
    {   3,          1,      2,      3,      3,      0,              0,              1,      2,      nvSignat}     // Макрос трехпроводного управления
};

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Type;                         //1  Type
    u16  Pnt_Set;                          //2  Set
    u16  Pnt_Revers;                       //3  Revers
    u16  Pnt_Forward;                      //4  Forward
    u16  Pnt_PermPusk2;                    //5  PermPusk2
    u16  Pnt_PermPusk1;                    //6  PermPusk1
    u16  Pnt_Stop2;                        //7  Stop2
    u16  Pnt_Stop1;                        //8  Stop1
    u16  Pnt_Pusk2;                        //9  Pusk2
    u16  Pnt_Pusk1;                        //10  Pusk1
    u16  Pnt_ChCntr1;                      //11  ChCntr1
    u16  Pnt_End;
}FuncMACROS_TYPE_CONTROL_type;
#define     nvSfMacroTypeControl     (*(PrmOutMacros_type*)0)
#define sizeNvTypeControl  (sizeof(PrmOutMacros_type))
//********************************************************
u16 *FuncMACROS_TYPE_CONTROL_1(FuncMACROS_TYPE_CONTROL_type *progPnt, u32 ramPnt)
{
    u16 TypeMacros;                     // Тип макроса
    u16 Comand;                         // Команда установки макроса
    s16 UstRecoveryFlagValue;
    void    *nvAdr1;

    PrmOutMacros_type * pPrmOutMacros;  // Указательна на структуру инициализации макроса
    PrmOutMacros_type  *nvAdr, nvMem;
    PrmOutMacros_type   tmpStruct;

//#ifdef _PROJECT_FOR_CCS_
//	#warning !!! ВНИМАНИЕ. ОТКЛЮЧЕН МАКРОС НАСТРОЙКИ ТИПА УПРАВЛЕНИЯ
//#endif
//    return &progPnt->Pnt_End;

    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfMacroTypeControl);
    nvAdr1 = (void *)((u32)&nv.UstRecoveryFlag);

    memset(&nvMem, 0, sizeNvTypeControl);
    memset(&tmpStruct, 0, sizeNvTypeControl);

    if (flagIsFirstCall){
        // Сбрасываем команду установки макроса
        save_s16(ramPnt, progPnt->Pnt_Set,  ComandSetup_NO);
        RdFramMicro(&nvMem, nvAdr, sizeNvTypeControl);
        RdFramMicro( &UstRecoveryFlagValue, nvAdr1,  sizeof(u16));
        if ((nvMem.nvVirgin == nvSignat)&&(!UstRecoveryFlagValue)){
            Comand = ComandSetup_YES;
            pPrmOutMacros = &nvMem;    // То присваеваем указатель на соответствующую структуру
        }
        else{
            return &progPnt->Pnt_End;
        }
    }
    else{
        tmpStruct.ChannalControl1   = load_s16(ramPnt, progPnt->Pnt_ChCntr1);
        tmpStruct.Pusk1             = load_s16(ramPnt, progPnt->Pnt_Pusk1);
        tmpStruct.Pusk2             = load_s16(ramPnt, progPnt->Pnt_Pusk2);
        tmpStruct.Stop1             = load_s16(ramPnt, progPnt->Pnt_Stop1);
        tmpStruct.Stop2             = load_s16(ramPnt, progPnt->Pnt_Stop2);
        tmpStruct.PermPusk1         = load_s16(ramPnt, progPnt->Pnt_PermPusk1);
        tmpStruct.PermPusk2         = load_s16(ramPnt, progPnt->Pnt_PermPusk2);
        tmpStruct.Forward           = load_s16(ramPnt, progPnt->Pnt_Forward);
        tmpStruct.Revers            = load_s16(ramPnt, progPnt->Pnt_Revers);
        tmpStruct.nvVirgin          = nvSignat;

        RdFramMicro(&nvMem, nvAdr, sizeNvTypeControl);
        if (memcmp(&nvMem,&tmpStruct,sizeNvTypeControl)){  //если структуры не равны
            memcpy(&nvMem,&tmpStruct,sizeNvTypeControl);   //скопировать в буфер для записи
            WrFramMicro(nvAdr, &nvMem, sizeNvTypeControl); //перезаписать в энергонезависимую
        }
        TypeMacros = load_s16(ramPnt, progPnt->Pnt_Type);
        Comand = load_s16(ramPnt, progPnt->Pnt_Set);
        pPrmOutMacros = &PrmOutMacros[ TypeMacros ];    // То присваеваем указатель на соответствующую структуру
    }
    
    if (Comand == ComandSetup_YES)                      // Если пришла команда установки макроса управления
    {

        // Инициализируем параметры в соответствии с макросом
        save_s16(ramPnt, progPnt->Pnt_ChCntr1,  pPrmOutMacros->ChannalControl1);
        save_s16(ramPnt, progPnt->Pnt_Pusk1,    pPrmOutMacros->Pusk1);
        save_s16(ramPnt, progPnt->Pnt_Pusk2,    pPrmOutMacros->Pusk2);
        save_s16(ramPnt, progPnt->Pnt_Stop1,    pPrmOutMacros->Stop1);
        save_s16(ramPnt, progPnt->Pnt_Stop2,    pPrmOutMacros->Stop2);
        save_s16(ramPnt, progPnt->Pnt_PermPusk1,pPrmOutMacros->PermPusk1);
        save_s16(ramPnt, progPnt->Pnt_PermPusk2,pPrmOutMacros->PermPusk2);
        save_s16(ramPnt, progPnt->Pnt_Forward,  pPrmOutMacros->Forward);
        save_s16(ramPnt, progPnt->Pnt_Revers,   pPrmOutMacros->Revers);
        // Сбрасываем команду установки макроса
        save_s16(ramPnt, progPnt->Pnt_Set,  ComandSetup_NO);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMACROS_TYPE_CONTROL_1[11]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Set
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Revers
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Forward
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  PermPusk2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  PermPusk1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Stop2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Stop1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Pusk2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Pusk1
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //11  ChCntr1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MACROS_TYPE_CONTROL", "DD", TblFuncMACROS_TYPE_CONTROL_1, FuncMACROS_TYPE_CONTROL_1, sizeof(TblFuncMACROS_TYPE_CONTROL_1), 0, sizeof(PrmOutMacros_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMACROS_TYPE_CONTROL_1,
#endif
//********************************************************
