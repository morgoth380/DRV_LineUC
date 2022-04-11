#ifdef    GET_FUNC_CODE
#ifdef _NVSA_
    #include "NVSADrvUart.h"
#else /** _NVSA_ */
    #include "NVSACDrvUart.h"
#endif /** _NVSA_ */

typedef struct{
    s16     NetAdr;
    s16     Baudrate;
}StatePORT_UART_2_type;

typedef struct{
   u16      NetAdr;
   u16      Baudrate;
   u16      CRC;
}nvStatePORT_UART_2_type;

//extern Tuart * const uartPtr[5] = { NULL, &uart1, &uart2, &uart3, &uart4}; // Массив указателей на порты

#define     nvSfPortUart     (*(nvStatePORT_UART_2_type*)0)

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
    u16  Pnt_TimeOver;                     //8  TimeOver
    u16  Pnt_End;
}FuncPORT_UART_2_type;
//********************************************************
u16 *FuncPORT_UART_2_1(FuncPORT_UART_2_type *progPnt, u32 ramPnt)
{
    u16                         out, crc;
    u16                         NetAdr, Baudrate;
    StatePORT_UART_2_type       *sPnt;
    u32                         tmp; 
    Tuart                       *uartLocl;
    nvStatePORT_UART_2_type     *nvAdr, nvMem;

    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfPortUart);


    out  = load_s16(ramPnt, progPnt->Pnt_PortNumb);
    switch (out){
        case 0:
            uartLocl = &uart0;                          // Указатель на порт
            break;
        case 1:
            uartLocl = &uart1;                          // Указатель на порт
            break;
        case 2:
            uartLocl = &uart2;                          // Указатель на порт
            break;
        default:
            return &progPnt->Pnt_End;
//              uartLocl = &uart1;                          // Указатель на порт
            //break;
     }

    sPnt = (StatePORT_UART_2_type *)(ramPnt + progPnt->Pnt_State);

    NetAdr      = load_s16(ramPnt, progPnt->Pnt_NetAdr);
    Baudrate    = load_s16(ramPnt, progPnt->Pnt_Baudrate);

    //if(flagIsFirstCall ||((uartLocl->state == STATEsci_RX)&&( (sPnt->NetAdr != NetAdr)||(sPnt->Baudrate != Baudrate)))){ //начальная инициализация
    
    //При старте проверяется КС, если она не совпала в nvstate записывается значение по умолчанию
    
    if(flagIsFirstCall){ //начальная инициализация
        RdFramMicro(&nvMem, nvAdr, sizeof(nvStatePORT_UART_2_type));
        crc = crcPacket((unsigned char *)&nvMem,  sizeof(nvStatePORT_UART_2_type) - sizeof(u16));
        if (crc != nvMem.CRC){
            nvMem.Baudrate = Baudrate;
            nvMem.NetAdr   = NetAdr;
            nvMem.CRC      = crcPacket((unsigned char *)&nvMem,  sizeof(nvStatePORT_UART_2_type) - sizeof(u16));
            WrFramMicro(nvAdr, &nvMem, sizeof(nvStatePORT_UART_2_type));
        }   

        sPnt->NetAdr    = nvMem.NetAdr;
        sPnt->Baudrate  = nvMem.Baudrate;
        save_s16(ramPnt, progPnt->Pnt_NetAdr, sPnt->NetAdr);
        save_s16(ramPnt, progPnt->Pnt_Baudrate, sPnt->Baudrate);
        uart_init(uartLocl, sPnt->NetAdr, sPnt->Baudrate);           // Проинициализировать порт
        UART_rx(uartLocl,uartLocl->buff);               // Поставить порт на прием
        return &progPnt->Pnt_End;
    }
    
    //В процессе работы проверяется изменение настроек, если это произошло, то записывается новое значение в NVSTATE
    
    if (uartLocl != &uart1){
        if(((uartLocl->state == STATEuart_RX)&&((sPnt->NetAdr != NetAdr)||(sPnt->Baudrate != Baudrate)))){
            sPnt->Baudrate = nvMem.Baudrate = Baudrate;
            sPnt->NetAdr = nvMem.NetAdr   = NetAdr;
            nvMem.CRC      = crcPacket((unsigned char *)&nvMem,  sizeof(nvStatePORT_UART_2_type) - sizeof(u16));
            WrFramMicro(nvAdr, &nvMem, sizeof(nvStatePORT_UART_2_type));
            uart_init(uartLocl, sPnt->NetAdr, sPnt->Baudrate);           // Проинициализировать порт
            UART_rx(uartLocl,uartLocl->buff);               // Поставить порт на прием

        }
    }

    out = 0;
    if(load_s16(ramPnt, progPnt->Pnt_TC_on_off) == 1) {
        tmp = load_s16(ramPnt, progPnt->Pnt_Time) * (1000/INT_TIME_CALL_MICRO_mS);  // Время в мс
        if(uartLocl->wdt >= tmp)    
            out = 1;
        else                    
            uartLocl->wdt++;
    }
    defBitMicro(ramPnt, progPnt->Pnt_TimeOver, out);

    if( testBitMicro(ramPnt, progPnt->Pnt_En_prm) )    uartLocl->flags.enablePrmChange = 1;
    else                                                uartLocl->flags.enablePrmChange = 0;

    if( testBitMicro(ramPnt, progPnt->Pnt_En_pusk))    uartLocl->flags.enablePusk = 1;
    else                                                uartLocl->flags.enablePusk = 0;

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPORT_UART_2_1[8]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  NetAdr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Baudrate
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Time
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  TC_on/off
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  PortNumb
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  En_prm
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  En_pusk
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //6  TimeOver
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PORT_UART_2", "DD", TblFuncPORT_UART_2_1, FuncPORT_UART_2_1, 
sizeof(TblFuncPORT_UART_2_1),sizeof(StatePORT_UART_2_type) , sizeof(nvStatePORT_UART_2_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPORT_UART_2_1,
#endif
//********************************************************
