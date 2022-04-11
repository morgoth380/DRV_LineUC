#ifdef    GET_FUNC_CODE

//#define UScanBegin   _IQ(0.04) /* ���������� ����� ��� ������� ���������� �� ������ �� �������� ������ ���������� */

typedef enum{
    InitPUState,
    waiteRunPUState,
    UpVoltBeginPUState,
    searchPUState,
//	searchPUStateRev,
	trimFPUState,
    UpVoltEndPUState,
    RunPUState,

    abracadabra = 1000
}PUStat_type;

typedef struct{
    PUStat_type     State;
	u16				RevSearch;
    _iq             IactSave;
    _iq             FoutPU;
    _iq             dFscan;
    _iq             Uscan;
    _iq             dUupVolt;
    _iq             dUsaturation;
    _iq				refAct;
	u16				timerOff;
}PickUpState_type;


typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_FStartScan;                   //1  FStartScan
    u16  Pnt_TSsaturation;                 //2  TSsaturation
    u16  Pnt_TempScanF;                    //3  TempScanF
    u16  Pnt_Ihh_reactiv;                  //4  Ihh_reativ
    u16  Pnt_TempRiseU;                    //5  TempRiseU
    u16  Pnt_Uscan;                        //6  Uscan
    u16  Pnt_UminScan;                     //7  UminScan
    u16  Pnt_Start;                        //8  Start
    u16  Pnt_KpAct;                        //9  KpAct
    u16  Pnt_KiAct;                        //10  KiAct
    u16  Pnt_KpAct4;                       //11  KpAct4
    u16  Pnt_KiAct4;                       //12  KiAct4
    u16  Pnt_KpRe;                         //13  KpRe
    u16  Pnt_KiRe;                         //14  KiRe
    u16  Pnt_ScanDownUp;                   //15  ScanDownUp
    u16  Pnt_En;                           //16  En
    u16  Pnt_Fpusk;                        //17  Fpusk
    u16  Pnt_StatePU;                      //18  StatePU
    u16  Pnt_HoldGT;                       //19  HoldGT
    u16  Pnt_Fout;                         //20  Fout
    u16  Pnt_Uout;                         //21  Uout
    u16  Pnt_End;
}FuncPICKUP_V2_type;
//********************************************************
u16 *FuncPICKUP_V2_1(FuncPICKUP_V2_type *progPnt, u32 ramPnt)
{
    PickUpState_type                *sPnt;
    u16                             run, reversSearchEn;
    s16                             sign, Enable;
	_iq								Uscan, FoutPU, errAct, errRe, refRe, KiAct, KpAct, KpRe, KiRe, KpAct4, KiAct4, UscanRef,/* minIact,*/ UScanBegin, FScanBegin;
	COMMONPARAMC28                  *c28Pnt;

    sPnt = (PickUpState_type *)(ramPnt + progPnt->Pnt_State);
    c28Pnt = &GlobalC28;
	refRe	= _IQ(-0.15);  // �������������� ��� ��������� ���� ���������

	refRe = -load_s32(ramPnt, progPnt->Pnt_Ihh_reactiv); //������. ���������� ��� ��������� ���� (��������� ��������)

	KpAct = _IQmpy(load_s32(ramPnt, progPnt->Pnt_KpAct), _IQ(0.1)); //���������������� ����������� ���-����������
	KiAct = _IQmpy(load_s32(ramPnt, progPnt->Pnt_KiAct), _IQ(0.1)); //������������ ����������� ���-����������

    KpRe = load_s32(ramPnt, progPnt->Pnt_KpRe); //���������������� ����������� ���-���������� ����������� ����
	KiRe = load_s32(ramPnt, progPnt->Pnt_KiRe); //������������ ����������� ���-���������� ����������� ����

	KpAct4 = load_s32(ramPnt, progPnt->Pnt_KpAct4);
	KiAct4 = load_s32(ramPnt, progPnt->Pnt_KiAct4);

	UScanBegin = load_s32(ramPnt, progPnt->Pnt_UminScan);   //����������� ���������� ������ ���������� �������
    FScanBegin = load_s32(ramPnt, progPnt->Pnt_FStartScan); //��������� ������� ������ (��������� � ������������ ������� - �������� ��)

    Enable = load_s16(ramPnt, progPnt->Pnt_En);             //������ ���������� ������ ��������

    if(flagIsFirstCall){
        sPnt->State = InitPUState; //��� �������� �������������� ��� ��������� �������
        //GlobalM3.GT_Work = 0;
    }

    if(Enable == 0) sPnt->State = InitPUState; //�� ������, ���� ����� �������� ��� �������, � ����� ��������

    UscanRef = load_s32(ramPnt, progPnt->Pnt_Uscan);  //���������� ������ ���������� �������
    run = testBitMicro(ramPnt, progPnt->Pnt_Start);   //����/���� (������ ����������� PWM)

    switch(sPnt->State){
        case InitPUState:                   // ������������������� �������� �������� (����� ������� ������ ��� ����� ����������/��������� ������� �������� �������� �����)
	        clrBitMicro(ramPnt, progPnt->Pnt_HoldGT);	        //GlobalM3.GT_Work = 0;//!!!!!!!!!!!!!
			if(run == 0){ //
                if(Enable) sPnt->State = waiteRunPUState; //������� ��������� ��� ���������� ������� ���� ��� ������� ����� ��������
				save_s32(ramPnt, progPnt->Pnt_Uout, _IQ(1.0)); //������ ������� �������� ���������� (����� ��������� ������ �������� ����� ������ �� �����)
			}
        break;
        case waiteRunPUState: //����� ��������� ������ �������� ������� �������� �����
            sPnt->IactSave = 0;                   //������ ����������?
            sPnt->FoutPU = FScanBegin;            //��������� ������� ������ (� ��� ����� �������� ������������ ����� ������ ������� ����)
            sPnt->dFscan = (sPnt->FoutPU * INT_TIME_CALL_MICRO_mS) / (load_s16(ramPnt, progPnt->Pnt_TempScanF) * 100);   //����������� ���������� ������� (�� ����� ������������ ������� ��������� �� ��������� �� ����)
			sPnt->RevSearch = 0; // ����� � ������� ������� �������
			sPnt->refAct = _IQ(0);
			sPnt->Uscan         = UScanBegin; //��������� �������� ���������� ������������
            sPnt->dUupVolt      = ((_IQ(1) - UscanRef) * INT_TIME_CALL_MICRO_mS) / (load_s16(ramPnt, progPnt->Pnt_TempRiseU) * 10); //���������� ������. ����. ��� ������. ������. c ������. ������. �� �������� ����. ���. U/f ����� ���������� ������� �������� ������
            sPnt->dUsaturation  = (UscanRef * INT_TIME_CALL_MICRO_mS) / (load_s16(ramPnt, progPnt->Pnt_TSsaturation) * 10);         //
            clrBitMicro(ramPnt, progPnt->Pnt_HoldGT);          //GlobalM3.GT_Work = 0;//!!!!!!!!!!!!!!!!!!
            if(run){  //���� ������ ����
                save_s32(ramPnt, progPnt->Pnt_Uout, sPnt->Uscan);  //������ ����������� ���������� ������
                save_s32(ramPnt, progPnt->Pnt_Fout, sPnt->FoutPU); //������ ��������� ������� ������������ (������������ - �������� ��)
    		    setBitMicro(ramPnt, progPnt->Pnt_HoldGT);             	//GlobalM3.GT_Work = 1;//!!!!!!!!!!!!!!!!
                sPnt->State = UpVoltBeginPUState; //��� ��������� ������� ����� �������� ���������� ������
            }
        break;
		case UpVoltBeginPUState:            // ��������� ���������� ������
            if(run == 0){  //���� ������ ����
                sPnt->State = waiteRunPUState; //������� � ��������� ��������� � �����
                break;
            }

			Uscan = sPnt->Uscan + sPnt->dUsaturation; //��������� ����������

            if(Uscan >= UscanRef){ //���� ����� ��� ��������� ���������� ������
                sPnt->Uscan = UscanRef; //�� ������������ ��� ��������� ���������� ���������
				sPnt->State = searchPUState; //����� ��������, ��� ��������� ������� ������ ������ �������
            }
			else{
				sPnt->Uscan = Uscan;
			}

            save_s32(ramPnt, progPnt->Pnt_Uout, sPnt->Uscan); //������ ����� �������� ����������
        break;
        case searchPUState:								// ����� ������� �������� ������ ���������
            if(run == 0){         //���� ������ ����
                sPnt->State = waiteRunPUState;
                break;
            }

			errRe = -(refRe - c28Pnt->Ireact); //��������� ������� ����� ��������� ��������� ����������� ���� ��������� � ����������� ��������� ���������� ����
			Uscan = sPnt->Uscan + _IQmpy(errRe, KiRe);
            if(Uscan > _IQ(1.0f)) Uscan = _IQ(1.0f);
			sPnt->Uscan = Uscan;
			Uscan += _IQmpy(errRe, KpRe);
			if(Uscan > _IQ(1.0f)) Uscan = _IQ(1.0f);
			save_s32(ramPnt, progPnt->Pnt_Uout, Uscan);

			if(sPnt->Uscan > _IQ(1.0f)) sPnt->Uscan = _IQ(1.0f);
	//		errAct			= -ctom.measData.ids;
	//		sPnt->FoutPU   += _IQmpy( errAct, mtoc.debug.KiAct4 );
	//		FoutPU			=  sPnt->FoutPU + _IQmpy(errAct, mtoc.debug.KpAct4);

            sPnt->FoutPU -= sPnt->dFscan; //��������� ������� ������������

            if(abs(sPnt->FoutPU) > FScanBegin){
            	if(sPnt->FoutPU > 0) sPnt->FoutPU = FScanBegin;
            	else                 sPnt->FoutPU = -FScanBegin;
            }

			if(abs(sPnt->FoutPU) <= abs(load_s32(ramPnt, progPnt->Pnt_Fpusk))){          // �� ����� �������� ���������
				reversSearchEn = load_s16(ramPnt, progPnt->Pnt_ScanDownUp);
				if((reversSearchEn != 0) && (sPnt->RevSearch == 0)){
					sPnt->RevSearch = 1;							// ����� � �������� ��������
					sPnt->dFscan	= -sPnt->dFscan;
					sPnt->FoutPU	= -FScanBegin;//load_s32(ramPnt, progPnt->Pnt_FStartScan);
					sPnt->Uscan		= UScanBegin;
					sPnt->State		= UpVoltBeginPUState;

					save_s32(ramPnt, progPnt->Pnt_Uout, sPnt->Uscan);
				}
				else{
					clrBitMicro(ramPnt, progPnt->Pnt_HoldGT);
					//GlobalM3.GT_Work = 0;//!!!!!!!!!!!!!
					save_s32(ramPnt, progPnt->Pnt_Uout, _IQ(1));
					sPnt->State = RunPUState;   // WARNING ���� �� ������� ��� ������ � ���� ������
				}
            }
			//minIact = _IQmpy(_IQ(0.01), GlobalM3.Isn);
            //sign = ( c28Pnt->Iact < (-minIact)) ? -1 : 1;
            sign = (c28Pnt->Iact < 0) ? -1 : 1;
			if(sign < 0){                                         // ������� �� ���� ��������� ���� � ��������� ������� � ��� ��� ������� �������� �������
                sPnt->State = trimFPUState;
            }

            save_s32(ramPnt, progPnt->Pnt_Fout, sPnt->FoutPU);
            break;

		case trimFPUState:					                                // �������� ������� ��� �� ���� �� ������������� �������
            if(run == 0){
                sPnt->State = waiteRunPUState;
                break;
            }

            sign = (c28Pnt->Iact < 0) ? -1 : 1;
            if(sign > 0){                                                   // ������� �� ���� ��������� ���� � ��������� ������� � ��� ��� ������� �������� �������
				sPnt->State = UpVoltEndPUState;
				break;
            }
			else{
				if(sPnt->FoutPU >= 0){
					errAct = (0 - c28Pnt->Iact);
				}
				else{
					errAct = -(0 - c28Pnt->Iact);
				}
				sPnt->FoutPU += _IQmpy(errAct, KiAct4);
				FoutPU = sPnt->FoutPU + _IQmpy(errAct, KpAct4);
			}
//          sPnt->FoutPU += sPnt->dFscan;
            if(abs(FoutPU) > FScanBegin){
            	if(FoutPU > 0) FoutPU = FScanBegin;
            	else           FoutPU = -FScanBegin;
            }

            if(abs(sPnt->FoutPU) > FScanBegin){
            	if(sPnt->FoutPU > 0) sPnt->FoutPU = FScanBegin;
            	else sPnt->FoutPU = -FScanBegin;
            }

			save_s32(ramPnt, progPnt->Pnt_Fout, FoutPU);//sPnt->FoutPU
            break;

        case UpVoltEndPUState:              // ������� ������� ��������� ���������� �� ������ �� �� ������������
            if(run == 0){
                sPnt->State = waiteRunPUState;
                break;
            }
            else{
                Uscan = sPnt->Uscan + sPnt->dUupVolt;
                if(Uscan >= _IQ(1.0)){
                    Uscan = _IQ(1.0);                     //GlobalM3.GT_Work = 0;//!!!!!!!!!!!!!!
                    clrBitMicro(ramPnt, progPnt->Pnt_HoldGT);
                    sPnt->State = RunPUState;
                }
				// ��� ������� ���������� ������������ ������� ����� �������,
				// ����� �������� ��� ��������� � ����

//				if(abs(ctom.measData.iqs) > _IQ(0.25) ){
//					refAct += _IQmpy(load_s32(ramPnt, progPnt->Pnt_diffUd), _IQ(0.05));
//				}

//Ihh = _IQ(0.3);
//				if( ctom.measData.iqs < Ihh  ){
//					refAct += _IQ(0.01);
//				}

				if(sPnt->FoutPU >= 0){
					errAct = (sPnt->refAct - c28Pnt->Iact);
				}
				else{
					errAct = -(sPnt->refAct - c28Pnt->Iact);
				}

				sPnt->FoutPU += _IQmpy(errAct, KiAct);
				FoutPU = sPnt->FoutPU + _IQmpy(errAct, KpAct);

				/*
				if( ctom.measData.ids < 0 ){
					sPnt->FoutPU += sPnt->dFscan;
				}
				else{
					sPnt->FoutPU -= sPnt->dFscan;
				}
				*/
	            if(abs(sPnt->FoutPU) > FScanBegin){
	            	if(sPnt->FoutPU > 0) sPnt->FoutPU = FScanBegin;
	            	else sPnt->FoutPU = -FScanBegin;
	            }

	            if(abs(FoutPU) > FScanBegin){
	            	if(FoutPU > 0) FoutPU = FScanBegin;
	            	else           FoutPU = -FScanBegin;
	            }

				save_s32(ramPnt, progPnt->Pnt_Fout, FoutPU);
				sPnt->Uscan = Uscan;
                save_s32(ramPnt, progPnt->Pnt_Uout, Uscan);
            }
        break;
        case RunPUState:
            if(run == 0) sPnt->State = waiteRunPUState;
        break;
        default:
            sPnt->State = InitPUState;
        break;
    }

    //mtoc.debug.Uscan		= sPnt->Uscan;
    //mtoc.debug.Fscan		= sPnt->FoutPU;
    //mtoc.debug.statePICAP	= sPnt->State;
    save_s16(ramPnt, progPnt->Pnt_StatePU, sPnt->State);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPICKUP_V2_1[21]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  FStartScan
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  TSsaturation
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TempScanF
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Ihh_reactiv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  TempRiseU
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Uscan
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  UminScan
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Start
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  KpAct
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  KiAct
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  KpAct4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  KiAct4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  KpRe
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  KiRe
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  ScanDownUp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  En
		S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Fpusk
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  StatePU
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //19  HoldGT
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //20  Fout
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //21  Uout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PICKUP_V2", "DA", TblFuncPICKUP_V2_1, FuncPICKUP_V2_1, sizeof(TblFuncPICKUP_V2_1), sizeof(PickUpState_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPICKUP_V2_1,
#endif
//********************************************************
