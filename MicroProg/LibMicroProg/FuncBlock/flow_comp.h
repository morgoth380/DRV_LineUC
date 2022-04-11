#ifdef    GET_FUNC_CODE

#define IQF_TO_HZF 0.00002384185791f		//(400 / 16777216)
#define PI_DEV4 0.78539816339f
#define BAR_TO_PASCAL 100000.0f
#define RATE1 0.1f
#define RATE2 0.01f
#define RATE3 0.001f

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_F_tek;                        //1  F_tek
    u16  Pnt_Pole_pairs;                   //2  Pole_pairs
    u16  Pnt_Fluid_density;                //3  Fluid_density
    u16  Pnt_Sensor_maxV;                  //4  Sensor_maxV
    u16  Pnt_Q_nominal;                    //5  Q_nominal
    u16  Pnt_F_nominal;                    //6  F_nominal
    u16	 Pnt_Tube_D;			   //7  Tube_D
    u16  Pnt_Comp_ON;                      //8  Comp_ON
    u16  Pnt_Flow_comp;                    //9  Flow_comp
    u16  Pnt_End;
}FuncFLOW_COMP_type;
//********************************************************
u16 *FuncFLOW_COMP_1(FuncFLOW_COMP_type *progPnt, u32 ramPnt)
{	
    s16   Fnom, Dtube, zp;
    float k, fluid_density, f_field, tube_d, tube_s, velocity, sensor_max, calc;
  
    Fnom = load_s16(ramPnt, progPnt->Pnt_F_nominal);
    Dtube = load_s16(ramPnt, progPnt->Pnt_Tube_D);	
      
    if((Fnom == 0)||(Dtube == 0)){
       return &progPnt->Pnt_End;
    }
	
    if(load_s16(ramPnt, progPnt->Pnt_Comp_ON)){     // Компенсация потока включена
		
       k = ((float)load_s16(ramPnt, progPnt->Pnt_Q_nominal)) * RATE2 / (float)Fnom;
       
       fluid_density = load_s16(ramPnt, progPnt->Pnt_Fluid_density);
       f_field = load_float(ramPnt, progPnt->Pnt_F_tek) * IQF_TO_HZF;
       zp = load_s16(ramPnt, progPnt->Pnt_Pole_pairs);
       tube_d = RATE2 * (float)load_s16(ramPnt, progPnt->Pnt_Tube_D);
       tube_s = tube_d * tube_d * PI_DEV4;  //(Pi*D^2)/4
       velocity = k * f_field /((float)zp * tube_s); 
       sensor_max = RATE2 * (float)load_s16(ramPnt, progPnt->Pnt_Sensor_maxV);
       calc = ((velocity * velocity) * fluid_density) / (2.0f * sensor_max * BAR_TO_PASCAL); //(p*V^2)/2
       save_float(ramPnt, progPnt->Pnt_Flow_comp, calc);
    }
    else{  // Компенсация потока отключена
       save_float(ramPnt, progPnt->Pnt_Flow_comp, 0.0f);	
    }
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFLOW_COMP_1[9]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  F_tek
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Pole_pairs
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Fluid_density
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Sensor_maxV
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Q_nominal
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  F_nominal
	S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Tube_D
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Comp_ON
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //9  Flow_comp
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FLOW_COMP", "DD", TblFuncFLOW_COMP_1, FuncFLOW_COMP_1, sizeof(TblFuncFLOW_COMP_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFLOW_COMP_1,
#endif
//********************************************************
