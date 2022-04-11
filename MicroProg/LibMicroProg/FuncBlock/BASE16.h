#ifdef    GET_FUNC_CODE

//********************************************************

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Base[15];
    u16  Pnt_End;
}FuncBASE16_type;
//********************************************************

#define KOLVSTATE 11		// 7 При изменении количества баз необходимо изменить значение KOLVSTATE

// !!! Эта функция должна вызываться после BASE_CALC !!!
u16 *FuncBASE16_1(FuncBASE16_type *progPnt, u32 ramPnt)
{
    MPOpis_type             *mPnt;
    s16                     i;
    
    mPnt = (MPOpis_type *)&MPOpis;

    if(KOLVSTATE > 15)  mPnt->kolvState = 15;
    else				mPnt->kolvState = KOLVSTATE;

    for(i=0; i < mPnt->kolvState; i++){
        mPnt->Base[i+1] = load_s16(ramPnt, progPnt->Pnt_Base[i]);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBASE16_1[15]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Base0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Base1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Base2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Base3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Base4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Base5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Base6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Base7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Base8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Base9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Base10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Base11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Base12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Base13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Base14
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("BASE16", "DA", TblFuncBASE16_1, FuncBASE16_1, sizeof(TblFuncBASE16_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBASE16_1,
#endif
//********************************************************
