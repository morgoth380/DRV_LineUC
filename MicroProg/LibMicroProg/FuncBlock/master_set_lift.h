/************************** (C) Copyright 2013 Triol ***************************
* File Name          : master_set_lift.h
* Author             : Kats Evgeniy
* TS Idea            : 2013.02.19
* TS Version         : 2013.02.20
* Description        : ���� �������� �������, ������� ��������� ������ ���������
*                      ��� ��������� ����������                        
*******************************************************************************/
#ifdef    GET_FUNC_CODE

// ������ ������� ���������
typedef enum{
    LiftState_level_START,
    LiftState_level_1,
    LiftState_level_2,
    LiftState_level_3,
    LiftState_level_4,
    LiftState_level_5,
    LiftState_level_6,
    LiftState_level_7,
    LiftState_level_8,
    LiftState_level_9,
    LiftState_level_10,
    LiftState_level_11,
    LiftState_level_END,
}LiftState_type;

// ������ ���������� ��� ������ ������ ������� ���������
const u16 uIDPrm[] = {
        0x0000,             //��������� ��������
        0x0000,             //00.00 ����������� ��� ���������
        0x0003,             //00.03 ����������� ���������� ������������ ���������
        0x0002,             //00.02 ����������� ������� ������������ ���������
        0x0001,             //00.01 ����������� �������� ������������ ���������
        0x0004,             //00.04 ����� ��� ������� ������������ ���������
        0x1219,             //36.25 ������������ ������ �������� ���, ����������� ��������
        0x0086,             //01.06 ������ �������
        0x1706,             //46.06 ��� ���������� (������������� ��������)
        0x0008,             //00.08 ��� ��������
        0x0006,             //00.06 ����� ����� (���������) �� ������
        0x0005,             //00.05 ���������� ��������
};
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt__Prm0;                         //1  &Prm
    u16  Pnt__Prm1;                         //1  &Prm
    u16  Pnt__Prm2;                         //1  &Prm
    u16  Pnt_NumState;                     //2  NumState
    u16  Pnt_SelectEnc;                    //3  SelectEnc
    u16  Pnt_End;
}FuncMASTER_SET_LIFT_type;
//********************************************************
u16 *FuncMASTER_SET_LIFT_1(FuncMASTER_SET_LIFT_type *progPnt, u32 ramPnt)
{   
    u16 NumState;   // ������� ��������� ������� ���������
    u16 IDPrmTek;   // ������� �������� ���������� ��������� (����� �������������� ��������� � �������) 
    u16 SelEnc;     // �������� ��������� ������ ��������� �������� ("0" - ���, "1" - ��)


    NumState = load_s16(ramPnt, progPnt->Pnt_NumState);
    IDPrmTek = load_s16(ramPnt, progPnt->Pnt__Prm0);
    SelEnc = load_s16(ramPnt, progPnt->Pnt_SelectEnc);

    switch (NumState)                                   // ���������� ������� ��������� ������� ��������� (��) 
    {
    case (LiftState_level_START):                       // ��������� ���������� ������ �� "����������� ��� ���������"
        if (IDPrmTek != uIDPrm[ LiftState_level_1 ])    // ���� �������� ���������� ��������� �� ������������� ������ ��������� ��� ������� ������
        {
            IDPrmTek = uIDPrm[ LiftState_level_1 ];     // �� ����������� ���������� ��������� ����������� ����� ���������
        }
        NumState = LiftState_level_1;
        break;
    case (LiftState_level_1):                           // ��������� 1-�� ������ �� "����������� ��� ���������"
        if (IDPrmTek != uIDPrm[ LiftState_level_1 ])    // ���� �������� ���������� ��������� �� ������������� ������ ��������� ��� ������� ������
        {
            IDPrmTek = uIDPrm[ LiftState_level_1 ];     // �� ����������� ���������� ��������� ����������� ����� ���������
        }
        break;
    case (LiftState_level_2):                           // ��������� 2-�� ������ �� "����������� ���������� ������������ ���������"
        if (IDPrmTek != uIDPrm[ LiftState_level_2])
        {
            IDPrmTek = uIDPrm[ LiftState_level_2];
        }
        break;
    case (LiftState_level_3):                           // ��������� 3-�� ������ �� "����������� ������� ������������ ���������"
        if (IDPrmTek != uIDPrm[ LiftState_level_3])
        {
            IDPrmTek = uIDPrm[ LiftState_level_3];
        }
        break;
    case (LiftState_level_4):                           // ��������� 4-�� ������ �� "����������� �������� ������������ ���������"
        if (IDPrmTek != uIDPrm[ LiftState_level_4])
        {
            IDPrmTek = uIDPrm[ LiftState_level_4];
        }
        break;
    case (LiftState_level_5):                           // ��������� 5-�� ������ �� "����� ��� ������� ������������ ���������"
        if (IDPrmTek != uIDPrm[ LiftState_level_5])
        {
            IDPrmTek = uIDPrm[ LiftState_level_5];
        }
        break;
    case (LiftState_level_6):                           // ��������� 6-�� ������ �� "������������ ������ �������� ���, ����������� ��������"
        if (IDPrmTek != uIDPrm[ LiftState_level_6])
        {
            IDPrmTek = uIDPrm[ LiftState_level_6];
        }
        break;
    case (LiftState_level_7):                           // ��������� 7-�� ������ �� "������ �������"
        if (IDPrmTek != uIDPrm[ LiftState_level_7])
        {
            IDPrmTek = uIDPrm[ LiftState_level_7];
        }
        break;
    case (LiftState_level_8):                           // ��������� 8-�� ������ �� "��� ���������� (������������� ��������)"
        if (IDPrmTek != uIDPrm[ LiftState_level_8])
        {
            IDPrmTek = uIDPrm[ LiftState_level_8];
        }
        break;
    case (LiftState_level_9):                           // ��������� 9-�� ������ �� "��� ��������"
        if (SelEnc)                                     // ���� ������� ��������� ��������
        {
            if (IDPrmTek != uIDPrm[ LiftState_level_9]) // �� ���� �������� ���������� ��������� �� ������������� ������ ��������� ��� ������� ������ 
            {
                IDPrmTek = uIDPrm[ LiftState_level_9];  // �� ����������� ���������� ��������� ����������� ����� ���������
            }
        }
        else
        {
            IDPrmTek = uIDPrm[ LiftState_level_8 ];     // ����� ����������� ���������� ��������� ����� ��������� "��� ����������"         
            NumState = LiftState_level_8;
        }
        break;
    case (LiftState_level_10):                          // ��������� 10-�� ������ �� "����� ����� (���������) �� ������"
        if (!SelEnc)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrm[ LiftState_level_8 ];     // �� ����������� ���������� ��������� ����� ��������� "��� ����������" 
            NumState = LiftState_level_8;
        }
        else if (IDPrmTek != uIDPrm[ LiftState_level_10])
        {
            IDPrmTek = uIDPrm[ LiftState_level_10];
        }
        break;
    case (LiftState_level_11):                          // ��������� 11-�� ������ �� "���������� ��������"
        if (!SelEnc)                                    // ���� ������� ��������� ��� ��������
        {
            IDPrmTek = uIDPrm[ LiftState_level_8 ];     // �� ����������� ���������� ��������� ����� ��������� "��� ����������" 
            NumState = LiftState_level_8;
        }
        else if (IDPrmTek != uIDPrm[ LiftState_level_11])
        {
            IDPrmTek = uIDPrm[ LiftState_level_11];
        }
        break;
    case (LiftState_level_END):                         // ��������� ������ �� ��� "������������"
        if (!SelEnc)
        {
            IDPrmTek = uIDPrm[ LiftState_level_8 ];
            NumState = LiftState_level_8;
        }
        else if (IDPrmTek != uIDPrm[ LiftState_level_11])
        {
            IDPrmTek = uIDPrm[ LiftState_level_11 ];
            NumState = LiftState_level_11;
        }
        else
        {
            NumState = LiftState_level_11;
        }
        break;
    default:                                                // ��������� ��� ������ ������ �� ���������� ������� (�������������� ��������)
        NumState = LiftState_level_1;
        IDPrmTek = uIDPrm[ LiftState_level_1 ];
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
const char TblFuncMASTER_SET_LIFT_1[5]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  &Prm0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  &Prm1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  &Prm2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  NumState
        S16_PIN_TYPE | INPUT_PIN_MODE      //5  SelectEnc
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MASTER_SET_LIFT", "DD", TblFuncMASTER_SET_LIFT_1, FuncMASTER_SET_LIFT_1, sizeof(TblFuncMASTER_SET_LIFT_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMASTER_SET_LIFT_1,
#endif
//********************************************************
