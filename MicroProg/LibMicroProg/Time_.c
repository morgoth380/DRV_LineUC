#include "Time_.h"
#include "stm32f4xx_hal.h"
#include "DrvRTC.h"
u32 UnixTime;
/*============================================================================*/
//Глобальная структура для модуля RTC.h
/*============================================================================*/
// для Не високосного года
const u16 monthSum[14] = {
    0,
    (0)                                 ,//Jan
    (31)                                ,//Feb
    (31+28)                             ,//Mar
    (31+28+31)                          ,//Apr
    (31+28+31+30)                       ,//May
    (31+28+31+30+31)                    ,//Jun
    (31+28+31+30+31+30)                 ,//Jul
    (31+28+31+30+31+30+31)              ,//Aug
    (31+28+31+30+31+30+31+31)           ,//Sep
    (31+28+31+30+31+30+31+31+30)        ,//Oct
    (31+28+31+30+31+30+31+31+30+31)     ,//Nov
    (31+28+31+30+31+30+31+31+30+31+30)  ,//Dec
    0xFFFF
};
// для високосного года
const u16 monthSumEx[14] = {
    0,
    (0)                                 ,//Jan
    (31)                                ,//Feb
    (31+28+1)                           ,//Mar
    (31+28+31+1)                        ,//Apr
    (31+28+31+30+1)                     ,//May
    (31+28+31+30+31+1)                  ,//Jun
    (31+28+31+30+31+30+1)               ,//Jul
    (31+28+31+30+31+30+31+1)            ,//Aug
    (31+28+31+30+31+30+31+31+1)         ,//Sep
    (31+28+31+30+31+30+31+31+30+1)      ,//Oct
    (31+28+31+30+31+30+31+31+30+31+1)   ,//Nov
    (31+28+31+30+31+30+31+31+30+31+30+1),//Dec
0xFFFF
};
// количество дней в месяце
const u16 dayInMonth[12] = {
    (31) ,//Jan
    (28) ,//Feb
    (31) ,//Mar
    (30) ,//May
    (31) ,//Jun
    (30) ,//Jun
    (31) ,//Jul
    (31) ,//Aug
    (30) ,//Sep
    (31) ,//Oct
    (30) ,//Nov
    (31)  //Dec 
};
//***Перевод времени в UnixTime ***// 
u32 Time_to_UnixTime(RTC_type *tpnt)
{
  u16 quantityYear, dayInCurrentYear;
  u32 calcTime;  

  quantityYear = (tpnt->Year + 2000) - (1970);                  // определение количестра годов
  calcTime = quantityYear * 365 + ((quantityYear+1) >> 2);      // количество дней в пройдех годах, с учет выкосовного ( каждый 4 год)
  if(tpnt->Year & 3)
    dayInCurrentYear = monthSum[tpnt->Month];           // количество пройденных дней в текущем году
  else 
    dayInCurrentYear = monthSumEx[tpnt->Month];         // количество пройденных дней в текущем году, если год високосный
  
  calcTime += dayInCurrentYear;                         // всего пройденных дней без учета текущего месяца                     
  calcTime += tpnt->Day - 1;                            // послное количесво дней
  calcTime *= 24*60*60;                                 // перевод дней в секунды
  calcTime += tpnt->Hours * 60 * 60;                    // добавляем часы в переводе на секунды
  calcTime += tpnt->Minutes * 60;                       // добавляем минуты в переводе на секунды
  calcTime += tpnt->Seconds;                            // добавляем текущее количество секунд
  UnixTime = calcTime;
  return calcTime;
}
//*****************Перевод времени из UnixTime в календарное**********************//
void UnixTime_to_Time(RTC_type *tpnt, u32 value,u8 *extraDay ) //Value - текущее UNIX-время, extraDay для высокостного года
{
    u8 numMonth = 0;
    u32 tmp;
    const u16 *tbl;
    
    value -= UnixConst;
    
    tmp = value;
    tmp = value / 60;
    tpnt->Seconds = (u16)(value - tmp * 60);
    
    value = tmp;
    tmp = value  / 60;
    tpnt->Minutes = (u16)(value - tmp *60);
    
    value = tmp;
    tmp = value  / 24;
    tpnt->Hours = (u16)(value - tmp *24);
    
    value = tmp;
    tpnt->Year = (u16)(value / 365);
    if ((u32)(tpnt->Year*365+(tpnt->Year>>2)) > value) 
        tpnt->Year--;
    value -= tpnt->Year * 365 + (tpnt->Year >> 2);
    tpnt->Year += 1;
    
    if (tpnt->Year & 3){
        tbl = &monthSum[1];
        *extraDay = 0;           
    }
    else{
        tbl = &monthSumEx[1];
        *extraDay = 1;            // доп. день для Високосного года
    }
    
    while (value >= tbl[numMonth]) 
        numMonth++;
    
    tpnt->Month = numMonth;
    tpnt->Day = (u16)(value - tbl[numMonth-1] + 1);
//    if((*extraDay == 1) &&(tpnt->Day <= 60))      // если в Високосный год дата меньше 29 февраля (31+29 = 60 день)
//      *extraDay = 0;                             // не добавляем день
    tpnt->Day_of_week = tpnt->Format_AM_PM = 0;
    
}
//*** определения времени с блока RTC ***//
u16 GetTime(RTC_type *pntTime)
{
    u32 RTC_TR, RTC_DR  = 0;
 /*   
    u32 RTC_CALIBR,RTC_CR,RTC_CALR = 0;
    RTC_CALIBR  = RTC->CALIBR;                                         // значение калибровки
    RTC_CR      = RTC->CR;
    RTC_CALR    = RTC->CALR;  
*/
    RTC_TR = RTC->TR;                                                   // вычитываем данные с регистра времени
    RTC_DR = RTC->DR;                                                   // вычитываем данные с регистра даты


    pntTime->Seconds     = (u8)RTC_TR;                                  // количество секунд
    pntTime->Minutes     = (u8)(RTC_TR >> RTC_OffsetRegTR_Minutes);     // определение количества минут
    pntTime->Hours       = (u8)(RTC_TR >> RTC_OffsetRegTR_Hours);       // определение количества часов + формат времени
    pntTime->Format_AM_PM= pntTime->Hours >> 6;                         // формат времени
    pntTime->Hours       &= ~Adr_Format_AM_PM;                          // определение количества часов (обнуляем формат отображеня часов)
    pntTime->Day         = (u8)RTC_DR;                                  // Количество дней
    pntTime->Month       = (u8)(RTC_DR >> RTC_OffsetRegDR_Month);       // Определение текущего месяца + дня недели
    pntTime->Day_of_week = (u8)(pntTime->Month >> 5);                   // День недели
    pntTime->Month       &= ~MaskMonth_Week;                            // Определение текушего времени (обнуляем день недели)
    pntTime->Year        = (u8)(RTC_DR >> RTC_OffsetRegDR_Year);        // Определение текущего года( в формате от 0 до 

    // перевод данных из BCD формата
    pntTime->Seconds     = ConvertBCDToByte(pntTime->Seconds);
    pntTime->Minutes     = ConvertBCDToByte(pntTime->Minutes);
    pntTime->Hours       = ConvertBCDToByte(pntTime->Hours);
    pntTime->Day         = ConvertBCDToByte(pntTime->Day);
    pntTime->Month       = ConvertBCDToByte(pntTime->Month);
    pntTime->Year        = ConvertBCDToByte(pntTime->Year);

    return 0;
}
//*** Задание время в блок RTC ***//
u16 SetTime(RTC_type *pntTime)
{
  u32 RTC_TR    = 0;
  u32 RTC_DR    = 0;
  // заполняем данные в формате BCD для регистра Времени
  RTC_TR        = ConvertByteToBCD(pntTime->Seconds);
  RTC_TR        |= (ConvertByteToBCD(pntTime->Minutes) << RTC_OffsetRegTR_Minutes);
  RTC_TR        |= (ConvertByteToBCD(pntTime->Hours) << RTC_OffsetRegTR_Hours); 
  RTC_TR        |= pntTime->Format_AM_PM << RTC_OffsetRegTR_Format_AM_PM;               // данные о формате
  // заполняем данные в формате BCD для регистра Даты
  RTC_DR        = ConvertByteToBCD(pntTime->Day);
  RTC_DR        |= ConvertByteToBCD(pntTime->Month) << RTC_OffsetRegDR_Month;
  RTC_DR        |= ConvertByteToBCD(pntTime->Year) << RTC_OffsetRegDR_Year;
  
  SetTimeRTC(RTC_TR, RTC_DR);

  return 0;
}
void SaveTimeRTC(StateFuncRTC_type  *sPnt, RTC_type *time)
{
      
    sPnt->second      = time->Seconds;      //сохраняем данные времени в статическом объекте
    sPnt->minute      = time->Minutes;      //сохраняем данные времени в статическом объекте
    sPnt->hour        = time->Hours;        //сохраняем данные времени в статическом объекте
    sPnt->date        = time->Day;          //сохраняем данные времени в статическом объекте
    sPnt->month       = time->Month;        //сохраняем данные времени в статическом объекте
    sPnt->year        = time->Year;         //сохраняем данные времени в статическом объекте
}
//*** Перевод ИЗ формата BCD ***//
u8 ConvertBCDToByte(u8 value)
{
    u32 convertValue = 0;
    convertValue = ((u8)(value & (u8)0xF0) >> (u8)0x4) * 10;
    return (convertValue + (value & (u8)0x0F)); 
}
//*** Перевод В формат BCD ***//
u8 ConvertByteToBCD(u8 value)
{
  u32 bcdhigh = 0;

  while(value >= 10)
  {
    bcdhigh++;
    value -= 10;
  }

  return  ((u8)(bcdhigh << 4) | value);
}
//*** Расчет количества дней в месяце ***//
u8 CalcNumDayInMonth( RTC_type *time)
{
  u8 MaxDays = 0;
  MaxDays = dayInMonth[time->Month-1];
  
  if(time->Month == 2)          // если месяц февраль
  {
    if(!(time->Year % 4))       // высокосный год
      MaxDays++;                // 29 дней
  }
  
  return MaxDays;
}