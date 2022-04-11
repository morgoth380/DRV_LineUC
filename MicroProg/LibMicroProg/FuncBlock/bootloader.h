#ifdef    GET_FUNC_CODE

#include "GlobalVar.h"

// static
typedef struct{
	u16 Pass;
        u16 State;
    
}BOOTLOADER_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_PassWord;                     //1  PassWord
    u16  Pnt_res1;                         //2  res1
    u16  Pnt_En;                           //3  En
    u16  Pnt_res2;                         //4  res2
    u16  Pnt_res3;                         //5  res3
    u16  Pnt_res4;                         //6  res4
    u16  Pnt_End;
}FuncBOOTLOADER_type;
//********************************************************

//********************************************************
#define FIRST_PASS     		 		0xAA55
#define SECOND_PASS     			0x55AA
#define FROM_MAINPROG			   	0x203040AA

extern BootCommonData_type BootCommonData;

u16 *FuncBOOTLOADER_1(FuncBOOTLOADER_type *progPnt, u32 ramPnt)
{
 
  BOOTLOADER_State_type         *sPnt;
  
  sPnt = (BOOTLOADER_State_type *)(ramPnt + progPnt->Pnt_State);

  if(((GlobalM4.FlagCommand.bit.PwmOn) || (GlobalM4.FlagCommand.bit.mWorkEnb)))
  {
      sPnt->Pass = 0;
      sPnt->State = 0;
      return &progPnt->Pnt_End;
  }
  // 1) Получение запроса на переход в загрузчик
  sPnt->Pass = load_s16(ramPnt, progPnt->Pnt_PassWord);

  BootCommonData.FlagFromMainPO = 0; 
  
  // 2) Аппарат состояний
  switch(sPnt->State){
     case 0:{
        if(sPnt->Pass == FIRST_PASS){
            sPnt->State = 1;                                   // Первый этап проверки пройден
        }
        else{
            save_s16(ramPnt, progPnt->Pnt_PassWord, 0);  // Первый этап проверки не пройден!!!
        }
     } break;
     case 1:{
        if(sPnt->Pass == FIRST_PASS){
            // ни чего не делаем ждем второй пароль
        }
        else if(sPnt->Pass == SECOND_PASS){
	    if(BootCommonData.CrcBoot == (u16)(~BootCommonData.invCrcBoot)){    // проверка наличия загрузчика
	        
                BootCommonData.FlagFromMainPO = FROM_MAINPROG;                  // Установить флаг что пришли с основного ПО 	
               
                // Переход в загрузчик
                #ifdef _PROJECT_FOR_STM32_
                   osDelay(4);                                                     // Чтоб успел уйти ответ на запись параметра
                   HAL_DeInit();               // Сбросить всю переферию 
                   HAL_NVIC_SystemReset();     // RESTART CPU  
                #endif //_PROJECT_FOR_STM32_
                
            }
            else{
                save_s16(ramPnt, progPnt->Pnt_PassWord, 0);
                sPnt->State = 0; // Не получили правильное подтверждение переходим в режим ожидания
            }
        }
     } break;
        default:
            save_s16(ramPnt, progPnt->Pnt_PassWord, 0);
            sPnt->State = 0;
            break;
    }
 
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBOOTLOADER_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  PassWord
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  res1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  En
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  res2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  res3
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //6  res4
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("BOOTLOADER", "DD", TblFuncBOOTLOADER_1, FuncBOOTLOADER_1, sizeof(TblFuncBOOTLOADER_1), sizeof(BOOTLOADER_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBOOTLOADER_1,
#endif
//********************************************************
