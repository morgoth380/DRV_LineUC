#include "journal.h"
#include "nonvolatile.h"
//#include "GlobalVar.h"
#ifdef    GET_FUNC_CODE
#define TYME_5_MIN  75000
extern u32 UnixTime;
typedef struct{
    s32     CntMin;
    u16     PrevIndex;
}StateFuncJRN_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Index;                        //1  Index
    u16  Pnt_Type;                         //2  Type
    u16  Pnt_Clear;                        //3  Clear
    u16  Pnt_E;                            //4  E
    u16  Pnt_Fout;                         //5  Fout
    u16  Pnt_Ic;                           //6  Ic
    u16  Pnt_Ib;                           //7  Ib
    u16  Pnt_Ia;                           //8  Ia
    u16  Pnt_Iact;                         //9  Iact
    u16  Pnt_Ifull;                        //10  Ifull
    u16  Pnt_Ud;                           //11  Ud
    u16  Pnt_TIGBT;                        //12  TIGBT
    u16  Pnt_HourMin;                      //13  HourMin
    u16  Pnt_MonthDay;                     //14  MonthDay
    u16  Pnt_StateWord;                    //15  StateWord
    u16  Pnt_Uca;                          //16  Uca
    u16  Pnt_Ubc;                          //17  Ubc
    u16  Pnt_Uab;                          //18  Uab
    u16  Pnt_IndFail;                      //19  IndFail
    u16  Pnt_End;
}FuncJRN_V2_type;
//********************************************************
u16 *FuncJRN_V2_1(FuncJRN_V2_type *progPnt, u32 ramPnt)
{
  #ifdef _PROJECT_FOR_STM32_
    u16                     Index, IndexLast, HourMin, MonthDay;
    RTC_type                Time;
    RecJrn_type             RecJrnRd;
    Index_type              *pRecIndex, RecIndexFail;
    StateFuncJRN_type       *sPnt;
    void                    *nvAdr;
    pRecIndex = &RecIndexFail;
    u8 extraDay = 0;
    
    sPnt = (StateFuncJRN_type *)(ramPnt + progPnt->Pnt_State);

    if (flagIsFirstCall){
       sPnt->PrevIndex = 0;                                     // Для гарантированного чтения журнала
       sPnt->CntMin = TYME_5_MIN;
    }
    Index = load_s16(ramPnt, progPnt->Pnt_Index);

    // через 5 минут меняем на индекс на 1 ( крайняя авария
    //**//
    if(sPnt->PrevIndex !=  Index)                              // Завести таймер
        sPnt->CntMin = TYME_5_MIN;
    if(sPnt->CntMin < 0 );
    else if( sPnt->CntMin > 0){ 
        sPnt->CntMin--; 
    }
    else if( sPnt->CntMin == 0 ){
        save_s16(ramPnt, progPnt->Pnt_Index, 1);                // Переписать индекс на 1
        sPnt->CntMin--;
    }
    //**//
    // запись аварийного журнала
    if(GlobalM4.FlagCommand.bit.FlagWriteJrn == 1){
        WriteJrn(&RecJrnRd);                                    // запись аварийных значений
        
        nvAdr = (void *)((u32)&nv.stopTime);                    // запись времени останова для АПВ
        WrFramMicro(nvAdr, &UnixTime, sizeof(u32));             // запись времени останова для АПВ
        
        Index = 1;
        save_s16(ramPnt, progPnt->Pnt_Index, Index);            // Переписать индекс
        sPnt->PrevIndex = 0;
        
        GlobalM4.FlagCommand.bit.FlagWriteJrn = 0;
    }
    // чтение аварийного журнала
    if (sPnt->PrevIndex != Index ){
        sPnt->CntMin    = TYME_5_MIN;
        sPnt->PrevIndex = Index;
            
        GetIndex(&RecIndexFail);                                    // индексы аварий
        IndexLast = pRecIndex->indexRec;
        if (IndexLast >= Index)                                     // Вычисление индекса для чтения (1-й индекс пользователя
            Index = IndexLast - Index + 1;                          // соответствует последнему индексу журнала)
        else
            Index = MAX_JRN_FAIL + IndexLast - Index + 1;
        
        ReadJrn(&RecJrnRd, Index);                                  // чтение аварийного журнала
            
        UnixTime_to_Time(&Time, RecJrnRd.timeUnix, &extraDay);                 // Перевод времени из UnixTime в календарное
        HourMin     = Time.Hours *60 + Time.Minutes;
        if(!extraDay)
            MonthDay    = monthSum[Time.Month] + Time.Day;              // 
        else
            MonthDay    = monthSumEx[Time.Month] + Time.Day;
        
        MonthDay = Time.Day * 128 + Time.Month;
        
        save_s16(ramPnt, progPnt->Pnt_HourMin       ,HourMin        );
        save_s16(ramPnt, progPnt->Pnt_MonthDay      ,MonthDay       );
        save_s16(ramPnt, progPnt->Pnt_Fout          ,RecJrnRd.Fout  );
        save_s16(ramPnt, progPnt->Pnt_Ia            ,RecJrnRd.Iu    );
        save_s16(ramPnt, progPnt->Pnt_Ib            ,RecJrnRd.Iv    );
        save_s16(ramPnt, progPnt->Pnt_Ic            ,RecJrnRd.Iw    );
        save_s16(ramPnt, progPnt->Pnt_Iact          ,RecJrnRd.Iact  );
        save_s16(ramPnt, progPnt->Pnt_Ifull         ,RecJrnRd.Ifull );
        save_s16(ramPnt, progPnt->Pnt_Ud            ,RecJrnRd.Ud    );
        save_s16(ramPnt, progPnt->Pnt_TIGBT         ,RecJrnRd.TIGBT );
        save_s16(ramPnt, progPnt->Pnt_StateWord     ,RecJrnRd.StateWord_CRC & 0x00FF);
        save_s16(ramPnt, progPnt->Pnt_Uab           ,RecJrnRd.Uab   );
        save_s16(ramPnt, progPnt->Pnt_Ubc           ,RecJrnRd.Ubc   );
        save_s16(ramPnt, progPnt->Pnt_Uca           ,RecJrnRd.Uca   );        
    }
#endif  
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncJRN_V2_1[19]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Index
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Type
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Clear
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  E
        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Fout
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Ic
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Ib
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Ia
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Iact
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Ifull
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Ud
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  TIGBT
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  HourMin
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14  MonthDay
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //15  StateWord
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  Uca
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  Ubc
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  Uab
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //19  IndFail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("JRN_V2", "DD", TblFuncJRN_V2_1, FuncJRN_V2_1, sizeof(TblFuncJRN_V2_1), sizeof(StateFuncJRN_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncJRN_V2_1,
#endif
//********************************************************
