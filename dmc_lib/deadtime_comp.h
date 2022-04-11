/* =================================================================================
File name:       DEADTIME_COMP.H  (IQ version)                  
                    
Originator:	Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the DEADTIME_COMP.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 26-08-2011	Version 1.00                                                   
------------------------------------------------------------------------------*/
#ifndef __DEADTIME_COMP_H__
#define __DEADTIME_COMP_H__
#include "type.h"

typedef struct {  float  Iu;             // Input: phase-u stator current
                  float  Iv;             // Input: phase-v stator current
                  float  Iw;             // Input: phase-w stator current    
                  float  Ta;             // Input and Output: phaza A compensetion potential
                  float  Tb;             // Input and Output: phaza B compensetion potential
                  
                  float  Tc;             // Input and Output: phaza C compensetion potential
                  float  Ud;             // Parameter: DC bus voltage
                  float  Rsw_2;          // Parameter: double key resistance             
                  float  Rsw2Const;      // Init: двойное сопротивление ключа для старого алгоритма (Забито константой)
                  float  dT;             // Variable : compensation coefficient = 2*DeadTime*(sample frequence) +
                                         // + (forward characteristic)/(DC bus voltage)
                  float Udf;             // Parameter: Forvard diode
                  float Utf;             // Input: прямое падение напр. на IGBT, В
                  float UdfConst;        // Init: прямое падение напр. на обр. диоде IGBT, В для старого алгоритма (Забито константой)
                  float Ksw;             // Variable : Coefficient of drop voltage = 2*(key resistance)/(DC bus voltage)
                  float dI;              // Parameter: to fight the sticky current
                  
                  float ThetaCorrCoeff;  // Input
                  float DtcRelative;     // Init:
                  float Fpwm;
                  u16   dtcMode;         // Input: dtc mode
                  float We;
                  
                  f32   CurrBASE;
                  f32   VoltBASE;
                  f32   FreqBASE;
                  
                  void  (*calc)();       // Pointer to calculation function                  
                  void  (*calcLm)();     // Pointer to calculation function
                  void  (*calcRs)();     // Pointer to calculation function
                  void  (*calcLsPMSM)(); // Pointer to calculation function
                  void  (*reset)();      // Pointer to calculation function                  
                  void  (*init)();       // Pointer to calculation function
                  } DEADTIME_COMP;            

typedef DEADTIME_COMP *DEADTIME_COMP_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the DEADTIME_COMP object.
-----------------------------------------------------------------------------*/                     
//#define DEADTIME_COMP_DEFAULTS { 0,0,0,0,0, \
//                                 0,0,0,0,0, \
//                                 0,0,0,0,0, \
//                                 0,0,0,0,0, \
//                                 0,0,0, \
//                                 (void (*)(u32))deadtime_comp_calc,\
//                                 (void (*)(u32))deadtime_comp_calc,\
//                                 (void (*)(u32))deadtime_comp_calc,\
//                                 (void (*)(u32))deadtime_comp_calc,\
//                                 (void (*)(u32))deadtime_comp_reset,\
//                                 (void (*)(u32))deadtime_comp_init}



#define DEADTIME_COMP_DEFAULTS { 0,0,0,0,0, \
                                 0,0,0,0,0, \
                                 0,0,0,0,0, \
                                 0,0,0,0,0, \
                                 0,0,0, \
                                 (void (*)(u32))deadtime_comp_calc,\
                                 (void (*)(u32))deadtime_comp_Lm_calc,\
                                 (void (*)(u32))deadtime_comp_Rs_calc,\
                                 (void (*)(u32))deadtime_comp_Ls_PMSM_calc,\
                                 (void (*)(u32))deadtime_comp_reset,       \
                                 (void (*)(u32))deadtime_comp_init}

/*------------------------------------------------------------------------------
Prototypes for the functions in DEADTIME_COMP.C
------------------------------------------------------------------------------*/
void deadtime_comp_calc(DEADTIME_COMP_handle);
void deadtime_comp_Lm_calc(DEADTIME_COMP_handle);
void deadtime_comp_Rs_calc(DEADTIME_COMP_handle);
void deadtime_comp_Ls_PMSM_calc(DEADTIME_COMP_handle);
void deadtime_comp_reset(DEADTIME_COMP_handle);
void deadtime_comp_init(DEADTIME_COMP_handle);

#endif // __DEADTIME_COMP_H__
