#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Xin;                          //1  Xin
    u16  Pnt_Y1;                           //2  Y1
    u16  Pnt_Y2;                           //3  Y2
    u16  Pnt_Hyst;                         //4  Hyst
    u16  Pnt_Mode;                         //5  Mode
    u16  Pnt_Yout;                         //6  Yout
    u16  Pnt_End;
}FuncKX_B_SCALE_type;
//********************************************************
u16 *FuncKX_B_SCALE_1(FuncKX_B_SCALE_type *progPnt, u32 ramPnt)
{
    s16         Mode;
    s32         X, Xin, Y1, Y2, Out;
    
    Out = 0;
    Mode = load_s16(ramPnt, progPnt->Pnt_Mode); //считв. форму задания Авх1

    Xin  = load_s32(ramPnt, progPnt->Pnt_Xin); //считыв. значение входного аналогового сигнала (в %)
    Y1   = load_s32(ramPnt, progPnt->Pnt_Y1);  //считыв. значение уставки максимальной частоты
    Y2   = load_s32(ramPnt, progPnt->Pnt_Y2);  //считв.
    
    X = (Xin >= 0) ? Xin : (-Xin);

    switch(Mode){
        case Stand_kx_b:
            Out = _IQrmpy((Y2 - Y1), X) + Y1;
            break;
        case Lim_kx_b:
            Out = _IQrmpy(Y2, X);
            if(Out < Y1)    Out = Y1;
            break;
        case NotSens_kx_b:
            Out = _IQrmpy(Y2, X);
            if(Out < Y1)    Out = 0;
            break;
        case NotSens0_kx_b:
            if(Xin == _IQ(0.))  Out = _IQ(0.);
            else            Out = _IQrmpy((Y2 - Y1), X) + Y1;
    }

    if(Xin < 0) Out = -Out;

    save_s32(ramPnt, progPnt->Pnt_Yout, Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncKX_B_SCALE_1[6]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Xin 
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Y1  
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Y2  
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  X2  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Mode
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Yout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("KX+B_SCALE", "DA", TblFuncKX_B_SCALE_1, FuncKX_B_SCALE_1, sizeof(TblFuncKX_B_SCALE_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncKX_B_SCALE_1,
#endif
//********************************************************

//********************************************************
/*     ЭТО БЫЛО
u16 *FuncKX_B_SCALE_1(FuncKX_B_SCALE_type *progPnt, u32 ramPnt)
{
    s16         Mode;
    s32         X, Xin, Y1, Y2, Out;
    
    Xin  = (s32)load_s16(ramPnt, progPnt->Pnt_Xin);
    Y1   = (s32)load_s16(ramPnt, progPnt->Pnt_Y1);
    Y2   = (s32)load_s16(ramPnt, progPnt->Pnt_Y2);
    
    Out = 0;
    Mode = load_s16(ramPnt, progPnt->Pnt_Mode);
    X = (Xin >= 0) ? Xin : (-Xin);
    switch(Mode){
    case Stand_kx_b:
        Out = ((((Y2 - Y1)*X))>>12) + Y1;
        break;
    case Lim_kx_b:
        Out = (Y2*X)>>12;
        if(Out < Y1)    Out = Y1;
        break;
    case NotSens_kx_b:
        Out = (Y2*X)>>12;
        if(Out < Y1)    Out = 0;
        break;
    case NotSens0_kx_b:
        if(Xin == 0)    Out = 0;
        else            Out = ((((Y2 - Y1)*X))>>12) + Y1;
    }
    if(Xin < 0) Out *= -1;
    save_s16(ramPnt, progPnt->Pnt_Yout, (s16)Out);
    return &progPnt->Pnt_End;
};
#endif
//=======================================================
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncKX_B_SCALE_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Xin 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Y1  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Y2  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  X2  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Mode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Yout
};
#endif
//=======================================================
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("KX+B_SCALE", "DA", TblFuncKX_B_SCALE_1, FuncKX_B_SCALE_1, sizeof(TblFuncKX_B_SCALE_1), 0, 0, 1),
#endif
//=======================================================
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncKX_B_SCALE_1,
#endif
//=======================================================
*/
