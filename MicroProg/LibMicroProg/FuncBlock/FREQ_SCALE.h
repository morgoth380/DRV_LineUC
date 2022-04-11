#ifdef    GET_FUNC_CODE
#define FIX_FREQ 3 //разрешение перехода на фиксированные частоты
typedef struct{
    f32 Out;
    u8 FailAinFix;
}FuncFREQ_SCALEtate_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Ain;                          //1  Ain
    u16  Pnt_UstFmin;                      //2  UstFmin
    u16  Pnt_UstFmax;                      //3  UstFmax
    u16  Pnt_Hyst;                         //4  Hyst
    u16  Pnt_FixFreqMax;                   //5  FixFreqMax
    u16  Pnt_FixFreqMin;                   //6  FixFreqMin
    u16  Pnt_AinFailCode;                  //7  AinFailCode
    u16  Pnt_EnFreqFix;                    //8  EnFreqFix
    u16  Pnt_FailAinEn;                    //9  FailAinEn
    u16  Pnt_Mode;                         //10 Mode
    u16  Pnt_OutFreq;                      //11 OutFreq
    u16  Pnt_End;
}FuncFREQ_SCALE_type;
//********************************************************
u16 *FuncFREQ_SCALE_1(FuncFREQ_SCALE_type *progPnt, u32 ramPnt)
{
    s16         Mode, PwmOn, FixfreqEn;
    u16         failCode;
    f32         Xin, Y1, Y2, X, Out;
    
    FuncFREQ_SCALEtate_type *sPnt;
    sPnt = (FuncFREQ_SCALEtate_type *)(ramPnt + progPnt->Pnt_State);

    Out = 0;
    Mode = load_s16(ramPnt, progPnt->Pnt_Mode);     //считв. форму задания Авх1

    Xin  = load_float(ramPnt, progPnt->Pnt_Ain);      //считыв. значение входного аналогового сигнала (в %)
    Y1   = load_float(ramPnt, progPnt->Pnt_UstFmin);  //считыв. значение уставки минимальной частоты
    Y2   = load_float(ramPnt, progPnt->Pnt_UstFmax);  //считв. значение уставки максимальной частоты

    X = (Xin >= 0) ? Xin : (-Xin);                              //проверка на отрицательный уровень аналогового сигнала
    PwmOn = testBitMicro(ramPnt, progPnt->Pnt_FailAinEn);       //флаг включенного ШИМ
    FixfreqEn = load_s16(ramPnt, progPnt->Pnt_EnFreqFix);       //сигнал включения переходов на фикс. частоты
    failCode = load_s16(ramPnt, progPnt->Pnt_AinFailCode);      //считываем значение кода ошибки аналогового входа
    
    if ((!failCode) || (FixfreqEn != FIX_FREQ))                 //работаем по хараетеристике "задание-частота" пока нет аварии или если переход на фиксированные скорости запрещен
    {
        switch(Mode){
            case Stand_kx_b:      //тип характеристики "стандартная"
                Out = ((Y2 - Y1) * X) + Y1;
                break;
            case Lim_kx_b:        //тип характеристики "ограничение"
                Out = Y2 * X;
                if(Out < Y1)    
                    Out = Y1;
                break;
            case NotSens_kx_b:   //тип характеристики "зона нечувствительности"
                Out = Y2 * X;
                if(Out < Y1)    
                    Out = 0;
                break;
            case NotSens0_kx_b:  //тип характеристики "зона нечувствительности 0"
                if(Xin == 0)  
                    Out = 0;
                else            
                    Out = ((Y2 - Y1) * X) + Y1;
            }
            if(Xin < 0) 
                Out = -Out;
    }
    else //иначе, обнаружена ошибка аналогового входа
    {
        if (PwmOn && (FixfreqEn == FIX_FREQ)) //если переход на фикс. частоты разрешен
        {
            if(sPnt->FailAinFix == 0) //если авария обнаружена впервый раз
            {
               if(failCode == 2) //если тип аварии - превышение значения входного аналогового сигнала
               {
                  sPnt->Out = load_float(ramPnt, progPnt->Pnt_FixFreqMax); //задаем максимально допустимую фиксированную частоту
               }
               else //иначе, авария пониженного уровня аналогового сигнала
               {
                  sPnt->Out = load_float(ramPnt, progPnt->Pnt_FixFreqMin); //иначе тип аварии - понижение уровня входного аналогового сигнала ниже допустимого
               }
               sPnt->FailAinFix = 1; //запоминаем то, что задавали фиксированную частоту, чтобы сделать это один раз при обнаружении аварии
            }
        }
    }

    if (!PwmOn) //если ШИМ отключен (переход на другую частоту после фиксированной будет возможен после останова двигателя)
    {
        sPnt->FailAinFix = 0; //сброс защелки
        sPnt->Out = 0;        //сброс фиксированного задания
    }

    if(sPnt->FailAinFix)
    {
        Out = sPnt->Out;
    }

    save_float(ramPnt, progPnt->Pnt_OutFreq, Out); //выдаем значение выходной частоты

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFREQ_SCALE_1[11]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ain
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  UstFmin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  UstFmax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Hyst
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  FixFreqMax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  FixFreqMin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  AinFailCode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  EnFreqFix
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  FailAinEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Mode
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //11  OutFreq
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FREQ_SCALE", "DA", TblFuncFREQ_SCALE_1, FuncFREQ_SCALE_1, sizeof(TblFuncFREQ_SCALE_1), sizeof(FuncFREQ_SCALEtate_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFREQ_SCALE_1,
#endif
//********************************************************
