#ifdef    GET_FUNC_CODE

typedef struct{
    u16  poshagEn;
    u16  prevPusk;
    //u16  waitStop;
}POSHAG_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_ExtStop;                      //1  ExtStop
    u16  Pnt_DinEn;                        //2  DinEn
    u16  Pnt_Pusk;                         //3  Pusk
    u16  Pnt_Logic;                        //4  Logic
    u16  Pnt_StopOut;                      //5  StopOut
    u16  Pnt_End;
}FuncPOSHAG_type;
//********************************************************
u16 *FuncPOSHAG_1(FuncPOSHAG_type *progPnt, u32 ramPnt)
{
    POSHAG_State_type               *sPnt;
    //_iq                             Fout;
    u16                             pusk, dinEn, extStop;
    
    
    pusk    = testBitMicro(ramPnt, progPnt->Pnt_Pusk);
    dinEn   = testBitMicro(ramPnt, progPnt->Pnt_DinEn);
    extStop = testBitMicro(ramPnt, progPnt->Pnt_ExtStop);

    sPnt = (POSHAG_State_type *)(ramPnt + progPnt->Pnt_State);
    
    clrBitMicro(ramPnt, progPnt->Pnt_StopOut);        //Снимаем стоп

    if (pusk != 0 && sPnt->prevPusk == 0 && dinEn != 0){
        sPnt->poshagEn = 1;
    }
    if (extStop && sPnt->poshagEn == 1){
        clrBitMicro(ramPnt, progPnt->Pnt_Logic);          //Меняем логику взад
        clrBitMicro(ramPnt, progPnt->Pnt_StopOut);        //Снимаем стоп
        sPnt->poshagEn = 0;
    }
    
    if (sPnt->poshagEn == 2 && pusk == 0){
        sPnt->poshagEn = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_Logic);
    }
    sPnt->prevPusk = pusk;
    
    if (sPnt->poshagEn == 1){
        setBitMicro(ramPnt, progPnt->Pnt_Logic);        //Меняем логику
//=================СТОПЫ ПОШАГОВОГО РЕЖИМА================================================
        if (dinEn == 0){   //Выход из пошагового
            sPnt->poshagEn = 2;
            setBitMicro(ramPnt, progPnt->Pnt_StopOut);      //Выдача стопа
            //clrBitMicro(ramPnt, progPnt->Pnt_Logic);        //изменение логики
        }
    }
    /*if (sPnt->waitStop){
        
    }*/

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPOSHAG_1[5]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  ExtStop
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  DinEn
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Pusk
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Logic
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  StopOut
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("POSHAG", "DA", TblFuncPOSHAG_1, FuncPOSHAG_1, sizeof(TblFuncPOSHAG_1), sizeof(POSHAG_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPOSHAG_1,
#endif
//********************************************************
