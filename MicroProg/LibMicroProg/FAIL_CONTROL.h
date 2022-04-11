
#ifndef	FAIL_CONTROL_H
#define FAIL_CONTROL_H
#include "Global_include.h"

// Defines
//#define FastFailMask_NONE           0
//#define FastFailMask_SwU            1       // ������ �� ������� �� �������� ����� U
//#define FastFailMask_SwV            2       // ������ �� ������� �� �������� ����� V
//#define FastFailMask_SwW            3       // ������ �� ������� �� �������� ����� W
//#define FastFailMask_SwT            4       // ������ �� ������� �� �������� ���������� �����
//#define FastFailMask_MtzD           5       // ��� �� ����������� ������� �� �����������
//#define FastFailMask_MtzA           6       // ��� ����������� �� ������� ���
//#define FastFailMask_R7             7       // ������
//#define FastFailMask_UdMin          8       // ������ Ud
//#define FastFailMask_UdMax          9       // ������� Ud
//#define FastFailMask_UdCharge       10      //
//#define FastFailMask_Ud_KZ          11      //
//#define FastFailMask_Tu             12      // ����������� ������
//#define FastFailMask_r13            13      // ������
//#define FastFailMask_r14            14      // ������
//#define FastFailMask_r15            15      // ������
//
//#define SlowFailMask_Contactor      34      // ������ ����������
//********************************************************
typedef struct{
	s32		DelayApvCnt;        //������ ���
	s16		Request;            //����� �� ����� �� �������� ���
	s16		N_STOP;                //������� ��������
	s16		N_PUSK;                //������� �����
}FailControlState_type;

typedef struct{
	s16  	DelayApvCnt;
	s16		N_STOP;                //������� ��������
	s16		N_PUSK;                //������� �����
}nvFailControlState_type;
#define		nvFailControlState	        (*(nvFailControlState_type*)0)

typedef struct{
    u16		Pnt_TekFail;       //TekFail
    u16		Pnt_Request;       //Request
    u16		Pnt_Time;          //Time
    u16		Pnt_TypeTorm;      //TypeTorm
}SlotFc_type;


//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Man_Auto;        //1  Man/Auto	    
    u16  Pnt_KolvFail;        //2  KolvFail
    u16  Pnt_MaskTek_3;       //3  MaskTek_3	����� ������� ������ � ������ �������� ��������
    u16  Pnt_MaskTek_2;       //4  MaskTek_2
    u16  Pnt_MaskTek_1;       //5  MaskTek_1
    u16  Pnt_MaskTek_0;       //6  MaskTek_0
    u16  Pnt_N_TekFail;       //7  N_TekFail	������� ������ � ��������� �����������
    u16  Pnt_APV_budet;       //8  APV_budet	���� ����� ������� � ���� �������� ��������������� ���.
    u16  Pnt_Type_torm;       //9  Type_torm	��� ����������
    
    u16  Pnt_TimeDoOff;       //10  TimeDoOff	����� �� ����������
    u16  Pnt_near_STOP;       //11  N_STOP	    �������� ���������� �������� (���� ��������� ���, �� = 0)
    u16  Pnt_N_STOP;          //12  N_STOP	    �������� �������� �������� (� ������ = 0)
    
    u16  Pnt_TimeDoAPV;       //13  TimeDoAPV	����� �� ���
    u16  Pnt_near_PUSK;       //14  N_PUSK	    �������� ���������� ����� (���� ��������� ���, �� = 0)
    u16  Pnt_N_PUSK;          //15  N_PUSK	    �������� �������� ����� (� �������� = 0)
    
	SlotFc_type	SlotFc[4];
    u16  Pnt_End;
}FuncTEXN_CONTROL_4_type;
//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Man_Auto;        //1  Man/Auto	    
    u16  Pnt_KolvFail;        //2  KolvFail
    u16  Pnt_MaskTek_3;       //3  MaskTek_3	����� ������� ������ � ������ �������� ��������
    u16  Pnt_MaskTek_2;       //4  MaskTek_2
    u16  Pnt_MaskTek_1;       //5  MaskTek_1
    u16  Pnt_MaskTek_0;       //6  MaskTek_0
    u16  Pnt_N_TekFail;       //7  N_TekFail	������� ������ � ��������� �����������
    u16  Pnt_APV_budet;       //8  APV_budet	���� ����� ������� � ���� �������� ��������������� ���.
    u16  Pnt_Type_torm;       //9  Type_torm	��� ����������
    
    u16  Pnt_TimeDoOff;       //10  TimeDoOff	����� �� ����������
    u16  Pnt_near_STOP;       //11  N_STOP	    �������� ���������� �������� (���� ��������� ���, �� = 0)
    u16  Pnt_N_STOP;          //12  N_STOP	    �������� �������� �������� (� ������ = 0)
    
    u16  Pnt_TimeDoAPV;       //13  TimeDoAPV	����� �� ���
    u16  Pnt_near_PUSK;       //14  N_PUSK	    �������� ���������� ����� (���� ��������� ���, �� = 0)
    u16  Pnt_N_PUSK;          //15  N_PUSK	    �������� �������� ����� (� �������� = 0)
    
	SlotFc_type	SlotFc[32];
    u16  Pnt_End;
}FuncFAIL_CONTROL_32_type;
//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Man_Auto;        //1  Man/Auto	    
    u16  Pnt_KolvFail;        //2  KolvFail
    u16  Pnt_MaskTek_3;       //3  MaskTek_3	����� ������� ������ � ������ �������� ��������
    u16  Pnt_MaskTek_2;       //4  MaskTek_2
    u16  Pnt_MaskTek_1;       //5  MaskTek_1
    u16  Pnt_MaskTek_0;       //6  MaskTek_0
    u16  Pnt_N_TekFail;       //7  N_TekFail	������� ������ � ��������� �����������
    u16  Pnt_APV_budet;       //8  APV_budet	���� ����� ������� � ���� �������� ��������������� ���.
    u16  Pnt_Type_torm;       //9  Type_torm	��� ����������
    
    u16  Pnt_TimeDoOff;       //10  TimeDoOff	����� �� ����������
    u16  Pnt_near_STOP;       //11  N_STOP	    �������� ���������� �������� (���� ��������� ���, �� = 0)
    u16  Pnt_N_STOP;          //12  N_STOP	    �������� �������� �������� (� ������ = 0)
    
    u16  Pnt_TimeDoAPV;       //13  TimeDoAPV	����� �� ���
    u16  Pnt_near_PUSK;       //14  N_PUSK	    �������� ���������� ����� (���� ��������� ���, �� = 0)
    u16  Pnt_N_PUSK;          //15  N_PUSK	    �������� �������� ����� (� �������� = 0)
    
	SlotFc_type	SlotFc[64];
    u16  Pnt_End;
}FuncFAIL_CONTROL_64_type;
//********************************************************
void FuncFAIL_CONTROL(FuncFAIL_CONTROL_64_type *progPnt, u32 ramPnt);
//********************************************************
#endif	//FAIL_CONTROL_H
