#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_TormOn;                       //1  TormOn
    u16  Pnt_Duty;                         //2  Duty
    u16  Pnt_End;
}FuncTORM2_type;
//********************************************************
extern u16 cntTormSw;
//********************************************************
u16 *FuncTORM2_1(FuncTORM2_type *progPnt, u32 ramPnt)
{
#if(CHAR_BIT == 16)
    //load_s16(ramPnt, progPnt->Pnt_Duty);
    if(testBitMicro(ramPnt, progPnt->Pnt_TormOn)){    
        if(cntTormSw >0){
            cntTormSw--;
            TormozUp();
        }
        else{
            cntTormSw = 9;
            TormozDown();
        }
    }
    else{
        cntTormSw = 0;
        TormozDown();
    }
#endif  
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTORM2_1[2]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  TormOn
        S16_PIN_TYPE | INPUT_PIN_MODE      //2  Duty
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TORM2", "", TblFuncTORM2_1, FuncTORM2_1, sizeof(TblFuncTORM2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTORM2_1,
#endif
//********************************************************
