#define  DEFINE_LIBDMC_GLOBAL
#include "LIB_DMC.h"

///-------------------------------------------------------///
///            Обьявление обьектов                        ///
///-------------------------------------------------------///

// Instance a few transform objects
PARK        park1       = PARK_DEFAULTS;
PARK        park2       = PARK_DEFAULTS;
IPARK       ipark1      = IPARK_DEFAULTS;
IPARK       ipark2      = IPARK_DEFAULTS;
CLARKE      clarke1     = CLARKE_DEFAULTS;
ICLARKE     iclarke1    = ICLARKE_DEFAULTS;
SVGENDQ     svgen_dqv   = SVGENDQ_DEFAULTS;

// Instance a regulators
PIREGSPD    pi_spd      = PIREGSPD_DEFAULTS;
PIREGCUR    pi_isd      = PIREGCUR_DEFAULTS;
PIREGCUR    pi_isq      = PIREGCUR_DEFAULTS;

// Линейная рампа поднятия тока возбуждения
LINEARRAMP linear_ramp1 = LINEARRAMP_DEFAULTS;
LINEARRAMP linear_ramp2 = LINEARRAMP_DEFAULTS;

// Instance a dead time compensator calculator
DEADTIME_COMP dtc1 = DEADTIME_COMP_DEFAULTS;
// Блок STFilter для блока dtc
STFILTR DtcSTFilter = STFILTR_DEFAULTS;

// Обьявление вычислителя параметров ВУ
DRIVECONST  DrvConst;
DRIVECONST  *dPnt = &DrvConst;

// Обьявление ограничителя скорости
SPEEDLIM spdlim1 = SPEEDLIM_DEFAULTS;

// Объявление обработчика плавного сброса тока
SOFTCURDOWN  soft_cur2 = SOFTCURDOWN_DEFAULTS;

// Объявление блока проверки целостности нагрузки
LOADCONTYCHK_Obj	loadContyChk = CONTYCHK_DEFS;

// Обьявление статусного слова
DRIVEWORDS DrvWords = DRVWORDS_DEFAULTS;

// Регулятор потокосцепления ротора
ACIPIREGPSIR AciPidRegPsir = ACIPIREGPSIR_DEFAULTS;

// Фильтр ЗПТ
UDCFILTR UdcFiltr = UDCFILTR_DEFAULTS;

//------------------------------------------------------
// PMSM Section
PMSMTORQCOMP_Obj pmsmTORQcomp = PMSMTORQCOMP_DEFAULTS;
// Объявление блока поддержания минимального полного тока
MINCURRCTRL_Obj	minCurrCtrl = MINCURRCTRL_DEFS;
// Объявление блока антиотката для синхронной лебедки.
PMSMANTIROLLBACK_Obj pmsmAntirollBack = PMSMANTIROLLBACK_DEFAULTS;