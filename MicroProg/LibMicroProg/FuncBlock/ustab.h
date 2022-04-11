#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Uin;                          //1  Uin
    u16  Pnt_Iout;                         //2  Iout
    u16  Pnt_Kstab;                        //3  Kstab
    u16  Pnt_Fstab;                        //4  Fstab
    u16  Pnt_Lf;                           //5  Lf
    u16  Pnt_on_off;                       //6  on_off
    u16  Pnt_Uf;                           //7  Uf
    u16  Pnt_Uout;                         //8  Uout
    u16  Pnt_End;
}FuncUSTAB_type;
//********************************************************
u16 *FuncUSTAB_1(FuncUSTAB_type *progPnt, u32 ramPnt)
{
//Изменил загрузку Pnt_Fstab. Надо добавить пересчёт через базу частоты
	save_s32(ramPnt, progPnt->Pnt_Uout, load_s32(ramPnt, progPnt->Pnt_Uin));
	
/*
	f32 Uf_temp;
	_iq Uf,I_tmp;
	if (load_s16(ramPnt, progPnt->Pnt_on_off))
	{
		//корректируем
		Uf_temp = 2 * PI * load_s16(ramPnt, progPnt->Pnt_Kstab) * load_s16(ramPnt, progPnt->Pnt_Fstab) * load_s16(ramPnt, progPnt->Pnt_Lf) / 100000000; // мк = /1000000, kstab = /100
		//Uf_temp /= moPnt->Base[NUM_VOLT_BASE];
		Uf_temp /= MPOpis.Base[NUM_VOLT_BASE];
		I_tmp = load_s32(ramPnt, progPnt->Pnt_Iout);
		//Uf = _IQ(Uf_temp * _IQtoF(I_tmp) * moPnt->Base[NUM_CUR_BASE] / 10);
		Uf = _IQ(Uf_temp * _IQtoF(I_tmp) * MPOpis.Base[NUM_CUR_BASE] / 10);
		save_s32(ramPnt, progPnt->Pnt_Uf, Uf);
		save_s32(ramPnt, progPnt->Pnt_Uout, (load_s32(ramPnt, progPnt->Pnt_Uin)+Uf));
	}
	else
	{
		//транзит
		save_s32(ramPnt, progPnt->Pnt_Uout, load_s32(ramPnt, progPnt->Pnt_Uin));
		save_s32(ramPnt, progPnt->Pnt_Uf, 0);
	}
*/
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUSTAB_1[8]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Uin
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Iout
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Kstab
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Fstab
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  Lf
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  on_off
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //7  Uf
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //8  Uout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("USTAB", "U", TblFuncUSTAB_1, FuncUSTAB_1, sizeof(TblFuncUSTAB_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUSTAB_1,
#endif
//********************************************************
