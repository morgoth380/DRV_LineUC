#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_EnSet[8];                       //1  EnSet0
    u16  Pnt_EnClr[8];                       //9  EnClr0
    u16  Pnt_Set[8];                         //17 Set7
    u16  Pnt_Clr[8];                         //25 Clr7
    u16  Pnt_SelfClr;                        //33 SelfClr
    u16  Pnt_End;
}FuncMODBUS_0X05_type;
//********************************************************
//////////////////////////////
#ifdef _DEBUG_LOG_
    void LogRSout(void *ptr, u16 size);
    const char stopMB05[5]   = {01, 05, 0xFF, 0xFF, 0xFF};
    const char stopMB05Al[5] = {01, 05, 0xAA, 0xAA, 0xAA};
#endif
//////////////////////////////
//********************************************************
u16 *FuncMODBUS_0X05_1(FuncMODBUS_0X05_type *progPnt, u32 ramPnt)
{
    u16                         *Pnt, mask, val, i;
    StateFuncMODBUS_0X05_type   *sPnt;
    
    sPnt = (StateFuncMODBUS_0X05_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    mask = 0;

    for(Pnt = progPnt->Pnt_EnSet, i=1; i!=0; i<<=1, Pnt++){
        if(testBitMicro(ramPnt, *Pnt)){
            mask |= i;
        }
    }
    sPnt->mask = mask;

    if( testBitMicro(ramPnt, progPnt->Pnt_SelfClr) ){
        for( Pnt = progPnt->Pnt_Set, i=1; i!=0; i<<=1, Pnt++ )
            clrBitMicro(ramPnt, *Pnt);                                  // Прочистить текущее состояние
    }
    readAckZero(val, sPnt->val);                                        // Вычитать присланное значение и обнулить
    for( Pnt = progPnt->Pnt_Set, i=1; i!=0; i<<=1, Pnt++ ){
        if(val & i){
            setBitMicro(ramPnt, *Pnt);                                  // Собственно установить, сформировать импульс
//////////////////////////////////////////////////////////        
#ifdef _DEBUG_LOG_
            if(i == (1<<9)){
                LogRSout((void*)stopMB05Al, 5); 
            }
            if(i == (1<<8) ){
                LogRSout((void*)stopMB05, 5);
            }
#endif
//////////////////////////////////////////////////////////        
        }   
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMODBUS_0X05_1[33]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  EnSet0
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  EnSet1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  EnSet2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  EnSet3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  EnSet4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  EnSet5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  EnSet6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  EnSet7
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  EnClr0
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10 EnClr1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //11 EnClr2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12 EnClr3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13 EnClr4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14 EnClr5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15 EnClr6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //16 EnClr7
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //17 Set0
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //18 Set1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //19 Set2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //20 Set3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //21 Set4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //22 Set5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //23 Set6
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //24 Set7
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //25 Clr0
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //26 Clr1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //27 Clr2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //28 Clr3
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //29 Clr4
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //30 Clr5
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //31 Clr6
        BIT_PIN_TYPE | OUTPUT_PIN_MODE | INDEPEND_FLAG,    //32 Clr7
        BIT_PIN_TYPE | INPUT_PIN_MODE  | INDEPEND_FLAG     //33 SelfClr
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("MODBUS_0X05", "MB5_", TblFuncMODBUS_0X05_1, FuncMODBUS_0X05_1, \
sizeof(TblFuncMODBUS_0X05_1), sizeof(StateFuncMODBUS_0X05_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMODBUS_0X05_1,
#endif
//********************************************************
