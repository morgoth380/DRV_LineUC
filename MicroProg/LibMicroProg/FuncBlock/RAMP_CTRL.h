#include    "GT_DEFs.h"

#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Tacc;                         //1  Tacc
    u16  Pnt_Tdec;                         //2  Tdec
    u16  Pnt_Tstop;                        //3  Tstop
    u16  Pnt_FrefNum;                      //4  FrefNum
    u16  Pnt_StateGT;                      //5  StateGT
    u16  Pnt_Start;                        //6  Start
    u16  Pnt_DecTime;                      //7  DecTime
    u16  Pnt_AccTime;                      //8  AccTime
    u16  Pnt_End;
}FuncRAMP_CTRL_type;
//********************************************************
typedef struct{  //!!! Не оптимально, ПЕРЕПИСАТЬ !!!
		u16			fReady2Stop;
		u16			fDecelerate;
}RAMPCTRL_State_type;
//********************************************************

u16 *FuncRAMP_CTRL_1(FuncRAMP_CTRL_type *progPnt, u32 ramPnt)
{
	RAMPCTRL_State_type		*sPnt;
	u16						tempTacc, tempTdec;

	sPnt 	 = (RAMPCTRL_State_type *)(ramPnt + progPnt->Pnt_State); 
	tempTacc = load_s16(ramPnt, progPnt->Pnt_Tacc);
	tempTdec = load_s16(ramPnt, progPnt->Pnt_Tdec);

	if(flagIsFirstCall == 1){ 
            sPnt->fReady2Stop = 0; 
            sPnt->fDecelerate = 0;
        }

	if(!testBitMicro(ramPnt, progPnt->Pnt_Start)){
            if((load_s16(ramPnt, progPnt->Pnt_FrefNum) - 16) == 1){  //см. DISKR_FUNC_V3.h - //Выбор направления приоритета
                if(load_s16(ramPnt, progPnt->Pnt_StateGT)&Dwn){
                        sPnt->fDecelerate = 1;
                }
                if((sPnt->fDecelerate==1) && (load_s16(ramPnt, progPnt->Pnt_StateGT)&EqRefVal)){
                    sPnt->fDecelerate = 0;
                    sPnt->fReady2Stop = 1;
                }
            }
            if((sPnt->fReady2Stop==1) && (load_s16(ramPnt, progPnt->Pnt_StateGT)&Dwn)){
                tempTdec = load_s16(ramPnt, progPnt->Pnt_Tstop);
            }
            if((load_s16(ramPnt, progPnt->Pnt_StateGT)&Up) && (sPnt->fDecelerate)){
                sPnt->fDecelerate = 0;
            }
            if((load_s16(ramPnt, progPnt->Pnt_StateGT)&RefZero) && (load_s16(ramPnt, progPnt->Pnt_StateGT)&EqRefVal)){
                    sPnt->fReady2Stop = 0; sPnt->fDecelerate = 0;
            }
	}
	else{ 
            sPnt->fReady2Stop = 0; 
            sPnt->fDecelerate = 0;
        }

	save_s16(ramPnt, progPnt->Pnt_AccTime, tempTacc);
	save_s16(ramPnt, progPnt->Pnt_DecTime, tempTdec);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRAMP_CTRL_1[8]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Tacc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Tdec
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Tstop
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  FrefNum
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  StateGT
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Start
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //7  DecTime
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //8  AccTime
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("RAMP_CTRL", "DA", TblFuncRAMP_CTRL_1, FuncRAMP_CTRL_1, sizeof(TblFuncRAMP_CTRL_1), sizeof(RAMPCTRL_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRAMP_CTRL_1,
#endif
//********************************************************
