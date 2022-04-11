#ifdef    GET_FUNC_CODE

#include "Filter.h"


 typedef struct {
    f32     Ki;
    f32     Kd;
    f32     Prescal;
    f32     FiltrIn_pred;
    f32     Sum;
    f32     Lock_temp;
    s16     Ti_pred;
    s16     Td_pred;
    s16     TimeScale_pred;
    s16     BypassActive;
    f32     Kp;
    f32     T;
    u32     Timer;
    f32     Out_prev;
}FuncPID_IQState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In   Задание
    u16  Pnt_FB;                           //2  FB   Обратная связь
    u16  Pnt_Ti;                           //3  Ti
    u16  Pnt_Kp;                           //4  Kp
    u16  Pnt_Td;                           //5  Td
    u16  Pnt_TypeHark;                     //6  TypeHark
    u16  Pnt_TimeScale;                    //7  TimeScale
    u16  Pnt_DifFiltr;                     //8  DifFiltr
    u16  Pnt_maxPid;                       //9  maxPid
    u16  Pnt_minPid;                       //10 minPid
    u16  Pnt_Res;                          //11 Res
    u16  Pnt_Hold;                         //12 Hold
    u16  Pnt_Bypass;                       //13 Bypass
    u16  Pnt_BP_Fref;                      //14 BP_Fref
    u16  Pnt_TempBP;                       //15 TempBP
    u16  Pnt_BypassOK;                     //16 BypassOK
    u16  Pnt_Err_PID;                      //17 Err_PID
    u16  Pnt_Out;                          //18 Out
    u16  Pnt_GT_Down;                      //19 GT_Down
    u16  Pnt_GT_Up;                        //20 GT_Up
    u16  Pnt_End;
}FuncPID_IQ_type;
//********************************************************
// Ti задаёт в формате s16
// Td задаёт в формате s16
// Kp задаётся в процентах в формате float
// TimeScale;        //X mS, X.XX S, X.X S, X S
// DifFiltr          //
//********************************************************
 typedef struct {
        f32     PrescVal    ;
        f32     T           ;
}PrescPid_type;
//********************************************************

//#warning Таблица изменена без вызова прескалера
const PrescPid_type TblP[4] = {
    {-1.0f, 100.0f * FLOAT_TIME_CALL_MICRO  },    // X.XX S
    {-1.0f, 1000.0f * FLOAT_TIME_CALL_MICRO },    // mS
    {-1.0f, 10.0f * FLOAT_TIME_CALL_MICRO },      // X.X  S
    {-1.0f, 1.0f * FLOAT_TIME_CALL_MICRO  },      // X    S
};  //FLOAT_TIME_CALL_MICRO
/*
TimeScale = mS
задаём 1 mS т.е. за секунду выход должен дойти до 1000
т.к. функция вызывается каждые 4mS то будет 250 вхождений   (1/FLOAT_TIME_CALL_MICRO = 1/0.004 = 250)
т.о. 1000/250 = 4           1000/(1/FLOAT_TIME_CALL_MICRO)

TimeScale = X.XX S
задаём 0.01 S т.е. за секунду выход должен дойти до 100
т.к. функция вызывается каждые 4mS то будет 250 вхождений
т.о. 100/250 = 0,4

TimeScale = X.X S
задаём 0.1 S т.е. за секунду выход должен дойти до 10
если бы функция вызывалась каждые 4mS то было бы 250 вхождений, но у нас есть прескаллер на 25
поэтому вхождений будет 10, поэтому Ti домножать ни на что не надо.
*/
static const float TblCnst[8] = {0.01F, 0.05F, 0.10F, 0.30F, 0.50F, 0.70F, 0.90F, 1.00F};
/*
DifFiltr = InParam * TblCnst * Kd  --> 70% ot Kd = (Kd / 100%) * 70%;
*/

//******************************************************************
u16 *FuncPID_IQ_1(FuncPID_IQ_type *progPnt, u32 ramPnt)
{

    FuncPID_IQState_type    *sPnt;
    s16         Ti, Td, TimeScale;
    f32         Kp,Ki,Kd, Prop, Dif, Result, In, max, min, DifFiltr, FiltrIn;
    u32         RiseTime; 
    f32         KpRef,KiRef,KdRef;
    f32         tmpF32;
    
    sPnt = (FuncPID_IQState_type *)(ramPnt + progPnt->Pnt_State);
    Ti          = load_s16(ramPnt, progPnt->Pnt_Ti);               // Интегральный коэффициент
    Td          = load_s16(ramPnt, progPnt->Pnt_Td);               // Дифференциальный коэфициент
    TimeScale   = load_s16(ramPnt, progPnt->Pnt_TimeScale);        // Считываем единицу измерения времени для задания интегрального и дифференциального коэффициентов ПИД-регулятора

    In = load_float(ramPnt, progPnt->Pnt_In) - load_float(ramPnt, progPnt->Pnt_FB); // Ошибка регулирования
    
    if(testBitMicro(ramPnt, progPnt->Pnt_Res)){           // Если  ПИД-регулятор находится в режиме сброса...
        tmpF32 = load_float(ramPnt, progPnt->Pnt_minPid); // Миним. значение частоты при ПИД-регулировании
        save_float(ramPnt, progPnt->Pnt_Out, tmpF32);     // Выдаем минимальное значение частоты при ПИД-регулировании, так как ПИД находится в сбросе

        sPnt->Sum = tmpF32;                               // Минимальное значение частоты
        sPnt->Kp = sPnt->Ki = sPnt->Kd = 0;               // Сброс коэффициентов
        sPnt->Lock_temp = 0;

        clrBitMicro(ramPnt, progPnt->Pnt_GT_Up);          // Обнуление флага увеличения частоты
        clrBitMicro(ramPnt, progPnt->Pnt_GT_Down);        // Обнуление флага уменьшения частоты
        
        return &progPnt->Pnt_End;                         // Выход!!!
    }
#ifdef _PROJECT_FOR_STM32_
    if(testBitMicro(ramPnt, progPnt->Pnt_Hold)){              // Если подан сигнал удержания
    	setBitMicro(ramPnt, progPnt->Pnt_BypassOK);           // Даем сигнал разрешения байпаса
    }
    else{                                                     // Иначе нет сигнала удержания
        clrBitMicro(ramPnt, progPnt->Pnt_BypassOK);           // Не даем сигнал разрешения байпаса
        
        if(testBitMicro(ramPnt, progPnt->Pnt_Bypass)){        // Если пришел сигнал активирования байпаса
            tmpF32 = load_float(ramPnt, progPnt->Pnt_BP_Fref);// Считываем частоту байпаса
            sPnt->Lock_temp = tmpF32;                         // Частота байпаса
        }
        else if(fabsf(sPnt->Lock_temp) > 0.01f){              // Если есть частота байпаса
            sPnt->Sum = sPnt->Lock_temp;                      // Сохраняем ее
            sPnt->Lock_temp = 0;
        }

        if((Ti != sPnt->Ti_pred) || (Td != sPnt->Td_pred) || (TimeScale != sPnt->TimeScale_pred)){  // Если изменились параметры
            sPnt->TimeScale_pred = TimeScale;       // Пересчёт параметров
            sPnt->Ti_pred = Ti;
            sPnt->Td_pred = Td;
            sPnt->T = TblP[TimeScale].T;            // В соответствии с новым значением параметра шкалы TimeScale считываем ...
        }

        RiseTime = load_s16(ramPnt, progPnt->Pnt_TempBP) * K_TIME_CALL_MICRO_TAKT / 10; // Время выхода коэффициентов ПИД-регулятора на заданное значения по пуску
        if(RiseTime == 0){
            Kp = load_float(ramPnt, progPnt->Pnt_Kp);
            if(Ti == 0){
                Ki = 0;
            }
            else{
                Ki = sPnt->T / (f32)Ti;
            }
            
            Kd = (f32)Td / sPnt->T;
        }
        else{                                             // Если задано время выхода коэффициентов ПИД-регулятора на заданное значения по пуску
            KpRef = load_float(ramPnt, progPnt->Pnt_Kp);  // Считываем значение пропорционального коэффициента ПИД-регулятора
            if(Ti == 0){
                 KiRef = 0;                               // Если интегральный коэффициент не задан
            }
            else{
                 KiRef = sPnt->T / (f32)Ti;
            }
            
            KdRef = (f32)Td / sPnt->T;

            tmpF32 = 1.0f / RiseTime;

            if(sPnt->Kp < KpRef){
                 sPnt->Kp += KpRef * tmpF32;
            }
            else{
                 sPnt->Kp = KpRef;
            }
            
            if(sPnt->Ki < KiRef){
                 sPnt->Ki += KiRef * tmpF32;
            }
            else{
                 sPnt->Ki = KiRef;
            }
            
            if(sPnt->Kd < KdRef){
                 sPnt->Kd += KdRef * tmpF32;
            }
            else{
                 sPnt->Kd = KdRef;
            }

            Kp = sPnt->Kp;
            Ki = sPnt->Ki;
            Kd = sPnt->Kd;
        }

        if(sPnt->Prescal > 0.0f){
            sPnt->Prescal --;
        }
        else{                                                               // Выполнить функцию
            sPnt->Prescal = TblP[TimeScale].PrescVal;                       // Заряжаю счётчик-таймер
            if( load_s16(ramPnt, progPnt->Pnt_TypeHark) )   In *= -1.0;     // Обратная характеристика
            
            DifFiltr = TblCnst[load_s16(ramPnt, progPnt->Pnt_DifFiltr)];    // Фильтрую сигнал ошибки
            FiltrIn = f32_Filtr(sPnt->FiltrIn_pred, In, DifFiltr);          //(f32 valOld, f32 ValNew, s16 Kfiltr)

            // Регулятор
            sPnt->Sum      += In * Ki;                                      // Интегральная составляющая
            Prop            = In * Kp;                                      // Пропорциональная составляющая 
            Dif             = (FiltrIn - sPnt->FiltrIn_pred) * Kd;          // Дифференциал от фильтрованной ошибки
            sPnt->FiltrIn_pred = FiltrIn;

            Result = sPnt->Sum + Prop + Dif;
            
            // Проверим результат на выход за приделы
            min = load_float(ramPnt, progPnt->Pnt_minPid);
            max = load_float(ramPnt, progPnt->Pnt_maxPid);
            
            if(Result < min){                                               // Ограничиваю выход регулятора
                Result      = min;
                sPnt->Sum   = Result - Prop;
            }
            else if(Result > max){
                Result      = max;
                sPnt->Sum   = Result - Prop;
            }

            // Выставим флаги GT
            if(Result > sPnt->Out_prev){
                setBitMicro(ramPnt, progPnt->Pnt_GT_Up);
                clrBitMicro(ramPnt, progPnt->Pnt_GT_Down);
            }else
            	if(Result < sPnt->Out_prev){
                    clrBitMicro(ramPnt, progPnt->Pnt_GT_Up);
                    setBitMicro(ramPnt, progPnt->Pnt_GT_Down);
                }else{
                    clrBitMicro(ramPnt, progPnt->Pnt_GT_Up);
                    clrBitMicro(ramPnt, progPnt->Pnt_GT_Down);
                }

            save_float(ramPnt, progPnt->Pnt_Out, Result);          // Выход регулятора
            sPnt->Out_prev = Result;
        }
    }
    
    save_float(ramPnt, progPnt->Pnt_Err_PID, In);                  // Выводим ошибку ругулирования
#endif
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPID_IQ_1[20]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //In
        S32_PIN_TYPE | INPUT_PIN_MODE,     //FB
        S16_PIN_TYPE | INPUT_PIN_MODE,     //Ti       
        S32_PIN_TYPE | INPUT_PIN_MODE,     //Kp       
        S16_PIN_TYPE | INPUT_PIN_MODE,     //Td       
        S16_PIN_TYPE | INPUT_PIN_MODE,     //TypeHark 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //TimeScale
        S16_PIN_TYPE | INPUT_PIN_MODE,     //DifFiltr 
        S32_PIN_TYPE | INPUT_PIN_MODE,     //maxPid   
        S32_PIN_TYPE | INPUT_PIN_MODE,     //minPid
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //Res
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //Hold
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //Bypass
        S32_PIN_TYPE | INPUT_PIN_MODE,     //BP_Fref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //TempBP
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //BypassOK
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //Err_PID
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //Out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //GT_Down
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //GT_Up
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PID_IQ", "DD", TblFuncPID_IQ_1, FuncPID_IQ_1, sizeof(TblFuncPID_IQ_1), sizeof(FuncPID_IQState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPID_IQ_1,
#endif
//********************************************************
