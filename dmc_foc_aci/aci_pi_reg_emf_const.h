/* =================================================================================
File name:      ACI_PI_REG_EMF_CONST.H                   
                    
Originator:     Triol 
 
Developer:      Sikalov Y.Y. 

Description: The PI field weakening controller adjuction

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 22-11-2011 Version 1.00
 02-04-2011 Version 2.10: New file name (late "PI_REG_EMF_CONST.H")  
------------------------------------------------------------------------------*/

#ifndef __PIREGEMFCONST_H__
#define __PIREGEMFCONST_H__

#include "type.h"

typedef struct {  float  Tcl;           // Input: Current loop response time (s) 
                  float  Temfl;         // Input: EMF loop response time (s) 
                  float  Tpwm;          // Input: Sampling time (s)
                  float  Rr;            // Input: Rotor resistance (ohm)
                  float  Lr;            // Input: Rotor inductance (H) 
                  float  Lm;            // Input: Magnetizing inductance (H)
                  float  Idn;           // Input: Amplitude Nominal motor current (А)
                  float  Wmax;          // Input: Maximum rotor speed (Hz)
                  float  Wnom;          // Input: Nominal rotor speed (Hz)
                  float  Ub;            // Input: Voltage base
                  float  Ib;            // Input: Current base 
                  float  Fb;            // Input: Frequency base
                  float  Kp;            // Output: Proportional gain 
                  float  Ki;            // Output: Integral gain 
                  float  K1;            // Output: Transfer function coefficient 
                  float  K2;            // Output: Transfer function coefficient
                  float  K3;            // Output: Transfer function coefficient 
                  float  OutMax;        // Output: Maximum output 
                  float  OutMin;        // Output: Minimum output 
                  float  InitInteg;     // Output: Integral init
                  float  NumTdly;
                  float  Wnom_Hz;
                  float  Kp_iq;         // Output: Proportional gain of speed regulator (_iq)
                  float  Ki_iq;         // Output: Integral gain of speed regulator     (_iq)
                  void  (*calc)();  // Pointer to calculation functiontion
                } PIREGEMFCONST;

typedef PIREGEMFCONST * PIREGEMFCONST_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIREGEMFCONST object.
-----------------------------------------------------------------------------*/                     
#define PIREGEMFCONST_DEFAULTS { 0,0,0,0,0,0,0,0,0,0,\
	                             0,0,0,0,0,0,0,0,0,0,\
                                 0,0,0,0, \
                          (void (*)(u32))pi_reg_emf_const}

/*------------------------------------------------------------------------------
Prototypes for the functions in ACI_PI_REG_EMF_CONST.C
------------------------------------------------------------------------------*/
void pi_reg_emf_const(PIREGEMFCONST_handle);

#endif // __PIREGEMFCONST_H__

