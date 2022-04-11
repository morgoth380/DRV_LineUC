/* =================================================================================
File name:      ACI_PI_REG_CUR_CONST.H                   
                    
Originator:     Triol 
 
Developer:      Sikalov Y.Y. 

Description:    The PI current controller adjuction

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 1.00
 02-04-2011 Version 1.10: New file name (late "PI_REG_CUR_CONST.H")  
 05-10-2020 Version 2.0: float                                                 
------------------------------------------------------------------------------*/
#ifndef __PIREGCURCONST_H__
#define __PIREGCURCONST_H__

#include "type.h"

typedef struct {  float  Tcl;           // Input: Current loop response time (s) 
                  float  Tpwm;          // Input: Sampling time (s)
                  float  Tdrv;          // Input: Driver response time (s) 
                  float  NumTdly;       // Input: Driver delay coefficient
                  float  Rs;            // Input: Stator resistance (ohm)
                  float  Rr;            // Input: Rotor resistance (ohm)
                  float  Lls;           // Input: Stator leakage inductance (H)
                  float  Lr;            // Input: Rotor inductance (H) 
                  float  Lm;            // Input: Magnetizing inductance (H)
                  float  Ub;            // Input: база напряжения
                  float  Ib;            // Input: база тока
                  float  Kp_manual;     // Значение регулятора тока из МП
                  float  Ki_manual;     // Значение регулятора тока из МП
                  u16	 ManualEnb;     // Режим расчета коэф. регулятора тока
                  float  sigma;         // Output sigma
                  float  Kp;            // Output: Proportional gain 
                  float  Ki;            // Output: Integral gain 
                  float  K1;            // Output: Transfer function coefficient 
                  float  K2;            // Output: Transfer function coefficient
                  float  K3;            // Output: Transfer function coefficient 
                  void  (*calc)();  // Pointer to calculation functiontion
                } PIREGCURCONST;

typedef PIREGCURCONST * PIREGCURCONST_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIREGCONST object.
-----------------------------------------------------------------------------*/                     
#define PIREGCURCONST_DEFAULTS { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
                          (void (*)(u32))pi_reg_cur_const}

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_PI_REG_CUR_CONST.C
------------------------------------------------------------------------------*/
void pi_reg_cur_const(PIREGCURCONST_handle);

#endif // __PIREGCURCONST_H__
