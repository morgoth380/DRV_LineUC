#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_TaccDef;                      //1 Tacc
    u16  Pnt_Tacc[18];                     //2...19   Tacc0...Tacc17
    u16  Pnt_TdecDef;                      //20 Tdec
    u16  Pnt_Tdec[18];                     //21...38  Tdec0...Tdec17
    u16  Pnt_Speed[18];                    //39...56  Speed0...Speed16
    u16  Pnt_Tcalc;                        //57  Tcalc
    u16  Pnt_StopOut;                      //58  SelAccel
    u16  Pnt_StopExt;                      //59  StopExt
    u16  Pnt_FreqNum;                      //60  FreqNum
    u16  Pnt_StateGT;                      //61  StateGT
    u16  Pnt_SpeedDI;                      //62  SpeedDI
    u16  Pnt_Start;                        //63  Start
    u16  Pnt_Stype1;                       //64  Stype1
    u16  Pnt_tA1_1;                        //65  tA1_1
    u16  Pnt_tA2_1;                        //66  tA2_1
    u16  Pnt_tA3_1;                        //67  tA3_1
    u16  Pnt_tA4_1;                        //68  tA4_1
    u16  Pnt_Stype2;                       //69  Stype2
    u16  Pnt_tA1_2;                        //70  tA1_2
    u16  Pnt_tA2_2;                        //71  tA2_2
    u16  Pnt_tA3_2;                        //72  tA3_2
    u16  Pnt_tA4_2;                        //73  tA4_2
    u16  Pnt_Stype3;                       //74  Stype3
    u16  Pnt_tA1_3;                        //75  tA1_3
    u16  Pnt_tA2_3;                        //76  tA2_3
    u16  Pnt_tA3_3;                        //77  tA3_3
    u16  Pnt_tA4_3;                        //78  tA4_3
    u16  Pnt_LiftBrakeOff;                 //79  Stype4 LiftBrakeOff
    u16  Pnt_tA1_4;                        //80  tA1_4
    u16  Pnt_tA2_4;                        //81  tA2_4
    u16  Pnt_tA3_4;                        //82  tA3_4
    u16  Pnt_tA4_4;                        //83  tA4_4
    u16  Pnt_tExtStop;                     //84  tExtStop
    u16  Pnt_Fmax;                         //85  Fmax
    u16  Pnt_ChRefDin;                     //86  ChRefDin
    u16  Pnt_StypeIn;                      //87  StypeIn
    u16  Pnt_Fref;                         //88  Fref
    u16  Pnt_DFSelTime;                    //89  DFSelTime
    u16  Pnt_EvacOn;                       //90  EvacOn
    u16  Pnt_EvacDir;                      //91  EvacDi
    u16  Pnt_EncPhasingEn;                 //92  EncPhasingWork
    u16  Pnt_EncPhasingWork;               //93  EncPhasingSpd    
    u16  Pnt_Functional;                   //94  Functional
    
    u16  Pnt_AccTime;                      //95  AccTime
    u16  Pnt_DecTime;                      //96  DecTime
    u16  Pnt_OutRef;                       //97  OutRef
    u16  Pnt_StypeOut;                     //98  StypeOut
    u16  Pnt_ResOut2;                      //99  ResOut2
    u16  Pnt_ResOut3;                      //100  ResOut3
    u16  Pnt_ResOut4;                      //101  ResOut4
    u16  Pnt_ResOut5;                      //102  ResOut5
    u16  Pnt_ResOut6;                      //103  ResOut6
    u16  Pnt_ResOut7;                      //104  ResOut7
    u16  Pnt_ResOut8;                      //105  ResOut8
    u16  Pnt_ResOut9;                      //106  ResOut9
    u16  Pnt_ResOut10;                     //107  ResOut10
    u16  Pnt_CodeSpeed[18];                //108...125  CodeSpeed0...CodeSpeed17    
    u16  Pnt_tDownDvig;                    //126  tDownDvig
    u16  Pnt_tUpDvig;                      //127  tUpDvig
    u16  Pnt_tDownGen;                     //128  tDownGen
    u16  Pnt_tUpGen;                       //129  tUpGen
    u16  Pnt_IactDvigMax;                  //130  IactDvigMax
    u16  Pnt_IactGenMax;                   //131  IactGenMax
    u16  Pnt_EnTimeComp;                   //132  EnTimeComp
    u16  Pnt_ta1_Out;                      //133  ta1_Out
    u16  Pnt_ta2_Out;                      //134  ta2_Out
    u16  Pnt_ta3_Out;                      //135  ta3_Out
    u16  Pnt_ta4_Out;                      //136  ta4_Out
    u16  Pnt_End;
}FuncRAMP_CTRL_MOD2_type;

typedef enum{
	brakeOnWaitState    = 0,
	countState          = 1,
	piSpdInitState      = 2,
	brakeOffWaitState   = 3,
}piSpdCmdStateType;

typedef struct{
        s16  prevNumSpeed;
        u16  prevAccTime;
        u16  prevDecTime;
        //u16  refChanging;
        u16  extStopflag;
        u16  speedFilterCounter;			//счётчик фильтрации задания скорости по бит-коду
        u16  oldRawNumSpeed;				//старое значение номера скорости до фильтрации
        s16  newNumSpeed;				//номер скорости после фильтрации бит-кода
        s16  newIndexSpeed;                             // Speed index in array. Not the same NumSpeed
        s16  prevIndexSpeed;                            // Speed index in array. Not the same NumSpeed
        u16 cnt;
        piSpdCmdStateType piSpdCmdState;

    	f32 debugDeltaF;
    	f32 debugDiv;
    	f32 debugTmp1;
    	f32 debugTmp2;
}RAMPCTRL2_1_State_type;



typedef struct{
	f32 rampSectors[4][4];
	u16 specRef1; //!номер скорости, кот. имеет свои значения нач. и кон. участков разгона и торможения
	u16 specRef2; //!номер скорости, кот. имеет свои значения нач. и кон. участков разгона и торможения
	u16 specRef3; //!номер скорости, кот. имеет свои значения нач. и кон. участков разгона и торможения
	u16 newNumSpeed;
	u16 prevNumSpeed;
	f32 newOutRef;
	f32 prevOutRef;
}rampParamsType;


/*Функция формирования значений начального и конечного участков при разгоне и торможении.*/
/*Возвращает индекс на группы значений начальных и конечных участков разгона/торможения*/
u16 rampParamSel(rampParamsType *v)
{
    #define GROUP_1         0
    #define GROUP_2         1
    #define GROUP_3         2
    #define DEFAULT_GROUP   3

	static u16 prev  = DEFAULT_GROUP;
	if (v->newOutRef > v->prevOutRef){            //!если разгоняемся
		if((v->specRef1) && ((v->newNumSpeed + 1) == v->specRef1)){  //!если новая скорость - это скорость со своими нач. участками разгона и торможения (спец. скорость 1)
			prev = GROUP_1;                       //!возвращаем указатель на группу значений участков для спец. скорости 1
			return(GROUP_1);                      //!группа значений 1
		}else if ((v->specRef2) && ((v->newNumSpeed + 1) == v->specRef2)){ //!если новая скорость - это скорость со своими нач. участками разгона и торможения (спец. скорость 2)
			prev = GROUP_2;                       //!возвращаем указатель на группу значений участков для спец. скорости 1
			return(GROUP_2);                      //!группа значений 2
		}else if ((v->specRef3) && ((v->newNumSpeed + 1) == v->specRef3)){ //!если новая скорость - это скоростьсо своими нач. участками разгона и торможения (спец. скорость 3)
			prev = GROUP_3;                       //!группа значений 3
			return(GROUP_3);                      //!группа значений 3
		}else{
			prev = DEFAULT_GROUP;                 //!указатель на группу значений по умолчанию
			return(DEFAULT_GROUP);                //!значения по умолчанию
		}
	} else if (v->newOutRef < v->prevOutRef){     //!если снижаем скорость
        if ((v->specRef1) && ((v->prevNumSpeed + 1) == v->specRef1)){      //!если скорость с которой снижаемся - это спец. скорость 1
        	prev = GROUP_1;                       //!группа значений начальных и конечных участков для скорости с которой снижаемся.
        	return(GROUP_1);
        }else if ((v->specRef2) && ((v->prevNumSpeed + 1) == v->specRef2)){
        	prev = GROUP_2;
        	return(GROUP_2);
        }else if ((v->specRef3) && ((v->prevNumSpeed + 1) == v->specRef3)){
        	prev = GROUP_3;
        	return(GROUP_3);
        }else{
        	prev = DEFAULT_GROUP;
        	return(DEFAULT_GROUP);
        }
	}
	else{  //! если вышли на скорость, то возвращаем предыдущие параметры
		return(prev);
	}
}

enum{
	FUp = 0,
	FDwn = 1,
	FzeroRef = 6,
};

typedef enum{
	notSelected = 0,
	Ain1 = 1,
	Ain2 = 2,
}AinSel;

typedef enum{
  SearchEvacDir = 0,
  ForwardEvacDir = 1,
  BackwardEvacDir = -1,  
}EvacOutDir_enum;

#define  MAX_NUM_SPD    18
#define  EVACUATION_SPD_CODE    16
#define  ENCOPHASING_SPD_CODE   17

//******************************Прототипы ф-ций*************************
u16 rampParamSel(rampParamsType *v);

//**********************************************************************
u16 *FuncRAMP_CTRL_MOD2_1(FuncRAMP_CTRL_MOD2_type *progPnt, u32 ramPnt)
{
	RAMPCTRL2_1_State_type *sPnt;
	u16 i; // temp, loadWeight, cabinWeight, currentDelay;
	rampParamsType primFinalRegions;
	u16 rawNumSpeed;
	u16 accTime;
	u16 decTime;
	f32 newOutRef, temp1, temp2, prevOutRef, refEncoPhasSpd; // Ain1Level, Ain2Level, Ain, offsetCurr, startIntegrValue,
	//f32 t1, t2, t3, t4;
	f32 fmax, fCur, deltaF;
	//s16 reCalcTime; // decTimeCalc
	//u16 numSpeedS1, numSpeedS2, numSpeedS3;
	//u16 GTstate;//, GTDwn, GTUp;
	u16 extDecTime;
	u16 speedFilterTime;
	f32 accDinTime, decDinTime; 
        u16     IndexSpeed;
        u16     EvacOn, EncoPhasingWork;
        EvacOutDir_enum    EvacDir;
        
	//static u16 debugVar = 0;
	sPnt = (RAMPCTRL2_1_State_type *)(ramPnt + progPnt->Pnt_State);

	if(flagIsFirstCall) {
          sPnt->speedFilterCounter = sPnt->oldRawNumSpeed = sPnt->newNumSpeed = 0;
          sPnt->piSpdCmdState = brakeOnWaitState;
          sPnt->prevNumSpeed = sPnt->prevIndexSpeed = -1;
          return &progPnt->Pnt_End;
	}

        // Check Lift functional  - if disable?, set default acc/dec time ans S type.
        if( ((AT24Functional_type)load_s16(ramPnt, progPnt->Pnt_Functional) != UseLiftFunc) ) {
            //Lift mode disable - Выдаем параметры разгона/торможения по умолчанию
         
          accTime = load_s16(ramPnt, progPnt->Pnt_TaccDef); //!время разгона по умолчанию
          decTime = load_s16(ramPnt, progPnt->Pnt_TdecDef); //!время торможения по умолчанию
          
          save_float(ramPnt, progPnt->Pnt_ta1_Out, load_float(ramPnt, progPnt->Pnt_tA1_4));       //!начальный участок разгона для новой скорости
          save_float(ramPnt, progPnt->Pnt_ta2_Out, load_float(ramPnt, progPnt->Pnt_tA2_4));       //!конечный участок разгона для новой скорости
          save_float(ramPnt, progPnt->Pnt_ta3_Out, load_float(ramPnt, progPnt->Pnt_tA3_4));       //!начальный участок торможения для новой скорости
          save_float(ramPnt, progPnt->Pnt_ta4_Out, load_float(ramPnt, progPnt->Pnt_tA4_4));       //!конечный участок торможения для новой скорости
          save_s16(ramPnt, progPnt->Pnt_StypeOut, load_s16(ramPnt, progPnt->Pnt_StypeIn));    //!Передаем значение параметрв тип S-образной кривой на выход.
          
          save_s16(ramPnt, progPnt->Pnt_AccTime, accTime);  //!время разгона по умолчанию
          save_s16(ramPnt, progPnt->Pnt_DecTime, decTime);  //!время торможения по умолчанию          
          return &progPnt->Pnt_End;
        }
        
	//ограничиваем макс. значение уставок скорости при изменении предела fmax
	fmax = load_float(ramPnt, progPnt->Pnt_Fmax);   //!максимальная частота работы двигателя
	for(i = 0; i < MAX_NUM_SPD; i++) {
          if (load_float(ramPnt, progPnt->Pnt_Speed[i]) > fmax) {
            save_float(ramPnt, progPnt->Pnt_Speed[i], fmax);
          }
	}
                
	/******************************************Вывод флагов изменения частоты на логгер********************************************************/
        GlobalM4.DF_NumSpeed = load_s16(ramPnt, progPnt->Pnt_FreqNum);  // ((const s16*)&GlobalM3.s16_DF_NumSpeed),				// 136 - ????? ???????? ???????? ?? ?????????? ??????        
	//GTstate = load_s16(ramPnt, progPnt->Pnt_StateGT);              //!Состояние генератора темпа
	//GTDwn = (GTstate & (1 << FDwn)) && !(GTstate & (1 << FzeroRef)); //!Проверка флага понижения частоты.  GTDwn = 1 только при процессе понижения частоты
	//GTUp = load_s16(ramPnt, progPnt->Pnt_StateGT) & (1 << FUp);     //!Проверка флага роста частоты
	//mPnt3->s16_Debug5 = GTUp;                                 //!Передача в C-ядро - в функции ACIM_Vector_Calc()
	//mPnt3->s16_Debug6 = GTDwn;                                //!Передача в C-ядро - в функции ACIM_Vector_Calc()
	//mPnt3->s16_Debug4  = (s16)(((GlobalM4.Fref / 16777216.0) * 80.0) * 100);
	/*******************************************************************************************************************************************/

	primFinalRegions.rampSectors[0][0] = load_float(ramPnt, progPnt->Pnt_tA1_1); //!начальный участок разгона для спец. кривой s1
	primFinalRegions.rampSectors[0][1] = load_float(ramPnt, progPnt->Pnt_tA2_1); //!конечный участок разгона для спец. кривой s1
	primFinalRegions.rampSectors[0][2] = load_float(ramPnt, progPnt->Pnt_tA3_1); //!начальный участок торможения для спец. кривой s1
	primFinalRegions.rampSectors[0][3] = load_float(ramPnt, progPnt->Pnt_tA4_1); //!конечный участок торможения для спец. кривой s1

	primFinalRegions.rampSectors[1][0] = load_float(ramPnt, progPnt->Pnt_tA1_2); //!начальный участок разгона для спец. кривой s2
	primFinalRegions.rampSectors[1][1] = load_float(ramPnt, progPnt->Pnt_tA2_2); //!конечный участок разгона для спец. кривой s2
	primFinalRegions.rampSectors[1][2] = load_float(ramPnt, progPnt->Pnt_tA3_2); //!начальный участок торможения для спец. кривой s2
	primFinalRegions.rampSectors[1][3] = load_float(ramPnt, progPnt->Pnt_tA4_2); //!конечный участок торможения для спец. кривой s2

	primFinalRegions.rampSectors[2][0] = load_float(ramPnt, progPnt->Pnt_tA1_3); //!начальный участок разгона для спец. кривой s3
	primFinalRegions.rampSectors[2][1] = load_float(ramPnt, progPnt->Pnt_tA2_3); //!конечный участок разгона для спец. кривой s3
	primFinalRegions.rampSectors[2][2] = load_float(ramPnt, progPnt->Pnt_tA3_3); //!начальный участок торможения для спец. кривой s3
	primFinalRegions.rampSectors[2][3] = load_float(ramPnt, progPnt->Pnt_tA4_3); //!конечный участок торможения для спец. кривой s3

	primFinalRegions.rampSectors[3][0] = load_float(ramPnt, progPnt->Pnt_tA1_4); //!начальный участок разгона для кривой по умолчанию
	primFinalRegions.rampSectors[3][1] = load_float(ramPnt, progPnt->Pnt_tA2_4); //!конечный участок разгона для кривой по умолчанию
	primFinalRegions.rampSectors[3][2] = load_float(ramPnt, progPnt->Pnt_tA3_4); //!начальный участок торможения для кривой по умолчанию
	primFinalRegions.rampSectors[3][3] = load_float(ramPnt, progPnt->Pnt_tA4_4); //!конечный участок торможения для кривой по умолчанию

	primFinalRegions.specRef1 = load_s16(ramPnt, progPnt->Pnt_Stype1);  //!считываем номер скорости, кот. будет иметь индивидуальные параметры s-кривой
	primFinalRegions.specRef2 = load_s16(ramPnt, progPnt->Pnt_Stype2);  //!считываем номер скорости, кот. будет иметь индивидуальные параметры s-кривой
	primFinalRegions.specRef3 = load_s16(ramPnt, progPnt->Pnt_Stype3);  //!считываем номер скорости, кот. будет иметь индивидуальные параметры s-кривой
        
        // Фазировка энкодера
        EncoPhasingWork = testBitMicro (ramPnt, progPnt->Pnt_EncPhasingWork); // Проверяем активность режима фазировки энкодерае
        
        // Get Evacuation input data
        EvacOn  = testBitMicro(ramPnt, progPnt->Pnt_EvacOn);
        EvacDir = (EvacOutDir_enum)load_s16(ramPnt, progPnt->Pnt_EvacDir);          
                
	rawNumSpeed = load_s16(ramPnt, progPnt->Pnt_FreqNum);                                //!Считываем бит-код скорости numOfSpeed = 0...15

	//фильтруем бит-код скорости (берём значение только после полного завершения перекоммутации)
	speedFilterTime = TXT_FILTR_TO_CYCLES(load_s16(ramPnt, progPnt->Pnt_SpeedDI));
	if(rawNumSpeed != sPnt->oldRawNumSpeed) {
          sPnt->speedFilterCounter = 0;
          sPnt->oldRawNumSpeed = rawNumSpeed;
	} else {
          if(sPnt->speedFilterCounter < speedFilterTime) {
                  sPnt->speedFilterCounter++;
          } else {
                  sPnt->newNumSpeed = rawNumSpeed;
          }
	}        
        
        // Check Evacuation mode
        if (EvacOn) { // If Evacuation mode start, set evacuation speed code without bit-code filtration
          sPnt->newNumSpeed = rawNumSpeed = EVACUATION_SPD_CODE; // Evacuation speed Code = 16. 
        } 
        // Если есть фазировка энкодера - то заменяем код скорости
        if (EncoPhasingWork) {
          sPnt->newNumSpeed = rawNumSpeed = ENCOPHASING_SPD_CODE; // Autophasing spd code = 17
        }        
        
        // Search speed index by the speed number
	for(i = 0; i < MAX_NUM_SPD; i++) { // Check all speed number
          IndexSpeed = load_s16(ramPnt, progPnt->Pnt_CodeSpeed[i]); // 
          IndexSpeed &= 0x1F; // Select by mask only active speedCode bit - 0bzxxxxx: z - hide speed bit for wizard, xxxxx - active speed code bit
          if (IndexSpeed == sPnt->newNumSpeed) { // Speed number are equal?
            sPnt->newIndexSpeed = i; // Yes, set new speed index
            break;
          }
	}
               
	//если присутствует флаг остановки ПЧ, обнуляем задание скорости независимо от битовой комбинации скорости
	if(testBitMicro(ramPnt, progPnt->Pnt_StopOut)){
		newOutRef = 0;
		sPnt->newIndexSpeed = -1; // !! 
	}else{
                if (sPnt->newIndexSpeed != -1) {
                    newOutRef = load_float(ramPnt, progPnt->Pnt_Speed[sPnt->newIndexSpeed]); // !!
                } else { // If speed index = -1, set Zero speed
                    newOutRef = 0;
                }
                // Check Evacuation Direction. Second step
                if (EvacOn) {                  
                  newOutRef = newOutRef*(f32)EvacDir; // EvacDir give a current direction for speed reference. No need switch/case construction for this.
                  // EvacDir 
                  // 0 - search light direction
                  // 1 - forward direction
                  // -1 - backward direction                 
                }
                
	}
	save_float(ramPnt, progPnt->Pnt_OutRef, newOutRef);     //!выдаем новое задание скорости по бит-коду скорости

	fCur = fabsf(load_float(ramPnt, progPnt->Pnt_Fref)); //!текущая частота работы двигателя

	primFinalRegions.newNumSpeed = sPnt->newIndexSpeed; // newIndexSpeed
	primFinalRegions.prevNumSpeed = sPnt->prevIndexSpeed;
	primFinalRegions.newOutRef = newOutRef;

	if (!testBitMicro(ramPnt, progPnt->Pnt_Start))          //!если ШИМ-включен
	{
          if (testBitMicro(ramPnt, progPnt->Pnt_ChRefDin))      //!если тип задания - дискретные входы
          {
            prevOutRef = (sPnt->prevIndexSpeed != -1) ? load_float(ramPnt, progPnt->Pnt_Speed[sPnt->prevIndexSpeed]) : 0; //!предыдущее задание (по факту, ПЧ щас работает на нем)
            primFinalRegions.prevOutRef = prevOutRef;
            if ((newOutRef != prevOutRef) && (fabsf(newOutRef) > fabsf(fCur))) //!если скорость, соотв. новому поступившему номеру скорости изменилась и больше текущей скорости
            {                                        //!разгоняемся со временем, соотв. новому бит-коду скорости
              accTime = load_s16(ramPnt, progPnt->Pnt_Tacc[sPnt->newIndexSpeed]); // !!                //!считываем время разгона до скорости, соотв. текущему поступившему бит-коду
              decTime = load_s16(ramPnt, progPnt->Pnt_Tdec[sPnt->newIndexSpeed]); // !!                //!ну и что бы было, выдам какое-то время торможения
              decTime = testBitMicro(ramPnt, progPnt->Pnt_StopExt) ? load_s16(ramPnt, progPnt->Pnt_tExtStop) : decTime; //!если задан останов по внешнему стопу, то переустанавливаем время торможения
              
              
              if(load_s16(ramPnt, progPnt->Pnt_Tcalc))                                    //!Если включен второй алгоритм задания времени,то  персчитываем время
              {
                deltaF = fabsf(newOutRef - fCur);                                        //!разница между новым заданием и текущей скоростью
                temp1 = (accTime * 0.1f) * (fmax / deltaF); //!пересчитанное время в формате iq                                                                                
                temp2 = (decTime * 0.1f) * (fmax / deltaF); //!пересчитанное время в формате iq
                accTime = (u16)((temp1) * 10.0 + 0.5);                                //!число с одним знаком после точки приводим к целому значению
                decTime = (u16)((temp2) * 10.0 + 0.5);
              }
              
              sPnt->prevIndexSpeed = sPnt->newIndexSpeed; //!!  //!запоминаем номер скорости на которую выходим и работаем на ней
              save_s16(ramPnt, progPnt->Pnt_AccTime, accTime); //!выдаем в генератор темпа время разгона
              save_s16(ramPnt, progPnt->Pnt_DecTime, decTime); //!выдаем в генератор темпа время торможения
              sPnt->prevAccTime = accTime;                     //!эти значения будем выдавать когда бит-код установится на определенном значении
              sPnt->prevDecTime = decTime;                     //!эти значения будем выдавать когда бит-код установится на определенном значении
            }
            else if (newOutRef == prevOutRef || (fabsf(fCur - newOutRef) == 0.0f) ) //!если задание не изменилось. хотя может пришел другой код скорости
            {
              save_s16(ramPnt, progPnt->Pnt_AccTime, sPnt->prevAccTime);
              save_s16(ramPnt, progPnt->Pnt_DecTime, sPnt->prevDecTime);
            }
            else //!иначе, скорость по новому бит-коду меньше чем скорость по предыдущему бит-коду (или ваще пришло нулевое задание)
            {
              //!тормозимся с временем торможения, соотв. скорости, на которой в данный момент работает ПЧ (понижаемся от текущей частоты к новой меньшей)
              decTime = load_s16(ramPnt, progPnt->Pnt_Tdec[sPnt->prevIndexSpeed]); //!время торможения, соотв. предыдущему бит-коду скорости, на которой в данный момент работает ПЧ
              accTime = load_s16(ramPnt, progPnt->Pnt_Tacc[sPnt->prevIndexSpeed]);
              if(load_s16(ramPnt, progPnt->Pnt_Tcalc))
              {
                deltaF = fabsf(fCur - newOutRef); //!разница между текущей рабочей частотой и новым меньшим заданием
                temp1 = (decTime * 0.1f) * (fmax / deltaF);
                temp2 = (accTime * 0.1f) * (fmax / deltaF);
                decTime = (u16)((temp1) * 10.0 + 0.5);
                accTime = (u16)((temp2) * 10.0 + 0.5);
              }
              
              sPnt->prevIndexSpeed = sPnt->newIndexSpeed; // !! //!запоминаем номер скорости на которую выходим и работаем на ней
              //sPnt->prevNumSpeed = -1;
              save_s16(ramPnt, progPnt->Pnt_AccTime, accTime); //!выдаем в генератор темпа время разгона
              save_s16(ramPnt, progPnt->Pnt_DecTime, decTime); //!выдаем в генератор темпа время торможения
              sPnt->prevAccTime = accTime;
              sPnt->prevDecTime = decTime;
            }
            i = rampParamSel(&primFinalRegions);                                              //!определяем начальные и конечные участки разгона/торможения
            save_float(ramPnt, progPnt->Pnt_ta1_Out, primFinalRegions.rampSectors[i][0]);       //!начальный участок разгона для новой скорости
            save_float(ramPnt, progPnt->Pnt_ta2_Out, primFinalRegions.rampSectors[i][1]);       //!конечный участок разгона для новой скорости
            save_float(ramPnt, progPnt->Pnt_ta3_Out, primFinalRegions.rampSectors[i][2]);       //!начальный участок торможения для новой скорости
            save_float(ramPnt, progPnt->Pnt_ta4_Out, primFinalRegions.rampSectors[i][3]);       //!конечный участок торможения для новой скорости
            
            if(testBitMicro(ramPnt, progPnt->Pnt_DFSelTime)){ //сигнал изменение темпа разгона/торможения (SELT)
              accTime = load_s16(ramPnt, progPnt->Pnt_AccTime);     //текущее время разгона
              decTime = load_s16(ramPnt, progPnt->Pnt_DecTime);     //текущее время торможения
              accDinTime = load_float(ramPnt, progPnt->Pnt_ResOut9);  //время разгона по сигналу на Din
              decDinTime = load_float(ramPnt, progPnt->Pnt_ResOut10); //время торможения по сигналу на Din
              
              temp1 = (accTime * 0.1f) * accDinTime;  //измененное время разгона
              accTime = (u16)((temp1) * 10.0 + 0.5);
              temp1 =  (decTime * 0.1f) * decDinTime;  //измененное время торможения
              decTime = (u16)((temp1) * 10.0 + 0.5);
              
              save_s16(ramPnt, progPnt->Pnt_AccTime, accTime); //измененное время разгона
              save_s16(ramPnt, progPnt->Pnt_DecTime, decTime); //измененное время торможения
            }else{ //нет сигнала выбора темпа
              save_s16(ramPnt, progPnt->Pnt_AccTime, sPnt->prevAccTime);
              save_s16(ramPnt, progPnt->Pnt_DecTime, sPnt->prevDecTime);
            }
            
          }
          else //!иначе, дискретные входы не используются в качестве задания. Выдаем параметры разгона/торможения по умолчанию
          {
            accTime = load_s16(ramPnt, progPnt->Pnt_TaccDef); //!время разгона по умолчанию
            decTime = load_s16(ramPnt, progPnt->Pnt_TdecDef); //!время торможения по умолчанию
            
            save_float(ramPnt, progPnt->Pnt_ta1_Out, primFinalRegions.rampSectors[3][0]);       //!начальный участок разгона для новой скорости
            save_float(ramPnt, progPnt->Pnt_ta2_Out, primFinalRegions.rampSectors[3][1]);       //!конечный участок разгона для новой скорости
            save_float(ramPnt, progPnt->Pnt_ta3_Out, primFinalRegions.rampSectors[3][2]);       //!начальный участок торможения для новой скорости
            save_float(ramPnt, progPnt->Pnt_ta4_Out, primFinalRegions.rampSectors[3][3]);       //!конечный участок торможения для новой скорости
            
            save_s16(ramPnt, progPnt->Pnt_AccTime, accTime);  //!время разгона по умолчанию
            save_s16(ramPnt, progPnt->Pnt_DecTime, decTime);  //!время торможения по умолчанию
          }
	}
	else {//!попадаем сюда после отключения ШИМ (полной остановки)
          primFinalRegions.newNumSpeed = 0;    //! ШИМ отключен - новая скорость 0
          primFinalRegions.newOutRef = 0;
          
          i = rampParamSel(&primFinalRegions); //!в зависимости от предыдущей определяем начальные и конечный участок торможения
          
          save_float(ramPnt, progPnt->Pnt_ta1_Out, primFinalRegions.rampSectors[i][0]);       //!начальный участок разгона для новой скорости
          save_float(ramPnt, progPnt->Pnt_ta2_Out, primFinalRegions.rampSectors[i][1]);       //!конечный участок разгона для новой скорости
          save_float(ramPnt, progPnt->Pnt_ta3_Out, primFinalRegions.rampSectors[i][2]);       //!начальный участок торможения для новой скорости
          save_float(ramPnt, progPnt->Pnt_ta4_Out, primFinalRegions.rampSectors[i][3]);       //!конечный участок торможения для новой скорости
          
          sPnt->prevIndexSpeed    = -1; //!сброс при первом вызове микропрограммы или после отключения ШИМ sPnt->prevIndexSpeed = 0 - вообще еще не было никаких скоростей
          sPnt->prevAccTime     = 0;
          sPnt->prevDecTime     = 0;
          sPnt->extStopflag     = 0;
	}
        
	if ( ((testBitMicro(ramPnt, progPnt->Pnt_StopExt)) || (sPnt->extStopflag)) && sPnt->prevIndexSpeed != -1) //!если поступил сигнал внешнего останова (или поступил, а потом исчез, но привод еще не успел остановиться)
	{
          extDecTime = load_s16(ramPnt, progPnt->Pnt_tExtStop);                        //!время торможения по внешнему останову
          prevOutRef = load_float(ramPnt, progPnt->Pnt_Speed[sPnt->prevIndexSpeed]); //!текущая рабочая скорость, на которую вышли
          deltaF = prevOutRef;                                                       //!тормозимся до нулевого задания, поэтому приращение равно текущей скорости
          if(load_s16(ramPnt, progPnt->Pnt_Tcalc))                                   //!если работает по второму алгоритму задания времени
          {
            temp1 = (extDecTime * 0.1f) * (fmax / deltaF);
            extDecTime = (u16)((temp1) * 10.0);      //!корректируем время останова
          }
          save_s16(ramPnt, progPnt->Pnt_DecTime, extDecTime);  //!выдаем время внешнего останова
          sPnt->extStopflag = 1;                               //!ставим флаг останова по внешнему сигналу на тот случай, если сигнал внешнего останова исчезнет до полного останова
	}
        
	//===========================================================================
	// Проверяем состояние сигналов выбора индивидуальных S-образных кривых.
	// Если хотя бы для одной из скоростей выбрано значение скорости, надо сразу сменить параметр Тип S-обр на пользователь.
	if (primFinalRegions.specRef1 != 0 || primFinalRegions.specRef2 != 0 || primFinalRegions.specRef3 != 0) {
		save_s16(ramPnt, progPnt->Pnt_StypeIn, 1); // Ставим режим - Пользователь.
	}
	save_s16(ramPnt, progPnt->Pnt_StypeOut, load_s16(ramPnt, progPnt->Pnt_StypeIn)); // Передаем значение параметрв тип S-образной кривой на выход.

//	//==================================================================
//	//Функция компенсации момента по уровню аналогового сигнала
//	//датчика загруженности кабины лифта
//	//================================================================
//	currentDelay = load_s16(ramPnt, progPnt->Pnt_ResOut2); //!Задержка срабатывания функции компенсации момента по Ain, мс
//	currentDelay = (u16)((currentDelay * 0.001f) * K_TIME_CALL_MICRO_TAKT);
//
//	if(load_s16(ramPnt, progPnt->Pnt_EnTimeComp)){                   //! Параметр разрешения функции
//		offsetCurr = load_float(ramPnt, progPnt->Pnt_tDownGen);        //! Ток смещения, % от тока удержания
//		startIntegrValue = load_float(ramPnt, progPnt->Pnt_tUpDvig);   //! Максимальный ток удержания для антиотката
//		switch(load_s16(ramPnt, progPnt->Pnt_tDownDvig)){            //! Проверка выбора аналогового входа
//		case notSelected:
//			Ain = 0;
//			break;
//		case Ain1:
//			Ain = load_float(ramPnt, progPnt->Pnt_IactGenMax);   //!Уровень Ain1
//			break;
//		case Ain2:
//			Ain = load_float(ramPnt, progPnt->Pnt_IactDvigMax);  //!Уровень Ain2
//			break;
//		}
//
//		//=== startIntegrCurVal = 2 * startIntegrValue * (1 - offsetCurr) * Ain - startIntegrValue ====//
//		temp1 = startIntegrValue * ((1.0f - offsetCurr) * 2);
//		temp1 = (temp1* Ain) - startIntegrValue;
//        //=============================================================================================//
//
//		temp1 = !load_s16(ramPnt, progPnt->Pnt_tUpGen) ? temp1 : -temp1; //!Учет инверсии тока
//
//		//управление сбросом регулятора скорости
//		switch(sPnt->piSpdCmdState){
//			case(brakeOnWaitState): //если ждем команду снятия тормоза
//			   if(testBitMicro(ramPnt, progPnt->Pnt_LiftBrakeOff)){ //если тормоз сняли..
//				   sPnt->cnt = currentDelay;                 //заряжаем таймер и
//				   sPnt->piSpdCmdState = countState;       //...переходим в состояние отчета времени
//			   }
//			break;
//			case(countState): //состояние отчета до команды сброса регулятора скорости
//				if(!sPnt->cnt){ //если таймер дотикал
//					sPnt->piSpdCmdState = piSpdInitState; //переходим в состояние сброса регулятора скорости
//				}else{
//					sPnt->cnt--;
//				}
//			break;
//			case(piSpdInitState): //состояние установки начальных условий интегрирования регулятора
//				GlobalM4.startIntegrCurVal = temp1; //!..Выдаем ток
//				GlobalM4.pi_spd_reset = 1;    //Команда на установку начального значения интегрирования
//				sPnt->piSpdCmdState = brakeOffWaitState; //переход в состояние ожидания повторного наложения тормоза
//			break;
//			case(brakeOffWaitState): //состояние ожидания наложения тормоза
//				if(!testBitMicro(ramPnt, progPnt->Pnt_LiftBrakeOff)){//если тормоз снова наложили..
//					sPnt->piSpdCmdState = brakeOnWaitState; //переходим в состояние ожидания снятия тормоза
//					GlobalM4.startIntegrCurVal = 0;
//					GlobalM4.pi_spd_reset = 0;
//				}
//			break;
//		}
//
//	}else{ //!Функция компенсации момента по Ain отключена. Все сбрасываем
//		GlobalM4.startIntegrCurVal = 0;
//		GlobalM4.pi_spd_reset = 0;
//		sPnt->piSpdCmdState = brakeOnWaitState;
//	}

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncRAMP_CTRL_MOD2_1[136]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Tacc
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Tacc0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Tacc1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Tacc2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Tacc3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Tacc4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Tacc5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Tacc6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Tacc7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Tacc8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Tacc9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Tacc10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Tacc11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Tacc12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Tacc13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Tacc14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Tacc15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Tacc16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Tacc17
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Tdec
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Tdec0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Tdec1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Tdec2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  Tdec3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Tdec4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  Tdec5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  Tdec6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Tdec7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  Tdec8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  Tdec9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  Tdec10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  Tdec11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  Tdec12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  Tdec13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  Tdec14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  Tdec15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  Tdec16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38  Tdec17
        S32_PIN_TYPE | INPUT_PIN_MODE,     //39  Speed0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //40  Speed1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //41  Speed2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //42  Speed3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //43  Speed4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //44  Speed5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //45  Speed6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //46  Speed7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //47  Speed8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //48  Speed9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //49  Speed10
        S32_PIN_TYPE | INPUT_PIN_MODE,     //50  Speed11
        S32_PIN_TYPE | INPUT_PIN_MODE,     //51  Speed12
        S32_PIN_TYPE | INPUT_PIN_MODE,     //52  Speed13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //53  Speed14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //54  Speed15
        S32_PIN_TYPE | INPUT_PIN_MODE,     //55  Speed16
        S32_PIN_TYPE | INPUT_PIN_MODE,     //56  Speed17
        S16_PIN_TYPE | INPUT_PIN_MODE,     //57  Tcalc
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //58  SelAccel
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //59  StopExt
        S16_PIN_TYPE | INPUT_PIN_MODE,     //60  FreqNum
        S16_PIN_TYPE | INPUT_PIN_MODE,     //61  StateGT
        S16_PIN_TYPE | INPUT_PIN_MODE,     //62  SpeedDI
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //63  Start
        S16_PIN_TYPE | INPUT_PIN_MODE,     //64  Stype1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //65  tA1_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //66  tA2_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //67  tA3_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //68  tA4_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //69  Stype2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //70  tA1_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //71  tA2_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //72  tA3_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //73  tA4_2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //74  Stype3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //75  tA1_3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //76  tA2_3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //77  tA3_3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //78  tA4_3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //79  LiftBrakeOff
        S32_PIN_TYPE | INPUT_PIN_MODE,     //80  tA1_4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //81  tA2_4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //82  tA3_4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //83  tA4_4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //84  tExtStop
        S32_PIN_TYPE | INPUT_PIN_MODE,     //85  Fmax
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //86  ChRefDin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //87  StypeIn
        S32_PIN_TYPE | INPUT_PIN_MODE,     //88  Fref
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //89  DFSelTime
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //90  EvacOn        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //91  EvacDir
        S16_PIN_TYPE | INPUT_PIN_MODE,     //92  EncPhasingEn
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //93  EncPhasingWork                
        S16_PIN_TYPE | INPUT_PIN_MODE,     //94  Functional 
        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //95  AccTime
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //96  DecTime
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //97  OutRef
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //98  StypeOut
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //99  ResOut2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //100  ResOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //101  ResOut4
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //102  ResOut5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //103  ResOut6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //104  ResOut7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //105  ResOut8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //106  ResOut9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //107  ResOut10        
        S16_PIN_TYPE | INPUT_PIN_MODE,     //108  CodeSpeed0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //109  CodeSpeed1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //110  CodeSpeed2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //111  CodeSpeed3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //112  CodeSpeed4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //113  CodeSpeed5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //114  CodeSpeed6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //115  CodeSpeed7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //116  CodeSpeed8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //117  CodeSpeed9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //118  CodeSpeed10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //119  CodeSpeed11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //120  CodeSpeed12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //121  CodeSpeed13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //122  CodeSpeed14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //123  CodeSpeed15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //124  CodeSpeed16                
        S16_PIN_TYPE | INPUT_PIN_MODE,     //125  CodeSpeed17                
        S16_PIN_TYPE | INPUT_PIN_MODE,     //126  tDownDvig
        S32_PIN_TYPE | INPUT_PIN_MODE,     //127  tUpDvig
        S32_PIN_TYPE | INPUT_PIN_MODE,     //128  tDownGen
        S16_PIN_TYPE | INPUT_PIN_MODE,     //129  tUpGen
        S32_PIN_TYPE | INPUT_PIN_MODE,     //130  IactDvigMax
        S32_PIN_TYPE | INPUT_PIN_MODE,     //131  IactGenMax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //132  EnTimeComp
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //133  ta1_Out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //134  ta2_Out
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //135  ta3_Out
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //136  ta4_Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("RAMP_CTRL_MOD2", "DA", TblFuncRAMP_CTRL_MOD2_1, FuncRAMP_CTRL_MOD2_1, sizeof(TblFuncRAMP_CTRL_MOD2_1), sizeof(RAMPCTRL2_1_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncRAMP_CTRL_MOD2_1,
#endif
//********************************************************
