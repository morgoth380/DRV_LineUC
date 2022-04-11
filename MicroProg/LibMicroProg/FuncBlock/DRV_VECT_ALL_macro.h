//#include    "vector.h"

//extern u16  cnt_Pusk;
//********************************************************
#define max3(U, V, W) (((U>V)&&(U>W))?U:((V>W)?V:W))
//********************************************************





//********************************************************
// Макрос по проверке на изменение состояния параметра при изменении от входа
// NAME - наименование параметра
// div  - делитель для приведения к float
//********************************************************
#define checkUstS16(NAME, div)                                                                \
    tmp = load_s16(ramPnt, progPnt->Pnt_##NAME);                                              \
    if( sPnt->prev.NAME != tmp ){                                                             \
        mPnt3->NAME    = int2f((s16)tmp , div);        /* Переписать в обменную структуру */  \
        sPnt->prev.NAME = tmp;                          /* Запомнить в State               */ \
        WrFramMicro(&nvAdr->NAME, &tmp, sizeof(s32));   /* Запомнить в nvState             */ \
        vect_cntr_flags.SysCalc = 1;                                                          \
    }
//********************************************************
// Макрос по проверке на изменение состояния параметра при изменении от входа
// NAME - наименование параметра
// div  - делитель для приведения к float
//********************************************************
#define checkUstF32( NAME )                                                                   \
    tmpF = load_float(ramPnt, progPnt->Pnt_##NAME);                                           \
    if( sPnt->prev.NAME!= tmpF ){                                                             \
        mPnt3->NAME    = tmpF;                         /* Переписать в обменную структуру */  \
        sPnt->prev.NAME = tmpF;                         /* Запомнить в State               */ \
        WrFramMicro(&nvAdr->NAME, &tmpF, sizeof(f32));  /* Запомнить в nvState             */ \
        vect_cntr_flags.SysCalc = 1;                                                          \
    }
//********************************************************
#define saveUstS16(NAME, mull)                                                                \
{                                                                                             \
    s32 tmps32;                                                                               \
    f32 tmpF = mPnt3->NAME;                             /* Получили новое                  */ \
    tmps32 = f2int( tmpF, mull);                        /* Привели к s16                   */ \
    sPnt->prev.NAME = tmps32;                           /* Запомнить в State               */ \
    save_s16(ramPnt, progPnt->Pnt_##NAME ,(s16)tmps32); /* Переписать уставку              */ \
    WrFramMicro(&nvAdr->NAME, &tmps32, sizeof(s32));    /* Запомнить в nvState             */ \
}
//********************************************************
//********************************************************
#define saveUstF32(NAME)                                                                  \
{                                                                                         \
    f32 tmpF = mPnt3->NAME;                            /* Получили новое                  */ \
    sPnt->prev.NAME = tmpF;                         /* Запомнить в State               */ \
    save_float(ramPnt, progPnt->Pnt_##NAME , tmpF );/* Переписать уставку              */ \
    WrFramMicro(&nvAdr->NAME, &tmpF, sizeof(f32));  /* Запомнить в nvState             */ \
}
//********************************************************

#ifndef _PROJECT_FOR_CCS_
    #define installDrv(drvType)
#endif
//********************************************************
//********************************************************
  // Запретить прерывание от таймера
//********************************************************
#ifdef _PROJECT_FOR_CCS_
    #define disableTmr0interrupt()  PieCtrlRegs.PIEIER1.bit.INTx7   = 0; 
#else
    #define initCpuTimer_vect()
    #define disableTmr0interrupt()
#endif
//********************************************************
//********************************************************
