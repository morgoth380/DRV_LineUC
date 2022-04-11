#ifdef    GET_FUNC_CODE
#include    "GT_DEFs.h"
#include    "Pjk_const.h"

typedef struct{
    union{
        struct{
            u16 PWM_Off  : 1; // ШИМ отключен
            u16 Stop_DC  : 1; // Торможение постоянным током
            u16 PickUpOn : 1; // Работает модуль подхвата
            u16 AlrmFref : 1; // flag alarm Fref < fPusk;
            u16 Reserv   : 12;
        }bit;
        u16 all;
    }flag;
}local_GT_type;

#define		ZERO_HZ_FREQ	0.0
#define		_0_3_HZ_FREQ	(0.3/80.f)
//------------------------------------------------------

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Fref;                         // 1  Fref         //Pnt_IN
    u16  Pnt_UpIn;                         // 2  UpIn           // not used
    u16  Pnt_DwnIn;                        // 3  DwnIn          // not used
    u16  Pnt_ZerTime;                      // 4  ZerTime        // zero (обнулитель времён разгона и торможения)
    u16  Pnt_PickUpEn;                     // 5  PickUpEn
    u16  Pnt_GTtype;                       // 6  GTtype      //Pnt_Type
    u16  Pnt_TimeScale;                    // 7  TimeScale   //Pnt_TimeScale
    u16  Pnt_Fmax;                         // 8  Fmax        //Pnt_FrS
    u16  Pnt_AccTime;                      // 9  AccTime     //Pnt_TimeUp
    u16  Pnt_DecTime;                      //10  DecTime     //Pnt_TimeDw
    u16  Pnt_Stype;                        //11  Stype       //Pnt_t2
    u16  Pnt_tA1;                          //12  tA1
    u16  Pnt_tA2;                          //13  tA2
    u16  Pnt_tA3;                          //14  tA3
    u16  Pnt_tA4;                          //15  tA4
    u16  Pnt_AccFhold;                     //16  Pnt_F_Pusk             //Pnt_ResVal
    u16  Pnt_AccThold;                     //17  Pnt_TimeFpusk          //Pnt_TfixFpusk
    u16  Pnt_DecFhold;                     //18  DecFhold
    u16  Pnt_DecThold;                     //19  DecThold
    u16  Pnt_CurlesTime;                   //20  CurlesTime  //Pnt_Tpause
    u16  Pnt_F_PU;                         //21  F_PU
    u16  Pnt_ResByPWM;                     //22  ResByPWM
    u16  Pnt_ResByDC;                      //23  ResByDC
    u16  Pnt_FreqStop;                     //24  StopFreq     //Pnt_Freeze
    u16  Pnt_Freeze2;                      //25  Freeze2    
    u16  Pnt_ForbidFreq;                   //26  ForbidFreq
    u16  Pnt_ForbidTacc;                   //27  ForbidTacc
    u16  Pnt_ForbidTdec;                   //28  ForbidTdec        
    u16  Pnt_JaccStart;			   //29  JaccStart
    u16  Pnt_AccelJ;			   //30  AccelJ
    u16  Pnt_JaccEnd;			   //31  JaccEnd
    u16  Pnt_JdecStart;			   //32  JaccStart
    u16  Pnt_DeccelJ;			   //33  AccelJ
    u16  Pnt_JdecEnd;			   //34  JaccEnd
    u16  Pnt_Sin2Mode;			   //35  Sin2Mode    
    u16  Pnt_vPhirUp;                      //36  vPhirUp
    u16  Pnt_AlrmFref;                     //37  AlrmFref
    u16  Pnt_FrefS16;                      //38  HldOnDel    //Pnt_Fp_delay
    u16  Pnt_Format;                       //39  Format
    u16  Pnt_HldOnFq;                      //40  HldOnFq     //Pnt_EquRV
    u16  Pnt_ZerroFq;                      //41  ZerroFq     //Pnt_EqZero
    u16  Pnt_GTstate;                      //42  GTstate     //Pnt_Status
    u16  Pnt_Dwn;                          //43  Dwn
    u16  Pnt_Up;                           //44  Up
    u16  Pnt_Fout;                         //45  Fout        //Pnt_Out
    u16  Pnt_End;
}FuncTEMP_LSU_2_type;
//*************************************************************************
//u16 FormModeGT(FuncTEMP_LSU_2_type *progPnt, u32 ramPnt);
#ifndef _PROJECT_FOR_STM32_
    #define PI       3.14159265358979F
#endif
void CalcSin2LSU(Temp_koef_type *sPnt, u16 Mode, f32 tAcc);
extern u16 ModeGT;


//*************************************************************************
u16 *FuncTEMP_LSU_2_1(FuncTEMP_LSU_2_type *progPnt, u32 ramPnt)
{
    Temp_koef_type	*sPnt;
    GlobalVar	        *mPnt;
    local_GT_type       loc;  
    s16			TestFref;
    f32		        tempSin, FVt ; // Переменные для синусного генератора темпа
    u16			tempRes, tempFreeze, Wait_UF, GTtype, tempTimeScale, Sin2Mode;
    f32                 utempFrSOld, utempFin,tempFrSOld, tempF_in, tempResVal, f_PU, t_1, t_2, t_3, t_4, TlinUp, TlinDn;
    u16                 tempTup, tempTdn;	
    f32                 oldFinFloat, tekFinFloat;
    sPnt 		= (Temp_koef_type *)(ramPnt + progPnt->Pnt_State); 
    mPnt		= &GlobalM4;
  


    if (GlobalM4.FlagCommand.bit.RingUpFreezeGT) {
        return &progPnt->Pnt_End;
    } 

    // Доработка для подхвата - Шахов
    tempResVal      = load_float(ramPnt, progPnt->Pnt_AccFhold); 
    utempFrSOld = GlobalM4.Fmax = load_float(ramPnt, progPnt->Pnt_Fmax); // Фиксируем максимальную частоту в систему параметров
    tempFrSOld      = utempFrSOld;//Так сделано чтобы не изменять типы во всей функции
    utempFin = GlobalM4.FinLsu = load_float(ramPnt, progPnt->Pnt_Fref);	                // Входное задание скорости
    
    GlobalM4.FixStopTime = load_s16(ramPnt, progPnt->Pnt_DecThold) * (K_TIME_CALL_MICRO_TAKT * 0.1f); 
    
    if(((s32)utempFin > 0) && (utempFin > utempFrSOld)) {
        utempFin = utempFrSOld; 
    } //Так сделано чтобы не изменять типы во всей функции
    else if(((s32)utempFin < 0) && (utempFin < -utempFrSOld)){ 
        utempFin =-utempFrSOld; 
    } //Так сделано чтобы не изменять типы во всей функции

    tempF_in            = utempFin;
    loc.flag.bit.PWM_Off  = testBitMicro(ramPnt, progPnt->Pnt_ResByPWM); //ШИМ откл == 1, вкл == 0
    loc.flag.bit.Stop_DC = testBitMicro(ramPnt, progPnt->Pnt_ResByDC); //Торможение постоянным током
    loc.flag.bit.PickUpOn = GlobalM4.PickupEnable = testBitMicro(ramPnt, progPnt->Pnt_PickUpEn); //Включен подхват == 1, отключен == 0
    tempRes         = ((!loc.flag.bit.PWM_Off) && (!loc.flag.bit.Stop_DC) && (!loc.flag.bit.PickUpOn));
    tempFreeze = Wait_UF = testBitMicro(ramPnt, progPnt->Pnt_Freeze2);
    GTtype        = load_s16(ramPnt, progPnt->Pnt_GTtype);
    tempTimeScale   = load_s16(ramPnt, progPnt->Pnt_TimeScale);

    if(flagIsFirstCall){
    	sPnt->TfixFpusk = 0;
    	sPnt->State.all = 0;
    	sPnt->StateGT.all = 0;
    	tempRes = 0;
    	clrBitMicro(ramPnt, progPnt->Pnt_AlrmFref);
    }

    tempTup		= load_s16(ramPnt, progPnt->Pnt_AccTime);  //Время разгона
    tempTdn		= load_s16(ramPnt, progPnt->Pnt_DecTime);  //Время торможения
    
    // Запоминаем значение времени разгона, для применения в измерении параметров двигателя
    GlobalM4.OriginAccelTime = tempTup * 0.1f;

    if (sPnt->TfixFpusk > 0){
        tempFreeze |= 0x1;
        sPnt->TfixFpusk--;
    }
    if((sPnt->Tpause > 0) && (tempRes == 1)){
        tempFreeze |= 0x1;
        sPnt->Tpause--;
    }
    //формирование формата
    save_s16(ramPnt, progPnt->Pnt_Format, (21 | ((tempTimeScale & 0xFF) << 8)));
    
    oldFinFloat = (s32)(IQ_to_FLOAT(sPnt->uFrefOld, FreqUser_base)*100);
    tekFinFloat = (s32)(IQ_to_FLOAT(utempFin, FreqUser_base)*100);
    
    if (loc.flag.bit.PWM_Off){ // Если ШИМ отключен - выключаем флаг возбуждения
          clrBitMicro (ramPnt, progPnt->Pnt_vPhirUp);
    }
    
    // Анализ бита-сигнала, что сейчас запрещенная частота.
    if (testBitMicro(ramPnt, progPnt->Pnt_ForbidFreq)) { 
      tempTup		= load_s16(ramPnt, progPnt->Pnt_ForbidTacc);  //Время разгона при запрещенных частотах
      tempTdn		= load_s16(ramPnt, progPnt->Pnt_ForbidTdec);  //Время торможения при запрещенных частотах
      //tempType = 0;  // Генератор темпа ставим линейным, но на выходе он станет тем что был задан исходно.
    }
    
    //Проверка на необходимость пересчета коэффициентов
    if((sPnt->uFrSOld != utempFrSOld) || (sPnt->TupOld != tempTup) || (sPnt->TdnOld != tempTdn)|| 
       (oldFinFloat != tekFinFloat)   || (flagIsFirstCall == 1))
	{
    	if (sPnt->StateGT.flazhok == 0){
            f32 t2, t3, t2_1, t3_1, t2_2, t3_2, t2_3, t3_3;                                                             //переменные для тквадрат и ткуб
            f32 FlinDn, FlinUp, FlinDn1, FlinUp1, FlinDn2, FlinDn3, FlinUp3;
            f32 tempF1, tempFold, absOldFout, absFin;
            u16 tempT;

            sPnt->OldF_out = sPnt->Fout;                                                        //присваиваю старой частоте текущую

            if((sPnt->OldF_out == 0.f) && (tempResVal != 0.f) && (utempFin != 0))               //если текущая частота 0 то прыгнуть на Fпуск
            {                                                   
                sPnt->State.ToRefF = 1;
                sPnt->State.Recalc = 1;
            }
            else {
                if(((sPnt->OldF_out > 0.f) && (tempF_in > 0.f)) ||
                ((sPnt->OldF_out < 0.f) && (tempF_in < 0.f))){
                    sPnt->State.ToUp = 1;
                }
                if( (sPnt->State.ToUp == 0) && (tempResVal != 0.f)&& 
                   ((sPnt->OldF_out == tempResVal) || (sPnt->OldF_out == -tempResVal))){    //если спускались по частоте и дошли до Fпуск
                        sPnt->State.ToZero = 1;                                             //то надо прыгнуть в 0
                }
                else {
                    if(tempF_in != 0.f){
                            sPnt->State.NeedZero = 0;                                   //устанавливаем бит необходимости прыгнуть в 0
                    }
                    if((tempF_in == 0.f) && (sPnt->OldF_out != 0.f)) {                  //если задание 0                                                                                                                            //то спускаемся до пусковой
                        sPnt->State.Recalc = 1;
                        sPnt->State.NeedZero = 1;                                   //устанавливаем бит необходимости прыгнуть в 0                                                                                                                 //после отработки задания
                        tempF_in = (sPnt->OldF_out > 0.f) ? tempResVal: -tempResVal;
                        sPnt->tempFin = tempF_in;
                        sPnt->uFrefOld   = utempFin;
                    }
                    else {
                        if((sPnt->State.ToZero == 0) &&                                     //если реверс то едем до F пусковой
                           (((sPnt->OldF_out > 0.f) && (tempF_in < 0.f)) ||
                                ((sPnt->OldF_out < 0.f) && (tempF_in > 0.f)))){
                            sPnt->StateGT.RevCur = 1;
                            sPnt->StateGT.RevRef = 1;
                            tempF_in = (tempF_in > 0.f) ? -tempResVal: tempResVal;
                            sPnt->tempFin = tempF_in;
                            sPnt->State.Revers = 1;
                            sPnt->State.Recalc = 1;
                            sPnt->uFrefOld   = utempFin;
                        }
                        else {                                                            //после проезда до F пуск выезжаем на задание
                            sPnt->State.Recalc = 1;
                            absFin = (tempF_in > 0.f) ? tempF_in : -tempF_in;
                            if((absFin < tempResVal) && (tempF_in != 0.f)) {
                                tempF_in = 0.f;
                            }
                            sPnt->tempFin = tempF_in;
                            sPnt->uFrefOld   = utempFin;
                        }
                    }
                }
            }

            if(sPnt->State.Recalc) {
                sPnt->State.Recalc = 0;
                sPnt->FrSOld     = tempFrSOld;
                sPnt->uFrSOld    = utempFrSOld;                                                 //Новое значение максимальной частоты
                sPnt->FrefOld    = tempF_in;
                sPnt->TdnOld     = tempTdn;                                                     //Новое значение времени торможения
                sPnt->TupOld     = tempTup;                                                     //Новое значение времени разгона

                absOldFout = (sPnt->OldF_out > 0.f) ? sPnt->OldF_out : -sPnt->OldF_out;
                absFin = (tempF_in > 0.f) ? tempF_in : -tempF_in;
                tempF1 = absFin;
                tempFold = absOldFout;
                
                if(absOldFout < absFin) {
                    // Расчет коэффициентов для разгонной кривой
                    tempFold = tempF1 - tempFold;
                    tempF1 = sPnt->FrefOld - sPnt->OldF_out;
                    tempT = tempTup;
                    sPnt->State.ToUp = 1;
                    sPnt->StateGT.Dwn = 0;
                    sPnt->StateGT.Up = 1;
                    sPnt->StateGT.EqRefVal=0;
                }
                else {
                    // Расчет коэффициентов для кривой торможения
                    tempFold -= tempF1;
                    tempF1 = sPnt->OldF_out - sPnt->FrefOld;
                    tempT = tempTdn;
                    sPnt->State.ToUp = 0;
                    sPnt->StateGT.Dwn = 1;
                    sPnt->StateGT.Up = 0;
                    sPnt->StateGT.EqRefVal=0;
                }
                //вычисляем время до выхода на Fin от текущей частоты
                if(tempFrSOld != 0) {
                    sPnt->TfupOld = (tempFold * (tempT * TimeScale[tempTimeScale])) / tempFrSOld;
                }
                else 
                    sPnt->TfupOld = 0.f;
                if (load_s16(ramPnt, progPnt->Pnt_Stype) == 1){
                    t_1 = load_float(ramPnt, progPnt->Pnt_tA1);
                    t_2 = load_float(ramPnt, progPnt->Pnt_tA2);
                    t_4 = load_float(ramPnt, progPnt->Pnt_tA3);
                    t_3 = load_float(ramPnt, progPnt->Pnt_tA4);

                    if (t_1 < 0.07f) t_1 = 0.07f;
                    if (t_2 < 0.07f) t_2 = 0.07f;
                    if (t_3 < 0.07f) t_3 = 0.07f;
                    if (t_4 < 0.07f) t_4 = 0.07f;

                    if (t_1 > 0.85f) t_1 = 0.85f;
                    if (t_2 > 0.85f) t_2 = 0.85f;
                    if (t_3 > 0.85f) t_3 = 0.85f;
                    if (t_4 > 0.85f) t_4 = 0.85f;

                    if ((1.f - t_1 - t_2) < 0.05f) t_1 -= (0.05f - (1.0f - t_1 - t_2));
                    if ((1.f - t_3 - t_4) < 0.05f) t_3 -= (0.05f - (1.0f - t_3 - t_4));

                    FlinDn = tempF1 * (t_1 - 0.035f);                    //Вычисление верхней и нижней точек перегиба
                    FlinUp = tempF1 - FlinDn;                                                       //по оси частоты
                    FlinDn1 = tempF1 * (t_2 - 0.035f);                    //Вычисление верхней и нижней точек перегиба
                    FlinUp1 = tempF1 - FlinDn1;                                                       //по оси частоты
                    FlinDn2 = tempF1 * (t_3 - 0.035f);                    //Вычисление верхней и нижней точек перегиба
                    FlinDn3 = tempF1 * (t_4 - 0.035f);                    //Вычисление верхней и нижней точек перегиба
                    FlinUp3 = tempF1 - FlinDn3;                                                       //по оси частоты

                    sPnt->TlinDn = sPnt->TfupOld * t_1;      //Вычисление нижней и верхней точек перегиба
                    sPnt->TlinUp = sPnt->TfupOld - sPnt->TlinDn;                                    //по оси времени
                    sPnt->TlinDn1 = sPnt->TfupOld * t_2;      //Вычисление нижней и верхней точек перегиба
                    sPnt->TlinUp1 = sPnt->TfupOld - sPnt->TlinDn1;                                    //по оси времени
                    sPnt->TlinDn2 = sPnt->TfupOld * t_3;      //Вычисление нижней и верхней точек перегиба
                    sPnt->TlinUp2 = sPnt->TfupOld - sPnt->TlinDn2;                                    //по оси времени
                    sPnt->TlinDn3 = sPnt->TfupOld * t_4;      //Вычисление нижней и верхней точек перегиба
                    sPnt->TlinUp3 = sPnt->TfupOld - sPnt->TlinDn3;                                    //по оси времени

                    t2 = sPnt->TlinDn * sPnt->TlinDn;
                    t3 = sPnt->TlinDn * sPnt->TlinDn * sPnt->TlinDn;
                    t2_1 = sPnt->TlinDn1 * sPnt->TlinDn1;
                    t3_1 = sPnt->TlinDn1 * sPnt->TlinDn1 * sPnt->TlinDn1;
                    t2_2 = sPnt->TlinDn2 * sPnt->TlinDn2;
                    t3_2 = sPnt->TlinDn2 * sPnt->TlinDn2 * sPnt->TlinDn2;
                    t2_3 = sPnt->TlinDn3 * sPnt->TlinDn3;
                    t3_3 = sPnt->TlinDn3 * sPnt->TlinDn3 * sPnt->TlinDn3;
                    // Разгон нижний диапазон
                    if(sPnt->TlinDn != 0.f) {
                        sPnt->KoefK = (FlinUp1 - FlinDn) / (sPnt->TlinUp1 - sPnt->TlinDn);          //вычисление коэфициента К для прямого участка
                        sPnt->KoefA = ((sPnt->KoefK  * sPnt->TlinDn) - (2 * FlinDn)) / t3;          //вычисление коэфициента А для полинома
                        sPnt->KoefB = ((3 * FlinDn) - (sPnt->KoefK * sPnt->TlinDn)) / t2;           //вычисление коэфициента Б для полинома
                        sPnt->KoefBLin = (sPnt->KoefA * t3 + sPnt->KoefB * t2)                      //вычисление коэфициента Б для прямого участка
                                                         - sPnt->KoefK * sPnt->TlinDn;
                        sPnt->LinKoefK = tempF1 / sPnt->TfupOld;                                    //вычисление коэффициента К для линейного                                                                                                                                          // генератора темпа
                    }
                    else {
                        sPnt->KoefK    = 0.f;
                        sPnt->KoefA    = 0.f;
                        sPnt->KoefB    = 0.f;
                        sPnt->KoefBLin = 0.f;
                    }

                    // Разгон верхний диапазон
                    if(sPnt->TlinDn1 != 0.f) {
                        sPnt->KoefA_1 = ((sPnt->KoefK  * sPnt->TlinDn1) - (2 * FlinDn1)) / t3_1;          //вычисление коэфициента А для полинома
                        sPnt->KoefB_1 = ((3 * FlinDn1) - (sPnt->KoefK * sPnt->TlinDn1)) / t2_1;           //вычисление коэфициента Б для полинома
                        sPnt->KoefBLin_1 = (sPnt->KoefA_1 * t3_1 + sPnt->KoefB_1 * t2_1)                      //вычисление коэфициента Б для прямого участка
                                                         - sPnt->KoefK * sPnt->TlinDn1;                                                                                                                                                // генератора темпа
                    }
                    else {
                        sPnt->KoefK    = 0.f;
                        sPnt->KoefA    = 0.f;
                        sPnt->KoefB    = 0.f;
                        sPnt->KoefBLin = 0.f;
                    }

                    // Торможение верхний диапазон
                    if(sPnt->TlinDn2 != 0.f) {
                        sPnt->KoefK_3 = (FlinUp3 - FlinDn2) / (sPnt->TlinUp3 - sPnt->TlinDn2);            //вычисление коэфициента К для прямого участка
                        sPnt->KoefA_2 = ((sPnt->KoefK_3  * sPnt->TlinDn2) - (2 * FlinDn2)) / t3_2;          //вычисление коэфициента А для полинома
                        sPnt->KoefB_2 = ((3 * FlinDn2) - (sPnt->KoefK_3 * sPnt->TlinDn2)) / t2_2;           //вычисление коэфициента Б для полинома
                        sPnt->KoefBLin_2 = (sPnt->KoefA_2 * t3_2 + sPnt->KoefB_2 * t2_2)                      //вычисление коэфициента Б для прямого участка
                                                         - sPnt->KoefK_3 * sPnt->TlinDn2;																			// генератора темпа
                    }
                    else {
                        sPnt->KoefK    = 0.f;
                        sPnt->KoefA    = 0.f;
                        sPnt->KoefB    = 0.f;
                        sPnt->KoefBLin = 0.f;
                    }

                    // Торможение нижний диапазон
                    if(sPnt->TlinDn3 != 0.f) {
                        sPnt->KoefA_3 = ((sPnt->KoefK_3  * sPnt->TlinDn3) - (2 * FlinDn3)) / t3_3;          //вычисление коэфициента А для полинома
                        sPnt->KoefB_3 = ((3 * FlinDn3) - (sPnt->KoefK_3 * sPnt->TlinDn3)) / t2_3;           //вычисление коэфициента Б для полинома
                        sPnt->KoefBLin_3 = (sPnt->KoefA_3 * t3_3 + sPnt->KoefB_3 * t2_3)                      //вычисление коэфициента Б для прямого участка
                                                         - sPnt->KoefK_3 * sPnt->TlinDn3;															// генератора темпа
                    }
                    else {
                        sPnt->KoefK    = 0.f;
                        sPnt->KoefA    = 0.f;
                        sPnt->KoefB    = 0.f;
                        sPnt->KoefBLin = 0.f;
                    }
                }
                else{
                    FlinDn = tempF1 * 0.2f;                    //Вычисление верхней и нижней точек перегиба
                    FlinUp = tempF1 - FlinDn;                                                       //по оси частоты
                    sPnt->TlinDn = sPnt->TfupOld * 0.3f;      //Вычисление нижней и верхней точек перегиба
                    sPnt->TlinUp = sPnt->TfupOld - sPnt->TlinDn;                                    //по оси времени
                    t2 = sPnt->TlinDn * sPnt->TlinDn;
                    t3 = sPnt->TlinDn * sPnt->TlinDn * sPnt->TlinDn;
                    
                    if(sPnt->TlinDn != 0.f) {
                        sPnt->KoefK = (FlinUp - FlinDn) / (sPnt->TlinUp - sPnt->TlinDn);            //вычисление коэфициента К для прямого участка
                        sPnt->KoefA = ((sPnt->KoefK  * sPnt->TlinDn) - (2 * FlinDn)) / t3;          //вычисление коэфициента А для полинома
                        sPnt->KoefB = ((3 * FlinDn) - (sPnt->KoefK * sPnt->TlinDn)) / t2;           //вычисление коэфициента Б для полинома
                        sPnt->KoefBLin = (sPnt->KoefA * t3 + sPnt->KoefB * t2)                      //вычисление коэфициента Б для прямого участка
                                                         - sPnt->KoefK * sPnt->TlinDn;
                        sPnt->LinKoefK = tempF1 / sPnt->TfupOld;                                    //вычисление коэффициента К для линейного
                        sPnt->ParKoefK = tempF1 / (sPnt->TfupOld * sPnt->TfupOld);                  //Вычисление коэффициента К для U-образного генератора темпа																			// генератора темпа
                    }
                    else {
                        sPnt->KoefK    = 0.f;
                        sPnt->KoefA    = 0.f;
                        sPnt->KoefB    = 0.f;
                        sPnt->KoefBLin = 0.f;
                    }
                   
                    sPnt->TlinDn3 = sPnt->TlinDn;
                    sPnt->TlinUp1 = sPnt->TlinUp2 = sPnt->TlinUp;
                    sPnt->KoefA_3 = sPnt->KoefA_2 = sPnt->KoefA_1 = sPnt->KoefA;
                    sPnt->KoefB_3 = sPnt->KoefB_2 = sPnt->KoefB_1 = sPnt->KoefB;
                    sPnt->KoefBLin_3 = sPnt->KoefBLin;
                    sPnt->KoefK_3 = sPnt->KoefK;
                }
            } // if(sPnt->State.Recalc)
            
            // Генератор Sin2
            // 09-12-2019 Сосновчик
            // Расчет параметров для S-образной кривой ускорения
            if ( (tempTup != 0) && (GTtype == 2) ) {
              absOldFout = (sPnt->OldF_out > 0.f) ? sPnt->OldF_out : -sPnt->OldF_out;
              absFin = (tempF_in > 0.f) ? tempF_in : -tempF_in;
              
              if (tempF_in < 0.f )
                sPnt->TsinRevers = 1;
              else if (sPnt->OldF_out < 0)
                sPnt->TsinRevers = 1;
              else
                sPnt->TsinRevers = 0;
              
              tempF1 = absFin;
              tempFold = absOldFout;
              
              if(absOldFout < absFin) {
                // Расчет коэффициентов для разгонной кривой
                tempFold = tempF1 - tempFold;
                tempF1 = sPnt->FrefOld - sPnt->OldF_out;
                
                tempT = tempTup;
                sPnt->State.ToUp = 1;
                sPnt->StateGT.Dwn = 0;
                sPnt->StateGT.Up = 1;
                sPnt->StateGT.EqRefVal = 0;
              }
              else {
                // Расчет коэффициентов для кривой торможения
                tempFold -= tempF1;
                tempF1 = sPnt->OldF_out - sPnt->FrefOld;
                
                tempT = tempTdn;
                sPnt->State.ToUp = 0;
                sPnt->StateGT.Dwn = 1;
                sPnt->StateGT.Up = 0;
                sPnt->StateGT.EqRefVal=0;
              }
              
              //вычисляем время до выхода на Fin от текущей частоты
              if(tempFrSOld != 0) sPnt->TfupOld = (tempFold * ((f32)tempT * TimeScale[tempTimeScale])) / tempFrSOld;
              else sPnt->TfupOld = 0.0f;
              
              // Не нулевое смещение убираем из расчета. Дальше по коду предыдущее значение частоты и так будет добавляться.
              sPnt->SinStartFreq = 0;
              sPnt->SinEndFreq = fabs(tempF_in-sPnt->OldF_out);
                            
              // Вызов функции расчета характеристики разгона
              Sin2Mode = load_s16(ramPnt, progPnt->Pnt_Sin2Mode);
              if (sPnt->State.ToUp) { // Идет разгон
                sPnt->JerkStart = load_float(ramPnt, progPnt->Pnt_JaccStart);
                sPnt->JerkEnd   = load_float(ramPnt, progPnt->Pnt_JaccEnd);
                sPnt->SinAcceleration = load_float(ramPnt, progPnt->Pnt_AccelJ);
              } else if (!sPnt->State.ToUp) { // Идет торможение
                sPnt->JerkStart = load_float(ramPnt, progPnt->Pnt_JdecStart);
                sPnt->JerkEnd   = load_float(ramPnt, progPnt->Pnt_JdecEnd);
                sPnt->SinAcceleration = load_float(ramPnt, progPnt->Pnt_DeccelJ);
              }
              
			  // Проверка времени разгона на ноль
			  if (sPnt->TfupOld != 0.0f) { // Если не ноль - делаем расчет кривой sin2
				CalcSin2LSU (sPnt, Sin2Mode, sPnt->TfupOld);
			  } else { // А если ноль - обнуляем все времена кривой.
				sPnt->t1Sin = sPnt->t2Sin = sPnt->tpSin = 0.0f;
			  }
              sPnt->TsinDn = sPnt->t1Sin;
              sPnt->TsinUp = sPnt->t2Sin;
              sPnt->TfupOld = sPnt->tpSin; // Если режим Jerk - то время будет совершенно другое, и его надо переопределить.
              
              // Общее время разгона
//              GlobalM3.timeUp = sPnt->tpSin;
              
            }
            
            // Конец генератора Sin2
            
            sPnt->State.Done = 0;                                                   //устанавливаю флаг того, что работа невыполнена
            //chemeris Debug 
            sPnt->CurTime = 0.f;
            sPnt->StateGT.flazhok = 1;
    	}
        else{
            sPnt->StateGT.flazhok = 0;
    	}
    }
    
    //-- Для корректной отработки Fпуск=0;
    if(tempRes && sPnt->StateGT.Reset) {
      sPnt->StateGT.all      = 0;
      sPnt->State.all        = 0;
      sPnt->State.ToRefF     = 1;
    }
        
    //проверяем не надо ли пускаться
    if(tempRes == 1) {
//#ifdef _PROJECT_FOR_STM32_
        if((fabsf(load_float(ramPnt, progPnt->Pnt_Fref))) < load_float(ramPnt, progPnt->Pnt_AccFhold)){
            if(!(testBitMicro(ramPnt, progPnt->Pnt_FreqStop)))
                loc.flag.bit.AlrmFref = 1;
            else
                loc.flag.bit.AlrmFref = 0;
            if(load_float(ramPnt, progPnt->Pnt_Fout) == 0){
                tempFreeze = 1;  //Морозим генератор темпа пока частота задания не будет выше частоты пуска
            }
        }

        else{
            loc.flag.bit.AlrmFref = 0;
        }
//#endif
        sPnt->StateGT.Reset = 0;
        if(!tempFreeze) {
            sPnt->StateGT.Freeze = 0;
            if(sPnt->State.Done == 0) {
                if(sPnt->State.ToRefF) {                        //если надо прыгнуть на пусковую
                    if(tempF_in >= 0.f) {                         //если задание положительное
                        sPnt->OldF_out = tempResVal;
                        sPnt->Fout = tempResVal;
                    }
                    else{                                      //если задание отрицательное
                        sPnt->OldF_out = -tempResVal;
                        sPnt->Fout = -tempResVal;
                    }
                    sPnt->State.ToRefF = 0;
                    sPnt->State.Done = 1;
                }
                else{
                    if(sPnt->State.ToZero){                    //если надо прыгнуть в 0

                        sPnt->OldF_out = 0.f;
                        sPnt->Fout = 0.f;

                        sPnt->State.ToZero = 0;
                        sPnt->State.Done = 1;
                    }
                    else{                                    //иначе просто едем по кривой
                        if(((TYPEDRV_ACIMFOC&(1<<mPnt->DriveType)) == 0)||(mPnt->FlagState.bit.vPrUp == 1)){ //!!!GoncharovKV ждем роста потока в векторе для АД
                            setBitMicro (ramPnt, progPnt->Pnt_vPhirUp); // Если работа генератора разрешена - ставим сигнал намагничения.
                            if(sPnt->CurTime < sPnt->TfupOld){
                                switch(GTtype){
                                case 0: // Linear
                                  //*********************************************************************************
                                  //         Вычисление приращения для линейного темпа
                                  //*********************************************************************************
                                  sPnt->Fcalc = sPnt->LinKoefK * sPnt->CurTime;
                                  break;
                                case 1: // S-curve
                                  //*********************************************************************************
                                  //         Вычисление приращения для S-образного темпа
                                  //*********************************************************************************
                                  if(sPnt->State.ToUp) {
                                    TlinDn = sPnt->TlinDn;
                                    TlinUp = sPnt->TlinUp1;
                                  }else{
                                    TlinDn = sPnt->TlinDn3;
                                    TlinUp = sPnt->TlinUp2;
                                  }
                                  if(sPnt->CurTime < TlinDn) {
                                    if(sPnt->State.ToUp) {
                                      sPnt->Fcalc = (sPnt->KoefA * sPnt->CurTime * sPnt->CurTime * sPnt->CurTime)
                                        + (sPnt->KoefB * sPnt->CurTime * sPnt->CurTime);
                                    }else{
                                      sPnt->Fcalc = (sPnt->KoefA_3 * sPnt->CurTime * sPnt->CurTime * sPnt->CurTime)
                                        + (sPnt->KoefB_3 * sPnt->CurTime * sPnt->CurTime);
                                    }
                                  }
                                  else {
                                    if((sPnt->CurTime >= TlinDn) && (sPnt->CurTime <= TlinUp)) {
                                      if (sPnt->State.ToUp)
                                        sPnt->Fcalc = sPnt->KoefK * sPnt->CurTime + sPnt->KoefBLin;
                                      else
                                        sPnt->Fcalc = sPnt->KoefK_3 * sPnt->CurTime + sPnt->KoefBLin_3;
                                    }
                                    else {
                                      f32 timeTemp;
                                      f32 freqTemp;
                                      timeTemp = sPnt->TfupOld - sPnt->CurTime;
                                      if(sPnt->State.ToUp) {
                                        freqTemp = sPnt->tempFin - sPnt->OldF_out;
                                        sPnt->Fcalc =  freqTemp - ((sPnt->KoefA_1 * timeTemp * timeTemp * timeTemp)
                                                                   + (sPnt->KoefB_1 * timeTemp * timeTemp));
                                      }
                                      else {
                                        freqTemp = sPnt->OldF_out - sPnt->tempFin;
                                        sPnt->Fcalc =  freqTemp - ((sPnt->KoefA_2 * timeTemp * timeTemp * timeTemp)
                                                                   + (sPnt->KoefB_2 * timeTemp * timeTemp));
                                      }
                                    }
                                  }       
                                  break;

                                case 2: // Sin2 - curve
                                  //sPnt->Fcalc =  sPnt->ParKoefK * (sPnt->CurTime * sPnt->CurTime);    			//U-образная кривая генератора темпа
                                  //*********************************************************************************
                                  //         Вычисление приращения для S-образного темпа ускорения - Синусный темп
                                  //*********************************************************************************
//                                  if(sPnt->State.ToUp) {
//                                    TsinDn = sPnt->TsinDn;
//                                    TsinUp = sPnt->TsinUp;
//                                  }else{
//                                    TsinDn = sPnt->TsinDn;
//                                    TsinUp = sPnt->TsinUp;
//                                  }
                                  if(sPnt->CurTime <= sPnt->t1Sin) {
                                    // Расчет разгонной части
                                    tempSin = (sin(PI*sPnt->CurTime / sPnt->t1Sin) * sPnt->t1Sin / (2*PI));
                                    FVt = (sPnt->CurTime*sPnt->CurTime/4) - tempSin * tempSin;
                                    sPnt->Fcalc = sPnt->JerkStart * FVt + sPnt->SinStartFreq;
                                  }
                                  else {
                                    if((sPnt->CurTime > sPnt->t1Sin) && (sPnt->CurTime < sPnt->t2Sin)) {
                                      //  Средняя зона
                                      sPnt->Fcalc = sPnt->SinAt1*(sPnt->CurTime - sPnt->t1Sin) + sPnt->SinVt1;
                                    }
                                    else {
                                      // Конечная выхода на частоту
                                      tempSin = (sin(PI * (sPnt->CurTime-sPnt->t2Sin) / sPnt->t3Sin) * sPnt->t3Sin / (2*PI));
                                      FVt = ((sPnt->CurTime - sPnt->t2Sin) * (sPnt->CurTime - sPnt->t2Sin) / 4) - tempSin * tempSin;
                                      sPnt->Fcalc = sPnt->SinAt1 * (sPnt->CurTime - sPnt->t2Sin) - sPnt->JerkEnd * FVt + sPnt->SinVt2;
                                    }
                                  }
                                  
                                  if(sPnt->TsinRevers) {
                                    // Если идет работа в минусах, берем со знаком минус
                                    sPnt->Fcalc = -sPnt->Fcalc;
                                  }
                                  
                                  break;
                                                                                
                                default:
                                  sPnt->Fcalc = 0.f;
                                  break ;
                                }
                                sPnt->CurTime += FLOAT_TIME_CALL_MICRO;

                                //*********************************************************************************
                                //         Вычисление частоты
                                //*********************************************************************************
                                if(sPnt->State.ToUp){
                                    sPnt->Fout = sPnt->OldF_out + sPnt->Fcalc;
                                }
                                else 
                                    sPnt->Fout = sPnt->OldF_out - sPnt->Fcalc;
                            }
                            else {                                    //если вышли на частоту
                                sPnt->OldF_out	 = sPnt->tempFin;
                                sPnt->Fout	        = sPnt->tempFin;
                                sPnt->State.Done        = 1;
                                if(sPnt->State.Revers){
                                    sPnt->StateGT.ReversHold    = 1;
                                    sPnt->uFrefOld              = utempFin - 1;
                                    sPnt->StateGT.RevCur        = 0;
                                    sPnt->StateGT.RevRef        = 0;
                                    sPnt->State.Revers	        = 0;
                                }
                                sPnt->StateGT.Dwn		= 0;
                                sPnt->StateGT.Up		= 0;
                                sPnt->StateGT.EqRefVal          = 1;
                                sPnt->StateGT.PickUpWork        = 0;
                            }
                        }
                    }
                }
            }
        }
        else {
            sPnt->StateGT.Freeze = 1;
        }
    }

    else if(loc.flag.bit.PWM_Off || loc.flag.bit.Stop_DC){ //Отключен ШИМ или происходит торможение постоянным током
        sPnt->StateGT.Reset     = 1;
        sPnt->StateGT.EqRefVal  = 1;
    	sPnt->StateGT.flazhok   = 1;
        sPnt->CurTime           = 0.f;
        sPnt->Fout              = 0;
        sPnt->OldF_out          = 0;
        sPnt->uFrSOld           = 0;
        sPnt->uFrefOld          = 0;
        sPnt->State.all         = 0;
        sPnt->State.Done        = 1;
    	sPnt->Tpause 	        = 0;
    	loc.flag.bit.AlrmFref   = 0;
    }
    else if(loc.flag.bit.PickUpOn){
	sPnt->uFrefOld         = load_float(ramPnt, progPnt->Pnt_F_PU); //считываем частоту с модуля подхвата
        f_PU = sPnt->uFrefOld;              // Подхват
        sPnt->StateGT.Reset             = 0;
        sPnt->StateGT.EqRefVal          = 1;
	sPnt->StateGT.EqResVal          = 1;
    	sPnt->StateGT.flazhok           = 1;		                //Нет пересчета коэф.
    	sPnt->StateGT.PickUpWork        = 1;
        sPnt->CurTime                   = 0.f;
        sPnt->Fout                      = f_PU;                 // было = 0.f
        sPnt->OldF_out                  = f_PU;                 // было = 0.f
        sPnt->F_PU                      = f_PU;
        sPnt->uFrSOld                   = utempFrSOld;	        //Fmax
        sPnt->State.all                 = 0;
        sPnt->State.Done                = 1;
    	sPnt->Tpause                    = 0;
        sPnt->TfixFpusk                 = 0;
        tempFreeze                      = 0;
        sPnt->State.ToRefF              = 0;
    	loc.flag.bit.AlrmFref           = 0;
    }

    if((sPnt->State.NeedZero == 1) && (sPnt->State.Done == 1)) {        //если задание было 0
        sPnt->State.NeedZero = 0;                                       //то после приезда на пусковую
        sPnt->State.Done = 0;                                           //прыгаем в 0
        sPnt->State.ToZero = 1;
    }
	
    defBitMicro(ramPnt, progPnt->Pnt_Dwn, ((sPnt->StateGT.Dwn == 1) && (tempRes == 1)));
    defBitMicro(ramPnt, progPnt->Pnt_Up , ((sPnt->StateGT.Up  == 1) && (tempRes == 1)));
	
    if(sPnt->Fout == 0.f) {
        sPnt->StateGT.RefZero = 1;
        setBitMicro(ramPnt, progPnt->Pnt_ZerroFq);
        if ((sPnt->Tpause <= 0) && (sPnt->State.EquZero == 0) && (tempRes == 1)&& (sPnt->StateGT.ReversHold == 1 )){
            sPnt->Tpause = load_s16(ramPnt, progPnt->Pnt_CurlesTime) / INT_TIME_CALL_MICRO_mS;
            sPnt->State.EquZero = 1;
            sPnt->StateGT.ReversHold = 0;
        }
    }
    else {
        sPnt->StateGT.RefZero = 0;
        sPnt->State.EquZero = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_ZerroFq);
        sPnt->Tpause = 0;
    }

    if(((sPnt->Fout == tempResVal)||(sPnt->Fout == -tempResVal)) && (tempRes == 1)) {
        setBitMicro(ramPnt, progPnt->Pnt_HldOnFq);
        sPnt->StateGT.EquRV = 1;
//        if ((sPnt->TfixFpusk <= 0) && (sPnt->StateGT.EqResVal == 0)){
//            sPnt->TfixFpusk = load_s16(ramPnt, progPnt->Pnt_AccThold) * (K_TIME_CALL_MICRO_TAKT / 10);
//            sPnt->StateGT.EqResVal = 1;
//            sPnt->StateGT.Freeze = 1;
//        }
        
        if ((sPnt->TfixFpusk <= 0) && (sPnt->StateGT.EqResVal == 0)){
		  // Если это старт (будет флаг UF) или если идет частотное торможение - тогда разрешим удержание на пусковой частоте
		  // Иначе удержание на пусковой не требуется и не будет дополнительной задержки 
		  if (Wait_UF || testBitMicro(ramPnt, progPnt->Pnt_FreqStop)) { 
			  //идёт торможение к нулевой частоте
			  if(sPnt->State.NeedZero || sPnt->State.ToZero) { // Пришел запрос на задание частоты 0, и я перехожу сюда на зарядку таймера, и тогда будет удержвание. Но зачем, сюда надо попасть если есть остановка!!!
				sPnt->TfixFpusk = load_s16(ramPnt, progPnt->Pnt_DecThold) * (K_TIME_CALL_MICRO_TAKT / 10);
			  }
			  //идёт разгон
			  else {
				sPnt->TfixFpusk = load_s16(ramPnt, progPnt->Pnt_AccThold) * (K_TIME_CALL_MICRO_TAKT / 10);
			  }
		  }
          sPnt->StateGT.EqResVal = 1;
          sPnt->StateGT.Freeze = 1;
        }        
        
        
    }
    else {
        sPnt->StateGT.EqResVal = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_HldOnFq);
        sPnt->StateGT.EquRV = 0;
        sPnt->TfixFpusk = 0;
    }
 
    GlobalM4.EqRefVal = (sPnt->StateGT.EqRefVal) ? 1 : 0; //флаг выхода на задание для блока profiNet/controlNet


/*
    // Дебаг параметр для проверки подхвата
    GlobalM3.TempLsuFout = (_IQ(sPnt->Fout) >> 16); //
    GlobalM3.GTState  = (s16)sPnt->StateGT.all; // Тек. режим - что делает ЗИ
    GlobalM3.GTStatus = (s16)sPnt->State.all; // Тек. состояние ЗИ - статус
    GlobalM3.s16_Debug11 = sPnt->Fcalc * 1000;
 */
    defBitMicro(ramPnt, progPnt->Pnt_AlrmFref, loc.flag.bit.AlrmFref);
    save_float(ramPnt, progPnt->Pnt_Fout, sPnt->Fout);
    save_s16(ramPnt, progPnt->Pnt_GTstate, sPnt->StateGT.all);
    GlobalM4.GTState  = (s16)sPnt->StateGT.all; // Тек. режим - что делает ЗИ
    GlobalM4.GTStatus = (s16)sPnt->State.all; // Тек. состояние ЗИ - статус
    GlobalM4.FlsuPickup = load_float (ramPnt, progPnt->Pnt_F_PU);

	// Тестовый вывод частоты задания в виде S16
	TestFref = (s16)(IQ_to_FLOAT(sPnt->Fout, Herz_base) * 100.0f);
	save_s16(ramPnt, progPnt->Pnt_FrefS16, TestFref);

    
    return &progPnt->Pnt_End;
};


void CalcSin2LSU(Temp_koef_type *sPnt, u16 Mode, f32 tAcc)
{
  f32	DJstart, DJend, JerkStart, JerkEnd, Acceleration, JerkMax, DJmax, Jerk0, Jerk1, Jerk2, Jerk3;
  f32	Fstart, Fend;
  f32	t0A, t0F, t0, Jp1, t1A, t1F, t1, t2, tp, t3A, t3F, t3, Jp3, At1, Vt1, Vt2;
  
  if (Mode == TIME_MODE) { // Режим генератора темпа, когда задаются время разгона и частота задания.
    // Делаем расчеты для разгона по времени и передаем расчет в алгоритм JERK_MODE
    Fstart = sPnt->SinStartFreq;
    Fend = sPnt->SinEndFreq;
    // Расчет параметров
    t1 = tAcc / 2;
    Jp1 = 2 * (Fend-Fstart) / (t1*t1);
    Acceleration = sqrt(Jp1 * (Fend-Fstart) / 2);
    DJstart = DJend = 100 * Jp1;
    JerkStart = JerkEnd = Jp1;
    // Переносим данные в общую структуру
    sPnt->JerkStart = sPnt->JerkEnd = JerkStart;
    sPnt->SinAcceleration = Acceleration;
    
    Mode = JERK_MODE;
  }
  
  if (Mode == JERK_MODE) { // Режим генератора темпа, когда задаются рывок и ускорение
    Fstart = sPnt->SinStartFreq;
    Fend = sPnt->SinEndFreq;
    JerkStart = sPnt->JerkStart;
    JerkEnd = sPnt->JerkEnd;
    Acceleration = sPnt->SinAcceleration;
    DJstart = 100 * JerkStart;
    DJend = 100 * JerkEnd;
    
    // 0) Находим минимальный рывок в паре
    JerkMax = (JerkStart < JerkEnd) ? JerkStart : JerkEnd;
    // Определяем максимальное ускорение при разгоне
    DJmax = 100*JerkMax;
    // Вычисляем все три значения рывка для предварительного расчета
    Jerk1 = JerkMax;
    Jerk2 = powf((2 * Acceleration * DJmax / PI) , (0.5) );
    Jerk3 = powf((2 * (Fend-Fstart) * DJmax*DJmax / (PI*PI)) , (0.3333333));
    // Функция нахождения минимума из трех значений
    Jerk0 = ((Jerk1 <= Jerk2) && (Jerk1 <= Jerk3)) ? Jerk1 : ((Jerk2 <= Jerk3) ? Jerk2 : Jerk3);
    
    t0A = 2 * Acceleration / Jerk0;
    t0F = sqrt (2 * (Fend-Fstart) / Jerk0);
    t0 = (t0A < t0F) ? t0A : t0F;
    
    Acceleration = Jerk0*t0/2; //максимальное ускорение конечного участка
    
    // 1) Находим рывок в начале разгона
    Jerk1 = JerkStart;
    Jerk2 = powf((2*Acceleration * DJstart / PI) , (0.5) );
    Jerk3 = powf((2 * (Fend-Fstart) * DJstart*DJstart / (PI*PI)) , (0.3333333));
    
    Jp1 = ((Jerk1 <= Jerk2) && (Jerk1 <= Jerk3)) ? Jerk1 : ((Jerk2 <= Jerk3) ? Jerk2 : Jerk3);
    t1A = 2 * Acceleration / Jp1;
    t1F = sqrt( 2 * (Fend-Fstart) / Jp1);
    t1 = (t1A < t1F) ? t1A : t1F;
    
    // 2) Находим рывок в конце разгона
    Jerk1 = JerkEnd;
    Jerk2 = powf((2*Acceleration * DJend / PI) , (0.5) );
    Jerk3 = powf((2 * (Fend-Fstart) * DJend*DJend / (PI*PI)) , (0.3333333));
    
    Jp3 = ((Jerk1 <= Jerk2) && (Jerk1 <= Jerk3)) ? Jerk1 : ((Jerk2 <= Jerk3) ? Jerk2 : Jerk3);
    t3A = 2 * Acceleration / Jp3;
    t3F = sqrt( 2 * (Fend-Fstart) / Jp3);
    t3 = (t3A < t3F) ? t3A : t3F;
    
    // 3) Находим время t2
    t2 = 2 * (Fend - Fstart + Jp1*t1*t1/4 - Jp3*t3*t3/4) / (Jp1*t1);
    tp = t1 + (t2-t1) + t3;
    
    // 4) Дополнительные значения для расчета
    At1 = Jp1*t1/2;
    Vt1 = Jp1*t1*t1/4 + Fstart;
    Vt2 = Vt1 + At1 * (t2-t1);
    
    // Запоминаем значения расчета
    sPnt->SinAt1 = At1;
    sPnt->SinVt1 = Vt1;
    sPnt->SinVt2 = Vt2;
    sPnt->t1Sin = t1;
    sPnt->t2Sin = t2;
    sPnt->t3Sin = t3;
    sPnt->tpSin = tp;
    sPnt->JerkStart = JerkStart;
    sPnt->JerkEnd = JerkEnd;
  }
}
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_LSU_2_1[45]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     // 1  Fref        //Pnt_IN
        BIT_PIN_TYPE | INPUT_PIN_MODE,     // 2  UpIn           // темп разгона
        BIT_PIN_TYPE | INPUT_PIN_MODE,     // 3  DwnIn          // темп торможения 
        BIT_PIN_TYPE | INPUT_PIN_MODE,     // 4  ZerTime
        BIT_PIN_TYPE | INPUT_PIN_MODE,     // 5  PickUpEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     // 6  GTtype      //Pnt_Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     // 7  TimeScale   //Pnt_TimeScale
        S32_PIN_TYPE | INPUT_PIN_MODE,     // 8  Fmax        //Pnt_FrS
        S16_PIN_TYPE | INPUT_PIN_MODE,     // 9  //Pnt_TimeUp // AccTime     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  //Pnt_TimeDw // DecTime     
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Stype       //Pnt_t2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  tA1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  tA2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  tA3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  tA4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  AccFhld     //Pnt_ResVal
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  AccThld     //Pnt_TfixFpusk
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  DecFhld
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  DecThld
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  CurlesTime  //Pnt_Tpause
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  F_PU
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //22  ResByPWM
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //23  ResByDC
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //24  Freeze1     //Pnt_Freeze
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //25  Freeze2        
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //26  ForbidFreq       
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  Pnt_ForbidTacc       
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Pnt_ForbidTdec          
        S32_PIN_TYPE | INPUT_PIN_MODE,     //29  JaccStart
        S32_PIN_TYPE | INPUT_PIN_MODE,     //30  AccelJ
        S32_PIN_TYPE | INPUT_PIN_MODE,     //31  JaccEnd
        S32_PIN_TYPE | INPUT_PIN_MODE,     //32  JaccStart
        S32_PIN_TYPE | INPUT_PIN_MODE,     //33  AccelJ
        S32_PIN_TYPE | INPUT_PIN_MODE,     //34  JaccEnd
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  Sin2Mode
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //36  vPhirUp                
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //37  AlrmFref
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //38  HldOnDel    //Pnt_Fp_delay
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //39  Format
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //40  HldOnFq     //Pnt_EquRV
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //41  ZerroFq     //Pnt_EqZero
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //42  GTstate     //Pnt_Status
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //43  Dwn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //44  Up
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //45  Fout        //Pnt_Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEMP_LSU_2", "DA", TblFuncTEMP_LSU_2_1, FuncTEMP_LSU_2_1, sizeof(TblFuncTEMP_LSU_2_1), sizeof(Temp_koef_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncTEMP_LSU_2_1,
#endif
//********************************************************
