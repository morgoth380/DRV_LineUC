/* =================================================================================
File name:       PMSM_PI_REG_SPD_CONST.H                   
                    
Originator: Triol 
 
Developer: Sikalov Y.Y. 

Description: The PI speed controller adjuction for PMSM

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-12-2011 Version 1.00
------------------------------------------------------------------------------*/


#ifndef __PMSMPIREGSPDCONST_H__
#define __PMSMPIREGSPDCONST_H__

#include "type.h"

typedef struct {  float         Jr;            // Input: Moment of inertia (kg*m2) 
                  float         Zp;            // Input: число пар полюсов
                  float         Tspd;          // Input: период обсчета контура скорости (частота ШИМ), с
                  float         Tsl;           // Input: Speed loop response time (s) 
                  float         Tcl;           // Input: Current loop response time (s) 
                  float         Imax;          // Input: Максимальный ток, действующее значение, А
                  float         PsiR;          // Input: Flux rotor constant (Wb)
                  float         fb;            // Input: база частоты
                  float         Ib;            // Input: база тока
                  float         Fc_spd;        // Input: Speed feedback Cut-off frequence
                  float         Force;         // Input: коэффициент усиления коэффициентов
                  float         Kp_manual;     // Input: Proportional gain of speed regulator (manual)
                  float         Ki_manual;     // Input: Integral gain of speed regulator  (manual)
                  u16           ManualEnb;     // Input: Ручная настройка регулятора скорости
                  float         Kp;            // Output: Proportional gain of speed regulator
                  float         Ki;            // Output: Integral gain of speed regulator
                  float         K1;            // Output: коэффициент входного фильтра по заданию скорости
                  float         K2;            // Output: коэффициент входного фильтра по заданию скорости
                  float         K3;            // Output: масштабирующий коэффициент
                  float         K4;            // Output: Transfer function coefficient
                  float         K5;            // Output: Transfer function coefficient
                  float         K6;            // Output: Transfer function coefficient
                  float         K7;            // Output: Transfer function coefficient
                  float         OutMax;        // Output: Maximum output
                  float         Kp_norm;       // Output: Proportional gain of speed regulator (_iq)
                  float         Ki_norm;       // Output: Integral gain of speed regulator     (_iq)
                  float         Kp_forced;     // Input: Proportional gain of speed regulator (_iq)
                  float         Ki_forced;     // Input: Integral gain of speed regulator     (_iq)
                  float         Kp_forced_flt; // Input: Proportional gain of speed regulator (forced, float)
                  float         Ki_forced_flt; // Input: Integral gain of speed regulator  (forced, float)
                  float         Kp_forced_iq;  // Input: Proportional gain of speed regulator (forced, float)
                  float         Ki_forced_iq;  // Input: Integral gain of speed regulator  (forced, float)

                  void  (*calc)();  // Pointer to calculation functiontion
               } PMSMPIREGSPDCONST;

typedef PMSMPIREGSPDCONST * PMSMPIREGSPDCONST_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PMSMPIREGSPDCONST object.
-----------------------------------------------------------------------------*/                     
#define PMSMPIREGSPDCONST_DEFAULTS {    0,0,0,0,0,0,0,0,0,0,0,0,0,0, \
                                        0,0,0,0,0,0,0,0,0,0,0,0,0,0, \
                                        0,0,0,0, \
                                        (void (*)(u32))pmsm_pi_reg_spd_const}

/*------------------------------------------------------------------------------
Prototypes for the functions in PMSM_PI_REG_SPD_CONST.H
------------------------------------------------------------------------------*/
void pmsm_pi_reg_spd_const(PMSMPIREGSPDCONST_handle);

#endif

