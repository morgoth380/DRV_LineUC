//********************************************************
//Системные не исполняемые функции
//********************************************************
#include "FuncBlock/CONST.h"
#include "FuncBlock/TEK.h"
#include "FuncBlock/TEK_4BYTE.h"
#include "FuncBlock/TEK_FORMAT.h"
#include "FuncBlock/TEK_TEXT.h"
#include "FuncBlock/TEXT_VAL.h"
#include "FuncBlock/TYPE_BIT.h"
#include "FuncBlock/TYPE_FLOAT.h"
#include "FuncBlock/TYPE_S16.h"
#include "FuncBlock/TYPE_S32.h"
#include "FuncBlock/UST.h"
#include "FuncBlock/UST_4BYTE.h"
#include "FuncBlock/UST_FORMAT.h"
#include "FuncBlock/UST_FORMAT_LIM.h"
#include "FuncBlock/UST_LIM.h"
#include "FuncBlock/UST_TEXT.h"
#include "FuncBlock/Atribut.h" // Gahov
#include "FuncBlock/End.h"
#include "FuncBlock/MGRP.h"
// Уставки переменной видимости
#include "FuncBlock/TEK_VISIBLE.h"
#include "FuncBlock/TEK_4BYTE_VISIBLE.h"
#include "FuncBlock/TEK_FORMAT_VISIBLE.h"
#include "FuncBlock/TEK_TEXT_VISIBLE.h"
#include "FuncBlock/UST_VISIBLE.h"
#include "FuncBlock/UST_4BYTE_VISIBLE.h"
#include "FuncBlock/UST_FORMAT_VISIBLE.h"
#include "FuncBlock/UST_FORMAT_LIM_VISIBLE.h"
#include "FuncBlock/UST_LIM_VISIBLE.h"
#include "FuncBlock/UST_TEXT_VISIBLE.h"

//********************************************************
//Системные исполняемые функции
//********************************************************
#include "FuncBlock/DEBUG_SYM.h"
#include "FuncBlock/SS_PARAMS.h"

#include "FuncBlock/PORT_UART_3.h"
#include "FuncBlock/MB_0X05.h"
#include "FuncBlock/MB_0X05_VAL.h"
#include "FuncBlock/RTC.h"
#include "FuncBlock/BASE16.h"
#include "FuncBlock/BASE_CALC.h"
#include "FuncBlock/Paroles.h"
#include "FuncBlock/NAMEFUNC16.h"
#include "FuncBlock/LOGER.h"		// Логер Степанова
#include "FuncBlock/TIME_CONTROL.h"     // Измерение времени
#include "FuncBlock/TEMP_CONTROL.h"     // Контроль и управление вентиляторами
#include "FuncBlock/factory_set.h"      // Выбор набора уставок
#include "FuncBlock/PERIF_V2.h"         // ПЕРИФФЕРИЯ ПРИВОДА (ВХОДА И ВЫХОДА)
#include "FuncBlock/AIN_SCALE3_V2.h"    // Аналоговые входы

#include "FuncBlock/Current_control.h"    // Регулятор нагрузки

#include "FuncBlock/DISKR_FUNC_V4.h"    // назначение дискретных входов
#include "FuncBlock/DIN_2U64.h"         // соятояние дискретных входов 
#include "FuncBlock/CONTROL_V2.h"       // выбор режима работы
#include "FuncBlock/FAIL_COMMON_V2.h"   // Обработка аварий

//#include "FuncBlock/TEMP_S_PICKUP.h"    // Генератор по частоте
//#include "FuncBlock/uf_4_filtr_scale.h" // Формирование U/F
#include "FuncBlock/DRV_UD.h"           // работа в Ud              
#include "FuncBlock/KX_B_32_2.h"        // назначение и разчет значений для аналоговых выходов
#include "FuncBlock/KX_B.h"
#include "FuncBlock/FREQ_SCALE.h"       // задание частоту по аналоговых входам
#include "FuncBlock/LOCAL_APV_V3.h"     // обработка аварий с АПВ
#include "FuncBlock/apv1.h"             // Блок для расчета параметров АПВ 
#include "FuncBlock/MUX24.h"
#include "FuncBlock/MUX_4X4_4.h"

#include "FuncBlock/UF5_F2S.h"          // Формирование U/F 
#include "FuncBlock/TEMP_LSU_2.h"       // Генератор по частоте
#include "FuncBlock/JRN_V2.h"           // аварийный журнал

#include "FuncBlock/FAULT_WARNING.h"    // предупреждения об аварии и времени до останова
#include "FuncBlock/READY_STATUS.h"     // формирования статуса готовности с учетом "моргания" от АПВ

#include "FuncBlock/GET_CRC_PO.h"       // для параметров CRC и версии ПО
#include "FuncBlock/LED_RUN.h"          // Формирование состояние работота для светодиода
#include "FuncBlock/LED_CONTROL.h"      // Формирование состояние светодиодов
#include "FuncBlock/LED_WIFI.h"
#include "FuncBlock/POWER_LOSS_APV.h"   // АПВ после восстановления питания
#include "FuncBlock/FREQLIMITCHECK.h"   // Проверка блоков Защиты по частоте
#include "FuncBlock/EXT_BLOCKS_PARAMS.h"
#include "FuncBlock/DERATING_CONTROL.h" // Модуль дерейтинга

/*

#include "FuncBlock/LOGGER1.h"
#include "FuncBlock/MK_TIME.h"


#include "FuncBlock/MASTER_SET_LIFT.h"
#include "FuncBlock/MASTER_SET_PUMP.h"
#include "FuncBlock/MACROS_TYPE_CONTROL.h"
#include "FuncBlock/MACROS_TYPE_TASK.h"
#include "FuncBlock/MACROS_LIFT.h"
*/
//********************************************************
//Преобразования типов
//********************************************************
#include "FuncBlock/TYPE_S16_TO_BIT.h"
#include "FuncBlock/TYPE_BIT_TO_S16.h"
#include "FuncBlock/BIT4_TO_U16.h"
#include "FuncBlock/BIT8_TO_U16.h"
#include "FuncBlock/BIT16_TO_U16.h"
#include "FuncBlock/S16_S16_TO_S16.h"
#include "FuncBlock/S16_TO_S16_S16.h"
#include "FuncBlock/U16_TO_BIT4.h"
#include "FuncBlock/U16_TO_BIT8.h"
#include "FuncBlock/U16_TO_BIT16.h"

//********************************************************
//Прикладные функции
//********************************************************
#include "FuncBlock/SEL_REF_V2.h"				
#include "FuncBlock/DIN_UP_DOWN.h"
#include "FuncBlock/REF_CONTROL.h"
#include "FuncBlock/FREQ_FORBIDDEN_V2.h"
//#include "FuncBlock/RAMP_CTRL.h"

#include "FuncBlock/DRV_V5.h"
//#include "FuncBlock/PUSK_READY.h"
#include "FuncBlock/PUSK_READY_2.h"
//#include "FuncBlock/STOP.h"
#include "FuncBlock/STOP_2.h"
#include "FuncBlock/POWER_ON_PUSK.h"
#include "FuncBlock/START_STOP_MUX.h"
#include "FuncBlock/FUNCTIONAL_BLOCK.h"

#include "FuncBlock/MUX8_2.h"
//#include "FuncBlock/FLOW_COMP.h" 	// не используется chemeris			
//#include "FuncBlock/FB_FUNC.h"        // не используется chemeris
#include "FuncBlock/FB_COMBO.h"
#include "FuncBlock/AUTOADJ_PID.h"
#include "FuncBlock/PID_IQ.h"
#include "FuncBlock/PID_REF_CONVERT.h"

#include "FuncBlock/TPM.h"
#include "FuncBlock/UNDERLOAD.h"
#include "FuncBlock/CURRENT_IMBALANCE.h" 		
#include "FuncBlock/LOAD_LOSS.h"		

#include "FuncBlock/WARNINGS.h"
#include "FuncBlock/TIMER_3.h"


#include "FuncBlock/TORMOZ_V2.h" // Тормозной ключ
#include "FuncBlock/RW_GROUP_PRM.h"         // блочное чтение/запись параметров
#include "FuncBlock/BRAKE_CNTRL.h"      // Механический тормоз общепром
#include "FuncBlock/PICKUP_V2.h"

#include "FuncBlock/LIFT_CMD_V3.h"
#include "FuncBlock/RAMP_CTRL_MOD2.h"
#include "FuncBlock/EVAC_V3.h"
#include "FuncBlock/CONT_FAIL.h"
#include "FuncBlock/ENCO_PHASING_BLOCK.h"
#include "FuncBlock/NO_FLOW_DR_RUN2.h"

/*

#include "FuncBlock/apv1.h"
//--
#include "FuncBlock/TRANZIT.h"	//kozloff
#include "FuncBlock/DISBALANS_I.h"
#include "FuncBlock/CURR_CONTR.h"
#include "FuncBlock/RES_CAB.h"


#include "FuncBlock/AIN_SCALE2_V2.h"




#include "FuncBlock/PROFINET_PARAM3.h"


//#include "FuncBlock/DISKR_FUNC_V3_B.h" // Здесь есть выходной сигнал - Разрешение пуска, который поступает на

//#include "FuncBlock/DRV_V3.h"

#include "FuncBlock/ECONOM.h"

#include "FuncBlock/LIFT_CMD_V2.h"
#include "FuncBlock/MUX2X2.h"




#include "FuncBlock/PRESS_RELE.h"   //Рере разности давлений
//#include "FuncBlock/PUSKREADY_2.h" - Модифицированная с входом разрешения дискретного входа
//#include "FuncBlock/REVERS_CONTROL.h"


#include "FuncBlock/Delay_Freq.h"
#include "FuncBlock/MASTER_SLAVE.h"
#include "FuncBlock/PCP_BLOCK.h"

#include "FuncBlock/PROFIDRIVE.h"
#include "FuncBlock/ACTIVEPROTOCOL.h"
#include "FuncBlock/PZDI_PZDO.h"


#include "FuncBlock/JOBTIME.h"
#include "FuncBlock/PID_PWM.h" 					// Partola
#include "FuncBlock/PROTECT.h" 					// Partola
#include "FuncBlock/TPBR.h"						// GAHOV


#include "FuncBlock/DELAY.h"

//#include "FuncBlock/INDEPENDENCE_2.h"
//#include "FuncBlock/TEMP_TIME_ONE.h"

#include "FuncBlock/recalc_frec.h"
#include "FuncBlock/TIMER_1.h"
#include "FuncBlock/TIMER_2.h"

#include "FuncBlock/CNT.h"

#include "FuncBlock/DBG_ADD_SIN.h"


#include "FuncBlock/cmh.h"
#include "FuncBlock/MOTOH.h"

#include "FuncBlock/jrn.h"
//#include "FuncBlock/FAIL_MASK.h"
#include "FuncBlock/ref_scale.h"

#include "FuncBlock/FIRE_MODE.h"				//Ilia Moroz




//Pump func////////////////////////////////////////////////////////////////
#include "FuncBlock/NO_FLOW_DRY_RUN.h" 			//Ilia Moroz
#include "FuncBlock/WORK_POINT.h"
#include "FuncBlock/SHORT_CYCLE.h"		        //Alexey Zhelanov
#include "FuncBlock/BELT_CONTROL.h"				//Ilia Moroz
#include "FuncBlock/CLEAN.h"		            //Partola
#include "FuncBlock/PUMP_CMD.h"
#include "FuncBlock/AUTOVENTIL_V2.h"
/////////////////////////////////////////////////////////////////


#include "FuncBlock/FREQ_PWR_SIMULATOR.h"		//Ilia Moroz
#include "FuncBlock/I_CONTROL.h" 				// Partola

#include "FuncBlock/WARNINGS.h"
#include "FuncBlock/UNDERLOAD.h"
#include "FuncBlock/TREND_V2.h"
#include "FuncBlock/UST_RESET.h"				//kozloff
#include "FuncBlock/ELCOUNTER.h"



#include "FuncBlock/CALENDAR.h"
#include "FuncBlock/ustab.h" 					// kozloff
*/
//********************************************************
//Элементарные математические
//********************************************************
#include "FuncBlock/ADD.h"
#include "FuncBlock/SUB.h"
#include "FuncBlock/MULL.h"
#include "FuncBlock/NOT.h"
#include "FuncBlock/sign.h"
#include "FuncBlock/XOR_2.h"
//#include "FuncBlock/ABS.h"
//#include "FuncBlock/invertor.h"

#include "FuncBlock/AND_2.h"
#include "FuncBlock/AND_3.h"
#include "FuncBlock/AND_4.h"

#include "FuncBlock/OR_2.h"
#include "FuncBlock/OR_3.h"
#include "FuncBlock/OR_4.h"
#include "FuncBlock/OR_8.h"
#include "FuncBlock/or_12.h" // Partola

#include "FuncBlock/MUX2.h"
#include "FuncBlock/MUX4.h"
#include "FuncBlock/MUX8.h"
#include "FuncBlock/MUX16.h"
#include "FuncBlock/DC4.h"
#include "FuncBlock/DC8.h"
/*
#include "FuncBlock/A_B___N.h"
#include "FuncBlock/A_B__N.h"
#include "FuncBlock/A_PLUS__B__N_.h"
*/
#include "FuncBlock/MIN_MAX_2.h"
#include "FuncBlock/MIN_MAX_3.h"
#include "FuncBlock/MIN_MAX_4.h"
#include "FuncBlock/MIN_MAX_LIMITER.h"
#include "FuncBlock/CMP_3.h"
#include "FuncBlock/CMP_4.h"
#include "FuncBlock/CMP_5.h"
//#include "FuncBlock/CMP_SELECT.h"
#include "FuncBlock/CMP_EQ.h"

//#include "FuncBlock/RISE_FALL_OUT.h"
//#include "FuncBlock/RISE_FALL_SEL.h"
#include "FuncBlock/PULSE_RISE.h"
#include "FuncBlock/PULSE_RISE_PRM.h"
#include "FuncBlock/PULSE_FALL.h"

#include "FuncBlock/BOOTLOADER.h"

/*

#include "FuncBlock/KX_B_SCALE.h"


//#include "FuncBlock/D_TRIG.h"
#include "FuncBlock/RS_TRIGGER.h"
#include "FuncBlock/rs_trigger_or.h"
*/
//********************************************************
#include "FuncBlock/KOEFF_RS1.h"
#include "FuncBlock/SVP_UNIT.h"

#include "FuncBlock/TORQUE_MAX.h"

/*/
/==========07.02.2018   Суходольский


#include "FuncBlock/KOEFF_ASU.h"

#include "FuncBlock/SAVE_INTEGR.h"
*/