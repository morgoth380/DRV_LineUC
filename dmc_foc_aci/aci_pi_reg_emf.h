/* =================================================================================
File name:       PI_REG_EMF.H  (float version)                    
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PI_REG_EMF.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 22-11-2011 Version 1.00
 02-10-2020 Version 1.00 float (PAN)
------------------------------------------------------------------------------*/
#ifndef __PIREGEMF_H__
#define __PIREGEMF_H__

#include "type.h"

typedef struct {  float  Ref;             // Input: Reference input 
                  float  Fdb;             // Input: Feedback input 
                  float  WrR;             // Input: Rotor speed Reference
                  float  Wr;              // Input: Rotor speed
                  u16    PrUp;            // Input: Rotor Flux is grow
                  float  Err;             // Variable: Error
                  float  ErrT;            // Variable: Error Transfer
                  float  ErrP;            // Variable: Error Previous
                  float  Up;              // Variable: Proportional output 
                  float  Ui;              // Variable: Integral output 
                  float  OutPreSat;       // Variable: Pre-saturated output
                  float  WrR_f;           // Variable: Rotor speed Reference (filtred) 
                  float  Wr_f;            // Variable: Rotor speed (filtred)             
                  float  Kp;              // Parameter: Proportional gain
                  float  Ki;              // Parameter: Integral gain
                  float  K1;              // Parameter: Transfer function coefficient
                  float  K2;              // Parameter: Transfer function coefficient
                  float  K3;              // Parameter: Transfer function coefficient
                  float  OutMax;          // Parameter: Maximum output 
                  float  OutMin;          // Parameter: Minimum output
                  float  InitInteg;       // Parameter: Integral init
                  float  Out;             // Output: Regulator output 
                  float  OutPrt;          // Output: Protected Regulator output 
                  void  (*calc)();      // Pointer to calculation function
                  void  (*reset)();     // Pointer to reset function
                 } PIREGEMF;          

typedef PIREGEMF *PIREGEMF_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIREGEMF object.
-----------------------------------------------------------------------------*/                     
#define PIREGEMF_DEFAULTS {0, \
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
                           1.3f, \
                           0.02f, \
                           0, \
                           0, \
                           0, \
                           1.0f, \
                           -1.0f, \
                           0, \
                           0, \
                           0, \
                          (void (*)(u32))pi_reg_emf_calc,\
                          (void (*)(u32))pi_reg_emf_reset }

/*------------------------------------------------------------------------------
Prototypes for the functions in PI_REG_EMF.C
------------------------------------------------------------------------------*/
void pi_reg_emf_calc(PIREGEMF_handle);
void pi_reg_emf_reset(PIREGEMF_handle);

#endif // __PIREGEMF_H__

