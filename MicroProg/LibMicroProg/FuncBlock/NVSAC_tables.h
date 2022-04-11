#ifndef __NVSAC_TABLES_H
#define __NVSAC_TABLES_H

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>

//42 9496 7295
//#define POSIT_POS_1SHIFT_IND    4       //положение обрабатываемой цифры положительного числа 1-й сдвиг
//#define POSIT_POS_2SHIFT_IND    8       //положение обрабатываемой цифры положительного числа 2-й сдвиг
//4 294 967 295
//#define POSIT_NEG_1SHIFT_IND    3       //положение обрабатываемой цифры отрицательного числа 1-й сдвиг
//#define POSIT_NEG_2SHIFT_IND    7       //положение обрабатываемой цифры отрицательного числа 2-й сдвиг
//#define POSIT_NEG_3SHIFT_IND    10       //положение обрабатываемой цифры отрицательного числа 2-й сдвиг
//extern u32 UnixTime;

/*
union{
    u16 shorts[2];
    float fnum;
} myunion;*/

#define SPACE   0x00  //0xFF  //      пустой индикатор
#define UNDER   0x08  //0xF7  //      подчеркивание
#define MINUS   0x40  //0xF7  //      -
#define EQUAL   0x48  //0xB7  //      =
#define QUEST   0x53  //0xAC  //      знак вопроса
#define BRACK   0x0F  //0xF0  //      ]
#define HORTL   0x70  //0x8F  //      + без черточки слева
#define HORTR   0x46  //0x8F  //      + без черточки справа

#define LAT_G   0x3D  //0xC2  //      G
#define LAT_H   0x76  //0x89  //      H, Н
#define LAT_I   0x30  //0xCF  //      I (левое)
#define LAT_J   0x0E  //0xF1  //      J
#define LAT_L   0x38  //0xC7  //      L
#define LAT_P   0x73  //0x8C  //      P, Р
#define LAT_U   0x3E  //0xC1  //      U
#define LAT_c   0x58  //0xA7  //      c
#define LAT_h   0x74  //0x8B  //      h
#define LAT_n   0x54  //0xAB  //      n, п
#define LAT_o   0x5C  //0xA3  //      o, o
#define LAT_i   0x10  //0xEF  //      i
#define LAT_j   0x0C  //0xF3  //      j
#define LAT_r   0x50  //0xAF  //      r, г
#define LAT_t   0x78  //0x87  //      t
#define LAT_u   0x1C  //0xE3  //      u

#define RUS_G   0x31  //0xCE  //      Г
#define RUS_P   0x37  //0xC8  //      П
#define RUS_U   0x6E  //0x91  //      У
#define RUS_D   0x5F  //0xA0  //      д

#define CIF_0   0x3F  //0xC0  //      0, O
#define CIF_1   0x06  //0xF9  //      1, I
#define CIF_2   0x5B  //0xA4  //      2
#define CIF_3   0x4F  //0xB0  //      3, З, Э
#define CIF_4   0x66  //0x99  //      4, Ч
#define CIF_5   0x6D  //0x92  //      5, S
#define CIF_6   0x7D  //0x82  //      6
#define CIF_7   0x07  //0xF8  //      7
#define CIF_8   0x7F  //0x80  //      8, B
#define CIF_9   0x6F  //0x90  //      9
#define CIF_A   0x77  //0x88  //        A, A
#define CIF_B   0x7C  //0x83  //      b, Ь
#define CIF_C   0x39  //0xC6  //      C, C
#define CIF_D   0x5E  //0xA1  //      d
#define CIF_E   0x79  //0x86  //      E, E
#define CIF_F   0x71  //0x8E  //      F


const unsigned char P24S_hexs[] =      // таблица шестнадцатеричных цифр
{   CIF_0,   //      0 - 0
    CIF_1,   //      1 - 1
    CIF_2,   //      2 - 2
    CIF_3,   //      3 - 3
    CIF_4,   //      4 - 4
    CIF_5,   //      5 - 5
    CIF_6,   //      6 - 6
    CIF_7,   //      7 - 7
    CIF_8,   //      8 - 8
    CIF_9,   //      9 - 9
    CIF_A,   //      A - 10
    CIF_B,   //      b - 11
    CIF_C,   //      C - 12
    CIF_D,   //      d - 13
    CIF_E,   //      E - 14
    CIF_F }; //      F - 15



const indsym_type P24S_syms[] = // полная таблица символов для индикаторов
{   { 0x24, CIF_5 },   //      $
    { 0x28, CIF_C },   //      (
    { 0x29, BRACK },   //      )
    { 0x2D, MINUS },   //      -
    { 0x30, CIF_0 },   //      0
    { 0x31, CIF_1 },   //      1
    { 0x32, CIF_2 },   //      2
    { 0x33, CIF_3 },   //      3
    { 0x34, CIF_4 },   //      4
    { 0x35, CIF_5 },   //      5
    { 0x36, CIF_6 },   //      6
    { 0x37, CIF_7 },   //      7
    { 0x38, CIF_8 },   //      8
    { 0x39, CIF_9 },   //      9
    { 0x3D, EQUAL },   //      =
    { 0x3F, QUEST },   //      ?
    { 0x41, CIF_A },   //      A
    { 0x42, CIF_8 },   //      B
    { 0x43, CIF_C },   //      C
    { 0x45, CIF_E },   //      E
    { 0x46, CIF_F },   //      F
    { 0x47, LAT_G },   //      G
    { 0x48, LAT_H },   //      H
    { 0x49, CIF_1 },   //      I
    { 0x4A, LAT_J },   //      J
    { 0x4C, LAT_L },   //      L
    { 0x4F, CIF_0 },   //      O
    { 0x50, LAT_P },   //      P
    { 0x51, CIF_0 },   //      Q
    { 0x53, CIF_5 },   //      S
    { 0x55, LAT_U },   //      U
    { 0x59, RUS_U },   //      Y
    { 0x5B, CIF_C },   //      [
    { 0x5D, BRACK },   //      ]
    { 0x5F, UNDER },   //      _
    { 0x62, CIF_B },   //      b
    { 0x63, LAT_c },   //      c
    { 0x64, CIF_D },   //      d
    { 0x68, LAT_h },   //      h
    { 0x69, LAT_i },   //      i
    { 0x6A, LAT_j },   //      j
    { 0x6C, CIF_1 },   //      l
    { 0x6E, LAT_n },   //      n
    { 0x6F, LAT_o },   //      o
    { 0x72, LAT_r },   //      r
    { 0x74, LAT_t },   //      t
    { 0x75, LAT_u },   //      u
    { 0x7B, CIF_C },   //      {
    { 0x7C, CIF_1 },   //      |
    { 0x7D, BRACK },   //      }
    { 0xC0, CIF_A },   //      А
    { 0xC1, CIF_6 },   //      Б
    { 0xC2, CIF_8 },   //      В
    { 0xC3, RUS_G },   //      Г
    { 0xC5, CIF_E },   //      Е
    { 0xC7, CIF_3 },   //      З
    { 0xC8, LAT_U },   //      И
    { 0xC9, LAT_U },   //      Й
    { 0xCD, LAT_H },   //      Н
    { 0xCE, CIF_0 },   //      О
    { 0xCF, RUS_P },   //      П
    { 0xD0, LAT_P },   //      Р
    { 0xD1, CIF_C },   //      С
    { 0xD3, RUS_U },   //      У
    { 0xD6, LAT_U },   //      Ц
    { 0xD7, CIF_4 },   //      Ч
    { 0xDA, CIF_B },   //      Ъ
    { 0xDC, CIF_B },   //      Ь
    { 0xDD, CIF_3 },   //      Э
    { 0xE3, LAT_r },   //      г
    { 0xE4, RUS_D },   //      д
    { 0xE8, LAT_u },   //      и
    { 0xE9, LAT_u },   //      й
    { 0xEE, LAT_o },   //      о
    { 0xEF, LAT_n },   //      п
    { 0xF1, LAT_c },   //      с
    { 0xF6, LAT_u },   //      ц
    { 0x00, SPACE } }; //      конец таблицы

const unsigned char minuses[] = { MINUS, MINUS, MINUS, MINUS, MINUS };
const unsigned char neredak[] = { LAT_H, CIF_E, LAT_P, CIF_E, CIF_D };
const unsigned char ostanow[] = { CIF_8, SPACE, LAT_P, CIF_A, CIF_6 };
const unsigned char errorrf[] = { CIF_E, LAT_r, LAT_r, LAT_L, LAT_n };
const unsigned char errorgr[] = { CIF_E, LAT_r, LAT_r, RUS_G, LAT_P };
const unsigned char errorfl[] = { CIF_E, LAT_r, LAT_r, CIF_F, LAT_L };
const unsigned char zaprety[] = { SPACE, CIF_3, CIF_A, RUS_P, LAT_P };
const unsigned char funkcja[] = { LAT_H, CIF_E, CIF_3, CIF_A, CIF_D };
const unsigned char Run[]     = { MINUS, LAT_r, LAT_u, LAT_n, MINUS };
const unsigned char Stop[]    = { CIF_5, LAT_t, CIF_0, LAT_P, SPACE };
//const unsigned char dbgtxt1[] = { LAT_H, HORTR, CIF_0, RUS_P, CIF_A };
//const unsigned char dbgtxt2[] = { HORTL, CIF_0, LAT_P, CIF_B, LAT_I };





#endif

