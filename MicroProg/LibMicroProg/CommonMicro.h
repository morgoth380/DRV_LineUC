#ifndef CommonMicro_H
#define CommonMicro_H

#include "DrvSpi.h"

#define kolvShort_RAM_MICROPROG     4096

typedef struct{
    u16     functMask;                      //Маска функционала
}Functional_type;

//********************************************************
typedef u16 *(* pFuncMicro_type)(const u16 *, u32);
//********************************************************
//описатель функции в *.dll таблице
typedef struct{
                char                *SymbolName     ;   //Имя элемента   "MUX8"
                char                *Prefix         ;   //Префикс        "DD"
                const char          *typePin        ;   //указатель на таблицу прототипа
                pFuncMicro_type     FuncPnt         ;   //указатель на функцию 
                u16                 kolvPin         ;   //количество ножек у графического символа
                u16                 sizeofState     ;   //размер ОЗУ используемый функцией для хранения внутренних состояний
                u16                 sizeofNVState   ;   //объём энергонезависимой памяти используемый функцией
                u16                 Exe             ;   //=1 блок является исполняемым
                u16                 sizeofInputStruct;  //размер входной структуры
                u16                 al              ;   //не используется
                }OpisFuncMicroProg_type;
//********************************************************
#define OpisFuncBlock(SymbolName, Prefix, typePin, FuncPnt, kolvPin, sizeofState, sizeofNVState, Exe) {SymbolName, Prefix, typePin, (pFuncMicro_type)FuncPnt, kolvPin, sizeofState, sizeofNVState, Exe}
#define OpisFuncBlockNew(SymbolName, Prefix, typePin, FuncPnt, kolvPin, sizeofState, sizeofNVState, Exe, sizeofInputStruct) {SymbolName, Prefix, typePin, (pFuncMicro_type)FuncPnt, kolvPin, sizeofState, sizeofNVState, Exe, sizeofInputStruct}

typedef struct{
                u16                 AdrVal          ;   //адрес значения (СОНОДМ)
                s16                 LowLim          ;   //значение нижнего предела или номер переменной в микропрограмме где лежит значение
                s16                 HighLim         ;   //значение нижнего предела или номер переменной в микропрограмме где лежит значение
                u16                 StepEdIsm       ;   //
                u32                 Flags           ;   //
                u16                 LedName[3]      ;   //имя для светодиодного пульта
                u16                 maskFuncEnable  ;
                }OpisPrm_type;

//********************************************************
typedef struct{
                u16                 PntNumOpisPrm   ;
                u16                 kolvPrmInGrp    ;
                u16                 LedName[4]      ;   //имя для светодиодного пульта
                }OpisGrp_type;
//********************************************************
//Структура микропрограммы. Лежит во внутренней флеше МК. Не содержит текстов названий
//СОНМ      - смещение относительно начала микропрограммы
//СОНОДМ  - смещение относительно начала области данных микропрограммы
typedef struct{
                u32                 CrcMicroprog    ;   //контрольная сумма микропрограммы
                u16                 SignatCoreMicro ;   //Требуемая сигнатура ядра для работы с микропрограммой
                u16                 szFlash         ;   //Размер всей этой структуры с закоментированными полями
//указатели на секции в области ОЗУ микропрограммы (выровненные по 4 байта)
                u16                 kolvRam_All     ;   //общий оъём ОЗУ требуемый для микропрограммы в char
                u16                 kolvNvRam_All   ;   //общий оъём NV ОЗУ требуемый для микропрограммы в char
                
                u16                 Pnt_float_Sect  ;   //Указатель на float секцию    (СОНОДМ)
                u16                 Pnt_s32_Sect    ;   //Указатель на s32 секцию  (СОНОДМ)
                u16                 Pnt_s16_Sect    ;   //Указатель на s16 секцию  (СОНОДМ)
                
                u16                 Sz_bit_Sect     ;   //размер bit секции (СОНОДМ)
//              
                u16                 PntRam_Mb_0x05  ;   //указатель на массив переменных 0x05 команды Modbus
                u16                 kolv_Mb_0x05    ;   //количество структур 
                
                u16                 PntConst_float  ;   //секция const float (СОНМ)
                u16                 kolvConst_float ;   //количество const float
                
                u16                 PntConst_s32    ;   //секция const s32 (СОНМ)
                u16                 kolvConst_s32   ;   //количество const s32
                
                u16                 PntConst_s16    ;   //секция const s16 (СОНМ)
                u16                 kolvConst_s16   ;   //количество const s16
                
                u16                 Reserv[16]      ;   //
                
                u16                 PntOpisGrp      ;   //указатель на описатели групп
                u16                 kolvGrp         ;   //количество описателей групп
                
                u16                 PntOpisUst      ;   //описатели усатвок (СОНМ)
                u16                 kolvUst         ;   //количество уставок
                
                u16                 PntOpisTek      ;   //описатели текущих параметров (СОНМ)
                u16                 kolvTek         ;   //количество текущих параметров
                
                u16                 PntMicroProg    ;   //указатель на тело микропрограммы (СОНМ)
                u16                 kolvFuncMicroProg;  //количество функций в микропрограмме

                u16                 PntPwrValUst    ;   //значения уставок после прошивки (СОНМ)
//---новое             
                u16                 CRC16_NV        ;   //контрольная сумма файла энергонезависимой памяти (сигнатура)
                
                u16                 PntModbus_0x2B  ;   //указатель на тело команды 0x2B
                u16                 kolvModbus_0x2B ;   //размер тела 0x2B
                
            }MicroProg_type;

//********************************************************
typedef struct{
                const MicroProg_type    *PntMicroProg           ;   //заголовок микропрограммы
                const OpisGrp_type      *PntOpisGrp             ;   //указатель на описатели групп
                const OpisPrm_type      *PntOpisUst             ;   //указатель на описатели уставок
                const OpisPrm_type      *PntOpisTek             ;   //указатель на описатели текущих параметров
                u16                     *PntBase                ;   //указатель на базы параметров
                u32                     ramPnt                  ;   //указатель на ОЗУ микропрограммы приведенный к u32
                const u16               *CodeMicroProg          ;   //указатель на таблицу с кодом функциональных блоков микропрограммы
                u16                     kolvGrpOpis             ;   //количество описателей групп
                u16                     kolvPrmOpis             ;   //количество описателей параметров
                s16                     Base[16]                ;   //значения баз
                f32                     BaseFloat[16]           ;   //значение баз во Float без учета знаков после запятой
                f32                     BaseFloatConverse[16]   ;   //значение баз во Float без учета знаков после запятой
                s16                     kolvState               ;   //количество состояний выполняемой функции
                u32                     cntCall                 ;   //количество вызовов микропрограммы
                u32                     cntCallStop             ;   //пошаговая отладка
                u16                     NumFuncStop             ;   //пошаговая отладка
                u16                     NumFunc                 ;   //номер выполняемой функции
                u16                     kolvFunc                ;   //количество функций в микропрограмме
                }MPOpis_type;
//********************************************************
typedef struct{
    u16     val;                // Текущее состояние
    u16     mask;       
    u16     state;              // Состояние катушек
    u16     rezerv;             // Резерв для выравнивания
}StateFuncMODBUS_0X05_type;
//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Out;                         //1  Out
    u16  Pnt_formula;                     //2  formula
    u16  Pnt_Arg[2];                      //3
}FuncFORMULA_X_type;
//********************************************************

typedef enum{
    FreqUser_base = 1,
    Volt_base,
    Current_base,
    Power_base,
    Proc_base,
    Herz_base,
    RefPID_base,
    Torq_base,             // База по моменту
}NumBase_type;
#define MaxNumBase 16

#define SS_GRP				(42)
#define NUM_SS_PARAMS		(16)
#define NPAR_NGRP_CLEAR		(DoAdrPrm(SS_GRP, NUM_SS_PARAMS))	// Address of the CLEAR param
#define PAR_BLACK_LIST_LEN	(3)									// Black list length

#define GetNumGrpFromAdrPrm(AdrPrm) ((AdrPrm>>7)&0x7F)
#define GetNumPrmFromAdrPrm(AdrPrm) (AdrPrm&0x7F)
#define DoAdrPrm(Ngrp, Nprm)        ((Nprm&0x7F)|((Ngrp&0x7F)<<7))

#define AddAdr(arg1, arg2)  ((void *)((u32)arg1 + (u32)arg2))
//********************************************************
#define minInt(x0,x1)   ((x0>x1) ? x1 : x0)
#define maxInt(x0,x1)   ((x0>x1) ? x0 : x1)

s16 s16LimControl(s32   val);

void setBit(void *ramPnt, u16 Nbit);
//********************************************************

int MicroProgWrCoil(s16 Adr, s16 val);
//********************************************************
//квантование и таймера
s32 TimeFromPusk(void);
s32 GetTimePowerOff(void);  //сколько времени питание было отключено
s16 IsSecondKvant(void);
//********************************************************
//командоаппарат
s16 PwmIsOn(void);
s16 DrvIsStop(void);

//********************************************************
extern u8 RamAreaTxBuffer[4096];
//********************************************************
//работа с памятью

#ifndef _PROJECT_FOR_STM32_
    int FastMemRd(void *kuda, void *otkuda, int skolko);
    int FastMemWr_1(void *kuda, void *otkuda, int skolko);
    #define WrFramMicro(AdrFram, Adr, sz)   FastMemWr_1(AdrFram, Adr, sz)
    #define RdFramMicro(Adr, AdrFram, sz)   FastMemRd(Adr, AdrFram, sz) //sz в байтах
    
    int es_ReadSpfl(void *buf, int adr, int size);
    int es_WriteSpfl(void *buf, int adr, int size);
    #define AT45RD(buf, adr, sz)            es_ReadSpfl(buf, adr, sz)
    #define AT45WR(buf, adr, sz)            es_WriteSpfl(buf, adr, sz)
    

#else
    #define WrFramMicro(AdrFram, Adr, sz)   framWr(AdrFram, Adr, sz)
    #define RdFramMicro(Adr, AdrFram, sz)   framRd(AdrFram, Adr, sz)    //sz в байтах
    
    
#endif

#define WrAt45FromFram(adr, size) WrAt45FromFram(adr, size)

u16 spflWr(void *AdrSpfl, void *AdrLpc, u16 skolko);
u16 spflRd(void *AdrLpc, void *AdrSpfl, u16 skolko);
int es_ReadDrvUst(void *buf, int adr, int size, char *fName);
//********************************************************
//**************************************************************************
//Другое
float IQ_to_FLOAT(float value, u8 numBase);
float FLOAT_to_IQ(float value, u8 numBase);
float FiltrValue(u16 tFiltr, float tekValue, float prevValue);
s16 Filtr(filtr_type *fPnt, s16 val, s16 Kfiltr);
s32    Fy_x1x2y1y2x(s32 x1, s32 x2, s32 y1, s32 y2, s32 x);
u32 u_Fy_x1x2y1y2x(u32 x1, u32 x2, u32 y1, u32 y2, u32 x);
float f32_Fy_x1x2y1y2x(float x1, float x2, float y1, float y2, float x);
s16 FuncFORMULA_X_s16(FuncFORMULA_X_type *progPnt, u32 ramPnt, s16 kolvOperand);
float iq_Filtr_prm(float valOld, float valNew, s16 Kfiltr);
//****************************************************************************
u32 MicroProg_Init(u32 ramPnt, const MicroProg_type *mPnt);
void CoreMicroProg(u32 ramPnt, MPOpis_type  *mPnt);

//********************************************************
extern const u16 MicroProgTabl[];


extern const pFuncMicro_type    TblMicroProgFunc[];
long __IQmpy(long A, long B, int Qval);


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/
#ifdef  DEFINE_CommonMicro_GLOBAL
#define CommonMicro_GLOBAL                  
#else   
#define CommonMicro_GLOBAL extern
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/

    CommonMicro_GLOBAL  Functional_type curFunct;
    CommonMicro_GLOBAL  MPOpis_type     MPOpis;
    CommonMicro_GLOBAL  u16             flagIsFirstCall;
    CommonMicro_GLOBAL  u16             CrcProgFlashM3;
 
#endif  //CommonMicro_H
