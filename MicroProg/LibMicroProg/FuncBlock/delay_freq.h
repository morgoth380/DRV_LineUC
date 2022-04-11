#ifdef    GET_FUNC_CODE
#ifdef _NVSA_
    #include "NVSADrvUart.h"
#else /** _NVSA_ */
    #include "NVSACDrvUart.h"
#endif /** _NVSA_ */

#include "Tsk_MasterUart.h"

typedef struct{
    u16  Pnt_Nfunc;
//    u16  Pnt_State;
//    u16  Pnt_nvState;
    u16  Pnt_Ftek_Input;                   //1  Ftek_Input
    u16  Pnt_Delay_time;                   //2  Delay_time
    u16  Pnt_Res1;                         //3  Res1 - Входная частота в IQ формате
    u16  Pnt_Res2;                         //4  Res2 = 0
    u16  Pnt_F_Out;                        //5  F_Out
    u16  Pnt_Res3;                         //6  Res3
    u16  Pnt_End;
}FuncDELAY_FREQ_type;
//********************************************************

#ifdef _PROJECT_FOR_CCS_
extern TGlobalMasterSlave GlobalMstSlv;
#else
      TGlobalMasterSlave GlobalMstSlv;
#endif //-- _PROJECT_FOR_CCS_

//********************************************************
// Мне нужен массив на 10 элементов s32
#define QUEUE_MULTIPLE 1 // параметр увеличения задержки в очереди.
#define QueueLen 11*QUEUE_MULTIPLE

s32 DelayQueue [QueueLen];
//********************************************************
u16 *FuncDELAY_FREQ_1(FuncDELAY_FREQ_type *progPnt, u32 ramPnt)
{
	u16  DelayTime,i, Uart0_Mode, Uart2_Mode;
	s32   freq;

	Uart0_Mode = GlobalMstSlv.Uart0_Mode; //режимы работы UARTов
	Uart2_Mode = GlobalMstSlv.Uart2_Mode; //режимы работы UARTов

	if (Uart0_Mode == ModbusSlave || Uart2_Mode == ModbusSlave) // Проверяем режимы работы UART'ов
	{ // Если режим работы UART'a - ModbusSlave, работаем по IQ формату частоты - Pnt_Res1
		// Если включен режим ModbusSlave - Значит наш ПЧ работает под управлением мастера
		// 1. Поэтому надо убрать из рассмотрения задержку установки частоты - она ведомому не нужна.
		// 2. Из параметра Pnt_Res1 - брать текущее значение частоты в IQ формате. Сюда мастер будет закидывать новую частоту.

		// Сбрасываем значение задержки установки частоты
//		save_s16(ramPnt, progPnt->Pnt_Delay_time, 0);

		// Загружаем задание частоты в IQ формате из переменной Fref_IQ_Slave - Блок MASTER_SLAVE
	    freq = load_s32 (ramPnt, progPnt->Pnt_Res1);
	    save_s32(ramPnt, progPnt->Pnt_F_Out, freq); // Передаем значение заданной частоты на выход
	    save_s32(ramPnt, progPnt->Pnt_Ftek_Input, freq); // !!!! Дублируем заданную частоту в качестве выходной из генератора темпа

#ifdef _PROJECT_FOR_CCS_
#warning НА ТЕКУЩИЙ МОМЕНТ ПАРАМЕТР Fref_IQ (Pnt_Res3) БОЛЬШЕ НЕ НУЖЕН, так как задатчиком выступает другой блок.
#endif //-- _PROJECT_FOR_CCS_

	    save_s32(ramPnt, progPnt->Pnt_Res3, freq);// Передаем значение на IQ Выход - НА ТЕКУЩИЙ МОМЕНТ ДАННЫЙ ПАРАМЕТР БОЛЬШЕ НЕ НУЖЕН
	}
	else
	{ // Если режим работы основного UART'a - НЕ ModbusSlave,
	  // оставляем возможности задержки установки частоты, так как в одиночном режиме работы
	  // это не влияет на работу ПЧ. Если же будет включен режим Ведущего - то задержка будет уже выполняться.

		DelayTime = load_s16 (ramPnt, progPnt->Pnt_Delay_time); // Заданное значение задержки
		DelayTime *= QUEUE_MULTIPLE; // Задержку увеличиваем в N раз

	    // сдвигаем все элементы на один
	    for (i=(QueueLen-1); i>0; i--)
	    { DelayQueue[i] = DelayQueue[i-1]; }

		// Берем текущую частоту с генератора темпа, и заносим в нулевой индекс
	    freq = load_s32 (ramPnt, progPnt->Pnt_Ftek_Input);
	    DelayQueue[0] = freq;

	    // Кроме всего прочего, текущую частоту в IQ формате, сразу размещаем в переменной Fref_IQ,
	    // если будет использоваться режим Ведущего - то частота будет там доступна и её можно брать через MakeOpis (52, 44)
	    save_s32(ramPnt, progPnt->Pnt_Res3, freq);// Передаем значение на выход

		// Берем значение из ячейки в соответствии с режимом задержки и выдаем на выход
	    freq = DelayQueue[DelayTime];
	    save_s32(ramPnt, progPnt->Pnt_F_Out, freq);// Передаем значение частоты на выход
	}

	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDELAY_FREQ_1[6]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Ftek_Input
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Delay_time
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Res1 - Входная частота в IQ формате
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  Res2 - Текущий режим работы ведомого UARТа - Режим работы UART2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //5  F_Out
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //6  Res3
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DELAY_FREQ", "U", TblFuncDELAY_FREQ_1, FuncDELAY_FREQ_1, sizeof(TblFuncDELAY_FREQ_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDELAY_FREQ_1,
#endif
//********************************************************
