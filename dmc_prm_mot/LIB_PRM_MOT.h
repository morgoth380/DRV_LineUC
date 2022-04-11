#ifndef __PRMMOTLIB_H__
#define __PRMMOTLIB_H__

#include "cntr_voltage.h"         
#include "meas_rs.h"    
#include "meas_rot_par_pow.h"    
#include "ramp_cntl_gen.h"
#include "RMS_val_calc.h"    
#include "meas_lm_cur.h"
 
#include "LIB_DMC.h"

typedef struct {
    u32 Init;
    u32 Start; 
    u32 RsCalc1;
    u32 RsCalc2;
    u32 LsCalc1; 
    u32 LsCalc2; 
    u32 LsCalc3; 
    u32 LmCalc1;
    u32 LmCalc2;
} PRMMOTTIME;

typedef struct {
    float LsLineFreq;
    float LsCyclFreq; 
    float LmLineFreq;
    float LmCyclFreq;
} FREQREF;

#ifdef _PROJECT_FOR_STM32_
    #undef  PRMMOTLIB_GLOBAL
    #ifdef  DEFINE_PRMMOTLIB_GLOBAL
        #define PRMMOTLIB_GLOBAL
    #else
        #define PRMMOTLIB_GLOBAL extern
    #endif

    PRMMOTLIB_GLOBAL   RAMPCNTLGEN     rcg1;       
    PRMMOTLIB_GLOBAL   RAMPCNTLGEN     rcg2;       
    PRMMOTLIB_GLOBAL   CNTRVOLTAGE     cntrvolt1;  
    PRMMOTLIB_GLOBAL   MEASRS          measRs1;
    PRMMOTLIB_GLOBAL   RMSVALCALC      rmsval1;
    PRMMOTLIB_GLOBAL   MEASROTPARPOW   measRP2;
    PRMMOTLIB_GLOBAL   PRMMOTTIME      prmTimes1;
    PRMMOTLIB_GLOBAL   FREQREF         FreqRef1;
    PRMMOTLIB_GLOBAL   MEASLMCUR       measLm2;

    PRMMOTLIB_GLOBAL   float           UlineStep;
    
#else
    RAMPCNTLGEN     rcg1;       
    RAMPCNTLGEN     rcg2;       
    CNTRVOLTAGE     cntrvolt1;  
    MEASRS          measRs1;
    MEASROTPARPOW   measRP2;
    RMSVALCALC      rmsval1;
    PRMMOTTIME      prmTimes1;
    FREQREF         FreqRef1;
    MEASLMCUR       measLm2;

    float             UlineStep;

#endif // _PROJECT_FOR_STM32_

#endif

