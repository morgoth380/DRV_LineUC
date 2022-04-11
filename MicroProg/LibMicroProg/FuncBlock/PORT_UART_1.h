#ifdef    GET_FUNC_CODE
#include "DrvUart.h"

typedef struct{
    s16     NetAdr;
    s16     Baudrate;
}StatePORT_UART_1_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_NetAdr;                       //1  NetAdr
    u16  Pnt_Baudrate;                     //2  Baudrate
    u16  Pnt_Time;                         //3  Time
    u16  Pnt_TC_on_off;                    //4  TC_on/off
    u16  Pnt_PortNumb;                     //5  PortNumb
    u16  Pnt_TimeOver;                     //6  TimeOver
    u16  Pnt_End;
}FuncPORT_UART_1_type;
//********************************************************
u16 *FuncPORT_UART_1_1(FuncPORT_UART_1_type *progPnt, u32 ramPnt)
{
    u16         out;
    u16         NetAdr, Baudrate;
    StatePORT_UART_1_type    *sPnt;
    u32         tmp; 
    Tuart       *uartLocl;

    out  = load_s16(ramPnt, progPnt->Pnt_PortNumb);
    if(out>=NUMuartPorts) return &progPnt->Pnt_End;    // Защита от неправильного задания порта
    uartLocl = uartPtr[out];                           // Указатель на порт

    sPnt = (StatePORT_UART_1_type *)(ramPnt + progPnt->Pnt_State);

    NetAdr      = load_s16(ramPnt, progPnt->Pnt_NetAdr);
    Baudrate    = load_s16(ramPnt, progPnt->Pnt_Baudrate);

    if(flagIsFirstCall ||((uartLocl->state == STATEsci_RX)&&( (sPnt->NetAdr != NetAdr)||(sPnt->Baudrate != Baudrate)))){ //начальная инициализация
        sPnt->NetAdr    = NetAdr;
        sPnt->Baudrate  = Baudrate;
        sci_init(uartLocl, NetAdr, Baudrate);           // Проинициализировать порт
        UART_rx(uartLocl,uartLocl->buff);               // Поставить порт на прием
        return &progPnt->Pnt_End;
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

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPORT_UART_1_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  NetAdr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Baudrate
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Time
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  TC_on/off
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  PortNumb
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //6  TimeOver
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PORT_UART_1", "DD", TblFuncPORT_UART_1_1, FuncPORT_UART_1_1, 
sizeof(TblFuncPORT_UART_1_1),sizeof(FuncPORT_UART_1_type) , 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPORT_UART_1_1,
#endif
//********************************************************
