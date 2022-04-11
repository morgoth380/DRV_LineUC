#ifndef JOURNAL_H
#define JOURNAL_H

#include "nonvolatile.h"

extern u16 EnaJrnFailWrite, EnaJrnWarnWrite;
extern RecJrn_type RecJrnWarnRd, RecJrnWarnWr, RecJrnFailRd, RecJrnFailWr;
/*============================================================================*/
//Прототипы функций
/*============================================================================*/
void ReadJrn(RecJrn_type *pntRecJrn, u16 index);
void WriteJrn(RecJrn_type *pntRecJrn);
void SetParamJrn( RecJrn_type *pntRecJrn);

void GetIndex(Index_type *pIndex);
void SetIndex(Index_type *pIndex);
void ClrJrn(void);
void FailValueCopy(FailValue_type   *mPnt);

#endif // JOURNAL_H
