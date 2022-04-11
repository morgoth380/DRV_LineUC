#ifdef    GET_FUNC_CODE

//--------------------------------------------------------------------
//--------------------------------------------------------------------
typedef struct{
        u16 Holiday;
        u16 PrevDayOfYear;
        u16 PrevYear;
        u16 PrevView;   //����������� ��������� �� ���
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
        sPnt->PrevDayOfYear = 0xFFFF;                                   //�������� �� ���������� ����
        sPnt->PrevYear = 0xFFFF;                                        //�������� �� ���������� ���
        sPnt->PrevView = 0xFFFF;//load_s16(ramPnt, progPnt->Pnt_View);  //��������� �������� ������������ ������ ���
        sPnt->PrevDayMonth = load_s16(ramPnt, progPnt->Pnt_DayMonth);   //��������� �������� ����������� ��� 0...365
    }
    else{
        UnixTime = load_s32(ramPnt, progPnt->Pnt_UnixTime); //��������� ������� UNIX-�����
        ch_to_smhdmy(&Time, UnixTime);                      //��������� ������ � ������� ������� �������� ���

        if ((Time.year & 0x01)== 0) AdrFr = (u32)&nv.file.HolidayEven[0]; //������ ���
        else                        AdrFr = (u32)&nv.file.HolidayOdd[0];  //�������� ���
        
        //i = load_s16(ramPnt, progPnt->Pnt_DayMonth) - 1; //sikalets
        i = load_s16(ramPnt, progPnt->Pnt_DayMonth); //��������� ���������� ���� ����, ��� �������� ����� �������� ��� ��� (��������/�������)
        if (sPnt->PrevDayMonth != i || sPnt->PrevYear != Time.year){
            sPnt->PrevDayMonth = i;

/////////            NumByte = i / 16;
/////////            NumBit  = i % 16;
            NumByte = i >> 4 ;
            NumBit  = i & 0xf;

            RdFramMicro(&i, (void *)(AdrFr + NumByte * 2), 2);

            i = (i & (1 << NumBit)) ? 1 : 0;
            save_s16(ramPnt, progPnt->Pnt_View, i);
            sPnt->PrevView = load_s16(ramPnt, progPnt->Pnt_View);   //����� �� �������� ������ ����������
        }

        //������ �������� ���� ��� (�������/��������)
        i = load_s16(ramPnt, progPnt->Pnt_View);  //��������� ��� ���  (�������/��������)
        if (sPnt->PrevView != i){   //���� ��������� ��� ��� ���������� �� ����������� ����������, �� ���� ������� ����� ��������
            sPnt->PrevView = i;     //��������� ����� ��������� ��� ��� (�������/��������) � ����� ����� �������� ��� ������ ������ ��� ���� �������� ��� ���
            i = load_s16(ramPnt, progPnt->Pnt_DayMonth); //��������� ���������� ����� ��� (���� ����), ��� �������� ��������
/////////            NumByte = i / 16;
/////////            NumBit  = i % 16;
            NumByte = i >> 4 ; //������� �����, � ������� ��������� ���, �������� �������� ���������� ��� ��� (��������/�������)
            NumBit  = i & 0xf; //������� ����� ����, ������� ������������� ��� ����
            RdFramMicro(&i, (void *)(AdrFr + NumByte * 2), 2); //��������� ������� �������� �����, � ������� ���������� ��������������� ���������� ��� ���
            
            if (sPnt->PrevView) i |=  (1 << NumBit);                //������������ ���� 1 (��������). ���� ������ ��������, �� ���������� � 1 ���, ������ ���
            else                i &= ~(1 << NumBit);                //�����, ���� - �������. �������� ���
            WrFramMicro((void *)(AdrFr + NumByte * 2), &i, 2);      //���������� ����� �������� �����
        }

// ������ ����� ��� ��� ��������� ����������???????????????????????????????????????????????????
        QntDayFromYear = monthSumEx[Time.month] + Time.date;    //���������� ���������� ����� � ���� �������� ���
        NumByte = QntDayFromYear / 16;   //������� �����, � ������� ��������� ���, ��������������� �������� ��� ����
        NumBit = QntDayFromYear % 16;    //���������� ����� ����, ��������������� �������� ��� ����
        
        if ((sPnt->PrevDayOfYear != QntDayFromYear) || (sPnt->PrevYear != Time.year)){ //���� ��������� ���� ��� ���
            sPnt->PrevYear = Time.year;    //��������� ����� ��� (��� ���� ����������� � ������, ����� ������� ������ ���� ��� ���)
            sPnt->PrevDayOfYear = QntDayFromYear; //��������� ����� ����
            RdFramMicro(&i, (void *)(AdrFr + NumByte * 2), 2); //��������� �����, � ������� ��������� ���, ��������������� �������� ���
            sPnt->Holiday = (i & (1 << NumBit)) ? 1 : 0; //����� ����� ������� ����: �������� ��� �������
        }

/*      Tmp = DayOfWeek(&Time);
        if (sPnt->Holiday == 1 ||             //�������� �� ������� ���������� ��� ������� ��� �����������
            Tmp == 0 || Tmp == 6){*/
      if (sPnt->Holiday == 1){                       //���� �������� ��� ������� ��� �����������, �� ����� �������� �� ����������� �������� �������
          setBitMicro(ramPnt, progPnt->Pnt_Hol__Wrk);   //������ ����, ������������, ��� �������� � ������ ��������� ���
          Pnt = &progPnt->Pnt_HolMas[0][0];             //�������� ����� �������� ��� ������� � ����� ������ �������, ��������������� ��������� ���
      }
      else{                                             //�����
          Pnt = &progPnt->Pnt_WrkMas[0][0];             //�������� ����� �������� ��� ������� � ����� ������ �������, ��������������� ��������  ���
          clrBitMicro(ramPnt, progPnt->Pnt_Hol__Wrk);   //������ ����, ������������, ��� �������� � ������ �������� ���
      }

 //�� ������ ������ ����� ����� ������� ���� � ����� � ����� ������ ������� �������� (� ������ ��������� ��� ��� ������ �������� ���). ����� � ����� �������� �� ����
 //��������� ����������, �� ������� ������ ������� ���� (����� 10 ���������� �������). ������� � ����� �� ����������� ����������� ������� �����, � ������
 //�������� �������, ��������������� �������� �������
      for (i = 0; i < 10; i ++){                         //�������������� � ������ HourMin � ��������
          QntMinT0 = load_s16(ramPnt, *(Pnt+i*2+1)); //������ �������� �������, ������������� �� ��������� ��� �����

          if (i != 9)
              QntMinT1 = load_s16(ramPnt, *(Pnt+(i+1)*2+1)); //������ �������� �������, ������������� �� ��������� ��� ������
          else
              QntMinT1 = 1440;                                  //23:59 (1440 = 24 ���� x 60 ����� - ����� �����)

          QntMinJ = Time.hour * 60 + Time.minute; //�������� ������� ����� (� �������)

          if (QntMinJ >= QntMinT0 && QntMinJ < QntMinT1){ //���� �������� �������, � ������� ����������� ������� �����
              Task = load_s32(ramPnt, *(Pnt+i*2)); //������ �������� �������, �����. ������� ���������� ���������
              break;
          }
          else Task = 0;
      }
      save_s32(ramPnt, progPnt->Pnt_Task, Task); //������ � �������������� �������� �������
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
