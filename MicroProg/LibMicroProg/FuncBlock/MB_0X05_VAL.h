#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_Val[8];                       //0-7 Val 0-7
    u16  Pnt_End;
}FuncMB_0X05_VAL_type;
//********************************************************
u16 *FuncMB_0X05_VAL_1(FuncMB_0X05_VAL_type *progPnt, u32 ramPnt) 
{
  
    u16                         *Pnt, val, i;
    StateFuncMODBUS_0X05_type   *sPnt;

    sPnt = (StateFuncMODBUS_0X05_type *)(ramPnt + progPnt->Pnt_State);

    if(flagIsFirstCall){                            // Начальная инициализация
        memset(sPnt, 0,sizeof(StateFuncMODBUS_0X05_type));
        sPnt->mask = 0xFFFF;
    }

    readAckZero(val, sPnt->val);                    // Вычитать присланное значение и обнулить
    sPnt->state |= val&0xFF;                        // Установить которые надо установить
    sPnt->state &= ~(val>>8);                        // Сбросить которые надо сбросить

    val = sPnt->state;
    for(Pnt = &progPnt->Pnt_Val[7], i=0x80; i!=0; i>>=1, Pnt--){
        defBitMicro(ramPnt, *Pnt, (val & i) );
    }
  
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMB_0X05_VAL_1[8]={
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //0  Val 0
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //1  Val 1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //2  Val 2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //3  Val 3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //4  Val 4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //5  Val 5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //6  Val 6
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG     //7  Val 7
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MB_0X05_VAL", "MB5_", TblFuncMB_0X05_VAL_1, FuncMB_0X05_VAL_1, sizeof(TblFuncMB_0X05_VAL_1), sizeof(StateFuncMODBUS_0X05_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMB_0X05_VAL_1,
#endif
//********************************************************
