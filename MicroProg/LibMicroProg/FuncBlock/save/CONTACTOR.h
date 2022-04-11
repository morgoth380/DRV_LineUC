//#include "journal.h"
#ifdef    GET_FUNC_CODE

typedef struct{
    u16     cntDelay;
    u16     cntDrebezg;
    u16     modeOn :1;
    u16     failEn :1;
}StateFuncContactor_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PwmIn;                        //1  PwmIn
    u16  Pnt_Control;                      //2  Control
    u16  Pnt_DelayMotorOn;                 //3  DelayMotorOn
    u16  Pnt_CntDrebezg;                   //4  CntDrebezg
    u16  Pnt_LogicContr;                   //5  LogicContr
    u16  Pnt_LogicOn;                      //6  LogicOn
    u16  Pnt_Mode;                         //7  Mode
    u16  Pnt_Stop;                         //8  Stop
    u16  Pnt_ErrControl;                   //9  ErrControl
    u16  Pnt_ContactorOn;                  //10  ContactorOn
    u16  Pnt_PwmOut;                       //11 PwmOut
    u16  Pnt_End;
}FuncCONTACTOR_type;
//********************************************************
//delayMotorOn - для режима по времени это задержка, 
//для режима теста доп. контакта это количество раз при котором не было дребезга
u16 *FuncCONTACTOR_1(FuncCONTACTOR_type *progPnt, u32 ramPnt)
{
    u16     pwm;
    u16     ctrlContactor;
    u16     delayMotorOn;
    u16     cntDrebezg;
    u16     logicContr;
    u16     logicOn;
    u16     mode;
    u16     setPwm;
    u16     err;
    measMem_type            *mPnt   = &measMem;
    
    StateFuncContactor_type *sPnt;
    
    sPnt = (StateFuncContactor_type *)(ramPnt + progPnt->Pnt_State);
    mode = load_s16(ramPnt, progPnt->Pnt_Mode);

    if (flagIsFirstCall){
        if (mode != 0){
            logicOn = load_s16(ramPnt, progPnt->Pnt_LogicOn);
            defBitMicro(ramPnt, progPnt->Pnt_ContactorOn, !logicOn);
        }
        clrBitMicro(ramPnt, progPnt->Pnt_PwmOut);
        clrBitMicro(ramPnt, progPnt->Pnt_ErrControl);
        return &progPnt->Pnt_End;
    }
    
    
    pwm = testBitMicro(ramPnt, progPnt->Pnt_PwmIn);
//==========================================================    

    
    if (pwm && sPnt->modeOn == 0){
//        if (sPnt->cntDelay == 0){
            sPnt->modeOn = 1;
            sPnt->failEn = 1;
//        }
        sPnt->cntDelay = 0;
        sPnt->cntDrebezg = 0;
    }
    else if (pwm == 0 && sPnt->modeOn){
        //if (sPnt->modeOn){                                  //Установка задержи на отключение, если кнопку стоп нажали раньше
        //    delayMotorOn = load_s16(ramPnt, progPnt->Pnt_DelayMotorOn);
        //    sPnt->cntDelay = delayMotorOn / INT_TIME_CALL_MICRO_mS + 1;
        //}
        sPnt->modeOn = 0;
        sPnt->failEn = 1;
        sPnt->cntDelay = sPnt->cntDrebezg = 0;//load_s16(ramPnt, progPnt->Pnt_DelayMotorOn);
    }
//==========================================================    
    setPwm = 0;
    err = 0;
    if (mode == 0){                                         //Отключен режим контроля и управления контактором
        setPwm = pwm;
    }
    else{                                         //Режим управления контактором
        logicOn = load_s16(ramPnt, progPnt->Pnt_LogicOn);
        ctrlContactor   = testBitMicro(ramPnt, progPnt->Pnt_Control);
        logicContr      = load_s16(ramPnt, progPnt->Pnt_LogicContr);// & sPnt->modeOn;
        
//==========================================================
//Включение контакотора
//==========================================================
        delayMotorOn = load_s16(ramPnt, progPnt->Pnt_DelayMotorOn);
        cntDrebezg = load_s16(ramPnt, progPnt->Pnt_CntDrebezg);
        if (sPnt->modeOn){
            defBitMicro(ramPnt, progPnt->Pnt_ContactorOn, logicOn);                 //Включаем контактор
            if (mode == 2 || mode == 1){
                if (sPnt->cntDelay * INT_TIME_CALL_MICRO_mS < delayMotorOn){        //Ожидаем окончание переходного процесса
                    sPnt->cntDelay++;
                }
                else{
                    if (mode == 1){
                        setPwm = 1;
                    }
                    else{
                        if (logicContr ^ ctrlContactor){                                //Проверка типа логики с состоянием контактора
                            err = 1;
                            setPwm = 0;
                        }
                        else{
                            err = 0;
                            setPwm = 1;
                        }
                    }
                }
            }
            else if (mode == 3){
                if (sPnt->cntDelay * INT_TIME_CALL_MICRO_mS < delayMotorOn){
                    sPnt->cntDelay++;
                }
                else{
                    if (logicContr ^ ctrlContactor){
                        err = 1;
                    }
                }

                if (logicContr ^ ctrlContactor){                       //не включился
                    if (sPnt->cntDrebezg * INT_TIME_CALL_MICRO_mS < cntDrebezg){
                        if (sPnt->cntDrebezg > 0){
                            sPnt->cntDrebezg--;
                        }
                    }
                    else{
                        err = 1;
                    }
                }
                else{
                    sPnt->cntDelay = 0;                             //сбрасываем контрольный счетчик если контактор начал дребезжать
                    if (sPnt->cntDrebezg * INT_TIME_CALL_MICRO_mS < cntDrebezg){
                        sPnt->cntDrebezg++;
                    }
                    else{
                        setPwm = 1;
                    }
                }
            }
        }
//==========================================================
//Выключение контактора
//==========================================================
        else{
            defBitMicro(ramPnt, progPnt->Pnt_ContactorOn, !logicOn);    //Выключаем контактор
            if (mode == 2){
                if (sPnt->cntDelay * INT_TIME_CALL_MICRO_mS < delayMotorOn){
                    sPnt->cntDelay++;
                }
                else{
                    if (!logicContr ^ ctrlContactor){                //Проверка типа логики с состоянием контактора
                        err = 1;
                    }
                }
            }
            else if (mode == 3){
                if (sPnt->cntDelay * INT_TIME_CALL_MICRO_mS < delayMotorOn){
                    sPnt->cntDelay++;
                }
                else{
                    if (!logicContr ^ ctrlContactor){
                        err = 1;
                    }
                }

                if (!logicContr ^ ctrlContactor){                     //не выключился
                    if (sPnt->cntDrebezg * INT_TIME_CALL_MICRO_mS < cntDrebezg){
                        if (sPnt->cntDrebezg > 0){
                            sPnt->cntDrebezg--;
                        }
                    }
                    else{
                        err = 1;
                    }
                }
                else{
                    sPnt->cntDelay = 0;
                    if (sPnt->cntDrebezg * INT_TIME_CALL_MICRO_mS < cntDrebezg){
                        sPnt->cntDrebezg++;
                    }
                }
            }
        }
    }

    if (setPwm){                                            //Устанавливается состояние сигнала ШИМ
        setBitMicro(ramPnt, progPnt->Pnt_PwmOut);
    }
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_PwmOut);
    }
    //if (!err && !sPnt->errOn){
    if (!err){
        clrBitMicro(ramPnt, progPnt->Pnt_Stop);
        clrBitMicro(ramPnt, progPnt->Pnt_ErrControl);
    }
    else{
        sPnt->modeOn = 0;
        defBitMicro(ramPnt, progPnt->Pnt_ContactorOn, !logicOn);
        setBitMicro(ramPnt, progPnt->Pnt_ErrControl);        
        setBitMicro(ramPnt, progPnt->Pnt_Stop);
        if (sPnt->failEn){
            sPnt->failEn = 0;
//            measMem.NumFail = SlowFailMask_Contactor;
            FailValueCopy(mPnt);
            SetParamJrnM3(&RecJrnFailWr );
            EnaJrnFailWrite = 1;
        }
    }
    //else{
    //    clrBitMicro(ramPnt, progPnt->Pnt_Stop);
    //}
    
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCONTACTOR_1[11]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  PwmIn
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Control
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  DelayMotorOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  CntDrebezg
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  LogicContr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  LogicOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Mode
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //8  Stop
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  ErrControl
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  ContactorOn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //11 PwmOut
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONTACTOR", "DD", TblFuncCONTACTOR_1, FuncCONTACTOR_1, sizeof(TblFuncCONTACTOR_1), sizeof(StateFuncContactor_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONTACTOR_1,
#endif
//********************************************************
