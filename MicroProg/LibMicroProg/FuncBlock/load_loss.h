#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Mode;                         //1  Mode
    u16  Pnt_Time;                         //2  Time
    u16  Pnt_Imin;                         //3  Imin
    u16  Pnt_Inom;                         //4  Inom
    u16  Pnt_DRVst;                        //5  DRV state
    u16  Pnt_PWM;                          //6  PWM
    u16  Pnt_Fail_W;                       //7  Fail_W
    u16  Pnt_Fail_V;                       //8  Fail_V
    u16  Pnt_Fail_U;                       //9  Fail_U
    u16  Pnt_Fault;                        //10  Fault
    u16  Pnt_End;
}FuncLOAD_LOSS_type;

typedef struct{
	u16		timer;
}FuncLOAD_LOSSState_type;

// DRV status
#define		DRVRunning 	 (1<<6) // Run mode
#define         MINIMAL_LOAD_LOSS_TIME  0.3F
#define         MINIMAL_LOAD_LOSS_FREQ  10.0F
//********************************************************
u16 *FuncLOAD_LOSS_1(FuncLOAD_LOSS_type *progPnt, u32 ramPnt)
{
  
    f32 	                Ifail, Inom, Iu, Iv, Iw, Imin, Isumabs; 
    u16			        Tfail, DRVstatus;
    f32                         LoadLossTime;
    FuncLOAD_LOSSState_type  *sPnt;

    sPnt 	= (FuncLOAD_LOSSState_type *)(ramPnt + progPnt->Pnt_State);
    LoadLossTime = (f32)(load_s16(ramPnt, progPnt->Pnt_Time)) * 0.1f;
    Inom	= IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Inom), Current_base);
    Ifail	= Inom * load_float(ramPnt, progPnt->Pnt_Imin);
        
    // На низких частотах значение времени контроля берем из уставки.
    // В остальных случаях - пересчитываем от текущей частоты и берем 5 запас
    if (fabsf(GlobalM4.We) > MINIMAL_LOAD_LOSS_FREQ ) {
      LoadLossTime = 5/fabsf(GlobalM4.We);  
      // Если полученное время менее 0,3с, ставми минимум 0,3 сек.
      if (LoadLossTime < MINIMAL_LOAD_LOSS_TIME)        LoadLossTime = MINIMAL_LOAD_LOSS_TIME;
    }
    Tfail	= (u16) (LoadLossTime * K_TIME_CALL_MICRO_TAKT);
    DRVstatus   = load_s16 (ramPnt, progPnt->Pnt_DRVst);

    // Высчитываем мгновенные токи
    Iu = fabsf(GlobalM4.ValIu.FlashVal);
    Iv = fabsf(GlobalM4.ValIv.FlashVal);
    Iw = fabsf(GlobalM4.ValIw.FlashVal);
    // Находим минимальный из них
    Imin = ((Iu <= Iv) && (Iu <= Iw)) ? Iu : ((Iv <= Iw) ? Iv : Iw); 
    // Находим среднее 
    Isumabs = (fabsf(Iu) + fabsf(Iv) + fabsf(Iw)) * 0.333333f;
      
    // Первичная инициализация
    if(flagIsFirstCall){
        sPnt->timer = Tfail;
        clrBitMicro(ramPnt, progPnt->Pnt_Fault);
    }

    // Добавлена проверка на скалярные режимы, при которых на выходе формируется ноль. В этом случае обрыв нагрузки блокируется.
    if( load_s16(ramPnt, progPnt->Pnt_Mode) != 0 && !((TYPEDRV_ANYSCALAR & (1<<GlobalM4.DriveType)) && (GlobalM4.Uref == 0)) ){    // Контроль нагрузки включен!!!
        if(testBitMicro(ramPnt, progPnt->Pnt_PWM) && (DRVstatus & DRVRunning)){ // ШИМ включен и ПЧ уже поехал в нормальной работе
             if(Imin < Ifail){
                 if (Isumabs < Ifail) { // Если среднее значении абсолютных токов также менше уставки
                   // То скорее всего отпали все три фазы
                   if(sPnt->timer > 3){ // Ускоряем счетчик в 3 раза
                     sPnt->timer-= 3;
                   }
                 }
                 if(sPnt->timer){
                     sPnt->timer--;
                 }
                 else{
                     setBitMicro(ramPnt, progPnt->Pnt_Fault);     // Авария!
                 }
             }
             else{
                 sPnt->timer = Tfail;    
             }
             
             // Дополнительная проверка обрыва выходных фаз на низком уровне
             if (GlobalM4.LoadLossFault) {
               setBitMicro(ramPnt, progPnt->Pnt_Fault);
             }             
        }
        else{
            sPnt->timer = Tfail;
        }
    }
    else{          // Контроль нагрузки отключен!!!
        clrBitMicro(ramPnt, progPnt->Pnt_Fault);
        sPnt->timer = Tfail;
    }
    
    if(testBitMicro(ramPnt, progPnt->Pnt_PWM) == 0){
        clrBitMicro(ramPnt, progPnt->Pnt_Fault);
    }
      
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLOAD_LOSS_1[10]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Mode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Time
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Imin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Inom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  DRV state
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  PWM
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Fail_W
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Fail_V
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Fail_U
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //10  Fault
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LOAD_LOSS", "DA", TblFuncLOAD_LOSS_1, FuncLOAD_LOSS_1, sizeof(TblFuncLOAD_LOSS_1), sizeof(FuncLOAD_LOSSState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLOAD_LOSS_1,
#endif
//********************************************************
