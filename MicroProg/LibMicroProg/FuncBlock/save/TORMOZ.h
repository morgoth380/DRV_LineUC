#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Enable;                      //1  Anable
    u16  Pnt_U_0;                         //2  U_0%
    u16  Pnt_U_100;                       //3  U_100%
    u16  Pnt_End;
}FuncTORMOZ_type;
//********************************************************
u16 *FuncTORMOZ_1(FuncTORMOZ_type *progPnt, u32 ramPnt)
{
    COMMONPARAMM3 *mPnt3;
    mPnt3 = &GlobalM3;
    
    if(load_s16(ramPnt, progPnt->Pnt_Enable))
        mPnt3->FlagCommand.bit.TormSwEnable = 1;
    else
        mPnt3->FlagCommand.bit.TormSwEnable = 0;

    mPnt3->Ud0   = load_s32(ramPnt, progPnt->Pnt_U_0);
    mPnt3->Ud100 = load_s32(ramPnt, progPnt->Pnt_U_100);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTORMOZ_1[3]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Enable
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  U_0%
        S32_PIN_TYPE | INPUT_PIN_MODE      //3  U_100%
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("TORMOZ", "DD", TblFuncTORMOZ_1, FuncTORMOZ_1, sizeof(TblFuncTORMOZ_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTORMOZ_1,
#endif
//********************************************************
