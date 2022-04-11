#include "DrvRTC.h"

RTC_HandleTypeDef hrtc;

void MX_RTC_Init(void){
  //u32  Count = 0;
  
  // 1) Если RTC не настроен то настраиваем
  if((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN){                 //Проверка работы
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 0x7F;
    hrtc.Init.SynchPrediv = 0xFF;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    //HAL_RCCEx_SelectLSEMode (RCC_LSE_HIGHDRIVE_MODE);

    HAL_RTC_Init(&hrtc);
       
    SetCalibr(RTC_CALIBSIGN_POSITIVE,0x1C);
  }
/*
  // 2) Проверим работают ли часы (контроль сбоя в работе)
  DisebleWriteProtRTC();                              // Отключить защиту записи
  RTC->ISR |= RTC_ISR_INIT;                           // Входим в режим редактирования
    
  // Ожидание пока RTC разрешит изменять данные
  do{
     Count++;
     if(Count > (TIMOUT_RTC * 20)){
       // 3) Часы не работают!!!
       // Сбросить Backup область
       RCC->BDCR |= RCC_BDCR_BDRST;                   
       RCC->BDCR &= ~RCC_BDCR_BDRST;
  
       RTC->ISR &= (u32)~RTC_ISR_INIT;                // Выход из редима инициализации
       EnableWriteProtRTC();                          // Включить защиту записи
       
       //RESTART CPU
       HAL_NVIC_SystemReset();              
      
       return;                                        
     }
     
  }while((RTC->ISR & RTC_ISR_INITF) == RESET);        // Ждем подтверждения входа в режим редактирования

  RTC->ISR &= (u32)~RTC_ISR_INIT;                // Выход из редима инициализации
  EnableWriteProtRTC();                          // Включить защиту записи
*/
}

u8 SetCalibr(u8 sign, u8 value)
{
    u32 RTC_CALIBR = 0;
    RTC_CALIBR |= sign << 7;
    RTC_CALIBR |= value;
    DisebleWriteProtRTC();                              // Отключить защиту записи
    RTC->ISR |= RTC_ISR_INIT;                           // Входим в режим редактирования
    while((RTC->ISR & RTC_ISR_INITF) == RESET);         // Ждем подтверждения входа в режим редактирования
    
    RTC->CALIBR = RTC_CALIBR;
    RTC->CR    |= (RTC_CR_DCE);
    RTC->ISR &= (u32)RTC_RSF_MASK;                      // Обнуление флага синхронизации
    while((RTC->ISR & RTC_ISR_RSF) == RESET);           // Ожидание синхронизации ( записи данных из теневых регистров в рабочие)
    RTC->ISR &= (u32)~RTC_ISR_INIT;                     // Выход из редима инициализации
    EnableWriteProtRTC();                               // Включить защиту записи
    
    return 0;
}

u8 SetTimeRTC(u32 TimeReg, u32 DateReg)
{
    u32  Count; 
            
    DisebleWriteProtRTC();                              // Отключить защиту записи
    RTC->ISR |= RTC_ISR_INIT;                           // Входим в режим редактирования
    
    // Ожидание пока RTC разрешит изменять данные
    Count = 0;
    
    do{
       Count++;
       if(Count > TIMOUT_RTC){
         RTC->ISR &= (u32)~RTC_ISR_INIT;                // Выход из редима инициализации
         EnableWriteProtRTC();                          // Включить защиту записи
         return 1;
       }
    }while((RTC->ISR & RTC_ISR_INITF) == RESET);        // Ждем подтверждения входа в режим редактирования
    
    RTC->TR = TimeReg;                                  // Запись данных в регистр времени
    RTC->DR = DateReg;                                  // Запись данных в регистр даты
    
    RTC->ISR &= (u32)RTC_RSF_MASK;                      // Обнуление флага синхронизации
    
    Count = 0;
    do{
       Count++;
       if(Count > TIMOUT_RTC){
          RTC->ISR &= (u32)~RTC_ISR_INIT;               // Выход из редима инициализации
          EnableWriteProtRTC();                         // Включить защиту записи
          return 1;
       }
    }while((RTC->ISR & RTC_ISR_RSF) == RESET);          // Ожидание синхронизации ( записи данных из теневых регистров в рабочие)
    
    RTC->ISR &= (u32)~RTC_ISR_INIT;                     // Выход из редима инициализации
    EnableWriteProtRTC();                               // Включить защиту записи
    
    return 0;
}
