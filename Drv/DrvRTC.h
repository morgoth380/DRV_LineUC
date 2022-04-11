#include "main.h"

extern RTC_HandleTypeDef hrtc;

#define DisebleWriteProtRTC()        RTC->WPR = 0xCA; \
                                     RTC->WPR = 0x53;

#define EnableWriteProtRTC()         RTC->WPR = 0xFFU 
                                     
#define TIMOUT_RTC                   1000                // Таймаут на ожидание от блока RTC (В тактах CPU)                                     
                                     
                                     
                                   
void MX_RTC_Init(void);
u8 SetTimeRTC(u32 TimeReg, u32 DataReg);
u8 SetCalibr(u8 sign, u8 value);
