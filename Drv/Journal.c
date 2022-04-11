#include "Global_include.h"
#include "GlobalVar.h"

#define MAX_JRN 32

RecJrn_type RecJrnWarnWr, RecJrnFailWr;
u16 EnaJrnFailWrite = 0;
u16 EnaJrnWarnWrite = 0;
extern u32 UnixTime;

void ClrIndex(void);

void SetParamJrn( RecJrn_type *pntRecJrn)
{
    FailData_type   *mPnt;
    mPnt = &GlobalM4.FailData;
    
    pntRecJrn->timeUnix         = UnixTime;
    pntRecJrn->TIGBT            = (u16)(((mPnt->FailValue.TIGBT * 100) + 5) /10);  
    pntRecJrn->Ud               = (u16)mPnt->FailValue.Ud;     
    pntRecJrn->Ifull            = (u16)(mPnt->FailValue.Ifull * 10);
    pntRecJrn->Iact             = (u16)(mPnt->FailValue.Iact * 10);
    pntRecJrn->Iu               = (u16)(mPnt->FailValue.Iu * 10);
    pntRecJrn->Iv               = (u16)(mPnt->FailValue.Iv * 10);
    pntRecJrn->Iw               = (u16)(mPnt->FailValue.Iw * 10);
    pntRecJrn->Fout             = (u16)(((mPnt->FailValue.Fref * 100) + 5) /10); 
    pntRecJrn->StateWord_CRC    = mPnt->FailValue.NumFail;                      // номер аварии    
    pntRecJrn->Uab              = (u16)(mPnt->FailValue.Uab * 10);
    pntRecJrn->Ubc              = (u16)(mPnt->FailValue.Ubc * 10);
    pntRecJrn->Uca              = (u16)(mPnt->FailValue.Uca * 10);
} 

/*============================================================================*/
void GetIndex(Index_type *pIndex)
{
    RdFramMicro(pIndex, &nv.file.FailJrn.RecIndex, sizeof(Index_type));
}
/*============================================================================*/
void SetIndex(Index_type *pIndex)
{
    WrFramMicro(&nv.file.FailJrn.RecIndex, pIndex, sizeof(Index_type));
}
/*============================================================================*/
//Чтение журнала
//Входные параметры:
//                   - указатель на структуру журнала
//                   - индекс журнала
/*============================================================================*/
void ReadJrn(RecJrn_type *pntJrn, u16 index)
{
    RdFramMicro(pntJrn, &nv.file.FailJrn.RecJrn[index - 1], sizeof(RecJrn_type));
}
/*============================================================================*/
//Запись журнала
//Входные параметры:
//                   - указатель на структуру журнала
/*============================================================================*/
void WriteJrn(RecJrn_type *pntRecJrn)
{
    u16             i, crc_, *Pnt;
    RecJrn_type     *PntRecNV;
    Index_type      RecIndex;

    pntRecJrn->timeUnix = UnixTime;
    GetIndex(&RecIndex);
    SetParamJrn(pntRecJrn);

    crc_ = crcPacket((unsigned char *)&RecIndex, sizeof(RecIndex) - sizeof(RecIndex.crc));
    if (crc_ != RecIndex.crc){
       ClrIndex();
       memset(&RecIndex, 0, sizeof(RecIndex));
    } 

    PntRecNV = nv.file.FailJrn.RecJrn;

    RecIndex.indexRec++;
    if (RecIndex.indexRec > MAX_JRN) 
        RecIndex.indexRec = 1;                   //Индекс
    
    RecIndex.quantRec++;                                          //Счетчик
    if (RecIndex.quantRec > MAX_JRN)
        RecIndex.quantRec = MAX_JRN;

    RecIndex.numRec++;
    if (RecIndex.numRec == 0) 
        RecIndex.numRec++;
    RecIndex.sizeRec = sizeof(RecJrn_type);

    crc_ = crcPacket((unsigned char *)&RecIndex, sizeof(RecIndex) - sizeof(RecIndex.crc));
    RecIndex.crc = crc_;

    SetIndex(&RecIndex);

    pntRecJrn->NumRec = RecIndex.numRec;                                //Запись индекса
    pntRecJrn->StateWord_CRC &= 0x00ff;                                 //Обнуление КС в старшем байте
    Pnt = (u16*)pntRecJrn;

    for (i = 0, crc_ = 0; i < sizeof(RecJrn_type); i++){                //Подсчет по остальным байтам
        crc_ += *Pnt;
        Pnt++;
    }

    crc_ = ((crc_ >> 8) + (crc_ & 0x00FF)) & 0x00FF;
    crc_ ^= 0xAA;
    pntRecJrn->StateWord_CRC = (crc_ << 8) | (pntRecJrn->StateWord_CRC & 0x00FF);

    WrFramMicro(&PntRecNV[RecIndex.indexRec - 1], pntRecJrn, sizeof(RecJrn_type));

}
/*============================================================================*/
void ClrIndex(void)
{
   Index_type      *RecIndex;
   u16 mas[sizeof(Index_type)] = {0};
   RecIndex = &nv.file.FailJrn.RecIndex;

   WrFramMicro(RecIndex, &mas[0], sizeof(Index_type));
}


