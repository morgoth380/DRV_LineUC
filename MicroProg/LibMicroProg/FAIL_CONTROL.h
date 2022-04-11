
#ifndef	FAIL_CONTROL_H
#define FAIL_CONTROL_H
#include "Global_include.h"

// Defines
//#define FastFailMask_NONE           0
//#define FastFailMask_SwU            1       // Ошибка по сигналу от силового ключа U
//#define FastFailMask_SwV            2       // Ошибка по сигналу от силового ключа V
//#define FastFailMask_SwW            3       // Ошибка по сигналу от силового ключа W
//#define FastFailMask_SwT            4       // Ошибка по сигналу от силового тормозного ключа
//#define FastFailMask_MtzD           5       // МТЗ по дискретному сигналу от компаратора
//#define FastFailMask_MtzA           6       // МТЗ программное по уставке МТЗ
//#define FastFailMask_R7             7       // резерв
//#define FastFailMask_UdMin          8       // Низкое Ud
//#define FastFailMask_UdMax          9       // Высокое Ud
//#define FastFailMask_UdCharge       10      //
//#define FastFailMask_Ud_KZ          11      //
//#define FastFailMask_Tu             12      // Температура ключей
//#define FastFailMask_r13            13      // резерв
//#define FastFailMask_r14            14      // резерв
//#define FastFailMask_r15            15      // резерв
//
//#define SlowFailMask_Contactor      34      // ошибка контактора
//********************************************************
typedef struct{
	s32		DelayApvCnt;        //таймер АПВ
	s16		Request;            //копия из слота по которому АПВ
	s16		N_STOP;                //причина останова
	s16		N_PUSK;                //причина пуска
}FailControlState_type;

typedef struct{
	s16  	DelayApvCnt;
	s16		N_STOP;                //причина останова
	s16		N_PUSK;                //причина пуска
}nvFailControlState_type;
#define		nvFailControlState	        (*(nvFailControlState_type*)0)

typedef struct{
    u16		Pnt_TekFail;       //TekFail
    u16		Pnt_Request;       //Request
    u16		Pnt_Time;          //Time
    u16		Pnt_TypeTorm;      //TypeTorm
}SlotFc_type;


//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Man_Auto;        //1  Man/Auto	    
    u16  Pnt_KolvFail;        //2  KolvFail
    u16  Pnt_MaskTek_3;       //3  MaskTek_3	Маска текущих аварий с учётом задержки контроля
    u16  Pnt_MaskTek_2;       //4  MaskTek_2
    u16  Pnt_MaskTek_1;       //5  MaskTek_1
    u16  Pnt_MaskTek_0;       //6  MaskTek_0
    u16  Pnt_N_TekFail;       //7  N_TekFail	Текущая авария с наивысшим приоритетом
    u16  Pnt_APV_budet;       //8  APV_budet	Если режим Автомат и есть источник сгенерировавший АПВ.
    u16  Pnt_Type_torm;       //9  Type_torm	Тип торможения
    
    u16  Pnt_TimeDoOff;       //10  TimeDoOff	Время до отключения
    u16  Pnt_near_STOP;       //11  N_STOP	    Источник ближайшего останова (Если источника нет, то = 0)
    u16  Pnt_N_STOP;          //12  N_STOP	    Источник текущего останова (в работе = 0)
    
    u16  Pnt_TimeDoAPV;       //13  TimeDoAPV	Время до АПВ
    u16  Pnt_near_PUSK;       //14  N_PUSK	    Источник ближайшего пуска (Если источника нет, то = 0)
    u16  Pnt_N_PUSK;          //15  N_PUSK	    Источник текущего пуска (в останове = 0)
    
	SlotFc_type	SlotFc[4];
    u16  Pnt_End;
}FuncTEXN_CONTROL_4_type;
//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Man_Auto;        //1  Man/Auto	    
    u16  Pnt_KolvFail;        //2  KolvFail
    u16  Pnt_MaskTek_3;       //3  MaskTek_3	Маска текущих аварий с учётом задержки контроля
    u16  Pnt_MaskTek_2;       //4  MaskTek_2
    u16  Pnt_MaskTek_1;       //5  MaskTek_1
    u16  Pnt_MaskTek_0;       //6  MaskTek_0
    u16  Pnt_N_TekFail;       //7  N_TekFail	Текущая авария с наивысшим приоритетом
    u16  Pnt_APV_budet;       //8  APV_budet	Если режим Автомат и есть источник сгенерировавший АПВ.
    u16  Pnt_Type_torm;       //9  Type_torm	Тип торможения
    
    u16  Pnt_TimeDoOff;       //10  TimeDoOff	Время до отключения
    u16  Pnt_near_STOP;       //11  N_STOP	    Источник ближайшего останова (Если источника нет, то = 0)
    u16  Pnt_N_STOP;          //12  N_STOP	    Источник текущего останова (в работе = 0)
    
    u16  Pnt_TimeDoAPV;       //13  TimeDoAPV	Время до АПВ
    u16  Pnt_near_PUSK;       //14  N_PUSK	    Источник ближайшего пуска (Если источника нет, то = 0)
    u16  Pnt_N_PUSK;          //15  N_PUSK	    Источник текущего пуска (в останове = 0)
    
	SlotFc_type	SlotFc[32];
    u16  Pnt_End;
}FuncFAIL_CONTROL_32_type;
//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Man_Auto;        //1  Man/Auto	    
    u16  Pnt_KolvFail;        //2  KolvFail
    u16  Pnt_MaskTek_3;       //3  MaskTek_3	Маска текущих аварий с учётом задержки контроля
    u16  Pnt_MaskTek_2;       //4  MaskTek_2
    u16  Pnt_MaskTek_1;       //5  MaskTek_1
    u16  Pnt_MaskTek_0;       //6  MaskTek_0
    u16  Pnt_N_TekFail;       //7  N_TekFail	Текущая авария с наивысшим приоритетом
    u16  Pnt_APV_budet;       //8  APV_budet	Если режим Автомат и есть источник сгенерировавший АПВ.
    u16  Pnt_Type_torm;       //9  Type_torm	Тип торможения
    
    u16  Pnt_TimeDoOff;       //10  TimeDoOff	Время до отключения
    u16  Pnt_near_STOP;       //11  N_STOP	    Источник ближайшего останова (Если источника нет, то = 0)
    u16  Pnt_N_STOP;          //12  N_STOP	    Источник текущего останова (в работе = 0)
    
    u16  Pnt_TimeDoAPV;       //13  TimeDoAPV	Время до АПВ
    u16  Pnt_near_PUSK;       //14  N_PUSK	    Источник ближайшего пуска (Если источника нет, то = 0)
    u16  Pnt_N_PUSK;          //15  N_PUSK	    Источник текущего пуска (в останове = 0)
    
	SlotFc_type	SlotFc[64];
    u16  Pnt_End;
}FuncFAIL_CONTROL_64_type;
//********************************************************
void FuncFAIL_CONTROL(FuncFAIL_CONTROL_64_type *progPnt, u32 ramPnt);
//********************************************************
#endif	//FAIL_CONTROL_H
