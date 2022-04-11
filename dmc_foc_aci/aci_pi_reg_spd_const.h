/* =================================================================================
File name:      ACI_PI_REG_SPD_CONST.H                   
                    
Originator:     Triol 
 
Developer:      Sikalov Y.Y. 

Description: The PI speed controller adjuction

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 11-11-2011 Version 2.00
 02-04-2011 Version 2.10: New file name (late "PI_REG_SPD_CONST.H")     
 02-10-2020 Version 3.0: float (PAN)                                   
------------------------------------------------------------------------------*/

#ifndef __PIREGSPDCONST_H__
#define __PIREGSPDCONST_H__

#include "type.h"

typedef struct {  float  Jr;            // Input: Moment of inertia (kg*m2) 
                  float  Lr;            // Input: Rotor inductance (H) 
                  float  Lm;            // Input: Magnetizing inductance (H) 
                  float  Zp;            // Input: число пар полюсов
                  float  Tspd;          // Input: период обсчета контура скорости (частота ШИМ), с
                  float  Tsl;           // Input: Speed loop response time (s) 
                  float  Tcl;           // Input: Current loop response time (s) 
                  float  Imax;          // Input: Максимальный ток, действующее значение, А
                  float  PrNom;         // Input: Номинальный поток ротора, Вб
                  float  fb;            // Input: база частоты
                  float  Ib;            // Input: база тока
                  float  Fc_spd;        // Input: Speed feedback Cut-off frequence 
                  float  Force;         // Input: коэффициент усиления коэффициентов
                  float  Kp_manual;     // Input: Proportional gain of speed regulator (manual)
                  float  Ki_manual;     // Input: Integral gain of speed regulator  (manual)
                  u16    ManualEnb;     // Input: Ручная настройка регулятора скорости
                  float  Kp;            // Variable: Proportional gain of speed regulator
                  float  Ki;            // Variable: Integral gain of speed regulator
                  float  K1;            // Output: коэффициент входного фильтра по заданию скорости
                  float  K2;            // Output: коэффициент входного фильтра по заданию скорости
                  float  K3;            // Output: масштабирующий коэффициент
                  float  K4;            // Output: Transfer function coefficient
                  float  K5;            // Output: Transfer function coefficient
                  float  K6;            // Output: Transfer function coefficient
                  float  K7;            // Output: Transfer function coefficient
                  float  OutMax;        // Output: Maximum output
                  float  Kp_norm;       // Output: Proportional gain of speed regulator (_iq)
                  float  Ki_norm;       // Output: Integral gain of speed regulator     (_iq)
                  float  Kp_forced;     // Output: Proportional gain of speed regulator (_iq)
                  float  Ki_forced;     // Output: Integral gain of speed regulator     (_iq)
                  void  (*calc)();  // Pointer to calculation functiontion
               } PIREGSPDCONST;

typedef PIREGSPDCONST * PIREGSPDCONST_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIREGSPDCONST object.
-----------------------------------------------------------------------------*/                     
#define PIREGSPDCONST_DEFAULTS { 0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                          (void (*)(u32))pi_reg_spd_const}

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_PI_REG_SPD_CONST.H
------------------------------------------------------------------------------*/
void pi_reg_spd_const(PIREGSPDCONST_handle);

#endif // __PIREGSPDCONST_H__

