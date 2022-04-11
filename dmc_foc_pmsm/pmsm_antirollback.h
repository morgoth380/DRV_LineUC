/** =================================================================================
* \file		: pmsm_antirollback.h
*
* \brief	: Заголовочный файл модуля антиотката для синхронной лебедки.
* \details	: включает типы данных, инициализационные константы, и функции
* 			  объекта модуля компенсации
*
* \autor	: Дмитрий Сосновчик
*
* \date		: 31.08.2018 \version	: 1.0
*
* \copyright: (C)TRIOLCORP, 2018
*
=====================================================================================*/

#ifndef __PMSMANTIROLLBACK_H__
#define __PMSMANTIROLLBACK_H__

#include "type.h"

typedef struct {
              f32 inId;      // Input: задание тока по оси d вращ. сист. коорд.(pu)
              f32 inIq;      // Input: задание тока по оси q вращ. сист. коорд.(pu)
              f32 fixId;     // Input: Номинальное задание тока Id удержания при антиоткате на нулевой частоте
              f32 inSpeedWr; // Input: Текущая скорость вращения
              f32 inTheta;   // Input: Текущая фаза поля
              
              u16 bitPhasing;// Флаг разрешения фазировки энкодера
              f32 outId;     // Output: Выход тока Id
              f32 outIq;     // Output: Выход тока Iq
              f32 outSpeedWr;// Output: Выход фактической частоты
              f32 outTheta;  // Output: Выход фактической фазы
              
              f32 timeKoef;  // Коэффициент для плавного нарастания и спадания тока возбуждения
              f32 Fpwm;      // Частота дискретизации
              s16 timeUI_Up; // Время подъёма/спада тока возбуждения, мсек
              u16 pwmOn;	 // Флаг включения ШИМ
              u16 onStartWork; // Активировать работу алгоритма - только по пуску
              
              f32 inSpeedWrz;// Input: Текущая скорость задания
              u16 BreakOff;  // Input - BrakeOff state
              u16 Mode;      // Input режим работы алгоритма антиотката
              u16 fixIdDone; // Ток удержания достигнут
              u16 fixIdDoneCnt;  // Счетчик времени сколько я уже в состоянии накачанного тока.
              
              f32 maxIfull;	 // Максимальное значение полного тока
              f32 FrequencyDown;
              f32 timeStepDown;
              f32 FixTimeId;              
              f32 Imag;
              
              u16 Enable;       // Сигнал на разрешение работы алгоритмов, либо их шунтирование
              
              void  (*calc)();   // Pointer to calculation function
              void  (*init)();   // Pointer to reset function
} PMSMANTIROLLBACK_Obj;

extern  PMSMANTIROLLBACK_Obj pmsmAntirollBack;

typedef PMSMANTIROLLBACK_Obj * PMSMANTIROLLBACK_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PMSMANTIROLLBACK object.
-----------------------------------------------------------------------------*/                     
#define PMSMANTIROLLBACK_DEFAULTS {0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0,  0,\
                                   (void (*)(u32))pmsm_antirollback_calc,\
                                   (void (*)(u32))pmsm_antirollback_init }

/*------------------------------------------------------------------------------
Prototypes for the functions
------------------------------------------------------------------------------*/
void pmsm_antirollback_calc(PMSMANTIROLLBACK_handle);
void pmsm_antirollback_init(PMSMANTIROLLBACK_handle);

#endif /* __PMSMANTIROLLBACK_H__ */


