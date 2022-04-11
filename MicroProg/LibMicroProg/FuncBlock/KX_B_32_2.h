#ifdef    GET_FUNC_CODE

typedef struct{
    u16  Pnt_Xin;                               //11  Xin
    u16  Pnt_X1;                                //12  X1
    u16  Pnt_X2;                                //13  X2
    u16  Pnt_Y1;                                //14  Y1
    u16  Pnt_Y2;                                //15  Y2
}inPrm_type;
//********************************************************
typedef struct{
    u16  Pnt_Num;                       	//1  Num_A1
    u16  Pnt_Polar;                     	//2  Polar_A1
    u16  Pnt_Min;				//3  Min_A1
    u16  Pnt_Max;				//4  Max_A1
}selPrm_type;
//********************************************************
typedef struct{
    u16  		Pnt_Nfunc;
    selPrm_type	selPrm[2];
    u16  		Pnt_Out2;		//9  Out2
    u16  		Pnt_Out1;		//10 Out1
    inPrm_type	inPrm[32];                      //11
    u16  		Pnt_End;
}FuncKX_B_32_2_type;
//********************************************************
typedef enum {
	unipolar=0,
	bipolar,
}dacPolarity_enum;
//********************************************************
typedef enum {
    RefFreq_dest = 1,
    RefPID_dest,
    FeedbackPID_dest,
    FailPid_dest,
    OutFreq_dest,
    RotorFreq_dest,
    Ain1_dest,
    Ain2_dest,
    Ain3_dest,
    Ain4_dest,
    Ain5_dest,
    OutCurr_dest,
    OutVoltage_dest,
    Ud_dest,
    RefMoment_dest,
    CurrentMoment_dest,
    PowerAct_dest,
    PowerReact_dest,
    PowerCoeff_dest,
    AoutPID_dset,
    Encoder_dest
}destinationAout_enum;

//********************************************************
u16 *FuncKX_B_32_2_1(FuncKX_B_32_2_type *progPnt, u32 ramPnt)
{
    s16         	i, Num, Polar;
    float         	InData, X1, X2, Y1, Y2, Min, Max;
    float         	OutData[2];
    inPrm_type		*iPnt;
    selPrm_type		*sPnt;

    for(i = 0, sPnt = progPnt->selPrm; i < 2; i++, sPnt++){
        Num = load_s16(ramPnt, sPnt->Pnt_Num);                     // Cчитываем назначение первого аналогового выхода
        if((Num == 0) || (Num > 32)){             
            OutData[i] = 0.0f;                                     // Назначение аналогового выхода не выбрано                  
        }
        else{
            iPnt	= &(progPnt->inPrm[Num - 1]);
            Polar	= load_s16(ramPnt, sPnt->Pnt_Polar);
            InData	= load_float(ramPnt, iPnt->Pnt_Xin);
            if((Polar == unipolar) && (InData < 0.0f)){
               InData = 0;
            }
            else{
               // Получить нормировочные коеффициенты
               X1 = load_float(ramPnt, iPnt->Pnt_X1);
               X2 = load_float(ramPnt, iPnt->Pnt_X2);
               Y1 = load_float(ramPnt, iPnt->Pnt_Y1); 
               Y2 = load_float(ramPnt, iPnt->Pnt_Y2);
              
               // Нормирование входных данных
               InData= f32_Fy_x1x2y1y2x(X1, X2, Y1, Y2, InData);
            }
            
            // Проверим тип выхода
            if(Polar == bipolar){
               InData= (InData + 1.0f)/2.0f;           // Для "bipolar" 0-это середина шкалы Aout
            }
            
            // Масштабируем выходной сигнал к приделам Aout
            Min = load_float(ramPnt, sPnt->Pnt_Min);
            Max = load_float(ramPnt, sPnt->Pnt_Max);
            
            OutData[i]= f32_Fy_x1x2y1y2x(Min, Max, 0.0f, 1.0f, InData);
            
            if(OutData[i] < 0.f)
                OutData[i] = 0.f;
            if(OutData[i] > 1.0f )
                OutData[i] = 1.0f;
        }
    }	
    
    // Передаем в блок Perif задание для Aout
    save_float(ramPnt, progPnt->Pnt_Out1, OutData[0]);
    save_float(ramPnt, progPnt->Pnt_Out2, OutData[1]);

	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncKX_B_32_2_1[170]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Num_A1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Polar_A1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Min_A1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Max_A1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Num_A2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Polar_A2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Min_A2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Min_A2
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Out2
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //10 Out1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //23  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //24  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //25  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //26  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //27  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //28  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //29  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //30  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //31  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //32  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //33  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //34  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //35  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //36  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //37  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //38  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //39  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //40  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //41  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //42  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //43  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //44  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //45  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //46  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //47  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //48  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //49  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //50  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //51  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //52  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //53  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //54  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //55  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //56  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //57  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //58  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //59  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //60  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //61  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //62  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //63  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //64  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //65  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //66  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //67  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //68  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //69  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //70  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //71  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //72  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //73  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //74  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //75  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //76  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //77  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //78  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //79  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //80  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //81  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //82  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //83  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //84  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //85  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //86  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //87  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //88  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //89  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //90  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //91  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //92  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //93  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //94  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //95  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //96  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //97  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //98  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //99  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //100  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //101  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //102  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //103  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //104  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //105  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //106  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //107  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //108  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //109  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //110  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //111  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //112  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //113  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //114  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //115  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //116  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //117  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //118  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //119  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //120  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //121  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //122  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //123  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //124  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //125  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //126  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //127  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //128  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //129  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //130  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //131  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //132  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //133  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //134  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //135  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //136  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //137  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //138  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //139  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //140  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //141  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //142  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //143  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //144  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //145  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //146  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //147  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //148  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //149  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //150  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //151  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //152  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //153  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //154  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //155  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //156  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //157  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //158  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //159  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //160  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //161  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //162  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //163  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //164  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //165  Y2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //166  Xin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //167  X1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //168  X2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //169  Y1
        S32_PIN_TYPE | INPUT_PIN_MODE      //170  Y2
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("KX+B_32_2", "DA", TblFuncKX_B_32_2_1, FuncKX_B_32_2_1, sizeof(TblFuncKX_B_32_2_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncKX_B_32_2_1,
#endif
//********************************************************
