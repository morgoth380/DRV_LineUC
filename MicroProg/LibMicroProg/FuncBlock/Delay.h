#ifdef    GET_FUNC_CODE
typedef struct{
    u32     shiftVal;
}DelayState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_in;                        //1  in
    u16  Pnt_Delay;                     //2  Delay
    u16  Pnt_Out;                       //3  Out
    u16  Pnt_End;
}FuncDelay_type;
//********************************************************
u16 *FuncDelay_1(FuncDelay_type *progPnt, u32 ramPnt)
{
    u16         val, del;
    u32         shiftVal;
    u32          *sPnt;

    sPnt = (u32 *)(ramPnt + progPnt->Pnt_State);
    shiftVal = *sPnt;
    
    if(testBitMicro(ramPnt, progPnt->Pnt_in))   shiftVal |=   (1<<0);
    else                                        shiftVal &= (~(1<<0));
    
    del = load_s16(ramPnt, progPnt->Pnt_Delay);
    if(del>30)  del = 30;
    
    if(shiftVal & (1<<del)) val = 1;
    else                    val = 0;
    
    *sPnt = (shiftVal<<1);
    defBitMicro(ramPnt, progPnt->Pnt_Out, val);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDelay_1[3]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  in    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Delay 
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //3  Out   
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("DELAY", "DD", TblFuncDelay_1, FuncDelay_1, sizeof(TblFuncDelay_1),sizeof(DelayState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDelay_1,
#endif
//********************************************************
