#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include "LCD.h"
#include "KEY.h"
#include "Beef.h"
#include "LCD.h"
#include "TempCode.h"

String DOOR_ID = "10000001"; //{"DOOR_ID":"10000001","NFC_ID":"12345678"}
#define pinDelay 4  // 开门继电器

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
String Old_uid = ""; //旧的uid，用于判断是否重复刷卡（在周期性任务中，实现每隔固定间隔清零，可以重复刷同一张卡）
unsigned long  Old_time = 0;

uint32_t setup_nfc(void);
String GetQrResult(String strQrCode);
void PeriodMession();
void OpenDoorViaDelay(void);

void setup(void) {
  Serial.begin(115200); //  For debug
  Serial1.begin(115200); //  For LCD(Txd) and QR scanner(Rxd)
  Serial.println("Hello!");

  pinMode(pinBeef, OUTPUT);//蜂鸣器
  digitalWrite(pinBeef, HIGH);
  pinMode(pinDelay, OUTPUT);//开门继电器
  digitalWrite(pinDelay, HIGH);

  ClearDisp();
    //静态固定显示
  //Display logo of the Company  "北京中坚居逸建筑科技",
  LcdDisplay(5,10, 32, hz[8], 4);
  delay(800);
  //Display comment for operation "房间号*密码，按确定键开门"
  LcdDisplay(5, 40, 32, hz[1], 4);

  KeyPinSetup();//初始化按键

  uint32_t nfc_version = setup_nfc();
  if (! nfc_version) {
    Serial.print("Didn't find PN53x board");
    //while (1); // halt
  }
  else
  {
    // Got ok data, print it out!
    Serial.print("Found chip PN5");
    Serial.println((nfc_version >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((nfc_version >> 16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((nfc_version >> 8) & 0xFF, DEC);
  }
  Serial.println("Waiting for an ISO14443A card");

}

String getUidCode(void);
String getUidCode(String);
void Di(void);
String GetQrCode();
String GetKeyInput(void);//返回按键的操作结果

void loop(void) {

  PeriodMession();
  String strUidCode = getUidCode();
  String strUidResult = getUidResult(strUidCode);//使用固定Json格式，格式化输出NFC的扫描结果
  String strQrCode = GetQrCode();
  String strQrResult = GetQrResult(strQrCode);  //使用固定Json格式，格式化输出QR的扫描结果
  String strCodeResults = strUidResult + strQrResult; //合并结果，用于和本地的密码对比后判断结果。

  if (!strCodeResults.equals(""))
  {
    Serial.println(strCodeResults);
    LegalAccessFromCode(strCodeResults);
  }

  String strKeyResults = GetKeyInput();// 返回按键的输入结果  和以 & 开头的特定的状态码
  if (!strKeyResults.equals(""))
  {
    if (strKeyResults.indexOf("&OK") != -1) //若返回得字符中包含OK按键，则进行密码判断
      LegalAccessFromCode(strKeyResults);
    else if(strKeyResults.indexOf("&ADMIN") != -1)//物业按键
    {
      RefreshLCD(STATUS_ADMIN);//   //呼叫物业状态
      Di3s();
    }
    else if(strKeyResults.indexOf("&CALL") != -1)//业主按键
    {
      RefreshLCD(STATUS_CALL);//   //呼叫业主状态
      Di3s();
    }
    else if(strKeyResults.indexOf("&CANCEL") != -1)//取消
    {
      RefreshLCD(STATUS_CANCEL);// 4 //取消状态，显示为空行
      DiDi();
    }
    else//在液晶上显示按键输入结果 
    {
      RefreshLCD(STATUS_KEY,strKeyResults);
    }
  }
}

/**
   通过输入的strCode字符串，查询本地密码表，判断书输入（包括QR和NFC）是否合法
   合法，则开门，并刷新LCD显示
*/
void LegalAccessFromCode(String strCode)
{
  if (true == LegaAccess(strCode))
  {
    RefreshLCD(STATUS_WELCOME);// 6//密码正确，欢迎光临
    OpenDoorViaDelay();//通过继电器开门，同时响铃
  }
  else
  {
    RefreshLCD(STATUS_ILLEGA);//  7//非法，禁止进入
    DiDiDi();//失败，被拒绝的音效
  }
}

uint32_t setup_nfc(void) {
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    return 0;
  }

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0x2);

  // configure board to read RFID tags
  nfc.SAMConfig();

  return versiondata;
}

String getUidCode(void) {
  String new_uid = "";
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success)
  {
    //    Serial.println("Found a card!");
    //    Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    //    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      //      Serial.print(" 0x"); Serial.print(uid[i], HEX);
      //      Serial.println("");
      if (uid[i] < 16) //首位补0
        new_uid += "0";
      new_uid += String(uid[i], HEX);
    }
    new_uid.toUpperCase();

    if (Old_uid.equals(new_uid))
      return "";
    else {
      Old_uid = new_uid;
      Old_time = millis(); //获取当前的系统运行时间长度
      return Old_uid;
    }
  }
  return "";

}
String getUidResult(String strUidCode) {
  //  {"DOOR_ID":"10000001","NFC_ID":"12345678"}
  String result = "{\"DOOR_ID\":\"" + DOOR_ID + "\",\"NFC_ID\":\"";
  if (strUidCode.equals(""))
  {
    return "";
  }
  return result + strUidCode + "\"}";

}
String GetQrCode(void)
{
  String strQrCode = "";
  int inByte;
  // read from port 1, send to port 0:
  while (Serial1.available()) {
    inByte = Serial1.read();
    strQrCode += (char)inByte;
  }
  return strQrCode;
}
String GetQrResult(String strQrCode)
{
  String result = "{\"DOOR_ID\":\"" + DOOR_ID + "\",\"QR_CODE\":\"";
  if (strQrCode.equals(""))
    return "";
  else {
    return result + strQrCode + "\"}";
  }
}


void PeriodMession()                        //中断处理函数，改变灯的状态
{
  if (millis() - Old_time > 3000)           //2000ms
  {
    //Di();
    Old_uid = ""; //在定时器中定时清零，实现每隔固定间隔，可以重复刷同一张卡
    Old_time = millis(); //获取当前的系统运行时间长度
  }
  //Serial.println(millis());
}

void OpenDoorViaDelay(void)
{
  digitalWrite(pinDelay, LOW);
  Diii();//开门提示音，同时继电器延时
  digitalWrite(pinDelay, HIGH);

}

void ClearThisLine();

/**
   iStatus表示提示信息的状态：
   STATUS_ADMIN  呼叫物业状态
   STATUS_CALL   呼叫业主状态
   STATUS_CANCEL 取消状态，显示为空行
   STATUS_KEY    按键输入状态，配合使用strKeyInput,用于显示键盘的输入状态
   STATUS_WELCOME 密码正确，欢迎光临
   STATUS_ILLEGA  非法，禁止进入
   STATUS_KEY     键盘输入
*/
void RefreshLCD(int iStatus, String strKeyInput)
{
  //静态固定显示
//  //Display logo of the Company  "北京中坚居逸建筑科技",
//  LcdDisplay(5, 0, 32, hz[0], 4);
//  //Display comment for operation "房间号*密码，按确定键开门"
//  LcdDisplay(5, 40, 32, hz[1], 4);

  //动态显示
  if (STATUS_ADMIN == iStatus) //"请稍等，正在呼叫物业……",//5
  {
    ClearThisLine();
    LcdDisplay(5, 80, 32, hz[5], 4);
  }
  else if (STATUS_CALL == iStatus) //"请稍等，正在呼叫业主……",//6
  {
    ClearThisLine();
    LcdDisplay(5, 80, 32, hz[6], 4);
  }
  else if (STATUS_CANCEL == iStatus) ////  7  15个空行
  {
    ClearThisLine();
  }
  else if (STATUS_WELCOME == iStatus) //"欢迎光临！",//4
  {
    ClearThisLine();
    LcdDisplay(5, 80, 32, hz[4], 4);
  }
  else if (STATUS_ILLEGA == iStatus) // "无权进入！",//3
  {
    ClearThisLine();
    LcdDisplay(5, 80, 32, hz[3], 4);
  }
  else if (STATUS_KEY == iStatus) // 键盘输入
  {
    ClearThisLine();
    String strInvisibleCode = strKeyInput;
    int index = strInvisibleCode.indexOf("*");
    int len = strInvisibleCode.length();
    for(int i=index+1; i<len;i++)
    {
      strInvisibleCode.setCharAt(i,"*");//将*之后得字符串替换成*，隐藏密码
    }
    //LcdDisplay(5, 80, 32, strInvisibleCode, 4);
    LcdDisplay(5, 80, 32, strKeyInput, 4);
  }
}

void RefreshLCD(int iStatus)
{
  String str = "";
  RefreshLCD(iStatus , str);
}

void ClearThisLine()//清除当前行显示
{
  char blank[] = "                    ";//20个空格，用于清除当前行
  LcdDisplay(5, 80, 32, blank, 4);
}

