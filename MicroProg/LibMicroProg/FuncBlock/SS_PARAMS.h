
/************************** (C) Copyright 2013 Triol ************************
* File Name          : SS_PARAMS.h
* Author             : Cheremskoy Igor
* TS Idea            : 2.07.13
* TS Version         : 3.07.13 15:18
* Description        : Deals with reference parameters menu (group #49)
*                      WARNING - refs do not work with TEXPO
****************************************************************************/

#include "commonmicro.h"
#include "loadvar.h"

#ifdef    GET_FUNC_CODE

// These parameters will never be displayed in ref menu
const u16 PAR_BLACK_LIST[] =
{
    //DoAdrPrm(49, 16),    // Default param
    DoAdrPrm(48,  7),     // User password
    DoAdrPrm(48,  9),     // Tech password
    DoAdrPrm(48, 11),      // Master password
};

// Stores address of the last changed value
extern u16 GlobalCurGrpPrm; // Declared in LibMicroProg/MakeOpis.c

typedef struct{
    u16  Pnt_Nfunc;
    //u16  Pnt_State;
    //u16  Pnt_nvState;
    
    u16  Pnt_Clear;                        //1  Clear
    u16  Pnt_Par16;                        //2  Par16
    u16  Pnt_Par15;                        //3  Par15
    u16  Pnt_Par14;                        //4  Par14
    u16  Pnt_Par13;                        //5  Par13
    u16  Pnt_Par12;                        //6  Par12
    u16  Pnt_Par11;                        //7  Par11
    u16  Pnt_Par10;                        //8  Par10
    u16  Pnt_Par9;                         //9  Par9
    u16  Pnt_Par8;                         //10  Par8
    u16  Pnt_Par7;                         //11  Par7
    u16  Pnt_Par6;                         //12  Par6
    u16  Pnt_Par5;                         //13  Par5
    u16  Pnt_Par4;                         //14  Par4
    u16  Pnt_Par3;                         //15  Par3
    u16  Pnt_Par2;                         //16  Par2
    u16  Pnt_Par1;                         //17  Par1
    
    u16  Pnt_End;
}FuncSS_PARAMS_type;

//********************************************************
void WriteS16(u16 GrpPrm, u16 Val)
{
    wm_type Wmem, *Wm;
    Wm = &Wmem;

    Wm->AdrPrm = GrpPrm;
    Wm->Val2byte = Val;

    //MakeOpis(Wm, 0);

    GetFlagsPrm(Wm);
    Wm->s32.Value = Val;
    WrFramMicro((void*)((Wm->AdrPrm & (u16)(~LINEadr)) * sizeof(u32)),&Wm->s32.Value, sizeof(s32));
}
/****************************************************************************
* Function Name  : ClearParams
* Description    : Sets all ref params to 49:16 value (CLEAR UST address)
* Input          : progPnt, ramPnt - taken from FuncPERIF_NVSA_1 function
* Output         : None
* Return         : None
****************************************************************************/
void ClearParams(FuncSS_PARAMS_type *progPnt, u32 ramPnt)
{
    u16 *ppp = &progPnt->Pnt_Par16;
    s32 i = 0;

    for(; i < NUM_SS_PARAMS; i++)
    {
        save_s16(ramPnt, *(ppp + i), NPAR_NGRP_CLEAR);
        WriteS16((u16)DoAdrPrm(SS_GRP, NUM_SS_PARAMS - i - 1),
                 NPAR_NGRP_CLEAR);
    }
}

/****************************************************************************
* Function Name  : CheckBlackList
* Description    : Checks whether param with such address exists in BLACK LIST or no
* Input          : Address (G:#) of the param we want to check  
* Output         : None
* Return         : 1 - when parameter exists in BLACK LIST (we do not want to add it)
                   0 - when there is no such param in BLACK LIST (we can add it to our ref menu)
****************************************************************************/
u16 CheckBlackList(u16 Par)
{
    u16 i = 0;

    for(; i < PAR_BLACK_LIST_LEN; i++)
    {
        if(Par == PAR_BLACK_LIST[i])
        {
            return 1; // This is bad param - do not add it
        }
    }

    if((Par >> 7) == SS_GRP)
    {
        return 1;
    }

    return 0; // Ok this param can be added
}

/****************************************************************************
* Function Name  : SearchForSimilar
* Description    : Checks whether param with such address already
                   exists in ref menu or no
* Input          : progPnt, ramPnt - taken from FuncPERIF_NVSA_1 function
                   Par - Address (G:#) of the param we want to check
* Output         : None
* Return         : 1 - when parameter exists in ref menu 
                   0 - when there is no such param in ref menu
****************************************************************************/
u16 SearchForSimilar(FuncSS_PARAMS_type *progPnt, u32 ramPnt, u16 Par)
{
	// Берем указатель на самый верхний измененный параметр
	u16 par1 = load_s16(ramPnt, progPnt->Pnt_Par1);

	// И проверяем только его
    if(par1 == Par)
    {
         return 1; // We already have such param
    }

    return 0; // This is new param - we can add it


/*  Было реализовано вот так
	u16 *ppp = &progPnt->Pnt_Par16;
    s32 i = 0;

    //for(; i < 16; i++)
    for(; i < 1; i++)
    {
        if(load_s16(ramPnt, *(ppp + i)) == Par)
        {
            return 1; // We already have such param
        }
    }

    return 0; // This is new param - we can add it
 */
}

/****************************************************************************
* Function Name  : ShiftParams
* Description    : Shifts all params in ref menu down by one
* Input          : progPnt, ramPnt - taken from FuncPERIF_NVSA_1 function
* Output         : None
* Return         : None
****************************************************************************/
void ShiftParams(FuncSS_PARAMS_type *progPnt, u32 ramPnt)
{
    u16 *ppp = &progPnt->Pnt_Par15;
    s32 i = 0;

    // Shift all parameters by one
    for(; i < 15; i++)// было i < 16
    {
        u16 tmp = load_s16(ramPnt, *(ppp + i));
        save_s16(ramPnt, *(ppp + i - 1), tmp);
        WriteS16((u16)DoAdrPrm(SS_GRP, NUM_SS_PARAMS - i - 1), tmp);
    }
}
//****************************************************************************
void ReinitQuickListP24U (void)
{
#ifdef _NVSA_
#else /** _NVSA_ */
#ifdef _PROJECT_FOR_CCS_
	wm_type *wm;
	wm = &wmemory;
	initQuickList(wm);                                                   /** Заполнение горячего списка */
	initStatusList(wm);
#endif

#endif  /** _NVSA_ */
}
//****************************************************************************

/****************************************************************************
* Function Name  : SaveNewParam
* Description    : Deals with adding new param to ref menu
* Input          : progPnt, ramPnt - taken from FuncPERIF_NVSA_1 function
                   Par - Address (G:#) of the param we want to add
* Output         : None
* Return         : None
****************************************************************************/
void SaveNewParam(FuncSS_PARAMS_type *progPnt, u32 ramPnt, u16 Par)
{

    /*// Do not add default param
    if(Par == NPAR_NGRP_CLEAR)
    {
        return;
    }*/

    // Check for bad params
    if(CheckBlackList(Par))
    {
        return;
    }

#ifdef _NVSA_
#else /** _NVSA_ */
#ifdef _PROJECT_FOR_CCS_
    wm_type *wm;
    wm = &wmemory;
    initStatusList(wm); //
#endif
#endif

    // Check for similar params
    if(SearchForSimilar(progPnt, ramPnt, Par))
    {
        return; // No need to add
    }

    // Shift values
    ShiftParams(progPnt, ramPnt);

    // Finally save param
    save_s16(ramPnt, progPnt->Pnt_Par1, GlobalCurGrpPrm);
    WriteS16(DoAdrPrm(SS_GRP, 0), GlobalCurGrpPrm);

#ifndef _NVSA_
    ReinitQuickListP24U ();
#endif
}
/****************************************************************************
* Function Name  : FuncSS_PARAMS_1
* Description    : Main function of the func block
* Input          : progPnt, ramPnt
* Output         : None
* Return         : &progPnt->Pnt_End
****************************************************************************/
u16 *FuncSS_PARAMS_1(FuncSS_PARAMS_type *progPnt, u32 ramPnt)
{
    // Check clear value
    if(load_s16(ramPnt, progPnt->Pnt_Clear))
    {
        ClearParams(progPnt, ramPnt);
        save_s16(ramPnt, progPnt->Pnt_Clear, 0); // Set back to NO

        // Clear global val
        GlobalCurGrpPrm = NPAR_NGRP_CLEAR;

        #ifndef _NVSA_
           ReinitQuickListP24U ();
        #endif
    }

    // Get & save new value
    SaveNewParam(progPnt, ramPnt, GlobalCurGrpPrm);

    // Clear global val
    GlobalCurGrpPrm = NPAR_NGRP_CLEAR;

    return &progPnt->Pnt_End;
};
#endif

//********************************************************

#ifdef    GET_FUNC_TABL_OPIS
const char TblFuncSS_PARAMS_1[17]={
        S16_PIN_TYPE | INPUT_PIN_MODE,    //1  Clear
        S16_PIN_TYPE | INPUT_PIN_MODE,    //2  Par16
        S16_PIN_TYPE | INPUT_PIN_MODE,    //3  Par15
        S16_PIN_TYPE | INPUT_PIN_MODE,    //4  Par14
        S16_PIN_TYPE | INPUT_PIN_MODE,    //5  Par13
        S16_PIN_TYPE | INPUT_PIN_MODE,    //6  Par12
        S16_PIN_TYPE | INPUT_PIN_MODE,    //7  Par11
        S16_PIN_TYPE | INPUT_PIN_MODE,    //8  Par10
        S16_PIN_TYPE | INPUT_PIN_MODE,    //9  Par9
        S16_PIN_TYPE | INPUT_PIN_MODE,    //10  Par8
        S16_PIN_TYPE | INPUT_PIN_MODE,    //11  Par7
        S16_PIN_TYPE | INPUT_PIN_MODE,    //12  Par6
        S16_PIN_TYPE | INPUT_PIN_MODE,    //13  Par5
        S16_PIN_TYPE | INPUT_PIN_MODE,    //14  Par4
        S16_PIN_TYPE | INPUT_PIN_MODE,    //15  Par3
        S16_PIN_TYPE | INPUT_PIN_MODE,    //16  Par2
        S16_PIN_TYPE | INPUT_PIN_MODE     //17  Par1
};
#endif

//********************************************************

#ifdef    GET_FUNC_OPIS
//SymbolName,  Prefix,  typePin, FuncPnt, SizeOfState, SizeOfNvState, ExeLogicBlock
OpisFuncBlock("SS_PARAMS", "DD", TblFuncSS_PARAMS_1, FuncSS_PARAMS_1, sizeof(TblFuncSS_PARAMS_1), 0, 0, 1),
#endif

//********************************************************

#ifdef    GET_FUNC_PNT
    (pFuncMicro_type)FuncSS_PARAMS_1,
#endif

/************************** (C) Copyright 2013 Triol ***********************/
