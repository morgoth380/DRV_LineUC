/* =================================================================================
File name:  aci_pi_reg_Psir.h  (IQ version)
                    
Originator: TRIOL
��������:   ��������� ��������������� ������
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-10-2021 Version 1.00
------------------------------------------------------------------------------*/
#include "type.h"
#include "math.h"
#include "aci_pi_reg_psir.h"


void AciPiRegPsirCalc(ACIPIREGPSIR_handle v){
  f32 fieldWeakening_psirRef, pi_psir_OutMaxPrefilt;
  f32 pi_psir_Err, pi_psir_Up, pi_psir_OutPreSat;
  
  // �������� ��������������� ������ (���������� ����)
  if (v->FWsecondZoneOnState) {
    if (v->WPsi != 0) {
      fieldWeakening_psirRef = v->E_nom_new /  fabsf(v->WPsi);
    } else {
      fieldWeakening_psirRef = v->Psir_nom_new;
    }
  } else {
    fieldWeakening_psirRef = v->Psir_nom_new;
  }
  
  // ����������� ������� ���������������
  if (fieldWeakening_psirRef > v->Psir_nom_new) {                          // ���� ������ ���. ������
    fieldWeakening_psirRef = v->Psir_nom_new;
  } else if (fieldWeakening_psirRef < (0.1f * v->Psir_nom_new)) {  // ���� ������ 10% ���. ������
    fieldWeakening_psirRef = (0.1f * v->Psir_nom_new);
  }

  // ���� ���������� ��� ���� ������������ ���������� ��������� - �������� ���������������
  if(v->Udcveryslow < v->fieldWeakening_Usmax) {
    fieldWeakening_psirRef = (fieldWeakening_psirRef * v->Udcveryslow) / v->fieldWeakening_Usmax;
  }

  // ������� ����������
  v->pi_psir_Ref = fieldWeakening_psirRef;
  
  // ����������� ����������
  if (v->FlagUsQmax) {
    pi_psir_OutMaxPrefilt = 0;
  } else {
    pi_psir_OutMaxPrefilt = v->Idn;
  }
  // ������������ �����������
  v->pi_psir_OutMax = (v->K1filtVolWeak * v->pi_psir_OutMax) + (v->K2filtVolWeak * pi_psir_OutMaxPrefilt);
  v->pi_psir_OutMin = -v->pi_psir_OutMax;
  
  // ��������� ��������������� ������
  //----------------------------------------------------------
  // Compute the error
  pi_psir_Err = v->pi_psir_Ref - v->pi_psir_Fdb;
  
  // Compute the proportional output
  pi_psir_Up = (v->pi_psir_Kp * pi_psir_Err);
  
  // Compute the integral output
  v->pi_psir_Ui = v->pi_psir_Ui + (v->pi_psir_Ki * pi_psir_Err);
  
  // Saturate the integral output
  if (v->pi_psir_Ui > v->pi_psir_OutMax) {
    v->pi_psir_Ui =  v->pi_psir_OutMax;
  } else if (v->pi_psir_Ui < v->pi_psir_OutMin) {
    v->pi_psir_Ui = v->pi_psir_OutMin;
  }
  
  // Compute the pre-saturated output
  pi_psir_OutPreSat = pi_psir_Up + v->pi_psir_Ui;
  
  // Saturate the output
  if(pi_psir_OutPreSat > v->pi_psir_OutMax) {
    v->pi_psir_Out = v->pi_psir_OutMax;
  } else if (pi_psir_OutPreSat < v->pi_psir_OutMin) {
    v->pi_psir_Out = v->pi_psir_OutMin;
  } else {
    v->pi_psir_Out = pi_psir_OutPreSat;
  }
}

// ����� �����
void AciPiRegPsirReset(ACIPIREGPSIR_handle v){
	v->pi_psir_Fdb             = 0;  // Input: �������� ����� ���������� ���������������
	v->WPsi                    = 0;  // Input: ������� ����
	v->Udcveryslow             = (1.4142135623730f);  // Input: ������� ���������� ��� (�������������)
	v->FWsecondZoneOnState     = 0;  // Input: ���� �������� �� ������ ����
	v->FlagUsQmax              = 0;  // Input: ���� ������� ����������� UsQ_MAXIM
	v->pi_psir_Ref             = 0;  // Var: ������� ���������� ���������������
	v->pi_psir_OutMax          = v->Idn;  // Var: ������������ ����������� ������ ����������
	v->pi_psir_OutMin          = -v->Idn;  // Var: ����������� ����������� ������ ����������
	v->pi_psir_Ui              = 0;  // Var: ������������ ������������ ����������

	v->pi_psir_Out             = 0;  // Output: ����� ���������� ��������������� ������
}
