#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Nvar1;                        //1  Nvar1
    u16  Pnt_Nvar2;                        //2  Nvar2
    u16  Pnt_Nvar3;                        //3  Nvar3
    u16  Pnt_Nvar4;                        //4  Nvar4
    u16  Pnt_Prescal;                      //5  Prescal
    u16  Pnt_TwrStop;                      //6  TwrStop
    u16  Pnt_Type;                         //7  Type
    u16  Pnt_Format;                       //8  Format
    u16  Pnt_End;
}FuncLOGGER1_type;
//********************************************************

#define _LOG_TYPE_OFF        0
#define _LOG_TYPE_ALLWAYS    1
#define _LOG_TYPE_ON_START   2

#define _LOG_FORMAT_2_IQ_FOC    0
#define _LOG_FORMAT_2_IQ        1
#define _LOG_FORMAT_2_S16       2
#define _LOG_FORMAT_4_S16       3

#define Type_logdef       GlobalM3.Logger.Type

#define Value1_logdef     GlobalM3.Logger.Value1
#define Value2_logdef     GlobalM3.Logger.Value2
#define Value3_logdef     GlobalM3.Logger.Value3
#define Value4_logdef     GlobalM3.Logger.Value4

#define TaterStop_logdef  GlobalM3.Logger.TafterStop
#define Prescal_logdef    GlobalM3.Logger.Prescal

#define Start_logdef      GlobalM3.Logger.StartLog

#define Format_logdef     GlobalM3.Logger.Format

#define PwmEn_logdef      GlobalC28.FlagState.bit.PwmEn

u16 *FuncLOGGER1_1(FuncLOGGER1_type *progPnt, u32 ramPnt)
{
    static u8 debug = 1;
    static u32 Timer = 0;

    Value1_logdef = load_s16(ramPnt, progPnt->Pnt_Nvar1);
    Value2_logdef = load_s16(ramPnt, progPnt->Pnt_Nvar2);
    Value3_logdef = load_s16(ramPnt, progPnt->Pnt_Nvar3);
    Value4_logdef = load_s16(ramPnt, progPnt->Pnt_Nvar4);

    TaterStop_logdef = load_s16(ramPnt, progPnt->Pnt_Type);
    Prescal_logdef   = load_s16(ramPnt, progPnt->Pnt_Prescal);

    Format_logdef    = load_s16(ramPnt, progPnt->Pnt_Format);

    Type_logdef = load_s16(ramPnt, progPnt->Pnt_Type);

    switch (Type_logdef){

    case _LOG_TYPE_OFF:
        Start_logdef = 0;
        break;

    case _LOG_TYPE_ALLWAYS:
        if(debug) 
            Start_logdef = 1;
        else
            Start_logdef = 0;
        break;

    case _LOG_TYPE_ON_START:
        if(PwmEn_logdef) {
            Start_logdef = 1;
            Timer = (u32)load_s16(ramPnt, progPnt->Pnt_TwrStop) * (u32)250;

            if(Timer == 0) { // для работы логгера, при нулевом времени
                Timer = 1;
            }
        }

        if(Timer == 0) {
            Start_logdef = 0;
        }
        else{
            Timer--;
        }
        break;

    default: Start_logdef = 0;
        break;
    }
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLOGGER1_1[8]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Nvar1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Nvar2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Nvar3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Nvar4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Prescal
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  TwrStop 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Type
        S16_PIN_TYPE | INPUT_PIN_MODE      //8  Format
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LOGGER1", "U", TblFuncLOGGER1_1, FuncLOGGER1_1, sizeof(TblFuncLOGGER1_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLOGGER1_1,
#endif
//********************************************************
