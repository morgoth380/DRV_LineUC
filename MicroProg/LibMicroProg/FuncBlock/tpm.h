#ifdef    GET_FUNC_CODE

#include <math.h>

typedef struct{
    u32  TimeSec;
    f32  Integral;
    u16  Sec05;
    u16  ActiveFault;
    u32  tmrDelayOff;
}TPM_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Ifull;                       //1  Ifull%
    u16  Pnt_Inom;                        //2  Inom%
    u16  Pnt_IovL;                        //3  IovL%
    u16  Pnt_Tact;                         //4  Tact
    u16  Pnt_Alg;                          //5  Alg
    u16  Pnt_E;                            //6  E
    u16  Pnt_PWM_ON;                       //7  PWM_ON
    u16  Pnt_ImaxApvEn;                    //8  ImaxApvEn
    u16  Pnt_Functional;                    //9  Functional
    u16  Pnt_Alarm;                        //10  Alarm
    u16  Pnt_TtoAcc;                       //11  TtoAcc
    u16  Pnt_ImaxWarn;                     //12  ImaxWarn   
    u16  Pnt_ImaxErr;                      //13  ImaxErr
    u16  Pnt_End;
}FuncTPM_type;

typedef enum {
  MODE_I_T = 0, // Время - ток
  MODE_I2t = 1, // Интеграл тока
} IMAX_Mode_enum;

typedef enum {
    IMAX_OFF = 0,
    IMAX_WARN = 1,
    IMAX_ERR = 2,
} IMAX_Enum;
//******************************************************************************
u16 *FuncTPM_1(FuncTPM_type *progPnt, u32 ramPnt)
{

    TPM_State_type  *sPnt;
    float           Ifull, Inom, IovL;
    u16             APV_En, ActiveWarning, Mode;  
    u32             MaxDelayOff;
    IMAX_Enum       Reaction;    
//    float           dS; TmpF32;// Tmp1F32;
    float           MaxI2t, OverloadTime, deltaI;
    //float static    presentI2t = 0;
    float           ElapsedTime;
    
    sPnt = (TPM_State_type *)(ramPnt + progPnt->Pnt_State);
    
    // Если не лифтовый функционал - разрешаем работать
    if((AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_Functional) != UseLiftFunc) {
    
      APV_En = load_s16 (ramPnt, progPnt->Pnt_ImaxApvEn); // Проверка включения АПВ
      Ifull = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Ifull), Current_base);    // Текущий полный ток двигателя
      Inom  = 0.01f + IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Inom), Current_base);     // Номинальный ток двигателя + добавка на округление чисел Float.
      IovL  = load_float(ramPnt, progPnt->Pnt_IovL);        // Ток перегруза в % 
      OverloadTime  = load_s16(ramPnt, progPnt->Pnt_Tact);
      Mode  = load_s16(ramPnt, progPnt->Pnt_Alg);
      Reaction = (IMAX_Enum)load_s16(ramPnt, progPnt->Pnt_E);     
      ActiveWarning = 0;  
      
      if (flagIsFirstCall /*|| !sPnt->ActiveFault*/) {
        sPnt->ActiveFault = 0;
        sPnt->tmrDelayOff = (u32)(OverloadTime * K_TIME_CALL_MICRO_TAKT);
      }
      
      MaxDelayOff = (u32)(OverloadTime * K_TIME_CALL_MICRO_TAKT);
      if (APV_En) { // Если АПВ для перегруза активировали - включаем аварию перегруза
        save_s16(ramPnt, progPnt->Pnt_E, IMAX_ERR);
      }
      
      //**************************************************************************
      //**************************************************************************
      // Сброс всех флагов
      clrBitMicro(ramPnt, progPnt->Pnt_ImaxWarn );
      clrBitMicro(ramPnt, progPnt->Pnt_ImaxErr);
      clrBitMicro(ramPnt, progPnt->Pnt_Alarm);
      save_s16(ramPnt, progPnt->Pnt_TtoAcc, 0);
      //**************************************************************************    
      if (Mode == MODE_I_T) { // Алгоритм отсчета времени при превышение уставки тока перегруза
        // Проверка собственно аварии
        if (Ifull > (Inom*IovL)) { // Есть превышение?
          ActiveWarning = 1;
          if (sPnt->tmrDelayOff)      sPnt->tmrDelayOff --;
          else                        sPnt->ActiveFault = 1;
        } else {          
          if (sPnt->tmrDelayOff > MaxDelayOff)      sPnt->tmrDelayOff = MaxDelayOff;
          if (sPnt->tmrDelayOff < MaxDelayOff)      sPnt->tmrDelayOff ++;
          else {sPnt->ActiveFault = 0;}
        }
      } else { //(Mode == MODE_I2t) 
        // Алгоритм Токового интеграла
        // Модуль оценки перегруза работает всегда
        // Делаем расчет токового интеграла двигателя
        MaxI2t = (Inom*IovL - Inom)*OverloadTime; // Предельное значение перегруза
        deltaI = Ifull-Inom; // Текущее приращение тока
        if (Ifull/Inom < 1/IovL ) { // Если текущее отношение тока, ниже отбратного значения перегруза
          deltaI = Inom*(1/IovL - 1); // Фиксируем величину отклонения тока со знаком минус, для плавного сброса интеграла тока
        }
        
        sPnt->Integral += deltaI*FLOAT_TIME_CALL_MICRO;
        if (sPnt->Integral < 0) { // Интеграл не должен быть меньше нуля.
          sPnt->Integral = 0;
          //clrBitMicro(ramPnt, progPnt->Pnt_ImaxErr);  // Установка флага
          sPnt->ActiveFault = 0;
        }
        
        ActiveWarning = 0;
        if (Ifull > Inom) { // Если ток выше номинального, формируем предупреждение
          //setBitMicro(ramPnt, progPnt->Pnt_ImaxWarn );
          ActiveWarning = 1;
        }
        
        ElapsedTime = ((MaxI2t - sPnt->Integral)/deltaI); // Показывает реальное время до отключения по перегрузу
        // Если время более 30 000 секунд - ставим 30 000.
        if (ElapsedTime > 30000)    ElapsedTime = 30000;
        
        //save_s16(ramPnt, progPnt->Pnt_TtoAcc, (s16)ElapsedTime); 
        if (sPnt->Integral > MaxI2t){
          //setBitMicro(ramPnt, progPnt->Pnt_ImaxErr);  // Установка флага
          sPnt->ActiveFault = 1;
        }
      }
      //******************************************************************************
      
      switch (Reaction){
      case IMAX_OFF:
        break;
        
      case IMAX_WARN:
        if (ActiveWarning) {
          setBitMicro(ramPnt, progPnt->Pnt_Alarm);  // даем предупреждения
        }
        break;
        
      case IMAX_ERR:
        if (Mode == MODE_I_T) {
          if (ActiveWarning) {
            setBitMicro(ramPnt, progPnt->Pnt_ImaxWarn );
            save_s16(ramPnt, progPnt->Pnt_TtoAcc, (s16)(sPnt->tmrDelayOff*FLOAT_TIME_CALL_MICRO));         
          }        
        } else {
          if (ActiveWarning) {
            setBitMicro(ramPnt, progPnt->Pnt_ImaxWarn );
            save_s16(ramPnt, progPnt->Pnt_TtoAcc, (s16)ElapsedTime);         
          }
        }
        
        if (sPnt->ActiveFault) {
          setBitMicro(ramPnt, progPnt->Pnt_ImaxErr);  // даем аварию
        }
        break;
        
      default:
        break;
        
      } 
    } else { // А если лифт - Сброс всех флагов
      clrBitMicro(ramPnt, progPnt->Pnt_ImaxWarn );
      clrBitMicro(ramPnt, progPnt->Pnt_ImaxErr);
      clrBitMicro(ramPnt, progPnt->Pnt_Alarm);
      save_s16(ramPnt, progPnt->Pnt_TtoAcc, 0);      
    }
     
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTPM_1[13]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ifull%
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Inom%
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  IovL%
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Tact
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Alg        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  E
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //7  PWM_ON
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8   ImaxApvEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9   Functional
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Alarm        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //11  TtoAcc
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //12  ImaxWarn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //13  ImaxErr
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TPM", "DD", TblFuncTPM_1, FuncTPM_1, sizeof(TblFuncTPM_1), sizeof(TPM_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTPM_1,
#endif
//********************************************************
