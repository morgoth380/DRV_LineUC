/* =================================================================================
File name:       PI_REG_CUR.H  (float version)                    
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PI_REG_CUR.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-11-2011 Version 1.00
 05-10-2020 Version 2.00 : float (PAN)
------------------------------------------------------------------------------*/
#ifndef __PIREGCUR_H__
#define __PIREGCUR_H__

#include "type.h"

typedef struct {  float  Ref;             // Input: Reference input 
                  float  Fdb;             // Input: Feedback input 
                  float  Err;             // Variable: Error
                  float  ErrT;            // Variable: Error Transfer
                  float  ErrP;            // Variable: Error Previous
                  float  Up;              // Variable: Proportional output 
                  float  Ui;              // Variable: Integral output 
                  float  OutPreSat;       // Variable: Pre-saturated output
                  float  Kp;              // Parameter: Proportional gain
                  float  Ki;              // Parameter: Integral gain
                  float  K1;              // Parameter: Transfer function coefficient
                  float  K2;              // Parameter: Transfer function coefficient
                  float  K3;              // Parameter: Transfer function coefficient
                  float  OutMax;          // Parameter: Maximum output 
                  float  OutMin;          // Parameter: Minimum output
                  float  Out;             // Output: PI output 
                  u16    FreezeUi;        // Input: Freeze integral part
                  void  (*calc)();      // Pointer to calculation function
                  void  (*reset)();     // Pointer to reset function
                 } PIREGCUR;          

typedef PIREGCUR *PIREGCUR_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIREGCUR object.
-----------------------------------------------------------------------------*/                     
#define PIREGCUR_DEFAULTS {0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           1.3f,  \
                           0.02f, \
                           0.0f,  \
                           0.0f,  \
                           0.0f,  \
                           1.0f,  \
                           -1.0f, \
                           0.0f,  \
                           0,     \
                          (void (*)(u32))pi_reg_cur_calc,\
                          (void (*)(u32))pi_reg_cur_reset }

/*------------------------------------------------------------------------------
Prototypes for the functions in PI_REG_CUR.C
------------------------------------------------------------------------------*/
void pi_reg_cur_calc(PIREGCUR_handle);
void pi_reg_cur_reset(PIREGCUR_handle);

#endif // __PIREGCUR_H__

