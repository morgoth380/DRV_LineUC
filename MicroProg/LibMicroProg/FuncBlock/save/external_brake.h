#ifdef    GET_FUNC_CODE
//-----------------------------------------------------------
typedef struct{
    s16        time_br_off;   // ������� ������� ������ �������
    s16        time_br_on;    // ������� ������� ��������� �������
    s16        time_sw_hold;  // ������� ������� ������������ ������ �������
}EXTERNAL_BRAKE_State_type;
//------------------------------------------------------------
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Aktiv;                        //1  Aktiv
    u16  Pnt_I_off;                        //2  I_off
    u16  Pnt_I_on;                         //3  I_on
    u16  Pnt_I_nom;                        //4  I_nom
    u16  Pnt_I_mot;                        //5  I_mot
    u16  Pnt_T_off;                        //6  T_off
    u16  Pnt_T_on;                         //7  T_on
    u16  Pnt_T_on_d;                       //8  T_on_d
    u16  Pnt_T_hold;                       //9  T_hold
    u16  Pnt_D_in;                         //10  D_in
    u16  Pnt_DI_or_T;                      //11  DI_or_T
    u16  Pnt_Fix_stop;                     //12  Pr_UP
    u16  Pnt_Fix_start;                    //13  Pr_UP
    u16  Pnt_PWM_ON;                       //14  PWM_ON
    u16  Pnt_STOP;                         //15  STOP
    u16  Pnt_TypeDrv;                      //16  TypeDrv
    u16  Pnt_HLD_ZR;                       //17  HLD_ZR
    u16  Pnt_DIS_BR;                       //18  DIS_BR
    u16  Pnt_End;
}FuncEXTERNAL_BRAKE_type;
//********************************************************

// ------------------------------------------------------------------------------
//                      ����������
//  time_sw_hold - ������� ������� ������������
//  Pnt_Fix_start - ���� ����������� ������� ������ �������
//  Pnt_DIS_BR - ���� ������/��������� �������
//  GlobalC28.FlagState.bit.vPrUp - ���� ����� ����� ��
// ------------------------------------------------------------------------------

u16 *FuncEXTERNAL_BRAKE_1(FuncEXTERNAL_BRAKE_type *progPnt, u32 ramPnt)
{
    s16                                      feedback, di;
    EXTERNAL_BRAKE_State_type                *sPnt;
    u8                                       PrUp_flag;
    s16 TypeDrv;

    sPnt     = (EXTERNAL_BRAKE_State_type *)(ramPnt + progPnt->Pnt_State);
    feedback = load_s16(ramPnt, progPnt->Pnt_DI_or_T);
    di       = testBitMicro(ramPnt, progPnt->Pnt_D_in);
    TypeDrv  = load_s16(ramPnt, progPnt->Pnt_TypeDrv);


    if (TypeDrv == TYPE_SCALAR_NO_DEADBAND || TypeDrv == TYPE_SCALAR )
        PrUp_flag = 1;
    else
        PrUp_flag = (u8)GlobalC28.FlagState.bit.vPrUp;


    if (load_s16(ramPnt, progPnt->Pnt_Aktiv) == 1){ // ���������, ���� ������������ ���������� ��������

        // ------------------------------------------------------------------------------
        //                      �������� ������ �������
        // ------------------------------------------------------------------------------

        if (sPnt->time_sw_hold <= 0){ // ���������� ������� ������� ������� �� ����� ������������
            // ���� ������� ������� ���� ������� ������, 
            // ����� �� ����� � ������ ��� �� ����,
            // �� ��������� ������� 
            if((testBitMicro(ramPnt, progPnt->Pnt_Fix_start) == 1)  &&   
               (testBitMicro(ramPnt, progPnt->Pnt_DIS_BR) == 0)     &&   
                PrUp_flag )                          
            {
                // ���� ������� ��� ������, ��� ������� ������� ������������ ����,
                // �� ����� ������
                if (_IQmpy(load_s32(ramPnt, progPnt->Pnt_I_nom), load_s32(ramPnt, progPnt->Pnt_I_off)) 
                    < load_s32(ramPnt, progPnt->Pnt_I_mot))
                {
                    setBitMicro(ramPnt, progPnt->Pnt_DIS_BR);
                }
                // ���������������� ������� ������ �������
                sPnt->time_br_off = (s16)((u32)load_s16(ramPnt, progPnt->Pnt_T_off) * K_TIME_CALL_MICRO_TAKT / 100);
            }
        }else{
            setBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);
            sPnt->time_sw_hold--;
        }

        // ���������� ������� ������� 
        // ���� �� ������ ����� ������ �������
        if (sPnt->time_br_off <= 0){
            clrBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);
        }else{
            // ���� �������� �������� �������� ����� (��),
            // � �� �� ������ ������ �������� �������,
            // �� ������ ������,
            // ����� ������ ����� ������� ��������
            if ((feedback == 1) && (di == 0)){
                sPnt->time_br_off = 0;
            }else{
                sPnt->time_br_off--;
            }
        }

        // ���� ������ �������, �� ����������  ������� ������ ����� ���������
        if (testBitMicro(ramPnt, progPnt->Pnt_DIS_BR) == 0){
            setBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);
        }

        // ------------------------------------------------------------------------------
        //                      �������� ��������� �������
        // ------------------------------------------------------------------------------
       
        // ���� ������ ������ ��������,
        // �� �������� ������ ���������  ������� ������ ����� ���������
        if (testBitMicro(ramPnt, progPnt->Pnt_STOP)){
            setBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);
            
            //  ���� ������� ������� ���� ������� ������, 
            // �� �������� ������, 
            // � ������ ������ ������� ��������� �������
            // ����� ������������������� ������� ������� ���������.
            if (testBitMicro(ramPnt, progPnt->Pnt_Fix_stop)){
                // ���������� ������� ������� 
                // ���� �� ������ ����� ��������� �������
                // �������� ��, ���� ��������
                if ((feedback == 1) && (di == 1)){
                    sPnt->time_br_on = 0;
                }else{
                    sPnt->time_br_on--;
                }
                clrBitMicro(ramPnt, progPnt->Pnt_DIS_BR);
            }
/*
            else{
                sPnt->time_br_on = (s16)((u32)load_s16(ramPnt, progPnt->Pnt_T_on) * K_TIME_CALL_MICRO_TAKT / 100);
            }
*/
            // ���� ����� ��������� �����,
            // �� ��������� ������ ��������� ������� �������,
            // � ������������������� ������� ������� ������������
            if (sPnt->time_br_on <= 0){
                clrBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);
                sPnt->time_sw_hold = (s16)((u32)load_s16(ramPnt, progPnt->Pnt_T_hold) * K_TIME_CALL_MICRO_TAKT / 100);
            }
        }else{
            sPnt->time_br_on = (s16)((u32)load_s16(ramPnt, progPnt->Pnt_T_on) * K_TIME_CALL_MICRO_TAKT / 100);
        }
    }else{
        clrBitMicro(ramPnt, progPnt->Pnt_HLD_ZR);   // ��������� ���������  ������� ������ ����� ���������
        clrBitMicro(ramPnt, progPnt->Pnt_DIS_BR);   // �������� ������
    }

    // ���� ��� �������� (�������� �� ������), �� �������� ������
    if(testBitMicro(ramPnt, progPnt->Pnt_PWM_ON) == 0) {
        clrBitMicro(ramPnt, progPnt->Pnt_DIS_BR);
        sPnt->time_br_on = (s16)((u32)load_s16(ramPnt, progPnt->Pnt_T_on) * K_TIME_CALL_MICRO_TAKT / 100);
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncEXTERNAL_BRAKE_1[18]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Aktiv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  I_off
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  I_on
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  I_nom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  I_mot
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  T_off
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  T_on
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  T_on_d
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  T_hold
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  D_in
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  DI_or_T
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //12  Fix_stop
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //13  Fix_start
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //14  PWM_ON
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15  STOP
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  TypeDrv
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //17  HLD_ZR
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //18  DIS_BR
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("EXTERNAL_BRAKE", "DA", TblFuncEXTERNAL_BRAKE_1, FuncEXTERNAL_BRAKE_1, sizeof(TblFuncEXTERNAL_BRAKE_1), sizeof(EXTERNAL_BRAKE_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncEXTERNAL_BRAKE_1,
#endif
//********************************************************
