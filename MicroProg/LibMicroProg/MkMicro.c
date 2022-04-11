#include "Global_include.h"


#define GET_FUNC_CODE               //получили сами функции
#include "AllFuncMicroProg.h"
#undef  GET_FUNC_CODE


#define GET_FUNC_PNT                //получили таблицу указателей на функции
const pFuncMicro_type   TblMicroProgFunc[]={
    #include "AllFuncMicroProg.h"
};
#undef  GET_FUNC_PNT

#ifndef _PROJECT_FOR_STM32_


#endif
