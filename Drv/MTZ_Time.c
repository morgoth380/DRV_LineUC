/************************** (C) Copyright 2020 Triol ***************************
 * File Name          : MTZ_Time.c
 * Author             : Sosnovchik Dmytry
 * TS Idea            : 2017.07.17
 * TS Version         : 2017.17.17
 * Description        : Файл содержит функцию, которая реализует алгоритм
 *                      блокировки пуска ПЧ на 1 минуту после возникновения аварии аппаратной МТЗ ,
 *                      и на 10 минут, если в течении пяти минут МТЗ сработала 3 раза.
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "MTZ_Time.h"
#include "Global_include.h"
#include "GlobalVar.h"

/* Exported functions --------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : MTZ_Time
 * Description    : Записывает время срабатывания МТЗ в буфер, на основании записанного
 *                  времени выдает/снимает флаг аварии для запрета/разрешения пуска
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
u32 mtzTimeBuff[MTZ_BUF_SIZE];
extern u32      UnixTime;

// Максимальное кол-во МТЗ не должно превышать MTZ_BUF_SIZE, либо надо увеличить размер буфера MTZ_BUF_SIZE.
#define		MAX_MTZ_COUNT		3 // Максимальное кол-во МТЗ
#define		MTZ_LIFE_TIME		5*60 // Максимальное время жизни времени МТЗ 5 минут
#define		MTZ_BLOCKED_TIME	10*60 // Максимальное время блокировки по МТЗ 10 минут
#define		MTZ_ACK_TIME		1*60 // Время квитирования МТЗ 1 минута

void MTZ_Time() {
	
        GlobalVar           *mPnt;
	void *nvAdrTime;
	u16 i, refreshFram;
	u16	static	MtzLatchFlag = 0;
	s32 tempTime;
                
        mPnt = &GlobalM4;   
	nvAdrTime = (void *) ((u32) &nv.mtzTimeBuff); // Адрес на буфер времЕн МТЗ

	if (flagIsFirstCall) {
		RdFramMicro(&mtzTimeBuff, nvAdrTime, MTZ_BUF_SIZE*sizeof(u32)); // При пуске вычитываем текущее состояние буфера МТЗ
		return;
	}

	// Новый обработчик МТЗ
	if (mPnt->FlagState.bit.MTZ_hold == 1  && !MtzLatchFlag) { // Если поймали МТЗ, заносим время в буфер
		for (i = (MAX_MTZ_COUNT-1); i >= 1; i--) {
			mtzTimeBuff[i] = mtzTimeBuff[i-1]; // Сдвигаем время
		}
		mtzTimeBuff[0] = UnixTime; // На вершину кладем самое последнее время МТЗ
		MtzLatchFlag = 1; // Ставим флаг - от повторных срабатываний, т.к. MTZ_hold будет висеть до сброса.
		WrFramMicro(nvAdrTime, &mtzTimeBuff, MTZ_BUF_SIZE*sizeof(u32));
	}

	// Проверка на выброс из буфера mtzTimeBuf метки времени, если она старше MTZ_LIFE_TIME
	refreshFram = 0;
	for (i = 0; i < MAX_MTZ_COUNT; i++) {
		if (mtzTimeBuff[i] == 0)   continue; // Если ячейка пустая - пропускаем.
		tempTime = UnixTime - mtzTimeBuff[i]; // Находим разницу времени между текущим временем и временем последнего МТЗ
		if ( tempTime >  MTZ_LIFE_TIME ) { // Время жизни истекло - сбросить в ноль.
			mtzTimeBuff[i] = 0;
			refreshFram = 1;
		}
	}
	// Обновить FRAM, если ячейка была сброшена
	if (refreshFram) { WrFramMicro(nvAdrTime, &mtzTimeBuff, MTZ_BUF_SIZE*sizeof(u32)); }// Записать в FRAM

	// Проверяем верхнюю ячейку на квитирование
	if ( (s32)(UnixTime - mtzTimeBuff[0]) >= MTZ_ACK_TIME) {
		if (mPnt->FlagState.bit.MTZ_hold == 1) {
			mPnt->FlagState.bit.MTZ_hold = 0; // Квитируем текущую аварию МТЗ
			MtzLatchFlag = 0; // После квитирования текущего МТЗ, разрешаем получать новые.
		}
                // Check MTZ Timeout bit. If it was set - reset, and reset MTZ time buffer
                if (mPnt->FlagState.bit.MTZ_timeout == 1) {
                    mPnt->FlagState.bit.MTZ_timeout = 0;
                    
                    for (i = 0; i < MAX_MTZ_COUNT; i++) { 
                            mtzTimeBuff[i] = 0;
                    }
                    WrFramMicro(nvAdrTime, &mtzTimeBuff, MTZ_BUF_SIZE*sizeof(u32));
                }
	}

//	// Проверяем буфер на блокировку работы (3 МТЗ за 5 минут)
//	if ( mtzTimeBuff[MAX_MTZ_COUNT-1] ) {// Если последняя ячейка не равна нулю, значит поймали 3 МТЗ, и при этом не истекло время жизни МТЗ
//		if ( (mtzTimeBuff[0] - mtzTimeBuff[MAX_MTZ_COUNT-1]) < MTZ_LIFE_TIME  && (mtzTimeBuff[0] - mtzTimeBuff[MAX_MTZ_COUNT-1]) != 0 ) {
//			tempTime = UnixTime + (MTZ_BLOCKED_TIME/* - MTZ_LIFE_TIME*/ - MTZ_ACK_TIME); // Сдвигаем время всех ячеек, на время блокировки, минус время квитирования и время жизни МТЗ.
//			for (i=0; i<MAX_MTZ_COUNT; i++) {
//				mtzTimeBuff[i]=tempTime;
//			}
//			WrFramMicro(nvAdrTime, &mtzTimeBuff, MTZ_BUF_SIZE*sizeof(u32)); // Обновить FRAM
//		}
//		mPnt->FlagState.bit.MTZ_timeout = 1; // Фиксируем аварию MTZ Timeout
//	}
}
/************************** (C) Copyright 2020 Triol **************************/
