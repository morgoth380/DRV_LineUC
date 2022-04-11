#ifdef    GET_FUNC_CODE
#ifdef _NVSA_
    #include "NVSADrvUart.h"
#else /** _NVSA_ */
    #include "NVSACDrvUart.h"
#endif /** _NVSA_ */

#include "Tsk_MasterUart.h"

typedef struct{
    u16  Pnt_Nfunc;
//    u16  Pnt_State;
//    u16  Pnt_nvState;
    u16  Pnt_Ftek_Input;                   //1  Ftek_Input
    u16  Pnt_Delay_time;                   //2  Delay_time
    u16  Pnt_Res1;                         //3  Res1 - ������� ������� � IQ �������
    u16  Pnt_Res2;                         //4  Res2 = 0
    u16  Pnt_F_Out;                        //5  F_Out
    u16  Pnt_Res3;                         //6  Res3
    u16  Pnt_End;
}FuncDELAY_FREQ_type;
//********************************************************

#ifdef _PROJECT_FOR_CCS_
extern TGlobalMasterSlave GlobalMstSlv;
#else
      TGlobalMasterSlave GlobalMstSlv;
#endif //-- _PROJECT_FOR_CCS_

//********************************************************
// ��� ����� ������ �� 10 ��������� s32
#define QUEUE_MULTIPLE 1 // �������� ���������� �������� � �������.
#define QueueLen 11*QUEUE_MULTIPLE

s32 DelayQueue [QueueLen];
//********************************************************
u16 *FuncDELAY_FREQ_1(FuncDELAY_FREQ_type *progPnt, u32 ramPnt)
{
	u16  DelayTime,i, Uart0_Mode, Uart2_Mode;
	s32   freq;

	Uart0_Mode = GlobalMstSlv.Uart0_Mode; //������ ������ UART��
	Uart2_Mode = GlobalMstSlv.Uart2_Mode; //������ ������ UART��

	if (Uart0_Mode == ModbusSlave || Uart2_Mode == ModbusSlave) // ��������� ������ ������ UART'��
	{ // ���� ����� ������ UART'a - ModbusSlave, �������� �� IQ ������� ������� - Pnt_Res1
		// ���� ������� ����� ModbusSlave - ������ ��� �� �������� ��� ����������� �������
		// 1. ������� ���� ������ �� ������������ �������� ��������� ������� - ��� �������� �� �����.
		// 2. �� ��������� Pnt_Res1 - ����� ������� �������� ������� � IQ �������. ���� ������ ����� ���������� ����� �������.

		// ���������� �������� �������� ��������� �������
//		save_s16(ramPnt, progPnt->Pnt_Delay_time, 0);

		// ��������� ������� ������� � IQ ������� �� ���������� Fref_IQ_Slave - ���� MASTER_SLAVE
	    freq = load_s32 (ramPnt, progPnt->Pnt_Res1);
	    save_s32(ramPnt, progPnt->Pnt_F_Out, freq); // �������� �������� �������� ������� �� �����
	    save_s32(ramPnt, progPnt->Pnt_Ftek_Input, freq); // !!!! ��������� �������� ������� � �������� �������� �� ���������� �����

#ifdef _PROJECT_FOR_CCS_
#warning �� ������� ������ �������� Fref_IQ (Pnt_Res3) ������ �� �����, ��� ��� ���������� ��������� ������ ����.
#endif //-- _PROJECT_FOR_CCS_

	    save_s32(ramPnt, progPnt->Pnt_Res3, freq);// �������� �������� �� IQ ����� - �� ������� ������ ������ �������� ������ �� �����
	}
	else
	{ // ���� ����� ������ ��������� UART'a - �� ModbusSlave,
	  // ��������� ����������� �������� ��������� �������, ��� ��� � ��������� ������ ������
	  // ��� �� ������ �� ������ ��. ���� �� ����� ������� ����� �������� - �� �������� ����� ��� �����������.

		DelayTime = load_s16 (ramPnt, progPnt->Pnt_Delay_time); // �������� �������� ��������
		DelayTime *= QUEUE_MULTIPLE; // �������� ����������� � N ���

	    // �������� ��� �������� �� ����
	    for (i=(QueueLen-1); i>0; i--)
	    { DelayQueue[i] = DelayQueue[i-1]; }

		// ����� ������� ������� � ���������� �����, � ������� � ������� ������
	    freq = load_s32 (ramPnt, progPnt->Pnt_Ftek_Input);
	    DelayQueue[0] = freq;

	    // ����� ����� �������, ������� ������� � IQ �������, ����� ��������� � ���������� Fref_IQ,
	    // ���� ����� �������������� ����� �������� - �� ������� ����� ��� �������� � � ����� ����� ����� MakeOpis (52, 44)
	    save_s32(ramPnt, progPnt->Pnt_Res3, freq);// �������� �������� �� �����

		// ����� �������� �� ������ � ������������ � ������� �������� � ������ �� �����
	    freq = DelayQueue[DelayTime];
	    save_s32(ramPnt, progPnt->Pnt_F_Out, freq);// �������� �������� ������� �� �����
	}

	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDELAY_FREQ_1[6]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ftek_Input
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Delay_time
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Res1 - ������� ������� � IQ �������
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Res2 - ������� ����� ������ �������� UAR�� - ����� ������ UART2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //5  F_Out
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //6  Res3
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DELAY_FREQ", "U", TblFuncDELAY_FREQ_1, FuncDELAY_FREQ_1, sizeof(TblFuncDELAY_FREQ_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDELAY_FREQ_1,
#endif
//********************************************************
