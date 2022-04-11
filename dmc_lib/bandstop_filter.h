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

typedef struct {  f32  	Xi[3];       	// ������ ������� ��������
                  f32  	Yi[3];       	// ������ �������� �������� �������
                  f32  	K1;      		// ����������� �1 = -cos(Fr*pi/Fn)
                  f32  	K2;       		// ����������� �2 = (1-sin(dF*pi/Fn))/cos(dF*pi/Fn)
                  f32  	Fstop;     		// ����������� ������� ����������
                  f32  	Bandwidth; 		// ������ �����������
                  f32  	Fsample;		// ������� ������������� �������
                  f32	A1;				// ����������� ��� �������� X(i) -   A1 = (1+k2)/2
                  f32	A2;				// ����������� ��� �������� X(i-1) - A2 = A1*2*K1
                  f32	A3;				// ����������� ��� �������� X(i-2) - A3 = A1
                  f32	B1;				// ����������� ��� �������� Y(i-1) - B1 = A1*K1*(1+K2)
                  f32	B2;  			// ����������� ��� �������� Y(i-2) - B2 = A1*K2

                  f32	In;				// ������� ������
                  f32	Out;			// �������� ������

                  u16   Enable;			// ��������� ������ �������

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

