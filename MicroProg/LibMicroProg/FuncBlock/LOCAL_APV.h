
#ifdef    GET_FUNC_CODE
//====================================================================
#include    "apv_type.h"
#ifndef _PROJECT_FOR_CCS_
    #include "GUI.h"
#endif
//====================================================================
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Fail_IN;                      //01 Fail_IN                    //���� ������� ������
    u16  Pnt_Off_Block_Apv;                //02 Off/Block/Apv 0-1-2        //���� ������� ���������� ������������� ���
    u16  Pnt_DeleyControl;                 //03 DeleyControl               //�������� �� ����� �����
    u16  Pnt_DeleyOff;                     //04 DeleyOff                   //�������� ������������ ������
    u16  Pnt_NormalWait;                   //05 NormalWait                 //������� ������������
    u16  Pnt_ApvTime_sec;                  //06 ApvTime_sec                //������� ����� ��� ���.
    u16  Pnt_ApvTime_min;                  //07 ApvTime_min                //������� ����� ��� ���. ���� = ( ��� + ��� )
    u16  Pnt_KolvApv;                      //08 KolvApv                    //������� ���������� ���������� ���
    u16  Pnt_ResetApv;                     //09 ResetApv                   //���� ����� �������� ���
    u16  Pnt_On_Off;                       //10 On/Off                     //��������\��������� ���� ���
    u16  Pnt_RTC;                          //11 UnixTime                   //�������� ����� �����������
    u16  Pnt_Vybeg_Time;                   //12 VybegTime                  //�������� ����� ������!
    u16  Pnt_ApvCnt;                       //13 ApvCnt                     //����� �������� ���
    u16  Pnt_Warning;                      //14 Warning                    //����� �������������� � ������
    u16  Pnt_Fail;                         //15 Fail                       //����� ������
    u16  Pnt_End;
}FuncLOCAL_APV_type;
//********************************************************
//********************************************************
u16 *FuncLOCAL_APV_1(FuncLOCAL_APV_type *progPnt, u32 ramPnt)
{

    u16                             Off_Block_Apv, Warning, Fail, FailIn, NormalWait;
    u32                             TimeToAPV;
    LOCAL_APVState_type             *sPnt;
    LOCAL_APVnvState_type           *nvAdr;
    APVState_type                   *globsPnt;

    volatile static count = 0;
    ++count;

//====================================================================
#ifndef _PROJECT_FOR_CCS_   //������ �� �������� � Visual ��� �������
    if (StateTestMicroProg != 0) return &progPnt->Pnt_End;
#endif
//====================================================================
    sPnt        = (LOCAL_APVState_type *)(ramPnt + progPnt->Pnt_State); //sPnt - ��������� �� ����������� ������� ������
    nvAdr       = GetNvAdr( progPnt->Pnt_nvState, 0);                   //nvAdr - ��������� �� ����������������� ������� ������
    globsPnt    = &APVState;                                            //��������� �� ���������� ���������, ������������ �  ������ ��� apv1.h
//====================================================================


    if(flagIsFirstCall)
    {
        static u16      numBlock = 0;                             //������� ��������� ������ ��� (������������ ����������� ������� ������� ��������������� �����: ������� �������, ������� � ��������������� ����������)
        RdFramMicro( sPnt, nvAdr, sizeof(LOCAL_APVnvState_type)); //������������ ��������� (���������� �� FRAM � ����������� ������)
        APVpnt[numBlock] = sPnt->TimeToAPV;      // ��������� �������� �������  �� ���
        sPnt->numBlock = numBlock++;            // ������� �������
        sPnt->tmrDeleyControl  = 0;             // �������� ������ �������� �� ����������� ����� ������
        sPnt->tmrDeleyOff      = 0;             // �������� ������ ������������ ������� ������� ������
        sPnt->flag.ApvLatch    = 0;             // �������� ���� �������� �������������� ���������
        return &progPnt->Pnt_End;
    }

    Off_Block_Apv = load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv); //��������� ����� ���������� ��������� ������ ���
    FailIn = testBitMicro(ramPnt, progPnt->Pnt_Fail_IN);          //��������� ����� "������"

    if(Off_Block_Apv == 0)           //���� ��������� ���� ��� ��������..
        {
            sPnt->tmrDeleyControl   = 0; //�������� ������ �������� �� ����������� ����� ������
            sPnt->tmrDeleyOff       = 0; //�������� ������ ������������ ������� ������� ������
            sPnt->flag.all          = 0; //�������� ��� �����
            if(sPnt->TimeToAPV != 0)
            {
                WrFramMicro(&nvAdr->TimeToAPV, &sPnt->TimeToAPV, sizeof(nvAdr->TimeToAPV));
            }
            sPnt->TimeToAPV         = 0;
            Warning = 0;
            Fail = 0;
        }
//====================================================================
//              ����������� � �������� ��������
//====================================================================
    else if(testBitMicro(ramPnt, progPnt->Pnt_On_Off) == 0) // �������� �������� ����� - � ��������
    {  //�������� ��� - ��� � ������
        Warning = 0;                      //����� �����
        Fail    = 0;                      //����� �����
        sPnt->tmrDeleyControl       = 0;  //����� ��������
        sPnt->tmrDeleyOff           = 0;  //����� ��������
    }
    else if( sPnt->tmrDeleyControl < load_s16(ramPnt, progPnt->Pnt_DeleyControl)+1) // �������� ������� - ����������� ����� �������� ����� ��������� ����� ������
    {
        if(globsPnt->APVstatus.SecondFlag == 1)  //���� ������� ������..
            sPnt->tmrDeleyControl++;            //��������� �������� �������� �������� ����� ������
        Warning = 0;
        Fail    = 0;
    }
    else if(FailIn == 0) //����� �������� �� �������� ����� ������ ������, �� ��������� ���� ������. ���� �� ���..
    {
        Warning = 0; //����� �����
        Fail    = 0; //����� �����
        /*
        if( sPnt->tmrDeleyOff !=0 ) //��������� ���� �� �� ����� ������, �� ���� �������� �� ������� ������������ ������ �� ������ ��������� �� ���� ��������. ���� ����, ����������� �������
        {
            if(globsPnt->APVstatus.SecondFlag ==1)
                sPnt->tmrDeleyOff--;                            // ������ ������
        }
        */
        sPnt->tmrDeleyOff = 0;
    }
    else //����� �� ����� ���� ������: FailIn = 1
    {
        Warning = 1;                                        // �������������� �� �����
        Fail    = 0;
        if( sPnt->tmrDeleyOff < load_s16(ramPnt, progPnt->Pnt_DeleyOff )) //������ �������� ������ ��� ����������� ������
        {
            if(globsPnt->APVstatus.SecondFlag == 1) { sPnt->tmrDeleyOff++; }  // ������ ������
        }
        else
        {                                                           // ����� ����� ���� ��������� ���� !!
            Fail    = 1;                                            // ������������ ���� ������� ���������� ��������
        }
    }
    //����� ��������������� ���������� ����� ���� �������������� ������ �� ����� ��������������� �����, ���� ���������� ������������� ������ (������ ������ �� ����� ����� ��������������, �� ���� ������ ����� ���� ��� �� �����������)
//====================================================================
//       ����������� � ��������� �� ���
//====================================================================
//====================================================================
    if(globsPnt->APVstatus.APV == 0) //���� � ���������� ����� ��������� ������ �� ���, ������������ �� �������� ���������� �����..
    {
        sPnt->flag.ApvLatch = 0;     //..�� ����� ���������� ���� �������� �������������� ��������� (��������, ��� �������������� ��������� � ���������� ��������� ���������)
    }
//====================================================================
#ifdef _PROJECT_FOR_CCS_
#warning �������� ���� ���������� �������� ���������� ������� �� ���
#endif
    if(globsPnt->APVstatus.AutoMode != 0) //���� ��� ��������� ���������
    {
        if((Off_Block_Apv == 2) && (globsPnt->APVstatus.StopKeyState == 0)) //��������� ��������� �� ������ ���������� ����� ���
        {
            NormalWait = load_s16(ramPnt, progPnt->Pnt_NormalWait);      //��������� ��������, �������� ����� �� ����� ������������ ����� ������� ������� ������� �����������
            TimeToAPV = load_s16(ramPnt, progPnt->Pnt_ApvTime_min)*60 + load_s16(ramPnt, progPnt->Pnt_ApvTime_sec)+ load_s16(ramPnt, progPnt->Pnt_Vybeg_Time); //��������� �������� �������� �� ��� ��� �������������� ���������
            TimeToAPV = TimeToAPV + load_s32(ramPnt, progPnt->Pnt_RTC);   //��������� �������� �������� �� ��� ��� �������������� ���������
            sPnt->TimeToAPV         = TimeToAPV;                          //��������� ���������� ����� � ����������� �������
            if(Fail != 0) //���� ���� �������������� ������..
            {
            	APVpnt[sPnt->numBlock]  = TimeToAPV;                          //��������� ���������� ����� � �������� �������. ��� �������� ������� ������������ � ���������� ����� ���
            	if(!NormalWait && (load_s16(ramPnt, progPnt->Pnt_KolvApv) > sPnt->CntApv) && !(sPnt->flag.ApvLatch))//..�� �������� ����� �� ����� ������������ ���������. ���� ����� �� ����� � �� ��������� ���������� ������� ��� �� ����� ��������� ������ �� ���
            	{
            		sPnt->flag.ApvLatch = 1;                                      //���� �������� �������������� ���������
            		sPnt->CntApv++;                                               //����������� ���� ������� ���
            		globsPnt->APVstatus.APV = 1;                                  //��������� ������ �� ��� ��� �������� �������������� ���������. �� ����� ����� ���������� ���� ��� ������ ������ ������� �������� �� ���
            	}
            	else if((load_s16(ramPnt, progPnt->Pnt_KolvApv) > sPnt->CntApv) && !(sPnt->flag.ApvLatch))//�����, ���� ����� ����� �������������� ��������� � �� ��������� ���������� ���
            	{
            		sPnt->flag.ApvLatch = 1;                                      //��������� ���� �������� �������������� ���������
            		globsPnt->APVstatus.APV = 0;                                  //���� ������ �� ���������
            	}
            }
            else if ((sPnt->flag.ApvLatch) && !(globsPnt->APVstatus.APV))   //Fail = 0 - ������ �������, ��������� �������������� ���������, � ��������� ��� �� ���������� ���� �������� �������������� ��������� (��� ����� � ������ ����  NormalWait != 0)
            {
            	globsPnt->APVstatus.APV = 1; //� ��������� ���� ���������� ������ �� ���. ���� ��� ���� ����� �������� �� �������������� ��������� �� ������, �� �����. ���. ����� ������ �� ����� � ������ ���������
            	sPnt->CntApv++;              //����������� ���� ������������� ������� ���
            	APVpnt[sPnt->numBlock]  = TimeToAPV;
            }
        }
        else
        {
        	sPnt->flag.ApvLatch     = 0;
        	sPnt->TimeToAPV         = 0;
        	APVpnt[sPnt->numBlock]  = 0;
        }
    }
    else //���� ��� ��������� ���������..
    {
        if(sPnt->flag.ApvLatch != 0) //� ��� ���� ���� �������� �������������� ���������
        {
            sPnt->flag.ApvLatch     = 0; //�� ����� ����� �������� �������������� ���������
            sPnt->TimeToAPV         = 0; //����� ��������� �������� ����� ���
            APVpnt[sPnt->numBlock]  = 0; //����� ������ ����� ���������� ���
            WrFramMicro(nvAdr, sPnt, sizeof(LOCAL_APVnvState_type)); //���������� ������� ���������� ��� � FRAM
        }
    }
//====================================================================
//       ����������� � ��������� �� ����� �������� ���
//====================================================================
    if(testBitMicro(ramPnt, progPnt->Pnt_ResetApv) == 1)
    {
        if(sPnt->flag.ResetState == 0)
        {
            sPnt->flag.ResetState   = 1;
            sPnt->CntApv            = 0;
            WrFramMicro(&nvAdr->CntApv, &sPnt->CntApv, sizeof(sPnt->CntApv));
        }
    }
    else {sPnt->flag.ResetState   = 0;}

//====================================================================
//                    ��������� ���������
//====================================================================
    save_s16(ramPnt, progPnt->Pnt_ApvCnt, sPnt->CntApv );
    defBitMicro(ramPnt, progPnt->Pnt_Warning,   Warning);
    defBitMicro(ramPnt, progPnt->Pnt_Fail,      Fail   );
//====================================================================
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncLOCAL_APV_1[15]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Fail_IN
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Off/Block/Apv
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  DeleyControl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  DeleyOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  NormalWait
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  ApvTime_sec
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  ApvTime_min
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  KolvApv
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  ResetApv
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10 On/Off
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11 RTC
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12 VybegTime
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13 ApvCnt
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14 Warning
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //15 Fail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("LOCAL_APV", "DA", TblFuncLOCAL_APV_1, FuncLOCAL_APV_1, sizeof(TblFuncLOCAL_APV_1),sizeof(LOCAL_APVState_type), sizeof(LOCAL_APVnvState_type), 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncLOCAL_APV_1,
#endif
//********************************************************
/* �������
 globsPnt->APVstatus.APV ==0
//=======================

    if( (load_s16(ramPnt, progPnt->Pnt_NormalWait) == 1) && sPnt->flag.ApvLatch == 1 ){      // ������� ������������
        if( sPnt->flag.secFlag == 1){
            sPnt->flag.secFlag = 0;
            sPnt->TimeToAPV++;                              // ����������� ����� ���������
            WrFramMicro(&nvAdr->TimeToAPV, &sPnt->TimeToAPV, sizeof(nvAdr->TimeToAPV));
        }
    }
    else{                                                   // ��������� ���������
        if(load_s16(ramPnt, progPnt->Pnt_Off_Block_Apv) == 2){
            if (!sPnt->flag.ApvLatch && testBitMicro(ramPnt, progPnt->Pnt_Fail)){
                ////sPnt->flag.FailState = 1;
                tmp = load_s16(ramPnt, progPnt->Pnt_KolvApv);
                if ((tmp > sPnt->CntApv || tmp == 0)&&(sPnt->flag.Stop == 0)){        //��������� ������ || ����� ����������� ��� �����
                    sPnt->flag.ApvLatch = 1;
                    TimeToAPV = load_s16(ramPnt, progPnt->Pnt_ApvTime_min)*60 + load_s16(ramPnt, progPnt->Pnt_ApvTime_sec);
                    sPnt->TimeToAPV = TimeToAPV + load_s32(ramPnt, progPnt->Pnt_RTC);   // ����� ��� ����������� ����� ���
                    sPnt->CntApv++;
                    WrFramMicro(&nvAdr, &sPnt, sizeof(LOCAL_APVnvState_type));  // ����� ������� � ����� �����
                }
            }
        }
    }
//=======================
*/





