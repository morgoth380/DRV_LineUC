#ifndef HEADFIRMWARE_H
#define HEADFIRMWARE_H

#include "type.h"

#define BEGIN_PROG       0x08008200L   // Адрес начала основной программы 
#define SIZE_PROG        0x77dffL      // Размер области основной программы 

#define SIGN_PROG_INIT   0x5a5a        // Сигнатура на ПО по умолчанию, при запуске основного ПО перезаписываем на 2
#define SIGN_PROG        2             // Сигнатура на ПО (штатный режим)

// Структура заголовка прошивки
typedef struct{
    u8      NameDRV[40];         // Название версии на латинице пустые поля заполнить нулями
    u32     UnixTime[2];         // UnixTime в новом (8 байтном) формате 
    u32     NumDRV;              // Номер привода
    u16     CrcNumDRV;           // CRC16 номера привода (контрольная блокировка от вмешательства)
    u16     CrcHex;              // Фактический CRC16 основной программы, которая будет приложена в виде HEX-файла
    u8      reserv[18];          // Резерв 
    u16     Crc;                 // CRC16 на всю прошивку после загрузки ПО в ПЧ (при формировании заголовка 0xFFFF)
    u16     Signatura;           // Контрольная сигнатура после загрузки ПО в ПЧ (при формировании заголовка 0x5a5a)
    u16     CrcHead;             // CRC на весь заголовок

}HeadFirmware_type;  

// Прототипы функций
void CheckHeadFirmware(void);                       // Контроль сигнатуры и crc на ПО

#endif // HEADFIRMWARE_H



