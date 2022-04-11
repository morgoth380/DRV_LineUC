#ifdef    GET_FUNC_CODE
#include "GlobalVar.h"
/*
// Описание настроек калибровки одного канала
typedef struct{
        float     Val;          // Значение параметра
        float     Kcor;         // Калибровочный коеффициент
        float     Ustir;        // Юстировочное число
}CalibrVal_type;

// static
typedef struct{
        CalibrVal_type     CalibrUd;
        CalibrVal_type     CalibrIu;
        CalibrVal_type     CalibrIv;
        CalibrVal_type     CalibrIw;
}DRVState_type;





// Расчет коеффициента для перевода кодов АЦП в реальное значение
float GainCalc(float Ust, float Kcor){
  return (Ust * Kcor) / 4096.0f;
}

// Функция каллибровки
float CalibrFunk(u32 ramPnt, float Korrect, u16 PntK, float AdjustNum, float *Gain, float NewVal, CalibrVal_type *PrevVal, void *nvAdr){
  float Kcor, KcorNew;
  
  // 1) Получить текущее значение
  Kcor = load_float(ramPnt, PntK);                 // Коефф калибровки
  
  // 2) Проверить изменил ли пользователь значения калиброви 
  if(Kcor != PrevVal->Kcor){
      PrevVal->Kcor = Kcor;                               // Сохраняем новое значение в static  
      *Gain = GainCalc(AdjustNum, Kcor);                  // Расчитаем новое значение калибровочного коеффициента 
      
      WrFramMicro(nvAdr, &PrevVal->Kcor, sizeof(float));  // Сохранить новое значение Kcor в FRAM
  }
  else if(fabsf(Korrect - PrevVal->Val) > 0.01f){         // Если пользователь изменил текущее значение параметра
      PrevVal->Val = Korrect;                             // Сохраняем новое значение в static 
   
      KcorNew = Kcor * (Korrect / NewVal);                // Расчитаем новое значение коеффициента калибровки Kcor 
      PrevVal->Kcor = KcorNew;                            // Сохраняем новое значение в static  
      save_float(ramPnt, PntK, KcorNew);                  // Сохранить новое значение коеффициента калибровки Kcor в уставку
          
      *Gain = GainCalc(AdjustNum, KcorNew);               // Расчитаем новое значение калибровочного коеффициента 
      
      WrFramMicro(nvAdr, &PrevVal->Kcor, sizeof(float));  // Сохранить новое значение Kcor в FRAM
  }
  else if(AdjustNum != PrevVal->Ustir){
      PrevVal->Ustir = AdjustNum;                         // Сохраняем новое значение в static 
      
      *Gain = GainCalc(AdjustNum, Kcor);                  // Расчитаем новое значение калибровочного коеффициента
  }
    
  // 3) Записать обновленное значение параметра в уставку
  PrevVal->Val = NewVal;
  return NewVal;
}
*/

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_F;                            //1  F
    u16  Pnt_U;                            //2  U
    u16  Pnt_Dtime;                        //3  Dtime
    u16  Pnt_Fpwm;                         //4  Fpwm
    u16  Pnt_GOTOV;                        //5  GOTOV    K_2PiC  
    u16  Pnt_PWM_On;                       //6  PWM_On
    u16  Pnt_ChargeOk;                     //7  ChargeOk
    u16  Pnt_Ud_Korrect;                   //8  Ud_Korrect
    u16  Pnt_K_Ud;                         //9  K_Ud
    u16  Pnt_Iu_Korrect;                   //10  Iu_Korrect
    u16  Pnt_K_Iu;                         //11  K_Iu
    u16  Pnt_Iv_Korrect;                   //12  Iv_Korrect
    u16  Pnt_K_Iv;                         //13  K_Iv
    u16  Pnt_Iw_Korrect;                   //14  Iw_Korrect
    u16  Pnt_K_Iw;                         //15  K_Iw
    u16  Pnt_Ustir_2PiC;                   //16  Ustir_2PiC
    u16  Pnt_MTZref;                       //17  MTZref
    u16  Pnt_Tmax;                         //18  Tmax
    u16  Pnt_Udmax;                        //19  Udmax
    u16  Pnt_Udmin;                        //20  Udmin
    u16  Pnt_Ustir_Cur;                    //21  Ustir_Cur
    u16  Pnt_Ustir_Ud;                     //22  Ustir_Ud
    u16  Pnt_ExternFail;                   //23  ExternFail
    u16  Pnt_Stop;                         //24  Stop        HardComplect
    u16  Pnt_TypeDrv;                      //25  TypeDrv
    u16  Pnt_status;                       //26  status //Ifull_fval
    u16  Pnt_Iact_fval;                    //27  Iact_fval
    u16  Pnt_Iw_fval;                      //28  Iw_fval
    u16  Pnt_Iv_fval;                      //29  Iv_fval
    u16  Pnt_Iu_fval;                      //30  Iu_fval
    u16  Pnt_Ud_fval;                      //31  Ud_fval
    u16  Pnt_NumFail_fval;                 //32  NumFail_fval
    u16  Pnt_Preact_out;                   //33  Preact_out
    u16  Pnt_NumFail;                      //34  NumFail
    u16  Pnt_FailMask_1;                   //35  FailMask_1
    u16  Pnt_FailMask_0;                   //36  FailMask_0
    u16  Pnt_Ireact_out;                   //37  Ireact_out
    u16  Pnt_Ifull_out;                    //38  Ifull_out
    u16  Pnt_Iact_out;                     //39  Iact_out
    u16  Pnt_Tw;                           //40  Tw
    u16  Pnt_Tu;                           //41  Tu
    u16  Pnt_Tv;                           //42  Tv
    u16  Pnt_cosFi_out;                    //43  cosFi_out
    u16  Pnt_Pact_out;                     //44  Pact_out
    u16  Pnt_Pfull_out;                    //45  Pfull_out
    u16  Pnt_PWMIsOn;                      //46  PWMIsOn OFF_ZAR
    u16  Pnt_Fout_fval;                    //47  Fout_fval
    u16  Pnt_Uout;	                   //48  Uout
    u16  Pnt_Pusk;	                   //49  Pusk
    u16  Pnt_CurLeakage;	           //50  CurLeakage
    u16  Pnt_End;
}FuncDRV_type;

//********************************************************
//********************************************************
u16 *FuncDRV_1(FuncDRV_type *progPnt, u32 ramPnt)
{/*
    u16   Pusk, Stop;
    float                    temp = 0;
    float                    UstirUd, UstirCur, TekVal, ValSave; 
    s16			     a, b;
    nvAdjCoef_type           *nvAdrCoef;
    DRVState_type            *sPnt;
    FailsSetup_type          *pFailsSetup;
    FailData_type            *pFailData;
    u16                      FlagNaN = 0, FlagLimit = 0; 
    MPOpis_type              *ePnt;
    
    sPnt = (DRVState_type *)(ramPnt + progPnt->Pnt_State);
    nvAdrCoef = (nvAdjCoef_type  *)&nv.adjCoefcnts;
    pFailsSetup = &GlobalM4.FailsSetup;
    pFailData = &GlobalM4.FailData;
*/   
/* 
    // Начальная инициализация до начала любых действий
    if(flagIsFirstCall){				
       
       
       // 1) Прочитать с FRAM калибровочные коеффициенты
       RdFramMicro(&sPnt->CalibrIu.Kcor, &nvAdrCoef->K_Iu, sizeof(float));
       RdFramMicro(&sPnt->CalibrIv.Kcor, &nvAdrCoef->K_Iv, sizeof(float));
       RdFramMicro(&sPnt->CalibrIw.Kcor, &nvAdrCoef->K_Iw, sizeof(float));
       RdFramMicro(&sPnt->CalibrUd.Kcor, &nvAdrCoef->K_Udc, sizeof(float));
       
       
       // 2) Проверим Kcor на валидность 
       //if(isnan(sPnt->CalibrIu.Kcor) || isnan(sPnt->CalibrIv.Kcor) || isnan(sPnt->CalibrIw.Kcor) || isnan(sPnt->CalibrUd.Kcor)){
        // FlagNaN = 1;   // Одно из значений Kcor не валидное
       //}
       
       // 3) Проверить значения Kcor на выход за приделы
       if(((sPnt->CalibrIu.Kcor < 0.5f)||(sPnt->CalibrIu.Kcor > 2.0f))||
          ((sPnt->CalibrIv.Kcor < 0.5f)||(sPnt->CalibrIv.Kcor > 2.0f))||
          ((sPnt->CalibrIw.Kcor < 0.5f)||(sPnt->CalibrIw.Kcor > 2.0f))||
          ((sPnt->CalibrUd.Kcor < 0.5f)||(sPnt->CalibrUd.Kcor > 2.0f))   ){
          FlagLimit = 1;
       }
       
       // 4) Принимаем решение       
       if((FlagNaN)||(FlagLimit)){
            // Загрузить значения по умолчанию
            sPnt->CalibrUd.Kcor = load_float(ramPnt, progPnt->Pnt_K_Ud);        
            sPnt->CalibrIu.Kcor = load_float(ramPnt, progPnt->Pnt_K_Iu);
            sPnt->CalibrIv.Kcor = load_float(ramPnt, progPnt->Pnt_K_Iv);
            sPnt->CalibrIw.Kcor = load_float(ramPnt, progPnt->Pnt_K_Iw);
            
            WrFramMicro(&nvAdrCoef->K_Udc, &sPnt->CalibrUd.Kcor, sizeof(float));	// Прописать начальные значения в FRAM
            WrFramMicro(&nvAdrCoef->K_Iu, &sPnt->CalibrIu.Kcor, sizeof(float));
            WrFramMicro(&nvAdrCoef->K_Iv, &sPnt->CalibrIv.Kcor, sizeof(float));
            WrFramMicro(&nvAdrCoef->K_Iw, &sPnt->CalibrIw.Kcor, sizeof(float));
       }
       else{   // В FRAM уставки валидные
           RdFramMicro(&sPnt->CalibrIu.Kcor, &nvAdrCoef->K_Iu, sizeof(float));
           RdFramMicro(&sPnt->CalibrIv.Kcor, &nvAdrCoef->K_Iv, sizeof(float));
           RdFramMicro(&sPnt->CalibrIw.Kcor, &nvAdrCoef->K_Iw, sizeof(float));
           RdFramMicro(&sPnt->CalibrUd.Kcor, &nvAdrCoef->K_Udc, sizeof(float));
            
           save_float(ramPnt, progPnt->Pnt_K_Iu, sPnt->CalibrIu.Kcor);
           save_float(ramPnt, progPnt->Pnt_K_Iv, sPnt->CalibrIv.Kcor);
           save_float(ramPnt, progPnt->Pnt_K_Iw, sPnt->CalibrIw.Kcor);
           save_float(ramPnt, progPnt->Pnt_K_Ud, sPnt->CalibrUd.Kcor);        // Загрузить значения по умолчанию
       
       }
    }
    
    
    
    
  
    // Задание ПЧ по напряжению и частоте
    GlobalM4.Fref = load_float(ramPnt, progPnt->Pnt_F);
    GlobalM4.Uref = load_float(ramPnt, progPnt->Pnt_U);  
    
    // Частота ШИМ и DeadTime
    GlobalM4.Fpwm = (float)load_s16(ramPnt, progPnt->Pnt_Fpwm) * 0.01f;      // Частота ШИМ + перевод в кГц
    GlobalM4.DeadTime = (float)load_s16(ramPnt, progPnt->Pnt_Dtime) * 0.1f;  // DeadTime + перевод в мкс
    
    // Пуск/Стоп ПЧ
    Pusk = testBitMicro(ramPnt, progPnt->Pnt_Pusk); 
    Stop = testBitMicro(ramPnt, progPnt->Pnt_Stop); 
     
    if(Pusk){
      GlobalM4.FlagCommand.bit.PwmOn = 1;
    }
    else if(Stop){
      GlobalM4.FlagCommand.bit.PwmOn = 0;
    }
    
// Каллибровка
    // 1) Получить юстировочные числа тока и ЗПТ
    UstirUd = (float)load_s16(ramPnt, progPnt->Pnt_Ustir_Ud);
    UstirCur = (float)load_s16(ramPnt, progPnt->Pnt_Ustir_Cur) * 0.1f; 
  
    // 2) Калибровка Ud
    // Получить текущее значение
    temp = load_float(ramPnt, progPnt->Pnt_Ud_Korrect);           // Текущее значение
    TekVal = IQ_to_FLOAT(temp, Volt_base);

    // Калибровка
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Ud, UstirUd, &GlobalM4.ValUd.Gain, GlobalM4.ValUd.Val, &sPnt->CalibrUd, &nvAdrCoef->K_Udc);  // Калибровка

    // Сохранить текущее значение Ud
    temp = FLOAT_to_IQ(ValSave, Volt_base);
    save_float(ramPnt, progPnt->Pnt_Ud_Korrect, temp);

    
    // 3) Калибровка Iu
    // Получить текущее значение
    temp = load_float(ramPnt, progPnt->Pnt_Iu_Korrect);           // Текущее значение
    TekVal = IQ_to_FLOAT(temp, Current_base);

    // Калибровка
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Iu, UstirCur, &GlobalM4.ValIu.Gain, GlobalM4.ValIu.Val, &sPnt->CalibrIu, &nvAdrCoef->K_Iu);  // Калибровка

    // Сохранить текущее значение Ud
    temp = FLOAT_to_IQ(ValSave, Current_base);
    save_float(ramPnt, progPnt->Pnt_Iu_Korrect, temp);

    
    // 4) Калибровка Iv
    // Получить текущее значение
    temp = load_float(ramPnt, progPnt->Pnt_Iv_Korrect);           // Текущее значение
    TekVal = IQ_to_FLOAT(temp, Current_base);

    // Калибровка
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Iv, UstirCur, &GlobalM4.ValIv.Gain, GlobalM4.ValIv.Val, &sPnt->CalibrIv, &nvAdrCoef->K_Iv);  // Калибровка

    // Сохранить текущее значение Ud
    temp = FLOAT_to_IQ(ValSave, Current_base);
    save_float(ramPnt, progPnt->Pnt_Iv_Korrect, temp);

    
    // 5) Калибровка Iw
    // Получить текущее значение
    temp = load_float(ramPnt, progPnt->Pnt_Iw_Korrect);           // Текущее значение
    TekVal = IQ_to_FLOAT(temp, Current_base);

    // Калибровка
    ValSave = CalibrFunk(ramPnt, TekVal, progPnt->Pnt_K_Iw, UstirCur, &GlobalM4.ValIw.Gain, GlobalM4.ValIw.Val, &sPnt->CalibrIw, &nvAdrCoef->K_Iw);  // Калибровка

    // Сохранить текущее значение Ud
    temp = FLOAT_to_IQ(ValSave, Current_base);
    save_float(ramPnt, progPnt->Pnt_Iw_Korrect, temp);

    
    
    //CalibrFunk(ramPnt, progPnt->Pnt_Iu_Korrect, progPnt->Pnt_K_Iu, UstirCur, &GlobalM4.ValIu.Gain, GlobalM4.ValIu.Val, &sPnt->CalibrIu, &nvAdrCoef->K_Iu);
    //CalibrFunk(ramPnt, progPnt->Pnt_Iv_Korrect, progPnt->Pnt_K_Iv, UstirCur, &GlobalM4.ValIv.Gain, GlobalM4.ValIv.Val, &sPnt->CalibrIv, &nvAdrCoef->K_Iv);
    //CalibrFunk(ramPnt, progPnt->Pnt_Iw_Korrect, progPnt->Pnt_K_Iw, UstirCur, &GlobalM4.ValIw.Gain, GlobalM4.ValIw.Val, &sPnt->CalibrIw, &nvAdrCoef->K_Iw);
    
    // Сохранить аварийные уставки
   
    
    pFailsSetup->Imtz = load_float(ramPnt, progPnt->Pnt_MTZref);        // Максимальный ток (МТЗ)
    pFailsSetup->Tmax = load_float(ramPnt, progPnt->Pnt_Tmax);          // Максимальная температура ключей
     
    // Текущая авария и причина останова
    save_s16(ramPnt, progPnt->Pnt_NumFail, pFailData->NumFail);
    save_s16(ramPnt, progPnt->Pnt_NumFail_fval, pFailData->NumFailStop);    
    
    // Отобразить значение параметров на момент аварийного останова
    save_float(ramPnt, progPnt->Pnt_Iu_fval, pFailData->FailValue.Iu);
    save_float(ramPnt, progPnt->Pnt_Iv_fval, pFailData->FailValue.Iv);
    save_float(ramPnt, progPnt->Pnt_Iw_fval, pFailData->FailValue.Iw);
    save_float(ramPnt, progPnt->Pnt_Ud_fval, pFailData->FailValue.Ud);
    save_float(ramPnt, progPnt->Pnt_Fout_fval, pFailData->FailValue.Fref);
    
    */
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDRV_1[50]={
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //1  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  U
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Dtime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Fpwm
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  K_2PiC
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  PWM_On
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  ChargeOk
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Ud_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  K_Ud
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  Iu_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  K_Iu
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  Iv_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  K_Iv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  Iw_Korrect
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  K_Iw
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Ustir_2PiC
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //17  MTZref
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,   //18  Tmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Udmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Udmin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Ustir_Cur
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Ustir_Ud
        S32_PIN_TYPE | INPUT_PIN_MODE,     //23  ExternFail
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //24  Stop HardComplect
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  TypeDrv
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  status //Ifull_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  Iact_fval
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,  //28  Iw_fval
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,  //29  Iv_fval
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,  //30  Iu_fval
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,  //31  Ud_fval
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  NumFail_fval
        
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //33  PWM_ON_DRV (Preact_out)
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //34  NumFail
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //35  FailMask_1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //36  FailMask_0
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //37  Ireact_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //38  Ifull_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //39  Iact_out
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //40  Tw
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //41  Tu
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //42  Tv
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //43  cosFi_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //44  Pact_out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //45  Pfull_out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //46  OFF_ZAR
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,  //47  Fout_fval
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //48  Uout
        BIT_PIN_TYPE | INPUT_PIN_MODE ,    //49  Pusk
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //50  CurLeakage
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DRV", "DA", TblFuncDRV_1, FuncDRV_1, sizeof(TblFuncDRV_1), sizeof(DRVState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDRV_1,
#endif
//********************************************************
