#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_INPUT1;                       //1  INPUT1
    u16  Pnt_INPUT2;                       //2  INPUT2
    u16  Pnt_INPUT3;                       //3  INPUT3
    u16  Pnt_INPUT4;                       //4  INPUT4
    u16  Pnt_INPUT5;                       //5  INPUT5
    u16  Pnt_INPUT6;                       //6  INPUT6
    u16  Pnt_INPUT7;                       //7  INPUT7
    u16  Pnt_INPUT8;                       //8  INPUT8
    u16  Pnt_INPUT9;                       //9  INPUT9
    u16  Pnt_INPUT10;                      //10  INPUT10
    u16  Pnt_INPUT11;                      //11  INPUT11
    u16  Pnt_INPUT12;                      //12  INPUT12
    u16  Pnt_INPUT13;                      //13  INPUT13
    u16  Pnt_INPUT14;                      //14  INPUT14
    u16  Pnt_INPUT15;                      //15  INPUT15
    u16  Pnt_INPUT16;                      //16  INPUT16
    u16  Pnt_INPUT17;                      //17  INPUT17
    u16  Pnt_INPUT18;                      //18  INPUT18
    u16  Pnt_INPUT19;                      //19  INPUT19
    u16  Pnt_INPUT20;                      //20  INPUT20
    u16  Pnt_OUTPUT1;                      //21  OUTPUT1
    u16  Pnt_OUTPUT2;                      //22  OUTPUT2
    u16  Pnt_OUTPUT3;                      //23  OUTPUT3
    u16  Pnt_OUTPUT4;                      //24  OUTPUT4
    u16  Pnt_OUTPUT5;                      //25  OUTPUT5
    u16  Pnt_OUTPUT6;                      //26  OUTPUT6
    u16  Pnt_OUTPUT7;                      //27  OUTPUT7
    u16  Pnt_OUTPUT8;                      //28  OUTPUT8
    u16  Pnt_OUTPUT9;                      //29  OUTPUT9
    u16  Pnt_OUTPUT10;                     //30  OUTPUT10
    u16  Pnt_OUTPUT11;                     //31  OUTPUT11
    u16  Pnt_OUTPUT12;                     //32  OUTPUT12
    u16  Pnt_End;
}FuncBLOK_SVP_type;


#define MODE_OFF  0
#define MODE_FAIL 1
#define MODE_WARN 2
//********************************************************
u16 *FuncBLOK_SVP_1(FuncBLOK_SVP_type *progPnt, u32 ramPnt)
{
u16                              mode;
//Анализ аварии обмена
	mode = load_s16(ramPnt, progPnt->Pnt_INPUT1);                          //86  авария обмена
 //GlobalM3.EnableConnectError=mode;
                 switch(mode)
	 			  {
	 			       case  MODE_OFF:
                        clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT1);  //138  avaria обмена
	 	                clrBitMicro(ramPnt, progPnt-> Pnt_OUTPUT2);                     //142 warning обмена
                       break;
	 				   case  MODE_FAIL:
	 					   if((testBitMicro(ramPnt, progPnt->Pnt_INPUT2)) !=0)
						   {
	 						   setBitMicro(ramPnt, progPnt->Pnt_OUTPUT1);  //138  avaria обмена
						   }
	 					   else
						   {
                               clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT1);  //138  avaria обмена
						   }
                               clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT2);                     //142 warning обмена
	 				   break;
					   case MODE_WARN:
						  if((testBitMicro(ramPnt, progPnt->Pnt_INPUT2)) !=0)
						   {
                               setBitMicro(ramPnt, progPnt->Pnt_OUTPUT2);                     //142 warning обмена
						   }
							else
							{  
								clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT2);                     //142 warning обмена
						    }
						      clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT1);  //138  avaria обмена
				       break;
					
				  }
//=======выбор алгоритма измерения Lm
	if(load_s16(ramPnt, progPnt->Pnt_INPUT3)==0)GlobalM3.FlagCommand.bit.MeasureLmRs=0;  //вы бор алгоритма измерения  LmRs
	else  GlobalM3.FlagCommand.bit.MeasureLmRs=1;

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBLOK_SVP_1[32]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  INPUT1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  INPUT2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  INPUT3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  INPUT4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  INPUT5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  INPUT6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  INPUT7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  INPUT8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  INPUT9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  INPUT10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  INPUT11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  INPUT12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  INPUT13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  INPUT14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  INPUT15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  INPUT16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  INPUT17
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  INPUT18
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  INPUT19
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  INPUT20

        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  OUTPUT1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  OUTPUT2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  OUTPUT3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  OUTPUT4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  OUTPUT5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  OUTPUT6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  OUTPUT7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  OUTPUT8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  OUTPUT9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  OUTPUT10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  OUTPUT11
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //32  OUTPUT12
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("BLOK_SVP", "DD", TblFuncBLOK_SVP_1, FuncBLOK_SVP_1, sizeof(TblFuncBLOK_SVP_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBLOK_SVP_1,
#endif
//********************************************************
//#ifdef   GET_FUNC_OPIS
////SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
//OpisFuncBlock("UNIT3", "U", TblFuncUNIT3_1, FuncUNIT3_1, sizeof(TblFuncUNIT3_1), sizeof(PICONTROL_State_type),sizeof(nvState_UNIT3_MODE_type), 1),
//#endif
////********************************************************
//#ifdef  GET_FUNC_PNT
//    (pFuncMicro_type)FuncUNIT3_1,
//#endif
