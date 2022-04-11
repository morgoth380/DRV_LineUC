#ifdef    GET_FUNC_CODE

typedef struct{
    s16     FiltrDinValue;
    s16     Filtr[8];
    
}DinFiltr8State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Tfiltr[8];                    //2-9  Tfiltr0
    u16  Pnt_Out;                          //10  Out
    u16  Pnt_End;
}FuncFILTR_DIN8_type;
//********************************************************
u16 *FuncFILTR_DIN8_1(FuncFILTR_DIN8_type *progPnt, u32 ramPnt)
{
    DinFiltr8State_type     *sPnt;
    s16                     InVal, numbDin, TimeFiltr, OutVal, cntDin, maskDin;
    
    sPnt = (DinFiltr8State_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    InVal= load_s16(ramPnt, progPnt->Pnt_In); 
    
    OutVal = sPnt->FiltrDinValue;
    for(numbDin=7; numbDin >0; numbDin--){
        cntDin = sPnt->Filtr[numbDin];
        maskDin = (1<<numbDin);
        if(InVal & maskDin){
            TimeFiltr = load_s16(ramPnt, progPnt->Pnt_Tfiltr[numbDin]); 
            if(cntDin < TimeFiltr)  cntDin++;
            else                    OutVal |= maskDin;
        }
        else{
            if(cntDin > 0)  cntDin--;
            else            OutVal &= ~maskDin;
        }
        sPnt->Filtr[numbDin] = cntDin;
    }
    save_s16(ramPnt, progPnt->Pnt_Out, OutVal);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFILTR_DIN8_1[10]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Tfiltr0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Tfiltr1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Tfiltr2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Tfiltr3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Tfiltr4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Tfiltr5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Tfiltr6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Tfiltr7
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //10  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("FILTR_DIN8", "DD", TblFuncFILTR_DIN8_1, FuncFILTR_DIN8_1, sizeof(TblFuncFILTR_DIN8_1), sizeof(DinFiltr8State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFILTR_DIN8_1,
#endif
//********************************************************
