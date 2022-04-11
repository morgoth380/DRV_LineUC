#ifdef    GET_FUNC_CODE

#include "Global_include.h"         // Include header for IQmath library

typedef struct{
    s16         AddState;
    s16         CntState;
    f32		TorqueMaxFiltr;
    f32		TorqueMinFiltr;
    f32		K1_Filtr;
    f32		K2_Filtr;
    f32		WrFiltr;
}StateFuncTORQUE_MAX_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Power_nom;                    //1  Power_nom
    u16  Pnt_Power_Dvig;                   //2  Power_Dvig 
    u16  Pnt_Power_Gen;                    //3  Power_Gen
    u16  Pnt_Tmax_Pult;                    //4  Tmax_Pult
    u16  Pnt_Tmin_Pult;                    //5  Tmin_Pult
    u16  Pnt_Select_Pmode;                 //6  Select_Pmode
    u16  Pnt_Reserve07;                    //7  Reserve07
    u16  Pnt_Tmax_Imax;                    //8  Tmax_Imax
    u16  Pnt_Reserve09;                    //9  Reserve09
    u16  Pnt_Choice_Imax;                  //10  Choice_Imax
    u16  Pnt_Reserve11;                    //11  Reserve11
    u16  Pnt_Reserve12;                    //12  Reserve12
    u16  Pnt_Param_T;                      //13  Param_T
    u16  Pnt_Param_P;                      //14  Param_P
    u16  Pnt_Param_I;                      //15  Param_I
    u16  Pnt_Choice_Tmax;                  //16  Choice_Tmax
    u16  Pnt_ASU_Tmin;                     //17  ASU_Tmin
    u16  Pnt_ASU_Tmax;                     //18  ASU_Tmax
    u16  Pnt_Base_T_Out;                   //19  Base_T_Out
    u16  Pnt_Base_P_Out;                   //20  Base_P_Out
    u16  Pnt_Base_I_Out;                   //21  Base_I_Out
    u16  Pnt_ProfiEn;			   //22  ProfiEn
    u16  Pnt_ZIP_OUT4;                     //23  ZIP_OUT4
    u16  Pnt_ZIP_OUT3;                     //24  ZIP_OUT3
    u16  Pnt_ZIP_OUT2;                     //25  ZIP_OUT2
    u16  Pnt_TminASU;                      //26  TminASU
    u16  Pnt_TmaxASU;                      //27  TmaxASU
    u16  Pnt_Tmax_I;                       //28  Tmax_I
    u16  Pnt_TmaxPgen;                     //29  TmaxPgen
    u16  Pnt_TmaxPdv;                      //30  TmaxPdv
    u16  Pnt_Tmin;                         //31  Tmin
    u16  Pnt_Tmax;                         //32  Tmax
    u16  Pnt_End;
}FuncTORQUE_MAX_type;

#define	ARasm	        5	 // А
#define	kWRasm	        12	 // кВт
#define	ProcRasm	30	 // %
#define	Nm_hRasm	59	 // Нм

#define OFF_VAL		0
#define ABSOLUTE_VAL	1
#define PERCENT_VAL	2

#define	T_FILTRATION	0.4f	 // Время фильтрации ограничения момента составит 1 секунду (3*tau)


const u16 TblSE_TorqLim[2] = { Nm_hRasm|(1 << 8),ProcRasm| (1 << 8)};		//проверить порядок};		//проверить порядок
const u16 TblSE_PowerLim[2] = { kWRasm|(1 << 8),ProcRasm| (1 << 8)};		//проверить порядок};		//проверить порядок
const u16 TblSE_CURRLim[2] = { ARasm|(1 << 8),ProcRasm| (1 << 8)};

f32 Wtekrotor = 0;
//********************************************************
u16 *FuncTORQUE_MAX_1(FuncTORQUE_MAX_type *progPnt, u32 ramPnt)
{
      f32 Tmax, Tmin, TmaxPult, TminPult, TmaxAsu, TminAsu, TmaxPdv, TmaxGen, TmaxI ;
      f32 Ptemp,Wtek,Wtek1;
      f32 temp;
      u16 SelectMode, profiCtrlEnable;
      u16 PowerNom;
      f32	LocalTorqueBase;
      StateFuncTORQUE_MAX_type *sPnt;

      sPnt = (StateFuncTORQUE_MAX_type *)(ramPnt + progPnt->Pnt_State);
      PowerNom = 0;

      if(flagIsFirstCall){
          sPnt->TorqueMaxFiltr = 0;
          sPnt->TorqueMinFiltr = 0;
          sPnt->K1_Filtr = T_FILTRATION / (T_FILTRATION + FLOAT_TIME_CALL_MICRO);
          sPnt->K2_Filtr = FLOAT_TIME_CALL_MICRO / (T_FILTRATION + FLOAT_TIME_CALL_MICRO);
          sPnt->WrFiltr = 0;
          
          return &progPnt->Pnt_End;
      }

      profiCtrlEnable = 0;//load_s16(ramPnt, progPnt->Pnt_ProfiCntrlEn);

      //===== выбор по местному пульту ====== //
      SelectMode = load_s16(ramPnt, progPnt->Pnt_Choice_Tmax);
      LocalTorqueBase = GlobalM4.EnginParam.Torque_n * 10.0f;                 // Выбираем в качестве базы по моменту для пульта, номинальный момент.
      
      switch (SelectMode) {
          case OFF_VAL:
          case ABSOLUTE_VAL:
              save_s16(ramPnt, progPnt->Pnt_Param_T, TblSE_TorqLim[0]);
              save_s16(ramPnt, progPnt->Pnt_Base_T_Out,(u16)LocalTorqueBase); // BASE9 // GlobalM3.TorqBASE
          break;

          case PERCENT_VAL:
              save_s16(ramPnt, progPnt->Pnt_Param_T, TblSE_TorqLim[1]);
              save_s16(ramPnt, progPnt->Pnt_Base_T_Out, 1000);                // BASE9
          break;
      }
      
      TmaxPult = load_float(ramPnt, progPnt->Pnt_Tmax_Pult);                // момент с пульта макс - приведенный к базе
      Tmax = TmaxPult = TmaxPult * LocalTorqueBase;                         // T_Max - переводим в абсолютные единицы для сравнения
      TminPult = load_float(ramPnt, progPnt->Pnt_Tmin_Pult);                // момент с пульта мин  - приведенный к базе
      Tmin = TminPult = TminPult * LocalTorqueBase;                         // T_Min - переводим в абсолютные единицы для сравнения

      //==== Выбор режима ограничения мощности ===== //
      SelectMode  = load_s16(ramPnt, progPnt->Pnt_Select_Pmode);
      PowerNom    = load_s16(ramPnt, progPnt->Pnt_Power_nom);               // Значение в кВт
      switch (SelectMode){
      case PERCENT_VAL:
        save_s16(ramPnt, progPnt->Pnt_Param_P, TblSE_PowerLim[1]);
        save_s16(ramPnt, progPnt->Pnt_Base_P_Out, 1000);              // BASE10
        break;
        
      case ABSOLUTE_VAL:
      case OFF_VAL:
        save_s16(ramPnt, progPnt->Pnt_Param_P, TblSE_PowerLim[0]);
        save_s16(ramPnt, progPnt->Pnt_Base_P_Out,(s16)(PowerNom));    // BASE10
        break;
      }

      TmaxPdv = 0;
      TmaxGen = 0;
      if(SelectMode != 0){
        if(GlobalM4.Fref == 0){ 
          Wtek = sPnt->WrFiltr = GlobalM4.EnginParam.Wnom;	
        }
        else{
          Wtek = fabsf(GlobalM4.Wr);
        }
        
        sPnt->WrFiltr = 0.999f*sPnt->WrFiltr + 0.001f*Wtek;
        Wtek = sPnt->WrFiltr;
        Wtek1 = Wtek * 6.283185307179586f / (f32)GlobalM4.EnginParam.Zp;
        
        TmaxPdv = load_float(ramPnt, progPnt->Pnt_Power_Dvig);
        Ptemp  = TmaxPdv * PowerNom * 1000.f;
        TmaxPdv  = Ptemp/Wtek1;
        
        TmaxGen = load_float(ramPnt, progPnt->Pnt_Power_Gen);
        Ptemp  = TmaxGen * PowerNom * 1000.f;
        TmaxGen  = Ptemp / Wtek1;
        
        if(Tmax > TmaxPdv)	Tmax = TmaxPdv;
        if(Tmin > TmaxGen)	Tmin = TmaxGen;
      }
	 
      save_s16(ramPnt, progPnt->Pnt_TmaxPdv,  (u16)(TmaxPdv * 1.0f));
      save_s16(ramPnt, progPnt->Pnt_TmaxPgen, (u16)(TmaxGen * 1.0f));

      // ======= выбор по ограничению тока =========
      SelectMode = load_s16(ramPnt, progPnt->Pnt_Choice_Imax);

      switch (SelectMode){
      case OFF_VAL:
      case ABSOLUTE_VAL:
        save_s16(ramPnt, progPnt->Pnt_Param_I, TblSE_CURRLim[0]);
        save_s16(ramPnt, progPnt->Pnt_Base_I_Out, (u16)(GlobalM4.EnginParam.Isn * 10.0f));  //BASE11
        break;
        
      case PERCENT_VAL:
        save_s16(ramPnt, progPnt->Pnt_Param_I, TblSE_CURRLim[1]);
        save_s16(ramPnt, progPnt->Pnt_Base_I_Out, 1000); //BASE11
        break;
      }

      if(SelectMode != 0){
        TmaxI = IQ_to_FLOAT(load_float(ramPnt, progPnt->Pnt_Tmax_Imax), Current_base);
        
        //преобразовать в момент
        temp = GlobalM4.PhiRTorqIact;
        
        if(temp != 0){
          temp = temp * GlobalM4.FluxBASE * 1.5f * (float)GlobalM4.EnginParam.Zp * GlobalM4.EnginParam.Lm / (GlobalM4.EnginParam.Lm + GlobalM4.EnginParam.Lls);
          TmaxI = TmaxI * temp * 10.0f; //мин ограничен
          
          save_s16(ramPnt, progPnt->Pnt_Tmax_I, (u16)TmaxI);
          if(Tmax > TmaxI)	Tmax = TmaxI;
          if(Tmin > TmaxI)	Tmin = TmaxI;
        }
      }

      //=================== ограничение с АСУ
      // В чем приходит с АСУ
      TminAsu = ((u16)load_s16(ramPnt, progPnt->Pnt_ASU_Tmin) / 1.f);
      TmaxAsu = ((u16)load_s16(ramPnt, progPnt->Pnt_ASU_Tmax) / 1.f);
 
      save_s16(ramPnt, progPnt->Pnt_TmaxASU, (u16)(TmaxAsu * 1.0f));
      save_s16(ramPnt, progPnt->Pnt_TminASU, (u16)(TminAsu * 1.0f));

      // Если управление по профинет активировано, проверяем задание момента
      if(profiCtrlEnable) {
          if(Tmax > TmaxAsu) Tmax = TmaxAsu;
          if(Tmin > TminAsu) Tmin = TminAsu;
      }

      // Выполнить фильтрацию результирующего момента ограничения
      sPnt->TorqueMinFiltr = sPnt->TorqueMinFiltr * sPnt->K1_Filtr + Tmin * sPnt->K2_Filtr;
      sPnt->TorqueMaxFiltr = sPnt->TorqueMaxFiltr * sPnt->K1_Filtr + Tmax * sPnt->K2_Filtr;

      Tmax = sPnt->TorqueMaxFiltr;
      Tmin = sPnt->TorqueMinFiltr;

      // Выдаем Фильтрованные значения ограничения в микропрограмму.
      save_s16(ramPnt, progPnt->Pnt_Tmax, (u16)(Tmax + 0.5f));
      save_s16(ramPnt,progPnt->Pnt_Tmin,  (u16)(Tmin + 0.5f));

      // Передаем ограничение в глобальные переменные
      GlobalM4.TorqueMax = FLOAT_to_IQ(Tmax, Torq_base) * 0.1f;     // 0.1f - так как один знак после запятой 
      GlobalM4.TorqueMin = FLOAT_to_IQ(Tmin, Torq_base) * 0.1f;     // 

      return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTORQUE_MAX_1[32]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Power_nom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Power_Dvig
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Motor_Max
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Gener_Ust%
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Gener_Max
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Choice_Pow
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Imax_Motion
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Imax%
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Imax_Zad
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Choice_Imax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Tnom
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Tmax%
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  TmaxZad
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Tmin%
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  TminZad
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Choice_Tmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  ASU_Tmin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  ASU_Tmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  ZIP_TORR1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  ZIP_TORR2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  ZIP_TORR3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  ZIP_TORR4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  ZIP_OUT4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  ZIP_OUT3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //25  ZIP_OUT2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //26  ZIP_OUT1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  Tmax4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  Tmax3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  Tmax2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  Tmax1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  Tmin
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //32  Tmax
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TORQUE_MAX", "U", TblFuncTORQUE_MAX_1, FuncTORQUE_MAX_1, sizeof(TblFuncTORQUE_MAX_1),sizeof(StateFuncTORQUE_MAX_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTORQUE_MAX_1,
#endif
//********************************************************
