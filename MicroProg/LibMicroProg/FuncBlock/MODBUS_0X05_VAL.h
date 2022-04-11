#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_EnSet[8];                     //1  EnSet0
    u16  Pnt_EnClr[8];                     //9  EnClr0
    u16  Pnt_Val[8];                       //17 Val7
    u16  Pnt_End;
}FuncMODBUS_0X05_VAL_type;
//********************************************************
u16 *FuncMODBUS_0X05_VAL_1(FuncMODBUS_0X05_VAL_type *progPnt, u32 ramPnt)
{
    u16                         *Pnt, mask, val, i;
    StateFuncMODBUS_0X05_type   *sPnt;

    sPnt = (StateFuncMODBUS_0X05_type *)(ramPnt + progPnt->Pnt_State);

    if(flagIsFirstCall){            //начальная инициализация
        memset(sPnt, 0,sizeof(StateFuncMODBUS_0X05_type));  // Начальная инициализация
    }

    mask = 0;
    for(Pnt = progPnt->Pnt_EnSet, i=1; i!=0; i<<=1, Pnt++){
        if(testBitMicro(ramPnt, *Pnt)) mask |= i;
    }
    sPnt->mask = mask;
    readAckZero(val, sPnt->val);        // Вычитать присланное значение и обнулить
    sPnt->state |= val&mask&0xFF;        // Установить которые надо установить
    sPnt->state &= ~((val&mask)>>8);    // Сбросить которые надо сбросить

    val = sPnt->state;
    for(Pnt = &progPnt->Pnt_Val[7], i=0x80; i!=0; i>>=1, Pnt--){
        defBitMicro(ramPnt, *Pnt, (val & i) );
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMODBUS_0X05_VAL_1[24]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  EnZer0
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  EnZer1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  EnZer2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  EnZer3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  EnZer4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  EnZer5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  EnZer6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  EnZer7
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  EnClr0
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  EnClr1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  EnClr2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  EnClr3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  EnClr4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  EnClr5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  EnClr6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //16  EnClr7
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //17  Val7
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //18  Val6
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //19  Val5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //20  Val4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //21  Val3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //22  Val2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //23  Val1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG     //24  Val0
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MODBUS_0X05_VAL", "MB5_", TblFuncMODBUS_0X05_VAL_1, FuncMODBUS_0X05_VAL_1, sizeof(TblFuncMODBUS_0X05_VAL_1), sizeof(StateFuncMODBUS_0X05_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMODBUS_0X05_VAL_1,
#endif
//********************************************************
