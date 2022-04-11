#define DEFINE_Modbus_GLOBAL
#include "Global_Include.h"
#include "FuncBlock/MASTER_SLAVE.h"
//#include "usart.h"
//#include "DrvUart.h"

u16 fileRd(unsigned char *Bff, u16 file, u32 adr, u16 size);
u16 fileWr(u16 file, u32 adr, unsigned char *Bff, u16 size);

//****************************************************************************
wm_type wmem; // Глобальная структура 
u16     WIFI_FOP_Action;
//u16     MODBUS_FILE_RDWR;
//u16     MODBUS_PWM_ON;
//****************************************************************************
// Обработка пакетов по порту UART
u16 ModbusFunc(unsigned char *Bff, char len, uart_type *uartLocl) // return(len)
{
    u16 sz, CrcVal;
      
    if ((Crc(Bff, len)) || flagIsFirstCall)
        return 0;
    if (Bff[0] != uartLocl->netAdr) {
        if (!((Bff[0] == 0) && (Bff[1] == 0x2B)))
            return 0;
    }

    Bff[0] = (char) uartLocl->netAdr;
    
    if (flagIsFirstCall == 1){
        Bff[1] |= 0x80;
        Bff[2] = SLAVE_DEVICE_BUSY_merr; // Устройчтво не готово
        sz = 3;
    } else {
        wmem.enablePrmChange = uartLocl->enablePrmChange; // Авторизация по номеру порта

        switch (Bff[1]) { //номер функции
        case 0x03:
        case 0x04:
            sz = modbus_0x03_4_67(Bff, len, 0);
            break;
        case 0x05:
        // Проверка авторизации на пуск. Если пуск запрещен - выдаем ошибку.
            if( ((uartLocl->enablePusk == 0) && ((((u16)Bff[2] << 8) | Bff[3]) < 0x08))/* || (GlobalM3.Regim_Privod == REGIM_SLAVE)*/ )  // Проверка авторизации на ПУСК 
            { 
                  Bff[1] = 0x85;
                  Bff[2] = Illegal_Function_merr;
                  sz = 3;
            } 
            else 
              sz = modbus_0x05(Bff, len);
            break;
        case 0x06:
            sz = modbus_0x06(Bff, len);
            break;
        case 0x10:
            sz = modbus_0x10(Bff, len);
            break;
        case 0x2B:
            sz = modbus_0x2B(Bff, len);
            break;
        case 0x64:
            sz = modbus_0x64(Bff, len);              // пошаговая отладка микропрограммы
            break; 
        case 0x65:
            sz = modbus_0x65(Bff, len);              // навигация по быстрому журналу
            break; 
        case 0x66:
            sz = modbus_0x66(Bff, len);              // чтение файлов из ОЗУ 70
            break; 
        case 0x67:
            sz = modbus_0x03_4_67(Bff, len, 1);      // чтение степени единиц измерений
            break; 
        case 0x68:
            if (Bff[2] == 1 && uartLocl == &uart3)
              WIFI_FOP_Action = 1;

            // Если ШИМ включен - запрет использования файлового чтения/записи через Модбас
            // Флаг ставим в DRV_V5
            if (0/*MODBUS_PWM_ON*/) { 
                Bff[1] |= 0x80;
                Bff[2] = SLAVE_DEVICE_BUSY_merr; // Устройчтво не готово
                sz = 3;
            } else {            
//                MODBUS_FILE_RDWR = 250; // Счетчик блокировки - пока не ноль, запрет обмена с блоком расширения, уменьшаем в микропрограмме FACTORY_SET
                sz = modbus_0x68(Bff, len);              // файловое чтение из файла
            }
            break; 
        case 0x69:
            if (Bff[2] == 1 && uartLocl == &uart3)
              WIFI_FOP_Action = 1;
            
            // Если ШИМ включен - запрет использования файлового чтения/записи через Модбас
            if (0/*MODBUS_PWM_ON*/) {
                Bff[1] |= 0x80;
                Bff[2] = SLAVE_DEVICE_BUSY_merr; // Устройчтво не готово
                sz = 3;
            } else {            
//                MODBUS_FILE_RDWR = 250; // Счетчик блокировки - пока не ноль, запрет обмена с блоком расширения
                sz = modbus_0x69(Bff, len);              // файловая запись в файл
            }
            break; 

        default:
            Bff[1] |= 0x80;
            Bff[2] = Illegal_Function_merr;
            sz = 3;
        }
    }

	if ((!sz) || (sz > 254)) {         // Не более 254 байт, т.к. внизу еще два байта CRC будут добавлены и в итоге получаем 256 байт. А 256 - лимит для Модбас.
		if (Bff[1] != 0x43) {      // Для функции 0х43 длина пакета может превышать стандартный размер
    		return 0;                  // Если это не 0x43 функция - то выходим с нулем.
    	}
    }

    CrcVal = Crc(Bff, sz);
    Bff += sz;
    *Bff++ = CrcVal & 0xFF;           //low
    *Bff++ = (CrcVal >> 8) & 0xFF;    //high
    return sz + 2;
}
//****************************************************************************
int ErrMb(unsigned char *Bff, int code) //sz + заполненный буфер
{
    Bff[1] |= 0x80;
    Bff[2] = code;
    return 3;
}
//****************************************************************************
int modbus_0x03_4_67(unsigned char *Bff, int len, int StepEdIsm) //sz + заполненный буфер
{
    u16 Adr, Kolv, tmp;
    unsigned char *pnt;
    wm_type *wm;

    wm = &wmem;
    if (len != 8)
        return 0;
    Adr = (Bff[2] << 8) | Bff[3];
    Kolv = (Bff[4] << 8) | Bff[5];

    pnt = Bff + 2; //отступ на NetAdr, Func
    *pnt++ = Kolv * 2; //ByteCount
    if ((Kolv > 0x7D) || (Kolv < 1))
        return ErrMb(Bff, Illegal_DataVal_merr);
    if (Adr & EXT_OPIS_PRM_ADR)
        return ExtInfoPrm(Bff, Adr, Kolv);
    for (; Kolv > 0; Kolv--, Adr++) {
        wm->AdrPrm = Adr;
        tmp = ModbusMakeOpis(wm, 1);
        if (tmp)
            return ErrMb(Bff, tmp);
        if (StepEdIsm) {
            *pnt++ = wm->StepEdIsm >> 8;
            *pnt++ = wm->StepEdIsm & 0xFF;
        } else {
            *pnt++ = wm->Val2byte >> 8;
            *pnt++ = wm->Val2byte & 0xFF;
        }
    }
       
    return Bff[2] + 3;
}

//****************************************************************************
#define BigEndSave_2b(AdrPnt, Val)      {(AdrPnt)[0] = (unsigned short)((Val>>8)&0xFF); (AdrPnt)[1] = (unsigned short)(Val&0xFF);}
#define BigEndSave_4b(AdrPnt, Val)      {(AdrPnt)[0] = (unsigned short)((Val>>8)&0xFF); (AdrPnt)[1] = (unsigned short)((Val>>0)&0xFF); (AdrPnt)[2] = (unsigned short)((Val>>24)&0xFF); (AdrPnt)[3] = (unsigned short)((Val>>16)&0xFF);}
int ExtInfoPrm(unsigned char *Bff, u16 Adr, u16 Kolv) {
    wm_type *wm;
    typedef union {
        SysPrm2_type SysPrm2;
        SysPrm4_type SysPrm4;
    } dataPrm_type;
    unsigned char *pntDst;

    wm = &wmem;
    wm->AdrPrm = Adr;
    MakeOpis(wm, 1);

    pntDst = Bff + 3; //отступ на NetAdr, Func и к-во байтов
    if (wm->TypeVar == sec2Frmt) {
        return ErrMb(Bff, Illegal_DataAdr_merr);
    } else if (wm->kolvShort == 1) {
        *pntDst++ = (unsigned char) ((wm->s32.Value >> 8) & 0xFF); // текущая величина
        *pntDst++ = (unsigned char) (wm->s32.Value & 0xFF);
        Kolv--;
        if (Kolv) {
            *pntDst++ = (unsigned char) ((wm->StepEdIsm >> 8) & 0xFF); // степень и единица изм.
            *pntDst++ = (unsigned char) (wm->StepEdIsm & 0xFF);
            Kolv--;
        }
        if (Kolv) {
            *pntDst++ = (unsigned char) ((wm->s32.LowLim >> 8) & 0xFF); // нижний предел
            *pntDst++ = (unsigned char) (wm->s32.LowLim & 0xFF);
            Kolv--;
        }
        if (Kolv) {
            *pntDst++ = (unsigned char) ((wm->s32.HighLim >> 8) & 0xFF); // верхний предел
            *pntDst++ = (unsigned char) (wm->s32.HighLim & 0xFF);
            Kolv--;
        }
        if (Kolv) {
            *pntDst++ = (unsigned char) ((wm->Flags >> 8) & 0xFF); // флаги
            *pntDst++ = (unsigned char) (wm->Flags & 0xFF);
            *pntDst++ = (unsigned char) ((wm->Flags >> 24) & 0xFF);
            *pntDst++ = (unsigned char) ((wm->Flags >> 16) & 0xFF);
        }
    } else {
        *pntDst++ = (unsigned char) ((wm->s32.Value >> 8) & 0xFF); // текущая величина
        *pntDst++ = (unsigned char) (wm->s32.Value & 0xFF);
        *pntDst++ = (unsigned char) ((wm->s32.Value >> 24) & 0xFF);
        *pntDst++ = (unsigned char) ((wm->s32.Value >> 16) & 0xFF);
        Kolv -= 2;
        if (Kolv) {
            *pntDst++ = ((wm->StepEdIsm >> 8) & 0xFF); // степень и единица изм.
            *pntDst++ = (wm->StepEdIsm & 0xFF);
            Kolv--;
        }
        if (Kolv) {
            *pntDst++ = (unsigned char) ((wm->s32.LowLim >> 8) & 0xFF); // нижний предел
            *pntDst++ = (unsigned char) (wm->s32.LowLim & 0xFF);
            *pntDst++ = (unsigned char) ((wm->s32.LowLim >> 24) & 0xFF); // Gahov
            *pntDst++ = (unsigned char) ((wm->s32.LowLim >> 16) & 0xFF); // Gahov
            Kolv -= 2; // Gahov
        }
        if (Kolv) {
            *pntDst++ = (unsigned char) ((wm->s32.HighLim >> 8) & 0xFF); // верхний предел
            *pntDst++ = (unsigned char) (wm->s32.HighLim & 0xFF);
            *pntDst++ = (unsigned char) ((wm->s32.HighLim >> 24) & 0xFF); // Gahov
            *pntDst++ = (unsigned char) ((wm->s32.HighLim >> 16) & 0xFF); // Gahov
            Kolv -= 2; // Gahov
        }
        if (Kolv) {
            *pntDst++ = (unsigned char) ((wm->Flags >> 8) & 0xFF); // флаги
            *pntDst++ = (unsigned char) (wm->Flags & 0xFF);
            *pntDst++ = (unsigned char) ((wm->Flags >> 24) & 0xFF);
            *pntDst++ = (unsigned char) ((wm->Flags >> 16) & 0xFF);
        }
    }
    
    return Bff[2] + 3;
}
//****************************************************************************
//*****************Суходольский 24.4.2018***********************************************************
//u16 AAAA = 0x0001;

int WRITE_TO_FRAM(unsigned int ADR, int DAN)
{
    u16 tmp;
    wm_type *wm;

    wm = &wmem;

    wm->Val2byte = DAN; //формируем 2-х байтное слово с двух считанных байт
    wm->AdrPrm = ADR;
    tmp = ModbusMakeOpis(wm, 0);
    return tmp;
}
//****************************************************************************

//****************************************************************************
u16 AdrStop = 2;
int modbus_0x05(unsigned char *Bff, int len) //sz + заполненный буфер
{
    u16 tmp, Adr, Val2byte;

    if (len != 8)
        return 0;
    Adr = (Bff[2] << 8) | Bff[3];
    Val2byte = (Bff[4] << 8) | Bff[5];

    if (Val2byte == 0x0000) {
        tmp = 0;
    } //Stop
    else if (Val2byte == 0xFF00) {
        tmp = 1;
    } //Pusk 
    else
        return ErrMb(Bff, Illegal_DataVal_merr);

    if (Adr == AdrStop) {
        AdrStop++;
        AdrStop--;
    }

    tmp = MicroProgWrCoil(Adr, tmp);
    if (tmp == 0)
        return len - 2;
    if (tmp == 1)
        return ErrMb(Bff, Illegal_DataAdr_merr);
    if (tmp == 2)
        return ErrMb(Bff, Failure_merr);
    return len - 2;
}
//****************************************************************************
int modbus_0x06(unsigned char *Bff, int len) //sz + заполненный буфер
{
    u16 tmp;
    wm_type *wm;

    wm = &wmem;
    if (len != 8)
        return 0;
    wm->Val2byte = (Bff[4] << 8) | Bff[5]; //формируем 2-х байтное слово с двух считанных байт
    wm->AdrPrm = (Bff[2] << 8) | Bff[3];
    
    tmp = ModbusMakeOpis(wm, 0);
    if (tmp)
        return ErrMb(Bff, tmp);
    return 6;
}
//****************************************************************************
int modbus_0x10(unsigned char *Bff, int len) //sz + заполненный буфер
{
    u16 Adr, Kolv, tmp;
    unsigned char *pntVal;
    wm_type *wm;

    wm = &wmem;
    Adr = (Bff[2] << 8) | Bff[3];
    Kolv = (Bff[4] << 8) | Bff[5];

    if (len != (9 + Kolv * 2))
        return 0;
    if ((Kolv < 1) || (Kolv >= 0x78) || (Bff[6] != 2 * Kolv))
        return ErrMb(Bff, Illegal_DataVal_merr);

    pntVal = &Bff[7];
    for (; Kolv > 0; Kolv--, Adr++, pntVal += 2) {
        wm->Val2byte = (pntVal[0] << 8) | pntVal[1];
        wm->AdrPrm = Adr;
        tmp = ModbusMakeOpis(wm, 0);
        if (tmp)
            return ErrMb(Bff, tmp);
    }
    return 6;
}
/*============================================================================*/
/*============================================================================*/
//               Копирование строки без 0 в конце
// Возвращает количество символов выведенных в буфер
/*============================================================================*/
u16 strcpywoz(u16 *dst, u16 *srs) {
    u16 cnt = 0;

    while (*srs) {
        cnt++;
        *dst++ = *srs++;
    }
    return cnt;
}
//****************************************************************************
int modbus_0x2B(unsigned char *Bff, int len) //sz + заполненный буфер
{ 
  u16 Count, LenTmp;
  
    // 1) Проверим длинну запроса
    if(len != 7){
        return 0;     // Не может такого быть! Ошибка
    }
    
    // 2) Провека адреса    
    if(Bff[2] != 0x0E){
        return ErrMb(Bff, Illegal_DataAdr_merr);   // Ошибка адреса
    }
    
    // 3) Проверка данных
    if (Bff[3] != 1){
        return ErrMb(Bff, Illegal_DataVal_merr);   // Ошибка данных
    }
    
    // Read Dev Id Code Чтение основных идентификаторов потоком
    Count = 1;
    
    Bff[Count++] = 0x2b;         // Function
    Bff[Count++] = 0x0e;         // MEI_Type  
    Bff[Count++] = 0x01;         // DevId_Code
    Bff[Count++] = 0x01;         // More_Follows
    Bff[Count++] = 0x00;         // Conf_Level
    Bff[Count++] = 0x00;         // Next_Obj_Id
    Bff[Count++] = 0x03;         // Num_Of_Obj
    
    LenTmp = strlen(VENDOR_NAME);
    Bff[Count++] = 0x00;            // Obj_Id
    Bff[Count++] = LenTmp;          // Obj_Len
    memcpy(&Bff[Count], VENDOR_NAME, LenTmp);
    
    Count = Count + LenTmp;
    LenTmp = strlen(PRODUCT_CODE);
    Bff[Count++] = 0x01;           // Obj_Id_1
    Bff[Count++] = LenTmp;         // Obj_Len_1   
    memcpy(&Bff[Count], PRODUCT_CODE, LenTmp);
    
    Count = Count + LenTmp;
    LenTmp = strlen(MAJOR_MINOR_REV);
    Bff[Count++] = 0x02;           // Obj_Id_2
    Bff[Count++] = LenTmp;         // Obj_Len_2 
    memcpy(&Bff[Count], MAJOR_MINOR_REV, LenTmp);
    
    return Count + LenTmp;  
}  
//****************************************************************************
//                          Файловое чтение
//****************************************************************************
u16 modbus_0x68(unsigned char *Bff, u16 len) {
    union {
        u32 L;
        u16 W[2];
    } adr;
    if(Bff[7] > 0x7A)           // размер больше 122 слов
        return Lim_err;
    if (len != 10)
        return 0;
    adr.W[0] = (Bff[5] << 8) | Bff[6];
    adr.W[1] = (Bff[3] << 8) | Bff[4];

    return fileRd(Bff, Bff[2], adr.L, Bff[7] );
}
//****************************************************************************
//                          Файловая запись
//****************************************************************************
u16 modbus_0x69(unsigned char *Bff, u16 len) {
    union {
        u32 L;
        u16 W[2];
    } adr;

    adr.W[0] = (Bff[5] << 8) | Bff[6];
    adr.W[1] = (Bff[3] << 8) | Bff[4];

#ifdef _PROJECT_FOR_STM32_
    fileWr(Bff[2], adr.L, Bff, Bff[7] );
#endif // _PROJECT_FOR_STM32_
    return 3;
}
//****************************************************************************
//char  NetAdr
//char  Nfunc
//char  size
//char  Nfile
//short AdrInFile
//short CRC

//char  NetAdr
//char  Nfunc
//char  size 
//char  reserv 
//short DATA
//short CRC
//****************************************************************************
void swapData(u8 *dst, const u16 *srs, u16 size)
{
    u16 i = 0;
    //u8  t1, t2;

    while(size--){
        //t1 = ((u8*)srs)[i+1];
        //t2 = ((u8*)srs)[i];
        *dst++ = ((u8*)srs)[i+1];
        *dst++ = ((u8*)srs)[i];
        i+=2;
    }
}

extern AllPrm_Type *AllPrmPnt;

//u16 modbus_0x66(u8 *Bff, u16 len)                  //sz + caiieiaiiue aooa?
//{
//    u16             Adr;
//    u16            *dataPtr;
//
//    if(len!=8)   return 0;
//
//    Adr = (Bff[4]<<8)|Bff[5];
//    len =  Bff[2];
//
//    if((len > 125)||(len < 1))      return ErrMb(Bff,Illegal_DataVal_merr);
//    if(Bff[3] == 0x20){         //NfileRamMicroprog
//        if((Adr + len) > MPOpis.PntMicroProg->kolvRam_All)      return ErrMb(Bff,Illegal_DataVal_merr);
//        dataPtr = (u16 *)AllPrmPnt;
//    }
//#if(LOGER_IS_PRESENT == 1)
//    else if(Bff[3] == 0x21){    //NfileRamLoger
//        if((Adr + len) > SZ_Bff_LOGER)      return ErrMb(Bff,Illegal_DataAdr_merr);
//#warning	Проверить разрядности указателей, были внесены изменения!
//        dataPtr = (void *)&LogerMem;
//    }
//#endif  //  LOGER_IS_PRESENT
//    else    return ErrMb(Bff,Illegal_DataAdr_merr);
//    
//    swapData(&Bff[4], &dataPtr[Adr], len);
//    
//    return      len*2 + 4;
//}

u16 modbus_0x66(u8 *Bff, u16 len)                  //sz + caiieiaiiue aooa?
{
  
    u16   static   cnt = 0;
    u16             Adr;
    u16            *dataPtr;
    if(len!=8)   return 0;
    Adr = (Bff[4]<<8)|Bff[5];
    len =  Bff[2];
    if((len > 125)||(len < 1))      {
       return ErrMb(Bff,Illegal_DataVal_merr);
    }
    if(Bff[3] == 0x19){         //NfileRamMicroprog
        if((Adr + len) > MPOpis.PntMicroProg->kolvRam_All) {     
           return ErrMb(Bff,Illegal_DataVal_merr);
        }
        dataPtr = (u16 *)AllPrmPnt;
        dataPtr += MPOpis.PntMicroProg->Sz_bit_Sect >> 1;               //РАЗМЕР СЕКЦИИ В БАЙТАХ, СМЕЩНЕИЕ В СЛОВАХ
    }else if(Bff[3] == 0x20){         //NfileRamMicroprog
        if((Adr + len) > MPOpis.PntMicroProg->kolvRam_All) {
           return ErrMb(Bff,Illegal_DataVal_merr);
        }
        dataPtr = (u16 *)AllPrmPnt;
    }
#if(LOGER_IS_PRESENT == 1)
    else if(Bff[3] == 0x21){    //NfileRamLoger
        if((Adr + len) > SZ_Bff_LOGER)      return ErrMb(Bff,Illegal_DataAdr_merr);
#warning Проверить разрядности указателей, были внесены изменения!
        dataPtr = (void *)&LogerMem;
    }
#endif  //  LOGER_IS_PRESENT
    else    
      return ErrMb(Bff,Illegal_DataAdr_merr);
    
    swapData(&Bff[4], &dataPtr[Adr], len);
    cnt++;
    return      len*2 + 4;
}

//****************************************************************************
//char  NetAdr
//char  Nfunc
//short NumFuncStop
//s32   cntCallStop
//short CRC

//char  NetAdr
//char  Nfunc
//short CRC
//****************************************************************************
u16 modbus_0x64(unsigned char *Bff, u16 len) //sz + заполненный буфер
{
    if (len != 10)
        return 0;
    MPOpis.cntCallStop = ((u32) Bff[4] << 24) | ((u32) Bff[5] << 16)
            | ((u32) Bff[6] << 8) | ((u32) Bff[7] << 0);
    MPOpis.NumFuncStop = ((u16) Bff[2] << 8) | ((u16) Bff[3] << 0);
    return 2;
}
//****************************************************************************
//char  NetAdr
//char  Nfunc
//char  PrintfVal
//char  ResetLogJrn
//s16   CRC

//char  NetAdr
//char  Nfunc
//s16   indexLogJrn
//s16   szBffLogJrn
//s16   typeLogJrn
//s16   CRC
//****************************************************************************
u16 modbus_0x65(unsigned char *Bff, u16 len) //sz + заполненный буфер
{
    //if (len != 6)
        return 0;
}
