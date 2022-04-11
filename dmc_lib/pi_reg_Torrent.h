/* =================================================================================
File name:       PI_REG_Torrent.H  (float version)                    
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PI_REG_CUR.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 06-02-2018 Version 1.00
 02-10-2020 Version 2.00 float (PAN)
------------------------------------------------------------------------------*/
#ifndef __PIREGTOR_H__
#define __PIREGTOR_H__

#include "type.h"

typedef struct {  float  Ref;           //1 Input: Reference input
                  float  Fdb;           // Input: Feedback input 
                  float  Err;           // Variable: Error
                  float  ErrT;          // Variable: Error Transfer
                  float  ErrP;          // Variable: Error Previous
                  float  Up;            // Variable: Proportional output 
                  float  Ui;            // Variable: Integral output 
                  float  OutPreSat;     // Variable: Pre-saturated output
                  float  Kp;            // Parameter: Proportional gain
                  float  Ki;            //10 Parameter: Integral gain
                  float  K1;            // Parameter: Transfer function coefficient
                  float  K2;            // Parameter: Transfer function coefficient
                  float  K3;            // Parameter: Transfer function coefficient
                  float  Filter_Kp;	// Фильтрованное значение Kp
                  float  Filter_Ki;	// Фильтрованное значение Ki
                  u16  SpdSaturate;     // Состояние КС - в насыщении или нет
                  u16  AccelOverrun;    // Флаг превышения текущего ускорения
                  s16  SaturateTimeCnt; // Счетчик - как долго в насыщенни РС
                  u16  SpdSaturateLast; // Состояние КС - в насыщении или нет
                  float  OutMax;        // Parameter: Maximum output 
                  float  OutMin;        // Parameter: Minimum output
                  float  UiOutMax;	// Saturate level for integral part Max
                  float  UiOutMin;	// Saturate level for integral part Min
                  float  Out;           //16 Output: PI output
                  void  (*calc)();   // Pointer to calculation function
                  void  (*reset)();  // Pointer to reset function
                 } PIREGTOR;          

typedef PIREGTOR *PIREGTOR_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIREGCUR object.  Wrz
-----------------------------------------------------------------------------*/                     
#define PIREGTOR_DEFAULTS {0, 		/*	Ref		*/\
                           0, 		/*	Fdb		*/\
                           0, 		/*	Err		*/\
                           0,		/*	ErrT	        */\
                           0, 		/*	ErrP	        */\
                           0, 		/*	Up		*/\
                           0, 		/*	Ui		*/\
                           0, 		/*	OutPreSat       */\
                           1.3f,        /*	Kp		*/\
                           0.02f,       /*	Ki		*/\
                           0, 		/*	K1		*/\
                           0, 		/*	K2		*/\
                           0, 		/*	K3		*/\
                           0, 		/*	Filter_Kp	*/\
                           0, 		/*	Filter_Ki	*/\
                           0, 		/*	SpdSaturate	*/\
			   0, 		/*	AccelOverrun	*/\
			   0, 		/*	SaturateTimeCnt	*/\
                           0, 		/*      SpdSaturateLast */\
                           0.1f,        /*	OutMax		*/\
                           -0.1f,       /*      OutMin		*/\
                           0, 		/*	UiOutMax	*/\
                           0, 		/*	UiOutMin	*/\
                           0, 		/*	Out		*/\
                          (void (*)(u32))pi_reg_tor_calc,\
                          (void (*)(u32))pi_reg_tor_reset }

/*------------------------------------------------------------------------------
Prototypes for the functions in PI_REG_TORRENT.C
------------------------------------------------------------------------------*/
void pi_reg_tor_calc(PIREGTOR_handle);
void pi_reg_tor_reset(PIREGTOR_handle);

#endif // __PIREGTOR_H__

