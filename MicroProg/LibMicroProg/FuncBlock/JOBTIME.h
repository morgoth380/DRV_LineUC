#ifdef    GET_FUNC_CODE

//--------------------------------------------------------------------
//--------------------------------------------------------------------
typedef struct{
        u16 Holiday;
        u16 PrevDayOfYear;
        u16 PrevYear;
        u16 PrevView;   //Просмотрщик праздника по дню
        u16 PrevDayMonth;
}StateFuncJOBTIME_type;
//--------------------------------------------------------------------
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;    
    //u16  Pnt_nvState;
    u16 Pnt_WrkMas[10][2];                 //1...20
    u16 Pnt_HolMas[10][2];                 //21...40
/*    u16  Pnt_Job1;                       //1  Job1
    u16  Pnt_dTime1;                       //2  dTime1
    u16  Pnt_Job2;                         //3  Job2
    u16  Pnt_dTime2;                       //4  dTime2
    u16  Pnt_Job3;                         //5  Job3
    u16  Pnt_dTime3;                       //6  dTime3
    u16  Pnt_Job4;                         //7  Job4
    u16  Pnt_dTime4;                       //8  dTime4
    u16  Pnt_Job5;                         //9  Job5
    u16  Pnt_dTime5;                       //10  dTime5
    u16  Pnt_Job6;                         //11  Job6
    u16  Pnt_dTime6;                       //12  dTime6
    u16  Pnt_Job7;                         //13  Job7
    u16  Pnt_dTime7;                       //14  dTime7
    u16  Pnt_Job8;                         //15  Job8
    u16  Pnt_dTime8;                       //16  dTime8
    u16  Pnt_Job9;                         //17  Job9
    u16  Pnt_dTime9;                       //18  dTime9
    u16  Pnt_Job10;                        //19  Job10
    u16  Pnt_dTime10;                      //20  dTime10
    u16  Pnt_Job1_1;                       //21  Job1
    u16  Pnt_dTime1_1;                     //22  dTime1
    u16  Pnt_Job2_1;                       //23  Job2
    u16  Pnt_dTime2_1;                     //24  dTime2
    u16  Pnt_Job3_1;                       //25  Job3
    u16  Pnt_dTime3_1;                     //26  dTime3
    u16  Pnt_Job4_1;                       //27  Job4
    u16  Pnt_dTime4_1;                     //28  dTime4
    u16  Pnt_Job5_1;                       //29  Job5
    u16  Pnt_dTime5_1;                     //30  dTime5
    u16  Pnt_Job6_1;                       //31  Job6
    u16  Pnt_dTime6_1;                     //32  dTime6
    u16  Pnt_Job7_1;                       //33  Job7
    u16  Pnt_dTime7_1;                     //34  dTime7
    u16  Pnt_Job8_1;                       //35  Job8
    u16  Pnt_dTime8_1;                     //36  dTime8
    u16  Pnt_Job9_1;                       //37  Job9
    u16  Pnt_dTime9_1;                     //38  dTime9
    u16  Pnt_Job10_1;                      //39  Job10
    u16  Pnt_dTime10_1;                    //40  dTime10*/
    u16  Pnt_DayMonth;                     //41  DayMonth
    u16  Pnt_View;                         //42  View
    u16  Pnt_UnixTime;                     //43  UnixTime
    u16  Pnt_dTChJob;                      //44  dTChJob
    u16  Pnt_Hol__Wrk;                     //45  Hol/~Wrk
    u16  Pnt_Task;                         //46  Task
    u16  Pnt_End;
}FuncJOBTIME_type;
//********************************************************
u16 *FuncJOBTIME_1(FuncJOBTIME_type *progPnt, u32 ramPnt)
{
//#ifdef _PROJECT_FOR_CCS_
    StateFuncJOBTIME_type               *sPnt;
    time_type                           Time;
    u32                                 UnixTime;
    u16                                 QntDayFromYear;
    u16                                 i, QntMinT0, QntMinT1, QntMinJ;
    s32                                 Task;
    u16                                 *Pnt;//[10][2];
    u16                                 NumByte, NumBit;
    u32                                 AdrFr;
//    u16 DayInMonth;

    sPnt = (StateFuncJOBTIME_type *)(ramPnt + progPnt->Pnt_State);

    if (flagIsFirstCall){
        sPnt->PrevDayOfYear = 0xFFFF;                                   //Заведомо не правильный день
        sPnt->PrevYear = 0xFFFF;                                        //Заведомо не правильный год
        sPnt->PrevView = 0xFFFF;//load_s16(ramPnt, progPnt->Pnt_View);  //Начальное значение просмотрщика одного дня
        sPnt->PrevDayMonth = load_s16(ramPnt, progPnt->Pnt_DayMonth);   //Начальное значение изменяемого дня 0...365
    }
    else{
        UnixTime = load_s32(ramPnt, progPnt->Pnt_UnixTime); //считываем текущее UNIX-время
        ch_to_smhdmy(&Time, UnixTime);                      //формируем объект с данными времени текущего дня

        if ((Time.year & 0x01)== 0) AdrFr = (u32)&nv.file.HolidayEven[0]; //четный год
        else                        AdrFr = (u32)&nv.file.HolidayOdd[0];  //нечетный год
        
        //i = load_s16(ramPnt, progPnt->Pnt_DayMonth) - 1; //sikalets
        i = load_s16(ramPnt, progPnt->Pnt_DayMonth); //считываем порядковый день года, для которого будем задавать его тип (выходной/рабочий)
        if (sPnt->PrevDayMonth != i || sPnt->PrevYear != Time.year){
            sPnt->PrevDayMonth = i;

/////////            NumByte = i / 16;
/////////            NumBit  = i % 16;
            NumByte = i >> 4 ;
            NumBit  = i & 0xf;

            RdFramMicro(&i, (void *)(AdrFr + NumByte * 2), 2);

            i = (i & (1 << NumBit)) ? 1 : 0;
            save_s16(ramPnt, progPnt->Pnt_View, i);
            sPnt->PrevView = load_s16(ramPnt, progPnt->Pnt_View);   //Чтобы не поменять массив праздников
        }

        //Запись значение типа дня (рабочий/выходной)
        i = load_s16(ramPnt, progPnt->Pnt_View);  //считываем тип дня  (рабочий/выходной)
        if (sPnt->PrevView != i){   //если считанный тип дня отличается от предыдущего введенного, то есть введено новое значение
            sPnt->PrevView = i;     //сохраняем новый введенный тип дня (рабочий/выходной) и далее нужно уточнить для какого именно дня года меняется его тип
            i = load_s16(ramPnt, progPnt->Pnt_DayMonth); //считываем порядковый номер дня (день года), тип которого меняется
/////////            NumByte = i / 16;
/////////            NumBit  = i % 16;
            NumByte = i >> 4 ; //находим слово, в котором находится бит, значение которого показывает тип дня (выходной/рабочий)
            NumBit  = i & 0xf; //находим номер бита, который соответствует дню года
            RdFramMicro(&i, (void *)(AdrFr + NumByte * 2), 2); //считываем текущее значение слова, в котором расположен соответствующий введенному дню бит
            
            if (sPnt->PrevView) i |=  (1 << NumBit);                //Пользователь ввел 1 (праздник). если введен выходной, то установить в 1 бит, соотвю дню
            else                i &= ~(1 << NumBit);                //иначе, день - рабочий. Обнуляем бит
            WrFramMicro((void *)(AdrFr + NumByte * 2), &i, 2);      //записываем новое значение слова
        }

// почему любой год тут считается высокосным???????????????????????????????????????????????????
        QntDayFromYear = monthSumEx[Time.month] + Time.date;    //определяем порядковый номер в году ТЕКУЩЕГО дня
        NumByte = QntDayFromYear / 16;   //находим слово, в котором находится бит, соответствующий ТЕКУЩЕМУ дню года
        NumBit = QntDayFromYear % 16;    //определяем номер бита, соответствующий ТЕКУЩЕМУ дню года
        
        if ((sPnt->PrevDayOfYear != QntDayFromYear) || (sPnt->PrevYear != Time.year)){ //если изменился день или год
            sPnt->PrevYear = Time.year;    //Сохраняем новый год (это тело выполняется в момент, когда настает другой день или год)
            sPnt->PrevDayOfYear = QntDayFromYear; //сохраняем новый день
            RdFramMicro(&i, (void *)(AdrFr + NumByte * 2), 2); //считываем слово, в котором находится бит, соответствующий ТЕКУЩЕМУ дню
            sPnt->Holiday = (i & (1 << NumBit)) ? 1 : 0; //узнаём какой текущий день: выходной или рабочий
        }

/*      Tmp = DayOfWeek(&Time);
        if (sPnt->Holiday == 1 ||             //Праздник по массиву праздников или суббота или воскресенье
            Tmp == 0 || Tmp == 6){*/
      if (sPnt->Holiday == 1){                       //если праздник или суббота или воскресенье, то будем работать по праздничным уставкам частоты
          setBitMicro(ramPnt, progPnt->Pnt_Hol__Wrk);   //выдаем флаг, показывающий, что работаем в режиме выходного дня
          Pnt = &progPnt->Pnt_HolMas[0][0];             //получаем адрес смещения для доступа к самой первой уставке, соответствующей выходному дню
      }
      else{                                             //Будни
          Pnt = &progPnt->Pnt_WrkMas[0][0];             //получаем адрес смещения для доступа к самой первой уставке, соответствующей рабочему  дню
          clrBitMicro(ramPnt, progPnt->Pnt_Hol__Wrk);   //выдаем флаг, показывающий, что работаем в режиме рабочего дня
      }

 //на данный момент знаем какой сегодня день и знаем в каком режиме сегодня работаем (в режиме выходного дня или режиме рабочего дня). Далее в цикле проходим по всем
 //временным интервалам, на которые разбит текущий день (всего 10 интервалов времени). находим в какой из инетервалов вписывается текущее время, и выдаем
 //значение уставки, соответствующее текущему времени
      for (i = 0; i < 10; i ++){                         //Преобразование в формат HourMin и обрабтка
          QntMinT0 = load_s16(ramPnt, *(Pnt+i*2+1)); //читаем значение времени, расположенное на временной оси слева

          if (i != 9)
              QntMinT1 = load_s16(ramPnt, *(Pnt+(i+1)*2+1)); //читаем значение времени, расположенное на временной оси справа
          else
              QntMinT1 = 1440;                                  //23:59 (1440 = 24 часа x 60 минут - конец суток)

          QntMinJ = Time.hour * 60 + Time.minute; //получаем текущее время (в минутах)

          if (QntMinJ >= QntMinT0 && QntMinJ < QntMinT1){ //Ищем интервал времени, в который вписывается текущее время
              Task = load_s32(ramPnt, *(Pnt+i*2)); //выдаем величину задания, соотв. данному временному интервалу
              break;
          }
          else Task = 0;
      }
      save_s32(ramPnt, progPnt->Pnt_Task, Task); //выдаем в микропрограмму величину задания
      save_s16(ramPnt, progPnt->Pnt_dTChJob, QntMinT1 - QntMinJ);
    }//else flagfirstcall

//#endif
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncJOBTIME_1[46]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Job1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  dTime1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Job2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  dTime2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Job3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  dTime3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Job4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  dTime4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  Job5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  dTime5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  Job6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  dTime6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  Job7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  dTime7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  Job8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  dTime8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Job9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  dTime9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19  Job10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  dTime10
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21  Job1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  dTime1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //23  Job2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  dTime2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //25  Job3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  dTime3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //27  Job4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  dTime4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //29  Job5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  dTime5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //31  Job6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  dTime6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //33  Job7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  dTime7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //35  Job8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  dTime8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //37  Job9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38  dTime9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //39  Job10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //40  dTime10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41  DayMonth
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  View
        S32_PIN_TYPE | INPUT_PIN_MODE,     //43  UnixTime        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //44  dTChJob
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //45  Hol/~Wrk
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //46  Task
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
//OpisFuncBlock("JOBTIME", "DD", TblFuncJOBTIME_1, FuncJOBTIME_1, sizeof(TblFuncJOBTIME_1), sizeof(StateFuncJOBTIME_type), sizeof(nvStateFuncJOBTIME_type), 1,),
OpisFuncBlock("JOBTIME", "DD", TblFuncJOBTIME_1, FuncJOBTIME_1, sizeof(TblFuncJOBTIME_1), sizeof(StateFuncJOBTIME_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncJOBTIME_1,
#endif
//********************************************************
