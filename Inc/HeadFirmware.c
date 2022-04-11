#include "HeadFirmware.h"
#include "stm32f4xx_hal.h"
#include "Global_include.h"

//=============================================================================
#warning ЗДЕСЬ ЗАДАЕМ ВЕРСИИ ПО В ЗАГОЛОВКАХ ПО
#if defined (_BOARD_CP24_V2_1) || defined (_BOARD_CP24_V2_0)
// Значение заголовка по умолчанию
__root static const HeadFirmware_type PrgVersion @ 0x8008000 = {
  
      "TRIOL AT24 LINE UC VER 168.01.1  BASE",      // Название версии на латинице пустые поля заполнить нулями
      0,0,                                          // UnixTime в новом (8 байтном) формате
      0,                                            // Номер привода
      0xffff,                                       // CRC16 номера привода
      0xffff,                                       // Фактический CRC16 основной программы
      {0},                                          // Резерв
      0xffff,                                       // CRC16 на всю прошивку после загрузки ПО в ПЧ 
      SIGN_PROG_INIT,                               // Контрольная сигнатура после загрузки ПО в ПЧ
      0xffff,                                       // CRC на весь заголовок
};       
#endif
//=============================================================================

//=============================================================================
#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1) 
// Значение заголовка по умолчанию
__root static const HeadFirmware_type PrgVersion @ 0x8008000 = {
  
      "TRIOL AT24 LINE UC VER 168.51.3 BASE",      // Название версии на латинице пустые поля заполнить нулями
      0,0,                                          // UnixTime в новом (8 байтном) формате
      0,                                            // Номер привода
      0xffff,                                       // CRC16 номера привода
      0xffff,                                       // Фактический CRC16 основной программы
      {0},                                          // Резерв
      0xffff,                                       // CRC16 на всю прошивку после загрузки ПО в ПЧ 
      SIGN_PROG_INIT,                               // Контрольная сигнатура после загрузки ПО в ПЧ
      0xffff,                                       // CRC на весь заголовок
};       
#endif
//=============================================================================

// Прототипы функций
u8 WriteFlash(u8 *Data, u32 Adr, u16 len);      // Записать массив данных во flash

// Контроль сигнатуры и crc на ПО
void CheckHeadFirmware(void){
  HeadFirmware_type   Head;
  u16                 Crc, Sign;
  
  // 1) Получить заголовок на ПО
  memcpy(&Head, &PrgVersion, sizeof(HeadFirmware_type));
  
  // 2) Проверить сигнатуру ПО
  if((Head.Signatura == SIGN_PROG_INIT)&&(Head.Crc == 0xffff)){
    // Похоже что ПО зашито программатором - необходимо востановить сигнатуру и crc
    Crc = crcPacket((u8*)BEGIN_PROG, SIZE_PROG);   // Контрольная сумма основного ПО
    Sign = SIGN_PROG;
    
    // Запишем сигнатуру и crc
    WriteFlash((u8*)&Sign, (u32)&PrgVersion.Signatura, sizeof(Sign));
    WriteFlash((u8*)&Crc, (u32)&PrgVersion.Crc, sizeof(Crc));
  }
}

// Записать массив данных во flash
u8 WriteFlash(u8 *Data, u32 Adr, u16 len){
  u16 i;

  // 1) Разблокировать флеш
  HAL_FLASH_Unlock();  
 
  for(i = 0; i < len; i++){
      // 2) Запись данных
      if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Adr, Data[i]) != HAL_OK){ // Запись во флеш по два байта
          HAL_FLASH_Lock();
          return 0;
      }

      Adr++;   // Инкримент адреса Flash памяти
  }

  // 3) Заблокировать флеш
  HAL_FLASH_Lock();    

  return 1;
}
