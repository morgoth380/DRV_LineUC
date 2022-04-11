#ifndef ModbusSlave_H
#define ModbusSlave_H

#include "type.h"
#include "DrvUart.h"

#define MB_MEM_SIZE 128

// Коды заданий/операций функции 0x43
#define Set_IQ_Freq_MstSlv      0x01 // Задание по частоте
#define Set_Start_Stop_MstSlv   0x02 // Задание на пуск/стоп
#define Set_Torque_Freq_MstSlv   0x03 // Задание момента скорости и слова состояния
//****************************************************************************
#define     AllOk_merr              0
#define     Illegal_Function_merr   1
#define     Illegal_DataAdr_merr    2
#define     Illegal_DataVal_merr    3
#define     Failure_merr            4
#define     SLAVE_DEVICE_BUSY_merr  6
//****************************************************************************

#define GetOffset(Pnt1, Pnt2)       ((u32)(Pnt1) - (u32)(Pnt2))

//****************************************************************************
extern wm_type wmem;   // Глобальная структура 
extern u16     WIFI_FOP_Action;
//****************************************************************************
//младшие 11 бит  - адреса параметров 0...2047    
//старшие 5 бит   - адресация по структуре
//****************************************************************************
 typedef struct {
                u16     Value       ;
                u16     StepEdIsm   ;
                u16     LowLim      ;
                u16     HighLim     ;
                u32     Flags       ;
        }SysPrm2_type;
//****************************************************************************
 typedef struct {
                u32     Value       ;
                u16     StepEdIsm   ;
                u16     zer         ;
                u32     LowLim      ;
                u32     HighLim     ;
                u32     Flags       ;
        }SysPrm4_type;
//****************************************************************************
u16 ModbusFunc(unsigned char *Bff, char len, uart_type *uartLocl);  //ret(len)

int ErrMb(unsigned char *Bff, int code);                            // sz + заполненный буфер
int modbus_0x03_4_67(unsigned char *Bff, int len, int StepEdIsm);   // sz + заполненный буфер
int modbus_0x05(unsigned char *Bff, int len);                       // sz + заполненный буфер
int modbus_0x06(unsigned char *Bff, int len);                       // sz + заполненный буфер
int modbus_0x10(unsigned char *Bff, int len);                       // sz + заполненный буфер
int modbus_0x2B(unsigned char *Bff, int len);                       // sz + заполненный буфер
u16 modbus_0x64(unsigned char *Bff, u16 len);                       // пошаговая отладка микропрограммы
u16 modbus_0x65(unsigned char *Bff, u16 len);                       // навигация по быстрому журналу   
u16 modbus_0x66(unsigned char *Bff, u16 len);                       // чтение файлов из ОЗУ 70        
u16 modbus_0x67(unsigned char *Bff, u16 len);                       // чтение степени единиц измерений
u16 modbus_0x68(unsigned char *Bff, u16 len);                       // файловое чтение из флеши
u16 modbus_0x69(unsigned char *Bff, u16 len);                       // файловая запись в флеш  

int ExtInfoPrm(unsigned char *Bff, u16 Adr, u16 Kolv);
//****************************************************************************
int MakeOpis_un(wm_type     *wm,
                int         AdrPrm,
                int         cmd,
                int         Rd);
//****************************************************************************
unsigned short Crc(unsigned char *Bff, unsigned short szBff);
u16 crcPacket(unsigned char *Bff, u32 szBff);
unsigned short CrcPortion(unsigned short *Bff, unsigned short szBff, unsigned short PrevCrc);
unsigned short CrcPortion8(unsigned char *Bff, unsigned short szBff, unsigned short PrevCrc);
void swapData(u8 *dst, const u16 *srs, u16 size);

extern const char TblSuecnType[6];
extern const char TblModbus_0x2B[37];
//****************************************************************************
#ifdef  DEFINE_Modbus_GLOBAL
    #define Modbus_GLOBAL                   
#else   
    #define Modbus_GLOBAL extern
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/


//****************************************************************************
#endif



