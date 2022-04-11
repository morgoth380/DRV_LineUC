#ifdef    GET_FUNC_CODE
typedef struct{
    u16  PrevRes;
    u32  CntMoto;
}StateFuncCMH_type;

typedef struct{
        u32  CntMoto;
}nvStateFuncCMH_type;

#define     nvSfCMH     (*(nvStateFuncCMH_type*)0)

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_On_Off;                       //1  On/Off
    u16  Pnt_ResMH;                        //2  ResMH
    u16  Pnt_TikMin;                       //3  TikMin
    u16  Pnt_CntMH;                        //4  CntMH
    u16  Pnt_End;
}FuncCMH_type;
#define MIN_KOEF    1
//********************************************************
u16 *FuncCMH_1(FuncCMH_type *progPnt, u32 ramPnt)
{
    StateFuncCMH_type *sPnt;
    nvStateFuncCMH_type *nvAdr;
    u16 Tmp;

    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfCMH);
    sPnt = (StateFuncCMH_type *)(ramPnt + progPnt->Pnt_State);

    if(flagIsFirstCall){
        RdFramMicro(&sPnt->CntMoto, nvAdr, sizeof(s32));
        sPnt->PrevRes  = 0;
    }
    else{
        if(testBitMicro(ramPnt, progPnt->Pnt_On_Off)){  //Разрешен счет моточасов?
            if(testBitMicro(ramPnt, progPnt->Pnt_TikMin)){  //Загрузка тика RTC
                sPnt->CntMoto++;
                WrFramMicro(nvAdr, &sPnt->CntMoto, sizeof(s32));
            }
        }

        Tmp = testBitMicro(ramPnt, progPnt->Pnt_ResMH);
        if (!sPnt->PrevRes && Tmp){                         //Пришел нарастающий фронт сброса моточасов?
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
const char TblFuncCMH_1[4]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  On/Off
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  ResMH
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  TikMin
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //4  CntMH
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CMH", "DD", TblFuncCMH_1, FuncCMH_1, sizeof(TblFuncCMH_1), sizeof(StateFuncCMH_type), sizeof(nvStateFuncCMH_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMH_1,
#endif
//********************************************************
//********************************************************
//***********2-я перегрузка*******************************
//********************************************************
//********************************************************
#ifdef    GET_FUNC_CODE
//********************************************************
u16 *FuncCMH_2(FuncCMH_type *progPnt, u32 ramPnt)
{
    StateFuncCMH_type *sPnt;
    nvStateFuncCMH_type *nvAdr;
    u16 Tmp;

    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfCMH);
    sPnt = (StateFuncCMH_type *)(ramPnt + progPnt->Pnt_State);

    if(flagIsFirstCall){
        RdFramMicro(&sPnt->CntMoto, nvAdr, sizeof(s32));
        sPnt->PrevRes  = 0;
    }
    else{
        if(testBitMicro(ramPnt, progPnt->Pnt_On_Off)){  //Разрешен счет моточасов?
            if(testBitMicro(ramPnt, progPnt->Pnt_TikMin)){  //Загрузка тика RTC
                sPnt->CntMoto++;
                WrFramMicro(nvAdr, &sPnt->CntMoto, sizeof(s32));
            }
        }

        Tmp = (u16)load_s16(ramPnt, progPnt->Pnt_ResMH);
        if (!sPnt->PrevRes && Tmp){                         //Пришел нарастающий фронт сброса моточасов?
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
const char TblFuncCMH_2[4]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  On/Off
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  ResMH
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  TikMin
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //4  CntMH
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("CMH", "DD", TblFuncCMH_2, FuncCMH_2, sizeof(TblFuncCMH_2), sizeof(StateFuncCMH_type), sizeof(nvStateFuncCMH_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCMH_2,
#endif
//********************************************************
