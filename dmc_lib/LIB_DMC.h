#ifndef _DMCLIB_H_
#define _DMCLIB_H_

#include "park.h"           // Include header for the PARK object 
#include "ipark.h"          // Include header for the IPARK object 
#include "clarke.h"         // Include header for the CLARKE object
#include "iclarke.h"
#include "svgen_dq.h"       // Include header for the SVGENDQ object
#include "deadtime_comp.h"
#include "pi_reg_spd.h"
#include "speed_lim.h"
#include "soft_cur_down.h"
#include "pi_reg_cur.h"
#include "linear_ramp.h"
#include "loadContyChk.h" 
#include "aci_pi_reg_psir.h"
#include "UdcFiltr.h"
#include "pmsm_torq_comp.h"
#include "minCurrCtrl.h"
#include "STFilter.h"
#include "pmsm_antirollback.h"

#include "DRIVE_CONST.h"
#include "DRIVE_WORDS.h"



#ifdef _PROJECT_FOR_STM32_
    #undef  LIBDMC_GLOBAL
    #ifdef  DEFINE_LIBDMC_GLOBAL
        #define LIBDMC_GLOBAL
    #else
        #define LIBDMC_GLOBAL extern
    #endif

    LIBDMC_GLOBAL   PARK            park1;   
    LIBDMC_GLOBAL   IPARK           ipark1;  
    LIBDMC_GLOBAL   IPARK           ipark2;      
    LIBDMC_GLOBAL   CLARKE          clarke1; 
    LIBDMC_GLOBAL   ICLARKE         iclarke1; 
    LIBDMC_GLOBAL   SVGENDQ         svgen_dqv;
    LIBDMC_GLOBAL   DEADTIME_COMP   dtc1; 
    LIBDMC_GLOBAL   STFILTR         DtcSTFilter;     // STFilter для блока dtc
    
    LIBDMC_GLOBAL   PIREGSPD        pi_spd;
    LIBDMC_GLOBAL   PIREGCUR        pi_isd;
    LIBDMC_GLOBAL   PIREGCUR        pi_isq;
    
    LIBDMC_GLOBAL   LOADCONTYCHK_Obj	loadContyChk;    
    
    LIBDMC_GLOBAL   LINEARRAMP      linear_ramp1;
    LIBDMC_GLOBAL   LINEARRAMP      linear_ramp2;
    
    LIBDMC_GLOBAL   SPEEDLIM        spdlim1;
    
    LIBDMC_GLOBAL   SOFTCURDOWN     soft_cur2;

    LIBDMC_GLOBAL   ACIPIREGPSIR    AciPidRegPsir;
    LIBDMC_GLOBAL   UDCFILTR        UdcFiltr;        // Фильтр Udc    
        
    LIBDMC_GLOBAL   DRIVECONST      DrvConst; 
    LIBDMC_GLOBAL   DRIVEWORDS      DrvWords;

    LIBDMC_GLOBAL   DRIVECONST      *dPnt; 

#else
    PARK            park1;           
    IPARK           ipark1;          
    IPARK           ipark2;              
    CLARKE          clarke1;
    ICLARKE         iclarke1;         
    SVGENDQ         svgen_dqv;
    SPEED_MEAS_QEP  speed1;
    DEADTIME_COMP   dtc1;
    //QEP             qep1;
    PIREGCUR        pi_isd;
    PIREGCUR        pi_isq;
    PIREGSPD        pi_spd;
    POSREG_Obj      pos_reg;
    SPEEDLIM        spdlim1;
    CALCVAR         calcvar1;
    ENCODER         encoder1;
    ENCOSPDEST      encoEst1;
    SOFTCURDOWN     soft_cur2;
    LINEARRAMP      linear_ramp1;
    LINEARRAMP      linear_ramp2;

    DRIVECONST      DrvConst;
    DRIVEWORDS      DrvWords;

    DRIVECONST      *dPnt;

#endif 




#endif

