#ifdef    GET_FUNC_CODE

typedef union{                          // ставится из обработчика прерывания ШИМ
    struct {
        u16  Up                         : 1;    // Кнопка вверх нажата
        u16  Down                       : 1;    // Кнопка вниз нажата
        u16  FixSpd	                : 1;    // Нажата кнопка фиксированной скорости в режиме UpDown
        u16  UpClamp		        : 1;    // Кнопка вверх ЗАЖАТА
        u16  DownClamp                  : 1;    // Кнопка Вниз  ЗАЖАТА
        u16  FixSpdClamp	        : 1;    // Кнопка Фикс. скорости зажата
    }bit;
    u16     all;
} DinUpDownState_type;

typedef struct{
    float	UpDwnRef;
    DinUpDownState_type		DinPrevState;
    u16		UpPressCnt;
    u16		DownPressCnt;
    u16		FixedSpdMode;
    u16         SyncFtekCnt;
}DIN_UP_DOWN_State_type;

// Структура выравнивается к 4 байтам, поэтому добавлен лишний u16
typedef struct{
	f32     UpDwnRef;                       // Значение режима - Ручной / Авто
        u16     reserved;                       
        u16     crc;
}nvDIN_UP_DOWN;

// Структура выравнивается к 4 байтам, поэтому добавлен лишний u16
typedef enum{
	RESET_BY_STOP = 0,       
        RESTORE_VALUE = 1,
}DIN_UP_DWN_mode;

#define         nvDinUpDown    (*(nvDIN_UP_DOWN*)0)
#define         F_TEK_SYNC_TIME         K_TIME_CALL_MICRO_TAKT // Синхронизация каждую секунду
// Задание Цифрового потенциометра всегда в диапазоне 0-100, а потом я его пересчитываю в что-то другое
#define         UpDwnMax        1.0F  // Максимум 100%
#define         UpDwnMin        0.0F    // Минимум 0 %

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Din_UP;                       //1  Din_UP
    u16  Pnt_Din_Down;                     //2  Din_Down
    u16  Pnt_TEMP_UP;                      //3  TEMP_UP
    u16  Pnt_TEMP_DOWN;                    //4  TEMP_DOWN
    u16  Pnt_Fmin;                         //5  Fmin
    u16  Pnt_Fmax;                         //6  Fmax
    u16  Pnt_PwmOn;                        //7  PwmOn
    u16  Pnt_AccTime;                      //8  AccTime
    u16  Pnt_DecTime;                      //9  DecTime
    u16  Pnt_PresetVal;                    //10 PresetVal
    u16  Pnt_PresetMode;                   //11 PresetMode
    u16  Pnt_Out;                          //12  Out
    u16  Pnt_Fout;                         //13  Fout
    u16  Pnt_PIDout;                       //14  PIDout
    u16  Pnt_End;
}FuncDIN_UP_DOWN_type;
//********************************************************
u16 *FuncDIN_UP_DOWN_1(FuncDIN_UP_DOWN_type *progPnt, u32 ramPnt)
{
    DIN_UP_DOWN_State_type    *sPnt;
    u16 MaxCntValue;
    u16 Tacc, Tdec, crc, PwmOn;
    f32 deltaRef, TempUP, TempDOWN;
    f32 Fmin, Fmax;
    f32 Fout, PIDOut, PresetVal;
    nvDIN_UP_DOWN     *nvAdr, nvMem;
    DIN_UP_DWN_mode PresetMode;
    
    DinUpDownState_type		DinState, Fall;
    sPnt = (DIN_UP_DOWN_State_type *)(ramPnt + progPnt->Pnt_State);
    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvDinUpDown);
    
    PresetMode = (DIN_UP_DWN_mode)load_s16(ramPnt, progPnt->Pnt_PresetMode);
    PwmOn = testBitMicro(ramPnt, progPnt->Pnt_PwmOn);
    PresetVal = load_float(ramPnt, progPnt->Pnt_PresetVal);
    
    //загрузка переменных
    if (flagIsFirstCall){
      RdFramMicro(&nvMem, nvAdr, sizeof(nvDIN_UP_DOWN));            
      crc = crcPacket((unsigned char *)&nvMem,  sizeof(nvDIN_UP_DOWN) - sizeof(u16));
      if (crc == nvMem.crc ) {
        sPnt->UpDwnRef = nvMem.UpDwnRef;
      } else {
        sPnt->UpDwnRef = 0;
      }
      sPnt->DinPrevState.all = 0;
      sPnt->DownPressCnt = sPnt->UpPressCnt = 0;
      sPnt->FixedSpdMode = 0;
      sPnt->SyncFtekCnt = F_TEK_SYNC_TIME; 

      // Проверяем режим работы 
      if (PresetMode == RESTORE_VALUE) {
        // Восстанавливаем значение в задание предустановки
        PresetVal = sPnt->UpDwnRef;
        save_float(ramPnt, progPnt->Pnt_PresetVal, sPnt->UpDwnRef); // Пересохранить значение в предзадание
      }      
    }
      
    // Сбросить задание - если я в останове и выбранн данный режим
    if (PresetMode == RESET_BY_STOP && !PwmOn) {
      sPnt->UpDwnRef = 0;
    } 
    
    // Проверяем предустановку цифрового потенциометра
    // Если ШИМ выключен и задание предустановки отличается от текущего, и активирован режим восстановления
    // То перезаписываем значение предустановки в текущее задание
    if (!PwmOn && PresetVal != sPnt->UpDwnRef && PresetMode == RESTORE_VALUE) {
      sPnt->UpDwnRef = PresetVal;
    }
        
    MaxCntValue = 2 * K_TIME_CALL_MICRO_TAKT; // Время зажатия клавиш 2 секунды
    
    TempUP      = load_float(ramPnt, progPnt->Pnt_TEMP_UP);
    TempDOWN    = load_float(ramPnt, progPnt->Pnt_TEMP_DOWN);
    // Округление флотов до целых значений процентов (0,1%)
    TempDOWN    = (u16)((TempDOWN * 1000) + 0.5);
    TempDOWN    = TempDOWN / 1000.0; 
    
    Fmin        = load_float(ramPnt, progPnt->Pnt_Fmin);
    Fmax        = load_float(ramPnt, progPnt->Pnt_Fmax);
    Tacc        = load_s16(ramPnt, progPnt->Pnt_AccTime)/10;
    Tdec        = load_s16(ramPnt, progPnt->Pnt_DecTime)/10;
        
    // Текущее состояние кнопок UP/Down
    DinState.all        = 0;
    DinState.bit.Up     = testBitMicro(ramPnt, progPnt->Pnt_Din_UP) ? 1 : 0;
    DinState.bit.Down   = testBitMicro(ramPnt, progPnt->Pnt_Din_Down) ? 1 : 0;
    DinState.bit.FixSpd = 0;

    Fall.all = ((sPnt->DinPrevState.all) & (~DinState.all));
    
    // Запоминаем предыдущее состояние кнопок
    sPnt->DinPrevState = DinState;
    
    // Если счетчик досчитал - клавиша зажата
    if (sPnt->UpPressCnt >= MaxCntValue){
            DinState.bit.UpClamp = 1;
            sPnt->UpPressCnt = MaxCntValue;
    }
    if (sPnt->DownPressCnt >= MaxCntValue){
            DinState.bit.DownClamp = 1;
            sPnt->DownPressCnt = MaxCntValue;
    }

    //Если состояние клавиш не поменялось и равно 1 - счетчик зажатия начинает тикать
    if (sPnt->DinPrevState.bit.Up && DinState.bit.Up){
            sPnt->UpPressCnt ++;
    } else {
            if (DinState.bit.UpClamp)
              Fall.bit.Up = 0;
            sPnt->UpPressCnt = 0;
            DinState.bit.UpClamp = 0; // Сбросить зажатие, если было
    }
    if (sPnt->DinPrevState.bit.Down && DinState.bit.Down ){
            sPnt->DownPressCnt ++;
    } else{
            if (DinState.bit.DownClamp)
              Fall.bit.Down = 0;
            sPnt->DownPressCnt = 0;
            DinState.bit.DownClamp = 0; // Сбросить зажатие, если было
    }
//*****************************************************************************************//
    // Изменение значений задания от 0 до 100%
    if (Fall.bit.Up && !DinState.bit.DownClamp) {
            deltaRef = (f32)TempUP;// %
            sPnt->UpDwnRef += (deltaRef + 0.000005);
    }

    if (Fall.bit.Down && !DinState.bit.UpClamp) {
            deltaRef = (f32)TempDOWN; // %
            sPnt->UpDwnRef -= (deltaRef+0.000005);
            DinState.bit.DownClamp = 0;
    }

    if (DinState.bit.UpClamp){
            deltaRef = UpDwnMax / (f32)K_TIME_CALL_MICRO_TAKT / (f32)(1.0*Tacc);
            sPnt->UpDwnRef += deltaRef;
    }
    if (DinState.bit.DownClamp){
            deltaRef = UpDwnMax / (f32)K_TIME_CALL_MICRO_TAKT / (f32)(1.0*Tdec);
            sPnt->UpDwnRef -= deltaRef;
    }
//*****************************************************************************************//
    // Ограничиваем выходное значение
    if (sPnt->UpDwnRef < UpDwnMin){
      sPnt->UpDwnRef = UpDwnMin;
    }
    else{
      if (sPnt->UpDwnRef > UpDwnMax){
        sPnt->UpDwnRef = UpDwnMax;
      }
    }    
//*****************************************************************************************//    
    // Пересчет значения в выходную частоту или задание для ПИДа
    // Значение задания для PID регулятора
    PIDOut = sPnt->UpDwnRef; // Просто проценты 
    
    // Расчет знчаения задания частоты
    Fout = (Fmax - Fmin)/UpDwnMax * sPnt->UpDwnRef + Fmin; // Пересчет в частоту
    
//    if (sPnt->UpDwnRef < Fmin){
//        sPnt->UpDwnRef = Fmin;
//    }
//    else{
//        if (sPnt->UpDwnRef > Fmax){
//            sPnt->UpDwnRef = Fmax;
//        }
//    }
    // Выдаем сигналы на выход    
    save_float(ramPnt, progPnt->Pnt_Out, sPnt->UpDwnRef);
    save_float(ramPnt, progPnt->Pnt_PresetVal, sPnt->UpDwnRef); // Пересохранить значение в предзадание
    save_float(ramPnt, progPnt->Pnt_Fout, Fout);
    save_float(ramPnt, progPnt->Pnt_PIDout, PIDOut);
    
    // Уменьшаем счетчик синхронизации данных текущей скорости/задания      
    if (sPnt->SyncFtekCnt)  {
      sPnt->SyncFtekCnt--;
    } else {
      sPnt->SyncFtekCnt = F_TEK_SYNC_TIME; // Перезарядка
      RdFramMicro(&nvMem, nvAdr, sizeof(nvDIN_UP_DOWN));
      // Сравниваем значение - надо сохранять или нет.
      if (sPnt->UpDwnRef != nvMem.UpDwnRef) {        
        nvMem.UpDwnRef = sPnt->UpDwnRef;
        nvMem.crc      = crcPacket((unsigned char *)&nvMem,  sizeof(nvDIN_UP_DOWN) - sizeof(u16));
        WrFramMicro(nvAdr, &nvMem, sizeof(nvDIN_UP_DOWN));
      }
    }
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDIN_UP_DOWN_1[14]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Din_UP
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Din_Down
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  TEMP_UP
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  TEMP_DOWN
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Fmin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Fmax
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  ON
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  ACC_TIME
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  DEC_TIME
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  PresetVal
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  PresetMode
        
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //12  Out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //13  Fout
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //14  PIDout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DIN_UP_DOWN", "U", TblFuncDIN_UP_DOWN_1, FuncDIN_UP_DOWN_1, sizeof(TblFuncDIN_UP_DOWN_1), sizeof(DIN_UP_DOWN_State_type), sizeof(nvDIN_UP_DOWN), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDIN_UP_DOWN_1,
#endif
//********************************************************
