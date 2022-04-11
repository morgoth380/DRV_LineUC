/* =================================================================================
File name:  UdcFiltr.c  (IQ version)
                    
Originator: TRIOL
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-10-2021 Version 1.00
------------------------------------------------------------------------------*/
#include "type.h"
#include "UdcFiltr.h"


void UdcFiltrCalc(UDCFILTR_handle v){

	v->UdcOut = v->UdcOut * v->K1udcFilt + v->UdcIn * v->K2udcFilt;
}

// ����� �����
void UdcFiltrReset(UDCFILTR_handle v){
	v->UdcIn     = 0;       // Input: ���������� ��� ��� ����������

	v->K1udcFilt = 0;       // Init: ����������� ������� 1
	v->K2udcFilt = 0;       // Init: ����������� ������� 2

	v->UdcOut    = 0;       // Output: ���������� ��� �������������
}
