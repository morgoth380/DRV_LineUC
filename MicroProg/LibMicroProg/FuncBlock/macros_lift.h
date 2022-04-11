/************************** (C) Copyright 2013 Triol ***************************
* File Name          : macros_lift.h
* Author             : Kats Evgeniy
* TS Idea            : 2013.02.26
* TS Version         : 2013.02.26
* Description        : ���� �������� �������, ������� ��������� ������ ���������
*                      "������ ����������" ��� ��������� ����������
* Fixed by:          : Andrey Kozlov
* Version            : 2013.09.09
*                    : ���������� ������������ ��������, ���������������� ��������
*                    : ���� �������� NVState.
*******************************************************************************/
#ifdef    GET_FUNC_CODE
// ���� ������������ ��������
typedef enum{
    BaseTypeF_FREQ,
    BaseTypeF_RAD,
    BaseTypeF_CYCLE,
}BaseTypeF_type;
// ��������� ������������� �������
typedef struct{
    u16  ChannalTask1;                  // ����� ������� 1
    u16  ChannalControl1;               // ����� ����������1
    u16  TymeUP;                        // ����� �������
    u16  TymeDOWN;                      // ����� ����������
    u16  SelBase;                       // ����� ������ ���������
    u16  PriorSpeed;                    // ��������� ��������
    u16  SpeedDI;                       // �������� �� DI
    f32  NomFreq;                       // ����������� ������� (��������)
    f32  FreqAligning;                  // ������� (��������) ������������
    u16  Speed1;                        // 1-��������
    u16  Speed4;                        // 4-��������
    u16  Forward;                       // ������
    u16  Revers;                        // �����
    u16  Pusk1;                         // ����1
    u16  Pusk2;                         // ����2
    u16  PermPusk1;                     // ���������� �����1
    u16  PermPusk2;                     // ���������� �����2
    u16  Relay1;                        // ���� 1
    u16  Relay2;                        // ���� 2
    u16  Relay3;                        // ���� 3
    u16  Relay4;                        // ���� 4
    u16  ContrBrake;                    // ��������� ���������� ��������
    f32  IBrakeDown;                    // ��� ������ �������
    f32  FBrakeDown;                    // ������� ������ �������
    f32  FBrakeUp;                      // ������� ��������� �������
    u16  TBrakeDown;                    // ����� ������ �������
    u16  TBrakeUp;                      // ����� ��������� �������
    u16  StateContr;                    // ����� ���������� �����������
    u16  DINcon;                        // DIN ���������
    f32  FreqPusk;                      // ������� �����
    u16  TymePusk;                      // ����� �����
    u16  nvVirgin;                      // �������� ����, ��� ������ ���������
}PrmOutMacrosLift_type;
#define nvSignat 0x5AA5
// ������ �������� ������������� ��������, ������ ������ ������������� ���� ������
PrmOutMacrosLift_type PrmOutMacrosLift[] = {
    //��� ���,��� ���,��.����,��.����,�������,��������,����DI,�������,�������,1����,  2����,  ������, �����,  ����1,  ����2,��������1,  
    {   3,      3,      30,     20,     0,      1,      1,      500.0,  150.0,  5,      6,      3,      4,      3,      4,      3, 
    //��������2,����1,����2,  ����3,  ����4,����������,I������,F������,F������,T������,T������,������,DIN����,F����,  T����, Virgin
        4,      24,     13,     22,     17,     1,      700.0,  0.0,    0.0,    5,      5,      1,      1,      0.0,    0,   nvSignat},          // ������ �� ���������
    {   3,      3,      40,     20,     0,      1,      1,      500.0,  80.0,   5,      6,      3,      4,      3,      4,      3, 
        4,      24,     13,     22,     17,     1,      500.0,  1.0,    1.0,    1,      15,     1,      0,      0.0,    0,    nvSignat},          // ������ ��� ���
    {   3,      3,      25,     20,     0,      1,      1,      500.0,  50.0,   5,      6,      3,      4,      3,      4,      3,
        4,      24,     13,     22,     17,     1,      500.0,  0.0,    0.0,    10,     10,     1,      0,      0.0,    0,    nvSignat},          // ������ ��� ��-�
};

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_Type;                         //1  Type
    u16  Pnt_Set;                          //2  Set
    u16  Pnt_TymePusk;                     //3  TymePusk
    u16  Pnt_FreqPusk;                     //4  FreqPusk
    u16  Pnt_DINcon;                       //5  DINcon
    u16  Pnt_StateContr;                   //6  StateContr
    u16  Pnt_TBrakeUp;                     //7  TBrakeUp
    u16  Pnt_TBrakeDown;                   //8  TBrakeDown
    u16  Pnt_FBrakeUp;                     //9  FBrakeUp
    u16  Pnt_FBrakeDown;                   //10  FBrakeDown
    u16  Pnt_IBrakeDown;                   //11  IBrakeDown
    u16  Pnt_ContrBrake;                   //12  ContrBrake
    u16  Pnt_Relay4;                       //13  Relay4
    u16  Pnt_Relay3;                       //14  Relay3
    u16  Pnt_Relay2;                       //15  Relay2
    u16  Pnt_Relay1;                       //16  Relay1
    u16  Pnt_PermPusk2;                    //17  PermPusk2
    u16  Pnt_PermPusk1;                    //18  PermPusk1
    u16  Pnt_Pusk2;                        //19  Pusk2
    u16  Pnt_Pusk1;                        //20  Pusk1
    u16  Pnt_Revers;                       //21  Revers
    u16  Pnt_Forward;                      //22  Forward
    u16  Pnt__2_Speed;                     //23  2_Speed
    u16  Pnt__1_Speed;                     //24  1_Speed
    u16  Pnt_FreqAligning;                 //25  FreqAligning
    u16  Pnt_NomFreq;                      //26  NomFreq
    u16  Pnt_SpeedDI;                      //27  SpeedDI
    u16  Pnt_PriorSpeed;                   //28  PriorSpeed
    u16  Pnt_SelBase;                      //29  SelBase
    u16  Pnt_TymeDOWN;                     //30  TymeDOWN
    u16  Pnt_TymeUP;                       //31  TymeUP
    u16  Pnt_ChCntr1;                      //32  ChCntr1
    u16  Pnt_ChUst1;                       //33  ChUst1
    u16  Pnt_End;
}FuncMACROS_LIFT_type;

#define     nvSfMacroLift     (*(PrmOutMacrosLift_type*)0)
#define sizeNvLift  (sizeof(PrmOutMacrosLift_type))
//********************************************************
u16 *FuncMACROS_LIFT_1(FuncMACROS_LIFT_type *progPnt, u32 ramPnt)
{
    u16 TypeMacros;                     // ��� �������
    u16 Comand;                         // ������� ��������� �������
    s16 UstRecoveryFlagValue;
    void                        *nvAdr1;

    PrmOutMacrosLift_type  *nvAdr, nvMem;
    PrmOutMacrosLift_type   *pPrmOutMacrosLift;  // ����������� �� ��������� ������������� �������
    PrmOutMacrosLift_type   tmpStruct;
    //s16 tmp,sizeNv;

//#ifdef _PROJECT_FOR_CCS_
//	#warning !!! ��������. �������� ������ ��������� �����
//#endif
//    return &progPnt->Pnt_End;

    nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfMacroLift);
    nvAdr1 = (void *)((u32)&nv.UstRecoveryFlag);
    //sizeNv = sizeof(PrmOutMacrosLift_type);

    memset(&nvMem, 0, sizeNvLift);
    memset(&tmpStruct, 0, sizeNvLift);

    if (flagIsFirstCall){
        // ���������� ������� ��������� �������
        save_s16(ramPnt, progPnt->Pnt_Set,  ComandSetup_NO);
        RdFramMicro(&nvMem, nvAdr, sizeNvLift);
        RdFramMicro( &UstRecoveryFlagValue, nvAdr1,  sizeof(u16));
        if ((nvMem.nvVirgin == nvSignat)&&(!UstRecoveryFlagValue)){
            Comand = ComandSetup_YES;
            pPrmOutMacrosLift = &nvMem;    // �� ����������� ��������� �� ��������������� ���������
        }
        else{
            return &progPnt->Pnt_End;
        }
    }
    else{
        tmpStruct.ChannalTask1     = load_s16(ramPnt, progPnt->Pnt_ChUst1);
        tmpStruct.ChannalControl1  = load_s16(ramPnt, progPnt->Pnt_ChCntr1);
        tmpStruct.TymeUP           = load_s16(ramPnt, progPnt->Pnt_TymeUP);
        tmpStruct.TymeDOWN         = load_s16(ramPnt, progPnt->Pnt_TymeDOWN);
        tmpStruct.SelBase          = load_s16(ramPnt, progPnt->Pnt_SelBase);
        tmpStruct.PriorSpeed       = load_s16(ramPnt, progPnt->Pnt_PriorSpeed);
        tmpStruct.SpeedDI          = load_s16(ramPnt, progPnt->Pnt_SpeedDI);
        tmpStruct.NomFreq          = (f32)load_s32(ramPnt, progPnt->Pnt_NomFreq);
        tmpStruct.FreqAligning     = (f32)load_s32(ramPnt, progPnt->Pnt_FreqAligning);
        tmpStruct.Speed1           = load_s16(ramPnt, progPnt->Pnt__1_Speed);
        tmpStruct.Speed4           = load_s16(ramPnt, progPnt->Pnt__2_Speed);
        tmpStruct.Forward          = load_s16(ramPnt, progPnt->Pnt_Forward);
        tmpStruct.Revers           = load_s16(ramPnt, progPnt->Pnt_Revers);
        tmpStruct.Pusk1            = load_s16(ramPnt, progPnt->Pnt_Pusk1);
        tmpStruct.Pusk2            = load_s16(ramPnt, progPnt->Pnt_Pusk2);
        tmpStruct.PermPusk1        = load_s16(ramPnt, progPnt->Pnt_PermPusk1);
        tmpStruct.PermPusk2        = load_s16(ramPnt, progPnt->Pnt_PermPusk2);
        tmpStruct.Relay1           = load_s16(ramPnt, progPnt->Pnt_Relay1);
        tmpStruct.Relay2           = load_s16(ramPnt, progPnt->Pnt_Relay2);
        tmpStruct.Relay3           = load_s16(ramPnt, progPnt->Pnt_Relay3);
        tmpStruct.Relay4           = load_s16(ramPnt, progPnt->Pnt_Relay4);
        tmpStruct.ContrBrake       = load_s16(ramPnt, progPnt->Pnt_ContrBrake);
        tmpStruct.IBrakeDown       = (f32)load_s32(ramPnt, progPnt->Pnt_IBrakeDown);
        tmpStruct.FBrakeDown       = (f32)load_s32(ramPnt, progPnt->Pnt_FBrakeDown);
        tmpStruct.FBrakeUp         = (f32)load_s32(ramPnt, progPnt->Pnt_FBrakeUp);
        tmpStruct.TBrakeDown       = load_s16(ramPnt, progPnt->Pnt_TBrakeDown);
        tmpStruct.TBrakeUp         = load_s16(ramPnt, progPnt->Pnt_TBrakeUp);
        tmpStruct.StateContr       = load_s16(ramPnt, progPnt->Pnt_StateContr);
        tmpStruct.DINcon           = load_s16(ramPnt, progPnt->Pnt_DINcon);
        tmpStruct.FreqPusk         = (f32)load_s32(ramPnt, progPnt->Pnt_FreqPusk);
        tmpStruct.TymePusk         = load_s16(ramPnt, progPnt->Pnt_TymePusk);
        tmpStruct.nvVirgin         = nvSignat;

        RdFramMicro(&nvMem, nvAdr, sizeNvLift);
        //tmp = memcmp(&nvMem,&tmpStruct,sizeNv);
        //if (tmp){
        if (memcmp(&nvMem,&tmpStruct,sizeNvLift)){  //���� ��������� �� �����
            memcpy(&nvMem,&tmpStruct,sizeNvLift);   //����������� � ����� ��� ������
            WrFramMicro(nvAdr, &nvMem, sizeNvLift); //������������ � �����������������
        }
        TypeMacros = load_s16(ramPnt, progPnt->Pnt_Type);
        Comand = load_s16(ramPnt, progPnt->Pnt_Set);
        pPrmOutMacrosLift = &PrmOutMacrosLift[ TypeMacros ];    // �� ����������� ��������� �� ��������������� ���������
    }



    if (Comand == ComandSetup_YES)                              // ���� ������ ������� ��������� ������� ����������
    {

        // �������������� ��������� � ������������ � ��������

        save_s16(ramPnt, progPnt->Pnt_ChUst1,       pPrmOutMacrosLift->ChannalTask1);
        save_s16(ramPnt, progPnt->Pnt_ChCntr1,      pPrmOutMacrosLift->ChannalControl1);
        save_s16(ramPnt, progPnt->Pnt_TymeUP,       pPrmOutMacrosLift->TymeUP);
        save_s16(ramPnt, progPnt->Pnt_TymeDOWN,     pPrmOutMacrosLift->TymeDOWN);
        save_s16(ramPnt, progPnt->Pnt_SelBase,      pPrmOutMacrosLift->SelBase);
        save_s16(ramPnt, progPnt->Pnt_PriorSpeed,   pPrmOutMacrosLift->PriorSpeed);
        save_s16(ramPnt, progPnt->Pnt_SpeedDI,      pPrmOutMacrosLift->SpeedDI);
        save_s16(ramPnt, progPnt->Pnt__1_Speed,     pPrmOutMacrosLift->Speed1);
        save_s16(ramPnt, progPnt->Pnt__2_Speed,     pPrmOutMacrosLift->Speed4);
        save_s16(ramPnt, progPnt->Pnt_Forward,      pPrmOutMacrosLift->Forward);
        save_s16(ramPnt, progPnt->Pnt_Revers,       pPrmOutMacrosLift->Revers);
        save_s16(ramPnt, progPnt->Pnt_Pusk1,        pPrmOutMacrosLift->Pusk1);
        save_s16(ramPnt, progPnt->Pnt_Pusk2,        pPrmOutMacrosLift->Pusk2);
        save_s16(ramPnt, progPnt->Pnt_PermPusk1,    pPrmOutMacrosLift->PermPusk1);
        save_s16(ramPnt, progPnt->Pnt_PermPusk2,    pPrmOutMacrosLift->PermPusk2);

        if (flagIsFirstCall){
            save_s32(ramPnt, progPnt->Pnt_NomFreq,      (s32)pPrmOutMacrosLift->NomFreq);
            save_s32(ramPnt, progPnt->Pnt_FreqAligning, (s32)pPrmOutMacrosLift->FreqAligning);
            save_s32(ramPnt, progPnt->Pnt_FBrakeDown,   (s32)pPrmOutMacrosLift->FBrakeDown);
            save_s32(ramPnt, progPnt->Pnt_FBrakeUp,     (s32)pPrmOutMacrosLift->FBrakeUp);
            save_s32(ramPnt, progPnt->Pnt_FreqPusk,     (s32)pPrmOutMacrosLift->FreqPusk);
        }
        else{
            save_s32(ramPnt, progPnt->Pnt_NomFreq,      _IQ(pPrmOutMacrosLift->NomFreq      /MPOpis.Base[NUM_FREQ_BASE]));
            save_s32(ramPnt, progPnt->Pnt_FreqAligning, _IQ(pPrmOutMacrosLift->FreqAligning /MPOpis.Base[NUM_FREQ_BASE]));
            save_s32(ramPnt, progPnt->Pnt_FBrakeDown,   _IQ(pPrmOutMacrosLift->FBrakeDown   /MPOpis.Base[NUM_FREQ_BASE]));
            save_s32(ramPnt, progPnt->Pnt_FBrakeUp,     _IQ(pPrmOutMacrosLift->FBrakeUp     /MPOpis.Base[NUM_FREQ_BASE]));
            save_s32(ramPnt, progPnt->Pnt_FreqPusk,     _IQ(pPrmOutMacrosLift->FreqPusk     /MPOpis.Base[NUM_FREQ_BASE]));
        }

        save_s16(ramPnt, progPnt->Pnt_Relay1,       pPrmOutMacrosLift->Relay1);
        save_s16(ramPnt, progPnt->Pnt_Relay2,       pPrmOutMacrosLift->Relay2);
        save_s16(ramPnt, progPnt->Pnt_Relay3,       pPrmOutMacrosLift->Relay3);
        save_s16(ramPnt, progPnt->Pnt_Relay4,       pPrmOutMacrosLift->Relay4);
        save_s16(ramPnt, progPnt->Pnt_ContrBrake,   pPrmOutMacrosLift->ContrBrake);

        if (flagIsFirstCall){
            save_s32(ramPnt, progPnt->Pnt_IBrakeDown,   (s32)pPrmOutMacrosLift->IBrakeDown);
        }
        else{
            //save_s32(ramPnt, progPnt->Pnt_IBrakeDown,   _IQ(pPrmOutMacrosLift->IBrakeDown/MPOpis.Base[NUM_CUR_BASE]));
            save_s32(ramPnt, progPnt->Pnt_IBrakeDown,   _IQ(pPrmOutMacrosLift->IBrakeDown   /MPOpis.Base[NUM_1000_BASE]));
        }

        save_s16(ramPnt, progPnt->Pnt_TBrakeDown,   pPrmOutMacrosLift->TBrakeDown);
        save_s16(ramPnt, progPnt->Pnt_TBrakeUp,     pPrmOutMacrosLift->TBrakeUp);
        save_s16(ramPnt, progPnt->Pnt_StateContr,   pPrmOutMacrosLift->StateContr);
        save_s16(ramPnt, progPnt->Pnt_DINcon,       pPrmOutMacrosLift->DINcon);
        save_s16(ramPnt, progPnt->Pnt_TymePusk,     pPrmOutMacrosLift->TymePusk);

        // ���������� ������� ��������� �������
        save_s16(ramPnt, progPnt->Pnt_Set,  ComandSetup_NO);
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMACROS_LIFT_1[33]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Set
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //3  TymePusk
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //4  FreqPusk
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  DINcon
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  StateContr
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //7  TBrakeUp
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //8  TBrakeDown
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //9  FBrakeUp
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //10  FBrakeDown
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //11  IBrakeDown
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  ContrBrake
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //13  Relay4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //14  Relay3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //15  Relay2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //16  Relay1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //17  PermPusk2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //18  PermPusk1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //19  Pusk2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //20  Pusk1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //21  Revers
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //22  Forward
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //23  2_Speed
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  1_Speed
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //25  FreqAligning
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //26  NomFreq
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  SpeedDI
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  PriorSpeed
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29  SelBase
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30  TymeDOWN
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //31  TymeUP
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //32  ChCntr1
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //33  ChUst1
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
//OpisFuncBlock("MACROS_LIFT", "DD", TblFuncMACROS_LIFT_1, FuncMACROS_LIFT_1, sizeof(TblFuncMACROS_LIFT_1), 0, sizeof(nvFuncMACROS_LIFT_type), 1),
OpisFuncBlock("MACROS_LIFT", "DD", TblFuncMACROS_LIFT_1, FuncMACROS_LIFT_1, sizeof(TblFuncMACROS_LIFT_1), 0, sizeof(PrmOutMacrosLift_type), 1),

#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMACROS_LIFT_1,
#endif
//********************************************************
