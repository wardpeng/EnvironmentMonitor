#ifndef KEY_H
#define KEY_H

#include "Arduino.h"

#define KEY_1 0x01
#define KEY_2 0x02
#define KEY_3 0x03
#define KEY_4 0x04
#define KEY_5 0x05
#define KEY_6 0x06
#define KEY_7 0x07
#define KEY_8 0x08
#define KEY_9 0x09
#define KEY_0 0x0a
#define KEY_STAR 0x0b
#define KEY_HEX 0x0c
#define KEY_ADMIN 0x0d
#define KEY_CALL 0x0e
#define KEY_CANCEL 0x10
#define KEY_OK 0x11
#define KEY_NULL 0x0f

//定义长按键的TICK数,以及连_发间隔的TICK数
#define KEY_LONG_PERIOD         100
#define KEY_CONTINUE_PERIOD     25
//定义按键返回值状态(按下,长按,连_发,释放)
#define KEY_DOWN                0x80
#define KEY_LONG                 0x40
#define KEY_CONTINUE            0x20
#define KEY_UP     0x10        //定义按键状态
#define KEY_STATE_INIT            0
#define KEY_STATE_WOBBLE            1
#define KEY_STATE_PRESS            2
#define KEY_STATE_LONG            3
#define KEY_STATE_CONTINUE       4
#define KEY_STATE_RELEASE         5

#define pinKey_1  22  //
#define pinKey_4  23  //
#define pinKey_7  24  //
#define pinKey_Star  25  //*

#define pinKey_2  26  //
#define pinKey_5  27  //
#define pinKey_8  28  //
#define pinKey_0  29  //

#define pinKey_3  30  //
#define pinKey_6  31  //
#define pinKey_9  32 //
#define pinKey_Hex  33  //#

#define pinKey_Admin  34//物业
#define pinKey_Call  35//呼叫
#define pinKey_Cancel  36//取消
#define pinKey_OK  37//确定

void KeyPinSetup(void);
String GetKeyInput(void);//返回按键的操作结果

#endif
