/*===================================================================================
File name:      SOFT_CUR_DOWN.C  (float version)                                    
Originator:     Triol 
Author:         Sikalov Y.  
Description:    Soft current down               
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 24-07-2012 Version 1.00
 23-09-2020 Version 2.00 PAN (Перевел во float)
-------------------------------------------------------------------------------------*/

#include "soft_cur_down.h"
#include "GlobalVar.h"

void soft_cur_down_calc(register SOFTCURDOWN *v)
{
  // Плавный сброс тока начинает работать только по запросу
  if(v->bit.Start_fl){
      if(v->IqRef_out != 0){
          if(v->IqRef_out < 0){
              // Плавный сброс по оси Q
              if(v->IqRef_out >= v->StepQ){
                  v->IqRef_out = 0;
              }
              else{
                  v->IqRef_out -= v->StepQ; // сбрасываем ток с заданным темпом
               }
          }
          else if(v->IqRef_out > 0){
               // Плавный сброс по оси Q
               if(v->IqRef_out <= v->StepQ){
                   v->IqRef_out = 0;
               }
               else{
                   v->IqRef_out -= v->StepQ; // сбрасываем ток с заданным темпом
               }
          }
      }
      else{
          // Плавный сброс по оси D
          if(v->IdRef_out <= v->StepD){
             v->IdRef_out = 0;
          }
          else{
             v->IdRef_out -= v->StepD; // сбрасываем ток с заданным темпом
          }
      }
  }
  else { // Работа в нормальном режиме
      v->IdRef_out = v->IdRef_in;
      v->IqRef_out = v->IqRef_in;
      v->ThetaPsi_out = v->ThetaPsi_in;
      v->StepD = (v->IdRef_in/(s32)(v->Fpwm * 0.5f * v->TimeCurDown));
      v->StepQ = (v->IqRef_in/(s32)(v->Fpwm * 0.5f * v->TimeCurDown));
  }
}

