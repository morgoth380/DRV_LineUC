#ifdef   GET_FUNC_CODE

#include "MakeOpis_at24.h"
 
#include "Tsk_MasterUart.h"
#include "Global_include.h"         // Include header for IQmath library

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    u16  Pnt_nvState;
    u16  Pnt_INPUT1;                       //1  K_PROPOR
    u16  Pnt_INPUT2;                       //2  K_INTEGR
    u16  Pnt_INPUT3;                       //3  режим U0 
    u16  Pnt_INPUT4;                        //4 контроль аварии обмена
    u16  Pnt_INPUT5;                       //5   
    u16  Pnt_INPUT6;                      //6  Fzad
    u16  Pnt_INPUT7;                       //7 K_PROPOR момент
    u16  Pnt_INPUT8;                       //8  K_INTEGR момент
    u16  Pnt_INPUT9;                       //9  ограничение момента  в pu
    u16  Pnt_INPUT10;                      //10 
	u16  Pnt_INPUT11;                      //11  
    u16  Pnt_INPUT12;                       //12
	u16  Pnt_INPUT13;                       //13
    u16  Pnt_INPUT14;                       //14
	u16  Pnt_INPUT15;                       //15
	u16  Pnt_INPUT16;                       //16
	u16  Pnt_INPUT17;                       //17
    u16  Pnt_INPUT18;                       //18
	u16  Pnt_INPUT19;                       //19
	u16  Pnt_INPUT20;                       //20

    u16  Pnt_OUTPUT1;                      //21  RELE master-slave
    u16  Pnt_OUTPUT2;                      //22  
    u16  Pnt_OUTPUT3;                      //23
    u16  Pnt_OUTPUT4;                      //24   
	u16  Pnt_OUTPUT5;                      //25  
    u16  Pnt_OUTPUT6;                      //26  
    u16  Pnt_OUTPUT7;                      //27 выбегчастотный 
    u16  Pnt_OUTPUT8;                      //28  время выбега 
	u16  Pnt_OUTPUT9;                      //29  
    u16  Pnt_OUTPUT10;                      //30  
    u16  Pnt_OUTPUT11;                      //31
    u16  Pnt_OUTPUT12;                      //32  
    u16  Pnt_End;
}FuncUNIT3_type;


typedef struct 
{                    
	  u16 pr_PHase_enable;
	 	u16 count_Pause_start;  //пауза при старте
		u16 count_Pause_phase;
			u16 count_temp_phase;
	 	u16  temp_phase_old ;
	 	u16	temp_phase;  
		
		u16 pr_error21;
	 	u16 Type_Break_ZIP;
		u16 Time_Vubeg_ZIP;

         u16 Regim_Privod;  // режим при включении
         f32     Koeff_Pr_Torque;
        
		 u16 Type_Break;
		 u16 Time_Vubeg;
 
}PICONTROL_State_type; 

 typedef struct{
    
u16     Regim_Privod;
   f32     Koeff_Pr_Torque_Master;
   f32     Koeff_Pr_Torque_Slave;
   u16     Type_Break;
   u16     Time_Vubeg;
}nvState_UNIT3_MODE_type;


#define Torque_Little 1
#define Torque_Big 2

 #define STOP_FREQ   1
 #define STOP_VUBEG  0
/** Структура данных ПИ-регулятора.  Она содержит настройки 
(коэффициент усиления, интегральная часть, а также конечный делитель), 
выходные пределы, и аккумулятор интегральной части. */

/*===========================================================================*/
// Преобразование int <-> float
/*===========================================================================*/
#define m1          ((f32)1.	)
#define m10         ((f32)10.	)
#define m100        ((f32)100.	)
#define m1000       ((f32)1000.	)
#define m10000      ((f32)10000.)

#define d1          ((f32)1.    )
#define d10         ((f32).1    )
#define d100        ((f32).01   )
#define d1000       ((f32).001  )
#define d10000      ((f32).0001 )


#define ModbusSlave   1
#define  ModbusMaster 2

    #define  REGIM_ALONE   0 /*0 - одиночный*/
    #define  REGIM_MASTER  1 /*1 - master*/
    #define  REGIM_SLAVE   2 /*2 - slave*/
    #define  REGIM_NEUPR   3 /*3 - неуправляемый*/

#define     nvSfUNIT3_MODE    (*(nvState_UNIT3_MODE_type*)0)
/*===========================================================================*/
// Преобразование из float32 в s16 с заданной точностью
// передается коэфициент преобразования переменной
/*===========================================================================*/
extern s16 f2int(f32 data, f32 mf);
/*===========================================================================*/
// Преобразование из s16 в float32 с заданной точностью
// передается коэфициент преобразования переменной
/*===========================================================================*/
extern f32 int2f(s16 data, f32 mf);

 
//********************************************************
u16 *FuncUNIT3_1(FuncUNIT3_type *progPnt, u32 ramPnt)
{ // _iq								tmpIQ;
//	u16   temp_regim;//,temp;//,mode;
	
   s16  temp_phase;
//	s16  temp_s16;
//	f32 fL;//,Koeff_Pr;
   // u16     Time_Vubeg,Type_Break,;
    u16     temp_state;//,State_word;
 //	_iq   Torrent_Real;//OUT_PI_REGULATOR,OUT_OLD,Ipartner,Iwork,delta;

	nvState_UNIT3_MODE_type    *nvAdr;//, nvMem;
   PICONTROL_State_type   *sPnt;

     COMMONPARAMM3	*mPnt3;
 
	mPnt3	= &GlobalM3;

  	   nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvSfUNIT3_MODE);

	 //GlobalM3.errorTimeoutCounter = GlobalMstSlv.errorTimeoutCounter;// (u16)GlobalM3.TorqBASE); //GlobalM3.Drive_STATE);   // Состояние приводаGlobalM3
  //   GlobalM3.errorConnectCounter= GlobalMstSlv.errorConnectCounter;
  //   GlobalM3.TrueConnectCounter= GlobalMstSlv.TrueConnectCounter;
     sPnt = (PICONTROL_State_type *)(ramPnt + progPnt->Pnt_State);		// флажок завершения пуска
  		if(flagIsFirstCall){

  			// первое вхождение для установки нач условий
                        RdFramMicro(&(sPnt->Regim_Privod), &(nvAdr->Regim_Privod), sizeof(u16)); //WrFramMicro(&(nvAdr->prevFailState), &(sPnt->prevFailState), sizeof(u64));
				 		  GlobalM3.Regim_Privod=sPnt->Regim_Privod;//nvMem.Regim_Privod;   // чтение из фрам состояния  при включении
                         

	

                        sPnt->count_Pause_start = 2 * K_TIME_CALL_MICRO_TAKT; //10sec
						sPnt->pr_PHase_enable=0;
						sPnt->count_Pause_phase=0;
 
 	       			  
 	       	 		  GlobalM3.FlagCommand.bit.BitWAitPhase =0;
        			  GlobalM3.FlagCommand.bit.BitGotovWorkSVP=0;

 
 	       	     }

  		//GlobalM3.Kvitirovanie  0x0800 - режим СВП 0x0400 - разреш квитирования  0x200 авария 0x25
  	//	if(GlobalM3.FlagCommand.bit.BitGotovWorkSVP){GlobalM3.Kvitirovanie |=0x0800;}
  	//	else {GlobalM3.Kvitirovanie &=~(0x0800);}
 
  // State_word=load_s16(ramPnt,progPnt->Pnt_INPUT5); //принял слово-состояние привода
////======внешняя блокировка
//  		if(testBitMicro(ramPnt, progPnt->Pnt_INPUT4)) 
//  		 {setBitMicro(ramPnt, progPnt->Pnt_OUTPUT11);
//  		 }
//  		 else
//  		 {
//  		  clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT11);
//  		 }
//  //===================реле дублирование кулера
//if( mPnt3->Cooler_PWM==0)
//	{clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT2);// rele    = 1.0f;
//
//	}
//else
//	{setBitMicro(ramPnt, progPnt->Pnt_OUTPUT2);// rele 2  = 1.0f;
//
//	}
           //=================ограничение момента  63 06
      // temp_s16	= load_s16(ramPnt, progPnt->Pnt_INPUT9); //ограничение момента
	 	   // fL=(f32)temp_s16/100.0;
      //mPnt3->TorqueBoard= _IQ(mPnt3->Torque_n/mPnt3->TorqBASE*fL) ;//номин момент в о
 
    //  mPnt3->TorqueMax =mPnt3->TorqueBoard;
	 // //НУЖНО МЕНЯТЬ ОГРАНИЧЕНИЕ ПРИ СМЕНЕ НАПРАВЛЕНИЯ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 //mPnt3->TorqueMax =  load_s32(ramPnt, progPnt->Pnt_INPUT14); 

////коэфф наблюдателя
//		    temp_s16= load_s16(ramPnt, progPnt->Pnt_INPUT2); //1  K_INTEGR
//	GlobalM3.KiFe =(f32)temp_s16*0.00001;
//	temp_s16 = load_s16(ramPnt, progPnt->Pnt_INPUT1);   //2  K_PROPOR
//	GlobalM3.KpFe =(f32)temp_s16*0.00001;
//// коэфф регулятора момента
//	temp_s16= load_s16(ramPnt, progPnt->Pnt_INPUT7); //  пропорц коэфф момента
//GlobalM3.Koeff_Pr_Torque=(f32)temp_s16*0.1;////int2f(load_s16(ramPnt, progPnt->Pnt_INPUT7), d10);
//temp_s16= load_s16(ramPnt, progPnt->Pnt_INPUT8); // интегр коэфф момента
//GlobalM3.Koeff_In_Torque=(f32)temp_s16*0.1;//int2f(load_s16(ramPnt, progPnt->Pnt_INPUT8), d10);


 //	GlobalM3.FlagCommand.bige=load_s16 (ramPnt, progPnt->Pnt_INPUT11);

////==============изменение режима======================================   
//temp_s16=load_s16(ramPnt, progPnt->Pnt_INPUT12); 
//
//if(temp_s16!=sPnt->Regim_Privod) // режим поменялся
//{
//	if(testBitMicro(ramPnt, progPnt->Pnt_INPUT9))  //GlobalM3.FlagCommand.bit.BitInput_ChangeDI==1) //мастер на линии уже есть
//	{
//		if((temp_s16==REGIM_MASTER)||(temp_s16==REGIM_ALONE))
//		{ save_s16(ramPnt,progPnt->Pnt_INPUT12,GlobalM3.Regim_Privod);
//		}
//		else
//		{  GlobalM3.Regim_Privod =  temp_s16;
//		}
//	}
//	else                                              //мастера на линии нет
//	{
//		GlobalM3.Regim_Privod =  temp_s16;
//	} 
////	sPnt->Regim_Privod=GlobalM3.Regim_Privod;
//	 // nvMem.Regim_Privod=GlobalM3.Regim_Privod;
//	  //  WrFramMicro(nvAdr, &nvMem, sizeof(nvState_UNIT3_MODE_type));
////	WrFramMicro(&(nvAdr->Regim_Privod), &(sPnt->Regim_Privod), sizeof(u16));
//}
 
 //=====================// режим поменялся=================================== 
 
//if(GlobalM3.Regim_Privod!=sPnt->Regim_Privod) // режим поменялся
//{  
//
//
////	 Koeff_Pr=int2f(load_s16(ramPnt, progPnt->Pnt_INPUT7), d10);
//	//if((GlobalM3.Regim_Privod==REGIM_SLAVE)&&(sPnt->Regim_Privod==REGIM_ALONE||sPnt->Regim_Privod==REGIM_MASTER))  //стал слейв ,был одиночный или мастер  уменьшить коэфф
//	// {
// //	   RdFramMicro(&(sPnt->Koeff_Pr_Torque), &(nvAdr->Koeff_Pr_Torque_Slave), sizeof(f32));
// //        sPnt->Type_Break=STOP_VUBEG; 
//	//	 sPnt->Time_Vubeg=0;
//	// }
//	// else if((sPnt->Regim_Privod==REGIM_SLAVE)&&(GlobalM3.Regim_Privod==REGIM_ALONE||GlobalM3.Regim_Privod==REGIM_MASTER)) //стал одиночный или мастер ,был слейв  увеличитить коэфф
//	// {	                  RdFramMicro(&(sPnt->Type_Break), &(nvAdr->Type_Break), sizeof(u16));
//	//					  RdFramMicro(&(sPnt->Time_Vubeg), &(nvAdr->Time_Vubeg), sizeof(u16));
// //                         RdFramMicro(&(sPnt->Koeff_Pr_Torque), &(nvAdr->Koeff_Pr_Torque_Master), sizeof(f32));
// //     }
// //
// ////	save_s16(ramPnt,progPnt->Pnt_INPUT18,sPnt->Type_Break);
// ////   save_s16(ramPnt,progPnt->Pnt_INPUT19,sPnt->Time_Vubeg);
//	//save_s16(ramPnt,progPnt->Pnt_OUTPUT7,sPnt->Type_Break);
// //   save_s16(ramPnt,progPnt->Pnt_OUTPUT8,sPnt->Time_Vubeg);
//	//save_s16(ramPnt, progPnt->Pnt_INPUT7	, f2int(sPnt->Koeff_Pr_Torque, m10) 	); 
//
// //	БЛОК3
//		
//    sPnt->Regim_Privod=GlobalM3.Regim_Privod;
//  	WrFramMicro(&(nvAdr->Regim_Privod), &(sPnt->Regim_Privod), sizeof(u16));
// }
//================смена выбега и времени выбега

//if(GlobalM3.FlagCommand.bit.BitGotovWorkSVP==1)
//{
// 
//	if((sPnt->Type_Break !=load_s16(ramPnt,progPnt->Pnt_INPUT18))||(sPnt->Time_Vubeg!=load_s16(ramPnt,progPnt->Pnt_INPUT19)))
//	{
//	        if((GlobalM3.Regim_Privod==REGIM_MASTER)||(GlobalM3.Regim_Privod==REGIM_ALONE))
//	         {
//
//		     sPnt->Type_Break =load_s16(ramPnt,progPnt->Pnt_INPUT18);
//             sPnt->Time_Vubeg =load_s16(ramPnt,progPnt->Pnt_INPUT19);
//    	     WrFramMicro(&(nvAdr->Type_Break), &(sPnt->Type_Break), sizeof(u16));
//             WrFramMicro(&(nvAdr->Time_Vubeg), &(sPnt->Time_Vubeg), sizeof(u16));
//           
// 	             save_s16(ramPnt,progPnt->Pnt_OUTPUT7,sPnt->Type_Break);
//                 save_s16(ramPnt,progPnt->Pnt_OUTPUT8,sPnt->Time_Vubeg);
//              }
//	}  //смена коэфф
//        if(sPnt->Koeff_Pr_Torque != int2f(load_s16(ramPnt, progPnt->Pnt_INPUT7), d10)); 
//		{
//			sPnt->Koeff_Pr_Torque = int2f(load_s16(ramPnt, progPnt->Pnt_INPUT7), d10);
//             if((GlobalM3.Regim_Privod==REGIM_MASTER)||(GlobalM3.Regim_Privod==REGIM_ALONE))
//			 {
//				 WrFramMicro(&(nvAdr->Koeff_Pr_Torque_Master), &(sPnt->Koeff_Pr_Torque), sizeof(f32));
//			 }
//			 else
//			 {    WrFramMicro(&(nvAdr->Koeff_Pr_Torque_Slave), &(sPnt->Koeff_Pr_Torque), sizeof(f32));
//			 }
//		}
//}

   //БЛОК2
 
//	БЛОК1
//============================контроль аварии обмена
//if((GlobalM3.Regim_Privod==REGIM_MASTER)||(GlobalM3.Regim_Privod==REGIM_SLAVE))
//{
// 
//						    if(testBitMicro(ramPnt, progPnt->Pnt_INPUT17))//)||((mPnt3->Slave_STATE &0x2000)!=0))
//
//							{
//							   setBitMicro(ramPnt, progPnt->Pnt_OUTPUT5);  //   avaria обмена
//							}
//						   else
//						   {
//                              clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT5);  //  avaria обмена
//							}
//
//			//	   clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT9);                     //  warning неупр
//}
//else 
//{
// clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT5);  //  avaria обмена
//  //if (GlobalM3.Regim_Privod==REGIM_NEUPR)
//  //{
//	 //setBitMicro(ramPnt, progPnt->Pnt_OUTPUT9);                     //  warning неупр
//  //}
//  //else
//  //{  clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT9);                     //  warning неупр
//  //}
//
//
//}
//==================================================================================================================
//=========================ожидание сети  // новый алгоритм проверки наличия 3 фаз при включении=======================================================================
//================================================================================================================= 
//GlobalM3.FlagCommand.bit.BitEnableWaitVoltage=load_s16 (ramPnt, progPnt->Pnt_INPUT11);
if(testBitMicro(ramPnt, progPnt->Pnt_INPUT3))
{temp_phase =1;
//GlobalM3.TEMP1=100;
}
else
{temp_phase =0;
//GlobalM3.TEMP1=0;
}




 if((GlobalM3.FlagCommand.bit.BitGotovWorkSVP ==1)&&(GlobalM3.FlagCommand.bit.BitEnableWaitVoltage!=0))
 {
    if(sPnt->count_Pause_start>0)
	{  setBitMicro(ramPnt, progPnt->Pnt_OUTPUT10);  //    запрета заряда
	        sPnt->count_Pause_start--;
           if(temp_phase !=0) //обрыв фаз
           {

              if(sPnt->count_Pause_start==0)
               {
                    GlobalM3.FlagCommand.bit.BitWAitPhase=1;

                            sPnt->pr_PHase_enable=0;
                            sPnt->count_Pause_phase=0;
               }
            }

           clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT3);
	}
	else
  	{
	    if(temp_phase ==0) //нет обрыв фаз
	    {
 	        sPnt->pr_PHase_enable=4;
 	         GlobalM3.FlagCommand.bit.BitWAitPhase=0;
 	                clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT10);  //   разреша заряда
 	               clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT3);  //    запрета аварии
	    }
	    else
 	    {
 	        if(sPnt->pr_PHase_enable>0)
 	      {  sPnt->pr_PHase_enable--;
 	        if(sPnt->pr_PHase_enable>0)
 	            {
 	                setBitMicro(ramPnt, progPnt->Pnt_OUTPUT3);
 	               sPnt->pr_error21=0;
 	            } //    запрета заряда
 	        else
 	           {clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT3);}
 	          setBitMicro(ramPnt, progPnt->Pnt_OUTPUT10);  //    запрета заряда
 	      }
 	      else
 	      {
 	         temp_state =load_s16(ramPnt, progPnt->Pnt_INPUT5);//слово состояние


 	         if((temp_state & 0xff)==0x21)
 	         {
 	            sPnt->pr_error21=1;
 	         }
 	        if(((temp_state & 0xff)==0)&&(sPnt->pr_error21==1))
 	          {
 	                       sPnt->pr_error21=0;
 	                        GlobalM3.FlagCommand.bit.BitWAitPhase=1;
 	          }
 	      }


	    }
  	}
 }

// //==================================================Работа мастером
//if(GlobalM3.FlagCommand.bit.BitGotovWorkSVP==0)
//{
//	if (GlobalMstSlv.Uart0_Mode == ModbusMaster || GlobalMstSlv.Uart2_Mode == ModbusMaster)
//		{temp_regim=REGIM_MASTER;
//
//		}
//	else
//		{temp_regim=0;
//
//		}
//}
//else
//{
//	 if((GlobalM3.Regim_Privod==REGIM_ALONE)||(GlobalM3.Regim_Privod==REGIM_MASTER))
//		 {temp_regim=REGIM_MASTER;
//
//		 }
//	 else
//	  {temp_regim=0;
//
//	  }
//}
//
//  if (temp_regim==REGIM_MASTER)//MASTER       (Uart0_Mode == ModbusMaster || Uart2_Mode == ModbusMaster)
// {
//     save_s16(ramPnt,progPnt->Pnt_OUTPUT7,sPnt->Type_Break);
//     save_s16(ramPnt,progPnt->Pnt_OUTPUT8,sPnt->Time_Vubeg);
// 
// 
////принимаю
//                   mPnt3->Slave_STATE =GlobalMstSlv.Slave_STATE;        // Состояние привода  слейв
//				   mPnt3->Slave_IQ_Torque=(_iq)GlobalMstSlv.Slave_IQ_Torque;    //со слейв принять  момент текущий
//	               mPnt3->Slave_IQ_FREQ=(_iq)GlobalMstSlv.Slave_IQ_FREQ ;         //со слейв принять скорост  текущий
//
//	// if(mPnt3->TorqueBoard< mPnt3->Slave_IQ_Torque)
//	//	 {
//			 mPnt3->Master_STATE = Torque_Big; //2 момент слейва больше уставки
//
//	//	 }
//	// else
//	//	 {mPnt3->Master_STATE = Torque_Little;//1
//
//	//	 }
//           
//	// if(GlobalC28.NumFailDRV !=0) mPnt3->Master_STATE|=0x80;
//               
//    //передаю
////ДБ
////GlobalM3.Master_STATE    формировать
////GlobalM3.Master_IQ_FREQ  =  GlobalC28.Wout
////GlobalM3.Master_IQ_Torque =GlobalC28.Mzad;
//
//
//                //   mPnt3->Master_IQ_Torque = GlobalC28.Tzad;
//	            //   mPnt3->Master_IQ_FREQ =  GlobalC28.Wout;
//
//	   //временно перенес сюда
//	 //  mPnt3->Master_IQ_Torque= GlobalC28.Tzad;
//	 // GlobalMstSlv.Master_IQ_Torque=GlobalC28.Tzad;
//        if((mPnt3->Slave_STATE &0xff)!=0) GlobalMstSlv.SlaveStatusWord1|=0x2000;        // Состояние привода  слейв  ваврии
//		else  GlobalMstSlv.SlaveStatusWord1 &=~(0x2000);
//			       GlobalMstSlv.Master_STATE = mPnt3->Master_STATE;     // Состояние привода  мастер  на слейв
//	             //  GlobalMstSlv.Master_IQ_Torque =;     //задание момента на слейв (выход РС)
//	               GlobalMstSlv.Master_IQ_FREQ =  mPnt3->Master_IQ_FREQ;    //задание скорости на слейв ( ОС скорости)
//				   mPnt3->Master_IQ_Torque=GlobalC28.Tzad;// _IQdiv(GlobalC28.Tzad,_IQ(mPnt3->Lm));//mPnt3->TorqueBoard);
//	               GlobalMstSlv.Master_IQ_Torque=mPnt3->Master_IQ_Torque;
//
//               	 mPnt3->Driver_STATE= ModbusMaster;
//}
//
//   //==================================================Работа слейвом
//if(GlobalM3.FlagCommand.bit.BitGotovWorkSVP==0)
//{
//	if (GlobalMstSlv.Uart0_Mode == ModbusSlave || GlobalMstSlv.Uart0_Mode == ModbusSlave)
//		{temp_regim=REGIM_SLAVE;
//
//		}
//	else
//		{temp_regim=0;
//
//		}
//}
//else
//{
//	 if((GlobalM3.Regim_Privod==REGIM_SLAVE)||(GlobalM3.Regim_Privod==REGIM_NEUPR))
//		 {temp_regim=REGIM_SLAVE;
//
//		 }
//	 else
//		 {temp_regim=0;
//
//		 }
//}
//
//  if (temp_regim==REGIM_SLAVE)//Slave
// {
//// А если хоть один Slave - работаем как Slave
// 
//     save_s16(ramPnt,progPnt->Pnt_OUTPUT7,STOP_VUBEG);//принуд установка в выбег
//	 save_s16(ramPnt,progPnt->Pnt_OUTPUT8,0);//принуд установка время выбег в 0
//	 save_s16(ramPnt,progPnt->Pnt_INPUT18,STOP_VUBEG);//принуд установка в выбег
//     save_s16(ramPnt,progPnt->Pnt_INPUT19,0);//принуд установка время выбег в 0
//
//                        mPnt3->Master_IQ_FREQ=GlobalMstSlv.Master_IQ_FREQ ;    //задание скорости на слейв 
//                	   
//  // принимаю
//						temp_s16 = load_s16(ramPnt, progPnt->Pnt_INPUT15);   //2  K_PROPOR
//	                  //   fL =(f32)temp_s16*0.01;
//					//	fL =1.0;//fL* mPnt3->Lm;
//			   mPnt3->Master_STATE=GlobalMstSlv.Master_STATE;        // Состояние привода	 мастер
//			   mPnt3->Master_IQ_Torque=GlobalMstSlv.Master_IQ_Torque;// _IQmpy(GlobalMstSlv.Master_IQ_Torque,_IQ(fL));//(GlobalMstSlv.Master_IQ_Torque,mPnt3->TorqueBoard);
//	           mPnt3->Master_IQ_FREQ=GlobalMstSlv.Master_IQ_FREQ ;      // будет задание на момент скорость принять
//	          // mPnt3->Master_IQ_Torque= _IQmpy(mPnt3->Master_IQ_Torque,_IQ(1.1));
//// передаю
////GlobalM3.Slave_STATE     формировать
////GlobalM3.Slave_IQ_FREQ  =  GlobalC28.Wout
////GlobalM3.Slave_IQ_Torque =GlobalC28.Mout;
//
////0x8000 - work
////0x4000 - charge
////0x2000 - error
////0x1000 - готовность
////mPnt3->Slave_STATE |=(State_word &0xf000);
//  if ((State_word & 0x2000)!=0) //обрыв фаз
// {
//	 mPnt3->Slave_STATE |=0x20;//авария на мастер
// }
//  else  mPnt3->Slave_STATE &=~(0x20);//
//			   mPnt3->Slave_STATE=  State_word;
//				   mPnt3->Slave_IQ_Torque = GlobalC28.Mout;
//                   mPnt3->Slave_IQ_FREQ = GlobalC28.Wout;
//                  
//	               GlobalMstSlv.Slave_STATE = mPnt3->Slave_STATE;     // Состояние привода  слейв  на мастер
//	               GlobalMstSlv.Slave_IQ_Torque = mPnt3->Slave_IQ_Torque;     //задание момента на слейв (выход РС)
//	               GlobalMstSlv.Slave_IQ_FREQ = mPnt3->Slave_IQ_FREQ;    //задание скорости на слейв ( ОС скорости)
//
// // if((GlobalMstSlv.Master_STATE &0x80)!=0) GlobalMstSlv.SlaveStatusWord1|=0x2000;        // Состояние привода  слейв  ваврии
//          
//	         mPnt3->Driver_STATE= ModbusSlave;
//  }  
 
////===================================== режим наружу
//
// if(GlobalM3.FlagCommand.bit.BitGotovWorkSVP==1)
// {
//	    switch (GlobalM3.Regim_Privod)
//		{
//		case   REGIM_ALONE:    //  0 //0 - одиночный
//			temp=0;
//		 clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT1);	//rele master-slave 
//        break;
//    case REGIM_MASTER:    //   1 //1 - master
//		temp= ModbusMaster;
//		 setBitMicro(ramPnt, progPnt->Pnt_OUTPUT1);	//rele master-slave
//		  break;
//   case REGIM_SLAVE:    //   2 //2 - slave
//	   temp=ModbusSlave;
//	    clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT1);	//rele master-slave
//	 	  break;
//    case REGIM_NEUPR:    //   3 //3 - неуправляемый
//		temp=0;
//		 clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT1);	//rele master-slave
//		    break;
//		}
//		save_s16(ramPnt,progPnt->Pnt_OUTPUT4,temp);
// }
if(GlobalM3.FlagCommand.bit.BitWAitPhase)setBitMicro(ramPnt, progPnt->Pnt_OUTPUT6);	 
else clrBitMicro(ramPnt, progPnt->Pnt_OUTPUT6);	 
     return &progPnt->Pnt_End;
};


#endif
//********************************************************
#ifdef   GET_FUNC_TABL_OPIS
const char TblFuncUNIT3_1[32]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1   K_PROPOR
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2   K_INTEGR
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3 авария фаз
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4 внешняя блокировка
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  режим привода
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  Fzad
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7   
        S16_PIN_TYPE | INPUT_PIN_MODE,     //8
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  change  master slave
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10 
		S16_PIN_TYPE | INPUT_PIN_MODE,     //11 
        S16_PIN_TYPE | INPUT_PIN_MODE,     //12  
		S16_PIN_TYPE | INPUT_PIN_MODE,     //13  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //14  
		S16_PIN_TYPE | INPUT_PIN_MODE,     //15  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //16  
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //17  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //18  
		S16_PIN_TYPE | INPUT_PIN_MODE,     //19  
        S16_PIN_TYPE | INPUT_PIN_MODE,     //20  


      
         BIT_PIN_TYPE | OUTPUT_PIN_MODE,     //21  OUTPUT1  
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //22  OUTPUT2
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //23  OUTPUT3
	    S16_PIN_TYPE | OUTPUT_PIN_MODE,    //24  OUTPUT4
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //25  OUTPUT5
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //26  OUTPUT6
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //27  OUTPUT7
		S16_PIN_TYPE | OUTPUT_PIN_MODE,    //28  OUTPUT8
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //29  OUTPUT9
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //30 OUTPUT10
		BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //31 OUTPUT11
		S16_PIN_TYPE | OUTPUT_PIN_MODE     //32 OUTPUT12
};
#endif
   
//********************************************************
#ifdef   GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UNIT3", "DD", TblFuncUNIT3_1, FuncUNIT3_1, sizeof(TblFuncUNIT3_1), sizeof(PICONTROL_State_type),sizeof(nvState_UNIT3_MODE_type), 1),
#endif
//********************************************************
#ifdef  GET_FUNC_PNT
    (pFuncMicro_type)FuncUNIT3_1,
#endif
//********************************************************
