#ifdef    GET_FUNC_CODE

#define BIT_SIZE_FAIL_MASK 16

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Mask[4];
/*
    u16  Pnt_Mask_1;                       //1  Mask_1
    u16  Pnt_Mask_2;                       //2  Mask_2
    u16  Pnt_Mask_3;                       //3  Mask_3
    u16  Pnt_Mask_4;                       //4  Mask_4
*/
    u16  Pnt_End;
}FuncFAIL_MASK_type;
//********************************************************
u16 *FuncFAIL_MASK_1(FuncFAIL_MASK_type *progPnt, u32 ramPnt)
{
    COMMONPARAMM3 *mPnt3 = &GlobalM3;
    u16 FailMask, i, j; //NumFail,
/*
    NumFail = 0;
    for(i=0, j = NUMFirstFailMicro; i<4; i++, j += 16){
        FailMask = load_s16(ramPnt, progPnt->Pnt_Mask[i]);
        for( ; FailMask !=0; j++ ){
            if( FailMask & 0x0001 ){                // Поиск первой аварии по флагу
                NumFail = j; 
                goto endSearch;                     // Номер найден прекратить поиск
             }
            FailMask >>= 1;
        }
    }
endSearch:
    mPnt3->NumSlowFail   = NumFail;
*/
    for(i=0, j = NUMFirstFailMicro; i<4; i++, j += BIT_SIZE_FAIL_MASK){
        FailMask = load_s16(ramPnt, progPnt->Pnt_Mask[i]);
        if (FailMask != 0){
            while((FailMask & 0x01) == 0){
                FailMask >>= 1;
                j++;
            }
        mPnt3->NumSlowFail = j;
        return &progPnt->Pnt_End;
        }
    }
    mPnt3->NumSlowFail = 0;
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncFAIL_MASK_1[4]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  Mask_1
        S16_PIN_TYPE | INPUT_PIN_MODE,     //2  Mask_2
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Mask_3
        S16_PIN_TYPE | INPUT_PIN_MODE      //4  Mask_4
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlockNew("FAIL_MASK", "DD", TblFuncFAIL_MASK_1, FuncFAIL_MASK_1, sizeof(TblFuncFAIL_MASK_1), 0, 0, 1, sizeof(FuncFAIL_MASK_type)),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncFAIL_MASK_1,
#endif
//********************************************************
