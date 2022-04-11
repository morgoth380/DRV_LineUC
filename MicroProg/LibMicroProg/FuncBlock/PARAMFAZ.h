#ifdef    GET_FUNC_CODE

#include "Faza.h"

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_FA;                           //1  FA
    u16  Pnt_FB;                           //2  FB
    u16  Pnt_FC;                           //3  FC
    u16  Pnt_FAZ;                          //4  FAZ
    u16  Pnt_End;
}FuncPARAMFAZ_type;
//********************************************************
u16 *FuncPARAMFAZ_1(FuncPARAMFAZ_type *progPnt, u32 ramPnt)
{
    static u16          timer = 5;

    if(timer){      // Для формирования сигнала для механизма АПВ по питанию
        timer--;
        setBitMicro(ramPnt, progPnt->Pnt_FA);
    }
    else{
        #ifdef _PROJECT_FOR_CCS_
//            defBitMicro(ramPnt, progPnt->Pnt_FA,  PhaseState.FailureA);
//            defBitMicro(ramPnt, progPnt->Pnt_FB,  PhaseState.FailureB);
//            defBitMicro(ramPnt, progPnt->Pnt_FC,  PhaseState.FailureC);


            defBitMicro(ramPnt, progPnt->Pnt_FA,  PhaseState.Failure);  // нов обрыв Partola

            save_s16   (ramPnt, progPnt->Pnt_FAZ, PhaseState.Fazirovka);
        #else
            clrBitMicro(ramPnt, progPnt->Pnt_FA);
        #endif    //_PROJECT_FOR_CCS_

    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPARAMFAZ_1[4]={
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //1  FA
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //2  FB
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //3  FC
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //4  FAZ
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PARAMFAZ", "DD", TblFuncPARAMFAZ_1, FuncPARAMFAZ_1, sizeof(TblFuncPARAMFAZ_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPARAMFAZ_1,
#endif
//********************************************************
