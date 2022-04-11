#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_AutoMode;                     //1  AutoMode
    u16  Pnt_WorkFlagNot;                  //2  WorkFlagNot
    u16  Pnt_FailTekDrvNot;                //3  FailTekDrvNot
    u16  Pnt_EnControlDin;                 //4  EnControlDin
    u16  Pnt_Prior;                        //5  Prior
    u16  Pnt_Prior_revers;                 //6  Prior_revers
    u16  Pnt_StandLift_Speed;              //7  StandLift_Speed
    u16  Pnt_Sel_Pusk1;                    //8  Sel_Pusk1
    u16  Pnt_Sel_Pusk2;                    //9  Sel_Pusk2
    u16  Pnt_Sel_PuskEn1;                  //10  Sel_PuskEn1
    u16  Pnt_Sel_PuskEn2;                  //11  Sel_PuskEn2
    u16  Pnt_Sel_Stop1;                    //12  Sel_Stop1
    u16  Pnt_Sel_Stop2;                    //13  Sel_Stop2
    u16  Pnt_Sel_StopVibeg;                //14  Sel_StopVibeg
    u16  Pnt_Sel_Forward;                  //15  Sel_Forward
    u16  Pnt_Sel_Bacward;                  //16  Sel_Bacward
    u16  Pnt_Sel_V1;                       //17  Sel_V1
    u16  Pnt_Sel_V2;                       //18  Sel_V2
    u16  Pnt_Sel_V3;                       //19  Sel_V3
    u16  Pnt_Sel_V4;                       //20  Sel_V4
    u16  Pnt_Sel_Bit0;                     //21  Sel_Bit0
    u16  Pnt_Sel_Bit1;                     //22  Sel_Bit1
    u16  Pnt_Sel_Bit2;                     //23  Sel_Bit2
    u16  Pnt_Sel_Bit3;                     //24  Sel_Bit3
    u16  Pnt_Sel_Up;                       //25  Sel_Up
    u16  Pnt_Sel_Down;                     //26  Sel_Down
    u16  Pnt_Sel_Control;                  //27  Sel_Control
    u16  Pnt_Sel_Ref;                      //28  Sel_Ref
    u16  Pnt_Sel_FastTorm;                 //29  Sel_FastTorm
    u16  Pnt_Sel_Evak;                     //30  Sel_Evak
    u16  Pnt_Sel_ContOs;                   //31  Sel_ContOs
    u16  Pnt_Sel_InspectEn;                //32  Sel_InspectEn
    u16  Pnt_Sel_TorqSpeed;                //33  Sel_TorqSpeed
    u16  Pnt_Sel_UF;                       //34  Sel_UF
    u16  Pnt_Sel_DryEn;                    //35  Sel_DryEn
    u16  Pnt_Sel_Water;                    //36  Sel_Water
    u16  Pnt_Sel_Clean;                    //37  Sel_Clean
    u16  Pnt_Sel_ResWorkCnt;               //38  Sel_ResWorkCnt
    u16  Pnt_Sel_Relay1;                   //39  Sel_Relay1
    u16  Pnt_Sel_Relay2;                   //40  Sel_Relay2
    u16  Pnt_Sel_Relay3;                   //41  Sel_Relay3
    u16  Pnt_Sel_Relay4;                   //42  Sel_Relay4
    u16  Pnt_Sel_Relay5;                   //43  Sel_Relay5
    u16  Pnt_Sel_Relay6;                   //44  Sel_Relay6
    u16  Pnt_Sel_Relay7;                   //45  Sel_Relay7
    u16  Pnt_Sel_Relay8;                   //46  Sel_Relay8
    u16  Pnt_Sel_Motor1;                   //47  Sel_Motor1
    u16  Pnt_Sel_Motor2;                   //48  Sel_Motor2
    u16  Pnt_Sel_Motor3;                   //49  Sel_Motor3
    u16  Pnt_Sel_Motor4;                   //50  Sel_Motor4
    u16  Pnt_Input_1;                      //51  Input_1
    u16  Pnt_Input_2;                      //52  Input_2
    u16  Pnt_AutoModeBit;                  //53  AutoModeBit
    u16  Pnt_DF_Pusk;                      //54  DF_Pusk
    u16  Pnt_DF_Stop;                      //55  DF_Stop
    u16  Pnt_DF_StopVibeg;                 //56  DF_StopVibeg
    u16  Pnt_DF_Revers;                    //57  DF_Revers
    u16  Pnt_DF_NumSel;                    //58  DF_NumSel
    u16  Pnt_DF_Fup;                       //59  DF_Fup
    u16  Pnt_DF_Fdown;                     //60  DF_Fdown
    u16  Pnt_DF_SelControl;                //61  DF_SelControl
    u16  Pnt_DF_SelChanRef;                //62  DF_SelChanRef
    u16  Pnt_DF_FastTorm;                  //63  DF_FastTorm
    u16  Pnt_DF_Evak;                      //64  DF_Evak
    u16  Pnt_DF_ContOs;                    //65  DF_ContOs
    u16  Pnt_DF_InspectEn;                 //66  DF_InspectEn
    u16  Pnt_DF_TorqSpeed;                 //67  DF_TorqSpeed
    u16  Pnt_DF_SelUF;                     //68  DF_SelUF
    u16  Pnt_PuskDryPulse;                 //69  PuskDryPulse
    u16  Pnt_DF_NoWater;                   //70  DF_NoWater
    u16  Pnt_DF_StartClean;                //71  DF_StartClean
    u16  Pnt_DF_ResCntWork;                //72  DF_ResCntWork
    u16  Pnt_DOut_K1;                      //73  DOut_K1
    u16  Pnt_DOut_K2;                      //74  DOut_K2
    u16  Pnt_DOut_K3;                      //75  DOut_K3
    u16  Pnt_DOut_K4;                      //76  DOut_K4
    u16  Pnt_DOut_K5;                      //77  DOut_K5
    u16  Pnt_DOut_K8;                      //78  DOut_K8
    u16  Pnt_DOut_K7;                      //79  DOut_K7
    u16  Pnt_DOut_K6;                      //80  DOut_K6
    u16  Pnt_MotEn1;                       //81  MotEn1
    u16  Pnt_MotEn2;                       //82  MotEn2
    u16  Pnt_MotEn3;                       //83  MotEn3
    u16  Pnt_MotEn4;                       //84  MotEn4
    u16  Pnt_End;
}FuncDISKR_FUNC_type;
#define		FDF	        (*(FuncDISKR_FUNC_type*)0)
//********************************************************
typedef struct{
		union{
			struct{
				u16		ePuskPuls : 1;
				u16		PuskPuls  : 1;
				u16		AutoPuls  : 1;
				u16		PuskDryPulse: 1;
			}fl;
			u16		allFl;
		}pulsVal;
	u16			delayPuskShift;
	
	
}StateFuncDISKR_FUNC;
//********************************************************
typedef union{
	u64		v1;
	u32		v2[2];
	u16		v4[4];
}u64u32u16_type;
//********************************************************
const u16 TblPriorSpeed[4][16]={
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		/*��� ����������*/
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		/*�� �����������*/
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		/*�� ��������	*/
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}		/*  */
};

/*������ ������� �����{
f1	f0  u1  u0  N
0	0	0	0	0 -  
0	0	0	1	1 - 1 
0	0	1	0	2 - 1 
0	0	1	1	3 - 1 
0	1	0	0	4 -  
0	1	0	1	5 -  
0	1	1	0	6 - 1 
0	1	1	1	7 -  	
1	0	0	0	8 - 
1	0	0	1	9 - 1
1	0	1	0	10- 
1	0	1	1	11- 
1	1	0	0	12- 
1	1	0	1	13- 
1	1	1	0	14- 
1	1	1	1	15- 
}*/
//********************************************************
/*void defDiskrFunc(u64 *maskPnt, u16 Sel, u16 DiskrFunc)
{
	s16		num;
	num = load_s16(ramPnt, Sel)&0x3F;
	num = ((*maskPnt)>>num)&1;
	defBitMicro(ramPnt, DiskrFunc, num);
}*/
//********************************************************
u16 *FuncDISKR_FUNC_1(FuncDISKR_FUNC_type *progPnt, u32 ramPnt)
{
    s16						autoMode, PuskDisable;
    s16						u1, u2, e1, e2, f1, f2;
    s16						forward, revers, pusk;
    u16						*selPnt, *outPnt;
    u64u32u16_type			Input;
    StateFuncDISKR_FUNC		*sPnt;
    
    autoMode = load_s16(ramPnt, progPnt->Pnt_AutoMode);
    defBitMicro(ramPnt, progPnt->Pnt_AutoModeBit, autoMode);
	
	Input.v2[0] = load_s32(ramPnt, progPnt->Pnt_Input_1);
	Input.v2[1] = load_s32(ramPnt, progPnt->Pnt_Input_2);

	if ( testBitMicro(ramPnt, progPnt->Pnt_EnControlDin) == 0 ){
		clrBitMicro(ramPnt, progPnt->Pnt_DF_Pusk);
		clrBitMicro(ramPnt, progPnt->Pnt_DF_Stop);
		clrBitMicro(ramPnt, progPnt->Pnt_DF_StopVibeg);
		sPnt->pulsVal.fl.allFl = 0;
	}
	else{	//������������ ����� � ����� ���� ��������� ���������� �� ���������� ������
//-- ������ ������� �� ����
		u1 = load_s16(ramPnt, Pnt_Sel_PuskEn1)&0x3F;
		u2 = load_s16(ramPnt, Pnt_Sel_PuskEn2)&0x3F;
		f1 = (Input.v1>>u1)&1;
		f2 = (Input.v1>>u2)&1;
		
		if((u0&&(!f0))||(u1&&(!f1)))	PuskDisable = 1;				//������ ������� �� ����. ���� ������� ��������, �� ��������, �� ������ �� ����
		else							PuskDisable = 0;

//-- ������	����
		e1 = load_s16(ramPnt, progPnt->Pnt_Sel_Stop1)&0x3F;
		e2 = load_s16(ramPnt, progPnt->Pnt_Sel_Stop2)&0x3F;
		f1 = (Input.v1>>e1)&1;
		f2 = (Input.v1>>e2)&1;
		
		u1 = (!sPnt->pulsVal.fl.ePuskPuls && PuskDisable);				//����������� �����. �� ������ �� ���� ������� �� ���� ��������� �������.
		sPnt->pulsVal.fl.ePuskPuls = u1;								//��������� �������� �������� �����
		
		f1 = f1 || f2 || u1;											//riseDisablePusk || ���� 1 || ���� 2
		defBitMicro(ramPnt, progPnt->Pnt_DF_Stop, f1);

//-- ������ ����
		e1 = load_s16(ramPnt, progPnt->Pnt_Sel_Pusk1)&0x3F;
		e2 = load_s16(ramPnt, progPnt->Pnt_Sel_Pusk2)&0x3F;
		f1 = (Input.v1>>e1)&1;
		f2 = (Input.v1>>e2)&1;
		pusk = (f1||f2)&& (!PuskDisable);								//Pusk1 || Pusk2 || (!PuskDisable)
		
		u1 = (!sPnt->pulsVal.fl.PuskPuls && pusk);						//����������� ����� �����
		sPnt->pulsVal.fl.PuskPuls = pusk;								//��������� �������� �������� �����
		
		u2 = testBitMicro(ramPnt, progPnt->Pnt_FailTekDrvNot);
		
		f1 = u1 && u2 && (!autoMode);									//ManualPusk = RisePusk && FailTekDrvNot && (!autoMode)
		f2 = pusk && u2 && autoMode;									//AutoPusk = pusk && FailTekDrvNot && autoMode
		
		u1 = (!sPnt->pulsVal.fl.AutoPuls && f2);						//����������� ����� AutoPusk
		sPnt->pulsVal.fl.AutoPuls = f2;									//��������� �������� �������� �����
		
		u1 = u1 || f1;													//RiseAutoPusk || ManualPusk
		//delayPuskShift			// �������� ������� ����
		defBitMicro(ramPnt, progPnt->Pnt_DF_Pusk, u1);
//-- ������ �������� �������
		e1 = load_s16(ramPnt, progPnt->Pnt_Sel_StopVibeg)&0x3F;
		e1 = (Input.v1>>e1)&1;
		defBitMicro(ramPnt, progPnt->Pnt_DF_StopVibeg, e1);
	}

//-- ������ ���������� ���������
	e1 = load_s16(ramPnt, progPnt->Pnt_StandLift_Speed);		//0-����������	1-�������������	2-��������
	if(e1 == 2){			//2-��������
		e1 = load_s16(ramPnt, progPnt->Pnt_Sel_Bit0)&0x3F;
		e2 = load_s16(ramPnt, progPnt->Pnt_Sel_Bit1)&0x3F;
		f1 = load_s16(ramPnt, progPnt->Pnt_Sel_Bit2)&0x3F;
		f2 = load_s16(ramPnt, progPnt->Pnt_Sel_Bit3)&0x3F;
		
		e1 = (Input.v1>>e1)&1;
		e2 = (Input.v1>>e2)&1;
		f1 = (Input.v1>>f1)&1;
		f2 = (Input.v1>>f2)&1;
		
		f1 = (e1<<0)|(e2<<1)|(f1<<2)|(f2<<3);
	}
	else if(e1 == 0){		//0-����������
		e1 = load_s16(ramPnt, progPnt->Pnt_Sel_Bit0)&0x3F;
		e2 = load_s16(ramPnt, progPnt->Pnt_Sel_Bit1)&0x3F;
		f1 = load_s16(ramPnt, progPnt->Pnt_Sel_Bit2)&0x3F;
		
		e1 = (Input.v1>>e1)&1;
		e2 = (Input.v1>>e2)&1;
		f1 = (Input.v1>>f1)&1;
		
		f1 = ((e1<<0)|(e2<<1)|(f1<<2))+16;						//������� ���� 16 ��������� ��� ���������, ����� 8 ��������
	}
	else if(e1 == 1){		//1-�������������
		e1 = load_s16(ramPnt, progPnt->Pnt_Sel_V1)&0x3F;		//�������� �����������  
		e2 = load_s16(ramPnt, progPnt->Pnt_Sel_V2)&0x3F;		//����������� ��������  
		f1 = load_s16(ramPnt, progPnt->Pnt_Sel_V3)&0x3F;    	//������������� ��������
		f2 = load_s16(ramPnt, progPnt->Pnt_Sel_V4)&0x3F;		//������������� ��������
		e1 = (Input.v1>>e1)&1;
		e2 = (Input.v1>>e2)&1;
		f1 = (Input.v1>>f1)&1;
		f2 = (Input.v1>>f2)&1;
		f1 = (e1<<0)|(e2<<1)|(f1<<2)|(f2<<3);
		e1 = load_s16(ramPnt, progPnt->Pnt_Prior)&0x3;
		f1 = TblPriorSpeed[e1][f1];								//����� ����������� ����������
	}	
	
	if(1){
		save_s16(ramPnt, progPnt->Pnt_DF_NumSel, f1);			//������ ������ ��������

		e1 = load_s16(ramPnt, progPnt->Pnt_Sel_Forward)&0x3F;
		e2 = load_s16(ramPnt, progPnt->Pnt_Sel_Bacward)&0x3F;
		e1 = (Input.v1>>e1)&1;
		e2 = (Input.v1>>e2)&1;
		if(e1 && e2){
			StopFixSpeed = 0;
		}
		else{				//�� ������ �����������
			StopFixSpeed = 1;
		}
		if(f1 == 0){
			PuskFixSpeed = 0;
		}
		else PuskFixSpeed = 0;
		
	}	
	save_s16(ramPnt, progPnt->Pnt_DF_Revers, f1);

//-- ������ ��������� �������
	selPnt = &progPnt->Pnt_Sel_Up;
	outPnt = &progPnt->Pnt_DF_Fup;
	for(u1=(u16)(FDF.Pnt_Sel_Motor4 - FDF.Pnt_Sel_Up); u1 > 0; u1--){
		e1 = load_s16(ramPnt, *selPnt++)&0x3F;
		e1 = (Input.v1>>e1)&1;
		defBitMicro(ramPnt, *outPnt++, e1);
	}
	
//-- ����������� ������� ������� ���� � DF_NoWater
	e1 = testBitMicro(ramPnt, progPnt->Pnt_DF_NoWater);
	defBitMicro(ramPnt, progPnt->Pnt_DF_NoWater, !e1);			//�����������
//-- ��������� ������� ����� �����
	e1 = testBitMicro(ramPnt, progPnt->Pnt_PuskDryPulse);		//��������� �������
	e1 = (!sPnt->pulsVal.fl.PuskDryPulse && e1);				//����������� ����� PuskDryPulse
	sPnt->pulsVal.fl.PuskDryPulse = e1;							//��������� �������� �������� �����
	defBitMicro(ramPnt, progPnt->Pnt_PuskDryPulse, e1);		
	
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
/*����� ����������� 1{
	�������������
	UPS- ����������
	�����		����������� �������� 
	����		����������� ��������
	bit 0		������� �������� �������� ��� 
	bit 1		������� �������� �������� ��� 
	bit 2		������� �������� �������� ��� 
	�������� �����������

����� ������� �������� 
Bit 0 (X2A.10)
Bit 1 (X2A.11)
Bit 2 (X2A.12)
			0 1 2		���� ������� ��������
0 			- - -
VR (LF.20)	1 - -		�������� �������
VL (LF.21)	- 1 -		�������� �����������
VN (LF.22)	1 1 -		����������� ��������
VI (LF.23)	- - 1		������������� ��������
V1 (LF.24)	1 - 1		������������� �������� 1
V2 (LF.25)	- 1 1		������������� �������� 2
V3 (LF.26)	1 1 1		������������� �������� 3

���� ������������ ��������� ���������� ���.
//--
������ ��������� ���������� ���
��������� ��������� ����������� ���������. ���� ������� ����� ���� ������� ����������.
���������� ��������� ���������
���������� ������. ��������

}
����� ����������� 2{
	�������������
	VR 			�������� �����������
	�����		����������� �������� 
	����		����������� ��������
	VL 			�������� �����������
	VN 			����������� ��������
	VI 			������������� ��������
	V1 			������������� ��������
}
����� ����������� 3{
	�������������
	���� ���������
	�����		����������� �������� 
	����		����������� ��������
	������� Bit0
	������� Bit1
	������� Bit2
	�������� �����������
}
*/
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDISKR_FUNC_1[84]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  AutoMode
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  WorkFlagNot
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  FailTekDrvNot
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  EnControlDin
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Prior
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Prior_revers
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  StandLift_Speed
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  Sel_Pusk1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Sel_Pusk2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  Sel_PuskEn1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  Sel_PuskEn2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Sel_Stop1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Sel_Stop2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Sel_StopVibeg
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Sel_Forward
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Sel_Bacward
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Sel_V1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Sel_V2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Sel_V3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Sel_V4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Sel_Bit0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Sel_Bit1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Sel_Bit2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  Sel_Bit3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Sel_Up
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  Sel_Down
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  Sel_Control
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Sel_Ref
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  Sel_FastTorm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  Sel_Evak
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  Sel_ContOs
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  Sel_InspectEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  Sel_TorqSpeed
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  Sel_UF
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  Sel_DryEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  Sel_Water
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  Sel_Clean
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38  Sel_ResWorkCnt
        S16_PIN_TYPE | INPUT_PIN_MODE,     //39  Sel_Relay1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //40  Sel_Relay2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41  Sel_Relay3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  Sel_Relay4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43  Sel_Relay5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //44  Sel_Relay6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //45  Sel_Relay7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //46  Sel_Relay8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //47  Sel_Motor1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //48  Sel_Motor2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //49  Sel_Motor3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //50  Sel_Motor4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //51  Input_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //52  Input_2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //53  AutoModeBit
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //54  DF_Pusk
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //55  DF_Stop
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //56  DF_StopVibeg
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //57  DF_Revers
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //58  DF_NumSel
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //59  DF_Fup
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //60  DF_Fdown
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //61  DF_SelControl
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //62  DF_SelChanRef
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //63  DF_FastTorm
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //64  DF_Evak
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //65  DF_ContOs
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //66  DF_InspectEn
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //67  DF_TorqSpeed
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //68  DF_SelUF
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //69  PuskDryPulse
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //70  DF_NoWater
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //71  DF_StartClean
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //72  DF_ResCntWork
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //73  DOut_K1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //74  DOut_K2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //75  DOut_K3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //76  DOut_K4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //77  DOut_K5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //78  DOut_K8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //79  DOut_K7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //80  DOut_K6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //81  MotEn1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //82  MotEn2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //83  MotEn3
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //84  MotEn4
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DISKR_FUNC", "DA", TblFuncDISKR_FUNC_1, FuncDISKR_FUNC_1, sizeof(TblFuncDISKR_FUNC_1), sizeof(StateFuncDISKR_FUNC), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDISKR_FUNC_1,
#endif
//********************************************************
