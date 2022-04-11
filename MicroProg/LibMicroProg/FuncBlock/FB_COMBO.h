#ifdef    GET_FUNC_CODE

#define NUSE 0
#define AIN1 1
#define AIN2 2
#define AIN3 3
#define AIN4 4
#define AIN5 5

#define OS1      0
#define OS2      1
#define SUM      2
#define RAZN     3
#define SREDN    4
#define MIN      5
#define MAX      6
#define MIN_RAZN 7
#define MAX_RAZN 8

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Ain1;                         //1  Ain1
    u16  Pnt_Ain2;                         //2  Ain2
    u16  Pnt_Ain3;                         //3  Ain3
    u16  Pnt_Ain4;                         //4  Ain4
    u16  Pnt_Ain5;                         //5  Ain5
    u16  Pnt_Addit1;                       //6  Addit1
    u16  Pnt_Addit2;                       //7  Addit2
    u16  Pnt_Addit3;                       //8  Addit3
    u16  Pnt_SelFB1;                       //9  SelFB1
    u16  Pnt_SelFB2;                       //10  SelFB2
    u16  Pnt_SelFB3;                       //11  SelFB3
    u16  Pnt_SelType;                      //12  SelType
    u16  Pnt_Out;                          //13  Out
    u16  Pnt_FB_frmt;                      //14  FB_Format
    u16  Pnt_WarnNoAin;                    //15  WarnNoAin
    u16  Pnt_End;
}FuncFB_COMBO_type;
//********************************************************
u16 *FuncFB_COMBO_1(FuncFB_COMBO_type *progPnt, u32 ramPnt)
{
    float fb1, fb2, fb3, parFB1, parFB2, parFB3, out;
    u16   fb1_Ain, fb2_Ain, out_Ain;

    parFB1 = load_float(ramPnt, progPnt->Pnt_Addit1);
    parFB2 = load_float(ramPnt, progPnt->Pnt_Addit2);
    parFB3 = load_float(ramPnt, progPnt->Pnt_Addit3);
    fb1_Ain = load_s16(ramPnt, progPnt->Pnt_SelFB1);
    fb2_Ain = load_s16(ramPnt, progPnt->Pnt_SelFB2);
        
    // 1) Получить значение ОС1
    switch(fb1_Ain){
        case NUSE:
            fb1 = 0;
            break;
        case AIN1:
            fb1 = load_float(ramPnt, progPnt->Pnt_Ain1);
            break;
        case AIN2:
            fb1 = load_float(ramPnt, progPnt->Pnt_Ain2);
            break;
        case AIN3:
            fb1 = load_float(ramPnt, progPnt->Pnt_Ain3);
            break;
        case AIN4:
            fb1 = load_float(ramPnt, progPnt->Pnt_Ain4);
            break;
        case AIN5:
            fb1 = load_float(ramPnt, progPnt->Pnt_Ain5);
            break;
        default:
            fb1 = load_float(ramPnt, progPnt->Pnt_Ain1);
            break;
    }
    
    // 2) Получить значение ОС2
    switch(fb2_Ain){
        case NUSE:
            fb2 = 0;
            break;
        case AIN1:
            fb2 = load_float(ramPnt, progPnt->Pnt_Ain1);
            break;
        case AIN2:
            fb2 = load_float(ramPnt, progPnt->Pnt_Ain2);
            break;
        case AIN3:
            fb2 = load_float(ramPnt, progPnt->Pnt_Ain3);
            break;
        case AIN4:
            fb2 = load_float(ramPnt, progPnt->Pnt_Ain4);
            break;
        case AIN5:
            fb2 = load_float(ramPnt, progPnt->Pnt_Ain5);
            break;
        default:
            fb2 = load_float(ramPnt, progPnt->Pnt_Ain1);
            break;
    }
    
    // 3) Получить значение ОС3
    switch(load_s16(ramPnt, progPnt->Pnt_SelFB3)){
        case NUSE:
            fb3 = 0;
            break;
        case AIN1:
            fb3 = load_float(ramPnt, progPnt->Pnt_Ain1);
            break;
        case AIN2:
            fb3 = load_float(ramPnt, progPnt->Pnt_Ain2);
            break;
        case AIN3:
            fb3 = load_float(ramPnt, progPnt->Pnt_Ain3);
            break;
        case AIN4:
            fb3 = load_float(ramPnt, progPnt->Pnt_Ain4);
            break;
        case AIN5:
            fb3 = load_float(ramPnt, progPnt->Pnt_Ain5);
            break;
        default:
            fb3 = load_float(ramPnt, progPnt->Pnt_Ain1);
            break;
    }
    
    // 4) Расчитать ОС на основании значений 3 ОС
    switch(load_s16(ramPnt, progPnt->Pnt_SelType)){
      
        case OS1:
          out = fb1;    // ОС1
          out_Ain = fb1_Ain;
          break;
          
        case OS2:
          out = fb2;    // ОС2
          out_Ain = fb2_Ain;          
          break;
            
        case SUM:{                            // ОС1 + ОС2 + ОС3
            out_Ain = fb1_Ain; // Все ОС должны быть одинаковыми
            out = fb1 + fb2 + fb3;
            if (out > 1.0f){
                 out = 1.0f;
            }
        } break;
        
        case RAZN:{                           // ОС1 - ОС2
            out_Ain = fb1_Ain; // Все ОС должны быть одинаковыми          
            out = fb1 - fb2;
        } break;
        
        case SREDN:{                          // Среднее ОС1..ОС3 
            out_Ain = fb1_Ain; // Все ОС должны быть одинаковыми          
            out = (fb1 + fb2 + fb3) * 0.33333333333f;
        } break;
        
        case MIN:{                            // Минимальное значение
            out_Ain = fb1_Ain; // Все ОС должны быть одинаковыми
            if(fb1 < fb2){
                if(fb1 < fb3){
                    out = fb1;
                }
                else{
                    out = fb3;
                }
            }
            else{
                if(fb2 < fb3){
                    out = fb2;
                }
                else{
                    out = fb3;
                }
        } break;
        
        case MAX:{                            // Максимальное значение
            out_Ain = fb1_Ain; // Все ОС должны быть одинаковыми          
            if(fb1 > fb2){
                if(fb1 > fb3){
                    out = fb1;
                }
                else{
                    out = fb3;
                }
            }
            else{
                if(fb2 > fb3){
                    out = fb2;
                }
                else{
                    out = fb3;
                }
            }
        } break;
                
        case MIN_RAZN:{                      // Наименьшее значение Доп ОС
            out_Ain = fb1_Ain; // Все ОС должны быть одинаковыми          
            fb1 -= parFB1;
            fb2 -= parFB2;
            fb3 -= parFB3;
            
            if(fb1 < fb2){
                if(fb1 < fb3){
                    out = fb1;
                }
                else{
                    out = fb3;
                }
            }
            else{
                if(fb2 < fb3){
                    out = fb2;
                }
                else{
                    out = fb3;
                }
            }
        } break;
        
        case MAX_RAZN:
            out_Ain = fb1_Ain; // Все ОС должны быть одинаковыми          
            fb1 -= parFB1;
            fb2 -= parFB2;
            fb3 -= parFB3;
            if(fb1 > fb2){
                if(fb1 > fb3){
                    out = fb1;
                }
                else{
                    out = fb3;
                }
            }
            else{
                if(fb2 > fb3){
                    out = fb2;
                }
                else{
                    out = fb3;
                }
            }
        } break;
        default:
                out_Ain = fb1_Ain; // Все ОС должны быть одинаковыми          
                out = fb1 + fb2 + fb3;
                break;
            }
                    
    save_float(ramPnt, progPnt->Pnt_Out, out);
    save_s16(ramPnt, progPnt->Pnt_FB_frmt, out_Ain); // Указываем тип Авх который сейчас активный
	
    // Проверка доступности аналоговых входов 3 и 4.
    if (!GlobalM4.AinAoutExtData.AinAoutBlockPresent && (fb1_Ain > AIN2 || fb2_Ain > AIN2)) {
       // Если блока нет но выбраны его входы, выдать предупреждение
       setBitMicro (ramPnt, progPnt->Pnt_WarnNoAin);
    } else {
      // Сбросить предупреждение
      clrBitMicro (ramPnt, progPnt->Pnt_WarnNoAin);
    }
        
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFB_COMBO_1[15]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ain1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Ain2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Ain3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Ain4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  Ain5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Addit1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  Addit2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Addit3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  SelFB1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  SelFB2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  SelFB3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  SelType
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //13  Out
        S16_PIN_TYPE | OUTPUT_PIN_MODE,     //14  FB_frmt
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //15  WarnNoAin          
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FB_COMBO", "DA", TblFuncFB_COMBO_1, FuncFB_COMBO_1, sizeof(TblFuncFB_COMBO_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFB_COMBO_1,
#endif
//********************************************************
