/************************** (C) Copyright 2013 Triol ***************************
* File Name          : macros_type_task.h
* Author             : Kats Evgeniy
* TS Idea            : 2013.02.25
* TS Version         : 2013.02.26
* Description        : Файл содержит функцию, которая реализует макрос настройки
*                      "типа задания" для насосного исполнения
* Fixed by:          : Andrey Kozlov
* Version            : 2013.09.09
*                    : Исправлено несохранение значений, устаноавливаемых макросом
*                    : путём введения NVState.
*******************************************************************************/
#ifdef    GET_FUNC_CODE
// Структура инициализации макроса
typedef struct{
    u16  ChannalTask1;                  // Канал задания 1
    u16  SrcPID;                        // Источник задания ПИД регулятора
    u16  SrcOS;                         // Источник обратной связи для ПИД регулятора
    u16  Speed1;                        // 1-скорость
    u16  Speed2;                        // 2-скорость
    u16  Speed3;                        // 3-скорость
    u16  Speed4;                        // 4-скорость
    f32  UstFreq1;                      // Частота задания 1
    f32  UstFreq2;                      // Частота задания 2
    f32  UstFreq3;                      // Частота задания 3
    f32  UstFreq4;                      // Частота задания 4
    u16  ActAvtovent;                   // Активация режима автовентиль
    u16  Relay1;                        // Реле 1
    u16  Relay2;                        // Реле 2
    u16  Relay3;                        // Реле 3
    u16  Relay4;                        // Реле 4

    u16  Relay5;                        // Реле 5
    u16  Relay6;                        // Реле 6
    u16  Relay7;                        // Реле 7
    u16  Relay8;                        // Реле 8
    u16  Permis1;                       // Вход разрешения 1
    u16  Permis2;                       // Вход разрешения 2
    u16  Permis3;                       // Вход разрешения 3
    u16  Permis4;                       // Вход разрешения 4
    u16  nvVirgin;                      // Признак того, что макрос вызывался
}PrmOutMacrosTask_type;
#define nvSignat 0x5AA5
// Массив структур инициализации макросов, каждой строке соответствует свой макрос
PrmOutMacrosTask_type PrmOutMacrosTask[] = {
    //Кан зад,Ист.ПИД,Ист.ОС, Скор.1, Скор.2, Скор.3, Скор.4, Част.1, Част.2, Част.3, Част.4, Акт.Авт,Реле1, Реле2,  Реле3,  Реле4,   Реле5,  Реле6,  Реле7,  Реле8, Perm1, Perm2, Perm3, Perm4,  Virgin
    {   1,       0,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,       0,      0,      0,      0,     0,     0,      0,    0,    nvSignat},     // Макрос по умолчанию
    {   10,      6,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,       0,      0,      0,      0,     0,     0,      0,    0,    nvSignat},     // Макрос ПИД регулятор
    {   3,       0,      1,      6,      5,      7,      8,      200.0,  300.0,  400.0,  500.0,   0,      0,      0,      0,      0,       0,      0,      0,     0,     0,     0,      0,    0,    nvSignat},     // Макрос дискретное задание
    {   10,      6,      1,      0,      0,      0,      0,      0,      0,      0,      0,      1,      30,     34,     31,     35,      32,     36,      0,     0,     6,     7,      8,    0,    nvSignat}      // Макрос автовентиль
};
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Type;                         //1  Type
    u16  Pnt_Set;                          //2  Set
    u16  Pnt_Perm4;                        //3  Perm4
    u16  Pnt_Perm3;                        //4  Perm3
    u16  Pnt_Perm2;                        //5  Perm2
    u16  Pnt_Perm1;                        //6  Perm1
    u16  Pnt_Relay8;                       //7  Relay8
    u16  Pnt_Relay7;                       //8  Relay7
    u16  Pnt_Relay6;                       //9  Relay6
    u16  Pnt_Relay5;                       //10  Relay5
    u16  Pnt_Relay4;                       //11  Relay4
    u16  Pnt_Relay3;                       //12  Relay3
    u16  Pnt_Relay2;                       //13  Relay2
    u16  Pnt_Relay1;                       //14  Relay1
    u16  Pnt_ActAvtovent;                  //15  ActCascad
    u16  Pnt_UstFreq4;                     //16  UstFreq4
    u16  Pnt_UstFreq3;                     //17  UstFreq3
    u16  Pnt_UstFreq2;                     //18  UstFreq2
    u16  Pnt_UstFreq1;                     //19  UstFreq1
    u16  Pnt__4_Speed;                     //20  4-Speed
    u16  Pnt__3_Speed;                     //21  3-Speed
    u16  Pnt__2_Speed;                     //22  2-Speed
    u16  Pnt__1_Speed;                     //23  1-Speed
    u16  Pnt_SrcOS;                        //24  SrcOS
    u16  Pnt_SrcPID;                       //25  SrcPID
    u16  Pnt_ChUst1;                       //26  ChUst1
    u16  Pnt_End;
}FuncMACROS_TYPE_TASK_type;
#define     nvSfMacroTask     (*(PrmOutMacrosTask_type*)0)
#define sizeNvTask  (sizeof(PrmOutMacrosTask_type))
//********************************************************
u16 *FuncMACROS_TYPE_TASK_1(FuncMACROS_TYPE_TASK_type *progPnt, u32 ramPnt)
{
    u16 TypeMacros;                     // Тип макроса
    u16 Comand;                         // Команда установки макроса
    s16 UstRecoveryFlagValue;
    void    *nvAdr1;
    PrmOutMacrosTask_type * pPrmOutMacrosTask;  // Указательна на структуру инициализации макроса
    PrmOutMacrosTask_type  *nvAdr, nvMem;
    PrmOutMacrosTask_type   tmpStruct;

//#ifdef _PROJECT_FOR_CCS_
//	#warning !!! ВНИМАНИЕ. ОТКЛЮЧЕН МАКРОС НАСТРОЙКИ ТИПА УПРАВЛЕНИЯ
//#endif
//    return &progPnt->Pnt_End;


    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfMacroTask);
    nvAdr1 = (void *)((u32)&nv.UstRecoveryFlag);

    memset(&nvMem, 0, sizeNvTask);
    memset(&tmpStruct, 0, sizeNvTask);

    if (flagIsFirstCall) {
        // Сбрасываем команду установки макроса
        save_s16(ramPnt, progPnt->Pnt_Set,  ComandSetup_NO);
        RdFramMicro(&nvMem, nvAdr, sizeNvTask);
        RdFramMicro( &UstRecoveryFlagValue, nvAdr1,  sizeof(u16));
        if ((nvMem.nvVirgin == nvSignat)&&(!UstRecoveryFlagValue)){
            Comand = ComandSetup_YES;
            pPrmOutMacrosTask = &nvMem;    // То присваеваем указатель на соответствующую структуру
        }
        else{
            return &progPnt->Pnt_End;
        }
    }
    else{
        tmpStruct.ChannalTask1  = load_s16(ramPnt, progPnt->Pnt_ChUst1);
        tmpStruct.SrcPID        = load_s16(ramPnt, progPnt->Pnt_SrcPID);
        tmpStruct.SrcOS         = load_s16(ramPnt, progPnt->Pnt_SrcOS);
        tmpStruct.Speed1        = load_s16(ramPnt, progPnt->Pnt__1_Speed);
        tmpStruct.Speed2        = load_s16(ramPnt, progPnt->Pnt__2_Speed);
        tmpStruct.Speed3        = load_s16(ramPnt, progPnt->Pnt__3_Speed);
        tmpStruct.Speed4        = load_s16(ramPnt, progPnt->Pnt__4_Speed);
        tmpStruct.UstFreq1      = (f32)load_s32(ramPnt, progPnt->Pnt_UstFreq1);
        tmpStruct.UstFreq2      = (f32)load_s32(ramPnt, progPnt->Pnt_UstFreq2);
        tmpStruct.UstFreq3      = (f32)load_s32(ramPnt, progPnt->Pnt_UstFreq3);
        tmpStruct.UstFreq4      = (f32)load_s32(ramPnt, progPnt->Pnt_UstFreq4);
        tmpStruct.ActAvtovent   = load_s16(ramPnt, progPnt->Pnt_ActAvtovent);
        tmpStruct.Relay1        = load_s16(ramPnt, progPnt->Pnt_Relay1);
        tmpStruct.Relay2        = load_s16(ramPnt, progPnt->Pnt_Relay2);
        tmpStruct.Relay3        = load_s16(ramPnt, progPnt->Pnt_Relay3);
        tmpStruct.Relay4        = load_s16(ramPnt, progPnt->Pnt_Relay4);
        tmpStruct.Relay5        = load_s16(ramPnt, progPnt->Pnt_Relay5);
        tmpStruct.Relay6        = load_s16(ramPnt, progPnt->Pnt_Relay6);
        tmpStruct.Relay7        = load_s16(ramPnt, progPnt->Pnt_Relay7);
        tmpStruct.Relay8        = load_s16(ramPnt, progPnt->Pnt_Relay8);
        tmpStruct.Permis1       = load_s16(ramPnt, progPnt->Pnt_Perm1);
        tmpStruct.Permis2       = load_s16(ramPnt, progPnt->Pnt_Perm2);
        tmpStruct.Permis3       = load_s16(ramPnt, progPnt->Pnt_Perm3);
        tmpStruct.Permis4       = load_s16(ramPnt, progPnt->Pnt_Perm4);
        tmpStruct.nvVirgin      = nvSignat;

        RdFramMicro(&nvMem, nvAdr, sizeNvTask);
        if (memcmp(&nvMem,&tmpStruct,sizeNvTask)){  //если структуры не равны
            memcpy(&nvMem,&tmpStruct,sizeNvTask);   //скопировать в буфер для записи
            WrFramMicro(nvAdr, &nvMem, sizeNvTask); //перезаписать в энергонезависимую
        }
        TypeMacros = load_s16(ramPnt, progPnt->Pnt_Type);
        Comand = load_s16(ramPnt, progPnt->Pnt_Set);
        pPrmOutMacrosTask = &PrmOutMacrosTask[ TypeMacros ];    // То присваеваем указатель на соответствующую структуру

    }


    if (Comand == ComandSetup_YES)                      // Если пришла команда установки макроса управления
    {
        // Инициализируем параметры в соответствии с макросом
        save_s16(ramPnt, progPnt->Pnt_ChUst1,   pPrmOutMacrosTask->ChannalTask1);
        save_s16(ramPnt, progPnt->Pnt_SrcPID,   pPrmOutMacrosTask->SrcPID);
        save_s16(ramPnt, progPnt->Pnt_SrcOS,    pPrmOutMacrosTask->SrcOS);
        save_s16(ramPnt, progPnt->Pnt__1_Speed, pPrmOutMacrosTask->Speed1);
        save_s16(ramPnt, progPnt->Pnt__2_Speed, pPrmOutMacrosTask->Speed2);
        save_s16(ramPnt, progPnt->Pnt__3_Speed, pPrmOutMacrosTask->Speed3);
        save_s16(ramPnt, progPnt->Pnt__4_Speed, pPrmOutMacrosTask->Speed4);

        if (flagIsFirstCall){
            save_s32(ramPnt, progPnt->Pnt_UstFreq1, (s32)pPrmOutMacrosTask->UstFreq1);
            save_s32(ramPnt, progPnt->Pnt_UstFreq2, (s32)pPrmOutMacrosTask->UstFreq2);
            save_s32(ramPnt, progPnt->Pnt_UstFreq3, (s32)pPrmOutMacrosTask->UstFreq3);
            save_s32(ramPnt, progPnt->Pnt_UstFreq4, (s32)pPrmOutMacrosTask->UstFreq4);
        }
        else{
            save_s32(ramPnt, progPnt->Pnt_UstFreq1, _IQ(pPrmOutMacrosTask->UstFreq1/MPOpis.Base[NUM_FREQ_BASE]));
            save_s32(ramPnt, progPnt->Pnt_UstFreq2, _IQ(pPrmOutMacrosTask->UstFreq2/MPOpis.Base[NUM_FREQ_BASE]));
            save_s32(ramPnt, progPnt->Pnt_UstFreq3, _IQ(pPrmOutMacrosTask->UstFreq3/MPOpis.Base[NUM_FREQ_BASE]));
            save_s32(ramPnt, progPnt->Pnt_UstFreq4, _IQ(pPrmOutMacrosTask->UstFreq4/MPOpis.Base[NUM_FREQ_BASE]));
        }

        save_s16(ramPnt, progPnt->Pnt_ActAvtovent, pPrmOutMacrosTask->ActAvtovent);
        save_s16(ramPnt, progPnt->Pnt_Relay1,   pPrmOutMacrosTask->Relay1);
        save_s16(ramPnt, progPnt->Pnt_Relay2,   pPrmOutMacrosTask->Relay2);
        save_s16(ramPnt, progPnt->Pnt_Relay3,   pPrmOutMacrosTask->Relay3);
        save_s16(ramPnt, progPnt->Pnt_Relay4,   pPrmOutMacrosTask->Relay4);
        save_s16(ramPnt, progPnt->Pnt_Relay5,   pPrmOutMacrosTask->Relay5);
        save_s16(ramPnt, progPnt->Pnt_Relay6,   pPrmOutMacrosTask->Relay6);
        save_s16(ramPnt, progPnt->Pnt_Relay7,   pPrmOutMacrosTask->Relay7);
        save_s16(ramPnt, progPnt->Pnt_Relay8,   pPrmOutMacrosTask->Relay8);
        save_s16(ramPnt, progPnt->Pnt_Perm1,   pPrmOutMacrosTask->Permis1);
        save_s16(ramPnt, progPnt->Pnt_Perm2,   pPrmOutMacrosTask->Permis2);
        save_s16(ramPnt, progPnt->Pnt_Perm3,   pPrmOutMacrosTask->Permis3);
        save_s16(ramPnt, progPnt->Pnt_Perm4,   pPrmOutMacrosTask->Permis4);

        // Сбрасываем команду установки макроса
        save_s16(ramPnt, progPnt->Pnt_Set,  ComandSetup_NO);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMACROS_TYPE_TASK_1[26]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Set
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //3  Perm4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Perm3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Perm2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Perm1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Relay8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Relay7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Relay6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10 Relay5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //11 Relay4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12 Relay3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13 Relay2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14 Relay1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //15  ActCascad
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //16  UstFreq4
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //17  UstFreq3
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //18  UstFreq2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //19  UstFreq1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //20  4-Speed
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //21  3-Speed
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //22  2-Speed
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  1-Speed
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  SrcOS
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  SrcPID
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //26  ChUst1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MACROS_TYPE_TASK", "DD", TblFuncMACROS_TYPE_TASK_1, FuncMACROS_TYPE_TASK_1, sizeof(TblFuncMACROS_TYPE_TASK_1), 0, sizeof(PrmOutMacrosTask_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMACROS_TYPE_TASK_1,
#endif
//********************************************************
