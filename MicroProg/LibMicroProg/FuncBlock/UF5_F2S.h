#include    "GT_DEFs.h"
//#include    "CommonMicro.h"

#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_mull_U;                       //1  mull_U  !!! не множитель !!!
    u16  Pnt_F;                            //2  F
    u16  Pnt_F5_1;                         //3  F5_1
    u16  Pnt_F4_1;                         //4  F4_1
    u16  Pnt_F3_1;                         //5  F3_1
    u16  Pnt_F2_1;                         //6  F2_1
    u16  Pnt_F1_1;                         //7  F1_1
    u16  Pnt_U5_1;                         //8  U5_1
    u16  Pnt_U4_1;                         //9  U4_1
    u16  Pnt_U3_1;                         //10  U3_1
    u16  Pnt_U2_1;                         //11  U2_1
    u16  Pnt_U1_1;                         //12  U1_1
    u16  Pnt_U0_1;                         //13  U0_1
    u16  Pnt_F5_2;                         //14  F5_2
    u16  Pnt_F4_2;                         //15  F4_2
    u16  Pnt_F3_2;                         //16  F3_2
    u16  Pnt_F2_2;                         //17  F2_2
    u16  Pnt_F1_2;                         //18  F1_2
    u16  Pnt_U5_2;                         //19  U5_2
    u16  Pnt_U4_2;                         //20  U4_2
    u16  Pnt_U3_2;                         //21  U3_2
    u16  Pnt_U2_2;                         //22  U2_2
    u16  Pnt_U1_2;                         //23  U1_2
    u16  Pnt_U0_2;                         //24  U0_2
    u16  Pnt_GTstate;                      //25  GTstate
    u16  Pnt_Reset;                        //26  Reset
    u16  Pnt_TimeUSt;                      //27  TimeUSt
    u16  Pnt_PickUp;                       //28  PickUp
    u16  Pnt_Tfiltr;                       //29  Tfiltr
    u16  Pnt_SelNum;                       //30  SelNum
    u16  Pnt_DinSel;                       //31  DinSel
    u16  Pnt_AutoUF1;                      //32  AutoUF1	- Автоадаптация UF1
    u16  Pnt_AutoUF2;                      //33  AutoUF2	- Автоадаптация UF2
    u16  Pnt_I_Hold;                       //34  I_Hold		- Удержание током
    u16  Pnt_I0_UF1;                       //35  I0_UF1		- Значение I0 U/F 1 в % от номинального тока двигателя
    u16  Pnt_I0_UF2;                       //36  I0_UF2		- Значение I0 U/F 2 в % от номинального тока двигателя
    u16  Pnt_ResOut;                       //37  ResOut
    u16  Pnt_WaitUF;                       //38  WaitUF
    u16  Pnt_U;                            //39  U
    u16  Pnt_I;                            //40  I
    u16  Pnt_End;

}FuncUF5_F2S_type;

//********************************************************
typedef struct {
  float     F5;
  float     F4;
  float     F3;
  float     F2;
  float     F1;
  float     U5;
  float     U4;
  float     U3;
  float     U2;
  float     U1;
  float     U0;
} UF_struct_type;
//********************************************************
typedef struct {
    UF_struct_type UF1;
    UF_struct_type UF2;
    u16 Autoset1;
    u16 Autoset2;
    u16 crc;
}UF5_F2SnvState_type;
//********************************************************
typedef struct{
	float     F5;
	float     F4;
	float     F3;
	float     F2;
	float     F1;
	float     U5;
	float     U4;
	float     U3;
	float     U2;
	float     U1;
   	float     U0;
        
        float     TimeUStart;
        s16       PrevRes;
        s16       PrevSet;
        s16	  PrevSelUF;
        
        float     Fnom;
        float     Usn;
        float     LastU5_1;
        float     LastF5_1;
        float     LastU5_2;
        float     LastF5_2;
        u16       SyncAutosetData; 
        u16       NumUF;
        UF5_F2SnvState_type nvMem;
}UF5_F2SState_type;

//********************************************************
// Адреса параметров для характеристик UF
//********************************************************

// Типовые точки по частоте для двигателя 50 Гц, а по ним уже будем рассчитывать точки по напряжению
float TYPICAL_F[5] = {50.0, 45.0, 40.0, 20.0, 1.5};
#define         USER_DEFINE_UF          0
#define         AUTOSET_UF              1
#define         nvUF    (*(UF5_F2SnvState_type*)0)

void RESTORE_UF_FROM_SPNT_TO_MICRO(FuncUF5_F2S_type *progPnt, u32 ramPnt, UF5_F2SState_type *sPnt) {                                                
        save_float(ramPnt, progPnt->Pnt_U0_1, sPnt->nvMem.UF1.U0);              
        save_float(ramPnt, progPnt->Pnt_U1_1, sPnt->nvMem.UF1.U1);              
        save_float(ramPnt, progPnt->Pnt_U2_1, sPnt->nvMem.UF1.U2);              
        save_float(ramPnt, progPnt->Pnt_U3_1, sPnt->nvMem.UF1.U3);              
        save_float(ramPnt, progPnt->Pnt_U4_1, sPnt->nvMem.UF1.U4);             
        save_float(ramPnt, progPnt->Pnt_U5_1, sPnt->nvMem.UF1.U5);               
        save_float(ramPnt, progPnt->Pnt_F1_1, sPnt->nvMem.UF1.F1);              
        save_float(ramPnt, progPnt->Pnt_F2_1, sPnt->nvMem.UF1.F2);              
        save_float(ramPnt, progPnt->Pnt_F3_1, sPnt->nvMem.UF1.F3);              
        save_float(ramPnt, progPnt->Pnt_F4_1, sPnt->nvMem.UF1.F4);              
        save_float(ramPnt, progPnt->Pnt_F5_1, sPnt->nvMem.UF1.F5);                      
        save_float(ramPnt, progPnt->Pnt_U0_2, sPnt->nvMem.UF2.U0);              
        save_float(ramPnt, progPnt->Pnt_U1_2, sPnt->nvMem.UF2.U1);              
        save_float(ramPnt, progPnt->Pnt_U2_2, sPnt->nvMem.UF2.U2);              
        save_float(ramPnt, progPnt->Pnt_U3_2, sPnt->nvMem.UF2.U3);              
        save_float(ramPnt, progPnt->Pnt_U4_2, sPnt->nvMem.UF2.U4);              
        save_float(ramPnt, progPnt->Pnt_U5_2, sPnt->nvMem.UF2.U5);               
        save_float(ramPnt, progPnt->Pnt_F1_2, sPnt->nvMem.UF2.F1);              
        save_float(ramPnt, progPnt->Pnt_F2_2, sPnt->nvMem.UF2.F2);              
        save_float(ramPnt, progPnt->Pnt_F3_2, sPnt->nvMem.UF2.F3);              
        save_float(ramPnt, progPnt->Pnt_F4_2, sPnt->nvMem.UF2.F4);              
        save_float(ramPnt, progPnt->Pnt_F5_2, sPnt->nvMem.UF2.F5); 
        save_s16  (ramPnt, progPnt->Pnt_AutoUF1, sPnt->nvMem.Autoset1); 
        save_s16  (ramPnt, progPnt->Pnt_AutoUF2, sPnt->nvMem.Autoset2); 
}
void LOAD_UF_FROM_MICRO_TO_SPNT(FuncUF5_F2S_type *progPnt, u32 ramPnt, UF5_F2SState_type *sPnt) {
        sPnt->nvMem.UF1.U0   = load_float(ramPnt, progPnt->Pnt_U0_1);           
        sPnt->nvMem.UF1.U1   = load_float(ramPnt, progPnt->Pnt_U1_1);               
        sPnt->nvMem.UF1.U2   = load_float(ramPnt, progPnt->Pnt_U2_1);           
        sPnt->nvMem.UF1.U3   = load_float(ramPnt, progPnt->Pnt_U3_1);           
        sPnt->nvMem.UF1.U4   = load_float(ramPnt, progPnt->Pnt_U4_1);               
        sPnt->nvMem.UF1.U5   = load_float(ramPnt, progPnt->Pnt_U5_1);                   
        sPnt->nvMem.UF1.F1   = load_float(ramPnt, progPnt->Pnt_F1_1);               
        sPnt->nvMem.UF1.F2   = load_float(ramPnt, progPnt->Pnt_F2_1);           
        sPnt->nvMem.UF1.F3   = load_float(ramPnt, progPnt->Pnt_F3_1);           
        sPnt->nvMem.UF1.F4   = load_float(ramPnt, progPnt->Pnt_F4_1);               
        sPnt->nvMem.UF1.F5   = load_float(ramPnt, progPnt->Pnt_F5_1);                       
        sPnt->nvMem.UF2.U0   = load_float(ramPnt, progPnt->Pnt_U0_2);           
        sPnt->nvMem.UF2.U1   = load_float(ramPnt, progPnt->Pnt_U1_2);               
        sPnt->nvMem.UF2.U2   = load_float(ramPnt, progPnt->Pnt_U2_2);           
        sPnt->nvMem.UF2.U3   = load_float(ramPnt, progPnt->Pnt_U3_2);           
        sPnt->nvMem.UF2.U4   = load_float(ramPnt, progPnt->Pnt_U4_2);               
        sPnt->nvMem.UF2.U5   = load_float(ramPnt, progPnt->Pnt_U5_2);               
        sPnt->nvMem.UF2.F1   = load_float(ramPnt, progPnt->Pnt_F1_2);               
        sPnt->nvMem.UF2.F2   = load_float(ramPnt, progPnt->Pnt_F2_2);           
        sPnt->nvMem.UF2.F3   = load_float(ramPnt, progPnt->Pnt_F3_2);           
        sPnt->nvMem.UF2.F4   = load_float(ramPnt, progPnt->Pnt_F4_2);               
        sPnt->nvMem.UF2.F5   = load_float(ramPnt, progPnt->Pnt_F5_2);           
        sPnt->nvMem.Autoset1 = load_s16(ramPnt, progPnt->Pnt_AutoUF1); 
        sPnt->nvMem.Autoset2 = load_s16(ramPnt, progPnt->Pnt_AutoUF2);        
}
//********************************************************
// Проверить переход с одной характеристики на другую. Чтобы не было бросков напряжения
//********************************************************
u16 *FuncUF5_F2S_1(FuncUF5_F2S_type *progPnt, u32 ramPnt)
{
  
    UF5_F2SState_type 	*sPnt;
    u16                 Tfiltr;
    u16			*u16PntStart;
    float               F, U, Ftmp[5], *iqPntF, *iqPntU, *iqPnt;
    u16			*u16PntU, *u16PntF;
    s16                 time, tmp, state_GT, Num;
    u16                 AutoSetUF1, AutoSetUF2, crc, i;
    float               KU, KF;
    float               tempU1, tempU2, tempF1, tempF2;
    u16                 UstRecoveryFlagValue, BackupUF;
    //u16			*u16PntStartF, *u16PntStartU;
    float               *f32PntStartF1, *f32PntStartF2, *f32PntStartU1, *f32PntStartU2;
    u16                 *u16PntStartF1, *u16PntStartF2, *u16PntStartU1, *u16PntStartU2;
    UF5_F2SnvState_type     *nvAdr;
    
    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvUF);
    sPnt = (UF5_F2SState_type *)(ramPnt + progPnt->Pnt_State);
    F = fabsf(load_float(ramPnt, progPnt->Pnt_F));
    Tfiltr = load_s16(ramPnt, progPnt->Pnt_Tfiltr);
    
    if(flagIsFirstCall){            // начальная инициализация      
        RdFramMicro( &UstRecoveryFlagValue, &nv.UstRecoveryFlag,  sizeof(u16)); // Проверяем флаг восстановления уставок
        Tfiltr = 0;
        RdFramMicro(&(sPnt->nvMem), nvAdr, sizeof(UF5_F2SnvState_type)); 
        crc = crcPacket((unsigned char *)&(sPnt->nvMem),  (2*sizeof(UF_struct_type)));
                
        if (sPnt->nvMem.crc != crc || UstRecoveryFlagValue) {
            LOAD_UF_FROM_MICRO_TO_SPNT(progPnt, ramPnt, sPnt);
            
            sPnt->nvMem.crc = crcPacket((unsigned char *)&(sPnt->nvMem),  (2*sizeof(UF_struct_type)));  //Ставим метку, что данные валидные
            WrFramMicro(nvAdr, &(sPnt->nvMem), sizeof(UF5_F2SnvState_type));
        }                  
        RESTORE_UF_FROM_SPNT_TO_MICRO(progPnt, ramPnt, sPnt);               
        return &progPnt->Pnt_End;
    }
            
    //-------------------------------------------------------
    // Проверка на пересохранение параметров
    // Проверка режима автонастройки U/F
    AutoSetUF1 = load_s16(ramPnt, progPnt->Pnt_AutoUF1);
    AutoSetUF2 = load_s16(ramPnt, progPnt->Pnt_AutoUF2);        
    BackupUF = 0;
    
    if (sPnt->nvMem.Autoset1 != AutoSetUF1 || sPnt->nvMem.Autoset2 != AutoSetUF2) {
      BackupUF = 1;
    } 
    if (!BackupUF) { // Если флаг уже стоит, значит изменили на авторежим
      // Проверка UF на изменения и запрос на сохранение в nv
      u16PntStartU1 = &(progPnt->Pnt_U5_1);
      u16PntStartU2 = &(progPnt->Pnt_U5_2);
      f32PntStartF1 = &(sPnt->nvMem.UF1.U5);
      f32PntStartF2 = &(sPnt->nvMem.UF2.U5);
      
      // Проверяем напряжения
      for (i=0; i < 6; i++, u16PntStartU1++, u16PntStartU2++, f32PntStartF1++, f32PntStartF2++) { // 6 точек по напряжению
          tempU1 = load_float(ramPnt, *u16PntStartU1);
          tempU2 = load_float(ramPnt, *u16PntStartU2);
          tempF1 = *f32PntStartF1;
          tempF2 = *f32PntStartF2;
          
          if (tempU1 != tempF1) {
            BackupUF = 1;
            save_s16(ramPnt, progPnt->Pnt_AutoUF1, 0);
          }
          if (tempU2 != tempF2) {
            BackupUF = 1;
            save_s16(ramPnt, progPnt->Pnt_AutoUF2, 0);          
          }
      }

      u16PntStartF1 = &(progPnt->Pnt_F5_1);
      u16PntStartF2 = &(progPnt->Pnt_F5_2);
      f32PntStartU1 = &(sPnt->nvMem.UF1.F5);
      f32PntStartU2 = &(sPnt->nvMem.UF2.F5);    
      // Проверяем Частоты
      for (i=0; i < 5; i++, u16PntStartF1++, u16PntStartF2++, f32PntStartU1++, f32PntStartU2++) { // 5 точек по частоте
          tempF1 = load_float(ramPnt, *u16PntStartF1);
          tempF2 = load_float(ramPnt, *u16PntStartF2);
          tempU1 = *f32PntStartU1;
          tempU2 = *f32PntStartU2;
          
          if (tempU1 != tempF1) {
            BackupUF = 1;          
            save_s16(ramPnt, progPnt->Pnt_AutoUF1, 0);          
          }
                      
          if (tempU2 != tempF2) { 
            BackupUF = 1;
            save_s16(ramPnt, progPnt->Pnt_AutoUF2, 0);           
          }
      } 
    }
    
    if (BackupUF) { // Проверка флага на обновление данных
        LOAD_UF_FROM_MICRO_TO_SPNT(progPnt, ramPnt, sPnt);        
        sPnt->nvMem.crc = crcPacket((unsigned char *)&(sPnt->nvMem),  (2*sizeof(UF_struct_type)));// Ставим метку, что данные валидные
        WrFramMicro(nvAdr, &(sPnt->nvMem), sizeof(UF5_F2SnvState_type));      
    }    
    
    // Проверка режима автонастройки U/F
    AutoSetUF1 = load_s16(ramPnt, progPnt->Pnt_AutoUF1);
    AutoSetUF2 = load_s16(ramPnt, progPnt->Pnt_AutoUF2);    
    if (AutoSetUF1 == AUTOSET_UF && GlobalM4.EnginParam.Fnom != 0 && GlobalM4.EnginParam.Usn !=0) {
        // Настраиваем указатели
        u16PntStartU1 = &(progPnt->Pnt_U5_1);
        u16PntStartF1 = &(progPnt->Pnt_F5_1);        
          
        KF = GlobalM4.EnginParam.Fnom / (TYPICAL_F[0]);// Коэффициент пересчета частоты
        KU = GlobalM4.EnginParam.Usn/ GlobalM4.EnginParam.Fnom;// Коэффициент пересчета частоты в напряжение   
        
        // Заполняем частоту с точки 0
        for (tmp = 0; tmp < (sizeof(TYPICAL_F)/sizeof(float)); tmp ++, u16PntStartF1++, u16PntStartU1++) {
            F = FLOAT_to_IQ(TYPICAL_F[tmp] * KF, Herz_base); // Заполняем данные по частоте
            U = FLOAT_to_IQ((TYPICAL_F[tmp] * KF * KU), Volt_base); // Заполняем данные по напряжению
            save_float (ramPnt, *u16PntStartU1, U);
            save_float (ramPnt, *u16PntStartF1, F);
        }
        save_float (ramPnt, *u16PntStartU1, 0);

        if (BackupUF || sPnt->Fnom != GlobalM4.EnginParam.Fnom || sPnt->Usn != GlobalM4.EnginParam.Usn) { // Проверка флага на обновление данных при выборе автонастройки, либо смена данных двигателя
            LOAD_UF_FROM_MICRO_TO_SPNT(progPnt, ramPnt, sPnt);        
            sPnt->nvMem.crc = crcPacket((unsigned char *)&(sPnt->nvMem),  (2*sizeof(UF_struct_type)));// Ставим метку, что данные валидные
            WrFramMicro(nvAdr, &(sPnt->nvMem), sizeof(UF5_F2SnvState_type));      
        } 
    }
      
    if (AutoSetUF2 == AUTOSET_UF && GlobalM4.EnginParam.Fnom != 0 && GlobalM4.EnginParam.Usn !=0) {
        // Настраиваем указатели
        u16PntStartU2 = &(progPnt->Pnt_U5_2);                
        u16PntStartF2 = &(progPnt->Pnt_F5_2);
          
        KF = GlobalM4.EnginParam.Fnom / (TYPICAL_F[0]);// Коэффициент пересчета частоты
        KU = GlobalM4.EnginParam.Usn/ GlobalM4.EnginParam.Fnom;// Коэффициент пересчета частоты в напряжение   
               
        // Заполняем частоту с точки 0
        for (tmp = 0; tmp < (sizeof(TYPICAL_F)/sizeof(float)); tmp ++, u16PntStartF2++, u16PntStartU2++) {
            F = FLOAT_to_IQ(TYPICAL_F[tmp] * KF, Herz_base); // Заполняем данные по частоте
            U = FLOAT_to_IQ((TYPICAL_F[tmp] * KF * KU), Volt_base); // Заполняем данные по напряжению
            save_float (ramPnt, *u16PntStartU2, U);
            save_float (ramPnt, *u16PntStartF2, F);            
        }
        save_float (ramPnt, *u16PntStartU2, 0);

        if (BackupUF || sPnt->Fnom != GlobalM4.EnginParam.Fnom || sPnt->Usn != GlobalM4.EnginParam.Usn) { // Проверка флага на обновление данных при выборе автонастройки, либо смена данных двигателя
            LOAD_UF_FROM_MICRO_TO_SPNT(progPnt, ramPnt, sPnt);        
            sPnt->nvMem.crc = crcPacket((unsigned char *)&(sPnt->nvMem),  (2*sizeof(UF_struct_type)));// Ставим метку, что данные валидные
            WrFramMicro(nvAdr, &(sPnt->nvMem), sizeof(UF5_F2SnvState_type));      
        }         
    }
              
    //********************************************************
    // РАСЧЕТ ХАРАКТЕРИСТИКИ UF
    F = fabsf(load_float(ramPnt, progPnt->Pnt_F));
    
    if (testBitMicro(ramPnt, progPnt->Pnt_PickUp) == 0){
        state_GT = load_s16(ramPnt, progPnt->Pnt_GTstate) & EquRV;      // 
        time = load_s16(ramPnt, progPnt->Pnt_TimeUSt);              // Время подема U пуска

        if(testBitMicro(ramPnt, progPnt->Pnt_Reset) == 1){          // -- Стоим, ШИМ отключен
	    Tfiltr = 0;	                                            // Фильтровать в стопе нет необходимости
            sPnt->TimeUStart = 0.0f;
            clrBitMicro(ramPnt, progPnt->Pnt_WaitUF);
        }
        else if(state_GT == EquRV){                                     // -- Если работаем на пусковой частоте
      	    if(sPnt->TimeUStart == 0.0f){                           // На время подъема напряжения замораживаем генератор темпа
                setBitMicro(ramPnt, progPnt->Pnt_WaitUF);
            }
            if(testBitMicro(ramPnt, progPnt->Pnt_WaitUF) == 1){
                sPnt->TimeUStart += (time == 0) ? 1.0f: ((float)(INT_TIME_CALL_MICRO_mS) / (float)time);
                if (sPnt->TimeUStart > 1.0f){
                    sPnt->TimeUStart = 1.0f;
                    clrBitMicro(ramPnt, progPnt->Pnt_WaitUF);       // Поднияли напряжение и разморозили генератор темпа
                }
            }
        }
    }
    else{
        sPnt->TimeUStart = 1.0f;
        clrBitMicro(ramPnt, progPnt->Pnt_WaitUF);               // разморозили генератор темпа
    }

//-- Выбор характеристики
    if(testBitMicro(ramPnt, progPnt->Pnt_Reset) == 1){ // ШИМ отключен, можно выбрать новую UF
      Num = load_s16(ramPnt, progPnt->Pnt_SelNum);
      if(Num >= 2){
        Num = testBitMicro(ramPnt, progPnt->Pnt_DinSel);
      }
      sPnt->NumUF = Num;
    }
    //-- Вычитка переменнх для работы UF
    if(sPnt->NumUF == 0){
	u16PntF	= &(progPnt->Pnt_F5_1);
	u16PntU	= &(progPnt->Pnt_U5_1);
    }else{
	u16PntF	= &(progPnt->Pnt_F5_2);
	u16PntU	= &(progPnt->Pnt_U5_2);
    }
	
    u16PntStart = u16PntF;
    iqPnt	= Ftmp;					        // F5
    for(tmp = 5; tmp > 0; tmp--, iqPnt++, u16PntF++){           // вычитываем во временный буфер F5, F4, F3, F2, F1	
        *iqPnt = load_float(ramPnt, *u16PntF);
    }

    u16PntF	= u16PntStart;
    iqPntF	= &(sPnt->F5);
    iqPntU	= &(sPnt->U5);
    iqPnt	= Ftmp;
    
    for(tmp = 5; tmp > 0; tmp--, u16PntF++, u16PntU++, iqPnt++, iqPntF++, iqPntU++){	// проверяем на превышение
        if(iqPnt[1] > *iqPnt){								// F4>F5		F3>F4		F2>F3		F1>F2
            iqPnt[1] = *iqPnt;								// F4=F5		F3=F4		F2=F3		F1=F2
            save_float(ramPnt, *u16PntF, (float)*iqPnt);
        }
        *iqPntF = FiltrValue(Tfiltr, *iqPnt, *iqPntF);
        //temp = load_float(ramPnt, *u16PntU);
        *iqPntU = FiltrValue(Tfiltr, load_float(ramPnt, *u16PntU), *iqPntU);
    }
    
    *iqPntU = FiltrValue(Tfiltr, load_float(ramPnt, *u16PntU),*iqPntU );        // точка    U0 
    
    //-- Расчёт
    if     (F >  sPnt->F4)  U = f32_Fy_x1x2y1y2x(sPnt->F4, sPnt->F5, sPnt->U4, sPnt->U5, F); // !!!F = заменил на U = iq_Fy....
    else if(F >  sPnt->F3)  U = f32_Fy_x1x2y1y2x(sPnt->F3, sPnt->F4, sPnt->U3, sPnt->U4, F); // !!!F = заменил на U = iq_Fy....
    else if(F >  sPnt->F2)  U = f32_Fy_x1x2y1y2x(sPnt->F2, sPnt->F3, sPnt->U2, sPnt->U3, F); // !!!F = заменил на U = iq_Fy....
    else if(F >  sPnt->F1)  U = f32_Fy_x1x2y1y2x(sPnt->F1, sPnt->F2, sPnt->U1, sPnt->U2, F); // !!!F = заменил на U = iq_Fy....
    else                    U = f32_Fy_x1x2y1y2x(0       , sPnt->F1, sPnt->U0, sPnt->U1, F); // !!!F = заменил на U = iq_Fy....
        
    GlobalM4.Utab_uf = U;	// Напряжение U/F для подхвата, к которому мы будем разгонять напряжение после нахождения частоты
    if (testBitMicro(ramPnt, progPnt->Pnt_PickUp) == 0){
        U = U * sPnt->TimeUStart;
    }
    else{
        U = load_float(ramPnt, progPnt->Pnt_mull_U);
    }

    if(fabsf(U) > sPnt->U5)    
        U = sPnt->U5;
    
    save_float(ramPnt, progPnt->Pnt_U, U);
    //GlobalM4.Debug1 = U;    
    
    if ((AutoSetUF1) || (AutoSetUF2)) {
        sPnt->Fnom = GlobalM4.EnginParam.Fnom;
        sPnt->Usn  = GlobalM4.EnginParam.Usn;    
    } else {
        sPnt->Fnom = 0;
        sPnt->Usn  = 0; 
    }
    //sPnt->SyncAutosetData = 0;
    
    //передача в обменную структуру времени подъёма U пуска
    GlobalM4.timeUI_Up = load_s16(ramPnt, progPnt->Pnt_TimeUSt);
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUF5_F2S_1[40]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  mull_U
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  F
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  F5_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  F4_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  F3_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  F2_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  F1_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  U5_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  U4_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  U3_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  U2_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  U1_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  U0_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  F5_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  F4_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  F3_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  F2_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  F1_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19  U5_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20  U4_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  U3_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22  U2_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //23  U1_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //24  U0_2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  GTstate
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //26  Reset
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  TimeUSt
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //28  PickUp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  Tfiltr
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  SelNum
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //31  DinSel
	S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  AutoUF1
	S16_PIN_TYPE | OUTPUT_PIN_MODE,    //33  AutoUF2
	S16_PIN_TYPE | OUTPUT_PIN_MODE,    //34  I_Hold
	S16_PIN_TYPE | OUTPUT_PIN_MODE,    //35  I0_UF1
	S16_PIN_TYPE | OUTPUT_PIN_MODE,    //36  I0_UF2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //37  ResOut
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //38  WaitUF
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //39  U
	S32_PIN_TYPE | OUTPUT_PIN_MODE     //40  I          
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UF5_F2S", "DA", TblFuncUF5_F2S_1, FuncUF5_F2S_1, sizeof(TblFuncUF5_F2S_1), sizeof(UF5_F2SState_type), sizeof(UF5_F2SnvState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUF5_F2S_1,
#endif
//********************************************************