#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Kpr2;                         //1  Kpr2
    u16  Pnt_Kint2;                        //2  Kint2
    u16  Pnt_Kpr3;                         //3  Kpr3
    u16  Pnt_Kint3;                        //4  Kint3
    u16  Pnt_FiltrPr1;                     //5  FiltrPr1
    u16  Pnt_FiltrInt1;                    //6  FiltrInt1
    u16  Pnt_FiltrProp2;                   //7  FiltrProp2
    u16  Pnt_FiltrInt2;                    //8  FiltrInt2
    u16  Pnt_FiltrProp3;                   //9  FiltrProp3
    u16  Pnt_FiltrInt3;                    //10  FiltrInt3
    u16  Pnt_Zip1;                         //11  Zip1
    u16  Pnt_Zip2;                         //12  Zip2
    u16  Pnt_Zip3;                         //13  Zip3
    u16  Pnt_Zip4;                         //14  Zip4
    u16  Pnt_Out;                          //15  Out
    u16  Pnt_End;
}FuncKOEFF_RS_type;
//********************************************************
u16 *FuncKOEFF_RS_1(FuncKOEFF_RS_type *progPnt, u32 ramPnt)
{
	f32 Ptemp;
	//================коэфф рег скорости

//		if((GlobalC28.Wout >_IQabs(_IQmpy(GlobalM3.F_ZAD,_IQ(0.1))))&&(GlobalC28.Wout <_IQabs(_IQmpy(GlobalM3.F_ZAD,_IQ(0.6)))))
//		{
//
//		   GlobalM3.KpSpd2 = int2f(load_s16(ramPnt, progPnt->Pnt_Kpr2), d100);
//		 //  if(GlobalM3.KpSpd2)GlobalM3.KpSpd=_IQmpy(GlobalM3.KpSpd)
//		   Ptemp = int2f(load_s16(ramPnt, progPnt->Pnt_Kint2), d1000);
//		   GlobalM3.KiSpd2=GlobalM3.KpSpd2/Ptemp;
//		}
//		if(GlobalC28.Wout >_IQabs(_IQmpy(GlobalM3.F_ZAD,_IQ(0.6))))
//		{
//			GlobalM3.KpSpd2 = int2f(load_s16(ramPnt, progPnt->Pnt_Kpr3), d100);
//			 Ptemp = int2f(load_s16(ramPnt, progPnt->Pnt_Kint3), d1000);
//			 GlobalM3.KiSpd2=GlobalM3.KpSpd2/Ptemp;
//		}

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncKOEFF_RS_1[15]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Kpr2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Kint2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Kpr3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Kint3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  FiltrPr1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  FiltrInt1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  FiltrProp2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  FiltrInt2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  FiltrProp3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  FiltrInt3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Zip1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Zip2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Zip3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Zip4
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //15  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("KOEFF_RS", "DD", TblFuncKOEFF_RS_1, FuncKOEFF_RS_1, sizeof(TblFuncKOEFF_RS_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncKOEFF_RS_1,
#endif
//********************************************************
