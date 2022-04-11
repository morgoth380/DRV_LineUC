#ifdef    GET_FUNC_CODE


typedef struct{
    u16  PrevRes;
    u32  CntMoto;
}StateFuncMOTOH_type;

typedef struct{
        u32  CntMoto;
}nvStateFuncMOTOH_type;

#define     nvSfMOTOH     (*(nvStateFuncMOTOH_type*)0)

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_On_Off;                       //1  On/Off
    u16  Pnt_EnCMH;                        //2  EnCMH
    u16  Pnt_ResMH;                        //3  ResMH
    u16  Pnt_TikMin;                       //4  TikMin
    u16  Pnt_CntMH;                        //5  CntMH
    u16  Pnt_End;
}FuncMOTOH_type;
//********************************************************
u16 *FuncMOTOH_1(FuncMOTOH_type *progPnt, u32 ramPnt)
{
    StateFuncMOTOH_type *sPnt;
    nvStateFuncMOTOH_type *nvAdr;
    u16 Tmp;

    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfMOTOH);
    sPnt = (StateFuncMOTOH_type *)(ramPnt + progPnt->Pnt_State);

    if(flagIsFirstCall){
        RdFramMicro(&sPnt->CntMoto, nvAdr, sizeof(s32));
        sPnt->PrevRes  = 0;
    }
    else{
        if(testBitMicro(ramPnt, progPnt->Pnt_On_Off) && (testBitMicro(ramPnt, progPnt->Pnt_EnCMH) == 0)){  //Разрешен счет моточасов и автовентиль отключен
            if(testBitMicro(ramPnt, progPnt->Pnt_TikMin)){  //Загрузка тика RTC
                sPnt->CntMoto++;
                WrFramMicro(nvAdr, &sPnt->CntMoto, sizeof(s32));
            }
        }

        Tmp = (u16)load_s16(ramPnt, progPnt->Pnt_ResMH);
        if((sPnt->PrevRes == 0) && Tmp){                         //Пришел нарастающий фронт сброса моточасов?
            sPnt->CntMoto = 0;
            WrFramMicro(nvAdr, &sPnt->CntMoto, sizeof(s32));
        }
        sPnt->PrevRes = Tmp;
    }
    save_s32(ramPnt, progPnt->Pnt_CntMH, sPnt->CntMoto/60 );

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMOTOH_1[5]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  On/Off
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  EnCMH
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  ResMH
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  TikMin
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //5  CntMH
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MOTOH", "DD", TblFuncMOTOH_1, FuncMOTOH_1, sizeof(TblFuncMOTOH_1), sizeof(StateFuncMOTOH_type), sizeof(nvStateFuncMOTOH_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMOTOH_1,
#endif
//********************************************************
