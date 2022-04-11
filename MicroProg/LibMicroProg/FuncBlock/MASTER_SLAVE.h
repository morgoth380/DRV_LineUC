#ifdef    GET_FUNC_CODE

#ifdef _NVSA_
    #include "NVSADrvUart.h"
#else /** _NVSA_ */
    #include "NVSACDrvUart.h"
#endif /** _NVSA_ */

#include "MakeOpis_at24.h"
#include "Tsk_MasterUart.h"
#include "Global_include.h"         // Include header for IQmath library

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Fref_to_Slave;                //1  Fref_to_Slave
    u16  Pnt_MasterPingTime;               //2  Pnt_MasterPingTime - Интервал опроса Ведомого
    u16  Pnt_SlaveErrorCount;              //3  Pnt_SlaveErrorCount - Кол-во ошибок Ведомого
    u16  Pnt_StatusWord1;                  //4  StatusWord1
    u16  Pnt_Mode_uart0;                   //5  Mode_uart0
    u16  Pnt_Mode_uart2;                   //6  Mode_uart2
    u16  Pnt_SlvAnsTimeout;                //7  Pnt_SlvAnsTimeout - Таймаут Ведомого
    u16  Pnt_ResSlv2;                      //8  ResSlv2
    u16  Pnt_Fref_from_Mstr;               //9  Fref_from_Mstr
    u16  Pnt_ResSlv3;                      //10  ResSlv3
    u16  Pnt_Err_Slave;                     //11  Err_Slave
    u16  Pnt_Err_Modbus;                   //12  Err_Modbus
    u16  Pnt_ResMst3;                      //13  ResMst3
    u16  Pnt_Slave_Fail;                   //14  Slave_Fail
    u16  Pnt_End;
}FuncMASTER_SLAVE_type;
//********************************************************

#ifdef _PROJECT_FOR_CCS_
extern TGlobalMasterSlave GlobalMstSlv;
#else
      TGlobalMasterSlave GlobalMstSlv;
#endif //-- _PROJECT_FOR_CCS_

	  #define  REGIM_ALONE   0 /*0 - одиночный*/
      #define  REGIM_MASTER  1 /*1 - master*/
      #define  REGIM_SLAVE   2 /*2 - slave*/
      #define  REGIM_NEUPR   3 /*3 - неуправляемый*/

#define  ModbusASU     0
#define  ModbusSlave   1
#define  ModbusMaster  2

//********************************************************
u16 *FuncMASTER_SLAVE_1(FuncMASTER_SLAVE_type *progPnt, u32 ramPnt)
{
	// Данная п\п собирает в такте микропрограммы информацию и размещает её в глобальной структуре MasterSlave
	// Откуда потом Таск мастера будет брать необходимые данные или размещать их там
	// Данные о частоте для ведомого и его статусе здесь будут появляться из функции modbus_0x43 - спецфункция

//	s16 VoltageBase, FreqBase; // База по напряжению, для расчета текущего Ud, и по частоте вращения ротора
	s16 Uart0_Mode, Uart2_Mode;//,temp, temp_regim;
	s16 static cntStart=0, cntStop=0;

//	FreqBase = MPOpis.Base[NUM_HZ_BASE];
	Uart0_Mode = load_s16 (ramPnt, progPnt->Pnt_Mode_uart0); // Режим Uart0 - Дополнительный
	Uart2_Mode = load_s16 (ramPnt, progPnt->Pnt_Mode_uart2); // Режим Uart2 - Главный

	GlobalMstSlv.MasterPingTime = load_s16 (ramPnt, progPnt->Pnt_MasterPingTime);
	GlobalMstSlv.maxConnectError = load_s16 (ramPnt, progPnt->Pnt_SlaveErrorCount);
	GlobalMstSlv.slaveAnswerTimeout = load_s16 (ramPnt, progPnt->Pnt_SlvAnsTimeout);
	GlobalMstSlv.Uart0_Mode = Uart0_Mode; // Делаем копии состояний режимов работы UARTов
	GlobalMstSlv.Uart2_Mode = Uart2_Mode; // Делаем копии состояний режимов работы UARTов

	if(GlobalM3.FlagCommand.bit.BitGotovWorkSVP==0){
		if (Uart0_Mode == ModbusMaster || Uart2_Mode == ModbusMaster){
			GlobalMstSlv.Master_Fref_IQ = load_s32 (ramPnt, progPnt->Pnt_Fref_to_Slave);  // Запоминаем в глобальной переменной текущую частоту для ведомого
			GlobalMstSlv.MasterStatusWord1 = load_s16 (ramPnt, progPnt->Pnt_StatusWord1);  // Запоминаем в глобальной переменной текущий статус 1, который а данном месте является статусом мастера
			// Если в режиме Ведущего - частоту для логгера берем из GlobalMstSlv.Master_Fref_IQ
//			GlobalM3.CurrentDriveFreq = (u16)_IQmpyI32int(GlobalMstSlv.Master_Fref_IQ, FreqBase*10);
			if ( (GlobalMstSlv.SlaveStatusWord1 & 0x2000) == 0x2000){ // выделяем бит аварии
				defBitMicro(ramPnt, progPnt->Pnt_Slave_Fail   , 1); // Есть авария
			}else{
				defBitMicro(ramPnt, progPnt->Pnt_Slave_Fail   , 0); // Нет аварии
			}
		}
		if (Uart0_Mode == ModbusSlave || Uart2_Mode == ModbusSlave) {
			GlobalMstSlv.SlaveStatusWord1 = load_s16 (ramPnt, progPnt->Pnt_StatusWord1);
			// Снимаем задание из переменной
			save_s32(ramPnt, progPnt->Pnt_Fref_from_Mstr, GlobalMstSlv.Slave_Fref_IQ); // Передаем задание частоты
			// Если в режиме Ведомого - частоту для логгера берем из GlobalMstSlv.Slave_Fref_IQ
//			GlobalM3.CurrentDriveFreq = (u16)_IQmpyI32int(GlobalMstSlv.Slave_Fref_IQ, FreqBase*10);
			defBitMicro(ramPnt, progPnt->Pnt_Slave_Fail   , 0); // Нет аварии
			if ( GlobalMstSlv.SlaveStopRequest == 1) {
				if ( !testBitMicro(ramPnt, progPnt->Pnt_ResSlv3) ) { // Если старт еще не запущен, делаем пуск
					defBitMicro(ramPnt, progPnt->Pnt_ResSlv3, 1); // Выдаем команду стоп
					GlobalMstSlv.SlaveStopRequest = 0; //  И сразу её квитируем
					cntStop = 10;
				}
			} else if ( GlobalMstSlv.SlaveStartRequest == 1 && GlobalMstSlv.SlaveStopRequest == 0) {
				if (!testBitMicro(ramPnt, progPnt->Pnt_ResMst3)) { // Если стоп еще не запущен, делаем стоп
					defBitMicro(ramPnt, progPnt->Pnt_ResMst3 , 1); // Выдаем команду пуск
					GlobalMstSlv.SlaveStartRequest = 0; //  И сразу её квитируем
					cntStart = 10;
				}
			}
		}
		if (cntStart) cntStart--;
		if (cntStop) cntStop--;

		if (!cntStart){
			defBitMicro(ramPnt, progPnt->Pnt_ResMst3, 0); // Выдаем команду старт = 0
		}
		if (!cntStop){
			defBitMicro(ramPnt, progPnt->Pnt_ResSlv3, 0); // Выдаем команду стоп = 0
		}
	} else {//это режим свп
		switch(GlobalM3.Regim_Privod)
		{
			case REGIM_SLAVE:
				GlobalMstSlv.Uart0_Mode = ModbusSlave;
				GlobalMstSlv.SlaveStatusWord1 = load_s16 (ramPnt, progPnt->Pnt_StatusWord1);
				GlobalMstSlv.Slave_IQ_Torque = GlobalC28.Mout;//задание момента на слейв (выход РС)
				GlobalMstSlv.Slave_IQ_FREQ = GlobalC28.Wout;//задание скорости на слейв ( ОС скорости)

				// Снимаем задание из переменной
				save_s32(ramPnt, progPnt->Pnt_Fref_from_Mstr, GlobalMstSlv.Slave_Fref_IQ); // Передаем задание частоты
				GlobalM3.Master_IQ_Torque = GlobalMstSlv.Master_IQ_Torque;
				GlobalM3.Master_IQ_FREQ = GlobalMstSlv.Master_IQ_FREQ;// будет задание на момент скорость принять
//				GlobalM3.CurrentDriveFreq = (u16)_IQmpyI32int(GlobalMstSlv.Slave_Fref_IQ, FreqBase*10);
				defBitMicro(ramPnt, progPnt->Pnt_Slave_Fail   , 0); // Нет аварии
				GlobalMstSlv.ConnectErrors = 0; // Обнулить, если не режим мастера
				clrBitMicro(ramPnt, progPnt->Pnt_Err_Modbus); // Убрать аварию обмена если не мастер

				if ( GlobalMstSlv.SlaveStopRequest == 1) {
					if ( !testBitMicro(ramPnt, progPnt->Pnt_ResSlv3) ) { // Если старт еще не запущен, делаем пуск
						defBitMicro(ramPnt, progPnt->Pnt_ResSlv3, 1); // Выдаем команду стоп
						GlobalMstSlv.SlaveStopRequest = 0; //  И сразу её квитируем
						cntStop = 10;
					}
				} else if ( GlobalMstSlv.SlaveStartRequest == 1 && GlobalMstSlv.SlaveStopRequest == 0) {
					if (!testBitMicro(ramPnt, progPnt->Pnt_ResMst3)) { // Если стоп еще не запущен, делаем стоп
						defBitMicro(ramPnt, progPnt->Pnt_ResMst3 , 1); // Выдаем команду пуск
						GlobalMstSlv.SlaveStartRequest = 0; //  И сразу её квитируем
						cntStart = 10;
					}
				}
			break;
			case REGIM_NEUPR:
				GlobalMstSlv.Uart0_Mode = ModbusASU;
				GlobalMstSlv.SlaveStatusWord1 = load_s16 (ramPnt, progPnt->Pnt_StatusWord1); //МБ в неуправляемом режиме не нужно
				defBitMicro(ramPnt, progPnt->Pnt_Slave_Fail   , 0); // Нет аварии
				// Снимаем задание из переменной
				save_s32(ramPnt, progPnt->Pnt_Fref_from_Mstr, GlobalMstSlv.Slave_Fref_IQ); // Передаем задание частоты
///				GlobalM3.CurrentDriveFreq = (u16)_IQmpyI32int(GlobalMstSlv.Slave_Fref_IQ, FreqBase*10);
				GlobalMstSlv.ConnectErrors = 0; // Обнулить, если не режим мастера
				clrBitMicro(ramPnt, progPnt->Pnt_Err_Modbus); // Убрать аварию обмена если не мастер
			break;
			case REGIM_ALONE:
				GlobalMstSlv.Uart0_Mode= ModbusMaster;
				GlobalMstSlv.Master_Fref_IQ = load_s32 (ramPnt, progPnt->Pnt_Fref_to_Slave);  // Запоминаем в глобальной переменной текущую частоту для ведомого
				GlobalMstSlv.MasterStatusWord1 = load_s16 (ramPnt, progPnt->Pnt_StatusWord1);  // Запоминаем в глобальной переменной текущий статус 1, который а данном месте является статусом мастера
//				GlobalM3.CurrentDriveFreq = (u16)_IQmpyI32int(GlobalMstSlv.Master_Fref_IQ, FreqBase*10);
				defBitMicro(ramPnt, progPnt->Pnt_Slave_Fail   , 0); // Нет аварии
				GlobalMstSlv.ConnectErrors = 0; // Обнулить, если не режим мастера
				clrBitMicro(ramPnt, progPnt->Pnt_Err_Modbus); // Убрать аварию обмена если не мастер
			break;
			case REGIM_MASTER:
				//из SVP_UNIT
				GlobalMstSlv.Master_IQ_FREQ = GlobalM3.Master_IQ_FREQ;//задание скорости на слейв ( ОС скорости)
				GlobalMstSlv.Master_IQ_Torque = GlobalC28.Tzad; //GlobalM3.Master_IQ_Torque;
				GlobalMstSlv.Uart0_Mode= ModbusMaster;
				GlobalMstSlv.Master_Fref_IQ = load_s32 (ramPnt, progPnt->Pnt_Fref_to_Slave);  // Запоминаем в глобальной переменной текущую частоту для ведомого
				GlobalMstSlv.MasterStatusWord1 = load_s16 (ramPnt, progPnt->Pnt_StatusWord1);  // Запоминаем в глобальной переменной текущий статус 1, который а данном месте является статусом мастера
//				GlobalM3.CurrentDriveFreq = (u16)_IQmpyI32int(GlobalMstSlv.Master_Fref_IQ, FreqBase*10);
				if   ((GlobalMstSlv.SlaveStatusWord1 & 0x2000) == 0x2000){ // выделяем бит аварии
					defBitMicro(ramPnt, progPnt->Pnt_Slave_Fail , 1); // Есть авария
				} else{
					defBitMicro(ramPnt, progPnt->Pnt_Slave_Fail , 0);
				}
				// Проверка аварийных состояний от ведомого
				if (GlobalMstSlv.ConnectErrors){
					setBitMicro(ramPnt, progPnt->Pnt_Err_Modbus);
				}
				else{
					clrBitMicro(ramPnt, progPnt->Pnt_Err_Modbus);
				}
			break;
		}

		if (cntStart) cntStart--;
		if (cntStop) cntStop--;

		if (!cntStart){
			defBitMicro(ramPnt, progPnt->Pnt_ResMst3, 0); // Выдаем команду старт = 0
		}
		if (!cntStop){
			defBitMicro(ramPnt, progPnt->Pnt_ResSlv3, 0); // Выдаем команду стоп = 0
		}
	}
	//	При работе в режиме  ведущий/ведомый   каналы должны быть в начальных установках и не должны меняться пользователем
	//	- канал TXD_1/RXD_1 - обмен с внешней системой АСУ (А1, В1);
	//	- канал TXD_2/RXD_2 - обмен между ПЧ для алгоритма ведущий/ведомый  (А2, В2);
	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncMASTER_SLAVE_1[14]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Fref_to_Slave
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Pnt_MasterPingTime
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Pnt_SlaveErrorCount
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  StatusWord1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Mode_uart0
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  Mode_uart2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  Pnt_SlvAnsTimeout
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  ResSlv2
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //9  Fref_from_Mstr
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  ResSlv3
    	BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11 Err_Slave
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //12 Err_Modbus
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //13  ResMst3
		BIT_PIN_TYPE | OUTPUT_PIN_MODE     //14  Slave_Fail
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("MASTER_SLAVE", "U", TblFuncMASTER_SLAVE_1, FuncMASTER_SLAVE_1, sizeof(TblFuncMASTER_SLAVE_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncMASTER_SLAVE_1,
#endif
//********************************************************
