//Файл автоматически сгенерирован в ТехПО. Ручная коррекция приведет к ошибкам.

#ifndef CONST_H 
#define CONST_H

typedef enum	{
_kAsinkhronnyjj_0,                        //Асинхронный
_kSinkhronnyjj_s_post__magnitami_1,       //Синхронный с пост. магнитами
_kSinkhronnyjj_s_vneshn_vozbuditelem_2,   //Синхронный с внешн.возбудителем
}NET00295;

typedef enum	{
_kSkalyar_bez_MV_0,                       //Скаляр без МВ
_kSkalyarnyjj_1,                          //Скалярный
_kVU_AD_s_OS_00_2,                        //ВУ АД с ОС%00
_kVU_AD_bez_OS_03_3,                      //ВУ АД без ОС%03
_kVU_VD_s_OS_00_4,                        //ВУ ВД с ОС%00
_kVU_VD_bez_OS_00_5,                      //ВУ ВД без ОС%00
}TXT_TIP_PRIVODA;

typedef enum	{
_kkVt_0,                                  //кВт
_kl_s__1,                                 //л.с.
}NET00310;

typedef enum	{
_kPryamoe_0,                              //Прямое
_kObratnoe_1,                             //Обратное
}TXT_PRYM_OBR;

typedef enum	{
_kZapretit__0,                            //Запретить
_kRazreshit__1,                           //Разрешить
}NET00349;

typedef enum	{
_kNet_dejjstviya_0,                       //Нет действия
_kSbrosit__1,                             //Сбросить
}NET00350;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kBez_vrashheniya__Ocenka_Lm_1,           //Без вращения. Оценка Lm
_kBez_vrashheniya__Izmerenie_Lm_2,        //Без вращения. Измерение Lm
_kC_vrashheniem_vala_3,                   //C вращением вала
}NET00014;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kVklyucheno_1,                           //Включено
}TXT_OFF_ON;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kPreduprezhdenie_1,                      //Предупреждение
_kAvariya_2,                              //Авария
}TXT_OFF_WARN_FAULT;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kPreduprezhdenie_1,                      //Предупреждение
_kSnizhenie_moshhnosti_2,                 //Снижение мощности
_kAvariya_3,                              //Авария
}NET00355;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kAbsolyutnoe_znachenie_1,                //Абсолютное значение
_kProcenty_2,                             //Проценты
}NET00977;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kPreduprezhdenie_1,                      //Предупреждение
_kAvariya_2,                              //Авария
}NET00232;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kAvariya_1,                              //Авария
}NET00047;

typedef enum	{
_kAktivnyjj_tok_0,                        //Активный ток
_kPolnyjj_tok_1,                          //Полный ток
}NET00293;

typedef enum	{
_kPeregruz_po_vremeni_0,                  //Перегруз по времени
_kPeregruz_po_I2t_1,                      //Перегруз по I2t
}TXT_TIP_PEREGRUZ;

typedef enum	{
_kRezerv_0,                               //Резерв
_kECN413_ECN1313_1,                       //ECN413/ECN1313
_kECN1325_2,                              //ECN1325
_kSSI_Kod_Greya_3,                        //SSI Код Грея
_kSSI_Dvoichnyjj_kod_4,                   //SSI Двоичный код
}TXT_ENCO_SERIAL_TYPE;

typedef enum	{
_kImpul_sy_Oborot_0,                      //Импульсы/Оборот
_kBity_Oborot_1,                          //Биты/Оборот
}TXT_ENCO_RESOL;

typedef enum	{
_kPryamaya_0,                             //Прямая
_kObratnaya_1,                            //Обратная
}TXT_ENCO;

typedef enum	{
_kAvtoraschet_0,                          //Авторасчет
_k2_1,                                    //2
_k4_2,                                    //4
_k8_3,                                    //8
_k16_4,                                   //16
_k32_5,                                   //32
_k64_6,                                   //64
_k128_7,                                  //128
_k256_8,                                  //256
_k512_9,                                  //512
_k1024_10,                                //1024
_k2048_11,                                //2048
_k4096_12,                                //4096
_k8192_13,                                //8192
}TXT_2_POW;

typedef enum	{
_kAvto_0,                                 //Авто
_kRuchn__Pryamaya_1,                      //Ручн. Прямая
_kRuchn__Obratn__2,                       //Ручн. Обратн.
}TXT_FAST_SPD_SIGN;

typedef enum	{
_kOtklyuchena_0,                          //Отключена
_kFazirovka_v_1_takt_1,                   //Фазировка в 1 такт
_kFazirovka_v_2_takta_2,                  //Фазировка в 2 такта
}NET00769;

typedef enum	{
_kVybeg_0,                                //Выбег
_kCHastotnoe_1,                           //Частотное
}NET00016;

typedef enum	{
_kLinejjnyjj_0,                           //Линейный
_kS_obraznyjj_1,                          //S-образный
_kSin2_obraznyjj_02_2,                    //Sin2-образный%02
}NET00249;

typedef enum	{
_kFiksirovanaya_0,                        //Фиксированая
_kPol_zovatel__1,                         //Пользователь
}TXT_TYPE_OF_S;

typedef enum	{
_kX_0,                                    //X
_kX_X_1,                                  //X_X
_kX_XX_2,                                 //X_XX
_kX_XXX_3,                                //X_XXX
}NET00250;

typedef enum	{
_kVremya_0,                               //Время
_kRyvok_Uskor_1,                          //Рывок/Ускор
}NET00976;

typedef enum	{
_kKHarakteristika_U_f__1_0,               //Характеристика U/f №1
_kKHarakteristika_U_f__2_1,               //Характеристика U/f №2
_kVybor_po_diskretnomu_signalu_2,         //Выбор по дискретному сигналу
}TXT_NUMB_UF;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kDiskretnyjj_vkhod_1_1,                  //Дискретный вход 1
_kDiskretnyjj_vkhod_2_2,                  //Дискретный вход 2
_kDiskretnyjj_vkhod_3_3,                  //Дискретный вход 3
_kDiskretnyjj_vkhod_4_4,                  //Дискретный вход 4
_kDiskretnyjj_vkhod_5_5,                  //Дискретный вход 5
_kDiskretnyjj_vkhod_6_6,                  //Дискретный вход 6
_kDiskretnyjj_vkhod_7_7,                  //Дискретный вход 7
}TXT_DIN_INPUT;

typedef enum	{
_kLinejjnaya_0,                           //Линейная
_kKvadratichnaya_1,                       //Квадратичная
_kProgrammiruemaya_2,                     //Программируемая
}NET00427;

typedef enum	{
_kPol_zovatel_skaya_nastrojjka_0,         //Пользовательская настройка
_kAvtonastrojjka_na_dvigatel__1,          //Автонастройка на двигатель
}NET00456;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kRegulyator_EMF_1,                       //Регулятор EMF
_kRegulyator_potoka_2,                    //Регулятор потока
}TXT_FIELD_WEAK;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kFiksirovannyjj_sdvig_fazy_1,            //Фиксированный сдвиг фазы
_kPI_regulyator_fazy_2,                   //ПИ регулятор фазы
}NET00594;

typedef enum	{
_kGc_kh_kh_0,                             //Гц х_х
_kGc_kh_khkh_1,                           //Гц х_хх
_kOb_min_2,                               //Об/мин
}NET00168;

typedef enum	{
_kKanal_upravleniya_1_0,                  //Канал управления 1
_kKanal_upravleniya_2_1,                  //Канал управления 2
_kDiskretnyjj_vkhod_2,                    //Дискретный вход
}TXT_SEL_CONTR;

typedef enum	{
_kMestnyjj_pul_t_0,                       //Местный пульт
_kWIFI_1,                                 //WIFI
_kASU_2,                                  //АСУ
_kDiskretnye_vkhody_3,                    //Дискретные входы
}TXT_SEL_CHANEL;

typedef enum	{
_kKanal_zadaniya_1_0,                     //Канал задания 1
_kKanal_zadaniya_2_1,                     //Канал задания 2
_kDiskretnyjj_vkhod_2,                    //Дискретный вход
}NET00721;

typedef enum	{
_kMestnyjj_pul_t_0,                       //Местный пульт
_kWIFI_1,                                 //WIFI
_kASU_2,                                  //АСУ
_kDiskretnye_vkhody_3,                    //Дискретные входы
_kAnalogovyjj_vkhod_1_4,                  //Аналоговый вход 1
_kAnalogovyjj_vkhod_2_5,                  //Аналоговый вход 2
_kPID_6,                                  //ПИД
_kCifrovojj_potenciometr_7,               //Цифровой потенциометр
_kAnalogovyjj_vkhod_3_8,                  //Аналоговый вход 3
_kAnalogovyjj_vkhod_4_9,                  //Аналоговый вход 4
}TXT_SEL_REF;

typedef enum	{
_kPo_komande_0,                           //По команде
_kPo_podache_pitaniya_1,                  //По подаче питания
}NET00442;

typedef enum	{
_kRuchnojj_0,                             //Ручной
_kAvtomaticheskijj_1,                     //Автоматический
}NET00223;

typedef enum	{
_kVpered_0,                               //Вперед
_kNazad_1,                                //Назад
_kAvariya_2,                              //Авария
_kStop_3,                                 //Стоп
}NET00181;

typedef enum	{
_kSbros_po_ostanovu_0,                    //Сброс по останову
_kSokhranit__zadanie_1,                   //Сохранить задание
}NET00592;

typedef enum	{
_kOtsutstvuet_0,                          //Отсутствует
_kPusk_vpered_potencial_nyjj_1,           //Пуск вперед потенциальный
_kPusk_vpered_po_frontu_01_2,             //Пуск вперед по фронту%01
_kPusk_vpered_po_spadu_01_3,              //Пуск вперед по спаду%01
_kPusk_nazad_potencial_nyjj_4,            //Пуск назад потенциальный
_kPusk_nazad_po_frontu_01_5,              //Пуск назад по фронту%01
_kPusk_nazad_po_spadu_01_6,               //Пуск назад по спаду%01
_kStop_po_frontu_01_7,                    //Стоп по фронту%01
_kStop_po_spadu_01_8,                     //Стоп по спаду%01
_kSbros_avarijj_01_9,                     //Сброс аварий%01
_kRazreshenie_puska_01_10,                //Разрешение пуска%01
_kZapret_puska_01_11,                     //Запрет пуска%01
_kAvarijjnyjj_stop_12,                    //Аварийный стоп
_kOstanov_vybegom_01_13,                  //Останов выбегом%01
_kUvelichenie_chastoty_01_14,             //Увеличение частоты%01
_kUmen_shenie_chastoty_01_15,             //Уменьшение частоты%01
_kCHastota__Bit_0_16,                     //Частота. Бит 0
_kCHastota__Bit_1_17,                     //Частота. Бит 1
_kCHastota__Bit_2_18,                     //Частота. Бит 2
_kCHastota__Bit_3_19,                     //Частота. Бит 3
_kFiksirovannaya_chastota_1_01_20,        //Фиксированная частота 1%01
_kFiksirovannaya_chastota_2_01_21,        //Фиксированная частота 2%01
_kFiksirovannaya_chastota_3_01_22,        //Фиксированная частота 3%01
_kFiksirovannaya_chastota_4_01_23,        //Фиксированная частота 4%01
_kIzmenenie_tempa_02_24,                  //Изменение темпа%02
_kVneshnijj_stop_02_25,                   //Внешний стоп%02
_kOS_kontaktora_02_26,                    //ОС контактора%02
_kOS_Tormoz_02_27,                        //ОС Тормоз%02
_kEHvakuaciya_02_28,                      //Эвакуация%02
_kPereklyuchenie_U_F_29,                  //Переключение U/F
_kVybor_kanala_upravleniya_01_30,         //Выбор канала управления%01
_kVybor_kanala_zadaniya_01_31,            //Выбор канала задания%01
_kAvariya_pol_zovatelya__1_01_32,         //Авария пользователя  1%01
_kAvariya_pol_zovatelya__2_01_33,         //Авария пользователя  2%01
_kRevers_po_spadu_34,                     //Реверс по спаду
_kRevers_po_frontu_35,                    //Реверс по фронту
_kOS_2_Tormoz_02_36,                      //ОС 2 Тормоз%02
}TXT_DIN_FUNC;

typedef enum	{
_kPryamaya_0,                             //Прямая
_kInversnaya_1,                           //Инверсная
}TXT_DIN_INV;

typedef enum	{
_k4_ms_0,                                 //4 мс
_k8_ms_1,                                 //8 мс
_k16_ms_2,                                //16 мс
_k32_ms_3,                                //32 мс
_k64_ms_4,                                //64 мс
_k128_ms_5,                               //128 мс
_k256_ms_6,                               //256 мс
_k512_ms_7,                               //512 мс
_k1_s_8,                                  //1 с
_k2_s_9,                                  //2 с
_k4_s_10,                                 //4 с
_k8_s_11,                                 //8 с
_k16_s_12,                                //16 с
}TXT_FILTR;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kDiskretnyjj_vkhod_1_01_1,               //Дискретный вход 1%01
_kDiskretnyjj_vkhod_2_01_2,               //Дискретный вход 2%01
_kDiskretnyjj_vkhod_3_01_3,               //Дискретный вход 3%01
_kDiskretnyjj_vkhod_4_01_4,               //Дискретный вход 4%01
_kDiskretnyjj_vkhod_5_01_5,               //Дискретный вход 5%01
_kDiskretnyjj_vkhod_6_01_6,               //Дискретный вход 6%01
_kDiskretnyjj_vkhod_7_01_7,               //Дискретный вход 7%01
_kDiskretnyjj_vkhod_8__opciya__01_8,      //Дискретный вход 8 (опция)%01
_kDiskretnyjj_vkhod_9__opciya__01_9,      //Дискретный вход 9 (опция)%01
_kDiskretnyjj_vkhod_10__opciya__01_10,    //Дискретный вход 10 (опция)%01
_kDiskretnyjj_vkhod_11__opciya__01_11,    //Дискретный вход 11 (опция)%01
_kDiskretnyjj_vkhod_12__opciya__01_12,    //Дискретный вход 12 (опция)%01
_kDiskretnyjj_vkhod_13__opciya__01_13,    //Дискретный вход 13 (опция)%01
_kRezerv_00_14,                           //Резерв%00
_kGotov_15,                               //Готов
_kRabota_16,                              //Работа
_kAvariya_17,                             //Авария
_kNet_avarii_18,                          //Нет аварии
_kMekhanicheskijj_tormoz_01_19,           //Механический тормоз%01
_kKontaktor_Lift_02_20,                   //Контактор Лифт%02
_kTormoz_Lift_02_21,                      //Тормоз Лифт%02
_kSverkhtok_dvigatelya_01_22,             //Сверхток двигателя%01
_kAvariya_Avkh1_23,                       //Авария Авх1
_kAvariya_Avkh2_24,                       //Авария Авх2
_kAvariya_Avkh3_25,                       //Авария Авх3
_kAvariya_Avkh4_26,                       //Авария Авх4
_kMotor_PCH1_27,                          //Мотор_ПЧ1
_kMotor_PCH2_28,                          //Мотор_ПЧ2
_kMotor_PCH3_29,                          //Мотор_ПЧ3
_kMotor_PCH4_30,                          //Мотор_ПЧ4
_kRele_Davleniya_31,                      //Реле Давления
_kKomanda_revers_32,                      //Команда реверс
_kPeregruz_EHD_33,                        //Перегруз ЭД
_kNet_svyazi_34,                          //Нет связи
_kPeregrev_EHD_35,                        //Перегрев ЭД
_kPreduprezhdenie_36,                     //Предупреждение
_kPodkhvat_37,                            //Подхват
_kMaster_slave_38,                        //Master/slave
_kCooler_Vkl_39,                          //Cooler Вкл
_kMTZ_utechki_40,                         //МТЗ утечки
}TXT_DIN;

typedef enum	{
_kBit_kod_0,                              //Бит-код
_kFiksirovanye_chastoty_1,                //Фиксированые частоты
}JOG_DI_MODE;

typedef enum	{
_kpo_maks_nomeru_0,                       //по макс.номеру
_kpo_minim_nomeru_1,                      //по миним.номеру
}NET00135;

typedef enum	{
_k0_10_V_0,                               //0-10 В
_k4_20_mA_1,                              //4-20 мА
}TXT_TYPE_AOI;

typedef enum	{
_k_0,                                     //
_kmV_1,                                   //мВ
_kV_2,                                    //В
_kkV_3,                                   //кВ
_kmA_4,                                   //мА
_kA_5,                                    //А
_kkA_6,                                   //кА
_kGc_7,                                   //Гц
_kkGc_8,                                  //кГц
_kkVAr_9,                                 //кВАр
_kkVA_10,                                 //кВА
_kVt_11,                                  //Вт
_kkVt_12,                                 //кВт
_kmOm_13,                                 //мОм
_kOm_14,                                  //Ом
_kkOm_15,                                 //кОм
_kMOm_16,                                 //МOм
_kmkGn_17,                                //мкГн
_kmGn_18,                                 //мГн
_kmks_19,                                 //мкс
_kms_20,                                  //мс
_ks_21,                                   //с
_kmin_22,                                 //мин
_kch_23,                                  //ч
_kmm_24,                                  //мм
_ksm_25,                                  //см
_kdm_26,                                  //дм
_km_27,                                   //м
_kob_min_28,                              //об/мин
_k_S_29,                                  //°С
_k__30,                                   //%
_km2_31,                                  //м2
_km3_32,                                  //м3
_km2_min_33,                              //м2/мин
_km3_sh_34,                               //м3/ч
_kopis_35,                                //опис
_katm_36,                                 //атм
_kl_37,                                   //л
_kg_38,                                   //g
_kGc_s_39,                                //Гц/с
_kV_s_40,                                 //В/с
_k__s_41,                                 //%/с
_kkm_42,                                  //км
_km_s2_43,                                //м/с2
_kkPa_44,                                 //кПа
_kMPa_45,                                 //МПа
_kkg_sm2_46,                              //кг/см2
_kpsi_47,                                 //psi
_k_F_48,                                  //°F
_kkub_s_49,                               //куб/с
_kkVt_ch_50,                              //кВт*ч
_kMVt_ch_51,                              //МВт*ч
_kV_km_52,                                //В/км
_kMvar_ch_53,                             //Мвар*ч
_krad_54,                                 //рад
_krad_s_55,                               //рад/с
_krad_s2_56,                              //рад/с2
_kkg_m2_57,                               //кг/м2
_km_s_58,                                 //м/с
_kNm_59,                                  //Нм
_kgrad_60,                                //град
_kGn_61,                                  //Гн
_k1_s_62,                                 //1/с
_kkg_63,                                  //кг
_kmm2_64,                                 //мм2
_kkg_m3_65,                               //кг/м3
_kbar_66,                                 //бар
_kVb_67,                                  //Вб
_kbit_68,                                 //бит
_kotn_ed__69,                             //отн.ед.
}TXT_ED_IZM;

typedef enum	{
_kXX_0,                                   //XX
_kXX_X_1,                                 //XX_X
_kXX_XX_2,                                //XX_XX
_kXX_XXX_3,                               //XX_XXX
}TXT_STEPEN;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kPreduprezhdenie_1,                      //Предупреждение
_kAvariya_2,                              //Авария
_kFiksirovannaya_chastota_3,              //Фиксированная частота
}TXT_AIN_PROTECTION;

typedef enum	{
_kNe_vpisyvat__0,                         //Не вписывать
_kKalibrovat__minimum_1,                  //Калибровать минимум
_kKalibrovat__maksimum_2,                 //Калибровать максимум
}TXT_AIN_CALIBR;

typedef enum	{
_kStandartnaya_0,                         //Стандартная
_kOgranichenie_1,                         //Ограничение
_kZona_nechuvstv_2,                       //Зона нечувств
_kZona_nechustv_0_3,                      //Зона нечуств 0
}TXT_AIN_CURVE_FORM;

typedef enum	{
_kNe_zadejjstvovan_0,                     //Не задействован
_kCHastota_zadaniya_1,                    //Частота задания
_kZadanie_PID_2,                          //Задание ПИД
_kObratnaya_svyaz__PID_3,                 //Обратная связь ПИД
_kOshibka_PID_4,                          //Ошибка ПИД
_kVykhodnaya_chastota_5,                  //Выходная частота
_kCHastota_rotora_6,                      //Частота ротора
_kAnalogovyjj_vkhod_1_7,                  //Аналоговый вход 1
_kAnalogovyjj_vkhod_2_8,                  //Аналоговый вход 2
_kMoshhnost__aktivnaya_9,                 //Мощность активная
_kMoshhnost__reaktivnaya_10,              //Мощность реактивная
_kCos_F_11,                               //Cos Ф
_kTok_dvigatelya_12,                      //Ток двигателя
_kVykhodnoe_napryazhenie_13,              //Выходное напряжение
_kUd_14,                                  //Ud
_kMoment_zadaniya_15,                     //Момент задания
_kMoment_tekushhijj_16,                   //Момент текущий
_kAnalogovyjj_vkhod_3_17,                 //Аналоговый вход 3
_kAnalogovyjj_vkhod_4_18,                 //Аналоговый вход 4
}TXT_AOUT;

typedef enum	{
_kOdnopolyarnyjj_0,                       //Однополярный
_kDvukhpolyarnyjj_1,                      //Двухполярный
}TXT_POLARITY;

typedef enum	{
_kNe_vpisyvat__0,                         //Не вписывать
_kTest_1,                                 //Тест
_kKalibrovat__minimum_2,                  //Калибровать минимум
_kKalibrovat__maksimum_3,                 //Калибровать максимум
_kSbros_kalibrovok_4,                     //Сброс калибровок
}TXT_AOUT_CALIBR;

typedef enum	{
_kAnalogovyjj_vkhod_1_0,                  //Аналоговый вход 1
_kAnalogovyjj_vkhod_2_1,                  //Аналоговый вход 2
_kRuchnoe__ASU_2,                         //Ручное, АСУ
_kCifrovojj_potenciometr_3,               //Цифровой потенциометр
_kAnalogovyjj_vkhod_3_4,                  //Аналоговый вход 3
_kAnalogovyjj_vkhod_4_5,                  //Аналоговый вход 4
}NET00121;

typedef enum	{
_kNe_ispol_zovat__0,                      //Не использовать
_kAnalogovyjj_vkhod_1_1,                  //Аналоговый вход 1
_kAnalogovyjj_vkhod_2_2,                  //Аналоговый вход 2
_kAnalogovyjj_vkhod_3_3,                  //Аналоговый вход 3
_kAnalogovyjj_vkhod_4_4,                  //Аналоговый вход 4
}NET00029;

typedef enum	{
_kOS1_0,                                  //ОС1
_kOS2_1,                                  //ОС2
_kOS1+OS2_2,                              //ОС1+ОС2
_kOS1_OS2_3,                              //ОС1-ОС2
_kSrednee_OS1_i_OS2_4,                    //Среднее ОС1 и ОС2
_kNaimen_shee_OS1_ili_OS2_5,              //Наименьшее ОС1 или ОС2
_kNaibol_shee_OS1_ili_OS2_6,              //Наибольшее ОС1 или ОС2
}NET00026;

typedef enum	{
_kPryamaya_0,                             //Прямая
_kObratnaya_1,                            //Обратная
}TXT_TYPEHAR;

typedef enum	{
_kms_0,                                   //мс
_k10_ms_1,                                //10 мс
_k100_ms_2,                               //100 мс
_k1_s_3,                                  //1 с
}TXT_TIMESCALE;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kVklyucheno_1,                           //Включено
_kSbrosit__chastoty_2,                    //Сбросить частоты
}NET00521;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kVse_avarii_1,                           //Все аварии
_kAvarii_iz_spiska_2,                     //Аварии из списка
}NET00435;

typedef enum	{
_kNe_sbroshen_0,                          //Не сброшен
_kSbrosit__1,                             //Сбросить
}TXT_RESET;

typedef enum	{
_k9600_0,                                 //9600
_k14400_1,                                //14400
_k19200_2,                                //19200
_k38400_3,                                //38400
_k56000_4,                                //56000
_k57600_5,                                //57600
_k115200_6,                               //115200
_k230400_7,                               //230400
_k460800_8,                               //460800
_k921600_9,                               //921600
_k1500000_10,                             //1500000
}TXT_NET_SPEED;

typedef enum	{
_kNet_podklyuch__0,                       //Нет подключ.
_kZanyat_1,                               //Занят
_kRabota_2,                               //Работа
_kObrabotka_fajjla___mem_3,               //Обработка файла *.mem
}TXT_ALLOW_PROHIBIT;

typedef enum	{
_kNasos_0,                                //Насос
_kLift_1,                                 //Лифт
_kSVP_2,                                  //СВП
}NET00399;

typedef enum	{
_kVektornyjj_0,                           //Векторный
_kTriol_60_1,                             //Триол 60
_kKombinirovannyjj_2,                     //Комбинированный
}NET00683;

typedef enum	{
_kZapreshheno_0,                          //Запрещено
_kRazresheno_1,                           //Разрешено
}NET00147;

typedef enum	{
_kNet_0,                                  //Нет
_kSbrosit__1,                             //Сбросить
}NET00012;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kVse_avarii_1,                           //Все аварии
_kAvarii_iz_spiska_2,                     //Аварии из списка
_kAvarii_vne_spiska_3,                    //Аварии вне списка
}NET00445;

typedef enum	{
_kOshibok_net_0,                          //Ошибок нет
_kOtkaz_IGBT_fazy_U_1,                    //Отказ IGBT фазы U
_kOtkaz_IGBT_fazy_V_2,                    //Отказ IGBT фазы V
_kOtkaz_IGBT_fazy_W_3,                    //Отказ IGBT фазы W
_kOtkaz_tormoznogo_klyucha_4,             //Отказ тормозного ключа
_kSverkhtok_v_faze_dvigatelya_5,          //Сверхток в фазе двигателя
_kKorotkoe_zamykanie_na_zemlyu_6,         //Короткое замыкание на землю
_kOshibka_datchika_toka_7,                //Ошибка датчика тока
_kTokovaya_otsechka_faza_U_8,             //Токовая отсечка фаза U
_kTokovaya_otsechka_faza_V_9,             //Токовая отсечка фаза V
_kTokovaya_otsechka_faza_W_10,            //Токовая отсечка фаза W
_kPonizhennoe_napryazhenie_ZPT_11,        //Пониженное напряжение ЗПТ
_kPovyshennoe_napryazhenie_ZPT_12,        //Повышенное напряжение ЗПТ
_kVneshnijj_stop_13,                      //Внешний стоп
_kPeregrev_IGBT_14,                       //Перегрев IGBT
_kPovyshennoe_napryazhenie_seti_15,       //Повышенное напряжение сети
_kPonizhennoe_napryazhenie_seti_16,       //Пониженное напряжение сети
_kObryv_faz_dvigatelya_17,                //Обрыв фаз двигателя
_kRezerv_3_18,                            //Резерв 3
_kRezerv_4_19,                            //Резерв 4
_kObryv_cepi_tormoznogo_soprotivleniya_20,  //Обрыв цепи тормозного сопротивления
_krezerv6_21,                             //резерв6
_kOshibka_izmereniya_parametrov_dvigatelya_22,  //Ошибка измерения параметров двигателя
_kIzmerenie_parametrov__Obryv_fazy__23,   //Измерение параметров. Обрыв фазы.
_krezerv9_24,                             //резерв9
_krezerv10_25,                            //резерв10
_krezerv11_26,                            //резерв11
_krezerv12_27,                            //резерв12
_krezerv13_28,                            //резерв13
_krezerv14_29,                            //резерв14
_krezerv15_30,                            //резерв15
_kOshibka_razmeshheniya_kalibrovok_31,    //Ошибка размещения калибровок
_kKnopka_Avarijjnyjj_ostanov_32,          //Кнопка Аварийный останов
_kObryv_vkhodnojj_fazy_33,                //Обрыв входной фазы
_kAvarijjnoe_otkl_kontaktora_lifta_34,    //Аварийное откл контактора лифта
_kPeregrev_tormoznogo_soprotivleniya_35,  //Перегрев тормозного сопротивления
_kOshibka_fazirovki_vkhodnogo_napryazheniya_36,  //Ошибка фазировки входного напряжения
_kNe_zaryazheno_ZPT_37,                   //Не заряжено ЗПТ
_kPeregruz_dvigatelya_38,                 //Перегруз двигателя
_kAvariya_analogovogo_vkhoda_1_39,        //Авария аналогового входа 1
_kAvariya_analogovogo_vkhoda_2_40,        //Авария аналогового входа 2
_kAvar__Avkh3_41,                         //Авар. Авх3
_kAvar__Avkh4_42,                         //Авар. Авх4
_kAvar__Avkh5_43,                         //Авар. Авх5
_kPrevyshenie_skorosti_dvigatelya_44,     //Превышение скорости двигателя
_kNizkaya_skorost__dvigatelya_45,         //Низкая скорость двигателя
_kObryv_fazy_dvigatelya_46,               //Обрыв фазы двигателя
_kObryv_EHnkodera_47,                     //Обрыв Энкодера
_kPusk_zapreshhen_48,                     //Пуск запрещен
_kPeregrev_dvigatelya_49,                 //Перегрев двигателя
_kOshibka_ModBus_50,                      //Ошибка ModBus
_kPrevyshenie_vkhodnogo_napryazheniya_51,  //Превышение входного напряжения
_kPeregrev_okhladitelya_52,               //Перегрев охладителя
_kOtsutstvie_seti_53,                     //Отсутствие сети
_kAvariya_obratnojj_svyazi_tormoza_54,    //Авария обратной связи тормоза
_kOshibka_kriticheskojj_skorosti_55,      //Ошибка критической скорости
_kPoterya_nagruzki_56,                    //Потеря нагрузки
_kKorotkoe_zamykanie_pri_zaryade_57,      //Короткое замыкание при заряде
_kOshibka_namagnichivaniya_58,            //Ошибка намагничивания
_kOtsutstvie_potoka_59,                   //Отсутствие потока
_kSukhojj_khod_60,                        //Сухой ход
_kNet_otveta_Vedomogo_PCH_61,             //Нет ответа Ведомого ПЧ
_kDisbalans_tokov_dvigatelya_62,          //Дисбаланс токов двигателя
_kOshibka_v_Vedomom_PCH_63,               //Ошибка в Ведомом ПЧ
_kAvariya_Avtoventilya_64,                //Авария Автовентиля
_kAvtoventil___Net_dvigatelya_65,         //Автовентиль. Нет двигателя
_kVedomyjj_PCH_ne_naznachen_66,           //Ведомый ПЧ не назначен
_kVedomyjj_PCH__Oshibka_paketa_67,        //Ведомый ПЧ. Ошибка пакета
_kVedomyjj_PCH__Oshibka_pri_puske_68,     //Ведомый ПЧ. Ошибка при пуске
_kNe_poluchena_OS_ot_kontaktora_69,       //Не получена ОС от контактора
_kRevers_Avtoventilya_70,                 //Реверс Автовентиля
_kNet_svyazi_po_kanalu_Wi_Fi_71,          //Нет связи по каналу Wi-Fi
_kOshibka_skorosti_Vedomogo_PCH_72,       //Ошибка скорости Ведомого ПЧ
_kTest_rezistora_nevozmozhen_73,          //Тест резистора невозможен
_kDiskretnyjj_pusk__Dva_napravleniya_74,  //Дискретный пуск. Два направления
_kOtklyuchenie_SHIM_pri_snyatom_tormoze_75,  //Отключение ШИМ при снятом тормозе
_kZapret_puska_bez_skorosti_76,           //Запрет пуска без скорости
_kIzmerenie_parametrov_ne_zaversheno_77,  //Измерение параметров не завершено
_kOshibka_ModBus_3__ExtRS2__78,           //Ошибка ModBus 3 (ExtRS2)
_kOshibka_ModBus_4__ExtRS2__79,           //Ошибка ModBus 4 (ExtRS2)
_kOshibka_nakhozhdeniya_R_metki_80,       //Ошибка нахождения R метки
_kOshibka_skorosti_fazirov__ehnkodera_81,  //Ошибка скорости фазиров. энкодера
_kFazirovka_ehnkodera_prervana_82,        //Фазировка энкодера прервана
_kOshibka_fiksacii_kabiny_lifta_83,       //Ошибка фиксации кабины лифта
_kAvariya_pol_zovatelya_1_84,             //Авария пользователя 1
_kAvariya_pol_zovatelya_2_85,             //Авария пользователя 2
_kNet_potoka_86,                          //Нет потока
_kSukhojj_khod_87,                        //Сухой ход
_kZaryad_Ud_88,                           //Заряд Ud
_kZaderzhkaMTZ_89,                        //ЗадержкаМТЗ
_kVybeg_90,                               //Выбег
}TXT_ERR;

typedef enum	{
_kRabota_bez_OS_tormoza_0,                //Работа без ОС тормоза
_kRabota_s_OS_tormoza_1,                  //Работа с ОС тормоза
}NET00465;

typedef enum	{
_kGotov_0,                                //Готов
_kDalee_1,                                //Далее
}NET00813;

typedef enum	{
_kNet_0,                                  //Нет
_kDa_1,                                   //Да
}ANS_YN;

typedef enum	{
_kNet_0,                                  //Нет
_kParametr_1_1,                           //Параметр 1
_kParametr_2_2,                           //Параметр 2
_kParametr_3_3,                           //Параметр 3
_kParametr_4_4,                           //Параметр 4
_kParametr_5_5,                           //Параметр 5
_kParametr_6_6,                           //Параметр 6
_kParametr_7_7,                           //Параметр 7
_kParametr_8_8,                           //Параметр 8
_kParametr_9_9,                           //Параметр 9
_kParametr_10_10,                         //Параметр 10
}STAT_PARAM_SET;

typedef enum	{
_kStatus_0,                               //Статус
_kCHastota_1,                             //Частота
}NET00757;

typedef enum	{
_kPostoyanno_0,                           //Постоянно
_kStart_zaryad_1,                         //Старт заряд
_kStart_SHIM_2,                           //Старт ШИМ
_kSHIM_vklyuchen_3,                       //ШИМ включен
_kStop_po_Avarii_4,                       //Стоп по Аварии
_kStart_po_komande_Pusk_5,                //Старт по команде Пуск
}TXT_TYPE_LOGER;

typedef enum	{
_kNe_vybrano_0,                           //Не выбрано
_kTok_fazy_U__Dejjstvuyushhijj__A_10_1,   //Ток фазы U. Действующий, A*10
_kTok_fazy_V__Dejjstvuyushhijj__A_10_2,   //Ток фазы V. Действующий, A*10
_kTok_fazy_W__Dejjstvuyushhijj__A_10_3,   //Ток фазы W. Действующий, A*10
_kNapryazhenie_Udc__V_4,                  //Напряжение Udc, В
_kTok_fazy_U__Mgnovennyjj__A_10_5,        //Ток фазы U. Мгновенный, A*10
_kTok_fazy_V__Mgnovennyjj__A_10_6,        //Ток фазы V. Мгновенный, A*10
_kTok_fazy_W__Mgnovennyjj__A_10_7,        //Ток фазы W. Мгновенный, A*10
_kVykhodnaya_chastota__Gc_10_8,           //Выходная частота, Гц*10
_kCHastota_rotora__Gc_10_9,               //Частота ротора, Гц*10
_kTemperatura_invertora__S_10,            //Температура инвертора, С
_kKontur_skorosti__Zadanie__Gc_10_11,     //Контур скорости. Задание, Гц*10
_kKontur_skorosti__OS__Gc_10_12,          //Контур скорости. ОС, Гц*10
_kKontur_akt__toka__Zadanie__A_10_13,     //Контур акт. тока. Задание, А*10
_kKontur_aktivnogo_toka__OS__A_10_14,     //Контур активного тока. ОС, А*10
_kMoment_dvigatelya__Nm_10_15,            //Момент двигателя, Нм*10
_kCos_F_16,                               //Cos Ф
_kPolnyjj_tok__A_10_17,                   //Полный ток, А*10
_kAktivnyjj_tok__A_10_18,                 //Активный ток, А*10
_kReaktivnyjj_tok__A_10_19,               //Реактивный ток, А*10
_kPolnaya_moshhnost___kVt_20,             //Полная мощность, кВт
_kAktivnaya_moshhnost___kVt_21,           //Активная мощность, кВт
_kReaktivnaya_moshhnost___kVt_22,         //Реактивная мощность, кВт
_kVykhodnoe_napryazhenie__V_23,           //Выходное напряжение, В
_kDiskretnyjj_vkhod_1_24,                 //Дискретный вход 1
_kDiskretnyjj_vkhod_2_25,                 //Дискретный вход 2
_kDiskretnyjj_vkhod_3_26,                 //Дискретный вход 3
_kDiskretnyjj_vkhod_4_27,                 //Дискретный вход 4
_kDiskretnyjj_vkhod_5_28,                 //Дискретный вход 5
_kDiskretnyjj_vkhod_6_29,                 //Дискретный вход 6
_kDiskretnyjj_vkhod_7_30,                 //Дискретный вход 7
_kRelejjnyjj_vykhod_1_31,                 //Релейный выход 1
_kRelejjnyjj_vykhod_2_32,                 //Релейный выход 2
_kTranzistornyjj_vykhod_33,               //Транзисторный выход
_kAvkh_1__Procent_ot_diapazona_34,        //Авх 1. Процент от диапазона
_kAvkh_2__Procent_ot_diapazona_35,        //Авх 2. Процент от диапазона
_kAvykh_1__Procent_ot_diapazona_36,       //Авых 1. Процент от диапазона
_kAvykh_2__Procent_ot_diapazona_37,       //Авых 2. Процент от диапазона
_kDiskretnyjj_vkhod_8_38,                 //Дискретный вход 8
_kDiskretnyjj_vkhod_9_39,                 //Дискретный вход 9
_kDiskretnyjj_vkhod_10_40,                //Дискретный вход 10
_kDiskretnyjj_vkhod_11_41,                //Дискретный вход 11
_kDiskretnyjj_vkhod_12_42,                //Дискретный вход 12
_kDiskretnyjj_vkhod_13_43,                //Дискретный вход 13
_kRelejjnyjj_vykhod_4_44,                 //Релейный выход 4
_kRelejjnyjj_vykhod_5_45,                 //Релейный выход 5
_kAvkh_3__Procent_ot_diapazona_46,        //Авх 3. Процент от диапазона
_kAvkh_4__Procent_ot_diapazona_47,        //Авх 4. Процент от диапазона
_kAvykh_3__Procent_ot_diapazona_48,       //Авых 3. Процент от диапазона
_kAvykh_4__Procent_ot_diapazona_49,       //Авых 4. Процент от диапазона
}TXT_LOGER;

typedef enum	{
_kEnglish_0,                              //English
_kLocal_1,                                //Local
}NET00516;

typedef enum	{
_kGotov_0,                                //Готов
_kSokhranit__nabor_1_1,                   //Сохранить набор 1
_kSokhranit__nabor_2_2,                   //Сохранить набор 2
_kVosstanovit__zavodskie_3,               //Восстановить заводские
_kVosstanovit__nabor_1_4,                 //Восстановить набор 1
_kVosstanovit__nabor_2_5,                 //Восстановить набор 2
_kOshibka_6,                              //Ошибка
_kTajjmaut_7,                             //Таймаут
}TXT_NABOR_UST;

typedef enum	{
_kNet_0,                                  //Нет
_kSlot_1_1,                               //Слот 1
_kSlot_2_2,                               //Слот 2
}SETUP_SLOTS;

typedef enum	{
_kNet_dejjstviya_0,                       //Нет действия
_kProverit__indikaciyu_1,                 //Проверить индикацию
}NET00449;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kAvariya_1,                              //Авария
}TXT_OFF_FAULT;

typedef enum	{
_kAvtoopredelenie_0,                      //Автоопределение
_kPB24_Low_1,                             //PB24 Low
_kCombo_2,                                //Combo
_kPB24_M40_3,                             //PB24-M40
}TXT_SEL_PWR_MODULE;

typedef enum	{
_kBez_kompensacii_0,                      //Без компенсации
_kStaryjj_algoritm_kompensacii_1,         //Старый алгоритм компенсации
_kNovyjj_algoritm_kompensacii_2,          //Новый алгоритм компенсации
_kIspravlennyjj_liftovyjj_algoritm_3,     //Исправленный лифтовый алгоритм
}NET00277;

typedef enum	{
_kOtkl_0,                                 //Откл
_kLinear_0_max_1,                         //Linear 0-max
_kLinear_0_1_2_max_2,                     //Linear 0-1-2-max
_kStep_0_1_2_max_3,                       //Step 0-1-2-max
}NET01039;

typedef enum	{
_kCHastota_0,                             //Частота
_kTok_1,                                  //Ток
}NET00564;

typedef enum	{
_kNe_naznacheno_0,                        //Не назначено
_kSkorost__nominal_naya_1,                //Скорость номинальная
_kSkorost__dotyagivaniya_2,               //Скорость дотягивания
_kSkorost__revizii_3,                     //Скорость ревизии
_kSkorost__ehvakuacii_4,                  //Скорость эвакуации
_kSkorost__vyravnivaniya_5,               //Скорость выравнивания
_kSkorost__dotyagivaniya_2_6,             //Скорость дотягивания 2
_kSkorost__promezhutochn__1_7,            //Скорость промежуточн. 1
_kSkorost__promezhutochn__2_8,            //Скорость промежуточн. 2
_kSkorost__nulevaya_9,                    //Скорость нулевая
_kSkorost___8_10,                         //Скорость №8
_kSkorost___9_11,                         //Скорость №9
_kSkorost___10_12,                        //Скорость №10
_kSkorost___11_13,                        //Скорость №11
_kSkorost___12_14,                        //Скорость №12
_kSkorost___13_15,                        //Скорость №13
_kSkorost___14_16,                        //Скорость №14
_kSkorost___15_17,                        //Скорость №15
}TXT_SPEED_OF_S;

typedef enum	{
_kOtklyuchen_0,                           //Отключен
_kUpravlenie_bez_OS_1,                    //Управление без ОС
_kUpravlenie_s_OS_2,                      //Управление с ОС
}TXT_CONT_MODE;

typedef enum	{
_kTikhaya_avariya_0,                      //Тихая авария
_kFormirovat__avariyu_1,                  //Формировать аварию
}NET00071;

typedef enum	{
_kOtklyuchen_0,                           //Отключен
_kUpravlenie_bez_OS_1,                    //Управление без ОС
_kUpravlenie_s_OS_2,                      //Управление с ОС
}NET00104;

typedef enum	{
_kV_legkuyu_storonu_0,                    //В легкую сторону
_kNapravlenie_stancii_1,                  //Направление станции
_kVsegda_vverkh_2,                        //Всегда вверх
_kVsegda_vniz_3,                          //Всегда вниз
}NET00109;

typedef enum	{
_kOtklyuchen_0,                           //Отключен
_kAktivnyjj_rezhim_1,                     //Активный режим
_kKombinirovannyjj_2,                     //Комбинированный
}NET00660;

typedef enum	{
_kPo_nominal_nojj_skorosti_0,             //По номинальной скорости
_kPo_vremeni_1,                           //По времени
}NET00987;

typedef enum	{
_kGc_0,                                   //Гц
_km_sek_1,                                //м/сек
}NET00142;

typedef enum	{
_kOtklyuchen_0,                           //Отключен
_kTok_CHastota_1,                         //Ток-Частота
_kTok_Vremya_2,                           //Ток-Время
}NET00663;

typedef enum	{
_kNe_vybran_0,                            //Не выбран
_kAnalog__vkhod_1_1,                      //Аналог. вход 1
_kAnalog__vkhod_2_2,                      //Аналог. вход 2
}NET01046;

typedef enum	{
_kOtkl_0,                                 //Откл
_kPreduprezhdenie_1,                      //Предупреждение
_kAvariya_2,                              //Авария
_kRezhim_sna_3,                           //Режим сна
}NET00867;

typedef enum	{
_kAnalog__vkhod_1_0,                      //Аналог. вход 1
_kAnalog__vkhod_2_1,                      //Аналог. вход 2
_kASU_2,                                  //АСУ
_kKombinaciya_3,                          //Комбинация
}NET00024;

typedef enum	{
_kVyklyuchen_0,                           //Выключен
_kVkl_pri_puske_1,                        //Вкл при пуске
_kVkl_vsegda_2,                           //Вкл всегда
}NET00772;

typedef enum	{
_kOtklyucheno_0,                          //Отключено
_kAvariya_1,                              //Авария
_kAPV_2,                                  //АПВ
}TXT_OFF_ON_APV;

typedef enum	{
_kMoment_0,                               //Момент
_kSkorost__1,                             //Скорость
}TXT_UPRAVL_ME_WR;

typedef enum	{
_kNorma_0,                                //Норма
_kObryv_1,                                //Обрыв
_kKorotkoe_zamykanie_2,                   //Короткое замыкание
}TXT_AIN_STATUS;

typedef enum	{
_kOtsutstvuet_0,                          //Отсутствует
_kENC24_I_1,                              //ENC24_I
_kENC24_E_2,                              //ENC24_E
_kENC24_SC_3,                             //ENC24_SC
_kEXT24_RS2_4,                            //EXT24_RS2
_kEXT24_Din_5,                            //EXT24_Din
_kEXT24_AIO2_6,                           //EXT24_AIO2
_kEXT24_CAN_7,                            //EXT24_CAN
_kEXT24_PRF_8,                            //EXT24_PRF
_kEXT24_Eth_9,                            //EXT24_Eth
_kEXT24_PRFI_10,                          //EXT24_PRFI
}TXT_EXT_123_TYPE;

typedef enum	{
_kABC_0,                                  //ABC
_kACB_1,                                  //ACB
_kObryv_fazy_2,                           //Обрыв фазы
_kNedostupno_3,                           //Недоступно
}TXT_FAZIROVKA;

typedef enum	{
_kOtsutstvuet_0,                          //Отсутствует
_kAvariya_Avkh_1_1,                       //Авария Авх 1
_kAvariya_Avkh_2_2,                       //Авария Авх 2
_kNedogruz_dvigatelya_3,                  //Недогруз двигателя
_kPeregruz_dvigatelya_4,                  //Перегруз двигателя
_kMinimal_naya_chastota_5,                //Минимальная частота
_kMaksimal_naya_chastota_6,               //Максимальная частота
_kNo_Water_7,                             //No Water
_kDisbalans_tokov_8,                      //Дисбаланс токов
_kAvariya_Avkh_3_9,                       //Авария Авх 3
_kAvariya_Avkh_4_10,                      //Авария Авх 4
}FAIL_WARNING_TXT;

typedef enum	{
_kNet_predupr__0,                         //Нет предупр.
_kVremya_aktivnosti_APV_isteklo_1,        //Время активности АПВ истекло
_kZapret_puska_bez_skorosti_2,            //Запрет пуска без скорости
_kPusk_otklonen__Net_razresheniya__3,     //Пуск отклонен. Нет разрешения.
_kAktivirovan_pusk_po_vklyucheniyu_4,     //Активирован пуск по включению
_kPoterya_nagruzki_5,                     //Потеря нагрузки
_kPeregruz_dvigatelya_6,                  //Перегруз двигателя
_kFazirovanie_ehnkodera_zaversheno_7,     //Фазирование энкодера завершено
_kDisbalans_tokov_8,                      //Дисбаланс токов
_kAktivirovan_rezhim_derejjtinga_9,       //Активирован режим дерейтинга
_kRezerv_10_10,                           //Резерв 10
_kRezerv_11_11,                           //Резерв 11
_kRezerv_12_12,                           //Резерв 12
_kRezerv_13_13,                           //Резерв 13
_kRezerv_14_14,                           //Резерв 14
_kMinimal_naya_chastota_15,               //Минимальная частота
_kMaksimal_naya_chastota_16,              //Максимальная частота
_kAvarijjnyjj_uroven__Avkh_1_17,          //Аварийный уровень Авх 1
_kAvarijjnyjj_uroven__Avkh_2_18,          //Аварийный уровень Авх 2
_kFiksirovannaya_chastota_Avkh1_19,       //Фиксированная частота Авх1
_kFiksirovannaya_chastota_Avkh2_20,       //Фиксированная частота Авх2
_kAvarijjnyjj_uroven__Avkh_3_21,          //Аварийный уровень Авх 3
_kAvarijjnyjj_uroven__Avkh_4_22,          //Аварийный уровень Авх 4
_kZadanie_nizhe_puskovojj_chastoty_23,    //Задание ниже пусковой частоты
_kFiksirovannaya_chastota_Avkh3_24,       //Фиксированная частота Авх3
_kFiksirovannaya_chastota_Avkh4_25,       //Фиксированная частота Авх4
_kOshibka_OS_PID__Vkhod_nedostupen_26,    //Ошибка ОС ПИД. Вход недоступен
_kSetevojj_adres_uzhe_naznachen_27,       //Сетевой адрес уже назначен
_kZaryad_Ud_28,                           //Заряд Ud
_kDiskretnyjj_vkhod_uzhe_naznachen_29,    //Дискретный вход уже назначен
_kDiskretnaya_funkciya_uzhe_naznachena_30,  //Дискретная функция уже назначена
_kAktivirovan_rezhim_ehvakuacii_31,       //Активирован режим эвакуации
_kAktiven_rezhim_simulyacii_32,           //Активен режим симуляции
_kNet_potoka_33,                          //Нет потока
_kSukhojj_khod_34,                        //Сухой ход
}TXT_WARN;

typedef enum	{
_kNeopredeleno_0,                         //Неопределено
_kCP24_CP24B_Low_1,                       //CP24/CP24B Low
_kCP24B_Combo_2,                          //CP24B Combo
_kCP24B_PB24_M40_3,                       //CP24B PB24-M40
_kCP24_v2_4_4,                            //CP24 v2.4
_kCP24_v2_3_5,                            //CP24 v2.3
_kCP24_v2_2_6,                            //CP24 v2.2
_kCP24_v2_1_7,                            //CP24 v2.1
}NET00166;

#endif // #ifndef CONST_H
