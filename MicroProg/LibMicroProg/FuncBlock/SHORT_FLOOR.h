#ifdef    GET_FUNC_CODE

#define NONE        0
#define MODE_UP     1
#define MODE_FIX    2
#define MODE_DOWN   3
#define MODE_STOP   4


typedef struct{
    f32     SqNormalMode;               //Площадь в нормальном режиме
    f32     Kdown;                      //тангенс угла при торможении
    f32     F0;
    f32     Fnom;
    f32     Stmp;
    //f32     Falign;
    //f32     tDownIn;
    u32     time;
    u16     modeFloor   :2;
}SHORT_FLOOR_State_type;

typedef struct{
    u16  Pnt_Nfunc;
    u16  Pnt_State;
    //u16  Pnt_nvState;
    u16  Pnt_Falign;                       //1  Falign
    u16  Pnt_Fcur;                         //2  Fcur
    u16  Pnt_Fnom;                         //3  Fnom
    u16  Pnt_Fmax;                         //4  Fmax
    u16  Pnt_TmScl;                        //5  Pnt_Fmax
    u16  Pnt_tDownIn;                      //6  tDownIn
    u16  Pnt_tPlato;                       //7  tPlato
    u16  Pnt_Align;                        //8  Align
    u16  Pnt_PWM;                          //9  PWM
    u16  Pnt_E;                            //10  E
    u16  Pnt_Logic;                        //11 Logic
    u16  Pnt_Fout;                         //12 Fout
    u16  Pnt_End;
}FuncSHORT_FLOOR_type;
//********************************************************
u16 *FuncSHORT_FLOOR_1(FuncSHORT_FLOOR_type *progPnt, u32 ramPnt)
{
    f32     S1, S2, S3, sum, delta, time, Fcur, Falign, kTime, tDownIn, Fmax, tPlato;
    _iq     FcurIQ, FnomIQ, FalignIQ, FmaxIQ;
    u16     En, timeScale, i;
    u16     Align;
    u16     PwmIn;
    SHORT_FLOOR_State_type  *sPnt;

    sPnt = (SHORT_FLOOR_State_type *)(ramPnt + progPnt->Pnt_State);

    En      = testBitMicro(ramPnt, progPnt->Pnt_E);
    Align   = testBitMicro(ramPnt, progPnt->Pnt_Align);
    PwmIn   = testBitMicro(ramPnt, progPnt->Pnt_PWM);

    

    if (En == 0) {
        clrBitMicro(ramPnt, progPnt->Pnt_Logic);
    }
    else{
       if (PwmIn && Align) {
           FcurIQ = _IQabs(load_s32(ramPnt, progPnt->Pnt_Fcur));
           FnomIQ = _IQabs(load_s32(ramPnt, progPnt->Pnt_Fnom));
           FmaxIQ = load_s32(ramPnt, progPnt->Pnt_Fmax);
           Fmax = _IQtoF(FmaxIQ);
           FalignIQ = _IQabs(load_s32(ramPnt, progPnt->Pnt_Falign));
           Fcur = _IQtoF(FcurIQ);
           Falign = _IQtoF(FalignIQ);
           tPlato = (f32)(load_s16(ramPnt, progPnt->Pnt_tPlato));

           timeScale = load_s16(ramPnt, progPnt->Pnt_TmScl);
           kTime = 1;
           for (i = 0; i < timeScale; i++) {
               kTime *= 10.; 
           }

           if ((FcurIQ >= (FnomIQ - 1) || FcurIQ < FalignIQ) && sPnt->modeFloor == NONE) {
               clrBitMicro(ramPnt, progPnt->Pnt_Logic);
               memset(sPnt, 0, sizeof(SHORT_FLOOR_State_type));
           }
           else{
               if (sPnt->modeFloor == NONE) {
                   sPnt->modeFloor     = MODE_UP;
                   sPnt->Fnom          = _IQtoF(FnomIQ);
                   sPnt->F0            = Fcur;
                   tDownIn             = (f32)load_s16(ramPnt, progPnt->Pnt_tDownIn) / kTime;///(f32)10;
                   //sPnt->Kdown         = (sPnt->Fnom - Falign) / tDownIn;
                   sPnt->Kdown         = Fmax / tDownIn;
                   delta = (sPnt->Fnom - Falign);
                   sPnt->SqNormalMode  = ((delta * delta) / sPnt->Kdown) / 2;
                   save_s32(ramPnt, progPnt->Pnt_Fout, FnomIQ);
                   setBitMicro(ramPnt, progPnt->Pnt_Logic);
               }
               else {
                   sPnt->time++;
                   time = (f32)(sPnt->time * INT_TIME_CALL_MICRO_mS / 1000.);
                   if (sPnt->modeFloor == MODE_UP) {
                       delta = Fcur - sPnt->F0;
                       S1 = ((delta) * time) / 2;                       //Площадь треугольника

                       delta = sPnt->F0 - Falign;
                       S1 += time * delta;                 //Площадь прямоугольника под треугольником с тругольником
                       
                       delta = Fcur - Falign;
                       S2 = delta * tPlato;                             //Плато
                       
                       S3 = delta * delta / (sPnt->Kdown * 2);
                       sum = S1 + S2 + S3;
                       if (sum > sPnt->SqNormalMode){
                           sPnt->Stmp = S1;
                           sPnt->F0 = Fcur;
                           sPnt->time = 0;
                           sPnt->modeFloor = MODE_FIX;
                           save_s32(ramPnt, progPnt->Pnt_Fout, FcurIQ);
                       }
                   }
                   else if (sPnt->modeFloor == MODE_FIX) {
                       delta = Fcur - Falign;
                       S2 = delta * time;                              //Плато
                       S3 = delta * delta / (sPnt->Kdown * 2);
                       sum = sPnt->Stmp + S2 + S3;
                       if (sum > sPnt->SqNormalMode){
                           sPnt->Stmp += S2;
                           sPnt->F0 = Fcur;
                           sPnt->time = 0;
                           sPnt->modeFloor = MODE_DOWN;
                           clrBitMicro(ramPnt, progPnt->Pnt_Logic);
                       }
                   }               
                   else if (sPnt->modeFloor == MODE_DOWN){
                       clrBitMicro(ramPnt, progPnt->Pnt_Logic);
                   }
               }
           }
       }
       else{
            memset(sPnt, 0, sizeof(SHORT_FLOOR_State_type));
            clrBitMicro(ramPnt, progPnt->Pnt_Logic);
        }
    }
    return &progPnt->Pnt_End;
};
#endif
//********************************************************
#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSHORT_FLOOR_1[12]={
        S32_PIN_TYPE | INPUT_PIN_MODE,     //1  Falign
        S32_PIN_TYPE | INPUT_PIN_MODE,     //2  Fcur
        S32_PIN_TYPE | INPUT_PIN_MODE,     //3  Fnom
        S32_PIN_TYPE | INPUT_PIN_MODE,     //4  Fmax
        S16_PIN_TYPE | INPUT_PIN_MODE,     //5  timeScale
        S16_PIN_TYPE | INPUT_PIN_MODE,     //6  tDownIn
        S16_PIN_TYPE | INPUT_PIN_MODE,     //7  tPlato
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //8  Align
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //9  PWM
        BIT_PIN_TYPE | INPUT_PIN_MODE,     //10  E
        BIT_PIN_TYPE | OUTPUT_PIN_MODE,    //11 Logic
        S32_PIN_TYPE | OUTPUT_PIN_MODE     //12 Fout
};
#endif
//********************************************************
#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SHORT_FLOOR", "DD", TblFuncSHORT_FLOOR_1, FuncSHORT_FLOOR_1, sizeof(TblFuncSHORT_FLOOR_1), sizeof(SHORT_FLOOR_State_type), 0, 1),
#endif
//********************************************************
#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSHORT_FLOOR_1,
#endif
//********************************************************
