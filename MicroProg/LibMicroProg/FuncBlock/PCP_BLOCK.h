#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PcpEn;                        //1  PcpEn
    u16  Pnt_IactZad;                      //2  IactZad
    u16  Pnt_KpUd;                         //3  KpUd
    u16  Pnt_KiUd;                         //4  KiUd
    u16  Pnt_PcpMode;                      //5  PcpMode
    u16  Pnt_Uvozb;                        //6  Uvozb
    u16  Pnt_Ffixed;                       //7  Ffixed
    u16  Pnt_Fcntrl;                       //8  Fcntrl
    u16  Pnt_UsclFiltr;                    //9  UsclFiltr
    u16  Pnt_dI_dt_Ireact;                 //10  dI_dt_Ireact
    u16  Pnt_ResOut3;                      //11  ResOut3
    u16  Pnt_ResOut2;                      //12  ResOut2
    u16  Pnt_ResOut1;                      //13  ResOut1
    u16  Pnt_End;
}FuncPCP_BLOCK_type;
//********************************************************
u16 *FuncPCP_BLOCK_1(FuncPCP_BLOCK_type *progPnt, u32 ramPnt)
{
    GlobalM3.Uvozb = (u16)( load_s16(ramPnt, progPnt->Pnt_Uvozb)) / 100.f; // Процент от задания намагничивания, при возврате двигателя на сеть
    GlobalM3.KiUd = ((u16)( load_s16(ramPnt, progPnt->Pnt_KiUd))) / 100000.f; //  Коэф. Ки для регулятора Ud
    GlobalM3.KpUd = ((s16)( load_s16(ramPnt, progPnt->Pnt_KpUd))) / 100000.f;  // Коэф. Кп для регулятора Ud
    GlobalM3.Iact_pcp = ( load_s16(ramPnt, progPnt->Pnt_IactZad)) / 1000.f; // Процент от номинального тока, для формирования задания тока в ген.режиме в скаляре, при спуске по частоте - PCP_SCALAR_PI_REG_F
    GlobalM3.Fcntrl = ( load_s16(ramPnt, progPnt->Pnt_Fcntrl)) / 100000.f; // Шаг по частоте (мин. 0,00001 Гц), при движении регулятора к генераторному режиму - PCP_SCALAR_PI_REG_F
    GlobalM3.Ffixed = ( load_s16(ramPnt, progPnt->Pnt_Ffixed)) / 100000.f; // Фиксированный Шаг по частоте (мин. 0,00001 Гц), при движении регулятора к генераторному режиму - PCP_SCALAR_PI_REG_F
    GlobalM3.UsclrFiltrN = load_s16(ramPnt, progPnt->Pnt_UsclFiltr); // Порядок фильтра для задания Ds в PCP Scalar
    GlobalM3.PcpMode = load_s16(ramPnt, progPnt->Pnt_PcpMode); // Режим работы в PCP Scalar: 0 - управление шагом частоты, 1 - пид-регулятор Ud
    GlobalM3.Ireact_di_dt = ( load_s16(ramPnt, progPnt->Pnt_dI_dt_Ireact)) / 1000.f; // di/dt в переходном режиме PCP SCALAR - ограничение роста для реактивного тока

    if ( load_s16(ramPnt, progPnt->Pnt_PcpEn) )
    	GlobalM3.FlagCommand.bit.PCPmodeEnable = 1;
    else
    	GlobalM3.FlagCommand.bit.PCPmodeEnable = 0;

#ifdef    _PROJECT_FOR_CCS_
    PhaseState.PCPmodeEnable = GlobalM3.FlagCommand.bit.PCPmodeEnable;
#endif
    
	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPCP_BLOCK_1[13]={
        S16_PIN_TYPE | INPUT_PIN_MODE,   //1  PcpEn
        S16_PIN_TYPE | INPUT_PIN_MODE,   //2  IactZad
        S16_PIN_TYPE | INPUT_PIN_MODE,   //3  KpUd
        S16_PIN_TYPE | INPUT_PIN_MODE,   //4  KiUd
        S16_PIN_TYPE | INPUT_PIN_MODE,   //5  PcpMode
        S16_PIN_TYPE | INPUT_PIN_MODE,   //6  Uvozb
        S16_PIN_TYPE | INPUT_PIN_MODE,   //7  Ffixed
        S16_PIN_TYPE | INPUT_PIN_MODE,   //8  Fcntrl
        S16_PIN_TYPE | INPUT_PIN_MODE,   //9  UsclFiltr
        S16_PIN_TYPE | INPUT_PIN_MODE,   //10  dI_dt_Ireact
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //11  ResOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //12  ResOut2
        S16_PIN_TYPE | OUTPUT_PIN_MODE    //13  ResOut1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PCP_BLOCK", "U", TblFuncPCP_BLOCK_1, FuncPCP_BLOCK_1, sizeof(TblFuncPCP_BLOCK_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPCP_BLOCK_1,
#endif
//********************************************************
