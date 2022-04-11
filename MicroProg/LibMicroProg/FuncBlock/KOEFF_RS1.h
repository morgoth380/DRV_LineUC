#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Kpr0;                         //1  Kpr0
    u16  Pnt_KprW1;                        //2  KprW1
    u16  Pnt_KprW2;                        //3  KprW2
    u16  Pnt_KprMax;                       //4  KprMax
    u16  Pnt_Kint0;                        //5  Kint0
    u16  Pnt_KintW1;                       //6  KintW1
    u16  Pnt_KintW2;                       //7  KintW2
    u16  Pnt_KintWmax;                     //8  KintWmax
    u16  Pnt_W1;                           //9  W1
    u16  Pnt_W2;                           //10  W2
    u16  Pnt_FiltpKpr;                     //11  FiltpKpr
    u16  Pnt_FiltpKint;                    //12  FiltpKint
    u16  Pnt_Regim;                        //13  Regim
    u16  Pnt_AutoNast;                     //14  AutoNast
    u16  Pnt_Zip1;                         //15  Zip1
    u16  Pnt_Zip2;                         //16  Zip2
    u16  Pnt_Zip3;                         //17  Zip3
    u16  Pnt_Zip4;                         //18  Zip4
    u16  Pnt_KprOut;                       //19  KprOut
    u16  Pnt_KintOut;                      //20  KintOut
    u16  Pnt_ZipOut1;                      //21  ZipOut1
    u16  Pnt_ZipOut2;                      //22  ZipOut2
    u16  Pnt_ZipOut3;                      //23  ZipOut3
    u16  Pnt_ZipOut4;                      //24  ZipOut4
    u16  Pnt_End;
}FuncKOEFF_RS1_type;
#define REGIM_CONST   0
#define REGIM_FILTER  3
#define REGIM_LINE_K0_KMAX   1
#define REGIM_LINE_K0_K1_K2_KMAX   2
#define SECTOR0            0
#define SECTOR1            1
#define SECTOR2            2

//********************************************************
u16 *FuncKOEFF_RS1_1(FuncKOEFF_RS1_type *progPnt, u32 ramPnt)
{
	float  Ptemp; 
	float  deltaW_SECTOR, deltaK_SECTOR, Wtek, delta_Wtek;
	float  KzadPR[4], KzadINT[4];
	float  KpSpd2 = 1.0f, KiSpd2 = 1.0f; //KiSpd;
	float  KpSpd0 = 1.0f, KiSpd0 = 1.0f;
	u16    REGIM;
	u16    SECTOR;
	float  Wzad[4];


// W0=======(SECTOR=0)===============W1========(SECTOR=1)==================W2=======(SECTOR=2)============Wmax
	Wtek	= fabsf(GlobalM4.Fref); 
	Wzad[0]	= 0.0f;
	Wzad[1]	= (float)load_s16(ramPnt, progPnt->Pnt_W1);
	Wzad[2]	= (float)load_s16(ramPnt, progPnt->Pnt_W2);
	Wzad[3]	= GlobalM4.EnginParam.Wmax;

	KpSpd0 = (float)load_s16(ramPnt, progPnt->Pnt_Zip2) * 0.01f;
	KiSpd0 = (float)load_s16(ramPnt, progPnt->Pnt_Zip3) * 0.0001f;

	if(!KpSpd0  || !KiSpd0) {
	    KpSpd0 = KiSpd0 = 1.0f;
	}

	KzadPR[0] = ((float)load_s16(ramPnt, progPnt->Pnt_Kpr0) / 1000.0f) * KpSpd0;
	KzadPR[1] = ((float)load_s16(ramPnt, progPnt->Pnt_KprW1) / 1000.0f) * KpSpd0;
	KzadPR[2] = ((float)load_s16(ramPnt, progPnt->Pnt_KprW2) / 1000.0f) * KpSpd0;
	KzadPR[3] = ((float)load_s16(ramPnt, progPnt->Pnt_KprMax) / 1000.0f) * KpSpd0;

        // По факту, интеграл трогать не надо, он изменится за счет Кп на такую же величину.
	KzadINT[0] = KiSpd0;//KzadPR[0] / (((float)load_s16(ramPnt, progPnt->Pnt_Kint0) / 1000.0f) * KiSpd0);
	KzadINT[1] = KiSpd0;//KzadPR[1] / (((float)load_s16(ramPnt, progPnt->Pnt_KintW1) / 1000.0f) * KiSpd0);
	KzadINT[2] = KiSpd0;//KzadPR[2] / (((float)load_s16(ramPnt, progPnt->Pnt_KintW2) / 1000.0f) * KiSpd0);
	KzadINT[3] = KiSpd0;//KzadPR[3] / (((float)load_s16(ramPnt, progPnt->Pnt_KintWmax) / 1000.0f) * KiSpd0);

	if( Wtek > Wzad[1] ) {
           if( Wtek > Wzad[2] ){
              SECTOR = SECTOR2;
           }
	   else{ 
              SECTOR = SECTOR1;
           }
	} 
        else{
	      SECTOR = SECTOR0;
	}

	REGIM = load_s16(ramPnt, progPnt->Pnt_Regim);
	switch(REGIM)
	{
		case  REGIM_LINE_K0_K1_K2_KMAX:
			deltaW_SECTOR 	= Wzad[SECTOR+1] - Wzad[SECTOR]; // W1-W0
			delta_Wtek 		= (Wtek - Wzad[SECTOR]);
			if(deltaW_SECTOR != 0.0f){
			    deltaK_SECTOR = KzadPR[SECTOR + 1]  - KzadPR[SECTOR];                  // K1-K0
			    KpSpd2 = deltaK_SECTOR / deltaW_SECTOR * delta_Wtek + KzadPR[SECTOR];  //(K1-K0)/(W1-W0)*(Wtek-W0)+K0;
			    deltaK_SECTOR = KzadINT[SECTOR + 1] - KzadINT[SECTOR];                 // K1-K0
			    KiSpd2 = deltaK_SECTOR / deltaW_SECTOR * delta_Wtek + KzadINT[SECTOR]; //(K1-K0)/(W1-W0)*(Wtek-W0)+K0;
			}
			// Минимальная фильтрация коэф. КС
			GlobalM4.Filter_KprRS = 0.25f;
			GlobalM4.Filter_KintRS = 0.25f;
		break;
		case  REGIM_LINE_K0_KMAX:

			deltaW_SECTOR = Wzad[3];
			if(deltaW_SECTOR != 0.0f){
			    deltaK_SECTOR = KzadPR[3]  - KzadPR[0];                     // K1-K0
			    KpSpd2 = deltaK_SECTOR / deltaW_SECTOR * Wtek + KzadPR[0];  //(K1-K0)/(W1-W0)*(Wtek-W0)+K0;
			    deltaK_SECTOR = KzadINT[3] - KzadINT[0];                    // K1-K0
			    KiSpd2 = deltaK_SECTOR / deltaW_SECTOR * Wtek + KzadINT[0]; //(K1-K0)/(W1-W0)*(Wtek-W0)+K0;
			}
			// Минимальная фильтрация коэф. КС
			GlobalM4.Filter_KprRS = 0.25f;
			GlobalM4.Filter_KintRS = 0.25f;
		break;
		case  REGIM_CONST:
                 	 KpSpd2 = KpSpd0;
			 KiSpd2 = KiSpd0;
			 //KpSpd2 = KzadPR[0]; 
                         //KiSpd2 = KzadINT[0];
                  
                         // Минимальная фильтрация коэф. КС
			 GlobalM4.Filter_KprRS = 0.25f;
			 GlobalM4.Filter_KintRS = 0.25f;
		break;
		case  REGIM_FILTER:
			KpSpd2 = KzadPR[SECTOR];
			KiSpd2 = KzadINT[SECTOR];
			// Нормальная фильтрация коэф. КС
			GlobalM4.Filter_KprRS  = 1.0f / powf(2.0f, (float)load_s16(ramPnt, progPnt->Pnt_FiltpKpr));
			GlobalM4.Filter_KintRS = 1.0f / powf(2.0f, (float)load_s16(ramPnt, progPnt->Pnt_FiltpKint));
		break;
	}

	// Перенос из локальных переменных в глобальные
	GlobalM4.KpSpd2 =  KpSpd2;
	GlobalM4.KiSpd2 =  (KpSpd2/KiSpd2) * GlobalM4.Tspd; // Перевод интеграла с учетом времени дискретизации

        
        
        
        /*
        save_s16(ramPnt,progPnt->Pnt_KprOut, (s16)(GlobalM4.KpSpd2 * 100.0f)); //проп
	save_s16(ramPnt,progPnt->Pnt_KintOut, (s16)(GlobalM4.KiSpd2 * 1000000.0f)); //инт
        */
        
	save_s16(ramPnt,progPnt->Pnt_KprOut, (s16)(KpSpd2 * 100.0f)); //проп
	if(KiSpd2 != 0.0f){
	   Ptemp = KpSpd0 / KiSpd2;
	}
        else{
	   Ptemp = 0.0f;
	}

	save_s16(ramPnt,progPnt->Pnt_KintOut, (s16)(Ptemp * 10000.0f)); //инт

	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncKOEFF_RS1_1[24]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Kpr0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  KprW1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  KprW2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  KprMax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Kint0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  KintW1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  KintW2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  KintWmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  W1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  W2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  FiltpKpr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  FiltpKint
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Regim
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  AutoNast
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Zip1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Zip2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Zip3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Zip4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //19  KprOut
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //20  KintOut
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //21  ZipOut1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //22  ZipOut2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  ZipOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //24  ZipOut4
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("KOEFF_RS1", "U", TblFuncKOEFF_RS1_1, FuncKOEFF_RS1_1, sizeof(TblFuncKOEFF_RS1_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncKOEFF_RS1_1,
#endif
//********************************************************
