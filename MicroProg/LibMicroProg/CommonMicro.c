#define DEFINE_CommonMicro_GLOBAL

#include "Global_include.h"


//****************************************************************************
/// MicroProg
//****************************************************************************
   u32            RamArea[2048];

//****************************************************************************
void CoreMicroProg(u32 ramPnt, MPOpis_type  *mpPnt)
{
    const u16           *progPnt;
    static u16          NfuncStop=125;
    u16                 kolvFuncMicroProg;

//     debug_PIN_set();
    kolvFuncMicroProg = mpPnt->PntMicroProg->kolvFuncMicroProg;
    progPnt = mpPnt->CodeMicroProg;
    for(mpPnt->NumFunc = 0; mpPnt->NumFunc < kolvFuncMicroProg; mpPnt->NumFunc++){
        if(mpPnt->NumFunc == NfuncStop){
            NfuncStop++;
            NfuncStop--;
        }
        if(*progPnt > 444){
            NfuncStop++;
            NfuncStop--;
        }
        progPnt = TblMicroProgFunc[*progPnt](progPnt, ramPnt);
    }
    mpPnt->cntCall++;
//     debug_PIN_clear();
}
//**************************************************************************
void CoreMicroProgWW(u32 ramPnt, MPOpis_type    *mpPnt)
{
    const u16           *progPnt;

    if((mpPnt->cntCallStop == 0xFFFFFFFF)||(mpPnt->cntCall < mpPnt->cntCallStop)){
        progPnt = mpPnt->CodeMicroProg;
        if(mpPnt->NumFuncStop >= mpPnt->kolvFunc)       mpPnt->NumFunc = 0;
        else if(mpPnt->NumFuncStop == mpPnt->NumFunc)   return;

        for(; *progPnt != 0; mpPnt->NumFunc++){
            if(mpPnt->NumFunc < mpPnt->NumFuncStop) break;
            progPnt = TblMicroProgFunc[*progPnt](progPnt, ramPnt);
        }
        mpPnt->cntCall++;
    }
}
//**************************************************************************
extern s16 BaseTabl[16];
u32 MicroProg_Init(u32 ramPnt, const MicroProg_type *prgPnt)
{
    void            *PntFlash, *PntRam;
    MPOpis_type     *ePnt;

    ePnt = &MPOpis;
    memset((char *)ePnt, 0, sizeof(MPOpis_type));
    //PAN flagIsFirstCall = 1;
    if(crcPacket((unsigned char *)&prgPnt->SignatCoreMicro,(prgPnt->szFlash - sizeof(s32))) != prgPnt->CrcMicroprog){
        memset((char *)ePnt, 0, sizeof(MPOpis_type));
        return 0;
    }

    ePnt->PntMicroProg      = prgPnt;
    ePnt->PntOpisGrp        = (const OpisGrp_type   *)((u32)prgPnt + prgPnt->PntOpisGrp);
    ePnt->PntOpisUst        = (const OpisPrm_type   *)((u32)prgPnt + prgPnt->PntOpisUst);
    ePnt->PntOpisTek        = (const OpisPrm_type   *)((u32)prgPnt + prgPnt->PntOpisTek);
    ePnt->CodeMicroProg     = (const u16            *)((u32)prgPnt + prgPnt->PntMicroProg);
    ePnt->kolvGrpOpis       = prgPnt->kolvGrp;
    ePnt->kolvPrmOpis       = prgPnt->kolvUst + prgPnt->kolvTek;
    
    ePnt->cntCallStop       = 0xFFFFFFFF;
    ePnt->NumFuncStop       = 0xFFFF;
    ePnt->kolvFunc          = 6000;         //WARNING

    memset((char *)ramPnt, 0, prgPnt->kolvRam_All);                 //протирание всей выделенной памяти
    AllPrmPnt = (AllPrm_Type *)ramPnt;
    ramPnt      = ramPnt + prgPnt->Sz_bit_Sect;
    ePnt->ramPnt= ramPnt;
    PntRam      = AddAdr(ramPnt, prgPnt->Pnt_float_Sect);           //инициализация float const
    PntFlash    = AddAdr(prgPnt, prgPnt->PntConst_float);
    memcpy(PntRam, PntFlash, prgPnt->kolvConst_float * sizeof(float));

    PntRam      = AddAdr(ramPnt, prgPnt->Pnt_s32_Sect);             //инициализация s32 const
    PntFlash    = AddAdr(prgPnt, prgPnt->PntConst_s32);
    memcpy(PntRam, PntFlash, prgPnt->kolvConst_s32 * sizeof(s32));

    PntRam      = AddAdr(ramPnt, prgPnt->Pnt_s16_Sect);             //инициализация s16 const
    PntFlash    = AddAdr(prgPnt, prgPnt->PntConst_s16);
    memcpy(PntRam, PntFlash, prgPnt->kolvConst_s16 * sizeof(s16));

    setBitMicro(ramPnt, 0x8011);                                    //устанавливаем первый бит в секции битовых переменных
    //debugCopy();
    StartExtMem();
    
    ePnt->Base[NUM_FREQ_BASE    ] = 800;
    ePnt->Base[NUM_VOLT_BASE    ] = 380;
    ePnt->Base[NUM_CUR_BASE     ] = 2000;
    ePnt->Base[NUM_POWER_BASE   ] = 1000;
    ePnt->Base[NUM_1000_BASE    ] = 1000;
    ePnt->Base[NUM_HZ_BASE      ] = 800;
    ePnt->Base[NUM_BASE_REF_PID ] = 1000;
    //TestMakeOpis();

    return  ramPnt;
}
//****************************************************************************
// Перевод значения из iq формата в Float
//****************************************************************************
float IQ_to_FLOAT(float value, u8 numBase)
{
    MPOpis_type *ePnt;
    ePnt = &MPOpis;
    return (value * ePnt->BaseFloat[numBase]);
}
//****************************************************************************
// Перевод значения из float в iq формат
//****************************************************************************
float FLOAT_to_IQ(float value, u8 numBase)
{
    MPOpis_type              *ePnt;
    ePnt = &MPOpis;
    return (value * ePnt->BaseFloatConverse[numBase]);
}
//****************************************************************************
// Функция для фильтрации значений
//****************************************************************************
float FiltrValue(u16 tFiltr, float tekValue, float prevValue)
{
  float K1, K2, outValue;
  
  K1 = (float)tFiltr/((float)(tFiltr + INT_TIME_CALL_MICRO_mS));
  K2 = (float)INT_TIME_CALL_MICRO_mS/((float)(tFiltr + INT_TIME_CALL_MICRO_mS));
  
  outValue = prevValue * K1 + tekValue * K2;
  
  return outValue;
}
//**************************************************************************
// Вариант функции для целочисленных значений
//****************************************************************************

s32 Fy_x1x2y1y2x(s32 x1, s32 x2, s32 y1, s32 y2, s32 x)
{
    s32     tmp, dx;
// если функиция обратная линейной( масимальное значение по X = минимальном по Y)
// для расчета температуры IGBT
    if(x1 > x2)         
    {
        tmp = (y2-y1)*(x1-x);
        tmp += ((x1-x2)>>1);
        dx = (x1-x2); 
    }
    else
    {
        tmp = (y2-y1)*(x-x1);
        tmp += ((x2-x1)>>1);
        dx = (x2-x1);
    }
    if(tmp == 0)    
        return  y1;
    if(dx  == 0)    
        return  0x7FFFFFFFL; // Максимальное число
    tmp /= dx;
    tmp += y1;
    return  tmp;    //return (((y2-y1)*(x-x1))/(x2-x1))+y1;
}

//**************************************************************************
// Вариант функции для целочисленных беззнаковых значений
//****************************************************************************
u32 u_Fy_x1x2y1y2x(u32 x1, u32 x2, u32 y1, u32 y2, u32 x)
{
    u64     tmp;
    u32     dx;

    tmp = (y2-y1)*(x-x1);
    tmp += ((x2-x1)>>1);
    dx = (x2-x1);
    if(tmp == 0)    return  y1;
    if(dx  == 0)    return  0x7FFFFFFFL; // Максимальное число //ошибка
    tmp /= dx;
    tmp += y1;
    return  (u32)tmp;   //return (((y2-y1)*(x-x1))/(x2-x1))+y1;
}

//****************************************************************************
// Вариант функции для параметров в формате float
//****************************************************************************
float f32_Fy_x1x2y1y2x(float x1, float x2, float y1, float y2, float x)
{
    
  float     tmp, dx;

  tmp = (y2 - y1) * (x - x1);          // Умножение с округлением
  dx = x2 - x1;

  if(tmp == 0)    
      return  y1;
//  if(dx  == 0)    
//      return  MAX_IQ_POS; // Максимальное число
  
  tmp = tmp / dx;
  tmp += y1;
  
  return  tmp;                      // return (((y2-y1)*(x-x1))/(x2-x1))+y1;
}
//****************************************************************************
s16 Filtr(filtr_type *fPnt, s16 val, s16 Kfiltr)
{
    s32     add;

    add = fPnt->i;
    fPnt->i = add + ((((s32)val<<16) - add +((1<<Kfiltr)-1)) >> Kfiltr);
    return  fPnt->s[1];
}
//**************************************************************************
//8855
s16 FuncFORMULA_X_s16(FuncFORMULA_X_type *progPnt, u32 ramPnt, s16 kolvOperand)
{
    s32     arg1, arg2;
    s16     *FormPnt, oper;
    u16     *u16Pnt;
    s32     Bff[24], *s32Pnt;

    for(s32Pnt = Bff, u16Pnt = progPnt->Pnt_Arg; kolvOperand > 0; kolvOperand--){
        *s32Pnt++ = (s32)load_s16(ramPnt, *u16Pnt++);
    }

    arg1 = load_s16(ramPnt, progPnt->Pnt_formula);
    FormPnt = GetFormulaPnt((u16)arg1);
    for(s32Pnt = &Bff[8];;){
        if((u16)*FormPnt >= 24){
            save_s16(ramPnt, progPnt->Pnt_Out, (s16)arg1);
            return 0;
        }
        arg1 = Bff[*FormPnt++];
        oper = *FormPnt++;
        arg2 = Bff[*FormPnt++];
        switch(oper){
        case MUL_OPERATION:     arg1 = arg1 *  arg2;    break;
        case DIV_OPERATION:
            if(arg2 == 0)   arg1 = 0xFFFFFFFF;
            else            arg1 = arg1 /  arg2;
            break;
        case QUO_OPERATION:     arg1 = arg1 %  arg2;    break;
        case ADD_OPERATION:     arg1 = arg1 +  arg2;    break;
        case SUB_OPERATION:     arg1 = arg1 -  arg2;    break;
        case LSH_OPERATION:     arg1 = arg1 << arg2;    break;
        case RSH_OPERATION:     arg1 = arg1 >> arg2;    break;
        case LOW_OPERATION:     arg1 = arg1 <  arg2;    break;
        case LEQ_OPERATION:     arg1 = arg1 <= arg2;    break;
        case HEQ_OPERATION:     arg1 = arg1 >= arg2;    break;
        case EQU_OPERATION:     arg1 = arg1 == arg2;    break;
        case NEQ_OPERATION:     arg1 = arg1 != arg2;    break;
        case AND_OPERATION:     arg1 = arg1 &  arg2;    break;
        case OR_OPERATION:      arg1 = arg1 |  arg2;    break;
        case XOR_OPERATION:     arg1 = arg1 ^  arg2;    break;
        case LAND_OPERATION:    arg1 = arg1 && arg2;    break;
        case LOR_OPERATION:     arg1 = arg1 || arg2;    break;
        case NOT_OPERATION:     arg1 = !arg1;           break;
        case END_OPERATION:
        default : ;
        }
        *s32Pnt++ = arg1;
    }
}

//****************************************************************************
void setBit(void *ramPnt, u16 Nbit)
{
    u16     *u16Pnt;
    u16Pnt = ramPnt;
    u16Pnt[Nbit>>4] |= (1<<(Nbit &0xF));
}
//********************************************************
float iq_Filtr_prm(float valOld, float valNew, s16 Kfiltr)
{
  /*
    _iq     iq_tmp;
    
    iq_tmp = ((valNew - valOld) + ((1<<Kfiltr)-1)) >> Kfiltr;
    if(iq_tmp == 0) return  valNew;
    else            return  valOld + iq_tmp;
  */
  return 0;
}
//********************************************************
