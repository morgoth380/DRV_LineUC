/*=====================================================================================
 File name:    LOAD_LOSS.C 
                    
 Originator:     Triol / Sosnovchik

 Description:  Load loss module

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 06-01-2022 Version 1.00      
-------------------------------------------------------------------------------------*/

#include "Global_Include.h"
#include "type.h"
#include "load_loss.h"
#include "Pjk_const.h"

LOADLOSSmodule	loadloss1 = LOADLOSS_DEFAULTS;
#define		MINIMAL_FREQ_LOAD_LOSS	0.2F
#define         MAX_FAULT_CNT           4
//========================================================================
void LoadLoss_calc(LOADLOSSmodule *v)
{   
  
    u16         TimeLoadLossCheck; // ������ �����
    f32         absFout;
    
    absFout = fabsf(v->Fout);
    
    // ������� �������� � ��
    if (absFout < MINIMAL_FREQ_LOAD_LOSS) { // ���� �������� ������� ����� 5 ��, �� ���� ���������� �������� ��� ������� 5 ��
        TimeLoadLossCheck = (u16)(((1 / MINIMAL_FREQ_LOAD_LOSS) / 12) / v->Tpwm); // ������ ������������� �������� ����� ��� ������� ������� � ������� ����. ��������� ������� �� 12 ��������
    } else {
        TimeLoadLossCheck = (u16)(((1 / absFout) / 12) / v->Tpwm); // ������ ������������� �������� ����� ��� ������� ������� � ������� ����
    }
    
    if (!v->PwmOn || absFout < MINIMAL_FREQ_LOAD_LOSS) { // ���� ��� ��������, ���������� ��������� ���� ��� ������ ������
      v->prevTheta = v->currentThetaI; // ������� ���� ���� ����������� �� �����/����
      v->timer = v->cntFault = v->Fault = 0;
      v->estimateThetaTmp = 0;
    } else { // ���� ��� ������� - ��������
      v->timer ++; // ����������� �������
      
      // Calculate to temporary variable
      v->estimateThetaTmp = v->estimateThetaTmp + _2PI * v->Fout * v->Tpwm; // ������ ������ ���������� ���������� ����
      // ��������� �������� ��������
      if (v->timer >= TimeLoadLossCheck) { // �������� ����� �����, ��������� ����
        v->timer = 0; // �������� ��������
        v->dTheta = v->currentThetaI - v->prevTheta; // ���������� ����
        v->prevTheta = v->currentThetaI; // ������������� ����
        v->estimateTheta = v->estimateThetaTmp;
        
        if (v->dTheta < -_PI) {
          v->dTheta = v->dTheta + _2PI;
        } 
        if (v->dTheta > _PI ) {
          v->dTheta = v->dTheta - _2PI;
        } 
        
        // ���������� ���������� ���� � ��������� �����������. ���� ����������� ������ � ��� ���� �� ����������, ����������� ������� ������
        if (fabsf(v->dTheta) < fabsf(v->estimateTheta / 4) && v->estimateTheta != 0){ // deg2rad(360/12-10) 
          v->cntFault = v->cntFault + 1;
        } else {
          if (v->cntFault) {
            v->cntFault = v->cntFault - 1;
          }
        }
        
        v->estimateThetaTmp = 0; // Reset estimate theta
        
        // ���� ������� ������ ������ �������� ��������
        if (v->cntFault >= MAX_FAULT_CNT) {
          v->Fault = 1;        // ������ ������ ������
          v->cntFault = MAX_FAULT_CNT;
        }
      }
          
    }           
}
//========================================================================
void LoadLoss_init(LOADLOSSmodule *v)
{   

}
//========================================================================
