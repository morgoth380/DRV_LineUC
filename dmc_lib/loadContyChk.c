/** =================================================================================
* \file		: loadContyChk.c
*
* \brief	: Модуль проверки целостности нагрузки
* \details	: включает функции инициализации и  расчета
*
* \autor	: Константин Гончаров
*
* \date		: 02.10.2016 \version	: 1.0
*
* \todo		:
*
* \copyright: (C)TRIOLCORP, 2016
*
=====================================================================================*/

#include "Global_Include.h"
#include "loadContyChk.h"
#include "Pjk_const.h"

void CONTYCHK_init()
{
}

void CONTYCHK_calc(LOADCONTYCHK_handle v)
{
  f32   Ts, Tfiltr;
  
  if(v->bitStart){
    switch(v->chkState){
    case 1:
      v->Error    = 0;
      v->IqRefOut = 0;
      v->IdRefOut = v->Ichk;
      v->timer	  = (u16)(v->timerTestTime*v->Fpwm);		// Время контроля нагрузки/2 в тиках ШИМА (0.5 set outside)
      v->filtrIu  = v->Ichk*0.5f;                       // Start current - half of reference
      v->filtrIv  = v->Ichk*0.5f;
      v->filtrIw  = v->Ichk*0.5f;
      Tfiltr      = v->timerTestTime * 0.25f; // Filtration time, half of observe tim
      Ts = 1/v->Fpwm;      
      v->K1       = Ts / (2*Tfiltr + Ts);
      v->K2       = (2*Tfiltr)/ (2*Tfiltr + Ts);
      v->chkState++;
      break;
    case 2:
      if(v->timer){
        v->timer--;
        v->filtrIu = v->K1 * v->Iu + v->filtrIu * v->K2;
        v->filtrIv = v->K1 * v->Iv + v->filtrIv * v->K2;
        v->filtrIw = v->K1 * v->Iw + v->filtrIw * v->K2;        
      }
      else{
        if((((v->filtrIu <= v->filtrIv)&&(v->filtrIu <= v->filtrIw)) ? v->filtrIu : ((v->filtrIv <= v->filtrIw) ? v->filtrIv : v->filtrIw)) < v->Imin){ //Iu,Iv,Iw- absolute values
          v->chkState++;
        }
      }
      break;
    case 3:
      v->IqRefOut = v->Ichk;
      v->IdRefOut = v->Ichk;	// Повернули вектор тока на 45 градусов
      v->timer	= (u16)(v->timerTestTime*v->Fpwm*0.9f);
      v->chkState++;
      break;
    case 4:
      if(v->timer){
        v->timer--;
        v->filtrIu = v->K1 * v->Iu + v->filtrIu * v->K2;
        v->filtrIv = v->K1 * v->Iv + v->filtrIv * v->K2;
        v->filtrIw = v->K1 * v->Iw + v->filtrIw * v->K2; 
      }
      else{
        if((((v->filtrIu <= v->filtrIv)&&(v->filtrIu <= v->filtrIw)) ? v->filtrIu : ((v->filtrIv <= v->filtrIw) ? v->filtrIv : v->filtrIw)) < v->Imin){ //Iu,Iv,Iw- absolute values
          v->Error = 1;
        }
      }
      
      break;              
    default:
      v->chkState=1;
    }
  }
  else{
    if(v->chkState !=0 ){v->bitForceSpd = 1;}
    else{v->bitForceSpd = 0;}
    v->timer	= (u16)(v->timerTestTime*v->Fpwm); // Время контроля нагрузки/2 в тиках ШИМА
    v->chkState = 0;
    v->IdRefOut = v->IdRefIn;
    v->IqRefOut = v->IqRefIn;
    v->ThetaOut = v->ThetaIn;
    v->Error = 0;            
  }
}

