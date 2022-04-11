#ifndef GT_DEFS_h
#define GT_DEFS_h

typedef union {
    u16 all;
    struct {
        u16 ToRefF:1;   // 1
        u16 ToZero:1;   // 2
        u16 ToUp:1;     // 4
        u16 Revers:1;   // 8
        u16 ToFCalc:1;  // 16
        u16 Done:1;     // 32
        u16 Recalc:1;   // 64
        u16 NeedZero:1; // 128
        u16 EquZero:1;  // 256
    };
}Stat_Type;

typedef union {
    u16         all;
    struct {
        u16   Up          :1;    // Задатчик темпа работает на увеличение частоты
        u16   Dwn         :1;    // Задатчик темпа работает на уменьшение частоты
        u16   RevCur      :1;    // Задатчик темпа работает на реверс
        u16   RevRef      :1;    // Задание для задатчика темпа на реверс
        
        u16   Reset       :1;
        u16   Freeze      :1;
        u16   RefZero     :1;    // Выход нулевой
        u16   EqResVal    :1;

        u16   Great       :1;
        u16   ReversHold  :1;
        u16   flazhok     :1;
        u16   EqRefVal    :1;

        u16   EquRV       :1;
        u16   PickUpWork  :1;
        u16   reserv      :2;
        
    };
}StateGT_type;

typedef enum {
    Up 		=(1<<0),               // Задатчик темпа работает на увеличение частоты
    Dwn		=(1<<1),               // Задатчик темпа работает на уменьшение частоты
    RevCur	=(1<<2),               // Задатчик темпа работает на реверс
    RevRef	=(1<<3),               // Задание для задатчика темпа на реверс
    Reset	=(1<<4),
    Freeze	=(1<<5),
    RefZero	=(1<<6),               // Выход нулевой
    EqResVal	=(1<<7),
    Great	=(1<<8),
    ReversHold	=(1<<9),
    flazhok	=(1<<10),
    EqRefVal	=(1<<11),
    EquRV       =(1<<12)
}GTSTATEtype_enum;

const f32 TimeScale[] = { 1.0f , 0.1f , 0.01f , 0.001f};

typedef enum {
	TIME_MODE = 0,
	JERK_MODE = 1,
}SIN2MODE_enum;

typedef struct {
   f32 KoefA;			// Коэффициент А для кривой
   f32 KoefB;			// Коэффициент Б для кривой
   f32 KoefK;			// Коэффициент К для прямой
   f32 KoefA_1;			// Коэффициент А для кривой
   f32 KoefB_1;			// Коэффициент Б для кривой
   f32 KoefK_1;			// Коэффициент К для прямой
   f32 KoefA_2;			// Коэффициент А для кривой
   f32 KoefB_2;			// Коэффициент Б для кривой
   f32 KoefK_2;			// Коэффициент К для прямой
   f32 KoefA_3;			// Коэффициент А для кривой
   f32 KoefB_3;			// Коэффициент Б для кривой
   f32 KoefK_3;			// Коэффициент К для прямой
   f32 KoefBLin;		// Коэффициент Б для прямой
   f32 KoefBLin_1;		// Коэффициент Б для прямой
   f32 KoefBLin_2;		// Коэффициент Б для прямой
   f32 KoefBLin_3;		// Коэффициент Б для прямой
   f32 TlinDn;			// Нижнее время перехода
   f32 TlinUp;			// Верхнее время перехода
   f32 TlinDn1;			// Нижнее время перехода
   f32 TlinUp1;			// Верхнее время перехода
   f32 TlinDn2;			// Нижнее время перехода
   f32 TlinUp2;			// Верхнее время перехода
   f32 TlinDn3;			// Нижнее время перехода
   f32 TlinUp3;			// Верхнее время перехода
   f32 Fcalc;			// Расчитанная частота
   f32 TfupOld;			// Предыдущее время до выхода на частоту Fref

   f32 LinKoefK;		// Коэффициент К для линейного генератора темпа
   f32 LinKoefK_1;		// Коэффициент К для линейного генератора темпа
   f32 ParKoefK;	        // Коэффициент К для U образного генератора темпа
   f32 Fout;		        // Выходная частота

   f32 OldF_out;	        // Текущая частота
   s32 DeltaFinWoutFiltr;       // Фильтрованное значение отклонения при ограничении регулятора скорости
   f32 LastWoutAccelmeter;      // Последнее значение скорости при оценке текущего ускорения
   u16 AccelmeterCnt;	        // Счетчик такта определения текущего ускорения
   u16 AccelOverrunCnt;         // Счетчик - сколько раз превысили ускорение
   u16 AccelOverrunBlockCnt;    // Счетчик блокировки повторных входов в алгоритм перезапуска генератора темпа
   u16 AccelRestartLSU;

   u16 TupOld;			// Предыдущее время разгона
   u16 TdnOld;			// Предыдущее время торможения
   f32 FrSOld;			// Предыдущее значение максимальной частоты
   f32 uFrSOld;			//
   f32 FrefOld;			// Предыдущая опорная частота
   f32 uFrefOld;		//

   f32 CurTime;			// Текущее время

   Stat_Type State;		// состояние генератора темпа
   StateGT_type StateGT;	// для совместимости с генератором темпа Степанова
   f32 tempFin;                 //храним значение входной частоты для корректной обработки перехода через 0
   s16 TfixFpusk;		//время частоты пуска
   s16 Tpause;			//время бестоковой паузы
   f32 F_PU;                    //Частота, найденная блоком PICKUP
   
   f32				SinStartFreq;
   f32				SinEndFreq;
   f32				JerkStart;
   f32				JerkEnd;
   f32				SinAcceleration;
   f32				TsinDn;
   f32				TsinUp;
   f32				SinAt1;
   f32				SinVt1;
   f32				SinVt2;
   f32				Jp;
   f32				t1Sin;
   f32				t2Sin;
   f32				t3Sin;
   f32				tpSin;
   u16				TsinRevers;   
}Temp_koef_type;


#endif/*GT_DEFS_h*/
