#ifdef    GET_FUNC_CODE

#ifdef _NVSA_
    #include "NVSAGPIODef.h"
#else /** _NVSA_ */
   #include "NVSACgpiodef.h"
#endif /** _NVSA_ */

#ifdef   _PROJECT_FOR_CCS_
#include "DrvSpiForVPC3.h"
#include "VPC3+S.h"
#endif

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_In1;                          //1  In1
    u16  Pnt_In2;                          //2  In2
    u16  Pnt_In3;                          //3  In3
    u16  Pnt_Out1;                         //4  Out1
    u16  Pnt_Out2;                         //5  Out2
    u16  Pnt_Out3;                         //6  Out3
    u16  Pnt_End;
}FuncACTIVEPROTOCOL_type;
//********************************************************

enum{
	ModBus=0,
	FF,
	LAN,
	ProfiBus,
	NoANET
};

u16 *FuncACTIVEPROTOCOL_1(FuncACTIVEPROTOCOL_type *progPnt, u32 ramPnt)
{
 #ifdef   _PROJECT_FOR_CCS_

	unsigned char Add = 0;
	static unsigned char  Addprv = 0;
	//char ddRd = 0;
	//char ddWr = 0xAA;
	//char adrBuf;

	Add = SEL1_0_STATE;
	/**Add |= GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0);
	Add |= GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1);*/

	if((Addprv != Add) || flagIsFirstCall){

		Addprv = Add;

		switch(Add){
			case ModBus:
#ifdef _NVSA_
				save_s16(ramPnt, progPnt->Pnt_Out1, ModBus);
				Task_setPri(TSK_MasterUart, 3);
				Task_setPri(TSK_uart, 2);
				Task_setPri(TSK_Profi, -1);
#else /*_NVSAC_*/
				save_s16(ramPnt, progPnt->Pnt_Out1, ModBus);
				Task_setPri(TSK_uart, 2);
				Task_setPri(TSK_Profi, -1);
				Task_setPri(TSK_EXT2, -1);
#endif /*_NVSA_*/
				break;
			case FF:
				Task_setPri(TSK_EXT2, 4);
				Task_setPri(TSK_Profi, -1);
				Task_setPri(TSK_uart, -1);
				save_s16(ramPnt, progPnt->Pnt_Out1, FF);

				/**Task_setPri(TSK_Profi, -1);
				Task_setPri(TSK_uart, -1);
				save_s16(ramPnt, progPnt->Pnt_Out1, FF);*/
				break;
			case LAN:
				Task_setPri(TSK_Profi, -1);
				Task_setPri(TSK_uart, -1);
				Task_setPri(TSK_EXT2, -1);
				save_s16(ramPnt, progPnt->Pnt_Out1, LAN);

				/**Task_setPri(TSK_Profi, -1);
				Task_setPri(TSK_uart, -1);
				save_s16(ramPnt, progPnt->Pnt_Out1, LAN);*/
				break;
			case ProfiBus:
				//adrBuf = GET_DP_BUFFERS(Diag_Buf2[0]);
				//writeVPC3(adrBuf, &ddWr, sizeof(ddWr));
				//readVPC3(&ddRd, adrBuf, sizeof(ddRd));

				Task_setPri(TSK_Profi, 2);
				Task_setPri(TSK_uart, -1);
				Task_setPri(TSK_EXT2, -1);
				save_s16(ramPnt, progPnt->Pnt_Out1, ProfiBus);

				/**Task_setPri(TSK_Profi, 1);*/

				/*if(ddRd == ddWr){
					Task_setPri(TSK_Profi, 1);
					Task_setPri(TSK_uart, -1);
					save_s16(ramPnt, progPnt->Pnt_Out1, ProfiBus);
				}
				else{
					Task_setPri(TSK_Profi, -1);
					Task_setPri(TSK_uart, -1);
					save_s16(ramPnt, progPnt->Pnt_Out1, NoANET);
				}*/
				break;
		}
	}

 #endif //-- _PROJECT_FOR_CCS_
    return &progPnt->Pnt_End;
};
#endif

//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncACTIVEPROTOCOL_1[6]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  In2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  In3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //4  Out1
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //5  Out2
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //6  Out3
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("ACTIVEPROTOCOL", "U", TblFuncACTIVEPROTOCOL_1, FuncACTIVEPROTOCOL_1, sizeof(TblFuncACTIVEPROTOCOL_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncACTIVEPROTOCOL_1,
#endif
//********************************************************
