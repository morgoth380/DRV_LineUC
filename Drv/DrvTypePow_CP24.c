#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "DrvTypePow.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "measTime.h"


// Прототипы функций
void delay(u32 time_delay);                  // Функция задержки

// Настроить 
void TypeDrvInit(DRVTYPE_handle d){
  // Для СР24 применяем тип LowPow
  d->DrvType = LowPow;
  return; 
}

// Получить Тип ПЧ
DrvType_Enum GetTypeDrv(DRVTYPE_handle d){
  return d->DrvType;
}

// Квитирование аварии ключей 
void AskErrKey(DRVTYPE_handle d){
  // Для СР24 здесь пусто
}

// Функция задержки
void delay(u32 time_delay){	
    u32 i;
    
    for(i = 0; i < time_delay; i++){
       __asm volatile("NOP");
    }
}

#endif // defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)