#ifndef __PACKET_H
#define __PACKET_H                

#define UART_BUFFER_SIZE   6
#define UART_TX_SIZE       5
#define LIM_MKAR_ADC       4095

//********************************************************

typedef struct 
#ifdef  _PROJECT_FOR_CCS_	
				__attribute__ ((__packed__))
#endif
{
	unsigned long	DAC1_U		:12;
	unsigned long	DAC2_I		:12;
	unsigned long	AoutShunt	:3;	
		//	unsigned long	SEL_U		:1;
		//	unsigned long	SEL_U1		:1;
		//	unsigned long	SEL_I		:1;
	unsigned long	Ain1Shunt	:2;	
		//	unsigned long	O_10V_IN1	:1;
		//	unsigned long	O_20mA_IN1	:1;
	unsigned long	Ain2Shunt  :2;
		//	unsigned long	O_10V_IN2	:1;
		//	unsigned long	O_20mA_IN2	:1;
	unsigned long	reserve	   :1;
//  unsigned char res[3];
	unsigned char	crc8;
}MkarTxBff_type;
//********************************************************
typedef struct 
#ifdef  _PROJECT_FOR_CCS_	
				__attribute__ ((__packed__))
#endif
{
	unsigned long	ADC_1		:12;
	unsigned long	ADC_2		:12;
	unsigned long	WD_STATUS	:1;
	unsigned long	reserve		:7;
//  unsigned char	res[3];
	unsigned char	crc8;
}MkarRxBff_type;
//********************************************************

unsigned char crc8(unsigned char *src, unsigned char size);

//********************************************************
#endif
