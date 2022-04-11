#ifdef    GET_FUNC_CODE

#include "GlobalVar.h"

extern BootCommonData_type BootCommonData;

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Crc_Main;                     //1  Crc_Main
    u16  Pnt_Ver_Main;                     //2  Ver_Main
    u16  Pnt_Subvers;                      //3  Subvers
    u16  Pnt_Crc_Boot;                     //4  Crc_Boot
    u16  Pnt_Ver_Boot;                     //5  Ver_Boot
    u16  Pnt_Sver_Boot;                    //6  Sver_Boot
    u16  Pnt_VfdSerial;                    //7  VFD Serial
    u16  Pnt_End;
}FuncGET_CRC_PO_type;
//********************************************************
u16 *FuncGET_CRC_PO_1(FuncGET_CRC_PO_type *progPnt, u32 ramPnt)
{
    // Версия ПО основного ПО
    save_s16(ramPnt, progPnt->Pnt_Crc_Main, GlobalM4.CrcProgFlash);
    save_s16(ramPnt, progPnt->Pnt_Ver_Main,VerProg);
    save_s16(ramPnt, progPnt->Pnt_Subvers,SubVerProg);
	
    // Версия ПО загрузчика
    save_s16(ramPnt, progPnt->Pnt_Crc_Boot, BootCommonData.CrcBoot);
    save_s16(ramPnt, progPnt->Pnt_Ver_Boot, BootCommonData.VerBoot);
    save_s16(ramPnt, progPnt->Pnt_Sver_Boot, BootCommonData.PodVerBoot);
    
    // Серийный номер ПЧ
    save_s32(ramPnt, progPnt->Pnt_VfdSerial, GlobalM4.VfdSerialNumber);
    
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncGET_CRC_PO_1[7]={
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //1  Crc_Main
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //2  Ver_Main
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //3  Subvers
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //4  Crc_Boot
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Ver_Boot
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //6  Sver_Boot        
        S32_PIN_TYPE | OUTPUT_PIN_MODE    //6  VFD serial
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("GET_CRC_PO", "U", TblFuncGET_CRC_PO_1, FuncGET_CRC_PO_1, sizeof(TblFuncGET_CRC_PO_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncGET_CRC_PO_1,
#endif
//********************************************************
