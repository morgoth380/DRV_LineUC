#include "math.h"
#include "Global_include.h"
#include "Pjk_const.h"

#ifdef    GET_FUNC_CODE

typedef struct{
	u16  Pnt_Nfunc;
	    //u16  Pnt_State;
	    //u16  Pnt_nvState;
	    u16  Pnt_Unom;                         //1  Unom
	    u16  Pnt_Inom;                         //2  Inom
	    u16  Pnt_EdIzmFerq;                    //3  EdIzmFerq
	    u16  Pnt_EdIzmPid;                     //4  EdIzmPid
	    u16  Pnt_StepenPid;                    //5  StepenPid
	    u16  Pnt_MaxPid;                       //6  MaxPid
	    u16  Pnt_MinPid;                       //7  MinPid
	    u16  Pnt_ParPolus;                     //8  ParPolus
	    u16  Pnt_Dshkiv;                       //9  Dshkiv
	    u16  Pnt_KredChisl;                    //10 KredChisl
	    u16  Pnt_KredZnam;                     //11 KredZnam
	    u16  Pnt_LiftNomSpd;                   //12 Pnt_LiftNomSpd
	    u16  Pnt_NomMotFrq;                    //13 Pnt_NomMotFrq
            u16  Pnt_LiftEdIzm;                    //14 Pnt_LiftEdIzm
            u16  Pnt_Functional;                   //15 Pnt_Functional
	    u16  Pnt_FreqUser;                     //16 FreqUser
	    u16  Pnt_VoltBase;                     //17 VoltBase
	    u16  Pnt_Wt;                           //18 Wt
	    u16  Pnt_X_XProc;                      //19 X_XProc
	    u16  Pnt_X_XHz;                        //20 X_XHz
	    u16  Pnt_RefPid;                       //21 RefPid
	    u16  Pnt_Base8;                        //22 Base8
	    u16  Pnt_Base9;                        //23 Base9
	    u16  Pnt_Base10;                       //24 Base10
	    u16  Pnt_Base11;                       //25 Base11
	    u16  Pnt_Base12;                       //26 Base12
	    u16  Pnt_Base13;                       //27 Base13
	    u16  Pnt_Base14;                       //28 Base14
	    u16  Pnt_Base15;                       //29 Base15
	    u16  Pnt_SE_rot;                       //30 SE_rot
	    u16  Pnt_SE_PID;                       //31 SE_PID
	    u16  Pnt_Torq_Frm;                     //32 Torq_Frm
	    u16  Pnt_End;
}FuncBASE_CALC_type;

const u16 TblSE_rot[7] = {HzRasm | (1 << 8), HzRasm | (2 << 8), obMinRsm | (0 << 8), rad_sec_hRasm | (1 << 8), m_SRasm | (2 << 8)};
//********************************************************
const u16 Tbl_Nom_Voltage[4] = {
  220,
  380,
  480,
  690
};
//********************************************************
typedef enum{
	Hz_X_X,
        Hz_X_XX,
	Rpm_sew,
	RadS_sew,
	ms_sew,
}FREQUNITStype_enum;
#define	FREQHZ_BASE		800		//80.0 Гц
#define INDEX_OFFSET 2
//********************************************************
u16 *FuncBASE_CALC_1(FuncBASE_CALC_type *progPnt, u32 ramPnt){
  s16 Base, EdIzmFerq, Index, EdIzmPid, StepenPid;
  u8 numBase;
  float Unom, Inom, Zp, Kch, Kz, DiamPulley;         
  AT24Functional_type Functional;
  f32   LiftNomSpd, NomMotFrq;
  u16   LiftEdIzm, IndexUnom;
  
  MPOpis_type             *mPnt;
  mPnt = (MPOpis_type *)&MPOpis;
  
  //---------------------------------------------------------------------------
#ifdef _PROJECT_FOR_STM32_
  // Блок теста калибровочных значений для привода. Здесь мы найдем где лежат наши калибровки и если они целые
  // мы их переместим в правильное место.
  nvAinCalState_type       *nvAdrAin, *nvAdrAinTest;
  nvAoutCalState_type      *nvAdrAout;
  nvAdjCoef_type           *nvAdrCoef;
  nvEngineParam_type       *nvEngineParam;
  FileFunctional_type      *nvFunctional;  
  
  nvAinCalState_type       nvMemAin;
  nvAoutCalState_type      nvMemAout;  
  nvAdjCoef_type           nvMemAdjCoef;
  nvEngineParam_type       nvMemEngineParam;  
  FileFunctional_type      nvMemFunctional;  
  s16                      nvCalibrOffset;
  
  // Блок flagIsFirstCall для анализа данных калибровок, на случай когда идет обновление ПО с разной системой параметров.
  if(flagIsFirstCall)                                         
  {
    // Тестовая оценка адреса калибровочных значений.
    // Зная что сдвиг у меня сейчас на 4 байта, сделаем сканирование на -8....+8 байт, с шагом в 2 байта.
    // Адрес начала поиска по Ан.входу       
    
    GlobalM4.AdjCoefFault = 0;
    // Проверяем адрес расположения калибровок
    nvAdrCoef          = (nvAdjCoef_type* )((s32)&nv.adjCoefcnts);
    if ((u32)nvAdrCoef != 0x7F94) {
      // Если не совпадает с четким адресом - ставим аварию
      GlobalM4.AdjCoefFault = 1;
    }
    
    #if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    u16			CalibrCode[7];
    nvAdrAin            = (nvAinCalState_type* )&nv.AInput.CalibrCode;    
    
    for (nvCalibrOffset = -8; nvCalibrOffset <= 8; nvCalibrOffset += 2){
      nvAdrAinTest = (nvAinCalState_type *)((s32)nvAdrAin + nvCalibrOffset);
      nvAdrCoef          = (nvAdjCoef_type* )((s32)&nv.adjCoefcnts);
      RdFramMicro((void*)&CalibrCode, nvAdrAinTest->CalibrCode[0], sizeof(CalibrCode));	
      if(!Crc((u8*)(&CalibrCode), sizeof(CalibrCode))) {
        
        // Если смещение равно нулю, то мы не делаем никаких манипуляций
        if (nvCalibrOffset == 0)        break;
        
        // Если смещение не нулевое - то делаем перенос калибровок
        // Нашли корректное смещение, и теперь вычитываем все данные калибровок
        nvAdrAin           = (nvAinCalState_type *)((s32)&nv.AInput + nvCalibrOffset); 
        nvAdrAout          = (nvAoutCalState_type* )((s32)&nv.AOutput + nvCalibrOffset);
        nvAdrCoef          = (nvAdjCoef_type* )((s32)&nv.adjCoefcnts + nvCalibrOffset);
        nvEngineParam      = (nvEngineParam_type* )((s32)&nv.nvEngineParam + nvCalibrOffset);
        nvFunctional       = (FileFunctional_type* )((s32)&nv.fileCurFunct + nvCalibrOffset);

        // Копируем калибровки в резервную память.
        RdFramMicro((void*)&nvMemAin, nvAdrAin, sizeof(nvAinCalState_type));	
        RdFramMicro((void*)&nvMemAout, nvAdrAout, sizeof(nvAoutCalState_type));	
        RdFramMicro((void*)&nvMemAdjCoef, nvAdrCoef, sizeof(nvAdjCoef_type));	
        RdFramMicro((void*)&nvMemEngineParam, nvEngineParam, sizeof(nvEngineParam_type));
        RdFramMicro((void*)&nvMemFunctional, nvFunctional, sizeof(FileFunctional_type));

        // А теперь размещаем копии калибровок по нулевым смещениям.
        nvAdrAin           = (nvAinCalState_type *)((s32)&nv.AInput); 
        nvAdrAout          = (nvAoutCalState_type* )((s32)&nv.AOutput);
        nvAdrCoef          = (nvAdjCoef_type* )((s32)&nv.adjCoefcnts);
        nvEngineParam      = (nvEngineParam_type* )((s32)&nv.nvEngineParam);

        WrFramMicro(nvAdrAin, (void*)&nvMemAin, sizeof(nvAinCalState_type));	
        WrFramMicro(nvAdrAout, (void*)&nvMemAout, sizeof(nvAoutCalState_type));	
        WrFramMicro(nvAdrCoef, (void*)&nvMemAdjCoef, sizeof(nvAdjCoef_type));	
        WrFramMicro(nvEngineParam, (void*)&nvMemEngineParam, sizeof(nvEngineParam_type));
	WrFramMicro(nvFunctional, (void*)&nvMemFunctional, sizeof(FileFunctional_type));
        
        break; //Найдено правильное смещение
      }        
    #endif
      
    // Тестовая оценка адреса калибровочных значений СР24   
    // Адрес начала поиска по Ан.выходу  
    #if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
    u16			CalibrCode[5]; // 5 для Aout
    nvAdrAout          = (nvAoutCalState_type* )((s32)&nv.AOutput);
    nvAdrCoef          = (nvAdjCoef_type* )((s32)&nv.adjCoefcnts);

    for (nvCalibrOffset = -4; nvCalibrOffset <= 8; nvCalibrOffset += 2){
      //nvAdrAinTest = (nvAinCalState_type *)((s32)nvAdrAin + nvCalibrOffset);
      nvAdrAout          = (nvAoutCalState_type* )((s32)&nv.AOutput + nvCalibrOffset);      
      RdFramMicro((void*)&CalibrCode, nvAdrAout->CalibrCode[0], sizeof(CalibrCode));	
      if(!Crc((u8*)(&CalibrCode), sizeof(CalibrCode))) {
        
        // Если смещение равно нулю, то мы не делаем никаких манипуляций
        if (nvCalibrOffset == 0)        break;
        
        // Если смещение не нулевое - то делаем перенос калибровок
        // Нашли корректное смещение, и теперь вычитываем все данные калибровок
        nvAdrAin           = (nvAinCalState_type *)((s32)&nv.AInput + nvCalibrOffset); 
        nvAdrAout          = (nvAoutCalState_type* )((s32)&nv.AOutput + nvCalibrOffset);
        nvAdrCoef          = (nvAdjCoef_type* )((s32)&nv.adjCoefcnts + nvCalibrOffset);
        nvEngineParam      = (nvEngineParam_type* )((s32)&nv.nvEngineParam + nvCalibrOffset);
        nvFunctional       = (FileFunctional_type* )((s32)&nv.fileCurFunct + nvCalibrOffset);

        // Копируем калибровки в резервную память.
        RdFramMicro((void*)&nvMemAin, nvAdrAin, sizeof(nvAinCalState_type));	
        RdFramMicro((void*)&nvMemAout, nvAdrAout, sizeof(nvAoutCalState_type));	
        RdFramMicro((void*)&nvMemAdjCoef, nvAdrCoef, sizeof(nvAdjCoef_type));	
        RdFramMicro((void*)&nvMemEngineParam, nvEngineParam, sizeof(nvEngineParam_type));
        RdFramMicro((void*)&nvMemFunctional, nvFunctional, sizeof(FileFunctional_type));

        for(u8 numAin = 0; numAin < 2; numAin++) {           // вычитать калибровки аналоговых входов
          // Проверить на старый формат данных калибровок
          if (nvMemAin.CalibrCode[numAin][4] !=0 && !nvMemAin.CalibrCode[numAin][5] && !nvMemAin.CalibrCode[numAin][6])
            nvMemAin.CalibrCode[numAin][4] = 0; // Обнулить старый CRC, 
          nvMemAin.CalibrCode[numAin][6] = Crc((unsigned char *)(nvMemAin.CalibrCode[0]), sizeof(nvAdrAin->CalibrCode[0]) - sizeof(u16));    // Записываем значение crc          
        }
        
        // А теперь размещаем копии калибровок по нулевым смещениям.
        nvAdrAin           = (nvAinCalState_type *)((s32)&nv.AInput); 
        nvAdrAout          = (nvAoutCalState_type* )((s32)&nv.AOutput);
        nvAdrCoef          = (nvAdjCoef_type* )((s32)&nv.adjCoefcnts);
        nvEngineParam      = (nvEngineParam_type* )((s32)&nv.nvEngineParam);

        WrFramMicro(nvAdrAin, (void*)&nvMemAin, sizeof(nvAinCalState_type));	
        WrFramMicro(nvAdrAout, (void*)&nvMemAout, sizeof(nvAoutCalState_type));	
        WrFramMicro(nvAdrCoef, (void*)&nvMemAdjCoef, sizeof(nvAdjCoef_type));	
        WrFramMicro(nvEngineParam, (void*)&nvMemEngineParam, sizeof(nvEngineParam_type));
	WrFramMicro(nvFunctional, (void*)&nvMemFunctional, sizeof(FileFunctional_type));
        
        break; //Найдено правильное смещение
      }      
    #endif
    
    } 
    
  } // if(flagIsFirstCall) 
  #endif
  //---------------------------------------------------------------------------
  Functional = (AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_Functional); // UseLiftFunc
  
  // Получаем значение базы по напряжению.
  // В версиях до 168.51.2 / 168.01.0 номинальное напряжение ПЧ задавалось из списка 220/380/480/690
  // Потом заменили на задаваемое значение с предустановлен максимальным значением, и пользователь теперь 
  // может конфигурировать ПЧ под свою сеть, которая ниже предельного значения входного напряжения
  // Для стыковки старых версий делаем проверку
  IndexUnom = load_s16(ramPnt, progPnt->Pnt_Unom);
  if (IndexUnom < 4) { // Если значение в параметре ниже 4, значит тянутся уставки из старого формата и надо заменить значение в уставке на правильное значение напряжения 
    Unom = (float)Tbl_Nom_Voltage[load_s16(ramPnt, progPnt->Pnt_Unom)]; // Номинальное напряжение ПЧ берем из списка
    save_s16(ramPnt, progPnt->Pnt_Unom, (u16)Unom); // и переписываем его значение в уставке
  } else {  // Ну а если уже задано числовое значение напряжения, то просто копируем его и все.
    Unom = (float)load_s16(ramPnt, progPnt->Pnt_Unom); // Номинальное напряжение ПЧ
  }
  
  // Получаем значение базы по току
  Inom = (float)load_s16(ramPnt, progPnt->Pnt_Inom);                  // Номинальный ток ПЧ                         
  
  EdIzmFerq = load_s16(ramPnt, progPnt->Pnt_EdIzmFerq);               // Единицы измерения по частоте
  Zp = (float)load_s16(ramPnt, progPnt->Pnt_ParPolus);                // Количество пар полюсов
  
  // база по напряжению
  save_s16(ramPnt, progPnt->Pnt_VoltBase, (u16)Unom);
  // 1) База по мощности
  // Формула для расчета  P = I*U/1000, а с округлением (I*U/1000 + 1/2)=(I*U + 500)/1000
  Base = (s16)(((Unom * Inom) / 1000.0f) + 0.5f);
  save_s16(ramPnt, progPnt->Pnt_Wt, Base);
  
  // 2) База процентов
  save_s16(ramPnt, progPnt->Pnt_X_XProc,  1000);		// 100.0
  
  // 3) База по частоте
  save_s16(ramPnt, progPnt->Pnt_X_XHz, FREQHZ_BASE);
  
  // 4) База для ПИД
  Base = load_s16(ramPnt, progPnt->Pnt_MaxPid);         // Максимум ПИД
  Base = Base - load_s16(ramPnt, progPnt->Pnt_MinPid);  // Минимум ПИД
  save_s16(ramPnt, progPnt->Pnt_RefPid, Base);          // Сохраняем базу для ПИД
  
  // Сформировать формат для ПИД
  EdIzmPid = load_s16(ramPnt, progPnt->Pnt_EdIzmPid);                    // Считываем единицы измерения для ПИД-регулятора
  StepenPid = load_s16(ramPnt, progPnt->Pnt_StepenPid);                  // Считываем степень для ПИД-регулятора
  save_s16(ramPnt, progPnt->Pnt_SE_PID, (EdIzmPid | (StepenPid << 8)));  // Сохраняем формат для ПИД-регулятора
  
  // 5) База пользовательской скорости
  if (Functional != UseLiftFunc) {
      switch(EdIzmFerq){
         case Hz_X_X:{
            Base = FREQHZ_BASE;
         }break;
         case Hz_X_XX:{
            Base = FREQHZ_BASE * 10;
         }break;
         case Rpm_sew:{
            Base = (u16)((((float)FREQHZ_BASE * 6.0f) / Zp) + 0.5f);  // 800*6/Zp,
         }break;
         case RadS_sew:{
            Base = (u16)(((float)FREQHZ_BASE * _2PI) + 0.5f);    // 800 * 2*Pi
         }break;
         case ms_sew:{
            Kch = (float)load_s16(ramPnt, progPnt->Pnt_KredChisl);
            Kz = (float)load_s16(ramPnt, progPnt->Pnt_KredZnam);
            DiamPulley = (float)load_s16(ramPnt, progPnt->Pnt_Dshkiv);
              
            Base = (u16)( ((Kch / Kz) * (((float)FREQHZ_BASE * _2PI) / Zp) * (DiamPulley / (1000.0f * 2.0f))) + 0.5f);   // Расчет
         }break;
      }
      // Индекс единиц измерения
      Index = EdIzmFerq;                                                         //  (EdIzmFerq == Relative) ? EdIzmFerq + INDEX_OFFSET : EdIzmFerq;    // Индекс в масиве      
      
  } else { // If set Lift, check user setting   
      LiftEdIzm = load_s16(ramPnt, progPnt->Pnt_LiftEdIzm);

      switch(LiftEdIzm){
         case 0/*Hz_X_X*/:{
            Base = FREQHZ_BASE;
            Index = 0; // Таблица TblSE_rot
         }break;
         case 1 /*ms_sew*/:{
           LiftNomSpd = load_s16(ramPnt, progPnt->Pnt_LiftNomSpd); // Два знака после запятой
           NomMotFrq = load_s16(ramPnt, progPnt->Pnt_NomMotFrq);   // Один знак после запятой
           Base = (u16)(100.0f*FREQHZ_BASE/ NomMotFrq * (LiftNomSpd / 100.f));
           Index = 4; // Таблица TblSE_rot
         }break;
      }    
  }
  save_s16(ramPnt, progPnt->Pnt_FreqUser, Base);
  // Сформировать формат для параметров с пользовательской базой по частоте
  save_s16(ramPnt, progPnt->Pnt_SE_rot,  TblSE_rot[Index]);                  // 
  
  // 6) База по моменту
  Base = (u16)(Unom * Inom * (float)Zp / ((float)FREQHZ_BASE * 4.18879020478639f));  // Степень базы по току и частоте учтена (она сократилась)
  save_s16(ramPnt, progPnt->Pnt_Base8, Base);                                        // 4.18879020478639=4/3*pi
    
  // 7) Остальные Базы пока =0 - при необходимости добавить
  save_s16(ramPnt, progPnt->Pnt_Base9, 0);
  save_s16(ramPnt, progPnt->Pnt_Torq_Frm, 0);
  save_s16(ramPnt, progPnt->Pnt_Base9, 0);

// запись значений баз во Float      
  
  // для пользовательской частоты делаем пересчет c учетом знаков после запятой
  mPnt->BaseFloat[FreqUser_base]  = (float)load_s16(ramPnt, progPnt->Pnt_FreqUser) / (powf(10.0f, (TblSE_rot[Index] >> 8)));
  mPnt->BaseFloat[Volt_base]      = (float)Unom;
  mPnt->BaseFloat[Current_base]   = (float)load_s16(ramPnt, progPnt->Pnt_Inom) * 0.1f;
  mPnt->BaseFloat[Power_base]     = (float)load_s16(ramPnt, progPnt->Pnt_Wt) * 0.1f;
  mPnt->BaseFloat[Proc_base]      = (float)load_s16(ramPnt, progPnt->Pnt_X_XProc) * 0.1f;
  mPnt->BaseFloat[Herz_base]      = (float)load_s16(ramPnt, progPnt->Pnt_X_XHz) * 0.1f;
  mPnt->BaseFloat[RefPID_base]    = (float)load_s16(ramPnt, progPnt->Pnt_RefPid) / (powf(10.0f, StepenPid));
  mPnt->BaseFloat[Torq_base]      = (float)load_s16(ramPnt, progPnt->Pnt_Base8);

  for(numBase = FreqUser_base; numBase < (Torq_base + 1); numBase++){
	  if (mPnt->BaseFloat[numBase]) {
          mPnt->BaseFloatConverse[numBase] = 1.f / mPnt->BaseFloat[numBase];
	  }
  }
  
  return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncBASE_CALC_1[32]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Unom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Inom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  EdIzmFerq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  EdIzmPid
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  StepenPid
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  MaxPid
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  MinPid
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  ParPolus
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Dshkiv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  KredChisl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  KredZnam
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  LiftNomSpd
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  NomMotFrq
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  LiftEdIzm        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Functional        
        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  FreqUser
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  VoltBase
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  Wt
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //19  X_XProc
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //20  X_XHz
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //21  RefPid
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Base8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  Base9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  Base10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  Base11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  Base12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  Base13
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  Base14
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  Base15
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  SE_rot
	S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  SE_PID
	S16_PIN_TYPE | OUTPUT_PIN_MODE     //32  Torq_Frm
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("BASE_CALC", "U", TblFuncBASE_CALC_1, FuncBASE_CALC_1, sizeof(TblFuncBASE_CALC_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncBASE_CALC_1,
#endif
//********************************************************
