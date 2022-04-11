/* =================================================================================
File name:  aci_pi_reg_Psir.h  (IQ version)
                    
Originator: TRIOL
Описание:   регулятор потокосцепления ротора
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
  
  // задатчик потокосцепления ротора (ослабление поля)
  if (v->FWsecondZoneOnState) {
    if (v->WPsi != 0) {
      fieldWeakening_psirRef = v->E_nom_new /  fabsf(v->WPsi);
    } else {
      fieldWeakening_psirRef = v->Psir_nom_new;
    }
  } else {
    fieldWeakening_psirRef = v->Psir_nom_new;
  }
  
  // ограничение задания потокосцепления
  if (fieldWeakening_psirRef > v->Psir_nom_new) {                          // если больше ном. потока
    fieldWeakening_psirRef = v->Psir_nom_new;
  } else if (fieldWeakening_psirRef < (0.1f * v->Psir_nom_new)) {  // если меньше 10% ном. потока
    fieldWeakening_psirRef = (0.1f * v->Psir_nom_new);
  }

  // Если напряжение ЗПТ ниже номинального напряжения двигателя - понижаем потокосцепление
  if(v->Udcveryslow < v->fieldWeakening_Usmax) {
    fieldWeakening_psirRef = (fieldWeakening_psirRef * v->Udcveryslow) / v->fieldWeakening_Usmax;
  }

  // Задание регулятора
  v->pi_psir_Ref = fieldWeakening_psirRef;
  
  // Ограничение регулятора
  if (v->FlagUsQmax) {
    pi_psir_OutMaxPrefilt = 0;
  } else {
    pi_psir_OutMaxPrefilt = v->Idn;
  }
  // Подфильтруем ограничение
  v->pi_psir_OutMax = (v->K1filtVolWeak * v->pi_psir_OutMax) + (v->K2filtVolWeak * pi_psir_OutMaxPrefilt);
  v->pi_psir_OutMin = -v->pi_psir_OutMax;
  
  // Регулятор потокосцепления ротора
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

// Сброс блока
void AciPiRegPsirReset(ACIPIREGPSIR_handle v){
	v->pi_psir_Fdb             = 0;  // Input: Обратная связь регулятора потокосцепления
	v->WPsi                    = 0;  // Input: Частота поля
	v->Udcveryslow             = (1.4142135623730f);  // Input: Текущее напряжение ЗПТ (фильтрованное)
	v->FWsecondZoneOnState     = 0;  // Input: Флаг перехода во вторую зону
	v->FlagUsQmax              = 0;  // Input: Флаг наличия ограничения UsQ_MAXIM
	v->pi_psir_Ref             = 0;  // Var: Задание регулятора потокосцепления
	v->pi_psir_OutMax          = v->Idn;  // Var: Максимальное ограничение выхода регулятора
	v->pi_psir_OutMin          = -v->Idn;  // Var: Минимальное ограничение выхода регулятора
	v->pi_psir_Ui              = 0;  // Var: Интегральная составляющая регулятора

	v->pi_psir_Out             = 0;  // Output: выход регулятора потокосцепления ротора
}
