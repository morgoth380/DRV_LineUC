#include "journal.h"
#include "nonvolatile.h"
#ifdef    GET_FUNC_CODE

#define TYME_5_MIN  32700

extern void GetIndex(Index_type *pIndex, u16 type);
extern void ReadJrn(RecJrn_type *pntRecJrn, u16 index, u16 type);

typedef struct{
    s16     CntMin;
    u16     PrevIndex;
}StateFuncJRN_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
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
    u16  Pnt_HourMin;                      //12  HourMin
    u16  Pnt_MonthDay;                     //13  MonthDay
    u16  Pnt_StateWord;                    //14  StateWord
    u16  Pnt_QntWarn;                      //15  QntWarn
    u16  Pnt_IndWarn;                      //16  IndWarn
    u16  Pnt_QntFail;                      //17  QntFail
    u16  Pnt_IndFail;                      //18  IndFail
    u16  Pnt_End;
}FuncJRN_type;
//********************************************************
u16 *FuncJRN_1(FuncJRN_type *progPnt, u32 ramPnt)
{
    u16                     Index, IndexLast, Type, HourMin, MonthDay, Qnt;
//    u16                     MasIndex[4];
//    u16                     *pntInd;
    time_type               Time;
    RecJrn_type             RecJrnRd;
    Index_type              *pRecIndex, RecIndexFail, RecIndexWarn;
    StateFuncJRN_type       *sPnt;
    void                    *nvAdr;

    sPnt = (StateFuncJRN_type *)(ramPnt + progPnt->Pnt_State);

    if (flagIsFirstCall){
       sPnt->PrevIndex = 0;                                     // Для гарантированного чтения журнала
       sPnt->CntMin = TYME_5_MIN;
    }

    Index = load_s16(ramPnt, progPnt->Pnt_Index);
    Type  = load_s16(ramPnt, progPnt->Pnt_Type);

    if( sPnt->PrevIndex !=  Index)                              // Завести таймер
        sPnt->CntMin = TYME_5_MIN;

    if(sPnt->CntMin <0 ){;
    }
    else if( sPnt->CntMin >0)
        { sPnt->CntMin--; }
    else if( sPnt->CntMin ==0 ){
        save_s16(ramPnt, progPnt->Pnt_Index, 1);                // Переписать индекс на 1
        save_s16(ramPnt, progPnt->Pnt_Type,  0);                // Переписать тип на остановы
        sPnt->CntMin--;
    }

    #ifdef _PROJECT_FOR_CCS_
    if (JrnFailWrite_chk()){
        EnaJrnFailWrite = 1;
        SetParamJrnC28( &RecJrnFailWr );        // Сформировать запись для журнала
        GlobalM3.FailValue.NumFail = GlobalC28.FailValue.NumFail;
        JrnFailWrite_ack();
    }
    #endif

    if ( EnaJrnFailWrite ==1 ){
        WriteJrn(&RecJrnFailWr, 0);                            // Если есть запрос на запись, то записать
        nvAdr = (void *)((u32)&nv.stopTime);                   // запись времени останова для АПВ Partola
        WrFramMicro(nvAdr, &UnixTime, sizeof(u32));             // запись времени останова для АПВ Partola
        if(Type ==0){
            Index = 1;
            save_s16(ramPnt, progPnt->Pnt_Index, Index);        // Переписать индекс
            sPnt->PrevIndex = 0;
        }
    }
    else{ // (Type ==1)
        if(Type ==1){
            save_s16(ramPnt, progPnt->Pnt_Index, 1);
            Index = 1;
            sPnt->PrevIndex = 0;
        }
    }

    if ( sPnt->PrevIndex != Index ){
        sPnt->CntMin    = TYME_5_MIN;
        EnaJrnWarnWrite = 0;
        EnaJrnFailWrite = 0;
        sPnt->PrevIndex = Index;

        if (Type == 0)  pRecIndex = &RecIndexFail;
        else            pRecIndex = &RecIndexWarn;

        GetIndex(&RecIndexFail, 0);                              // индексы аварий
        GetIndex(&RecIndexWarn, 1);                              // индексы предупреждений

        IndexLast = pRecIndex->indexRec;
        Qnt = MAX_JRN_FAIL;

        if (IndexLast >= Index)                                 // Вычисление индекса для чтения (1-й индекс пользователя
            Index = IndexLast - Index + 1;                      // соответствует последнему индексу журнала)
        else
            Index = Qnt + IndexLast - Index + 1;

        ReadJrn(&RecJrnRd, Index, Type);

        ch_to_smhdmy(&Time, RecJrnRd.timeUnix);
        HourMin = Time.hour * 60 + Time.minute;
        MonthDay = monthSumEx[Time.month] + Time.date;

        save_s16(ramPnt, progPnt->Pnt_HourMin,  HourMin         );
        save_s16(ramPnt, progPnt->Pnt_MonthDay, MonthDay        );
        save_s16(ramPnt, progPnt->Pnt_Ud,       RecJrnRd.Ud     );
        save_s16(ramPnt, progPnt->Pnt_Ifull,    RecJrnRd.Ifull  );
        save_s16(ramPnt, progPnt->Pnt_Iact,     RecJrnRd.Iact   );
        save_s16(ramPnt, progPnt->Pnt_Ia,       RecJrnRd.Iu     );
        save_s16(ramPnt, progPnt->Pnt_Ib,       RecJrnRd.Iw     );
        save_s16(ramPnt, progPnt->Pnt_Ic,       RecJrnRd.Iv     );
        save_s16(ramPnt, progPnt->Pnt_Fout,     RecJrnRd.Fout   );
        save_s16(ramPnt, progPnt->Pnt_StateWord,RecJrnRd.StateWord_CRC & 0x00FF);

        save_s16(ramPnt, progPnt->Pnt_IndFail,  RecIndexFail.indexRec);
        save_s16(ramPnt, progPnt->Pnt_QntFail,  RecIndexFail.numRec);
        save_s16(ramPnt, progPnt->Pnt_IndWarn,  RecIndexWarn.indexRec);
        save_s16(ramPnt, progPnt->Pnt_QntWarn,  RecIndexWarn.numRec);
    }

    if (testBitMicro(ramPnt, progPnt->Pnt_Clear)){
        clrBitMicro(ramPnt, progPnt->Pnt_Clear);
        #ifndef _PROJECT_FOR_CCS_
            Index = load_s16(ramPnt, progPnt->Pnt_Index);
            if (Index == 0) Index = 1;
            SetParamJrnC28(&RecJrnRd);
            WriteJrn(&RecJrnRd, 0);
            WriteJrn(&RecJrnRd, 1);
        #else
                ClrJrn(0);
                ClrJrn(1);
        #endif
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncJRN_1[18]={
        S16_PIN_TYPE | INPUT_PIN_MODE,   //1  Index
        S16_PIN_TYPE | INPUT_PIN_MODE,   //2  Type
        BIT_PIN_TYPE | INPUT_PIN_MODE,   //3  Clear
        BIT_PIN_TYPE | INPUT_PIN_MODE,   //4  E
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //5  Fout
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //6  Ic
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //7  Ib
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //8  Ia
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //9  Iact
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //10  Ifull
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //11  Ud
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //12  HourMin
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //13  MonthDay
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //14  StateWord
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //15  QntWarn
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //16  IndWarn
        S16_PIN_TYPE | OUTPUT_PIN_MODE,  //17  QntFail
        S16_PIN_TYPE | OUTPUT_PIN_MODE   //18  IndFail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("JRN", "DD", TblFuncJRN_1, FuncJRN_1, sizeof(TblFuncJRN_1), sizeof(StateFuncJRN_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncJRN_1,
#endif
//********************************************************
