#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_BrkFail;                     //1  BrkFail
    u16  Pnt_FdbFail;                     //2  FdbFail
    u16  Pnt_MeasPar;                     //3  MeasPar
    u16  Pnt_Mode;                        //4  Mode
    u16  Pnt_Silence;                     //5  Silence
    u16  Pnt_FBrkFail;                    //6  FBrkFail
    u16  Pnt_FFdbFail;                    //7  FFdbFail
    u16  Pnt_End;
}FuncCONT_FAIL_type;

#define SilentFault 0
//********************************************************
u16 *FuncCONT_FAIL_1(FuncCONT_FAIL_type *progPnt, u32 ramPnt)
{
    u16 Mode, MeasParam, BrkFault, FdbFault;
    
    BrkFault = testBitMicro (ramPnt, progPnt->Pnt_BrkFail);
    FdbFault = testBitMicro (ramPnt, progPnt->Pnt_FdbFail);
    MeasParam = testBitMicro (ramPnt, progPnt->Pnt_MeasPar);
    Mode = load_s16(ramPnt, progPnt->Pnt_Mode);
      
    // Если включено измерение параметров - мы всегда контролируем аварию по контактору
    clrBitMicro (ramPnt, progPnt->Pnt_FBrkFail);
    clrBitMicro (ramPnt, progPnt->Pnt_FFdbFail);
    clrBitMicro (ramPnt, progPnt->Pnt_Silence);
    if (MeasParam) {
      defBitMicro(ramPnt, progPnt->Pnt_FBrkFail, BrkFault);
      defBitMicro(ramPnt, progPnt->Pnt_FFdbFail, FdbFault);
    } else {
      if (Mode == SilentFault) {
        defBitMicro(ramPnt, progPnt->Pnt_Silence, (BrkFault | FdbFault));
      } else {
        defBitMicro(ramPnt, progPnt->Pnt_FBrkFail, BrkFault);
        defBitMicro(ramPnt, progPnt->Pnt_FFdbFail, FdbFault);
      }
    }
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCONT_FAIL_1[7]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  BrkFail
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  FdbFail
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  MeasPar
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Mode
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Silence
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //6  FBrkFail
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //7  FFdbFail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CONT_FAIL", "DD", TblFuncCONT_FAIL_1, FuncCONT_FAIL_1, sizeof(TblFuncCONT_FAIL_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCONT_FAIL_1,
#endif
//********************************************************
