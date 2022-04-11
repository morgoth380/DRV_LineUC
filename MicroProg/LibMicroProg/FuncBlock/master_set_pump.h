/************************** (C) Copyright 2013 Triol ***************************
* File Name          : master_set_pump.h
* Author             : Kats Evgeniy
* TS Idea            : 2013.02.19
* TS Version         : 2013.02.21
* Description        : ���� �������� �������, ������� ��������� ������ ���������
*                      ��� ��������� ����������
*******************************************************************************/
#ifdef    GET_FUNC_CODE

// ������ ������� ���������
typedef enum{
    PumpState_level_START,
    PumpState_level_1,
    PumpState_level_2,
    PumpState_level_3,
    PumpState_level_4,
    PumpState_level_5,
    PumpState_level_6,
    PumpState_level_7,
    PumpState_level_8,
    PumpState_level_9,
    PumpState_level_10,
    PumpState_level_11,
    PumpState_level_12,
    PumpState_level_13,
    PumpState_level_14,
    PumpState_level_15,
    PumpState_level_16,
    PumpState_level_17,
    PumpState_level_END,
}PumpState_type;

// ������ ���������� ��� ������ ������ ������� ���������
const u16 uIDPrmPump[] = {
        0x0000,             //��������� ��������
        0x0000,             //00.00 ����������� ��� ���������
        0x0200,             //04.00 ������� �����
        0x0201,             //04.01 ����� �������� �������� �������
        0x0180,             //03.00 ����� �������
        0x1704,             //46.04 ������������� ��� ����������
        0x0D01,             //26.01 ���������������� �����������
        0x0D02,             //26.02 ������������ �����������
        0x0D03,             //26.03 ���������������� �����������
        0x1705,             //46.05 ������������� ���������� �����������
        0x0A09,             //20.09 ������� ��������� ������������ ���������
        0x0A0A,             //20.10 ������� ���������� ������������ ���������
        0x0A02,             //20.02 ����� ��������� �1
        0x0A03,             //20.03 ����� ��������� �2
        0x0A04,             //20.04 ����� ��������� �3
        0x0A05,             //20.05 ����� ��������� �4
        0x0A80,             //21.00 ������������ �������
        0x1203,             //36.03 �������� �������� ����������
};

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt__Prm0;                         //1  &Prm
    u16  Pnt__Prm1;                         //1  &Prm
    u16  Pnt__Prm2;                         //1  &Prm
    u16  Pnt_NumState;                     //2  NumState
    u16  Pnt_SelPID;                       //3  SelPID
    u16  Pnt_SelCascad;                    //4  SelCascad
    u16  Pnt_End;
}FuncMASTER_SET_PUMP_type;
//********************************************************
u16 *FuncMASTER_SET_PUMP_1(FuncMASTER_SET_PUMP_type *progPnt, u32 ramPnt)
{
    u16 NumState;   // ������� ��������� ������� ���������
    u16 IDPrmTek;   // ������� �������� ���������� ��������� (����� �������������� ��������� � �������) 
    u16 SelPID;     // �������� ��������� ������ ��������� ��� ���������� ("0" - ���, "1" - ��)
    u16 SelCascad;  // �������� ��������� ������ ��������� ���������� ���������� ("0" - ���, "1" - ��)


    NumState = load_s16(ramPnt, progPnt->Pnt_NumState);
    IDPrmTek = load_s16(ramPnt, progPnt->Pnt__Prm0);
    SelPID = load_s16(ramPnt, progPnt->Pnt_SelPID);
    SelCascad = load_s16(ramPnt, progPnt->Pnt_SelCascad);

    switch (NumState)                                   // ���������� ������� ��������� ������� ��������� (��) 
    {
    case (PumpState_level_START):                       // ��������� ���������� ������ �� "����������� ��� ���������"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_1 ])    // ���� �������� ���������� ��������� �� ������������� ������ ��������� ��� ������� ������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_1 ];     // �� ����������� ���������� ��������� ����������� ����� ���������
        }
        NumState = PumpState_level_1;
        break;
    case (PumpState_level_1):                           // ��������� 1-�� ������ �� "����������� ��� ���������"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_1 ])    // ���� �������� ���������� ��������� �� ������������� ������ ��������� ��� ������� ������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_1 ];     // �� ����������� ���������� ��������� ����������� ����� ���������
        }
        break;
    case (PumpState_level_2):                           // ��������� 2-�� ������ �� "������� �����"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_2])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_2];
        }
        break;
    case (PumpState_level_3):                           // ��������� 3-�� ������ �� "����� �������� �������� �������"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_3])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_3];
        }
        break;
    case (PumpState_level_4):                           // ��������� 4-�� ������ �� "����� �������"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_4])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_4];
        }
        break;
    case (PumpState_level_5):                           // ��������� 5-�� ������ �� "������������� ��� ����������"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_5])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_5];
        }
        break;
    case (PumpState_level_6):                           // ��������� 6-�� ������ �� "���������������� �����������"
        if (SelPID)                                     // ���� ������� ��������� ��������
        {
            if (IDPrmTek != uIDPrmPump[ PumpState_level_6]) // �� ���� �������� ���������� ��������� �� ������������� ������ ��������� ��� ������� ������ 
            {
                IDPrmTek = uIDPrmPump[ PumpState_level_6];  // �� ����������� ���������� ��������� ����������� ����� ���������
            }
        }
        else
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // ����� ����������� ���������� ��������� ����� ��������� "������������� ���������� �����������"         
            NumState = PumpState_level_9;
        }
        break;
    case (PumpState_level_7):                           // ��������� 7-�� ������ �� "������������ �����������"
        if (!SelPID)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_5 ];     // �� ����������� ���������� ��������� ����� ��������� "������������� ���������� �����������" 
            NumState = PumpState_level_5;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_7])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_7];
        }
        break;
    case (PumpState_level_8):                           // ��������� 8-�� ������ �� "���������������� �����������"
        if (!SelPID)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_5 ];     // �� ����������� ���������� ��������� ����� ��������� "������������� ���������� �����������" 
            NumState = PumpState_level_5;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_8])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_8];
        }
        break;
    case (PumpState_level_9):                           // ��������� 9-�� ������ �� "������������� ���������� �����������"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_9]) // ���� �������� ���������� ��������� �� ������������� ������ ��������� ��� ������� ������ 
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9];  // �� ����������� ���������� ��������� ����������� ����� ���������
        }
        break;
    case (PumpState_level_10):                          // ��������� 10-�� ������ �� "������� ��������� ������������ ���������"
        if (SelCascad)                                     // ���� ������� ��������� ��������
        {
            if (IDPrmTek != uIDPrmPump[ PumpState_level_10]) // �� ���� �������� ���������� ��������� �� ������������� ������ ��������� ��� ������� ������ 
            {
                IDPrmTek = uIDPrmPump[ PumpState_level_10];  // �� ����������� ���������� ��������� ����������� ����� ���������
            }
        }
        else
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_16 ];     // ����� ����������� ���������� ��������� ����� ��������� "������������ �������"         
            NumState = PumpState_level_16;
        }
        break;
    case (PumpState_level_11):                          // ��������� 11-�� ������ �� "������� ���������� ������������ ���������"
        if (!SelCascad)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // �� ����������� ���������� ��������� ����� ��������� "������������ �������" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_11])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_11];
        }
        break;
    case (PumpState_level_12):                          // ��������� 12-�� ������ �� "����� ��������� �1"
        if (!SelCascad)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // �� ����������� ���������� ��������� ����� ��������� "������������ �������" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_12])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_12];
        }
        break;
    case (PumpState_level_13):                          // ��������� 13-�� ������ �� "����� ��������� �2"
        if (!SelCascad)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // �� ����������� ���������� ��������� ����� ��������� "������������ �������" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_13])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_13];
        }
        break;
    case (PumpState_level_14):                          // ��������� 14-�� ������ �� "����� ��������� �3"
        if (!SelCascad)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // �� ����������� ���������� ��������� ����� ��������� "������������ �������" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_14])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_14];
        }
        break;
    case (PumpState_level_15):                          // ��������� 15-�� ������ �� "����� ��������� �4"
        if (!SelCascad)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_9 ];     // �� ����������� ���������� ��������� ����� ��������� "������������ �������" 
            NumState = PumpState_level_9;
        }
        else if (IDPrmTek != uIDPrmPump[ PumpState_level_15])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_15];
        }
        break;
    case (PumpState_level_16):                           // ��������� 16-�� ������ �� "������������ �������"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_16])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_16];
        }
        break;
    case (PumpState_level_17):                           // ��������� 17-�� ������ �� "�������� �������� ����������"
        if (IDPrmTek != uIDPrmPump[ PumpState_level_17])
        {
            IDPrmTek = uIDPrmPump[ PumpState_level_17];
        }
        break;
    case (PumpState_level_END):                         // ��������� ������ �� ��� "������������"
        NumState = PumpState_level_17;
        break;
    default:                                                // ��������� ��� ������ ������ �� ���������� ������� (�������������� ��������)
        NumState = PumpState_level_1;
        IDPrmTek = uIDPrmPump[ PumpState_level_1 ];
        break;
    }
    save_s16(ramPnt, progPnt->Pnt_NumState,  NumState);
    save_s16(ramPnt, progPnt->Pnt__Prm0,  IDPrmTek);
    save_s16(ramPnt, progPnt->Pnt__Prm1,  IDPrmTek);
    save_s16(ramPnt, progPnt->Pnt__Prm2,  IDPrmTek);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMASTER_SET_PUMP_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  &Prm0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  &Prm1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  &Prm2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  NumState
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  SelPID
        S16_PIN_TYPE | INPUT_PIN_MODE      //6  SelCascad
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MASTER_SET_PUMP", "DD", TblFuncMASTER_SET_PUMP_1, FuncMASTER_SET_PUMP_1, sizeof(TblFuncMASTER_SET_PUMP_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMASTER_SET_PUMP_1,
#endif
//********************************************************
