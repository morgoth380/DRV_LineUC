#ifdef    GET_FUNC_CODE
#include "Time_.h"

//====================================================================

StateFuncRTC_type stateRTC;

typedef struct{
    s32  UnixTimePowerOff;
}nvRTCState_type;
#define     nvSfRTC     (*(nvRTCState_type*)0)

typedef struct{
    u16  Pnt_Nfunc;
//    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_YearIn;                       //1  YearIn
    u16  Pnt_MonthIn;                      //2  MonthIn
    u16  Pnt_DayIn;                        //3  DayIn
    u16  Pnt_HourIn;                       //4  HourIn
    u16  Pnt_MinuteIn;                     //5  MinuteIn
    u16  Pnt_SecondIn;                     //6  SecondIn
    u16  Pnt_EN_CHNG;                      //7  SecondIn
    u16  Pnt_TPowOff;                      //8  TPowOff
    u16  Pnt_ms100;                        //9  ms100
    u16  Pnt_Sec1;                         //10  Sec1
    u16  Pnt_Min1;                         //11  Min1
    u16  Pnt_ErrRtc;                       //12  ErrRtc
    u16  Pnt_UnixTime;                     //13  UnixTime
    u16  Pnt_MaxDate;                      //14  MaxDate
    u16  Pnt_End;
}FuncRTC_type;
//********************************************************

u16 *FuncRTC_1(FuncRTC_type *progPnt, u32 ramPnt)
{
#ifdef _PROJECT_FOR_STM32_  
    u32 unixTime = 0;
    u8 maxDate = 0;
    RTC_type time;
    StateFuncRTC_type  *sPnt;
    nvRTCState_type     *nvAdr, nvMem;

    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfRTC);
    sPnt = &stateRTC;
    
    
    if(flagIsFirstCall)
    {
        GetTime(&time); 
        unixTime = Time_to_UnixTime(&time);
        
        SaveTimeRTC(sPnt, &time);
        maxDate = CalcNumDayInMonth(&time);
        
        sPnt->Prev100ms   = 0;                                                  
        sPnt->Prev1Sec    = time.Seconds;
        sPnt->Prev1Min    = time.Minutes;
        
        save_s16(ramPnt, progPnt->Pnt_SecondIn,     time.Seconds);
        save_s16(ramPnt, progPnt->Pnt_MinuteIn,     time.Minutes);
        save_s16(ramPnt, progPnt->Pnt_HourIn,       time.Hours);
        save_s16(ramPnt, progPnt->Pnt_DayIn,        time.Day);
        save_s16(ramPnt, progPnt->Pnt_MonthIn,      time.Month);
        save_s16(ramPnt, progPnt->Pnt_YearIn,       time.Year);
        save_s32(ramPnt, progPnt->Pnt_UnixTime,     unixTime);
        save_s16(ramPnt, progPnt->Pnt_MaxDate,      maxDate);
          
        RdFramMicro(&nvMem, nvAdr, sizeof(nvRTCState_type));
        save_s32(ramPnt, progPnt->Pnt_TPowOff, nvMem.UnixTimePowerOff);         // Время отключения питание
    }
    else
    {
        if (testBitMicro(ramPnt, progPnt->Pnt_EN_CHNG))
        {
            time.Seconds = load_s16(ramPnt, progPnt->Pnt_SecondIn);     //счит. введенное количество секунд (при втором вызове это значения, считанные из FRAM)
            time.Minutes = load_s16(ramPnt, progPnt->Pnt_MinuteIn);     //счит. введенное количество минут (при втором вызове это значения, считанные из FRAM)
            time.Hours   = load_s16(ramPnt, progPnt->Pnt_HourIn);       //счит. введенное количество часов (при втором вызове это значения, считанные из FRAM)
            time.Day     = load_s16(ramPnt, progPnt->Pnt_DayIn);        //счит. введенный номер дня (при втором вызове это значения, считанные из FRAM)
            time.Month   = load_s16(ramPnt, progPnt->Pnt_MonthIn);      //счит. введенный номер месяца (при втором вызове это значения, считанные из FRAM)
            time.Year    = load_s16(ramPnt, progPnt->Pnt_YearIn);       //счит. введенный год (при втором вызове это значения, считанные из FRAM)
        }

        if (sPnt->second != time.Seconds || sPnt->minute != time.Minutes  ||
            sPnt->hour != time.Hours    || sPnt->date != time.Day      ||
            sPnt->month != time.Month   || sPnt->year != time.Year)
        {
            if((sPnt->month != time.Month) || (sPnt->year != time.Year))// если  изменими месяц или год
            {
              maxDate = CalcNumDayInMonth(&time);                       // определение максимального количества дней в месяце 
              if(maxDate < time.Day)                                    // если текущий день больше чем количество дней в месяце
              {
                time.Day = 1;                                           
                save_s16(ramPnt, progPnt->Pnt_DayIn,        time.Day);
              }
              save_s16(ramPnt, progPnt->Pnt_MaxDate, maxDate);          // запись максимального предела для задания дня
            }  
            time.Format_AM_PM = 0;                                      // параметра для формата нет, значит всегда пишем 0 (AM - 24)
            time.Day_of_week = 0;                                       // нет потребности в дне недели, 0 - не используеться
            SetTime(&time);                                             // если какой либо из параметров времени изменился
            SaveTimeRTC(sPnt, &time);                                   // сохранить введенные данные в статическую область памяти
        }

        if ( sPnt->TimeDiv <= 0 )                                       //передаем в микропрограмму время с интервалом в 20 тактов
        { 
            sPnt->TimeDiv = 20;
            GetTime(&time);                                             // получить значение о времени
            unixTime = Time_to_UnixTime(&time);                         // расчитать UnixTime
            
            save_s16(ramPnt, progPnt->Pnt_SecondIn,     time.Seconds);
            save_s16(ramPnt, progPnt->Pnt_MinuteIn,     time.Minutes);
            save_s16(ramPnt, progPnt->Pnt_HourIn,       time.Hours);
            save_s16(ramPnt, progPnt->Pnt_DayIn,        time.Day);
            save_s16(ramPnt, progPnt->Pnt_MonthIn,      time.Month);
            save_s16(ramPnt, progPnt->Pnt_YearIn,       time.Year);
            save_s32(ramPnt, progPnt->Pnt_UnixTime,     unixTime);
            
            SaveTimeRTC(sPnt, &time);
        }
        else 
           sPnt->TimeDiv--;
//=============================================================================//
//             Отработка 100мс интервалов                                      //
//=============================================================================//
          if (sPnt->Prev100ms >= 25) //25 вызовов МП
          { 
              sPnt->Prev100ms = 0;
              setBitMicro(ramPnt, progPnt->Pnt_ms100); //устанавливаем флаг прохождения 100 мс
          }
          else
          {
              sPnt->Prev100ms ++;
              clrBitMicro(ramPnt, progPnt->Pnt_ms100);
          }
//=============================================================================//
//             Отработка 1с интервалов                                         //
//=============================================================================//
        if (sPnt->Prev1Sec != time.Seconds)
        {
            sPnt->Prev1Sec = time.Seconds;
            setBitMicro(ramPnt, progPnt->Pnt_Sec1);

            nvMem.UnixTimePowerOff = unixTime;
            WrFramMicro(nvAdr, &nvMem, sizeof(nvRTCState_type));
        }
        else
            clrBitMicro(ramPnt, progPnt->Pnt_Sec1);

//=============================================================================//
//             Отработка 1мин интервалов                                       //
//=============================================================================//
        if(sPnt->Prev1Min != time.Minutes)
        {
            sPnt->Prev1Min = time.Minutes;
            setBitMicro(ramPnt, progPnt->Pnt_Min1);
        }
        else
            clrBitMicro(ramPnt, progPnt->Pnt_Min1);

    }
#endif    
    return &progPnt->Pnt_End;
};

#endif

//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRTC_1[14]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  YearIn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  MonthIn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  DayIn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  HourIn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  MinuteIn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  SecondIn
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  SecondIn
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //8  TPowOff
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  ms100
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Sec1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Min1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  ErrRtc
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //13  UnixTime
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //14  MaxDate
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("RTC", "DD", TblFuncRTC_1, FuncRTC_1, sizeof(TblFuncRTC_1), 0, sizeof(nvRTCState_type), 1),
//OpisFuncBlock("RTC", "DD", TblFuncRTC_1, FuncRTC_1, sizeof(TblFuncRTC_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRTC_1,
#endif
//********************************************************