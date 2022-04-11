#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Xin;                          //1  Xin
    u16  Pnt_X1 ;                           //2  X1
    u16  Pnt_X2 ;                           //3  X2
    u16  Pnt_Y1 ;                           //4  Y1
    u16  Pnt_Y2 ;                           //5  Y2
    u16  Pnt_Yout;                         //6  Yout
    u16  Pnt_End;
}FuncKX_B_type;
//********************************************************
u16 *FuncKX_B_3(FuncKX_B_type *progPnt, u32 ramPnt)
{
    f32     Xin;
    s16 X1, X2, Y1, Y2;

    Xin= (f32)load_s16(ramPnt, progPnt->Pnt_Xin);
    X1 = load_s16(ramPnt, progPnt->Pnt_X1 );
    X2 = load_s16(ramPnt, progPnt->Pnt_X2 );
    Y1 = load_s32(ramPnt, progPnt->Pnt_Y1 );
    Y2 = load_s32(ramPnt, progPnt->Pnt_Y2 );

    Xin = f32_Fy_x1x2y1y2x( (f32)X1, (f32)X2, (f32)Y1, (f32)Y2, Xin);
    //Xin = s32LimControl(Xin);
    save_float(ramPnt, progPnt->Pnt_Yout, Xin);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncKX_B_3[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  X1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Y2
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //6  Yout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("KX+B", "DA", TblFuncKX_B_3, FuncKX_B_3, sizeof(TblFuncKX_B_3), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncKX_B_3,
#endif
