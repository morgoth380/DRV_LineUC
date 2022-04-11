/* =================================================================================
File name:       PMSM_PI_REG_CUR_CONST.H                   
                    
Originator: Triol 
 
Developer: Sikalov Y.Y. 

Description: The PI current controller adjuction for PMSM 

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-12-2011 Version 1.00
------------------------------------------------------------------------------*/
#ifndef __PMSMPIREGCURCONST_H__
#define __PMSMPIREGCURCONST_H__

#include "type.h"

typedef struct {  float  Tcl;           // Input: Current loop response time (s) 
                  float  Tpwm;          // Input: Sampling time (s)
                  float  Tdrv;          // Input: Driver response time (s) 
                  float  NumTdly;       // Input: Driver delay coefficient
                  float  Rs;            // Input: Stator resistance (ohm)
                  float  Lsd;           // Input: Stator leakage inductance (H)
                  float  Lsq;           // Input: Rotor inductance (H) 
                  float  Ub;            // Input: база напряжения
                  float  Ib;            // Input: база тока
                  float  Kpd;           // Output: Proportional gain (D-axis)
                  float  Kpq;           // Output: Proportional gain (Q-axis)
                  float  Ki;            // Output: Integral gain
                  float  Kp;
                  float  Kp_manual;
                  float  Ki_manual;
                  u16	   ManualEnb;
                  float  K1;            // Output: Transfer function coefficient 
                  float  K2;            // Output: Transfer function coefficient
                  float  K3;            // Output: Transfer function coefficient 
                  void  (*calc)();        // Pointer to calculation functiontion
                } PMSMPIREGCURCONST;

typedef PMSMPIREGCURCONST * PMSMPIREGCURCONST_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIREGCONST object.
-----------------------------------------------------------------------------*/                     
#define PMSMPIREGCURCONST_DEFAULTS { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, \
                          (void (*)(u32))pmsm_pi_reg_cur_const}

/*------------------------------------------------------------------------------
Prototypes for the functions in PMSM_PI_REG_CUR_CONST.C
------------------------------------------------------------------------------*/
void pmsm_pi_reg_cur_const(PMSMPIREGCURCONST_handle);

#endif 


