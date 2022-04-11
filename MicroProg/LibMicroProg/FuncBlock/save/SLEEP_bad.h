#ifdef    GET_FUNC_CODE
typedef struct{
    u16  TimeOn;
    u16  TimeOff;
    u16  Sec1On;
    u16  Sec1Off;
}SLEEP_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Vin;                          //1  Vin
    u16  Pnt_Vust;                         //2  Vust
    u16  Pnt_TimeOff;                      //3  TimeOff
    u16  Pnt_TimeOn;                       //4  TimeOn
    u16  Pnt_Alg;                          //5  Alg
    u16  Pnt_Vin__Vust;                    //6  Vin<>Vust
    u16  Pnt_DriveOff;                     //7  DriveOff
    u16  Pnt_End;
}FuncSLEEP_type;
//********************************************************
// ���������� ��� ��������� ������� �� ��������
// ����������� ������� �� �����.
// ������� ������:
// - ����� ��� - 1 ��� 2;
// - ����� ��������� � ���������� � ��������
// - ������� ��� ��������� � ���������� ��������
// - ��� ��������� "���� ������ �������" ��� "���� ������"
// �������� ������:
// - ����� ��� �������;
// - ������ �������;
// - ��������� ������� "0" - ������ ������ ��������, "1" 
//   ������ ����� ����������
//********************************************************
u16 *FuncSLEEP_1(FuncSLEEP_type *progPnt, u32 ramPnt)
{
    s32 Vin, Vust;
    u16 Alg, Cond = 0;
    u16 tmp = 0;
    SLEEP_State_type *sPnt;
    
    sPnt = (SLEEP_State_type *)(ramPnt + progPnt->Pnt_State);


    Alg  = load_s16(ramPnt, progPnt->Pnt_Alg);
    Vin  = load_s32(ramPnt, progPnt->Pnt_Vin);
    Vust = load_s32(ramPnt, progPnt->Pnt_Vust);

    switch (Alg){
      case 0:               //������� 
        if (Vin < Vust) Cond = 1;         break;
      case 1:               //��������
        if (Vin > Vust) Cond = 1;         break;
    }

    if(Cond){                                                 //����� �� ������ � DriveOff �� ����������
        setBitMicro(ramPnt, progPnt->Pnt_Vin__Vust);
        tmp = load_s16(ramPnt, progPnt->Pnt_TimeOff);         //������ ������� �� ������� ����������
        sPnt->TimeOff = 0;
        sPnt->Sec1Off = 0;
        sPnt->Sec1On ++;
      
        if (sPnt->TimeOn >= tmp){                     //������ �������� ����� ����.
            setBitMicro(ramPnt, progPnt->Pnt_DriveOff);
            sPnt->TimeOn = 0;
        }
        else{
            if (sPnt->Sec1On >= 250){
                sPnt->Sec1On = 0;
                sPnt->TimeOn ++;
            }
        }
    }
    else{
        clrBitMicro(ramPnt, progPnt->Pnt_Vin__Vust);
        tmp = load_s16(ramPnt, progPnt->Pnt_TimeOn);        //������ ������� �� ������� ���������
        sPnt->Sec1On = 0;
        sPnt->TimeOn = 0;
        sPnt->Sec1Off ++;
        
        if (sPnt->TimeOff >= tmp){                   //������ �������� ����� ����.
            clrBitMicro(ramPnt, progPnt->Pnt_DriveOff);
            sPnt->TimeOff = 0;
        }
        else{
            if (sPnt->Sec1Off > 250){            
                sPnt->Sec1Off = 0;
                sPnt->TimeOff ++;
            }
        }
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSLEEP_1[7]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Vin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Vust
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  TimeOff
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  TimeOn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Alg
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Vin<>Vust
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //9  DriveOff        
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SLEEP", "DD", TblFuncSLEEP_1, FuncSLEEP_1, sizeof(TblFuncSLEEP_1), sizeof(SLEEP_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSLEEP_1,
#endif
//********************************************************
