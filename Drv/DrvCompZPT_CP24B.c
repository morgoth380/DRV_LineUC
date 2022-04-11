#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "DrvCompZPT.h"
#include "DrvPWM.h"
#include "GlobalVar.h"

// Прототипы privat функций

extern COMPZPTDRV CompZPT;
extern PWMGEN Pwm;           // Обект формирователя ШИМ (для отключения ШИМ)


// Настроить прерывание от компаратора
void CompZPTInit(COMPZPTDRV *p){
// Заглушка
}

// Установить режим работы компаратора
void SetModeComp(COMPZPTDRV *p){
 // Заглушка
}

// Прерывание от компаратора
void HAL_GPIO_EXTI6_Callback(void){
// Заглушка
}

#endif //defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)