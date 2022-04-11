#include "GlobalVar.h"


extern const u16 monthSum[14];
extern const u16 monthSumEx[14];
extern const u16 DayInMonth[14];

typedef struct{
  u8 Seconds;
  u8 Minutes;
  u8 Hours;
  u8 Day;
  u8 Month;
  u8 Day_of_week;
  u16 Year;
  u8 Format_AM_PM;                // 0: AM или 24-часовой формат, 1:PM 
}RTC_type;

typedef struct{
        u16 year;
        u16 month;
        u16 date;
        u16 hour;
        u16 minute;
        u16 second;
        u16 ErrRtcStart;                // нужно ли !? 2038 не очень скоро
        u16 Prev100ms;
        u16 Prev1Sec;
        u16 Prev1Min;
        s16 TimeDiv;                    // это для 80 мс, но можно же сделать привязку к (Prev100ms) 100 мс )
}StateFuncRTC_type;

//extern StateFuncRTC_type stateRTC;
#define UnixConst   ((u32)(11323L*24L*3600L))     //2001-1970

#define MaskMonth_Week 0xE0
#define Adr_Format_AM_PM 0x40;          // смещение относительно размещения данных минут в регистре DR
typedef enum
{
    RTC_OffsetRegTR_Minutes             = 0x08, 
    RTC_OffsetRegTR_Hours               = 0x10,
    RTC_OffsetRegTR_Format_AM_PM        = 0x16,
    RTC_OffsetRegDR_Month               = 0x08,
    RTC_OffsetRegDR_Year                = 0x10
      
}RTC_Offset;

u32 Time_to_UnixTime(RTC_type *tpnt);
void  UnixTime_to_Time(RTC_type *tpnt, u32 valUnixTime, u8 *extraDay);

u16 GetTime(RTC_type *pntTime);
u16 SetTime(RTC_type *pntTime);
void SaveTimeRTC(StateFuncRTC_type  *sPnt, RTC_type *time);

u8 ConvertBCDToByte(u8 value);
u8 ConvertByteToBCD(u8 value);

u8 CalcNumDayInMonth( RTC_type *time);