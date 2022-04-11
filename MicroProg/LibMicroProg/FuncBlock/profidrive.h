#ifdef    GET_FUNC_CODE

#ifdef   _PROJECT_FOR_CCS_
#include "VPC3+S.h"
#include "DrvSpiForVPC3.h"
#endif

#define		eventTrue 			1
#define 	eventFalse			0		

#define		NumOfTransition		66
#define		ON					1
#define		OFF					0

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Fault;                        //1  Fault
    u16  Pnt_Speed;                        //2  Speed
    u16  Pnt_Freq;                         //3  Freq
    u16  Pnt_Current;                      //4  Current
    u16  Pnt_ResIn1;                       //5  ResIn1
    u16  Pnt_ResIn2;                       //6  ResIn2
    u16  Pnt_ResIn3;                       //7  ResIn3
    u16  Pnt_ResIn4;                       //8  ResIn4
    u16  Pnt_ResIn5;                       //9  ResIn5
    u16  Pnt_ResIn6;                       //10  ResIn6
    u16  Pnt_ResIn7;                       //11  ResIn7
    u16  Pnt_ResIn8;                       //12  ResIn8
    u16  Pnt_ResIn9;                       //13  ResIn9
    u16  Pnt_ResIn10;                      //14  ResIn10
    u16  Pnt_ResIn11;                      //15  ResIn11
    u16  Pnt_ResIn12;                      //16  ResIn12
    u16  Pnt_OperInhib;                    //17  OperInhib
    u16  Pnt_Enable;                       //18  Enable
    u16  Pnt_Contactor;                    //19  Contactor
    u16  Pnt_Voltage;                      //20  Voltage
    u16  Pnt_ResOut1;                      //21  ResOut1
    u16  Pnt_ResOut2;                      //22  ResOut2
    u16  Pnt_ResOut3;                      //23  ResOut3
    u16  Pnt_ResOut4;                      //24  ResOut4
    u16  Pnt_ResOut5;                      //25  ResOut5
    u16  Pnt_ResOut6;                      //26  ResOut6
    u16  Pnt_ResOut7;                      //27  ResOut7
    u16  Pnt_ResOut8;                      //28  ResOut8
    u16  Pnt_ResOut9;                      //29  ResOut9
    u16  Pnt_ResOut10;                     //30  ResOut10
    u16  Pnt_ResOut11;                     //31  ResOut11
    u16  Pnt_ResOut12;                     //32  ResOut12
    u16  Pnt_ResOut13;                     //33  ResOut13
    u16  Pnt_ResOut14;                     //34  ResOut14
    u16  Pnt_ResOut15;                     //35  ResOut15
    u16  Pnt_ResOut16;                     //36  ResOut16
    u16  Pnt_ResOut17;                     //37  ResOut17
    u16  Pnt_ResOut18;                     //38  ResOut18
    u16  Pnt_ResOut19;                     //39  ResOut19
    u16  Pnt_ResOut20;                     //40  ResOut20
    u16  Pnt_End;
}FuncPROFIDRIVE_type;

u16 ProfiBAddr;
u16 numOfPPO;

char PowerM = 1, FailM, SpeedM, CurrentM, OperationInhibitM, LoadContactorOpenM;
#ifdef   _PROJECT_FOR_CCS_
extern ConnectionControl_type ConnectionControl;
#endif
typedef enum{
	SwitchedOff,
	SwitchOnInhibit,
	NotReadyForSwitchOn,
	ReadyForSwitchOn,
	Ready,
	EnebleOperation,
	RFGEnableOut,
	RFGAccEnabled,
	OperatingStatus,
	Off1activeStage1,
	Off1activeStage2,
	Off2active,
	Off3active,
	Fault,
	InhibitOperationActive
} States_type;

typedef union{
	struct{
		u16 FailTekDrvBit_0:1;
		u16 Gotov_1:1;
		u16 PWNOnDrv_2:1;
		u16 UstFZadanSign_3:1;
		u16 GTUp_4:1;
		u16 GTDown_5:1;
		u16 Charge_6:1;
		u16 StopByDC_7:1;
		u16 StopAll_8:1;
		u16 PickUpOfAll_9:1;
		u16 AutoTune_10:1;
		u16 FazirovkaBit_11:1;
		u16 StopKey_12:1;
		u16 ChanelRefAuto_13:1;
		u16 AutoMode_14:1;
		u16 Reserved_15:1;
	};
	u16 all;
}AT24_StatusWord2_type;

typedef struct{
	States_type currState;
	char (*Event)(FuncPROFIDRIVE_type *, u32);
	States_type nextState;
	void (*Func)(FuncPROFIDRIVE_type *, u32);
} Transition_type;

#ifdef   _PROJECT_FOR_CCS_
unsigned char eventPower(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventNoReadyForSwitch_ReadyForSwitch(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventReadyForSwitch_Ready(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventFromAnyState_SwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventFromAnyState_Off3active(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventFromAnyState_Off2active(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventFromAnyState_Fault(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventSwitchOnInhibit_NotReadyForSwitch(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventInhibitOperationActive_Ready(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventReady_EnableOperation(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventEnableOperation_RFGEnableOut(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventToInhibitOperationActive(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventToOff1activeStage1(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventRFGEnableOut_RFGAccEnabled(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventToEnableOperation(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventRFGAccEnabled_OperatingStatus(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventOff1activeStage1_Off1activeStage2(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventOff2active_NoReadyForSwitchOn(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventFault_SwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventOff3active_SwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventOff2active_SwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventToRFGEnableOut(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
unsigned char eventToRFGAccEnabled(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);

void funcSwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcNotReadyForSwitchOn(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcReadyForSwitchOn(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcReady(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcEnebleOperation(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcRFGEnableOut(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcRFGAccEnabled(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcOperatingStatus(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcOff1activeStage1(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcOff1activeStage2(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcOff2active(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcOff3active(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcFault(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);
void funcInhibitOperationActive(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);

void funcSelectRC(FuncPROFIDRIVE_type *progPnt, u32 ramPnt);

StatusWord_type 			StatusWord;
ControlWord_type			ControlWord;

AT24_StatusWord2_type		AT24_StatusWord2;

Transition_type TransitionTable[NumOfTransition] = {
{	SwitchedOff				,(char (*)(FuncPROFIDRIVE_type *, u32))eventPower,								NotReadyForSwitchOn		,(void (*)(FuncPROFIDRIVE_type *, u32))funcNotReadyForSwitchOn	},
{	NotReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventNoReadyForSwitch_ReadyForSwitch,	ReadyForSwitchOn		,(void (*)(FuncPROFIDRIVE_type *, u32))funcReadyForSwitchOn		},
//{	NotReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	NotReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active		 		,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	NotReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	NotReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	ReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventReadyForSwitch_Ready,				Ready					,(void (*)(FuncPROFIDRIVE_type *, u32))funcReady					},
//{	ReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	ReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	ReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	ReadyForSwitchOn		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	Ready					,(char (*)(FuncPROFIDRIVE_type *, u32))eventReady_EnableOperation,				EnebleOperation			,(void (*)(FuncPROFIDRIVE_type *, u32))funcEnebleOperation		},
//{	Ready					,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	Ready					,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	Ready					,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	Ready					,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	Ready					,(char (*)(FuncPROFIDRIVE_type *, u32))eventToOff1activeStage1,					Off1activeStage1		,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff1activeStage1		},
{	EnebleOperation			,(char (*)(FuncPROFIDRIVE_type *, u32))eventEnableOperation_RFGEnableOut,		RFGEnableOut			,(void (*)(FuncPROFIDRIVE_type *, u32))funcRFGEnableOut			},
{	EnebleOperation			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToOff1activeStage1,					Off1activeStage1		,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff1activeStage1		},
{	EnebleOperation			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToInhibitOperationActive,			InhibitOperationActive	,(void (*)(FuncPROFIDRIVE_type *, u32))funcInhibitOperationActive	},
//{	EnebleOperation			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	EnebleOperation			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	EnebleOperation			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	EnebleOperation			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	RFGEnableOut			,(char (*)(FuncPROFIDRIVE_type *, u32))eventRFGEnableOut_RFGAccEnabled,			RFGAccEnabled			,(void (*)(FuncPROFIDRIVE_type *, u32))funcRFGAccEnabled			},
{	RFGEnableOut			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToOff1activeStage1,					Off1activeStage1		,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff1activeStage1		},
{	RFGEnableOut			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToEnableOperation,					EnebleOperation			,(void (*)(FuncPROFIDRIVE_type *, u32))funcEnebleOperation		},
{	RFGEnableOut			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToInhibitOperationActive,			InhibitOperationActive	,(void (*)(FuncPROFIDRIVE_type *, u32))funcInhibitOperationActive	},
//{	RFGEnableOut			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	RFGEnableOut			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	RFGEnableOut			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	RFGEnableOut			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	RFGAccEnabled			,(char (*)(FuncPROFIDRIVE_type *, u32))eventRFGAccEnabled_OperatingStatus,		OperatingStatus			,(void (*)(FuncPROFIDRIVE_type *, u32))funcOperatingStatus		},
{	RFGAccEnabled			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToRFGEnableOut,						RFGEnableOut			,(void (*)(FuncPROFIDRIVE_type *, u32))funcRFGEnableOut			},
{	RFGAccEnabled			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToOff1activeStage1,					Off1activeStage1		,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff1activeStage1		},
{	RFGAccEnabled			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToInhibitOperationActive,			InhibitOperationActive	,(void (*)(FuncPROFIDRIVE_type *, u32))funcInhibitOperationActive	},
//{	RFGAccEnabled			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	RFGAccEnabled			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	RFGAccEnabled			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	RFGAccEnabled			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToEnableOperation,					EnebleOperation			,(void (*)(FuncPROFIDRIVE_type *, u32))funcEnebleOperation		},
{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToRFGAccEnabled,					RFGAccEnabled			,(void (*)(FuncPROFIDRIVE_type *, u32))funcRFGAccEnabled			},
{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToRFGEnableOut,						RFGEnableOut			,(void (*)(FuncPROFIDRIVE_type *, u32))funcRFGEnableOut			},
{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToOff1activeStage1,					Off1activeStage1		,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff1activeStage1		},
{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToInhibitOperationActive,			InhibitOperationActive	,(void (*)(FuncPROFIDRIVE_type *, u32))funcInhibitOperationActive	},
//{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	OperatingStatus			,(char (*)(FuncPROFIDRIVE_type *, u32))eventToEnableOperation,					EnebleOperation			,(void (*)(FuncPROFIDRIVE_type *, u32))funcEnebleOperation		},
{	InhibitOperationActive	,(char (*)(FuncPROFIDRIVE_type *, u32))eventInhibitOperationActive_Ready,		Ready					,(void (*)(FuncPROFIDRIVE_type *, u32))funcReady					},
{	InhibitOperationActive	,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	InhibitOperationActive	,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	InhibitOperationActive	,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
//{	InhibitOperationActive	,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	SwitchOnInhibit			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	SwitchOnInhibit			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	SwitchOnInhibit			,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	SwitchOnInhibit			,(char (*)(FuncPROFIDRIVE_type *, u32))eventSwitchOnInhibit_NotReadyForSwitch,	NotReadyForSwitchOn		,(void (*)(FuncPROFIDRIVE_type *, u32))funcNotReadyForSwitchOn	},
{	Off1activeStage1		,(char (*)(FuncPROFIDRIVE_type *, u32))eventOff1activeStage1_Off1activeStage2,	Off1activeStage2		,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff1activeStage2		},
//{	Off1activeStage1		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
//{	Off1activeStage1		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	Off1activeStage1		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	Off1activeStage1		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	Off1activeStage2		,(char (*)(FuncPROFIDRIVE_type *, u32))eventOff2active_NoReadyForSwitchOn,		NotReadyForSwitchOn		,(void (*)(FuncPROFIDRIVE_type *, u32))funcNotReadyForSwitchOn	},
//{	Off1activeStage2		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	Off1activeStage2		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	Off1activeStage2		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	Off1activeStage2		,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	Off2active				,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				},
{	Off2active				,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	Off2active				,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	Off3active				,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	Off3active				,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_SwitchOnInhibit,		SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit		},
{	Off3active				,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Fault,					Fault					,(void (*)(FuncPROFIDRIVE_type *, u32))funcFault					},
{	Fault					,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off2active,			Off2active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff2active				},
{	Fault					,(char (*)(FuncPROFIDRIVE_type *, u32))eventFault_SwitchOnInhibit,				SwitchOnInhibit			,(void (*)(FuncPROFIDRIVE_type *, u32))funcSwitchOnInhibit				},
{	Fault					,(char (*)(FuncPROFIDRIVE_type *, u32))eventFromAnyState_Off3active,			Off3active				,(void (*)(FuncPROFIDRIVE_type *, u32))funcOff3active				}
};
#endif

States_type currState = SwitchedOff;
States_type currStatePrv = SwitchedOff;

//********************************************************
u16 *FuncPROFIDRIVE_1(FuncPROFIDRIVE_type *progPnt, u32 ramPnt)
{	
	#ifdef   _PROJECT_FOR_CCS_
	//States_type currState = SwitchedOff;
	unsigned int i = 0;

	AT24_StatusWord2.all = load_s16(ramPnt, progPnt->Pnt_ResIn5); //StatusWord2
	ProfiBAddr = load_s16(ramPnt, progPnt->Pnt_ResIn9);
	numOfPPO = load_s16(ramPnt, progPnt->Pnt_ResIn11);

	if(load_s16(ramPnt, progPnt->Pnt_ResIn12) == 3){
		if(flagIsFirstCall){
			SetCW(0);
			SetSW(0);
		}

		for(i = 0; i < NumOfTransition; i++){
			if((currState == TransitionTable[i].currState) && TransitionTable[i].Event(progPnt, ramPnt)){
				TransitionTable[i].Func(progPnt, ramPnt);
				currStatePrv = currState;
				currState = TransitionTable[i].nextState;
				break;
			}
		}

		//Print CW
		save_s16(ramPnt, progPnt->Pnt_ResIn10, GetCW());
		//Saving SW
		save_s16(ramPnt, progPnt->Pnt_ResOut9, GetSW());
	}
	else{

	}

	//Контроль связи по ProfiBus
	if(load_s16(ramPnt, progPnt->Pnt_ResIn1)){
		if(ConnectionControl.EnFlag == 0){
			ConnectionControl.EnFlag = 1;
			ConnectionControl.TimerSec = load_s16(ramPnt, progPnt->Pnt_ResIn2);
		}
		else{
			save_s16(ramPnt, progPnt->Pnt_ResIn2, ConnectionControl.TimerSec);
		}
		save_s16(ramPnt, progPnt->Pnt_ResOut1, ConnectionControl.ErrStatus);
		if(ConnectionControl.TimerSec == 0){
			ConnectionControl.EnFlag = 0;
			save_s16(ramPnt, progPnt->Pnt_ResIn1, ConnectionControl.EnFlag);
			save_s16(ramPnt, progPnt->Pnt_ResIn2, ConnectionControl.TimerSec);
			save_s16(ramPnt, progPnt->Pnt_ResOut1, ConnectionControl.ErrStatus);
		}
	}

	#endif
    return &progPnt->Pnt_End;
};

//********************************************************
#ifdef   _PROJECT_FOR_CCS_
//*********************************************** E V E N T S********************************************//
//NoReadyForSwitch -> ReadyForSwitch
unsigned char eventNoReadyForSwitch_ReadyForSwitch(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((ControlWord.Off2NotActive_1) && (ControlWord.Off3NotActive_2) && (ControlWord.RemoteControl_10) && (!ControlWord.Off1NotActive_0)){
		eventStatus = eventTrue;
		funcSelectRC(progPnt, ramPnt);
		save_s16(ramPnt, progPnt->Pnt_ResOut3, ON); //FreqStop
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut8); //STOP
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//ReadyForSwitch -> Ready
unsigned char eventReadyForSwitch_Ready(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((ControlWord.Off3NotActive_2) && (ControlWord.Off2NotActive_1) && (ControlWord.Off1NotActive_0) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut8); //STOP
		funcSelectRC(progPnt, ramPnt);
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//FromAnyState -> SwitchOnInhibit
unsigned char eventFromAnyState_SwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	//if(!EnableM){
		eventStatus = eventTrue;
	//}
	//else{
	//	eventStatus = eventFalse;
	//}
	return eventStatus;
}
//FromAnyState -> Off3active
unsigned char eventFromAnyState_Off3active(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((!(ControlWord.all & (1<<2))) && (ControlWord.all & (1<<10))){
		eventStatus = eventTrue;
		funcSelectRC(progPnt, ramPnt);
		save_s16(ramPnt, progPnt->Pnt_ResOut3, 1); //FreqStop
		setBitMicro(ramPnt, progPnt->Pnt_ResOut8); //STOP
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!PUSK
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//FromAnyState -> Off2active
unsigned char eventFromAnyState_Off2active(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((!(ControlWord.Off2NotActive_1)) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
		funcSelectRC(progPnt, ramPnt);
		save_s16(ramPnt, progPnt->Pnt_ResOut3, 0); //CoastStop
		setBitMicro(ramPnt, progPnt->Pnt_ResOut8); //STOP
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!PUSK
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//FromAnyState -> Fault
unsigned char eventFromAnyState_Fault(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	if(AT24_StatusWord2.FailTekDrvBit_0){ //Fault
		eventStatus = eventTrue;
		save_s16(ramPnt, progPnt->Pnt_ResOut3, 0);
		save_s16(ramPnt, progPnt->Pnt_ResOut4, 0);
		setBitMicro(ramPnt, progPnt->Pnt_ResOut8); //STOP
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!PUSK
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//SwitchOnInhibit -> NotReadyForSwitch
unsigned char eventSwitchOnInhibit_NotReadyForSwitch(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if(((!ControlWord.Off1NotActive_0) && (ControlWord.RemoteControl_10))){
		eventStatus = eventTrue;
		funcSelectRC(progPnt, ramPnt);
		setBitMicro(ramPnt, progPnt->Pnt_ResOut8); //STOP
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//InhibitOperationActive -> Ready
unsigned char eventInhibitOperationActive_Ready(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	if(OperationInhibitM){
		eventStatus = eventTrue;
		OperationInhibitM = 0;
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut8); //!STOP
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!PUSK
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//Ready -> EnableOperation
unsigned char eventReady_EnableOperation(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((ControlWord.PWMOn_3) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
		funcSelectRC(progPnt, ramPnt);
		setBitMicro(ramPnt, progPnt->Pnt_ResOut2); //PUSK
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//EnableOperation -> RFGEnableOut
unsigned char eventEnableOperation_RFGEnableOut(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((ControlWord.Working_4) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
		funcSelectRC(progPnt, ramPnt);
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//ToInhibitOperationActive
unsigned char eventToInhibitOperationActive(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((!ControlWord.PWMOn_3) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!PUSK
		setBitMicro(ramPnt, progPnt->Pnt_ResOut8); //!STOP
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
} 
//ToOff1Active
unsigned char eventToOff1activeStage1(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	static unsigned char newState = 0, prvState = 0, enterFlag = 0;

	ControlWord.all = GetCW();

	newState = ControlWord.Off1NotActive_0;

	if(enterFlag && (ControlWord.RemoteControl_10)){
		enterFlag = 0;
		eventStatus = eventTrue;
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //!PUSK
		setBitMicro(ramPnt, progPnt->Pnt_ResOut8); //STOP
	}
	else{
		eventStatus = eventFalse;
	}

	if((newState != prvState) && (newState == 0)){
		enterFlag = 1;
	}

	prvState = newState;
	return eventStatus;
}
//RFGEnableOut -> RFGAccEnabled
unsigned char eventRFGEnableOut_RFGAccEnabled(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((ControlWord.GTOn_5) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//ToEnableOperation
unsigned char eventToEnableOperation(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if(!(ControlWord.Working_4)){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//RFGAccEnabled -> OperatingStatus
unsigned char eventRFGAccEnabled_OperatingStatus(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((ControlWord.GTRefOn_6) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//Off1activeStage1 -> Off1activeStage2
unsigned char eventOff1activeStage1_Off1activeStage2(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	if((load_s16(ramPnt, progPnt->Pnt_Freq) == 0) && (load_s16(ramPnt, progPnt->Pnt_Current) == 0)){
		eventStatus = eventTrue;
		clrBitMicro(ramPnt, progPnt->Pnt_ResOut2); //Pusk
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//Off2active -> NoReadyForSwitchOn
unsigned char eventOff2active_NoReadyForSwitchOn(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	eventStatus = eventTrue;
	return eventStatus;
}
//Fault -> SwitchOnInhibit
unsigned char eventFault_SwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	static unsigned char newState = 0, prvState = 0;
	ControlWord.all = GetCW();
	newState = ControlWord.FaultChek_7;
	if(newState && (!prvState)){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	prvState = newState;
	return eventStatus;
}
//Off3active -> SwitchOnInhibit
unsigned char eventOff3active_SwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	if((load_s16(ramPnt, progPnt->Pnt_Freq) == 0) && (load_s16(ramPnt, progPnt->Pnt_Current) == 0) && load_s16(ramPnt, progPnt->Pnt_Contactor)){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//Off2active -> SwitchOnInhibit
unsigned char eventOff2active_SwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	if(load_s16(ramPnt, progPnt->Pnt_Contactor)){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//ToRFGEnableOut
unsigned char eventToRFGEnableOut(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if(!(ControlWord.GTOn_5) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//ToRFGAccEnabled
unsigned char eventToRFGAccEnabled(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	ControlWord.all = GetCW();
	if((!(ControlWord.GTRefOn_6)) && (ControlWord.RemoteControl_10)){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//Power
unsigned char eventPower(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	unsigned char eventStatus = eventFalse;
	if(PowerM){
		eventStatus = eventTrue;
	}
	else{
		eventStatus = eventFalse;
	}
	return eventStatus;
}
//*********************************************** E N D   O F   E V E N T S ********************************************//

//************************************************** F U N C T I O N S *************************************************//
//SwitchOnInhibit
void funcSwitchOnInhibit(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	StatusWord.SwitchOnInhibit_6 = ON;
	if(AT24_StatusWord2.Gotov_1 == OFF){
		StatusWord.ReadyForSwitchOn_0 = OFF;
		StatusWord.Ready_1 = OFF;
		StatusWord.EnableOperation_2 = OFF;
	}
	SetSW(StatusWord.all);
}
//NotReadyForSwitchOn
void funcNotReadyForSwitchOn(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	if(AT24_StatusWord2.Gotov_1 == OFF){
		StatusWord.ReadyForSwitchOn_0 = OFF;
		StatusWord.SwitchOnInhibit_6 = OFF;
	}
	if(AT24_StatusWord2.PWNOnDrv_2 == OFF){
		StatusWord.EnableOperation_2 = OFF;
	}
	SetSW(StatusWord.all);
}
//ReadyForSwitchOn
void funcReadyForSwitchOn(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	if(AT24_StatusWord2.Gotov_1 == ON){
		StatusWord.ReadyForSwitchOn_0 = ON;
	}
	StatusWord.Off2active_4 = ON;
	StatusWord.Off3active_5 = ON;
	SetSW(StatusWord.all);
}
//Ready
void funcReady(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	if(AT24_StatusWord2.Gotov_1 == ON){
		StatusWord.Ready_1 = ON;
	}
	SetSW(StatusWord.all);
}
//EnebleOperation
void funcEnebleOperation(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	if(AT24_StatusWord2.Gotov_1 == ON){
		StatusWord.EnableOperation_2 = ON;
	}
	SetSW(StatusWord.all);
}
//RFGEnableOut
void funcRFGEnableOut(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	SetSW(StatusWord.all);
}
//RFGAccEnabled
void funcRFGAccEnabled(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	SetSW(StatusWord.all);
}
//OperatingStatus
void funcOperatingStatus(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	if(AT24_StatusWord2.GTUp_4 == OFF){
		StatusWord.FreqReached_10 = ON;
	}
	SetSW(StatusWord.all);
}
//Off1activeStage1
void funcOff1activeStage1(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	StatusWord.Ready_1 = OFF;
	SetSW(StatusWord.all);
}
//Off1activeStage2
void funcOff1activeStage2(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	StatusWord.Ready_1 = OFF;
	SetSW(StatusWord.all);
}
//Off2active
void funcOff2active(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	StatusWord.Off2active_4 = OFF; //this OFF means ON
	SetSW(StatusWord.all);
}
//Off3active
void funcOff3active(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	StatusWord.Off3active_5 = OFF; //this OFF means ON
	SetSW(StatusWord.all);
}
//Fault
void funcFault(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	if(load_s16(ramPnt, progPnt->Pnt_ResIn8)){
		StatusWord.Fault_3 = ON;
	}
	SetSW(StatusWord.all);
}
//InhibitOperationActive
void funcInhibitOperationActive(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){
	StatusWord.all |= GetSW();
	StatusWord.ReadyForSwitchOn_0 = OFF;
	StatusWord.Off2active_4 = ON; //this ON means OFF
	StatusWord.Off3active_5 = ON; //this ON means OFF
	SetSW(StatusWord.all);
}

void funcSelectRC(FuncPROFIDRIVE_type *progPnt, u32 ramPnt){

	#define  RC   2
	s16 AC = load_s16(ramPnt, progPnt->Pnt_ResIn7);
	switch(AC){ //Active Chanel
		case 0://Ch1
			save_s16(ramPnt, progPnt->Pnt_ResOut5, RC); //Ch1
			break;
		case 1://Ch2
			save_s16(ramPnt, progPnt->Pnt_ResOut6, RC); //Ch2
			break;
		case 2:
			break;
	}
}
#endif
#endif
//********************************************* E N D   O F   F U N C T I O N S ****************************************//

#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPROFIDRIVE_1[40]={
        S16_PIN_TYPE | INPUT_PIN_MODE,   //1  Fault
        S16_PIN_TYPE | INPUT_PIN_MODE,   //2  Speed
        S32_PIN_TYPE | INPUT_PIN_MODE,   //3  Freq
        S32_PIN_TYPE | INPUT_PIN_MODE,   //4  Current
        S16_PIN_TYPE | INPUT_PIN_MODE,   //5  ResIn1
        S16_PIN_TYPE | INPUT_PIN_MODE,   //6  ResIn2
        S16_PIN_TYPE | INPUT_PIN_MODE,   //7  ResIn3
        S16_PIN_TYPE | INPUT_PIN_MODE,   //8  ResIn4
        S16_PIN_TYPE | INPUT_PIN_MODE,   //9  ResIn5
        S16_PIN_TYPE | INPUT_PIN_MODE,   //10  ResIn6
        S16_PIN_TYPE | INPUT_PIN_MODE,   //11  ResIn7
        BIT_PIN_TYPE | INPUT_PIN_MODE,   //12  ResIn8
        S16_PIN_TYPE | INPUT_PIN_MODE,   //13  ResIn9
        S16_PIN_TYPE | INPUT_PIN_MODE,   //14  ResIn10
        S16_PIN_TYPE | INPUT_PIN_MODE,   //15  ResIn11
        S16_PIN_TYPE | INPUT_PIN_MODE,   //16  ResIn12
        S16_PIN_TYPE | INPUT_PIN_MODE,   //17  OperInhib
        S16_PIN_TYPE | INPUT_PIN_MODE,   //18  Enable
        BIT_PIN_TYPE | INPUT_PIN_MODE,   //19  Contactor
        S16_PIN_TYPE | INPUT_PIN_MODE,   //20  Voltage
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //21  ResOut1
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //22  ResOut2
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //23  ResOut3
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //24  ResOut4
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //25  ResOut5
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //26  ResOut6
        S32_PIN_TYPE | OUTPUT_PIN_MODE,   //27  ResOut7
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,   //28  ResOut8
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //29  ResOut9
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //30  ResOut10
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //31  ResOut11
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //32  ResOut12
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //33  ResOut13
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //34  ResOut14
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //35  ResOut15
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //36  ResOut16
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //37  ResOut17
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //38  ResOut18
        S16_PIN_TYPE | OUTPUT_PIN_MODE,   //39  ResOut19
        S16_PIN_TYPE | OUTPUT_PIN_MODE    //40  ResOut20
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PROFIDRIVE", "U", TblFuncPROFIDRIVE_1, FuncPROFIDRIVE_1, sizeof(TblFuncPROFIDRIVE_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPROFIDRIVE_1,
#endif
//********************************************************
