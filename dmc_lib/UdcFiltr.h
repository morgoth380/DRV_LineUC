/* =================================================================================
File name:  UdcFiltr.h  (IQ version)
                    
Originator: TRIOL
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-10-2021 Version 1.00
------------------------------------------------------------------------------*/
#ifndef _UDCFILTR_H_
#define _UDCFILTR_H_

typedef struct {  f32  UdcIn;           // Input: ���������� ��� ��� ����������

                  f32  K1udcFilt;       // Init: ����������� ������� 1
		  f32  K2udcFilt;       // Init: ����������� ������� 2

                  f32  UdcOut;          // Output: ���������� ��� �������������

                  void  (*calc)();     // Pointer to calculation function
                  void  (*reset)();    // Pointer to reset function
                 } UDCFILTR;

typedef UDCFILTR *UDCFILTR_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the UDCFILTR object.
-----------------------------------------------------------------------------*/                     
#define UDCFILTR_DEFAULTS { 0,        \
                            0,        \
                            0,        \
			    0,        \
                            (void (*)(u32))UdcFiltrCalc, \
                            (void (*)(u32))UdcFiltrReset }

/*------------------------------------------------------------------------------
Prototypes for the functions in UdcFiltr.c
------------------------------------------------------------------------------*/
void UdcFiltrCalc(UDCFILTR_handle);
void UdcFiltrReset(UDCFILTR_handle);

#endif // _UDCFILTR_H_
