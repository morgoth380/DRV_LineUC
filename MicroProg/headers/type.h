#ifndef type_H
#define type_H

#include <limits.h>
//****************************************************************************
#ifndef NULL
#define	NULL	((void *)0)
#endif

#define absMacro(val)  ( (val > 0)    ? val : (-val))
//****************************************************************************
typedef signed int		INT;
typedef unsigned int	UINT;
//typedef signed char		CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;
typedef signed short	SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef signed long		LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;
//typedef enum { FALSE = 0, TRUE } bol;

#ifdef _PROJECT_FOR_CCS_
    #ifndef bool 
        #define bool Bool 

        #ifndef true
        #define true ((Bool)0)
        #endif
        
        #ifndef false
        #define false ((Bool)1)
        #endif
    #endif 
#else
    
#endif
//****************************************************************************
//****************************************************************************
  
  typedef long double              f64;
  typedef float                    f32;

  typedef unsigned long long       u64;
  typedef unsigned long            u32;
  typedef unsigned short           u16;
  typedef unsigned char            u8;

  typedef signed long long         s64;
  typedef signed long              s32;
  typedef signed short             s16;
  typedef signed char              s8;

  typedef volatile unsigned long   vu32;
  typedef volatile unsigned short  vu16;
  typedef volatile unsigned char   vu8;

  typedef volatile signed long     vs32;
  typedef volatile signed short    vs16;
  typedef volatile signed char     vs8;





// PAN typedef enum { RESET = 0, SET   = !RESET } FlagStatus, ITStatus;

// PAN typedef enum { DISABLE = 0, ENABLE  = !DISABLE} FunctionalState;


//****************************************************************************
typedef void (*pFuncVoid_typedef)(void);        //указатель на функцию
typedef int (*piFuncVoid_typedef)(void);        //указатель на функцию
typedef int (*intFuncVoid_typedef)(void);       //указатель на функцию
typedef	void *(*ppvFuncVoid_typedef)(void);     //указатель на функц. которая возвращает указатель на функ

struct moStruct;
typedef int (*intFuncMoS16_typedef)(struct moStruct *,s16);        //указатель на функцию

typedef void (*pFuncInt_typedef)(int);        //указатель на функцию
//****************************************************************************
#define	GetOffset(Pnt1, Pnt2)        ((u32)(Pnt1) - (u32)(Pnt2))
#define	Align_4b(Adr)        ((void *)((((unsigned int)Adr)+3)&(~3)))
#define	IntMax(val1, val2)    ((val1 > val2) ? val1 : val2)
#define	IntMin(val1, val2)    ((val1 < val2) ? val1 : val2)
//**************************************************************************
 typedef union	{
                    	s32	    	i;
                    	s16	    	s[2];
        }filtr_type;
//**************************************************************************
 typedef union	{
                    	s64	    	lli;
                    	s32	    	i[2];
        }LL_filtr_type;
//**************************************************************************
#define	Pi	        	3.1415926535897932384626433832795



#endif
