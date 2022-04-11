#ifndef _FILERDWD_
#define _FILERDWD_

#include "nonvolatile.h"
#include "commonmicro.h"
/*============================================================================*/
//                Распределение памяти в AT45DB161
/*============================================================================*/
#define AT45_SIGNATURA NV_MEM_SIGNATURA

typedef struct{
    u16     DataIf     ;    // Полный ток
    u16     DataUd     ;    // Напряж Ud
    u16     DataFout   ;    // Выходн частота
    u16     DataAdd    ;    // Значение дополнительного параметра
}type_avarHltRec;
//=================================================
typedef struct {
    u32                 DateTime        ;   // Дата и врмя события
    u16                 NumAddPrm       ;   // Номер дополнительного параметра
    type_avarHltRec     avarHltRec[800];    // Записи со значениями параметров
    u16                 FileCRC         ;   // Контрольная сумма на файл
}type_hltPrmJrn;
/*============================================================================*/

// заголовок файла уставок
typedef struct {
    u16     Crc;
    u16     MicroSignatura;
    u16     SzUst;
    u16     rsrvd[61];
}HeadUst_type;

// Файл уставок
typedef struct {
   HeadUst_type HeadUst;                        // заголовок файла уставок
   u16          MicroProg[SZ_MICROPROG_DATA];   // файл уставок
}ustSet_type;



#define SZ_OF_PAGE  4096

typedef union {
    ustSet_type UstMicro;
}file_microprog_type;

//#define SIZE_FOP                  0x80000       // Размер ФОПа (в словах)
//#define SIZE_FIRMWARE_PULT_OP24B  0x10000       // Размер файла прошивки пульта OP24B (в словах) - размер пульта не более 128 кБ
//#define SIZE_FIRMWARE_MKAR        0x10000       // Размер файла прошивки MKAR (в словах) - 128 кб заложить
//#define SIZE_PULT_FONT_OP24B      0x10000       // Размер файла шрифтов пульта OP24B (в словах) 
//#define SIZE_FIRMWARE_PULT_OP24S  0x40000       // Размер файла прошивки пульта OP24S (в словах) 

#define SIZE_FOP                  0x80000       // Размер ФОПа (в словах)
#define SIZE_FIRMWARE_PULT_OP24S  0x40000       // Размер файла прошивки пульта OP24B (в словах) 
#define SIZE_FIRMWARE_MKAR        0x10000       // Размер файла прошивки MKAR (в словах) 
#define SIZE_PULT_FONT_OP24B      0x8000        // Размер файла шрифтов пульта OP24B (в словах) - предварительно 64 кБ
#define SIZE_PULT_GUIDE_OP24B     0x4000        // Размер файла визарда пульта OP24B (в словах) - предварительно 64 кБ
#define SIZE_FIRMWARE_PULT_OP24B  0xF000       // Размер файла прошивки пульта OP24S (в словах) - предварительно сделаем 256 кБ на пульт
#define SIZE_FOP_GZ               0x14000       // Размер ФОПа архивированный (в словах)

// Распределение Flash памяти
typedef struct {
    // 1) ФОП
    u16                 fileDescrPrm[SIZE_FOP];          
    u8                  rezmem[(SZ_OF_PAGE - ((sizeof(u16) * SIZE_FOP) % SZ_OF_PAGE))]; 
    
    // 2) Прошивка пульта OP24B
    u16                 filePultOP24S[SIZE_FIRMWARE_PULT_OP24S];
    u8                  rezmem1[(SZ_OF_PAGE - ((sizeof(u16) * SIZE_FIRMWARE_PULT_OP24S) % SZ_OF_PAGE))];
    
    // 3) Журнал аварий (пока не используется)
    type_hltPrmJrn      fileHltPrmJrn;                  
    u8                  rezmem2[(SZ_OF_PAGE - (sizeof(type_hltPrmJrn) % SZ_OF_PAGE))];
    
    // 4) Не известный файл в ПО не используется
    u16                 fileProg[0x7fe0];               
    u8                  rezmem3[(SZ_OF_PAGE - ((sizeof(u16) * 0x7fe0) % SZ_OF_PAGE))];
    
    // 5) 1-вый файл заводских уставок
    file_microprog_type fileMicroProg1;
    u8                  rezmem4[SZ_OF_PAGE - (sizeof(file_microprog_type) % SZ_OF_PAGE)];
    
    // 6) 2-вый файл заводских уставок
    file_microprog_type fileMicroProg2;
    u8                  rezmem5[SZ_OF_PAGE - (sizeof(file_microprog_type) % SZ_OF_PAGE)];
    
    // 7) 3-вый файл заводских уставок
    file_microprog_type fileMicroProg3;
    u8                  rezmem6[SZ_OF_PAGE - (sizeof(file_microprog_type) % SZ_OF_PAGE)];
    
    // 8) Файл шрифтов пульта OP24B
    u16                 filePultFontOP24B[SIZE_PULT_FONT_OP24B];
    u8                  rezmem7[(SZ_OF_PAGE - ((sizeof(u16) * SIZE_PULT_FONT_OP24B) % SZ_OF_PAGE))];
    
    // 9) Прошивка пульта OP24S
    u16                 filePultOP24B[SIZE_FIRMWARE_PULT_OP24B];
    u8                  rezmem8[(SZ_OF_PAGE - ((sizeof(u16) * SIZE_FIRMWARE_PULT_OP24B) % SZ_OF_PAGE))];    

    // 10) Файл ФОПа в архиве gz
    u16                 fileDescrPrmGz[SIZE_FOP_GZ];
    u8                  rezmem9[(SZ_OF_PAGE - ((sizeof(u16) * SIZE_FOP_GZ) % SZ_OF_PAGE))];

    // 11) Файл гида пульта OP24B
    u16                 filePultOP24BGuide[SIZE_PULT_GUIDE_OP24B];
    u8                  rezmem10[(SZ_OF_PAGE - ((sizeof(u16) * SIZE_PULT_GUIDE_OP24B) % SZ_OF_PAGE))];

//    // 8) Файл прошивки модуля МКАР в блоке СР24В
//    u16                 fileMkarCP24B[SIZE_FIRMWARE_MKAR];
//    u8                  rezmem7[(SZ_OF_PAGE - ((sizeof(u16) * SIZE_FIRMWARE_MKAR) % SZ_OF_PAGE))];
    
}spfl_type;

#define     spfl        (*(spfl_type*)0)        // Для обращения к АТ45

/*============================================================================*/
#define    NfilePrm         0
#define    NfileMicroProg   1
#define    NfileProg        2

#define    NfileAlarmJrn    10
#define    NfileHltJrn      11
#define    NfileKalendOdd   12


#define    NfileRamMicroprog 20
#define    NfileRamLoger     21

/*============================================================================*/
//          Тип для FAT таблицы
typedef struct{
    u32     adres;
    u32     size;
    u16     alloc;
    u16     flags;
}FATtable_type;

#define    N_Files          28  // Колич файлов
/*============================================================================*/
//          Области хранения файла для FAT таблицы
#define     FILE_In_ERR         0   // Ошибка такого файла нет
#define     FILE_In_Flash       1   // Файл лежит в FLASH памяти
#define     FILE_In_Fram        2   // Файл лежит в FRAM памяти
#define     FILE_In_Ram         3   // Файл лежит в ОЗУ
/*============================================================================*/
//          Флаги для файлов
#define     FILE_ReadOnly       0
#define     FILE_RdWr           1


extern const FATtable_type FATtable[N_Files];

/*============================================================================*/
//extern const FATtable_type  FATtable[N_Files];
/*============================================================================*/
u16 WrAt45FromFram(u32 adr, u16 size);                  // AT45->FRAM
u16 RdAt45ToFram(u32 adr, u16 size);                    // FRAM->AT45

void* adrInMem(u16 file, u32 adr);
int checkAdrInFile(u16 file, u32 adr, u16 size);
void unpackData(char *dst, const char *srs, u16 sizeU16);
void packData(char *dst, const char *srs, u16 sizeU16);
void fm25readFile(void);
void fm25writeFile(void);
u16 fileRd(unsigned char *Bff, u16 file, u32 adr, u16 size );
u16 fileWr(u16 file, u32 adr, unsigned char *Bff,  u16 size );
u16 GrupOneAt45ToFram(u32 Grup, u16 size);         //AT45->FRAM (одну группу)
/*============================================================================*/

#endif
