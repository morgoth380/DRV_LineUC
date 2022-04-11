/* =================================================================================
File Name:      SCALAR_ORDINAR.H

Description:    Primary system header file for Scalar Control. 
 
Originator:     Triol

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 27-03-2019 Version 1.00
 10-11-2020 Version 2.00 (float)
=================================================================================  */

#ifndef __SCALARORDINAR_H__
#define __SCALARORDINAR_H__

#include "type.h"

typedef struct  { float  Udc;         // Input: DC-bus voltage (pu) 
                  float  Uscl;        // Input: Output voltage (pu)
                  float  Fscl;        // Input: Output frequence (pu)
                  u16    PWMType;     // Input: Select of PWM type
                  float  Uout;        // Output: Real Output voltage (pu)
                  s16    Period;      // Output: Period
                  s16    Ta;          // Output:
                  s16    Tb;          // Output:
                  s16    Tc;          // Output:
                  s16    Sector;      // Output:
                  float  StepAngle;   // Parameter: Step Angle (pu)
                  float  Ds;          // Variable: Amplitude (pu) 
                  float  Angle;       // Variable: Field angle (pu) 
                  float  sinT;        // Variable: cosine (pu)
                  float  cosT;        // Variable: sine (pu)
                  void   (*calc)();  // Pointer to calculation function
                } SCALARORDINAR;
                                                                                                                                                                                                                                                                                                                                                                                                        
typedef SCALARORDINAR *SCALARORDINAR_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the SCALARORDINAR object.
-----------------------------------------------------------------------------*/                     
#define SCALARCONTROL_ORD  { 0, \
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
                             0, \
                             (void (*)(u32))Scalar_ORDINAR }
                              
/*------------------------------------------------------------------------------
Prototypes for the functions in SCALAR_ORDINAR.C
------------------------------------------------------------------------------*/
void Scalar_ORDINAR(SCALARORDINAR_handle);

#endif
