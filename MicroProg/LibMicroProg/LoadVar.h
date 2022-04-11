#ifndef LoadVar_H
#define LoadVar_H

//********************************************************
//загрузка выгрузка переменных
#if(DEFINE_LOAD == 1)
    #define load_s16(ramPnt, Num)           (*(s16 *)(ramPnt + Num))
    #define load_s32(ramPnt, Num)           (*(s32 *)(ramPnt + Num))
    #define load_float(ramPnt, Num)         (*(float *)(ramPnt + Num))
    
    #define save_s16(ramPnt, Num, Val)      *(s16   *)(ramPnt + Num) = Val
    #define save_s32(ramPnt, Num, Val)      *(s32   *)(ramPnt + Num) = Val
    #define save_float(ramPnt, Num, Val)    *(float *)(ramPnt + Num) = Val
#else
    s16 load_s16(u32 ramPnt, u16 Num);
    s32 load_s32(u32 ramPnt, u16 Num);
    float load_float(u32 ramPnt, u16 Num);
    
    void save_s16(u32 ramPnt, u16 Num, s16 Val);
    void save_s32(u32 ramPnt, u16 Num, s32 Val);
    void save_float(u32 ramPnt, u16 Num, float Val);
    void save_float_check(u32 ramPnt, u16 Num, s32 Val);
#endif



void clrBitMicro(u32 ramPnt, s16 Nbit);
void setBitMicro(u32 ramPnt, s16 Nbit);
void defBitMicro(u32 ramPnt, s16 Nbit, s16 ValBit);
void moveBitMicro(u32 ramPnt, s16 NbitDest, s16 NbitSrc);
void tranzitBitMicro(u32 ramPnt, s16 Nbit, s16 NbitSrc);
s16 testBitMicro(u32 ramPnt, s16 Nbit);     //0 / 1

void *GetFormulaPnt(u16 adrForm);
void *GetNvAdr(u32 Num, void *nvAdr);

//********************************************************
#ifdef _PROJECT_FOR_CCS_
    #define readAckZero(dst, srs)           \
    {                                       \
        IntMasterDisable();      \
        dst = srs;                          \
        srs = 0;                            \
        IntMasterEnable();         \
    }
#else
    #define readAckZero(dst, srs)           \
    {                                       \
        dst = srs;                          \
        srs = 0;                            \
    }
#endif
//********************************************************
//********************************************************


//AllPrm_Type *AllPrmPnt;
#endif  //LoadVar_H
