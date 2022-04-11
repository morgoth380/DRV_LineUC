#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_Set[8];                         //0-7 Set7
    u16  Pnt_Clr[8];                         //8-15 Clr7
    u16  Pnt_End;
}FuncMB_0X05_type;
//********************************************************
u16 *FuncMB_0X05_1(FuncMB_0X05_type *progPnt, u32 ramPnt)
{
    u16                         *Pnt, val, i;
    StateFuncMODBUS_0X05_type   *sPnt;
    
    sPnt = (StateFuncMODBUS_0X05_type *)(ramPnt + progPnt->Pnt_State);
    sPnt->mask = 0xFFFF;

    readAckZero(val, sPnt->val);                                        // Вычитать присланное значение и обнулить
    for( Pnt = progPnt->Pnt_Set, i=1; i!=0; i<<=1, Pnt++ ){
        if(val & i)
            setBitMicro(ramPnt, *Pnt);                      // Собственно установить, сформировать импульс
        else
            clrBitMicro(ramPnt, *Pnt);                      // Прочистить текущее состояние
        }   

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMB_0X05_1[16]={
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //1  Set0
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //2  Set1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //3  Set2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //4  Set3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //5  Set4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //6  Set5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //7  Set6
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //8  Set7
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //9  Clr0
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //10 Clr1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //11 Clr2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //12 Clr3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //13 Clr4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //14 Clr5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //15 Clr6
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //16 Clr7
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MB_0X05", "MB5_", TblFuncMB_0X05_1, FuncMB_0X05_1, sizeof(TblFuncMB_0X05_1), sizeof(StateFuncMODBUS_0X05_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMB_0X05_1,
#endif
//********************************************************
