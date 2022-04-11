#ifdef _PROJECT_FOR_STM32_
#include <stdbool.h>        
#endif

#include "GlobalVar.h"

#ifdef    GET_FUNC_CODE

#ifndef _PROJECT_FOR_STM32_
u16 CodChangeUst = 0;
#else
extern u16 CodChangeUst;
#endif

//extern u16     MODBUS_FILE_RDWR;
//extern u16     MODBUS_PWM_ON;


typedef struct{
    u16  Code;
    u16  NumberUst;
    u16  TimeOut;
    u16  ResetRecoveryFlag;
    u16  Param;            //ДОБАВЛЕНО текущий номер параметра
    u16  TimeOutDef;       //ДОБАВЛЕНО
    u16  AT45toFRAMbusy;   //ДОБАВЛЕНО
    u16  AT45to;           //ДОБАВЛЕНО
    u16  TSK_uart_Priority;
    u16  TSK_MasterUart_Priority;
}FACTORY_SET_State_type;

/**Входные параметры функционального блока**/
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_NUMB;                         //1 NUMB
    u16  Pnt_G_def;                        //2 Номер группы, сбрасываемой в значения по умолчанию
    u16  Pnt_PWM;                          //3 PWM
    u16  Pnt_End;
}FuncFACTORY_SET_type;
//********************************************************
u16 *FuncFACTORY_SET_1(FuncFACTORY_SET_type *progPnt, u32 ramPnt)
{
//#ifdef _PROJECT_FOR_STM32_
    s16 						Tmp = 0, UstRecoveryFlagValue=1;
    u32 						adrAt45;
    void						*nvAdr1;
    FACTORY_SET_State_type		*sPnt;
//*************************
	u16 Group = 0;    //ДОБАВЛЕНО
//*************************
#define NOGROUP 99                                //Константа, обозначающая отсутствие заданного номера группы для сброса
    nvAdr1 = (void *)((u32)&nv.UstRecoveryFlag);


#ifdef _PROJECT_FOR_STM32_
    /*bool*/ u16(*ptrFunc)(u32 Adr, u16 size);
#endif

    sPnt = (FACTORY_SET_State_type *)(ramPnt + progPnt->Pnt_State); //sPnt - указатель на статическую область памяти

    if(flagIsFirstCall){
    	save_s16(ramPnt, progPnt->Pnt_G_def, NOGROUP);//NOGROUP (число 99) означает, что никакую группу сбрасывать не нужно (иначе, при считывании, значение 0 интерпретировалось бы как сигнал на сброс нулевой группы)
    	return &progPnt->Pnt_End;
    }

    // Счетчик блокировки работы блоков расширения если идут запросы по Модбас к файлам
    //if (MODBUS_FILE_RDWR)       MODBUS_FILE_RDWR --;
    
    if(sPnt->ResetRecoveryFlag == 0){
        UstRecoveryFlagValue = 0;
        WrFramMicro( nvAdr1, &UstRecoveryFlagValue,  sizeof(u16));
        sPnt->ResetRecoveryFlag = 1;
        //GlobalM3.UstRecoveryFlag = 1;
    }

    Tmp =  load_s16(ramPnt, progPnt->Pnt_NUMB);

    if(testBitMicro(ramPnt, progPnt->Pnt_PWM)){
        if(Tmp == 0) {return &progPnt->Pnt_End;}
        else if((Tmp != 1) && (Tmp != 2)){
        	sPnt->Code = 0;
            save_s16(ramPnt, progPnt->Pnt_NUMB, sPnt->Code);             //Уставка в положение "ничего"
            return &progPnt->Pnt_End;
        }
    }

    if((Tmp > 0) && (Tmp < 6) && (sPnt->Code == 0)){         //Задание на изменение уставок, 10 - ожидание
        sPnt->Code = 1;                                      //Код запуска изменения уставок
        sPnt->TimeOut = 0;
        sPnt->NumberUst = Tmp;
        CodChangeUst = 0;
    }

    if(sPnt->Code == 1){                                    //Запуск изменения уставок
        switch (sPnt->NumberUst){
//            case 1: adrAt45 = (u32)&spfl.fileMicroProg1; Tmp = 1; break;//At45<-Fram набор 1
            case 1: adrAt45 = (u32)&spfl.fileMicroProg2; Tmp = 1; break;//At45<-Fram набор 2
            case 2: adrAt45 = (u32)&spfl.fileMicroProg3; Tmp = 1; break;//At45<-Fram набор 3
            case 3: adrAt45 = (u32)&spfl.fileMicroProg1;          break;//Fram<-At45 набор 1
            case 4: adrAt45 = (u32)&spfl.fileMicroProg2;          break;//Fram<-At45 набор 2
            case 5: adrAt45 = (u32)&spfl.fileMicroProg3;          break;//Fram<-At45 набор 3
            default:                                                //Не верный выбор задания
                sPnt->Code = 0; 
                save_s16(ramPnt, progPnt->Pnt_NUMB, sPnt->Code);             //Уставка в положение "ничего"
                break;
        }
    }

    if(sPnt->Code == 1){
        #ifdef _PROJECT_FOR_STM32_
        if (Tmp == 1)//Задача Fram->At45
            ptrFunc = &WrAt45FromFram;
        else                                                  //Задача At45->Fram
            ptrFunc = &RdAt45ToFram;
        #endif

        #ifdef _PROJECT_FOR_STM32_
        if (!ptrFunc(adrAt45, SZ_MICROPROG_DATA)){
        #else
        if (0){
        #endif
            if(sPnt->TimeOut > 10){
              save_s16(ramPnt, progPnt->Pnt_NUMB, 7);           //Ошибка, превышен таймаут ожидания
              sPnt->Code = 0;
            }
            else 
              sPnt->TimeOut ++;
        }
        else 
          sPnt->Code = 2;                                  //Задача поставлена
    }//if (sPnt->Code != 0)
    else{
    #ifdef _PROJECT_FOR_STM32_
        if (sPnt->Code == 2){
    #else
        if (1){
    #endif
            if(CodChangeUst == 1 || CodChangeUst == 2)          //Задача произведена успешно
            {       
                sPnt->Code = 0;
                save_s16(ramPnt, progPnt->Pnt_NUMB, 0);          //Возвращаем код выполнения операции
                if (CodChangeUst == 1)
                {
                    //CodChangeUst = 0xAA;          //Переинициализация программы
                    WrFramMicro( nvAdr1, &UstRecoveryFlagValue, sizeof(u16));
                    GlobalM4.CPU_restart = 1;   //RESTART CPU
                }
                else CodChangeUst = 0;
            }
            else if(CodChangeUst == 3)
            {
            	sPnt->Code = 0;
            	save_s16(ramPnt, progPnt->Pnt_NUMB, 6);        //Возвращаем код ошибки
            	CodChangeUst = 0;
            }
        }
    }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Сброс на стандартные настройки по группам
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Group = load_s16(ramPnt, progPnt->Pnt_G_def); //Читаем номер групы для сброса в заводские настройки
	if(Group != NOGROUP)                          //Если передан номер сбрасываемой группы (поступило число меньше, чем 99)
	{
		if(!sPnt->AT45toFRAMbusy)                 //Проверяем, идет ли в данный момент процесс сброса какой-либо группы. если нет..
		{
#ifdef _PROJECT_FOR_STM32_                  //Без #ifdef вижуал студия при компиляции формирует ошибку
                  ptrFunc = &GrupOneAt45ToFram; //Указатель на функцию подготовки посылки
                  ptrFunc(Group, 0);            //Подготавливаем посылку. Как аргумент передаем номер группы (0 не используется). После этого в задачу в TSK_mcbsp передастся посылка с номером сбрасываемой группы
#endif
                  sPnt->AT45toFRAMbusy = 1;     //Установить флаг запуска процеса записи уставок.
		}

		if(CodChangeUst == 1)            //Если уставки успешно записаны
		{
                  sPnt->AT45toFRAMbusy = 0;    //Сбросить флаг процеса записи уставок
                  save_s16(ramPnt, progPnt->Pnt_G_def, NOGROUP);
                  CodChangeUst = 0;
                  
                  WrFramMicro( nvAdr1, &UstRecoveryFlagValue, sizeof(u16));
                  GlobalM4.CPU_restart = 1;   //RESTART CPU
		}

		else if(CodChangeUst == 3)   //ОШИБКА Уставки не записаны!
		{
			save_s16(ramPnt, progPnt->Pnt_G_def, NOGROUP);
			save_s16(ramPnt, progPnt->Pnt_NUMB, 6);        //Возвращаем код ошибки
			CodChangeUst = 0;

			sPnt->AT45toFRAMbusy = 0; //Сбросить флаг процеса записи уставок
		}
	}


//===============================================================================
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFACTORY_SET_1[3]={
		S16_PIN_TYPE | INPUT_PIN_MODE,     //1  NUMB
		S16_PIN_TYPE | INPUT_PIN_MODE,
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  NUMB
        };
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FACTORY_SET", "DD", TblFuncFACTORY_SET_1, FuncFACTORY_SET_1, sizeof(TblFuncFACTORY_SET_1), sizeof(FACTORY_SET_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFACTORY_SET_1,
#endif
//********************************************************