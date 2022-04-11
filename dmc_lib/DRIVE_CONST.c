/* =================================================================================
File name:   ACI_DRIVE_CONST.C                   
                     
Originator:  Triol 

Description: Расчет параметров, для ВУ вентильным двигателем

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 25-12-2010 Version 1.00
 26-01-2012 Version 2.00: Удалены не нужные расчеты, наведена красота,
                          изменено название файла (ранее drive_param.c)
 22-03-2012 Version 3.00: Отредактирована обменная структура
 23-09-2020 Version 4.00: PAN (Перевел во float)
------------------------------------------------------------------------------*/
#include "DRIVE_CONST.h"
#include "Pjk_const.h"
#include "LIB_GLOBAL_DEFINE.h"


void DriveInit(DRIVECONST *v)
{
    f32 Tmp1;
    f32 Ixx;

    // расчет периода ШИМ
    v->Tpwm = 1.0f / v->Fpwm;

    // расчет прироста угла выходного поля
    v->StepAngle = /*v->FreqBASE * */ v->Tpwm; // Частоты для скаляра идут в Гц, так что база не нужна

    // расчет базы потокосцепления
    v->FluxBASE = v->Lm * v->CurrBASE;    
    
    // расчет количества полюсов
    v->Pol = v->Zp * 2;

    // Расчет индуктивностей статора и ротора
    v->sigmaLs = v->Lls * 2.0f;
    v->Lr = v->Lm + v->Lls;
    v->Ls = v->Lr;
    v->sigma = 1.0f - v->Lm * v->Lm / (v->Ls * v->Lr);

    // Находим относительный ток холостого хода. Приводим обязательно к амплитудному значению.
    Ixx = _SQRT_2 * v->Ixx;                 
    v->Ixx_Ampl = Ixx;


    // Расчет номинального потокосцепления ротора
    Tmp1 = v->Fnom * v->FreqBASE;

    // В зависимости от типа управления, рассчитываем номинальное возбуждение
    switch (v->DriveType){
        case TYPE_SCALAR_NO_DEADBAND:
        case TYPE_SCALAR:
        case TYPE_ACIM_FOC_SENSORED:
        case TYPE_ACIM_FOC_SENSORLESS:
          // Расчет возбуждения для асинхронных двигателей
          if ((v->Fnom != 0.0f) && (v->Lm != 0.0f))
          {
            if (v->Ixx == 0.0f){
              // Если тока ХХ нету - то вычисляем возбуждение через формулу
              v->PhiR_nom = 0.9f * _SQRT_2_DIV_3 * v->Usn * v->VoltBASE / (2.0f * _PI * Tmp1 * v->FluxBASE); //0.9
              Ixx = v->PhiR_nom * v->CurrBASE;
              v->EmfRef_iq = Ixx * 2.0f * _PI * v->Fnom * v->FreqBASE * v->Lm / (v->VoltBASE * _SQRT_2);
              v->EmfFdb_K = 2.0f * _PI * v->FluxBASE * v->FreqBASE / v->VoltBASE;
            } else {
              v->EmfRef_iq = Ixx * 2.0f * _PI * v->Fnom * v->FreqBASE * v->Lm / (v->VoltBASE * _SQRT_2);
              v->EmfFdb_K = 2.0f * _PI * v->FluxBASE * v->FreqBASE / v->VoltBASE;
              v->PhiR_nom = Ixx / v->CurrBASE; //0.9
            }
          } else {
            v->PhiR_nom = 0;
          }
          break;
      
        case TYPE_PMSM_FOC_SENSORED:
        case TYPE_PMSM_FOC_SENSORLESS:
          // Расчет номинального возбуждения 
          v->PhiR_nom = 0.9f * v->Usn * v->VoltBASE * _SQRT_2 / (_SQRT_3 * _2PI * v->Fnom * v->FreqBASE); // Psi = sqrt(2)*Unom/(sqrt(3)*2*pi*Fel)
          v->PhiR_nom = v->PhiR_nom / v->CurrBASE;
          break;
    }
    
#warning Занижен уровень активации достижения потока, для нормальной работы регулятора потока.
    v->PhiR_min = 0.7f * v->PhiR_nom;//0.9f * v->PhiR_nom; // 

    // Расчет номинального тока намагничивания
    v->Istn = v->Isn * _SQRT_2;
    v->Idn = v->PhiR_nom;                      // Idn  = PhiR/Lm (base: Lm*Ib)
    if (v->Idn > v->Istn){                     // Номинальный ток АД, амплитудное значение
        v->Idn = v->Istn;
    }

    // Расчет максимальной скорости для регулятора ЭДС
    v->Wnom_Hz = v->Wnom * v->FreqBASE;
    v->Wmax_Hz = v->Wmax * v->FreqBASE;
    if (v->Wmax_Hz < 2.0f * v->Wnom_Hz){
        v->Wmax_Hz = 2.0f * v->Wnom_Hz;
    }

    // Расчет ограничевающего диапазона, для режима управления моментом 
    v->WrH = v->Wmax;
    v->WrL = v->Wmax - v->WrH * 0.1f;

    // Расчет частоты вызова регулятора скорости или периода расчета скорости
    v->DivRegSpd = (u16)(v->TregSpd / v->Tpwm);

    if(v->DivRegSpd == 0){
        v->DivRegSpd = 1;
    }

    if(v->DivRegSpd > 100){
        v->DivRegSpd = 100;
    }

    // Пероевод в _iq коэффициентов регулятора скорости
    v->Idn_iq = v->Idn;
//    v->KpSpd_iq  = v->KpSpd;
//    v->KiSpd_iq  = v->KiSpd;
    
    v->Imax_f32  = v->Imax;
    v->ImaxAmpl_f32  = v->Imax * _SQRT_2;
    
    v->PhiR_min_iq = v->PhiR_min;
}


