#include "LCD.h"

#define LCD_SERIAL Serial1


//PIC(0,0,1);在(x,y)处画第n幅图

//DS12(x,y,'显示内容字符串',c);  在(x,y)处用颜色c显示一行12点阵字；
void LcdDisplay(int iPosX, int iPosY, int iSize,char *strToDis,int iColor )
{
  String strCmd = "DS" + String(iSize) + "(" + String(iPosX) + "," + String(iPosY) + ",'";
  LCD_SERIAL.print(strCmd);

  LCD_SERIAL.print(strToDis);

  strCmd = "'," + String(iColor) + ");";
  LCD_SERIAL.println(strCmd);
  LCD_SERIAL.flush();
}

//DS12(x,y,'显示内容字符串',c);  在(x,y)处用颜色c显示一行12点阵字；
void LcdDisplay(int iPosX, int iPosY, int iSize,String strDisp,int iColor )
{
  String strCmd = "DS" + String(iSize) + "(" + String(iPosX) + "," + String(iPosY) + ",'";
  LCD_SERIAL.print(strCmd);

  LCD_SERIAL.print(strDisp);

  strCmd = "'," + String(iColor) + ");";
  LCD_SERIAL.println(strCmd);
  LCD_SERIAL.flush();
}

void ClearDisp(void)
{
  LCD_SERIAL.println("CLS(0);");
  LCD_SERIAL.flush();
}




