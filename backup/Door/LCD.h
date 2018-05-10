#ifndef LCD_H
#define LCD_H

#include "Arduino.h"

const char hz[][64]={
  "环境监测DemoV1.0", //0
  "Room*Code,Press OK",  //1
  "智能门禁系统", //2
  "Sorry,Wrong password!",//3
  "Welcome!",//4
  "Calling the administrator......",//5
  "Calling the home owner......",//6
  "               ",//7  15个空行
 "Welcome home!", //0
};


#define STATUS_ADMIN 8  //呼叫物业状态
#define STATUS_CALL 3  //呼叫业主状态
#define STATUS_CANCEL 4 //取消状态，显示为空行
#define STATUS_KEY 5    // 按键输入状态，配合使用strKeyInput,用于显示键盘的输入状态
#define STATUS_WELCOME 6//密码正确，欢迎光临
#define STATUS_ILLEGA  7//非法，禁止进入


void ClearDisp(void);
void LcdDisplay(int posX, int posY, int size,char *d,int iColor );
void LcdDisplay(int iPosX, int iPosY, int iSize,String strDisp,int iColor );

#endif
