#include "Global_include.h"
#include "GlobalVar.h"

//********************************************************
//********************************************************
#if(SECTION_LIM_CONTROL == 1)
void LoopForever(void){ 
    for(;;);
}
//********************************************************
void NbitControl(s16 Nbit)
{
    if(Nbit >= 0)    LoopForever();
}
//********************************************************
void FloatControl(u16 Num)
{
    if((Num < MPOpis.PntMicroProg->Pnt_float_Sect)
        ||(Num > MPOpis.PntMicroProg->Pnt_s32_Sect))    LoopForever();
}
//********************************************************
void S32Control(u16 Num)
{
    if((Num < MPOpis.PntMicroProg->Pnt_s32_Sect)
        ||(Num > MPOpis.PntMicroProg->Pnt_s16_Sect))    LoopForever();
}
//********************************************************
void S16Control(u16 Num)
{
    if(Num < MPOpis.PntMicroProg->Pnt_s16_Sect)
            LoopForever();
}
//********************************************************
#else 
    #define NbitControl(Nbit)
    #define FloatControl(Num)
    #define S32Control(Num)
    #define S16Control(Num)
#endif  //SECTION_LIM_CONTROL
//********************************************************
//********************************************************
#if(DEFINE_LOAD == 0)
in_line s16 load_s16(u32 ramPnt, u16 Num){ 
    S16Control(Num);
    return *(s16 *)(ramPnt + Num);
}

in_line s32 load_s32(u32 ramPnt, u16 Num){
    S32Control(Num);
    return *(s32 *)(ramPnt + Num);
}

in_line f32 load_float(u32 ramPnt, u16 Num){
    FloatControl(Num);
    return *(f32 *)(ramPnt + Num);
}

//********************************************************
in_line void save_s16(u32 ramPnt, u16 Num, s16 Val){
    S16Control(Num);
    *(s16 *)(ramPnt + Num) = Val;
}

in_line void save_s32(u32 ramPnt, u16 Num, s32 Val){
    S32Control(Num);
    *(s32 *)(ramPnt + Num) = Val;
}

in_line void save_float(u32 ramPnt,u16 Num, f32 Val){
    FloatControl(Num);
    *(f32 *)(ramPnt + Num) = Val;
}

in_line void save_float_check(u32 ramPnt, u16 Num, s32 Val){
    FloatControl(Num);
    *(s32 *)(ramPnt + Num) = Val;
}
//********************************************************
#endif	//DEFINE_LOAD

//********************************************************
//********************************************************
u16 nBitNeg[]={
(u16)(~(1<<0 )),
(u16)(~(1<<1 )),
(u16)(~(1<<2 )),
(u16)(~(1<<3 )),
(u16)(~(1<<4 )),
(u16)(~(1<<5 )),
(u16)(~(1<<6 )),
(u16)(~(1<<7 )),
(u16)(~(1<<8 )),
(u16)(~(1<<9 )),
(u16)(~(1<<10)),
(u16)(~(1<<11)),
(u16)(~(1<<12)),
(u16)(~(1<<13)),
(u16)(~(1<<14)),
(u16)(~(1<<15)),
};
/*
#define RAM_OFFSET_BIT      (((Nbit<<1)>>4)+1)
in_line void clrBitMicro(u32 ramPnt, s16 Nbit)
{
    *(u16 *)(ramPnt - RAM_OFFSET_BIT ) &= nBitNeg[((Nbit<<12)>>12)];
}
*/
/*#ifdef  _PROJECT_FOR_CCS_
    #define RAM_OFFSET_BIT      (((Nbit<<1)>>5))
#else*/
    #define RAM_OFFSET_BIT ((((Nbit&0x7FFE)>>4)) * sizeof(s16))
/*#endif*/
//#define   RAM_OFFSET_BIT      ((1+((Nbit&0x7FFE)>>4)) * sizeof(s16))
in_line void clrBitMicro(u32 ramPnt, s16 Nbit)
{
    NbitControl(Nbit);
    *(u16 *)(ramPnt - RAM_OFFSET_BIT) &= (~(1<<(Nbit &0xF)));
}

//********************************************************
in_line void setBitMicro(u32 ramPnt, s16 Nbit)
{
    NbitControl(Nbit);
    *(u16 *)(ramPnt - RAM_OFFSET_BIT) |= (1<<(Nbit &0xF));
}
//********************************************************
in_line void defBitMicro(u32 ramPnt, s16 Nbit, s16 ValBit)
{
    NbitControl(Nbit);
    if(ValBit)  *(u16 *)(ramPnt - RAM_OFFSET_BIT) |=   (1<<(Nbit &0xF));
    else        *(u16 *)(ramPnt - RAM_OFFSET_BIT) &= (~(1<<(Nbit &0xF)));
}
//********************************************************
in_line void tranzitBitMicro(u32 ramPnt, s16 Nbit, s16 NbitSrc)
{
    s16     val;

    NbitControl(NbitSrc);
    NbitControl(Nbit);
    val = *(s16 *)(ramPnt - ((((NbitSrc & 0x7FFE) >> 4)) * sizeof(s16)));
    if((val >> (NbitSrc & 0xF)) & 1)  *(u16 *)(ramPnt - RAM_OFFSET_BIT) |= (1 << (Nbit & 0xF));
    else                              *(u16 *)(ramPnt - RAM_OFFSET_BIT) &= (~(1 << (Nbit & 0xF)));
}
//********************************************************
in_line void moveBitMicro(u32 ramPnt, s16 Nbit, s16 NbitSrc)
{
    s16     val;
    
    NbitControl(Nbit);
    NbitControl(NbitSrc);
    val = *(u16 *)(ramPnt - (NbitSrc>>4));
    if(val & (1<<(NbitSrc &0xF)))   *(u16 *)(ramPnt - RAM_OFFSET_BIT) |=   (1<<(Nbit &0xF));
    else                            *(u16 *)(ramPnt - RAM_OFFSET_BIT) &= (~(1<<(Nbit &0xF)));
}
//********************************************************
in_line s16 testBitMicro(u32 ramPnt, s16 Nbit)      //0 / 1
{
    s16     val;
    
    NbitControl(Nbit);
    val = *(s16 *)(ramPnt - RAM_OFFSET_BIT);
    return  (val>>(Nbit &0xF)) & 1;
}
//********************************************************
//********************************************************
in_line void *GetNvAdr(u32 Num, void *nvAdr)
{
    return  ((void *)((u32)(Num) + (u32)nvAdr));
}
//********************************************************
in_line s16 s16LimControl(s32   val)
{
    if(val > 32767)     return 32767;
    if(val < -32768)    return  -32768;
    return  (s16)val;
}
//********************************************************
in_line void *GetFormulaPnt(u16 adrForm)
{
    return  AddAdr(MPOpis.PntMicroProg, adrForm);
}
//********************************************************
//
//********************************************************
//********************************************************
in_line s32 TimeFromPusk(void)
{
    return  0;
}
//********************************************************
in_line s16 IsSecondKvant(void)
{
    return  0;
}
//********************************************************
// ѕри первом проходе цикла микропрограммы возвращает 1
// ѕо окончании первого прохода возвращает 0.
//********************************************************
in_line s32 GetTimePowerOff(void)   //сколько времени питание было отключено
{
    return  0;
}
//********************************************************
in_line s16 DrvIsStop(void)
{
    return (!GlobalM4.FlagCommand.bit.PwmOn) && (!GlobalM4.FlagCommand.bit.mWorkEnb);
}
//********************************************************
in_line s16 PwmIsOn(void)
{
  u16   pwmStat;
  pwmStat = 0;
  #ifndef _PROJECT_FOR_STM32_  // If compile for VS2005
  pwmStat = (s16)(GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb);  
  #endif
  #ifdef _PROJECT_FOR_STM32_ // if compile for IAR
  pwmStat = (s16)(GlobalM4.FlagCommand.bit.PwmOn || GlobalM4.FlagCommand.bit.mWorkEnb);
  #endif

  return pwmStat;
}
//********************************************************
