#include "DrvSpi.h"
#include "GPIODef.h"
#include "Common_Param_C28.h"

#ifdef _PROJECT_FOR_CCS_
#include "packet.h"
#endif // _PROJECT_FOR_CCS_

#ifdef    GET_FUNC_CODE

#define AIN1_EXT_BIT_mA 3
#define AIN2_EXT_BIT_mA 4
#define AIN3_EXT_BIT_mA 6

#define AIN1_EXT_BIT_V 2
#define AIN2_EXT_BIT_V 5
#define AIN3_EXT_BIT_V 7

#define INVERTED_ZERO 0xFF

#define MIN_4_mA 700

#define CALIBR_AOUT_NICHT 0
#define CALIBR_AOUT_MIN   1
#define CALIBR_AOUT_MAX   2
#define DO_CALIBR         3

#define SET_BIT(num, bit) {num |= (1 << bit);}

const nvAoutCalState_type  nvAoutCalState_default = {
0       ,        // 0 NotWr_calibrAin      РќРµ Р’РїРёСЃС‹РІР°С‚СЊ
0       ,        // 1 WrZer_5mA_calibrAin  РљР°Р»РёР±СЂРљРѕРґ 0 РґР»СЏ РґР°С‚С‡РёРєР° 0 - 5 РјРђ
0       ,        // 2 WrZer_20mA_calibrAin РљР°Р»РёР±СЂРљРѕРґ РЅСѓР»СЏ РґР»СЏ РґР°С‚С‡РёРєР° 4 - 20 РјРђ
0       ,        // 3 WrZer_10V_calibrAin  РљР°Р»РёР±СЂРљРѕРґ РЅСѓР»СЏ РґР»СЏ РґР°С‚С‡РёРєР° 0 - 10 Р’
1024    ,        // 5 Wr5_calibrAin        РљР°Р»РёР±СЂРљРѕРґ 5 РјРђ
4095    ,        // 6 Wr10_calibrAin       РљР°Р»РёР±СЂРљРѕРґ 10 Р’
4095    ,        // 7 Wr20_calibrAin       РљР°Р»РёР±СЂРљРѕРґ 20 mA
};

typedef struct{
        u16         Ain1;
        u16         Ain2;

        s16         Tcur;
        s16         Tmin;
        s16         Tmax;
        s16         Tsys_max;
        s16         CoolerState;

        f32         Kmin;
        f32         Kmax;
        f32         Cooler_PWM_MIN;
        f32         Cooler_PWM_MAX;
        f32         step;
        u16			DinCnt[8];
        u16			DinCntStop;
        u16         DinDataState;
        s16         CalibrStatePrev;
        s16         CalibrCodeAout[kolv_calibrAout];
}PERIF_NVSAState_type;

typedef struct{
	u16  Pnt_Nfunc;
	u16  Pnt_State;
	//u16  Pnt_nvState;

	u16  Pnt_Zar;                          //1  Zar
	u16  Pnt_Start;                        //2  Start
	u16  Pnt_Fan;                          //3  Fan

	u16  Pnt_Rele1;                        //4  Rele1
	u16  Pnt_Rele2;                        //5  Rele2
	u16  Pnt_Rele3;                        //6  Rele3
	u16  Pnt_Rele4;                        //7  Rele4
	u16  Pnt_Rele1Ext;                     //8  Rele1Ext
	u16  Pnt_Rele2Ext;                     //9  Rele2Ext
	u16  Pnt_Rele3Ext;                     //10  Rele3Ext
	u16  Pnt_Rele4Ext;                     //11  Rele4Ext

	u16  Pnt_Inv_Dout[8];					   //12-19 Inv_Dout1-8

	u16  Pnt_LEDFail;                      //20  LEDFail
	u16  Pnt_LEDReady;                     //21  LEDReady
	u16  Pnt_LEDRun;                       //22  LEDRun

	u16  Pnt_Aout1;                        //23  Aout1
	u16  Pnt_TypeAout1;                    //24  TypeAout1
	u16  Pnt_TestAout;                     //25  TestAout
	u16  Pnt_CalibrAout;                   //26  CalibrAout
	u16  Pnt_TypeAin1;                     //27  TypeAin1
	u16  Pnt_TypeAin2;                     //28  TypeAin2
	u16  Pnt_T_cooler;                     //29  T_cooler
	u16  Pnt_CoolerType;                   //30  CoolerType
	u16  Pnt_DinMode;                      //31  DinMode

	u16  Pnt_Inv_Din[12];                  //32  Inv_Din1-12

	u16  Pnt_Din[12];                      //44  Din12-1

	u16  Pnt_Din1_12;                      //56  Din1-12

	u16  Pnt_Stop;                         //57  Stop
	u16  Pnt_T_diskr;                      //58  T_diskr

	u16  Pnt_Ain5;                         //59  Ain5
	u16  Pnt_Ain4;                         //60  Ain4
	u16  Pnt_Ain3;                         //61  Ain3
	u16  Pnt_Ain2;                         //62  Ain2
	u16  Pnt_Ain1;                         //63  Ain1

	u16  Pnt_PTC;                          //64  PTC

	u16  Pnt_Ain3Ext;                      //65  Ain3Ext
	u16  Pnt_Ain2Ext;                      //66  Ain2Ext
	u16  Pnt_Ain1Ext;                      //67  Ain1Ext

	u16  Pnt_TypeAin3Ext;                  //68  TypeAin3Ext
	u16  Pnt_TypeAin2Ext;                  //69  TypeAin2Ext
	u16  Pnt_TypeAin1Ext;                  //70  TypeAin1Ext

	u16  Pnt_TypeExt123;                   //71  TypeExt123

	u16  Pnt_End;
}FuncPERIF_NVSA_1_type;
//********************************************************

#ifdef  _PROJECT_FOR_CCS_

//===================================================================
// С„СѓРЅРєС†РёСЏ РІРѕР·РІСЂР°С‰Р°РµС‚ СЃРѕСЃС‚РѕСЏРЅРёРµ Р·Р°РїСЂР°С€РёРІР°РµРјРѕРіРѕ РґРёСЃРєСЂРµС‚РЅРѕРіРѕ РІС…РѕРґР°
//===================================================================
u16 DinTest(u16 PinNumber)
{
    u16 res;
    switch (PinNumber)
    {
        case DIN1:
            res = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_4);
            break; //DIN1
        case DIN2:
            res = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7);
            break; //DIN2
        case DIN3:
            res = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_6);
            break; //DIN3
        case DIN4:
            res = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_6);
            break; //DIN4
        case DIN5:
            res = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_2);
            break; //DIN5
        case DIN6:
            res = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_5);
            break; //DIN6
        case DIN7:
            res = GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_2);
            break; //DIN7
        case DIN8:
            res = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_5);
            break; //DIN8
        case PTC :
            res = GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_4);
            break; //PTC
        case STOP:
            res = GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_3);
            break; //STOP
        case T_DISKR:
            res = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_7);
            break; //T_DISKR
        default:
            res = 0xFFFF; break;
    }
    return res;
}

/****************************************************************************
* Function Name  : ExtAinsGetTypes
* Description    : Reads EXT1 analog input types (mA or V) from funcblock pins
* Input          : progPnt, ramPnt - taken from FuncPERIF_NVSA_1 function
* Output         : None
* Return         : 16 bits: 0-8 represents AIN 1-3 types (needed in *595 reg on EXT1);
                   9-15 - zeros (does not matter)
****************************************************************************/
u16 ExtAinsGetTypes(FuncPERIF_NVSA_1_type *progPnt, u32 ramPnt)
{
    u16 AinType = 0, AinTypes = 0;

    // Get AIN1 type from its pin
    AinType = load_s16(ramPnt, progPnt->Pnt_TypeAin1Ext);

    if(AinType == mA0_5_TypeAin ||
       AinType == mA4_20_TypeAin)
    {
        SET_BIT(AinTypes, AIN1_EXT_BIT_mA);
    }
    else
    {
        SET_BIT(AinTypes, AIN1_EXT_BIT_V);
    }

    // Get AIN2 type from its pin
    AinType = load_s16(ramPnt, progPnt->Pnt_TypeAin2Ext);

    if(AinType == mA0_5_TypeAin ||
       AinType == mA4_20_TypeAin)
    {
        SET_BIT(AinTypes, AIN2_EXT_BIT_mA);
    }
    else
    {
        SET_BIT(AinTypes, AIN2_EXT_BIT_V);
    }

    // Get AIN3 type from its pin
    AinType = load_s16(ramPnt, progPnt->Pnt_TypeAin3Ext);

    if(AinType == mA0_5_TypeAin ||
       AinType == mA4_20_TypeAin)
    {
        SET_BIT(AinTypes, AIN3_EXT_BIT_mA);
    }
    else
    {
        SET_BIT(AinTypes, AIN3_EXT_BIT_V);
    }

    return AinTypes;
}

/****************************************************************************
* Function Name  : ExtAinsRead
* Description    : Reads current EXT1 AINs values from C28's ADC
* Input          : progPnt, ramPnt - taken from FuncPERIF_NVSA_1 function
* Output         : None
* Return         : None
****************************************************************************/
void ExtAinsRead(FuncPERIF_NVSA_1_type *progPnt, u32 ramPnt)
{
    // Get & save EXT1 analog inputs values from the ADC
    save_s16(ramPnt, progPnt->Pnt_Ain1Ext, GlobalC28.AdcValues.Ain1);
    save_s16(ramPnt, progPnt->Pnt_Ain2Ext, GlobalC28.AdcValues.Ain2);
    save_s16(ramPnt, progPnt->Pnt_Ain3Ext, GlobalC28.AdcValues.Ain3);
}

/****************************************************************************
* Function Name  : ExtDinsRead
* Description    : Reads & saves EXT1 discrete inputs values
* Input          : progPnt, ramPnt - taken from FuncPERIF_NVSA_1 function
* Output         : None
* Return         : None
****************************************************************************/
void ExtDinsRead(FuncPERIF_NVSA_1_type *progPnt, u32 ramPnt)
{
    u16 DinsData = 0;

    // Read DINs states via SPI
    DinsData = (~(DoutIn())) & 15;

    // Save states to pin
    //save_s16(ramPnt, progPnt->Pnt_Din0_3Ext, DinsData);
}

#endif  //_PROJECT_FOR_CCS_

u16 *FuncPERIF_NVSA_1_1(FuncPERIF_NVSA_1_type *progPnt, u32 ramPnt)
{
#ifdef  _PROJECT_FOR_CCS_
    u16             AinsTypes = 0,  calibrAoutCommand;
    u16             i, ReleData, DinData, typeAout, typeAin1, typeAin2,  tmp16;//Ain1, Ain2,
    u16             *pmU16;
    s16             numExtBlock;
    s16             inversiomMask;
    
#endif  // _PROJECT_FOR_CCS_

    s16             Tcool;
    s32             tmp;
    void            *nvAdr;
    PERIF_NVSAState_type    *sPnt;
    COMMONPARAMM3  *mPnt3 = &GlobalM3;

    sPnt = (PERIF_NVSAState_type *)(ramPnt + progPnt->Pnt_State);


    if(flagIsFirstCall){
    
#ifdef  _PROJECT_FOR_CCS_

        Start_dir_output(); // START как выход
        Zarr_dir_output();  // FAN как GPIO на выход
        Fan_dir_output();

        Stop_dir_input();

        T_diskr_input();

        PTC_dir_input();

        Start_clr();
        Zarr_clr();

        //Определить номер блока расширения
		numExtBlock = (DoutIn() & 0xF0) >> 4;	//Старшая тетрада - номер блока
		save_s16(ramPnt, progPnt->Pnt_TypeExt123, numExtBlock);

#endif  // _PROJECT_FOR_CCS_

        mPnt3->Cooler_PWM = 0.0f;
        sPnt->CoolerState = 0;
        sPnt->Kmin = 0.37f;
        sPnt->Kmax = 0.52f;
        sPnt->Cooler_PWM_MIN = 0.6f;
        sPnt->Cooler_PWM_MAX = 1.0f;
        sPnt->Tsys_max = mPnt3->Tmax_deg;
        sPnt->Tmin = (s16)(sPnt->Kmin * (f32)sPnt->Tsys_max);
        sPnt->Tmax = (s16)(sPnt->Kmax * (f32)sPnt->Tsys_max);

        tmp = (s32)(sPnt->Tmax - sPnt->Tmin);
        if(tmp == 0)
        {
            sPnt->step = 0.0f;
        }
        else
        {
            sPnt->step = (sPnt->Cooler_PWM_MAX - sPnt->Cooler_PWM_MIN) / (f32)tmp;
        }
//        Fan_set();
        sPnt->DinDataState = INVERTED_ZERO;

        nvAdr = (void *)((u32)&nv.AOutput.CalibrCode);
		RdFramMicro(sPnt->CalibrCodeAout, nvAdr, sizeof(nvAoutCalState_type));

		if(sPnt->CalibrCodeAout[Wr10_calibrAout] == 0 ){
			memcpy( sPnt->CalibrCodeAout, &nvAoutCalState_default, sizeof(nvAoutCalState_type));
			WrFramMicro(nvAdr, sPnt->CalibrCodeAout, sizeof(nvAoutCalState_type));
		}
    }

#ifdef  _PROJECT_FOR_CCS_
/*    ////////////////////////////////////
    //Аналоговые выходы
    ////////////////////////////////////
    }*/

    ////////////////////////////////////
    //ZAR, START, FAN
    ////////////////////////////////////
    if(testBitMicro(ramPnt, progPnt->Pnt_Zar))      
        Zarr_set();
    else                                            
        Zarr_clr();

    if(testBitMicro(ramPnt, progPnt->Pnt_Start))    
        Start_set();
    else                                            
        Start_clr();

    ////////////////////////////////////
    //              RELE EXT
    ////////////////////////////////////
    ReleData = 0;
    pmU16 = &progPnt->Pnt_Rele1Ext;
    for (i = 0;i < 4;i++, pmU16++){
      if(testBitMicro(ramPnt, (u32)*pmU16))         ReleData |= (1 << i);
    }
    ////////////////////////////////////
    //              RELE
    ////////////////////////////////////
    pmU16 = &progPnt->Pnt_Rele1;
    for (i = 0;i < 4;i++, pmU16++){
      if(testBitMicro(ramPnt, (u32)*pmU16))         ReleData |= (1 << (4+i));

    }

	/**************************************
	* INVERSION DOUT
	*/
	inversiomMask = 0;
	pmU16 = progPnt->Pnt_Inv_Dout;
	for(i = 0; i < 8; i++){
		if(load_s16(ramPnt, *pmU16) != 0){
			inversiomMask |= 1 << i;
		}
		pmU16++;
	}
	ReleData ^= inversiomMask;

    ////////////////////////////////////
    //              LEDs
    ////////////////////////////////////
    pmU16 = &progPnt->Pnt_LEDFail;
    for (i = 0; i < 3; i++, pmU16++){
      if(testBitMicro(ramPnt, (u32)*pmU16))         ReleData |= (1 << (9+i));
    }
    
    // Get & save EXT1 analog input types
    AinsTypes = ExtAinsGetTypes(progPnt, ramPnt); // Get types of the EXT1 AINs
    DoutOut(ReleData | (AinsTypes << 16));        // Save data to 3 *595 shift registers

    // Read & save discrete input values
    ExtDinsRead(progPnt, ramPnt);

    /**************************************
	* Get & save EXT1 analog input types
	*/
	AinsTypes = ExtAinsGetTypes(progPnt, ramPnt); // Get types of the EXT1 AINs
	DoutOut(ReleData | (AinsTypes << 16));        // Save data to 3 *595 shift registers

	/**************************************
	* GET DIN NVSA
	*/
	for (i = DIN1; i <= DIN8; i++){
		if (DinTest(i)){
			if (sPnt->DinCnt[i] < 3)		sPnt->DinCnt[i]++; //3 - три опроса
			else							sPnt->DinDataState |=1<<i;
		}
		else{
			if (sPnt->DinCnt[i] > 0)		sPnt->DinCnt[i]--;
			else	    	                sPnt->DinDataState &= ~(1<<i);
		}
	}
	DinData = (~(sPnt->DinDataState))&0xFF;

	/**************************************
	* GET DIN EXT tates via SPI
	*/
	DinData |= ((~DoutIn()) & 0x0F) << 8;	//Младшая тетрада - состояние цифровых входов EXT

	/**************************************
	* INVERSION DIN
	*/
	inversiomMask = 0;
	pmU16 = progPnt->Pnt_Inv_Din;
	for(i = 0; i < 12; i++){
		if(load_s16(ramPnt, *pmU16) != 0){
			inversiomMask |= 1 << i;
		}
		pmU16++;
	}
	DinData ^= inversiomMask;

	/**************************************
	* SAVE DIN STATE NVSA+EXT WITH INVERSION
	*/
	save_s16( ramPnt, progPnt->Pnt_Din1_12, DinData );
    
    // Get analog input values from EXT1
	ExtAinsRead(progPnt, ramPnt);

    ////////////////////////////////////
    //              DIN
    ////////////////////////////////////
	/*
    DinData = 0;
    for (i = DIN1; i <= DIN8; i++){
        if (DinTest(i)){
            DinData |=1<<i;
        }
    }
    DinData = (~DinData)&0xFF;
    save_s16( ramPnt, progPnt->Pnt_Din0_7, DinData );
   */
	//sPnt->DinDataState = INVERTED_ZERO;
	for (i = DIN1; i <= DIN8; i++){
	    if (DinTest(i)){
	    	if (sPnt->DinCnt[i] < 3)		sPnt->DinCnt[i]++; //3 - три опроса
	    	else							sPnt->DinDataState |=1<<i;
	    }
	    else{
	    	if (sPnt->DinCnt[i] > 0)		sPnt->DinCnt[i]--;
	    	else	    	                sPnt->DinDataState &= ~(1<<i);
	    }
	}
	DinData = (~(sPnt->DinDataState))&0xFF;
	save_s16( ramPnt, progPnt->Pnt_Din1_12, DinData );
    ////////////////////////////////////
    //              PTC
    ////////////////////////////////////
    if (DinTest(PTC))								//Инверснул датчик PTC (Moroz)
    	setBitMicro(ramPnt, progPnt->Pnt_PTC);
    else
        clrBitMicro(ramPnt, progPnt->Pnt_PTC);

    ////////////////////////////////////
    //              STOP
    ////////////////////////////////////
    if (DinTest(STOP)){
    	if (sPnt->DinCntStop < 3)		sPnt->DinCntStop++; //3 - три опроса
    	else							setBitMicro(ramPnt, progPnt->Pnt_Stop);
    }
    else{
    	if (sPnt->DinCntStop > 0)		sPnt->DinCntStop--;
    	else	    	                clrBitMicro(ramPnt, progPnt->Pnt_Stop);
    }
    ////////////////////////////////////
    //              T_DISKR
    ////////////////////////////////////
    if (DinTest(T_DISKR))
        setBitMicro(ramPnt, progPnt->Pnt_T_diskr);
    else
        clrBitMicro(ramPnt, progPnt->Pnt_T_diskr);
    ////////////////////////////////////
    //              AIN/AOUT
    ////////////////////////////////////
// так как структуры скопированы из проэкта stm32
// RX_PACKET по отношению к данному проэкту передаваемый
// Чтение данных c МКАР
    t_RX_PACKET *pRxPacket = (t_RX_PACKET *)&UartBuf4[0];
    if (!UART_rx_FIFO(&uart4, &UartBuf4[0])){
        t_TX_PACKET *pTxPacket = (t_TX_PACKET *)uart4.bufStrt;
        if (!(crc8(&UartBuf4[0], (5-1)) - pTxPacket->crc8)){
            sPnt->Ain1 = pTxPacket->ADC_1;
            sPnt->Ain2 = pTxPacket->ADC_2;
        }else{
            //testRprt[Ntst_obmen_s_MKAR] = TEST_falure;
        }
    }
// Подготовка пакета и отправка в МКАР

// Подготовка значения для аналогового выхода
    // Установка типа аналогового входа 1
    typeAin1 = load_s16(ramPnt, progPnt->Pnt_TypeAin1);
    typeAin2 = load_s16(ramPnt, progPnt->Pnt_TypeAin2);

    switch (typeAin1){
    case mA0_5_TypeAin:
        pRxPacket->O_10V_IN1 = 0;
        pRxPacket->O_20mA_IN1 = 1;
//      Ain1 = (s16)Fy_x1x2y1y2x(0, 1024, 0, 4095, Ain1);
        break;
    case mA4_20_TypeAin:
        pRxPacket->O_10V_IN1 = 0;
        pRxPacket->O_20mA_IN1 = 1;
//      Ain1 = (s16)Fy_x1x2y1y2x(819, 4096, 0, 4095, Ain1);
        break;
    case V0_10_TypeAin:
        pRxPacket->O_10V_IN1 = 1;
        pRxPacket->O_20mA_IN1 = 0;
        break;
    default:
        break;
    }
    save_s16(ramPnt, progPnt->Pnt_Ain1, sPnt->Ain1);
// Установка типа аналогового входа 2
    switch (typeAin2){
    case mA0_5_TypeAin:
        pRxPacket->O_10V_IN2 = 0;
        pRxPacket->O_20mA_IN2 = 1;
//          Ain2 = (s16)Fy_x1x2y1y2x(0, 1024, 0, 4095, Ain2);
        break;
    case mA4_20_TypeAin:
        pRxPacket->O_10V_IN2 = 0;
        pRxPacket->O_20mA_IN2 = 1;
//          Ain2 = (s16)Fy_x1x2y1y2x(819, 4096, 0, 4095, Ain2);
        break;
    case V0_10_TypeAin:
        pRxPacket->O_10V_IN2 = 1;
        pRxPacket->O_20mA_IN2 = 0;
        break;
    default:
        break;
    }
    save_s16(ramPnt, progPnt->Pnt_Ain2, sPnt->Ain2);
// Установка типа аналогового выхода
    tmp = load_s32(ramPnt, progPnt->Pnt_Aout1);
    tmp16 = (u16)(tmp >> 12);
	if(tmp16 > 4095){
		tmp16 = 4095;
	}
    typeAout = load_s16(ramPnt, progPnt->Pnt_TypeAout1);
    calibrAoutCommand = load_s16(ramPnt, progPnt->Pnt_TestAout);
    switch (calibrAoutCommand){
    case CALIBR_AOUT_NICHT:
    	break;
    case CALIBR_AOUT_MIN:
    	tmp16 = 0;
    	break;
    case CALIBR_AOUT_MAX:
    	tmp16 = 4095;
        break;
    default:
    	break;
    }
    switch (typeAout){

    case mA0_5_TypeAout:
        pRxPacket->SEL_U = pRxPacket->SEL_U1 = 0;
        pRxPacket->SEL_I = 1;

		if((calibrAoutCommand == CALIBR_AOUT_MIN) & (sPnt->CalibrStatePrev != CALIBR_AOUT_MIN)){
			sPnt->CalibrStatePrev = CALIBR_AOUT_MIN;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, (((s32)sPnt->CalibrCodeAout[WrZer_5mA_calibrAout]) << 12));
		}
		if((calibrAoutCommand == CALIBR_AOUT_MAX) & (sPnt->CalibrStatePrev != CALIBR_AOUT_MAX)){
			sPnt->CalibrStatePrev = CALIBR_AOUT_MAX;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, (((s32)sPnt->CalibrCodeAout[Wr5_calibrAout] + 1) << 14));
		}
		if((calibrAoutCommand == CALIBR_AOUT_NICHT) & (sPnt->CalibrStatePrev != CALIBR_AOUT_NICHT)){
			sPnt->CalibrStatePrev = CALIBR_AOUT_NICHT;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, 0);
        }

        if ((calibrAoutCommand == DO_CALIBR) & (sPnt->CalibrStatePrev != DO_CALIBR)){
    		if(sPnt->CalibrStatePrev == CALIBR_AOUT_MAX){
    		    sPnt->CalibrCodeAout[Wr5_calibrAout] = ((u16)(load_s32(ramPnt, progPnt->Pnt_CalibrAout) >> 14)) - 1;
    		    nvAdr = (void *)((u32)&nv.AOutput.CalibrCode[Wr5_calibrAout]);
				WrFramMicro(nvAdr, &sPnt->CalibrCodeAout[Wr5_calibrAout], sizeof(s16));
				save_s16(ramPnt, progPnt->Pnt_TestAout, CALIBR_AOUT_MAX);
    		}
    		if(sPnt->CalibrStatePrev == CALIBR_AOUT_MIN){
				sPnt->CalibrCodeAout[WrZer_5mA_calibrAout] = (u16)(load_s32(ramPnt, progPnt->Pnt_CalibrAout) >> 12);
				nvAdr = (void *)((u32)&nv.AOutput.CalibrCode[WrZer_5mA_calibrAout]);
				WrFramMicro(nvAdr, &sPnt->CalibrCodeAout[WrZer_5mA_calibrAout], sizeof(s16));
				save_s16(ramPnt, progPnt->Pnt_TestAout, CALIBR_AOUT_MIN);
			}
    		sPnt->CalibrStatePrev = DO_CALIBR;
        }

        tmp16 = (u16)Fy_x1x2y1y2x(0, 4095, sPnt->CalibrCodeAout[WrZer_5mA_calibrAout], sPnt->CalibrCodeAout[Wr5_calibrAout], (s32)tmp16);
        pRxPacket->DAC1_U = 3600;
        pRxPacket->DAC2_I = tmp16;
        break;
    case mA4_20_TypeAout:
        pRxPacket->SEL_U = pRxPacket->SEL_U1 = 0;
        pRxPacket->SEL_I = 1;

		if((calibrAoutCommand == CALIBR_AOUT_MIN) & (sPnt->CalibrStatePrev != CALIBR_AOUT_MIN)){
			sPnt->CalibrStatePrev = CALIBR_AOUT_MIN;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, (((s32)sPnt->CalibrCodeAout[WrZer_20mA_calibrAout]) << 12));
		}
		if((calibrAoutCommand == CALIBR_AOUT_MAX) & (sPnt->CalibrStatePrev != CALIBR_AOUT_MAX)){
			sPnt->CalibrStatePrev = CALIBR_AOUT_MAX;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, (((s32)sPnt->CalibrCodeAout[Wr20_calibrAout] + 1) << 12));
		}
		if(calibrAoutCommand == CALIBR_AOUT_NICHT){
			sPnt->CalibrStatePrev = CALIBR_AOUT_NICHT;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, 0);
        }

		if ((calibrAoutCommand == DO_CALIBR) & (sPnt->CalibrStatePrev != DO_CALIBR)){
			if(sPnt->CalibrStatePrev == CALIBR_AOUT_MAX){
				sPnt->CalibrCodeAout[Wr20_calibrAout] = ((u16)(load_s32(ramPnt, progPnt->Pnt_CalibrAout) >> 12)) - 1;
				nvAdr = (void *)((u32)&nv.AOutput.CalibrCode[Wr20_calibrAout]);
				WrFramMicro(nvAdr, &sPnt->CalibrCodeAout[Wr20_calibrAout], sizeof(s16));
				save_s16(ramPnt, progPnt->Pnt_TestAout, CALIBR_AOUT_MAX);
			}
			if(sPnt->CalibrStatePrev == CALIBR_AOUT_MIN){
				sPnt->CalibrCodeAout[WrZer_20mA_calibrAout] = ((u16)(load_s32(ramPnt, progPnt->Pnt_CalibrAout) >> 12));
				nvAdr = (void *)((u32)&nv.AOutput.CalibrCode[WrZer_20mA_calibrAout]);
				WrFramMicro(nvAdr, &sPnt->CalibrCodeAout[WrZer_20mA_calibrAout], sizeof(s16));
				save_s16(ramPnt, progPnt->Pnt_TestAout, CALIBR_AOUT_MIN);
			}
    		sPnt->CalibrStatePrev = DO_CALIBR;
        }

        tmp16 = (u16)Fy_x1x2y1y2x(0, 4095, MIN_4_mA + sPnt->CalibrCodeAout[WrZer_20mA_calibrAout], sPnt->CalibrCodeAout[Wr20_calibrAout], (s32)tmp16);
        pRxPacket->DAC1_U = 3600;
        pRxPacket->DAC2_I = tmp16;
        break;
    case V0_10_TypeAout:
        pRxPacket->SEL_U = pRxPacket->SEL_U1 = 1;
        pRxPacket->SEL_I = 0;

		if((calibrAoutCommand == CALIBR_AOUT_MIN) & (sPnt->CalibrStatePrev != CALIBR_AOUT_MIN)){
			sPnt->CalibrStatePrev = CALIBR_AOUT_MIN;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, (((s32)sPnt->CalibrCodeAout[WrZer_10V_calibrAout]) << 12));
		}
		if((calibrAoutCommand == CALIBR_AOUT_MAX) & (sPnt->CalibrStatePrev != CALIBR_AOUT_MAX)){
			sPnt->CalibrStatePrev = CALIBR_AOUT_MAX;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, (((s32)sPnt->CalibrCodeAout[Wr10_calibrAout] + 1) << 12));
		}
		if((calibrAoutCommand == CALIBR_AOUT_NICHT) & (sPnt->CalibrStatePrev != CALIBR_AOUT_NICHT)){
			sPnt->CalibrStatePrev = CALIBR_AOUT_NICHT;
			save_s32(ramPnt, progPnt->Pnt_CalibrAout, 0);
        }

		if ((calibrAoutCommand == DO_CALIBR) & (sPnt->CalibrStatePrev != DO_CALIBR)){
			if(sPnt->CalibrStatePrev == CALIBR_AOUT_MAX){
				sPnt->CalibrCodeAout[Wr10_calibrAout] = ((u16)(load_s32(ramPnt, progPnt->Pnt_CalibrAout) >> 12)) - 1;
				nvAdr = (void *)((u32)&nv.AOutput.CalibrCode[Wr10_calibrAout]);
				WrFramMicro(nvAdr, &sPnt->CalibrCodeAout[Wr10_calibrAout], sizeof(s16));
				save_s16(ramPnt, progPnt->Pnt_TestAout, CALIBR_AOUT_MAX);
			}
			if(sPnt->CalibrStatePrev == CALIBR_AOUT_MIN){
				sPnt->CalibrCodeAout[WrZer_10V_calibrAout] = ((u16)(load_s32(ramPnt, progPnt->Pnt_CalibrAout) >> 12));
				nvAdr = (void *)((u32)&nv.AOutput.CalibrCode[WrZer_10V_calibrAout]);
				WrFramMicro(nvAdr, &sPnt->CalibrCodeAout[WrZer_10V_calibrAout], sizeof(s16));
				save_s16(ramPnt, progPnt->Pnt_TestAout, CALIBR_AOUT_MIN);
			}
			if(sPnt->CalibrStatePrev == CALIBR_AOUT_NICHT){
				save_s16(ramPnt, progPnt->Pnt_TestAout, CALIBR_AOUT_NICHT);
			}
    		sPnt->CalibrStatePrev = DO_CALIBR;
        }

        tmp16 = (u16)Fy_x1x2y1y2x(0, 4095, sPnt->CalibrCodeAout[WrZer_10V_calibrAout], sPnt->CalibrCodeAout[Wr10_calibrAout], (s32)tmp16);
        pRxPacket->DAC1_U = tmp16;
        pRxPacket->DAC2_I = 3600;
        break;
    default:
        break;
    }

    pRxPacket->reserve1 = 0;
    pRxPacket->crc8 = crc8(&UartBuf4[0], (5-1));

    UART_tx_FIFO(&uart4, &UartBuf4[0], 5);
    if (UartBuf4[3]!=83){
        tmp16++;
        tmp16--;
     }
//////////////////////////////////////////////////////
#endif  //  _PROJECT_FOR_CCS_

#define Cooler_turnOff_differ 10 // гистерезис в 1.0 градуса Цельсия

    sPnt->Tcur = load_s16(ramPnt, progPnt->Pnt_Fan);
    if(load_s16(ramPnt, progPnt->Pnt_CoolerType) == 1){
        if(sPnt->Tsys_max != mPnt3->Tmax_deg){
            sPnt->Tsys_max = mPnt3->Tmax_deg;
            sPnt->Tmin = (s16)(sPnt->Kmin * (f32)sPnt->Tsys_max);
            sPnt->Tmax = (s16)(sPnt->Kmax * (f32)sPnt->Tsys_max);
            tmp = (s32)(sPnt->Tmax - sPnt->Tmin);
            if(tmp == 0){
                sPnt->step = 0.0f;
            }else{
                sPnt->step = (sPnt->Cooler_PWM_MAX - sPnt->Cooler_PWM_MIN) / (f32)tmp;
            }
        }

        if(testBitMicro(ramPnt, progPnt->Pnt_LEDRun)){
            if(sPnt->Tcur > sPnt->Tmin){
                sPnt->CoolerState = 1;
                if(sPnt->Tcur >= sPnt->Tmax){
                    mPnt3->Cooler_PWM = sPnt->Cooler_PWM_MAX;
                }else{
                    mPnt3->Cooler_PWM = (f32)(sPnt->Tcur - sPnt->Tmin) * sPnt->step + sPnt->Cooler_PWM_MIN;
                }
            }else{
                if(sPnt->CoolerState){
                    mPnt3->Cooler_PWM = sPnt->Cooler_PWM_MIN;
                }
            }
        }else{
            if(sPnt->CoolerState){
                if(sPnt->Tcur < (sPnt->Tmin - Cooler_turnOff_differ)){
                    sPnt->CoolerState = 0;
                    mPnt3->Cooler_PWM = 0.0f;
                }
            }else{
                if(sPnt->Tcur >= sPnt->Tmin){
                    sPnt->CoolerState = 1;
                    if(sPnt->Tcur >= sPnt->Tmax){
                        mPnt3->Cooler_PWM = sPnt->Cooler_PWM_MAX;
                    }else{
                        mPnt3->Cooler_PWM = (f32)(sPnt->Tcur - sPnt->Tmin) * sPnt->step + sPnt->Cooler_PWM_MIN;
                    }
                }
            }
        }
    }else{
        Tcool = load_s16(ramPnt, progPnt->Pnt_T_cooler);
        if(testBitMicro(ramPnt, progPnt->Pnt_LEDRun)){
            if(sPnt->Tcur >= Tcool){
                mPnt3->Cooler_PWM = 1.0f;
            }
        }else{
            if(sPnt->Tcur >= Tcool){
                mPnt3->Cooler_PWM = 1.0f;
            }else{
                if(sPnt->Tcur <= (Tcool - Cooler_turnOff_differ)){
                    mPnt3->Cooler_PWM = 0.0f;
                }
            }
        }
    }

    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPERIF_NVSA_1_1[71]={
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Zar
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Start
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Fan

		BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Rele1
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Rele2
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Rele3
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Rele4
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Rele1Ext
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  Rele2Ext
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  Rele3Ext
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //11  Rele4Ext

        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  Inv_R1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  Inv_R2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  Inv_R3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Inv_R4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Inv_R5Ext
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Inv_R6Ext
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Inv_R7Ext
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Inv_R8Ext

		BIT_PIN_TYPE | INPUT_PIN_MODE,     //20  LEDFail
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //21  LEDReady
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //22  LEDRun

		S32_PIN_TYPE | INPUT_PIN_MODE,     //23  Aout1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  TypeAout1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  TestAout
		S32_PIN_TYPE | INPUT_PIN_MODE,     //26  CalibrAout
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  TypeAin1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  TypeAin2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  T_cooler
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  CoolerType
		BIT_PIN_TYPE | INPUT_PIN_MODE,     //31  DinMode

        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  Inv_Din1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  Inv_Din2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  Inv_Din3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  Inv_Din4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  Inv_Din5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  Inv_Din6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38  Inv_Din7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //39  Inv_Din8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //40  Inv_Din9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41  Inv_Din10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  Inv_Din11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43  Inv_Din12

        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //44  Din12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //45  Din11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //46  Din10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //47  Din9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //48  Din8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //49  Din7
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //50  Din6
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //51  Din5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //52  Din4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //53  Din3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //54  Din2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //55  Din1

        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //56  Din1-12
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //57  Stop
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //58  T_diskr
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //59  Ain5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //60  Ain4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //61  Ain3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //62  Ain2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //63  Ain1
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //64  PTC

        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //65  Ain3Ext
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //66  Ain2Ext
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //67  Ain1Ext

        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //68  TypeAin3Ext
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //69  TypeAin2Ext
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //70  TypeAin1Ext

        S16_PIN_TYPE | OUTPUT_PIN_MODE     //71  TypeExt123
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PERIF_NVSA_1",
			  "DA",
			  TblFuncPERIF_NVSA_1_1,
			  FuncPERIF_NVSA_1_1,
			  sizeof(TblFuncPERIF_NVSA_1_1),
			  sizeof(PERIF_NVSAState_type),
			  0,
			  1
			  ),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPERIF_NVSA_1_1,
#endif
//********************************************************