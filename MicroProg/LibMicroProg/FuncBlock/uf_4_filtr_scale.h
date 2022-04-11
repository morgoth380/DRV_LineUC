#ifdef    GET_FUNC_CODE
#define T_filtr  512
typedef struct{
    float   F4;
    float   F3;
    float   F2;
    float   F1;
    float   U4;
    float   U3;
    float   U2;
    float   U1;
}UF_4_FILTRState_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_F;                            //1  F
    u16  Pnt_F4;                           //2  F4
    u16  Pnt_F3;                           //3  F3
    u16  Pnt_F2;                           //4  F2
    u16  Pnt_F1;                           //5  F1
    u16  Pnt_U4;                           //6  U4
    u16  Pnt_U3;                           //7  U3
    u16  Pnt_U2;                           //8  U2
    u16  Pnt_U1;                           //9  U1
    u16  Pnt_Error;                        //10  Error
    u16  Pnt_U;                            //11  U
    u16  Pnt_Tfiltr;                       //12  PICKUP_ON
    u16  Pnt_Scale;                        //13  Scale
    u16  Pnt_End;
}FuncUF_4_FILTR_SCALE_type;
//********************************************************
u16 *FuncUF_4_FILTR_SCALE_1(FuncUF_4_FILTR_SCALE_type *progPnt, u32 ramPnt)
{
  
    UF_4_FILTRState_type    *sPnt;
    float                     F, U;
  #ifdef _PROJECT_FOR_STM32_
    sPnt = (UF_4_FILTRState_type *)(ramPnt + progPnt->Pnt_State);
    F =  fabsf(load_float(ramPnt, progPnt->Pnt_F));
////    Tfiltr = load_s16(ramPnt, progPnt->Pnt_Tfiltr);
    
//    if(FlagIsFirstCall()){            //начальная инициализация
//        Tfiltr = 0;
//    }
/*
    sPnt->F1 = iq_Filtr_prm(sPnt->F1, load_s32(ramPnt, progPnt->Pnt_F1), T_filtr);
    sPnt->F2 = iq_Filtr_prm(sPnt->F2, load_s32(ramPnt, progPnt->Pnt_F2), T_filtr);
    sPnt->F3 = iq_Filtr_prm(sPnt->F3, load_s32(ramPnt, progPnt->Pnt_F3), T_filtr);
    sPnt->F4 = iq_Filtr_prm(sPnt->F4, load_s32(ramPnt, progPnt->Pnt_F4), T_filtr);

    sPnt->U1 = iq_Filtr_prm(sPnt->U1, load_s32(ramPnt, progPnt->Pnt_U1), T_filtr);
    sPnt->U2 = iq_Filtr_prm(sPnt->U2, load_s32(ramPnt, progPnt->Pnt_U2), T_filtr);
    sPnt->U3 = iq_Filtr_prm(sPnt->U3, load_s32(ramPnt, progPnt->Pnt_U3), T_filtr);
    sPnt->U4 = iq_Filtr_prm(sPnt->U4, load_s32(ramPnt, progPnt->Pnt_U4), T_filtr);
*/
    
    sPnt->F1 = FiltrValue(T_filtr,load_float(ramPnt, progPnt->Pnt_F1), sPnt->F1);
    sPnt->F2 = FiltrValue(T_filtr,load_float(ramPnt, progPnt->Pnt_F2), sPnt->F2);
    sPnt->F3 = FiltrValue(T_filtr,load_float(ramPnt, progPnt->Pnt_F3), sPnt->F3);
    sPnt->F4 = FiltrValue(T_filtr,load_float(ramPnt, progPnt->Pnt_F4), sPnt->F4);
    
    sPnt->U1 = FiltrValue(T_filtr,load_float(ramPnt, progPnt->Pnt_U1), sPnt->U1);
    sPnt->U2 = FiltrValue(T_filtr,load_float(ramPnt, progPnt->Pnt_U2), sPnt->U2);
    sPnt->U3 = FiltrValue(T_filtr,load_float(ramPnt, progPnt->Pnt_U3), sPnt->U3);
    sPnt->U4 = FiltrValue(T_filtr,load_float(ramPnt, progPnt->Pnt_U4), sPnt->U4);

    if((sPnt->F4 < sPnt->F3)||(sPnt->F3 < sPnt->F2)||(sPnt->F2 < sPnt->F1)){
        save_float(ramPnt, progPnt->Pnt_U, 0);
        setBitMicro(ramPnt, progPnt->Pnt_Error);
    }

    else{
        if(F > sPnt->F3)        
            U = f32_Fy_x1x2y1y2x(sPnt->F3, sPnt->F4, sPnt->U3, sPnt->U4, F);
        else if(F > sPnt->F2)   
            U = f32_Fy_x1x2y1y2x(sPnt->F2, sPnt->F3, sPnt->U2, sPnt->U3, F);
        else if(F > sPnt->F1)   
            U = f32_Fy_x1x2y1y2x(sPnt->F1, sPnt->F2, sPnt->U1, sPnt->U2, F);
        else                    
            U = f32_Fy_x1x2y1y2x(0       , sPnt->F1, 0       , sPnt->U1, F);
/*        
////        Kscale = load_s32(ramPnt, progPnt->Pnt_Scale);
//
//       U = _IQmpy( U, Kscale );
//		if(_IQabs(U) > _IQ(1.0)){    
//			U = _IQ(1.0);
//		}

        mtoc.Utab = U;		// Для подхвата
        if(testBitMicro(ramPnt, progPnt->Pnt_Tfiltr))  	
            U = load_s32(ramPnt, progPnt->Pnt_Scale); 	// Работает подхват
*/
        save_float(ramPnt, progPnt->Pnt_U, U);
        clrBitMicro(ramPnt, progPnt->Pnt_Error);
    }
#endif
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUF_4_FILTR_SCALE_1[13]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  F4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  F3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  U4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  U3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  U2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  U1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Error
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //11  U
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  PICKUP_ON
        S32_PIN_TYPE | INPUT_PIN_MODE      //13  Scale
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UF_4_FILTR_SCALE", "DA", TblFuncUF_4_FILTR_SCALE_1, FuncUF_4_FILTR_SCALE_1, sizeof(TblFuncUF_4_FILTR_SCALE_1), sizeof(UF_4_FILTRState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUF_4_FILTR_SCALE_1,
#endif
//********************************************************
