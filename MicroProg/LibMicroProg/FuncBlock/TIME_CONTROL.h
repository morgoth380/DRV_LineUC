#ifdef    GET_FUNC_CODE
#include "GlobalVar.h"

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Max1;                         //1  Max1
    u16  Pnt_Min1;                         //2  Min1
    u16  Pnt_Val1;                         //3  Val1
    u16  Pnt_Max2;                         //4  Max2
    u16  Pnt_Min2;                         //5  Min2
    u16  Pnt_Val2;                         //6  Val2
    u16  Pnt_Max3;                         //7  Max3
    u16  Pnt_Min3;                         //8  Min3
    u16  Pnt_Val3;                         //9  Val3
    u16  Pnt_Max4;                         //10  Max4
    u16  Pnt_Min4;                         //11  Min4
    u16  Pnt_Val4;                         //12  Val4
    u16  Pnt_Max5;                         //13  Max5
    u16  Pnt_Min5;                         //14  Min5
    u16  Pnt_Val5;                         //15  Val5
    u16  Pnt_Max6;                         //16  Max6
    u16  Pnt_Min6;                         //17  Min6
    u16  Pnt_Val6;                         //18  Val6
    u16  Pnt_Max7;                         //19  Max7
    u16  Pnt_Min7;                         //20  Min7
    u16  Pnt_Val7;                         //21  Val7
    u16  Pnt_Max8;                         //22  Max8
    u16  Pnt_Min8;                         //23  Min8
    u16  Pnt_Val8;                         //24  Val8
    u16  Pnt_Max9;                         //25  Max9
    u16  Pnt_Min9;                         //26  Min9
    u16  Pnt_Val9;                         //27  Val9
    u16  Pnt_Max10;                        //28  Max10
    u16  Pnt_Min10;                        //29  Min10
    u16  Pnt_Val10;                        //30  Val10
    u16  Pnt_Max11;                        //31  Max11
    u16  Pnt_Min11;                        //32  Min11
    u16  Pnt_Val11;                        //33  Val11
    u16  Pnt_Max12;                        //34  Max12
    u16  Pnt_Min12;                        //35  Min12
    u16  Pnt_Val12;                        //36  Val12
    u16  Pnt_End;
}FuncTIME_CONTROL_type;
//********************************************************
u16 *FuncTIME_CONTROL_1(FuncTIME_CONTROL_type *progPnt, u32 ramPnt)
{
    
  save_s32(ramPnt, progPnt->Pnt_Max1, GlobalM4.MeasTime[TskMicroProgTC].ValMax);    
  save_s32(ramPnt, progPnt->Pnt_Min1, GlobalM4.MeasTime[TskMicroProgTC].ValMin);
  save_s32(ramPnt, progPnt->Pnt_Val1, GlobalM4.MeasTime[TskMicroProgTC].ValTekTime);
  
  save_s32(ramPnt, progPnt->Pnt_Max2, GlobalM4.MeasTime[PwmPeriodProcessingTC].ValMax);   
  save_s32(ramPnt, progPnt->Pnt_Min2, GlobalM4.MeasTime[PwmPeriodProcessingTC].ValMin);
  save_s32(ramPnt, progPnt->Pnt_Val2, GlobalM4.MeasTime[PwmPeriodProcessingTC].ValTekTime);
  
  save_s32(ramPnt, progPnt->Pnt_Max3, GlobalM4.MeasTime[PwmPeriodTC].ValMax);   
  save_s32(ramPnt, progPnt->Pnt_Min3, GlobalM4.MeasTime[PwmPeriodTC].ValMin);
  save_s32(ramPnt, progPnt->Pnt_Val3, GlobalM4.MeasTime[PwmPeriodTC].ValTekTime);
  
  save_s32(ramPnt, progPnt->Pnt_Max4, GlobalM4.MeasTime[PeriodADC].ValMax);   
  save_s32(ramPnt, progPnt->Pnt_Min4, GlobalM4.MeasTime[PeriodADC].ValMin);
  save_s32(ramPnt, progPnt->Pnt_Val4, GlobalM4.MeasTime[PeriodADC].ValTekTime);

  // Фиксируем период текущего такта ШИМа, для дальнейшего блокирования установки запредельного значения частоты ШИМа (см. DRV_V5)  
  GlobalM4.PwmCalcTime_us = GlobalM4.MeasTime[PwmPeriodProcessingTC].ValTekTime * 0.1f; // Значение в мкс
  
  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTIME_CONTROL_1[36]={
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //1  Max1
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //2  Min1
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //3  Val1
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //4  Max2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //5  Min2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //6  Val2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //7  Max3
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //8  Min3
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //9  Val3
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //10  Max4
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //11  Min4
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //12  Val4
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //13  Max5
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //14  Min5
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //15  Val5
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //16  Max6
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //17  Min6
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //18  Val6
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //19  Max7
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //20  Min7
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //21  Val7
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //22  Max8
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //23  Min8
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //24  Val8
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //25  Max9
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //26  Min9
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //27  Val9
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //28  Max10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //29  Min10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //30  Val10
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //31  Max11
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //32  Min11
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //33  Val11
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //34  Max12
        S32_PIN_TYPE | OUTPUT_PIN_MODE,     //35  Min12
        S32_PIN_TYPE | OUTPUT_PIN_MODE      //36  Val12
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TIME_CONTROL", "DD", TblFuncTIME_CONTROL_1, FuncTIME_CONTROL_1, sizeof(TblFuncTIME_CONTROL_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTIME_CONTROL_1,
#endif
//********************************************************
