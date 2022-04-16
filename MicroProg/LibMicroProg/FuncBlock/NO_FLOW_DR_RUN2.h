#ifdef    GET_FUNC_CODE

#define _OFF 			0
#define _ON_WARNING 	1
#define _ON_CRASH 		2
#define _ON_SLEEP 		3

#define _25PERCENT  0.25f		
#define _50PERCENT  0.5f
#define _75PERCENT  0.75f
#define LIM_PERCENT 0.001f  //допуск 0.1%
#define _00_DOT_1PERCENT   0.0001f
#define _100PERCENT 1.0f
#define BELOW_ZERO	-666

#define F_delta	(F_max - F_min)

typedef enum{
  ON_CMD_WAIT = 0,
  START_CMD_HOLD = 1
}DriveCmdStateType;

typedef struct{
    f32  AT_P1;
    f32  AT_P2;
    f32  AT_P3;
    f32  AT_P4;
    f32  AT_P5;	
}nvFuncNO_FLOW_DRY_RUN_State_type;

#define     nvSfFuncNO_FLOW_DRY_RUN_State_type (*(nvFuncNO_FLOW_DRY_RUN_State_type*)0)

typedef struct{
    f32 p1;
    f32 p2;
    f32 p3;
    f32 p4;
    f32 p5;
    u32 counter;
    u32 counter_i;      //i = idle = холостой ход
    u32 counter_sleep;  // спящий режим
    u16 blink_sleep;    // спящий режим
    u16 flag_sleep;
    u16 DriveOnCnt;
    DriveCmdStateType  DriveOnState;
    nvFuncNO_FLOW_DRY_RUN_State_type	nonvolatile_at;
}FuncNO_FLOW_DRY_RUN_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_F_TEK;                        //1  F_TEK
    u16  Pnt_P_ACT;                        //2  P_ACT
    u16  Pnt_Curve_P1;                     //3  Curve_P1
    u16  Pnt_Curve_P2;                     //4  Curve_P2
    u16  Pnt_Curve_P3;                     //5  Curve_P3
    u16  Pnt_Curve_P4;                     //6  Curve_P4
    u16  Pnt_Curve_P5;                     //7  Curve_P5
    u16  Pnt_DRV_Fmax;                     //8  DRV_Fmax
    u16  Pnt_DRV_Fmin;                     //9  DRV_Fmin
    u16  Pnt_NF_delay;                     //10  NF_delay
    u16  Pnt_DR_delay;                     //11  DR_delay
    u16  Pnt_PWM_ON;                       //12  PWM_ON
    u16  Pnt_Mode_NF;                      //13  Mode_NF
    u16  Pnt_AutoTuning;                   //14  AutoTuning
    u16  Pnt_Mode_DR;                      //15  Mode_DR
    u16  Pnt_AT_perc;                      //16  AT_perc
    u16  Pnt_T_sleep;                      //17  T_sleep
    u16  Pnt_DR_perc;                      //18  DR_perc
    u16  Pnt_FC_FAIL;                      //19  FC_FAIL
    u16  Pnt_DinNFstate;                   //20  DinNFstate
    u16  Pnt_DinDRstate;                   //21  DinDRstate
    u16  Pnt_In1;                          //22  In1
    u16  Pnt_In2;                          //23  In2
    u16  Pnt_NF_SBlink;                    //24  NF_SBlink
    u16  Pnt_Drive_On;                     //25  Drive_On
    u16  Pnt_DR_Warning;                   //26  DR_Warning
    u16  Pnt_DR_Failure;                   //27  DR_Failure
    u16  Pnt_Stop_Sleep;                   //28  Stop_Sleep
    u16  Pnt_NF_SleepW;                    //29  NF_SleepW
    u16  Pnt_NF_Warning;                   //30  NF_Warning
    u16  Pnt_NF_Failure;                   //31  NF_Failure
    u16  Pnt_End;
}FuncNO_FLOW_DR_RUN2_type;
//********************************************************
u16 *FuncNO_FLOW_DR_RUN2_1(FuncNO_FLOW_DR_RUN2_type *progPnt, u32 ramPnt)
{
  FuncNO_FLOW_DRY_RUN_State_type *sPnt;
  nvFuncNO_FLOW_DRY_RUN_State_type *nvAdr;  
  f32 F_tek, F_min, F_max;
  f32 P_min, P_act;
  u16 imode, mode, Autotuning;
  f32 noFlowFactor, DR_Factor;
  f32 lim_coeff;
  u16 onCmd;
  
  sPnt = (FuncNO_FLOW_DRY_RUN_State_type *)(ramPnt + progPnt->Pnt_State);
  nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfFuncNO_FLOW_DRY_RUN_State_type);	
  P_act = load_float(ramPnt, progPnt->Pnt_P_ACT);
  mode = load_s16(ramPnt, progPnt->Pnt_Mode_NF);
  imode = load_s16(ramPnt, progPnt->Pnt_Mode_DR);
  
  if (flagIsFirstCall){
          
    RdFramMicro(&sPnt->nonvolatile_at, nvAdr, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
    
    save_float(ramPnt, progPnt->Pnt_Curve_P1, sPnt->nonvolatile_at.AT_P1);				
    save_float(ramPnt, progPnt->Pnt_Curve_P2, sPnt->nonvolatile_at.AT_P2);
    save_float(ramPnt, progPnt->Pnt_Curve_P3, sPnt->nonvolatile_at.AT_P3);
    save_float(ramPnt, progPnt->Pnt_Curve_P4, sPnt->nonvolatile_at.AT_P4);
    save_float(ramPnt, progPnt->Pnt_Curve_P5, sPnt->nonvolatile_at.AT_P5);
    
    clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
    clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
    clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
    clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
    clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
    clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
    clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);
    clrBitMicro(ramPnt, progPnt->Pnt_NF_SBlink);
    
    sPnt->p1 = sPnt->nonvolatile_at.AT_P1;
    sPnt->p2 = sPnt->nonvolatile_at.AT_P2;
    sPnt->p3 = sPnt->nonvolatile_at.AT_P3;
    sPnt->p4 = sPnt->nonvolatile_at.AT_P4;
    sPnt->p5 = sPnt->nonvolatile_at.AT_P5;
    
    sPnt->flag_sleep = 0;
    sPnt->blink_sleep = 0;
    sPnt->counter = ( (s32) load_s16(ramPnt, progPnt->Pnt_NF_delay) * K_TIME_CALL_MICRO_TAKT);
    sPnt->counter_i = ( (s32) load_s16(ramPnt, progPnt->Pnt_DR_delay) * K_TIME_CALL_MICRO_TAKT);
    sPnt->counter_sleep  = load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT;
    sPnt->DriveOnState = ON_CMD_WAIT;
  }
  
  if(sPnt->p1 != load_float(ramPnt, progPnt->Pnt_Curve_P1)){
    sPnt->p1 = load_float(ramPnt, progPnt->Pnt_Curve_P1);
    sPnt->nonvolatile_at.AT_P1 = sPnt->p1;
    WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
  }	
  if(sPnt->p2 != load_float(ramPnt, progPnt->Pnt_Curve_P2)){
    sPnt->p2 = load_float(ramPnt, progPnt->Pnt_Curve_P2);
    sPnt->nonvolatile_at.AT_P2 = sPnt->p2;
    WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
  }	
  if(sPnt->p3 != load_float(ramPnt, progPnt->Pnt_Curve_P3)){
    sPnt->p3 = load_float(ramPnt, progPnt->Pnt_Curve_P3);
    sPnt->nonvolatile_at.AT_P3 = sPnt->p3;
    WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
  }	
  if(sPnt->p4 != load_float(ramPnt, progPnt->Pnt_Curve_P4)){
    sPnt->p4 = load_float(ramPnt, progPnt->Pnt_Curve_P4);
    sPnt->nonvolatile_at.AT_P4 = sPnt->p4;
    WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
  }	
  if(sPnt->p5 != load_float(ramPnt, progPnt->Pnt_Curve_P5)){
    sPnt->p5 = load_float(ramPnt, progPnt->Pnt_Curve_P5);
    sPnt->nonvolatile_at.AT_P5 = sPnt->p5;
    WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
  }
  
  onCmd = 0;
  if ((mode == _ON_SLEEP) && (sPnt->flag_sleep)){
    if(testBitMicro(ramPnt, progPnt->Pnt_FC_FAIL)){
        sPnt->flag_sleep = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
        clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
        clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
        clrBitMicro(ramPnt, progPnt->Pnt_NF_SBlink);
        sPnt->counter_sleep  =  (load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT);
    }else{
      if (sPnt->counter_sleep == 0){
        clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
        clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
        clrBitMicro(ramPnt, progPnt->Pnt_NF_SBlink);
        sPnt->flag_sleep = 0;
        onCmd = 1;
        sPnt->counter_sleep  =  (load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT);        
      }else{
        clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
        sPnt->counter_sleep--;
        if (!(sPnt->counter_sleep % 250)){
          sPnt->blink_sleep =~ sPnt->blink_sleep;
          defBitMicro(ramPnt, progPnt->Pnt_NF_SBlink,sPnt->blink_sleep);
        }
      }
    }
  }
  
  //formation of start command from sleep-mode
  switch(sPnt->DriveOnState){
  case ON_CMD_WAIT:
    if(onCmd == 1){
      setBitMicro(ramPnt, progPnt->Pnt_Drive_On);
      sPnt->DriveOnCnt = 0.2f * K_TIME_CALL_MICRO_TAKT;
      sPnt->DriveOnState = START_CMD_HOLD;
    }
    break;
  case START_CMD_HOLD:
    if(sPnt->DriveOnCnt == 0){
      clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);
      sPnt->DriveOnState = ON_CMD_WAIT;
    }else{
      sPnt->DriveOnCnt--;
    }
    break;
  }
  
  if(testBitMicro(ramPnt, progPnt->Pnt_PWM_ON)){
		
    if (sPnt->flag_sleep){
      sPnt->flag_sleep = 0;
      clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
      clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
      clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);
      clrBitMicro(ramPnt, progPnt->Pnt_NF_SBlink);
      sPnt->counter_sleep  =  (load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT);
    }
    
    Autotuning = load_s16(ramPnt, progPnt->Pnt_AutoTuning);
    F_tek = fabsf(load_float(ramPnt, progPnt->Pnt_F_TEK));
    F_max = load_float(ramPnt, progPnt->Pnt_DRV_Fmax);
    F_min = load_float(ramPnt, progPnt->Pnt_DRV_Fmin);
            
    if(Autotuning){		
      f32 Lim_Percent = F_max * LIM_PERCENT;	
      clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);	
      clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
      clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
      clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
      clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
      clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
      clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);
      
      if( (F_tek > (F_min - Lim_Percent)) && (F_tek < (F_min + Lim_Percent)) ){							
         //F_tek = F_min +- 0.1%F_max
         sPnt->nonvolatile_at.AT_P1 = P_act; 
      }
      else if( (F_tek > (F_delta * _25PERCENT + F_min - Lim_Percent)) && (F_tek < (F_delta * _25PERCENT + F_min + Lim_Percent)) ){
         //F_tek = 25%F_delta +- 0.1%F_max
         sPnt->nonvolatile_at.AT_P2 = P_act; 
      }
      else if( (F_tek > (F_delta * _50PERCENT + F_min - Lim_Percent)) && (F_tek < (F_delta * _50PERCENT + F_min + Lim_Percent)) ){	
          //F_tek = 50%F_delta +- 0.1%F_max
          sPnt->nonvolatile_at.AT_P3 = P_act;  
      }
      else if( (F_tek > (F_delta * _75PERCENT + F_min - Lim_Percent)) && (F_tek < (F_delta * _75PERCENT + F_min + Lim_Percent)) ){	
           //F_tek = 75%F_delta +- 0.1%F_max
           sPnt->nonvolatile_at.AT_P4 = P_act;  
      }
      else if( (F_tek > (F_max - Lim_Percent)) && (F_tek < (F_max + Lim_Percent)) ){							
          //F_tek = F_max +- 0.1%F_max
          sPnt->nonvolatile_at.AT_P5 = P_act; 
          WrFramMicro(nvAdr, &sPnt->nonvolatile_at, sizeof(nvFuncNO_FLOW_DRY_RUN_State_type));
          save_s16(ramPnt, progPnt->Pnt_AutoTuning, 0);
          save_float(ramPnt, progPnt->Pnt_Curve_P1, sPnt->nonvolatile_at.AT_P1);				
          save_float(ramPnt, progPnt->Pnt_Curve_P2, sPnt->nonvolatile_at.AT_P2);
          save_float(ramPnt, progPnt->Pnt_Curve_P3, sPnt->nonvolatile_at.AT_P3);
          save_float(ramPnt, progPnt->Pnt_Curve_P4, sPnt->nonvolatile_at.AT_P4);
          save_float(ramPnt, progPnt->Pnt_Curve_P5, sPnt->nonvolatile_at.AT_P5);
      }
      return &progPnt->Pnt_End;
    }
    //*********************************************************************************************
    if (F_tek >= (F_delta * _75PERCENT + F_min)){
      P_min = f32_Fy_x1x2y1y2x((F_delta * _75PERCENT + F_min),F_max,sPnt->p4, sPnt->p5, F_tek);
    }
    else if(F_tek >= (F_delta * _50PERCENT + F_min)){
      P_min = f32_Fy_x1x2y1y2x((F_delta * _50PERCENT + F_min),(F_delta * _75PERCENT + F_min),sPnt->p3, sPnt->p4, F_tek);
    }
    else if(F_tek >= (F_delta * _25PERCENT + F_min)){
      P_min = f32_Fy_x1x2y1y2x((F_delta * _25PERCENT + F_min),(F_delta * _50PERCENT + F_min),sPnt->p2, sPnt->p3, F_tek);
    }
    else if(F_tek >= F_min){
      P_min = f32_Fy_x1x2y1y2x(F_min,(F_delta * _25PERCENT + F_min),sPnt->p1, sPnt->p2, F_tek);
    }
    else{
      P_min = BELOW_ZERO;
    }
    
    //No-flow control
    lim_coeff = load_s16(ramPnt, progPnt->Pnt_AT_perc);
    noFlowFactor = lim_coeff * 0.01f;
    DR_Factor = load_s16(ramPnt, progPnt->Pnt_DR_perc);
    DR_Factor = DR_Factor * 0.01f;
    if ((P_act > P_min * DR_Factor) && (P_act < P_min * noFlowFactor)){
      if(mode > _OFF){
        if(sPnt->counter == 0){
          if (mode == _ON_WARNING ){
            clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
            clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);

            clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);	
            setBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
          }
          else if (mode == _ON_CRASH ){
            clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
            clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);

            clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);	
            setBitMicro(ramPnt, progPnt->Pnt_NF_Failure);         
          }
          else if (mode == _ON_SLEEP ){						
            clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
            clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);

            setBitMicro(ramPnt, progPnt->Pnt_NF_SleepW); 
            setBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
            sPnt->flag_sleep = 1;       
          }
        }
        else{
           sPnt->counter--;
        }
      }
      else{
          clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
          clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
          clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
          clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
          sPnt->counter	 = (s32) (load_s16(ramPnt, progPnt->Pnt_NF_delay) * K_TIME_CALL_MICRO_TAKT);
      }
    
      //****************************************************************************************************
    }else if(P_act <= P_min * DR_Factor){ //Dry-run control
      if ((F_tek == F_max) && (imode > _OFF)){
        if (sPnt->counter_i == 0){
          if(imode == _ON_WARNING){
             clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
             setBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
          }
          else{							
             clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
             setBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
          }
        }else{
          sPnt->counter_i--;
        }
      }else{
         clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
         clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
         sPnt->counter_i = (s16) (load_s16(ramPnt, progPnt->Pnt_DR_delay) * K_TIME_CALL_MICRO_TAKT);
      }
    }
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
        clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
        clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
        clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
        
        clrBitMicro(ramPnt, progPnt->Pnt_NF_SleepW);
        clrBitMicro(ramPnt, progPnt->Pnt_Stop_Sleep);
        clrBitMicro(ramPnt, progPnt->Pnt_Drive_On);
        
        sPnt->counter_i      =  (s16) (load_s16(ramPnt, progPnt->Pnt_DR_delay) * K_TIME_CALL_MICRO_TAKT);
        sPnt->counter        =  (s16) (load_s16(ramPnt, progPnt->Pnt_NF_delay) * K_TIME_CALL_MICRO_TAKT);
        sPnt->counter_sleep  =  (load_s16(ramPnt, progPnt->Pnt_T_sleep) * K_TIME_CALL_MICRO_TAKT);
    }	
    //**********************************************************************************************
  }else{
    clrBitMicro(ramPnt, progPnt->Pnt_NF_Warning);
    clrBitMicro(ramPnt, progPnt->Pnt_NF_Failure);
    clrBitMicro(ramPnt, progPnt->Pnt_DR_Warning);
    clrBitMicro(ramPnt, progPnt->Pnt_DR_Failure);
    sPnt->counter = (s16) (load_s16(ramPnt, progPnt->Pnt_NF_delay) * K_TIME_CALL_MICRO_TAKT);
  }
  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncNO_FLOW_DR_RUN2_1[31]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  F_TEK
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  P_ACT
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Curve_P1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Curve_P2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Curve_P3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Curve_P4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Curve_P5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  DRV_Fmax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  DRV_Fmin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  NF_delay
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  DR_delay
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  PWM_ON
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Mode_NF
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  AutoTuning
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Mode_DR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  AT_perc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  T_sleep
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  DR_perc
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //19  FC_FAIL
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  DinNFstate
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  DinDRstate
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  In2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //24  NF_SBlink
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  Drive_On
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  DR_Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //27  DR_Failure
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //28  Stop_Sleep
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //29  NF_SleepW
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //30  NF_Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //31  NF_Failure
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("NO_FLOW_DR_RUN2", "DA", TblFuncNO_FLOW_DR_RUN2_1, FuncNO_FLOW_DR_RUN2_1, sizeof(TblFuncNO_FLOW_DR_RUN2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncNO_FLOW_DR_RUN2_1,
#endif
//********************************************************
