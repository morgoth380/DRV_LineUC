#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Din[8];                         //1-8  Din1
    u16  Pnt_chPrior;                        //9  chPrior
    u16  Pnt_Din_out[8];                     //10-17  Din8_out
    u16  Pnt_End;
}FuncPRIORITY_type;
//********************************************************
u16 *FuncPRIORITY_1(FuncPRIORITY_type *progPnt, u32 ramPnt)
{
    u16 i = 0, k, f = 0;
    s16 tmp;

    tmp = load_s16(ramPnt, progPnt->Pnt_chPrior);
    if (tmp == 0){
        while(i < 8){
            defBitMicro(ramPnt, progPnt->Pnt_Din_out[7 - i], testBitMicro(ramPnt, progPnt->Pnt_Din[i]));
            i++;
        }
    }else{
        while(i < 8){
            if (tmp == 1){
                k = 7 - i;
            }else{
                if (tmp == 2){
                    k = i;
                }
            }
            if (testBitMicro(ramPnt, progPnt->Pnt_Din[k]) && (f == 0)){
                setBitMicro(ramPnt, progPnt->Pnt_Din_out[7 - k]);
                f = 1;
            }else{
                clrBitMicro(ramPnt, progPnt->Pnt_Din_out[7 - k]);
            }
            i++;
        }
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncPRIORITY_1[17]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Din1
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Din2
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //3  Din3
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //4  Din4
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //5  Din5
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //6  Din6
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //7  Din7
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Din8
        S16_PIN_TYPE | INPUT_PIN_MODE,     //9  chPrior
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //10  Din8_out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11  Din7_out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //12  Din6_out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //13  Din5_out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //14  Din4_out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //15  Din3_out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //16  Din2_out
        BIT_PIN_TYPE | OUTPUT_PIN_MODE     //17  Din1_out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("PRIORITY", "DA", TblFuncPRIORITY_1, FuncPRIORITY_1, sizeof(TblFuncPRIORITY_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncPRIORITY_1,
#endif
//********************************************************
