#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "Gui.h"
#include "DrvUart.h"

#include "logger.h"
#include "type.h"

#include "MainControl_CP24B.h"

//#include "CurVoltMeas.h"

extern  uart_type *const uartPtr[5];
void LogerPwmPrintf(const char *FileName);
int GetFileSizeMy(const TCHAR *fileName);

//**************************************************************************
//**************************************************************************
#define	LOG_BFF_SIZE		(256L*1024L)		// Объем выделенной памяти
#define	LOG_BFF_nPoint		(256L*1024L)		// Объем выделенной памяти
//**************************************************************************

//**************************************************************************
//**************************************************************************

	IFaceGUI_type		IFaceGUI;
	char	        	BffName[256];
	char                *pGlobalWorkDirName;
//**************************************************************************
u16 __stdcall ModbusFunc_STD(char *Bff, char len, char NetAdr)//!!!!char
{
    uartPtr[1]->netAdr = NetAdr; // Для АСУ мне нужен uart1
    return	ModbusFunc(Bff, len, uartPtr[1] );
}
//**************************************************************************
/* Оригинальная версия
u16 * __stdcall FuncMicrNum(u16 *progPnt, u32 ramPnt)
{
	sprintf(BffName, "%s", TblMicroProgFuncOpis[*progPnt].SymbolName);
    return TblMicroProgFunc[*progPnt](progPnt, ramPnt);
}
*/
//**************************************************************************
//                      Только для отладки
//**************************************************************************
//**************************************************************************
/////////#define	FLOAT_PIN_TYPE	        	0
/////////#define	S32_PIN_TYPE	        	1
/////////#define	S16_PIN_TYPE	        	2
/////////#define	BIT_PIN_TYPE	        	3
/////////#define	TXT_PIN_TYPE	        	4
/////////
/////////#define	INPUT_PIN_MODE               (0<<4)
/////////#define	OUTPUT_PIN_MODE              (1<<4)
/////////#define	PASSIVE_PIN_MODE             (2<<4)
/////////#define	INDEPEND_FLAG                (1<<7)

/////////#define MASK_PIN_TYPE 0x7
/////////#define MASK_PIN_MODE (0x3<<3)
//**************************************************************************
//                      Только для отладки
//**************************************************************************
u16 testCheckMem(u16 *progPnt, u32 ramPnt, u32 testRamPnt, u16 data)
{
    const char *ptrTypePin;
    u16  *ptrIn;
    u16 i, j, tmp;
    u32 tmp4;
    u16 sizeofState, sizeofNVState;

    sizeofState   = TblMicroProgFuncOpis[*progPnt].sizeofState;
    sizeofNVState = TblMicroProgFuncOpis[*progPnt].sizeofNVState;

    ptrIn  = progPnt+1;

    if(sizeofState !=0 )
        {
        tmp4 = (testRamPnt + (*ptrIn)/SZ_DIV);
        memset((char*)tmp4, data, sizeofState);       // Скопировать State
        ptrIn++;
        }
    if(sizeofNVState !=0 )
        {
        tmp4 = (testRamPnt + *ptrIn/SZ_DIV);
        memset((char*)tmp4, data, sizeofNVState);       // Скопировать NVState
        ptrIn++;
        }

    i           = TblMicroProgFuncOpis[*progPnt].kolvPin;       // Количество входных переменных
    ptrTypePin  = TblMicroProgFuncOpis[*progPnt].typePin;       // Указатель на таблицу внешних переменных

    for(j=0; j<i; j++)
        {
        tmp = ptrTypePin[j];
        switch(tmp & MASK_PIN_TYPE)
            {
            case FLOAT_PIN_TYPE:
                {
                u32 tmp3;

                tmp3 = ((data<<16) + data);
                save_float_check(testRamPnt, *ptrIn, tmp3);
                }
                break;


        	case S32_PIN_TYPE  :
                {
                u32 tmp3;

                tmp3 = ((data<<16) + data);
                save_s32(testRamPnt, *ptrIn, (s32)tmp3);
                }
                break;

            case S16_PIN_TYPE  :
                save_s16(testRamPnt, *ptrIn, data);
                break;

            case BIT_PIN_TYPE  :
               if( data == 0xFFFF)  setBitMicro(testRamPnt, *ptrIn);
               else                 clrBitMicro(testRamPnt, *ptrIn);
           break;
            }
        ptrIn++;
        }

    tmp = (MPOpis.PntMicroProg->kolvRam_All)/SZ_DIV;
    ptrIn = (u16*)(testRamPnt - MPOpis.PntMicroProg->Sz_bit_Sect/SZ_DIV);

    for(i=0; i < tmp ; i++) // Проверка на чистоту массива
        {
        if(*ptrIn++ != data)
            return 0;
        }

    return 1;
}
//**************************************************************************
//                      Только для отладки
//
//**************************************************************************
void testFunDataCpy(u16 *progPnt, u32 ramPnt, u32 testRamPnt)
{
    const char *ptrTypePin;
    u16  *ptrIn;
    u16 i, j, tmp;
    u32 tmp2, tmp4;
    u16 sizeofState, sizeofNVState;

    sizeofState   = TblMicroProgFuncOpis[*progPnt].sizeofState;
    sizeofNVState = TblMicroProgFuncOpis[*progPnt].sizeofNVState;

    ptrIn  = progPnt+1;

    if(sizeofState !=0 )
        {
        tmp2 = (ramPnt     + *ptrIn/SZ_DIV);
        tmp4 = (testRamPnt + *ptrIn/SZ_DIV);
        memcpy((char*)tmp4, (char*)tmp2, sizeofState);       // Скопировать State
        ptrIn++;
        }
    if(sizeofNVState !=0 )
        {
        tmp2 = (ramPnt     + *ptrIn/SZ_DIV);
        tmp4 = (testRamPnt + *ptrIn/SZ_DIV);
        memcpy((char*)tmp4, (char*)tmp2, sizeofNVState);       // Скопировать NVState
        ptrIn++;
        }

    i           = TblMicroProgFuncOpis[*progPnt].kolvPin;       // Количество входных переменных
    ptrTypePin  = TblMicroProgFuncOpis[*progPnt].typePin;       // Указатель на таблицу внешних переменных

    for(j=0; j<i; j++)
        {
        tmp = ptrTypePin[j];
        switch(tmp & MASK_PIN_TYPE)
            {
            case FLOAT_PIN_TYPE:
                {
                float tmp3;

                tmp3 = load_float(ramPnt, *ptrIn);
                save_float(testRamPnt, *ptrIn, tmp3);
                }
                break;
            case S32_PIN_TYPE  :
                {
                s32 tmp3;

                tmp3 = load_s32(ramPnt, *ptrIn);
                save_s32(testRamPnt, *ptrIn, tmp3);
                }
                break;

            case S16_PIN_TYPE  :
                {
                s16 tmp3;

                tmp3 = load_s16(ramPnt, *ptrIn);
                save_s16(testRamPnt, *ptrIn, tmp3);
                }
                break;

            case BIT_PIN_TYPE  :
               if(testBitMicro(ramPnt, *ptrIn))     
                   setBitMicro(testRamPnt, *ptrIn);
               else                                 
                   clrBitMicro(testRamPnt, *ptrIn);
           break;
            }
        ptrIn++;
        }
}
//**************************************************************************
//                      Только для отладки
//**************************************************************************
u16 * __stdcall FuncMicrNum(u16 *progPnt, u32 ramPnt)
{
//    u32 RamArea_test[1536];        // Временный массив с данными
    //!!!! u32 RamArea_test[4096];        // Временный массив с данными
    //!!!! u16 tmp;
    //!!!! s32 beginRam;         // Смещение адреса по ОЗУ
    //!!!! u32 tmp2;
	//!!!! u32 testRamPnt;
	static u16 *progPntPred=NULL;
	u16		KolvFuncMicro;
    
	KolvFuncMicro = MPOpis.PntMicroProg->kolvFuncMicroProg;
	MPOpis.cntCall++;
	/*
//	meas_20ms_calc(); //Meas_20mS(); //Partola Visual
	if((progPnt < progPntPred)&&(progPntPred != NULL)){
    	//PAN flagIsFirstCall = 0;
    }
	progPntPred = progPnt;*/

	flagIsFirstCall = 0;
	if (MPOpis.cntCall < KolvFuncMicro) {
		flagIsFirstCall = 1;
	}
//------------
	sprintf(BffName, "%s", TblMicroProgFuncOpis[*progPnt].SymbolName);
//------------
/*
    tmp2 = MPOpis.PntMicroProg->Sz_bit_Sect;
    beginRam = ramPnt - tmp2;                                  // Начало боевого ОЗУ
//    shiftMem = beginRam - (s32)RamArea_test;                 // Смещение по памяти для тестового массива
	StateTestMicroProg = 1;
    testRamPnt = (u32)RamArea_test + tmp2;                   // Указатель на память для тестового вызова функции
    memset(RamArea_test, 0xFFFF, sizeof(RamArea_test));         // Заполнение FFFF
    testFunDataCpy(progPnt, ramPnt, testRamPnt);                // Скопировать входные выходные переменные
    TblMicroProgFunc[*progPnt](progPnt, testRamPnt);            // Вызвать функцию
    tmp = testCheckMem(progPnt, ramPnt, testRamPnt, 0xFFFF);    // Проверить массив после выполнения функции
    if(tmp == 0) return NULL;                                   // Если память запорчена, то вернуть NULL

	StateTestMicroProg = 2;
    memset(RamArea_test, 0x0000, sizeof(RamArea_test));         // Заполнение 0000
    testFunDataCpy(progPnt, ramPnt, testRamPnt);                // Скопировать входные выходные переменные
    TblMicroProgFunc[*progPnt](progPnt, testRamPnt);            // Вызвать функцию
    tmp = testCheckMem(progPnt, ramPnt, testRamPnt, 0x0000);    // Проверить массив после выполнения функции
    if(tmp == 0) return NULL;                                   // Если память запорчена, то вернуть NULL
	*/
	{
		// Попытка вызова функций PWM ядра

	}

    {
    void *ptr;
	StateTestMicroProg = 0;
    ptr = TblMicroProgFunc[*progPnt](progPnt, ramPnt);        // Боевой вызов функции
    return ptr;
    }
}
//**************************************************************************
	extern AllPrm_Type	    *AllPrmPnt;
u16 swapU16(u16 Val)
{
	u16 tmp;

	tmp = Val;

	return ((tmp << 8)&0xff00)+((tmp>>8)&0xff);
}
u32 swapU32(u32 Val)
{
    u32 tmp = Val;
    Val = ((tmp >> 8) & 0xFF0000) +((tmp << 8) & 0xFF000000) +((tmp >> 8) & 0xFF) +((tmp << 8) & 0xFF00);    
    return Val;
    
}
//**************************************************************************
u32 __stdcall MicroProg_Init_STD(u32 ramPnt, const MicroProg_type	*prgPnt, char *pFileFramName)
{
	u32		tmp;
	u16     szUst, i;

	char    buf[sizeof(ustSet_type)] = {0};
	ustSet_type *ustSet;
    
	if (pFileFramName != NULL) {
    	ustSet = NULL;
    	if(es_ReadDrvUst(&szUst, (u32)&ustSet->HeadUst.SzUst, sizeof(ustSet->HeadUst.SzUst), pFileFramName)){
        	szUst = swapU16(szUst);
        	ustSet = (ustSet_type *)buf;
        	if(es_ReadDrvUst(buf, (u32)0, szUst * 4 + 128, pFileFramName)){
            	for(i = 0; i < szUst * 4 + 128; i += 2){
                    *(u16 *)&buf[i] = swapU16(*(u16 *)&buf[i]);
                }
                WrFramMicro(&nv.MicroProg, &ustSet->MicroProg, szUst * 4);
            }
        }
    }
	
	tmp = MicroProg_Init(ramPnt, prgPnt);
	return	tmp;
}
//**************************************************************************
void __stdcall Cleanup_STD(void)
{
	Cleanup();
}
//**************************************************************************
extern const char *NameLogFile;
const char *NameLogFile = "1.bin";
char nameFileAr[512];

typedef struct {
	u16	crc;
	u16	signatura;
	u16	kolvUst;
	u16 reserved[0x40 - 0x03];
} dtuHof_type;

// Структура файла уставок
typedef struct{
	dtuHof_type		hof;                     // Заголовок
	s32				data[4000];					   // Массив с данными
}dtuFile_type;


void __stdcall IndividualFunc_STD(char *FileName)
{
    if(!strlen(FileName)){
        strcpy(nameFileAr, IFaceGUI.workDirName );
        strcat( nameFileAr, NameLogFile);
        FileName = nameFileAr;
    }

	#ifdef _PROJECT_FOR_VS2005_
	{
		char * pos;
		u16		tmp;
		u16     szUst, i, microKolvUst;

		char    buf[sizeof(ustSet_type)] = {0};
		ustSet_type *ustSet;
	    
		// Определяем число уставок в системе параметров
		microKolvUst = MPOpis.PntMicroProg->kolvUst;
		// Делаем проверку, что это файл зав. уставок
		pos = strchr (FileName, '.dtu');
		if (pos) { // Таки да, файл уставок
    		ustSet = NULL;
    		if(es_ReadDrvUst(&szUst, (u32)&ustSet->HeadUst.SzUst, sizeof(ustSet->HeadUst.SzUst), FileName)){
        		szUst = swapU16(szUst);
        		ustSet = (ustSet_type *)buf;
				
				// Если кол-во уставок не совпадает, то уставки не грузим.
				if (szUst != microKolvUst)
					return;

        		if(es_ReadDrvUst(buf, (u32)0, szUst * 4 + 128, FileName)){
            		for(i = 0; i < szUst * 4 + 128; i += 2){
						*(u16 *)&buf[i] = swapU16(*(u16 *)&buf[i]);
					}
					WrFramMicro(&nv.MicroProg, &ustSet->MicroProg, szUst * 4);
					Ust_Copy(NV_to_RAM);
					tmp = es_CloseDrvUst();
				}
			}
			return;

		}
	}
	#endif


    LogerPwmPrintf(FileName);
//	DemoSaw();
}
//**************************************************************************
void TestS(void);
int __declspec(dllexport) __stdcall InitDllIFace(void)
{
    int             i;
    //TestMicroProg();
    //TestMeas();
    //return (int)&IFaceGUI;
	IFaceGUI.IndividualFunc	= IndividualFunc_STD;
	flagIsFirstCall = 1;

	IFaceGUI.Pnt_TblMicroProgFuncOpis	= TblMicroProgFuncOpis;
	IFaceGUI.kolvFuncOpis	            = kolvFuncOpis;
	IFaceGUI.Pnt_FuncMicroPC	        = FuncMicrNum;
	IFaceGUI.SignatCoreMicro	        = 2; // исправить Partola
	IFaceGUI.pFuncModbus	            = ModbusFunc_STD;
	IFaceGUI.pMicroProg_Init	        = MicroProg_Init_STD;
	IFaceGUI.Cleanup	                = Cleanup_STD;
	IFaceGUI.workDirName[0]                = 0;
	memset(IFaceGUI.NameBlockExe, 0, sizeof(IFaceGUI.NameBlockExe));

    for( i=0; i<graphNum; i++){
        IFaceGUI.GrafsMem.Graf[i].DataPnt = (double *)malloc(LOG_BFF_nPoint * sizeof(double));
    }

	IFaceGUI.GrafsMem.kolvPoint = LOG_BFF_nPoint;
	IFaceGUI.GrafsMem.timePoint = 1000;


	return (int)&IFaceGUI;
}
//***********************************************************************
#define	tkub	1
int Scalc(int i)
{
	static int	S=0, t=0, tkv=0;
	int	    	Pr, tmp;

	tkv = (tkv + tkub);
	t += tkv;
	S+=t;

	i++;
	Pr = ((i*i*i+3*i*i+2*i)*tkub)/6;
	if(Pr != S){
    	tmp = 1;
    }
	return	S;
}
//***********************************************************************
void TestS(void)
{
	int		i;
	int		S;
	FILE	*stream;

	stream = fopen("test.txt", "w+t");
	for(i=0;i<500;i++){
    	S = Scalc(i);
    	fprintf(stream, "%i,\n", (int)S);
    }
	fclose(stream);
}
//***********************************************************************
//**************************************************************************
typedef struct{
    s16		var[graphNum];
}Log_type_s;
#define SZlogFileHead (sizeof(u32)*4)
//****************************************************************************
void LogerPwmPrintf(const char *FileName)
{


	FILE			*streamRD;
	u32				szFile, point;
	Log_type_s		LogMem;

	logFile_type	*fileData;
//	Log_type_s		*dataPnt;
	u32				cntRd;
	u32				LogInd;
    u16             Nvar;
    s16             *dataPnt;
    u32             sz, sz2;
    int				grf;//, c, tmp;
    u32             i, j;

	point = 0;
	cntRd = 0;

	streamRD = fopen( FileName,	"r+b" );
    if(streamRD == NULL) {
        return;
    }
	if(streamRD != NULL){
		szFile = GetFileSizeMy(FileName);

		fileData = (void*)malloc( szFile );			// Выделить место
		fread(fileData, szFile, 1, streamRD);		// Вычитать данные
		fclose(streamRD);

        Nvar = fileData->hof.Nvar;
		if(Nvar >= 9){
			return;
		}
		IFaceGUI.GrafsMem.NVar = Nvar;

        sz = fileData->hof.Sz;
        sz2 = (szFile - sizeof( headOfLogFile_type ) )/(sizeof( s16 )*Nvar);
        if(sz > sz2 ) sz = sz2;

		LogInd = fileData->hof.LogInd+1;
        dataPnt = (s16*)&fileData->data[ (fileData->hof.LogInd+1) * Nvar ];
        
        for(i=0; i< graphNum; i++){
            for( j=0; j<sz; j++){
                IFaceGUI.GrafsMem.Graf[i].DataPnt[j] = 0;
            }
        }

		for(point=0, cntRd=1; ; point++ , cntRd++ ){
            if( cntRd >= sz  || cntRd >= sz2 )
                break;
			if(point >= LOG_BFF_nPoint )	break;
            if(dataPnt >= &fileData->data[ (sz * Nvar) ] )
                dataPnt = &fileData->data[ 0 ];

            memcpy(&LogMem, dataPnt, sizeof( u16 ) * Nvar );
            dataPnt += Nvar;

            switch( Nvar ){
			case 9: IFaceGUI.GrafsMem.Graf[8].DataPnt[point] = (s16)LogMem.var[8];
			case 8: IFaceGUI.GrafsMem.Graf[7].DataPnt[point] = (s16)LogMem.var[7];
			case 7: IFaceGUI.GrafsMem.Graf[6].DataPnt[point] = (s16)LogMem.var[6];
			case 6: IFaceGUI.GrafsMem.Graf[5].DataPnt[point] = (s16)LogMem.var[5];
			case 5: IFaceGUI.GrafsMem.Graf[4].DataPnt[point] = (s16)LogMem.var[4];
			case 4: IFaceGUI.GrafsMem.Graf[3].DataPnt[point] = (s16)LogMem.var[3];
			case 3: IFaceGUI.GrafsMem.Graf[2].DataPnt[point] = (s16)LogMem.var[2];
			case 2: IFaceGUI.GrafsMem.Graf[1].DataPnt[point] = (s16)LogMem.var[1];
			case 1: IFaceGUI.GrafsMem.Graf[0].DataPnt[point] = (s16)LogMem.var[0];
            }
		}

		IFaceGUI.GrafsMem.kolvPoint = point;

	}
	else
		IFaceGUI.GrafsMem.NVar = 0;
    IFaceGUI.GrafsMem.timePoint = (fileData->hof.TimeOneTick * 1e-4) * (fileData->hof.Ntic + 1);
    
    for(grf = 0; grf < Nvar; grf++)		sprintf(IFaceGUI.GrafsMem.Graf[grf].Name, "varN%d", grf+1);
	
    free( fileData );	// Освободить память
}

#include <string.h>
#include <stdio.h>
#include <windows.h>

//const char *NameLogFile = "1.bin";
//********************************************************
int GetFileSizeMy(const TCHAR *fileName)
{
	BOOL						fOk;
	WIN32_FILE_ATTRIBUTE_DATA	fileInfo;

	if(NULL == fileName)	return -1;

	fOk = GetFileAttributesEx(fileName, GetFileExInfoStandard, (void*)&fileInfo);
	if (!fOk)	return -1;
	if(fileInfo.nFileSizeHigh == 0)	return (long)fileInfo.nFileSizeLow;
	return	0xFFFFFFFF;
}
//**************************************************************************
