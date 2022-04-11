#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PZDIgroup1;                   //1  PZDIgroup1
    u16  Pnt_PZDInum1;                     //2  PZDInum1
    u16  Pnt_PZDIgroup2;                   //3  PZDIgroup2
    u16  Pnt_PZDInum2;                     //4  PZDInum2
    u16  Pnt_PZDIgroup3;                   //5  PZDIgroup3
    u16  Pnt_PZDInum3;                     //6  PZDInum3
    u16  Pnt_PZDIgroup4;                   //7  PZDIgroup4
    u16  Pnt_PZDInum4;                     //8  PZDInum4
    u16  Pnt_PZDIgroup5;                   //9  PZDIgroup5
    u16  Pnt_PZDInum5;                     //10  PZDInum5
    u16  Pnt_PZDIgroup6;                   //11  PZDIgroup6
    u16  Pnt_PZDInum6;                     //12  PZDInum6
    u16  Pnt_PZDIgroup7;                   //13  PZDIgroup7
    u16  Pnt_PZDInum7;                     //14  PZDInum7
    u16  Pnt_PZDOgroup1;                   //15  PZDOgroup1
    u16  Pnt_PZDOnum1;                     //16  PZDOnum1
    u16  Pnt_PZDOgroup2;                   //17  PZDOgroup2
    u16  Pnt_PZDOnum2;                     //18  PZDOnum2
    u16  Pnt_PZDOgroup3;                   //19  PZDOgroup3
    u16  Pnt_PZDOnum3;                     //20  PZDOnum3
    u16  Pnt_PZDOgroup4;                   //21  PZDOgroup4
    u16  Pnt_PZDOnum4;                     //22  PZDOnum4
    u16  Pnt_PZDOgroup5;                   //23  PZDOgroup5
    u16  Pnt_PZDOnum5;                     //24  PZDOnum5
    u16  Pnt_PZDOgroup6;                   //25  PZDOgroup6
    u16  Pnt_PZDOnum6;                     //26  PZDOnum6
    u16  Pnt_PZDOgroup7;                   //27  PZDOgroup7
    u16  Pnt_PZDOnum7;                     //28  PZDOnum7
    u16  Pnt_End;
}FuncPZDI_PZDO_type;

#ifdef   _PROJECT_FOR_CCS_
extern PzdiPzdo_Type PzdiPzdo;
#endif

//********************************************************
u16 *FuncPZDI_PZDO_1(FuncPZDI_PZDO_type *progPnt, u32 ramPnt)
{
	#ifdef   _PROJECT_FOR_CCS_
	PzdiPzdo.PZDIadr1 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDIgroup1)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDInum1)));
	PzdiPzdo.PZDIadr2 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDIgroup2)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDInum2)));
	PzdiPzdo.PZDIadr3 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDIgroup3)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDInum3)));
	PzdiPzdo.PZDIadr4 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDIgroup4)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDInum4)));
	PzdiPzdo.PZDIadr5 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDIgroup5)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDInum5)));
	PzdiPzdo.PZDIadr6 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDIgroup6)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDInum6)));
	PzdiPzdo.PZDIadr7 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDIgroup7)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDInum7)));
	PzdiPzdo.PZDOadr1 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDOgroup1)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDOnum1)));
	PzdiPzdo.PZDOadr2 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDOgroup2)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDOnum2)));
	PzdiPzdo.PZDOadr3 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDOgroup3)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDOnum3)));
	PzdiPzdo.PZDOadr4 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDOgroup4)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDOnum4)));
	PzdiPzdo.PZDOadr5 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDOgroup5)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDOnum5)));
	PzdiPzdo.PZDOadr6 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDOgroup6)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDOnum6)));
	PzdiPzdo.PZDOadr7 = (s16)((load_s16(ramPnt, progPnt->Pnt_PZDOgroup7)<<7)+(load_s16(ramPnt, progPnt->Pnt_PZDOnum7)));
	#endif
    return &progPnt->Pnt_End;
};

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPZDI_PZDO_1[28]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  PZDIgroup1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  PZDInum1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  PZDIgroup2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  PZDInum2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  PZDIgroup3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  PZDInum3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  PZDIgroup4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  PZDInum4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  PZDIgroup5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  PZDInum5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  PZDIgroup6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  PZDInum6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  PZDIgroup7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  PZDInum7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  PZDOgroup1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  PZDOnum1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  PZDOgroup2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  PZDOnum2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  PZDOgroup3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  PZDOnum3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  PZDOgroup4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  PZDOnum4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  PZDOgroup5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  PZDOnum5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  PZDOgroup6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  PZDOnum6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  PZDOgroup7
        S16_PIN_TYPE | INPUT_PIN_MODE      //28  PZDOnum7
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PZDI_PZDO", "U", TblFuncPZDI_PZDO_1, FuncPZDI_PZDO_1, sizeof(TblFuncPZDI_PZDO_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPZDI_PZDO_1,
#endif
//********************************************************
