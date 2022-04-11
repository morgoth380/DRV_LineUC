
// Global Define Drive Type
#define TYPE_SCALAR_NO_DEADBAND     0 // Скалярное управление без компенсации мертвого времени
#define TYPE_SCALAR                 1 // Скалярное управление
#define TYPE_ACIM_FOC_SENSORED      2 // Асинхронное Датчиковое Векторное управление   
#define TYPE_ACIM_FOC_SENSORLESS    3 // Асинхронное Без Датчиковое Векторное управление  
#define TYPE_PMSM_FOC_SENSORED      4 // Вентильное Датчиковое Векторное управление
#define TYPE_PMSM_FOC_SENSORLESS    5 // Вентильное Без Датчиковое Векторное управление 
#define TYPE_SynM_FOC_SENSORED      6 // Синхронное Датчиковое Векторное управление    
#define TYPE_SynM_FOC_SENSORLESS    7 // Синхронное Без Датчиковое Векторное управление

#define TYPE_ACIM_PICKUP_MASK        0x10 // Маска для подхвата
#define TYPE_ACIM_PICKUP_SENSORLESS  TYPE_ACIM_PICKUP_MASK + TYPE_ACIM_FOC_SENSORLESS 
#define TYPE_ACIM_PICKUP_SENSORED    TYPE_ACIM_PICKUP_MASK + TYPE_ACIM_FOC_SENSORED 
#define TYPE_ACIM_PICKUP_SCALAR      TYPE_ACIM_PICKUP_MASK + TYPE_SCALAR
#define TYPE_ACIM_PICKUP_SCALAR_DB   TYPE_ACIM_PICKUP_MASK + TYPE_SCALAR_NO_DEADBAND

// Global Define Encoder Type
#define ENCO_TYPE_INCREMENTAL      0 // Инкрементальный энкодер
#define ENCO_TYPE_ABS_ENDAT_2_0    1 // Абсолютный энкодер с протоколом EnDAT 2.0
#define ENCO_TYPE_ABS_ENDAT_2_1    2 // Абсолютный энкодер с протоколом EnDAT 2.1
#define ENCO_COS_SIN               3 // sin/cos энкодер

// Global Define Encoder Direction
#define ENCO_DIRECT_FORWARD     0 // Прямое направление вращения
#define ENCO_DIRECT_REVERSE     1 // Реверсное направление вращения

