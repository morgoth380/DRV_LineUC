/* =================================================================================
File name:  aci_pi_reg_Psir.h  (f32 version)
                    
Originator: TRIOL
Описание:   регулятор потокосцепления ротора
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-10-2021 Version 1.00
------------------------------------------------------------------------------*/
#ifndef _ACIPIREGPSIR_H_
#define _ACIPIREGPSIR_H_

typedef struct {
  f32  pi_psir_Fdb;              // Input: Обратная связь регулятора потокосцепления
  f32  WPsi;                     // Input: Частота поля
  f32  Udcveryslow;              // Input: Текущее напряжение ЗПТ (фильтрованное)
  u16  FWsecondZoneOnState;      // Input: Флаг перехода во вторую зону
  u16  FlagUsQmax;               // Input: Флаг наличия ограничения UsQ_MAXIM
  
  f32  Idn;                      // Init: Номинальный ток возбуждения
  f32  E_nom_new;                // Init:
  f32  Psir_nom_new;             // Init:
  f32  fieldWeakening_Usmax;     // Init:
  f32  fieldWeakening_invUsmax;  // Init:
  f32  K1filtVolWeak;            // Init:
  f32  K2filtVolWeak;            // Init:
  f32  pi_psir_Kp;               // Init:
  f32  pi_psir_Ki;               // Init:
  
  f32  pi_psir_Ref;              // Var: Задание регулятора потокосцепления
  f32  pi_psir_OutMax;           // Var: Максимальное ограничение выхода регулятора
  f32  pi_psir_OutMin;           // Var: Минимальное ограничение выхода регулятора
  f32  pi_psir_Ui;               // Var: Интегральная составляющая регулятора
  
  f32  pi_psir_Out;              // Output: выход регулятора потокосцепления ротора
  
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
