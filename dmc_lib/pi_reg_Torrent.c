/*=====================================================================================
 File name:        PI_REG_TORRENT.C  (IQ version)                  
                    
 Originator: Triol

 Description:  The PI torrent controller               

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 06-02-2018 Version 1.00
-------------------------------------------------------------------------------------*/

#include "pi_reg_torrent.h"
#include "Filter.h"

void pi_reg_tor_calc(register PIREGTOR *v)
{
    float  TmpKp, TmpKi;

    // Исходные коэф. регулятора момента - нормальная скорость работы регулятора
    TmpKp = v->Kp;
    TmpKi = v->Ki;
    // Проверка на ограничение, и изменение скорости работы регулятора момента
    if( (v->AccelOverrun == 0) && (v->SpdSaturate == 1) ){ // Если видим наличие насыщения регулятора скорости и не зафиксировано превышение ускорения
       // заменяем настройки регулятора момента на более медленные
       TmpKp =  TmpKp * 0.5f;
       TmpKi =  TmpKp * 0.5f;

       // Если счетчик = -1, значит это первое вхождение в ограничение
       if(v->SaturateTimeCnt == -1){
           v->SaturateTimeCnt = 0;                   // и мы заряжаем таймер время задаем в секудах и умножаем на частоту ШИМа  
       } 

       // Если счетчик заряжен - начинаем фиксировать выход регулятора
       if(v->SaturateTimeCnt > 0) {
           v->SaturateTimeCnt --;      // Уменьшаем счетчик
           v->Ref = v->Fdb;            // делаю задание равным текущей ОС, таким образом выход регулятора прекратит меняться.
       }
    }
    else if( (v->AccelOverrun == 1) && (v->SpdSaturate == 1) ) { // Если видим наличие насыщения регулятора скорости и зафиксировано превышение ускорения
	//v->Ref = v->Fdb; // Блокирую изменение выхода регулятора, для предотвращения дополнительного увеличения, т.к. я и при этом моменте уже еду.
    }
    else{
	v->SaturateTimeCnt = -1;       // Сбрасываем счетчик в исходную позицию
    }

    // Фильтрация коэффициентов, для плавного перехода
    v->Filter_Kp = f32_Filtr(v->Filter_Kp, TmpKp, 0.0625f);  //(0.0625f = 4 для iq фильтра)
    TmpKp = v->Filter_Kp;
    v->Filter_Ki = f32_Filtr(v->Filter_Ki, TmpKi, 0.0625f);  //(0.0625f = 4 для iq фильтра)
    TmpKi = v->Filter_Ki;

    // Compute the error
    v->Err = v->Ref - v->Fdb;

    // Compute the proportional output
    v->Up = TmpKp * v->Err;

    // Compute the integral output
    v->Ui += TmpKi * v->Err;

    // Saturate the integral output
    if(v->Ui > v->UiOutMax){
        v->Ui =  v->UiOutMax;
    }
    else if(v->Ui < v->UiOutMin){
        v->Ui =  v->UiOutMin;
    }

    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui;

    // Saturate the output
    if(v->OutPreSat > v->OutMax){
        v->Out =  v->OutMax;
    }
    else if(v->OutPreSat < v->OutMin){
        v->Out =  v->OutMin;
    }
    else{
        v->Out = v->OutPreSat;
    }

    // Устраняем насыщение интеграла
    if(v->Out >= 0.0f){                    // при Out > 0, минимум даем какой задан, а корректируем только интеграл максимума.
        v->UiOutMax = v->OutMax - v->Out;  // Коррекция максимума
        v->UiOutMin = v->OutMin;           // Минимум не трогаем
    } 
    else{ // if v->Out < 0
        v->UiOutMax = v->OutMax;          // Максимум не трогаем
        v->UiOutMin = v->OutMin - v->Out; // Коррекция минимума, при отрицательном выходе
    }
}

void pi_reg_tor_reset(PIREGTOR *v)
{
    v->Err = 0.0f;
    v->ErrP = 0.0f;
    v->ErrT = 0.0f;
    v->Up = 0.0f;
    v->Ui = 0.0f;
    v->OutPreSat = 0.0f;
    v->Out = 0.0f;
    v->UiOutMax = 0.0f;
    v->UiOutMin = 0.0f;
}

