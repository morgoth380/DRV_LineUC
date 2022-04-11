#ifndef __GLOBALINCLUDE_H__
#define __GLOBALINCLUDE_H__

#include "type.h"

//****************************************************************************
#include "..\LibMicroSch\AT_24_prog_RAM.h"

#ifdef  DEFINE_CommonMicro_GLOBAL
    AllPrm_Type *AllPrmPnt;
#endif
extern AllPrm_Type  *AllPrmPnt;

#ifdef  DEFINE_CommonMicro_GLOBAL
    #include "..\LibMicroSch\AT_24_prog_Mem.h"
#endif
//****************************************************************************

#ifdef _PROJECT_FOR_STM32_
#define in_line inline
#else 
#define in_line
#endif // _PROJECT_FOR_STM32_

#define SZ_DIV   1
//#warning ЗДЕСЬ ЗАДАЕМ ВЕРСИИ ПО
#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)
    #define VerProg        16801
    #define SubVerProg     1
    #define VerProgTxt     "168.01.1"
    #define  VENDOR_NAME    "TRIOL"
    #define  PRODUCT_CODE   "AT24"
    #define  MAJOR_MINOR_REV "168.01.1"
#elif defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)
    #define VerProg        16851
    #define SubVerProg     3
    #define VerProgTxt     "168.51.3"
    #define  VENDOR_NAME    "TRIOL"
    #define  PRODUCT_CODE   "AT24"
    #define  MAJOR_MINOR_REV "168.51.3"
#endif

#include "MicroDef.h"

#include "MakeOpis.h"
#include "CommonMicro.h"

#ifdef _PROJECT_FOR_STM32_
#include <math.h>
#include <arm_math.h>
#endif // _PROJECT_FOR_STM32_

#include "MakeOpis_at24.h"
#include "nonVolatile.h"
#include "ModbusSlave.h"
#include "GuiMicro.h"
#include "LoadVar.h"

#include "FileRdWr.h"

#include "string.h"

#endif
