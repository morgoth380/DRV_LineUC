#ifdef    GET_FUNC_CODE

typedef struct{
    s16         AddState;
    s16         CntState;
}StateFuncUP_DOWN_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Up;                           //1  Up
    u16  Pnt_Dn;                           //2  Dn
    u16  Pnt_Temp;                         //3  Temp
    u16  Pnt_In;                           //4  In
    u16  Pnt_MaxOut;                       //5  MaxOut
    u16  Pnt_MinOut;                       //6  MinOut
    u16  Pnt_Out;                          //7  Out
    u16  Pnt_End;
}FuncUP_DOWN_type;
//********************************************************
u16 *FuncUP_DOWN_1(FuncUP_DOWN_type *progPnt, u32 ramPnt)
{
    s16                     Add, In, Out, Up, Dn, Max, Min;
    StateFuncUP_DOWN_type   *sPnt;
    
    sPnt = (StateFuncUP_DOWN_type *)(ramPnt + progPnt->Pnt_State/SZ_DIV);
    
    Up  = testBitMicro(ramPnt, progPnt->Pnt_Up);
    Dn  = testBitMicro(ramPnt, progPnt->Pnt_Dn);
    Max = load_s16(ramPnt, progPnt->Pnt_MaxOut);
    Min = load_s16(ramPnt, progPnt->Pnt_MinOut);
    In  = load_s16(ramPnt, progPnt->Pnt_In);
    Add = sPnt->AddState;
    //Проверяю корректность задания диапазона
    if(Max > Min){
        //Поступил сигнал меньше
        if(Dn == 1){
            //Это счетчик , формирует временной интервал 1 с
            if(sPnt->CntState){
                if((sPnt->CntState++) >= 249){
                    sPnt->CntState = 0;
                }
            }
            else{
                //Формирую добавку ко входному сигналу
                Add -= load_s16(ramPnt, progPnt->Pnt_Temp);
                sPnt->CntState++;
                }
        }//Поступил сигнал больше
        else{
            if(Up == 1){
                if(sPnt->CntState){
                    if((sPnt->CntState++) >= 249){
                        sPnt->CntState = 0;
                    }
                }
                else{
                    Add += load_s16(ramPnt, progPnt->Pnt_Temp);
                    sPnt->CntState++;
                }
             }
             else{
                //Если обе кнопки отпущены - сбрасываю счетчик
                sPnt->CntState = 0;
             }
        }
        //Формирую на выход сумму входа и временной переменной (накопитель)
        Out = In + Add;
        //Если выход в заданом диапазоне, сохраняю накопитель,
        //иначе ограничеваю его, что бы избежать насыщения накопителя
        if((Out <= Max) && (Out >= Min)){
            sPnt->AddState = Add;
        }
        else{
            if(Out > Max){
                Out = Max;
            }
            if(Out < Min){
                Out = Min;
            }
            sPnt->AddState = Out - In;
        }
        //Ограничеваю выходное значение
        if(Out > Max){
            Out = Max;
        }
        if(Out < Min){
            Out = Min;
        } 
    }
    else{
        Out = In;
    }
    save_s16(ramPnt, progPnt->Pnt_Out, Out);
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncUP_DOWN_1[7]={
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //1  Up
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //2  Dn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //3  Temp
        S16_PIN_TYPE | INPUT_PIN_MODE,     //4  In
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  MaxIn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  MinIn
        S16_PIN_TYPE | OUTPUT_PIN_MODE     //7  Out
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("UP_DOWN", "DA", TblFuncUP_DOWN_1, FuncUP_DOWN_1, sizeof(TblFuncUP_DOWN_1), sizeof(StateFuncUP_DOWN_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncUP_DOWN_1,
#endif
//********************************************************
