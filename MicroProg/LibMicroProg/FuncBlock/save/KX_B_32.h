#ifdef    GET_FUNC_CODE
typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_Num;
    u16  Pnt_Polar;
    u16  Pnt_Out;                      
    u16  Pnt_Xin[32][5];
    u16  Pnt_End;
}FuncMUX32_type;
//********************************************************
u16 *FuncKX_B_32_1(FuncMUX32_type *progPnt, u32 ramPnt)
{
    s16         Num, polar;
    s32         data;
    
    Num  = load_s16(ramPnt, progPnt->Pnt_Num)&0xFF;
        if (Num == 0){
            save_s32(ramPnt, progPnt->Pnt_Out, 0);
            return &progPnt->Pnt_End;
        }
        polar  = load_s16(ramPnt, progPnt->Pnt_Polar)&0x1;
//        if (Num > 0)
        Num -= 1;
        data = iq_Fy_x1x2y1y2x(load_s32(ramPnt, progPnt->Pnt_Xin[Num][1]), load_s32(ramPnt, progPnt->Pnt_Xin[Num][2]), load_s32(ramPnt, progPnt->Pnt_Xin[Num][3]), load_s32(ramPnt, progPnt->Pnt_Xin[Num][4]), load_s32(ramPnt, progPnt->Pnt_Xin[Num][0]));
        if (polar == 1) {
            data = (data + _IQ(1))/2;
        }else{
            if (load_s32(ramPnt, progPnt->Pnt_Xin[Num][0]) < 0)
                data = 0;
        }
    save_s32(ramPnt, progPnt->Pnt_Out, data);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncKX_B_32_1[163]={
        S16_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S16_PIN_TYPE | INPUT_PIN_MODE,
        S32_PIN_TYPE | OUTPUT_PIN_MODE,    //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In10
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In11
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In12
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In15
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In16
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In17
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In18
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In19
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In20
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In21
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In22
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In23
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In24
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In25
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In26
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In27
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In28
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In29
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In30
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In31
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Num
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  Out
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In10
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In11
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In12
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In15
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In16
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In17
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In18
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In19
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In20
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In21
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In22
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In23
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In24
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In25
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In26
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In27
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In28
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In29
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In30
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In31
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Num
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  Out
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In10
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In11
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In12
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In15
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In16
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In17
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In18
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In19
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In20
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In21
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In22
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In23
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In24
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In25
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In26
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In27
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In28
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In29
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In30
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In31
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Num
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  Out
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In10
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In11
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In12
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In15
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In16
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In17
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In18
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In19
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In20
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In21
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In22
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In23
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In24
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In25
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In26
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In27
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In28
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In29
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In30
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In31
        S32_PIN_TYPE | INPUT_PIN_MODE,     //17  Num
        S32_PIN_TYPE | INPUT_PIN_MODE,     //18  Out
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In0
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In1
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In2
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In3
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In4
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In5
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In6
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In7
        S32_PIN_TYPE | INPUT_PIN_MODE,     //9  In8
        S32_PIN_TYPE | INPUT_PIN_MODE,     //10  In9
        S32_PIN_TYPE | INPUT_PIN_MODE,     //11  In10
        S32_PIN_TYPE | INPUT_PIN_MODE,     //12  In11
        S32_PIN_TYPE | INPUT_PIN_MODE,     //13  In12
        S32_PIN_TYPE | INPUT_PIN_MODE,     //14  In13
        S32_PIN_TYPE | INPUT_PIN_MODE,     //15  In14
        S32_PIN_TYPE | INPUT_PIN_MODE,     //16  In15
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  In16
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  In17
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  In18
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  In19
        S32_PIN_TYPE | INPUT_PIN_MODE,     //5  In20
        S32_PIN_TYPE | INPUT_PIN_MODE,     //6  In21
        S32_PIN_TYPE | INPUT_PIN_MODE,     //7  In22
        S32_PIN_TYPE | INPUT_PIN_MODE,     //8  In23
        S32_PIN_TYPE | INPUT_PIN_MODE,
        S32_PIN_TYPE | INPUT_PIN_MODE     //9  In24
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, sizeofState, sizeofNVState, ExeLogicBlock
OpisFuncBlock("KX+B_32", "DA", TblFuncKX_B_32_1, FuncKX_B_32_1, sizeof(TblFuncKX_B_32_1), 0, 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncKX_B_32_1,
#endif
