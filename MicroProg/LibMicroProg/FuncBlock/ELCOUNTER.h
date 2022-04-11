#ifdef    GET_FUNC_CODE

#define Ptime (3600UL * 50 / 100) //час 20 мс вт
//#define Ptime (3600UL / 100) //час 20 мс вт
#define P100W   100
#define time1min 15000

typedef struct{
    u32             Pcnt100w;
    u32             PcntMonth;
    u32             PcntYear;
    f32             Pcnt_sum;
    u16             cnt20ms;
    s16             cnt1min;
    u16             PrevMonth;
    u16             PrevYear;
    u32             EcoMonth;
    u32             EcoYear;
    ElcountJrn_type ElJRNo;
    u16             JrnNumRec;
}ELCOUNTER_State_type;

typedef struct{
    u32     Pcnt100w;
    u32     PcntMonth;
    u32     PcntYear;
    u32     EcoMonth;
    u32     EcoYear;
    u16     PrevMonth;
    u16     PrevYear;
    u16     JrnNumRec;
}ELCOUNTER_nvState_type;

#define     nvElCountero     (*(ELCOUNTER_nvState_type*)0)
#define sizeNvElCountero  (sizeof(ELCOUNTER_nvState_type))

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Pact_in;                      //1  Pact_in
    u16  Pnt_KPD;                          //2  KPD
    u16  Pnt_Pel_stop;                     //3  Pel_stop
    u16  Pnt_PwoFC;                        //4  PwoFC
    u16  Pnt_Year;                         //5  Year
    u16  Pnt_Month;                        //6  Month
    u16  Pnt_Min_Pulse;                    //7  Min_Pulse
    u16  Pnt_BASE_kW;                      //8  BASE_kW
    u16  Pnt_Res;                          //9  Res
    u16  Pnt_EcoYear;                      //10  EcoYear
    u16  Pnt_EcoMonth;                     //11  EcoMonth
    u16  Pnt_Eco;                          //12  Eco
    u16  Pnt_CntYear;                      //13  CntYear
    u16  Pnt_CntMonth;                     //14  CntMonth
    u16  Pnt_Cnt;                          //15  Cnt
    u16  Pnt_End;
}FuncELCOUNTER_type;
//********************************************************
u16 *FuncELCOUNTER_1(FuncELCOUNTER_type *progPnt, u32 ramPnt)
{
    f32 KPD, Pact, Ppotr_tek;
    u16 Pel_stop, base_kw;
    u32 P100wtemp, PwoFC;
    //u32   PwoFC;
    
    ELCOUNTER_State_type   *sPnt;
    ELCOUNTER_nvState_type  *nvAdr, nvMem;
    void                        *nvAdr1;

    sPnt = (ELCOUNTER_State_type *)(ramPnt + progPnt->Pnt_State);
    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvElCountero);
    nvAdr1 = (void *)((u32)&nv.file.ElCountero);

    if(flagIsFirstCall){            //начальная инициализация
        //восстановить счетчики из фрам
        RdFramMicro(&nvMem, nvAdr, sizeNvElCountero);
        sPnt->Pcnt100w = nvMem.Pcnt100w;
        sPnt->PcntMonth = nvMem.PcntMonth;
        sPnt->PcntYear = nvMem.PcntYear;
        sPnt->PrevMonth = nvMem.PrevMonth;
        sPnt->PrevYear = nvMem.PrevYear;
        
        sPnt->EcoMonth = nvMem.EcoMonth;
        sPnt->EcoYear = nvMem.EcoYear;
        sPnt->JrnNumRec = nvMem.JrnNumRec;
        sPnt->cnt1min = time1min;
        //восстановить структуру ElJRNo из фрам
        RdFramMicro(&sPnt->ElJRNo, nvAdr1, sizeof(ElcountJrn_type));
        return &progPnt->Pnt_End;
    }
    if (load_s16(ramPnt, progPnt->Pnt_Res)){
        sPnt->Pcnt_sum = 0;
        sPnt->Pcnt100w = sPnt->PcntMonth = sPnt->PcntYear = 0;
        nvMem.Pcnt100w = nvMem.PcntMonth = nvMem.PcntYear = 0;
        WrFramMicro(nvAdr,&nvMem, sizeNvElCountero);
        save_s16(ramPnt, progPnt->Pnt_Res, 0);
    }
    sPnt->cnt20ms++;
    sPnt->cnt1min--;
    if (sPnt->cnt20ms % 5 == 0){ //20 ms
        base_kw     = load_s16(ramPnt, progPnt->Pnt_BASE_kW);
        Pact        = _IQtoF(load_s32(ramPnt, progPnt->Pnt_Pact_in)) * base_kw;
        KPD         = _IQtoF(load_s32(ramPnt, progPnt->Pnt_KPD));
        Pel_stop    = load_s16(ramPnt, progPnt->Pnt_Pel_stop);
        Ppotr_tek   = Pact / KPD + (f32)Pel_stop/10;
        sPnt->Pcnt_sum  +=   Ppotr_tek / Ptime;
        if (sPnt->Pcnt_sum > P100W){ //инкрементируем счетчик по 100 Вт
            P100wtemp = (u32)(sPnt->Pcnt_sum / P100W);
            sPnt->Pcnt100w  += P100wtemp;
            sPnt->PcntMonth += P100wtemp;
            sPnt->PcntYear  += P100wtemp;
            sPnt->Pcnt_sum = sPnt->Pcnt_sum - P100wtemp * P100W;
        }
    }
    
    if(sPnt->PrevMonth != load_s16(ramPnt, progPnt->Pnt_Month)){
    	PwoFC = (u32)load_s32(ramPnt, progPnt->Pnt_PwoFC);
    	if(PwoFC < sPnt->PcntMonth){
    		sPnt->EcoMonth = 0;
    	}else{
            sPnt->EcoMonth = PwoFC - sPnt->PcntMonth;
    	}
        ///////////J
        sPnt->ElJRNo.HeadOfFileConsum.OfsLastItem = sPnt->JrnNumRec;
        sPnt->ElJRNo.ItemConsum[sPnt->JrnNumRec].Year = (u8)(sPnt->PrevYear);//load_s16(ramPnt, progPnt->Pnt_Year);
        sPnt->ElJRNo.ItemConsum[sPnt->JrnNumRec].Month = (u8)(sPnt->PrevMonth);//load_s16(ramPnt, progPnt->Pnt_Month);
        sPnt->ElJRNo.ItemConsum[sPnt->JrnNumRec].ConsumptionValue = sPnt->PcntMonth;
        sPnt->ElJRNo.HeadOfFileConsum.FileCrc = crcPacket((unsigned char *)&sPnt->ElJRNo.HeadOfFileConsum.OfsLastItem, sizeof(ElcountJrn_type) - 
                                                sizeof(sPnt->ElJRNo.HeadOfFileConsum.FileCrc));
        if (sPnt->JrnNumRec < (NUM_REC_JRN_ELCOUNTERO-1)){
            sPnt->JrnNumRec++;
        }
        else sPnt->JrnNumRec = 0;
        
        WrFramMicro( nvAdr1,&sPnt->ElJRNo,  sizeof(ElcountJrn_type));
        //////////////EOJ
        sPnt->PcntMonth = 0;
        sPnt->cnt1min = 0;
        sPnt->PrevMonth = load_s16(ramPnt, progPnt->Pnt_Month);
        if(sPnt->PrevYear != load_s16(ramPnt, progPnt->Pnt_Year)){
                sPnt->EcoYear = load_s32(ramPnt, progPnt->Pnt_PwoFC) * 12 - sPnt->PcntYear ;
                sPnt->PcntYear = 0;
                sPnt->cnt1min = 0;
                sPnt->PrevYear = load_s16(ramPnt, progPnt->Pnt_Year);
        }
    }
    
    

    if (sPnt->cnt1min <= 0){
        sPnt->cnt1min = time1min;
        //RdFramMicro(&nvMem, nvAdr, sizeNvElCountero);
        //if (nvMem.Pcnt100w != sPnt->Pcnt100w){
            nvMem.Pcnt100w      = sPnt->Pcnt100w;
            nvMem.PcntMonth     = sPnt->PcntMonth;
            nvMem.PcntYear      = sPnt->PcntYear;
            nvMem.PrevMonth     = sPnt->PrevMonth;
            nvMem.PrevYear      = sPnt->PrevYear;
            nvMem.EcoMonth      = sPnt->EcoMonth;
            nvMem.EcoYear       = sPnt->EcoYear;
            nvMem.JrnNumRec     = sPnt->JrnNumRec;
            WrFramMicro(nvAdr,&nvMem, sizeNvElCountero);
        //}
    }
    
    save_s32(ramPnt, progPnt->Pnt_Cnt, sPnt->Pcnt100w);
    save_s32(ramPnt, progPnt->Pnt_CntMonth, sPnt->PcntMonth);
    save_s32(ramPnt, progPnt->Pnt_CntYear, sPnt->PcntYear);
    save_s32(ramPnt, progPnt->Pnt_EcoMonth, sPnt->EcoMonth);
    save_s32(ramPnt, progPnt->Pnt_EcoYear, sPnt->EcoYear);
///////////////////////////true Economy//////////////////real////
    //PwoFC = load_s32(ramPnt, progPnt->Pnt_PwoFC);

    



    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncELCOUNTER_1[15]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Pact_in
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  KPD
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Pel_stop
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  PwoFC
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Year
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Month
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Min_Pulse
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Res
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  BASE_kW
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //10  EcoYear
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //11  EcoMonth
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Eco
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //13  CntYear
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //14  CntMonth
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //15  Cnt
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("ELCOUNTER", "DD", TblFuncELCOUNTER_1, FuncELCOUNTER_1, sizeof(TblFuncELCOUNTER_1), sizeof(ELCOUNTER_State_type), sizeof(ELCOUNTER_nvState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncELCOUNTER_1,
#endif
//********************************************************
