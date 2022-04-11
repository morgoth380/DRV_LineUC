#ifndef MakeOpis_H
#define MakeOpis_H

#include "commonmicro.h"

//********************************************************
typedef enum{NV_to_RAM=0, RAM_to_NV, PWR_to_RAM}UstCopy_enum;
//****************************************************************************
//Коды ошибок в приводе
typedef enum {
            Ok_err          ,       // 0
            IllegalFunc_err ,       // 1
            BadAdr_err      ,       // 2
            Lim_err         ,       // 3
            NonRdk_err      ,       // 4
            SysPrmInitBad_err,      // 5
            NotInCurFunctional_err, // 6
            }Drv_err_type;
//****************************************************************************
#define LINEadr             (1uL<<14)     // Признак линейного адреса параметра
#define EXT_OPIS_PRM_ADR    (1uL<<15)     // Признак расширенного описателя

//********************************************************
struct wmStruct;
typedef int (*pFuncMosAf_typedef)(struct wmStruct *,s16);       //указатель на функцию
//********************************************************
typedef union{                  
    struct {
        u32  TypeVar          : 5;
        u32  MasterVis        : 1;
        u32  MasterChange     : 1;
        u32  TexnVis          : 1;
        u32  TexnChange       : 1;
        u32  UserVis          : 1;
        u32  UserChange       : 1;
        u32  Power            : 3;
        u32  EditType         : 3;
        u32  nonVolatile      : 1;
        u32  LimChange        : 1;     
        u32  FormatChange     : 1;
        u32  NumBase          : 4;
        u32  NumFunc          : 4;
        u32  FieldType        : 1;
        u32  VisType          : 1;        
    } bit;
    u32     all;
} UstFlag_type;   

struct wmStruct{
                u32                 Flags;       //
                u16                 StepEdIsm;   //
                u16                 AdrPrm;      //
                union{      
                    struct{
                             s32    Value;
                             s32    LowLim;
                             s32    HighLim;
                            }s32;
                    struct{
                             float  Value;
                             float  LowLim;
                             float  HighLim;
                            }f;
                };   

                const OpisPrm_type  *oPnt;
                
                void                *valPnt;
                pFuncMosAf_typedef  mosaf;
                u16                 kolvShort;   //
                u16                 NumBase;
                u16                 Val2byte;
                u16                 TypeVar;
                u16                 LedName[4];   //имя для светодиодного пульта
                struct{
                    u16             enablePrmChange :1; // Флаг для блокировки записи параметров задания
                  };
                };
//********************************************************
typedef struct wmStruct wm_type;
//********************************************************
Drv_err_type ModbusMakeOpis(wm_type *wm, s16 Rd);
Drv_err_type GetFlagsPrm(wm_type *wm);
Drv_err_type MakeOpis(wm_type *wm, s16 Rd);
Drv_err_type MOspecial(wm_type *wm, s16 Rd);
void StartExtMem(void);
int Ust_Copy(UstCopy_enum Ust_src_to_dest);
u16 InitFlagsFunctional(void);

//********************************************************
//********************************************************
#endif
