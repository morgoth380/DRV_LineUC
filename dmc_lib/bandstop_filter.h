/* =================================================================================
File name:      PMSM_BANDSTOP_FILTER.H   (IQ version)
                     
Originator:     Triol / Sosnovchik
URL: 			http://www.dsplib.ru/content/notch/notch.html

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PMSMBSF.
====================================================================================
 History:
-------------------------------------------------------------------------------------
 22-12-2011 Version 1.00                                                
------------------------------------------------------------------------------*/

#ifndef __BSF_H__
#define __BSF_H__

typedef struct {  f32  	Xi[3];       	// Массив входных отсчетов
                  f32  	Yi[3];       	// Массив выходных отсчетов фильтра
                  f32  	K1;      		// Коэффициент К1 = -cos(Fr*pi/Fn)
                  f32  	K2;       		// Коэффициент К2 = (1-sin(dF*pi/Fn))/cos(dF*pi/Fn)
                  f32  	Fstop;     		// Центральная частота задержания
                  f32  	Bandwidth; 		// Полоса пропускания
                  f32  	Fsample;		// Частота дискретизации сигнала
                  f32	A1;				// Коэффициент при значении X(i) -   A1 = (1+k2)/2
                  f32	A2;				// Коэффициент при значении X(i-1) - A2 = A1*2*K1
                  f32	A3;				// Коэффициент при значении X(i-2) - A3 = A1
                  f32	B1;				// Коэффициент при значении Y(i-1) - B1 = A1*K1*(1+K2)
                  f32	B2;  			// Коэффициент при значении Y(i-2) - B2 = A1*K2

                  f32	In;				// Входной отсчет
                  f32	Out;			// Выходной отсчет

                  u16   Enable;			// Активация работы фильтра

                 void  	(*calc)();   // Pointer to calculation function
                 void  	(*init)();   // Pointer to reset function
                 } BSF;

typedef BSF *BSFD_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PMSMBSF object.
-----------------------------------------------------------------------------*/                     
#define BSF_DEFAULTS {          0,0,0,0,0,0,\
				0,0,0,0,0,0,\
				0,0,0,0,0,0,\
				0,\
                                (void (*)(u32))BandStopFilter_calc, \
                                (void (*)(u32))BandStopFilter_init }

/*------------------------------------------------------------------------------
Prototypes for the functions in PMSM_BSF.C
------------------------------------------------------------------------------*/
void BandStopFilter_calc(BSFD_handle);
void BandStopFilter_init(BSFD_handle);

extern BSF	reject1;
extern BSF	reject2;
#endif

