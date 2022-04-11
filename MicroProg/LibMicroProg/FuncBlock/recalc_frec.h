#ifdef    GET_FUNC_CODE
extern  f32       Fbase_rad_s;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Fbase;                        //1  Fbase               //Базовая частота в Гц
    u16  Pnt_Zp;                           //2  Zp                  //Количество пар полюсов
    u16  Pnt_Mode;                         //3  Mode                //Тип едениц измерения 0 - Гц, 1 - Об/мин, 2 - Рад/сек.
    u16  Pnt_OutBase;                      //4  OutBase
    u16  Pnt_End;
}FuncRECALC_FREC_type;
//********************************************************
/* Пересчет параметра скорость вращения ротора в различные единицы измерения */
//********************************************************
u16 *FuncRECALC_FREC_1(FuncRECALC_FREC_type *progPnt, u32 ramPnt)
{
    u16       Zp, Mode;
    f32       Fbase, tmp;
    
  
    Fbase   = load_s16(ramPnt, progPnt->Pnt_Fbase);
    Zp      = load_s16(ramPnt, progPnt->Pnt_Zp);
    Mode    = load_s16(ramPnt, progPnt->Pnt_Mode);
    if (Zp < 1) Zp = 1;

    tmp = Fbase /(f32)Zp;

    if      (Mode == 1){ Fbase = 6.0f * tmp;  }
    else if (Mode == 2){ Fbase = Fbase_rad_s;}

    Fbase_rad_s = _2PI * tmp;
    
    save_s16(ramPnt, progPnt->Pnt_OutBase, (s16)Fbase);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRECALC_FREC_1[4]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Fbase
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Zp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Mode
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //4  OutBase
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("RECALC_FREC", "DA", TblFuncRECALC_FREC_1, FuncRECALC_FREC_1, sizeof(TblFuncRECALC_FREC_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRECALC_FREC_1,
#endif
//********************************************************
