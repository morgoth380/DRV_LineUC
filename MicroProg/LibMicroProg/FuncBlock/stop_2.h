#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_StopKey;                      //1  StopKey
    u16  Pnt_StopNet;                      //2  StopNet
    u16  Pnt_StopNetFail;                  //3  StopNetFail
    u16  Pnt_StopNetPB;                    //4  StopNetPB
    u16  Pnt_StopDF;                       //5  StopDF
    u16  Pnt_StopRun;                      //6  StopRun
    u16  Pnt_StopSleep;                    //7  StopSleep
    u16  Pnt_StopStep;                     //8  StopStep
    u16  Pnt_StopModbSlave;                //9  StopModbSlave
    u16  Pnt_Stop1;                        //10  Stop1
    u16  Pnt_Stop2;                        //11  Stop2
    u16  Pnt_Functional;                   //12  Functional
    u16  Pnt_StopAPV;                      //13  StopAPV
    u16  Pnt_StopOut;                      //14  StopOut
    u16  Pnt_End;
}FuncSTOP_2_type;
//********************************************************
u16 *FuncSTOP_2_1(FuncSTOP_2_type *progPnt, u32 ramPnt)
{
    s16 Stop, StopAPV = 0;
    volatile u16 tempStop = 0;
    AT24Functional_type  functional;
    
    functional = (AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_Functional);

    Stop = (testBitMicro(ramPnt, progPnt->Pnt_StopNet)   | testBitMicro(ramPnt, progPnt->Pnt_StopNetFail) |
        testBitMicro(ramPnt, progPnt->Pnt_StopNetPB) | testBitMicro(ramPnt, progPnt->Pnt_StopDF)      |
        testBitMicro(ramPnt, progPnt->Pnt_StopRun)   | testBitMicro(ramPnt, progPnt->Pnt_StopModbSlave) |
        testBitMicro(ramPnt, progPnt->Pnt_StopStep) );
    
    if(functional == UsePumpFunc) {              //Если выбран функционал Насос - отслеживаем еще 2 сигнала
    	Stop |= (testBitMicro(ramPnt, progPnt->Pnt_StopSleep) | testBitMicro(ramPnt, progPnt->Pnt_StopStep));
    	StopAPV |= Stop | testBitMicro(ramPnt, progPnt->Pnt_StopKey); 
        defBitMicro(ramPnt, progPnt->Pnt_StopAPV, StopAPV);
    }
    
    defBitMicro(ramPnt, progPnt->Pnt_StopOut, Stop);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSTOP_2_1[14]={
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //1  StopKey
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //2  StopNet
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //3  StopNetFail
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //4  StopNetPB
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //5  StopDF
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //6  StopRun
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //7  StopSleep
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //8  StopStep
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //9  StopModbSlave
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //10  Stop1
        BIT_PIN_TYPE| INPUT_PIN_MODE,     //11  Stop2
        S16_PIN_TYPE| INPUT_PIN_MODE,     //12  Functional
        BIT_PIN_TYPE| OUTPUT_PIN_MODE,    //13  StopAPV
        BIT_PIN_TYPE| OUTPUT_PIN_MODE     //14  StopOut
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("STOP_2", "DD", TblFuncSTOP_2_1, FuncSTOP_2_1, sizeof(TblFuncSTOP_2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSTOP_2_1,
#endif
//********************************************************
