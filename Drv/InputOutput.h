#ifdef _PROJECT_FOR_STM32_
#include "stm32f4xx_hal.h"
#endif
#include "GlobalVar.h"
 
//  управляющие сигналы для WiFi


#define WIFI_RESET_MK                   GPIO_PIN_10
#define WIFI_RESET_MK_GPIO_Port         GPIOD
#define WIFI_EN_MK                      GPIO_PIN_15
#define WIFI_EN_MK_GPIO_Port            GPIOB

// дискретные выходы
#define RELE1_MK_Pin                    GPIO_PIN_1
#define RELE1_MK_GPIO_Port              GPIOC
#define RELE1_MK_Adr_Port               GPIOC_BASE

#define RELE2_MK_Pin                    GPIO_PIN_0
#define RELE2_MK_GPIO_Port              GPIOC
#define RELE2_MK_Adr_Port               GPIOC_BASE

#define DOUT1_HF_MK_Pin                 GPIO_PIN_7
#define DOUT1_HF_MK_GPIO_Port           GPIOB
#define DOUT1_HF_MK_Adr_Port            GPIOB_BASE

// аналоговые входы
#define AIN1_0_10V_MK_Pin GPIO_PIN_8
#define AIN1_0_10V_MK_GPIO_Port GPIOA
#define AIN1_4_20mA_MK_Pin GPIO_PIN_9
#define AIN1_4_20mA_MK_GPIO_Port GPIOC
#define AIN2_0_10V_MK_Pin GPIO_PIN_8
#define AIN2_0_10V_MK_GPIO_Port GPIOC
#define AIN2_4_20mA_MK_Pin GPIO_PIN_12
#define AIN2_4_20mA_MK_GPIO_Port GPIOA

// аналоговые выходы
#ifdef _BOARD_CP24_V2_0
    #define AOUT_1_0_10V_MK_Pin GPIO_PIN_4
    #define AOUT_1_0_10V_MK_GPIO_Port GPIOD
    #define AOUT_1_4_20mA_MK_Pin GPIO_PIN_15
    #define AOUT_1_4_20mA_MK_GPIO_Port GPIOD
    #define AOUT_2_0_10V_MK_Pin GPIO_PIN_14
    #define AOUT_2_0_10V_MK_GPIO_Port GPIOD
    #define AOUT_2_4_20mA_MK_Pin GPIO_PIN_13
    #define AOUT_2_4_20mA_MK_GPIO_Port GPIOD

    #define AOUT_1_PWM_MK_Pin GPIO_PIN_12           // TIM4 CH4
    #define AOUT_1_PWM_MK_GPIO_Port GPIOD
    #define AOUT_2_PWM_MK_Pin GPIO_PIN_1            // TIM3 CH4
    #define AOUT_2_PWM_MK_GPIO_Port GPIOB

#else   // _BOARD_CP24_V2_1 
    #define AOUT_1_0_10V_MK_Pin GPIO_PIN_4
    #define AOUT_1_0_10V_MK_GPIO_Port GPIOD
    #define AOUT_1_4_20mA_MK_Pin GPIO_PIN_15
    #define AOUT_1_4_20mA_MK_GPIO_Port GPIOD
    #define AOUT_2_0_10V_MK_Pin GPIO_PIN_14
    #define AOUT_2_0_10V_MK_GPIO_Port GPIOD
    #define AOUT_2_4_20mA_MK_Pin GPIO_PIN_11
    #define AOUT_2_4_20mA_MK_GPIO_Port GPIOD

    #define AOUT_1_PWM_MK_Pin GPIO_PIN_13            // TIM4 CH4
    #define AOUT_1_PWM_MK_GPIO_Port GPIOD
    #define AOUT_2_PWM_MK_Pin GPIO_PIN_12            // TIM3 CH4
    #define AOUT_2_PWM_MK_GPIO_Port GPIOD
#endif 

#define FAN_MK_Pin GPIO_PIN_3                   // PC3
#define FAN_MK_GPIO_Port GPIOC

//// для светодиодов для пульта
//#define LED24_WiFi              GPIO_PIN_14     // PE 14
//#define LED24_WiFi_Adr_Port     GPIOE_BASE
//
//#define LED24_Alarm             GPIO_PIN_13     // PD 13
//#define LED24_Alarm_Adr_Port    GPIOB_BASE
//
//#define LED24_Work              GPIO_PIN_12     // PB 12
//#define LED24_Work_Adr_Port     GPIOB_BASE
//
//#define LED24_Ready             GPIO_PIN_15     // PE 15
//#define LED24_Ready_Adr_Port    GPIOE_BASE

// реле для размыкания предзарядных резисторов
#define RELE_ON_MK              GPIO_PIN_2
#define RELE_ON_MK_GPIO_Port    GPIOC

//****************************************************************************//
#define PeriodPWM_Aout          10000           // переполнение для таймера PWM

//// управление зарядом
//#define Start_set()  RELE_ON_MK_GPIO_Port->BSRR = RELE_ON_MK 
//#define Start_clr()  RELE_ON_MK_GPIO_Port->BSRR = (uint32_t)RELE_ON_MK << 16U  
//
//#define Zarr_set()   1
//#define Zarr_clr()   0

//// управление вентилятором
//#define FanOn()  FAN_MK_GPIO_Port->BSRR = FAN_MK_Pin 
//#define FanOff()  FAN_MK_GPIO_Port->BSRR = (uint32_t)FAN_MK_Pin << 16U  


void SetValuePWM_Aout(s16 RefPWM, s16 numAout);
//void DinDoutInit(void);
//void WiFiDinControl(void);
void AinAoutInit(void);
void PWM_Aout_Init(void);
//void CoolerPortInit(void);         // Настройка порта для управления вентилятором охлаждения силовых ключей
//void LedPultPortInit(void);        // Настройка пинов для управления светодиодами пульта


#ifdef _PROJECT_FOR_STM32_
void Timer_Port_Init(void);
#endif


