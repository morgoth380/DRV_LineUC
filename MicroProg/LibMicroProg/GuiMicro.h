#ifndef GuiMicro_H
#define GuiMicro_H

#include    "type.h"

#if(CHAR_BIT == 8)

//********************************************************
    typedef struct{
            char    sem;
        }SEM_Obj;
//********************************************************

//********************************************************


#define SEM_postBinary(sem)



void PWM_OutEnable();
void PWM_OutDisable();
s16 checkErrKey(s16     *NumFailPnt);

//********************************************************
#ifdef  DEFINE_GuiMicro_GLOBAL
#define GuiMicro_GLOBAL                 
#else   
#define GuiMicro_GLOBAL extern
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/


// разобраться?    GuiMicro_GLOBAL     SEM_Obj     SEM_uart, SEM_20mS;
    

#endif  //CHAR_BIT
#endif  //GuiMicro_H
