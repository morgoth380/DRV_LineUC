#ifdef    GET_FUNC_CODE

#define cooper 0.019f       // удельное сопротивление меди
#define aluminium 0.031f    // удельное сопротивление алюминия

typedef struct{
    f32 R ;
    f32 S ;
    s16 L ;
    s16 Material ;
}FuncRES_CAB_STATE_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_L;                            //1  L
    u16  Pnt_Material;                     //2  Material
    u16  Pnt_S;                            //3  S
    u16  Pnt_R;                            //4  R
    u16  Pnt_End;
}FuncRES_CAB_type;
//********************************************************
u16 *FuncRES_CAB_1(FuncRES_CAB_type *progPnt, u32 ramPnt)
{
    s16     L, Material;
    f32     R, S;
    FuncRES_CAB_STATE_type *sPnt;
    COMMONPARAMM3 *mPnt3;
    
        mPnt3 = &GlobalM3;
    
    sPnt = (FuncRES_CAB_STATE_type *)(ramPnt + progPnt->Pnt_State);
    L = load_s16(ramPnt, progPnt->Pnt_L);
    S = load_float(ramPnt, progPnt->Pnt_S);
    R = load_float(ramPnt, progPnt->Pnt_R);
    Material = load_s16(ramPnt, progPnt->Pnt_Material);

    if (sPnt->R != R)
    {
        sPnt->R = R;
        mPnt3->R_cab = sPnt->R;
        return &progPnt->Pnt_End;
    }

    if ((sPnt->S != S) || (sPnt->L != L) || (sPnt->Material != Material))
    {
        sPnt->S = S;
        sPnt->L = L;
        sPnt->Material = Material;
        if (sPnt->Material > 0) sPnt->R = aluminium * sPnt->L / sPnt->S;
        else sPnt->R = cooper * sPnt->L / sPnt->S;
        save_float(ramPnt, progPnt->Pnt_R, sPnt->R);
        mPnt3->R_cab = sPnt->R;
        return &progPnt->Pnt_End;
    }
    mPnt3->R_cab = sPnt->R;
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRES_CAB_1[4]={
        S16_PIN_TYPE | INPUT_PIN_MODE,       //1  L
        S16_PIN_TYPE | INPUT_PIN_MODE,       //2  Material
        FLOAT_PIN_TYPE | INPUT_PIN_MODE,     //3  S
        FLOAT_PIN_TYPE | INPUT_PIN_MODE      //4  R
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("RES_CAB", "DA", TblFuncRES_CAB_1, FuncRES_CAB_1, sizeof(TblFuncRES_CAB_1), sizeof(FuncRES_CAB_STATE_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRES_CAB_1,
#endif
//********************************************************
