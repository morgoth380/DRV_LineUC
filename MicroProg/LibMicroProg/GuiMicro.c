#include <windows.h>
#include "Global_include.h"
#include "Gui.h"
#include <string.h>
#include <stdio.h>
//********************************************************
//********************************************************
#define GET_FUNC_CODE               //получили сами функции
#include "AllFuncMicroProg.h"
#undef  GET_FUNC_CODE


#define GET_FUNC_PNT                //получили таблицу указателей на функции
const pFuncMicro_type   TblMicroProgFunc[]={
    #include "AllFuncMicroProg.h"
};
#undef  GET_FUNC_PNT


#define GET_FUNC_TABL_OPIS              //
    #include "AllFuncMicroProg.h"
#undef  GET_FUNC_TABL_OPIS


#define GET_FUNC_OPIS               //получили таблицу описателей функций для ПК
const OpisFuncMicroProg_type    TblMicroProgFuncOpis[]={
    #include "AllFuncMicroProg.h"
};
#undef  GET_FUNC_OPIS

extern char *pGlobalWorkDirName;
extern IFaceGUI_type        IFaceGUI;

//          Тип для FAT таблицы


const FATtable_type FATtable[N_Files] = {
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 0
    0                           , 0,                        0  , FILE_RdWr    , // 1
    0,0,FILE_In_ERR,FILE_ReadOnly, // 2
    0,0,FILE_In_ERR,FILE_ReadOnly, // 3
    0,0,FILE_In_ERR,FILE_ReadOnly, // 4
    0,0,FILE_In_ERR,FILE_ReadOnly, // 5
    0,0,FILE_In_ERR,FILE_ReadOnly, // 6
    0,0,FILE_In_ERR,FILE_ReadOnly, // 7
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 8
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 9
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 10
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 11
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 12
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 13
    (u32)&MPOpis.Base           , sizeof(MPOpis.Base)        , FILE_In_Ram    , FILE_ReadOnly, // 14
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 15
    0                           , 0                          , FILE_In_ERR    , FILE_ReadOnly, // 16
};


const int kolvFuncOpis = (sizeof(TblMicroProgFuncOpis) / sizeof(OpisFuncMicroProg_type));
/*********************************************************/
//********************************************************
FILE    *es_fFRAM=NULL, *es_fspi=NULL, *es_fDRVUST=NULL;
SEM_Obj     SEM_uart;


//********************************************************
//********************************************************
//Работа с вычитанным из привода файлом
//********************************************************
int es_OpenDrvUst(char *fName)
{

    if (es_fDRVUST) return 1;

    es_fDRVUST=fopen(fName,"r+b");
    if (!es_fDRVUST) {
        return 0;
    }

    return 1;
}
//**************************************************************************
int es_CloseDrvUst(void)
{
    if (!es_fDRVUST) return 0;

    fclose(es_fDRVUST);

    return 1;
}
//**************************************************************************
int es_ReadDrvUst(void *buf, int adr, int size, char *fName)
{
    if (!es_OpenDrvUst(fName)) return 0;
    if (fseek(es_fDRVUST, adr, SEEK_SET)) return 0;
    if (!fread(buf, size, 1, es_fDRVUST)) return 0;
    return 1;
}
//**************************************************************************
int es_WriteDrvUst(void *buf, int adr, int size, char *fName)
{
    if (!es_OpenDrvUst(fName)) return 0;
    if (fseek(es_fFRAM, adr, SEEK_SET)) return 0;
    if (!fwrite(buf, size, 1, es_fFRAM)) return 0;
    fflush(es_fFRAM);
    return 1;
}
//********************************************************
//Работа с памятью
//********************************************************
int es_OpenFRAM(void)
{
    int i,buf;
    char Name[1024];

    if (es_fFRAM) return 1;
    if (IFaceGUI.workDirName[0] == 0) return 0;
    //if(strlen(pGlobalWorkDirName) > 1000) return 0;
    strcpy(Name, IFaceGUI.workDirName);
    strcat(Name, "Fram.bin");

    es_fFRAM=fopen(Name,"r+b");
    if (!es_fFRAM) {
        es_fFRAM=fopen(Name,"w+b");
        if (!es_fFRAM) return 0;
        buf=0;
        for(i=0;i<32768/4;i++)
            fwrite(&buf, 4, 1, es_fFRAM);
    }

    return es_fFRAM?1:0;
}
//**************************************************************************
int es_CloseFRAM(void)
{
    if (!es_fFRAM) return 0;

    fclose(es_fFRAM);

    return 1;
}
//**************************************************************************
int es_ReadFRAM(void *buf, int adr, int size)
{
    if (!es_OpenFRAM()) return 0;
    if (fseek(es_fFRAM, adr, SEEK_SET)) return 0;
    if (!fread(buf, size, 1, es_fFRAM)) return 0;
    return 1;
}
//**************************************************************************
int es_WriteFRAM(void *buf, int adr, int size)
{
    if (!es_OpenFRAM()) return 0;
    if (fseek(es_fFRAM, adr, SEEK_SET)) return 0;
    if (!fwrite(buf, size, 1, es_fFRAM)) return 0;
    fflush(es_fFRAM);
    return 1;
}
//**************************************************************************
int es_OpenSpfl(void)
{
    int i,buf;

    if (es_fspi) return 1;

    es_fspi=fopen("spfl.bin","r+b");
    if (!es_fspi) {
        es_fspi=fopen("spfl.bin","w+b");
        if (!es_fspi) return 0;
        buf=0;
        for(i=0;i<1056*8192/4;i++)
            fwrite(&buf, 4, 1, es_fspi);
    }

    return es_fspi?1:0;
}
//**************************************************************************
int es_CloseSpfl(void)
{
    if (!es_fspi) return 0;

    fclose(es_fspi);

    return 1;
}
//**************************************************************************
int es_ReadSpfl(void *buf, int adr, int size)
{
    if (!es_OpenSpfl()) return 0;
    if(fseek(es_fspi, adr, SEEK_SET))   return  0;
    if (!fread(buf, size, 1, es_fspi))  return  0;
    return 1;
}
//**************************************************************************
int es_WriteSpfl(void *buf, int adr, int size)
{
    if (!es_OpenSpfl()) return 0;
    if(fseek(es_fspi, adr, SEEK_SET))   return  0;
    if (!fwrite(buf, size, 1, es_fspi)) return  0;
    fflush(es_fspi);
    return 1;
}
//**************************************************************************
void Cleanup(void)
{
    es_CloseSpfl();
    es_CloseFRAM();
}
//**************************************************************************
int FastMemWr_1(void *kuda, void *otkuda, int skolko)
{
    if(skolko == 0) return 1;
    es_WriteFRAM(otkuda, (int)kuda, skolko);
    return es_WriteFRAM(otkuda, (int)(kuda), skolko);
}
//**************************************************************************
int FastMemRd(void *kuda, void *otkuda, int skolko)
{
    if(skolko == 0) return 1;
    return es_ReadFRAM(kuda, (int)otkuda, skolko);
}
//**************************************************************************
u16 spflRd(void *AdrLpc, void *AdrSpfl, u16 skolko)
{
    if(skolko == 0) return 1;
    es_ReadSpfl(AdrLpc, (int)AdrSpfl, skolko);
    return 1;
}
//**************************************************************************
u16 spflWr(void *AdrSpfl, void *AdrLpc, u16 skolko)
{
    if(skolko == 0) return 1;
    es_WriteSpfl(AdrLpc, (int)AdrSpfl, skolko);
    return 1;
}
//**************************************************************************
/*============================================================================*/
//                 Вычисление адреса по номеру файла
/*============================================================================*/
void * adrInMem(u16 file, u32 adr)
{
    return (void*)(FATtable[file].adres + (adr << 1));
}
inline int checkAdrInFile(u16 file, u32 adr, u16 size)
{
    if(file < N_Files){
        if(((adr << 1) + (size << 1)) > FATtable[file].size )     return 0;
        else                                        return FATtable[file].alloc;
    }
    else            return 0;
}
u16 fileRd(unsigned char *Bff, u16 file, u32 adr, u16 size)
{
    u16                 memType;
    u16                 i;
    void                *srs;

    // Проверка на допустимый диапазон файлов
    if(size == 0)                                           return ErrMb(Bff, Illegal_DataVal_merr);
    if((memType = checkAdrInFile(file, adr, size)) == 0 )   return ErrMb(Bff, Illegal_DataAdr_merr);
    
    srs = adrInMem(file, adr);


    if(memType == FILE_In_Flash){
        ErrMb(Bff, Illegal_DataVal_merr);
    }
    else if((memType == FILE_In_Fram)){
        //ptrMsgFM25 = pmsg;
        es_ReadFRAM(Bff, adr, size);
    }
    else if(memType == FILE_In_Ram){
        memcpy(Bff+8, srs, size * 2);
        for (i = 0; i < size * 2; i+= 2){
            *(u16 *)&Bff[i + 8] = swapU16(*(u16 *)&Bff[i + 8]);
        }
    }

    return  size*2+8;                   // Длина ответа

}
//**************************************************************************
u16 fileWr(u16 file, u32 adr, unsigned char *Bff,  u16 size )
{
    return  size;
}
//**************************************************************************
//********************************************************
    OpisPrm_type    OpisPrmCopy[1024];
void debugCopy(void)
{
    memcpy((char *)&OpisPrmCopy, (char *)MPOpis.PntOpisUst, sizeof(OpisPrm_type)*75);
}
//********************************************************
void PWM_ReInit(s16 Fpwm_X_XXkHz, s16 Dtime_X_XuS, s16 TypeDrv)
{
    
}

//********************************************************
void PWM_OutEnable(s16 ventel)
{
    //PwmEn = 1;
}
//********************************************************
void PWM_OutDisable(s16 ventel)
{
    //PwmEn = 0;
}
//********************************************************
s16 checkErrKey(s16     *NumFailPnt)
{
    return 0;
}
//********************************************************
void readExtADC(void)
{
}
//********************************************************
void GPIO12_set(void){}
void GPIO12_clear(void){}
void GPIO12_toggle(void){}

void GPIO13_set(void){}
void GPIO13_clear(void){}
void GPIO13_toggle(void){}


u16 extADCbuf[8];
u16 ErrCharge;
u16 ErrKzUd;
/////u16    bDin;
//u16 PwmEn;



//********************************************************
u16 RecBrCharge, RecBrStatus;
//********************************************************
void LogRSout(void *ptr, u16 size){}


//**************************************************************************
void unpackData(char *dst, const char *srs, u16 size)
{
    memcpy(dst, srs, size*2);
}
//**************************************************************************
