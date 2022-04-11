#include "MakeOpis.h"

#ifdef    GET_FUNC_CODE

#define Read_Block      0
#define Write_Block     1 
#define Prm_99_99       12771
#define MASTER_ETALON   33*128+5
#define PROCMAN_ETALON  33*128+3
#define USER_ETALON     33*128+1
#define QuantityPrm     20
#define PARAM_MAX_NUM   99

typedef struct{
	u16   prevValue[QuantityPrm]; // Данные о предыдущих значениях перед выходом из блока
	u16   startStop;
}FuncRW_GROUP_PRM_State_type; /// !!!Название поменять


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_RD_WR;                             //1  RD_WR
    u16  Pnt_Enable;                            //2  Enable
    u16  Pnt_Prm[QuantityPrm];                  //3...22  Prm1 - Prm20
    u16  Pnt_Val[QuantityPrm];                  //23...42 Val1 - Val20
    u16  Pnt_End;
}FuncRW_GROUP_PRM_type;

//********************************************************
u16 BlockGroup[6] = {2, 33, 37, 38, 97, 98}; // Набор групп для блокировки

u16 CheckAddrParam (u16 adrPrm) {
  u16   i, adrMax, adrMin;  
  
  for (i=0; i<sizeof(BlockGroup); i++) {
      adrMax = (u16)(BlockGroup[i]*128.0 + PARAM_MAX_NUM);
      adrMin = (u16)(BlockGroup[i]*128.0);
      
      if (adrPrm >= adrMin && adrPrm <= adrMax) 
        return (1); // Попали в диапазон - ошибка
  }
  
  return (0); // Адрес разрешен
  
}
//********************************************************
u16 *FuncRW_GROUP_PRM_1(FuncRW_GROUP_PRM_type *progPnt, u32 ramPnt)
{
    u16         num_block, adrPrm, ustValue;
    wm_type     *wm;
    u16         numPrm;
    FuncRW_GROUP_PRM_State_type *sPnt; 
    
    sPnt = (FuncRW_GROUP_PRM_State_type *)(ramPnt + progPnt->Pnt_State);    
    wm = &wmem;
    num_block  = 0;

    // ========================================================================  
    if (!(load_s16(ramPnt, progPnt->Pnt_Enable))) {                              // Проверка включен ли режим
      for(numPrm = 0; numPrm < QuantityPrm; numPrm++){
        save_s16(ramPnt, progPnt->Pnt_Val[numPrm], 0);                           // Обнулялка значений, если блок отключен
      }      
      return &progPnt->Pnt_End;
    }
    
    // ========================================================================
    num_block = load_s16(ramPnt, progPnt->Pnt_RD_WR);                           // Текущий блок ( чтение или запись)
    
    /* Блочное чтение параметров */
    if(num_block == Read_Block){                                                  
        for(numPrm = 0; numPrm < QuantityPrm; numPrm++){
            adrPrm = load_s16(ramPnt, progPnt->Pnt_Prm[numPrm]);                // вычитка адресса параметра
            if ( CheckAddrParam(adrPrm) ){ // Блокировка чтения параметров эталонов паролей
                adrPrm = Prm_99_99;
                save_s16(ramPnt, progPnt->Pnt_Prm[numPrm], Prm_99_99);
            }
             
            if(adrPrm != Prm_99_99){                                            // если адрес = 99:99, то не сохраняем значение
                wm->AdrPrm = adrPrm;                                             
                MakeOpis(wm, 1);                                                // 
                save_s16(ramPnt, progPnt->Pnt_Val[numPrm], (u16)wm->s32.Value); // запись значения параметра
                wm->s32.Value = 0; // Обнулить значение wmem, для следующего значения
            }
        }
    }
    else if(num_block == Write_Block){
        
        // =====================================================================
        // Перед выходом прочитать все значения из MakeOpis и синхронизировать с sPnt 
        for(numPrm = 0; numPrm < QuantityPrm; numPrm++){
            adrPrm = load_s16(ramPnt, progPnt->Pnt_Prm[numPrm]);                // вычитка адреса параметра
            
            if ( CheckAddrParam(adrPrm) ){ // Блокировка чтения параметров эталонов паролей
                adrPrm = Prm_99_99;
                save_s16(ramPnt, progPnt->Pnt_Prm[numPrm], Prm_99_99);
                break;
            }

            ustValue = (u16)load_s16(ramPnt, progPnt->Pnt_Val[numPrm]);
            if (ustValue != sPnt->prevValue[numPrm]) { // Если новая и старая не равны, делаем запись нового значения
                wm->AdrPrm = adrPrm;                                             
                wm->s32.Value = (u16)ustValue;
                MakeOpis(wm, 0);                                                // 
                wm->s32.Value = 0; // Обнулить значение wmem, для следующего значения              
            }
                                    
            // Повторная вычитка параметра с проверкой записался или нет
            if(adrPrm != Prm_99_99){                                            // если адрес = 99:99, то не сохраняем значение
                wm->AdrPrm = adrPrm;                                             
                MakeOpis(wm, 1);                                                // 
                sPnt->prevValue[numPrm] = wm->s32.Value;
                save_s16(ramPnt, progPnt->Pnt_Val[numPrm], (u16)wm->s32.Value); // запись значения параметра
                wm->s32.Value = 0; // Обнулить значение wmem, для следующего значения
            }
        }      
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRW_GROUP_PRM_1[42]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  RD_WR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Enable
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Prm1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Prm2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Prm3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Prm4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Prm5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Prm6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Prm7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Prm8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Prm9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Prm10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Prm11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Prm12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Prm13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Prm14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Prm15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Prm16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Prm17
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Prm18
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Prm19
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Prm20
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  Val1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  Val2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  Val3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  Val4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  Val5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  Val6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  Val7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  Val8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  Val9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  Val10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //33  Val11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //34  Val12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //35  Val13
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //36  Val14
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //37  Val15
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //38  Val16
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //39  Val17
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //40  Val18
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //41  Val19
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //42  Val20
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("RW_GROUP_PRM", "DD", TblFuncRW_GROUP_PRM_1, FuncRW_GROUP_PRM_1, sizeof(TblFuncRW_GROUP_PRM_1), sizeof(FuncRW_GROUP_PRM_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRW_GROUP_PRM_1,
#endif
//********************************************************
