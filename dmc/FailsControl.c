#include "Global_include.h"
#include "FailsControl.h"
#include "GlobalVar.h"
#include "DrvPWM.h"
#include "DrvADC.h"
#include "DrvErrSWandSTO.h"
#include "TormSW.h"
#include "LIB_DMC.h"
#include "load_loss.h"


void FailValueCopy(FailValue_type   *mPnt);

#define NumFail_CONTROL(Num)    if(NumFail == 0)    NumFail = Num
// Маска для сброса всех МТЗ
#define FastFailMask_MtzMask     (1<<FastFailMask_MtzW_Comp) + (1<<FastFailMask_MtzZPT) + (1<<FastFailMask_MtzU_Prog) + (1<<FastFailMask_MtzV_Prog) + (1<<FastFailMask_MtzW_Prog) + (1<<FastFailMask_SwU) + (1<<FastFailMask_SwV) + (1<<FastFailMask_SwW) + (1<<FastFailMask_SwT)
// Маска управления ошибкой Udmax
#define FastFailMask_UdmaxMask     (1<<FastFailMask_UdMax)
#define UnlockUdmaxTime         2 // Время разблокировки Udmax после безопасного снижения напряжения, 2 секунды
#define UDC_MAX_ADC             4015    // Максимально допустимое значение АЦП для Ud. Выше него принимаем Udmax. Это 98% от полной шкалы.

extern PWMGEN Pwm;           // Объект формирователя ШИМ (для отключения ШИМ)
extern ADCDRV Adc;           // Объект настройки модуля АЦП
extern TORMSW TormSw;        // Объект управления тормозным ключем

// Обработчик аварий в такте ШИМ
void FailsControl(void){
  
    FailsSetup_type          *pFailsSetup;
    FailData_type            *pFailData;
    GlobalVar                *mPnt;
    FastFailMask_type        FastFailMask;
    static FastFailMask_type HoldFailMask;
    s16                      NumFail;
    float                    Imax;
    static u16               UnlockUdmaxCnt = 0;
    static u16               UnlockUinFailCnt = 5000;   
    
    NumFail = 0;
    FastFailMask.all = 0;
    pFailsSetup = &GlobalM4.FailsSetup;
    pFailData = &GlobalM4.FailData;
    mPnt = &GlobalM4;  
    if (UnlockUinFailCnt)       UnlockUinFailCnt --;
    
    // Восстановление МТЗ 
    if(mPnt->FlagState.bit.MTZ_hold == 0) { // если нет защелкнутого МТЗ               
        HoldFailMask.all &= (0xFFFFFFFF ^ (u32)(FastFailMask_MtzMask));                     
    }
    else{                                   // есть защелкнутое МТЗ
        FastFailMask.all = HoldFailMask.all & ((u32)(FastFailMask_MtzMask)); // Восстановить только аварии МТЗ
    }
    
    // Проверить флаг таймаута по МТЗ
    if (mPnt->FlagState.bit.MTZ_timeout) {
        NumFail_CONTROL(FastFailMask_MTZ_Timeout);
        FastFailMask.bit.MTZ_Timeout = 1;
    }

    //------------------------------------------------------------------------
    //                  Проверка аварии по расположению калибровок
    //------------------------------------------------------------------------
    if(GlobalM4.AdjCoefFault){            
       FastFailMask.bit.AdjCoefFault = 1;
       NumFail_CONTROL(FastFailMask_AdjCoefFault);
    }
      
    //------------------------------------------------------------------------
    //                  Аварии звена постоянного напряжения
    //------------------------------------------------------------------------

    // Обработка аварии Udmin, после того как железо запущено и мы не находимся в процессе заряда ЗПТ
    //if( (mPnt->ValUd.FlashVal < pFailsSetup->UdMin) && (mPnt->FlagCommand.bit.PerifInitComplete == 1) && (!GlobalM4.FlagCommand.bit.ChargeInProgress)){
    // Обработка аварии Udmin, после того как железо запущено и ЗПТ заряжено (если не заряжено - не проверяем)
    if( (mPnt->ValUd.FlashVal < pFailsSetup->UdMin) && (mPnt->FlagCommand.bit.PerifInitComplete == 1) && (GlobalM4.FlagCommand.bit.ChargeOk)){            
       FastFailMask.bit.UdMin = 1;
       NumFail_CONTROL(FastFailMask_UdMin);
    }

    // Обработка аварии Udmax
    FastFailMask.bit.UdMax = HoldFailMask.bit.UdMax; // Восстановить состояния аварии Udmax
    if( (mPnt->ValUd.FlashVal >= pFailsSetup->UdMax || mPnt->ValUd.AdcVal >= UDC_MAX_ADC) && (mPnt->FlagCommand.bit.PerifInitComplete == 1) ){
        FastFailMask.bit.UdMax = 1;
        HoldFailMask.bit.UdMax = 1; // Защелкиваем аварию Udmax, до снижения напряжения
        NumFail_CONTROL(FastFailMask_UdMax);
        UnlockUdmaxCnt = (u16)(UnlockUdmaxTime * mPnt->Fpwm); // Заряжаем таймер на задержку отключения после снижения напряжения
        
    } else if ( mPnt->ValUd.FlashVal < (0.98f*pFailsSetup->Udmax_Charge) ) { // Если Ud опустилось на 98% от максимума напряжения допустимого при заряде
            
            if ( UnlockUdmaxCnt )       UnlockUdmaxCnt --;
            else                        HoldFailMask.bit.UdMax = 0; // Отщелкиваем аварию Udmax, до снижения напряжения                        
    }  
    
    //------------------------------------------------------------------------
    //                  Обработка МТЗ 
    //------------------------------------------------------------------------
    // Обработка МТЗ от компаратора
    if(pFailData->FailsFlag.bit.MtzComp){
       FastFailMask.bit.MtzW_Comp = 1;
       NumFail_CONTROL(FastFailMask_MtzW_Comp);
       if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если МТЗ в работе, защелкиваем ее
          mPnt->FlagState.bit.MTZ_hold = 1;
          HoldFailMask.bit.MtzW_Comp = 1;
       }      
            
       pFailData->FailsFlag.bit.MtzComp = 0;
    } 
        
    Imax = pFailsSetup->Imtz * 0.9f;
    // Обработка аварии МТЗ программа фаза U
    if(fabsf(mPnt->ValIu.FlashVal) > Imax){      
        if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если МТЗ в работе, защелкиваем ее
          mPnt->FlagState.bit.MTZ_hold = 1;
          HoldFailMask.bit.MtzU_Prog = 1;
        }      
        FastFailMask.bit.MtzU_Prog = 1;
        NumFail_CONTROL(FastFailMask_MtzU_Prog);
    }
    // Обработка аварии МТЗ программа фаза V
    if(fabsf(mPnt->ValIv.FlashVal) > Imax){
        if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если МТЗ в работе, защелкиваем ее
          mPnt->FlagState.bit.MTZ_hold = 1;
          HoldFailMask.bit.MtzV_Prog = 1;
        }            
        FastFailMask.bit.MtzV_Prog = 1;
        NumFail_CONTROL(FastFailMask_MtzV_Prog);
    }
    // Обработка аварии МТЗ программа фаза W
    if(fabsf(mPnt->ValIw.FlashVal) > Imax){
        if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если МТЗ в работе, защелкиваем ее
          mPnt->FlagState.bit.MTZ_hold = 1;
          HoldFailMask.bit.MtzW_Prog = 1;
        }            
        FastFailMask.bit.MtzW_Prog = 1;
        NumFail_CONTROL(FastFailMask_MtzW_Prog);
    }
    
    // Обработка МТЗ в цепи ЗПТ во время накачки
    if(pFailData->FailsFlag.bit.MtzZPT){
       if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если МТЗ в работе, защелкиваем ее
          mPnt->FlagState.bit.MTZ_hold = 1;
          HoldFailMask.bit.MtzZPT = 1;
       }      
      
       FastFailMask.bit.MtzZPT = 1;
       NumFail_CONTROL(FastFailMask_MtzZPT);
      
       pFailData->FailsFlag.bit.MtzZPT = 0;                      // Сбросить флаг
    }
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    extern SWANDSTO SwAndSto;    // Обект контроля аварий ключей и STO        
    //------------------------------------------------------------------------
    //               Обработка внешнего сигнала отключения ШИМ
    //------------------------------------------------------------------------
    if(pFailData->FailsFlag.bit.ExtStop || (SwAndSto.getStoStatus(&SwAndSto) && (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb))){
       FastFailMask.bit.ExtStop = 1;
       NumFail_CONTROL(FastFailMask_ExtStop);
      
       pFailData->FailsFlag.bit.ExtStop = 0;                      // Сбросить флаг
    }
        
    //------------------------------------------------------------------------
    //                  Аварии по входной сети
    //------------------------------------------------------------------------
    if( (GlobalM4.FlagCommand.bit.PerifInitComplete == 1) && (pFailsSetup->ControlUin == 1) && !UnlockUinFailCnt) {
       // Обработка аварии Uab, Ubc, Uca 
       if( (mPnt->ValUab.Val < pFailsSetup->UinMin) || 
           (mPnt->ValUbc.Val < pFailsSetup->UinMin) || 
           (mPnt->ValUca.Val < pFailsSetup->UinMin) ){             
                FastFailMask.bit.UinMin = 1;
                NumFail_CONTROL(FastFailMask_UinMin);
       }
       else if( (mPnt->ValUab.Val > pFailsSetup->UinMax) ||
                (mPnt->ValUbc.Val > pFailsSetup->UinMax) || 
                (mPnt->ValUca.Val > pFailsSetup->UinMax) ){
            FastFailMask.bit.UinMax = 1;
            NumFail_CONTROL(FastFailMask_UinMax);
       }
    }     
    
    //------------------------------------------------------------------------
    //               Обработка аварии ключей
    //------------------------------------------------------------------------
    // Тормозной ключ
    if(SwAndSto.getSwTStatus(&SwAndSto) == SwErr){
       FastFailMask.bit.SwT = 1;
       NumFail_CONTROL(FastFailMask_SwT);       
       if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если силовой ключ в работе, защелкиваем его
         mPnt->FlagState.bit.MTZ_hold = 1;
         HoldFailMask.bit.SwT = 1;
       }
       pFailData->FailsFlag.bit.SwT = 0;                      // Сбросить флаг
       
       SwAndSto.AskErr(&SwAndSto);                            // Квитирование аварии ключей
    }
    
    // Ключ по фазе U
    if(SwAndSto.getSwUStatus(&SwAndSto) == SwErr){              
       FastFailMask.bit.SwU = 1;
       NumFail_CONTROL(FastFailMask_SwU);
       if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если силовой ключ в работе, защелкиваем его
         mPnt->FlagState.bit.MTZ_hold = 1;
         HoldFailMask.bit.SwU = 1;
       }       
       pFailData->FailsFlag.bit.SwU = 0;                      // Сбросить флаг
       
       SwAndSto.AskErr(&SwAndSto);                            // Квитирование аварии ключей
    }
    
    // Ключ по фазе V
    if(SwAndSto.getSwVStatus(&SwAndSto) == SwErr){
       FastFailMask.bit.SwV = 1;
       NumFail_CONTROL(FastFailMask_SwV);
       if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если силовой ключ в работе, защелкиваем его
         mPnt->FlagState.bit.MTZ_hold = 1;
         HoldFailMask.bit.SwV = 1;
       }       
       pFailData->FailsFlag.bit.SwV = 0;                      // Сбросить флаг
       
       SwAndSto.AskErr(&SwAndSto);                            // Квитирование аварии ключей
    }
    
    // Ключ по фазе W
    if(SwAndSto.getSwWStatus(&SwAndSto) == SwErr){
       FastFailMask.bit.SwW = 1;
       NumFail_CONTROL(FastFailMask_SwW);
       if (GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb) { // Если силовой ключ в работе, защелкиваем его
         mPnt->FlagState.bit.MTZ_hold = 1;
         HoldFailMask.bit.SwW = 1;
       }       
       pFailData->FailsFlag.bit.SwW = 0;                      // Сбросить флаг
       
       SwAndSto.AskErr(&SwAndSto);                            // Квитирование аварии ключей
    }   
    
    #endif    // defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    //------------------------------------------------------------------------
    //                  Обработка аварии таймаута накачки
    //------------------------------------------------------------------------
    if(pFailData->FailsFlag.bit.TimOutPump){                         // Флаг устанавливается в аппарате состояний модуля ШИМ
       FastFailMask.bit.TimOutPump = 1;
       NumFail_CONTROL(FastFailMask_TimOutPump);
      
       pFailData->FailsFlag.bit.TimOutPump = 0;                      // Сбросить флаг
    }
    
    //------------------------------------------------------------------------
    //                  Обработка Температуры ключей
    //------------------------------------------------------------------------
    if (mPnt->TIGBT > pFailsSetup->Tmax){
        FastFailMask.bit.Tsw = 1;
        NumFail_CONTROL(FastFailMask_Tsw);
    }
    //------------------------------------------------------------------------
    //                  ошибка измерения параметров двигателя
    //------------------------------------------------------------------------
    if(pFailData->FailsFlag.bit.ErrMeasPrm) {
        FastFailMask.bit.ErrMeasPrm = 1;
        NumFail_CONTROL(FastFailMask_ErrMeasPrm);
        
        pFailData->FailsFlag.bit.ErrMeasPrm = 0;
    }
    
    if (pFailData->FailsFlag.bit.MeasPrmBrkPhase) {
        FastFailMask.bit.MeasPrmBrkPhase = 1;
        NumFail_CONTROL(FastFailMask_MeasPrmBrkPhase);
        
        pFailData->FailsFlag.bit.MeasPrmBrkPhase = 0; 
    }
    //------------------------------------------------------------------------
    //                  Обработка аварий прозвонки нагрузки
    //------------------------------------------------------------------------
    if (DrvConst.RingUpError) {
        FastFailMask.bit.RingUpErr = 1;
        NumFail_CONTROL(FastFailMask_ruU);                
        DrvConst.RingUpError = 0; // TEST АВТОСБРОС АВАРИИ ПРОЗВОНКИ !!!
    }
    
    //------------------------------------------------------------------------
    //           Обработка аварии обрыва тормозного резистора
    //------------------------------------------------------------------------    
   	if (TormSw.TestResResult == Breakdown) {
            FastFailMask.bit.tormCheck = 1;
            NumFail_CONTROL(FastFailMask_chkTorm);
   	}
        
    //------------------------------------------------------------------------
    //                  Обработка Действий при аварии
    //------------------------------------------------------------------------

    if((NumFail == 0) && (mPnt->NumSlowFail > 31)){     //если нет быстрых аварий, то принимаем только медленную аварию
        NumFail = mPnt->NumSlowFail;                    // Аварии из микропрограммы если нет быстрых аварий
    }

    // Сохраняем текущую аварию в глобальную структуру журнала
    pFailData->NumFail = NumFail;                    
    
    // Если модуль ШИМ в работе или в режиме накачки и присутствует текущая авария то...
    if((NumFail != 0)&&((GlobalM4.FlagCommand.bit.PwmOn) || (GlobalM4.FlagCommand.bit.mWorkEnb))){ 
       Pwm.Off(&Pwm);                                          // Отключить ШИМ!!!    /*Также отключение ШИМ реализовано еще и в обработчике от компаратора*/
       if(mPnt->FlagState.bit.FailLockFlag == 0){        
           pFailData->NumFailStop  = NumFail;                  // Сохраняем причину останова. Пререписать код текущей аварии
           FailValueCopy(&pFailData->FailValue);               // Сохраним значения основных параметров на момент останова
           mPnt->FlagCommand.bit.FlagWriteJrn = 1;             // записать аварийный журнал
           mPnt->FlagState.bit.FailLockFlag = 1;               // блокируем фикзацию аварии, пока в МП не отработает Командоапарат( снимится флаг "ШИМ вкл")
       }
    }
    else{
        mPnt->FlagState.bit.FailLockFlag = 0;
    }
    
    // Контрольное отключение ШИМа когда есть авария
    if (NumFail != 0) {
      Pwm.Off(&Pwm);                                          // Отключить ШИМ!!!    /*Также отключение ШИМ реализовано еще и в обработчике от компаратора*/
    }

    if(FastFailMask.all != 0){
      pFailData->FastFailMask0 = FastFailMask.all;       // Копия для микропрограммы
    }
}

//--------------------------------------------------------------------------
// Копирует значения при аварийном останове
//--------------------------------------------------------------------------
void FailValueCopy(FailValue_type   *mPnt){
  
    mPnt->Iu            = GlobalM4.ValIu.FlashVal;
    mPnt->Iv            = GlobalM4.ValIv.FlashVal;
    mPnt->Iw            = GlobalM4.ValIw.FlashVal;
    mPnt->Ifull         = GlobalM4.Ifull; 
    mPnt->Iact          = GlobalM4.Iact; 
    mPnt->Ud            = GlobalM4.ValUd.FlashVal;
    mPnt->Fref          = GlobalM4.Fref;
    mPnt->TIGBT         = GlobalM4.TIGBT;
    mPnt->NumFail       = GlobalM4.FailData.NumFail;    
    mPnt->Uab           = GlobalM4.ValUab.FlashVal;
    mPnt->Ubc           = GlobalM4.ValUbc.FlashVal;
    mPnt->Uca           = GlobalM4.ValUca.FlashVal;    
}

// Обработчик прерывания по МТЗ
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
    
    // Сюда попадаем по компаратору АЦП (МТЗ)
    // 1) Отключить ШИМ!!!
    htim1.Instance->CCER &= ~(0xfff);   // Dct
          
    // 2) Установить флаг наличие аварии МТЗ от компаратора
    GlobalM4.FailData.FailsFlag.bit.MtzComp = 1;     // Установить флаг о том что авария была
      
    #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
//    // ТЕСТОВЫЙ РЕЖИМ ВЫДАЧИ СИГНАЛА    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    #endif

    // 3) Запретить работу компаратора
    Adc.CompStop(&Adc);
}

// Обработчик прерывания по внешнему сигналу аварийного останова
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_6)
  {
    // Сюда попадаем по внешнему сигналу отключения ШИМ
    // 1) Отключить ШИМ!!!
    htim1.Instance->CCER &= ~(0xfff);   // 
    
    //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET); 
      
    // 2) Установить флаг наличие аварии внешнего останова
    GlobalM4.FailData.FailsFlag.bit.ExtStop = 1;     // Установить флаг о том что авария была
  }
  
}
