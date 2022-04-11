/* =================================================================================
File name:       CALC_VAR.H  (float version)                   
                    
Originator: Triol

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the CALC_VAR.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 03-03-2011 Version 1.00           
 05-10-2020 Version 2.00 : float                                                   
------------------------------------------------------------------------------*/

#include "type.h"

typedef struct  { float IRm;           // Input: потокосцепление ротора
                  float Isq;           // Input: проекция тока на ось q вращ. сист. коорд.
                  float WrElec;        // Input: скорость вращения ротора
                  float Me;            // Output: текущий момент двигателя 
                  float Me_f;          // Output: текущий момент двигателя фидьтрованное
                  float P;             // Output: текущая выходная мощность 
                  float K3;            // Parameter: коэффициент пересчета момента     
                  float K4;            // Parameter: глубина фильтрации момента     
                  float Tf_Me;         // Parameter: коэффициент пересчета мощности  
                  void (*calc)();  // Pointer to calculation function
                } CALCVAR;

typedef CALCVAR *CALCVAR_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the CALCVAR object.
-----------------------------------------------------------------------------*/                     
#define CALCVAR_DEFAULTS { 0.0f, \
                           0.0f, \
                           0.0f, \
                           0.0f, \
                           0.0f, \
                           0.0f, \
                           0.0f, \
                           0.0f, \
                           0.0f, \
                          (void (*)(u32))calc_var }

/*------------------------------------------------------------------------------
Prototypes for the functions in CALC_VAR.C
------------------------------------------------------------------------------*/
void calc_var(CALCVAR_handle);
