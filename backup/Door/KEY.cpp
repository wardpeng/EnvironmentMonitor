#include "KEY.h"
#include "Beef.h"

static unsigned char KeyScan(void)
{
  if (digitalRead(pinKey_1) == HIGH)return KEY_1 ;
  if (digitalRead(pinKey_2) == HIGH)return KEY_2 ;
  if (digitalRead(pinKey_3) == HIGH)return KEY_3 ;
  if (digitalRead(pinKey_4) == HIGH)return KEY_4 ;
  if (digitalRead(pinKey_5) == HIGH)return KEY_5 ;
  if (digitalRead(pinKey_6) == HIGH)return KEY_6 ;
  if (digitalRead(pinKey_7) == HIGH)return KEY_7;
  if (digitalRead(pinKey_8) == HIGH)return KEY_8 ;
  if (digitalRead(pinKey_9) == HIGH)return KEY_9 ;
  if (digitalRead(pinKey_0) == HIGH)return KEY_0 ;
  if (digitalRead(pinKey_Star) == HIGH)return KEY_STAR ;
  if (digitalRead(pinKey_Hex) == HIGH)return KEY_HEX ;
  if (digitalRead(pinKey_Admin) == HIGH)return KEY_ADMIN ;
  if (digitalRead(pinKey_Call) == HIGH)return KEY_CALL ;
  if (digitalRead(pinKey_Cancel) == HIGH)return KEY_CANCEL ;
  if (digitalRead(pinKey_OK) == HIGH)return KEY_OK ;

  return KEY_NULL ;
}

void KeyPinSetup(void)
{
  pinMode(pinKey_1, INPUT);
  pinMode(pinKey_2, INPUT);
  pinMode(pinKey_3, INPUT);
  pinMode(pinKey_4, INPUT);
  pinMode(pinKey_5, INPUT);
  pinMode(pinKey_6, INPUT);
  pinMode(pinKey_7, INPUT);
  pinMode(pinKey_8, INPUT);
  pinMode(pinKey_9, INPUT);
  pinMode(pinKey_0, INPUT);
  pinMode(pinKey_Star, INPUT);
  pinMode(pinKey_Hex, INPUT);
  pinMode(pinKey_Admin, INPUT);
  pinMode(pinKey_Call, INPUT);
  pinMode(pinKey_Cancel, INPUT);
  pinMode(pinKey_OK, INPUT);
}

void KeyMainLoop(unsigned char *pKeyValue)
{
  static unsigned char s_u8KeyState = KEY_STATE_INIT ;
  static unsigned char s_u8KeyTimeCount = 0 ;
  static unsigned char s_u8LastKey = KEY_NULL ;   //保存按键释放时候的键值
  unsigned char KeyTemp = KEY_NULL ;

  KeyTemp = KeyScan();         //获取键值
  switch (s_u8KeyState)
  {
    case KEY_STATE_INIT :
      {
        if (KEY_NULL != (KeyTemp))
        {
          s_u8KeyState = KEY_STATE_WOBBLE ;
        }
      }
      break ;
    case KEY_STATE_WOBBLE:       //消抖
      {
        s_u8KeyState = KEY_STATE_PRESS ;
      }
      break ;
    case KEY_STATE_PRESS :
      {
        if (KEY_NULL != (KeyTemp))
        {
          s_u8LastKey = KeyTemp ; //保存键值,以便在释放按键状态返回键值
          KeyTemp |= KEY_DOWN ;   //按键按下
          s_u8KeyState = KEY_STATE_LONG ;
        }
        else
        {
          s_u8KeyState = KEY_STATE_INIT ;
        }
      }
      break ;
    case KEY_STATE_LONG :
      {
        if (KEY_NULL != (KeyTemp))
        {
          if (++s_u8KeyTimeCount > KEY_LONG_PERIOD)
          {
            s_u8KeyTimeCount = 0 ;
            KeyTemp |= KEY_LONG ;   //长按键事件发生
            s_u8KeyState = KEY_STATE_CONTINUE ;
          }
        }
        else
        {
          s_u8KeyState = KEY_STATE_RELEASE ;
        }
      }
      break ;
    case KEY_STATE_CONTINUE :
      {
        if (KEY_NULL != (KeyTemp))
        {
          if (++s_u8KeyTimeCount > KEY_CONTINUE_PERIOD)
          {
            s_u8KeyTimeCount = 0 ;
            KeyTemp |= KEY_CONTINUE ;
          }
        }
        else
        {
          s_u8KeyState = KEY_STATE_RELEASE ;
        }
      }
      break ;
    case KEY_STATE_RELEASE :
      {
        s_u8LastKey |= KEY_UP ;
        KeyTemp = s_u8LastKey ;
        s_u8KeyState = KEY_STATE_INIT ;
      }
      break ;
    default : break ;
  }
  *pKeyValue = KeyTemp ; //返回键值
}

String GetKeyInput(void)//返回按键的操作结果
{
  static String KeyResults = ""; //Static变量，用于返回按键的累计效果

  unsigned char KeyValue = KEY_NULL;
  KeyMainLoop(&KeyValue) ;

  if (KeyValue == (KEY_1 | KEY_DOWN))
  {
    Di();
    KeyResults += "1";
  }
  if (KeyValue == (KEY_2 | KEY_DOWN))
  {
    Di();
    KeyResults += "2";
  }
  if (KeyValue == (KEY_3 | KEY_DOWN))
  {
    Di();
    KeyResults += "3";
  }
  if (KeyValue == (KEY_4 | KEY_DOWN))
  {
    Di();
    KeyResults += "4";
  }
  if (KeyValue == (KEY_5 | KEY_DOWN))
  {
    Di();
    KeyResults += "5";
  }
  if (KeyValue == (KEY_6 | KEY_DOWN))
  {
    Di();
    KeyResults += "6";
  }
  if (KeyValue == (KEY_7 | KEY_DOWN))
  {
    Di();
    KeyResults += "7";
  }
  if (KeyValue == (KEY_8 | KEY_DOWN))
  {
    Di();
    KeyResults += "8";
  }
  if (KeyValue == (KEY_9 | KEY_DOWN))
  {
    Di();
    KeyResults += "9";
  }
  if (KeyValue == (KEY_0 | KEY_DOWN))
  {
    Di();
    KeyResults += "0";
  }
  if (KeyValue == (KEY_STAR | KEY_DOWN))
  {
    Di();
    KeyResults += "*";
  }
  if (KeyValue == (KEY_HEX | KEY_DOWN))
  {
    Di();
    KeyResults += "#";
  }
  if (KeyValue == (KEY_ADMIN | KEY_DOWN))
  {
    //Di();
    KeyResults = "";//清空局部Static记录
    return "&ADMIN";
  }
  if (KeyValue == (KEY_CALL | KEY_DOWN))
  {
    //Di();
    String strResults = KeyResults + "&CALL";
    KeyResults = "";//清空局部Static记录
    return strResults;
  }
  if (KeyValue == (KEY_CANCEL | KEY_DOWN))
  {
    //DiDi();
    KeyResults = "";//清空局部Static记录
    return "&CANCEL";
  }
  if (KeyValue == (KEY_OK | KEY_DOWN))
  {
    //DiDiDi();
    KeyResults ;
    String strResults = KeyResults + "&OK";
    KeyResults = "";//清空局部Static记录
    return strResults;
  }
  return KeyResults;
}
