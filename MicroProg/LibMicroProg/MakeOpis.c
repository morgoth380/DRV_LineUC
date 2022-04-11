#include "Global_include.h"
//#include "commonmicro.h"
//#include "MakeOpis.h"

// These vars store address of the last changed UST
u16 GlobalCurGrpPrm = NPAR_NGRP_CLEAR; // This is used in FuncBlock/SS_PARAMS.h
u16 LocalCurGrpPrm  = NPAR_NGRP_CLEAR; // This is used here to temporary store group & num
void CheckUstReload (MicroProg_type * pntM);
//**************************************************************************
void TestMakeOpis(void)
{
    wm_type wmem, *wm;
    s16 Kolv, Adr, tmp;

    wm = &wmem;

    for(Kolv = 0; Kolv < 20; Kolv++){
        wm->AdrPrm = LINEadr + 3;
        wm->Val2byte = Kolv;
        tmp = ModbusMakeOpis(wm, 0);
        tmp = ModbusMakeOpis(wm, 1);
    }

    for(Kolv = 75, Adr = LINEadr; Kolv > 0; Kolv--, Adr++){
        wm->AdrPrm = Adr;
        tmp = ModbusMakeOpis(wm, 1);
        if(tmp)
            for(;;) ;
    }

}
//**************************************************************************
Drv_err_type GetFlagsPrm(wm_type *wm)
{
    MPOpis_type *ePnt;
    const OpisPrm_type *oPnt;
    const OpisGrp_type *gPnt;
    //const Functional_type *fPnt;
    u16 NopisPrm, NopisGrp, *NNNPnt, adr, tmp;

    ePnt = &MPOpis;
    //PAN fPnt = &curFunct;

  // PAN if(flagIsFirstCall == 1)    return  SysPrmInitBad_err;
    wm->mosaf = NULL;
    if(wm->AdrPrm & LINEadr){
        adr = wm->AdrPrm & (u16)(~LINEadr);        // Сбросить бит признака адреса
        if(adr >= ePnt->kolvPrmOpis)
            return BadAdr_err;

        oPnt = &ePnt->PntOpisUst[adr];    //указатель на описатель параметра

    }
    else{
        LocalCurGrpPrm = wm->AdrPrm; // Save group and # for ref menu

        NopisPrm = GetNumPrmFromAdrPrm(wm->AdrPrm);
        NopisGrp = GetNumGrpFromAdrPrm(wm->AdrPrm);

        if(NopisGrp >= ePnt->kolvGrpOpis)
            return BadAdr_err;

        gPnt = &ePnt->PntOpisGrp[NopisGrp];

        NNNPnt = AddAdr(ePnt->PntMicroProg, gPnt->PntNumOpisPrm); //указатель на описатель параметров
        tmp = NNNPnt[NopisPrm];

        if (tmp == 0xFFFF)
            return BadAdr_err;

        oPnt = AddAdr(ePnt->PntMicroProg, tmp); //указатель на описатель параметра
        if(NopisPrm >= gPnt->kolvPrmInGrp)
            return BadAdr_err;

        adr = (u16)(((u32) oPnt) - ((u32)(ePnt->PntOpisUst))) / sizeof(OpisPrm_type);
        adr |= LINEadr;

        wm->AdrPrm = adr;
    }

    wm->Flags = oPnt->Flags;
    wm->NumBase = (u16) (wm->Flags >> BaseNumber_fprm) & BaseNumber_fprm_mask;
    wm->valPnt = AddAdr(ePnt->ramPnt, oPnt->AdrVal);
    wm->TypeVar = (u16)((wm->Flags >> TypeVar_fprm) & TypeVar_fprm_mask);    //
    wm->kolvShort = ((1UL << wm->TypeVar) & mask4byteFormat) ? 2 : 1;
    wm->oPnt = oPnt;

#ifdef _PROJECT_FOR_CCS_
	//#warning !!!Незабыть вернуть (функционал) !!!!
    if((oPnt->maskFuncEnable & fPnt->functMask) == 0){ //Условие запрета доступа к уставке
        return NotInCurFunctional_err;
    }
#endif
    return Ok_err;
}
//****************************************************************************
Drv_err_type ModbusMakeOpis(wm_type *wm, s16 Rd)
{
    Drv_err_type code;
    static u16 adr;

    if((adr == wm->AdrPrm) && (!Rd)){
        code = Ok_err;
    }
    code = GetFlagsPrm(wm);

    if(code == NotInCurFunctional_err){
        wm->Val2byte = 0;
        wm->StepEdIsm = 0;
        return Ok_err;
    }

    if((wm->TypeVar == sec2Frmt) && Rd){
        wm->Val2byte = (u16)(wm->s32.Value >> 16);
        return Ok_err;
    }

    if(code == Ok_err){
        if(!Rd){
            if(wm->kolvShort == 2){
                wm->s32.Value = wm->Val2byte & 0xFFFF;
                return code;
            }
            else if(wm->TypeVar == sec2Frmt){
                wm->s32.Value |= ((u32)wm->Val2byte << 16);
                wm->AdrPrm--;
            }
            else if((wm->Flags & TypeVar_fprm_mask) == uns16Frmt){
                    wm->s32.Value = (u16)wm->Val2byte;
            }
            else{
                wm->s32.Value = (s16)wm->Val2byte;
            }
        }
        code = MakeOpis(wm, Rd);
        if(!Rd){
            code = MakeOpis(wm, !Rd);
        }

        if(Rd){
            wm->Val2byte = (u16) wm->s32.Value & 0xFFFF;
        }
    }

    //перекодировать код возврата
    return code;
}

//****************************************************************************
Drv_err_type MakeOpis(wm_type *wm, s16 Rd){
    MPOpis_type *ePnt;
    const OpisPrm_type *oPnt;
    s16 Base;
    Drv_err_type code;
    u16 rdkAtr;    
    u32 Flags;
    float Fval;
    s32 IQval;
    

    ePnt = &MPOpis;
    code = GetFlagsPrm(wm);

    if(code != Ok_err){
        if(Rd /*&& code == NotInCurFunctional_err*/){                                 //Если идет чтение и уставка не принадлежит текущей версии, то обнуляем
            wm->Val2byte = 0;
            wm->StepEdIsm = 0;
            return Ok_err;
        }
        else
            return code;
    }

    oPnt = wm->oPnt;
    memcpy(wm->LedName, oPnt->LedName, sizeof(oPnt->LedName));
    Flags = wm->Flags;
    rdkAtr = (u16)((Flags >> EditType_fprm) & EditType_fprm_mask); //

    if(Flags & (1UL << FormatChange_fprm))
        wm->StepEdIsm = *(s16 *)AddAdr(ePnt->ramPnt, oPnt->StepEdIsm);
    else
        wm->StepEdIsm = oPnt->StepEdIsm;

    if(wm->TypeVar == sec2Frmt)
        return BadAdr_err;

    if(wm->kolvShort == 2){     //параметр 4 байтный
        wm->s32.LowLim  = ((u32) oPnt->LowLim  & 0xFFFF) | (((u32) oPnt[1].LowLim) << 16);
        wm->s32.HighLim = ((u32) oPnt->HighLim & 0xFFFF) | (((u32) oPnt[1].HighLim) << 16);

        if(Rd){
            wm->s32.Value = *(s32 *)(wm->valPnt);
            if(code = MOspecial(wm, Rd)) return code;
        }
        else{
            if(rdkAtr == Non_RdkPrm)
                return NonRdk_err;

            if((rdkAtr == Stop_RdkPrm) && (!DrvIsStop()))
                return NonRdk_err;

            if(wm->TypeVar == floatFrmt){
                if((wm->f.Value < wm->f.LowLim) || (wm->f.Value > wm->f.HighLim))
                    return Lim_err;
            }
            else{
                if((wm->s32.Value < wm->s32.LowLim) || (wm->s32.Value > wm->s32.HighLim))
                    return Lim_err;
            }

            if(code = MOspecial(wm, Rd))
                return code;

            *(s32 *)(wm->valPnt) = wm->s32.Value;

            if(wm->mosaf != NULL)
                wm->mosaf(wm, Rd);

            // Save group and # for ref menu (FuncBlock/SS_PARAMS.h)
            GlobalCurGrpPrm = LocalCurGrpPrm - 1; // minus one because we want address of the first UST

            if(Flags & (1UL << nonVolatile_fprm)){    //сохранение в nvRam
                WrFramMicro((void *)((wm->AdrPrm & (u16)(~LINEadr)) * sizeof(u32)), &wm->s32.Value, sizeof(s32));
                //GlobalCurGrpPrm = LocalCurGrpPrm;
            }
        }
    }                //wm->Flags
    else{       //параметр 2 байтный
        if(wm->NumBase == 0){       //

            if(Rd){
                if((wm->Flags & TypeVar_fprm_mask) == uns16Frmt)
                    wm->s32.Value = *(u16 *) (wm->valPnt);
                else
                    wm->s32.Value = *(s16 *) (wm->valPnt);
            }

            if(Flags & (1UL << LimChange_fprm)){      //переменные пределы
                if((wm->Flags & TypeVar_fprm_mask) == uns16Frmt){
                    wm->s32.LowLim = *(u16 *)AddAdr(ePnt->ramPnt, oPnt->LowLim);
                    wm->s32.HighLim = *(u16 *)AddAdr(ePnt->ramPnt, oPnt->HighLim);
                }
                else{
                    wm->s32.LowLim = *(s16 *)AddAdr(ePnt->ramPnt, oPnt->LowLim);
                    wm->s32.HighLim = *(s16 *)AddAdr(ePnt->ramPnt, oPnt->HighLim);
                }
            }
            else{
                if((wm->Flags & TypeVar_fprm_mask) == uns16Frmt){
                    wm->s32.LowLim = (u16)oPnt->LowLim;
                    wm->s32.HighLim = (u16)oPnt->HighLim;
                }
                else{
                    wm->s32.LowLim = oPnt->LowLim;
                    wm->s32.HighLim = oPnt->HighLim;
                }
            }
        }
        else{   //не нулевая база

          Base = ePnt->Base[wm->NumBase];
  
          if(Rd){
              wm->f.Value = *(float *)(wm->valPnt);
              if(wm->f.Value > 0)
                  wm->s32.Value = (s32)((wm->f.Value * (float)Base) + 0.5f);    // Перевод значения параметра с float в s32
              else
                  wm->s32.Value = (s32)((wm->f.Value * (float)Base) - 0.5f);    // Перевод значения параметра с float в s32

          }

          if(Flags & (1UL << LimChange_fprm)){                                  // Переменные пределы
              wm->f.LowLim = *(float *)AddAdr(ePnt->ramPnt, oPnt->LowLim);
              wm->f.HighLim = *(float *)AddAdr(ePnt->ramPnt, oPnt->HighLim);
          }
          else{
              wm->f.LowLim = (float)oPnt->LowLim / 4096.0f;
              wm->f.HighLim = (float)oPnt->HighLim / 4096.0f;
          }
          
          if(wm->f.LowLim > 0)     
              wm->s32.LowLim = (s32)((wm->f.LowLim * (float)Base) + 0.5f);
          else 
              wm->s32.LowLim = (s32)((wm->f.LowLim * (float)Base) - 0.5f);
          
          if(wm->f.HighLim > 0)   
              wm->s32.HighLim = (s32)((wm->f.HighLim * (float)Base) + 0.5f);
          else
              wm->s32.HighLim = (s32)((wm->f.HighLim * (float)Base) - 0.5f);
      }

        if(Rd){
            if(code = MOspecial(wm, Rd))
                return code;
        }
        else //если поступил запрос на запись в FRAM
        {
            if(rdkAtr == Non_RdkPrm)
                return NonRdk_err;

            if((rdkAtr == Stop_RdkPrm) && (!DrvIsStop()))
                return NonRdk_err;

            if((wm->s32.Value < wm->s32.LowLim) || (wm->s32.Value > wm->s32.HighLim))
                return Lim_err;

            if(wm->NumBase == 0){
                if(code = MOspecial(wm, Rd))
                    return code;

                *(s16 *)(wm->valPnt) = (s16) wm->s32.Value;
                if(wm->mosaf != NULL)
                    wm->mosaf(wm, Rd);
            }
            else{   // Параметр с базой

               if(code = MOspecial(wm, Rd)){
                    return code;
               }

               *(float *)(wm->valPnt) = (float)wm->s32.Value / (float)Base;

               if(wm->mosaf != NULL){
                    wm->mosaf(wm, Rd);
               }
              
               GlobalCurGrpPrm = LocalCurGrpPrm;            // Save group and # for ref menu (FuncBlock/SS_PARAMS.h)
               if(Flags & (1UL << nonVolatile_fprm)){       // сохранение в nvRam
                   Fval = *(float *)(wm->valPnt);           // Получить значение параметра в формате float
                   IQval = (s32)(Fval * 16777216.0f);       // Конвертировать float в IQ24 
                  
                   WrFramMicro((void *)((wm->AdrPrm & (u16)(~LINEadr)) * sizeof(s32)), &IQval, sizeof(s32));  // Сохранить значение параметра в формате IQ24
               }
               
               return Ok_err;
            }

            GlobalCurGrpPrm = LocalCurGrpPrm;            // Save group and # for ref menu (FuncBlock/SS_PARAMS.h)

            if(Flags & (1UL << nonVolatile_fprm)){      //сохранение в nvRam
                WrFramMicro((void *)((wm->AdrPrm & (u16)(~LINEadr)) * sizeof(u32)), &wm->s32.Value, sizeof(s32));

                //RdFramMicro((void*)&temp, (void *)((wm->AdrPrm & ~LINEadr) * sizeof(u32)), sizeof(s32));
            }
        }
    }

    return Ok_err;
}
//****************************************************************************
int MicroProgWrCoil(s16 Adr, s16 val)
{
    MPOpis_type *ePnt;
    const MicroProg_type *mPnt;
    StateFuncMODBUS_0X05_type *sPnt;
    u16 tmp;

    ePnt = &MPOpis;
    mPnt = ePnt->PntMicroProg;
    if(Adr >= (mPnt->kolv_Mb_0x05 << 3))
        return Illegal_DataAdr_merr;  //err DataAdr
    sPnt = (StateFuncMODBUS_0X05_type *)((u32) ePnt->ramPnt + mPnt->PntRam_Mb_0x05);
    sPnt = &sPnt[Adr >> 3];
    tmp = 1 << (Adr & 0x7);
    if(val){
        if(sPnt->mask & tmp)
            sPnt->val |= tmp;      // Установить бит установки
        else
            return 2;                          //err   Failure
    }
    else{
        if(sPnt->mask & (tmp << 8))
            sPnt->val |= (tmp << 8);                    // Установить бит сброса
        else
            return 2;                          //err   Failure
    }
    return 0;
}
//****************************************************************************
#ifndef _PROJECT_FOR_CCS_
extern u16 swapU16(u16 Val);
#else
u16 swapU16(u16 Val)
{
    Val = (Val >> 8) + (Val << 8);
    return Val;
}
#endif //_PROJECT_FOR_CCS_
void StartExtMem(void)
{
    u16 usTmp1, usTmp2;
    //u32 Zer, *u32Pnt;
    //s16 i;
    MicroProg_type *pntM;	   
    
    pntM = (MicroProg_type *)MicroProgTabl;

    RdFramMicro(&usTmp1, &nv.MemSignatura, sizeof(u16));
    RdFramMicro(&usTmp2, &nv.MicroSignatura, sizeof(u16));           
    
    // Проверка на новую микропрограмму и восстановление уже настроенных уставок
    CheckUstReload (pntM);
    
//    i = InitFlagsFunctional();
//    
//    if((usTmp1 != pntM->SignatCoreMicro) //DEFAULT UST!
//            || (usTmp2 != MPOpis.PntMicroProg->CRC16_NV) || i == 2){
//        if(i != 2){    //2 не настроенный функционал
//            for(Zer = i = 0, u32Pnt = NULL; i < ((u32)&nv.stopTime)/4; i++, u32Pnt++)
//                WrFramMicro(u32Pnt, &Zer, sizeof(u32));
//        }
//        Ust_Copy(PWR_to_RAM);
//        Ust_Copy(RAM_to_NV);
//        usTmp1 = pntM->SignatCoreMicro;
//        usTmp2 = MPOpis.PntMicroProg->CRC16_NV;
//        WrFramMicro( &nv.MemSignatura, &usTmp1, sizeof(u16));
//        WrFramMicro( &nv.MicroSignatura, &usTmp2, sizeof(u16));
//    }
    
    Ust_Copy(NV_to_RAM);	

}
//****************************************************************************
int Ust_Copy(UstCopy_enum Ust_src_to_dest)
{
    const OpisPrm_type *oPnt;
    u16 Adr, *u16Pnt, kolv, f4byte;
    s32 Val, *s32Pnt;
    float Temp;
    UstFlag_type    wm_flag;
    wm_type wmem;

    u16Pnt = (u16 *)(MPOpis.PntMicroProg->PntPwrValUst + (u32)MPOpis.PntMicroProg);
    kolv = MPOpis.PntMicroProg->kolvUst;
    oPnt = MPOpis.PntOpisUst;
    s32Pnt = NULL;
    f4byte = 0;
    for(Adr = LINEadr; kolv > 0; kolv--, Adr++, oPnt++, u16Pnt++){
        if(f4byte){
            f4byte = 0;
            continue;
        }
        wmem.AdrPrm = Adr;
        GetFlagsPrm(&wmem);
        wm_flag.all = wmem.Flags;
        if(Ust_src_to_dest == PWR_to_RAM){ // Восстанавливаем значения Default u16Pnt
            if(wmem.kolvShort == 1){
              if(wmem.NumBase == 0){
                    *(s16 *)wmem.valPnt = *u16Pnt;
              }
              else{
                     //Temp = (float)Val / 16777216.0f;
                     //*(float *)wmem.valPnt = Temp;             // PAN
                    *(s32 *)wmem.valPnt = (s32)(*u16Pnt) * 4096;
              }
            }
            else{
                *(s32 *)wmem.valPnt = (s32) u16Pnt[0] | ((s32) u16Pnt[1] << 16);
                f4byte = 1;
            }
        }
        else if(Ust_src_to_dest == RAM_to_NV){
            if((wmem.kolvShort == 1) && (wmem.NumBase == 0)){
                Val = *(s16 *)wmem.valPnt;
                WrFramMicro(s32Pnt++, &Val, sizeof(u32));
            }
            else if((wmem.kolvShort == 1) && (wmem.NumBase != 0)){
                Val = *(s32 *)wmem.valPnt;
                //Val = (s32)((float)Val * 16777216.0f);                      //PAN
                WrFramMicro(s32Pnt++, &Val, sizeof(u32));
            }
            else{
                Val = *(s32 *)wmem.valPnt;
                WrFramMicro(s32Pnt++, &Val, sizeof(u32));
                f4byte = 1;
                s32Pnt++;
            }
        }
        else{       //NV_to_RAM
            RdFramMicro(&Val, s32Pnt++, sizeof(u32));
            // Проверяем, если флаг параметра является Энергозависимым, то обнуляем значение уставки
            if (!wm_flag.bit.nonVolatile)       Val = 0;
            
            if((wmem.kolvShort == 1) && (wmem.NumBase == 0))
                *(s16 *) wmem.valPnt = (s16)Val;
            else if((wmem.kolvShort == 1) && (wmem.NumBase != 0)){
              Temp = (float)Val / 16777216.0f;
              *(float *)wmem.valPnt = Temp;             // PAN
            }
            else{
                *(s32 *)wmem.valPnt = Val;
                f4byte = 1;
                s32Pnt++;
            }
        }
    }
    return 0;
}
//****************************************************************************
u16 InitFlagsFunctional(void) 
{    
    FileFunctional_type 	FlagsFunctional, FlagsFunctionalPrev;
    //u16				crc;
    Functional_type 		*fPnt;
        
    fPnt = &curFunct;
    fPnt->functMask = 0;
    RdFramMicro(&FlagsFunctional, &nv.fileCurFunct, sizeof(FileFunctional_type));
    RdFramMicro(&FlagsFunctionalPrev, &nv.fileCurFunctPrev, sizeof(FileFunctional_type));

    // Заблокировал прогрузку функционала
//    //!!!!! Принудительная установка функционала в приводе в полный формат !!!!!
//    FlagsFunctional.curFunct.functMask = 0xFFFF;
//    crc = crcPacket((unsigned char *) &FlagsFunctional, sizeof(FlagsFunctional.curFunct)); 
//    FlagsFunctional.crc = crc;
//    WrFramMicro(&nv.fileCurFunct, &FlagsFunctional, sizeof(FileFunctional_type));
//  
    
//    crc = crcPacket((unsigned char *) &FlagsFunctional, sizeof(FlagsFunctional.curFunct));
//    if(crc != swapU16(FlagsFunctional.crc)){
//        return 1;
//    }
//
//    fPnt->functMask = FlagsFunctional.curFunct.functMask;
//
//    if((FlagsFunctional.curFunct.functMask != FlagsFunctionalPrev.curFunct.functMask) || (FlagsFunctional.crc != FlagsFunctionalPrev.crc)){
//        WrFramMicro(&nv.fileCurFunctPrev, &FlagsFunctional, sizeof(FileFunctional_type));
//        return 2;
//    }

    return 0;
}
//****************************************************************************
// Общая структура памяти, для работы с разными данными во время восстановления уставок.
typedef union	{
  s32	    	ustArray[2048];
  s16	    	addrArray[4096];
  nvParamAdr_type nvStruct;
}Backup_memory_type;

//Backup_memory_type  backupMem;

void CheckUstReload (MicroProg_type * pntM){
    u16  grp, par, lineAdr, modbusAdr, microCRC, microKolUst, i, crcStruct, size, nvCrcStruct;
    MPOpis_type *ePnt;
    const OpisPrm_type *oPnt;
    const OpisGrp_type *gPnt;
    u16 NopisPrm, NopisGrp, *NNNPnt, adr, tmp;
    Backup_memory_type  backupMem;   
    s32 Val, *s32Pnt;          
    u16 usTmp1, usTmp2;
    void	*nvAdr1;
    u16		UstRecoveryFlagValue;
    
    // 0. Сделать проверку CRC данных адресов параметров - если не совпал, сделать новое кэширование адресов.
    // Читаем массив кэша адресов уставок в локальную память
    RdFramMicro(&backupMem.nvStruct.crcMicro, &nv.UstModbusAdr.crcMicro, sizeof(nvParamAdr_type)-sizeof(u16)); 
    size = sizeof(nvParamAdr_type)-sizeof(u16);
    crcStruct = Crc((u8*)&backupMem.nvStruct.crcMicro, size);
     RdFramMicro(&nvCrcStruct, &nv.UstModbusAdr.crcStruct, sizeof(u16)); 

    // Если CRC совпадают, значит таблица корректная, можно ее проверять. Иначе сразу идем на кэширование адресов
    if (crcStruct == nvCrcStruct) {
    
      // 1. Проверить текущую микропрограмму
      RdFramMicro(&usTmp1, &nv.MemSignatura, sizeof(u16)); // Данные о старой МП 
      RdFramMicro(&usTmp2, &nv.MicroSignatura, sizeof(u16)); 
      RdFramMicro(&microCRC, &nv.UstModbusAdr.crcMicro, sizeof(u16));    
      
      // Если все элементы совпадают то ничего не надо делать, все актуальное
      if((usTmp1 == pntM->SignatCoreMicro) && (usTmp2 == pntM->CRC16_NV) && (pntM->CrcMicroprog == microCRC)){
        return;
      }      
          
      // 2. Делаем копию текущих уставок из Фрам в ОЗУ
      RdFramMicro(&microKolUst, &nv.UstModbusAdr.UstKolv, sizeof(u16)); // Читаем старое кол-во уставок
      s32Pnt = (s32*)&nv.MicroProg;  // Адрес начала уставок микропрограммы - по идее 0 адрес  
      for (i=0; i<microKolUst; i++){
        RdFramMicro(&Val, s32Pnt++, sizeof(u32));
        backupMem.ustArray[i] = Val;      
      }
      
      // 3.Выполнить восстановление уставок по умолчанию
      Ust_Copy(PWR_to_RAM);
      Ust_Copy(RAM_to_NV);     


      // 4. Выполняем восстановление уставок
      // Из массива nv.UstModbusAdr берем подряд все адреса Модбас, находим его 
      // новый линейник и копируем туда его значение из массива backupMem.ustArray[i]
      for (i=0; i<microKolUst; i++){ // Перебор всех уставок
        RdFramMicro(&modbusAdr, &nv.UstModbusAdr.UstAdr[i], sizeof(u16)); // Читаем модбас адрес по индексам
        if (modbusAdr == 14*128+03) {
          modbusAdr ++;
          modbusAdr --;
        }
        wmem.AdrPrm = modbusAdr;        
        ePnt = &MPOpis;
        NopisPrm = GetNumPrmFromAdrPrm(wmem.AdrPrm);
        NopisGrp = GetNumGrpFromAdrPrm(wmem.AdrPrm);        
        if(NopisGrp >= ePnt->kolvGrpOpis)
          continue;        
        gPnt = &ePnt->PntOpisGrp[NopisGrp];
        if (gPnt->kolvPrmInGrp == 0) 
          continue;        
        NNNPnt = AddAdr(ePnt->PntMicroProg, gPnt->PntNumOpisPrm); //указатель на описатель параметров
        tmp = NNNPnt[NopisPrm];        
        if (tmp == 0xFFFF)
          continue;        
        oPnt = AddAdr(ePnt->PntMicroProg, tmp); //указатель на описатель параметра
        if(NopisPrm >= gPnt->kolvPrmInGrp)
          continue;        
        adr = (u16)(((u32) oPnt) - ((u32)(ePnt->PntOpisUst))) / sizeof(OpisPrm_type);
        adr |= LINEadr;        
        wmem.AdrPrm = adr & (u16)(~LINEadr);
        lineAdr = wmem.AdrPrm; // Возвращается линейный адрес
        
        s32Pnt = (s32*)&nv.MicroProg;  // Адрес начала уставок микропрограммы - по идее 0 адрес  
        s32Pnt += lineAdr; // Адрес в соответствии с новым линейным адресом!!
        Val = backupMem.ustArray[i]; // Берем значение уставки из копии по ее старому линейному адресу!!
        WrFramMicro(s32Pnt, &Val, sizeof(u32)); // Записываем в фрам.
      }
      
      // Уставки восстановлены. А теперь надо кинуть в систему команду, чтобы все остальные настройки втянулись из системы параметров
      // а не тянулись из FRAM. Это касается структур NV. Вот с ними могут быть проблемы, т.к. адреса в FRAM могут не совпадать
      // Эта команда - восстановление заводских, тогда все втянется из уставок.
      nvAdr1 = (void *)((u32)&nv.UstRecoveryFlag);
      UstRecoveryFlagValue = 1;
      WrFramMicro( nvAdr1, &UstRecoveryFlagValue, sizeof(u16));      
            
      // Переходим на кэширование новых адресов - для этого просто выйти из условия.
      
//      // ТЕСТ ЧТЕНИЯ ДАННЫХ    
//      s32Pnt = (s32*)&nv.MicroProg;  // Адрес начала уставок микропрограммы - по идее 0 адрес  
//      for (i=0; i<microKolUst; i++){
//        if (i==415) {
//          i++;
//          i--;
//        }          
//        RdFramMicro(&Val, s32Pnt++, sizeof(u32));
//        //backupMem.ustArray[i] = Val;      
//      }      
    } else { // if (!crcStruct)
      // Если CRC на кэш адресов не совпал, возможно переход со старой версии или что-то новое
      // Поэтому сделаем сброс на Default
      Ust_Copy(PWR_to_RAM);
      Ust_Copy(RAM_to_NV);           
    }
    
    // 4. Кэширование адресов в память.
    // В тупую сканируем все возможные Модбас адреса, а при ошибке прекращаем
    // Заносим найденные адреса в массив, по их фактическим линейным адресам.
    for (grp = 0; grp < 100; grp ++) {
      for (par = 0; par < 100; par ++) {
        modbusAdr = grp*128 + par;
        wmem.AdrPrm = modbusAdr;
        
        ePnt = &MPOpis;
        NopisPrm = GetNumPrmFromAdrPrm(wmem.AdrPrm);
        NopisGrp = GetNumGrpFromAdrPrm(wmem.AdrPrm);
        
        if(NopisGrp >= ePnt->kolvGrpOpis)
          break;
        
        gPnt = &ePnt->PntOpisGrp[NopisGrp];
        if (gPnt->kolvPrmInGrp == 0) 
          break;
        
        NNNPnt = AddAdr(ePnt->PntMicroProg, gPnt->PntNumOpisPrm); //указатель на описатель параметров
        tmp = NNNPnt[NopisPrm];
        
        if (tmp == 0xFFFF)
          continue;
        
        oPnt = AddAdr(ePnt->PntMicroProg, tmp); //указатель на описатель параметра
        if(NopisPrm >= gPnt->kolvPrmInGrp)
          break;
        
        adr = (u16)(((u32) oPnt) - ((u32)(ePnt->PntOpisUst))) / sizeof(OpisPrm_type);
        adr |= LINEadr;
        
        wmem.AdrPrm = adr & (u16)(~LINEadr);
        //-----------------------------------------
        lineAdr = wmem.AdrPrm; // Возвращается линейный адрес
        WrFramMicro( &nv.UstModbusAdr.UstAdr[lineAdr], &modbusAdr, sizeof(u16)); // 			        
      }
    }
    modbusAdr = MPOpis.PntMicroProg->CrcMicroprog;
    WrFramMicro( &nv.UstModbusAdr.crcMicro, &modbusAdr, sizeof(u16));
        
    modbusAdr = MPOpis.PntMicroProg->kolvUst;
    WrFramMicro( &nv.UstModbusAdr.UstKolv, &modbusAdr, sizeof(u16));

    // Вычитать все данные в локальный массив и посчитать на него CRC
    RdFramMicro(&backupMem.nvStruct.crcMicro, &nv.UstModbusAdr.crcMicro, sizeof(nvParamAdr_type)-sizeof(u16)); 
    size = sizeof(nvParamAdr_type)-sizeof(u16);
    crcStruct = Crc((u8*)&backupMem.nvStruct.crcMicro, size);
    WrFramMicro( &nv.UstModbusAdr.crcStruct, &crcStruct, sizeof(u16));

    usTmp1 = pntM->SignatCoreMicro;
    usTmp2 = MPOpis.PntMicroProg->CRC16_NV;
    WrFramMicro( &nv.MemSignatura, &usTmp1, sizeof(u16));
    WrFramMicro( &nv.MicroSignatura, &usTmp2, sizeof(u16));      
    
    
//    // ТЕСТ ЧТЕНИЯ ДАННЫХ    
//    for (grp = 0; grp < 100; grp ++) {
//      RdFramMicro(&modbusAdr, &nv.UstModbusAdr.UstAdr[grp], sizeof(u16));
//    }
	//**************************************************
    
    // Освободить память.
}