#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "Global_include.h"
#include "GlobalVar.h"
#include "FileRdWr.h"

u16 opcodeRdUST = RDUST;    //задача чтения для ат45
u16 opcodeWrUST = WRUST;    //задача записи для ат45
type_FlashMessage  msg, *pmsg;
/*============================================================================*/
// Таблица с указателями на начало файла, размер, физическое расположение, флаги файла
const FATtable_type FATtable[N_Files] = {
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 0
    (u32)&spfl.fileDescrPrm     , sizeof(spfl.fileDescrPrm  ), FILE_In_Flash  , FILE_RdWr    , // 1 Файл описателя параметров (ФОП)
    (u32)&spfl.fileHltPrmJrn    , sizeof(spfl.fileHltPrmJrn ), FILE_In_Flash  , FILE_ReadOnly, // 2
    (u32)&spfl.fileProg         , sizeof(spfl.fileProg      ), FILE_In_Flash  , FILE_RdWr    , // 3  ..Файл распределения памяти ОЗУ микропрограммы
    (u32)&spfl.fileMicroProg1   , sizeof(spfl.fileMicroProg1), FILE_In_Flash  , FILE_RdWr    , // 4 заводские уставки
    (u32)&spfl.fileMicroProg2   , sizeof(spfl.fileMicroProg2), FILE_In_Flash  , FILE_RdWr    , // 5 пользовательский набор №1
    (u32)&spfl.fileMicroProg3   , sizeof(spfl.fileMicroProg3), FILE_In_Flash  , FILE_RdWr    , // 6 пользовательский набор №2
    (u32)&nv.file.ElCountero    , sizeof(nv.file.ElCountero ), FILE_In_Fram   , FILE_RdWr    , // 7
    (u32)&spfl.filePultOP24S    , sizeof(spfl.filePultOP24S ), FILE_In_Flash  , FILE_RdWr    , // 8 Цветной пульт
    (u32)&nv.DK_file            , sizeof(nv.DK_file)         , FILE_In_Fram   , FILE_RdWr    , // 9
    (u32)&nv.file.FailJrn       , sizeof(nv.file.FailJrn  )  , FILE_In_Fram   , FILE_ReadOnly, // 10
    (u32)&nv.file.WarnJrn       , sizeof(nv.file.WarnJrn  )  , FILE_In_Fram   , FILE_ReadOnly, // 11
    (u32)&nv.file.HolidayOdd    , sizeof(nv.file.HolidayOdd) , FILE_In_Fram   , FILE_RdWr    , // 12 файл нечетного года
    (u32)&nv.file.HolidayEven   , sizeof(nv.file.HolidayEven), FILE_In_Fram   , FILE_RdWr    , // 13 файл четного года
    (u32)&MPOpis.Base           , sizeof(MPOpis.Base)        , FILE_In_Ram    , FILE_ReadOnly, // 14
    (u32)&nv.MicroProg[0]       , sizeof(nv.MicroProg)       , FILE_In_Fram   , FILE_ReadOnly, // 15 ..Файл описания микропрограммы ( Уставки !?)
    (u32)&((MicroProg_type *)MicroProgTabl)->kolvUst, sizeof(((MicroProg_type *)MicroProgTabl)->kolvUst), FILE_In_Ram, FILE_ReadOnly, //16
    (u32)&nv.MemSignatura       , sizeof(nv.MemSignatura)    , FILE_In_Fram   , FILE_ReadOnly,  // 17
    (u32)&nv.fileCurFunct       , sizeof(nv.fileCurFunct)    , FILE_In_Fram   , FILE_RdWr,      // 18
    (u32)&nv.trend_file         , sizeof(nv.trend_file  )    , FILE_In_Fram   , FILE_ReadOnly,  // 19
    (u32)&nv                    , sizeof(nv)                 , FILE_In_Fram   , FILE_ReadOnly,  // 20 ..Файл распределения энергонезависимой
    (u32)&nv.fire_list          , sizeof(nv.fire_list)       , FILE_In_Fram   , FILE_ReadOnly,  // 21
    (u32)&ExtRAMmem 	        , sizeof(ExtRAMmem)	     , FILE_In_Ram    , FILE_ReadOnly,	// 22
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly,  // 23 Пока файл заблокирован
    (u32)&spfl.filePultFontOP24B, sizeof(spfl.filePultFontOP24B), FILE_In_Flash, FILE_RdWr   ,  // 24 Файл шрифтов пульта OP24B     
    (u32)&spfl.filePultOP24B    , sizeof(spfl.filePultOP24B ), FILE_In_Flash  , FILE_RdWr    ,  // 25 Прошивка пульта OP24B - Черно-белый
    (u32)&spfl.filePultOP24BGuide, sizeof(spfl.filePultOP24BGuide ), FILE_In_Flash , FILE_RdWr    ,  // 26 Файл гида настройки для чб пульта
    (u32)&spfl.fileDescrPrmGz   , sizeof(spfl.fileDescrPrmGz ), FILE_In_Flash , FILE_RdWr    ,  // 27 Файл описателя параметров в архиве gz
};

//    (u32)&spfl.fileMkarCP24B    , sizeof(spfl.fileMkarCP24B) , FILE_In_Flash  , FILE_RdWr    ,      // 23 Файл прошивки для модуля МКАР блока СР24В
#define numLogFile 22

/*============================================================================*/
//       Распаковка данных для передачи по протоколу модбас
// size в словах по 2 байта!!!
/*============================================================================*/
#if(CHAR_BIT == 8)
void unpackData(char *dst, const char *srs, u16 sizeU8)
{
 while(sizeU8--){
        *dst++ = *srs++;
    }  
}
void unpackData0x68(char *dst, const char *srs, u16 sizeU8)
{
    u16 * pack = (u16*)dst;

    sizeU8 /= 2;
    while(sizeU8--)
    {
        *pack = ((u16)*srs << 8);
        srs++;
        *pack = (*pack + *srs++);
        *pack++;
    }
}
#else
void unpackData(char *dst, const char *srs, u16 sizeU8)
{
    u16 i = 0;

    sizeU8 = 2*sizeU8;
    while(sizeU8--){
        *dst++ = __byte((int*)srs, i+1);
        *dst++ = __byte((int*)srs, i);
        i+=2;
    }
}
#endif

#if(CHAR_BIT == 8)
void packData(char *dst, const char *srs, u16 sizeU8)
{
    u16 * pack = (u16*)dst;

    sizeU8 /= 2;
    while(sizeU8--)
    {        
//        *pack = ((*srs++ << 8) + *srs++);
        *pack   = *srs++ << 8;
        *pack  += *srs++;
        *pack++;
        
    }
}
#else
void packData(char *dst, const char *srs, u16 sizeU8)
{
    u16 i = 0;

    sizeU8 = 2*sizeU8;
    while(sizeU8--){
        __byte((int*)dst, i++) = *((int*)srs+1);
        __byte((int*)dst, i++) = *(int*)srs;
        srs += 2;
    }
}
#endif

/*============================================================================*/
//           Упаковка данных из формата модбас в 2 байтовые слова
// size в словах по 2 байта!!!
/*============================================================================*/
/*============================================================================*/
// Проверка запроса на допустимый адрес по номеру файла адресу и количеству регистров
// Возвращает номер с указанием области хранения файла
/*============================================================================*/
int checkAdrInFile(u16 file, u32 adr, u16 size)
{    
    if(file < N_Files)
    {
        if((adr + size) > FATtable[file].size)     
            return 0;
        else                                        
            return FATtable[file].alloc;
    }
    else
        return 0;
}
/*============================================================================*/
//                 Вычисление адреса по номеру файла
/*============================================================================*/
void * adrInMem(u16 file, u32 adr)
{
    return (void*)(FATtable[file].adres + adr);
}
/*============================================================================*/
//                          Файловое чтение
//      Функция ЗАВЕШИВАЕТ процесс на семафоре и ожидает ответа.
// Возвращает код ошибки и в буфере возвращает ответ для модбаса.
/*============================================================================*/
void log4msFunc(void);

u16 fileRd(unsigned char *Bff, u16 file, u32 adr, u16 size)//SEM_Obj
{
    u16                 opcode, memType;
    type_FlashMessage    msg;                    // Структура с запросом к процессу
    type_FlashMessage    *pmsg = &msg;

    // Проверка на допустимый диапазон файлов
    if(size == 0)                                           
        return ErrMb(Bff, Illegal_DataVal_merr);
    if((memType = checkAdrInFile(file, adr<<1, size<<1)) == 0 )   
        return ErrMb(Bff, Illegal_DataAdr_merr);


    opcode      = RD;
    msg.opCode  = &opcode;                      // Действие / статус
    msg.dest    = Bff+8+2;                      // Куда    // Потом распакуем
    msg.srs     = adrInMem(file, adr<<1);       // Откуда
    msg.size    = (size*2);                     // Сколько
    msg.ptrSem  = FlashSemRdWr;                  // указатель на семафор

    switch(memType)
    {
      case FILE_In_Flash:
          xQueueSend(xQueueFlash, &pmsg, portMAX_DELAY);
          osSemaphoreAcquire(FlashSemRdWr, portMAX_DELAY);               // Завесить процесс на ожидание записи
      break;

      case FILE_In_Fram:
          framRd(pmsg->srs,  pmsg->dest, pmsg->size);                   // Чтение данных из FRAM
      break;

      case FILE_In_Ram:
        
          memcpy(msg.dest, msg.srs, msg.size);
          if( file == numLogFile )
          {
            GlobalM4.debLockWriteLogReq = 1;                                    // Заблокировать запись логера на момент чтения файла
            if( adr == 0 )
            {
                setupLogFileHOF( msg.dest );
                log4msFunc();
            }
          }
      break;
    }
          unpackData0x68( (char*)Bff+8, (char*)(Bff+10), size*2);               //

    if((*msg.opCode & ERRat45sign) != 0 )
        return ErrMb(Bff, SLAVE_DEVICE_BUSY_merr);
    else
        return  size*2+8;                   // Длина ответа
}

/*============================================================================*/
//                          Файловая запись
//      Функция ЗАВЕШИВАЕТ процесс на семафоре и ожидает ответа.
// Возвращает код ошибки и в буфере возвращает ответ для модбаса.
/*============================================================================*/
u16 fileWr(u16 file, u32 adr, unsigned char *Bff,  u16 size)
{
    u16                 opcode, memType;
    type_FlashMessage    msg;            // Структура с запросом к процессу
    type_FlashMessage    *pmsg = &msg;

    // Проверка на допустимый диапазон файлов
    if(size == 0)                                           
      return ErrMb(Bff, Illegal_DataVal_merr);
    if((memType = checkAdrInFile(file, adr<<1, size<<1)) == 0 )   
      return ErrMb(Bff, Illegal_DataAdr_merr);

    opcode = WR;
    msg.opCode  = &opcode;                              // Действие / статус
    msg.dest    = adrInMem(file, adr<<1);               // Куда  потом  запакуем
    msg.srs     = Bff+6;                                // Откуда
    msg.size    = (size*2);                             // Сколько
    msg.ptrSem  = FlashSemRdWr;                          // указатель на семафор
    
    unpackData0x68( (char*)Bff+6, (char*)(Bff+8), size*2);               //
    
    switch(memType)
    {
      case FILE_In_Flash:
            xQueueSend(xQueueFlash, &pmsg, portMAX_DELAY);
            osSemaphoreAcquire(FlashSemRdWr, portMAX_DELAY);             // Завесить процесс на ожидание записи
      break;
      
      case FILE_In_Fram:
            framWr(pmsg->dest, pmsg->srs, pmsg->size);                  // Запись данных из FRAM
      break;
      
      case FILE_In_Ram:                                                 // запись в RAM. Возможно стоить убрать case ибо нет файлов, которые пишем с UART в ОЗУ.
            memcpy(msg.dest, msg.srs, msg.size);
      break;
    }
    
    if( (*msg.opCode & ERRat45sign) != 0 )  
      return ErrMb(Bff, SLAVE_DEVICE_BUSY_merr);
    else    
        return 3;                       // Длина ответа

}

//***************************************************************
// Чтение уставкок из AT45 в FRAM
// Входные данные:
//   - указатель на буфер в который читаем
//   - значение адреса AT45 откуда читать
//   - количество байт
//   - возвращает TRUE, если задача была запущена, false, если задача занята
//****************************************************************
u16 RdAt45ToFram(u32 adr, u16 size)
{
    pmsg = &msg;
    opcodeRdUST = RDUST;
    msg.opCode = &opcodeRdUST;
    msg.srs     = (void *)adr;                  // Откуда
    msg.size    = size;                         // Сколько
    msg.ptrSem  = NULL;                         // указатель на семафор

    return xQueueSend(xQueueFlash, &pmsg, portMAX_DELAY);
}
/***************************************************************/
// Запись в AT45 из FRAM
// Входные данные:
//   - указатель на записываемый буфер
//   - значение адреса куда писать
//   - количество байт
//   - возвращает TRUE, если задача была запущена, false, если задача занята
//****************************************************************
u16 WrAt45FromFram(u32 adr, u16 size)
{
    
    pmsg = &msg;
    opcodeWrUST = WRUST;
    msg.opCode = &opcodeWrUST;
    msg.dest     = (void *)adr;                 // Куда
    msg.size    = size;                         // Сколько
    msg.ptrSem  = NULL;                         // указатель на семафор

    return xQueueSend(xQueueFlash, &pmsg, portMAX_DELAY);
}
//****************************************************************

//***************************************************************
// Чтение одной уставки из AT45 в FRAM
// Входные данные:
//   - указатель на буфер в который читаем
//   - значение адреса AT45 откуда читать
//****************************************************************
u16 GrupOneAt45ToFram(u32 Grup, u16 size)
{
    pmsg = &msg;                  //указатель на объект посылки
    opcodeRdUST = RDONEUST;       //Инициализируем переменную для кода операции
    msg.opCode  = &opcodeRdUST;   //формируем ссылку на код операции
    msg.srs     = NULL;//(void *)adr;       // Откуда
    msg.size    = Grup;//sizeof(s32);       //Вместо размера (msg.size) пишем номер группы !!!!!!!!!!!!!!!
    msg.ptrSem  = NULL;                     // указатель на семафор

    return xQueueSend(xQueueFlash, &pmsg, portMAX_DELAY);
}
#endif