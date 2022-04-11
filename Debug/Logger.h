#ifndef LOGGER_H
#define LOGGER_H

#include "type.h"

#define debTimeLockWriteLog 		250 		/* 1 сек */

extern u16 TimeOneTick;

// Тип логгера
typedef enum{
	typeLog_normal,	    // 0 Пишем без останова
	typeLog_start,      // 1 Пишем время после пуска по заряду
	typeLog_startPwmOn, // 2 Пишем время после пуска по PWMon
	typeLog_PwmOn,	    // 3 Пишем время до останова по PWMon
	typeLog_NoFail,	    // 4 Пишем время до возникновения аварии
        typeLog_startCmd,   // 5 Пишем время после подачи команды пуск ПЧ
}typeLog_type;

// Типы переменных логгера
typedef enum{
	VarLog_s16,	    // 0 Тип переменной логгера s16
        VarLog_s32,         // 1 Тип переменной логгера s32
        VarLog_float,       // 2 Тип переменной логгера float
        VarLog_iq,          // 3 Тип переменной логгера float iq
}VarLog_type;

// Структура для описания переменной логгера
typedef struct{
	void          *AdresVar;    // Указатель на переменную
        VarLog_type   VarLogType;   // Тип переменной
        float         Scale;        // Масштаб переменной
}LogVarTable_type;

extern const LogVarTable_type debPntVar[];	// Массив с описанием переменных

// Номера переменных храним тут
typedef struct{
	u16  n[9];
}LogPnt_type;

extern 	LogPnt_type LogPnt;


#define szExtRAMmem 	  8016UL                // Размер массива под логгер
#define szExtRAMmemData   (szExtRAMmem - 16)

extern   u16 	ExtRAMmem[szExtRAMmem];         // Файл логгера

// Структура заголовка логгера
typedef struct{
	u16	Ntic;		// Количество тиков в прореживалке
	u16	Nvar;		// Количество переменных в логе
	u32	LogInd;		// Указатель на текущую запись
	u32	Sz;		// Количество отсчетов.
	u16     TimeOneTick;    //
	u16	rez2;		// Резерв
}headOfLogFile_type;

// Структура файла логгера
typedef struct{
	headOfLogFile_type  hof;                     // Заголовок
	s16		    data[ szExtRAMmemData ]; // Массив с данными
}logFile_type;


typedef struct{
    headOfLogFile_type 	    HOFlogFile;
    LogPnt_type 	    dataPnt;
    typeLog_type    	    TypeLog;
    s16                     cmpPrescalLog1;
    u16                     LogLen;
    u16                     logEndTmr;
    u16                     NlogVar;
    u16                     TimeOneTick;
    u16                     ResetLogInd;
}logSetup_type;

extern headOfLogFile_type 	HOFlogFile;

extern logFile_type 	*const logFile;


extern s16		*pntLogEnd;

void LogN9( void );


#if LOGER_GRAPH_IS_PRESENT != 0
        

#endif


void setupLogFileHOF( void * dst);

#endif	// LOGGER_H




