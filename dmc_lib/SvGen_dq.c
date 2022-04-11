#include "Global_include.h"
#include "SvGen_dq.h"


#include "svgen_dq.h"

void SvGenDqCalc(register SVGENDQ *v){   
 float Va,Vb,Vc,t1,t2;
 u16 Sector = 0;  
 
  
  // 1) Обратное преобразование Кларка (для опредиления текущего сектора)
    Va = v->Ubeta;
    Vb = (-0.5f * v->Ubeta) + (0.8660254f * v->Ualpha);  // 0.8660254 = sqrt(3)/2 
    Vc = (-0.5f * v->Ubeta) - (0.8660254f * v->Ualpha);    // 0.8660254 = sqrt(3)/2

  // 2) Разбить период на сектора по 60 град
    if(Va > 0){
       Sector = 1;
    }
    if(Vb > 0){ 
       Sector = Sector + 2;
    }   
    if(Vc > 0){   
       Sector = Sector + 4;
    }

    // 3) Обратное преобразование Кларка
    Va = v->Ubeta;
    Vb = (0.5 * v->Ubeta) + (0.8660254f * v->Ualpha);  // 0.8660254 = sqrt(3)/2 
    Vc = (0.5 * v->Ubeta) - (0.8660254f * v->Ualpha);    // 0.8660254 = sqrt(3)/2
  
    // 4) Расчитать настройки для таймера в зависемости от сектора
    if(Sector == 0)  // Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)
    {
       v->Ta = 0.5f;
       v->Tb = 0.5f;
       v->Tc = 0.5f;
    }
    if(Sector == 1){  // Sector 1: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)
       t1 = Vc;
       t2 = Vb;
       v->Tb = 0.5f * (1.0f - t1 - t2);       // tbon = (1-t1-t2)/2
       v->Ta = v->Tb + t1;                  // taon = tbon+t1
       v->Tc = v->Ta + t2;                  // tcon = taon+t2
    }
    else if(Sector == 2){  // Sector 2: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)
       t1 = Vb;
       t2 = -Va;
       v->Ta = 0.5f * (1.0f - t1 - t2);       // taon = (1-t1-t2)/2
       v->Tc = v->Ta + t1;                  // tcon = taon+t1
       v->Tb = v->Tc + t2;                  // tbon = tcon+t2
    }      
    else if(Sector == 3){  // Sector 3: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)
       t1 = -Vc;
       t2 = Va;
       v->Ta = 0.5f * (1.0f - t1 - t2);      // taon = (1-t1-t2)/2
       v->Tb = v->Ta + t1;                 // tbon = taon+t1
       v->Tc = v->Tb + t2;                 // tcon = tbon+t2
    }   
    else if(Sector == 4){  // Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)
       t1 = -Va;
       t2 = Vc;
       v->Tc = 0.5f * (1.0f - t1 - t2);      // tcon = (1-t1-t2)/2
       v->Tb = v->Tc + t1;                 // tbon = tcon+t1
       v->Ta = v->Tb + t2;                 // taon = tbon+t2
    }   
    else if(Sector == 5){  // Sector 5: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)
       t1 = Va;
       t2 = -Vb;
       v->Tb = 0.5f * (1.0f - t1 - t2);      // tbon = (1-t1-t2)/2
       v->Tc = v->Tb + t1;                  // tcon = tbon+t1
       v->Ta = v->Tc + t2;                  // taon = tcon+t2
    }   
    else if(Sector == 6){  // Sector 6: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)
       t1 = -Vb;
       t2 = -Vc;
       v->Tc = 0.5f * (1.0f - t1 - t2);      // tcon = (1-t1-t2)/2
       v->Ta = v->Tc + t1;                  // taon = tcon+t1
       v->Tb = v->Ta + t2;                  // tbon = taon+t2 
    }
  
    // 5) Преобразовать диапазон (0...1) в диапазон (-1...1)
    v->Ta = 2.0f * (v->Ta - 0.5f);
    v->Tb = 2.0f * (v->Tb - 0.5f);
    v->Tc = 2.0f * (v->Tc - 0.5f);        
}
//==============================================================================
void DPMW60_calc(register SVGENDQ *v)
{
	float Va,Vb,Vc;
	u16 Sector = 0;  // Sector is treated as Q0 - independently with global Q

	Va =  1.1547f * v->Ualpha; 	// 1.1547 = 2/sqrt(3)
	Vb =  (-0.57735f) * v->Ualpha + v->Ubeta;  // 0.57735 = 1/sqrt(3)
	Vc =  (-0.57735f) * v->Ualpha - v->Ubeta;  // 0.57735 = 1/sqrt(3)

//	%------------------------------------------
//	% 60 degree Sector determination
	if (Va > 0)
		Sector = 1;
	if (Vb > 0)
		Sector = Sector + 2;
	if (Vc > 0)
		Sector = Sector + 4;
//	%------------------------------------------

	switch(Sector){
	case 1:
		v->Ta = 1.0f;
		v->Tb = v->Ta + (0.5f * (Vb - Va));
		v->Tc = v->Ta + (0.5f * (Vc - Va));
		break;
	case 6:
		v->Ta = 0;
		v->Tb = v->Ta + (0.5f * (Vb - Va));
		v->Tc = v->Ta + (0.5f * (Vc - Va));
		break;
	case 2:
		v->Tb = 1.0f;
		v->Ta = v->Tb + (0.5f * (Va - Vb));
		v->Tc = v->Tb + (0.5f * (Vc - Vb));
		break;
	case 5:
		v->Tb = 0;
		v->Ta = v->Tb + (0.5f * (Va - Vb));
		v->Tc = v->Tb + (0.5f * (Vc - Vb));
		break;
	case 4:
		v->Tc = 1.0f;
		v->Ta = v->Tc + (0.5f * (Va - Vc));
		v->Tb = v->Tc + (0.5f * (Vb - Vc));
		break;
	case 3:
		v->Tc = 0;
		v->Ta = v->Tc +  (0.5f * (Va - Vc));
		v->Tb = v->Tc +  (0.5f * (Vb - Vc));
		break;
	}

	v->Ta = 2.0f * (v->Ta - 0.5f);
	if(v->Ta >= 1.0f )		v->Ta = 1.0f;
	if(v->Ta < -1.0f )		v->Ta = -1.0f;

	v->Tb = 2.0f * (v->Tb - 0.5f);
	if(v->Tb >= 1.0f)		v->Tb = 1.0f;
	if(v->Tb < -1.0f)		v->Tb = -1.0f;

	v->Tc = 2.0f * (v->Tc - 0.5f);
	if(v->Tc >= 1.0f)		v->Tc = 1.0f;
	if(v->Tc < -1.0f)		v->Tc = -1.0f;

        // Дополнительная инверсия сигналов, для стыковки с ШИМом векторным.
        v->Ta = -v->Ta;
        v->Tc = -v->Tc;
        v->Tb = -v->Tb;        
                
        v->Sector = (u16)Sector;
        
        v->Ta_60 = v->Ta;
        v->Tc_60 = v->Tc;
        v->Tb_60 = v->Tb;        
}

