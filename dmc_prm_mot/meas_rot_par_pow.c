/* =================================================================================
File name:      MEASROTPARPOW.H  (float)                   
                    
Originator:	Triol

Description: 
This file contains type definitions, constants and initializers for
the ramp generation functions contained in MEASROTPARPOW.C
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 11-06-2011 Version 1.00
 04-11-2020 Version 2.00  PAN (float)                                                
------------------------------------------------------------------------------*/
#include "Global_Include.h"
#include "meas_rot_par_pow.h"
#include "DRIVE_CONST.h"
#include "Pjk_const.h"
#include "GlobalVar.h"
#include "Filter.h"


extern DRIVECONST  *dPnt;

float UsU1, UsU2, UsU3 = 0;
s16 Pact_s16, Pact1_s16;

void measrotparpow_calc(MEASROTPARPOW *v)
{
    float  Tmp1;
    static u16 PeriodPr = 0;

    if (v->Period > PeriodPr) {
        Tmp1 = v->PwrSum / (float)v->Counter;
        v->Pact = f32_Filtr(v->Pact, Tmp1, 0.125f);

        Pact_s16 = (s16)(v->Pact * dPnt->VoltBASE * dPnt->CurrBASE * 10.0f);

        Tmp1 = v->Ifaz * v->Ifaz;
        v->Req = v->Pact / Tmp1;
        v->Zeq = v->Uline / v->Ifaz;

        arm_sqrt_f32((v->Zeq * v->Zeq) - (v->Req * v->Req), &Tmp1);
        Tmp1 = Tmp1 / v->Wout;
        Tmp1 = v->K1 * Tmp1;

        v->sLr  = f32_Filtr(v->sLr, Tmp1, 0.125f);

        Tmp1   = (v->K2 * v->Req) - dPnt->Rs;

        v->Rr  = f32_Filtr(v->Rr, Tmp1, 0.125f);

        v->PwrSum  = 0;
        v->Counter = 0;
    }
// Задержка напряжения на 2 периода ШИМ
    UsU3 = UsU2;
    UsU2 = UsU1;
    UsU1 = GlobalM4.ValUu.FlashVal;
    v->PwrSum+= UsU3 * dPnt->IsU;	// Расчетное напряжение
      
    v->Counter++ ;
    PeriodPr = v->Period;
}

