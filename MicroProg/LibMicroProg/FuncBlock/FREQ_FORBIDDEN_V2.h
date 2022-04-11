#ifdef    GET_FUNC_CODE

#include "GT_DEFs.h"

#define	 FORBIDDEN_FREQ_NUMBER	5  // Количество запрещенных частот

//********************************************************
// Указательный тип для хождения по параметрам на схеме.
typedef struct{
    u16  Flow;        // 3  F_low
    u16  Fhigh;       // 4  F_high
}pFreqForb_type;

//********************************************************
// Числовые значения для запрещенных частот
typedef struct{
    float  Flow;      // 3  F_low
    float  Fhigh;     // 4  F_high
}FreqForb_type;

//********************************************************

typedef struct{
    float Fin;
    float Fmin;
    float Fmax;
    float Sign;                  // Знак частоты задания
    float FMaxUst;              // Максимальная частота
    float FMinUst;              // Минимальная частота
    float Fprevmax;
    float dFreq;
    float Fdrv;                 // Текущая выходная частота
    union{
    	struct{
            u16 FpuskForb : 1;   // Ошибка частота пуска в диапазоне запрещенных частот
            u16 RefPID    : 1;   // Выбран режим работы по ПИД-регулятору
			u16 SetNextFreq: 1;   // Выставить следующую частоту диапазона
            u16 reserv    : 14;  // Резерв
        }bit;
    	u16 all;
    }flg;
    StateGT_type GT;

}localsFreqForb_type;
//********************************************************
typedef enum {
  FORBFREQ_OFF = 0,
  FORBFREQ_ON = 1,
  FORBFREQ_RESET = 2,
} ForbFreqMode_enum;

//********************************************************
typedef struct{
  FreqForb_type	        forbFreq[FORBIDDEN_FREQ_NUMBER];  // Список запрещенных частот
  ForbFreqMode_enum     ForbFreqMode;                     // Статус параметра режима работы критических частот
  u16      	        Crc;
}nvStateFREQ_FORBIDDEN_V2State_type;

#define     nvForbiddenFreq		(*(nvStateFREQ_FORBIDDEN_V2State_type*)0)

//********************************************************
typedef struct{
  float Fout;
  float Fpusk;
  float NextFreqGT;
  u16	  TypeNextFreqGT;
  float ForbdFout;
  float prevForbdFout;	    // Предыдущее значение выходного задания,
  nvStateFREQ_FORBIDDEN_V2State_type sPntForbFreq;
}FuncFREQ_FORBIDDEN_V2State_type;

//********************************************************
typedef struct{
    u16           		Pnt_Nfunc;
    u16           		Pnt_State;
    u16       	  		Pnt_nvState;
    u16           		Pnt_Fin;              		   // 1  Fin
    u16           		Pnt_On;               		   // 2  On
    pFreqForb_type		Pnt_Forb[FORBIDDEN_FREQ_NUMBER];   // 3 - 12
    u16          		Pnt_Res;             		   // 13  Res
    u16           		Pnt_F_Min_Ust;                     // 14  F_Min
    u16           		Pnt_Ref_PID;           		   // 15  Ref_PID
    u16           		Pnt_F_Max_Ust;                     // 16  Ref_PID    
    u16           		Pnt_ForbidTaccIn;                  // 17  ForbidTacc
    u16           		Pnt_ForbidTdecIn;                  // 18  ForbidTdec    
    u16           		Pnt_FrefDrv;                       // 19  FrefDrv     
    u16           		Pnt_ForbidFreqNow;                 // 20  ForbidFreqNow    
    u16           		Pnt_ForbidTdecOut;                 // 21  ForbidTdecOut    
    u16           		Pnt_ForbidTaccOut;                 // 22  ForbidTaccOut        
    u16           		Pnt_Forbidden;         		   // 23  Forbidden
    u16           		Pnt_Fout;              		   // 24  Fout
    u16           		Pnt_End;
}FuncFREQ_FORBIDDEN_V2_type;

//********************************************************
void CheckForbidenFreq(FuncFREQ_FORBIDDEN_V2_type *progPnt, u32 ramPnt, FuncFREQ_FORBIDDEN_V2State_type *sPnt);
void RestoreForbidenFreq(FuncFREQ_FORBIDDEN_V2_type *progPnt, u32 ramPnt, FuncFREQ_FORBIDDEN_V2State_type *sPnt);

//********************************************************
u16 *FuncFREQ_FORBIDDEN_V2_1(FuncFREQ_FORBIDDEN_V2_type *progPnt, u32 ramPnt)
{
    localsFreqForb_type                 loc;
    u8                                  i;
    u16                                 crc, ForbidTacc, ForbidTdec, State_GT;
    FuncFREQ_FORBIDDEN_V2State_type     *sPnt;
    nvStateFREQ_FORBIDDEN_V2State_type  *nvAdr, nvMem;
    FreqForb_type                       *pForbFreq;
    u16                                 UstRecoveryFlagValue;
	
    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvForbiddenFreq);
    sPnt = (FuncFREQ_FORBIDDEN_V2State_type *)(ramPnt + progPnt->Pnt_State);
    RdFramMicro( &UstRecoveryFlagValue, &nv.UstRecoveryFlag,  sizeof(u16)); // Проверяем флаг восстановления уставок
        
    if(flagIsFirstCall){
        sPnt->Fout = 0;
        sPnt->Fpusk = 0;

        // Вычитываем значения запрещенных частот из Fram
        RdFramMicro(&nvMem, nvAdr, sizeof(nvStateFREQ_FORBIDDEN_V2State_type));
        crc = crcPacket((unsigned char *)&nvMem,  sizeof(nvStateFREQ_FORBIDDEN_V2State_type) - sizeof(u32));   // sizeof(u32)!!! так и не понял по чему (2 лишних байта)
    	
        // Указатель на начало блока частот 
    	pForbFreq = &(nvMem.forbFreq[0]);
    	
        // Проверка данных - если не совпадает CRC или есть восстановление из уставок, выполняем загрузку из системы параметров
        if (crc != nvMem.Crc || UstRecoveryFlagValue == 1){
          // Если CRC не совпал - грузим инфу из системы параметров
          // Переносим все частоты
          for (i=0; i<FORBIDDEN_FREQ_NUMBER; i++) {
            pForbFreq[i].Flow = load_float(ramPnt, progPnt->Pnt_Forb[i].Flow);
            pForbFreq[i].Fhigh = load_float(ramPnt, progPnt->Pnt_Forb[i].Fhigh);
          }
          nvMem.ForbFreqMode = (ForbFreqMode_enum)load_s16(ramPnt, progPnt->Pnt_On); // Читаем режим работы критических частот
          // Находим CRC блока и записываем его
          nvMem.Crc      = crcPacket((unsigned char *)&nvMem,  sizeof(nvStateFREQ_FORBIDDEN_V2State_type) - sizeof(u16));
          WrFramMicro(nvAdr, &nvMem, sizeof(nvStateFREQ_FORBIDDEN_V2State_type));
        }

        // Переносим данные о запрещенных частотах из FRAM в систему параметров и копию в sPnt для контроля изменений
        sPnt->sPntForbFreq.ForbFreqMode = nvMem.ForbFreqMode;
        save_s16(ramPnt, progPnt->Pnt_On, (u16)(sPnt->sPntForbFreq.ForbFreqMode)); // Возвращаем режим работы каким он был до сброса
        for (i=0; i<FORBIDDEN_FREQ_NUMBER; i++) {
            sPnt->sPntForbFreq.forbFreq[i].Flow = pForbFreq[i].Flow;
            sPnt->sPntForbFreq.forbFreq[i].Fhigh = pForbFreq[i].Fhigh;
            save_float(ramPnt, progPnt->Pnt_Forb[i].Flow, pForbFreq[i].Flow);
            save_float(ramPnt, progPnt->Pnt_Forb[i].Fhigh, pForbFreq[i].Fhigh);
        }
    }

    loc.FMaxUst = load_float(ramPnt, progPnt->Pnt_F_Max_Ust);
    loc.FMinUst = load_float(ramPnt, progPnt->Pnt_F_Min_Ust);
    // Частота задания
    loc.Fin = load_float(ramPnt, progPnt->Pnt_Fin);
    if(loc.Fin >= 0) 
        loc.Sign = 1.0f;
    else{
	loc.Fin = -loc.Fin;
	loc.Sign = -1.0f;
    }
    loc.flg.all = 0;
    loc.flg.bit.RefPID = testBitMicro(ramPnt, progPnt->Pnt_Ref_PID);

    // Проверка запретных частот
    CheckForbidenFreq(progPnt, ramPnt, sPnt);

    // Работа по модулю запрещенных частот
    if(load_s16(ramPnt, progPnt->Pnt_On) && (!loc.flg.bit.RefPID)){      // Включен модуль запрещенных частот.
        loc.GT.all = 0;
    	for(i = 0; i < FORBIDDEN_FREQ_NUMBER; i++){
            loc.Fmin = load_float(ramPnt, progPnt->Pnt_Forb[i].Flow);
            loc.Fmax = load_float(ramPnt, progPnt->Pnt_Forb[i].Fhigh);
            if(!testBitMicro(ramPnt, progPnt->Pnt_Res) && loc.Fmax){    // Если ШИМ включен  и введена верхняя граница запрещенных частот
                if((loc.Fin > loc.Fmin) && (loc.Fin < loc.Fmax)){       // Если частота задания входит в диапазон запретных частот
               	    if(loc.Fin >= ((loc.Fmin + loc.Fmax)/2.0f)){
               	        if (loc.Fmax > loc.FMaxUst)                     // Если верхняя граница больше уставки Максимальная частота
                            sPnt->Fout = loc.Fmin;
                        else
                            sPnt->Fout = loc.Fmax;
                	}
                    else{
                         if(loc.Fmin < loc.FMinUst)                     // Если нижняя граница меньше уставки Минимальная частота
                              sPnt->Fout = loc.Fmax;
                          else
                              sPnt->Fout = loc.Fmin;
                    }
                    break;
                }
                else 
                    sPnt->Fout = loc.Fin;
            }
            else{                                                       // ШИМ отключен
                if((loc.Fin > loc.Fmin) && (loc.Fin < loc.Fmax)){
                    loc.flg.bit.FpuskForb = 1;
                    break;
                }
                sPnt->Fout = loc.Fin;
                sPnt->Fpusk = loc.Fin;
            }
    	}
    }
    else{                                                              // Модуль отключен - транслируем входную частоту на выход.
            sPnt->Fout = loc.Fin;
    }
    
	sPnt->ForbdFout = (sPnt->Fout * loc.Sign);	// Конечная частота задания с учетом фильтрации через запрещенные частоты
    save_float(ramPnt, progPnt->Pnt_Fout, (sPnt->Fout * loc.Sign));

    defBitMicro(ramPnt, progPnt->Pnt_Forbidden, loc.flg.bit.FpuskForb); // Сбросить/Установить аварию "Ошибка запрещенная частота" (Пусковая частота попала в диапазон запретных частот).

    // Проверка на сохранение настроек запрещенных частот после изменений
    for (i=0; i<FORBIDDEN_FREQ_NUMBER; i++) {
    	// Сравниваем уставки в sPnt и в системе параметров
    	if ( sPnt->sPntForbFreq.forbFreq[i].Flow != load_float(ramPnt, progPnt->Pnt_Forb[i].Flow)  ||
             sPnt->sPntForbFreq.forbFreq[i].Fhigh != load_float(ramPnt, progPnt->Pnt_Forb[i].Fhigh) ||
             sPnt->sPntForbFreq.ForbFreqMode != (ForbFreqMode_enum)load_s16(ramPnt, progPnt->Pnt_On)) { // Дополнительно проверяем режми работы запрещенных частот
            // Нашли различие, надо сохранить в FRAM
			nvMem.ForbFreqMode = sPnt->sPntForbFreq.ForbFreqMode = (ForbFreqMode_enum)load_s16(ramPnt, progPnt->Pnt_On);
            pForbFreq = &(nvMem.forbFreq[0]); // Указатель на начало блока частот
            for(i=0; i<FORBIDDEN_FREQ_NUMBER; i++) {
                sPnt->sPntForbFreq.forbFreq[i].Flow = load_float(ramPnt, progPnt->Pnt_Forb[i].Flow);
                sPnt->sPntForbFreq.forbFreq[i].Fhigh = load_float(ramPnt, progPnt->Pnt_Forb[i].Fhigh);
                pForbFreq[i].Flow = load_float(ramPnt, progPnt->Pnt_Forb[i].Flow);
                pForbFreq[i].Fhigh = load_float(ramPnt, progPnt->Pnt_Forb[i].Fhigh);
            }
            // Находим CRC блока и записываем его
            nvMem.Crc = crcPacket((unsigned char *)&nvMem,  sizeof(nvStateFREQ_FORBIDDEN_V2State_type) - sizeof(u32));
            WrFramMicro(nvAdr, &nvMem, sizeof(nvStateFREQ_FORBIDDEN_V2State_type));
    	    break;
    	}
    }
    //*****************************************************************************
    // Формирование задания частоты особым образом, с учетом зон запрещенных частот
    // Движение выполняется ступенями между зонами разрешенных и запрещенных частот
    loc.Fdrv = fabsf(load_float(ramPnt, progPnt->Pnt_FrefDrv)); // Текущая частота на выходе ПЧ
    State_GT = AllPrmPnt->STATE_GT;// Берем статус GT, так будет правильнее//  & (RefZero|EquRV|Freeze)
    // Если ШИМ включен  и введена верхняя граница запрещенных частот
    // Включен модуль запрещенных частот и не режим не ПИД
    if(load_s16(ramPnt, progPnt->Pnt_On) && (!loc.flg.bit.RefPID) && !testBitMicro(ramPnt, progPnt->Pnt_Res)){      
      loc.GT.all = 0;
      if ( ((State_GT & EqRefVal) == EqRefVal && loc.Fdrv != fabsf(sPnt->ForbdFout)) || (sPnt->prevForbdFout != sPnt->ForbdFout) ) { // ??? Если вышли на рабочую частоту, делаем следующий переход
        // Ищем частоту ближайшую к текущей на выходе ПЧ
        sPnt->NextFreqGT = sPnt->Fout; // Начальное значение - равно частоте задания с учетом запрещенных частот
        sPnt->TypeNextFreqGT = 0; // Считаем что зона будет нормальная
        for(i = 0; i < FORBIDDEN_FREQ_NUMBER; i++){
          loc.Fmin = load_float(ramPnt, progPnt->Pnt_Forb[i].Flow);
          loc.Fmax = load_float(ramPnt, progPnt->Pnt_Forb[i].Fhigh);

          if (loc.Fmin == 0 && loc.Fmax == 0)   continue;
          
          if (loc.Fdrv < fabsf(sPnt->ForbdFout)) { // Блок для разгона
            if(loc.Fdrv < loc.Fmin && loc.Fmin < sPnt->NextFreqGT){  // Если текущая частота, меньше минимальной, и эта самая минимальная 
              // то принимаем ее как претендента.
              sPnt->NextFreqGT = loc.Fmin;
              sPnt->TypeNextFreqGT = 0; // Разгон в нормальной зоне
            } else if (loc.Fdrv < loc.Fmax && loc.Fmax <= sPnt->NextFreqGT) { // Если текущая частота, меньше максимальной, и эта самая минимальная
              // Берем ее
              sPnt->NextFreqGT = loc.Fmax;
              sPnt->TypeNextFreqGT = 1; // Разгон в зоне запрещенной частоты
            }
          } else if (loc.Fdrv > fabsf(sPnt->ForbdFout)) { // Блок для торможения
            if(loc.Fdrv > loc.Fmax && loc.Fmax > sPnt->NextFreqGT){  // Если текущая частота, больше максимальной, и эта самая большая частота
              // то принимаем ее как претендента.
              sPnt->NextFreqGT = loc.Fmax;
              sPnt->TypeNextFreqGT = 0; // Разгон в нормальной зоне
            } else if (loc.Fdrv > loc.Fmin && loc.Fmin >= sPnt->NextFreqGT) { // Если текущая частота, больше минимальной, и эта самая большая из минимальных
              // Берем ее
              sPnt->NextFreqGT = loc.Fmin;
              sPnt->TypeNextFreqGT = 1; // Разгон в зоне запрещенной частоты
            }
          }
        }
      }
      
      if (sPnt->TypeNextFreqGT)	setBitMicro (ramPnt, progPnt->Pnt_ForbidFreqNow);
      else						clrBitMicro (ramPnt, progPnt->Pnt_ForbidFreqNow);
      save_float(ramPnt, progPnt->Pnt_Fout, (sPnt->NextFreqGT * loc.Sign));      
    }
    //*****************************************************************************

    
    sPnt->prevForbdFout = sPnt->ForbdFout;	
    ForbidTacc = load_s16(ramPnt, progPnt->Pnt_ForbidTaccIn);
    ForbidTdec = load_s16(ramPnt, progPnt->Pnt_ForbidTdecIn);
    save_s16(ramPnt, progPnt->Pnt_ForbidTaccOut, ForbidTacc);
    save_s16(ramPnt, progPnt->Pnt_ForbidTdecOut, ForbidTdec);

    return &progPnt->Pnt_End;
};
//********************************************************
// Востанавливаем запрещенные частоты
void RestoreForbidenFreq(FuncFREQ_FORBIDDEN_V2_type *progPnt, u32 ramPnt, FuncFREQ_FORBIDDEN_V2State_type *sPnt)
{
    u8	j;

    // Откатываем запрещенные частоты
    for (j=0; j<FORBIDDEN_FREQ_NUMBER; j++) {
    	save_float(ramPnt, progPnt->Pnt_Forb[j].Flow, sPnt->sPntForbFreq.forbFreq[j].Flow);
    	save_float(ramPnt, progPnt->Pnt_Forb[j].Fhigh, sPnt->sPntForbFreq.forbFreq[j].Fhigh);
    }
}
//********************************************************
void CheckForbidenFreq(FuncFREQ_FORBIDDEN_V2_type *progPnt, u32 ramPnt, FuncFREQ_FORBIDDEN_V2State_type *sPnt)
{
    u8		i, j;
    ForbFreqMode_enum     locForbFreqMode;
    float	Fmin, Fmax, tstFmin, tstFmax;
    float	dFreq = 0;

#ifdef _PROJECT_FOR_STM32_
	dFreq = 1.0f / ((float)MPOpis.Base[NUM_FREQ_BASE]);
#else
	dFreq = 1.0f / (800.0f);
	//dFreq = 1.0f / ((float)mPnt->Base[Herz_base]);
#endif
    // Проверка необходимости сброса запрещенных частот - выполняем обнуление.
    locForbFreqMode = (ForbFreqMode_enum)load_s16(ramPnt, progPnt->Pnt_On); // Читаем значение из системы параметров, не трогая sPnt
    if (locForbFreqMode == FORBFREQ_RESET) {
      save_s16(ramPnt, progPnt->Pnt_On, (u16)(sPnt->sPntForbFreq.ForbFreqMode)); // Возвращаем режим работы каким он был до сброса
      for (j=0; j<FORBIDDEN_FREQ_NUMBER; j++) { // И выполняем обнуление всех параметров
        sPnt->sPntForbFreq.forbFreq[j].Flow = 0;
        sPnt->sPntForbFreq.forbFreq[j].Fhigh = 0;
        save_float(ramPnt, progPnt->Pnt_Forb[j].Flow, sPnt->sPntForbFreq.forbFreq[j].Flow);
        save_float(ramPnt, progPnt->Pnt_Forb[j].Fhigh, sPnt->sPntForbFreq.forbFreq[j].Fhigh);
      }
    }
        
    // Проверяем значения запрещенных частот на предмет их обнуления
    // Считываем пару значений и проверяем их на ноль для сброса или ввода нового значения
    for (j=0; j<FORBIDDEN_FREQ_NUMBER; j++) {
      Fmin	= load_float(ramPnt, progPnt->Pnt_Forb[j].Flow);
      Fmax	= load_float(ramPnt, progPnt->Pnt_Forb[j].Fhigh);
      
      if( Fmin && Fmax ){   // Если не нули - ничего с ними не делаем
        continue;
      }
      else{  // Кто-то один не ноль
        // Если перед вводом значения, sPnt значения были оба равны нулю. То можно вводить новое значение
        if(!sPnt->sPntForbFreq.forbFreq[j].Flow && !sPnt->sPntForbFreq.forbFreq[j].Fhigh){
          if(Fmin){
            Fmax = Fmin + dFreq;
            save_float(ramPnt, progPnt->Pnt_Forb[j].Fhigh, Fmax);
          }else if(Fmax){
            Fmin = Fmax - dFreq;
            save_float(ramPnt, progPnt->Pnt_Forb[j].Flow, Fmin);
          }
          continue;
        }
        sPnt->sPntForbFreq.forbFreq[j].Flow = 0;
        sPnt->sPntForbFreq.forbFreq[j].Fhigh = 0;
        save_float(ramPnt, progPnt->Pnt_Forb[j].Flow, sPnt->sPntForbFreq.forbFreq[j].Flow);
        save_float(ramPnt, progPnt->Pnt_Forb[j].Fhigh, sPnt->sPntForbFreq.forbFreq[j].Fhigh);
      }
    }
    
    // Проверка на попадание внутрь чужого диапазона и на перемену мест для одной из пар (Fmin > Fmax)
    for (j=0; j<FORBIDDEN_FREQ_NUMBER; j++) {
      Fmin = load_float(ramPnt, progPnt->Pnt_Forb[j].Flow);
      Fmax = load_float(ramPnt, progPnt->Pnt_Forb[j].Fhigh);
      
      if (Fmin >= Fmax && Fmin && Fmax ) {
        RestoreForbidenFreq(progPnt, ramPnt, sPnt);
        break;
      }
      
      for (i=0; i<FORBIDDEN_FREQ_NUMBER; i++) {
        if (i == j) continue;
        tstFmin = load_float(ramPnt, progPnt->Pnt_Forb[i].Flow);
        tstFmax = load_float(ramPnt, progPnt->Pnt_Forb[i].Fhigh);
        
        if ( (Fmin >= tstFmin && Fmin <= tstFmax) && Fmin )  {
          RestoreForbidenFreq(progPnt, ramPnt, sPnt);
          break;
        }
        if ( (Fmax >= tstFmin && Fmax <= tstFmax) && Fmax )  {
          RestoreForbidenFreq(progPnt, ramPnt, sPnt);
          break;
        }
      }
    }
}

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFREQ_FORBIDDEN_V2_1[24]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Fin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  On
	S32_PIN_TYPE | INPUT_PIN_MODE,     //3  F1_low
	S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F1_high
	S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F2_low
	S32_PIN_TYPE | INPUT_PIN_MODE,     //6  F2_high
	S32_PIN_TYPE | INPUT_PIN_MODE,     //7  F3_low
	S32_PIN_TYPE | INPUT_PIN_MODE,     //8  F3_high
	S32_PIN_TYPE | INPUT_PIN_MODE,     //9  F4_low
	S32_PIN_TYPE | INPUT_PIN_MODE,     //10  F4_high
	S32_PIN_TYPE | INPUT_PIN_MODE,     //11  F5_low
	S32_PIN_TYPE | INPUT_PIN_MODE,     //12  F5_high
	BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  Res
	S32_PIN_TYPE | INPUT_PIN_MODE,     //14  F_Min
	BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  Ref_PID
	S32_PIN_TYPE | INPUT_PIN_MODE,     //16  F_lim        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  ForbidTaccIn        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  ForbidTdecIn        
	S32_PIN_TYPE | INPUT_PIN_MODE,     //19  Pnt_FrefDrv                
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //20  ForbidFreqNow                
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //21  ForbidTaccOut        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //22  ForbidTdecOut        
	BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  Forbidden
	S32_PIN_TYPE | OUTPUT_PIN_MODE     //24  Fout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FREQ_FORBIDDEN_V2", "DA", TblFuncFREQ_FORBIDDEN_V2_1, FuncFREQ_FORBIDDEN_V2_1, sizeof(TblFuncFREQ_FORBIDDEN_V2_1), sizeof(FuncFREQ_FORBIDDEN_V2State_type), sizeof(nvStateFREQ_FORBIDDEN_V2State_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFREQ_FORBIDDEN_V2_1,
#endif
//********************************************************
