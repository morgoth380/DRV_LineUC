#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In0;                          //1  In0
    u16  Pnt_In1;                          //2  In1
    u16  Pnt_In2;                          //3  In2
    u16  Pnt_In3;                          //4  In3
    u16  Pnt_In4;                          //5  In4
    u16  Pnt_In5;                          //6  In5
    u16  Pnt_In6;                          //7  In6
    u16  Pnt_In7;                          //8  In7
    u16  Pnt_Num;                          //9  Num
    u16  Pnt_Out1;                         //10  Out1
    u16  Pnt_Out2;                         //11  Out2
    u16  Pnt_Out3;                         //12  Out3
    u16  Pnt_Out4;                         //13  Out4
    u16  Pnt_End;
}FuncMUX_4X4_4_type;
//********************************************************
u16 *FuncMUX_4X4_4_1(FuncMUX_4X4_4_type *progPnt, u32 ramPnt)
{
  f32   spd1, spd2, spd3, spd4;
  
  if (load_s16(ramPnt, progPnt->Pnt_Num) == 0) { // Bit code speed
    spd1 = load_float(ramPnt, progPnt->Pnt_In0);
    spd2 = load_float(ramPnt, progPnt->Pnt_In1);
    spd3 = load_float(ramPnt, progPnt->Pnt_In2);
    spd4 = load_float(ramPnt, progPnt->Pnt_In3);    
  } else { // Numerical speed
    spd1 = load_float(ramPnt, progPnt->Pnt_In4);
    spd2 = load_float(ramPnt, progPnt->Pnt_In5);
    spd3 = load_float(ramPnt, progPnt->Pnt_In6);
    spd4 = load_float(ramPnt, progPnt->Pnt_In7);        
  }
  
  save_float(ramPnt, progPnt->Pnt_Out1, spd1);
  save_float(ramPnt, progPnt->Pnt_Out2, spd2);
  save_float(ramPnt, progPnt->Pnt_Out3, spd3);
  save_float(ramPnt, progPnt->Pnt_Out4, spd4);
    
  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMUX_4X4_4_1[13]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Num
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Out1
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Out2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Out3
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //13  Out4
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MUX_4X4_4", "DD", TblFuncMUX_4X4_4_1, FuncMUX_4X4_4_1, sizeof(TblFuncMUX_4X4_4_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMUX_4X4_4_1,
#endif
//********************************************************
