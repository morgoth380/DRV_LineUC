#ifdef    GET_FUNC_CODE
#include "GlobalVar.h"
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_EncoUse;                      //1  EncoUse
    u16  Pnt_SerialType;                   //2  SerialType
    u16  Pnt_ResolType;                    //3  ResolType
    u16  Pnt_PulseResol;                   //4  PulseResol
    u16  Pnt_BitResol;                     //5  BitResol
    u16  Pnt_PhasingSign;                  //6  PhasingSign
    u16  Pnt_AngleShift;                   //7  AngleShift
    u16  Pnt_PhaseFlt;                     //8  PhaseFlt
    u16  Pnt_SpdFlt;                       //9  SpdFlt
    u16  Pnt_FastSpdUse;                   //10  FastSpdUse
    u16  Pnt_FastSpdSign;                  //11  FastSpdSign
    u16  Pnt_CalcPeriod;                   //12  CalcPeriod
    u16  Pnt_EmulPulses;                   //13  EmulPulses
    u16  Pnt_ADC_Ampl;                     //14  ADC_Ampl
    u16  Pnt_Res1;                         //15  Res1
    u16  Pnt_Res2;                         //16  Res2
    u16  Pnt_Res3;                         //17  Res3
    u16  Pnt_Res4;                         //18  Res4
    u16  Pnt_Param1;                       //19  Param1
    u16  Pnt_Param2;                       //20  Param2
    u16  Pnt_Param3;                       //21  Param3
    u16  Pnt_Param4;                       //22  Param4
    u16  Pnt_Param5;                       //23  Param5
    u16  Pnt_Param6;                       //24  Param6
    u16  Pnt_Param7;                       //25  Param7
    u16  Pnt_Param8;                       //26  Param8
    u16  Pnt_Param9;                       //27  Param9
    u16  Pnt_Param10;                      //28  Param10
    u16  Pnt_Param11;                      //29  Param11
    u16  Pnt_Param12;                      //30  Param12
    u16  Pnt_Param13;                      //31  Param13
    u16  Pnt_Param14;                      //32  Param14
    u16  Pnt_Param15;                      //33  Param15
    u16  Pnt_Param16;                      //34  Param16
    u16  Pnt_Param17;                      //35  Param17
    u16  Pnt_Param18;                      //36  Param18
    u16  Pnt_Param19;                      //37  Param19
    u16  Pnt_Param20;                      //38  Param20
    u16  Pnt_Param21;                      //39  Param21
    u16  Pnt_Param22;                      //40  Param22
    u16  Pnt_Param23;                      //41  Param23
    u16  Pnt_Param24;                      //42  Param24
    u16  Pnt_Rs2NetAddr3;                  //43  Rs2NetAddr3
    u16  Pnt_Rs2Bdr3;                      //44  Rs2Bdr3
    u16  Pnt_Rs2NetAddr4;                  //45  Rs2NetAddr4
    u16  Pnt_Rs2Bdr4;                      //46  Rs2Bdr4
    u16  Pnt_Rs2MbErr3;                    //47  Rs2MbErr3
    u16  Pnt_Rs2MbErr4;                    //48  Rs2MbErr4
    u16  Pnt_Ext1Type;                     //49  Ext1Type
    u16  Pnt_Ext1VerPO;                    //50  Ext1VerPO
    u16  Pnt_Ext1SubVerPO;                 //51  Ext1SubVerPO    
    u16  Pnt_Ext1CRC;                      //52  Ext1CRC
    u16  Pnt_Ext2Type;                     //53  Ext2Type
    u16  Pnt_Ext2VerPO;                    //54  Ext2VerPO
    u16  Pnt_Ext2SubVerPO;                 //55  Ext2SubVerPO    
    u16  Pnt_Ext2CRC;                      //56  Ext2CRC
    u16  Pnt_Ext3Type;                     //57  Ext3Type
    u16  Pnt_Ext3VerPO;                    //58  Ext3VerPO
    u16  Pnt_Ext3SubVerPO;                 //59  Ext3SubVerPO    
    u16  Pnt_Ext3CRC;                      //60  Ext3CRC    
    u16  Pnt_DinExtVisible;                //61  DinExtVisible    
    u16  Pnt_AIO2ExtVisible;               //62  AIO2ExtVisible
	u16  Pnt_AIO2ExtVisErr;                //63  AIO2ExtVisErr
    u16  Pnt_RS2ExtVisible;                //64  RS2ExtVisible
	u16  Pnt_RS2ExtVisErr;                 //65  RS2ExtVisErr
    u16  Pnt_IncEncoExtVis;                //66  IncEncoExtVis
    u16  Pnt_EdtEncoExtVis;                //67  EdtEncoExtVis
    u16  Pnt_ScEncoExtVis;                 //68  ScEncoExtVis
    u16  Pnt_EthernetExtVis;               //69  EthernetExtVis
    u16  Pnt_ProfinetExtVis;               //70  ProfinetExtVis
    u16  Pnt_ProfibusExtVis;               //71  ProfibusExtVis    
    u16  Pnt_AnyEncoExtVis;                //72  AnyEncoExtVis
    u16  Pnt_EndtScExtVis;                 //73  EndtScExtVis    
    u16  Pnt_EncoSpeed;                    //74  EncoSpeed
    u16  Pnt_EncoTheta;                    //75  EncoTheta    
    u16  Pnt_End;
}FuncEXT_BLOCKS_PARAMS_type;
//********************************************************
u16 getEncoderFazaFilter (u16 inVal);
u16 *FuncEXT_BLOCKS_PARAMS_1(FuncEXT_BLOCKS_PARAMS_type *progPnt, u32 ramPnt)
{
  u16 fltBufSizeIndex;
    
  // Вывод версий ПО блоков расширения и типов блоков расширения.
  save_s16(ramPnt, progPnt->Pnt_Ext1Type, GlobalM4.extConnectedBlocks.ext1.extType);
  save_s16(ramPnt, progPnt->Pnt_Ext1VerPO, GlobalM4.extConnectedBlocks.ext1.softVersion);
  save_s16(ramPnt, progPnt->Pnt_Ext1SubVerPO, GlobalM4.extConnectedBlocks.ext1.softSubVersion);
  save_s16(ramPnt, progPnt->Pnt_Ext1CRC, GlobalM4.extConnectedBlocks.ext1.crc);
  
  save_s16(ramPnt, progPnt->Pnt_Ext2Type, GlobalM4.extConnectedBlocks.ext2.extType);
  save_s16(ramPnt, progPnt->Pnt_Ext2VerPO, GlobalM4.extConnectedBlocks.ext2.softVersion);
  save_s16(ramPnt, progPnt->Pnt_Ext2SubVerPO, GlobalM4.extConnectedBlocks.ext2.softSubVersion);
  save_s16(ramPnt, progPnt->Pnt_Ext2CRC, GlobalM4.extConnectedBlocks.ext2.crc);

  save_s16(ramPnt, progPnt->Pnt_Ext3Type, GlobalM4.extConnectedBlocks.ext3.extType);
  save_s16(ramPnt, progPnt->Pnt_Ext3VerPO, GlobalM4.extConnectedBlocks.ext3.softVersion);
  save_s16(ramPnt, progPnt->Pnt_Ext3SubVerPO, GlobalM4.extConnectedBlocks.ext3.softSubVersion);
  save_s16(ramPnt, progPnt->Pnt_Ext3CRC, GlobalM4.extConnectedBlocks.ext3.crc);
    
  // Проверяем - надо ли показывать дополнительные параметры
  // Сброс видимости блоков расширения
  save_s16(ramPnt, progPnt->Pnt_DinExtVisible, 0); // Скрыть параметры блока расширения
  save_s16(ramPnt, progPnt->Pnt_AIO2ExtVisible, 0); // Показать параметры Ext блока
  save_s16(ramPnt, progPnt->Pnt_AIO2ExtVisErr, 0); // Показать параметры Ext блока
  save_s16(ramPnt, progPnt->Pnt_RS2ExtVisible, 0); // Показать параметры Ext блока
  save_s16(ramPnt, progPnt->Pnt_RS2ExtVisErr, 0); // Показать параметры Ext блока
  save_s16(ramPnt, progPnt->Pnt_IncEncoExtVis, 0); // Показать параметры Ext блока
  save_s16(ramPnt, progPnt->Pnt_EdtEncoExtVis, 0); // Показать параметры Ext блока  
  save_s16(ramPnt, progPnt->Pnt_ScEncoExtVis, 0); // Показать параметры Ext блока  
  save_s16(ramPnt, progPnt->Pnt_EthernetExtVis, 0); // Показать параметры Ext блока  
  save_s16(ramPnt, progPnt->Pnt_ProfinetExtVis, 0); // Показать параметры Ext блока  
  save_s16(ramPnt, progPnt->Pnt_ProfibusExtVis, 0); // Показать параметры Ext блока    
    
  //  Проверяем видимость блоков
  if (GlobalM4.extConnectedBlocks.ext1.extType == DIN_DOUT_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == DIN_DOUT_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == DIN_DOUT_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_DinExtVisible, 1); // Показать параметры дискретного блока
  }  
  if (GlobalM4.extConnectedBlocks.ext1.extType == AIN_AOUT_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == AIN_AOUT_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == AIN_AOUT_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_AIO2ExtVisible, 1); // Показать параметры Ext блока
    GlobalM4.AinAoutExtData.AinAoutBlockPresent = 1; // Ставим флаг, что блок есть
  } else {
    GlobalM4.AinAoutExtData.AinAoutBlockPresent = 0; // Если блока нету, сбросить флаг наличия
  }
  if (GlobalM4.extConnectedBlocks.ext1.extType == RS485_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == RS485_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == RS485_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_RS2ExtVisible, 1); // Показать параметры Ext блока
  }  
  if (GlobalM4.extConnectedBlocks.ext1.extType == INCREMENT_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == INCREMENT_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == INCREMENT_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_IncEncoExtVis, 1); // Показать параметры Ext блока
    save_s16(ramPnt, progPnt->Pnt_AnyEncoExtVis, 1); // Показать параметры Ext блока    
  }
  if (GlobalM4.extConnectedBlocks.ext1.extType == ENDAT_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == ENDAT_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == ENDAT_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_EdtEncoExtVis, 1); // Показать параметры Ext блока
    save_s16(ramPnt, progPnt->Pnt_AnyEncoExtVis, 1); // Показать параметры Ext блока
    save_s16(ramPnt, progPnt->Pnt_EndtScExtVis, 1);  // Показать параметры Ext блока Endat+SC
  }
  if (GlobalM4.extConnectedBlocks.ext1.extType == SIN_COS_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == SIN_COS_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == SIN_COS_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_ScEncoExtVis, 1); // Показать параметры Ext блока
    save_s16(ramPnt, progPnt->Pnt_AnyEncoExtVis, 1); // Показать параметры Ext блока 
    save_s16(ramPnt, progPnt->Pnt_EndtScExtVis, 1);  // Показать параметры Ext блока Endat+SC    
  }

  if (GlobalM4.extConnectedBlocks.ext1.extType == ETHER_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == ETHER_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == ETHER_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_EthernetExtVis, 1); // Показать параметры Ext блока
  }  
  if (GlobalM4.extConnectedBlocks.ext1.extType == PROFINET_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == PROFINET_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == PROFINET_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_ProfinetExtVis, 1); // Показать параметры Ext блока
  }  
  if (GlobalM4.extConnectedBlocks.ext1.extType == PROFIBUS_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext2.extType == PROFIBUS_EXT_BLOCK || 
      GlobalM4.extConnectedBlocks.ext3.extType == PROFIBUS_EXT_BLOCK) {
    save_s16(ramPnt, progPnt->Pnt_ProfibusExtVis, 1); // Показать параметры Ext блока
  }  
     
  // Если есть хоть один блок энкодера - ставим в структуре энкодера флаг надичия блока, и тогда будем брать из него данные
  if (load_s16(ramPnt, progPnt->Pnt_IncEncoExtVis) || load_s16(ramPnt, progPnt->Pnt_EdtEncoExtVis) || load_s16(ramPnt, progPnt->Pnt_ScEncoExtVis)) {  
    GlobalM4.EncoderParam.EncoBlockPresent = 1;
    save_s16(ramPnt, progPnt->Pnt_EncoTheta, (u16)(GlobalM4.EncoderCalcParam.electricTheta * 3600.0F + 0.5F)); //Позиция энкодера
    save_float(ramPnt, progPnt->Pnt_EncoSpeed, GlobalM4.EncoderCalcParam.electricSpd);                         // Частота энкодера    
  } else {
    GlobalM4.EncoderParam.EncoBlockPresent = 0;      
    save_s16(ramPnt, progPnt->Pnt_EncoTheta, 0);   //Позиция энкодера
    save_float(ramPnt, progPnt->Pnt_EncoSpeed, 0); // Частота энкодера    
  }
    
  // Настройка параметров энкодера
  // Проверяем какой блок энкодера подключен.
  GlobalM4.EncoderParam.serialMode = (serialModesType)load_s16(ramPnt, progPnt->Pnt_SerialType);
  
  if (load_s16(ramPnt, progPnt->Pnt_IncEncoExtVis) || load_s16(ramPnt, progPnt->Pnt_ScEncoExtVis)) { // Это инкрементальный или син/кос энкодер?
    // Ставим режим в имп/оборот
    save_s16(ramPnt, progPnt->Pnt_ResolType, (u16)PULSES_PER_TURN); 
  } else if (load_s16(ramPnt, progPnt->Pnt_EdtEncoExtVis)) { // Это Endat энкодер?
    // Ставим режим в битах
    save_s16(ramPnt, progPnt->Pnt_ResolType, (u16)BITS_PER_TURN); 
    // Задаем кол-во бит в зависимости от типа энкодера
    switch (GlobalM4.EncoderParam.serialMode){       
       case ECN1325: 
         save_s16(ramPnt, progPnt->Pnt_BitResol, 25);
       break;
       case ECN1313:
       default:
         save_s16(ramPnt, progPnt->Pnt_BitResol, 13);
       break;       
    }
  }
    
  GlobalM4.EncoderParam.encoResolMode = (encoResolModeType)load_s16(ramPnt, progPnt->Pnt_ResolType); 
  GlobalM4.EncoderParam.encoPulseResol = load_s16(ramPnt, progPnt->Pnt_PulseResol);
  GlobalM4.EncoderParam.encoBitResol = load_s16(ramPnt, progPnt->Pnt_BitResol);
  GlobalM4.EncoderParam.speedPhasing = (spdPhasingType)load_s16(ramPnt, progPnt->Pnt_PhasingSign);
  GlobalM4.EncoderParam.encoAngleShift = load_s16(ramPnt, progPnt->Pnt_AngleShift);
  fltBufSizeIndex = load_s16(ramPnt, progPnt->Pnt_PhaseFlt);
  GlobalM4.EncoderParam.EncoFlt_N = getEncoderFazaFilter(fltBufSizeIndex);
  GlobalM4.EncoderParam.spdFltTime = load_s16(ramPnt, progPnt->Pnt_SpdFlt);
  GlobalM4.EncoderParam.fastSpdUse = load_s16(ramPnt, progPnt->Pnt_FastSpdUse);
  GlobalM4.EncoderParam.fastSpeedPhasing = (fastSpdPhasingType)load_s16(ramPnt, progPnt->Pnt_FastSpdSign);
  GlobalM4.EncoderParam.processingPeriod = load_s16(ramPnt, progPnt->Pnt_CalcPeriod);
  GlobalM4.EncoderParam.encoEmulResol = load_s16(ramPnt, progPnt->Pnt_EmulPulses);  
  GlobalM4.EncoderParam.ADC_Amplitude = load_s16(ramPnt, progPnt->Pnt_ADC_Ampl);
  // Конец блока анализа входных данных энкодера

  //*******************************************************
  // Дополнительный блок обработки и формирования видимости
  // для блока RS2 и AIO2
  //*******************************************************
  // Для блока RS2
  // Если блок есть в наличии, либо если есть хоть одна авария по связи, то я показываю параметры настройки аварии по RS485
  if (load_s16(ramPnt, progPnt->Pnt_RS2ExtVisible) || testBitMicro(ramPnt, progPnt->Pnt_Rs2MbErr3) || testBitMicro(ramPnt, progPnt->Pnt_Rs2MbErr4)) {  
	save_s16(ramPnt, progPnt->Pnt_RS2ExtVisErr, 1); // Показать параметры для аварийных настроек Ext_RS2 блока
  }

  // Для блока AIO2 надо доделать !!!
  //save_s16(ramPnt, progPnt->Pnt_AIO2ExtVisErr, 0); // Показать параметры Ext блока
  
  return &progPnt->Pnt_End;
}

u16 getEncoderFazaFilter (u16 inVal)
{ 
  f32 Tmax;
  f32 Tpwm;
  u16 FilterSize;
  float Fmax;

  FilterSize = 0;
    
  Fmax = (GlobalM4.Fmax != 0) ? IQ_to_FLOAT(GlobalM4.Fmax, Herz_base) : 50.0f;
  Tmax = 1.0F / Fmax; 
  Tpwm = 1;
  if (GlobalM4.Fpwm) {
	Tpwm = 1.0F / GlobalM4.Fpwm; 
  }
  
  if (inVal != 0) { 
    FilterSize = 1 << inVal;
  } else { 
    FilterSize = (u16)(Tmax / (4*Tpwm));
    
    if (FilterSize >= 64) 		FilterSize = 64;
    else if (FilterSize >= 32) 	FilterSize = 32;
    else if (FilterSize >= 16) 	FilterSize = 16;
    else if (FilterSize >= 8) 	FilterSize = 8;
    else if (FilterSize >= 4) 	FilterSize = 4;
    else FilterSize = 2;
  }
  
  return FilterSize;
}
;
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncEXT_BLOCKS_PARAMS_1[75]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  EncoUse
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  SerialType
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  ResolType
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  PulseResol
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  BitResol
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  PhasingSign
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  AngleShift
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8  PhaseFlt
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  SpdFlt
        S16_PIN_TYPE | INPUT_PIN_MODE,     //10  FastSpdUse
        S16_PIN_TYPE | INPUT_PIN_MODE,     //11  FastSpdSign
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  CalcPeriod
        S16_PIN_TYPE | INPUT_PIN_MODE,     //13  EmulPulses
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  ADC_Ampl
        S16_PIN_TYPE | INPUT_PIN_MODE,     //15  Res1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  Res2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //17  Res3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  Res4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //19  Param1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  Param2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //21  Param3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //22  Param4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //23  Param5
        S16_PIN_TYPE | INPUT_PIN_MODE,     //24  Param6
        S16_PIN_TYPE | INPUT_PIN_MODE,     //25  Param7
        S16_PIN_TYPE | INPUT_PIN_MODE,     //26  Param8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //27  Param9
        S16_PIN_TYPE | INPUT_PIN_MODE,     //28  Param10
        S16_PIN_TYPE | INPUT_PIN_MODE,     //29  Param11
        S16_PIN_TYPE | INPUT_PIN_MODE,     //30  Param12
        S16_PIN_TYPE | INPUT_PIN_MODE,     //31  Param13
        S16_PIN_TYPE | INPUT_PIN_MODE,     //32  Param14
        S16_PIN_TYPE | INPUT_PIN_MODE,     //33  Param15
        S16_PIN_TYPE | INPUT_PIN_MODE,     //34  Param16
        S16_PIN_TYPE | INPUT_PIN_MODE,     //35  Param17
        S16_PIN_TYPE | INPUT_PIN_MODE,     //36  Param18
        S16_PIN_TYPE | INPUT_PIN_MODE,     //37  Param19
        S16_PIN_TYPE | INPUT_PIN_MODE,     //38  Param20
        S16_PIN_TYPE | INPUT_PIN_MODE,     //39  Param21
        S16_PIN_TYPE | INPUT_PIN_MODE,     //40  Param22
        S16_PIN_TYPE | INPUT_PIN_MODE,     //41  Param23
        S16_PIN_TYPE | INPUT_PIN_MODE,     //42  Param24
        S16_PIN_TYPE | INPUT_PIN_MODE,     //43  Rs2NetAddr3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //44  Rs2Bdr3
        S16_PIN_TYPE | INPUT_PIN_MODE,     //45  Rs2NetAddr4
        S16_PIN_TYPE | INPUT_PIN_MODE,     //46  Rs2Bdr4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //47  Rs2MbErr3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //48  Rs2MbErr4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //49  Ext1Type
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //50  Ext1VerPO
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //51  Ext1SubVerPO
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //52  Ext1CRC                
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //53  Ext2Type
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //54  Ext2VerPO
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //55  Ext2SubVerPO
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //56  Ext2CRC        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //57  Ext3Type
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //58  Ext3VerPO
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //59  Ext3SubVerPO
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //60  Ext3CRC                
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //61  DinExtVisible
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //62  AIO2ExtVisible        
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //63  AIO2ExtVisErr        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //64  RS2ExtVisible        
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //65  RS2ExtVisErr        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //66  IncEncoExtVis        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //67  EdtEncoExtVis        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //68  ScEncoExtVis        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //69  EthernetExtVis        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //70  ProfinetExtVis        
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //71  ProfibusExtVis                
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //72  AnyEncoExtVis                
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //73  EndtScExtVis                
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //74  EncoSpeed                
        S16_PIN_TYPE | OUTPUT_PIN_MODE,    //75  EncoTheta                
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("EXT_BLOCKS_PARAMS", "DA", TblFuncEXT_BLOCKS_PARAMS_1, FuncEXT_BLOCKS_PARAMS_1, sizeof(TblFuncEXT_BLOCKS_PARAMS_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncEXT_BLOCKS_PARAMS_1,
#endif
//********************************************************
