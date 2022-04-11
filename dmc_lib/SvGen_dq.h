#ifndef __SVGEN_DQ_H__
#define __SVGEN_DQ_H__

#include "type.h"

typedef struct  { float  Ualpha;        // Input: reference alpha-axis phase voltage 
                  float  Ubeta;         // Input: reference beta-axis phase voltage 
                  float  Ta;            // Output: reference phase-a switching function     
                  float  Tb;            // Output: reference phase-b switching function 
                  float  Tc;            // Output: reference phase-c switching function

                  float  Ta_60;         // Output: reference phase-a switching function     
                  float  Tb_60;         // Output: reference phase-b switching function 
                  float  Tc_60;         // Output: reference phase-c switching function
                  
                  u16    Sector;        // Текущий сектор формирования ШИМа
                  void (*calc)();       // Pointer to calculation function
                  void (*dpwm60)();       // Pointer to calculation function                  
                } SVGENDQ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
typedef SVGENDQ *SVGENDQ_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the SVGENDQ object.
-----------------------------------------------------------------------------*/                     
#define SVGENDQ_DEFAULTS { 0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           (void (*)(u32))SvGenDqCalc, \
                           (void (*)(u32))DPMW60_calc,\
                          }

/*------------------------------------------------------------------------------
Prototypes for the functions in SVGEN_DQ.C
------------------------------------------------------------------------------*/
void SvGenDqCalc(SVGENDQ_handle);
void DPMW60_calc(SVGENDQ_handle);

#endif // __SVGEN_DQ_H__