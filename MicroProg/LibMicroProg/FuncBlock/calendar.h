#ifdef    GET_FUNC_CODE
typedef struct{
    u16  ref;
    u16  time;
}RefTime_type;

typedef struct{
    u16  Pnt_Nfunc;
    RefTime_type    RefTime[16];
    u16             Pnt_d[31];                        //33      d_1 d_31
    u16             Pnt_Enable;                       //64      Enable
    u16             Pnt_MinSutki;                     //65      MinSutki
    u16             Pnt_Date;                         //66      Date
    u16             Pnt_RefOut;                       //67      RefOut
    u16             Pnt_End;
}FuncCALENDAR_type;
//********************************************************
u16 *FuncCALENDAR_1(FuncCALENDAR_type *progPnt, u32 ramPnt)
{
    s16             tmp, point, tekTime, Ref;
    s16             timePred;
    s16             timeTmp;
    RefTime_type    *rtPnt;
    
    Ref = 0;
    if(load_s16(ramPnt, progPnt->Pnt_Enable)){
        tmp = load_s16(ramPnt, progPnt->Pnt_Date);
        tmp = load_s16(ramPnt, progPnt->Pnt_d[tmp - 1]);
        rtPnt = (tmp == 0) ? progPnt->RefTime : &progPnt->RefTime[8];
        
        timePred = 0;
        tekTime = load_s16(ramPnt, progPnt->Pnt_MinSutki);
        for(point = 0; point < 8; point++, rtPnt++){
            timeTmp = load_s16(ramPnt, rtPnt->time);
            if((tekTime > timePred)&&(tekTime < timeTmp)){
                break;
            }
            timePred = timeTmp;
        }
        if(point != 0)  Ref = load_s16(ramPnt, rtPnt[-1].ref);
    }
    save_s16(ramPnt, progPnt->Pnt_RefOut, Ref);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncCALENDAR_1[67]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Ref0    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Time0   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Ref1    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Time1   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Ref2    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Time2   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Ref3    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Time3   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Ref4    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10 Time4   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11 Ref5    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12 Time5   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13 Ref6    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14 Time6   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15 Ref7    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16 Time7   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17 Ref0    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18 Time0   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19 Ref1    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20 Time1   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21 Ref2    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22 Time2   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23 Ref3    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24 Time3   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25 Ref4    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26 Time4   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27 Ref5    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28 Time5   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29 Ref6    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30 Time6   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31 Ref7    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32 Time7   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33 d_1     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34 d_2     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35 d_3     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36 d_4     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37 d_5     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38 d_6     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //39 d_7     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //40 d_8     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41 d_9     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42 d_10    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43 d_11    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //44 d_12    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //45 d_13    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //46 d_14    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //47 d_15    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //48 d_16    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //49 d_17    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //50 d_18    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //51 d_19    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //52 d_20    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //53 d_21    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //54 d_22    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //55 d_23    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //56 d_24    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //57 d_25    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //58 d_26    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //59 d_27    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //60 d_28    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //61 d_29    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //62 d_30    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //63 d_31    
        S16_PIN_TYPE | INPUT_PIN_MODE,     //64 Enable  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //65 MinSutki
        S16_PIN_TYPE | INPUT_PIN_MODE,     //66 Date    
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //67 RefOut  
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("CALENDAR", "DA", TblFuncCALENDAR_1, FuncCALENDAR_1, sizeof(TblFuncCALENDAR_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncCALENDAR_1,
#endif
//********************************************************
