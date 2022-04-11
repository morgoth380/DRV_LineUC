#ifdef    GET_FUNC_CODE
typedef struct{
    u32     Prescaller;
    u16     Value;
    u16     Type;
    u16     PrevLoad;
}TIMER_1State_type;


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_Data;                         //1  Data
    u16  Pnt_Load;                         //2  Load
    u16  Pnt_Type;                         //3  Type
    u16  Pnt_Time;                         //4  Time
    u16  Pnt_OutPuls;                      //5  OutPuls
    u16  Pnt_OutLevel;                     //6  OutLevel
    u16  Pnt_End;
}FuncTIMER_1_type;

const u32   TblPrescal[5]={
    (                                     0),    // мс  
    (    100UL / INT_TIME_CALL_MICRO_mS - 1),    // 100 мс 
    (   1000UL / INT_TIME_CALL_MICRO_mS - 1),    // сек
    (  60000UL / INT_TIME_CALL_MICRO_mS - 1),    // мин
    (3600000UL / INT_TIME_CALL_MICRO_mS - 1)    // час
};
//********************************************************
u16 *FuncTIMER_1_1(FuncTIMER_1_type *progPnt, u32 ramPnt)
{
    u16                 Load, Out;
    TIMER_1State_type   *sPnt;

    sPnt = (TIMER_1State_type *)(ramPnt + progPnt->Pnt_State);
    Load = testBitMicro(ramPnt, progPnt->Pnt_Load);
    
    Out = 0;
    if(!Load){
        clrBitMicro(ramPnt, progPnt->Pnt_OutLevel);
    }
    if((!sPnt->PrevLoad)&&Load){        //Rise
        sPnt->Type  = load_s16(ramPnt, progPnt->Pnt_Type);
        sPnt->Value = load_s16(ramPnt, progPnt->Pnt_Data);
        if(sPnt->Type == 0){            //мс
            if(sPnt->Value < INT_TIME_CALL_MICRO_mS)    sPnt->Value = 1;
            else        sPnt->Value = (sPnt->Value + (INT_TIME_CALL_MICRO_mS>>1)) / INT_TIME_CALL_MICRO_mS;
        }
        sPnt->Prescaller = TblPrescal[sPnt->Type];
    }
    else{
        if(sPnt->Prescaller > 0)    sPnt->Prescaller--;
        else{
            if(sPnt->Value > 1){
                sPnt->Value--;
                sPnt->Prescaller = TblPrescal[sPnt->Type];
            }
            else if(sPnt->Value == 1){
                setBitMicro(ramPnt, progPnt->Pnt_OutLevel);
                Out = 1;
                sPnt->Value = 0;
            }
        }
    }

    sPnt->PrevLoad = Load;
    
    defBitMicro(ramPnt, progPnt->Pnt_OutPuls, Out);
    if(sPnt->Type != 0) save_s16(ramPnt, progPnt->Pnt_Time, sPnt->Value);
    else                save_s16(ramPnt, progPnt->Pnt_Time, sPnt->Value * INT_TIME_CALL_MICRO_mS);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTIMER_1_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Data
         BIT_PIN_TYPE | INPUT_PIN_MODE,    //2  Load
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Type
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Time
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  OutPuls 
         BIT_PIN_TYPE | OUTPUT_PIN_MODE    //6  OutLevel
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TIMER_1", "DD", TblFuncTIMER_1_1, FuncTIMER_1_1, sizeof(TblFuncTIMER_1_1), sizeof(TIMER_1State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTIMER_1_1,
#endif
//********************************************************
