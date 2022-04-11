#include    "GT_DEFs.h"
#ifdef    GET_FUNC_CODE


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In;                                //1  Fref
    u16  Pnt_TempUp;                            //2  TempUp
    u16  Pnt_TempDw;                            //3  TempDw
    u16  Pnt_ResVal;                            //4  Fpsk
    u16  Pnt_Freze;                             //5  Freze
    u16  Pnt_Res;                               //6  PWM_ON
    u16  Pnt_None2;                             //7  FPickUp            NotUsed   
    u16  Pnt_None3;                             //8  PickUpIsOn         NotUsed
    u16  Pnt_TimeFpusk;                         //9  TimeFpusk
    u16  Pnt_Status;                            //10  Status
    u16  Pnt_Out;                               //11  Fout
    u16  Pnt_None4;                             //12  DeltaF            NotUsed
    u16  Pnt_End;
}FuncTEMP_LIN_PICKUP_V2_type;

//u16 FormModeGT(FuncTEMP_LIN_PICKUP_V2_type *progPnt, u32 ramPnt);

//extern u16 ModeGT;
//********************************************************
u16 *FuncTEMP_LIN_PICKUP_V2_1(FuncTEMP_LIN_PICKUP_V2_type *progPnt, u32 ramPnt)
{
  /*
  #ifdef _PROJECT_FOR_STM32_
    TempAcsl_type   *sPnt;
    f32    	TempUp, TempDw;
    f32  	Fpsk,Fref,Fout,FrefAbs,FoutAbs;
    //s64		Tmp;
    s16     StateRefFlags,sign;

    sPnt = (TempAcsl_type *)(ramPnt + progPnt->Pnt_State);

    ModeGT = FormModeGT(progPnt, ramPnt);
    if(flagIsFirstCall){
    	sPnt->StateGT.all = 0;
    }
    StateRefFlags = 0;
    Fref   = load_float(ramPnt, progPnt->Pnt_In);                            //Задание для генератора темпа
    Fout   = load_float(ramPnt, progPnt->Pnt_Out);			        //Частота на выходе
    Fpsk   = load_float(ramPnt, progPnt->Pnt_ResVal);    	                //Частота пуска

                 
    TempUp = load_float(ramPnt, progPnt->Pnt_TempUp);	                //Темп разгона
    TempUp = TempUp * FLOAT_TIME_CALL_MICRO;
    
    TempDw = load_float(ramPnt, progPnt->Pnt_TempDw);		        //Темп торможения
    TempDw = TempDw * FLOAT_TIME_CALL_MICRO;

	sign = 0;
	if(Fout < 0) 
            sign = 1;
	if(Fref != sPnt->FrefSave)
            sPnt->TempS = 0;	                                // Изменилось задание
	sPnt->FrefSave = Fref;
	if(ModeGT == en_ModeGT_Rev) 
            Fref = Fpsk;			                // Для S-образн
	FrefAbs = fabsf(Fref);
	FoutAbs = fabsf(Fout);

//----------------------------------------------------
        switch(ModeGT){

        case	en_ModeGT_Reset:{
            Fout = 0;
            //StateRefFlags |= SRFLAG_RESET;                        // Генератор темпа в состоянии сброса
            sPnt->StateGT.Reset = 0;
        } break;
//----------------------------------------------------
    	case	en_ModeGT_Fpsk:{
            Fout = Fpsk;
            if(Fout > 0){
                sPnt->FpskTime = (load_s16(ramPnt, progPnt->Pnt_TimeFpusk) * K_TIME_CALL_MICRO_TAKT) / 10; //Время фиксации пусковой частоты
                ModeGT = en_ModeGT_FpskTime;
                if(Fref < 0) 
                    Fout = -Fout;
            }
            else{
                ModeGT = en_ModeGT_Go;
            }
    	} break;
//----------------------------------------------------
    	case    en_ModeGT_FpskTime:{
            if(sPnt->FpskTime == 0) {
               if (testBitMicro(ramPnt, progPnt->Pnt_Freze) == 0){
                 sPnt->StateGT.EquRV = 0;
                 ModeGT = en_ModeGT_Go;
            }
            else 
               sPnt->StateGT.EquRV = 1;
            }
            else{
                sPnt->FpskTime--;
                sPnt->StateGT.EquRV = 1;
            }
    	}break;
//----------------------------------------------------
    	case	en_ModeGT_Go:{
            if(Fref == Fout){
                if(fabsf(Fout) <= Fpsk){
                    break;
                }
                sPnt->StateGT.Dwn       = 0;
                sPnt->StateGT.Up        = 0;
                sPnt->StateGT.EqRefVal  = 1;
                break;
            }    // Вышел на задание
            if(FrefAbs > FoutAbs){		                // Разгон
                //StateRefFlags |= SRFLAG_UP;                     // Задатчик темпа работает на увеличение частоты
                sPnt->StateGT.Dwn       = 0;
                sPnt->StateGT.Up        = 1;
                sPnt->StateGT.EqRefVal  = 0;
                
                sPnt->TempS = TempUp;			        // Ограничение темпа
                Fout = FoutAbs + sPnt->TempS;
                if(Fout > FrefAbs)
                {
                    Fout = FrefAbs;
                    sPnt->TempS = 0;
                }
                if(Fref < 0) 
                    Fout = -Fout;
            }
            else{					        // Торможение
                //StateRefFlags |= SRFLAG_DOWN;                   // Задатчик темпа работает на уменьшение частоты
                sPnt->StateGT.Dwn       = 1;
                sPnt->StateGT.Up        = 0;
                sPnt->StateGT.EqRefVal  = 0;
                
                sPnt->TempS = TempDw;			        // Ограничение темпа
                Fout = FoutAbs - sPnt->TempS;
                if(Fout < 0) 
                    Fout = 0;
                if(sign) 
                    Fout = -Fout;
            }
    	} break;

    	default: break;
    }
//--------------------------------------------------------------

        save_s16(ramPnt, progPnt->Pnt_Status, sPnt->StateGT.all);
        save_float(ramPnt, progPnt->Pnt_Out, Fout);

#endif 
        */
    return &progPnt->Pnt_End;
};
//---------------------------------------------------------------
// Формирование состояний генератора темпа
//---------------------------------------------------------------
/*
u16 ModeGT;
u16 FormModeGT(FuncTEMP_LIN_PICKUP_V2_type *progPnt, u32 ramPnt)
{

  f32 Sign;

    if(testBitMicro(ramPnt, progPnt->Pnt_Res)   == 0) 	   
        return en_ModeGT_Reset;  //Состояние ШИМ. 0 - ШИМ отключен, 1 - ШИМ включен
    Sign = (load_float(ramPnt, progPnt->Pnt_In)) * (load_float(ramPnt, progPnt->Pnt_Out));

    if(ModeGT == en_ModeGT_Reset)  
        return en_ModeGT_Fpsk;   //Пришел Пуск
    if((Sign < 0 ) && (ModeGT == en_ModeGT_Go )) 
        return en_ModeGT_Rev; 	
    if((Sign >= 0) && (ModeGT == en_ModeGT_Rev)) 
        return en_ModeGT_Go; 	

    return ModeGT;	//Режим не изменился
}
*/
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_LIN_PICKUP_V2_1[12]={
            S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
            S32_PIN_TYPE | INPUT_PIN_MODE,     //2  TempUp
            S32_PIN_TYPE | INPUT_PIN_MODE,     //3  TempDw
            S32_PIN_TYPE | INPUT_PIN_MODE,     //4  ResVal
            BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Freze               Not used
            BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Res
            BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  FPickUp             Not used
            BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  PickUpIsOn          Not used
            S16_PIN_TYPE | INPUT_PIN_MODE,     //9  TimeFpusk
            S16_PIN_TYPE | OUTPUT_PIN_MODE,    //10 Status
            S32_PIN_TYPE | OUTPUT_PIN_MODE,    //11 Out
            BIT_PIN_TYPE | INPUT_PIN_MODE      //12 DeltaF              Not used
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEMP_LIN_PICKUP_V2", "DA", TblFuncTEMP_LIN_PICKUP_V2_1, FuncTEMP_LIN_PICKUP_V2_1, sizeof(TblFuncTEMP_LIN_PICKUP_V2_1), sizeof(TempAcsl_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_LIN_PICKUP_V2_1,
#endif
//********************************************************
