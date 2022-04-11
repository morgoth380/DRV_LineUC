#ifndef Gui_H
#define Gui_H


#include "Global_include.h"

//********************************************************
typedef u16 * (__stdcall *pFuncMicroPC_type)(u16 *, u32);
typedef u16  (__stdcall *pFuncModbus_type)(char *, char, char);//char
typedef u32   (__stdcall *pMicroProg_Init_type)(u32, const MicroProg_type *, char *);
typedef void (__stdcall *voidcb_type)(void);

typedef void (__stdcall *void_char_type)(char*);


void __stdcall Cleanup_STD(void);
void __stdcall IndividualFunc_STD(char *FileName);


extern const OpisFuncMicroProg_type	TblMicroProgFuncOpis[];
extern const int kolvFuncOpis;
//********************************************************
typedef struct{
	double	        *DataPnt	;
	char	    	Name[32]    ;
}Graf_type;
#define graphNum 9
typedef struct{
	Graf_type		Graf[32]    ;
	double	    	timePoint	;        //время между соседними точками
	u32	        	kolvPoint	;        //если kolvPoint = 0, то отображаем в осцилографическом режиме
	u16	        	Mode	    ;        //=0 всё в одном окне, =1 каждый в отдельном окне
	u16	        	NVar	    ;        //количество графиков начиная с нулевого
}GrafsMem_type;
//********************************************************
typedef struct{
	const OpisFuncMicroProg_type	*Pnt_TblMicroProgFuncOpis;    //таблица описателей элементов библиотеки
	u32	                        	kolvFuncOpis;                //количество элементов
	pFuncMicroPC_type	        	Pnt_FuncMicroPC;            //FuncMicrNum	индивидуальный вызов функций библиотеки
	u32	                        	SignatCoreMicro;            //сигнатура библиотеки
	pFuncModbus_type	        	pFuncModbus;                //Modbus
	pMicroProg_Init_type	    	pMicroProg_Init;            //MicroProg_Init_STD. Инициализация. Вызывать при Reset.
	voidcb_type                 	Cleanup;                    //деструктор
	char	                    	NameBlockExe[256];            //
	char	                    	workDirName[256];
	char	                    	ExceptionText[256];            //описание проблемной ситуации
	void_char_type                 	IndividualFunc;                //деструктор
	GrafsMem_type	            	GrafsMem;
}IFaceGUI_type;
//********************************************************
u16 __stdcall ModbusFunc_STD(char *Bff, char len, char NetAdr);//!!!char
u16 * __stdcall FuncMicrNum(u16 *progPnt, u32 ramPnt);
u32 __stdcall MicroProg_Init_STD(u32 ramPnt, const MicroProg_type	*prgPnt, char *pFileFramName);
void Cleanup(void);
u16 swapU16(u16 Val);

u16  __disable_interrupts(void);
void __restore_interrupts(u16 data);
void GUI_AnalizeMaskFail(void);


/*extern*/ u16 StateTestMicroProg;
extern IFaceGUI_type		IFaceGUI;


//********************************************************
#endif	//Gui_H
