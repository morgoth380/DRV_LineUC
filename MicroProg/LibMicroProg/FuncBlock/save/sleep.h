#ifdef    GET_FUNC_CODE
typedef struct{
    u16  tmrMax;
    u16  tmrMin;
    u16  tmrSec;
    u16  stateOn;
    u16  stateOff;
    u16  blink;
}SLEEP_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Max;                          //2  Max
    u16  Pnt_Min;                          //3  Min
    u16  Pnt_TimeOff;                      //4  TimeOff     // Время до отключения
    u16  Pnt_TimeOn;                       //5  TimeOn      // Время до включения
    u16  Pnt_Min_Max;                      //6  Min/Max
    u16  Pnt_En1;                          //7  En1
    u16  Pnt_En2;                          //8  En2
    u16  Pnt_En3;                          //9  En3
    u16  Pnt__DriveOff;                    //10  ~DriveOff
    u16  Pnt_DriveOff;                     //11  DriveOff
    u16  Pnt_DriveOn;                      //12  DriveOn
    u16  Pnt_End;
}FuncSLEEP_type;
//********************************************************
u16 *FuncSLEEP_1(FuncSLEEP_type *progPnt, u32 ramPnt)
{
    u16                 time;
    _iq                 in, max, min;
    SLEEP_State_type    *sPnt;

    sPnt = (SLEEP_State_type *)(ramPnt + progPnt->Pnt_State);

//====================================================================
    if( ( testBitMicro(ramPnt, progPnt->Pnt_En1) == 0 )||
        ( testBitMicro(ramPnt, progPnt->Pnt_En2) == 0 )||
        ( testBitMicro(ramPnt, progPnt->Pnt_En3) == 0 ))
        {
        clrBitMicro(ramPnt, progPnt->Pnt_DriveOn );
        clrBitMicro(ramPnt, progPnt->Pnt_DriveOff );
        sPnt->tmrMax    = 0;
        sPnt->tmrMin    = 0;
        sPnt->stateOn   = 0;
        sPnt->stateOff  = 0;
        sPnt->blink     = 1;
    }
    else if( (sPnt->tmrSec--) != 0 ){     
        sPnt->blink = !sPnt->blink ;
        return &progPnt->Pnt_End;
        }
    else{                                                       // Вызов 1 раз в секунду
        sPnt->tmrSec = ( 1000/INT_TIME_CALL_MICRO_mS - 1);    

        in  = _IQabs(load_s32(ramPnt, progPnt->Pnt_In ));
        max = _IQabs(load_s32(ramPnt, progPnt->Pnt_Max));
        min = _IQabs(load_s32(ramPnt, progPnt->Pnt_Min));
        
        if(load_s16(ramPnt, progPnt->Pnt_Min_Max) != 0){        // Работа по максимуму
            if( in >= max ){
                time = load_s16(ramPnt, progPnt->Pnt_TimeOff);
                if( sPnt->tmrMax >=  time){                     // Выключить время пришло
                    sPnt->stateOn   = 0;
                    sPnt->stateOff  = 1;
                }
                else                        sPnt->tmrMax++;
                sPnt->tmrMin = 0;
            }
            else if( in <= min ){
                time = load_s16(ramPnt, progPnt->Pnt_TimeOn);
                if( sPnt->tmrMin >= time){                      // Включить время пришло
                    sPnt->stateOn   = 1;
                    sPnt->stateOff  = 0;
                }
                else                        sPnt->tmrMin++;
                sPnt->tmrMax = 0;
            }
            else{
                if( sPnt->tmrMin ) sPnt->tmrMin--;
                if( sPnt->tmrMax ) sPnt->tmrMax--;
                sPnt->stateOff  = 0;
            }
        }
        else{                                                   // Работа по минимуму
            if( in >= max ){
                time = load_s16(ramPnt, progPnt->Pnt_TimeOff);
                if( sPnt->tmrMax >=  time){                     // Включить время пришло
                    sPnt->stateOn   = 1;    
                    sPnt->stateOff  = 0;
                }
                else                        sPnt->tmrMax++;
                sPnt->tmrMin = 0;
            }
            else if( in <= min ){
                time = load_s16(ramPnt, progPnt->Pnt_TimeOn);
                if( sPnt->tmrMin >= time){                      // Включить время пришло
                    sPnt->stateOn   = 0;
                    sPnt->stateOff  = 1;
                }
                else                        sPnt->tmrMin++;
                sPnt->tmrMax = 0;
            }
            else{
                if( sPnt->tmrMin ) sPnt->tmrMin--;
                if( sPnt->tmrMax ) sPnt->tmrMax--;
                sPnt->stateOff  = 0;
            }
        }
    }
    defBitMicro(ramPnt, progPnt->Pnt_DriveOn,   sPnt->stateOn   );
    defBitMicro(ramPnt, progPnt->Pnt_DriveOff,  sPnt->stateOff  );
    defBitMicro(ramPnt, progPnt->Pnt__DriveOff, (!sPnt->stateOff) && (sPnt->blink));

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS

const char TblFuncSLEEP_1[12]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  TimeOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  TimeOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Min/Max
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  En1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  En2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  En3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  ~DriveOff
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11  DriveOff
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //12  DriveOn
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew("SLEEP", "DA", TblFuncSLEEP_1, FuncSLEEP_1, sizeof(TblFuncSLEEP_1), sizeof(SLEEP_State_type), 0, 1, sizeof(FuncSLEEP_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSLEEP_1,
#endif
//********************************************************
