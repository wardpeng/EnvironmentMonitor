#ifndef TEMPCODE_H
#define TEMPCODE_H
#include "Arduino.h"

const int PasswordNum = 13;  //临时定义，密码数量，用于测试使用

const String tempCode[]={
  "q1w2e3Q!W@E#",  //NFC or QR code
  "r4t5y6R$T%Y^",  //NFC or QR code
  "u7i8o9U&I*O(",  //NFC or QR code
  "50566EA4",  //NFC or QR code
  "101*101101",//房间号*密码组合
  "102*102102",//房间号*密码组合
  "103*103103",//房间号*密码组合
  "201*201201",//房间号*密码组合
  "202*202202",//房间号*密码组合
  "203*203203",//房间号*密码组合
  "301*301301",//房间号*密码组合
  "302*302302",//房间号*密码组合
  "303*303303",//房间号*密码组合
};

boolean LegaAccess(String strCode);

#endif
