#ifdef    GET_FUNC_CODE
#include    "apv_type.h"
                                                                 //Tcycl = 0.004 cek => Tcycl x 250 = 1 cek.

#ifndef _PROJECT_FOR_CCS_
#include "GUI.h"
#endif

typedef struct{
    u16         ResetState:1;                                                                   //Предыдущее состояние входного сигнала Сброс счётчика АПВ         
    u16         FailState:1;                                                                    //Предыдущее состояние выходного сигнала Ошибка                    
    u16         FlagApvState:1;                                                                 //Флаг - событие АПВ, сбрасывается общим блоком формирования АПВ   
    u16         WaitNorm:1;                                                                                                                                        
}BitField_type;                                                                                                                                                    
                                                                                                                                                                   
typedef struct{                                                                                                                                                    
    u16              CntDeleyControlState;                                 //Задержка, в секундах, запрет обработки ошибок на время пуска     
    u32              CntDeleyOffState;                                     //Задержка ,в секундах, выдачи сигнала Авария                      
    u16              CntApvState;                    
    u32              TimeToAPV;                                            //Счётчик количества АПВ
    u16              StateProg;
    BitField_type    BitField;
}LOCAL_APVState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Fail_IN;                      //01 Fail_IN                    //Вход сигнала Ошибка
    u16  Pnt_Off_Block_Apv;                //02 Off/Block/Apv 0-1-2        //Вход сигнала управления формированием АПВ
    u16  Pnt_DeleyControl;                 //03 DeleyControl               //Задержка на время пуска
    u16  Pnt_DeleyOff;                     //04 DeleyOff                   //Задержка формирования ошибки
    u16  Pnt_NormalWait;                   //05 NormalWait                 //Не используется
    u16  Pnt_ApvTime_sec;                  //06 ApvTime_sec                //Уставка Время АПВ сек.
    u16  Pnt_ApvTime_min;                  //07 ApvTime_min                //Уставка время АПВ мин. Тапв = ( мин + сек )
    u16  Pnt_KolvApv;                      //08 KolvApv                    //Уставка Допустимое количаство АПВ
    u16  Pnt_ResetApv;                     //09 ResetApv                   //Вход Сброс счётчика АПВ
    u16  Pnt_On_Off;                       //10 On/Off                     //Включить\выключить блок АПВ
    u16  Pnt_RTC;                          //11 UnixTime                   //Реальное время контроллера
    u16  Pnt_ApvCnt;                       //12 ApvCnt                     //Выход счётчика АПВ
    u16  Pnt_Warning;                      //13 Warning                    //Выход предупреждение о Аварии
    u16  Pnt_Fail;                         //14 Fail                       //Выход Авария
    u16  Pnt_End;
}FuncLOCAL_APV_type;
//********************************************************
//extern  FuncLOCAL_APV_type *APVpnt[MAX_BLOCK_COUNT];
//********************************************************
u16 *FuncLOCAL_APV_1(FuncLOCAL_APV_type *progPnt, u32 ramPnt)
{

    LOCAL_APVState_type             *sPnt;
    s16                             i, fail, numApv;
    u32 UnixTime;
    u16 Tmp;
    
    sPnt = (LOCAL_APVState_type *)(ramPnt + progPnt->Pnt_State);
    
    // Записатся в структуру
#ifndef _PROJECT_FOR_CCS_
        if (StateTestMicroProg != 0){                                                 //Защита от проверок в Visual при отладке
        return &progPnt->Pnt_End;
    }
#endif
    
    if(flagIsFirstCall){
        sPnt->StateProg = 0;
        sPnt->BitField.FailState = 0;
        sPnt->BitField.ResetState = 0;
        sPnt->BitField.FlagApvState = 0;
        sPnt->BitField.WaitNorm = 0;
#ifndef _PROJECT_FOR_CCS_
    if (StateTestMicroProg == 0){                                                 //Защита от проверок в Visual при отладке
#endif
        for(i = 0;(i < MAX_BLOCK_COUNT) && (APVpnt[i] != NULL);i++);       //Ищу свободное место в структуре
            if(i < MAX_BLOCK_COUNT)                                            //Если место есть, записываю туда свой адрес
                APVpnt[i] = progPnt;

        sPnt->CntDeleyControlState = 0;
        sPnt->CntDeleyOffState = 0;
    }
#ifndef _PROJECT_FOR_CCS_
  }
#endif
//============================================================================================================= 
    if (sPnt->CntDeleyControlState < 10 && sPnt->StateProg == 0)
        sPnt->CntDeleyControlState ++;
    else
        sPnt->StateProg = 1;

    if(sPnt->StateProg == 1){                               //Задержка для вычитывания нормального времени
        UnixTime = load_s32(ramPnt, progPnt->Pnt_RTC);
        i = testBitMicro(ramPnt, progPnt->Pnt_On_Off);
        
        if((testBitMicro(ramPnt, progPnt->Pnt_On_Off) == 0) || (load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv) == 0)){ //Пуск есть, сигнал Off отсутствует           
            sPnt->CntDeleyControlState = 0;
            sPnt->CntDeleyOffState = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_Warning);
            clrBitMicro(ramPnt, progPnt->Pnt_Fail);
        }
        else{
            if((sPnt->CntDeleyControlState) < load_s16(ramPnt, progPnt->Pnt_DeleyControl) * CYCLE_FACTOR){
                sPnt->CntDeleyControlState++;
            }
            else{
                if(testBitMicro(ramPnt, progPnt->Pnt_Fail_IN) == 0){                                    //Ошибка?
                    clrBitMicro(ramPnt, progPnt->Pnt_Warning);
                    clrBitMicro(ramPnt, progPnt->Pnt_Fail);
                    sPnt->BitField.FailState = 0;
                    if( sPnt->CntDeleyOffState ) sPnt->CntDeleyOffState--;                              // Отнимаем счетчик
                }
                else{
                    setBitMicro(ramPnt, progPnt->Pnt_Warning);                                          //Выдаю предупреждение
                    if((sPnt->CntDeleyOffState) < load_s16(ramPnt, progPnt->Pnt_DeleyOff) * CYCLE_FACTOR)
                        sPnt->CntDeleyOffState++;
                        
                    else{
                        setBitMicro(ramPnt, progPnt->Pnt_Fail);                                         //Выдаю ошибку
                        fail = 1;
                        sPnt->BitField.FailState = 1;
                    }
                }
            }
//**********************************************************
//Тестирование на количество АПВ
//**********************************************************
        numApv = load_s16(ramPnt, progPnt->Pnt_KolvApv);                    

        if( (sPnt->CntApvState < numApv || numApv == 0)&& (load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv)==2)){ //Если допустимо количество АПВ || уставка = 0? -> бесконечное АПВ
            if(load_s16(ramPnt, progPnt->Pnt_NormalWait) == 0){                                     // и нет сигнала "ожидать нормализации"
                if((sPnt->BitField.FailState == 0) && (sPnt->BitField.FlagApvState == 0) && fail ){  // Проверяю наличие фронта аварии

                    if( load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv) > 1){                                       //Если нет сигнала блокировки Block                                
                        sPnt->CntApvState++;                                                                        //Считаю количество отработаных аварий
                        sPnt->BitField.FlagApvState = 1;                                                        //Взвожу флаг АПВ, сброс общим блоком 
                        sPnt->TimeToAPV  = (s32)load_s16(ramPnt, progPnt->Pnt_ApvTime_min);                     //Читаю уставку времени АПВ
                        sPnt->TimeToAPV  = sPnt->TimeToAPV * 60 + (s32)load_s16(ramPnt, progPnt->Pnt_ApvTime_sec);//В секундах
                        sPnt->TimeToAPV += UnixTime;
                    }
                }
            }
        }

        if(!sPnt->BitField.ResetState && testBitMicro(ramPnt, progPnt->Pnt_ResetApv)){
            sPnt->CntApvState = 0;
            sPnt->BitField.FailState = 0;
        }
        
        save_s16(ramPnt, progPnt->Pnt_ApvCnt, sPnt->CntApvState);
        sPnt->BitField.ResetState = testBitMicro(ramPnt, progPnt->Pnt_ResetApv);
        sPnt->BitField.FailState = testBitMicro(ramPnt, progPnt->Pnt_Fail);                     //Запоминаем состояние ошибки, для отлавливания фронтов
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLOCAL_APV_1[14]={
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
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12 ApvCnt
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //13 Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //14 Fail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LOCAL_APV", "DA", TblFuncLOCAL_APV_1, FuncLOCAL_APV_1, sizeof(TblFuncLOCAL_APV_1),sizeof(LOCAL_APVState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLOCAL_APV_1,
#endif
//********************************************************
