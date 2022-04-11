/* =================================================================================
File name:  STFilter.h  (IQ version)
                    
Originator: TRIOL
Описание: Фильтр Коваленкова/Шипулина (ФНЧ не вносящий задержек)
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 18-12-2021 Version 1.00
------------------------------------------------------------------------------*/
#include "Global_Include.h"
#include "type.h"
#include "STFilter.h"



void STFilterCalc(STFILTR_handle s){
	f32  Tmp, Cosine, Sine;
	f32  AlphaCorr, BetaCorr;
	f32  AlphaErr, BetaErr;
	f32  Alphadt, Betadt;

	// Обратное преобразование Park тока, dq to alpha-beta
	Tmp = (s->K1 * s->We);
        Sine = arm_sin_f32(Tmp);
        Cosine = arm_cos_f32(Tmp);
//        Sine = sinf(Tmp);
//        Cosine = cosf(Tmp);
        
	AlphaCorr = (s->AlphaOut * Cosine) - (s->BetaOut * Sine);
	BetaCorr  = (s->AlphaOut * Sine)   + (s->BetaOut * Cosine);

	// STF фильтр
	AlphaErr = s->AlphaIn - AlphaCorr;
	BetaErr  = s->BetaIn  - BetaCorr;
	Alphadt = (AlphaErr * s->K2) - (BetaCorr *  (s->K3 * s->We));
	Betadt  = (BetaErr *  s->K2) + (AlphaCorr * (s->K3 * s->We));
	s->AlphaOut = s->AlphaOut + Alphadt;
	s->BetaOut  = s->BetaOut  + Betadt;
}

// Сброс блока
void STFilterReset(STFILTR_handle s){
	s->AlphaIn  = 0;    // Input: Вход фильтра Alfa
	s->BetaIn   = 0;    // Input: Вход фильтра Beta
	s->We       = 0;    // Input: Текущий угол

	s->K1       = 0;    // Init: Коеффициент фильтра 1
	s->K2       = 0;    // Init: Коеффициент фильтра 2
	s->K3       = 0;    // Init: Коеффициент фильтра 3

	s->AlphaOut = 0;    // Output: Выход фильтра Alpha
	s->BetaOut  = 0;    // Output: Выход фильтра Beta
}



