#include "commonmicro.h"
#include "loadvar.h"

#ifdef    GET_FUNC_CODE

typedef enum{
    DisablePU = 0,
    InitPUState = 1,
    searchPUState = 2,
    prepareStartPUState = 3,
    UpVoltPUState = 4,
    WaitInomPUState = 5,
    RunPUState = 6,
}PUStat_type;

typedef struct{
    PUStat_type     State;
    u16	            RevSearch;
    float           Iampref;
    float           Isaferef;
    float           FoutPU;
    float           Uscan;
    float           dUupVolt;
    float           dUsaturation;
    u16 	    timerOff;
}PickUpState_type;


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Reserved1;                    //1  FStartScan		Fmax поиска
    u16  Pnt_Reserved2;                    //2  TSsaturation	Время нас.ротор  0.00 сек
    u16  Pnt_Reserved3;                    //3  TempScanF		Время поиска
    u16  Pnt_Isaturation;                  //4  Ihh_reativ
    u16  Pnt_Reserved5;                    //5  TempRiseU		Задание тока %
    u16  Pnt_Reserved6;                    //6  Uscan
    u16  Pnt_Reserved7;                    //7  UminScan		Inom
    u16  Pnt_Start;                        //8  Start			Пуск
    u16  Pnt_KpAct;                        //9  KpAct			Кп
    u16  Pnt_KiAct;                        //10  KiAct			Ки
    u16  Pnt_KpAct4;                       //11  KpAct4
    u16  Pnt_KiAct4;                       //12  KiAct4
    u16  Pnt_KpRe;                         //13  KpRe
    u16  Pnt_KiRe;                         //14  KiRe
    u16  Pnt_ScanDownUp;                   //15  ScanDownUp		Направление Впер/Назад/Оба
    u16  Pnt_En;                           //16  En				Активизация
    u16  Pnt_F_PU;                         //17  Fpusk			Частота пуска
    u16  Pnt_StatePU;                      //18  StatePU
    u16  Pnt_HoldGT;                       //19  HoldGT			Работает подхват, Block TEMP_LSU_2
    u16  Pnt_Fout;                         //20  Fout			Задание частоты
    u16  Pnt_Uout;                         //21  Uout			Задание напряжения
    u16  Pnt_End;
}FuncPICKUP_V2_type;
//********************************************************
u16 *FuncPICKUP_V2_1(FuncPICKUP_V2_type *progPnt, u32 ramPnt)
{
  PickUpState_type         *sPnt;
  u16                      run;
  s16                      Enable, Isaturation;
  float		           Uscan, WoutC28; 
//  wm_type Wmem, *Wm;
//  Wm = &Wmem;

  sPnt = (PickUpState_type *)(ramPnt + progPnt->Pnt_State);

  if(flagIsFirstCall){
    sPnt->State = InitPUState; //для начальной инициализации при следующем проходе
    clrBitMicro(ramPnt, progPnt->Pnt_HoldGT);
    return &progPnt->Pnt_End;
  }
  
  Enable = load_s16(ramPnt, progPnt->Pnt_En);             //сигнал разрешения режима подхвата
  run = testBitMicro(ramPnt, progPnt->Pnt_Start);   //Пуск/Стоп (сигнал включенного PWM)
  Isaturation = load_s16(ramPnt, progPnt->Pnt_Isaturation);
  GlobalM4.IsaturationPU = Isaturation * 0.01f;
  
  // Additional pickup setting
  GlobalM4.FirstRiseIdTime = load_s16(ramPnt, progPnt->Pnt_KpAct);
  GlobalM4.FirstFixedIdTime = load_s16(ramPnt, progPnt->Pnt_KiAct); 
  GlobalM4.SecondRiseIdTime = load_s16(ramPnt, progPnt->Pnt_KpAct4); 
  GlobalM4.RiseTorqueTime = load_s16(ramPnt, progPnt->Pnt_KiAct4);    
  GlobalM4.CurRegMultiple = 0.1f*load_s16(ramPnt, progPnt->Pnt_KpRe);    
  GlobalM4.FluxExtimMultiple = 0.1f*load_s16(ramPnt, progPnt->Pnt_KiRe);                 
  
    
  if( Enable && !run ) { // If Enable Flying start, check Drive Type
    if ( (TYPEDRV_ACIMFOC & (1<<GlobalM4.DriveType)) || (TYPEDRV_ANYSCALAR & (1<<GlobalM4.DriveType)) )
      sPnt->State = InitPUState; // Induction motor set, Enable Flying start
    else 
      sPnt->State = DisablePU; // If motor not Induction motor - Flying start disable
  } else if ( !Enable ) {
      sPnt->State = DisablePU; // Flying start disable    
  }
      
    switch (sPnt->State) {
        case DisablePU:
            GlobalM4.FlagState.bit.PickUpFOC = 0; // Flying start Disable  
            clrBitMicro(ramPnt, progPnt->Pnt_HoldGT);
            break;
    
        case InitPUState:
            GlobalM4.FlagState.bit.PickUpFOC = 1; // Preset Flying start Enable
            clrBitMicro(ramPnt, progPnt->Pnt_HoldGT);

            if (Enable && run) { // START!
                sPnt->State = searchPUState; // START SEARCH FREQUENCY
            }    
            break;
            
        case searchPUState:
            // Пока идет Подхват - ЗИ заблокирован
            setBitMicro(ramPnt, progPnt->Pnt_HoldGT);
            WoutC28 = FLOAT_to_IQ(GlobalM4.We, Herz_base);  // Берем скорость поля & convert into iq Format
            // Учитываем текущее направление вращения в знаке скорости           
            if (GlobalM4.DirectRotation == 1) {
              WoutC28 = -WoutC28;  // Которое выполняется в DRV_V5
            }
            save_float(ramPnt, progPnt->Pnt_Fout, WoutC28); // Выдаем частоту 
            if (GlobalM4.FlagState.bit.PickUpReadyToGo) { //Frequency found
                sPnt->State++;
            }
            sPnt->Uscan = 0; // 0.5f*FLOAT_to_IQ(GlobalM4.Uout, Volt_base);                  
            save_float(ramPnt, progPnt->Pnt_Uout, sPnt->Uscan); 
            break;
            
        case prepareStartPUState:
          WoutC28 = FLOAT_to_IQ(GlobalM4.We, Herz_base);  // Берем скорость поля & convert into iq Format
          // Учитываем текущее направление вращения в знаке скорости
          if (GlobalM4.DirectRotation == 1) {
            WoutC28 = -WoutC28;  // Которое выполняется в DRV_V5
          }
          save_float(ramPnt, progPnt->Pnt_Fout, WoutC28); // Выдаем частоту          
          save_float(ramPnt, progPnt->Pnt_F_PU, WoutC28); // Set last PICKUP frequency 
          
          if ((TYPEDRV_ACIMFOC & (1<<GlobalM4.DriveType)) ) { // It's a FOC?
              // Wait PhirUp and Start in normal
                if (GlobalM4.FlagState.bit.vPrUp) {             
                  clrBitMicro(ramPnt, progPnt->Pnt_HoldGT); // Разрешаем работу ЗИ 
                  sPnt->State = RunPUState; // Flying start is Done! Routine VFD work.
                }
            } else { // It's a Scalar
                // If we have a scalar control - start Up voltage
                sPnt->State = UpVoltPUState;
                sPnt->dUupVolt = 0.005; // 0.005 Step up voltage
                sPnt->Iampref = 1.414f*GlobalM4.EnginParam.Isn * 0.5f; // Set half of nominal current
                sPnt->Isaferef = GlobalM4.FailsSetup.Imtz * 0.8f; // Set 80% form Imtz
                sPnt->timerOff = 0; //0.05f * K_TIME_CALL_MICRO_TAKT;
            }
            
            sPnt->Uscan = 0.25f*FLOAT_to_IQ(GlobalM4.Uout, Volt_base); // 0; // Scalar Voltage up start from Zero Volt          
            save_float(ramPnt, progPnt->Pnt_Uout, sPnt->Uscan); 
            GlobalM4.FlagState.bit.PickUpFOC = 0; // Стоп поиск 
            break;
            
        case UpVoltPUState:   
          if (sPnt->timerOff)  {
            sPnt->timerOff --;
            break;
          }
          
          Uscan = sPnt->Uscan;
          if(Uscan >= GlobalM4.Utab_uf){
            sPnt->State = WaitInomPUState;
            break;
          }
          if(GlobalM4.Iflash <= sPnt->Iampref){
            Uscan += sPnt->dUupVolt;
          } else {
            if(GlobalM4.Iflash <= sPnt->Isaferef) Uscan += (sPnt->dUupVolt*0.2f);
          }
          if(Uscan > GlobalM4.Utab_uf) Uscan = GlobalM4.Utab_uf;
          sPnt->Uscan = Uscan;
          save_float(ramPnt, progPnt->Pnt_Uout, Uscan); 
          break;
      
        case WaitInomPUState:
          sPnt->State = RunPUState;
          break;
          
        case RunPUState:
          clrBitMicro(ramPnt, progPnt->Pnt_HoldGT); // Разрешаем работу ЗИ               
          break;
    }

    save_s16(ramPnt, progPnt->Pnt_StatePU, sPnt->State);
//    GlobalM4.Debug1 = sPnt->Uscan;
//    GlobalM4.Debug2 = 1.414f*GlobalM4.EnginParam.Isn * 0.5f; // Iampref - Set half of nominal current 
//    GlobalM4.Debug3 = GlobalM4.FailsSetup.Imtz * 0.8f; // Isaferef - Set 80% form Imtz  
//    GlobalM4.Debug4 = FLOAT_to_IQ(GlobalM4.Uout, Volt_base); 
//    GlobalM4.Debug5 = (f32)sPnt->State;
    
//    // TEST
//    
//    Wm->AdrPrm = 96*128+0;
//    Wm->Val2byte = 0;
//
//
//    GetFlagsPrm(Wm);
//    
//    //-----------------
    
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPICKUP_V2_1[21]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  FStartScan
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  TSsaturation
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TempScanF
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Ihh_reactiv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  TempRiseU
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Uscan
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  UminScan
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Start
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  KpAct
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  KiAct
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  KpAct4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  KiAct4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  KpRe
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  KiRe
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  ScanDownUp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  En
	S32_PIN_TYPE | OUTPUT_PIN_MODE,     //17  Fpusk
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  StatePU
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //19  HoldGT
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //20  Fout
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //21  Uout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PICKUP_V2", "DA", TblFuncPICKUP_V2_1, FuncPICKUP_V2_1, sizeof(TblFuncPICKUP_V2_1), sizeof(PickUpState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPICKUP_V2_1,
#endif
//********************************************************
