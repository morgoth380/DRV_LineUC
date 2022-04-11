#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Ready_all;                        //1  Ready
    u16  Pnt_Blink_APV;                        //2  Blink Apv
    u16  Pnt_Power_Start;                      //3  Power_Start
    u16  Pnt_ReadyStatus;                      //4  Ready Status
    u16  Pnt_End;
}FuncREADY_STATUS_type;
//********************************************************
u16 *FuncREADY_STATUS_1(FuncREADY_STATUS_type *progPnt, u32 ramPnt)
{ 
    u16 readyStatus = 0;
    u16 static PowerOnStartCnt = 0, readyStatusPwr = 0;
    
    if(testBitMicro(ramPnt, progPnt->Pnt_Ready_all))
    {   
        if(testBitMicro(ramPnt, progPnt->Pnt_Blink_APV))
            readyStatus = 0;
        else
            readyStatus = 1;
        
        
        if (testBitMicro(ramPnt, progPnt->Pnt_Power_Start)) {
          if (PowerOnStartCnt) {
            PowerOnStartCnt --;
          } else {
            PowerOnStartCnt = K_TIME_CALL_MICRO_TAKT;
            readyStatusPwr = !readyStatusPwr;
          }
          
         // Если ести запуск по подаче питания делаем его главным в миганиее Готовностьюы
         readyStatus =  readyStatusPwr;
        }
        
    }
    defBitMicro(ramPnt, progPnt->Pnt_ReadyStatus, readyStatus);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncREADY_STATUS_1[4]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Arg_1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Arg_2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Arg_3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Arg_4
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("READY_STATUS", "DD", TblFuncREADY_STATUS_1, FuncREADY_STATUS_1, sizeof(TblFuncREADY_STATUS_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncREADY_STATUS_1,
#endif
//********************************************************
