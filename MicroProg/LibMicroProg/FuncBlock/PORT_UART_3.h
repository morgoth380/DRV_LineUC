#include "MakeOpis.h"
#ifdef    GET_FUNC_CODE

typedef struct{
    s16     NetAdr;
    s16     Baudrate;
    u16     cntToClean;
    u16     cleanSignat_OK;
}StatePORT_UART_3_type;

typedef struct{
   u16      NetAdr;
   u16      Baudrate;
   u16      crc;
}nvStatePORT_UART_3_type;

#define         nvSfPortUart            (*(nvStatePORT_UART_3_type*)0)
#define         TIME_SET_DEFAULT        5* K_TIME_CALL_MICRO_TAKT
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_NetAdr;                       //1  NetAdr
    u16  Pnt_Baudrate;                     //2  Baudrate
    u16  Pnt_Time;                         //3  Time
    u16  Pnt_TC_on_off;                    //4  TC_on/off
    u16  Pnt_PortNumb;                     //5  PortNumb
    u16  Pnt_En_prm;                       //6  En_prm
    u16  Pnt_En_pusk;                      //7  En_pusk
    u16  Pnt_NotUsed;                      //8  Pnt_NotUsed( константа "0", бывший вход для modePort)
    u16  Pnt_Warning;                      //9  Warning
    u16  Pnt_TimeOver;                     //10 TimeOver
    u16  Pnt_Link;                         //11 Link          - наличие обмена по порту
    u16  Pnt_End;
}FuncPORT_UART_3_type;
//********************************************************

//********************************************************
u16 *FuncPORT_UART_3_1(FuncPORT_UART_3_type *progPnt, u32 ramPnt)
{
  
    u16                         out, crc;
    u16                         NetAdr, Baudrate;
    StatePORT_UART_3_type       *sPnt;
    u32                         tmp; 
    uart_type                   *uartLocl;
    nvStatePORT_UART_3_type     *nvAdr, nvMem;
    s16 			UstRecoveryFlagValue;
    u8                          cleanSignat = 0;
    void			*nvAdr1;
    
    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfPortUart);
    out  = load_s16(ramPnt, progPnt->Pnt_NetAdr);
    out  = load_s16(ramPnt, progPnt->Pnt_Baudrate);    
    out  = load_s16(ramPnt, progPnt->Pnt_PortNumb);

    switch (out){
        case 0: // WIFI
            uartLocl = &uart3;                          // Указатель на порт
            break;
        case 1: // PULT
            uartLocl = &uart6;                          // Указатель на порт
            break;
        case 2: // RS-485
            uartLocl = &uart1;                          // Указатель на порт
            #ifdef _PROJECT_FOR_VS2005_
            uartLocl = &uart0;                          // Указатель на порт АСУ в режиме симуляции в VS2005
            #endif
            break;
        case 3: // Вирутальный порт блока RS2 - канал 3
          uartLocl = &uartVirtual1;
          break;
        case 4: // Вирутальный порт блока RS2 - канал 4
          uartLocl = &uartVirtual2;
          break;          
        default:
            return &progPnt->Pnt_End;
    }
    
    //При старте проверяется КС, если она не совпала в nvstate записывается значение по умолчанию
    sPnt = (StatePORT_UART_3_type *)(ramPnt + progPnt->Pnt_State);

    clrBitMicro(ramPnt, progPnt->Pnt_Warning); // Сброс предупреждения
    NetAdr      = load_s16(ramPnt, progPnt->Pnt_NetAdr);
    Baudrate    = load_s16(ramPnt, progPnt->Pnt_Baudrate);
    
    if(flagIsFirstCall)  //начальная инициализация
    {
        RdFramMicro(&nvMem, nvAdr, sizeof(nvStatePORT_UART_3_type));
        crc = crcPacket((unsigned char *)&nvMem,  sizeof(nvStatePORT_UART_3_type) - sizeof(u16));

        nvAdr1 = (void *)((u32)&nv.UstRecoveryFlag);
        RdFramMicro( &UstRecoveryFlagValue, nvAdr1,  sizeof(u16)); // Проверяем флаг восстановления уставок

        if (crc != nvMem.crc || UstRecoveryFlagValue)
        {
            nvMem.Baudrate = Baudrate;
            nvMem.NetAdr   = NetAdr;
            nvMem.crc      = crcPacket((unsigned char *)&nvMem,  sizeof(nvStatePORT_UART_3_type) - sizeof(u16));
            WrFramMicro(nvAdr, &nvMem, sizeof(nvStatePORT_UART_3_type));
        }   

        sPnt->NetAdr    = nvMem.NetAdr;
        sPnt->Baudrate  = nvMem.Baudrate;
        save_s16(ramPnt, progPnt->Pnt_NetAdr, sPnt->NetAdr);
        save_s16(ramPnt, progPnt->Pnt_Baudrate, sPnt->Baudrate);
        uart_init(uartLocl, sPnt->NetAdr, sPnt->Baudrate, flagIsFirstCall); //          // Проинициализировать порт
        
        sPnt->cntToClean = 0;
        sPnt->cleanSignat_OK = 0;
        return &progPnt->Pnt_End;
    }
#ifdef _PROJECT_FOR_STM32_
    if(uartLocl == &uart3)      // UART для WIFI
    {      
      uartLocl->status = GlobalM4.status_WIFI;
      GlobalM4.SetUstDefault = 0;
      cleanSignat = DinControlWiFi();
      if((cleanSignat == WiFi_ModeProg) && (!sPnt->cleanSignat_OK))
      {
          sPnt->cntToClean++;
          if(sPnt->cntToClean >= TIME_SET_DEFAULT)
          {            
            // Проверяем не включен ли ШИМ? Иначе не разрешаем сброс на Default
            if (!GlobalM4.FlagCommand.bit.mWorkEnb && !GlobalM4.FlagCommand.bit.PwmOn) {
              Ust_Copy(PWR_to_RAM);
              Ust_Copy(RAM_to_NV);
              Ust_Copy(NV_to_RAM);
              sPnt->cleanSignat_OK = 1;
              GlobalM4.SetUstDefault = 1;
            }
          }             
      }
      else if(sPnt->cntToClean > 0)
          sPnt->cntToClean--;
    }
#endif

    // Проверяем настройки УАРТов, но НЕ разрешаем менять данные пульта. Хотя он должен быть невидим
    if (1/*uartLocl != &uart6*/)
    {
        if(((sPnt->NetAdr != NetAdr) || (sPnt->Baudrate != Baudrate)))
        {
          if (uartLocl == &uartVirtual1 && sPnt->NetAdr != NetAdr && NetAdr == uartVirtual2.netAdr) { 
            // Если изменились данные в виртульных портах блока RS2, надо проверить их адреса.
            // В этом блоке Адреса не должны повторяться.
            save_s16(ramPnt, progPnt->Pnt_NetAdr, sPnt->NetAdr); // Возвращаем старый адрес
            setBitMicro(ramPnt, progPnt->Pnt_Warning); // Выдаем предупреждение, что совпадают адреса
          } else if (uartLocl == &uartVirtual2 && sPnt->NetAdr != NetAdr && NetAdr == uartVirtual1.netAdr) { 
            save_s16(ramPnt, progPnt->Pnt_NetAdr, sPnt->NetAdr); // Возвращаем старый адрес 
            setBitMicro(ramPnt, progPnt->Pnt_Warning); // Выдаем предупреждение, что совпадают адреса
          } else { // Иначе можно менять настройки
            sPnt->Baudrate = nvMem.Baudrate = Baudrate;
            sPnt->NetAdr = nvMem.NetAdr   = NetAdr;
            nvMem.crc      = crcPacket((unsigned char *)&nvMem,  sizeof(nvStatePORT_UART_3_type) - sizeof(u16));
            WrFramMicro(nvAdr, &nvMem, sizeof(nvStatePORT_UART_3_type));
            uart_init(uartLocl, sPnt->NetAdr, sPnt->Baudrate, flagIsFirstCall);
          }
        }
    }

    // Контроль аварии связи
    out = 0;
    if(load_s16(ramPnt, progPnt->Pnt_TC_on_off) == 1){ 
        tmp = load_s16(ramPnt, progPnt->Pnt_Time) * K_TIME_CALL_MICRO_TAKT;  // Время в мс
        
        if(uartLocl->wdt >= tmp)    
            out = 1;
        else                    
            uartLocl->wdt++;
        
    }
    defBitMicro(ramPnt, progPnt->Pnt_TimeOver, out);
    

    // Проверим наличие связи
    if(uartLocl->WdtLink >= (2 * K_TIME_CALL_MICRO_TAKT)){    // 2 сек  
       setBitMicro(ramPnt, progPnt->Pnt_Link);                // связь отсутствует 
    } else {              
       uartLocl->WdtLink++;      
       clrBitMicro(ramPnt, progPnt->Pnt_Link);                // связь присутствует  
    }

    if( testBitMicro(ramPnt, progPnt->Pnt_En_prm) ) {    
        uartLocl->enablePrmChange = 1;
    } else {                                                
        uartLocl->enablePrmChange = 0;
    }

    if( testBitMicro(ramPnt, progPnt->Pnt_En_pusk))  {
        uartLocl->enablePusk = 1;
        if (uartLocl == &uart1) { // Если это канал АСУ, и разрешен пуск через АСУ
          // То надо для всех внешних каналов разрешить пуски по удаленным каналам
	  #ifdef _PROJECT_FOR_STM32_
          uartExt1.enablePusk = 1;
          uartExt2.enablePusk = 1;
	  #endif
        }
    } else {                                                
        uartLocl->enablePusk = 0;
        if (uartLocl == &uart1) { // Если это канал АСУ, и запрешен пуск через АСУ
          // То надо для всех внешних каналов запретить пуски по удаленным каналам
          #ifdef _PROJECT_FOR_STM32_
          uartExt1.enablePusk = 0;
          uartExt2.enablePusk = 0;
	  #endif
        }        
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPORT_UART_3_1[11]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  NetAdr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Baudrate
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Time
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  TC_on/off
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  PortNumb
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  En_prm
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  En_pusk
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  ModePort
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //9  Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  TimeOver
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //11 Link
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
//OpisFuncBlock("PORT_UART_3", "DD", TblFuncPORT_UART_3_1, FuncPORT_UART_3_1, sizeof(TblFuncPORT_UART_3_1), 0, 0, 1),
OpisFuncBlock("PORT_UART_3", "DD", TblFuncPORT_UART_3_1, FuncPORT_UART_3_1, 
sizeof(TblFuncPORT_UART_3_1),sizeof(StatePORT_UART_3_type) , sizeof(nvStatePORT_UART_3_type), 1),

#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPORT_UART_3_1,
#endif
//********************************************************
