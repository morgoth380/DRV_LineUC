#ifdef    GET_FUNC_CODE
typedef struct{
    u16     val;
}StateFuncSTATEMASHINE_type;

typedef struct{
    u16     val;
}nvStateFuncSTATEMASHINE_type;

#define     nvSMState           (*(nvStateFuncSTATEMASHINE_type*)0)
typedef struct{
    u16  Pnt_Nfunc;            //
    u16  Pnt_State;         //
    u16  Pnt_nvState;       //
    u16  Pnt_PuskFail    ;  //
    u16  Pnt_NearPusk    ;    //
    u16  Pnt_T_DoApvFail ;  //
    u16  Pnt_StopFail    ;  //
    u16  Pnt_NearStop    ;    //
    u16  Pnt_T_DoOffFail ;  //
    u16  Pnt_TypeTormFail;  //
    u16  Pnt_PuskTexn    ;  //
    u16  Pnt_T_DoApvTexn ;  //
    u16  Pnt_StopTexn    ;  //
    u16  Pnt_T_DoOffTexn ;  //
    u16  Pnt_TypeTormTexn;  //
    u16  Pnt_PuskMan     ;  //
    u16  Pnt_StopMan     ;  //
    u16  Pnt_FailStopMan ;  //
    u16  Pnt_Kolv_Fail   ;  //
    u16  Pnt_Kolv_Man    ;  //
    u16  Pnt_Kolv_Texn   ;  //
    u16  Pnt_Off_Zar     ;  //
    u16  Pnt_FailStop    ;  //
    u16  Pnt_Stop        ;  //
    u16  Pnt_Pusk        ;  //
    u16  Pnt_TimeDoStop  ;  //
    u16  Pnt_NearStopOut ;  //
    u16  Pnt_N_Stop      ;  //
    u16  Pnt_TimeDoApv   ;  //
    u16  Pnt_NearPuskOut ;  //
    u16  Pnt_N_Pusk      ;    //
    u16  Pnt_End         ;  //
}FuncSTATEMASHINE_type;
//********************************************************
u16 *FuncSTATEMASHINE_1(FuncSTATEMASHINE_type *progPnt, u32 ramPnt)
{
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSTATEMASHINE_1[28]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1   PuskFail    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2   NearPusk    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3   T_DoApvFail 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4   StopFail    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5   NearStop    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6   T_DoOffFail 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7   TypeTormFail
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8   PuskTexn    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9   T_DoApvTexn 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  StopTexn    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  T_DoOffTexn 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  TypeTormTexn
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  PuskMan     
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  StopMan     
         BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  FailStopMan 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Kolv_Fail   
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  Kolv_Man    
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  Kolv_Texn   
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //19  Off_Zar     
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //20  FailStop    
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  Stop        
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Pusk        
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //23  TimeDoStop  
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  NearStop    
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  N_Stop      
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //26  TimeDoApv   
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  NearPusk    
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  N_Pusk      
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("STATEMASHINE", "DD", TblFuncSTATEMASHINE_1, FuncSTATEMASHINE_1, sizeof(TblFuncSTATEMASHINE_1), sizeof(StateFuncSTATEMASHINE_type), sizeof(nvStateFuncSTATEMASHINE_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSTATEMASHINE_1,
#endif
//********************************************************
