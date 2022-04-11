/* =================================================================================
File Name:      CONST.H
Description:    Const defines
Developer:      PAN 
Ts Idea:        29-09-2020 
Originator:     Triol Corporation
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 29-09-2020 Version 1.00
===================================================================================== */

#ifndef __PJK_CONST_H__
#define __PJK_CONST_H__

#define _PI             3.14159265358f
#define _2PI            6.283185307179586f   // (PI * 2.0f)
#define _DEG_TO_RAD      (_PI / 180.0f)
#define _RAD_TO_DEG      (180.0f / _PI)

#define _1PI_DIV6       0.523598775598298f  //(_PI) / 6.0f
#define _5PI_DIV6       2.617993877991494f  //(5.0f * _PI) / 6.0f

#define _SQRT_2         1.414213562373095f
#define _1DIV_SQRT_2    0.707106781186547f
#define _SQRT_2_DIV_3   0.816496580927726f   // sqrt(2.0/3.0)
#define _SQRT3_2        0.866025403784438f

#define _SQRT_3         1.73205080756887f
#define _SQRT_3_DIV_2   1.22474487139158f    // sqrt(3/2)
#define _1DIV_SQRT_3    0.57735026918962f
#define _2_SQRT3        1.15470053837925f 

#define _2SQRT2_SQRT3   1.6329931618554520654648560498039f
#define _SQRT3_2SQRT2   0.61237243569579f   // sqrt(3)/(2*sqrt(2))

#define _01_Hz          0.00125f             // 0.1f/80.0f
#define _1_Hz           0.0125f              // 1.0f/80.0f
#define _2_HZ           0.025f               // 2.0f/80.0f
#define _3_HZ           0.0375f              // 3.0f/80.0f

#define _MINIMAL_FPWM_FREQ      1500         // Минимальная частота ШИМа
#define _FPWM_SAFETY_FACTOR     1.5f         // Коэффициент запаса по максимальной частоте ШИМа



#endif //__PJK_CONST_H__ 
