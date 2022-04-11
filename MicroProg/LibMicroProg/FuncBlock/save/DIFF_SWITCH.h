#ifdef    GET_FUNC_CODE
typedef struct{
    s16 In;
    s16 nvVirgin;
}Diff_Switch_NV_type;
#define nvSignat 0x5AA5
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Cond;                         //2  Cond
    u16  Pnt_Mask;                         //3  Mask
    u16  Pnt_ResVal;                       //4  ResVal
    u16  Pnt_Out;                          //5  Out
    u16  Pnt_End;
}FuncDIFF_SWITCH_type;
#define     nvSfDiffSwitch  (*(Diff_Switch_NV_type*)0)
#define     sizeNvDS        (sizeof(Diff_Switch_NV_type))
//********************************************************
u16 *FuncDIFF_SWITCH_1(FuncDIFF_SWITCH_type *progPnt, u32 ramPnt)
{
    s16 In,Cond,Mask,ResVal;
    s16 UstRecoveryFlagValue;
    void    *nvAdr1;
    
    Diff_Switch_NV_type  *nvAdr, nvMem;
    

    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfDiffSwitch);
    nvAdr1 = (void *)((u32)&nv.UstRecoveryFlag);

    In = load_s16(ramPnt, progPnt->Pnt_In);
    Cond = load_s16(ramPnt, progPnt->Pnt_Cond);
    Mask = load_s16(ramPnt, progPnt->Pnt_Mask);
    ResVal = load_s16(ramPnt, progPnt->Pnt_ResVal);

    if (flagIsFirstCall){
        RdFramMicro(&nvMem, nvAdr, sizeNvDS);
        RdFramMicro( &UstRecoveryFlagValue, nvAdr1,  sizeof(u16));
        if ((nvMem.nvVirgin == nvSignat)&&(!UstRecoveryFlagValue)){
            save_s16(ramPnt, progPnt->Pnt_In,   nvMem.In);
        }
        else{
            return &progPnt->Pnt_End;
        }
    }
    else{
        if ((1 << Cond) & Mask){
            save_s16(ramPnt, progPnt->Pnt_Out, In);
        }
        else{
            In = ResVal;
            save_s16(ramPnt, progPnt->Pnt_In, ResVal);
            save_s16(ramPnt, progPnt->Pnt_Out, ResVal);
        }

        RdFramMicro(&nvMem, nvAdr, sizeNvDS);
        if (nvMem.In != In){
            nvMem.In = In;
            nvMem.nvVirgin = nvSignat;
            WrFramMicro(nvAdr, &nvMem, sizeNvDS);
        }
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDIFF_SWITCH_1[5]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Cond
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Mask
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  ResVal
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //5  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DIFF_SWITCH", "DA", TblFuncDIFF_SWITCH_1, FuncDIFF_SWITCH_1, sizeof(TblFuncDIFF_SWITCH_1), 0, sizeof(Diff_Switch_NV_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDIFF_SWITCH_1,
#endif
//********************************************************
