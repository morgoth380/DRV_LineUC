/*=====================================================================================
 File name:        SPEED_LIM.C  (float version)                  
                    
 Originator:    Triol

 Description:  Speed limiter              

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 28-02-2011 Version 1.00
 02-10-2020 Version 2.00  float (PAN)  
-------------------------------------------------------------------------------------*/
#include "Global_include.h"
#include "speed_lim.h"

void speed_lim_calc(SPEEDLIM * v)
{
    float Wr_abs  = fabsf(v->WrElect);
    float Wrz_abs = fabsf(v->Wrz);

    if(Wr_abs >  v->WrL ){
       //ПЛАВНОЕ ЛИНЕЙНОЕ СНИЖЕНИЕ МОМЕНТА ДО 0
       if (Wr_abs > v->WrH){
           v->Mez = 0;
       }
       else{
           if((Wr_abs > v->WrL) && (Wr_abs <= v->WrH)){
               v->Mez = v->K1 * v->Mez * (v->WrH - Wr_abs);
           }
       }
    }

    // Управление направлением вращения, через знак скорости, а значит через параметр направление вращения
    if(v->Wrz < 0.0f ){
    	v->Mez = -v->Mez;
    }

    //Приведение заданиЯ по моменту к заданию по току
    if(v->IRm != 0.0f){
        v->Isqz = (v->Mez * v->K2) / v->IRm;
    }
    if(fabsf(v->Isqz) > v->OutMax){
        if (v->Isqz > 0.0f){
            v->Isqz = v->OutMax;
        }
        else{
            v->Isqz = -v->OutMax;
        }
    }
}

