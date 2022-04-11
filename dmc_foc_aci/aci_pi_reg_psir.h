/* =================================================================================
File name:  aci_pi_reg_Psir.h  (f32 version)
                    
Originator: TRIOL
��������:   ��������� ��������������� ������
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-10-2021 Version 1.00
------------------------------------------------------------------------------*/
#ifndef _ACIPIREGPSIR_H_
#define _ACIPIREGPSIR_H_

typedef struct {
  f32  pi_psir_Fdb;              // Input: �������� ����� ���������� ���������������
  f32  WPsi;                     // Input: ������� ����
  f32  Udcveryslow;              // Input: ������� ���������� ��� (�������������)
  u16  FWsecondZoneOnState;      // Input: ���� �������� �� ������ ����
  u16  FlagUsQmax;               // Input: ���� ������� ����������� UsQ_MAXIM
  
  f32  Idn;                      // Init: ����������� ��� �����������
  f32  E_nom_new;                // Init:
  f32  Psir_nom_new;             // Init:
  f32  fieldWeakening_Usmax;     // Init:
  f32  fieldWeakening_invUsmax;  // Init:
  f32  K1filtVolWeak;            // Init:
  f32  K2filtVolWeak;            // Init:
  f32  pi_psir_Kp;               // Init:
  f32  pi_psir_Ki;               // Init:
  
  f32  pi_psir_Ref;              // Var: ������� ���������� ���������������
  f32  pi_psir_OutMax;           // Var: ������������ ����������� ������ ����������
  f32  pi_psir_OutMin;           // Var: ����������� ����������� ������ ����������
  f32  pi_psir_Ui;               // Var: ������������ ������������ ����������
  
  f32  pi_psir_Out;              // Output: ����� ���������� ��������������� ������
  
  void  (*calc)();              // Pointer to calculation function
  void  (*reset)();             // Pointer to reset function
} ACIPIREGPSIR;

typedef ACIPIREGPSIR *ACIPIREGPSIR_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the ACIPIREGPSIR object.
-----------------------------------------------------------------------------*/                     
#define ACIPIREGPSIR_DEFAULTS { 0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                0,        \
                                (void (*)(u32))AciPiRegPsirCalc, \
                                (void (*)(u32))AciPiRegPsirReset }

/*------------------------------------------------------------------------------
Prototypes for the functions in aci_pi_reg_Psir.c
------------------------------------------------------------------------------*/
void AciPiRegPsirCalc(ACIPIREGPSIR_handle);
void AciPiRegPsirReset(ACIPIREGPSIR_handle);

#endif // _ACIPIREGPSIR_H_
