#ifdef    GET_FUNC_CODE

typedef struct{
    u16 Fail;
    u16 CodeFail;
}AinFailsCode_type;

typedef struct{
    s32 Out;
    u8 FailAinFix;
}FuncSEL_REFState_type;

typedef struct{
    u16               Pnt_Nfunc;
    u16               Pnt_State;
    //u16  Pnt_nvState;
    u16               Pnt_SelRef1;       //1  SelRef1
    u16               Pnt_SelRef2;       //2  SelRef2
    u16               Pnt_UstSel;        //3  UstSel
    u16               Pnt_DinSel_1_2;    //4  DinSel_1_2
    u16               Pnt_In0_15[16];    //5-20  In0_15
    AinFailsCode_type Pnt_Ain[5];        //21 - 30
    u16               Pnt_FailAinEn;     //31 FailAinEn
    u16               Pnt_FixFreqMin;    //32 FixFreqMin
    u16               Pnt_FixFreqMax;    //33 FixFreqMax
    u16               Pnt_EnFailAin;     //34 EnFailAin
    u16               Pnt_CH_PID;        //35 CH_PID
    u16               Pnt_CH_REF_RP;     //36 CH_REF_RP
    u16               Pnt_CH_REF_LP;     //37 CH_REF_LP
    u16               Pnt_Out;           //38 Out
    u16               Pnt_End;
}FuncSEL_REF_type;
//********************************************************

enum{
	selCh1,
	selCh2,
	setDinSel
};

enum{
	chanel_LP  = 1,
	chanel_RP  = 2,
	chanel_DI  = 3,
	chanel_A1  = 4,
	chanel_A2  = 5,
	chanel_A3  = 6,
	chanel_A4  = 7,
	chanel_A5  = 8,
	chanel_PID = 10,
};

u16 *FuncSEL_REF_1(FuncSEL_REF_type *progPnt, u32 ramPnt)
{
	s32     Out;
	s16     selRef;
	FuncSEL_REFState_type *sPnt;

	sPnt = (FuncSEL_REFState_type *)(ramPnt + progPnt->Pnt_State);

	if(flagIsFirstCall){
	    sPnt->Out = 0;
	    sPnt->FailAinFix = 0;
	}

	switch(load_s16(ramPnt, progPnt->Pnt_UstSel)){ //�������� ����� ������ ������� (1-�, 2-� ��� �������� ���������� ����������� �����)
		case(selCh1):{ //���� ����� ������� ����� ����� 1 (selCh1 = 0)
			selRef = load_s16(ramPnt, progPnt->Pnt_SelRef1); //��������� ���������� ��� ��������� ������� ������ 1
			Out = load_s32(ramPnt, progPnt->Pnt_In0_15[selRef]); //�� ����������� ������ ��������� ���������� �������� �������, �� ���� �������� �������
		}break;
		case(selCh2):{ //���� ����� ������� ����� ����� 2 (selCh2 = 1)
			selRef = load_s16(ramPnt, progPnt->Pnt_SelRef2); //��������� ���������� ��� ��������� ������� ������ 2
			Out = load_s32(ramPnt, progPnt->Pnt_In0_15[selRef]);
		}break;
		case(setDinSel):{ //���� ����� ��������� ������ ������� �������� ���������� ������ �� ���������� ������ (setDinSel = 2), �� ��������� ����� ����� ������� ������
			if((testBitMicro(ramPnt, progPnt->Pnt_DinSel_1_2)) == 0){ //���� ������ 1-� �����
				selRef = load_s16(ramPnt, progPnt->Pnt_SelRef1);      //��������� ���������� ��� ��������� ������� ������ 1
				Out = load_s32(ramPnt, progPnt->Pnt_In0_15[selRef]);  //�� ����������� ������ ��������� ���������� �������� �������, �� ���� �������� �������
			}
			else{ //���� ������ ������ �����
				selRef = load_s16(ramPnt, progPnt->Pnt_SelRef2);      //��������� ���������� ��� ��������� ������� ������ 2
				Out = load_s32(ramPnt, progPnt->Pnt_In0_15[selRef]);  //�� ����������� ������ ��������� ���������� �������� �������, �� ���� �������� �������
			}
		}break;
		default:{
			Out = 0;
		}
	}

	if(selRef == chanel_PID){ //���� ��� ������� ������� - ��� ���������
		sPnt->FailAinFix = 0;
		clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_LP);
		clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_RP);
		setBitMicro(ramPnt, progPnt->Pnt_CH_PID);
	}
	else if(selRef == chanel_RP){ //���� ��� ������� ������� - ���
		sPnt->FailAinFix = 0;
		clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_LP);
		setBitMicro(ramPnt, progPnt->Pnt_CH_REF_RP);
		clrBitMicro(ramPnt, progPnt->Pnt_CH_PID);
	}
	else{		//Pnt_CH_REF_LP �����, ��� ��������� ������� - ��� ��������� �����, ��� ���������� �����, ��� ������� �����
		setBitMicro(ramPnt, progPnt->Pnt_CH_REF_LP);
		clrBitMicro(ramPnt, progPnt->Pnt_CH_REF_RP);
		clrBitMicro(ramPnt, progPnt->Pnt_CH_PID);
		if(testBitMicro(ramPnt, progPnt->Pnt_FailAinEn)) //���� ��� �������
		{
            if(load_s16(ramPnt, progPnt->Pnt_EnFailAin) && /*���� �������� ������� �� ������������� ������� � ������� - ���� �� ���������� ������*/
               ((selRef == chanel_A1) || (selRef == chanel_A2) || (selRef == chanel_A3) || (selRef == chanel_A4) || (selRef == chanel_A5)))
            {
                if(testBitMicro(ramPnt, progPnt->Pnt_Ain[selRef - chanel_A1].Fail)) //���������� ���� �� ������ ����������� �����, ���������� � �������� �������
                {
                    if(sPnt->FailAinFix == 0){ //���� ������ ���������� ������� ���
                        if(load_s16(ramPnt, progPnt->Pnt_Ain[selRef - chanel_A1].CodeFail) == 2) //���� ��� ������ - ���������� �������� �������� ����������� �������
                        {
                        	sPnt->Out = load_s32(ramPnt, progPnt->Pnt_FixFreqMax); //������ ����������� ���������� ������������� �������
                        }
                        else{
                        	sPnt->Out = load_s32(ramPnt, progPnt->Pnt_FixFreqMin); //����� ��� ������ - ��������� ������ �������� ����������� ������� ���� �����������
                        }
                        sPnt->FailAinFix = 1; //���������� ��, ��� �������� ������������� �������, ����� ������� ��� ���� ��� ��� ����������� ������
                    }
                }
            }
		}
		else{ //����� ��� ��������, ��������� �� � ������
			sPnt->FailAinFix = 0; //����� �������
			sPnt->Out = 0;        //����� �������������� �������
		}
	}

	if(sPnt->FailAinFix){
		Out = sPnt->Out;
	}

	save_s32(ramPnt, progPnt->Pnt_Out, Out); //������ � �������������� ������� �������

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSEL_REF_1[38]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  SelRef1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  SelRef2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  UstSel
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  DinSel_1_2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Local_0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Local_1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Asu
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Din
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  Ain1
		S32_PIN_TYPE | INPUT_PIN_MODE,     //10  Ain2
		S32_PIN_TYPE | INPUT_PIN_MODE,     //11  Ain3
		S32_PIN_TYPE | INPUT_PIN_MODE,     //12  Ain4
		S32_PIN_TYPE | INPUT_PIN_MODE,     //13  Ain5
		S32_PIN_TYPE | INPUT_PIN_MODE,     //14  Kalendar
		S32_PIN_TYPE | INPUT_PIN_MODE,     //15  Cascade
		S32_PIN_TYPE | INPUT_PIN_MODE,     //16  UpDown
		S32_PIN_TYPE | INPUT_PIN_MODE,     //17  FireFixed
		S32_PIN_TYPE | INPUT_PIN_MODE,     //18  In13
		S32_PIN_TYPE | INPUT_PIN_MODE,     //19  In14
		S32_PIN_TYPE | INPUT_PIN_MODE,     //20  In15
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //21  FailAin1
		S16_PIN_TYPE | INPUT_PIN_MODE,     //22  FailCode1
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //23  FailAin2
		S16_PIN_TYPE | INPUT_PIN_MODE,     //24  FailCode2
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //25  FailAin3
		S16_PIN_TYPE | INPUT_PIN_MODE,     //26  FailCode3
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //27  FailAin4
		S16_PIN_TYPE | INPUT_PIN_MODE,     //28  FailCode4
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //29  FailAin5
		S16_PIN_TYPE | INPUT_PIN_MODE,     //30  FailCode1
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //31  FailAinEn
		S32_PIN_TYPE | INPUT_PIN_MODE,     //32  FixFreqMin
		S32_PIN_TYPE | INPUT_PIN_MODE,     //33  FixFreqMax
		S16_PIN_TYPE | INPUT_PIN_MODE,     //34  EnFailAin
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //35  CH_PID
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //36  CH_REF_RP
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //37  CH_REF_LP
		S32_PIN_TYPE | OUTPUT_PIN_MODE     //38  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SEL_REF", "DA", TblFuncSEL_REF_1, FuncSEL_REF_1, sizeof(TblFuncSEL_REF_1), sizeof(FuncSEL_REFState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSEL_REF_1,
#endif
//********************************************************
