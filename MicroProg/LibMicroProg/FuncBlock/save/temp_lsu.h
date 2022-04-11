#ifdef    GET_FUNC_CODE
typedef union {
    u16 all;
    struct {
        u16 ToRefF:1;
        u16 ToZero:1;
        u16 ToUp:1;
        u16 Revers:1;
        u16 ToFCalc:1;
        u16 Done:1;
        u16 Recalc:1;
        u16 NeedZero:1;

        u16 EquZero:1;
    };
}Stat_Type;

typedef union {
    u16         all;
    struct {
        u16         Up          :1;                             // Задатчик темпа работает на увеличение частоты
        u16         Dwn         :1;                             // Задатчик темпа работает на уменьшение частоты
        u16         RevCur      :1;                             // Задатчик темпа работает на реверс
        u16         RevRef      :1;                             // Задание для задатчика темпа на реверс

        u16         Reset       :1;
        u16         Freeze      :1;

        u16         RefZero     :1;                             // Выход нулевой
        u16         EqResVal    :1;
        u16         Great       :1;
        u16         ReversHold  :1;
        u16         flazhok     :1;

    };
}StateGT_type;

const f32 TimeScale[] = { 1.0f , 0.1f , 0.01f , 0.001f};

typedef struct {
    f32       KoefA;                          //Коэффициент А для кривой
    f32       KoefB;                          //Коэффициент Б для кривой
    f32       KoefK;                          //Коэффициент К для прямой
    f32       KoefA_1;                        //Коэффициент А для кривой
    f32       KoefB_1;                        //Коэффициент Б для кривой
    f32       KoefK_1;                        //Коэффициент К для прямой
    f32       KoefA_2;                        //Коэффициент А для кривой
    f32       KoefB_2;                        //Коэффициент Б для кривой
    f32       KoefK_2;                        //Коэффициент К для прямой
    f32       KoefA_3;                        //Коэффициент А для кривой
    f32       KoefB_3;                        //Коэффициент Б для кривой
    f32       KoefK_3;                        //Коэффициент К для прямой
    f32       KoefBLin;                       //Коэффициент Б для прямой
    f32       KoefBLin_1;                     //Коэффициент Б для прямой
    f32       KoefBLin_2;                     //Коэффициент Б для прямой
    f32       KoefBLin_3;                     //Коэффициент Б для прямой
    f32       TlinDn;                         //Нижнее время перехода
    f32       TlinUp;                         //Верхнее время перехода
    f32       TlinDn1;                        //Нижнее время перехода
    f32       TlinUp1;                        //Верхнее время перехода
    f32       TlinDn2;                        //Нижнее время перехода
    f32       TlinUp2;                        //Верхнее время перехода
    f32       TlinDn3;                        //Нижнее время перехода
    f32       TlinUp3;                        //Верхнее время перехода
    f32       Fcalc;                          //Расчитанная частота
    f32       TfupOld;                        //Предыдущее время до выхода на частоту Fref

    f32       LinKoefK;                       //Коэффициент К для линейного генератора темпа
    f32       LinKoefK_1;                       //Коэффициент К для линейного генератора темпа
	f32       ParKoefK;						  //Коэффициент К для U образного генератора темпа
    f32       Fout;                           //Выходная частота

    f32       OldF_out;                        //Текущая частота

    u16       TupOld;                         //Предыдущее время разгона
    u16       TdnOld;                         //Предыдущее время торможения
    f32       FrSOld;                         //Предыдущее значение максимальной частоты
    u32       uFrSOld;
    f32       FrefOld;                        //Предыдущая опорная частота
    u32       uFrefOld;

    f32       CurTime;                        //Текущее время

    Stat_Type State;                          //состояние генератора темпа
    StateGT_type        StateGT;              // для совместимости с генератором темпа Степанова
    f32       tempFin;                        //храним значение входной частоты для корректной обработки перехода через 0
    s16       EqvRV;                          // время частоты пуска
    s16       GT_ZERO;                         // время бестоковой паузы
}Temp_koef_type;

typedef struct {
u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In;                           //1  IN
    u16  Pnt_TimeUp;                       //2  TimeUp
    u16  Pnt_TimeDw;                       //3  TimeDw
    u16  Pnt_FrS;                          //4  FrS
    u16  Pnt_t2;                           //5  t2
    u16  Pnt_tA1;                          //6  tA1
    u16  Pnt_tA2;                          //7  tA2
    u16  Pnt_tA3;                          //8  tA3
    u16  Pnt_tA4;                          //9  tA4
    u16  Pnt_TimeScale;                    //10  TimeScale
    u16  Pnt_ResVal;                       //11  ResVal
    u16  Pnt_Type;                         //12  Type
    u16  Pnt_Freeze;                       //13  Freeze
    u16  Pnt_Freeze2;                      //14  EqvRV
    u16  Pnt_Freeze3;                      //15  GT_ZERO
    u16  Pnt_F_PU;                         //16  F_PU
    u16  Pnt_Reset;                        //17  Reset
    u16  Pnt_Format;                       //18  Format
    u16  Pnt_EquRV;                        //19  EquRV
    u16  Pnt_Zero;                         //20  Zero
    u16  Pnt_Dwn;                          //21  Dwn
    u16  Pnt_Up;                           //22  Up
    u16  Pnt_Status;                       //23  Status
    u16  Pnt_Out;                          //24  Out
    u16  Pnt_Fp_delay;                     //25  Fp_delay
    u16  Pnt_End;
}FuncTEMP_LSU_type;

u16 *FuncTEMP_LSU_1(FuncTEMP_LSU_type *progPnt, u32 ramPnt)
{
    Temp_koef_type    *sPnt;
    u16                 tempRes, tempFreeze, tempType, tempTimeScale;
    f32                 tempFrSOld, tempF_in, tempResVal, f_PU, t_1, t_2, t_3, t_4, TlinUp, TlinDn;
    u32                 utempFrSOld, utempFin;
    u16                 tempTup, tempTdn;


    sPnt = (Temp_koef_type *)(ramPnt + progPnt->Pnt_State); 

    

    //загрузка переменных

    tempResVal      =    _IQtoF( load_s32(ramPnt, progPnt->Pnt_ResVal)  );
    tempFrSOld      =    _IQtoF( load_s32(ramPnt, progPnt->Pnt_FrS)  );
    utempFrSOld     =    load_s32(ramPnt, progPnt->Pnt_FrS);
    tempTup         =    load_s16(ramPnt, progPnt->Pnt_TimeUp);
    tempTdn         =    load_s16(ramPnt, progPnt->Pnt_TimeDw);
    tempF_in         =    _IQtoF( load_s32(ramPnt, progPnt->Pnt_In)    );
    utempFin        =    load_s32(ramPnt, progPnt->Pnt_In);
    //signRef         =    _IQ((tempF_in >= 0.f) ? 1 : -1 );
    tempRes         =    testBitMicro(ramPnt, progPnt->Pnt_Reset);
    tempFreeze      =    testBitMicro(ramPnt, progPnt->Pnt_Freeze);
    tempType        =    load_s16(ramPnt, progPnt->Pnt_Type);
    tempTimeScale   =    load_s16(ramPnt, progPnt->Pnt_TimeScale);

    if ((sPnt->EqvRV > 0)/* && (sPnt->StateGT.Up == 1)*/){
        tempFreeze|=0x1;
        sPnt->EqvRV--;
    }
    if ((sPnt->GT_ZERO > 0) && (tempRes == 1)){
        tempFreeze|=0x1;
        sPnt->GT_ZERO--;
    }
    //формирование формата
    save_s16(ramPnt, progPnt->Pnt_Format, 21|((tempTimeScale&0xFF)<<8));

    //Проверка на необходимость пересчета коэффициентов
    if((sPnt->uFrSOld != utempFrSOld) || (sPnt->TupOld != tempTup) || (sPnt->TdnOld != tempTdn)
       || (sPnt->uFrefOld != utempFin) || (flagIsFirstCall == 1)) {

    	if (sPnt->StateGT.flazhok == 0){
			f32 t2, t3, t2_1, t3_1, t2_2, t3_2, t2_3, t3_3;                                                             //переменные для тквадрат и ткуб
			f32 FlinDn, FlinUp, FlinDn1, FlinUp1, FlinDn2, /*FlinUp2,*/ FlinDn3, FlinUp3;
			f32 tempF1, tempFold, absOldFout, absFin;
			u16 tempT;

			sPnt->OldF_out = sPnt->Fout;                                             //присваиваю старой частоте текущую

			if((sPnt->OldF_out == 0.f) && (tempResVal != 0.f) &&
			   (utempFin != 0)) {                                                   //если текущая частота 0 то надо прыгнуть на пусковую

				sPnt->State.ToRefF = 1;
				sPnt->State.Recalc = 1;
			}
			else {

//			    if ((( )&&( ))||(( )&&( )))
			    if(((sPnt->OldF_out > 0.f) && (tempF_in > 0.f)) ||
                        ((sPnt->OldF_out < 0.f) && (tempF_in < 0.f))){
                            sPnt->State.ToUp = 1;
                        }

				if((sPnt->State.ToUp == 0) && (tempResVal != 0.f)
				   && ((sPnt->OldF_out == tempResVal) || (sPnt->OldF_out == -tempResVal))) { //если спускались по частоте и дошли до F пуск
					//то надо прыгнуть в 0
					sPnt->State.ToZero = 1;
				}
				else {
					if(tempF_in != 0.f){
						sPnt->State.NeedZero = 0;                                   //устанавливаем бит необходимости прыгнуть в 0
					}
					if((tempF_in == 0.f) && (sPnt->OldF_out != 0.f)) {                //если задание 0
																					//то спускаемся до пусковой
						sPnt->State.Recalc = 1;

						sPnt->State.NeedZero = 1;                                   //устанавливаем бит необходимости прыгнуть в 0
																					//после отработки задания
						tempF_in = (sPnt->OldF_out > 0.f) ? tempResVal: -tempResVal;
						sPnt->tempFin = tempF_in;

						sPnt->uFrefOld   = utempFin;
					}
					else {
						if((sPnt->State.ToZero == 0) &&                                     //если реверс то едем до F пусковой
						   (((sPnt->OldF_out > 0.f) && (tempF_in < 0.f)) ||
							((sPnt->OldF_out < 0.f) && (tempF_in > 0.f)))) {

							sPnt->StateGT.RevCur = 1;
							sPnt->StateGT.RevRef = 1;

							tempF_in = (tempF_in > 0.f) ? -tempResVal: tempResVal;
							sPnt->tempFin = tempF_in;
							sPnt->State.Revers = 1;

							sPnt->State.Recalc = 1;
							sPnt->uFrefOld   = utempFin;// - 1;

						}
						else {                                                            //после проезда до F пуск выезжаем на задание

							sPnt->State.Recalc = 1;
							absFin = (tempF_in > 0.f) ? tempF_in : -tempF_in;

							if((absFin < tempResVal) && (tempF_in != 0.f)) {

								tempF_in = 0.f;//(tempFin > 0.f) ? tempResVal : -tempResVal;
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
				}
				else {
					// Расчет коэффициентов для кривой торможения
					tempFold -= tempF1;

					tempF1 = sPnt->OldF_out - sPnt->FrefOld;

					tempT = tempTdn;
					sPnt->State.ToUp = 0;
					sPnt->StateGT.Dwn = 1;
					sPnt->StateGT.Up = 0;
				}

				//вычисляем время до выхода на Fin от текущей частоты
				if(tempFrSOld != 0) sPnt->TfupOld = (tempFold * (tempT * TimeScale[tempTimeScale])) / tempFrSOld;
				else sPnt->TfupOld = 0.f;
				if (load_s16(ramPnt, progPnt->Pnt_t2) == 1){
					t_1 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_tA1));
					t_2 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_tA2));
					t_4 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_tA3));
					t_3 = _IQtoF(load_s32(ramPnt, progPnt->Pnt_tA4));

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
	//                FlinUp2 = tempF1 - FlinDn2;                                                       //по оси частоты
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

						sPnt->KoefK = (FlinUp1 - FlinDn) / (sPnt->TlinUp1 - sPnt->TlinDn);            //вычисление коэфициента К для прямого участка
						// k=y/x
						sPnt->KoefA = ((sPnt->KoefK  * sPnt->TlinDn) - (2 * FlinDn)) / t3;          //вычисление коэфициента А для полинома

						sPnt->KoefB = ((3 * FlinDn) - (sPnt->KoefK * sPnt->TlinDn)) / t2;           //вычисление коэфициента Б для полинома

						sPnt->KoefBLin = (sPnt->KoefA * t3 + sPnt->KoefB * t2)                      //вычисление коэфициента Б для прямого участка
										 - sPnt->KoefK * sPnt->TlinDn;

						sPnt->LinKoefK = tempF1 / sPnt->TfupOld;                                    //вычисление коэффициента К для линейного
																									// генератора темпа
					}
					else {

						sPnt->KoefK    = 0.f;
						sPnt->KoefA    = 0.f;
						sPnt->KoefB    = 0.f;
						sPnt->KoefBLin = 0.f;
					}

					// Разгон верхний диапазон
					if(sPnt->TlinDn1 != 0.f) {

		//                sPnt->KoefK_1 = (FlinUp1 - FlinDn1) / (sPnt->TlinUp1 - sPnt->TlinDn1);            //вычисление коэфициента К для прямого участка
						// k=y/x
						sPnt->KoefA_1 = ((sPnt->KoefK  * sPnt->TlinDn1) - (2 * FlinDn1)) / t3_1;          //вычисление коэфициента А для полинома

						sPnt->KoefB_1 = ((3 * FlinDn1) - (sPnt->KoefK * sPnt->TlinDn1)) / t2_1;           //вычисление коэфициента Б для полинома

						sPnt->KoefBLin_1 = (sPnt->KoefA_1 * t3_1 + sPnt->KoefB_1 * t2_1)                      //вычисление коэфициента Б для прямого участка
										 - sPnt->KoefK * sPnt->TlinDn1;

		//                sPnt->LinKoefK_1 = tempF1 / sPnt->TfupOld;                                    //вычисление коэффициента К для линейного
																									// генератора темпа
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
						// k=y/x
						sPnt->KoefA_2 = ((sPnt->KoefK_3  * sPnt->TlinDn2) - (2 * FlinDn2)) / t3_2;          //вычисление коэфициента А для полинома

						sPnt->KoefB_2 = ((3 * FlinDn2) - (sPnt->KoefK_3 * sPnt->TlinDn2)) / t2_2;           //вычисление коэфициента Б для полинома

						sPnt->KoefBLin_2 = (sPnt->KoefA_2 * t3_2 + sPnt->KoefB_2 * t2_2)                      //вычисление коэфициента Б для прямого участка
										 - sPnt->KoefK_3 * sPnt->TlinDn2;

		//                sPnt->LinKoefK_1 = tempF1 / sPnt->TfupOld;                                    //вычисление коэффициента К для линейного
																									// генератора темпа
					}
					else {

						sPnt->KoefK    = 0.f;
						sPnt->KoefA    = 0.f;
						sPnt->KoefB    = 0.f;
						sPnt->KoefBLin = 0.f;
					}

					// Торможение нижний диапазон
					if(sPnt->TlinDn3 != 0.f) {

		//                sPnt->KoefK_3 = (FlinUp3 - FlinDn2) / (sPnt->TlinUp3 - sPnt->TlinDn2);            //вычисление коэфициента К для прямого участка
						// k=y/x
						sPnt->KoefA_3 = ((sPnt->KoefK_3  * sPnt->TlinDn3) - (2 * FlinDn3)) / t3_3;          //вычисление коэфициента А для полинома

						sPnt->KoefB_3 = ((3 * FlinDn3) - (sPnt->KoefK_3 * sPnt->TlinDn3)) / t2_3;           //вычисление коэфициента Б для полинома

						sPnt->KoefBLin_3 = (sPnt->KoefA_3 * t3_3 + sPnt->KoefB_3 * t2_3)                      //вычисление коэфициента Б для прямого участка
										 - sPnt->KoefK_3 * sPnt->TlinDn3;

		//                sPnt->LinKoefK_1 = tempF1 / sPnt->TfupOld;                                    //вычисление коэффициента К для линейного
																									// генератора темпа
					}
					else {

						sPnt->KoefK    = 0.f;
						sPnt->KoefA    = 0.f;
						sPnt->KoefB    = 0.f;
						sPnt->KoefBLin = 0.f;
					}
				}else{
					FlinDn = tempF1 * 0.2f;                    //Вычисление верхней и нижней точек перегиба
					FlinUp = tempF1 - FlinDn;                                                       //по оси частоты
					sPnt->TlinDn = sPnt->TfupOld * 0.3f;      //Вычисление нижней и верхней точек перегиба
					sPnt->TlinUp = sPnt->TfupOld - sPnt->TlinDn;                                    //по оси времени
					t2 = sPnt->TlinDn * sPnt->TlinDn;
					t3 = sPnt->TlinDn * sPnt->TlinDn * sPnt->TlinDn;
					if(sPnt->TlinDn != 0.f) {

						sPnt->KoefK = (FlinUp - FlinDn) / (sPnt->TlinUp - sPnt->TlinDn);            //вычисление коэфициента К для прямого участка
						// k=y/x
						sPnt->KoefA = ((sPnt->KoefK  * sPnt->TlinDn) - (2 * FlinDn)) / t3;          //вычисление коэфициента А для полинома

						sPnt->KoefB = ((3 * FlinDn) - (sPnt->KoefK * sPnt->TlinDn)) / t2;           //вычисление коэфициента Б для полинома

						sPnt->KoefBLin = (sPnt->KoefA * t3 + sPnt->KoefB * t2)                      //вычисление коэфициента Б для прямого участка
										 - sPnt->KoefK * sPnt->TlinDn;

						sPnt->LinKoefK = tempF1 / sPnt->TfupOld;                                    //вычисление коэффициента К для линейного
						sPnt->ParKoefK = tempF1 / (sPnt->TfupOld * sPnt->TfupOld);					//Вычисление коэффициента К для U-образного генератора темпа
																									// генератора темпа
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
			}

			sPnt->State.Done = 0;                                                   //устанавливаю флаг того, что работа невыполнена
			sPnt->CurTime = 0.f;
			sPnt->StateGT.flazhok = 1;
    	}else{
    		sPnt->StateGT.flazhok = 0;
    	}
    }


    //проверяем не надо ли пускаться
    if(tempRes) {
        sPnt->StateGT.Reset = 0;
        if(!tempFreeze) {
            sPnt->StateGT.Freeze = 0;
            if(sPnt->State.Done == 0) {
                if(sPnt->State.ToRefF) {                        //если надо прыгнуть на пусковую

                    if(tempF_in > 0.f) {                         //если задание положительное
                        sPnt->OldF_out = tempResVal;
                        sPnt->Fout = tempResVal;
                    }
                    else {                                      //если задание отрицательное
                        sPnt->OldF_out = -tempResVal;
                        sPnt->Fout = -tempResVal;
                    }
                    sPnt->State.ToRefF = 0;
                    sPnt->State.Done = 1;
                }
                else {
                    if(sPnt->State.ToZero) {                    //если надо прыгнуть в 0

                        sPnt->OldF_out = 0.f;
                        sPnt->Fout = 0.f;

                        sPnt->State.ToZero = 0;
                        sPnt->State.Done = 1;
                    }
                    else {                                    //иначе просто едем по кривой
                        if(sPnt->CurTime < sPnt->TfupOld) {

                            switch(tempType) {
                            case 0:
                                //*********************************************************************************
                                //         Вычисление приращения для линейного темпа
                                //*********************************************************************************
                                sPnt->Fcalc = sPnt->LinKoefK * sPnt->CurTime;
                                break;
                            case 1:
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
                            case 2:
                            	sPnt->Fcalc =  sPnt->ParKoefK * (sPnt->CurTime * sPnt->CurTime);    			//U-образная кривая генератора темпа
								break;
                            default:
                                sPnt->Fcalc = 0.f;
                                break ;
                            }

                            sPnt->CurTime += FLOAT_TIME_CALL_MICRO;

                            //*********************************************************************************
                            //         Вычисление частоты
                            //*********************************************************************************
                            if(sPnt->State.ToUp) {
                                sPnt->Fout = sPnt->OldF_out + sPnt->Fcalc;
                            }
                            else sPnt->Fout = sPnt->OldF_out - sPnt->Fcalc;

                        }
                        else {                                    //если вышли на частоту

                            sPnt->OldF_out = sPnt->tempFin;
                            sPnt->Fout = sPnt->tempFin;
                            sPnt->State.Done = 1;
                            if(sPnt->State.Revers) {
                                sPnt->StateGT.ReversHold = 1;
                                sPnt->uFrefOld   = utempFin - 1;
                                sPnt->StateGT.RevCur = 0;
                                sPnt->StateGT.RevRef = 0;
                                sPnt->State.Revers = 0;
                            }
                            sPnt->StateGT.Dwn = 0;
                            sPnt->StateGT.Up = 0;
                        }
                    }
                }
            }
        }
        else {
            sPnt->StateGT.Freeze = 1;
        }
    }
    else {
        f_PU = _IQtoF(load_s32(ramPnt, progPnt->Pnt_F_PU));     // Подхват
        sPnt->StateGT.Reset = 1;
        sPnt->CurTime = 0.f;
        sPnt->Fout = f_PU;               // было = 0.f
        sPnt->OldF_out = f_PU;            // было = 0.f
        sPnt->uFrSOld = 0;
        sPnt->uFrefOld = 0;
        sPnt->State.all = 0;
        sPnt->State.Done = 1;
    }


    if((sPnt->State.NeedZero == 1) && (sPnt->State.Done == 1)) {        // если задание было 0
        sPnt->State.NeedZero = 0;                                       //то после приезда на пусковую
        sPnt->State.Done = 0;                                           //прыгаем в 0
        sPnt->State.ToZero = 1;
    }

    if((sPnt->StateGT.Dwn == 1) && (tempRes == 1)) {
        setBitMicro(ramPnt, progPnt->Pnt_Dwn);
    }
    else {
        clrBitMicro(ramPnt, progPnt->Pnt_Dwn);
    }

    if((sPnt->StateGT.Up == 1) && (tempRes == 1)) {
        setBitMicro(ramPnt, progPnt->Pnt_Up);
    }
    else {
        clrBitMicro(ramPnt, progPnt->Pnt_Up);
    }

    if(sPnt->Fout == 0.f) {
        sPnt->StateGT.RefZero = 1;
        setBitMicro(ramPnt, progPnt->Pnt_Zero);
        if ((sPnt->GT_ZERO <= 0) && (sPnt->State.EquZero == 0) && (tempRes == 1)&& (sPnt->StateGT.ReversHold == 1 )){
            sPnt->GT_ZERO = load_s16(ramPnt, progPnt->Pnt_Freeze3) / INT_TIME_CALL_MICRO_mS;
            sPnt->State.EquZero = 1;
            sPnt->StateGT.ReversHold = 0;
        }
        if (tempResVal != 0){
            sPnt->StateGT.EqResVal = 0;
        }else{
            sPnt->StateGT.EqResVal = 1;
        }
    }
    else {
        sPnt->StateGT.RefZero = 0;
        sPnt->State.EquZero = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_Zero);
        sPnt->GT_ZERO = 0;
    }

    if((sPnt->Fout == tempResVal) ||(sPnt->Fout == -tempResVal)) {
        setBitMicro(ramPnt, progPnt->Pnt_EquRV);
        if ((sPnt->EqvRV <= 0) && (sPnt->StateGT.EqResVal == 0)){
            sPnt->EqvRV = load_s16(ramPnt, progPnt->Pnt_Freeze2) * (K_TIME_CALL_MICRO_TAKT / 10);
            sPnt->StateGT.EqResVal = 1;
        }
    }
    else {
//        sPnt->StateGT.EqResVal = 0;
        clrBitMicro(ramPnt, progPnt->Pnt_EquRV);
        sPnt->EqvRV = 0;
    }

    save_s32(ramPnt, progPnt->Pnt_Out, _IQ(sPnt->Fout));
    save_s16(ramPnt, progPnt->Pnt_Status, sPnt->StateGT.all);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncTEMP_LSU_1[25]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  IN
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  TimeUp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TimeDw
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  FrS
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  t2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  tA1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  tA2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  tA3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  tA4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  TimeScale
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  ResVal
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Type
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  Freeze
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  EqvRV
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  GT_ZERO
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  F_PU
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //17  Reset
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  Format
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //19  EquRV
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //20  Zero
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //21  Dwn
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Up
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  Status
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //24  Out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //25  Fp_delay
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("TEMP_LSU", "DA", TblFuncTEMP_LSU_1, FuncTEMP_LSU_1, sizeof(TblFuncTEMP_LSU_1), sizeof(Temp_koef_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
(pFuncMicro_type)FuncTEMP_LSU_1,
#endif


