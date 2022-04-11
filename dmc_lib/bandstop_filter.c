/*=====================================================================================
 File name:    PMSM_BSF.C  (IQ version)
                    
 Originator:     Triol / Sosnovchik
URL: 			http://www.dsplib.ru/content/notch/notch.html

 Description:  Bandstop filter

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 22-12-2011 Version 1.00      
-------------------------------------------------------------------------------------*/

#include "Global_Include.h"
#include "type.h"
#include "bandstop_filter.h"

BSF	reject1 = BSF_DEFAULTS;
BSF	reject2 = BSF_DEFAULTS;

//========================================================================
void BandStopFilter_calc(BSF *v)
{   
  f32	Fnykv; // ������� ��������� (�������� ������� �������������)
  f32	tmpB, tmpA1;
  
  if (v->Enable) {  // ���� ��������� ��������� - ��������� �
    
    // 1) ������ ������������� �������
    
    /* ����� ������� ������������� ����:
    * 1. ������ ������� ������������� � ��.
    * 2. ������� ������� �������� � ��
    * 3. ������� ������ ����������� � ��
    */
    
    // http://www.dsplib.ru/content/notch/notch.html
    Fnykv = v->Fsample / 2.0f; // ������� ���������
    v->K1 = -arm_cos_f32( (v->Fstop*PI) / Fnykv ); // ����. �������� �� ����������� �������
    tmpB = (v->Bandwidth*PI) / Fnykv ;
    v->K2 = ( 1 -  arm_sin_f32(tmpB) ) / arm_cos_f32(tmpB); // ����. �������� �� ������ �����������
    
    tmpA1 = ( (1 + v->K2) / 2.0 );
    v->A1 = tmpA1;			// ����������� ��� �������� X(i) -   A1 = (1+k2)/2
    v->A2 = 2.0 * tmpA1 * v->K1;		// ����������� ��� �������� X(i-1) - A2 = A1*2*K1
    v->A3 = v->A1;			// ����������� ��� �������� X(i-2) - A3 = A1
    v->B1 = v->K1 * (1.0 + v->K2);	// ����������� ��� �������� Y(i-1) - B1 = K1*(1+K2)
    v->B2 = v->K2;			// ����������� ��� �������� Y(i-2) - B2 = K2
            
    // 2) Filter calculation
    v->Xi[0] = v->In;
    
    // �������� �������� ������� ������� Yi
    v->Yi[2] = v->Yi[1];
    v->Yi[1] = v->Yi[0];
        
    // ������ ��������� ��������
    v->Yi[0] = (v->A1*v->Xi[0]) + (v->A2*v->Xi[1]) + (v->A3*v->Xi[2]) - (v->B1*v->Yi[1]) - (v->B2*v->Yi[2]);
    
    // �������� ������� �������
    v->Xi[2] = v->Xi[1];
    v->Xi[1] = v->Xi[0];
    
    // �������� ��������
    v->Out = v->Yi[0];
  } else { // ���� ���������� �����������
    v->Out = v->In; // ������ �������� ���� �� �����
  }
}
//========================================================================
void BandStopFilter_init(BSF *v)
{   
    // ����� ��������� ��������
    v->Xi[0] = 0;      	v->Xi[1] = 0;      	v->Xi[2] = 0;
    v->Yi[0] = 0;	v->Yi[1] = 0; 		v->Yi[2] = 0;
}
//========================================================================