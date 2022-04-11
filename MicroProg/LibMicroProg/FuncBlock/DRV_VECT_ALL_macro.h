//#include    "vector.h"

//extern u16  cnt_Pusk;
//********************************************************
#define max3(U, V, W) (((U>V)&&(U>W))?U:((V>W)?V:W))
//********************************************************





//********************************************************
// ������ �� �������� �� ��������� ��������� ��������� ��� ��������� �� �����
// NAME - ������������ ���������
// div  - �������� ��� ���������� � float
//********************************************************
#define checkUstS16(NAME, div)                                                                \
    tmp = load_s16(ramPnt, progPnt->Pnt_##NAME);                                              \
    if( sPnt->prev.NAME != tmp ){                                                             \
        mPnt3->NAME    = int2f((s16)tmp , div);        /* ���������� � �������� ��������� */  \
        sPnt->prev.NAME = tmp;                          /* ��������� � State               */ \
        WrFramMicro(&nvAdr->NAME, &tmp, sizeof(s32));   /* ��������� � nvState             */ \
        vect_cntr_flags.SysCalc = 1;                                                          \
    }
//********************************************************
// ������ �� �������� �� ��������� ��������� ��������� ��� ��������� �� �����
// NAME - ������������ ���������
// div  - �������� ��� ���������� � float
//********************************************************
#define checkUstF32( NAME )                                                                   \
    tmpF = load_float(ramPnt, progPnt->Pnt_##NAME);                                           \
    if( sPnt->prev.NAME!= tmpF ){                                                             \
        mPnt3->NAME    = tmpF;                         /* ���������� � �������� ��������� */  \
        sPnt->prev.NAME = tmpF;                         /* ��������� � State               */ \
        WrFramMicro(&nvAdr->NAME, &tmpF, sizeof(f32));  /* ��������� � nvState             */ \
        vect_cntr_flags.SysCalc = 1;                                                          \
    }
//********************************************************
#define saveUstS16(NAME, mull)                                                                \
{                                                                                             \
    s32 tmps32;                                                                               \
    f32 tmpF = mPnt3->NAME;                             /* �������� �����                  */ \
    tmps32 = f2int( tmpF, mull);                        /* ������� � s16                   */ \
    sPnt->prev.NAME = tmps32;                           /* ��������� � State               */ \
    save_s16(ramPnt, progPnt->Pnt_##NAME ,(s16)tmps32); /* ���������� �������              */ \
    WrFramMicro(&nvAdr->NAME, &tmps32, sizeof(s32));    /* ��������� � nvState             */ \
}
//********************************************************
//********************************************************
#define saveUstF32(NAME)                                                                  \
{                                                                                         \
    f32 tmpF = mPnt3->NAME;                            /* �������� �����                  */ \
    sPnt->prev.NAME = tmpF;                         /* ��������� � State               */ \
    save_float(ramPnt, progPnt->Pnt_##NAME , tmpF );/* ���������� �������              */ \
    WrFramMicro(&nvAdr->NAME, &tmpF, sizeof(f32));  /* ��������� � nvState             */ \
}
//********************************************************

#ifndef _PROJECT_FOR_CCS_
    #define installDrv(drvType)
#endif
//********************************************************
//********************************************************
  // ��������� ���������� �� �������
//********************************************************
#ifdef _PROJECT_FOR_CCS_
    #define disableTmr0interrupt()  PieCtrlRegs.PIEIER1.bit.INTx7   = 0; 
#else
    #define initCpuTimer_vect()
    #define disableTmr0interrupt()
#endif
//********************************************************
//********************************************************
