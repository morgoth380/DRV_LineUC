#ifdef    GET_FUNC_CODE

#include "GlobalVar.h"
#include "logger.h"

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Nvar1;                        //1  Nvar1
    u16  Pnt_Nvar2;                        //2  Nvar2
    u16  Pnt_Nvar3;                        //3  Nvar3
    u16  Pnt_Nvar4;                        //4  Nvar4
    u16  Pnt_Nvar5;                        //5  Nvar5
    u16  Pnt_Nvar6;                        //6  Nvar6
    u16  Pnt_Nvar7;                        //7  Nvar7
    u16  Pnt_Nvar8;                        //8  Nvar8
    u16  Pnt_Nvar9;                        //9  Nvar9
    u16  Pnt_LogTime;                      //10  Pnt_LogTime
    u16  Pnt_Npoint;                       //11  Npoint
    u16  Pnt_Type;                         //12  Type
    u16  Pnt_res12;                        //13  res12
    u16  Pnt_res11;                        //14  res11
    u16  Pnt_res10;                        //15  res10
    u16  Pnt_res9;                         //16  res9
    u16  Pnt_res8;                         //17  res8
    u16  Pnt_res7;                         //18  res7
    u16  Pnt_res6;                         //19  res6
    u16  Pnt_res5;                         //20  res5
    u16  Pnt_res4;                         //21  res4
    u16  Pnt_res3;                         //22  res3
    u16  Pnt_res2;                         //23  res2
    u16  Pnt_res1;                         //24  res1
    u16  Pnt_End;
}FuncLOGER_type;

//********************************************************
#ifdef _PROJECT_FOR_STM32_
void log4msFunc(void);
#endif

u16 *FuncLOGER_1(FuncLOGER_type *progPnt, u32 ramPnt){
	
    logSetup_type       *pntLogSetup;	
    //u32			nLogVar;
    //u16                 nPoint;
    u16                 activeMaxNvar, index, timeVal, ResetLogInd;   
    float               temp;    
	
    ResetLogInd = 0;
    pntLogSetup = &GlobalM4.logSetup;
    
    // 1) Сохраняем номера переменных
    pntLogSetup->dataPnt.n[0] = load_s16(ramPnt, progPnt->Pnt_Nvar1);
    pntLogSetup->dataPnt.n[1] = load_s16(ramPnt, progPnt->Pnt_Nvar2);
    pntLogSetup->dataPnt.n[2] = load_s16(ramPnt, progPnt->Pnt_Nvar3);
    pntLogSetup->dataPnt.n[3] = load_s16(ramPnt, progPnt->Pnt_Nvar4);
    pntLogSetup->dataPnt.n[4] = load_s16(ramPnt, progPnt->Pnt_Nvar5);
    pntLogSetup->dataPnt.n[5] = load_s16(ramPnt, progPnt->Pnt_Nvar6);
    pntLogSetup->dataPnt.n[6] = load_s16(ramPnt, progPnt->Pnt_Nvar7);
    pntLogSetup->dataPnt.n[7] = load_s16(ramPnt, progPnt->Pnt_Nvar8);
    pntLogSetup->dataPnt.n[8] = load_s16(ramPnt, progPnt->Pnt_Nvar9);

    activeMaxNvar = 1; // Делаем предварительную установку максимального числа переменных в 1, иначе ниже будет деление, и при нуле будет ошибка
    // Определяем максимальный номер переменной где указан график
    for (index = 0; index < 9; index++) {
      if (pntLogSetup->dataPnt.n[index]) {       
        activeMaxNvar = (index+1);
      }
    }
      
    /// Новый формат расчета параметров логера ****  
    // Проверка на изменение числа активных переменных
    if (pntLogSetup->NlogVar != activeMaxNvar) {
      ResetLogInd = 1; // Создаем запрос на сброс логера
    }      
    // Кол-во точек на 1 график   
    pntLogSetup->LogLen = (u16)(szExtRAMmemData / activeMaxNvar); 
    pntLogSetup->HOFlogFile.Sz = pntLogSetup->LogLen;    
    // Количество активных переменных
    pntLogSetup->NlogVar = activeMaxNvar; 
    pntLogSetup->HOFlogFile.Nvar = pntLogSetup->NlogVar;
    // Тип логера
    pntLogSetup->TypeLog = (typeLog_type)load_s16(ramPnt, progPnt->Pnt_Type); 	
    // Определяем степень децимации по заданному времени записи
    timeVal = (u16)(0.1f * ((f32)load_s16(ramPnt, progPnt->Pnt_LogTime))); 
    temp = ((f32)timeVal)/(f32)pntLogSetup->LogLen;
    temp = temp * (f32)GlobalM4.Fpwm;
    // Децимация логгера
    pntLogSetup->cmpPrescalLog1	 =  (u16)(temp) ; 
    pntLogSetup->HOFlogFile.Ntic = pntLogSetup->cmpPrescalLog1;          
    // Устанавливаем флаг сброса индекса логера
    pntLogSetup->ResetLogInd = ResetLogInd;
    //*********************************************

    if(PwmIsOn() == 1) {
      // Если ШИМ включен и выбран (режим ШИМ включен или Нет Аварии),
      // то мы заряжаем счетчик послезаписи      
      if (pntLogSetup->TypeLog == typeLog_PwmOn || pntLogSetup->TypeLog == typeLog_NoFail) {
        pntLogSetup->logEndTmr = load_s16(ramPnt, progPnt->Pnt_res2);
      } else { // Если другие режимы записи - то logEndTmr сбросить в ноль
        pntLogSetup->logEndTmr = 0;
      }
    } else if( pntLogSetup->logEndTmr != 0 ) {		
        pntLogSetup->logEndTmr--;
    }

	if (GlobalM4.Fpwm)
		pntLogSetup->HOFlogFile.TimeOneTick = (u16)(10000/(GlobalM4.Fpwm/1000.f)); 
    #ifdef _PROJECT_FOR_STM32_
    log4msFunc();
    #endif //_PROJECT_FOR_STM32_  
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLOGER_1[24]={
        S16_PIN_TYPE | INPUT_PIN_MODE,   //1  Nvar1
        S16_PIN_TYPE | INPUT_PIN_MODE,   //2  Nvar2
        S16_PIN_TYPE | INPUT_PIN_MODE,   //3  Nvar3
        S16_PIN_TYPE | INPUT_PIN_MODE,   //4  Nvar4
        S16_PIN_TYPE | INPUT_PIN_MODE,   //5  Nvar5
        S16_PIN_TYPE | INPUT_PIN_MODE,   //6  Nvar6
        S16_PIN_TYPE | INPUT_PIN_MODE,   //7  Nvar7
        S16_PIN_TYPE | INPUT_PIN_MODE,   //8  Nvar8
        S16_PIN_TYPE | INPUT_PIN_MODE,   //9  Nvar9
        S16_PIN_TYPE | INPUT_PIN_MODE,   //10  LogTime
        S16_PIN_TYPE | INPUT_PIN_MODE,   //11  Npoint
        S16_PIN_TYPE | INPUT_PIN_MODE,   //12  Type
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //13  res12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //14  res11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //15  res10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //16  res9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //17  res8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //18  res7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //19  res6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //20  res5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //21  res4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //22  res3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //23  res2
        S16_PIN_TYPE | OUTPUT_PIN_MODE    //24  res1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LOGER", "DD", TblFuncLOGER_1, FuncLOGER_1, sizeof(TblFuncLOGER_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLOGER_1,
#endif
//********************************************************
