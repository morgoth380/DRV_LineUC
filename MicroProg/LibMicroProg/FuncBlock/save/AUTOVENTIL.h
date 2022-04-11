#ifdef    GET_FUNC_CODE
//********************************************************
typedef struct{
        u16         Prior1;
        u16         Prior2;
        u16         Prior3;
}nvAUTOVENTILState_type;
#define     nvSfAvtoventil     (*(nvAUTOVENTILState_type*)0)
typedef struct{
        u32         wait;
        u32         T_non_rot;
        u16         T_Razm;
        u16         razmagnich_now;
        u16         MH1;
        u16         MH2;
        u16         MH3;
        u16         D_rdy_for;
        u16         systemOverload;
        u16         systemEdle;
        u16         MPR1;
        u16         MPR2;
        u16         MPR3;
        nvAUTOVENTILState_type Prior;
}AUTOVENTILState_type;
typedef union{
struct{
        u16         Ena1:1;
        u16         Ena2:1;
        u16         Ena3:1;
        u16         ON1:1;
        u16         ON2:1;
        u16         ON3:1;
        u16         M1_AC:1;
        u16         M2_AC:1;
        u16         M3_AC:1;
        u16         M1_FC:1;
        u16         M2_FC:1;
        u16         M3_FC:1;
        u16         MHO1:1;
        u16         MHO2:1;
        u16         MHO3:1;
        u16         firstStart:1;
        u16         conRotM1:1;
        u16         conRotM2:1;
        u16         conRotM3:1;
        u16         fail:1;
        u16         prior:1;
        u16         connectNewFC:1;
        u16         rezerv4:1;
        u16         rezerv5:1;
        u16         rezerv6:1;
        u16         rezerv7:1;
        u16         rezerv8:1;
        u16         rezerv9:1;
        u16         rezerv10:1;
        u16         rezerv11:1;
        u16         rezerv12:1;
        u16         rezerv13:1;
};
        u32         all;
}Mot_ENA;
Mot_ENA Mot;

//********************************************************
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_ON_OFF;                       //1  ON_OFF
    u16  Pnt_Fref;                         //2  Fref
    u16  Pnt_Err_Pid;                      //3  Err_Pid
    u16  Pnt_maxPID;                       //4  maxPID
    u16  Pnt_minPID;                       //5  minPID
    u16  Pnt_Dopusk;                       //6  Dopusk
    u16  Pnt_T_Dop;                        //7  T_Dop
    u16  Pnt_Prior;                        //8  Prior
    u16  Pnt_T_PID;                        //9  T_PID
    u16  Pnt_T_Razm;                       //10  T_Razm
    u16  Pnt_T_rot;                        //11  T_rot
    u16  Pnt_Mot1;                         //12  Mot1
    u16  Pnt_Mot2;                         //13  Mot2
    u16  Pnt_Mot3;                         //14  Mot3
    u16  Pnt_Ena1;                         //15  Ena1
    u16  Pnt_Ena2;                         //16  Ena2
    u16  Pnt_Ena3;                         //17  Ena3
    u16  Pnt_MH1;                          //18  MH1
    u16  Pnt_MH2;                          //19  MH2
    u16  Pnt_MH3;                          //20  MH3
    u16  Pnt_M_ST1;                        //21  M_ST1
    u16  Pnt_M_ST2;                        //22  M_ST2
    u16  Pnt_M_ST3;                        //23  M_ST3
    u16  Pnt_Enable;                       //24  Enable
    u16  Pnt_M3_IU;                        //25  M3_IU
    u16  Pnt_M2_IU;                        //26  M2_IU
    u16  Pnt_M1_IU;                        //27  M1_IU
    u16  Pnt_MHO3;                         //28  MHO3
    u16  Pnt_MHO2;                         //29  MHO2
    u16  Pnt_MHO1;                         //30  MHO1
    u16  Pnt_M3_AC;                        //31  M3_AC
    u16  Pnt_M2_AC;                        //32  M2_AC
    u16  Pnt_M1_AC;                        //33  M1_AC
    u16  Pnt_M3_FC;                        //34  M3_FC
    u16  Pnt_M2_FC;                        //35  M2_FC
    u16  Pnt_M1_FC;                        //36  M1_FC
    u16  Pnt_Out;                          //37  Out
    u16  Pnt_Fail;                         //38  Fail
    u16  Pnt_End;
}FuncAUTOVENTIL_type;
void mot_state(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void mot_decrease_MH(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void mot_decrease_PR(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void mot_increase(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void motors_control(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void contactor_man(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
u16 mot_disconnect_FC();
void mot_connect_AC(u16 numb);
void mot_connect_FC_MH(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void mot_connect_FC_PR(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void connect_New_FC(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void busywork(AUTOVENTILState_type *sPnt);
void prior(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void prior_MH(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void rotatsnuti(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt);
void internal_fail();

//********************************************************
u16 *FuncAUTOVENTIL_1(FuncAUTOVENTIL_type *progPnt, u32 ramPnt)
{
    AUTOVENTILState_type    *sPnt;
    nvAUTOVENTILState_type           *nvAdr;
    s16 enable;
    sPnt = (AUTOVENTILState_type *)(ramPnt + progPnt->Pnt_State);
    nvAdr       = GetNvAdr(progPnt->Pnt_nvState, &nvSfAvtoventil);

    if (flagIsFirstCall)
    {
        RdFramMicro(&sPnt->Prior, nvAdr, sizeof(nvAUTOVENTILState_type));
        sPnt->MPR1 = sPnt->Prior.Prior1;
        sPnt->MPR2 = sPnt->Prior.Prior2;
        sPnt->MPR3 = sPnt->Prior.Prior3;
        save_s16(ramPnt, progPnt->Pnt_Mot1, sPnt->MPR1);
        save_s16(ramPnt, progPnt->Pnt_Mot2, sPnt->MPR2);
        save_s16(ramPnt, progPnt->Pnt_Mot3, sPnt->MPR3);


        sPnt->T_non_rot = K_TIME_CALL_MICRO_TAKT * 600; // 10 минут задержки дл€ устранени€ ротации двух моторов подр€д
        return &progPnt->Pnt_End;
    }

    enable = (testBitMicro(ramPnt, progPnt->Pnt_Enable) && load_s16(ramPnt, progPnt->Pnt_ON_OFF));
    prior(progPnt, ramPnt, sPnt);
    if ((sPnt->MPR1!=sPnt->Prior.Prior1)||(sPnt->MPR2!=sPnt->Prior.Prior2)||(sPnt->MPR3!=sPnt->Prior.Prior3))
    {
        sPnt->Prior.Prior1 = sPnt->MPR1;
        sPnt->Prior.Prior2 = sPnt->MPR2;
        sPnt->Prior.Prior3 = sPnt->MPR3;
        WrFramMicro(nvAdr, &sPnt->Prior, sizeof(nvAUTOVENTILState_type));
    }


    prior_MH(progPnt, ramPnt, sPnt);
    Mot.fail = 0;
    // ѕровер€ем разрешена ли работа
    if (enable){
        mot_state(progPnt, ramPnt, sPnt);
        busywork(sPnt);
        if (sPnt->razmagnich_now != 1){
            if((Mot.conRotM1 == 0) && (Mot.conRotM2 == 0) && (Mot.conRotM3 == 0)){
                if(sPnt->T_non_rot == 0){
                    rotatsnuti(progPnt, ramPnt, sPnt);
                }else{
                    sPnt->T_non_rot--;
                }
                if(Mot.firstStart == 0){
                    sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop) * K_TIME_CALL_MICRO_TAKT * 60;
                    sPnt->T_Razm = load_s16(ramPnt, progPnt->Pnt_T_Razm) * K_TIME_CALL_MICRO_TAKT / 10;
                    if(sPnt->systemOverload == 0){
                        save_s16(ramPnt, progPnt->Pnt_Out, 0);
                        if((Mot.Ena1 == RAZRESHON) && (Mot.Ena2 == RAZRESHON) && (Mot.Ena3 == RAZRESHON)){
                            if((sPnt->MPR1 <= sPnt->MPR2) && (sPnt->MPR1 <= sPnt->MPR3)){
                                Mot.ON1 = ON;
                                Mot.M1_FC = ON;
                            }else{
                                if(sPnt->MPR2 <= sPnt->MPR3){
                                    Mot.ON2 = ON;
                                    Mot.M2_FC = ON;
                                }else{
                                    Mot.ON3 = ON;
                                    Mot.M3_FC = ON;
                                }
                            }
                        }else{
                            if(Mot.Ena1 == ZAPRESCHEN){
                                if(sPnt->MPR2 <= sPnt->MPR3){
                                    if (Mot.Ena2 == RAZRESHON){
                                        Mot.ON2 = ON;
                                        Mot.M2_FC = ON;
                                    }else{
                                        if (Mot.Ena3 == RAZRESHON){
                                            Mot.ON3 = ON;
                                            Mot.M3_FC = ON;
                                        }else{
                                            internal_fail();
                                        }
                                    }
                                }else{
                                    if (Mot.Ena3 == RAZRESHON){
                                        Mot.ON3 = ON;
                                        Mot.M3_FC = ON;
                                    }else{
                                        if (Mot.Ena2 == RAZRESHON){
                                            Mot.ON2 = ON;
                                            Mot.M2_FC = ON;
                                        }else{
                                            internal_fail();
                                        }
                                    }
                                }
                            }else{
                                if(Mot.Ena2 == ZAPRESCHEN){
                                    if(sPnt->MPR1 <= sPnt->MPR3){
                                        Mot.ON1 = ON;
                                        Mot.M1_FC = ON;
                                    }else{
                                        if (Mot.Ena3 == RAZRESHON){
                                            Mot.ON3 = ON;
                                            Mot.M3_FC = ON;
                                        }else{
                                            Mot.ON1 = ON;
                                            Mot.M1_FC = ON;
                                        }
                                    }
                                }else{
                                    if(Mot.Ena3 == ZAPRESCHEN){
                                        if(sPnt->MPR1 <= sPnt->MPR2){
                                            Mot.ON1 = ON;
                                            Mot.M1_FC = ON;
                                        }else{
                                            Mot.ON2 = ON;
                                            Mot.M2_FC = ON;
                                        }
                                    }
                                }
                            }
                        }
                        Mot.firstStart = 1;
                    }else{
                        save_s16(ramPnt, progPnt->Pnt_Out, 1);
                    }
                }else{
                    motors_control(progPnt, ramPnt, sPnt);
                }
            }else{
                if(load_s32(ramPnt, progPnt->Pnt_Fref) == 0){
                    if(Mot.conRotM1){
                        Mot.M1_FC = 1;
                        Mot.conRotM1 = 0;
                        save_s16(ramPnt, progPnt->Pnt_Out, 0);
                    }else{
                        if(Mot.conRotM2){
                            Mot.M2_FC = 1;
                            Mot.conRotM2 = 0;
                            save_s16(ramPnt, progPnt->Pnt_Out, 0);
                        }else{
                            if(Mot.conRotM3){
                                Mot.M3_FC = 1;
                                Mot.conRotM3 = 0;
                                save_s16(ramPnt, progPnt->Pnt_Out, 0);
                            }else{
                                internal_fail();
                            }
                        }
                    }
                }
                sPnt->T_non_rot = K_TIME_CALL_MICRO_TAKT * 600; // 10 минут задержки дл€ устранени€ ротации двух моторов подр€д
            }
        }
		else{
            //save_s16(ramPnt, progPnt->Pnt_Out, 1);	//сброс ГенератораТемпа 
            mot_increase(progPnt, ramPnt, sPnt);
        }
       contactor_man(progPnt, ramPnt, sPnt);
    }else{
        save_s16(ramPnt, progPnt->Pnt_Out, 0);
        defBitMicro(ramPnt, progPnt->Pnt_M1_AC, 0);
        defBitMicro(ramPnt, progPnt->Pnt_M2_AC, 0);
        defBitMicro(ramPnt, progPnt->Pnt_M3_AC, 0);
        defBitMicro(ramPnt, progPnt->Pnt_M1_FC, 0);
        defBitMicro(ramPnt, progPnt->Pnt_M2_FC, 0);
        defBitMicro(ramPnt, progPnt->Pnt_M3_FC, 0);
        Mot.firstStart = 0;
        Mot.all = 0;
        sPnt->razmagnich_now = 0;
        sPnt->T_non_rot = K_TIME_CALL_MICRO_TAKT * 600; // 10 минут задержки дл€ устранени€ ротации двух моторов подр€д
    }
    if(Mot.fail == 1){
        setBitMicro(ramPnt, progPnt->Pnt_Fail);
    }else{
        clrBitMicro(ramPnt, progPnt->Pnt_Fail);
    }
    return &progPnt->Pnt_End;
};
void motors_control(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    s32        in, max, min, delta, err;
    // осуществл€ем регулирование
    in = load_s32(ramPnt, progPnt->Pnt_Fref);
    err = load_s32(ramPnt, progPnt->Pnt_Err_Pid);
    delta = load_s32(ramPnt, progPnt->Pnt_Dopusk);
    min = load_s32(ramPnt, progPnt->Pnt_minPID);
    max = load_s32(ramPnt, progPnt->Pnt_maxPID);
    if (err < -delta){
        if (in < (min + delta)){
            //≈сли частота опустилась до минимальной - начинаем отсчет времени на переключение
            if (sPnt->systemEdle == 0){
                // если система не в простое, т.е. подключены дополнительные двигатели, то начинаем отсчет на отключение
                if (sPnt->wait == 0){
                    // когда врем€ истекло - отключаем двиагетль
                    if(load_s16(ramPnt, progPnt->Pnt_Prior)){
                        mot_decrease_PR(progPnt, ramPnt, sPnt);
                    }else{
                        mot_decrease_MH(progPnt, ramPnt, sPnt);
                    }
                    sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop) * K_TIME_CALL_MICRO_TAKT * 60;
                }else{
                    sPnt->wait--;
                }
            }else{
                // если система в простое, перезагружаем таймер
                sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop) * K_TIME_CALL_MICRO_TAKT * 60;
            }
        }else{
            sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop) * K_TIME_CALL_MICRO_TAKT * 60;
        }
    }else{
        if (err > delta){
            //≈сли частота превысила верхний порог
            if (in > (max - delta)){
                //≈сли частота превысила пороговое значение - начинаем отсчет времени на переключение
                if (sPnt->systemOverload == 0){
                    // если система не зан€та полностью, т.е. есть свободные дополнительные двигатели, то начинаем отсчет на подключение
                    if (sPnt->wait == 0){
                        // когда врем€ истекло - отключаем двиагетль
                        mot_increase(progPnt, ramPnt, sPnt);
                    }else{
                        sPnt->wait--;
                        sPnt->T_Razm = load_s16(ramPnt, progPnt->Pnt_T_Razm) * K_TIME_CALL_MICRO_TAKT / 10;
                        sPnt->D_rdy_for = 0;
                    }
                }else{
                    // если система в простое, перезагружаем таймер
                    sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop) * K_TIME_CALL_MICRO_TAKT * 60;
                }
            }
        }else{
            sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop) * K_TIME_CALL_MICRO_TAKT * 60;
        }
    }
}
void mot_decrease_MH(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    if(Mot.M1_AC){
        if(Mot.M2_AC){
            if(Mot.M3_AC){
                internal_fail();
            }else{
                if (sPnt->MH1 > sPnt->MH2){
                    Mot.M1_AC = OFF;
                }else{
                    Mot.M2_AC = OFF;
                }
            }
        }else{
            if(Mot.M3_AC){
                if (sPnt->MH1 > sPnt->MH3){
                    Mot.M1_AC = OFF;
                }else{
                    Mot.M3_AC = OFF;
                }
            }else{
                Mot.M1_AC = OFF;
            }
        }
    }else{
        if(Mot.M2_AC){
            if(Mot.M3_AC){
                if (sPnt->MH2 > sPnt->MH3){
                    Mot.M2_AC = OFF;
                }else{
                    Mot.M3_AC = OFF;
                }
            }else{
                Mot.M2_AC = OFF;
            }
        }else{
            if(Mot.M3_AC){
                Mot.M3_AC = OFF;
            }
/*
            else{
                internal_fail();
            }
*/
        }
    }
}
void mot_decrease_PR(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    if(Mot.M1_AC){
        if(Mot.M2_AC){
            if(Mot.M3_AC){
                internal_fail();
            }else{
                if (sPnt->MPR1 > sPnt->MPR2){
                    Mot.M1_AC = OFF;
                }else{
                    Mot.M2_AC = OFF;
                }
            }
        }else{
            if(Mot.M3_AC){
                if (sPnt->MPR1 > sPnt->MPR3){
                    Mot.M1_AC = OFF;
                }else{
                    Mot.M3_AC = OFF;
                }
            }else{
                Mot.M1_AC = OFF;
            }
        }
    }else{
        if(Mot.M2_AC){
            if(Mot.M3_AC){
                if (sPnt->MPR2 > sPnt->MPR3){
                    Mot.M2_AC = OFF;
                }else{
                    Mot.M3_AC = OFF;
                }
            }else{
                Mot.M2_AC = OFF;
            }
        }else{
            if(Mot.M3_AC){
                Mot.M3_AC = OFF;
            }
/*
            else{
                internal_fail();
            }
*/
        }
    }
}
void mot_increase(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    if(sPnt->D_rdy_for == 0){
        sPnt->D_rdy_for = mot_disconnect_FC();
    }
	if(sPnt->T_Razm == 1){
		save_s16(ramPnt, progPnt->Pnt_Out, 1);
	}
    if(sPnt->T_Razm == 0){
        mot_connect_AC(sPnt->D_rdy_for);
        if(load_s16(ramPnt, progPnt->Pnt_Prior)){
            mot_connect_FC_PR(progPnt, ramPnt, sPnt);
        }else{
            mot_connect_FC_MH(progPnt, ramPnt, sPnt);
        }
        sPnt->razmagnich_now = 0;
        sPnt->T_Razm = load_s16(ramPnt, progPnt->Pnt_T_Razm) * K_TIME_CALL_MICRO_TAKT / 10;
        sPnt->wait = (u32)load_s16(ramPnt, progPnt->Pnt_T_Dop) * K_TIME_CALL_MICRO_TAKT * 60;
        save_s16(ramPnt, progPnt->Pnt_Out, 0);
    }else{
        sPnt->T_Razm--;
        sPnt->razmagnich_now = 1;
    }
}
void mot_state(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    Mot.prior = load_s16(ramPnt, progPnt->Pnt_Prior);
    connect_New_FC(progPnt, ramPnt, sPnt);
    // ѕроверка разрешени€ первого двигател€
    if (testBitMicro(ramPnt, progPnt->Pnt_Ena1)){
        Mot.Ena1 = RAZRESHON;
    }else{
        Mot.Ena1 = ZAPRESCHEN;
        Mot.ON1 = OFF;
        if(Mot.M1_FC){
            Mot.M1_FC = OFF;
            save_s16(ramPnt, progPnt->Pnt_Out, 1);
            Mot.connectNewFC = 1;
        }
        Mot.M1_AC = OFF;
    }
    // ѕроверка разрешени€ второго двигател€
    if (testBitMicro(ramPnt, progPnt->Pnt_Ena2)){
        Mot.Ena2 = RAZRESHON;
    }else{
        Mot.Ena2 = ZAPRESCHEN;
        Mot.ON2 = OFF;
        if(Mot.M2_FC){
            Mot.M2_FC = OFF;
            save_s16(ramPnt, progPnt->Pnt_Out, 1);
            Mot.connectNewFC = 1;
        }
        Mot.M2_AC = OFF;
    }
    // ѕроверка разрешени€ тертьего двигател€
    if (testBitMicro(ramPnt, progPnt->Pnt_Ena3)){
        Mot.Ena3 = RAZRESHON;
    }else{
        Mot.Ena3 = ZAPRESCHEN;
        Mot.ON3 = OFF;
        if(Mot.M3_FC){
            Mot.M3_FC = OFF;
            save_s16(ramPnt, progPnt->Pnt_Out, 1);
            Mot.connectNewFC = 1;
        }
        Mot.M3_AC = OFF;
    }
}
u16 mot_disconnect_FC(){
    if(Mot.M1_FC){
        if(Mot.M2_FC){
            if(Mot.M3_FC){
                internal_fail();
            }else{
                internal_fail();
            }
        }else{
            if(Mot.M3_FC){
                internal_fail();
            }else{
                Mot.M1_FC = OFF;
                return RDYMOT_1;
            }
        }
    }else{
        if(Mot.M2_FC){
            if(Mot.M3_FC){
                internal_fail();
            }else{
                Mot.M2_FC = OFF;
                return RDYMOT_2;
            }
        }else{
            if(Mot.M3_FC){
                Mot.M3_FC = OFF;
                return RDYMOT_3;
            }else{
                internal_fail();
            }
        }
    }
    return 0;
}
void mot_connect_AC(u16 numb){
    switch (numb){
    case RDYMOT_FAIL:
        internal_fail();
        break;
    case RDYMOT_1:
        if((Mot.Ena1 == RAZRESHON) && (((Mot.Ena2 == RAZRESHON) && (Mot.ON2 == OFF)) || ((Mot.Ena3 == RAZRESHON)&& (Mot.ON3 == OFF)))){
            Mot.M1_AC = 1;
            Mot.ON1 = 1;
        }else{
            internal_fail();
        }
        break;
    case RDYMOT_2:
        if((Mot.Ena2 == RAZRESHON) && (((Mot.Ena1 == RAZRESHON) && (Mot.ON1 == OFF)) || ((Mot.Ena3 == RAZRESHON)&& (Mot.ON3 == OFF)))){
            Mot.M2_AC = 1;
            Mot.ON2 = 1;
        }else{
            internal_fail();
        }
        break;
    case RDYMOT_3:
        if((Mot.Ena3 == RAZRESHON) && (((Mot.Ena2 == RAZRESHON) && (Mot.ON2 == OFF)) || ((Mot.Ena1 == RAZRESHON)&& (Mot.ON1 == OFF)))){
            Mot.M3_AC = 1;
            Mot.ON3 = 1;
        }else{
            internal_fail();
        }
        break;
    default:
        internal_fail();
        break;
    }
}
void mot_connect_FC_MH(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    if((Mot.ON1 == 0) && (Mot.Ena1 == RAZRESHON)){
       if((Mot.ON2 == 0) && (Mot.Ena2 == RAZRESHON)){
           if((Mot.ON3 == 0) && (Mot.Ena3 == RAZRESHON)){
               internal_fail();
           }else{
               if(sPnt->MH1 > sPnt->MH2){
                   Mot.ON2 = 1;
                   Mot.M2_FC = 1;
               }else{
                   Mot.ON1 = 1;
                   Mot.M1_FC = 1;
               }
           }
       }else{
           if((Mot.ON3 == 0) && (Mot.Ena3 == RAZRESHON)){
             if(sPnt->MH1 > sPnt->MH3){
                  Mot.ON3 = 1;
                  Mot.M3_FC = 1;
              }else{
                  Mot.ON1 = 1;
                  Mot.M1_FC = 1;
              }
         }else{
             Mot.ON1 = 1;
             Mot.M1_FC = 1;
         }
       }
    }else{
       if((Mot.ON2 == 0) && (Mot.Ena2 == RAZRESHON)){
          if((Mot.ON3 == 0) && (Mot.Ena3 == RAZRESHON)){
              if(sPnt->MH2 > sPnt->MH3){
                  Mot.ON3 = 1;
                  Mot.M3_FC = 1;
              }else{
                  Mot.ON2 = 1;
                  Mot.M2_FC = 1;
              }
          }else{
              Mot.ON2 = 1;
              Mot.M2_FC = 1;
          }
      }else{
          if((Mot.ON3 == 0) && (Mot.Ena3 == RAZRESHON)){
              Mot.ON3 = 1;
              Mot.M3_FC = 1;
          }else{
              internal_fail();
          }
      }
    }
}
void mot_connect_FC_PR(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    if((Mot.ON1 == 0) && (Mot.Ena1 == RAZRESHON)){
       if((Mot.ON2 == 0) && (Mot.Ena2 == RAZRESHON)){
           if((Mot.ON3 == 0) && (Mot.Ena3 == RAZRESHON)){
               internal_fail();
           }else{
               if(sPnt->MPR1 > sPnt->MPR2){
                   Mot.ON2 = 1;
                   Mot.M2_FC = 1;
               }else{
                   Mot.ON1 = 1;
                   Mot.M1_FC = 1;
               }
           }
       }else{
           if((Mot.ON3 == 0) && (Mot.Ena3 == RAZRESHON)){
             if(sPnt->MPR1 > sPnt->MPR3){
                  Mot.ON3 = 1;
                  Mot.M3_FC = 1;
              }else{
                  Mot.ON1 = 1;
                  Mot.M1_FC = 1;
              }
         }else{
             Mot.ON1 = 1;
             Mot.M1_FC = 1;
         }
       }
    }else{
       if((Mot.ON2 == 0) && (Mot.Ena2 == RAZRESHON)){
          if((Mot.ON3 == 0) && (Mot.Ena3 == RAZRESHON)){
              if(sPnt->MPR2 > sPnt->MPR3){
                  Mot.ON3 = 1;
                  Mot.M3_FC = 1;
              }else{
                  Mot.ON2 = 1;
                  Mot.M2_FC = 1;
              }
          }else{
              Mot.ON2 = 1;
              Mot.M2_FC = 1;
          }
      }else{
          if((Mot.ON3 == 0) && (Mot.Ena3 == RAZRESHON)){
              Mot.ON3 = 1;
              Mot.M3_FC = 1;
          }else{
              internal_fail();
          }
      }
    }
}
void contactor_man(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    if (Mot.M1_AC){
        if (Mot.M1_FC){
            Mot.ON1 = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_M1_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M1_FC);
            internal_fail();
        }else{
            setBitMicro(ramPnt, progPnt->Pnt_M1_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M1_FC);
            Mot.ON1 = 1;
        }
    }else{
        if (Mot.M1_FC){
            setBitMicro(ramPnt, progPnt->Pnt_M1_FC);
            clrBitMicro(ramPnt, progPnt->Pnt_M1_AC);
            Mot.ON1 = 1;
        }else{
            Mot.ON1 = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_M1_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M1_FC);
        }
    }
    if (Mot.M2_AC){
        if (Mot.M2_FC){
            Mot.ON2 = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_M2_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M2_FC);
            internal_fail();
        }else{
            setBitMicro(ramPnt, progPnt->Pnt_M2_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M2_FC);
            Mot.ON2 = 1;
        }
    }else{
        if (Mot.M2_FC){
            setBitMicro(ramPnt, progPnt->Pnt_M2_FC);
            clrBitMicro(ramPnt, progPnt->Pnt_M2_AC);
            Mot.ON2 = 1;
        }else{
            Mot.ON2 = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_M2_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M2_FC);
        }
    }
    if (Mot.M3_AC){
        if (Mot.M3_FC){
            Mot.ON3 = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_M3_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M3_FC);
            internal_fail();
        }else{
            setBitMicro(ramPnt, progPnt->Pnt_M3_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M3_FC);
            Mot.ON3 = 1;
        }
    }else{
        if (Mot.M3_FC){
            setBitMicro(ramPnt, progPnt->Pnt_M3_FC);
            clrBitMicro(ramPnt, progPnt->Pnt_M3_AC);
            Mot.ON3 = 1;
        }else{
            Mot.ON3 = 0;
            clrBitMicro(ramPnt, progPnt->Pnt_M3_AC);
            clrBitMicro(ramPnt, progPnt->Pnt_M3_FC);
        }
    }
    defBitMicro(ramPnt, progPnt->Pnt_M1_IU, Mot.ON1);
    defBitMicro(ramPnt, progPnt->Pnt_M2_IU, Mot.ON2);
    defBitMicro(ramPnt, progPnt->Pnt_M3_IU, Mot.ON3);
}
void busywork(AUTOVENTILState_type *sPnt){
    if(Mot.ON1 || (!Mot.Ena1)){
        if(Mot.ON2 || (!Mot.Ena2)){
            if(Mot.ON3 || (!Mot.Ena3)){
                sPnt->systemOverload = 1;
            }else{
                sPnt->systemOverload = 0;
            }
        }else{
            sPnt->systemOverload = 0;
        }
    }else{
        sPnt->systemOverload = 0;
    }
    if(!Mot.ON1 && !Mot.ON2 && !Mot.ON3){
        sPnt->systemEdle = 1;
    }else{
        sPnt->systemEdle = 0;
    }
}
void prior(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    s16 mpr1, mpr2, mpr3;
    mpr1 = load_s16(ramPnt, progPnt->Pnt_Mot1);
    mpr2 = load_s16(ramPnt, progPnt->Pnt_Mot2);
    mpr3 = load_s16(ramPnt, progPnt->Pnt_Mot3);

    if (sPnt->MPR1 == 0)
        sPnt->MPR1 = mpr1;
    if (sPnt->MPR2 == 0)
        sPnt->MPR2 = mpr2;
    if (sPnt->MPR3 == 0)
        sPnt->MPR3 = mpr3;
    if (sPnt->MPR1 != mpr1){
        if(mpr1 == sPnt->MPR2){
            sPnt->MPR2 = sPnt->MPR1;
            save_s16(ramPnt, progPnt->Pnt_Mot2, sPnt->MPR1);
            sPnt->MPR1 = mpr1;
            return;
        }else{
            sPnt->MPR3 = sPnt->MPR1;
            save_s16(ramPnt, progPnt->Pnt_Mot3, sPnt->MPR1);
            sPnt->MPR1 = mpr1;
            return;
        }
    }
    if (sPnt->MPR2 != mpr2){
        if(mpr2 == sPnt->MPR1){
            sPnt->MPR1 = sPnt->MPR2;
            save_s16(ramPnt, progPnt->Pnt_Mot1, sPnt->MPR2);
            sPnt->MPR2 = mpr2;
            return;
        }else{
            sPnt->MPR3 = sPnt->MPR2;
            save_s16(ramPnt, progPnt->Pnt_Mot3, sPnt->MPR2);
            sPnt->MPR2 = mpr2;
            return;
        }
    }
    if (sPnt->MPR3 != mpr3){
        if(mpr3 == sPnt->MPR1){
            sPnt->MPR1 = sPnt->MPR3;
            save_s16(ramPnt, progPnt->Pnt_Mot1, sPnt->MPR3);
            sPnt->MPR3 = mpr3;
            return;
        }else{
            sPnt->MPR2 = sPnt->MPR3;
            save_s16(ramPnt, progPnt->Pnt_Mot2, sPnt->MPR3);
            sPnt->MPR3 = mpr3;
            return;
        }
    }
    if ((mpr1 == mpr2)||(mpr2 == mpr3)||(mpr1 == mpr3))
    {
        save_s16(ramPnt, progPnt->Pnt_Mot1, 1);
        save_s16(ramPnt, progPnt->Pnt_Mot2, 2);
        save_s16(ramPnt, progPnt->Pnt_Mot3, 3);
        sPnt->MPR1 = 1;
        sPnt->MPR2 = 2;
        sPnt->MPR3 = 3;

        return;
    }
}
void prior_MH(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    sPnt->MH1 = load_s16(ramPnt, progPnt->Pnt_MH1);
    sPnt->MH2 = load_s16(ramPnt, progPnt->Pnt_MH2);
    sPnt->MH3 = load_s16(ramPnt, progPnt->Pnt_MH3);
}
void rotatsnuti(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){
    s16 T_dvigMax, T_dvig1, T_dvig2, T_dvig3;
    T_dvigMax = load_s16(ramPnt, progPnt->Pnt_T_rot);
    if(T_dvigMax > 0){
        T_dvig1 = (s16)load_s32(ramPnt, progPnt->Pnt_M_ST1);
        T_dvig2 = (s16)load_s32(ramPnt, progPnt->Pnt_M_ST2);
        T_dvig3 = (s16)load_s32(ramPnt, progPnt->Pnt_M_ST3);
        if(T_dvig3 >= T_dvigMax){
            if(T_dvig2 >= T_dvigMax){
                if(T_dvig1 >= T_dvigMax){
                    internal_fail();
                }else{
                    if(Mot.M1_FC){
                        if(sPnt->MH2 > sPnt->MH3){
                            mot_disconnect_FC();
                            save_s16(ramPnt, progPnt->Pnt_Out, 1);
                            Mot.conRotM2 = 1;
                        }else{
                            mot_disconnect_FC();
                            save_s16(ramPnt, progPnt->Pnt_Out, 1);
                            Mot.conRotM3 = 1;
                        }
                    }else{
                        internal_fail();
                    }
                }
            }else{
                if(T_dvig1 >= T_dvigMax){
                    if(Mot.M2_FC){
                        if(sPnt->MH1 > sPnt->MH3){
                            mot_disconnect_FC();
                            save_s16(ramPnt, progPnt->Pnt_Out, 1);
                            Mot.conRotM1 = 1;
                        }else{
                            mot_disconnect_FC();
                            save_s16(ramPnt, progPnt->Pnt_Out, 1);
                            Mot.conRotM3 = 1;
                        }
                    }else{
                        internal_fail();
                    }
                }else{
                    mot_disconnect_FC();
                    save_s16(ramPnt, progPnt->Pnt_Out, 1);
                    Mot.conRotM3 = 1;
                }
            }
        }else{
            if(T_dvig2 >= T_dvigMax){
                if(T_dvig1 >= T_dvigMax){
                    if(Mot.M3_FC){
                        if(sPnt->MH1 > sPnt->MH2){
                            mot_disconnect_FC();
                            save_s16(ramPnt, progPnt->Pnt_Out, 1);
                            Mot.conRotM1 = 1;
                        }else{
                            mot_disconnect_FC();
                            save_s16(ramPnt, progPnt->Pnt_Out, 1);
                            Mot.conRotM3 = 1;
                        }
                    }else{
                        internal_fail();
                    }
                }else{
                    mot_disconnect_FC();
                    save_s16(ramPnt, progPnt->Pnt_Out, 1);
                    Mot.conRotM2 = 1;
                }
            }else{
                if(T_dvig1 >= T_dvigMax){
                    mot_disconnect_FC();
                    save_s16(ramPnt, progPnt->Pnt_Out, 1);
                    Mot.conRotM1 = 1;
                }
            }
        }
    }
}
void connect_New_FC(FuncAUTOVENTIL_type *progPnt, u32 ramPnt, AUTOVENTILState_type *sPnt){

    if(Mot.connectNewFC == 1){
        if(load_s32(ramPnt, progPnt->Pnt_Fref) == 0){
            if(Mot.prior){
                mot_connect_FC_PR(progPnt, ramPnt, sPnt);
            }else{
                mot_connect_FC_MH(progPnt, ramPnt, sPnt);
            }
            Mot.connectNewFC = 0;
            save_s16(ramPnt, progPnt->Pnt_Out, 0);
        }
    }
}
void internal_fail(){
    Mot.fail = 1;
}

#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncAUTOVENTIL_1[38]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1 ON_OFF
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2 Fref
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3 Err_Pid
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4 maxPID
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5 minPID
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6 Dopusk
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7 T_Dop
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8 Prior
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9 T_PID
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10 T_Razm
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11 T_rot
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12 Mot1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13 Mot2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14 Mot3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //15 Ena1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //16 Ena2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //17 Ena3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18 MH1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //19 MH2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //20 MH3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //21 M_ST1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //22 M_ST2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //23 M_ST3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //24 In_Enable
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25 M3_IU
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26 M2_IU
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //27 M1_IU
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28 MHO3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //29 MHO2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //30 MHO1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //31 M3_AC
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //32 M2_AC
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //33 M1_AC
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //34 M3_FC
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //35 M2_FC
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //36 M1_FC
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //37 Out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //38 Fail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("AUTOVENTIL", "U", TblFuncAUTOVENTIL_1, FuncAUTOVENTIL_1, sizeof(TblFuncAUTOVENTIL_1), sizeof(AUTOVENTILState_type), sizeof(nvAUTOVENTILState_type) , 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncAUTOVENTIL_1,
#endif
//********************************************************
