#ifdef    GET_FUNC_CODE
#include "math.h"
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In;                           //1  In
    u16  Pnt_Type;                         //2  Type
    u16  Pnt_K;                            //3  K
    u16  Pnt_B;                            //4  B
    u16  Pnt_A1;                           //5  A1
    u16  Pnt_A2;                           //6  A2
    u16  Pnt_A3;                           //7  A3
    u16  Pnt_S;                            //8  S
    u16  Pnt_Kro;                          //9  Kro
    u16  Pnt_Out;                          //10  Out
    u16  Pnt_End;
}FuncFB_FUNC_type;
//********************************************************
u16 *FuncFB_FUNC_1(FuncFB_FUNC_type *progPnt, u32 ramPnt)
{
    float in, out;
    float A1, A2, A3;
    float S, Kro;  

    in = load_float(ramPnt, progPnt->Pnt_In);         // Получить ОС ПИД

    switch(load_s16(ramPnt, progPnt->Pnt_Type)){    // Тип преобразователя 
        case 0:  // Нет преобразования
            out = in; //Y = X;
            break;

        case 1:  // Линейное преобразование
            out = ((f32)load_s16(ramPnt, progPnt->Pnt_K)) * 0.1f * in + 0.1f * ((f32)load_s16(ramPnt, progPnt->Pnt_B));   // Y = K*X + B;
            break;

        case 2:  // Корень квадратный
            out = sqrtf(in);                              // Y = sqrt(X);
            break;

        case 3:  // Давление в температуру
            A1 = ((float)load_s16(ramPnt, progPnt->Pnt_A1)) * 0.1f;
            A2 = ((float)load_s16(ramPnt, progPnt->Pnt_A2)) * 0.1f;
            A3 = ((float)load_s16(ramPnt, progPnt->Pnt_A3)) * 0.1f;

	    #ifdef _PROJECT_FOR_STM32_
                out = (A2 / (logf(in + 1.0f) - A1)) - A3;     // Y = A2/(ln(X+1) - A1) - A3;
            #else 
                out = 0; 
            #endif // _PROJECT_FOR_STM32_

            break;

        case 4:  // Давление в поток
            S = ((float)load_s16(ramPnt, progPnt->Pnt_S)) * 0.0001f;
            Kro = ((float)load_s16(ramPnt, progPnt->Pnt_Kro)) * 0.001f;
            out = S * Kro * sqrtf(in);                    // out = S*K*sqrt(X)
            break;

        case 5:  // Скорость в поток
            S = ((float)load_s16(ramPnt, progPnt->Pnt_S)) * 0.0001f;
            out = S * in;                                 // Y = S*X
            break;
    }

    save_float(ramPnt, progPnt->Pnt_Out, out);

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFB_FUNC_1[10]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Type
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  K
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  B
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  A1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  A2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  A3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  S
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  Kro
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //10  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("FB_FUNC", "DA", TblFuncFB_FUNC_1, FuncFB_FUNC_1, sizeof(TblFuncFB_FUNC_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFB_FUNC_1,
#endif
//********************************************************
