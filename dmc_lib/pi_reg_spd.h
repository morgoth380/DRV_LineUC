/* =================================================================================
File name:       PI_REG_SPD.H  (float version)                    
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PI_REG_SPD.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 11-11-2011 Version 1.00
 02-10-2020 Version 2.00 :float версия (PAN)
------------------------------------------------------------------------------*/
#ifndef __PIREGSPD_H__
#define __PIREGSPD_H__

#include "type.h"

typedef struct {  float  Ref;             // Input: Reference input 
                  float  Fdb;             // Input: Feedback input 
                  float  Imax;            // Input: Maximum Current (Amplitude value)
                  u16    RefFiltrEnb;     // Input: Enable Reference input filter
                  u16    NewRegEnb;       // Input: Enable New regulator
                  u16    SavePrevUiFlag;   // Input: While flag is set, copy Ui into Ui_prev
                  u16    RestorePrevUiFlag;// Input: Start restore Ui process flag
                  u16    ResetUiPreviuos;  // Input: Reset Ui previous value pi_spd
                  u16    FreezeUi;         // Input: Freeze flag integral part
                  f32    RestorePrevUiTime;// Input: Pretorque restore full time
                  float  Tpwm;            // Tpwm time in sec
                  float  Ref_filtr;       // Variable: Filtred Reference input 
                  float  Err;             // Variable: Error
                  float  ErrP;            // Variable: Previous Error
                  float  ErrT;            // Variable: Transfer Error
                  float  ErrP1;           // Variable: Previous Error (1)
                  float  ErrT1;           // Variable: Transfer Error (1)
                  float  Up;              // Variable: Proportional output 
                  float  Ui;              // Variable: Integral output 
                  float  Ui_prev;         // Variable: Integral output previous step
                  float  OutPreSat;       // Variable: Pre-saturated output
                  float  RestoreTime;     // Variable: Pretorque current restore time
                  float  Kp;              // Parameter: Proportional gain
                  float  Ki;              // Parameter: Integral gain
                  float  K1;              // Parameter: Filtr coefficient
                  float  K2;              // Parameter: Filtr coefficient
                  float  K3;              // Parameter: масштабирующий коэффициент
                  float  K4;              // Parameter: Transfer function coefficient 
                  float  K5;              // Parameter: Transfer function coefficient 
                  float  K6;              // Parameter: Transfer function coefficient 
                  float  K7;              // Parameter: Transfer function coefficient 
                  
                  float  Kp_norm;         // Parameter: Proportional gain
                  float  Ki_norm;         // Parameter: Integral gain
                  float  Kp_forced;       // Parameter: Proportional gain
                  float  Ki_forced;       // Parameter: Integral gain
                  
                  float  OutMax;          // Parameter: Maximum output 
                  float  OutMin;          // Parameter: Minimum output
                  float  UiOutMax;        // Parameter: Integral Maximum output
                  float  UiOutMin;        // Parameter: Integral Minimum output
                  float  Out;             // Output: PI output 
                  
		  u16  SpdOutMaxFix;
		  u16  SpdOutMaxFixCnt;
                  void  (*calc)();      // Pointer to calculation function
                  void  (*reset)();     // Pointer to reset function
                 } PIREGSPD;          

typedef PIREGSPD *PIREGSPD_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIREGCUR object.
-----------------------------------------------------------------------------*/                     
#define PIREGSPD_DEFAULTS { 0,     /* Ref */            \
                            0,     /* Fdb */            \
                            0,     /* Imtz_Ampl */      \
                            0,     /* RefFiltrEnb */    \
                            0,     /* NewRegEnb */      \
                            0,     /* SavePrevUiFlag */   \
                            0,     /* RestorePrevUiFlag */  \
                            0,     /* ResetUiPreviuos */    \
                            0,     /* FreezeUi    */        \
                            0,     /* RestorePrevUiTime */  \
                            0,     /* Tpwm */             \
                            0,     /* Ref_filtr */      \
                            0,     /* Err */            \
                            0,     /* ErrP */           \
                            0,     /* ErrT */           \
                            0,     /* ErrP1 */          \
                            0,     /* ErrT1 */          \
                            0,     /* Up */             \
                            0,     /* Ui */             \
                            0,     /* Ui_prev */        \
                            0,     /* OutPreSat */      \
                            0,     /* RestoreTime */    \
                            1.3f,  /* Kp */             \
                            0.02f, /* Ki */             \
                            0,     /* K1 */             \
                            0,     /* K2 */             \
                            0,     /* K3 */             \
                            0,     /* K4 */             \
                            0,     /* K5 */             \
                            0,     /* K6 */             \
                            0,     /* K7 */             \
                            0,     /* Kp_norm */        \
                            0,     /* Ki_norm */        \
                            0,     /* Kp_forced */      \
                            0,     /* Ki_forced */      \
                            1.0f,  /* OutMax */         \
                            -1.0f, /* OutMin */         \
			    1.0f,  /* UiOutMax */       \
			    -1.0f, /* UiOutMin */       \
                            0,     /* Out */            \
		            0,     /* SpdOutMaxFix */   \
			    0,     /* SpdOutMaxFixCnt*/ \
                            (void (*)(u32))pi_reg_spd_calc, \
                            (void (*)(u32))pi_reg_spd_reset }

/*------------------------------------------------------------------------------
Prototypes for the functions in PI_REG_CUR.C
------------------------------------------------------------------------------*/
void pi_reg_spd_calc(PIREGSPD_handle);
void pi_reg_spd_reset(PIREGSPD_handle);

#endif // __PIREGCUR_H__


