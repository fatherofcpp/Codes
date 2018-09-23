#ifndef COMMON_H
#define COMMON_H
#include "include_all_qt.h"

QString utf_2_gbk(const char* str);
const char* qstr_2_str(const QString& qstr);
QString str_2_qstr(const string &s);

#define KEYNUM (16 * 6 - 4)
const static QString gKeyboardVal[KEYNUM] =
{
    "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  ".",  ":",  "-",  "*",  "/",  "%",
    "A",  "B",  "C",  "D",  "E",  "F",  "G",  "H",  "I",  "J",  "K",  "L",  "M",  "N",  "O",  "P",
    "Q",  "R",  "S",  "T",  "U",  "V",  "W",  "X",  "Y",  "Z",  "a",  "b",  "c",  "d",  "e",  "f",
    "g",  "h",  "i",  "j",  "k",  "l",  "m",  "n",  "o",  "p",  "q",  "r",  "s",  "t",  "u",  "v",
    "w",  "x",  "y",  "z",  "!",  "@",  "#",  "$",  "&",  "(",  ")",  "{",  "}",  "[",  "]",  "\\",
    "|",  "+",  ";",  "\"", "'",  ",",  "=",  "<",  ">",  "?",  "_",  " "
};

QString pos_2_val(int pos);
int val_2_pos(QString val);

#endif // COMMON_H
