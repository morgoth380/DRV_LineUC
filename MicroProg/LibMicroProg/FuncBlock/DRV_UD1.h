#ifdef    GET_FUNC_CODE

#include "Faza.h"

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_OFF_ZAR;                      //1  OFF_ZAR
    u16  Pnt_DelayOffZar;                  //2  DelayOffZar
    u16  Pnt_UbridgeOnNom;                 //3  UbridgeOnNom
    u16  Pnt_UdminNom;                     //4  UdminNom
    u16  Pnt_UbridgeOnLv;                  //5  UbridgeOnLv
    u16  Pnt_UdminLV;                      //6  UdminLV
    u16  Pnt_UbSel;                        //7  UbSel
    u16  Pnt_Udmax;                        //8  Udmax
    u16  Pnt_T_ZAR;                        //9  T_ZAR
    u16  Pnt_AbcAcbEn;                     //10  AbcAcbEn
    u16  Pnt_ObrFazEn;                     //11  ObrFazEn
    u16  Pnt_Fazirovka;                    //12  Fazirovka
    u16  Pnt_UinErr;                       //13  UinErr
    u16  Pnt_CHARGE_OK;                    //14  CHARGE_OK
    u16  Pnt_ERR_ZAR;                      //15  ERR_ZAR
    u16  Pnt_Ud_fast;                      //16  Ud_fast
    u16  Pnt_START;                        //17  START
    u16  Pnt_Zar;                          //18  Zar
    u16  Pnt_End;
}FuncDRV_UD_type;
//********************************************************
// Объявление переменных с глобальным временем жизни
typedef struct{
    u16     powerUpDelay;
	u16     chargeTimer;
	u16		PcpChargeTimer;
}DRVUD_State_type;

typedef enum {
	phaseItrlvABC=0,	// phase interleaving is- ACB
	phaseItrlvACB,
	phaseLoss,
}PHASESTATE_code_enum;

#define	PCP_RELE_TIMEOUT	10  // 10 тактов микропрограммы = 40 мс
//********************************************************
u16 *FuncDRV_UD_1(FuncDRV_UD_type *progPnt, u32 ramPnt)
{
	DRVUD_State_type		   *sPnt;
    s32							UbridgeOn, Udmin;
    u16							phaseStFail, phaseStCode;

 
#ifdef _PROJECT_FOR_CCS_
	phaseStFail	= PhaseState.Failure;
	GlobalM3.UinErr = phaseStFail;
	phaseStCode	= PhaseState.Fazirovka; // Phase state code
#else
	phaseStFail = 0;
	phaseStCode = 0;
	clrBitMicro(ramPnt, progPnt->Pnt_Zar);
	setBitMicro(ramPnt, progPnt->Pnt_CHARGE_OK);
	return &progPnt->Pnt_End;
#endif    //_PROJECT_FOR_CCS_
	
	sPnt=(DRVUD_State_type *)(ramPnt + progPnt->Pnt_State);

	if(flagIsFirstCall){
		sPnt->powerUpDelay	= 5;
		sPnt->chargeTimer 	= load_s16(ramPnt, progPnt->Pnt_T_ZAR) * K_TIME_CALL_MICRO_TAKT;
		sPnt->PcpChargeTimer = 0;
		clrBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR); 
		GlobalM3.Kvitirov=0;
	}

	if(sPnt->powerUpDelay)
	{												// По подаче питания временно выставляем UinErr
		sPnt->powerUpDelay--;
		setBitMicro(ramPnt, progPnt->Pnt_UinErr);
	}
    else{
		if(load_s16(ramPnt, progPnt->Pnt_ObrFazEn)){
			defBitMicro(ramPnt, progPnt->Pnt_UinErr,  phaseStFail);		// нов обрыв Partola
		}
		else if((load_s16(ramPnt, progPnt->Pnt_AbcAcbEn)) && (phaseStCode == phaseItrlvACB)){   // нов обрыв Partola
			setBitMicro(ramPnt, progPnt->Pnt_UinErr);
		}
		else{
			clrBitMicro(ramPnt, progPnt->Pnt_UinErr);
		}
		save_s16(ramPnt, progPnt->Pnt_Fazirovka, phaseStCode);
    }

//--	Контроль  напряжения Ud и управление зарядом
	save_s32(ramPnt, progPnt->Pnt_Ud_fast, GlobalC28.UdcFast);			// из глобальной структуры формируется выход UdcFast просто транзитом через этот блок

	if(testBitMicro(ramPnt, progPnt->Pnt_UbSel)){						// Перегружаем уставки в зависимости от UbSel (соотвует сигналу EVAC_ENABLE)
		UbridgeOn	= load_s32(ramPnt, progPnt->Pnt_UbridgeOnLv);
		Udmin		= load_s32(ramPnt, progPnt->Pnt_UdminLV);
	}
	else{
		UbridgeOn	= load_s32(ramPnt, progPnt->Pnt_UbridgeOnNom);
		Udmin		= load_s32(ramPnt, progPnt->Pnt_UdminNom);
	}
    GlobalM3.Udmax	= load_s32(ramPnt, progPnt->Pnt_Udmax);
    GlobalM3.Udmin	= Udmin;



	if(GlobalM3.FlagCommand.bit.BitGotovWorkSVP ==1)
	{

	    if(sPnt->chargeTimer>0)
	    { // Ветвь стартового заряда Ud
	        sPnt->chargeTimer--;
	       if(sPnt->chargeTimer == 5) setBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR); //
	       if((sPnt->chargeTimer == 0)&&((GlobalM3.Kvitirov & 0x10)!=0))
	           {clrBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR);
	           }
	    }
	    else
	    {
              if(GlobalM3.Kvitirov =0x11)
			  {GlobalM3.Kvitirov=0;
			  sPnt->chargeTimer 	= load_s16(ramPnt, progPnt->Pnt_T_ZAR) * K_TIME_CALL_MICRO_TAKT;
			  }
			  //if( GlobalM3.Kvitirov==0x20)
			  // {
				 //  setBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR); //
	          //       }
	    }

	}
	else
	{
//--	Контроль входного напряжения
	if (testBitMicro(ramPnt, progPnt->Pnt_OFF_ZAR)) {
		clrBitMicro(ramPnt, progPnt->Pnt_Zar);
		clrBitMicro(ramPnt, progPnt->Pnt_START);
 	}
	else{
		if(sPnt->chargeTimer){ // Ветвь стартового заряда Ud
			sPnt->chargeTimer--;
			if (GlobalC28.Udc >= UbridgeOn){ // Сравниваем напряжение звена из глобальной структуры с уставкой
				if(GlobalC28.Udc <= load_s32(ramPnt, progPnt->Pnt_Udmax)){
					setBitMicro(ramPnt, progPnt->Pnt_START);
					sPnt->chargeTimer=0;
				}
				else{
					GlobalM3.FlagCommand.bit.ChargeOk = 0;
					clrBitMicro(ramPnt, progPnt->Pnt_CHARGE_OK);
				}
			}
			else{
				setBitMicro(ramPnt, progPnt->Pnt_Zar);
				 if(sPnt->chargeTimer == 1) setBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR); // Выставляем аварию за один тик до окончания отсчета
			}
		}
		else{ // Ветвь нормальной работы после заряда Ud

			// PCP - Sosnovchik - Измененный алгоритм работы зарядных резисторов
			if (GlobalM3.FlagCommand.bit.PCPmodeEnable == 1 && GlobalM3.FlagCommand.bit.PwmOn) { // Если разрешен РСР - В скаляре его надо отключить !!!!!
				if (GlobalM3.UinErr) // Если есть авария сети и включен ШИМ
				{   // Проверяем порог Ud, для включения запитки Ud через зарядные резисторы
					if ( 1/*GlobalC28.Udc < (_IQmpy(UbridgeOn, _IQ(0.9)))*/ ){ // Если Ud упало ниже чем 0,9*Udвкл.моста, ставим бит перехода на зарядные резисторы,
						// и отключаем зарядное реле
						GlobalM3.FlagCommand.bit.ZarrResistor = 1; // Резисторы включены в цепь заряда.
						//setBitMicro(ramPnt, progPnt->Pnt_Zar); //
						clrBitMicro(ramPnt, progPnt->Pnt_START);
						sPnt->PcpChargeTimer = PCP_RELE_TIMEOUT; // Заводим таймер на 40 мс - время срабатывания реле шунтирования зарядных резисторов
					}

				} else { // Если сеть восстановилась, проверяем когда можно отключить зарядные резисторы.
					if (GlobalC28.Udc >= UbridgeOn){
						// Выключаем опцию зарядных резисторов
						//clrBitMicro(ramPnt, progPnt->Pnt_Zar);
						setBitMicro(ramPnt, progPnt->Pnt_START);

						if (!sPnt->PcpChargeTimer){   // Если таймер блокировки отстучал
							GlobalM3.FlagCommand.bit.ZarrResistor = 0; // Сброс флага - Резисторы Отключены из цепи заряда. Здесь нужна пауза на срабтывания мех.реле.
						} else {
							sPnt->PcpChargeTimer--;
						}

					}
				}
			}
			else { // Если не влючен PCP и ШИМ не включен, пусть работает штатный обработчик. Но когда мы в движении - действия ниже не имеют на нас влияние.
				// Восстановление питания при работе в РСР
				if (GlobalM3.FlagCommand.bit.PCPmodeEnable == 1 && !GlobalM3.FlagCommand.bit.PwmOn) {
					if (GlobalC28.Udc >= UbridgeOn && GlobalC28.Udc <= load_s32(ramPnt, progPnt->Pnt_Udmax)){ // Сравниваем напряжение звена из глобальной структуры с уставкой
						setBitMicro(ramPnt, progPnt->Pnt_START);
						GlobalM3.FlagCommand.bit.ZarrResistor = 0;
						sPnt->PcpChargeTimer = 0;
					}
				}
				// Штатная обработка, которая повторно не проверяет отключение
				if((GlobalC28.Udc >= Udmin) && (GlobalC28.Udc <= load_s32(ramPnt, progPnt->Pnt_Udmax))){
					if(!testBitMicro(ramPnt, progPnt->Pnt_ERR_ZAR)){
						GlobalM3.FlagCommand.bit.ChargeOk = 1;
						clrBitMicro(ramPnt, progPnt->Pnt_Zar);
						setBitMicro(ramPnt, progPnt->Pnt_CHARGE_OK);
					}
				}
				else{
					GlobalM3.FlagCommand.bit.ChargeOk = 0;
					clrBitMicro(ramPnt, progPnt->Pnt_CHARGE_OK);

					//clrBitMicro(ramPnt, progPnt->Pnt_Zar);
					//clrBitMicro(ramPnt, progPnt->Pnt_START);
					//sPnt->chargeTimer	= (load_s16(ramPnt, progPnt->Pnt_T_ZAR) * K_TIME_CALL_MICRO_TAKT * 2);

				}
			}
		}
	  }
	}
	GlobalM3.TEMP1=sPnt->chargeTimer;
	GlobalM3.s16_Debug6 = (u16)GlobalM3.FlagCommand.bit.ZarrResistor;

	return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncDRV_UD_1[18]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  OFF_ZAR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  DelayOffZar
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  UbridgeOnNom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  UdminNom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  UbridgeOnLv
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  UdminLV
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  UbSel
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  Udmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  T_ZAR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  AbcAcbEn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  ObrFazEn
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Fazirovka
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //13  UinErr
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14  CHARGE_OK
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //15  ERR_ZAR
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //16  Ud_fast
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //17  START
        BIT_PIN_TYPE | OUTPUT_PIN_MODE    //18  Zar
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("DRV_UD", "DA", TblFuncDRV_UD_1, FuncDRV_UD_1, sizeof(TblFuncDRV_UD_1), sizeof(DRVUD_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncDRV_UD_1,
#endif
//********************************************************
