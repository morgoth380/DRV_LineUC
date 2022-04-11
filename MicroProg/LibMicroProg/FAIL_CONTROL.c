
#include "FAIL_CONTROL.h"

//********************************************************
void FuncFAIL_CONTROL(FuncFAIL_CONTROL_64_type *progPnt, u32 ramPnt)
{
	FailControlState_type	*sPnt;
	SlotFc_type	            *slotPnt;
	void	                *nvAdr;
	u16	                	FailBmTek[4];                //текущие остановы с учетом задержки контроля
	s16	                	N_CTR, kolvSlot;
	s16	                	TekFail, Request, TypeTormOut;
	s16	                	N_PUSK, N_STOP;
	s32	                	TimeDoOff, Time;
	s16	                	tekStop_CTR, Man_Auto;
    
	sPnt = (FailControlState_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
	if(flagIsFirstCall){            //начальная инициализация
    	nvAdr = GetNvAdr(progPnt->Pnt_nvState, &nvFailControlState.DelayApvCnt);
    	RdFramMicro(&sPnt->DelayApvCnt, nvAdr, 4);
    	sPnt->DelayApvCnt += GetTimePowerOff();            //пересчёт времени без питания
    }
    kolvSlot	= load_s16(ramPnt, progPnt->Pnt_KolvFail);
	memset(FailBmTek, 0, sizeof(FailBmTek));
	N_STOP = N_PUSK = 0;
    TypeTormOut	= 0;
	tekStop_CTR = 0;
 	TimeDoOff	= 0x7FFFFFFF;
	slotPnt	    = &progPnt->SlotFc[1];
    N_CTR	    = 1;
    
	Man_Auto	= load_s16(ramPnt, progPnt->Pnt_Man_Auto);
	if(!DrvIsStop()){
    	for(; N_CTR < kolvSlot; N_CTR++, slotPnt++){
        	TekFail = testBitMicro(ramPnt, slotPnt->Pnt_TekFail );
        	Request	= load_s16(ramPnt, slotPnt->Pnt_Request	);
        	Time	= load_s32(ramPnt, slotPnt->Pnt_Time	);
               
           	if(Time < TimeDoOff)	TimeDoOff = Time;
        	if(TekFail){
            	setBit(FailBmTek, N_CTR);
            	if(tekStop_CTR != 0)	tekStop_CTR = N_CTR;
            }
            
        	if((Request & REQUEST_STOP_MASK)&&(N_STOP == 0)){        //поймали останов
            	TypeTormOut = load_s16(ramPnt, slotPnt->Pnt_TypeTorm);
            	N_STOP	    = N_CTR;
            	sPnt->DelayApvCnt	= Time;                            //заряжаем АПВ
            	sPnt->Request	    = Request;
            	sPnt->N_STOP	    = N_CTR;
            	sPnt->N_PUSK	    = 0;
            }
        }
    }
	else if(PwmIsOn()){            //частотное торможение
    	for(; N_CTR < kolvSlot; N_CTR++, slotPnt++){
        	TekFail = testBitMicro(ramPnt, slotPnt->Pnt_TekFail );
        	Request	= load_s16(ramPnt, slotPnt->Pnt_Request	);
               
        	if(TekFail){
            	setBit(FailBmTek, N_CTR);
            	if(tekStop_CTR != 0)	tekStop_CTR = N_CTR;
            }
            
        	if(N_STOP == 0){
            	if(Request & REQUEST_STOP_MASK){        //поймали останов
                	TypeTormOut = load_s16(ramPnt, slotPnt->Pnt_TypeTorm);
                	if(TypeTormOut == FAIL_MODE_STOP){
                    	N_STOP	            = N_CTR;
                    	sPnt->DelayApvCnt	= load_s32(ramPnt, slotPnt->Pnt_Time);
                    	sPnt->Request	    = Request;
                    	sPnt->N_STOP	    = N_CTR;
                    	sPnt->N_PUSK	    = 0;
                    }
                }
            	else if(Request & REQUEST_PUSK_MASK){
                	N_PUSK = N_CTR;
                }
            }
        }
    }
	else{                    //ключи отключены
    	for(; N_CTR < kolvSlot; N_CTR++, slotPnt++){
        	TekFail = testBitMicro(ramPnt, slotPnt->Pnt_TekFail );
        	Request	= load_s16(ramPnt, slotPnt->Pnt_Request	);
        	if(TekFail){
            	setBit(FailBmTek, N_CTR);
            	if(tekStop_CTR != 0)	tekStop_CTR = N_CTR;
            }
        	if((Request & REQUEST_PUSK_MASK)&&(N_PUSK == 0)){
            	N_PUSK = N_CTR;
            }
        }
    	if(Man_Auto == 1){        //АПВ разрешены
        	if(IsSecondKvant()){                                            //считаем АПВ
            	slotPnt	= &progPnt->SlotFc[sPnt->N_STOP];
            	TekFail = testBitMicro(ramPnt, slotPnt->Pnt_TekFail );
            	if(!((sPnt->Request & REQUEST_NormWait_MASK) && TekFail)){    //не висит авария
                	if(sPnt->DelayApvCnt > 0)	sPnt->DelayApvCnt--;
                	else{
                    	N_PUSK = sPnt->N_STOP;
                    }
                    //сохранить в FRAM
                }
            }
        }
    }
	if((Man_Auto == 0)||(N_STOP != 0)){                    //АПВ запрещены
    	sPnt->DelayApvCnt	= 0;
    	sPnt->Request	    = REQUEST_OK;
    	sPnt->N_PUSK	    = 0;
    }
	else if(N_PUSK){                                    //АПВ разрешены
    	sPnt->DelayApvCnt	= 0;
    	sPnt->Request	    = REQUEST_PUSK_MASK;
    	sPnt->N_STOP	    = 0;
    	sPnt->N_PUSK	    = N_PUSK;
    }
	save_s16(ramPnt, progPnt->Pnt_Type_torm, TypeTormOut );
	save_s16(ramPnt, progPnt->Pnt_N_TekFail, tekStop_CTR );
	save_s16(ramPnt, progPnt->Pnt_N_PUSK,      sPnt->N_PUSK);
	save_s16(ramPnt, progPnt->Pnt_N_STOP,      sPnt->N_STOP);
	save_s16(ramPnt, progPnt->Pnt_MaskTek_0, FailBmTek[0]);
	save_s16(ramPnt, progPnt->Pnt_MaskTek_1, FailBmTek[1]);
	save_s16(ramPnt, progPnt->Pnt_MaskTek_2, FailBmTek[2]);
	save_s16(ramPnt, progPnt->Pnt_MaskTek_3, FailBmTek[3]);
    
//	save_s16(ramPnt, progPnt->Pnt_near_STOP, 0);
//	save_s16(ramPnt, progPnt->Pnt_near_PUSK, 0);
	save_s32(ramPnt, progPnt->Pnt_TimeDoOff, TimeDoOff);
	save_s32(ramPnt, progPnt->Pnt_TimeDoAPV, sPnt->DelayApvCnt);
};
//********************************************************
