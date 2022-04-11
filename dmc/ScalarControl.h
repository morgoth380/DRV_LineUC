#ifndef __SCALARCONTROL_H__
#define __SCALARCONTROL_H__

#include "type.h"

typedef struct  { float  Udc;         // Input: DC-bus voltage (pu) 
                  float  Uscl;        // Input: Output voltage (pu)
                  float  Fscl;        // Input: Output frequence (pu)
                  float  Uout;        // Output: Real Output voltage (pu)
                  u16    Period;      // Output: Period
                  u16    Ta;          // Output:
                  u16    Tb;          // Output:
                  u16    Tc;          // Output:
                  u16    SvgenMode;   // Output:
                  float  StepAngle;   // Parameter: Step Angle (pu)
                  float  Ds;          // Variable: Amplitude (pu) 
                  float  Angle;       // Variable: Field angle (pu) 
                  float  sinT;        // Variable: cosine (pu)
                  float  cosT;        // Variable: sine (pu)
                  void   (*calc)(); // Pointer to calculation function
                } SCALARCONTROL;
                                                                                                                                                                                                                                                                                                                                                                                                        
typedef SCALARCONTROL *SCALARCONTROL_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the SCALARCONTROL object.
-----------------------------------------------------------------------------*/                     
#define SCALARCONTROL_DEFAULTS { 0, \
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
                                 0, \
                                 (void (*)(u32))ScalarControlCalc \
                                }
                              
/*------------------------------------------------------------------------------
Prototypes for the functions in SCALARCONTROL.C
------------------------------------------------------------------------------*/
void ScalarControlCalc(SCALARCONTROL_handle);

#endif //__SCALARCONTROL_H__
