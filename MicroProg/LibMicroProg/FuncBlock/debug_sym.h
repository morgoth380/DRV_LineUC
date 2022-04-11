#ifdef    GET_FUNC_CODE

#include "GlobalVar.h"
#include "math.h"

typedef struct{
        u16 cntErr;
}DS_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_In0 ;                          //1  In0
    u16  Pnt_In1 ;                          //2  In1
    u16  Pnt_In2 ;                          //3  In2
    u16  Pnt_In3 ;                          //4  In3
    u16  Pnt_In4 ;                          //5  In4
    u16  Pnt_In5 ;                          //6  In5
    u16  Pnt_In6 ;                          //7  In6
    u16  Pnt_In7 ;                          //8  In7
    u16  Pnt_In8 ;                          //9  In8
    u16  Pnt_In9 ;                          //10  In9
    u16  Pnt_In10;                         //11  In10
    u16  Pnt_In11;                         //12  In11
    u16  Pnt_In12;                         //13  In12
    u16  Pnt_In13;                         //14  In13
    u16  Pnt_In14;                         //15  In14
    u16  Pnt_In15;                         //16  In15
    u16  Pnt_out15;                        //17  out15
    u16  Pnt_out14;                        //18  out14
    u16  Pnt_out13;                        //19  out13
    u16  Pnt_out12;                        //20  out12
    u16  Pnt_out11;                        //21  out11
    u16  Pnt_out10;                        //22  out10
    u16  Pnt_out9 ;                         //23  out9
    u16  Pnt_out8 ;                         //24  out8
    u16  Pnt_out7 ;                         //25  out7
    u16  Pnt_out6 ;                         //26  out6
    u16  Pnt_out5 ;                         //27  out5
    u16  Pnt_out4 ;                         //28  out4
    u16  Pnt_out3 ;                         //29  out3
    u16  Pnt_out2 ;                         //30  out2
    u16  Pnt_out1 ;                         //31  out1
    u16  Pnt_out0 ;                         //32  out0
    u16  Pnt_End;
}FuncDEBUG_SYM_type;

f32  v_outId=0, v_outIq=0, v_outTheta=0;
u16	 v_pwmOn=0, v_onStartWork=0;
f32	 v_fixIdDone = 0, v_fixIdDoneCnt=0, v_timeKoef=0, v_FixTimeId=0; 
void pmsm_antirollback_calc_sym(void);

//********************************************************
u16 *FuncDEBUG_SYM_1(FuncDEBUG_SYM_type *progPnt, u32 ramPnt)
{ 
    //GlobalM4.SoftCurrentTime = 1; // Пока 1 секунда. (float)load_s16(ramPnt, progPnt->Pnt_In11) * 0.1f;    // 1 знак после запятой
    GlobalM4.NewRefFiltrTime = 0.1f; // Пока намертво 0,1 секунда. Нужен параметр. (float)load_s16(ramPnt, progPnt->Pnt_In4) * 0.01f;    // постоянная времени фильтра задания скорости (задержка АСУ)
    
    // Управление переменной частотой ШИМа
    GlobalM4.Debug0 = (float)load_s16(ramPnt, progPnt->Pnt_In0); // Частота ШИМ в Гц
    GlobalM4.Debug1 = (float)load_s16(ramPnt, progPnt->Pnt_In1); // Сигнал разрешения для ручного задания частоты ШИМа
          
    // Блокировка работы ВУ ВД - ТЕСТ
    GlobalM4.Debug2 = (f32)load_s16(ramPnt, progPnt->Pnt_In2);    

    // Данная опция отключена в ACI_Vector_Cntrl
    //GlobalM4.Debug3 = (f32)load_s16(ramPnt, progPnt->Pnt_In3);  // Записать 111 для переключения на фильтр коваленкова
    
    // Переменная для подстройки компенсации Дедтайма.
    GlobalM4.Debug3 = ((f32)load_s16(ramPnt, progPnt->Pnt_In3)) / 10.0f;
      
    // По коду не используется Debug4
    save_s16(ramPnt, progPnt->Pnt_In4, (u16)(GlobalM4.Debug4)); //GlobalM4.Debug4 = //load_s16(ramPnt, progPnt->Pnt_In4)*0.001;  // TDiskr
    // По коду не используется Debug5
    save_s16(ramPnt, progPnt->Pnt_In5, (u16)(GlobalM4.Debug5)); //GlobalM4.Debug4 = //load_s16(ramPnt, progPnt->Pnt_In4)*0.001;  // TDiskr

//    GlobalM4.Debug5 = load_s16(ramPnt, progPnt->Pnt_In5);  // Включение энкодера
                
    GlobalM4.TfiltVolWeak   = 0.04f; // (f32)load_s16(ramPnt, progPnt->Pnt_TfiltVolWeak) * 0.0001f;
    GlobalM4.TudcFilt       = 0.004f; // (f32)load_s16(ramPnt, progPnt->Pnt_TudcFilt) * 0.0001f;    
    
#ifdef _PROJECT_FOR_VS2005_
    {
      u16		i, repeat;
      // Вызов функции низкоуровневого фазирования в соотвтветствии с тактом ШИМа
      repeat = (u16)(FLOAT_TIME_CALL_MICRO * GlobalM4.Fpwm);
      for (i=0; i < repeat; i++)
        pmsm_antirollback_calc_sym();
      
      save_s16(ramPnt, progPnt->Pnt_out0, (u16)(v_outId * 1000.0));
      save_s16(ramPnt, progPnt->Pnt_out1, (u16)(v_outTheta*1000.0));
    }
#endif
	
    return &progPnt->Pnt_End;
};
//******************************************************************************************************************
#ifdef _PROJECT_FOR_VS2005_
void pmsm_antirollback_calc_sym(void)
{
    f32	        tempId, timeKStep, timeKStepDown;
    f32		v_timeUI_Up, v_timeStepDown, v_Fpwm, v_inId, v_inSpeedWrz, v_fixId, v_bitPhasing;
    u16         v_Enable;
    
    // Перенаправление
    v_outId = 0;
    v_outIq = 0;    

    // Симуляция и входные данные
    v_Fpwm = GlobalM4.Fpwm;
    v_timeUI_Up = v_timeStepDown = 500;
    v_inSpeedWrz = GlobalM4.Fref; // Проверить что сюда приходит, т.к. надо о.е.
    v_inId = 0;
    v_pwmOn = (u16)GlobalM4.FlagCommand.bit.PwmOn;
    v_fixId = 1; // Задание 1
    v_Enable = (u16)GlobalM4.pmsmAntirollback.pmsmAntirollBackEnable;

    //расчёт шага коэффициента плавного изменения тока
    //ф-ция вызывается с частотой Fpwm
    //коэффициент 1000 - для перевода времени из мсек в сек
    timeKStep = (1000.0f / (v_Fpwm * v_timeUI_Up));
    timeKStepDown = 1000.0f / (v_Fpwm * v_timeStepDown);

    // Если ШИМ выключен - взводим флаг разрешения работы алгоритма в старте
    if(!v_pwmOn) {
    	v_onStartWork  = 1;
    	v_fixIdDone    = 0;
    	v_fixIdDoneCnt = (u16)(v_FixTimeId * v_Fpwm / 1000.f); // Определяем число тиков в тактах ШИМа
        v_outTheta = 0;
    }

	v_bitPhasing = AllPrmPnt->ENCODER_PHASING_WORK;
    // Определяем режим работы алгоритмов
    if (v_bitPhasing && v_Enable) {            
      // Стадия фазировки энкодера
      
      // 1. Пока ШИМ выключен, мы фазу на выход обнуляем. В этом случае и задание частоты бужет ноль.
      if (!v_pwmOn) {
        v_outTheta = 0.0f;        
      }
            
      // 2. Рассчитываем выходную фазу в режиме фазировки энкодера. Вроде как она нужна в радианах !?!?!?
      v_outTheta = v_outTheta + ((1.0f/v_Fpwm) * v_inSpeedWrz * 6.283185);
      // Ограничиваем фазу по диапазону.
      if(v_outTheta > 6.283185) {
        v_outTheta -= 6.283185;
      } else if (v_outTheta < -6.283185) {
        v_outTheta += 6.283185;
      }
      
      // 3. Формируем задание выходного тока удержания с плавным подъемом. Сброс тока в конце работы будет проходить штатным плавным сбросом тока.
      // PMSM_ANTIROLLBACK_ID_FIX_TIME: v_fixId up to v_inId
      if (v_onStartWork /*&& v_fixIdDoneCnt*/) { // Если ШИМ включен, частота задания 0, и счетчик фиксации еще не ноль - даем рост возбуждения
        tempId = (v_fixId * v_timeKoef); //плавное нарастание/спад тока возбуждения
        // Если вычисленная величина больше входного значения, заменяем ее. Иначе оставляем входное значение.
        if (tempId > v_inId) {
          v_outId = tempId; // Заменяем, иначе тут будет значение минимального тока 
        }
        
        if(v_pwmOn) {
          //повышаем коэффициент плавного изменения тока от 0 до 1
          if (v_timeKoef < 1.0f)       v_timeKoef += timeKStep;
          else                          v_timeKoef = 1.0f;          
        } else {
          //сброс коэффициента плавного изменения тока
          v_timeKoef = 0;
        }
      }
       
    }

}
#endif

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDEBUG_SYM_1[32]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  In8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  In9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  In10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  In11
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In12
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In15
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //17  out15
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //18  out14
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //19  out13
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //20  out12
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  out11
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  out10
        FLOAT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  out9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  out8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  out7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  out6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  out5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  out4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  out3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  out2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  out1
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //32  out0
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DEBUG_SYM", "DA", TblFuncDEBUG_SYM_1, FuncDEBUG_SYM_1, sizeof(TblFuncDEBUG_SYM_1), sizeof(DS_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDEBUG_SYM_1,
#endif
//********************************************************
